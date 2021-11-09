#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

/*
void move_personagem(ALLEGRO_BITMAP* player, ALLEGRO_KEYBOARD_STATE keyState, Atirador atirador[], int alt, int larg, int *sourceX, int *sourceY, bool *draw)
{

	enum Direction { DOWN, LEFT, RIGHT, UP };
	//LOGICA DE MOVIMENTACAO DO PERSONAGEM

	for (int i = 0; i < tamanho_a; i++)
	{
			if (al_key_down(&keyState, ALLEGRO_KEY_S) && *y < alt - 30)
			{
				*y += *atirador[i].velocidade;
				*dir = DOWN;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_W) && *y > 0)
			{
				*y -= *moveSpeed;
				*dir = UP;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_D) && *x < larg - 30)
			{
				*x += *moveSpeed;
				*dir = RIGHT;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_A) && *x > 0)
			{
				*x -= *moveSpeed;
				*dir = LEFT;
				//cout << "(" << x << "," << y << ")" << endl;
				//cout << " , ";
				//cout << y;
			}
			else
				*active = false;

			if (*active)
				*sourceX += al_get_bitmap_width(player) / 3;
			else
				*sourceX = 32;

			if (*sourceX >= al_get_bitmap_width(player))
				*sourceX = 0;

			*sourceY = *dir;

			*draw = true;
		//}
	//}


}
*/