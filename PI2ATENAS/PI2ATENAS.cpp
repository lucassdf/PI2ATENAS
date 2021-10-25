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
#include "movimentacoes.h"
#include "projeteis.h"


using namespace std;



// -------- VARIAVEIS GLOBAIS/CONSTANTES E INICIALIZACAO DE OBJETOS ----------
const int NUM_BALAS_C = 8;
const int NUM_BALAS_B = 8;
const int NUM_BALAS_E = 8;
const int NUM_BALAS_D = 8;
const int NUM_ATIRADOR = 2;
const int NUM_BALASATIRADOR = 4;

// ---------- PROTÓTIPOS -------------


void InitAtirador(Atirador atirador[], int tamanho);
void LiberaTiros(Atirador atirador[], int tamanho);
void AtualizaAtirador(Atirador atirador[], int tamanho);

void InitBalasAtirador(ProjeteisAtirador balas[], int tamanho);
void AtiraBalas(ProjeteisAtirador balas[], int tamanho);
void AtualizaBalasAtirador(ProjeteisAtirador balas[], int tamanho);
void DesenhaBalasAtirador(ProjeteisAtirador balas[], int tamanho);


void DesenhaAtirador(Atirador atirador[], int tamanho);

Projeteis balas_c[NUM_BALAS_C];
Projeteis balas_b[NUM_BALAS_B];
Projeteis balas_e[NUM_BALAS_E];
Projeteis balas_d[NUM_BALAS_D];
//DEFINICAO DO FPS
const float FPS = 60.0;
//ALTURA E LARGURA DA TELA
#define ScreenWidht 640	
#define ScreenHeight 480

int main()
{
	//DEFINICAO DA VARIAVEL DE DISPLAY DO ALLEGRO	
	ALLEGRO_DISPLAY* display;

	//DEFINICAO DAS TECLAS DE DIRECAO
	enum Direction { DOWN, LEFT, RIGHT, UP };
	enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA };

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
	int x2 = 200, y2 = 200;
	int dir = DOWN, dir2 = 1, dir3 = 0, sourceX = 32, sourceY = 0;
	bool tiros[] = { false, false, false, false };
	

	// ------------ INICIALIZA��O DE OBJETOS --------------
	Projeteis balas_c[NUM_BALAS_C];
	Projeteis balas_b[NUM_BALAS_B];
	Projeteis balas_e[NUM_BALAS_E];
	Projeteis balas_d[NUM_BALAS_D];
	Atirador atirador[NUM_ATIRADOR];
	ProjeteisAtirador balas[NUM_BALASATIRADOR];


	// -------- FUN��ES INICIAIS ---------------
	InitBalas(balas_c, balas_b, balas_d, balas_e, NUM_BALAS_C);
	InitBalas(balas_b, balas_c, balas_e, balas_d, NUM_BALAS_B);
	InitBalas(balas_e, balas_d, balas_b, balas_c, NUM_BALAS_E);
	InitBalas(balas_d, balas_e, balas_b, balas_c, NUM_BALAS_D);
	InitAtirador(atirador, NUM_ATIRADOR);
	InitBalasAtirador(balas, NUM_BALASATIRADOR);


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

		else if (events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (events.keyboard.keycode)
			{
			case ALLEGRO_KEY_RIGHT:
				tiros[DIREITA] = true;
				AtiraBalas(balas_d, NUM_BALAS_D, x, y, tiros);
				break;
			case ALLEGRO_KEY_LEFT:
				tiros[ESQUERDA] = true;
				AtiraBalas(balas_e, NUM_BALAS_E, x, y, tiros);
				break;
			case ALLEGRO_KEY_DOWN:
				tiros[BAIXO] = true;
				AtiraBalas(balas_b, NUM_BALAS_B, x, y, tiros);
				break;
			case ALLEGRO_KEY_UP:
				tiros[CIMA] = true;
				AtiraBalas(balas_c, NUM_BALAS_C, x, y, tiros);
				break;
			}
		}


		else if (events.type == ALLEGRO_EVENT_TIMER)
		{
			active = true;
			move_personagem(player, keyState, ScreenHeight, ScreenWidht, &x, &y, &dir, &moveSpeed, &active, &sourceX, &sourceY, &draw);
			move_inimigo(&x2, &y2, &dir2, ScreenHeight, ScreenWidht, enemy);
			
			if (tiros[DIREITA])
				AtualizarBalasDireita(balas_d, NUM_BALAS_D, tiros);
			if (tiros[ESQUERDA])
				AtualizarBalasEsquerda(balas_e, NUM_BALAS_E, tiros);
			if (tiros[BAIXO])
				AtualizarBalasBaixo(balas_b, NUM_BALAS_B, tiros);
			if (tiros[CIMA])
				AtualizarBalasCima(balas_c, NUM_BALAS_C, tiros);

			AtualizaBalasAtirador(balas, NUM_BALASATIRADOR);
			AtiraBalas(balas, NUM_BALASATIRADOR);
			LiberaTiros(atirador, NUM_ATIRADOR);
			AtualizaAtirador(atirador, NUM_ATIRADOR);
			


		}



		//DESENHO DO PERSONAGEM, INIMIGO, TELA, TEXTO E ETC
		if (draw)
		{
			DesenhaBalas(balas_c, NUM_BALAS_C);
			DesenhaBalas(balas_b, NUM_BALAS_B);
			DesenhaBalas(balas_e, NUM_BALAS_E);
			DesenhaBalas(balas_d, NUM_BALAS_D);
			DesenhaAtirador(atirador, NUM_ATIRADOR);
			DesenhaBalasAtirador(balas, NUM_BALASATIRADOR);
			al_draw_bitmap_region(player, sourceX, sourceY * al_get_bitmap_height(player) / 4, 32, 32, x, y, NULL);
			al_draw_bitmap(enemy, x2, y2, NULL);
			al_flip_display();
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

// -------------- DEFINI��O DE FUN��ES E PROCEDIMENTOS ---------------


//__________________________________________________
//----------------- ATIRADOR ----------------------

void InitAtirador(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		atirador[i].ID = INIMIGOS;
		atirador[i].velocidade = 5;
		atirador[i].borda_x = 18;
		atirador[i].borda_y = 18;
		atirador[i].ativo = false;
	}
}

