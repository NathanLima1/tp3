folders = obj
CFLAGS = -O3 -Wall
OBJS = obj/huffman.o obj/bmh_compressed.o obj/hash-table.o
SRCS = main.c

TARGET = tp3_parte2

all: $(TARGET)

$(TARGET): $(folders) $(OBJS) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(OBJS)

obj/hash-table.o: utils/hash-table.c utils/hash-table.h
	$(CC) $(CFLAGS) -c utils/hash-table.c -o $@

obj/huffman.o: huffman.c huffman.h utils/hash-table.h
	$(CC) $(CFLAGS) -c huffman.c -o $@

obj/bmh_compressed.o: bmh_compressed.c bmh_compressed.h utils/hash-table.h
	$(CC) $(CFLAGS) -c bmh_compressed.c -o $@

folders:
obj:
		mkdir -p obj

run: all
	./$(TARGET)

clean:
		rm -f obj/*.o $(TARGET)
		rm -r obj