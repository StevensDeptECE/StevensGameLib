CXX	:= g++ -std=c++11 -I external -I external/glad/include $(DEBUG) $(OPT)
OBJ	:= main.o GLWin.o glad.o
LIBS	:= -lglfw -lGL -lX11 -ldl

#	g++ -g -std=c++11  -pthread -fsanitize=address ./linux/ltest.cc ./linux/glad.o -o dbkbd 

testwin: $(OBJ) 
	$(CXX) $(OBJ) -o testwin $(LIBS)

main.o: main.cc GLWin.hh
	$(CXX) -c main.cc

GLWin.o: GLWin.cc GLWin.hh
	$(CXX) -c GLWin.cc

glad.o: external/glad/src/glad.c
	g++ -g -std=c++11 -c -I external/glad/include external/glad/src/glad.c -o glad.o
