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


testc1:
	./parser test/compulsory/1.cmm

testc2:
	./parser test/compulsory/2.cmm

testc3:
	./parser test/compulsory/3.cmm

testc4:
	./parser test/compulsory/4.cmm

testc5:
	./parser test/compulsory/5.cmm

testc6:
	./parser test/compulsory/6.cmm

testc7:
	./parser test/compulsory/7.cmm

testc8:
	./parser test/compulsory/8.cmm

testc9:
	./parser test/compulsory/9.cmm

testc10:
	./parser test/compulsory/10.cmm

testc11:
	./parser test/compulsory/11.cmm

testc12:
	./parser test/compulsory/12.cmm

testc13:
	./parser test/compulsory/13.cmm

testc14:
	./parser test/compulsory/14.cmm

testc15:
	./parser test/compulsory/15.cmm

testc16:
	./parser test/compulsory/16.cmm

testc17:
	./parser test/compulsory/17.cmm

testo1:
	./parser test/others/1.cmm

testo2:
	./parser test/others/2.cmm

testo3:
	./parser test/others/3.cmm

testm1:
	./parser test/my/1.cmm

testall:
	./parser test/compulsory/1.cmm test/compulsory/2.cmm test/compulsory/3.cmm test/compulsory/4.cmm test/compulsory/5.cmm test/compulsory/6.cmm test/compulsory/7.cmm test/compulsory/8.cmm test/compulsory/9.cmm test/compulsory/10.cmm test/compulsory/11.cmm test/compulsory/12.cmm test/compulsory/13.cmm test/compulsory/14.cmm test/compulsory/15.cmm test/compulsory/16.cmm test/compulsory/17.cmm test/others/1.cmm test/others/2.cmm test/others/3.cmm
