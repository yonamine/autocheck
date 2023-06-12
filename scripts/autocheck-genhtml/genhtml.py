#!/usr/bin/env python3

import argparse
from autosar_rules import autosar_rule_info, Obligation, Enforcement, RuleInfo
from dataclasses import dataclass
from datetime import datetime
import html
import json
import os
import sys
from pathlib import Path
import shutil
# TODO: Remove List, Dict when updating to python 3.9+
# TODO: Use explicit TypeAlias when updating to python 3.10+
from typing import NamedTuple, TypedDict, Optional, List, Dict


def parse_args():
    parser = argparse.ArgumentParser(
        prog='autocheck-genhtml',
        description='Generate autocheck report in HTML format'
    )

    # General
    parser.add_argument(
        'input',
        help='Path to autocheck output file or directory with autocheck output files'
    )
    parser.add_argument(
        'output',
        help='File or directory (based on input) in which to save the report'
    )
    parser.add_argument(
        '--version',
        action='version',
        version='%(prog)s 1.0'
    )
    parser.add_argument(
        '-v', '--verbose',
        dest='verbose',
        action='store_true',
        help='Show warning messages'
    )
    parser.add_argument(
        '--clean-output-dir',
        dest='clean_output',
        action='store_true',
        help='Empty the output directory before writing reports. Only works if outputting to a directory'
    )
    parser.add_argument(
        '--open',
        dest='open',
        action='store_true',
        help='Open report in a web browser'
    )

    return parser.parse_args()


def to_percentages(votes: List[int], decimals: int = 1) -> List[float]:
    """
    Implements the Hamilton method for apportionment.

    This ensures the sum of percentages is equal to 100.
    """

    # Total number of votes for all candidates.
    total_votes = sum(votes)

    if total_votes == 0:
        return [0 for _ in range(len(votes))]

    # Number of total representatives is determined by the result precision we want.
    total_representatives = 100 * (10**decimals)

    # Determine the lower quotas for representatives.
    quotas = [vote * total_representatives / total_votes for vote in votes]
    representatives = [int(quota) for quota in quotas]

    # Give remaining representatives to candidates with the largest quota remainders.
    # Ties are resolved in favor of a random candidate.
    remainders = [votes[i] - quotas[i] for i in range(len(votes))]
    remaining_representatives = total_representatives - sum(representatives)
    largest_remainders = sorted(enumerate(remainders), key=lambda x: x[1], reverse=True)
    for vote, _ in largest_remainders[:remaining_representatives]:
        representatives[vote] += 1

    # Convert representative counts to percentages.
    return [x/(10**decimals) for x in representatives]


class TemplateManager:
    def __init__(self) -> None:
        self.templates: Dict[str, str] = {}
        self.this_dir = os.path.dirname(os.path.realpath(__file__))

    def get(self, template_name: str) -> Optional[str]:
        template = self.templates.get(template_name)
        if template is not None:
            return template
        template_path = os.path.join(self.this_dir, "templates", f"{template_name}.html.template")
        if not os.path.isfile(template_path):
            return None
        with open(template_path, 'r') as f:
            template_contents = f.read()[:-1]
        self.templates[template_name] = template_contents
        return template_contents

    def __getitem__(self, key: str):
        return self.get(key)


class Display:
    def __init__(self, verbose: bool) -> None:
        self.verbose = verbose

    def log(self, message: str):
        print(message)

    def warn(self, message: str):
        if self.verbose:
            print(f'Warning: {message}')

    def err(self, message: str):
        print(f'Error: {message}')


class ChartValue(NamedTuple):
    name: str
    description: str
    percentage: float


ChartData = List[ChartValue]


class RuleViolationTableValue(NamedTuple):
    rule_id: str
    rule_info: RuleInfo
    n_violations: int
    p_violations: float


RuleViolationTableData = List[RuleViolationTableValue]


@dataclass
class FileTableValue():
    filename: str
    url: Optional[str]
    n_violations: int
    p_violations: float


FileTableData = List[FileTableValue]


