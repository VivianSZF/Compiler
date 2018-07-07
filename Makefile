lexical:
	cd lexical_syntax && flex lexical.l

syntax:
	cd lexical_syntax && bison -d -v syntax.y

parser:
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c semantic/symbol_table.c semantic/semantic.c intercode/intercode.c intercode/translate.c objectcode/objectcode.c -std=c99 -w -lfl -ly -o parser

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

lab4:
	cd lexical_syntax && bison -d -v syntax.y
	cd lexical_syntax && flex lexical.l
	gcc main.c lexical_syntax/syntax.tab.c lexical_syntax/treeop.c semantic/symbol_table.c semantic/semantic.c intercode/intercode.c intercode/translate.c objectcode/objectcode.c -std=c99 -w -lfl -ly -o parser

testc1:
	./parser test/compulsory/1.cmm output/c1.s

testc2:
	./parser test/compulsory/2.cmm output/c2.s

testm1:
	./parser test/my/1.cmm output/m1.s

testm2:
	./parser test/my/2.cmm output/m2.s

testall:
	./parser test/compulsory/1.cmm output/c1.s test/compulsory/2.cmm output/c2.s test/my/1.cmm output/m1.s test/my/2.cmm output/m2.s
