
MODEL_LIST *create_model_list(const char *base_filename, const int model_count) {

	MODEL_LIST *model_list = calloc(1, sizeof(MODEL_LIST));
	MODEL_LIST *start_model = model_list;


	for (int i = 0; i < model_count; i++) {

		char *filename = calloc(1, 100);
		char num[3];
		sprintf(num, "%d", i);
		strcpy(filename, base_filename);
		strcat(filename, num);
		strcat(filename, ".bmp");
		SDL_Surface *bmp = SDL_LoadBMP(filename);

		if (bmp == NULL) {
			puts("couldn't load BMP file:");
			puts(filename);
			exit(1);
		}

		while (start_model->next != NULL) {
			start_model = start_model->next;
		}
		start_model->next = calloc(1, sizeof(MODEL_LIST));
		start_model->first = model_list;

		start_model->model = SDL_DisplayFormat(bmp);
		SDL_SetColorKey(start_model->model, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(start_model->model->format, 0, 0, 0));
		free(filename);
		SDL_FreeSurface(bmp);
	}

	return model_list;
}


PLAYER *create_player(const char *char_name, META_STATS* meta_stats) {

	PLAYER *player = calloc(1, sizeof(PLAYER));

	if (strcmp(char_name, "piccolo") == 0) {


		player->walk_right = create_model_list("../res/piccolo/wr", 5);
		player->walk_left = create_model_list("../res/piccolo/wl", 5);
		player->jump_right = create_model_list("../res/piccolo/jr", 1);
		player->jump_left = create_model_list("../res/piccolo/jl", 1);
		player->spell_0 = create_model_list("../res/piccolo/sp0_", 3);
		player->spell_1 = create_model_list("../res/piccolo/sp1_", 4);
		player->spell_2 = create_model_list("../res/piccolo/sp2_", 4);
		player->spell_3 = create_model_list("../res/piccolo/sp3_", 12);
		player->spell_4 = create_model_list("../res/piccolo/sp4_", 3);
		player->spell_0_left = create_model_list("../res/piccolo/sp0_l", 3);
		player->spell_1_left = create_model_list("../res/piccolo/sp1_l", 4);
		player->spell_2_left = create_model_list("../res/piccolo/sp2_l", 4);
		player->spell_3_left = create_model_list("../res/piccolo/sp3_l", 12);
		player->spell_4_left = create_model_list("../res/piccolo/sp4_l", 3);
		player->death = create_model_list("../res/piccolo/death", 2);
		player->death_left = create_model_list("../res/piccolo/deathl", 2);
		player->got_hit = create_model_list("../res/piccolo/got_hit", 1);
		player->got_hit_left = create_model_list("../res/piccolo/got_hitl", 1);


		player->health = 25 + meta_stats->level*25;
		player->position.x = 500;
		player->position.y = 310;
		player->position.h = 150;
		player->position.w = 161;

		player->current_model = player->walk_right->model;

		player->walk_intervall = 8;
		player->walk_count = 0;

		player->direction_left = 0;

		player->spell_timer_count = 0;
		
		player->harm_cd = 0;
		
		player->name = calloc(1, strlen("piccolo") + 1);
		strcpy(player->name, "piccolo");



	} else if (strcmp(char_name,"warlock") == 0) {

		player->walk_right = create_model_list("../res/piccolo/wr", 5);
		player->walk_left = create_model_list("../res/piccolo/wl", 5);
		player->jump_right = create_model_list("../res/piccolo/jr", 1);
		player->jump_left = create_model_list("../res/piccolo/jl", 1);
		player->spell_0 = create_model_list("../res/piccolo/sp0_", 3);
		player->spell_1 = create_model_list("../res/piccolo/sp1_", 4);
		player->spell_2 = create_model_list("../res/piccolo/sp2_", 4);
		player->spell_3 = create_model_list("../res/piccolo/sp3_", 12);
		player->spell_4 = create_model_list("../res/piccolo/sp4_", 3);
		player->spell_0_left = create_model_list("../res/piccolo/sp0_l", 3);
		player->spell_1_left = create_model_list("../res/piccolo/sp1_l", 4);
		player->spell_2_left = create_model_list("../res/piccolo/sp2_l", 4);
		player->spell_3_left = create_model_list("../res/piccolo/sp3_l", 12);
		player->spell_4_left = create_model_list("../res/piccolo/sp4_l", 3);
		player->death = create_model_list("../res/piccolo/death", 2);
		player->death_left = create_model_list("../res/piccolo/deathl", 2);
		player->got_hit = create_model_list("../res/piccolo/got_hit", 1);
		player->got_hit_left = create_model_list("../res/piccolo/got_hitl", 1);


		player->health = 25 + meta_stats->level*25;
		player->position.x = 100;
		player->position.y = 310;
		player->position.h = 150;
		player->position.w = 161;

		player->current_model = player->walk_right->model;

		player->walk_intervall = 8;
		player->walk_count = 0;

		player->direction_left = 0;

		player->spell_timer_count = 0;
		
		player->harm_cd = 0;
		
		player->name = calloc(1, strlen("warlock") + 1);
		strcpy(player->name, "warlock");


	} else {
		puts("unknown char");
		exit(1);
	}

	return player;
}