void LiberaTiros(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (!atirador[i].ativo)
		{
			
				atirador[i].x = 30;
				atirador[i].y = 100;
				atirador[i].ativo = true;
	

			/*if (rand() % 500 == 0)
			{
				atirador[i].x = 1;
				atirador[i].y = 5;
				atirador[i].ativo = true;
				break;
			}*/
		}
	}
}

void AtualizaAtirador(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (atirador[i].ativo)
		{
			//atirador[i].x -= atirador[i].velocidade;
			atirador[i].x = 560;

			if (atirador[i].x < 0)
			{
				atirador[i].ativo = false;
			}
		}
	}
}

void InitBalasAtirador(ProjeteisAtirador balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++) {
		balas[i].ID = PROJETEISATIRADOR;
		balas[i].velocidade = 10;
		balas[i].ativo = false;
	}
}

void AtiraBalas(ProjeteisAtirador balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (!balas[i].ativo)
		{
			balas[i].x = 530;
			balas[i].y = 100;
			balas[i].ativo = true;
			break;
		}

	}
}

void AtualizaBalasAtirador(ProjeteisAtirador balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			balas[i].x -= balas[i].velocidade;

			if (balas[i].x < 0)
				balas[i].ativo = false;
		}

	}
}

void DesenhaBalasAtirador(ProjeteisAtirador balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			al_draw_filled_circle(balas[i].x, balas[i].y, 8, al_map_rgb(255, 0, 255));
		}
	}
}

void DesenhaAtirador(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (atirador[i].ativo)
		{
			al_draw_filled_circle(atirador[i].x, atirador[i].y, 20, al_map_rgb(255, 255, 0));
		}
	}
}
