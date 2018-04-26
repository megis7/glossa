all: y.tab.c lex.yy.c 

	mkdir -p objs
	g++ -c main.cpp -o objs/main.o -std=c++11
	g++ -c AstNode.cpp -o objs/AstNode.o -std=c++11
	g++ -c Structures.cpp -o objs/Structures.o -std=c++11
	g++ -c Result.cpp -o objs/Result.o -std=c++11
	g++ -c Scope.cpp -o objs/Scope.o -std=c++11

	# output file, which contains grammar confilcts
	# bison -r state -d interp.y

	g++ objs/main.o objs/lex.yy.o objs/y.tab.o objs/AstNode.o objs/Structures.o objs/Result.o objs/Scope.o -o interp -std=c++11

run: all
	./interp < test2.txt

lex.yy.c: y.tab.c interp.l
	lex interp.l
	mkdir -p objs	
	g++ -c lex.yy.c -o objs/lex.yy.o -std=c++11

y.tab.c: interp.y
	yacc -d interp.y
	mkdir -p objs
	g++ -c y.tab.c -o objs/y.tab.o -std=c++11 -Wwrite-strings

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h interp interp.tab.c interp.tab.h
	rm -rf objs/*