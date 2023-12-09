CC=gcc
CFLAGS=-Wall -g
TARGET=my_mlfq

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
