
#ifndef _GAME_H_
#define _GAME_H_

class game;
#include "world.h"
#include "menu.h"
#include "creation.h"

enum game_state {playing, win, fail};
enum game_status {in_menu, in_game, in_creation};
enum game_daytime {day, night};
enum game_weather {sunny, rainy, snowy};
enum game_difficulty {easy, medium, hard};

class game {
	world w;
	menu m;
	creation c;
	int gamestatus, daytime, weather, difficulty, sensitivity;
	int campaign_status, maps_created;
	int mapchosen, userchosen;
	int esc_hit;
public:
	void init();
	void finish();
	bool update (float timediff, bool space_down, bool tab_down, bool esc_down, bool left_mouse_down, bool right_mouse_down, int mouse_x, int mouse_y, int mouse__x, int mouse__y);
	void render();
	void save_game();
	void save_user();
	void load_game();
	void read_user_info();
	void change_userchosen (int Userchosen);
	int get_userchosen();
	void change_mapchosen (int Mapchosen);
	int get_mapchosen();
	void change_sensitivity (int Sensitivity);
	int get_sensitivity();
	void change_daytime (int Daytime);
	int get_daytime();
	void change_weather (int Weather);
	int get_weather();
	void change_difficulty (int Difficulty);
	int get_difficulty();
	float get_min_timediff();
	void go_to_menu() {
		gamestatus = in_menu;
	}
	void go_to_game() {
		gamestatus = in_game;
		w.init (daytime, weather, difficulty);
	}
	void create_map() {
		gamestatus = in_creation;
	}
};

#endif
