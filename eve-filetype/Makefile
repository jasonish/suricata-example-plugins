SRCS :=		template.c

CPPFLAGS +=	-DSURICATA_PLUGIN -I. -I$(SURICATA_SRC)/src

eve-template.so: $(SRCS)
	$(CC) -fPIC $(CPPFLAGS) -o eve-template.so -shared $(SRCS)

clean:
	rm -f *.so *~
