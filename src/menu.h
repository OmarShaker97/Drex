
#include "game.h"

#ifndef _MENU_H_
#define _MENU_H_

#include <vector>
#include <string>

using namespace std;

#include "../vendor/OGLFT.h"

class menu {
	OGLFT::TranslucentTexture *face, *face2, *face3;
	int menustatus;
	int cursor_pos;
	int left_mouse_hit;
	vector<string> items;
public:
	void init();
	bool update(float timediff,bool esc_down,bool left_mouse_down,bool right_mouse_down,int mouse_x,int mouse_y,game& g);
	void render();
	bool handle_menu_click(int item,game& g);
	void set_menu(int newstatus);
};

#endif

