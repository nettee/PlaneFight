#ifndef BUFFER_H_
#define BUFFER_H_

#include "defs.h"

class Buffer {
private:
	char *a;  // character
	char *c;  // color

public:
	Buffer();
	void clear();

public:
	void draw_character(char ch, int row_index, int col_index);
	void draw_color(char color, int row_index, int col_index);
	void draw_string(const char *str, int row_index, int col_index, char color);
	void draw_string(const char *str, int row_index, int col_index);
	void draw_string(const char *str, int row_index);

	void slogan(const char *str, int row_index);
	void slogan(const char *str);
	void deslogan(const char *str, int row_index);
	void deslogan(const char *str);

	void display();
};

#endif /* BUFFER_H_ */
