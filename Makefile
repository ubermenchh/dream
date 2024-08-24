CC = gcc 
CFLAGS = -Wall -Wextra -std=c11 
LIBFLAGS = -lm
TARGET = main 

$(TARGET): main.o 
	$(CC) $(CFLAGS) $(LIBFLAGS) -o $(TARGET) main.o 

main.o: main.c 
	$(CC) $(CFLAGS) $(LIBFLAGS) -c main.c 

clean: 
	rm -rf $(TARGET) *.o *.exe 

.PHONY: clean