@dataclass
class ReportData():
    rule_violations_total: int
    rule_violations_required: int
    rule_violations_advisory: int
    rules_violated_total: int
    rules_violated_required: int
    rules_violated_advisory: int
    rule_compliance_percent: int
    chart_data_by_rule: ChartData
    chart_data_by_obligation: ChartData
    chart_data_by_enforcement: ChartData
    table_data_rule_violations: RuleViolationTableData


class HtmlGenerator:
    def __init__(self) -> None:
        self.templates = TemplateManager()
        self.palette_chart_by_rule = ["teal", "goldenrod",
                                      "indigo", "lightgreen", "indianred", "orange"]
        self.palette_chart_by_obligation = ["cornflowerblue", "indianred"]
        self.palette_chart_by_enforcement = ["cornflowerblue", "yellow", "indianred"]
        style_path = os.path.join(
            os.path.dirname(os.path.realpath(__file__)),
            'templates', 'style.css')
        with open(style_path) as f:
            self.style = f.read()

    @staticmethod
    def _get_ancestor_path(backtrack: int, filename: str = 'index.html'):
        return ''.join(['../' for i in range(backtrack)]) + filename

    def create_header_path(self, path: str, input_dir: str):
        path_obj = Path(path)
        segments = path_obj.parts
        backtrack = len(segments)-1
        if os.path.isdir(os.path.join(input_dir, path)):
            backtrack += 1

        res = [self.templates['path_link_segment'].format(
            segment="root",
            url=self._get_ancestor_path(backtrack)
        )]
        backtrack -= 1
        if len(segments) > 0:
            for segment in segments[:-1]:
                res.extend([
                    self.templates['path_segment'].format(segment='>'),
                    self.templates['path_link_segment'].format(
                        segment=segment,
                        url=self._get_ancestor_path(backtrack)
                    )
                ])
                backtrack -= 1
            res.extend([
                self.templates['path_segment'].format(segment='>'),
                self.templates['path_segment'].format(segment=segments[-1])
            ])
        return '\n'.join(res)

    def create_chart(self, title: str, data: ChartData, palette: List[str]):
        chart_template = self.templates['pie_chart']
        if len(data) == 0:
            return chart_template.format(
                title=title,
                background="grey 0% 100%",
                legend=self.templates['pie_chart_empty']
            )

        percentages = [0]
        for item in data:
            percentages.append(round(percentages[-1] + item.percentage, 2))
        background = ", ".join([
            f"{palette[i-1]} {percentages[i-1]}% {percentages[i]}%"
            for i in range(1, len(percentages))
        ])

        legend_row_template = self.templates['legend_row']
        legend = "".join([
            legend_row_template.format(
                color=palette[i],
                title=html.escape(item.description),
                id=item.name,
                percentage=item.percentage
            )
            for i, item in enumerate(data)
        ])

        return chart_template.format(
            title=title,
            background=background,
            legend=legend
        )

    def create_rule_violations_rows(self, data: RuleViolationTableData):
        if len(data) == 0:
            return self.templates['rule_table_empty']
        template = self.templates['rule_table_row']
        return "".join([
            template.format(
                id=item.rule_id,
                obligation=item.rule_info.obligation.value,
                message=html.escape(item.rule_info.message),
                n_violations=item.n_violations,
                p_violations=item.p_violations
            )
            for item in data
        ])

    @staticmethod
    def _wrap_name_as_link(filename: str, url: Optional[str]):
        if url is None:
            return filename
        return f"<a href={url}>{filename}</a>"

    def create_file_table_rows(self, data: FileTableData):
        row_template = self.templates['file_table_row']
        return "\n".join([
            row_template.format(
                file=self._wrap_name_as_link(file.filename, file.url),
                violations=file.n_violations,
                p_violations=file.p_violations
            )
            for file in data
        ])

    def create_style_tag(self, root: str, path: str, embed_css: bool = False):
        if embed_css:
            return f"<style>\n{self.style}</style>"
        else:
            path_obj = Path(path)
            segments = path_obj.parts
            backtrack = len(segments)-1
            if os.path.isdir(os.path.join(root, path)):
                backtrack += 1
            return f"<link rel=\"stylesheet\" type=\"text/css\" href=\"{self._get_ancestor_path(backtrack, 'style.css')}\">"

    def create_file_report(
        self, filename: str, root: str, path: str, generated_on: str,
        report_data: ReportData, embed_css: bool
    ):
        return self.templates['file_report'].format(
            style=self.create_style_tag(root, path, embed_css),
            filename=filename,
            path=self.create_header_path(path, root),
            generated_on=generated_on,
            rule_violations_total=report_data.rule_violations_total,
            rule_violations_required=report_data.rule_violations_required,
            rule_violations_advisory=report_data.rule_violations_advisory,
            rules_violated_total=report_data.rules_violated_total,
            rules_violated_required=report_data.rules_violated_required,
            rules_violated_advisory=report_data.rules_violated_advisory,
            autosar_rules_compliant_percent=report_data.rule_compliance_percent,
            autosar_rules_compliant=len(autosar_rule_info)-report_data.rules_violated_total,
            autosar_rules_supported=len(autosar_rule_info),
            chart_by_rule=self.create_chart(
                'Most violated AUTOSAR rules',
                report_data.chart_data_by_rule,
                self.palette_chart_by_rule
            ),
            chart_by_obligation=self.create_chart(
                'Violated AUTOSAR rules by obligation',
                report_data.chart_data_by_obligation,
                self.palette_chart_by_obligation
            ),
            chart_by_enforcement=self.create_chart(
                'Violated AUTOSAR rules by static analysis enforcement',
                report_data.chart_data_by_enforcement,
                self.palette_chart_by_enforcement
            ),
            rules_table_rows=self.create_rule_violations_rows(
                report_data.table_data_rule_violations
            )
        )

    def create_directory_report(
        self, dirname: str, root: str, path: str, report_data: ReportData,
        file_table_data: FileTableData, embed_css: bool
    ):
        return self.templates['directory_report'].format(
            style=self.create_style_tag(root, path, embed_css),
            dirname=dirname,
            path=self.create_header_path(path, root),
            rule_violations_total=report_data.rule_violations_total,
            rule_violations_required=report_data.rule_violations_required,
            rule_violations_advisory=report_data.rule_violations_advisory,
            rules_violated_total=report_data.rules_violated_total,
            rules_violated_required=report_data.rules_violated_required,
            rules_violated_advisory=report_data.rules_violated_advisory,
            autosar_rules_compliant_percent=report_data.rule_compliance_percent,
            autosar_rules_compliant=len(autosar_rule_info)-report_data.rules_violated_total,
            autosar_rules_supported=len(autosar_rule_info),
            chart_by_rule=self.create_chart(
                'Most violated AUTOSAR rules',
                report_data.chart_data_by_rule,
                self.palette_chart_by_rule
            ),
            chart_by_obligation=self.create_chart(
                'Violated AUTOSAR rules by obligation',
                report_data.chart_data_by_obligation,
                self.palette_chart_by_obligation
            ),
            chart_by_enforcement=self.create_chart(
                'Violated AUTOSAR rules by static analysis enforcement',
                report_data.chart_data_by_enforcement,
                self.palette_chart_by_enforcement
            ),
            files_table_rows=self.create_file_table_rows(file_table_data),
            rules_table_rows=self.create_rule_violations_rows(
                report_data.table_data_rule_violations
            )
        )


