dbkbd: ./linux/ltest.cc glad.o
	g++ -g -std=c++11 -I./linux/glad/include -pthread -fsanitize=address ./linux/ltest.cc ./linux/glad.o -o dbkbd -lglfw -lGL -lX11 -ldl 
	rm ./linux/glad.o
	
windows: ./windows/wintest.cc
	g++ -g -std=c++11 -lgdi32 ./windows/wintest.cc -o dbkbd.exe -mwindows

glad.o: ./linux/glad/src/glad.c
	g++ -g -std=c++11 -c -I./linux/glad/include ./linux/glad/src/glad.c -o ./linux/glad.o
