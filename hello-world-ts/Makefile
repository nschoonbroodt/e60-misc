CFLAGS=-g

ODIR=obj
BDIR=bin

_OBJ = hello-world-ts.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

HEADERS=ts.h

$(ODIR)/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/hello-ts: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

all: $(BDIR)/hello-ts

