folders = obj
CC = gcc
CFLAGS = -O3 -Wall
OBJS = obj/levenshtein.o obj/file.o obj/word.o obj/shift_and.o
SRCS = main.c

TARGET = tp3_parte1

all: $(TARGET)

$(TARGET): $(folders) $(OBJS) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(OBJS)

obj/levenshtein.o: levenshtein.c levenshtein.h
	$(CC) $(CFLAGS) -c levenshtein.c -o $@

obj/file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c -o $@

obj/shift_and.o: shift_and.c shift_and.h
	$(CC) $(CFLAGS) -c shift_and.c -o $@

obj/word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c -o $@


folders:
obj:
		mkdir -p obj

run: all
	./$(TARGET)

clean:
		rm -f obj/*.o $(TARGET)
		rm -r obj