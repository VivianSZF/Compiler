lexical:
	flex lexical.l

syntax:
	bison -d -v syntax.y

parser:
	gcc main.c syntax.tab.c treeop.c -lfl -ly -o parser

clean:
	rm -f parser

lab1:
	bison -d -v syntax.y
	flex lexical.l
	gcc main.c syntax.tab.c treeop.c -lfl -ly -o parser

test:
	./parser test.cmm
