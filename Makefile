CC = gcc
CFLAGS = -Wall

# Executable name
TARGET = calc.exe

# Compile all .c files into the executable
$(TARGET): main.c list_functions.c list_operations.c
	$(CC) $(CFLAGS) main.c list_functions.c list_operations.c -o $(TARGET)

# Delete the executable
clean:
	rm -f $(TARGET)


