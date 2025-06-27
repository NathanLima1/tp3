folders = obj
CFLAGS = -O3 -Wall
OBJS = obj/bmh_decompressed.o obj/file.o
SRCS = main.c

TARGET = tp3_parte2

all: $(TARGET)

$(TARGET): $(folders) $(OBJS) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(OBJS)

obj/file.o: utils/file.c utils/file.h
	$(CC) $(CFLAGS) -c utils/file.c -o $@

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
