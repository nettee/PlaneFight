#include "buffer.h"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <windows.h>

#include "defs.h"

HANDLE writeHandle;

void init_console()
{
	writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTitle(TEXT("Plane Fight"));

	SMALL_RECT windowSize = {0, 0, (short)(scr_width-1), (short)(scr_height-1)};
	SetConsoleWindowInfo(writeHandle, TRUE, &windowSize);

	COORD bufferSize = {scr_width, (short)(scr_height)};
	SetConsoleScreenBufferSize(writeHandle, bufferSize);
}


Buffer::Buffer() {
	a = new char[scr_height * scr_width];
	c = new char[scr_height * scr_width];
	clear();
}

void Buffer::clear() {
	for (int y = 0; y < scr_height; y++) {
		for (int x = 0; x < scr_width; x++) {
			a[flat(x, y)] = ' ';
			c[flat(x, y)] = C_NORMAL;
		}
	}
}

void Buffer::draw_character(char ch, int x, int y) {
	assert(x >= 0 && x < scr_width);
	assert(y >= 0 && y < scr_height);
	a[flat(x, y)] = ch;
}

void Buffer::draw_color(char color, int x, int y) {
	assert(x >= 0 && x < scr_width);
	assert(y >= 0 && y < scr_height);
	c[flat(x, y)] = color;
}

void Buffer::draw_string(const char *str, int x, int y, char color) {
	assert(x >= 0 && x < scr_width);
	assert(y >= 0 && y < scr_height);
	for (int i = 0; str[i] != '\0'; i++) {
		draw_character(str[i], x + i, y);
		draw_color(color, x + i, y);
	}
}

void Buffer::draw_string(const char *str, int x, int y) {
	draw_string(str, x, y, C_NORMAL);
}

void Buffer::draw_string(const char *str, int y) {
	int x = (scr_width - strlen(str)) / 2;
	draw_string(str, x, y);
}

void Buffer::slogan(const char *str, int y) {
	int N = strlen(str);
	int start = (scr_width - N) / 2;

	for (int x = 0; x < scr_width; x++) {
		a[flat(x, y-1)] = ' ';
		a[flat(x, y+1)] = ' ';
		c[flat(x, y-1)] = C_SLOGAN;
		c[flat(x, y)] = C_SLOGAN;
		c[flat(x, y+1)] = C_SLOGAN;
		if (x >= start && x < start + N) {
			a[flat(x, y)] = str[x-start];
		} else {
			a[flat(x, y)] = ' ';
		}
	}
}

void Buffer::slogan(const char *str) {
	slogan(str, scr_height / 3);
}

void Buffer::deslogan(const char *str, int y) {
	int N = strlen(str);
	int start = (scr_width - N) / 2;

	for (int x = 0; x < scr_width; x++) {
		c[flat(x, y-1)] = C_NORMAL;
		c[flat(x, y)] = C_NORMAL;
		c[flat(x, y+1)] = C_NORMAL;
	}
	for (int i = 0; i < N; i++) {
		int x = start + i;
		a[flat(x, y)] = ' ';
	}
}

void Buffer::deslogan(const char *str) {
	deslogan(str, scr_height / 3);
}

void Buffer::display() {
	CHAR_INFO chars[scr_width * scr_height];
	for (short y = 0; y < scr_height; y++) {
		for (short x = 0; x < scr_width; x++) {
			chars[flat(x, y)].Char.AsciiChar = a[flat(x, y)];
			if (c[flat(x, y)] == C_SLOGAN) {
				chars[flat(x, y)].Attributes = FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
			} else if (c[flat(x, y)] == C_SUPPLY) {
				chars[flat(x, y)].Attributes = FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
			} else if (c[flat(x, y)] == C_HEART) {
				chars[flat(x, y)].Attributes = FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
			} else if (c[flat(x, y)] == C_BULLET) {
				chars[flat(x, y)].Attributes = FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
			} else {
				chars[flat(x, y)].Attributes = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
			}
		}
	}
	COORD charBufSize = {scr_width, scr_height};
	COORD charPosition = {0, 0};
	SMALL_RECT writeArea = {0, 0, (short)(scr_width-1), (short)(scr_height-1)};
	WriteConsoleOutput(writeHandle, chars, charBufSize, charPosition, &writeArea);
}


