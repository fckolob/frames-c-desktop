CC = gcc
CFLAGS = -Wall -O3
LIBS = -lgdi32 -luser32

SRC = main.c logic.c
OBJ = $(SRC:.c=.o)
TARGET = frames-c.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -mwindows

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o *.exe
