#!/usr/bin/env python3
"""
Extract data from MDSplus and plot
"""
import sys
import numpy as np
import matplotlib.pyplot as plt
from MDSplus import Tree

mdsTreeName = 'rtappisttok'

if len(sys.argv) > 1:
    mdsPulseNumber = int(sys.argv[1])
else:
    mdsPulseNumber = 1
    #treename = ''

try:
    tree = Tree(mdsTreeName, mdsPulseNumber)
except:
    print(f'Failed opening {mdsTreeName} for pulse number {mdsPulseNumber:d}')
    exit()
"""
Available Channels

ATCAIOP1.ADC0RAW
ATCAIOP1.ADC1RAW
ATCAIOP1.ADC2RAW
ATCAIOP1.ADC3RAW
ATCAIOP1.ADC4RAW
ATCAIOP1.ADC5RAW
ATCAIOP1.ADC6RAW
ATCAIOP1.ADC7RAW

ATCAIOP1.ADC0INT
ATCAIOP1.ADC1INT

"""

if __name__ == '__main__':
    mdsNode = tree.getNode("ATCAIOP1.ADC0RAW")
    mdsNode = tree.getNode("ATCAIOP1.ADC1INT")
    dataAdc = mdsNode.getData().data()
    timeData = mdsNode.getDimensionAt(0).data()



