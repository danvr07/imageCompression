CC=gcc

build: quadtree

quadtree: quadtree.c functii.h
	$(CC) quadtree.c -o quadtree

.PHONY: clean

clean:
	rm -f quadtree
