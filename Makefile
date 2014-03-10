PROD=cprnt

all: main

main:
	gcc main.c -o $(PROD)

clean:
	rm -f *.o
	rm -f $(PROD)
