# EVE plugin for Suricata 7.0.x

This is an example EVE plugin for Suricata.

## Building

To build this plugin you will first need to have the Suricata
development headers and libraries installed. At the time of this
writing this means building Suricata from source and running:
- `make install-headers`
- `make install-library`

then make sure the newly installed tool `libsuricata-config` can be
found in your path, for example:
```
libsuricata-config --cflags
```

Then a simple `make` should build this plugin.

## Running

To run the plugin, first add the path to the plugin you just compiled to
your `suricata.yaml`, for example:
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
