#ifndef DEFS_H_
#define DEFS_H_

#define scr_height 31
#define scr_width 50
#define top_y 1

enum State {
	st_end,
	st_running,
	st_stop,
};

#define flat(x, y) ((x) + (y) * (scr_width))

// colors
#define C_NORMAL 41
#define C_SLOGAN 55
#define C_SUPPLY 44
#define C_HEART 45
#define C_BULLET 46

struct Area {
	int left, top, right, bottom;

	Area(int left, int top, int right, int bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	bool contains(int x, int y) {
		return (x >= left && x <= right)
				&& (y >= bottom && y <= top);
	}
};

#endif /* DEFS_H_ */
