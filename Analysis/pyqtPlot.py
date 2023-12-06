"""
This example demonstrates many of the 2D plotting capabilities
in pyqtgraph. All of the plots may be panned/scaled by dragging with 
the left/right mouse buttons. Right click on any plot to show a context menu.
"""

import sys
import numpy as np

from pandas import read_csv
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore

app = pg.mkQApp("Plotting MARTe2 Data")
#mw = QtWidgets.QMainWindow()
#mw.resize(800,800)

MAX_SAMPLES = 1000
ADC_CHANNELS = 4
DECIM_RATE = 200

if len(sys.argv) > 1:
    filename = str(sys.argv[1])
else:
    filename = 'AtcaIop.csv'

dataCsv = read_csv(filename, sep=',')
adc3Dec = dataCsv['AdcRawDecim3 (float32)[1]']
time = dataCsv['#Time (uint32)[1]']
timeRel = time - time[0]
#vals = adc3Dec[:MAX_SAMPLES]
#x = DECIM_RATE * np.arange(vals.shape[0])
#x = DECIM_RATE * np.arange(len(vals))

win = pg.GraphicsLayoutWidget(show=True, title="Basic plotting examples")
win.resize(1000,600)
win.setWindowTitle('pyqtgraph example: Plotting')

# Enable antialiasing for prettier plots
pg.setConfigOptions(antialias=True)


p1 = win.addPlot(title="ATCA ADC raw decim")
# add plt.addLegend() BEFORE you create the curves.
p1.addLegend()
for i in range(1,3):
    adc = dataCsv[f'AdcRawDecim{i} (float32)[1]']
    y = adc[:MAX_SAMPLES]
    x = DECIM_RATE * np.arange(len(y))
    p1.plot(x,y, pen=pg.mkPen(i, width=2), name=f"Ch {i}")
#p1.setLabel('bottom', "Y Axis", units='s')

win.nextRow()
p4 = win.addPlot(title="ATCA Integral Channels")
p4.addLegend()
for i in range(8,12):
    adc = dataCsv[f'AdcInteg{i} (float32)[1]']
    y = adc[:MAX_SAMPLES]
    x = DECIM_RATE * np.arange(len(y))
    p4.plot(x,y, pen=pg.mkPen(i, width=2), name=f"Ch {i}")
p4.setLabel('bottom', "Y Axis", units='s')

#updatePlot()

if __name__ == '__main__':
    pg.exec()
# vim: syntax=python ts=4 sw=4 sts=4 sr et
