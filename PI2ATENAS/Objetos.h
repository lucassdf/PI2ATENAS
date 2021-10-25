// ------ ARQUIVO OBJETOS -------

enum IDS { JOGADOR, PROJETIL, INIMIGOS, PROJETEISATIRADOR };

struct Projeteis
{
	int ID;
	int x;
	int y;
	int velocidade;
	bool ativo_c;
	bool ativo_b;
	bool ativo_d;
	bool ativo_e;
};

struct  Atirador
{
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
