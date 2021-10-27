// ------ ARQUIVO OBJETOS -------
#include <string>

enum IDS { JOGADOR, PROJETIL, INIMIGOS, PROJETEISATIRADOR};
enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA };

struct Projeteis
{
	int ID;
	int x;
	int y;
	int velocidade;
	int dir;
	bool ativo;
};

struct Atirador {
	int ID;
	int x;
	int y;
	int velocidade;
	int borda_x;
	int borda_y;
	int dir;
	int vida;
	int cont;
	bool ativo;
};

// ---------- PROT�TIPOS -------------

void AtiraBalas(Projeteis balas[], int tamanho, Atirador atirador[], int tamanho_a, float x, float y, bool tiros[], string tipo);
void DesenhaBalas(Projeteis balas[], int tamanho);
void BalaColidida(Projeteis balas[], int b_tamanho, int c_tamanho, int x, int y);
//void ColisaoPersonagem(Atirador atirador, int tamanho_a,;
void InitAtirador(Atirador atirador[], int tamanho);
void LiberaTiros(Atirador atirador[], int tamanho);
void AtualizaAtirador(Atirador atirador[], int tamanho);
void InitBalas(Projeteis balas[], int tamanho, string tipo);
void AtualizaBalas(Projeteis balas[], int tamanho);
void DesenhaAtirador(Atirador atirador[], int tamanho);

// ------------- PROJETEIS ----------------
/*int ColisaoPersonagem(Atirador atirador, int tamanho_a, int x, int y, int borda_x, int borda_y)
{
	for (int i = 0; i < tamanho_a; i++)
	{
		if (atirador[i].ativo)
		{
			if ((atirador[i].x - atirador[i].borda_x) < (x + borda_x) &&
				(atirador[i].x + atirador[i].borda_x) > (x - borda_x) &&
				(atirador[i].y - atirador[i].borda_y) < (y + borda_y) &&
				(atirador[i].y + atirador[i].borda_y) > (y - borda_y))
				return 0;
		}
	}


}*/
void BalaColidida(Projeteis balas[], int b_tamanho, Atirador atirador[], int c_tamanho)
{
	for (int i = 0; i < b_tamanho; i++)
	{
		if (balas[i].ativo)
		{
			for (int j = 0; j < c_tamanho; j++)
			{
				if (atirador[j].ativo)
				{
					if (balas[i].x > (atirador[j].x - atirador[j].borda_x) &&
						balas[i].x < (atirador[j].x + atirador[j].borda_x) &&
						balas[i].y >(atirador[j].y - atirador[j].borda_y) &&
						balas[i].y < (atirador[j].y + atirador[j].borda_y))
					{
						balas[i].ativo = false;
						--atirador[j].vida;
						if (atirador[j].vida <= 0)
						{
							atirador[j].ativo = false;
						}

					}
				}
			}
		}
	}
}

void InitAtirador(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		atirador[i].ID = INIMIGOS;
		atirador[i].velocidade = 5;
		atirador[i].borda_x = 18;
		atirador[i].borda_y = 18;
		atirador[i].vida = 3;
		atirador[i].ativo = false;
	}
}

void AtiraBalas(Projeteis balas[], int tamanho, Atirador atirador[], int tamanho_a, float x, float y, bool tiros[], string tipo)
{
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!balas[i].ativo)
			{
				balas[i].x = x + 23;
				balas[i].y = y;
				balas[i].ativo = true;
				break;
			}

		}
	}
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (!balas[i].ativo && atirador[i].vida > 0)
			{
				for (int j = 0; j < tamanho_a; j++)
				{
					balas[i].x = atirador[i].x;
					balas[i].y = atirador[i].y;
					balas[i].dir = rand() % 4 + 1;
					balas[i].ativo = true;
					break;
				}
			}

		}
	}

}

void LiberaTiros(Atirador atirador[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (!atirador[i].ativo && atirador[i].vida > 0)
		{
			atirador[i].x = rand() % 400 + 30;
			atirador[i].y = rand() % 400 + 30;
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

void InitBalas(Projeteis balas[], int tamanho, string tipo)
{
	if (tipo == "atirador")
	{
		for (int i = 0; i < tamanho; i++) {
			balas[i].ID = PROJETEISATIRADOR;
			balas[i].velocidade = 2;
			balas[i].ativo = false;
		}
	}
	if (tipo == "personagem")
	{
		for (int i = 0; i < tamanho; i++) {
			balas[i].ID = PROJETIL;
			balas[i].velocidade = 10;
			balas[i].ativo = false;
		}
	}
}

void AtualizaBalas(Projeteis balas[], int tamanho, int dir)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			//balas[i].dir = 4;
			if (balas[i].dir == 1 || dir == 1)
				balas[i].x -= balas[i].velocidade;
			else if (balas[i].dir == 2 || dir == 2)
				balas[i].x += balas[i].velocidade;
			else if (balas[i].dir == 3 || dir == 3)
				balas[i].y -= balas[i].velocidade;
			else if (balas[i].dir == 4 || dir == 4)
				balas[i].y += balas[i].velocidade;

			if (balas[i].x < 20 || balas[i].x > 680 || balas[i].y < 20 || balas[i].y > 480)
				balas[i].ativo = false;
		}

	}
}

void AtualizaAtirador(Atirador atirador[], int  alt, int larg, int tamanho)
{
	int opcao = 2;
	for (int i = 0; i < tamanho; i++)
	{
		if (atirador[i].ativo && opcao == 1)
		{
			if (atirador[i].dir == 1 && atirador[i].x != 20)
			{
				--atirador[i].x;
			}
			else if (atirador[i].dir == 2 && atirador[i].y != alt - 50)
			{
				++atirador[i].y;
			}
			else if (atirador[i].dir == 3 && atirador[i].x != larg - 50)
			{
				++atirador[i].x;
			}
			else if (atirador[i].dir == 4 && atirador[i].y != 20)
			{
				--atirador[i].y;
			}
			else
			{
				atirador[i].dir = rand() % 4 + 1;
			}

		}
		if (atirador[i].ativo && opcao == 2)
		{
			if (atirador[i].dir == 1 && atirador[i].x != 20 && atirador[i].y != 20)
			{
				--atirador[i].x;
				--atirador[i].y;
			}
			else if (atirador[i].dir == 2 && atirador[i].x != 20 && atirador[i].y != alt - 20)
			{
				--atirador[i].x;
				++atirador[i].y;
			}
			else if (atirador[i].dir == 3 && atirador[i].x != larg - 20 && atirador[i].y != 20)
			{
				++atirador[i].x;
				--atirador[i].y;
			}
			else if (atirador[i].dir == 4 && atirador[i].x != larg - 20 && atirador[i].y != alt - 20)
			{

				++atirador[i].x;
				++atirador[i].y;
			}
			else
			{
				atirador[i].dir = rand() % 4 + 1;
			}
		}

	}
}

void DesenhaAtirador(Atirador atirador[], int tamanho)
{
	//ALLEGRO_BITMAP* enemy = al_load_bitmap("trash.png");
	for (int i = 0; i < tamanho; i++)
	{
		if (atirador[i].ativo)
		{
			//al_draw_bitmap(enemy, atirador[i].x, atirador[i].y, NULL);
			al_draw_filled_circle(atirador[i].x, atirador[i].y, 20, al_map_rgb(0, 128, 0));
		}
	}
}

void DesenhaBalas(Projeteis balas[], int tamanho, int raio, int rgb1, int rgb2, int rgb3)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			al_draw_filled_circle(balas[i].x, balas[i].y, raio, al_map_rgb(rgb1, rgb2, rgb3));
		}
	}
}
