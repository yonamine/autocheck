#!/usr/bin/env python3

import argparse
from enum import IntEnum, unique
import multiprocessing
import os
import platform
import re
import shlex
import signal
import subprocess
import sys
import time
# TODO: When we update to python3.9+ remove typing imports.
from typing import Optional, List, Iterable
from dataclasses import dataclass


def check_positive_int(arg: str):
    try:
        value = int(arg)
        if value <= 0:
            raise argparse.ArgumentTypeError(
                "required positive integer but found {}".format(arg))
    except ValueError:
        raise argparse.ArgumentTypeError(
            "required positive integer but found {}".format(arg))
    return value


def check_regex(arg: str):
    try:
        return re.compile(arg, re.IGNORECASE)
    except re.error as reason:
        raise argparse.ArgumentTypeError(
            "invalid regular expression: '{}', {}", arg, reason)


num_threads = os.cpu_count()


def parse_args():
    parser = argparse.ArgumentParser(
        prog='autocheck-dir',
        description='Run autocheck on all C++ files in a directory'
    )

    # General
    parser.add_argument(
        'scan_dir',
        help='Path to directory to recursively check'
    )
    parser.add_argument(
        'output_dir',
        help='Directory to write diagnostics data to'
    )

    parser.add_argument(
        '--version',
        action='version',
        version='%(prog)s 1.0'
    )
    parser.add_argument(
        '-p', '--compilation-database',
        dest='compilation_database',
        help='Path to compilation database'
    )
    parser.add_argument(
        '-j', '--threads', '--workers',
        dest='workers',
        type=check_positive_int,
        default=num_threads,
        help='Number of parallel workers. Uses all available logical CPUs by default'
    )
    parser.add_argument(
        '--output-type',
        dest='output_type',
        choices=['full', 'summary'],
        default='full',
        help='Should write all diagnostics or just a summary'
    )
    parser.add_argument(
        '--skip-existing',
        dest='skip_existing',
        action='store_true',
        help='Don\'t check files for which output already exists'
    )

    # Autocheck
    autocheck_group = parser.add_argument_group('Autocheck')
    autocheck_group.add_argument(
        '--autocheck-executable',
        dest='executable',
        help='Path to autocheck executable'
    )
    autocheck_group.add_argument(
        '--autocheck-flags',
        dest='flags',
        default='',
        help='Flags to pass to autocheck'
    )
    autocheck_group.add_argument(
        '--check-includes',
        dest='check_includes',
        action='store_true',
        help='Report warnings from included files'
    )

    # File selection
    selection_group = parser.add_argument_group('File Selection')
    selection_group.add_argument(
        '--filter',
        metavar='REGEX',
        type=check_regex,
        default='.*',
        help='Only run files with paths matching the given regular expression'
    )
    selection_group.add_argument(
        '--filter-out',
        metavar='REGEX',
        type=check_regex,
        default='^$',
        help='Skip files with paths matching the given regular expression'
    )
    selection_group.add_argument(
        '--extensions',
        default='cpp, h, hpp',
        help='Comma separated list of file extensions to be filtered. Default is \'cpp, h, hpp\''
    )

    # Output format
    output_format_group = parser.add_argument_group('Output Format')
    output_format_group.add_argument(
        '-v', '--verbose',
        dest='verbose',
        action='store_true',
        help='Show error output when compilation fails'
    )
    output_format_group.add_argument(
        '-vv', '--echo-all-commands',
        dest='echo',
        action='store_true',
        help='Echo all commands as they are executed to stdout'
    )
    output_format_group.add_argument(
        '-q', '--quiet',
        dest='quiet',
        action='store_true',
        help='Show message only when compilation fails'
    )

    opts = parser.parse_args()

    opts.flags = shlex.split(opts.flags)
    opts.allowed_extensions = ['.' + ext.strip()
                               for ext in opts.extensions.split(',')]

    return opts


def find_autocheck(executable: Optional[str] = None):
    if executable:
        if os.path.exists(executable):
            return executable
        else:
            print(f'Specified autocheck executable not found in: \'{executable}\'. Aborting')
            sys.exit(1)
    else:
        search_locations = []
        if platform.system() == 'Linux' or platform.system() == 'Darwin':
            search_locations = [
                './autocheck'
                '/usr/bin/autocheck'
            ]
        elif platform.system() == 'Windows':
            search_locations = [
                '.\\autocheck.exe',
                'C:\\Program Files\\autocheck\\bin\\autocheck.exe'
            ]

        autocheck_found = False
        for location in search_locations:
            if os.path.exists(location) and os.path.isfile(location):
                autocheck_executable = location
                autocheck_found = True
                break
        if autocheck_found:
            print(f'Using autocheck in \'{autocheck_executable}\'')
        else:
            print('Unable to find autocheck executable. Please specify the path using \'--autocheck-executable\' flag')
            sys.exit(1)
        return autocheck_executable


def discover_files(path: str, filter: re.Pattern, filter_out: re.Pattern,
                   allowed_extensions: List[str]) -> Iterable[str]:
    yield from _discover_files(path, '', filter, filter_out, allowed_extensions)


