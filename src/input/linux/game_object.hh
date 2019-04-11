#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

class GameObject {
private:

protected:
	float r, g, b;
	unsigned int *indices;
	float *vertices;
	int remove;

	virtual void make_shader() = 0;
	virtual void set_shape() = 0;
	virtual void set_transform() = 0;

public:
	float x, y;
	float angle;
	float size;

	virtual void create_shader() = 0;
	virtual void update(float dt) = 0;
	virtual void render() const = 0;
	virtual int should_remove() = 0;
	virtual void do_remove() = 0;

	virtual void move(float x, float y) = 0;
	virtual void rotate(float angle) = 0;
};

#endif
