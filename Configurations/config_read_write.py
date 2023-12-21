#!/usr/bin/env python3
"""
Open modify and save a MARTe2 configuration file

In case MARTe2 python resources fail to install with pip, 
copy this directory, the files:
StandardParser.py
StandardPrinter.py
from:
MARTe2/Resources/Python/Source
"""
import sys
from StandardParser import StandardParser
from StandardPrinter import StandardPrinter

parser = StandardParser()
printer = StandardPrinter()

if len(sys.argv) > 1:
    filename = str(sys.argv[1])
else:
    filename = 'RTApp-Atca-mds.cfg'

if len(sys.argv) > 2:
    fileW = str(sys.argv[2])
else:
    fileW = filename[:-4] + '_new.cfg'

with open(filename) as f:
    cfg = parser.parse_file(f)
    cfg.keys()
    # Change here nodes:
    # eg. cfg['$IsttokApp']['+Data']['+AtcaIop']['BoardId'] = '2'
    with open(fileW, 'w') as fw:
        o_str = printer.print(cfg)
        fw.write(o_str)
        fw.close()
    f.close()

# vim: syntax=python ts=4 sw=4 sts=4 sr et
