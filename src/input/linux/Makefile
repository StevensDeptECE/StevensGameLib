CXX := g++ -g -std=c++11

linux: dbkbd.o ltest.o glad.o
	$(CXX) dbkbd.o ltest.o glad.o -pthread -fsanitize=address -lglfw -lGL -lX11 -ldl -o a.out

ltest.o: ltest.cc
	$(CXX) -c -I./glad/include ltest.cc -o ltest.o 	
dbkbd.o: ./dbkbd.cc ./dbkbd.hh
	$(CXX) -c dbkbd.cc -o dbkbd.o

glad.o: ./glad/src/glad.c
	$(CXX) -c -I./glad/include ./glad/src/glad.c -o glad.o

clean:
	rm -f a.out dbkbd.o ltest.o glad.o
