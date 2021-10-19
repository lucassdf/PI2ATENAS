#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
using namespace std;

//ALTURA E LARGURA DA TELA
#define ScreenWidht 640		
#define ScreenHeight 480

//VARIAVEIS DE POSICAO DO INIMIGO
int x2 = 600, y2 = 400, dir2 = 1;

//FUNCAO DE MOVIMENTO DO INIMIGO
void move_inimigo(void);

void move_inimigo(void)
{

	if (dir2 == 1 && x2 != 20 && y2 != 20)
	{
		--x2; 
		--y2;
	}
	else if (dir2 == 2 && x2 != 20 && y2 != ScreenHeight - 50)
	{
		--x2;
		++y2;
	}
	else if (dir2 == 3 && x2 != ScreenWidht - 50 && y2 != 20)
	{
		++x2;
		--y2;
	}
	else if (dir2 == 4 && x2 != ScreenWidht - 50 && y2 != ScreenHeight - 50)
	{
		++x2;
		++y2;
	}
	else
	{
		dir2 = rand() % 4 + 1;
	}

}

int main()
{
	//DEFINICAO DA VARIAVEL DE DISPLAY DO ALLEGRO	
	ALLEGRO_DISPLAY* display;

	//DEFINICAO DAS TECLAS DE DIRECAO
	enum Direction { DOWN, LEFT, RIGHT, UP };

	//DEFINICAO DO FPS
	const float FPS = 60.0;

	//INICIALIZACAO DA TELA
	if (!al_init())
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	display = al_create_display(ScreenWidht, ScreenHeight);
	al_set_window_title(display, "PI2-BCC-ATENAS");

	if (!display)
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel criar o display do allegro", NULL, NULL);

	al_set_window_position(display, 200, 200);

	//VARIAVEIS DE SUPORTE
	bool done = false, draw = true, active = false;
	float x = 10, y = 10, moveSpeed = 5;
	int dir = DOWN, sourceX = 32, sourceY = 0;

	//INICIALIZACAO DOS ADDONS DO ALLEGRO
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();
	al_reserve_samples(10);

	//INICIALIZACAO DOS EVENTOS DO ALLEGRO (TEXTO, PERSONAGEM, FILA DE EVENTOS E ETC)
	ALLEGRO_FONT* font = al_load_font("fast99.ttf", 36, NULL);
    ALLEGRO_BITMAP* player = al_load_bitmap("avatar.png");
	ALLEGRO_BITMAP* enemy = al_load_bitmap("trash.png");
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	ALLEGRO_SAMPLE* trilha_sonora = NULL;
	ALLEGRO_SAMPLE* projeteis_lancados = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_projeteis_lancados = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;
	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);
	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 0.8);


	
	//LOOP CONTENDO A LOGICA DO JOGO
	srand(time(NULL));
	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		al_get_keyboard_state(&keyState);
		al_play_sample_instance(inst_trilha_sonora);

		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (events.type == ALLEGRO_EVENT_TIMER)
		{
			//LOGICA DE MOVIMENTACAO DO PERSONAGEM
			active = true;
			if (al_key_down(&keyState, ALLEGRO_KEY_S) && y < ScreenHeight - 30)
			{
				y += moveSpeed;
				dir = DOWN;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_W) && y > 0)
			{
				y -= moveSpeed;
				dir = UP;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_D) && x < ScreenWidht - 30)
			{
				x += moveSpeed;
				dir = RIGHT;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_A) && x > 0)
			{
				x -= moveSpeed;
				dir = LEFT;
			}
			else
				active = false;

			if (active)
				sourceX += al_get_bitmap_width(player) / 3;
			else
				sourceX = 32;

			if (sourceX >= al_get_bitmap_width(player))
				sourceX = 0;

			sourceY = dir;

			draw = true;

		}

		//DESENHO DO PERSONAGEM, INIMIGO, TELA, TEXTO E ETC
		if (draw)
		{
		
			al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player) / 4, 32, 32, x, y, NULL);
			al_draw_bitmap(enemy, x2, y2, NULL);
			al_flip_display();
			move_inimigo();
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(font, al_map_rgb(0, 128, 0), 320, 15, ALLEGRO_ALIGN_CENTRE, "INICIO");

		}

	}

	//LIMPEZA DE MEMORIA
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_bitmap(player);
	al_destroy_bitmap(enemy);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_sample(trilha_sonora);
	al_destroy_sample(projeteis_lancados);
	al_destroy_sample_instance(inst_trilha_sonora);
	al_destroy_sample_instance(inst_projeteis_lancados);


	return 0;






}

