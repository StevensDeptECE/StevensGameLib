CXX := g++ -g -std=c++11 -I./includes/glm -I./includes/glad/include

OBJ := main.o dbkbd.o player.o bullet.o asteroid.o glad.o shader.o game.o 

main: $(OBJ) game_object.hh
	$(CXX) $(OBJ) -pthread -lglfw -lGL -lX11 -ldl -o main

main.o: main.cc 
	$(CXX) -c  main.cc -o main.o 	

asteroid.o: asteroid.cc asteroid.hh game_object.hh
	$(CXX) -c  -I./includes/glm asteroid.cc -o asteroid.o

player.o: player.cc player.hh
	$(CXX) -c  -I./includes/glm player.cc -o player.o 	

bullet.o: bullet.cc bullet.hh
	$(CXX) -c  -I./includes/glm bullet.cc -o bullet.o 	

dbkbd.o: dbkbd.cc dbkbd.hh
	$(CXX) -c dbkbd.cc -o dbkbd.o

shader.o: shader.cc shader.hh
	$(CXX) -c shader.cc -o shader.o

glad.o: ./includes/glad/src/glad.c
	$(CXX) -c ./includes/glad/src/glad.c -o glad.o

game.o: game.cc game.hh
	$(CXX) -c  game.cc -o game.o

clean:
	rm -f *.o *.out main
