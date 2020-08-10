NAME :=		source-pfring

SRCS :=		plugin.c \
		source-pfring.c \
		runmode-pfring.c

CPPFLAGS +=	-DSURICATA_PLUGIN -DHAVE_CONFIG_H -I.
CPPFLAGS +=	-DHAVE_PF_RING_FLOW_OFFLOAD

all:
	$(CC) -fPIC $(CPPFLAGS) -o $(NAME).so -shared $(SRCS)

clean:
	rm -f *.so *~
