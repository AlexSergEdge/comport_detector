### Virtual COM ports in Linux

#### Socat

To create virtual COM ports for testing purposes we can use socat:
```bash
$ socat -d -d pty,rawer,echo=0 pty,rawer,echo=0
2023/11/29 16:06:20 socat[528] N PTY is /dev/pts/2
2023/11/29 16:06:20 socat[528] N PTY is /dev/pts/3
2023/11/29 16:06:20 socat[528] N starting data transfer loop with FDs [5,5] and [7,7]
```
Two virtual serial ports will be created.

We can send data to `/dev/pts/2`:
```bash
echo "Hi!" > /dev/pts/4
```
And receive from `/dev/pts/3`
```bash
cat < /dev/pts/8
```
Or we can read it to variable.
```bash
read data_var < /dev/pts/8
echo $data_var
```

We can do it other way around too.

#### Minicom

Better way to communicate is using minicom:
```
minicom -D /dev/pts/2 -b 9600
```

#### Systemd service

To create and use ports in background we can use systemd service.

Copy `./system/vcom.target` and `./system/vcom@.service` to `/etc/systemd/system/`

Run:
```bash 
systemctl start vcom.target
```

Following two pairs of ports will be created:
```
/dev/PORT100 + /dev/PORT101
/dev/PORT200 + /dev/PORT201
```

You can customise number of port pairs created in `target` file in line:
```
Requires=vcom@1.service vcom@2.service
```

> If process uses port and cannot be terminated use:
> ```bash
> fuser -k /dev/PORT100
> ```
