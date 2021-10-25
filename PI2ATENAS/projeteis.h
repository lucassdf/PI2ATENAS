// ------ ARQUIVO OBJETOS -------


enum IDS { PROJETEISATIRADOR };

struct Projeteis
{
	int ID;
	int x;
	int y;
	int velocidade;
	bool ativo;
};

struct Atirador {
	int ID;
	int x;
	int y;
	int velocidade;
	int borda_x;
	int borda_y;
	bool ativo;
};

struct ProjeteisAtirador
{
	int ID;
	int x;
	int y;
	int velocidade;
	bool ativo;
};

// ---------- PROT�TIPOS -------------

void InitBalas(Projeteis balas_c[], Projeteis balas_b[], Projeteis balas_e[], Projeteis balas_d[], int tamanho);

void AtiraBalas(Projeteis balas[], int tamanho, float x, float y, bool tiros[]);

void AtualizarBalasCima(Projeteis balas[], int tamanho, bool tiros[]);
void AtualizarBalasBaixo(Projeteis balas[], int tamanho, bool tiros[]);
void AtualizarBalasEsquerda(Projeteis balas[], int tamanho, bool tiros[]);
void AtualizarBalasDireita(Projeteis balas[], int tamanho, bool tiros[]);

void DesenhaBalas(Projeteis balas[], int tamanho);

enum IDS { JOGADOR, PROJETIL, INIMIGOS };
enum TIROS { CIMA, BAIXO, ESQUERDA, DIREITA };

// ------------- PROJETEIS ----------------



void InitBalas(Projeteis balas_c[], Projeteis balas_b[], Projeteis balas_e[], Projeteis balas_d[], int tamanho)
{
	for (int i = 0; i < tamanho; i++) {
		balas_c[i].ID = PROJETIL;
		balas_c[i].velocidade = 10;
		balas_c[i].ativo = false;
	}
}



void AtiraBalas(Projeteis balas[], int tamanho, float x, float y, bool tiros[])
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

void AtualizarBalasBaixo(Projeteis balas[], int tamanho, bool tiros[])
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			balas[i].y += balas[i].velocidade;

			if (balas[i].y > 480)
				balas[i].ativo = false;

		}


	}
}

void AtualizarBalasCima(Projeteis balas[], int tamanho, bool tiros[])
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			balas[i].y -= balas[i].velocidade;

			if (balas[i].y < 0)
				balas[i].ativo = false;

		}

	}
}

void AtualizarBalasEsquerda(Projeteis balas[], int tamanho, bool tiros[])
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

void AtualizarBalasDireita(Projeteis balas[], int tamanho, bool tiros[])
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			balas[i].x += balas[i].velocidade;

			if (balas[i].x > 680)
				balas[i].ativo = false;

		}


	}
}



void DesenhaBalas(Projeteis balas[], int tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (balas[i].ativo)
		{
			al_draw_filled_circle(balas[i].x, balas[i].y, 5, al_map_rgb(0, 0, 0));
		}
	}
}