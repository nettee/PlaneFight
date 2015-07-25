#include <windows.h>

static int sec = 0;

const int time_count = 37;

bool tick() {
	static int count = 0;
	Sleep(1);
	if (count == time_count) {
		count = 0;
		sec++;
		return true;
	} else {
		count += 1;
		return false;
	}
}

int second() {
	return sec;
}

