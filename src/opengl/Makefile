# we compile with external/glm-0.9.7.1 in the path so that it does not have to be installed into the system, we can get it locally
# same for glad

DEBUG	:=	-g
CXX	:= g++ $(DEBUG) -std=c++11 -I external -I external/glm-0.9.7.1 -I external/glad/include $(DEBUG) $(OPT)
SHARED	:= GLWin.o glad.o shader.o
LIBS	:= -L . -lsitgame -lglfw -lGL -lX11 -ldl

#	g++ -g -std=c++11  -pthread -fsanitize=address ./linux/ltest.cc ./linux/glad.o -o dbkbd 

OBJ	:= main.o libsitgame.a

all: testwin demo1

testwin: $(OBJ)
	$(CXX) $(OBJ) -o testwin $(LIBS)

libsitgame.a: $(SHARED)
	ar rvs libsitgame.a $(SHARED) 

DEMO1OBJ	:= TriangleDemo.o libsitgame.a
demo1: $(DEMO1OBJ)
	$(CXX) $(DEMO1OBJ) -o demo1 $(LIBS)

TriangleDemo.o: TriangleDemo.cc
	$(CXX) -c $<

main.o: main.cc GLWin.hh
	$(CXX) -c main.cc

GLWin.o: GLWin.cc GLWin.hh
	$(CXX) -c GLWin.cc

shader.o: shader.cpp shader.hpp
	$(CXX) -c shader.cpp

glad.o: external/glad/src/glad.c
	g++ -g -std=c++11 -c -I external/glad/include external/glad/src/glad.c -o glad.o

clean:
	rm *.o demo1 testwin
