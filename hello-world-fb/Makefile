CFLAGS=-g

ODIR=obj
BDIR=bin

_OBJ = hello-world-fb.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

HEADERS=s3c_epd_userland.h

$(ODIR)/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/hello-fb: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

all: $(BDIR)/hello

