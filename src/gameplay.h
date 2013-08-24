

SDL_Surface *get_effect_model(const char *effect_name, EFFECT_MODEL_LIST *effects) {

	while (effects->next != NULL) {

		if (strcmp(effects->effect_name, effect_name) == 0) {
			return effects->model;
		}
		effects = effects->next;
	}
	return NULL;
}

MODEL_LIST* get_game_model_list(GAME_MODEL_LISTS *game_models, const char* name) {
	
	while(game_models->next != NULL) {
		
		if (strcmp(game_models->name, name) == 0)
			return game_models->models;
		
		
		game_models = game_models->next;
	}
	return NULL;
}

CURRENT_EFFECTS* create_effect(CURRENT_EFFECTS *effects, SDL_Rect position, int target_x, int target_y, char *harms_target, STATE is_moving, int move_speed, const char *effect_name, EFFECT_MODEL_LIST *effect_models, GAME_MODEL_LISTS *game_models, int anim_intervall, STATE is_loop_anim) {

	while (effects->next != NULL) {
		effects = effects->next;
	}
	
	effect_id_count++;
	
	effects->next = calloc(1, sizeof(CURRENT_EFFECTS));
	effects->next->before = effects;

	EFFECT_OBJECT *effect_object = calloc(1, sizeof(EFFECT_OBJECT));
	effect_object->position.x = position.x;
	effect_object->position.y = position.y;

	effect_object->target_x = target_x;
	effect_object->target_y = target_y;

	effect_object->harms_target = calloc(1, strlen(harms_target) + 1);
	strcpy(effect_object->harms_target, harms_target);

	effect_object->is_moving = is_moving;
	effect_object->move_speed = move_speed;

	effect_object->effect_name = calloc(1, strlen(effect_name) + 1);
	strcpy(effect_object->effect_name, effect_name);
	
	if (strcmp(effect_name, "eb0") == 0) {
		effect_object->damage = 50;
	}
	
	if (effect_models != NULL) {
		effect_object->model = get_effect_model(effect_name, effect_models);
		effect_object->is_animated = 0;
		
	} else {
		
		effect_object->model_list = get_game_model_list(game_models, effect_name);
		effect_object->model = effect_object->model_list->model;
		effect_object->anim_intervall = anim_intervall;
		effect_object->is_animated = 1;
		effect_object->is_loop_anim = is_loop_anim;
		
	}
	
	effect_object->position.w = effect_object->model->w;
	effect_object->position.h= effect_object->model->h;

	effects->object = effect_object;
	effects->id = effect_id_count;
	
	return effects;
}





void spell_timer(GAME_VARIABLES *game_variables) {
	game_variables->spell_timer += 1;
	if (game_variables->spell_timer == 100) {
		game_variables->spell_timer = 0;
		memset(game_variables->spell_input, 0, 4);
	}
}

void walk_anim(GAME_VARIABLES *game_variables, PLAYER *player, SDL_Surface *display) {

	if (game_variables->direction_changed == 0) {
		player->walk_count += 1;

		if (player->walk_count == player->walk_intervall) {
			player->walk_count = 0;

			if (player->direction_left) {

				if (player->walk_left->next->model == NULL) {
					player->walk_left = player->walk_left->first;
					player->current_model = player->walk_left->model;

				} else {
					player->walk_left = player->walk_left->next;
					player->current_model = player->walk_left->model;
				}

			} else {

				if (player->walk_right->next->model == NULL) {
					player->walk_right = player->walk_right->first;
					player->current_model = player->walk_right->model;

				} else {
					player->walk_right = player->walk_right->next;
					player->current_model = player->walk_right->model;
				}

			}

		}

	} else {

		game_variables->direction_changed = 0;

		if (player->direction_left) {
			player->walk_left = player->walk_left->first;
			player->current_model = player->walk_left->model;

		} else {
			player->walk_right = player->walk_right->first;
			player->current_model = player->walk_right->model;
		}

	}
}


void jump_anim(GAME_VARIABLES *game_variables, PLAYER *player) {

	if (player->direction_left == 0) {
		player->current_model = player->jump_right->model;
	} else {
		player->current_model = player->jump_left->model;
	}


	if (game_variables->jump_counter < 40) {

		player->position.y -= 5;

		game_variables->jump_counter += 1;

	} else if (game_variables->jump_counter < 80) {
		player->position.y += 5;
		game_variables->jump_counter += 1;

	} else {
		game_variables->jumping = 0;
		game_variables->jump_counter = 0;

		if (player->direction_left == 0) {
			player->current_model = player->walk_right->model;
		} else {
			player->current_model = player->walk_left->model;
		}

	}

}

