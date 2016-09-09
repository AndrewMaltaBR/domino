#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct _piece
{
	int right;
	int left;
	struct _piece* ante;
	struct _piece* prox;
}piece;

piece * init_lot(piece * p);
piece * init_player(piece * p,piece ** l);
piece * inserir_fim(piece * p,int i,int j);
void trocar(piece ** p,piece ** l,int i,int j,bool ini);
piece * inserir_ini(piece * p,int i,int j);
piece * deletar(piece * p,int i,int j);
piece * limpar(piece * p);
void mostrar(piece * p);
int contar(piece * p);
piece * buscar(piece * p,int i,int j);
void jogo(piece **l,piece **p,piece **b,piece **t);

int main()
{
	piece *lot=NULL,*player=NULL,*boot=NULL,*table=NULL;
	bool menu=true;

	while(menu)
	{
		system("clear");
		char escolha;
		printf("\n  //------------------------//");
		printf("\n  //         Dominó         //");
		printf("\n  //------------------------//");
		printf("\n  1 - Iniciar jogo 2 - Sair");
		printf("\n  Sua escolha: ");
		scanf("%c",&escolha);
		switch(escolha)
		{
			case '1':
				jogo(&lot,&player,&boot,&table);
			break;
			case '2':
				menu = false;
				system("clear");
			break;
		}
	}

	lot = init_lot(lot);
	player = init_player(player,&lot);
	boot = init_player(boot,&lot);

	return 0;
}

piece * init_lot(piece * p)
{
	int i,j;
	p = NULL;
	for(i=6;i>=0;i--)
		for(j=i;j>=0;j--)
			p = inserir_fim(p,i,j);

	return p;
}

piece * init_player(piece * p,piece ** l)
{
	int i,j;
	for(i=0;i<6;i++)
	{
		srand((unsigned)time(NULL));
		int n = rand() % (contar(*l)-1);
		piece * m = *l;
		for(j=0;j<=n;j++)
		{
			if(j == n)
				trocar(&p,l,m->right,m->left,false);
			m = m->prox;
		}
	}
	return p;
}

piece * inserir_fim(piece * p,int i,int j)
{
	if(p == NULL)
		return inserir_ini(p,i,j);
	piece * last = p;
	while(last->prox != NULL)
		last = last->prox;
	piece * new = (piece *) malloc(sizeof(piece));
	new->right = i;
	new->left = j;
	new->prox = NULL;
	new->ante = last;
	last->prox = new;
	return p;
}

piece * inserir_ini(piece * p,int i,int j)
{
	piece * new = (piece *) malloc(sizeof(piece));
	new->right = i;
	new->left = j;
	new->ante = NULL;
	new->prox = p;
	return new;
}

void trocar(piece ** p,piece ** l,int i,int j,bool ini)
{
	if(ini)
		*p = inserir_ini(*p,i,j);
	else
		*p = inserir_fim(*p,i,j);
	*l = deletar(*l,i,j);
}

piece * deletar(piece * p,int i,int j)
{
	piece * l = p;

	while((l != NULL) && ((l->right != i) || (l->left != j)))
		l = l->prox;
	if(l == NULL)
		return p;
	if (l == p)
	{
		p = l->prox;
		p->ante = NULL;
		free(l);
		return p;
	}
	else
	{
		if (l->ante != NULL)
			l->ante->prox = l->prox;
		if (l->prox != NULL)
			l->prox->ante = l->ante;
	}
	free(l);
	return p;
}

piece * limpar(piece * p)
{
	piece * n;
	for(n=p;n!=NULL;n=n->prox)
		p = deletar(p,n->right,n->left);
	return p;
}

void mostrar(piece * p)
{
	piece * n;
	printf("\n");
	for(n=p;n!=NULL;n=n->prox)
		printf("|%d|%d|",n->right,n->left);
	printf("\n");
}

int contar(piece * p)
{
	int i=0;
	piece * n = p;
	while(n != NULL)
	{
		i++;
		n = n->prox;
	}
	return i;
}

piece * buscar(piece * p,int i,int j)
{
	piece * n = p;
	while((n != NULL) && ((n->right != i) || (n->left != j)))
		n = n->prox;
	if(n == NULL)
		return NULL;
	else
		return n;
}

void jogo(piece **l,piece **p,piece **b,piece **t)
{
	piece *lot=*l,*player=*p,*boot=*b,*table=*t;
	bool jogando = true;
	// Inicializando //
	lot = init_lot(lot);
	player = init_player(player,l);
	boot = init_player(boot,l);
	// Tela de jogo //
	while(jogando)
	{
		system("clear");
		mostrar(table);
	}
}