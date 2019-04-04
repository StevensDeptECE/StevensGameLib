CXX := g++ -g -std=c++11

asteroids: dbkbd.o player.o bullet.o asteroids.o glad.o shader.o
	$(CXX) dbkbd.o player.o bullet.o asteroids.o glad.o shader.o -pthread -lglfw -lGL -lX11 -ldl -o a.out

asteroids.o: asteroids.cc
	$(CXX) -c -I./includes/glad/include asteroids.cc -o asteroids.o 	

player.o: player.cc player.hh
	$(CXX) -c -I./includes/glad/include -I./includes/glm player.cc -o player.o 	

bullet.o: bullet.cc bullet.hh
	$(CXX) -c -I./includes/glad/include -I./includes/glm bullet.cc -o bullet.o 	

dbkbd.o: dbkbd.cc dbkbd.hh
	$(CXX) -c dbkbd.cc -o dbkbd.o

shader.o: shader.cc shader.hh
	$(CXX) -c -I./includes/glad/include shader.cc -o shader.o

glad.o: ./includes/glad/src/glad.c
	$(CXX) -c -I./includes/glad/include ./includes/glad/src/glad.c -o glad.o

clean:
	rm -f *.o *.out
