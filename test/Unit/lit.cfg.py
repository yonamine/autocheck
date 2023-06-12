import os

import lit.formats

# name: The name of this test suite.
config.name = 'Autocheck-Unit'

# test_format: The test format to use to interpret tests.
config.test_format = lit.formats.GoogleTest('.', 'Tests')

# suffixes: A list of file extensions to treat as test files.
config.suffixes = []

# test_source_root: The root path where tests are located.
config.test_source_root = os.path.join(config.autocheck_build_dir, 'unittests')

# test_exec_root: The root path where tests should be run.
config.test_exec_root = config.test_source_root

# Propagate the temp directory. Windows requires this because it uses \Windows\
# if none of these are present.
if 'TMP' in os.environ:
  config.environment['TMP'] = os.environ['TMP']
if 'TEMP' in os.environ:
  config.environment['TEMP'] = os.environ['TEMP']

# Propagate HOME as it can be used to override incorrect homedir in passwd
# that causes the tests to fail.
if 'HOME' in os.environ:
  config.environment['HOME'] = os.environ['HOME']
