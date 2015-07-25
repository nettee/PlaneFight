#include "enemy.h"
#include "pool.h"

#include <cstring>
#include <cassert>

#include "defs.h"
#include "buffer.h"


void add_score(int incr);

enemy_info enemy_list[NR_ENEMY] = {
		{"-.-", 3, 4, 1},
		{"=.=", 3, 5, 1},
		{"(v.v)", 5, 10, 2},
		{"(w.w)", 5, 19, 3},
		{"=-[o_o]-=", 9, 29, 5},
		{"=-[@_@]-=", 9, 33, 7},
		{"0.0", 3, 37, 9},
};

Enemy::Enemy(int x, int y, int type): Flyable(x, y),
		h_step(1), v_step(1) {
	enemy_info ei = enemy_list[type];
	width = ei.width;
	shape = (char *) ei.shape;
	life = ei.life;
	value = ei.value;
}

void Enemy::move_down() {
	assert(alive);
	hide();
	y = y + v_step;
	if (y < scr_height) {
		show();
	} else {
		alive = false;
	}
}

void Enemy::get_harm(Bullet *bullet) {
	this->life -= bullet->harm;
}

void EnemyPool::clear() {
	enemies.clear();
}

void EnemyPool::add(int x, int y, int type) {
	enemies.push_back(Enemy(x, y, type));
}

void EnemyPool::forward() {
	for (list_iter it = enemies.begin(); it != enemies.end(); ++it) {
		if (it->alive) {
			it->forward();
		} else {
			// remove out-of-screen enemies
			it = enemies.erase(it);
		}
	}
}

void EnemyPool::check_collision_with_bullet() {
	for (list_iter it = enemies.begin(); it != enemies.end(); ++it) {
		bpool.check_hitting_enemy(&(*it));
		if (it->life > 0) {
			it->redraw();  // ensure enemy is wholly displayed
		} else {
			it->destroy();
			add_score(it->value);
			it = enemies.erase(it);
		}
	}
}

void EnemyPool::check_collision_with_plane() {
	Area plane_area = plane.area();
	for (list_iter it = enemies.begin(); it != enemies.end(); ++it) {
		if (it->overlaps(plane_area)) {
			it->destroy();
			add_score(it->value);
			plane.decrease_life();
			it = enemies.erase(it);
		}
	}
}
