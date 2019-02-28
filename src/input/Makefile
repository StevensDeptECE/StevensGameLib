dbkbd: ./linux/ltest.cc
	g++ -g -std=c++11 -pthread -fsanitize=address ./linux/ltest.cc -o dbkbd -lglut -lGL 
	
windows: ./windows/wintest.cc
	g++ -g -std=c++11 -lgdi32 ./windows/wintest.cc -o dbkbd.exe -mwindows