void move_player(GAME_VARIABLES *game_variables, PLAYER *player, SDL_Surface *display) {

	if (game_variables->is_casting == 0) {
		if (game_variables->move_right == 1) {

			player->direction_left = 0;
			player->position.x += 4;
			if (game_variables->jumping == 0) {
				walk_anim(game_variables, player, display);

			} else {
				jump_anim(game_variables, player);
			}

		} else if (game_variables->move_left == 1) {

			player->direction_left = 1;
			player->position.x -= 4;
			if (game_variables->jumping == 0) {
				walk_anim(game_variables, player, display);

			} else {
				jump_anim(game_variables, player);
			}

		} else if (game_variables->jumping == 1) {

			jump_anim(game_variables, player);
		}
	}
}


void handle_std_attack(GAME_VARIABLES *game_variables, PLAYER *player) {

	if (game_variables->is_casting && game_variables->current_cast == 0) {
		player->spell_timer_count += 1;

		if (strcmp(game_variables->player_name, "piccolo") == 0) {



			if (player->direction_left == 0) {
				if (player->spell_timer_count < 20) {
					player->current_model = player->spell_0->model;

				} else if (player->spell_timer_count < 40) {
					player->current_model = player->spell_0->next->model;

				} else if (player->spell_timer_count < 60) {
					player->current_model = player->spell_0->next->next->model;

				} else {
					player->current_model = player->walk_right->model;
					player->spell_timer_count = 0;
					game_variables->is_casting = 0;
				}

			} else {

				if (player->spell_timer_count < 20) {
					player->current_model = player->spell_0_left->model;

				} else if (player->spell_timer_count < 40) {
					player->current_model = player->spell_0_left->next->model;

				} else if (player->spell_timer_count < 60) {
					player->current_model = player->spell_0_left->next->next->model;

				} else {
					player->current_model = player->walk_left->model;
					player->spell_timer_count = 0;
					game_variables->is_casting = 0;
				}
			}

		}
	}
}




void check_melee_attack_collision(GAME_VARIABLES *game_variables, PLAYER* player, CURRENT_ENEMIES* enemies) {
	
	if (game_variables->is_casting && game_variables->current_cast == 0) {
		
		while (enemies->next != NULL) {
			
			if (enemies->object->is_targetable) {
				
				if (detect_rect_collision(&enemies->object->position, &player->position)) {
					
					if (detect_pixel_collision(enemies->object->current_model, player->current_model, &enemies->object->position, &player->position)) {
						enemies->object->health -= 50;
						if (enemies->object->health <= 0)
							enemies->object->death_type = 0;
					}
				}
			}
			
			enemies = enemies->next;
		}
	}
}



void remove_effect_by_name(CURRENT_EFFECTS *effects, char *effect_name) {

	while (effects->next != NULL) {


		if (strcmp(effects->object->effect_name, effect_name) == 0) {
			
			free(effects->object->harms_target);
			free(effects->object->effect_name);
			free(effects->object);

			if (effects->before != NULL && effects->next->object != NULL) {
				effects->next->next->before = effects;
				CURRENT_EFFECTS *buf0 = effects->before;
				CURRENT_EFFECTS *buf1 = effects->next;
				memcpy(effects, effects->next, sizeof(CURRENT_EFFECTS));
				free(buf1);
				effects->before = buf0;
				continue;

			} else if (effects->before == NULL && effects->next->object != NULL) {
				effects->next->next->before = effects;
				CURRENT_EFFECTS *buf = effects->next;
				memcpy(effects, effects->next, sizeof(CURRENT_EFFECTS));
				free(buf);
				effects->before = NULL;
				continue;

			} else if (effects->before != NULL && effects->next->object == NULL) {
				effects->before->next = effects->next;
				free(effects);
				break;

			} else if (effects->next->object == NULL && effects->before == NULL) {
				free(effects->next);
				memset(effects, 0, sizeof(CURRENT_EFFECTS));
				continue;
				
			} else {
				puts("unhandled condition");
			}
			
		}


		if (effects->next != NULL)
			effects = effects->next;
		else
			break;

	}
}

EFFECT_OBJECT* get_effect(CURRENT_EFFECTS* effects, ULONG id) {
	
	while(effects->next != NULL) {
		
		if (effects->id == id)
			return effects->object;
		
		effects = effects->next;
	}
	
	return NULL;
	
}

