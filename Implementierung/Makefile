# Comiler flags
CFLAGS = -O3 -g2 -Wall -Werror -Wextra -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L
# Compiler
CC = gcc
# Libraries
LIBRARYS = -lm

# The resulting executable files to be created
output = approxArsinh_lookup.o approxArsinh_series.o approxArsinh_complexInstructions.o Implementierung.o performance.o

# The command 'make' will execute the first target entry (this) in the file
Implementierung: $(output)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBRARYS)

# .o files depend on .c and .h files with the same name
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

# will run regardless of whether a file named clean exists or what it's contents are
.PHONY: clean
clean:
	rm -f main $(output) Implementierung

# Source/Reference: https://www.gnu.org/software/make/manual/make.html
# Source/Reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# Source/Reference: https://stackoverflow.com/questions/60353633/how-to-include-d-posix-c-source-200809-when-you-compile-the-code
