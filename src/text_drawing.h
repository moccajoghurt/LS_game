
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
				puts("illegal char used!");
				exit(1);
			}
		}
		
		
		pos.x += 25;
		SDL_BlitSurface(first_val->model, NULL, display, &pos);
	}
	
}