void remove_effect_by_id(CURRENT_EFFECTS *effects, ULONG id) {

	while (effects->next != NULL) {


		if (effects->id == id) {
			
			free(effects->object->harms_target);
			free(effects->object->effect_name);
			free(effects->object);

			if (effects->before != NULL && effects->next->object != NULL) {
				effects->next->next->before = effects;
				CURRENT_EFFECTS *buf0 = effects->before;
				CURRENT_EFFECTS *buf1 = effects->next;
				memcpy(effects, effects->next, sizeof(CURRENT_EFFECTS));
				free(buf1);
				effects->before = buf0;
				break;

			} else if (effects->before == NULL && effects->next->object != NULL) {
				effects->next->next->before = effects;
				CURRENT_EFFECTS *buf = effects->next;
				memcpy(effects, effects->next, sizeof(CURRENT_EFFECTS));
				free(buf);
				effects->before = NULL;
				break;

			} else if (effects->before != NULL && effects->next->object == NULL) {
				effects->before->next = effects->next;
				free(effects);
				break;

			} else if (effects->next->object == NULL && effects->before == NULL) {
				free(effects->next);
				memset(effects, 0, sizeof(CURRENT_EFFECTS));
				break;
				
			} else {
				puts("unhandled condition in remove_effect_by_id()");
			}
			
		}


		if (effects->next != NULL)
			effects = effects->next;
		else
			break;

	}
}


void remove_enemy_by_id(CURRENT_ENEMIES* enemies, ULONG id) {
	
	
	while (enemies->next != NULL) {


		if (enemies->id == id) {

			if (enemies->before != NULL && enemies->next->object != NULL) {
				enemies->next->next->before = enemies;
				CURRENT_ENEMIES *buf0 = enemies->before;
				CURRENT_ENEMIES *buf1 = enemies->next;
				memcpy(enemies, enemies->next, sizeof(CURRENT_ENEMIES));
				free(buf1);
				enemies->before = buf0;
				break;

			} else if (enemies->before == NULL && enemies->next->object != NULL) {
				enemies->next->next->before = enemies;
				CURRENT_ENEMIES *buf = enemies->next;
				memcpy(enemies, enemies->next, sizeof(CURRENT_ENEMIES));
				free(buf);
				enemies->before = NULL;
				break;

			} else if (enemies->before != NULL && enemies->next->object == NULL) {
				enemies->before->next = enemies->next;
				free(enemies);
				break;

			} else if (enemies->next->object == NULL && enemies->before == NULL) {
				free(enemies->next);
				memset(enemies, 0, sizeof(CURRENT_ENEMIES));
				break;
				
			} else {
				puts("unhandled condition in remove_enemy_by_id()");
			}
			
		}


		if (enemies->next != NULL)
			enemies = enemies->next;
		else
			break;

	}
	
}

void change_effect_model_by_name(CURRENT_EFFECTS* effects, EFFECT_MODEL_LIST* effect_model_list, const char* effect_name, const char* new_effect_name) {
	
	while (effects->next != NULL) {
		
		if (strcmp(effects->object->effect_name, effect_name) == 0) {
			effects->object->model = get_effect_model(new_effect_name, effect_model_list);
			memset(effects->object->effect_name, 0, strlen(effects->object->effect_name));
			strcpy(effects->object->effect_name, new_effect_name);
		}

		effects = effects->next;
	}
	
}

void set_effect_move_mode_by_name(CURRENT_EFFECTS* effects, const char* effect_name, STATE is_moving) {
	
		while (effects->next != NULL) {
		
		if (strcmp(effects->object->effect_name, effect_name) == 0) {
			effects->object->is_moving = is_moving;
		}

		effects = effects->next;
	}
}


