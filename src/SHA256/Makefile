all: sha256

sha256: sha256.o
	gcc $< -o $@

%.o: %.c
	gcc -c $< -o $@

clean:
	rm *.o sha256