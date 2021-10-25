//inimigo.h

//FUNCAO DE MOVIMENTO DO INIMIGO
void move_inimigo(int *x, int *y, int *dir, int alt, int larg, ALLEGRO_BITMAP* inimigo)
{

	if (*dir == 1 && *x != 20 && *y != 20)
	{
		--*x;
		--*y;
	}
	else if (*dir == 2 && *x != 20 && *y != alt - 50)
	{
		--*x;
		++*y;
	}
	else if (*dir == 3 && *x != larg - 50 && *y != 20)
	{
		++*x;
		--*y;
	}
	else if (*dir == 4 && *x != larg - 50 && *y != alt - 50)
	{
		++*x;
		++*y;
	}
	else
	{
		*dir = rand() % 4 + 1;
	}

}

