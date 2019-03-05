CXX	:= g++ -std=c++11 -I external -I external/glm-0.9.7.1 -I external/glad/include $(DEBUG) $(OPT)
SHARED	:= GLWin.o glad.o
OBJ	:= main.o $(SHARED)
LIBS	:= -lglfw -lGL -lX11 -ldl

#	g++ -g -std=c++11  -pthread -fsanitize=address ./linux/ltest.cc ./linux/glad.o -o dbkbd 

testwin: $(OBJ) 
	$(CXX) $(OBJ) -o testwin $(LIBS)

DEMO1OBJ	:= OldSchoolTriangleDemo.o $(SHARED)
demo1: $(DEMO1OBJ)
	$(CXX) $(DEMO1OBJ) -o demo1 $(LIBS)

main.o: main.cc GLWin.hh
	$(CXX) -c main.cc

GLWin.o: GLWin.cc GLWin.hh
	$(CXX) -c GLWin.cc

glad.o: external/glad/src/glad.c
	g++ -g -std=c++11 -c -I external/glad/include external/glad/src/glad.c -o glad.o
