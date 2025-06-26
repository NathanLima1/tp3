folders = obj
CFLAGS = -O3 -Wall
OBJS = obj/huffman_marc.o obj/bmh_compressed.o obj/bmh_decompressed.o obj/hash-table.o obj/file.o obj/alfabeto.o obj/tipos.o obj/compressao.o obj/descompressao.o
SRCS = main.c

TARGET = tp3_parte2

all: $(TARGET)

$(TARGET): $(folders) $(OBJS) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(OBJS)

obj/hash-table.o: utils/hash-table.c utils/hash-table.h
	$(CC) $(CFLAGS) -c utils/hash-table.c -o $@

obj/file.o: utils/file.c utils/file.h
	$(CC) $(CFLAGS) -c utils/file.c -o $@

obj/tipos.o: utils/tipos.c utils/tipos.h
	$(CC) $(CFLAGS) -c utils/tipos.c -o $@

obj/compressao.o: compressao.c compressao.h huffman_header.h utils/tipos.h
	$(CC) $(CFLAGS) -c compressao.c -o $@

obj/descompressao.o: descompressao.c descompressao.h huffman_header.h utils/tipos.h
	$(CC) $(CFLAGS) -c descompressao.c -o $@

obj/alfabeto.o: alfabeto.c alfabeto.h huffman_header.h utils/tipos.h
	$(CC) $(CFLAGS) -c alfabeto.c -o $@

obj/huffman_marc.o: huffman_marc.c huffman_marc.h huffman_header.h utils/tipos.h
	$(CC) $(CFLAGS) -c huffman_marc.c -o $@

obj/bmh_compressed.o: bmh_compressed.c bmh_compressed.h huffman_header.h utils/tipos.h
	$(CC) $(CFLAGS) -c bmh_compressed.c -o $@

obj/bmh_decompressed.o: bmh_decompressed.c bmh_decompressed.h
	$(CC) $(CFLAGS) -c bmh_decompressed.c -o $@

folders:
obj:
	mkdir -p obj

run: all
	./$(TARGET)

clean:
	rm -f obj/*.o $(TARGET)
	rm -r obj
