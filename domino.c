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
piece * inserir_ini(piece * p,int i,int j);
piece * inserir_fim(piece * p,int i,int j);
void trocar(piece ** p,piece ** l,int i,int j,bool ini);
void trocar_aleatoriamente(piece ** p,piece ** l);
piece * deletar(piece * p,int i,int j);
void limpar(piece * p);
void mostrar(piece * p,bool jogando);
void imprimir(piece *l);
void imprimirh(piece *l, char ** v, int i);
void imprimirplayer(piece *l);
int contar(piece * p);
piece * buscar_ultimo(piece * p);
bool pode_jogar(piece * p,piece * t,bool ini);
void abrir_menu();
void jogar();
void bot_ia(piece **bot, piece **table, piece **lot,bool *comprar,bool *player_turn);
bool testar_vencedor(piece *player,piece *bot);
void jogada(piece ** player,piece ** table,bool * player_turn);
void inverter_info(piece ** p);

int main()
{
	abrir_menu();
	system("cls");
	//system("clear");
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
	new->left = i;
	new->right = j;
	new->prox = NULL;
	new->ante = last;
	last->prox = new;
	return p;
}

piece * inserir_ini(piece * p,int i,int j)
{
	piece * new = (piece *) malloc(sizeof(piece));
	new->left = i;
	new->right = j;
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
			trocar(p,l,m->left,m->right,false);
		if(m->prox != NULL)
			m = m->prox;
	}
}

piece * deletar(piece * p,int i,int j)
{
	piece * l = p;

	while((l != NULL) && ((l->right != j) || (l->left != i)))
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
			p = deletar(p,n->left,n->right);
		}
	}
}

void mostrar(piece * p,bool jogando)
{
	piece * n;
	printf("\n");
	for(n=p;n!=NULL;n=n->prox)
		printf("|%d|%d|",n->left,n->right);

	if(jogando)
	{
		printf("\n");
		int i;
		for(i=1;i<contar(p)+1;i++)
			printf("  %i  ",i);
	}
	printf("\n");
}

void imprimir(piece *l)
{
	char ** v;
	piece *p = l;
	int n = contar(l),i,j,k,a,sobe,c;
	v =  calloc (18, sizeof(char *));
	if (v == NULL) 
	{
	   printf ("** Erro: Memoria Insuficiente **");
	}
	else
	{
		for (i = 0; i < 18; i++) {
			v[i] =  calloc (150*n, sizeof(char));
		    if (v[i] == NULL) {
		       printf ("** Erro: Memoria Insuficiente **");
		    }
		    strcpy(v[i]," ");
		}
	}

	j = 0;
	a = 0;
	sobe = 0;
	c= 0;

	for (p = p; p != NULL; p = p->prox)
	{	
		if (sobe == 1 && j == 0)
		{
			j = 9;
		}

		if (sobe == 1 && j == 18 && c == 0)
		{
			j = 0;
			c = 1;
		}

		if ((j % 18) == 0 && a != 1 && j != 0)
		{
			if (sobe == 0)
			{
				imprimirh(p,v,i-5);
				sobe = 1;
			}
			else
			{
				imprimirh(p,v,i-18);
				sobe = 0;
			}
			c = 0;
			a=1;
			j=0;
			continue;
		}

		a=0;		

		for (i = j; abs(j - i) < 9; i++)
		{
			if (sobe == 0)
			{
				if ((i-j) < 4)
				{
					k = p->right;
				}
				else
				{
					k = p->left; 
				}
			}
			else
			{
				if ((i-j) < 4)
				{
					k = p->left;
				}
				else
				{
					k = p->right; 
				}
			}

			if (i == j || i == (j+4) || i == (j+8))
			{
				strcat(v[i], "  - - -  ");
			}

			if ((i-j) == 1 || (i-j) == 5)
			{
				if(k == 2 || k == 3)
				{
					strcat(v[i],"| 0     |");
				}
				else
				{ 
					if (k == 4 || k == 5 || k == 6)
					{
						strcat(v[i],"| 0   0 |");
					}
					else 
					{
						strcat(v[i],"|       |");
					}
				}
			}

			if ((i-j) == 2 || (i-j) == 6)
			{
				if(k == 1 || k == 3 || k == 5)
				{
					strcat(v[i],"|   0   |");
				}
				else
				{ 
					if (k == 6)
					{
						strcat(v[i],"| 0   0 |");
					}
					else 
					{
						strcat(v[i],"|       |");
					}
				}
			}

			if ((i-j) == 3 || (i-j) == 7)
			{
				if(k == 2 || k == 3)
				{
					strcat(v[i],"|     0 |");
				}
				else
				{ 
					if (k == 6 || k == 5 || k == 4)
					{
						strcat(v[i],"| 0   0 |");
					}
					else 
					{
						strcat(v[i],"|       |");
					}
				}
			}

		}
		if (sobe == 1 && i == 9 && c == 1)
		{
			i = 18;
		}

		j = i;
	}

	for (i = 0; i < 18; i++)
	{
		printf("%s\n",v[i]);
	}

	free(v);
}

