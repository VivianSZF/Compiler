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

testc1:
	./parser test/compulsory/1.cmm

testc2:
	./parser test/compulsory/2.cmm

testc3:
	./parser test/compulsory/3.cmm

testc4:
	./parser test/compulsory/4.cmm

testo1:
	./parser test/others/1.cmm

testo2:
	./parser test/others/2.cmm

testo3:
	./parser test/others/3.cmm

testo4:
	./parser test/others/4.cmm

testo5:
	./parser test/others/5.cmm

testo6:
	./parser test/others/6.cmm
