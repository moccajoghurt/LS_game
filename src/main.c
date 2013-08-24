/*
	TODO: ebl1 explosion und direction_left bei spell mit eb

*/

#include <SDL/SDL.h>
#include <stdio.h>
typedef unsigned int UINT;
typedef unsigned char UCHAR;
#include "datatypes.h"
#include "collision.h"
#include "init.h"
#include "gameplay.h"
#include "user_input.h"


int WinMain(int argc, char** argv) {
	
	
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface* display =  SDL_SetVideoMode(1307, 600, 32,SDL_HWSURFACE /*SDL_FULLSCREEN*/);
	
	GAME_TIMER* timer = calloc(1, sizeof(GAME_TIMER));
	init_game_timer(timer);
	
	META_STATS* meta_stats = calloc(1, sizeof(META_STATS));
	init_meta_stats(meta_stats);
	
	PLAYER* player = create_player("piccolo", meta_stats);
	GAME_VARIABLES* game_variables = calloc(1, sizeof(GAME_VARIABLES));
	init_game_variables(game_variables, "piccolo");
	
	EFFECT_MODEL_LIST* effect_models = calloc(1, sizeof(EFFECT_MODEL_LIST));
	init_effects(effect_models);
	
	GAME_MODEL_LISTS *game_models = calloc(1, sizeof(GAME_MODEL_LISTS));
	init_game_model_lists(game_models);
	
	CURRENT_EFFECTS* effects = calloc(1, sizeof(CURRENT_EFFECTS));
	
	CURRENT_ENEMIES* enemies = calloc(1, sizeof(CURRENT_ENEMIES));
	
	STATIC_MODELS* static_models = calloc(1, sizeof(STATIC_MODELS));
	init_static_models(static_models, display);
	
	
	while (game_variables->game_running) {
		handle_game_time(timer);
		enemy_creation(timer, enemies, game_models, player);
		draw_background(display, static_models);
		check_keyboard_input(&event, game_variables);
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
		handle_enemy_death(enemies);
		draw_model(player->current_model, player->position, display);
		draw_enemies(enemies, display);
		draw_effects(effects, display);
		draw_health_bar(player, static_models, meta_stats, display);
		
		
		SDL_Flip(display);
		SDL_Delay(10);
	}
	
	
	SDL_Quit();
	
}
