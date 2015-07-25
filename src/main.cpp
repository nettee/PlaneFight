#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <conio.h>

#include "defs.h"
#include "buffer.h"

void init_console();
int play_game();

State game_state = st_end;

Buffer buffer;

void do_welcome()
{
	buffer.clear();
	buffer.draw_string("PLANE FIGHT", 7);
	buffer.draw_string("version 2.6.3", 9);
	buffer.draw_string("Help poor children in Uganda!", 13);
	buffer.draw_string("A: left  D: right  W: up  S: down", 19);
	buffer.draw_string("Q: quit", 21);
	buffer.draw_string("press SPACE to start...", 24);
	buffer.display();
}

void do_encourage(int score) {
	buffer.clear();
	buffer.draw_string("GAME OVER!", 7);

	char s[20];
	sprintf(s, "Score: %d", score);
	buffer.draw_string(s, 10);

	buffer.draw_string("press SPACE to restart...", 24);
	buffer.draw_string("Q to quit.", 26);

	buffer.display();
}

int main()
{
	init_console();
	do_welcome();

	while (true) {

		while (!kbhit());
		char c = getch();

		if (c == 'q') {
			exit(0);
		} else if (c == ' ') {
			int score = play_game();
			do_encourage(score);
		}
	}

    return 0;
}


/* version history:
 *
 * 2.6.3 different colors
 *
 * 2.5.0 remove ~Buffer to reduce bug
 * 2.5.1 fix bug of macro flat
 *
 * 2.4.1 new enemy
 * 2.4.2 avoid strcpy when newing fly objects
 *
 * 2.3.0 make Plane subclass of Flyable
 * 2.3.1 fix bug of slogan
 * 2.3.2 release v2.3.2
 *
 * 2.2.0 new level system
 * 2.2.1 different enemy ratio
 * 2.2.2 new level up
 * 2.2.3 pause when going to level up (with bug)
 * 2.2.4 new slogan LEVEL UP
 * 2.2.5 make slogan display-able
 * 2.2.6 add level up slogan count-down
 * 2.2.7 make plane movable when slogan shows
 * 2.2.8 remove slogan when count-down over
 *
 * 2.1.0 change color scheme
 *
 * 2.0.0 new structure of checking collision
 * 2.0.1 different bullet power enabled
 * 2.0.3 remove unnecessary parameter and return of check_collision_with_plane
 *
 * 1.11.0 fix bug, supply fire_enhanced remains a problem
 * 1.11.1 move declaration to pool.h
 * 1.11.2 add area() info flyable
 * 1.11.3 fix overlap of enemy with supply
 *
 * 1.10.0 four basic supply
 * 1.10.1 add countdown for supply
 *
 * 1.9.0 stable
 * 1.9.1 add new supply $
 * 1.9.2 change parameter of check_collision()
 * 1.9.3 new effect() of supply
 * 1.9.4 unify check_collision_with_...()
 * 1.9.5 make effect() only for supply
 * 1.9.6 ammo_enhanced supply take effect
 * 1.9.7 make variable score static
 * 1.9.8 add double_score supply
 * 1.9.9 add triple_score supply
 * 1.9.10 print more plane status
 * 1.9.11 add fire_double supply
 *
 * 1.8.0 design game status // for game over
 * 1.8.1 improve method of adding heart
 * 1.8.2 now plane can die
 * 1.8.3 add Plane::init()
 * 1.8.4 add Pool::clear()
 * 1.8.5 now can game over
 * 1.8.6 use srand() to make better random number
 * 1.8.7 improve welcome message
 * 1.8.8 add game-over message
 * 1.8.9 add pause
 * 1.8.10 fix bug of CAPS-LOCK
 *
 * 1.7.0 add supply apple
 * 1.7.1 set apple shape
 * 1.7.2 set frequency of apple
 * 1.7.3 apple can be eaten by plane
 * 1.7.4 eating apple can add heart
 * 1.7.5 fix bug of redrawing enemy
 * 1.7.6 merge pool header files
 * 1.7.7 merge bullet, enemy, supply files with pool
 *
 * 1.6.0 can collide with enemy
 * 1.6.1 enemy will destroy (totally disappear) after collision
 * 1.6.2 heart decrease after collision
 * 1.6.3 add Area type
 * 1.6.4 use Area for bullet collision judgment
 * 1.6.5 use overlaps() instead of in()
 * 1.6.6 improvement and stability
 * 1.6.7 abandon in()
 *
 */
