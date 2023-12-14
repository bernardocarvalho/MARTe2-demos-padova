"""
This example demonstrates many of the 2D plotting capabilities
in pyqtgraph. All of the plots may be panned/scaled by dragging with 
the left/right mouse buttons. Right click on any plot to show a context menu.
"""

import sys
import numpy as np

import pyqtgraph as pg
from pyqtgraph.Qt import QtCore
from MDSplus import Tree

app = pg.mkQApp("Plotting MARTe2 Data")
#mw = QtWidgets.QMainWindow()
#mw.resize(800,800)

MAX_SAMPLES = 50000
ADC_CHANNELS = 4
DECIM_RATE = 200

if len(sys.argv) > 1:
    mdsPulseNumber = int(sys.argv[1])
else:
    mdsPulseNumber = 1
    #treename = ''

mdsTreeName = 'rtappisttok'

try:
    tree = Tree(mdsTreeName, mdsPulseNumber)
except:
    print(f'Failed opening {mdsTreeName} for pulse number {mdsPulseNumber:d}')
    exit()

#time = dataCsv['#Time (uint32)[1]']
#timeRel = time - time[0]
#x = DECIM_RATE * np.arange(len(vals))

win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting examples")
win.resize(1000,600)
win.setWindowTitle('pyqtgraph example: Plotting')

# Enable antialiasing for prettier plots
pg.setConfigOptions(antialias=True)

p1 = win.addPlot(title="ATCA ADC raw decim")
# add plt.addLegend() BEFORE you create the curves.
mdsNode = tree.getNode("ATCAIOP1.ADC0RAW")
dataAdc = mdsNode.getData().data()
timeData = mdsNode.getDimensionAt(0).data()
p1.addLegend()
for i in range(0,4):
    mdsNode = tree.getNode(f"ATCAIOP1.ADC{i}RAW")
    dataAdc = mdsNode.getData().data()
    timeData = mdsNode.getDimensionAt(0).data()
    y = dataAdc[:MAX_SAMPLES, 0]
    x = DECIM_RATE * np.arange(len(y)) / 2.0e6
    p1.plot(x,y, pen=pg.mkPen(i, width=2), name=f"Ch {i}")
#p1.setLabel('bottom', "Y Axis", units='s')

win.nextRow()
p4 = win.addPlot(title="ATCA Integral Channels")
p4.addLegend()
for i in range(8,12):
    mdsNode = tree.getNode(f"ATCAIOP1.ADC{i}INT")
    dataAdc = mdsNode.getData().data()
    timeData = mdsNode.getDimensionAt(0).data()
    y = dataAdc[:MAX_SAMPLES, 0]
    #x = DECIM_RATE * np.arange(len(y))
    x = DECIM_RATE * np.arange(len(y)) / 2.0e6
    p4.plot(x,y, pen=pg.mkPen(i, width=2), name=f"Ch {i}")

p4.setLabel('bottom', "Time", units='s')

#updatePlot()

if __name__ == '__main__':
    pg.exec()
# vim: syntax=python ts=4 sw=4 sts=4 sr et