void handle_casting(GAME_VARIABLES *game_variables, PLAYER *player, CURRENT_EFFECTS *effects, EFFECT_MODEL_LIST *effect_models, GAME_MODEL_LISTS *game_models) {


	if (game_variables->is_casting) {
		player->spell_timer_count += 1;

		if (strcmp(game_variables->player_name, "piccolo") == 0) {


			if (game_variables->current_cast == 1) {

				if (player->direction_left == 0) {
					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_1->model;
						player->spell_1 = player->spell_1->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_1->model;
						player->spell_1 = player->spell_1->next;
						SDL_Rect loc = player->position;
						loc.y += 80;
						create_effect(effects, loc, 1310, loc.y, "enemy", 1, 30, "fod", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 30) {
						player->current_model = player->spell_1->model;
						player->spell_1 = player->spell_1->next;

					} else if (player->spell_timer_count == 35) {
						player->current_model = player->spell_1->model;
						player->spell_1 = player->spell_1->first;

					} else if (player->spell_timer_count == 50) {

						player->current_model = player->walk_right->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}

				} else {

					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_1_left->model;
						player->spell_1_left = player->spell_1_left->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_1_left->model;
						player->spell_1_left = player->spell_1_left->next;
						SDL_Rect loc = player->position;
						loc.y += 80;
						create_effect(effects, loc, 0, loc.y, "enemy", 1, 30, "fod", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 30) {
						player->current_model = player->spell_1_left->model;
						player->spell_1_left = player->spell_1_left->next;

					} else if (player->spell_timer_count == 35) {
						player->current_model = player->spell_1_left->model;
						player->spell_1_left = player->spell_1_left->first;


					} else if (player->spell_timer_count == 50) {
						player->current_model = player->walk_left->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}


				}

			} else if (game_variables->current_cast == 2) {

				if (player->direction_left == 0) {
					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_2->model;
						player->spell_2 = player->spell_2->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_2->model;
						player->spell_2 = player->spell_2->next;

					} else if (player->spell_timer_count == 40) {
						player->current_model = player->spell_2->model;
						player->spell_2 = player->spell_2->next;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->spell_2->model;
						player->spell_2 = player->spell_2->first;

					} else if (player->spell_timer_count == 53) {
						player->current_model = player->walk_right->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}

				} else {

					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_2_left->model;
						player->spell_2_left = player->spell_2_left->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_2_left->model;
						player->spell_2_left = player->spell_2_left->next;

					} else if (player->spell_timer_count == 40) {
						player->current_model = player->spell_2_left->model;
						player->spell_2_left = player->spell_2_left->next;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->spell_2_left->model;
						player->spell_2_left = player->spell_2_left->first;

					} else if (player->spell_timer_count == 53) {
						player->current_model = player->walk_left->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}


				}


			} else if (game_variables->current_cast == 3) {

				if (player->direction_left == 0) {
					if (player->spell_timer_count < 25) {
						player->position.y -= 5;
						player->current_model = player->spell_3->model;

					} else if (player->spell_timer_count == 25) {
						player->spell_3 = player->spell_3->next;
						player->current_model = player->spell_3->model;

					} else if (player->spell_timer_count == 40) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;
						
						SDL_Rect loc;
						loc.x = player->position.x + 200;
						loc.y = player->position.y - 20;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x + 200 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 250;
						loc.y = player->position.y - 40;
						create_effect(effects, loc, player->position.x + 250 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 170;
						loc.y = player->position.y;
						create_effect(effects, loc, player->position.x + 100 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 210;
						loc.y = player->position.y - 80;
						create_effect(effects, loc, player->position.x + 130 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 60) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;

					} else if (player->spell_timer_count == 70) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;
						
						
						SDL_Rect loc;
						loc.x = player->position.x + 240;
						loc.y = player->position.y;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x + 240 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 300;
						loc.y = player->position.y + 10;
						create_effect(effects, loc, player->position.x + 300 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 80) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;

					} else if (player->spell_timer_count == 90) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;
						
						SDL_Rect loc;
						loc.x = player->position.x + 350;
						loc.y = player->position.y - 20;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x + 350 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 370;
						loc.y = player->position.y - 60;
						create_effect(effects, loc, player->position.x + 370 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 100) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;

					} else if (player->spell_timer_count == 110) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;
						
						SDL_Rect loc;
						loc.x = player->position.x + 430;
						loc.y = player->position.y;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x + 430 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x + 380;
						loc.y = player->position.y + 10;
						create_effect(effects, loc, player->position.x + 380 + 430 - player->position.y, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 120) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->next;

					} else if (player->spell_timer_count == 140) {
						player->current_model = player->spell_3->model;
						player->spell_3 = player->spell_3->first;
						change_effect_model_by_name(effects, effect_models, "eb0", "eb1");
						set_effect_move_mode_by_name(effects, "eb1", 1);
						

					} else if (player->spell_timer_count > 150 && player->spell_timer_count <= 174) {
						player->current_model = player->jump_right->model;
						player->position.y += 5;

					} else if (player->spell_timer_count == 180) {
						player->current_model = player->walk_right->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}

				} else {

					if (player->spell_timer_count < 25) {
						player->position.y -= 5;
						player->current_model = player->spell_3_left->model;

					} else if (player->spell_timer_count == 25) {
						player->spell_3_left = player->spell_3_left->next;
						player->current_model = player->spell_3_left->model;

					} else if (player->spell_timer_count == 40) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;
						
						SDL_Rect loc;
						loc.x = player->position.x - 50;
						loc.y = player->position.y - 20;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x - 450 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 100;
						loc.y = player->position.y - 40;
						create_effect(effects, loc, player->position.x - 500 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 30;
						loc.y = player->position.y + 10;
						create_effect(effects, loc, player->position.x - 400 - 300 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 40;
						loc.y = player->position.y - 80;
						create_effect(effects, loc, player->position.x - 400 - 340 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 60) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;

					} else if (player->spell_timer_count == 70) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;
						
						SDL_Rect loc;
						loc.x = player->position.x - 90;
						loc.y = player->position.y;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x - 490 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 150;
						loc.y = player->position.y + 10;
						create_effect(effects, loc, player->position.x - 550 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 80) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;

					} else if (player->spell_timer_count == 90) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;
						
						SDL_Rect loc;
						loc.x = player->position.x - 200;
						loc.y = player->position.y - 20;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x - 600 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 220;
						loc.y = player->position.y - 60;
						create_effect(effects, loc, player->position.x - 620 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);

					} else if (player->spell_timer_count == 100) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;

					} else if (player->spell_timer_count == 110) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;
						
						SDL_Rect loc;
						loc.x = player->position.x - 280;
						loc.y = player->position.y;
						loc.w = 40;
						loc.h = 42;
						create_effect(effects, loc, player->position.x - 680 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
						loc.x = player->position.x - 230;
						loc.y = player->position.y + 10;
						create_effect(effects, loc, player->position.x - 630 - 380 + player->position.y + 260, 430, "enemy", 0, 8, "eb0", effect_models, game_models, 0, 0);
						
					} else if (player->spell_timer_count == 120) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->next;

					} else if (player->spell_timer_count == 140) {
						player->current_model = player->spell_3_left->model;
						player->spell_3_left = player->spell_3_left->first;
						
						change_effect_model_by_name(effects, effect_models, "eb0", "ebl1");
						set_effect_move_mode_by_name(effects, "ebl1", 1);

					} else if (player->spell_timer_count > 150 && player->spell_timer_count <= 174) {
						player->current_model = player->jump_left->model;
						player->position.y += 5;

					} else if (player->spell_timer_count == 180) {
						player->current_model = player->walk_left->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}
				}


			} else if (game_variables->current_cast == 4) {

				if (player->direction_left == 0) {
					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_4->model;
						player->spell_4 =  player->spell_4->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_4->model;
						player->spell_4 =  player->spell_4->next;

					} else if (player->spell_timer_count == 30) {
						player->current_model = player->spell_4->model;
						player->spell_4 =  player->spell_4->first;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->walk_right->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}

				} else {

					if (player->spell_timer_count == 1) {
						player->current_model = player->spell_4_left->model;
						player->spell_4_left = player->spell_4_left->next;

					} else if (player->spell_timer_count == 25) {
						player->current_model = player->spell_4_left->model;
						player->spell_4_left = player->spell_4_left->next;

					} else if (player->spell_timer_count == 30) {
						player->current_model = player->spell_4_left->model;
						player->spell_4_left = player->spell_4_left->first;

					} else if (player->spell_timer_count == 50) {
						player->current_model = player->walk_left->model;
						player->spell_timer_count = 0;
						game_variables->is_casting = 0;

					}


				}

			}
		}

	}
}