void imprimirh (piece *l, char ** v, int a)
{
	int j,i,k;

	if (a == 13)
	{
		for (i = 0; i < 13; ++i)
		{
			strcat(v[i],"           ");
		}
		for (i = 13; i < 18; ++i)
		{
			strcat(v[i]," ");
		}
	}

	if (a == 0)
	{
		for (i = 5; i < 18; ++i)
		{
			strcat(v[i],"           ");
		}
		for (i = 0; i < 5; ++i)
		{
			strcat(v[i]," ");
		}
	}

	for (i = a; i < (a+5); i++)
	{
		if ((i == a) || (i == (a+4)))
		{
			strcat(v[i]," --- ---  ");
		}
		else
		{
			k = l->right;
			for (j = 0; j < 2; j++)
			{
				if (i == (a+1))
				{
					if (k == 6)
					{
						strcat(v[i],"|000");
					}
					else
					{
						if (k == 5 || k == 4)
						{
							strcat(v[i],"|0 0");
						}
						else
						{
							if (k == 3 || k == 2)
							{
								strcat(v[i],"|  0");
							}
							else
							{
								strcat(v[i],"|   ");
							}
						}
					}
				}

				if (i == (a+2))
				{
					if (k == 6 || k == 0 || k == 4 || k == 2)
					{
						strcat(v[i],"|   ");
					}
					else
					{
						strcat(v[i],"| 0 ");
					}
				}

				if (i == (a+3))
				{
					if (k == 6)
					{
						strcat(v[i],"|000");
					}
					else
					{
						if (k == 5 || k == 4)
						{
							strcat(v[i],"|0 0");
						}
						else
						{
							if (k == 3 || k == 2)
							{
								strcat(v[i],"|0  ");
							}
							else
							{
								strcat(v[i],"|   ");
							}
						}
					}
				}
				k = l->left;
			}
			strcat(v[i],"| ");
		}
	}
}

