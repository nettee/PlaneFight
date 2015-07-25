#ifndef ENEMY_H_
#define ENEMY_H_

#include "flyable.h"
#include "bullet.h"

struct enemy_info {
	const char *shape;
	const int width;
	const int life;
	const int value;
};

#define NR_ENEMY 7

extern enemy_info enemy_list[];

class Enemy : public Flyable {
private:
	const int h_step, v_step;

public:
	int value;
	int life;

public:
	Enemy(int x, int y, int type);

	void forward() {
		move_down();
	}

private:
	void move_down();

public:
	void get_harm(Bullet *bullet);
};

#endif /* ENEMY_H_ */
