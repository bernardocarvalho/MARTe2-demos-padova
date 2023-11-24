#!/usr/bin/env python3
"""
Extract data from csv files
"""
import sys
import numpy as np
#from scipy.io import savemat import scipy.io as sio 
import matplotlib.pyplot as plt
from pandas import read_csv
#from atcaDataFunctions import get_data32_raw, get_data32
#, get_float, get_decim_float, get_interlock_status, get_count64
if len(sys.argv) > 1:
    filename = str(sys.argv[1])
else:
    filename = '/tmp/AtcaIop.csv'

MAX_SAMPLES = 1000
ADC_CHANNELS = 4
DECIM_RATE = 200

if __name__ == '__main__':
    data = read_csv(filename, sep=',')
    # data.columns
    #data_m=np.mean(data, axis=1).astype(int)
    #for i in range(ADC_CHANNELS):
    #    print(f"Ch {i}: {data_m[i]:d}")
    plt.figure()
   # plt.plot(data[0,:MAX_SAMPLES],label=f"M{1} CHA")
    #plt.plot(data[1,0:MAX_SAMPLES],label="M"+str(1)+" CHA")
    #plt.plot(data[2,0:MAX_SAMPLES],label="M"+str(2)+" CHA")
    #plt.plot(data[3,0:MAX_SAMPLES],label="M"+str(3)+" CHA")
    adc3Dec = data['AdcRawDecim3 (float32)[1]']
    vals = adc3Dec[:MAX_SAMPLES]
    #x = DECIM_RATE * np.arange(vals.shape[0])
    x = DECIM_RATE * np.arange(len(vals))
    plt.plot(x,vals,label=f"M{2} ChB" )
    adc4Dec = data['AdcRawDecim4 (float32)[1]']
    vals = adc4Dec[:MAX_SAMPLES]
    plt.plot(x,vals,label=f"M{3} ChA" )
    adc5Dec = data['AdcRawDecim5 (float32)[1]']
    vals = adc5Dec[:MAX_SAMPLES]
    plt.plot(x,vals,label=f"M{3} ChB" )
    #x = DECIM_RATE * np.arange(data.shape[1])
    #for i in range(1,3):
    #    plt.plot(data[i*2,  :MAX_SAMPLES],label=f"M{i} ChA")
    #    plt.plot(data[i*2+1,:MAX_SAMPLES],label=f"M{i} ChB")
    #plt.plot(data[0,:],label="M"+str(0)+"CHA")
    #for i in range(3):
#		plt.plot(counts[::decim]*0.5/1000,signal[i*2+0][::decim],label="M"+str(i+1)+"CHA")
#		plt.plot(counts[::decim]*0.5/1000,signal[i*2+1][::decim],label="M"+str(i+1)+"CHB")
    #plt.plot(counts[::decim]*0.5/1000,chopper[::decim]*np.max(signal),"k",label="Phase")
    plt.legend()
    plt.grid()
    plt.ylabel("ADC counts")
    plt.xlabel("Sample")
    plt.show()


# vim: sta:et:sw=4:ts=4:sts=4
