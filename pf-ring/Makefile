NAME :=		source-pfring

SRCS :=		plugin.c \
		source-pfring.c \
		runmode-pfring.c

CPPFLAGS +=	`libsuricata-config --cflags`
CPPFLAGS +=	-DSURICATA_PLUGIN
CPPFLAGS +=	-DHAVE_PF_RING_FLOW_OFFLOAD

# Currently the Suricata logging system requires this to be even for
# plugins.
CPPFLAGS +=    "-D__SCFILENAME__=\"$(*F)\""

all:
	$(CC) -fPIC $(CPPFLAGS) -o $(NAME).so -shared $(SRCS) -lpfring

clean:
	rm -f *.so *~
