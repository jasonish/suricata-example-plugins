SRCS :=		template.c

LIBSURICATA_CONFIG ?= libsuricata-config

CPPFLAGS +=	`$(LIBSURICATA_CONFIG) --cflags`
CPPFLAGS +=	-DSURICATA_PLUGIN -I.

all:
	$(CC) -fPIC $(CPPFLAGS) -o eve-filetype.so -shared $(SRCS)

clean:
	rm -f *.so *~
