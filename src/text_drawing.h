
void draw_text(char* text, FONT_LIST* font_list, SDL_Rect pos, SDL_Surface* display) {
	
	
	for (int i = 0; i < strlen(text); i++) {
		FONT_LIST* first_val = font_list;
		
		if (text[i] == ' ') {
			pos.x += 25;
			continue;
		}
		
		while (first_val->value != text[i]) {
			first_val = first_val ->next;
			
			if (first_val->next == NULL) {
				printf("illegal char used: %c\n", text[i]);
				exit(1);
			}
		}
		
		
		pos.x += 25;
		SDL_BlitSurface(first_val->model, NULL, display, &pos);
	}
	
}


void draw_meta_data(FONT_LIST* font_list, PLAYER* player, META_DATA* meta_data, SDL_Surface* display) {
	SDL_Rect pos_name;
	pos_name.y += 15;
	draw_text(player->name, font_list, pos_name, display);
	
	char* level = calloc(1, strlen("level ") + 4);
	char *lvl = calloc(1, 3);
	snprintf(lvl, sizeof(lvl), "%d", meta_data->level);
	strcpy(level, "LVL ");
	strcat(level, lvl);
	SDL_Rect pos_level;
	pos_level.y += 15;
	pos_level.x += 200;
	draw_text(level, font_list, pos_level, display);
	free(level);
	free(lvl);
	
}