dbkbd: ./linux/ltest.cc glad.o
	g++ -g -std=c++11 -I./linux/glad/include -pthread -fsanitize=address ./linux/ltest.cc ./linux/glad.o -o dbkbd -lglfw -lGL -lX11 -ldl 
	rm ./linux/glad.o
	
windows: ./windows/wintest.cc
	g++ -g -std=c++11 -lgdi32 ./windows/wintest.cc -o dbkbd.exe -mwindows

glad.o: ./linux/glad/src/glad.c
	g++ -g -std=c++11 -c -I./linux/glad/include ./linux/glad/src/glad.c -o ./linux/glad.o

asm: ./x86_64asm/test.s glad_asm.o
	gcc -I./x86_64asm/glad/include ./x86_64asm/test.s ./x86_64asm/glad_asm.o -o ./x86_64asm/test -lglfw -lGL -lX11 -ldl -fPIC

glad_asm.o: ./x86_64asm/glad/src/glad.c
	gcc -c -I./x86_64asm/glad/include ./x86_64asm/glad/src/glad.c -o ./x86_64asm/glad_asm.o
