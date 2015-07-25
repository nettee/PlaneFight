#include "supply.h"
#include "pool.h"

#include <cstring>
#include <cassert>

#include "plane.h"

supply_info supply_list[NR_SUPPLY_TYPE] = {
		{"\3", C_HEART},
		{"$$", C_SUPPLY},
		{"%%", C_SUPPLY},
		{"CC", C_SUPPLY},
};

Supply::Supply(int x, int y, int type): Flyable(x, y),
		h_step(1), v_step(1) {
	supply_info si = supply_list[type];
	width = strlen(si.shape);
	shape = (char *) si.shape;
	color = si.color;
	this->type = type;
}

void Supply::move_down() {
	assert(alive);
	hide();
	y = y + v_step;
	if (y < scr_height) {
		show();
	} else {
		alive = false;
	}
}

void Supply::effect() {
	if (type == supply_apple) {
		plane.increase_life();
	} else if (type == supply_fire_double) {
		plane.fire_double += 240;
	} else if (type == supply_fire_enhance) {
		plane.fire_enhance += 240;
	} else if (type == supply_score_double) {
		plane.score_double += 240;
	} else {
		assert(!"No such supply type");
	}
}

void SupplyPool::clear() {
	supplies.clear();
}

void SupplyPool::add(int x, int y, int type) {
	supplies.push_back(Supply(x, y, type));
}

void SupplyPool::forward() {
	for (list_iter it = supplies.begin(); it != supplies.end(); ++it) {
		if (it->alive) {
			it->forward();
		} else {
			// remove inactive supplies
			it = supplies.erase(it);
		}
	}
}

void SupplyPool::check_collision_with_plane() {
	Area plane_area = plane.area();
	for (list_iter it = supplies.begin(); it != supplies.end(); ++it) {
		if (it->overlaps(plane_area)) {
			it->effect();
			it->destroy();
			it = supplies.erase(it);
		}
	}
}
