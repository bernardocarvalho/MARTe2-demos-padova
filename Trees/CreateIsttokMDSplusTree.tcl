edit rtappisttok/new

add node .ATCAIOP1
add node .INP
add node .REFS
add node .ERRORS
add node .CONTROLS
add node .STATES
add node .PERF
add node TIME/usage=signal
add node TIMED/usage=signal

set def \RTAPPISTTOK::TOP.ATCAIOP1
add node ADC0RAW/usage=signal
add node ADC0RAWD/usage=signal
add node ADC1RAW/usage=signal
add node ADC1RAWD/usage=signal
add node ADC2RAW/usage=signal
add node ADC2RAWD/usage=signal
add node ADC3RAW/usage=signal
add node ADC3RAWD/usage=signal
add node ADC4RAW/usage=signal
add node ADC4RAWD/usage=signal
add node ADC5RAW/usage=signal
add node ADC5RAWD/usage=signal
add node ADC6RAW/usage=signal
add node ADC6RAWD/usage=signal
add node ADC7RAW/usage=signal
add node ADC7RAWD/usage=signal
add node ADC8RAW/usage=signal
add node ADC8RAWD/usage=signal
add node ADC9RAW/usage=signal
add node ADC9RAWD/usage=signal
add node ADC10RAW/usage=signal
add node ADC10RAWD/usage=signal
add node ADC11RAW/usage=signal
add node ADC11RAWD/usage=signal

add node ADC0INT/usage=signal
add node ADC0INTD/usage=signal
add node ADC1INT/usage=signal
add node ADC1INTD/usage=signal
add node ADC2INT/usage=signal
add node ADC2INTD/usage=signal
add node ADC3INT/usage=signal
add node ADC3INTD/usage=signal
add node ADC4INT/usage=signal
add node ADC4INTD/usage=signal
add node ADC5INT/usage=signal
add node ADC5INTD/usage=signal
add node ADC6INT/usage=signal
add node ADC6INTD/usage=signal
add node ADC7INT/usage=signal
add node ADC7INTD/usage=signal
add node ADC8INT/usage=signal
add node ADC8INTD/usage=signal
add node ADC9INT/usage=signal
add node ADC9INTD/usage=signal
add node ADC10INT/usage=signal
add node ADC10INTD/usage=signal
add node ADC11INT/usage=signal
add node ADC11INTD/usage=signal

set def \RTAPPISTTOK::TOP.INP
add node MEAS0D/usage=signal
add node MEAS0/usage=signal
add node MEAS0D/usage=signal
add node MEAS0F/usage=signal
add node MEAS0FD/usage=signal
add node MEAS1/usage=signal
add node MEAS1D/usage=signal
add node MEAS1F/usage=signal
add node MEAS1FD/usage=signal

set def \RTAPPISTTOK::TOP.REFS
add node REF0/usage=signal
add node REF0D/usage=signal
add node REF1/usage=signal
add node REF1D/usage=signal

set def \RTAPPISTTOK::TOP.ERRORS
add node ERROR0/usage=signal
add node ERROR0D/usage=signal
add node ERROR1/usage=signal
add node ERROR1D/usage=signal

set def \RTAPPISTTOK::TOP.CONTROLS
add node CONTROL0/usage=signal
add node CONTROL0D/usage=signal
add node CONTROL1/usage=signal
add node CONTROL1D/usage=signal

set def \RTAPPISTTOK::TOP.STATES
add node STATE0/usage=signal
add node STATE0D/usage=signal
add node STATE1/usage=signal
add node STATE1D/usage=signal
add node STATE2/usage=signal
add node STATE2D/usage=signal
add node STATE3/usage=signal
add node STATE3D/usage=signal

set def \RTAPPISTTOK::TOP.PERF
add node CYCLET/usage=signal
add node CYCLETD/usage=signal

write
close
set tree rtappisttok
create pulse 1
exit
