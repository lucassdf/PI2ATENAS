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
#include <time.h>
#include <string>
#include "movimentacoes.h"
#include "projeteis.h"
#include <sstream>
using namespace std;



// -------- VARIAVEIS GLOBAIS/CONSTANTES E INICIALIZACAO DE OBJETOS ----------
const int NUM_BALAS_C = 10;
const int NUM_BALAS_B = 10;
const int NUM_BALAS_E = 10;
const int NUM_BALAS_D = 10;
const int NUM_ATIRADOR = 10;
const int NUM_BALASATIRADOR = 5;
const int NUM_PERSONAGEM = 1;

// ---------- PROTÓTIPOS -------------
Projeteis balas_c[NUM_BALAS_C];
Projeteis balas_b[NUM_BALAS_B];
Projeteis balas_e[NUM_BALAS_E];
Projeteis balas_d[NUM_BALAS_D];
//DEFINICAO DO FPS
const float FPS = 60.0;
//ALTURA E LARGURA DA TELA
#define largura 640				
#define altura 495

int main()
{
	//DEFINICAO DA VARIAVEL DE DISPLAY DO ALLEGRO	
	ALLEGRO_DISPLAY* display;

	//DEFINICAO DAS TECLAS DE DIRECAO
	enum Direction { DOWN = 0, LEFT = 3, RIGHT = 6, UP = 9};
	enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA, ENTER};

	//INICIALIZACAO DA TELA
	if (!al_init())
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	display = al_create_display(largura, altura);
	al_set_window_title(display, "PI2-BCC-ATENAS");

	if (!display)
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	al_set_window_position(display, 200, 200);

	//VARIAVEIS DE SUPORTE
	bool done = false, draw = true, active = false, gameover = false, proximafase = false;
	int x = 10, y = 10, moveSpeed = 5, pontos = 0, contador = 0;
	int x2 = 200, y2 = 200, borda_x = 16, borda_y=16, borda_x2=16, borda_y2=16;
	int dir = DOWN, dir2 = 1, dir3 = 0, prevDir = 0, fase = 1;
	bool tiros[] = {false, false, false, false, false};
	
	// ------------ INICIALIZA��O DE OBJETOS --------------
	Atirador personagem[NUM_PERSONAGEM];
	Projeteis balas_c[NUM_BALAS_C];
	Projeteis balas_b[NUM_BALAS_B];
	Projeteis balas_e[NUM_BALAS_E];
	Projeteis balas_d[NUM_BALAS_D];
	Atirador atirador[NUM_ATIRADOR];
	Projeteis balas[NUM_BALASATIRADOR];



	// -------- FUN��ES INICIAIS ---------------
	InitAtirador(personagem, NUM_PERSONAGEM, "personagem", 5, 5.0);
	InitBalas(balas_c, NUM_BALAS_C, "personagem");
	InitBalas(balas_b, NUM_BALAS_B, "personagem");
	InitBalas(balas_e, NUM_BALAS_E, "personagem");
	InitBalas(balas_d, NUM_BALAS_D, "personagem");
	InitAtirador(atirador, NUM_ATIRADOR-7, "atirador", 1, 0.5);
	InitBalas(balas, NUM_BALASATIRADOR, "atirador");

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
	ALLEGRO_FONT* font = al_load_font("fast99.ttf", 14, NULL);
	ALLEGRO_BITMAP* playerWalk[12];
	for (int i = 0; i < 12; i++)
	{
		std::stringstream str;
		str << "sprites/" << i + 1 << ".png";
		playerWalk[i] = al_load_bitmap(str.str().c_str());
	}
	
	ALLEGRO_BITMAP* mapa = al_load_bitmap("mapa.png");;
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

	//al_draw_textf(font, al_map_rgb(255, 255, 255), largura / 2, altura / 2, ALLEGRO_ALIGN_CENTRE, "Bem vindo. Tecle ENTER para jogar");

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

		else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				tiros[DIREITA] = true;
				AtiraBalas(balas_d, NUM_BALAS_D, personagem, NUM_ATIRADOR, tiros, "personagem", 2);
				break;
			case ALLEGRO_KEY_LEFT:
				tiros[ESQUERDA] = true;
				AtiraBalas(balas_e, NUM_BALAS_E, personagem, NUM_ATIRADOR, tiros, "personagem", 1);
				break;
			case ALLEGRO_KEY_DOWN:
				tiros[BAIXO] = true;
				AtiraBalas(balas_b, NUM_BALAS_B, personagem, NUM_ATIRADOR, tiros, "personagem", 4);
				break;
			case ALLEGRO_KEY_UP:
				tiros[CIMA] = true;
				AtiraBalas(balas_c, NUM_BALAS_C, personagem, NUM_ATIRADOR, tiros, "personagem", 3);
				break;
			case ALLEGRO_KEY_ENTER:
				tiros[ENTER] = true;
				break;
			}
		}


		else if (events.type == ALLEGRO_EVENT_TIMER)
		{

			active = true;
			
				tiros[ENTER] = false;
				if (tiros[DIREITA])
					AtualizaBalas(balas_d, NUM_BALAS_D, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[ESQUERDA])
					AtualizaBalas(balas_e, NUM_BALAS_E, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[BAIXO])
					AtualizaBalas(balas_b, NUM_BALAS_B, personagem, NUM_PERSONAGEM, largura, altura);
				if (tiros[CIMA])
					AtualizaBalas(balas_c, NUM_BALAS_C, personagem, NUM_PERSONAGEM, largura, altura);

				if (!gameover)
				{
					AtiraBalas(balas, NUM_BALASATIRADOR, atirador, NUM_ATIRADOR, tiros, "atirador", 0);
					AtualizaBalas(balas, NUM_BALASATIRADOR, personagem, NUM_PERSONAGEM, largura, altura);
					LiberaTiros(atirador, NUM_ATIRADOR, "atirador");
					move_personagem(keyState, personagem, NUM_PERSONAGEM, altura, largura, &draw);
					LiberaTiros(personagem, NUM_ATIRADOR, "personagem");
					AtualizaAtirador(atirador, altura, largura, NUM_ATIRADOR);

					BalaColidida(balas, NUM_BALASATIRADOR, personagem, NUM_PERSONAGEM, "atirador", &pontos);
					BalaColidida(balas_b, NUM_BALAS_B, atirador, NUM_ATIRADOR, "personagem", &pontos);
					BalaColidida(balas_c, NUM_BALAS_C, atirador, NUM_ATIRADOR, "personagem", &pontos);
					BalaColidida(balas_d, NUM_BALAS_D, atirador, NUM_ATIRADOR, "personagem", &pontos);
					BalaColidida(balas_e, NUM_BALAS_E, atirador, NUM_ATIRADOR, "personagem", &pontos);
				}
		
		}

		//DESENHO DO PERSONAGEM, INIMIGO, TELA, TEXTO E ETC
		if (draw)
		{
			if (!gameover && !proximafase)
			{
				DesenhaBalas(balas_c, NUM_BALAS_C, 5, 0, 0, 0);
				DesenhaBalas(balas_b, NUM_BALAS_B, 5, 0, 0, 0);
				DesenhaBalas(balas_e, NUM_BALAS_E, 5, 0, 0, 0);
				DesenhaBalas(balas_d, NUM_BALAS_D, 5, 0, 0, 0);
				DesenhaAtirador(enemy, playerWalk, atirador, NUM_ATIRADOR, "atirador");
				DesenhaBalas(balas, NUM_BALASATIRADOR, 8, 0, 128, 0);
				DesenhaAtirador(enemy, playerWalk, personagem, NUM_PERSONAGEM, "personagem");
				//al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player) / 4, 32, 32, x, y, NULL);
				//al_draw_bitmap(enemy, x2, y2, NULL);
				al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, NULL, "FASE: %d   -   VIDAS: %d  /   PONTOS: %d ", fase, personagem->vida, pontos);
				if (personagem->vida <= 0)
					gameover = true;
				contador = pontos;
				if (contador == 3 && fase == 1)
				{
					proximafase = true;
					contador = 0;
				}
				if (contador == 8 && fase == 2)
				{
					proximafase = true;
					contador = 0;
				}
				if (contador == 16 && fase == 3)
				{
					proximafase = true;
					contador = 0;
				}
				
			}
			else
			{
				if(gameover)
					al_draw_textf(font, al_map_rgb(255, 255, 255),largura/2, altura/2, ALLEGRO_ALIGN_CENTRE, "Fim de jogo. Tecle enter para recomecar.");
				if(proximafase)
					al_draw_textf(font, al_map_rgb(255, 255, 255), largura / 2, altura / 2, ALLEGRO_ALIGN_CENTRE, "Voce derrotou todos os inimigos. Tecle enter para continuar para a proxima fase.");
				if (tiros[ENTER])
				{
					if (gameover)
					{
						fase = 1;
						mapa = al_load_bitmap("mapa.png");
						enemy = al_load_bitmap("trash.png");
						InitAtirador(personagem, NUM_PERSONAGEM, "personagem", 5, 5.0);
						InitBalas(balas_c, NUM_BALAS_C, "personagem");
						InitBalas(balas_b, NUM_BALAS_B, "personagem");
						InitBalas(balas_e, NUM_BALAS_E, "personagem");
						InitBalas(balas_d, NUM_BALAS_D, "personagem");
						InitAtirador(atirador, NUM_ATIRADOR-7, "atirador", 1, 0.5);
						InitBalas(balas, NUM_BALASATIRADOR, "atirador");
						gameover = false;
						tiros[ENTER] = false;
						pontos = 0;
					}
					if (proximafase && fase == 1)
					{
						fase++;
						mapa = al_load_bitmap("map1.png");
						enemy = al_load_bitmap("trash2.png");
						InitBalas(balas_c, NUM_BALAS_C, "personagem");
						InitBalas(balas_b, NUM_BALAS_B, "personagem");
						InitBalas(balas_e, NUM_BALAS_E, "personagem");
						InitBalas(balas_d, NUM_BALAS_D, "personagem");
						InitAtirador(atirador, NUM_ATIRADOR-5, "atirador", 3, 1.0);
						InitBalas(balas, NUM_BALASATIRADOR, "atirador");
						proximafase = false;
						tiros[ENTER] = false;
					}
					if (proximafase && fase == 2)
					{
						fase++;
						mapa = al_load_bitmap("map2.png");
						enemy = al_load_bitmap("trash3.png");
						InitBalas(balas_c, NUM_BALAS_C, "personagem");
						InitBalas(balas_b, NUM_BALAS_B, "personagem");
						InitBalas(balas_e, NUM_BALAS_E, "personagem");
						InitBalas(balas_d, NUM_BALAS_D, "personagem");
						InitAtirador(atirador, NUM_ATIRADOR-2, "atirador", 3, 1.0);
						InitBalas(balas, NUM_BALASATIRADOR, "atirador");
						proximafase = false;
						tiros[ENTER] = false;
					}

				}

			}
				al_flip_display();
				al_clear_to_color(al_map_rgb(75, 54, 33));
				al_draw_bitmap(mapa, 0, 15, NULL);
				
			
		}
	}

	//LIMPEZA DE MEMORIA
	al_destroy_display(display);
	al_destroy_timer(timer);
	for (int i = 0; i < 12; i++)
	{
		al_destroy_bitmap(playerWalk[i]);
	}
	//al_destroy_bitmap(enemy);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_sample(trilha_sonora);
	al_destroy_sample(projeteis_lancados);
	al_destroy_sample_instance(inst_trilha_sonora);
	al_destroy_sample_instance(inst_projeteis_lancados);

	return 0;






}


/*



*/
