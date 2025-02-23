# Serial port device detection

Util is user to tell if any device is connected to serial port.

> Important note: These checks do no guarantee that no device is connected to serial port. It all depends on device implementation and how it handles connections to serial ports. There can be cases when device does not set any lines or does not send ant data.

Is case we do not know, what device will be connected, we do not want to send random commands to port to find out if something is connected.

So util tries to find device it in 3 steps:
- Check if any of DCD, DSR, CTS or RI control lines are up
- If not - try to read from port (maybe device is sending something?)
- If not - set DTR up and wait a bit and check if DSR is up

# How to use

Go to `detector` directory and use command:
```
cd ./detector
make && ./build/detector /dev/PORT101
```
Second param is name of port (path)


# Serial port lines

Serial port lines:
```
TX -> RX
RX <- TX
RTS -> CTS
CTS <- RTS
DSR <- DTR
CD <- DTR
DTR -> DSR
DTR -> CD
```

# Testing

For testing we can create virtual ports using `socat` or [tty0tyy](https://github.com/freemed/tty0tty).

You can start `socat` as service (see `emulator/system/`)

> NOTE: socat can only be used to test if device is detected by reading data from port. It does not support command lines and trying to access will result in `Inappropriate ioctl for device`.

So to test detecting by lines you can use `tty0tty` and another util `emulator/src/set_signal`.

> TODO: test case with setting DTR and getting DSR is not yet implemented
