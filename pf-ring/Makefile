NAME :=		source-pfring

SRCS :=		plugin.c \
		source-pfring.c \
		runmode-pfring.c

CPPFLAGS +=	`libsuricata-config --cflags`
CPPFLAGS +=	-DSURICATA_PLUGIN
CPPFLAGS +=	-DHAVE_PF_RING_FLOW_OFFLOAD

all:
	$(CC) -fPIC $(CPPFLAGS) -o $(NAME).so -shared $(SRCS)

clean:
	rm -f *.so *~
