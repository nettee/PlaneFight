#include "bullet.h"
#include "pool.h"

#include <cstring>
#include <cassert>

#include "buffer.h"

bullet_info bullet_list[NR_BULLET_TYPE] = {
		{"'", 1},
		{";", 2},
};

Bullet::Bullet(int x, int y, int type): Flyable(x, y),
		h_step(1), v_step(1) {
	this->type = type;

	bullet_info bi = bullet_list[type];
	width = strlen(bi.shape);
	shape = (char *) bi.shape;
	color = C_BULLET;
	this->harm = bi.harm;
}

void Bullet::move_up() {
	assert(alive);
	hide();
	y = y - v_step;
	if (y >= 0) {
		show();
	} else {
		alive = false;
	}
}

void BulletPool::clear() {
	bullets.clear();
}

void BulletPool::add(int x, int y, int type) {
	bullets.push_back(Bullet(x, y, type));
}

void BulletPool::forward() {
	for (list_iter it = bullets.begin(); it != bullets.end(); ++it) {
		if (it->alive) {
			it->forward();
		} else {
			// remove inactive bullets
			it = bullets.erase(it);
		}
	}
}

void BulletPool::check_hitting_enemy(Enemy *enemy) {
	Area enemy_area = enemy->area();
	for (list_iter it = bullets.begin(); it != bullets.end(); ++it) {
		if (it->overlaps(enemy_area)) {
			enemy->get_harm(&(*it));
			it->destroy();
			it = bullets.erase(it);
		}
	}
}

/* Check if any bullet collides with the object who occupies the area
 * if there exists, erase the bullet
 * return bullet_type, -1 for no bullet
 */
bool BulletPool::check_collision_with_enemy(Enemy *enemy) {
	Area enemy_area = enemy->area();
	bool result = false;
	for (list_iter it = bullets.begin(); it != bullets.end(); ++it) {
		if (it->overlaps(enemy_area)) {
			it->destroy();
			it = bullets.erase(it);
			result = true;
		}
	}
	return result;
}