void init_game_variables(GAME_VARIABLES *game_variables, char *player_name) {
	game_variables->spell_input = calloc(1,5);
	game_variables->spell_timer = 0;
	game_variables->game_running = 1;
	game_variables->move_left = 0;
	game_variables->move_right = 0;
	game_variables->jumping= 0;
	game_variables->jump_counter = 0;
	game_variables->direction_changed = 0;
	game_variables->current_cast = -1;

	game_variables->player_name = calloc(1, strlen(player_name) + 1);
	strcpy(game_variables->player_name, player_name);
}

void init_static_models(STATIC_MODELS *static_models, SDL_Surface* display) {

	SDL_Surface *bg_src = SDL_LoadBMP("../res/background.bmp");
	static_models->background = SDL_DisplayFormat(bg_src);
	
	SDL_Surface *health_src = SDL_LoadBMP("../res/healthbar.bmp");
	static_models->health_bar_frame = SDL_DisplayFormat(health_src);
	static_models->health_bar_frame_pos.x = 20;
	static_models->health_bar_frame_pos.y = 20;
	SDL_SetColorKey(static_models->health_bar_frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(static_models->health_bar_frame->format, 0, 0, 0));
	
	static_models->health_bar.x = 24;
	static_models->health_bar.y = 24;
	
	static_models->health_bar.w = 294;
	static_models->health_bar.h = 23;
	
	static_models->health_bar_color = SDL_MapRGB(display->format, 237, 27, 36);
	
	static_models->health_bar_max_width = 294;

}

void add_effect(EFFECT_MODEL_LIST *first, const char *filename, const char *spell_name) {

	EFFECT_MODEL_LIST *start = first;

	while (start->next != NULL) {
		start = start->next;
	}

	start->first = first;
	start->next = calloc(1, sizeof(EFFECT_MODEL_LIST));

	start->effect_name = calloc(1, strlen(spell_name) + 1);
	strcpy(start->effect_name, spell_name);


	SDL_Surface *bmp = SDL_LoadBMP(filename);
	if (bmp == NULL) {
		puts("couldn't load BMP file:");
		puts(filename);
		exit(1);
	}

	start->model = SDL_DisplayFormat(bmp);
	SDL_SetColorKey(start->model, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(start->model->format, 0, 0, 0));
	SDL_FreeSurface(bmp);
}

void init_effects(EFFECT_MODEL_LIST *start) {

	add_effect(start, "../res/piccolo/spells/eb0.bmp", "eb0");
	add_effect(start, "../res/piccolo/spells/eb1.bmp", "eb1");
	add_effect(start, "../res/piccolo/spells/ebl0.bmp", "ebl0");
	add_effect(start, "../res/piccolo/spells/ebl1.bmp", "ebl1");
	add_effect(start, "../res/piccolo/spells/fod.bmp", "fod");

}


void add_game_model_list(GAME_MODEL_LISTS *game_models, const char *filename, int filenum, const char *model_name) {
	

	while (game_models->next != NULL) {
		game_models = game_models->next;
	}
	
	game_models->name = calloc(1, strlen(model_name));
	strcpy(game_models->name, model_name);
	game_models->models = create_model_list(filename, filenum);
	game_models->next = calloc(1, sizeof(GAME_MODEL_LISTS));
}

void init_game_model_lists(GAME_MODEL_LISTS *game_models) {
	
	//explosions
	add_game_model_list(game_models, "../res/explosions/exp", 26, "exp");
	add_game_model_list(game_models, "../res/explosions/small_exp", 6, "small_exp");
	
	//mummy
	add_game_model_list(game_models, "../res/mummy/attack", 28, "mummy_atk");
	add_game_model_list(game_models, "../res/mummy/attackl", 28, "mummy_atk_left");
	add_game_model_list(game_models, "../res/mummy/breath", 16, "mummy_breath");
	add_game_model_list(game_models, "../res/mummy/breathl", 16, "mummy_breath_left");
	add_game_model_list(game_models, "../res/mummy/death_exp", 12, "mummy_death_exp");
	add_game_model_list(game_models, "../res/mummy/death_expl", 12, "mummy_death_exp_left");
	add_game_model_list(game_models, "../res/mummy/death_normal", 45, "mummy_death_normal");
	add_game_model_list(game_models, "../res/mummy/death_normall", 45, "mummy_death_normal_left");
	add_game_model_list(game_models, "../res/mummy/walk", 18, "mummy_walk");
	add_game_model_list(game_models, "../res/mummy/walkl", 18, "mummy_walk_left");
	add_game_model_list(game_models, "../res/mummy/breath", 16, "mummy_breath");
	add_game_model_list(game_models, "../res/mummy/breathl", 16, "mummy_breath_left");
	
	//arab
	add_game_model_list(game_models, "../res/arab/attack", 14, "arab_atk");
	add_game_model_list(game_models, "../res/arab/attackl", 14, "arab_atk_left");
	add_game_model_list(game_models, "../res/arab/death", 10, "arab_death");
	add_game_model_list(game_models, "../res/arab/walk", 4, "arab_walk");
	add_game_model_list(game_models, "../res/arab/walkl", 4, "arab_walk_left");
	
}

void init_game_timer(GAME_TIMER* timer) {
	timer->seconds_played = 0;
	timer->enemy_spawn_timer = 200;
	timer->counter = 0;
	
}


void init_meta_stats(META_STATS* meta_stats) {
	
	//read file here
	meta_stats->experience = 0;
	meta_stats->level = 1;
}