Summary = Dict[str, int]


class AutocheckExport(TypedDict, total=False):
    filename: str
    generated_on: int
    file_summaries: Dict[str, Summary]


class WriteStatus(NamedTuple):
    saved_html: bool
    summary: Optional[Summary]
    total_violations: int


class Reporter:
    def __init__(self, verbose: bool) -> None:
        self.html_generator = HtmlGenerator()
        self.display = Display(verbose)

    @staticmethod
    def merge_summaries(acc: Dict[str, int], val: Dict[str, int]):
        for rule, violations in val.items():
            if acc.get(rule) is not None:
                acc[rule] += violations
            else:
                acc[rule] = violations

    def calculate_report_data(self, summary: Summary):
        if len(summary) == 0:
            return ReportData(0, 0, 0, 0, 0, 0, 100, [], [], [], [])

        rule_violations_total = 0
        rule_violations_required = 0
        rule_violations_advisory = 0
        rules_violated_total = 0
        rules_violated_required = 0
        rules_violated_advisory = 0
        rules_violated = set()
        for rule, violations in summary.items():
            rule_info = autosar_rule_info.get(rule)
            if rule_info.obligation == Obligation.REQUIRED:
                rule_violations_required += violations
            elif rule_info.obligation == Obligation.ADVISORY:
                rule_violations_advisory += violations
            rule_violations_total += violations

            if rule not in rules_violated:
                if rule_info.obligation == Obligation.REQUIRED:
                    rules_violated_required += 1
                elif rule_info.obligation == Obligation.ADVISORY:
                    rules_violated_advisory += 1
                rules_violated_total += 1

        rules_violated_percent = round(100 - (rules_violated_total*100/len(autosar_rule_info)))

        # Violations per rule.
        violations_per_rule = sorted(summary.items(), key=lambda x: x[1], reverse=True)
        violation_percentages = to_percentages([x[1] for x in violations_per_rule])
        violation_percentages_per_rule = [
            (rule, p_violations)
            for (rule, _), p_violations in zip(violations_per_rule, violation_percentages)
        ]

        max_entries = 5
        percent_threshold = 5.0

        chart_data_by_rule: ChartData = [
            ChartValue(rule, autosar_rule_info[rule].message, violations)
            for rule, violations in violation_percentages_per_rule[:max_entries]
        ]

        # Tidy up the chart.
        # Group rules whose percentage is less than threshold into 'Other rules'.
        if len(violation_percentages_per_rule) > 2:
            if chart_data_by_rule[0].percentage >= percent_threshold:
                while chart_data_by_rule[-1].percentage < percent_threshold:
                    chart_data_by_rule.pop()

            if len(chart_data_by_rule) < len(violations_per_rule):
                if len(chart_data_by_rule) == max_entries:
                    chart_data_by_rule.pop()
                chart_data_by_rule.append(ChartValue("Other", "Other rules", round(
                    sum(violation_percentages[len(chart_data_by_rule):]), 1)))

        # Rule violations per obligation and enforcement.
        rules_violated_per_obligation = {
            Obligation.REQUIRED: 0,
            Obligation.ADVISORY: 0
        }
        rules_violated_per_enforcement = {
            Enforcement.AUTOMATED: 0,
            Enforcement.PARTIALLY_AUTOMATED: 0,
            Enforcement.NON_AUTOMATED: 0
        }
        for rule, violations in summary.items():
            rule_info = autosar_rule_info.get(rule)
            if rule_info is not None:
                rules_violated_per_obligation[rule_info.obligation] += 1
                rules_violated_per_enforcement[rule_info.enforcement] += 1

        rules_violated_per_obligation = list(rules_violated_per_obligation.items())
        violation_percentages_by_obligation = zip(
            [obligation for obligation, _ in rules_violated_per_obligation],
            to_percentages([n_rules for _, n_rules in rules_violated_per_obligation])
        )
        chart_data_by_obligation: ChartData = list(filter(lambda x: x.percentage != 0, [
            ChartValue(obligation.value, obligation.value, p_rules)
            for obligation, p_rules in violation_percentages_by_obligation
        ]))

        rules_violated_per_enforcement = list(rules_violated_per_enforcement.items())
        violation_percentages_by_enforcement = zip(
            [enforcement for enforcement, _ in rules_violated_per_enforcement],
            to_percentages([n_rules for _, n_rules in rules_violated_per_enforcement])
        )
        chart_data_by_enforcement: ChartData = list(filter(lambda x: x.percentage != 0, [
            ChartValue(enforcement.value, enforcement.value, p_rules)
            for enforcement, p_rules in violation_percentages_by_enforcement
        ]))

        # Rule table.
        sorted_rule_violations = sorted(summary.items(), key=lambda x: x[1], reverse=True)
        rule_violation_percentages = to_percentages([x[1] for x in sorted_rule_violations])
        table_data_rule_violations: RuleViolationTableData = []
        for (rule, n_violations), p_violations in zip(sorted_rule_violations, rule_violation_percentages):
            rule_info = autosar_rule_info.get(rule)
            if rule_info is not None:
                table_data_rule_violations.append(RuleViolationTableValue(
                    rule, rule_info, n_violations, p_violations
                ))

        return ReportData(
            rule_violations_total, rule_violations_required, rule_violations_advisory,
            rules_violated_total, rules_violated_required, rules_violated_advisory,
            rules_violated_percent,
            chart_data_by_rule, chart_data_by_obligation, chart_data_by_enforcement,
            table_data_rule_violations
        )

    def read_input_file(self, input_file: str):
        if not os.path.isfile(input_file):
            return None

        with open(input_file, 'r') as f:
            json_report: AutocheckExport = json.load(f)
            filename = json_report.get('filename')
            generated_on = json_report.get('generated_on')
            if generated_on is not None:
                generated_on = datetime.fromtimestamp(generated_on).strftime('%Y-%m-%d %H:%M:%S')

            file_summaries = json_report.get('file_summaries')
            if file_summaries is None:
                self.display.warn(f"Invalid summaries field for file '{input_file}'. Skipping")
                return None
            file_summary = file_summaries.get(filename)
            if file_summary is None:
                file_summary: Summary = {}
            json_report = None

        # Clean up summary object by removing unrecognized rules.
        for rule in file_summary.keys():
            if autosar_rule_info[rule] is None:
                self.display.warn(f"Unknown rule '{rule}'. Ignoring")
                file_summary.pop(rule)

        return filename, generated_on, file_summary

    def write_report_for_file(
        self, root: str, path: str, output_file: str, embed_css: bool
    ) -> WriteStatus:
        # Read input file.
        full_path = os.path.join(root, path)

        parsed_input = self.read_input_file(full_path)
        if parsed_input is None:
            self.display.warn(f"Could not parse '{full_path}'. Skipping")
            return False, None, 0

        file_abspath, generated_on, file_summary = parsed_input

        report_data = self.calculate_report_data(file_summary)

        # Skip writing html if the ouput dir wasn't created.
        # The violations will still be counted in the parent report.
        if not os.path.isdir(os.path.dirname(output_file)):
            return WriteStatus(False, file_summary, report_data.rule_violations_total)

        # Check if the output path exists but it's not a file. Don't overwrite user files.
        if os.path.exists(output_file) and not os.path.isfile(output_file):
            if os.path.isdir(output_file) and len(os.listdir(output_file)) == 0:
                try:
                    os.rmdir(output_file)
                except:
                    self.display.err(
                        f"Could not write report '{output_file}'. A directory with that name already exists")
                    return WriteStatus(False, file_summary, report_data.rule_violations_total)
            else:
                return WriteStatus(False, file_summary, report_data.rule_violations_total)

        # Write file report.
        self.display.log(f"{os.path.join(os.path.dirname(path), os.path.basename(output_file))}")
        try:
            with open(output_file, 'w') as f:
                f.write(self.html_generator.create_file_report(
                    file_abspath, root, os.path.splitext(path)[0], generated_on,
                    report_data, embed_css
                ))
        except:
            self.display.err(
                f"Error occurred while writing report '{output_file}'. File will be deleted."
            )
            if os.path.exists(output_file):
                os.remove(output_file)
            return WriteStatus(False, file_summary, report_data.rule_violations_total)

        return WriteStatus(True, file_summary, report_data.rule_violations_total)

    def write_report_for_directory(
        self, root: str, dir: str, output_dir: str, embed_css: bool
    ) -> Optional[WriteStatus]:
        if not os.path.exists(output_dir):
            os.mkdir(output_dir)

        # Collect summaries from subdirectories and files.
        summary: Summary = {}
        file_table_data: FileTableData = []
        for filename in os.listdir(os.path.join(root, dir)):
            full_name = os.path.join(dir, filename)

            # Skip hidden files/folders.
            if filename.startswith('.'):
                continue

            if os.path.isfile(os.path.join(root, full_name)):
                if os.path.splitext(filename)[1] != '.json':
                    continue
                saved_html, file_summary, file_total = self.write_report_for_file(
                    root, full_name,
                    os.path.join(output_dir, os.path.splitext(os.path.basename(filename))[0] + ".html"),
                    embed_css
                )
                self.merge_summaries(summary, file_summary)
                file_table_data.append(FileTableValue(
                    os.path.splitext(filename)[0],
                    os.path.splitext(filename)[0] + '.html' if saved_html else None,
                    file_total, 0
                ))
            elif os.path.isdir(os.path.join(root, full_name)):
                dir_result = self.write_report_for_directory(
                    root, full_name, os.path.join(output_dir, filename), embed_css
                )
                if dir_result is not None:
                    saved_html, dir_summary, dir_total = dir_result
                    self.merge_summaries(summary, dir_summary)
                    file_table_data.append(FileTableValue(
                        filename, filename + '/index.html' if saved_html else None, dir_total, 0
                    ))

        # If a directory is empty don't create a report.
        if len(file_table_data) == 0:
            return None

        file_table_data.sort(key=lambda item: item.filename)

        file_table_percantages = to_percentages([file.n_violations for file in file_table_data])
        for value, percentage in zip(file_table_data, file_table_percantages):
            value.p_violations = percentage

        report_data = self.calculate_report_data(summary)

        # Write report for directory.
        if not os.path.isdir(output_dir):
            self.display.err(f"Path '{output_dir}' is not a directory")
            return WriteStatus(False, summary, report_data.rule_violations_total)

        output_file = os.path.join(output_dir, 'index.html')
        self.display.log(os.path.join(dir, "index.html"))
        try:
            with open(output_file, 'w') as f:
                f.write(self.html_generator.create_directory_report(
                    os.path.join(root, dir), root, dir, report_data,
                    file_table_data, embed_css
                ))
        except:
            self.display.err(
                f"Error occurred while writing report '{output_file}'. File will be deleted.")
            if os.path.exists(output_file):
                os.remove(output_file)
            return WriteStatus(False, summary, report_data.rule_violations_total)

        return WriteStatus(True, summary, report_data.rule_violations_total)

    def write_css(self, output_dir: str):
        css_file = os.path.join(output_dir, "style.css")
        with open(css_file, 'w') as f:
            f.write(self.html_generator.style)

    def write_report(self, input: str, output: str):
        if os.path.isdir(input):
            # Write global css file.
            self.write_css(output)

            self.write_report_for_directory(input, "", output, False)
        else:
            self.write_report_for_file(
                os.path.dirname(input),
                os.path.basename(input),
                output, True)


