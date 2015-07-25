#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "defs.h"
#include "buffer.h"
#include "plane.h"
#include "enemy.h"
#include "supply.h"
#include "pool.h"

bool tick();
int second();

extern Buffer buffer;

/* ===== key pressing related declaration ===== */

struct Key {
	char left, right, up, down;
	char space;
	char pause;
} key = {
		'a', 'd', 'w', 's',
		' ', 'p',
};

bool isKey(char ch, char key) {
	return tolower(ch) == tolower(key);
}

void pause() {
	while (true) {
		if (kbhit()) {
			char k = getch();
			if (isKey(k, key.pause) || isKey(k, key.space)) {
				return;
			}
		}
	}
}

/* ===== level related declaration ===== */

#define NR_LEVEL 18

struct LevelInfo {
	int enemy_ratio[NR_ENEMY];
	int speed;
} level_info[NR_LEVEL+1] = {
		{{0, 0, 0, 0, 0, 0, 0}, 0},  // empty entry
		{{6, 0, 3, 0, 1, 0, 0}, 1},
		{{3, 3, 3, 0, 1, 0, 0}, 1},
		{{2, 3, 3, 0, 2, 0, 0}, 1},
		{{1, 3, 3, 1, 2, 0, 0}, 2},
		{{0, 3, 3, 2, 2, 0, 0}, 2},
		{{0, 3, 2, 3, 2, 0, 0}, 2},

		{{0, 2, 2, 2, 4, 0, 0}, 3},
		{{0, 2, 1, 3, 4, 0, 0}, 3},
		{{0, 2, 0, 4, 4, 0, 0}, 3},
		{{0, 2, 0, 4, 3, 0, 1}, 3},
		{{0, 2, 0, 3, 3, 1, 1}, 3},
		{{0, 2, 0, 3, 2, 2, 1}, 3},
		{{0, 2, 0, 2, 3, 2, 1}, 3},
		{{0, 2, 0, 1, 3, 3, 1}, 3},

		{{0, 2, 0, 1, 2, 4, 1}, 3},
		{{0, 2, 0, 1, 2, 3, 2}, 3},
		{{0, 1, 0, 1, 2, 4, 2}, 3},
		{{0, 1, 0, 1, 0, 5, 2}, 3},
};

static int level;

#define LEVEL_GAP 20
#define LEVEL_UP_COUNTDOWN 25

static bool going_to_level_up;

static int level_up_countdown;

void level_up() {

}

/* ===== score related declaration ===== */

static int score;

void add_score(int incr) {
	if (plane.score_double) {
		score += incr * 2;
	} else {
		score += incr;
	}

	if (score > level * LEVEL_GAP && level < NR_LEVEL) {
		level = (score + LEVEL_GAP - 1) / LEVEL_GAP;
		going_to_level_up = true;
	}
}

/* =================================== */

Plane plane;
BulletPool bpool;
EnemyPool epool;
SupplyPool spool;

void init_game()
{
	plane.init();
	bpool.clear();
	epool.clear();
	spool.clear();

	srand(time(NULL));

	buffer.clear();
	plane.redraw();
	buffer.display();
	score = 0;
	level = 1;
	going_to_level_up = false;
	level_up_countdown = 0;
}

void print_score() {
	char s[20];
	sprintf(s, "Level %d  Score: %d", level, score);
	buffer.draw_string(s, 0, 0);
}

void print_plane_status() {
	char s[20];
	sprintf(s, "%c %c x%d",
			(plane.fire_double > 0) ? 'D' : ' ',
			(plane.fire_enhance > 0) ? 'E' : ' ',
			(plane.score_double > 0) ? 2 : 1);
	buffer.draw_string(s, 30, 0);

	for (int i = 0; i < max_life; i++) {
		if (i < plane.life) {
			buffer.draw_character('\3', scr_width-1 - i, 0);
		} else {
			buffer.draw_character(' ', scr_width-1 - i, 0);
		}
	}
}

void print_game_status() {
	print_score();
	print_plane_status();
}

int pick(int *ratios, int n) {
	int total = 0;
	for (int i = 0; i < n; i++) {
		total += ratios[i];
	}
	int r = rand() % total;
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += ratios[i];
		if (r < sum) {
			return i;
		}
	}
	return n-1;
}

void generate_enemy() {
	int type = pick(level_info[level].enemy_ratio, NR_ENEMY);
	enemy_info ei = enemy_list[type];
	int x = rand() % (scr_width - ei.width + 1);
	epool.add(x, top_y, type);
}

void generate_supply() {
	int type = rand() % NR_SUPPLY_TYPE;
	supply_info si = supply_list[type];
	int x = rand() % (scr_width - strlen(si.shape) + 1);
	spool.add(x, top_y, type);
}

void check_collision_enemy_bullet() {
	epool.check_collision_with_bullet();
}

void check_collision_plane_supply() {
	spool.check_collision_with_plane();
}

void check_collision_plane_enemy() {
	epool.check_collision_with_plane();
}

/**
 * @return score
 */
int play_game()
{
	init_game();

	while (true) {

		// watch keyboard hit
		if (kbhit()) {
			char k = getch();
			if (isKey(k, key.left)) {
				plane.move_left();
			} else if (isKey(k, key.right)) {
				plane.move_right();
			} else if (isKey(k, key.up)) {
				plane.move_up();
			} else if (isKey(k, key.down)) {
				plane.move_down();
			} else if (isKey(k, key.pause)) {
				pause();
			}
		}

		if (tick()) {

			if (level_up_countdown > 0) {
				level_up_countdown--;
				if (level_up_countdown == 0) {
					buffer.deslogan("LEVEL UP");
				}
			}

			// bullets go forward
			bpool.forward();
			check_collision_enemy_bullet();

			// enemy go forward
			if (second() % 2 == 0) {
				epool.forward();
				check_collision_enemy_bullet();
			}

			if (second() % 2 == 0) {
				spool.forward();
			}

			check_collision_plane_supply();
			check_collision_plane_enemy();
			plane.update();
			if (plane.isDead()) {
				return score;
			}

			if (second() % 43 == 0) {
				generate_supply();
			}
			else if (second() % 3 == 0 && abs(second() % 43) > 1) {
				generate_enemy();
			}

			plane.fire();

		}

		if (level_up_countdown > 0) {
			buffer.slogan("LEVEL UP");
		}
		plane.redraw();  // ensure the plane is always visual
		print_game_status();
		buffer.display();

		if (going_to_level_up) {
			level_up_countdown = LEVEL_UP_COUNTDOWN;
			going_to_level_up = false;
		}

	}
}
