# Example Suricata Plugins

## eve-file-type

This is an example plugin that implements a "file-type" for Suricata eve logs. A file type plugin receives rendered EVE records which can then be written to a file, sent to a database, etc.

## pf-ring

The PF_RING plugin is an example of a plugin implementing a capture source.

## Building

To build a plugin, go into the plugin directory and execute make like:
```
CPPFLAGS="-I/path/to/suricata/src" make
```
