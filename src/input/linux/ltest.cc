#include "dbkbd.hh"
#include <time.h>
#include <pthread.h>
#include <glad/glad.h>

class Player {
private:
	const int size = 75;
	double r, g, b;

public:
	float x, y;
	Player(float a, float b) : x(a), y(b)
	{
		r = (double)rand() / RAND_MAX;
		g = (double)rand() / RAND_MAX;
		b = (double)rand() / RAND_MAX;
	}

	void draw()
	{
	}
};

vector<Player> players;
bool isIdle = true;
bool play = true;
void draw()
{
	glClearColor(0.4, 0.4, 0.4, 0.4);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < players.size(); ++i)
		players[i].draw();
}

void idle()
{
//	if (!isIdle)
//		glutPostRedisplay();
//	isIdle = true;
	usleep(10000);
}

void *my_keyboard_handler(void *);
void drawTriangle();
int main(int argc, char **argv)
{
//	srand(time(NULL));
//	Inputs i;
//
//	cout << "Keyboards: " << i.keyboard_count() << endl;
//
//	pthread_t myThread;
//	pthread_create(&myThread, NULL, &my_keyboard_handler, &i);
//
	for (int j = 0; j < 1; ++j)
		players.push_back(Player(rand() % 725, rand() % 525));


	play = false;

	return 0;
}

void *my_keyboard_handler(void *input)
{
	Inputs *i = static_cast<Inputs *>(input);
	while (play) {
		for (int j = 0; play && j < players.size(); ++j) {
			float o_x = players[j].x;
			float o_y = players[j].y;
			if (i->get_key(j, 103))
				players[j].y -= 1;
			if (i->get_key(j, 105))
				players[j].x -= 1;
			if (i->get_key(j, 106))
				players[j].x += 1;
			if (i->get_key(j, 108))
				players[j].y += 1;
			if (o_x != players[j].x || o_y != players[j].y)
				isIdle = false;
		}
		usleep(5000);	// sample at 2x framerate?
	}
	return nullptr;
}
