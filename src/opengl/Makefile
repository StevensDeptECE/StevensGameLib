CXX	:= g++ -std=c++11 $(DEBUG) $(OPT)
OBJ	:= main.o GLWin.o
LIBS	:= -lGLFW -lGL
testwin: $(OBJ) 
	$(CXX) (OBJ) -o testwin $(LIBS)

main.o: main.cc GLWin.hh
	$(CXX) -c main.cc

GLWin.o: GLWin.cc GLWin.hh
	$(CXX) -c GLWin.cc
