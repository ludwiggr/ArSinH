# Add additional compiler flags here
CFLAGS = -O0 -g2 -Wall -Werror -Wextra -pedantic -std=c17
CC = gcc

objects = main.c approxArsinh_lookup.c approxArsinh_series.c Implementierung.c

main: $(objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f main 





#cc -O0 -o main main.c xor.S