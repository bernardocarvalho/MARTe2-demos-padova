edit rtappdemo/new

add node .INP
add node .REFS
add node .ERRORS
add node .CONTROLS
add node .STATES
add node .PERF
add node TIME/usage=signal
add node TIMED/usage=signal

set def \RTAPPDEMO::TOP.INP
add node MEAS0/usage=signal
add node MEAS0D/usage=signal
add node MEAS0F/usage=signal
add node MEAS0FD/usage=signal
add node MEAS1/usage=signal
add node MEAS1D/usage=signal
add node MEAS1F/usage=signal
add node MEAS1FD/usage=signal
add node MEAS2/usage=signal
add node MEAS2D/usage=signal
add node MEAS2F/usage=signal
add node MEAS2FD/usage=signal
add node MEAS3/usage=signal
add node MEAS3D/usage=signal
add node MEAS3F/usage=signal
add node MEAS3FD/usage=signal

set def \RTAPPDEMO::TOP.REFS
add node REF0/usage=signal
add node REF0D/usage=signal
add node REF1/usage=signal
add node REF1D/usage=signal
add node REF2/usage=signal
add node REF2D/usage=signal
add node REF3/usage=signal
add node REF3D/usage=signal

set def \RTAPPDEMO::TOP.ERRORS
add node ERROR0/usage=signal
add node ERROR0D/usage=signal
add node ERROR1/usage=signal
add node ERROR1D/usage=signal
add node ERROR2/usage=signal
add node ERROR2D/usage=signal
add node ERROR3/usage=signal
add node ERROR3D/usage=signal

set def \RTAPPDEMO::TOP.CONTROLS
add node CONTROL0/usage=signal
add node CONTROL0D/usage=signal
add node CONTROL1/usage=signal
add node CONTROL1D/usage=signal
add node CONTROL2/usage=signal
add node CONTROL2D/usage=signal
add node CONTROL3/usage=signal
add node CONTROL3D/usage=signal

set def \RTAPPDEMO::TOP.STATES
add node STATE0/usage=signal
add node STATE0D/usage=signal
add node STATE1/usage=signal
add node STATE1D/usage=signal
add node STATE2/usage=signal
add node STATE2D/usage=signal
add node STATE3/usage=signal
add node STATE3D/usage=signal

set def \RTAPPDEMO::TOP.PERF
add node CYCLET/usage=signal
add node CYCLETD/usage=signal

write
close
exit
