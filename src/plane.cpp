#include "plane.h"

#include <cassert>
#include <cstring>

#include "defs.h"
#include "pool.h"

#define init_x scr_width / 2
#define init_y scr_height * 4 / 5
const char *plane_shape = "^_^";

Plane::Plane(): Flyable(init_x, init_y),
		h_step(2), v_step(1) {
	width = strlen(plane_shape);
	shape = new char[width+1];
	strcpy(shape, plane_shape);
	init();
}

void Plane::init() {
	life = 3;
	fire_double = 0;
	fire_enhance = 0;
	score_double = 0;
}

void Plane::move_left() {
	hide();
	if (x - h_step >= 0) {
		x = x - h_step;
	}
	show();
}

void Plane::move_right() {
	hide();
	if ((x + width - 1) + h_step <= scr_width - 1) {
		x = x + h_step;
	}
	show();
}

void Plane::move_up() {
	hide();
	if (y - v_step >= 1) {
		y = y - v_step;
	}
	show();
}

void Plane::move_down() {
	hide();
	if (y + v_step <= scr_height - 1) {
		y = y + v_step;
	}
	show();
}

void Plane::fire() {
	int type = (fire_enhance > 0) ? bullet_enhanced : bullet_common;
	if (fire_double > 0) {
		bpool.add(x, y-1, type);
		bpool.add(x+2, y-1, type);
	} else {
		bpool.add(x+1, y-1, type);
	}
}

void Plane::update() {
	if (fire_double > 0) fire_double--;
	if (fire_enhance > 0) fire_enhance--;
	if (score_double > 0) score_double--;
}
