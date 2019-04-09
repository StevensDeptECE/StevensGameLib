CXX := g++ -g -std=c++11

main: main.o dbkbd.o player.o bullet.o asteroid.o glad.o shader.o game.o
	$(CXX) main.o dbkbd.o player.o bullet.o asteroid.o glad.o shader.o game.o -pthread -lglfw -lGL -lX11 -ldl -o a.out

main.o: main.cc 
	$(CXX) -c -I./includes/glad/include main.cc -o main.o 	

asteroid.o: asteroid.cc asteroid.hh
	$(CXX) -c -I./includes/glad/include -I./includes/glm asteroid.cc -o asteroid.o

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

game.o: game.cc game.hh
	$(CXX) -c -I./includes/glad/include -I./includes/glm game.cc -o game.o

clean:
	rm -f *.o *.out
