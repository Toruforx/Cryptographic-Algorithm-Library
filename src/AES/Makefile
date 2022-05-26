all: aes

aes: aes.o
	gcc $< -o $@

%.o: %.c
	gcc -c $< -o $@

clean:
	rm *.o aes