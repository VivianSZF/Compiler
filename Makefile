lexical:
	cd lexical_syntax && flex lexical.l

syntax:
	cd lexical_syntax && bison -d -v syntax.y

parser:
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c semantic/symbol_table.c semantic/semantic.c intercode/intercode.c intercode/translate.c -std=c99 -w -lfl -ly -o parser

clean:
	rm -f parser

lab1:
	cd lexical_syntax && bison -d -v syntax.y
	cd lexical_syntax && flex lexical.l
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c -lfl -ly -o parser

lab2:
	cd lexical_syntax && bison -d -v syntax.y
	cd lexical_syntax && flex lexical.l
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c semantic/symbol_table.c semantic/semantic.c -std=c99 -w -lfl -ly -o parser

lab3:
	cd lexical_syntax && bison -d -v syntax.y
	cd lexical_syntax && flex lexical.l
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c semantic/symbol_table.c semantic/semantic.c intercode/intercode.c intercode/translate.c -std=c99 -w -lfl -ly -o parser

testc1:
	./parser test/compulsory/1.cmm output/c1.ir

testc2:
	./parser test/compulsory/2.cmm output/c2.ir

testo1:
	./parser test/others/1.cmm output/o1.ir

testo2:
	./parser test/others/2.cmm output/o2.ir

testm1:
	./parser test/my/1.cmm output/m1.ir

testm2:
	./parser test/my/2.cmm output/m2.ir

testall:
	./parser test/compulsory/1.cmm output/c1.ir test/compulsory/2.cmm output/c2.ir test/others/1.cmm output/o1.ir test/others/2.cmm output/o2.ir test/my/1.cmm output/m1.ir test/my/2.cmm output/m2.ir
