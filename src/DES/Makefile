all: des

des: des.o
	gcc $< -o $@

%.o: %.c
	gcc -c $< -o $@

clean:
	rm *.o des