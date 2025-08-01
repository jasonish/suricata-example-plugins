# Example Suricata Plugins

These are example plugins for Suricata 8.0. To see plugins for 7.0 and
6.0 see the 7.0 and 6.0 branches.

## eve-filetype

This is an example plugin that implements a "file-type" for Suricata
eve logs. A file type plugin receives rendered EVE records which can
then be written to a file, sent to a database, etc.

## Building

To build a plugin, go into the plugin directory and execute make like:
```
CPPFLAGS="-I/path/to/suricata/src" make
```
