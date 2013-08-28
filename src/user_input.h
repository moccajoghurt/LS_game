

void handle_key_event(SDL_KeyboardEvent* key, GAME_VARIABLES* game_variables, META_DATA* meta_data);
int check_spell(char key, GAME_VARIABLES* game_variables);

int check_keyboard_input(SDL_Event *event, GAME_VARIABLES* game_variables, META_DATA* meta_data) {
	
	while(SDL_PollEvent(event)) {
		
		switch (event->type) {
			
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handle_key_event(&event->key, game_variables, meta_data);
				break;
			default:
				break;
			
		}
		
		
	}
}


void handle_key_event(SDL_KeyboardEvent* key, GAME_VARIABLES* game_variables, META_DATA* meta_data) {
	
	if (meta_data->level_running) {
	
		if (key->type == SDL_KEYDOWN) {
			
			if (key->keysym.sym == SDLK_LEFT) {
				game_variables->move_left = 1;
				game_variables->move_right = 0;
				game_variables->direction_changed = 1;
				
			} else if (key->keysym.sym == SDLK_RIGHT) {
				game_variables->move_right = 1;
				game_variables->move_left = 0;
				game_variables->direction_changed = 1;
				
			} else if (key->keysym.sym == SDLK_UP) {
				game_variables->jumping = 1;
				
			} else if (key->keysym.sym == SDLK_r) {
				if (game_variables->is_casting == 0) {
					game_variables->is_casting = 1;
					game_variables->current_cast = 0;
				}
				
			} else if (key->keysym.sym == SDLK_ESCAPE) {
				meta_data->level_running = 0;
				meta_data->game_paused = 1;
				
			} else {
				
				if (game_variables->is_casting == 0) { 
					check_spell(key->keysym.sym, game_variables);
				}
			}
			
			
		} else if (key->type == SDL_KEYUP) {
			
			if (key->keysym.sym == SDLK_LEFT) {
				game_variables->move_left = 0;
				
			} else if (key->keysym.sym == SDLK_RIGHT) {
				game_variables->move_right = 0;
			}
			
		}
		
	} else if (meta_data->game_paused) {
		
		if (key->type == SDL_KEYDOWN) {
		
			if (key->keysym.sym == SDLK_ESCAPE) {
				
				meta_data->level_running = 1;
				meta_data->game_paused = 0;
				
			} else if (key->keysym.sym == SDLK_RETURN) {
				if (meta_data->pause_select_state == 0) {
					//main menu
					
				} else if (meta_data->pause_select_state == 1) {
					//music on / off
					
				} else if (meta_data->pause_select_state == 2) {
					meta_data->game_running = 0;
				}
				
			} else if (key->keysym.sym == SDLK_UP) {
				
				if (meta_data->pause_select_state == 1) {
					meta_data->pause_select_symbol_pos.y -= 40;
					meta_data->pause_select_state = 0;
					
				} else if (meta_data->pause_select_state == 2) {
					meta_data->pause_select_symbol_pos.y -= 40;
					meta_data->pause_select_state = 1;
				}
				
			} else if (key->keysym.sym == SDLK_DOWN) {
				
				if (meta_data->pause_select_state == 0) {
					meta_data->pause_select_symbol_pos.y += 40;
					meta_data->pause_select_state = 1;
					
				} else if (meta_data->pause_select_state == 1) {
					meta_data->pause_select_symbol_pos.y += 40;
					meta_data->pause_select_state = 2;
				}
				
			}
		}
	}
	
}


int check_spell(char key, GAME_VARIABLES* game_variables) {
	
	game_variables->spell_timer = 0;
	
	char *buf = calloc(1, 4);
	buf[0] = key;
	strncpy(buf + 1, game_variables->spell_input, 3);
	strcpy(game_variables->spell_input, buf);
	game_variables->spell_input[4] = '\0';
	free(buf);
	
	
	if (strlen(game_variables->spell_input) == 4) {
		
		if (strcmp(game_variables->player_name, "piccolo") == 0) {
			
			
			if (strcmp(game_variables->spell_input, "ewqq") == 0) {
				// finger of death
				game_variables->is_casting = 1;
				game_variables->current_cast = 1;
				
			} else if (strcmp(game_variables->spell_input, "asww") == 0) {
				// boden schredder
				game_variables->is_casting = 1;
				game_variables->current_cast = 2;
				
			} else if (strcmp(game_variables->spell_input, "edss") == 0) {
				// energie-ball regen
				game_variables->is_casting = 1;
				game_variables->current_cast = 3;
				
			} else if (strcmp(game_variables->spell_input, "aaaw") == 0) {
				// energie-schild
				game_variables->is_casting = 1;
				game_variables->current_cast = 4;
			}
			
		}

		
	}
}