void move_effects(CURRENT_EFFECTS *effects, GAME_MODEL_LISTS *game_models) {
	
	while (effects->next != NULL) {

		if (effects->object->is_moving) {

			int x_distance = effects->object->position.x - effects->object->target_x;
			x_distance = x_distance < 0 ? -x_distance : x_distance;

			int y_distance = effects->object->position.y - effects->object->target_y;
			y_distance = y_distance < 0 ? -y_distance : y_distance;

			int x_step = x_distance / effects->object->move_speed;
			int y_step = y_distance / effects->object->move_speed;

			if (x_step > y_step) {
				
				if (effects->object->position.x < effects->object->target_x)
					effects->object->position.x += effects->object->move_speed;
				else
					effects->object->position.x -= effects->object->move_speed;

				if (y_distance / x_step == 0) {
					
					if (effects->object->position.y < effects->object->target_y)
						effects->object->position.y += 1;
					else
						effects->object->position.y -= 1;
					
				} else {
					
					if (effects->object->position.y < effects->object->target_y)
						effects->object->position.y += y_distance / x_step;
					else
						effects->object->position.y -= y_distance / x_step;
				}

			} else if (x_step < y_step) {
				
				if (effects->object->position.y < effects->object->target_y)
					effects->object->position.y += effects->object->move_speed;
				else
					effects->object->position.y -= effects->object->move_speed;

				if (x_distance / y_step == 0) {
					
					if (effects->object->position.x < effects->object->target_x)
						effects->object->position.x += 1;
					else
						effects->object->position.x -= 1;
					
				} else {
					
					if (effects->object->position.x < effects->object->target_x)
						effects->object->position.x += x_distance / y_step;
					else
						effects->object->position.x -= x_distance / y_step;
				}

			} else {

				if (x_step == 0 || y_step == 0) {
					
					if (effects->object->position.y < effects->object->target_y)
						effects->object->position.y += 1;
					else
						effects->object->position.y -= 1;
					
					
					if (effects->object->position.x < effects->object->target_x)
						effects->object->position.x += 1;
					else
						effects->object->position.x -= 1;
					
						
				} else {
					if (effects->object->position.y < effects->object->target_y)
						effects->object->position.y += effects->object->move_speed;
					else
						effects->object->position.y -= effects->object->move_speed;
					
					if (effects->object->position.x < effects->object->target_x)
						effects->object->position.x += effects->object->move_speed;
					else
						effects->object->position.x -= effects->object->move_speed;
				}
			}


			if (x_distance == 0 && y_distance == 0 || x_distance == 1 && y_distance == 0 || x_distance == 0 && y_distance == 1|| x_distance == 1 && y_distance == 1) {
				
				effects->object->position.y -= 20;
				if (strcmp(effects->object->effect_name, "eb1") == 0 || strcmp(effects->object->effect_name, "ebl1") == 0) {
					create_effect(effects, effects->object->position, 0, 0, "enemy", 0, 0, "exp", NULL, game_models, 3, 0);
				}
				remove_effect_by_id(effects, effects->id);
				
			} else if (effects->object->position.x < 0) {
				remove_effect_by_id(effects, effects->id);
			}

		}

		if (effects->next != NULL)
			effects = effects->next;
		else
			break;

	}

}


