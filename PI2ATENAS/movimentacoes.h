#include <iostream>
#include <cstdlib>
using namespace std;

void move_personagem(ALLEGRO_BITMAP* player, ALLEGRO_KEYBOARD_STATE keyState, int alt, int larg, float *x, float *y, int *dir, float *moveSpeed, bool *active, int *sourceX, int *sourceY, bool *draw) {

	enum Direction { DOWN, LEFT, RIGHT, UP };
	//LOGICA DE MOVIMENTACAO DO PERSONAGEM
	
	if (al_key_down(&keyState, ALLEGRO_KEY_S) && *y < alt - 30)
	{
		*y += *moveSpeed;
		*dir = DOWN;
		cout << "(" << x << "," << y << ")" << endl;
		//cout << " , ";
		//cout << y;
	}
	else if (al_key_down(&keyState, ALLEGRO_KEY_W) && *y > 0)
	{
		*y -= *moveSpeed;
		*dir = UP;
		cout << "(" << x << "," << y << ")" << endl;
		//cout << " , ";
		//cout << y;
	}
	else if (al_key_down(&keyState, ALLEGRO_KEY_D) && *x < larg - 30)
	{
		*x += *moveSpeed;
		*dir = RIGHT;
		cout << "(" << x << "," << y << ")" << endl;
		//cout << " , ";
		//cout << y;
	}
	else if (al_key_down(&keyState, ALLEGRO_KEY_A) && *x > 0)
	{
		*x -= *moveSpeed;
		*dir = LEFT;
		cout << "(" << x << "," << y << ")" << endl;
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


}

//FUNCAO DE MOVIMENTO DO INIMIGO
void move_inimigo(int* x, int* y, int* dir, int alt, int larg, ALLEGRO_BITMAP* inimigo)
{

	if (*dir == 1 && *x != 20 && *y != 20)
	{
		--* x;
		--* y;
	}
	else if (*dir == 2 && *x != 20 && *y != alt - 50)
	{
		--* x;
		++* y;
	}
	else if (*dir == 3 && *x != larg - 50 && *y != 20)
	{
		++* x;
		--* y;
	}
	else if (*dir == 4 && *x != larg - 50 && *y != alt - 50)
	{
		++* x;
		++* y;
	}
	else
	{
		*dir = rand() % 4 + 1;
	}

}
