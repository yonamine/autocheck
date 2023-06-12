#!/usr/bin/env python3

import sys
import os.path

this_dir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(os.path.dirname(this_dir), 'lib', 'autocheck', 'autocheck-genhtml'))

from genhtml import main

if __name__ == '__main__':
    main()
