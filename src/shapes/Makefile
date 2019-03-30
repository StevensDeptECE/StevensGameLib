# we compile with external/glm-0.9.7.1 in the path so that it does not have to be installed into the system, we can get it locally
# same for glad

DEBUG	:=	-g
CXX	:= g++ $(DEBUG) -std=c++11 -I external/glm-0.9.7.1 $(DEBUG) $(OPT)


all: shapes

OBJ	:= ShapeTest.o
shapedemo: $(OBJ)
	$(CXX) $(OBJ) -o shapedemo

DEMO1OBJ	:= TriangleDemo.o libsitgame.a

clean:
	rm *.o shapedemo