void create_enemy(SDL_Rect pos, const char* enemy_name, CURRENT_ENEMIES* enemies, GAME_MODEL_LISTS* model_lists, PLAYER* player) {
	
	
	while (enemies->next != NULL) {
		
		enemies = enemies->next;
	}
	
	enemy_id_count++;
	
	enemies->id = enemy_id_count;
	enemies->next = calloc(1, sizeof(CURRENT_ENEMIES));
	enemies->next->before = enemies;
	
	if (strcmp(enemy_name, "mummy") == 0) {
		
		enemies->object = calloc(1, sizeof(ENEMY));
		
		enemies->object->walk_right = get_game_model_list(model_lists, "mummy_walk");
		enemies->object->walk_left = get_game_model_list(model_lists, "mummy_walk_left");
		enemies->object->attack = get_game_model_list(model_lists, "mummy_atk");
		enemies->object->attack_left = get_game_model_list(model_lists, "mummy_atk_left");
		enemies->object->death_1 = get_game_model_list(model_lists, "mummy_death_normal");
		enemies->object->death_1_left = get_game_model_list(model_lists, "mummy_death_normal_left");
		enemies->object->death_2 = get_game_model_list(model_lists, "mummy_death_exp");
		enemies->object->death_2_left = get_game_model_list(model_lists, "mummy_death_exp_left");
		
		
		enemies->object->walk_intervall = 5;
		enemies->object->walk_count = 0;
		
		enemies->object->death_intervall = 3;
		enemies->object->death_count = 0;
		
		enemies->object->attack_intervall = 3;
		enemies->object->attack_count = 0;
		
		enemies->object->position = pos;
		
		if (pos.x < player->position.x) {
			enemies->object->direction_left = 1;
			enemies->object->current_model = enemies->object->walk_right->model;
		} else {
			enemies->object->direction_left = 0;
			enemies->object->current_model = enemies->object->walk_left->model;
		}
		
		enemies->object->is_targetable = 1;
		enemies->object->is_attacking = 0;
		enemies->object->is_alive = 1;
		enemies->object->death_type = 1;
		
		enemies->object->health = 50;
		
	}
	
	enemies->object->position.w = enemies->object->current_model->w;
	enemies->object->position.h = enemies->object->current_model->h;
	
}

void handle_game_time(GAME_TIMER* timer) {
	
	timer->counter += 1;
	if (timer->counter == 100) {
		timer->seconds_played += 1;
		timer->counter = 0;
	}
	
	
}

void enemy_creation(GAME_TIMER* timer, CURRENT_ENEMIES* enemies, GAME_MODEL_LISTS* model_lists, PLAYER* player) {
	
	
	if (timer->counter == 1) {
		SDL_Rect pos;
		pos.x = 1300;
		pos.y = 350;
		create_enemy(pos, "mummy", enemies, model_lists, player);
		
	}
	
	
	if (timer->counter == 1 && timer->seconds_played % 5 == 0) {
		SDL_Rect pos;
		pos.x = 0;
		pos.y = 350;
		create_enemy(pos, "mummy", enemies, model_lists, player);
		
	}
	
}

