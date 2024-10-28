CC = gcc
CFLAGS = -Wall -Wextra
TARGET = runme

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET) home/alexduna/Task2 home/alexduna/Task2Destination

clean:
	rm -f $(TARGET)

