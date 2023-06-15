# Add additional compiler flags here
CFLAGS = -O0 -g2 -Wall -Werror -Wextra -pedantic -std=c17
CC = gcc

output = approxArsinh_lookup.o approxArsinh_series.o Implementierung.o
tocompile = approxArsinh_lookup.c approxArsinh_lookup.h approxArsinh_series.c approxArsinh_series.h Implementierung.c

main: $(output)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main $(output)





#cc -O0 -o main main.c xor.S