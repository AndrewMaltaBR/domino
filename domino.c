#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void trocar_aleatoriamente(piece ** p,piece ** l);
piece * inserir_ini(piece * p,int i,int j);
piece * deletar(piece * p,int i,int j);
void limpar(piece * p);
void mostrar(piece * p);
int contar(piece * p);
piece * buscar_ultimo(piece * p);
bool pode_jogar(piece * p,piece * t,bool ini);
void abrir_menu();
void jogar();
void boot_ia(piece **boot, piece **table, piece **lot,bool *comprar,bool *player_turn);
void testar_vencedor(piece *player,piece *boot,bool *jogando);

int main()
{
	
	abrir_menu();

	system("clear");
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
	int i;
	for(i=0;i<6;i++)
		trocar_aleatoriamente(&p,l);
	return p;
}

piece * inserir_fim(piece * p,int i,int j)
{
	if(p == NULL)
		return inserir_ini(p,i,j);
	piece * last = buscar_ultimo(p);
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

void trocar_aleatoriamente(piece ** p,piece ** l)
{
	srand((unsigned)time(NULL));
	int j,n;
	if(contar(*l) > 1)
		n = rand() % (contar(*l)-1);
	else
		n = 0;
	piece * m = *l;
	for(j=0;j<=n;j++)
	{
		if(j == n)
			trocar(p,l,m->right,m->left,false);
		if(m->prox != NULL)
			m = m->prox;
	}
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
		if(p != NULL)
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

void limpar(piece * p)
{
	if(p != NULL)
	{
		piece *n,*m;
		for(n=p;n->prox!=NULL;n=m)
		{
			m = n->prox;
			p = deletar(p,n->right,n->left);
		}
	}
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

piece * buscar_ultimo(piece * p)
{
	piece * last = p;
	while(last->prox != NULL)
		last = last->prox;
	return last;
}

bool pode_jogar(piece * p,piece * t,bool ini)
{
	bool retorno = false;
	if(t == NULL)
		return true;

	piece *m = t;
	if(!ini)
		m = buscar_ultimo(t);

	piece *l;
	for(l=p;p!=NULL;l=l->prox)
		if((l->right == m->right) || (l->right == m->left) || (l->left == m->right) || (l->left == m->left))
		{
			retorno = true;
			break;
		}

	return retorno;
}

void abrir_menu()
{
	bool menu=true;

	while(menu)
	{
		system("clear");
		int escolha;
		printf("\n  //------------------------//");
		printf("\n  //         Dominó         //");
		printf("\n  //------------------------//");
		printf("\n  1 - Iniciar jogo\n  2 - Sair");
		printf("\n\n  Sua escolha: ");
		scanf("%i",&escolha);
		switch(escolha)
		{
			case 1:
				jogar();
			break;
			case 2:
				menu = false;
				system("clear");
			break;
		}
	}
}

void jogar()
{
	piece *lot=NULL,*player=NULL,*boot=NULL,*table=NULL;
	bool jogando = true, player_turn=true;
	// Inicializando //
	lot = init_lot(lot);
	player = init_player(player,&lot);
	boot = init_player(boot,&lot);
	// Tela de jogo //
	system("clear");
	while(jogando)
	{
		bool comprar = true;
		while(player_turn)
		{
			mostrar(table);
			mostrar(lot);
			mostrar(boot);
			mostrar(player);
			int escolha;
			printf("\n  //------------------------//");
			printf("\n  //     Ações possíveis    //");
			printf("\n  //------------------------//");
			if(pode_jogar(player,table,true))
				printf("\n  1 - Jogar no início");
			if(pode_jogar(player,table,false))
				printf("\n  2 - Jogar no fim");
			if((lot != NULL) && comprar)
				printf("\n  3 - Comprar");
			else
				printf("\n  3 - Passar");
			printf("\n  4 - Desistir");
			printf("\n\n  Sua escolha: ");
			scanf("%i",&escolha);
			switch(escolha)
			{
				case 1:
					if(pode_jogar(player,table,true))
					{
						player_turn = false;
					}
				break;
				case 2:
					if(pode_jogar(player,table,false))
					{
						player_turn = false;
					}
				break;
				case 3:
					if(comprar && (lot != NULL))
					{
						comprar = false;
						trocar_aleatoriamente(&player,&lot);
					}
					else
						player_turn = false;
				break;
				case 4:
					player_turn = false;
					jogando = false;
				break;
			}
			if(!player_turn && jogando)
				comprar = true;
			while(!player_turn && jogando)
				boot_ia(&boot,&table,&lot,&comprar,&player_turn);
			system("clear");
		}
	}
	// Limpando listas //
	limpar(lot);
	limpar(player);
	limpar(boot);
	limpar(table);
}

void boot_ia(piece **boot, piece **table, piece **lot,bool *comprar,bool *player_turn)
{
	// VERSÃO APENAS PARA TESTES //
	if(pode_jogar(*boot,*table,true))
	{
		piece *l,*first=*table;
		for(l=*boot;l!=NULL;l=l->prox)
			if((first == NULL) || (l->right == first->right) || (l->right == first->left) || (l->left == first->right) || (l->left == first->left))
			{
				trocar(table,boot,l->right,l->left,true);
				break;
			}
		*player_turn = true;
	}
	else if(pode_jogar(*boot,*table,false))
	{
		piece *l,*last=buscar_ultimo(*table);
		for(l=*boot;l!=NULL;l=l->prox)
			if((l->right == last->right) || (l->right == last->left) || (l->left == last->right) || (l->left == last->left))
			{
				trocar(table,boot,l->right,l->left,false);
				break;
			}
		*player_turn = true;
	}
	else if(*comprar && (*lot != NULL))
	{
		*comprar = false;
		trocar_aleatoriamente(boot,lot);
	}
	else
		*player_turn = true;
}

void testar_vencedor(piece *player,piece *boot,bool *jogando)
{
	if((player == NULL) || (boot == NULL))
	{
		char vencedor[4];
		if(player == NULL)
			strcpy(vencedor,"Você");
		else if(boot == NULL)
			strcpy(vencedor,"Boot");
		*jogando = false;
		system("clear");
		printf("\n  %s venceu!\n\n",vencedor);
		system("read -p \"Pressione enter para continuar\" Saindo");
	}
}