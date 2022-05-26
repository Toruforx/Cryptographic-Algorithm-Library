all: md5

md5: md5.o
	gcc $^ -o $@

%.o: %.c
	gcc -c $^ -o $@ -g

clean:
    rm -rf *.o md5