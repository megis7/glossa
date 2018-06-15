
INC_DEST 	:= include interface
INCLUDES := $(patsubst %, -I%, $(INC_DEST))

SOURCES := $(wildcard *.cpp)

OBJECTS :=  $(SOURCES:.cpp=.o)
OBJ_DEST := objs
OBJ_DEPS := $(patsubst %,$(OBJ_DEST)/%,$(OBJECTS))

all: y.tab.c lex.yy.c $(OBJ_DEPS)

	mkdir -p objs
	# use the following line to output output file, which contains grammar confilcts
	# bison -r state -d interp.y

	g++ objs/lex.yy.o objs/y.tab.o $(OBJ_DEPS) -o interp -std=c++11

	echo BUILD SUCCESS

run: all
	./interp < test2.txt

$(OBJ_DEST)/%.o: %.cpp
	g++ -c $< -o $@ -std=c++11 $(INCLUDES)

lex.yy.c: y.tab.c interp.l
	lex interp.l
	mkdir -p objs	
	g++ -c lex.yy.c -o objs/lex.yy.o -std=c++11 $(INCLUDES)

y.tab.c: interp.y
	yacc -d interp.y
	mkdir -p objs
	g++ -c y.tab.c -o objs/y.tab.o -std=c++11 -Wwrite-strings $(INCLUDES)

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h interp interp.tab.c interp.tab.h
	rm -rf objs/*