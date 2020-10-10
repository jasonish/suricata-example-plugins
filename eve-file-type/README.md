EVE plugin for Suricata 6.0 (and above)
=======================================

This is an example EVE plugin for Suricata.

## Building

To build this plugin you will first need to build Suricata from source
(or use git master).

The EVE plugin can be built with a command like:
```
CPPFLAGS=-I/home/jason/oisf/code/suricata/master/src make
```

## Running

To run the plugin, first add the path to the plugin you just compiled to
your `suricata.yaml`:
```
plugins:
  - /home/jason/oisf/code/suricata-example-plugins/eve-file-type/eve-filetype.so
```

Then add an output for the plugin:
```
outputs:
  - eve-log:
      enabled: yes
      filetype: template-filetype-plugin
      types:
        - dns
        - tls
        - http
```

In the example above we use the name specified in the plugin as the
`filetype` and specify that all `dns`, `tls` and `http` log entries
should be sent to the plugin.
