# Add additional compiler flags here
CFLAGS = -O0 -g2 -Wall -Werror -Wextra -pedantic -std=c17
CC = gcc

objects = main.o approxArsinh_lookup.o approxArsinh_series.o Implementierung.o

main: $(objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(objects)





#cc -O0 -o main main.c xor.S