def _discover_files(root, current_path, filter, filter_out, allowed_extensions) -> Iterable[str]:
    path = os.path.join(root, current_path)
    for filename in os.listdir(path):
        file_path = os.path.join(path, filename)

        if os.path.isdir(file_path):
            if filename.startswith('.'):
                continue
            yield from _discover_files(root, os.path.join(current_path, filename),
                                       filter, filter_out, allowed_extensions)
        if os.path.isfile(file_path):
            file_extension = os.path.splitext(file_path)[1]
            if file_extension not in allowed_extensions:
                continue

            if filter.search(file_path) and not filter_out.search(file_path):
                yield os.path.join(current_path, filename)


def bytes_to_str(data: bytes):
    try:
        return data.decode('utf-8')
    except UnicodeDecodeError:
        # If the value is not valid Unicode, return the default
        # repr-line encoding.
        return str(data)


@unique
class Status(IntEnum):
    SUCCESS = 0
    FAILED = 1
    SKIPPED = 2


class Display:
    def __init__(self, verbose: bool = False, echo_commands: bool = False, quiet: bool = False):
        self.verbose = verbose
        self.echo_commands = echo_commands
        self.quiet = quiet

    def on_command(self, command: str):
        if self.echo_commands:
            print(f'Running {command}')

    def print_result(self, test: str, status: Status, stderr: bytes):
        if status is Status.SUCCESS:
            if not self.quiet:
                print(f'Succeeded: {test}')
        elif status is Status.FAILED:
            print(f'Failed: {test}')
            if self.verbose:
                print(bytes_to_str(stderr))
        elif status is Status.SKIPPED:
            if not self.quiet:
                print(f'Skipped: {test}')


@dataclass
class Result:
    test: str
    status: Status
    stderr: bytes


def get_output_path_for_file(output_dir: str, file: str):
    output_rel_path = file + '.json'
    output_abs_path = os.path.join(os.path.abspath(output_dir), output_rel_path)
    return output_abs_path


class Run:
    def __init__(self, executable: str, scan_dir: str, output_dir: str,
                 compilation_database: Optional[str], output_type: str,
                 flags: List[str], skip_existing: bool, check_headers: bool,
                 display: Display):
        self.executable = executable
        self.scan_dir = scan_dir
        self.output_dir = output_dir
        self.flags = []
        if compilation_database:
            self.flags.extend(['-p', compilation_database])
        if not check_headers:
            self.flags.append('--dont-check-headers')
        self.flags.extend(['--output-type', output_type])
        self.flags.extend(flags)
        self.skip_existing = skip_existing
        self.display = display

    def init_worker(self):
        # Gracefully handle SIGINT / Keyboardinterrupt. Explained in:
        # https://noswap.com/blog/python-multiprocessing-keyboardinterrupt
        signal.signal(signal.SIGINT, signal.SIG_IGN)

    def run_worker(self, test: str):
        command_args: list[str] = []
        command_args.extend(self.flags)
        if self.output_dir:
            output_abs_path = get_output_path_for_file(self.output_dir, test)
            if self.skip_existing and os.path.exists(output_abs_path):
                return Result(test, Status.SKIPPED, None)
            output_abs_dir = os.path.dirname(output_abs_path)
            if not os.path.exists(output_abs_dir):
                os.makedirs(output_abs_dir)
            command_args.extend(['-o', output_abs_path])

        cmd = [self.executable] + command_args + \
            [os.path.normpath(os.path.join(self.scan_dir, test))]
        self.display.on_command(shlex.join(cmd))
        p = subprocess.Popen(
            cmd,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE
        )

        _, err = p.communicate()
        exit_code = p.wait()

        if exit_code == 0:
            return Result(test, Status.SUCCESS, None)
        else:
            return Result(test, Status.FAILED, err)

    def on_file_done(self, result: Result):
        self.display.print_result(result.test, result.status, result.stderr)
        # Empty stderr to save memory.
        result.stderr = None

    def execute(self, n_workers: int, files: Iterable[str]):
        try:
            pool = multiprocessing.Pool(
                n_workers, initializer=self.init_worker)
            results = [
                pool.apply_async(
                    self.run_worker, args=[file], callback=self.on_file_done
                )
                for file in files
            ]
            for result in results:
                result.wait()
        except KeyboardInterrupt:
            pool.terminate()
            pool.join()
            raise KeyboardInterrupt
        else:
            pool.close()
            pool.join()


def main():
    args = parse_args()
    autocheck_executable = find_autocheck(args.executable)

    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    display = Display(args.verbose, args.echo, args.quiet)
    files = list(discover_files(args.scan_dir, args.filter,
                 args.filter_out, args.allowed_extensions))
    print(f'Checking {len(files)} files')

    if len(files) == 0:
        print('No files found')
        sys.exit(1)

    n_workers = min(args.workers, len(files))
    print(f'Workers: {n_workers}')

    try:
        start = time.time()

        run = Run(autocheck_executable, args.scan_dir, args.output_dir,
                  args.compilation_database, args.output_type, args.flags,
                  args.skip_existing, args.check_includes, display)
        run.execute(n_workers, files)
    except KeyboardInterrupt:
        print('Interrupted by user')
    else:
        end = time.time()
        elapsed = end - start
        print(f'Finished in {int(elapsed // 60)}m {int(elapsed % 60)}s')


if __name__ == '__main__':
    main()
