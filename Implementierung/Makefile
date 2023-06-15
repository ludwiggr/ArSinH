# Add additional compiler flags here
CFLAGS = -O0 -g2 -Wall -Werror -Wextra -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L
CC = gcc

output = approxArsinh_lookup.o approxArsinh_series.o Implementierung.o

Implementierung: $(output)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(output)

