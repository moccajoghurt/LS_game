#include <SDL/SDL.h>
#include <stdio.h>
typedef unsigned int UINT;
typedef unsigned char UCHAR;
#include "datatypes.h"
#include "collision.h"
#include "init.h"
#include "gameplay.h"
#include "user_input.h"
#include "text_drawing.h"
#include "sequences.h"


int WinMain(int argc, char** argv) {
	
	
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface* display =  SDL_SetVideoMode(1307, 600, 32,SDL_HWSURFACE /*SDL_FULLSCREEN*/);
	
	GAME_TIMER* timer = calloc(1, sizeof(GAME_TIMER));
	init_game_timer(timer);
	
	META_DATA* meta_data = calloc(1, sizeof(META_DATA));
	init_meta_data(meta_data);
	
	FONT_LIST* font_list = calloc(1, sizeof(FONT_LIST));
	init_font_list("../res/font/font", font_list);
	
	STATIC_POSITIONS* static_positions = calloc(1, sizeof(STATIC_POSITIONS));
	init_static_positions(static_positions);
	
	PLAYER* player = create_player("PICCOLO", meta_data);
	GAME_VARIABLES* game_variables = calloc(1, sizeof(GAME_VARIABLES));
	init_game_variables(game_variables, "PICCOLO");
	
	EFFECT_MODEL_LIST* effect_models = calloc(1, sizeof(EFFECT_MODEL_LIST));
	init_effects(effect_models);
	
	GAME_MODEL_LISTS *game_models = calloc(1, sizeof(GAME_MODEL_LISTS));
	init_game_model_lists(game_models);
	
	CURRENT_EFFECTS* effects = calloc(1, sizeof(CURRENT_EFFECTS));
	
	CURRENT_ENEMIES* enemies = calloc(1, sizeof(CURRENT_ENEMIES));
	
	STATIC_MODELS* static_models = calloc(1, sizeof(STATIC_MODELS));
	init_static_models(static_models, display);
	
	while (meta_data->game_running) {
		
		if (meta_data->level_running) {
			check_keyboard_input(&event, game_variables, meta_data);
			handle_game_time(timer);
			enemy_creation(timer, enemies, game_models, player);
			draw_background(display, static_models);
			spell_timer(game_variables);
			handle_casting(game_variables, player, effects, effect_models, game_models);
			handle_std_attack(game_variables, player);
			move_effects(effects, game_models);
			handle_anim_effects(effects);
			move_player(game_variables, player, display);
			move_enemies(enemies, player);
			handle_enemy_attack(enemies, player, effects, effect_models, game_models);
			check_melee_attack_collision(game_variables, player, enemies);
			check_spell_collisions(effects, enemies, player, game_models, effect_models);
			check_enemy_effects_collision(player, effects);
			check_enemy_melee_collision(game_variables, player, enemies);
			handle_enemy_death(enemies, meta_data);
			handle_player_harm_cd(player);
			draw_model(player->current_model, player->position, display);
			draw_enemies(enemies, display);
			draw_effects(effects, display);
			draw_health_bar(player, static_models, meta_data, display);
			draw_meta_data(font_list, player, meta_data, display);
			check_level_progress(meta_data);
			if (meta_data->level_running == 0)
				clean_gameplay_memory(effects, enemies);
			
			
		} else if (meta_data->game_paused) {
			check_keyboard_input(&event, game_variables, meta_data);
			draw_background(display, static_models);
			draw_model(player->current_model, player->position, display);
			draw_enemies(enemies, display);
			draw_effects(effects, display);
			draw_health_bar(player, static_models, meta_data, display);
			draw_pause_screen(static_models->pause_shade, display);
			draw_text("PAUSE", font_list, static_positions->pause_text_pos, display);
			draw_text("MAIN MENU", font_list, static_positions->main_menu_pos, display);
			draw_text("MUSIC ON", font_list, static_positions->music_pos, display);
			draw_text("EXIT GAME", font_list, static_positions->exit_pos, display);
			draw_model(static_models->select_symbol, meta_data->pause_select_symbol_pos, display);
			
		} else if (meta_data->level_intro) {
			
			draw_background(display, static_models);
			draw_pause_screen(static_models->pause_shade, display);
			handle_level_intro(meta_data, font_list, static_positions, timer, display);
			
		} else if (meta_data->level_completed) {
			
			meta_data->level_running = 1;
			//meta_data->game_running = 0;
		}
		
		SDL_Flip(display);
		SDL_Delay(10);
	}
	
	
	SDL_Quit();
	
}