void move_enemies(CURRENT_ENEMIES* enemies, PLAYER* player) {
	
	while (enemies->next != NULL) {
		
		if (enemies->object->is_attacking == 0 && enemies->object->is_alive) {
			
			enemies->object->walk_count += 1;
			
			if (enemies->object->walk_count == enemies->object->walk_intervall) {
				
				enemies->object->walk_count = 0;
				
				if (player->position.x > enemies->object->position.x) {
					
					enemies->object->direction_left = 0;
					enemies->object->position.x += 4;
					
					if (enemies->object->walk_right->next->model == NULL) {
						enemies->object->walk_right = enemies->object->walk_right->first;
					} else {
						enemies->object->walk_right = enemies->object->walk_right->next;
					}
					
					enemies->object->current_model = enemies->object->walk_right->model;
					
					
				} else {
					
					enemies->object->direction_left = 1;
					enemies->object->position.x -= 4;
					
					if (enemies->object->walk_left->next->model == NULL) {
						enemies->object->walk_left = enemies->object->walk_left->first;
					} else {
						enemies->object->walk_left = enemies->object->walk_left->next;
					}
					
					enemies->object->current_model = enemies->object->walk_left->model;
					
				}
				
			}
		}
		
		enemies = enemies->next;
	}
}


void handle_enemy_death(CURRENT_ENEMIES* enemies) {
	
	
	while (enemies->next != NULL) {
		if (enemies->object->health <= 0) {
			enemies->object->is_targetable = 0;
			enemies->object->is_alive = 0;
			enemies->object->death_count += 1;
			if (enemies->object->death_count == enemies->object->death_intervall) {
				
				enemies->object->death_count = 0;
			
				if (enemies->object->direction_left == 0) {
					
					if (enemies->object->death_type == 1) {
					
						if (enemies->object->death_1->next->model == NULL) {
							remove_enemy_by_id(enemies, enemies->id);
							continue;
							
						} else {
							enemies->object->death_1 = enemies->object->death_1->next;
							enemies->object->current_model = enemies->object->death_1->model;
						}
						
					} else {
						
						if (enemies->object->death_2->next->model == NULL) {
							remove_enemy_by_id(enemies, enemies->id);
							continue;
							
						} else {
							enemies->object->death_2 = enemies->object->death_2->next;
							enemies->object->current_model = enemies->object->death_2->model;
						}
						
						
					}
					
					
				} else {
					
					if (enemies->object->death_type == 1) {
					
					
						if (enemies->object->death_1_left->next->model == NULL) {
							remove_enemy_by_id(enemies, enemies->id);
							continue;
							
						} else {
							enemies->object->death_1_left = enemies->object->death_1_left->next;
							enemies->object->current_model = enemies->object->death_1_left->model;
						}
						
					} else {
						
						if (enemies->object->death_2_left->next->model == NULL) {
							remove_enemy_by_id(enemies, enemies->id);
							continue;
							
						} else {
							enemies->object->death_2_left = enemies->object->death_2_left->next;
							enemies->object->current_model = enemies->object->death_2_left->model;
						}
						
					}
				}
			}
		}
		
		
		enemies = enemies->next;
	}
	
}


void handle_enemy_attack(CURRENT_ENEMIES* enemies, PLAYER *player, CURRENT_EFFECTS *effects, EFFECT_MODEL_LIST *effect_models, GAME_MODEL_LISTS *game_models) {
	
	while (enemies->next != NULL) {
		
		int distance = enemies->object->position.x - player->position.x;
		distance = distance < 0 ? -distance + 50 : distance;
		
		if (distance < 150 && enemies->object->is_attacking == 0 && enemies->object->is_alive) {
			enemies->object->is_attacking = 1;
			
			if (enemies->object->position.x < player->position.x) {
				enemies->object->direction_left = 0;
				SDL_Rect pos = enemies->object->position;
				pos.x += 60;
				create_effect(effects, pos, 0, 0, "player", 0, 0, "mummy_breath", NULL, game_models, 3, 0);
				
			} else {
				enemies->object->direction_left = 1;
				SDL_Rect pos = enemies->object->position;
				pos.x -= 100;
				create_effect(effects, pos, 0, 0, "player", 0, 0, "mummy_breath_left", NULL, game_models, 3, 0);
			}
		
		} else if (enemies->object->is_attacking == 1 && enemies->object->is_alive) {
			
			enemies->object->attack_count += 1;
			
			if (enemies->object->attack_count == enemies->object->attack_intervall) {
				
				enemies->object->attack_count = 0;
			
				if (enemies->object->direction_left == 0) {
					
					if (enemies->object->attack->next->model == NULL) {
						enemies->object->attack = enemies->object->attack->first;
						enemies->object->is_attacking = 0;
						
					} else {
						enemies->object->attack = enemies->object->attack->next;
						enemies->object->current_model = enemies->object->attack->model;
					}
					
				} else {
					
					if (enemies->object->attack_left->next->model == NULL) {
						enemies->object->attack_left = enemies->object->attack_left->first;
						enemies->object->is_attacking = 0;
						
					} else {
						enemies->object->attack_left = enemies->object->attack_left->next;
						enemies->object->current_model = enemies->object->attack_left->model;
					}
					
				}
			}
			
			
			
		}
		
		enemies = enemies->next;
	}
	
}


