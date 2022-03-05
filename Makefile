.PHONY: run clean

run: main
	./main

main: main.o
	gcc -o $@ $<

main.o: main.c
	gcc -c $<
	
clean: 
	rm -f main.o
	rm -f main