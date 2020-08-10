# PF_RING Plugin for Suricata 6.0beta1+

This is an example capture plugin for Suricata 6.0beta1.

## Building

To build this plugin you will first need to build Suricata 6.0beta1
from source (or use git master). It is recommended to not enable
PF_RING in that build.

The the PF_RING plugin can be build with a command like:
```
CPPFLAGS=-I/home/jason/oisf/code/suricata/master/src make
```

## Running
```
/opt/suricata/bin/suricata \
    --set plugins.0=./source-pfring.so \
    --capture-plugin=pfring-plugin \
    --set pfring.0.interface=eno1
```

### --set plugins.0=./source-pfring.so

This command line option tells Suricata about this plugin. This could also
be done in `suricata.yaml` with the following section:
```
plugins:
  - /path/to/source-pfring.so
```

### --capture-plugin=pfring-plugin

This is the option that tells Suricata to use a plugin for capture, much like
`--pcap` tells Suricata to use libpcap or `--af-packet` tells Suricata to use
AF_PACKET. Here we are telling it to look for a loaded plugin of the name
`pfring-plugin` to provide the capture method.

### --set pfring.0.interface=eno1

This is just overriding the interface name in the example `pfring` configuration
found in the default suricata.yaml, which this plugin knows how to read already
as its based off the PF_RING support in Suricata proper.

There is another command line option `--capture-plugin-args` to pass arbitrary
data on the command line a capture plugin, but this plugin does not yet handle
data provided through this command line parameter.