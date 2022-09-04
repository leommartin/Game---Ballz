PROGS = ballz

ALLEGRO = -lallegro_image  -lallegro_primitives  -lallegro_dialog  -lallegro_ttf -lallegro_font  -lallegro

CFLAGS = -I.
LFLAGS = $(ALLEGRO)

CC = gcc -std=c99 -Wall

%.o:  %.c %.h
	$(CC) $(CFLAGS) -c $< -lm

debug: CFLAGS += -g -D__DEBUG__

all debug: $(PROGS)

$(PROGS) : % : %.o ballzGameplay.o graphicStructure.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) -lm

clean:
	@echo "Limpando sujeira ..."
	@rm -f *% *.bak *~

purge: clean
	@echo "Limpando tudo ..."
	@rm -rf core *.o
	@rm -f $(PROGS)