void imprimirplayer(piece *l)
{
	char ** v;
	piece *p = l;
	int n = contar(l),i,k;
	printf("\n//-------------Suas-pecas-------------//\n");
	v =  calloc (9, sizeof(char *));
	if (v == NULL) 
	{
	   printf ("** Erro: Memoria Insuficiente **");
	}
	else
	{
		for (i = 0; i < 9; i++) {
			v[i] =  calloc (10*n, sizeof(char));
		    if (v[i] == NULL) {
		       printf ("** Erro: Memoria Insuficiente **");
		    }
		    strcpy(v[i]," ");
		}
	}
	for (p = p; p != NULL; p = p->prox)
	{
		for (i = 0; i < 9; i++)
		{
			if (i < 4)
			{
				k = p->right;
			}
			else
			{
				k = p->left; 
			}

			if (i == 0 || i == 4 || i == 8)
			{
				strcat(v[i], "  - - -   ");
			}

			if (i == 1 || i == 5)
			{
				if(k == 2 || k == 3)
				{
					strcat(v[i],"| 0     | ");
				}
				else
				{ 
					if (k == 4 || k == 5 || k == 6)
					{
						strcat(v[i],"| 0   0 | ");
					}
					else 
					{
						strcat(v[i],"|       | ");
					}
				}
			}

			if (i == 2 || i == 6)
			{
				if(k == 1 || k == 3 || k == 5)
				{
					strcat(v[i],"|   0   | ");
				}
				else
				{ 
					if (k == 6)
					{
						strcat(v[i],"| 0   0 | ");
					}
					else 
					{
						strcat(v[i],"|       | ");
					}
				}
			}

			if (i == 3 || i == 7)
			{
				if(k == 2 || k == 3)
				{
					strcat(v[i],"|     0 | ");
				}
				else
				{ 
					if (k == 6 || k == 5 || k == 4)
					{
						strcat(v[i],"| 0   0 | ");
					}
					else 
					{
						strcat(v[i],"|       | ");
					}
				}
			}

		}
	}

	for (i = 0; i < 9; i++)
	{
		printf("%s\n",v[i]);
	}

	for (i = 1; i <= contar(l); ++i)
	{
		printf("     %d    ",i );
	}

	free(v);
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
		retorno = true;
	else
	{
		piece *m = t;
		if(!ini)
			m = buscar_ultimo(t);

		piece *l;
		for(l=p;l!=NULL;l=l->prox)
		{
			if(ini && ((m->left == l->left) || (m->left == l->right)))
			{
				retorno = true;
				break;
			}
			else if(!ini && ((m->right == l->left) || (m->right == l->right)))
			{
				retorno = true;
				break;
			}
		}
	}

	return retorno;
}

void abrir_menu()
{
	bool menu=true;

	while(menu)
	{
		system("cls");
		//system("clear");
		int escolha;
		printf("\n//------------------------//");
		printf("\n//         Domino         //");
		printf("\n//------------------------//");
		printf("\n1 - Iniciar jogo\n2 - Sair");
		printf("\n\n  Sua escolha: ");
		scanf("%i",&escolha);
		switch(escolha)
		{
			case 1:
				jogar();
			break;
			case 2:
				menu = false;
				system("cls");
				//system("clear");
			break;
		}
	}
}

void jogar()
{
	piece *lot=NULL,*player=NULL,*bot=NULL,*table=NULL;
	bool jogando = true, player_turn=true;
	// Inicializando //
	lot = init_lot(lot);
	player = init_player(player,&lot);
	bot = init_player(bot,&lot);
	// Tela de jogo //
	while(jogando)
	{
		bool comprar = true;
		while(player_turn)
		{
			system("cls");
			//system("clear");
			imprimir(table);
			imprimirplayer(player);

			int escolha;
			printf("\n//------------------------//");
			printf("\n//     Acoes possiveis    //");
			printf("\n//------------------------//");
			if(pode_jogar(player,table,true) || pode_jogar(player,table,false))
				printf("\n1 - Jogar peca");
			if((lot != NULL) && comprar)
				printf("\n2 - Comprar");
			else
				printf("\n2 - Passar");
			printf("\n3 - Desistir");
			printf("\n\n  Sua escolha: ");
			scanf("%i",&escolha);
			switch(escolha)
			{
				case 1:
					if(pode_jogar(player,table,true) || pode_jogar(player,table,false))
					{
						jogada(&player,&table,&player_turn);
					}
				break;
				case 2:
					if(comprar && (lot != NULL))
					{
						comprar = false;
						trocar_aleatoriamente(&player,&lot);
					}
					else
						player_turn = false;
				break;
				case 3:
					limpar(bot);
					player_turn = false;
					jogando = false;
				break;
			}
		}

		if(!player_turn && jogando)
			comprar = true;
		while(!player_turn && jogando)
			bot_ia(&bot,&table,&lot,&comprar,&player_turn);

		if(jogando)
		{
			jogando = testar_vencedor(player,bot);
			player_turn = testar_vencedor(player,bot);
		}
	}

	system("cls");
	//system("clear");
	if(player == NULL)
		printf("\n  Voce venceu!\n\n");
	else
		printf("\n  Bot venceu!\n\n");
	system("pause");
	//system("read -p \"Pressione enter para continuar\" Saindo");	
	// Limpando listas //
	limpar(lot);
	limpar(player);
	limpar(bot);
	limpar(table);
}

