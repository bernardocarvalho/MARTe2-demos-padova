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

if __name__ == '__main__':
    mdsNode = tree.getNode("ATCAIOP1.ADC0RAW")
    dataAdc = mdsNode.getData().data()
    timeData = mdsNode.getDimensionAt(0).data()
    plt.figure()
    plt.grid()
    plt.ylabel("ADC counts")
    plt.xlabel("Sample")
    plt.subplot(2, 1, 1)
    plt.plot(timeData, dataAdc, label=f"M{1} CHA")
    mdsNode = tree.getNode("ATCAIOP1.ADC1RAW")
    dataAdc = mdsNode.getData().data()
    plt.plot(timeData, dataAdc, label=f"M{1} CHB")
    plt.legend()
    # Bootom plot
    plt.subplot(2, 1, 2)
    mdsNode = tree.getNode("ATCAIOP1.ADC1INT")
    dataIntegAdc = mdsNode.getData().data()
    timeData = mdsNode.getDimensionAt(0).data()
    plt.plot(timeData, dataIntegAdc, label=f"M{1} CHB")
    plt.show()



