p5: p5.c
	gcc -pthread -g -Wall -o p5 p5.c

clean:
	rm -f p5