

void handle_level_intro(META_DATA* meta_data, FONT_LIST* font_list, STATIC_POSITIONS* static_positions, GAME_TIMER* timer, SDL_Surface* display) {
	
	
	if (meta_data->current_stage == 1) {
		
		draw_text("STAGE 1!", font_list, static_positions->level_intro_pos, display);
		SDL_Rect pos = static_positions->level_intro_pos;
		pos.y += 50;
		pos.x -= 370;
		draw_text("EINE GEHEIME MACHT SCHUERT KREBS IM FORUM!", font_list, pos, display);
		SDL_Rect pos1 = static_positions->level_intro_pos;
		pos1.y += 80;
		pos1.x -=220;
		draw_text("BEKAEMPFE DEN FORUM KREBS!", font_list, pos1, display);
		
	}
	
	
	if (timer->level_intro_timer == 120) {
		meta_data->level_running = 1;
		meta_data->level_intro = 0;
		timer->level_intro_timer = 0;
	} else {
		timer->level_intro_timer++;
	}
}