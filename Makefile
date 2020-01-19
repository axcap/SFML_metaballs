IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=.
LDIR =../lib

LIBS=-lm -lsfml-graphics -lsfml-window -lsfml-system

_DEPS = metaballs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = metaballs.o metaballs.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

metaballs: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) && ./metaballs

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ metaballs $(INCDIR)/*~
