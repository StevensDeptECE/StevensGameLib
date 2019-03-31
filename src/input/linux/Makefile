CXX := g++ -g -std=c++11

asteroids: dbkbd.o asteroids.o glad.o
	$(CXX) dbkbd.o asteroids.o glad.o -pthread -fsanitize=address -lglfw -lGL -lX11 -ldl -o a.out

asteroids.o: asteroids.cc
	$(CXX) -c -I./includes/glad/include -I./includes/glm asteroids.cc -o asteroids.o 	

dbkbd.o: ./dbkbd.cc ./dbkbd.hh
	$(CXX) -c dbkbd.cc -o dbkbd.o

glad.o: ./includes/glad/src/glad.c
	$(CXX) -c -I./includes/glad/include ./includes/glad/src/glad.c -o glad.o

clean:
	rm -f *.o *.out
