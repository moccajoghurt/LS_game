typedef unsigned char STATE;
typedef unsigned long long ULONG;

//only modified by create_effect()
static ULONG effect_id_count = 0;

//only modified by create_enemy()
static ULONG enemy_id_count = 0;

static ULONG score = 0;

typedef struct GAME_VARIABLES {

	UCHAR game_running;

	char *spell_input;
	int spell_timer;
	UCHAR move_left;
	UCHAR move_right;
	UCHAR jumping;
	int jump_counter;
	UCHAR direction_changed;
	UCHAR is_casting;
	int current_cast;

	char *player_name;

} GAME_VARIABLES;


typedef struct EFFECT_MODEL_LIST EFFECT_MODEL_LIST;
struct EFFECT_MODEL_LIST {

	EFFECT_MODEL_LIST *first;
	SDL_Surface *model;
	EFFECT_MODEL_LIST *next;
	char *effect_name;

};

typedef struct MODEL_LIST MODEL_LIST;
struct MODEL_LIST {
	MODEL_LIST *first;
	SDL_Surface *model;
	MODEL_LIST *next;
};

typedef struct GAME_MODEL_LISTS GAME_MODEL_LISTS;
struct GAME_MODEL_LISTS {
	
	char* name;
	MODEL_LIST* models;
	GAME_MODEL_LISTS* next;
};

typedef struct STATIC_MODELS {

	SDL_Surface *background;
	SDL_Surface *health_bar_frame;
	
	SDL_Rect health_bar_frame_pos;
	SDL_Rect health_bar;
	
	Uint32 health_bar_color;
	
	int health_bar_max_width;


} STATIC_MODELS;

typedef struct ENEMY {
	SDL_Rect position;
	int health;

	MODEL_LIST *walk_right;
	MODEL_LIST *walk_left;
	MODEL_LIST *attack;
	MODEL_LIST *attack_left;
	MODEL_LIST *death_1;
	MODEL_LIST *death_1_left;
	MODEL_LIST *death_2;
	MODEL_LIST *death_2_left;
	
	SDL_Surface* current_model;
	
	int attack_intervall;
	int attack_count;
	int walk_intervall;
	int walk_count;
	int death_intervall;
	int death_count;
	STATE direction_left;
	STATE is_targetable;
	STATE is_alive;
	STATE is_attacking;
	int death_type;
	

} ENEMY;

typedef struct CURRENT_ENEMIES CURRENT_ENEMIES;
struct CURRENT_ENEMIES {
	
	ULONG id;
	CURRENT_ENEMIES* before;
	ENEMY *object;
	CURRENT_ENEMIES *next;
};

typedef struct PLAYER {

	MODEL_LIST *walk_right;
	MODEL_LIST *walk_left;
	MODEL_LIST *jump_right;
	MODEL_LIST *jump_left;
	MODEL_LIST *spell_0;
	MODEL_LIST *spell_1;
	MODEL_LIST *spell_2;
	MODEL_LIST *spell_3;
	MODEL_LIST *spell_4;
	MODEL_LIST *spell_0_left;
	MODEL_LIST *spell_1_left;
	MODEL_LIST *spell_2_left;
	MODEL_LIST *spell_3_left;
	MODEL_LIST *spell_4_left;
	MODEL_LIST *death;
	MODEL_LIST *death_left;
	MODEL_LIST *got_hit;
	MODEL_LIST *got_hit_left;
	MODEL_LIST *current_cast;

	SDL_Surface *current_model;

	int spell_timer_count;

	int health;
	SDL_Rect position;

	int walk_intervall;
	int walk_count;

	STATE direction_left;
	
	char* name;


} PLAYER;


typedef struct EFFECT_OBJECT {

	SDL_Rect position;
	int target_x;
	int target_y;
	char *harms_target;
	STATE is_moving;
	
	int anim_count;
	int anim_intervall;
	STATE is_animated;
	STATE is_loop_anim;
	MODEL_LIST* model_list;

	int move_speed;
	int damage;

	char *effect_name;
	SDL_Surface *model;

} EFFECT_OBJECT;


typedef struct CURRENT_EFFECTS CURRENT_EFFECTS;
struct CURRENT_EFFECTS {
	
	ULONG id;
	EFFECT_OBJECT *object;
	CURRENT_EFFECTS *before;
	CURRENT_EFFECTS *next;
};



typedef struct GAME_TIMER {
	
	ULONG seconds_played;
	int enemy_spawn_timer;
	
	int counter;
	
} GAME_TIMER;


typedef struct META_STATS {
	
	
	int experience;
	int level;
	
	
} META_STATS;


