#ifndef SUPPLY_H_
#define SUPPLY_H_

#include "defs.h"
#include "flyable.h"

struct supply_info {
	const char *shape;
	char color;
};

extern supply_info supply_list[];

#define NR_SUPPLY_TYPE 4

enum supply_type {
		supply_apple,
		supply_fire_double,
		supply_fire_enhance,
		supply_score_double,
};

class Supply : public Flyable {
private:
	const int h_step, v_step;
	int type;

public:
	Supply(int x, int y, int type);

	void forward() {
		move_down();
	}

private:
	void move_down();

public:
	void effect();
};

#endif /* SUPPLY_H_ */
