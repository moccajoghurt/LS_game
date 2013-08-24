
int detect_rect_collision(SDL_Rect* rect1, SDL_Rect* rect2) {
	
	// sides rectangle 1
	int rect1_left = rect1->x;
	int rect1_right = rect1->x + rect1->w;
	int rect1_top = rect1->y;
	int rect1_bot = rect1->y + rect1->h;
	
	// sides rectangle 2
	int rect2_left = rect2->x;
	int rect2_right = rect2->x + rect2->w;
	int rect2_top = rect2->y;
	int rect2_bot = rect2->y + rect2->h;
	
	
	
	if (rect1_bot <= rect2_top || rect1_top >= rect2_bot || rect1_right <= rect2_left || rect1_left >= rect2_right) {
		return 0;
	}
	
	return 1;
	
}

SDL_Rect* get_intersection_rect(SDL_Rect* rect1, SDL_Rect* rect2) {
	
	SDL_Rect* intersection = calloc(1, sizeof(SDL_Rect));
	
	intersection->x = rect1->x > rect2->x ? rect1->x : rect2->x;
	intersection->y = rect1->y > rect2->y ? rect1->y : rect2->y;
	
	if (rect1->x + rect1->w < rect2->x + rect2->w) {
		intersection->w = rect1->x + rect1->w - intersection->x;
	} else {
		intersection->w = rect2->x + rect2->w - intersection->x;
	}
	
	if (rect1->y + rect1->h < rect2->y + rect2->h) {
		intersection->h = rect1->y + rect1->h - intersection->y;
	} else {
		intersection->h = rect2->y + rect2->h - intersection->y;
	}
	
	/*
	printf("rect1: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", rect1->x, rect1->y, rect1->x + rect1->w, rect1->y, rect1->x, rect1->y + rect1->h, rect1->x + rect1->w, rect1->y + rect1->h);
	printf("rect2: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", rect2->x, rect2->y, rect2->x + rect2->w, rect2->y, rect2->x, rect2->y + rect2->h, rect2->x + rect2->w, rect2->y + rect2->h);
	printf("rect3: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", intersection->x, intersection->y, intersection->x + intersection->w,intersection->y, intersection->x, intersection->y + intersection->h, intersection->x + intersection->w, intersection->y + intersection->h);
	*/
	
	return intersection;
}


int detect_pixel_collision(SDL_Surface* model1, SDL_Surface* model2, SDL_Rect* pos1, SDL_Rect* pos2) {
	
	SDL_LockSurface(model1);
	SDL_LockSurface(model2);
	
	SDL_Rect* intersection = get_intersection_rect(pos1, pos2);
	Uint32* pixel_ptr_1;
	Uint32* pixel_ptr_2;
	
	//startpoint model 1 & 2
	unsigned int model1_startpixel;
	unsigned int model2_startpixel;
	
	// number of pixels per line before the startpixel
	model1_startpixel = (intersection->y - pos1->y) * model1->w;
	model2_startpixel = (intersection->y - pos2->y) * model2->w;
	
	
	// add the number of pixels before the startpixel in the startpixel line
	model1_startpixel += intersection->x - pos1->x;
	model2_startpixel += intersection->x - pos2->x;
	
	pixel_ptr_1 = (Uint32*)model1->pixels + model1_startpixel;
	pixel_ptr_2 = (Uint32*)model2->pixels + model2_startpixel;
	
	
	int width_count = 0;
	for (int i = 0; i < intersection->w * intersection->h; i++) {
		
		
		if (*pixel_ptr_1 != 0 && *pixel_ptr_2 != 0) {
			// collision
			SDL_UnlockSurface(model1);
			SDL_UnlockSurface(model2);
			free(intersection);
			return 1;
			
		}
		
		if (width_count == intersection->w - 1) {
			width_count = 0;
			
			pixel_ptr_1 += model1->w - intersection->w + 1;
			pixel_ptr_2 += model2->w - intersection->w + 1;
			continue;
		}
		
		
		
		pixel_ptr_1 = pixel_ptr_1 + 1;
		pixel_ptr_2 = pixel_ptr_2 + 1;
		width_count++;
	}
	
	SDL_UnlockSurface(model1);
	SDL_UnlockSurface(model2);
	
	free(intersection);
	return 0;
	
}