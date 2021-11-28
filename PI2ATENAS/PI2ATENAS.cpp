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
#include "projeteis.h"
#include "imagens.h"
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
#define altura 480

int main()
{
	//DEFINICAO DA VARIAVEL DE DISPLAY DO ALLEGRO	
	ALLEGRO_DISPLAY* display;

	//DEFINICAO DAS TECLAS DE DIRECAO
	enum Direction { DOWN = 0, LEFT = 3, RIGHT = 6, UP = 9};
	enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA, ENTER, UM, DOIS, TRES, F1, F2, F3, F4, F5};

	//INICIALIZACAO DA TELA
	if (!al_init())
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	display = al_create_display(largura, altura);
	al_set_window_title(display, "PI2-BCC-ATENAS");

	if (!display)
		al_show_native_message_box(NULL, "Error", NULL, "Nao foi possivel iniciar o allegro", NULL, NULL);

	al_set_window_position(display, 200, 200);

	//VARIAVEIS DE SUPORTE
	bool done = false, draw = true, active = false, gameover = false, proximafase = false, iniciar = false, fimdejogo = false, respondido[8] = { false, false, false, false, false, false, false, false };
	bool vermelho = false, verde = false, azul = false, amarelo = false, marrom = false;
	int x = 10, y = 10, moveSpeed = 5, pontos = 0, contador = 0, r = 0, g = 138, b = 200, r2 = 0, g2 = 138, b2 = 200, corBala = 4, corAtirador = 4;
	int dir = DOWN, dir2 = 1, dir3 = 0, prevDir = 0, fase = 1;
	bool tiros[] = {false, false, false, false, false, false, false, false, false, false, false, false, false};
	
	// ------------ INICIALIZA��O DE OBJETOS --------------
	Atirador personagem[NUM_PERSONAGEM];
	Projeteis balas_c[NUM_BALAS_C];
	Projeteis balas_b[NUM_BALAS_B];
	Projeteis balas_e[NUM_BALAS_E];
	Projeteis balas_d[NUM_BALAS_D];
	Atirador atirador[NUM_ATIRADOR];
	Projeteis balas[NUM_BALASATIRADOR];


	// -------- FUN��ES INICIAIS ---------------
	InitAtirador(personagem, NUM_PERSONAGEM, "personagem", 5, 5.0, 0);
	InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
	InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
	InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
	InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
	InitAtirador(atirador, NUM_ATIRADOR-9, "atirador", 1, 0, corAtirador);
	InitBalas(balas, NUM_BALASATIRADOR, "atirador", 1, corBala);

	
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
	ALLEGRO_FONT* font = al_load_font("fontes/Minecraft.ttf", 25, NULL);
	ALLEGRO_FONT* font2 = al_load_font("fontes/Minecraft.ttf", 14, NULL);
	ALLEGRO_BITMAP* playerWalk[12];
	for (int i = 0; i < 12; i++)
	{
		std::stringstream str;
		str << "sprites/" << i + 1 << ".png";
		playerWalk[i] = al_load_bitmap(str.str().c_str());
	}

	ALLEGRO_BITMAP* mapa = al_load_bitmap("mapas/bosque1.png");
	ALLEGRO_BITMAP* lixeira = al_load_bitmap("imagens/azul.png");
	ALLEGRO_BITMAP* imagem1 = al_load_bitmap("imagens/inicio.png");
	ALLEGRO_BITMAP* imagem2 = al_load_bitmap("imagens/historia.png");
	ALLEGRO_BITMAP* imagem3 = al_load_bitmap("imagens/gameover.png");
	ALLEGRO_BITMAP* imagem4 = al_load_bitmap("imagens/comandos.png");
	ALLEGRO_BITMAP* imagem5 = al_load_bitmap("imagens/congratulations.png");
	ALLEGRO_BITMAP* pergunta = al_load_bitmap("perguntas/1/1.png");
	ALLEGRO_BITMAP* pergunta2 = al_load_bitmap("perguntas/2/1.png");
	ALLEGRO_BITMAP* pergunta3 = al_load_bitmap("perguntas/3/1.png");
	ALLEGRO_BITMAP* pergunta4 = al_load_bitmap("perguntas/4/1.png");
	ALLEGRO_BITMAP* pergunta5 = al_load_bitmap("perguntas/5/1.png");
	ALLEGRO_BITMAP* pergunta6 = al_load_bitmap("perguntas/6/1.png");
	ALLEGRO_BITMAP* pergunta7 = al_load_bitmap("perguntas/7/1.png");
	ALLEGRO_BITMAP* pergunta8 = al_load_bitmap("perguntas/8/1.png");
	ALLEGRO_BITMAP* enemy = al_load_bitmap("trash.png");
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* timer2 = al_create_timer(1);
	ALLEGRO_TIMER* timer3 = al_create_timer(1);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//---------------- TRILHA SONORA MENU ---------------------
	ALLEGRO_SAMPLE* sound_menu = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_sound_menu = NULL;

	sound_menu = al_load_sample("sound_menu.ogg");
	inst_sound_menu = al_create_sample_instance(sound_menu);
	al_play_sample_instance(inst_sound_menu);



	al_attach_sample_instance_to_mixer(inst_sound_menu, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_sound_menu, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_sound_menu, 1);


	//---------------- EFEITO SONORO PROJETIL ------------------

	ALLEGRO_SAMPLE* projeteis_lancados = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_projeteis_lancados = NULL;
	ALLEGRO_SAMPLE* sound_projetil = NULL;
	ALLEGRO_SAMPLE_INSTANCE* inst_sound_projetil = NULL;


	sound_projetil = al_load_sample("sound_bullet.ogg");
	inst_sound_projetil = al_create_sample_instance(sound_projetil);
	

	al_attach_sample_instance_to_mixer(inst_sound_projetil, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_sound_projetil, ALLEGRO_PLAYMODE_BIDIR);
	al_set_sample_instance_gain(inst_sound_projetil, 0.5);


	//---------------- TRILHA SONORA ------------------
	ALLEGRO_SAMPLE* trilha_sonora = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;

	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);

	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 0.9);



	//---------------- EFEITO SONORO DE ANDAR ------------------
	ALLEGRO_SAMPLE* andar = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_andar = NULL;

	andar = al_load_sample("andar.ogg");
	inst_andar = al_create_sample_instance(andar);

	al_attach_sample_instance_to_mixer(inst_andar, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_andar, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_andar, 0.9);


	//---------------- EFEITO SONORO GAME OVER ------------------
	ALLEGRO_SAMPLE* finaldejogo = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_finaldejogo = NULL;

	finaldejogo = al_load_sample("finaldejogo.ogg");
	inst_finaldejogo = al_create_sample_instance(finaldejogo);

	al_attach_sample_instance_to_mixer(inst_finaldejogo, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_finaldejogo, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_gain(inst_finaldejogo, 1);


	//---------------- EFEITO SONORO BOSS FIGHT ------------------
	ALLEGRO_SAMPLE* bossfight = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_bossfight = NULL;

	bossfight = al_load_sample("bossfight.ogg");
	inst_bossfight = al_create_sample_instance(bossfight);

	al_attach_sample_instance_to_mixer(inst_bossfight, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_bossfight, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_bossfight, 0.8);


	//---------------- EFEITO SONORO WINNER ------------------
	ALLEGRO_SAMPLE* winner = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_winner = NULL;

	winner = al_load_sample("winner.ogg");
	inst_winner = al_create_sample_instance(winner);

	al_attach_sample_instance_to_mixer(inst_winner, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_winner, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_winner, 2);
	/*
	//---------------- EFEITO SONORO DAMAGE ------------------
	ALLEGRO_SAMPLE* trilha_sonora = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;

	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);

	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 2);


	//---------------- EFEITO SONORO FIM DO JOGO ------------------
	ALLEGRO_SAMPLE* trilha_sonora = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;

	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);

	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 2);



	//---------------- EFEITO SONORO TRILHA DO JOGO ------------------
	ALLEGRO_SAMPLE* trilha_sonora = NULL;

	ALLEGRO_SAMPLE_INSTANCE* inst_trilha_sonora = NULL;

	trilha_sonora = al_load_sample("trilha_sonora.ogg");
	inst_trilha_sonora = al_create_sample_instance(trilha_sonora);

	al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
	al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(inst_trilha_sonora, 2);
	*/


	
	//LOOP CONTENDO A LOGICA DO JOGO
	srand(time(NULL));
	al_start_timer(timer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(imagem1, 0, 0, NULL);
	al_flip_display();

	while (!done)
	{
		int a = al_get_timer_count(timer2);
		int t_winner = al_get_timer_count(timer3);

		ALLEGRO_TIMEOUT* tempowinner;

		cout << t_winner << endl;
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		al_get_keyboard_state(&keyState);
		

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
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_LEFT:
				tiros[ESQUERDA] = true;
				AtiraBalas(balas_e, NUM_BALAS_E, personagem, NUM_ATIRADOR, tiros, "personagem", 1);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_DOWN:
				tiros[BAIXO] = true;
				AtiraBalas(balas_b, NUM_BALAS_B, personagem, NUM_ATIRADOR, tiros, "personagem", 4);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_UP:
				tiros[CIMA] = true;
				AtiraBalas(balas_c, NUM_BALAS_C, personagem, NUM_ATIRADOR, tiros, "personagem", 3);
				al_stop_sample_instance(inst_sound_projetil);
				al_play_sample_instance(inst_sound_projetil);
				break;
			case ALLEGRO_KEY_ENTER:
				tiros[ENTER] = true;
				break;
			case ALLEGRO_KEY_1:
				tiros[UM] = true;
				break;
			case ALLEGRO_KEY_2:
				tiros[DOIS] = true;
				break;
			case ALLEGRO_KEY_3:
				tiros[TRES] = true;
				break;
			case ALLEGRO_KEY_F1:
				tiros[F1] = true;
				r = 0;
				g = 177;
				b = 78;
				verde = true;
				corBala = 1;
				break;
			case ALLEGRO_KEY_F2:
				tiros[F2] = true;
				r = 195;
				g = 52;
				b = 48;
				vermelho = true;
				corBala = 2;
				break;
			case ALLEGRO_KEY_F3:
				tiros[F3] = true;
				r = 249;
				g = 189;
				b = 0;
				amarelo = true;
				corBala = 3;
				break;
			case ALLEGRO_KEY_F4:
				tiros[F4] = true;
				r = 0;
				g = 138;
				b = 200;
				azul = true;
				corBala = 4;
				break;
			case ALLEGRO_KEY_F5:
				tiros[F5] = true;
				r = 105;
				g = 66;
				b = 54;
				marrom = true;
				corBala = 5;
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

					BalaColidida(balas, NUM_BALASATIRADOR, personagem, NUM_PERSONAGEM, "atirador", &pontos, corBala, corAtirador);
					BalaColidida(balas_b, NUM_BALAS_B, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_c, NUM_BALAS_C, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_d, NUM_BALAS_D, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					BalaColidida(balas_e, NUM_BALAS_E, atirador, NUM_ATIRADOR, "personagem", &pontos, corBala, corAtirador);
					
				}
				
		}
		//DESENHO DO PERSONAGEM, INIMIGO, TELA, TEXTO E ETC
		if (draw)
		{	
			if (tiros[ENTER])
			{
				al_start_timer(timer2);
<<<<<<< HEAD
				if(a < 1)
=======
				if(a < 15)
>>>>>>> 0e2d78cb2eb14b4921f439d669b4e6d0a9081155
				{
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_bitmap(imagem2, 0, 0, NULL);
					al_flip_display();
				}
<<<<<<< HEAD
				if (a > 2 && a < 3)
=======
				if (a > 15 && a < 25)
>>>>>>> 0e2d78cb2eb14b4921f439d669b4e6d0a9081155
				{
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_bitmap(imagem4, 0, 0, NULL);
					al_flip_display();
				}
<<<<<<< HEAD
				if(a > 4)
=======
				if(a > 30)
>>>>>>> 0e2d78cb2eb14b4921f439d669b4e6d0a9081155
					iniciar = true;
			}

			if (iniciar)
			{
				al_draw_bitmap(mapa, 0, 0, NULL);


				al_stop_sample_instance(inst_sound_menu);
				al_play_sample_instance(inst_trilha_sonora);
				


				if (!gameover && !proximafase)
				{
				

					DesenhaBalas(balas_c, NUM_BALAS_C, 5, r, g, b);
					DesenhaBalas(balas_b, NUM_BALAS_B, 5, r, g, b);
					DesenhaBalas(balas_e, NUM_BALAS_E, 5, r, g, b);
					DesenhaBalas(balas_d, NUM_BALAS_D, 5, r, g, b);
					DesenhaAtirador(enemy, playerWalk, atirador, NUM_ATIRADOR, "atirador");
					DesenhaBalas(balas, NUM_BALASATIRADOR, 8, r2, g2, b2);
					DesenhaAtirador(enemy, playerWalk, personagem, NUM_PERSONAGEM, "personagem"); 
					al_draw_textf(font, al_map_rgb(255, 255, 255), 165, 15, NULL, "%d", personagem->vida);
					al_draw_filled_circle(280, 25, 10, al_map_rgb(r, g, b));
					al_draw_bitmap(lixeira, 240, 8, NULL);
					al_draw_textf(font, al_map_rgb(255, 255, 255), 430, 12, NULL, "%d", fase);
					al_draw_textf(font, al_map_rgb(255, 255, 255), 580, 15, NULL, "%d", pontos);
					if (personagem->vida <= 0)
						gameover = true;
					contador = pontos;
					if (contador == 1 && fase == 1)
					{

						proximafase = true;
						contador = 0;
					}
					if (contador == 4 && fase == 2)
					{	


						proximafase = true;
						contador = 0;
					}
					if (contador == 7 && fase == 3)
					{
						

						proximafase = true;
						contador = 0;
					}
					if (contador == 11 && fase == 4)
					{

						proximafase = true;
						contador = 0;
					}
					if (contador == 15 && fase == 5)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 20 && fase == 6)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 24 && fase == 7)
					{
						proximafase = true;
						contador = 0;
					}
					if (contador == 29 && fase == 8)
					{
						proximafase = true;
						contador = 0;
					}
					if(contador == 30 && fase == 9)
					{
						fimdejogo = true;
						contador = 0;
					}
				}
				else
				{
					if (gameover)
					{ 
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(imagem3, 0, 0, NULL);
						al_flip_display();
						//al_draw_textf(font2, al_map_rgb(255, 255, 255), largura / 2, altura / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER: VOCE PERDEU TODAS AS VIDAS, COMECE NOVAMENTE");
						al_draw_textf(font2, al_map_rgb(255, 255, 255), largura / 2, 360, ALLEGRO_ALIGN_CENTRE, "SEUS PONTOS FORAM: %d", pontos);
					}
					if (proximafase && fase == 1)
					{	
						al_play_sample_instance(inst_winner);
						al_start_timer(timer3);

						
						if (t_winner > 1) {
							al_stop_sample_instance(inst_winner);
							al_set_timer_count(timer3, NULL);
							printf("---------------------------------------------------");
						}
						

						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta, 0, 0, NULL);
						al_flip_display();

						

						if (tiros[UM] && !respondido[0])
						{
							al_stop_sample_instance(inst_winner);
							tiros[UM] = false;
							respondido[0] = true;
							personagem->vida++;
<<<<<<< HEAD

							pergunta = al_load_bitmap("imagens/pergunta1-resp.png");
=======
							pergunta = al_load_bitmap("perguntas/1/4.png");
>>>>>>> 0e2d78cb2eb14b4921f439d669b4e6d0a9081155
						}
						if (tiros[DOIS] && !respondido[0])
						{
							al_stop_sample_instance(inst_winner);
							tiros[DOIS] = false;
							respondido[0] = true;
							pergunta = al_load_bitmap("perguntas/1/2.png");
						}
						if (tiros[TRES] && !respondido[0])
						{
							al_stop_sample_instance(inst_winner);
							tiros[TRES] = false;
							respondido[0] = true;
							pergunta = al_load_bitmap("perguntas/1/3.png");
						}
						
					}
					if (proximafase && fase == 2)
<<<<<<< HEAD
					{	
						

						al_set_timer_count(timer3, NULL);
						al_start_timer(timer3);
						al_play_sample_instance(inst_winner);


						if (t_winner > 1) {
							al_stop_sample_instance(inst_winner);
							printf("///////////////////////////////////////////");
						}

=======
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta2, 0, 0, NULL);
						al_flip_display();
>>>>>>> 0e2d78cb2eb14b4921f439d669b4e6d0a9081155
						if (tiros[UM] && !respondido[1])
						{
							tiros[UM] = false;
							respondido[1] = true;
							pergunta2 = al_load_bitmap("perguntas/2/2.png");
						}
						if (tiros[DOIS] && !respondido[1])
						{
							tiros[DOIS] = false;
							respondido[1] = true;
							pergunta2 = al_load_bitmap("perguntas/2/3.png");
						}
						if (tiros[TRES] && !respondido[1])
						{
							tiros[TRES] = false;
							respondido[1] = true;
							personagem->vida++;
							pergunta2 = al_load_bitmap("perguntas/2/4.png");
						}

					}
					if (proximafase && fase == 3)
					{
						
						

						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta3, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[2])
						{
							tiros[UM] = false;
							respondido[2] = true;
							pergunta3 = al_load_bitmap("perguntas/3/2.png");
						}
						if (tiros[DOIS] && !respondido[2])
						{
							tiros[DOIS] = false;
							respondido[2] = true;
							personagem->vida++;
							pergunta3 = al_load_bitmap("perguntas/3/4.png");
						}
						if (tiros[TRES] && !respondido[2])
						{
							tiros[TRES] = false;
							respondido[2] = true;
							pergunta3 = al_load_bitmap("perguntas/3/3.png");
						}

					}

					if (proximafase && fase == 4)
					{

						

						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta4, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[3])
						{
							tiros[UM] = false;
							respondido[3] = true;
							pergunta4 = al_load_bitmap("perguntas/4/2.png");
						}
						if (tiros[DOIS] && !respondido[3])
						{
							tiros[DOIS] = false;
							respondido[3] = true;
							pergunta4 = al_load_bitmap("perguntas/4/3.png");
						}
						if (tiros[TRES] && !respondido[3])
						{
							tiros[TRES] = false;
							respondido[3] = true;
							personagem->vida++;
							pergunta4 = al_load_bitmap("perguntas/4/4.png");
						}

					}

					if (proximafase && fase == 5)
					{

					

						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta5, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[4])
						{
							tiros[UM] = false;
							respondido[4] = true;
							pergunta5 = al_load_bitmap("perguntas/5/2.png");
						}
						if (tiros[DOIS] && !respondido[4])
						{
							tiros[DOIS] = false;
							respondido[4] = true;
							personagem->vida++;
							pergunta5 = al_load_bitmap("perguntas/5/4.png");
						}
						if (tiros[TRES] && !respondido[4])
						{
							tiros[TRES] = false;
							respondido[4] = true;
							pergunta5 = al_load_bitmap("perguntas/5/3.png");
						}

					}

					if (proximafase && fase == 6)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta6, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[5])
						{
							tiros[UM] = false;
							respondido[5] = true;
							personagem->vida++;
							pergunta6 = al_load_bitmap("perguntas/6/4.png");
						}
						if (tiros[DOIS] && !respondido[5])
						{
							tiros[DOIS] = false;
							respondido[5] = true;
							pergunta6 = al_load_bitmap("perguntas/6/2.png");
						}
						if (tiros[TRES] && !respondido[5])
						{
							tiros[TRES] = false;
							respondido[5] = true;
							pergunta6 = al_load_bitmap("perguntas/6/3.png");
						}

					}

					if (proximafase && fase == 7)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta7, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[6])
						{
							tiros[UM] = false;
							respondido[6] = true;
							pergunta7 = al_load_bitmap("perguntas/7/2.png");
						}
						if (tiros[DOIS] && !respondido[6])
						{
							tiros[DOIS] = false;
							respondido[6] = true;
							personagem->vida++;
							pergunta7 = al_load_bitmap("perguntas/7/4.png");
						}
						if (tiros[TRES] && !respondido[6])
						{
							tiros[TRES] = false;
							respondido[6] = true;
							pergunta7 = al_load_bitmap("perguntas/7/3.png");
						}

					}

					if (proximafase && fase == 8)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(pergunta8, 0, 0, NULL);
						al_flip_display();
						if (tiros[UM] && !respondido[6])
						{
							tiros[UM] = false;
							respondido[7] = true;
							pergunta8 = al_load_bitmap("perguntas/8/2.png");
						}
						if (tiros[DOIS] && !respondido[6])
						{
							tiros[DOIS] = false;
							respondido[7] = true;
							pergunta8 = al_load_bitmap("perguntas/8/3.png");
						}
						if (tiros[TRES] && !respondido[6])
						{
							tiros[TRES] = false;
							respondido[7] = true;
							personagem->vida++;
							pergunta8 = al_load_bitmap("perguntas/8/4.png");
						}

					}

					if (proximafase && fase == 9)
					{
						al_clear_to_color(al_map_rgb(0, 0, 0));
						al_draw_bitmap(imagem5, 0, 0, NULL);
						al_flip_display();

					}



					if (tiros[ENTER])
					{
						if (gameover)
						{
							done = true;
							/*
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
							pontos = 0;*/
						}
						if (proximafase && fase == 1)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 4;
							fase++;
							mapa = al_load_bitmap("mapas/bosque2.png");
							enemy = al_load_bitmap("trash.png");
							//lixeira = al_load_bitmap("imagens/azul.png");
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 7, "atirador", 2, 0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 2)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 3;
							fase++;
							mapa = al_load_bitmap("mapas/cidade1.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/amarela.png");
							r2 = 249, g2 = 189, b2 = 0;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 7, "atirador", 2, 1.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 3)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 3;
							fase++;
							mapa = al_load_bitmap("mapas/cidade2.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/amarela.png");
							r2 = 249, g2 = 189, b2 = 0;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 3, 1.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 2, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 4)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 2;
							fase++;
							mapa = al_load_bitmap("mapas/deserto1.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/vermelha.png");
							r2 = 195, g2 = 52, b2 = 48;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 3, 1.5, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 5)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 2;
							fase++;
							mapa = al_load_bitmap("mapas/deserto2.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/vermelha.png");
							r2 = 195, g2 = 52, b2 = 48;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 5, "atirador", 3, 1.5, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 6)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 1;
							fase++;
							mapa = al_load_bitmap("mapas/lixao1.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/verde.png");
							r2 = 0, g2 = 177, b2 = 78;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 6, "atirador", 4, 2.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 7)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 1;
							fase++;
							mapa = al_load_bitmap("mapas/lixao2.png");
							enemy = al_load_bitmap("trash.png");
							lixeira = al_load_bitmap("imagens/verde.png");
							r2 = 0, g2 = 177, b2 = 78;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 5, "atirador", 4, 2.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 3, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}
						if (proximafase && fase == 8)
						{
							personagem->x = 50;
							personagem->y = 50;
							corAtirador = 5;
							fase++;
							mapa = al_load_bitmap("mapas/lixao3.png");
							enemy = al_load_bitmap("inimigos/boss1.png");
							lixeira = al_load_bitmap("imagens/marrom.png");
							r2 = 105, g2 = 66, b2 = 54;
							InitBalas(balas_c, NUM_BALAS_C, "personagem", 10, corBala);
							InitBalas(balas_b, NUM_BALAS_B, "personagem", 10, corBala);
							InitBalas(balas_e, NUM_BALAS_E, "personagem", 10, corBala);
							InitBalas(balas_d, NUM_BALAS_D, "personagem", 10, corBala);
							InitAtirador(atirador, NUM_ATIRADOR - 9, "atirador", 10, 3.0, corAtirador);
							InitBalas(balas, NUM_BALASATIRADOR, "atirador", 10, 0);
							proximafase = false;
							tiros[ENTER] = false;
						}

					}

				}
				al_flip_display();
			}
		
			
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
	al_destroy_font(font2);
	al_destroy_sample(trilha_sonora);
	al_destroy_sample(projeteis_lancados);
	al_destroy_sample_instance(inst_trilha_sonora);
	al_destroy_sample_instance(inst_projeteis_lancados);

	return 0;






}

/*



*/
