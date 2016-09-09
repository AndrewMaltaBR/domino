#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
piece * inserir_ini(piece * p,int i,int j);
piece * deletar(piece * p,int i,int j);
void mostrar(piece * p);
int contar(piece * p);
piece * buscar(piece * p,int i,int j);

int main()
{
	system("clear");
	piece *lot=NULL,*player=NULL;//,*boot,*table;
	lot = init_lot(lot);
	mostrar(lot);

	player = init_player(player,&lot);
	mostrar(player);

	mostrar(lot);


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
	srand( (unsigned)time(NULL));
	for(i=0;i<6;i++)
	{
		int n = rand() % (contar(*l)-1);
		piece * m = *l;
		for(j=0;j<=n;j++)
		{
			if(j == n)
			{
				p = inserir_fim(p,m->right,m->left);
				*l = deletar(*l,m->right,m->left);
			}
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

piece * deletar(piece * p,int i,int j)
{
	piece * n = buscar(p,i,j);
	if(n != NULL)
	{
		if(n->prox != NULL)
			n->prox->ante = n->ante;
		if(n->ante != NULL)
			n->ante->prox = n->prox;
		else
			p = n->prox;			
		free(n);
	}
	return p;
}

void mostrar(piece * p)
{
	piece * n;
	printf("\n");
	for(n=p;n!=NULL;n=n->prox)
		printf(" ___ ");
	printf("\n");
	for(n=p;n!=NULL;n=n->prox)
		printf("|%d|%d|",n->right,n->left);
	printf("\n\n");
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
	while((n != NULL) && (n->right != i) && (n->left != j))
		n = n->prox;
	if(n == NULL)
		return NULL;
	else
		return n;
}