void bot_ia(piece **bot, piece **table, piece **lot,bool *comprar,bool *player_turn)
{
	// VERSÃO APENAS PARA TESTES //
	if(pode_jogar(*bot,*table,true))
	{
		piece *l,*first=*table;
		for(l=*bot;l!=NULL;l=l->prox)
			if((first == NULL) || (first->left == l->left) || (first->left == l->right))
			{
				if((first != NULL) && (first->left == l->left))
					inverter_info(&l);

				trocar(table,bot,l->left,l->right,true);
				break;
			}
		*player_turn = true;
	}
	else if(pode_jogar(*bot,*table,false))
	{
		piece *l,*last=buscar_ultimo(*table);
		for(l=*bot;l!=NULL;l=l->prox)
			if((last == NULL) || (last->right == l->left) || (last->right == l->right))
			{
				if((last != NULL) && (last->right == l->right))
					inverter_info(&l);
				trocar(table,bot,l->left,l->right,false);
				break;
			}
		*player_turn = true;
	}
	else if(*comprar && (*lot != NULL))
	{
		*comprar = false;
		trocar_aleatoriamente(bot,lot);
	}
	else
		*player_turn = true;
}

bool testar_vencedor(piece *player,piece *bot)
{
	if((player == NULL) || (bot == NULL))
		return false;
	return true;
}

void jogada(piece ** player,piece ** table,bool * player_turn)
{
	bool ativo = true;
	while(ativo)
	{
		system("cls");
		//system("clear");
		int escolha;
		imprimir(*table);
		imprimirplayer(*player);
		printf("\n//---------------------------------------------//");
		printf("\n// Digite qual peca quer jogar ou 0 pra voltar //");
		printf("\n//---------------------------------------------//");
		printf("\n\n  Sua escolha: ");
		scanf("%i",&escolha);
		if(escolha == 0)
			ativo = false;
		else if(escolha <= contar(*player))
		{
			bool jogou = false;
			int i = 1;
			piece * p;
			for(p=*player;p!=NULL;p=p->prox)
			{
				if(i == escolha)
					break;
				i++;
			}
			while(!jogou)
			{
				system("cls");
				//system("clear");
				imprimir(*table);
				printf("\n//------------------------//");
				printf("\n//       Onde jogar?      //");
				printf("\n//------------------------//");
				printf("\n0 - Voltar");
				printf("\n1 - No inicio");
				printf("\n2 - No fim");
				printf("\n\n  Sua escolha: ");
				scanf("%i",&escolha);

				switch(escolha)
				{
					case 0:
						jogou = true;
					break;
					case 1:;
						piece * first = *table;
						if((first == NULL) || (first->left == p->left) || (first->left == p->right))
						{
							if((first != NULL) && (first->left == p->left))
								inverter_info(&p);
							trocar(table,player,p->left,p->right,true);
							jogou = true;
							*player_turn = false;
							ativo = false;
						}
						else
						{
							printf("\n\n  Nao e possivel jogar esta peca no inicio! \n");
							system("pause");
						}
					break;
					case 2:;
						piece * last = buscar_ultimo(*table);
						if((first == NULL) || (last->right == p->left) || (last->right == p->right))
						{
							if((first != NULL) && (first->right == p->right))
								inverter_info(&p);
							trocar(table,player,p->left,p->right,false);
							jogou = true;
							*player_turn = false;
							ativo = false;
						}
						else
						{
							printf("\n\n  Nao e possivel jogar esta peca no inicio! \n");
							system("pause");
						}						
					break;
				}
			}
		}
	}
}


void inverter_info(piece ** p)
{
	piece *l=*p;
	int aux = l->right;
	l->right = l->left;
	l->left = aux;
}