def main():
    args = parse_args()

    # Check and prepare input and output.
    if not os.path.exists(args.input):
        print("Input not found")
        sys.exit(1)
    elif os.path.isdir(args.input):
        input_is_file = False
        if not os.path.exists(args.output):
            os.makedirs(args.output)
        elif not os.path.isdir(args.output):
            print("Output path is not a directory")
            sys.exit(1)
        elif args.clean_output:
            try:
                for file in os.listdir(args.output):
                    file_path = os.path.join(args.output, file)
                    if os.path.isdir(file_path):
                        shutil.rmtree(file_path)
                    else:
                        os.remove(file_path)
            except:
                print("Error clearing output directory. Make sure you have access privileges and any file in the directory is not in use.")
                sys.exit(1)
    elif os.path.isfile(args.input):
        input_is_file = True
        if os.path.exists(args.input) and not os.path.isfile(args.input):
            print("Input path is not a file")
            sys.exit(1)
        if not os.path.exists(os.path.dirname(args.output)):
            os.makedirs(os.path.dirname(args.output))
    else:
        print("Input must be file or directory")
        sys.exit(1)

    try:
        reporter = Reporter(args.verbose)
        reporter.write_report(args.input, args.output)
    except KeyboardInterrupt:
        print("\rInterrupted by user")
        sys.exit(1)

    # Open the report in browser if the flag is present.
    if input_is_file:
        output_index = args.output
    else:
        output_index = f"{args.output}/index.html"
    if args.open and os.path.isfile(output_index):
        import webbrowser
        webbrowser.open(f"file://{output_index}")


if __name__ == '__main__':
    main()
