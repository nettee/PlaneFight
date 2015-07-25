#ifndef POOL_H_
#define POOL_H_

#include <list>

#include "defs.h"
#include "bullet.h"
#include "enemy.h"
#include "supply.h"
#include "plane.h"

extern Plane plane;

class BulletPool {
private:
	std::list<Bullet> bullets;
	typedef std::list<Bullet>::iterator list_iter;

public:
	void clear();
	void add(int x, int y, int type);
	void forward();
	void check_hitting_enemy(Enemy *enemy);
	bool check_collision_with_enemy(Enemy *enemy);
};

class EnemyPool {
private:
	std::list<Enemy> enemies;
	typedef std::list<Enemy>::iterator list_iter;

public:
	void clear();
	void add(int x, int y, int type);
	void forward();
	void check_collision_with_bullet();
	void check_collision_with_plane();
};

class SupplyPool {
private:
	std::list<Supply> supplies;
	typedef std::list<Supply>::iterator list_iter;

public:
	void clear();
	void add(int x, int y, int type);
	void forward();
	void check_collision_with_plane();
};

extern BulletPool bpool;
extern EnemyPool epool;
extern SupplyPool spool;

#endif /* POOL_H_ */
