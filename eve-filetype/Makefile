SRCS :=		template.c

LIBSURICATA_CONFIG ?= libsuricata-config

CPPFLAGS +=	`$(LIBSURICATA_CONFIG) --cflags`
CPPFLAGS +=	-DSURICATA_PLUGIN -I.
CPPFLAGS +=	"-D__SCFILENAME__=\"$(*F)\""

OBJS :=		$(SRCS:.c=.o)

eve-filetype.so: $(OBJS)
	$(CC) -fPIC -shared -o $@ $(OBJS)

%.o: %.c
	$(CC) -fPIC $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.so *~
