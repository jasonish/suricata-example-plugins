NAME :=		source-pfring

SRCS :=		plugin.c \
		source-pfring.c \
		runmode-pfring.c

# This needs to point to the Suricata include directory.
CPPFLAGS +=	-I/usr/local/include/suricata

CPPFLAGS +=	-DHAVE_PF_RING_FLOW_OFFLOAD
CPPFLAGS +=	-DSURICATA_PLUGIN -I. 

all:
	$(CC) -fPIC $(CPPFLAGS) -o $(NAME).so -shared $(SRCS)

clean:
	rm -f *.so *~
