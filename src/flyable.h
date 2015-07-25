#ifndef FLYABLE_H_
#define FLYABLE_H_

#include <cstdio>

#include "defs.h"
#include "buffer.h"

extern Buffer buffer;

class Flyable {
protected:
	int x, y;
	char *shape;
	int width;
	char color;

public:
	bool alive;

public:
	// must be initialized by coordinates
	Flyable(int x, int y): shape(NULL), width(0), color(C_NORMAL) {
		this->x = x;
		this->y = y;
		alive = true;
	}

public:
	Area area() {
		return Area(x, y, x+width-1, y);
	}

	bool overlaps(Area area) {
		for (int i = 0; i < width; i++) {
			if (area.contains(x + i, y)) {
				return true;
			}
		}
		return false;
	}

protected:
	void hide() {
		char *blanks = new char[width+1];
		for (int i = 0; i < width; i++) {
			blanks[i] = ' ';
		}
		blanks[width] = '\0';
		buffer.draw_string(blanks, x, y, C_NORMAL);
	}

	void show() {
		buffer.draw_string(shape, x, y, color);
	}

public:
	void redraw() {
		if (alive) {
			show();
		}
	}

	void forward();

	void destroy() {
		hide();
	}
};

#endif /* FLYABLE_H_ */
