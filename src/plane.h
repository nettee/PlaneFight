#ifndef PLANE_H_
#define PLANE_H_

#include "defs.h"
#include "buffer.h"
#include "flyable.h"

extern Buffer buffer;

#define max_life 9

class Plane : public Flyable {
private:
	const int h_step, v_step;

public:
	int life;
	int fire_double;
	int fire_enhance;
	int score_double;

public:
	Plane();
	void init();

public:
	bool isDead() { return life == 0; }
	void increase_life() {
		if (life < max_life) life++;
	}
	void decrease_life() {
		if (life > 0) life--;
	}

public:
	void move_left();
	void move_right();
	void move_up();
	void move_down();

	void fire();
	void update();

private:
	bool hit_enemy();
};

#endif /* PLANE_H_ */