void check_spell_collisions(CURRENT_EFFECTS* effects, CURRENT_ENEMIES* enemies, PLAYER* player, GAME_MODEL_LISTS *game_models, EFFECT_MODEL_LIST* effect_models) {
	
	CURRENT_EFFECTS* first_effect = effects;
	CURRENT_ENEMIES* first_enemy = enemies;
	
	//check effects -> enemy collisions
	while (effects->next != NULL) {
		
		if (strcmp(effects->object->effect_name, "eb0") == 0 || strcmp(effects->object->effect_name, "eb1") == 0 || strcmp(effects->object->effect_name, "ebl1") == 0  || strcmp(effects->object->effect_name, "fod") == 0) {
		
			while (enemies->next != NULL) {
				
				if (enemies->object->is_targetable) {
				
					if (detect_rect_collision(&enemies->object->position, &effects->object->position)) {
						
						if (detect_pixel_collision(enemies->object->current_model, effects->object->model, &enemies->object->position, &effects->object->position)) {
							
							if (strcmp(effects->object->effect_name, "eb0") == 0 || strcmp(effects->object->effect_name, "eb1") == 0 || strcmp(effects->object->effect_name, "ebl1") == 0) {
								enemies->object->health -= 25;
								create_effect(effects, effects->object->position, 0, 0, "enemy", 0, 0, "small_exp", NULL, game_models, 3, 0);
								remove_effect_by_id(effects, effects->id);
							
								
								break;
							}
							
							if (strcmp(effects->object->effect_name, "fod") == 0) {
								enemies->object->health -= 50;
								if(enemies->object->health <= 0)
									enemies->object->death_type = 2;
								remove_effect_by_id(effects, effects->id);
								break;
							}
							
						}
						
					}
				}
				
				enemies = enemies->next;
			}
			
			enemies = first_enemy;
		}
		
		if (effects->next != NULL)
			effects = effects->next;
		else
			break;
	}
	
	
}

void draw_model(SDL_Surface *model, SDL_Rect pos, SDL_Surface *display) {
	SDL_BlitSurface(model, NULL, display, &pos);
}


void handle_anim_effects(CURRENT_EFFECTS *effects) {
	
	while (effects->next != NULL) {
		if (effects->object->is_animated) {
			effects->object->anim_count += 1;
			
			if (effects->object->anim_count == effects->object->anim_intervall) {
				effects->object->anim_count = 0;
				
				if (effects->object->model_list->next->model == NULL && effects->object->is_loop_anim == 0) {
					remove_effect_by_id(effects, effects->id);
					continue;
					
				} else if (effects->object->model_list->next->model == NULL && effects->object->is_loop_anim == 1) {
					effects->object->model_list = effects->object->model_list->first;
					effects->object->model = effects->object->model_list->model;
					
				} else if (effects->object->model_list->next->model != NULL) {
					effects->object->model_list = effects->object->model_list->next;
					effects->object->model = effects->object->model_list->model;
				} else {
					puts("unhandled condition in handle_anim_effects()");
				}
				
			}
		}
		effects = effects->next;
		
	}
}


void draw_health_bar(PLAYER* player, STATIC_MODELS* static_models, META_STATS* meta_stats, SDL_Surface *display) {
	SDL_BlitSurface(static_models->health_bar_frame, NULL, display, &static_models->health_bar_frame_pos);
	
	
	int health_percentage = (player->health * 100) / (25 + meta_stats->level*25);
	int progress_bar_percentage = (static_models->health_bar.w*100) / static_models->health_bar_max_width;
	
	if (health_percentage < progress_bar_percentage) {
		
		if (static_models->health_bar.w - 2 < 0) {
			static_models->health_bar.w = 0;
			//end
		} else {
			static_models->health_bar.w -= 2;
		}
	}
	
	
	SDL_FillRect(display, &static_models->health_bar, static_models->health_bar_color);
	
	
}


void draw_effects(CURRENT_EFFECTS *effects, SDL_Surface *display) {

	while (effects->next != NULL) {
		
		SDL_BlitSurface(effects->object->model, NULL, display, &effects->object->position);
		

		effects = effects->next;
	}
}

void draw_enemies(CURRENT_ENEMIES* enemies, SDL_Surface *display) {
	
	while (enemies->next != NULL) {
		
		SDL_BlitSurface(enemies->object->current_model, NULL, display, &enemies->object->position);
		
		enemies = enemies->next;
	}
	
}

void draw_background(SDL_Surface *display, STATIC_MODELS *static_models) {
	SDL_BlitSurface(static_models->background, NULL, display, NULL);
}


