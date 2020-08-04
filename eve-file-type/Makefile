SRCS :=		template.c

# This needs to point to the Suricata includes.
CPPFLAGS +=	-I/usr/local/include/suricata

CPPFLAGS +=	-DSURICATA_PLUGIN -I.

all:
	$(CC) -fPIC $(CPPFLAGS) -o eve-filetype.so -shared $(SRCS)

clean:
	rm -f *.so *~
