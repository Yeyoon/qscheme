app = qscheme

all: scheme.tab.c lex.yy.c 
	gcc -g *.c -o $(app)

scheme.tab.c: scheme.y
	bison -d scheme.y -v

lex.yy.c: scheme.l
	flex scheme.l

clean:
	-rm scheme.tab.*
	-rm lex.yy.c
	-rm $(app)
	-rm scheme.output
