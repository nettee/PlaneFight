#ifndef BULLET_H_
#define BULLET_H_

#include "flyable.h"

struct bullet_info {
	const char *shape;
	int harm;
};

extern bullet_info bullet_list[];

#define NR_BULLET_TYPE 2

enum bullet_type {
	bullet_common,
	bullet_enhanced,
};

class Bullet : public Flyable {
private:
	const int h_step, v_step;

public:
	int type;
	int harm;

public:
	Bullet(int x, int y, int type);

	void forward() {
		move_up();
	}

private:
	void move_up();
};

#endif /* BULLET_H_ */
