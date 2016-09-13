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
int contar(piece * p);
piece * buscar_ultimo(piece * p);
bool pode_jogar(piece * p,piece * t,bool ini);
void abrir_menu();
void jogar();
void boot_ia(piece **boot, piece **table, piece **lot,bool *comprar,bool *player_turn);
bool testar_vencedor(piece *player,piece *boot);
void jogada(piece ** player,piece ** table,bool * player_turn);
void inverter_info(piece * p);

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

void mostrar(piece * p,bool jogando)
{
	piece * n;
	printf("\n");
	for(n=p;n!=NULL;n=n->prox)
		printf("|%d|%d|",n->right,n->left);

	if(jogando)
	{
		printf("\n");
		int i;
		for(i=1;i<contar(p)+1;i++)
			printf("  %i  ",i);
	}
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
		retorno = true;
	else
	{
		piece *m = t;
		if(!ini)
			m = buscar_ultimo(t);

		piece *l;
		for(l=p;l!=NULL;l=l->prox)
		{
			if((l->right == m->right) || (l->right == m->left) || (l->left == m->right) || (l->left == m->left))
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
	piece *lot=NULL,*player=NULL,*boot=NULL,*table=NULL;
	bool jogando = true, player_turn=true;
	// Inicializando //
	lot = init_lot(lot);
	player = init_player(player,&lot);
	boot = init_player(boot,&lot);
	// Tela de jogo //
	while(jogando)
	{
		bool comprar = true;
		while(player_turn)
		{
			system("cls");
			//system("clear");
			mostrar(table,false);
			mostrar(player,false);
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
					limpar(boot);
					player_turn = false;
					jogando = false;
				break;
			}
		}

		if(!player_turn && jogando)
			comprar = true;
		while(!player_turn && jogando)
			boot_ia(&boot,&table,&lot,&comprar,&player_turn);

		if(jogando)
		{
			jogando = testar_vencedor(player,boot);
			player_turn = testar_vencedor(player,boot);
		}
	}

	system("cls");
	//system("clear");
	if(player == NULL)
		printf("\n  Voce venceu!\n\n");
	else
		printf("\n  Boot venceu!\n\n");
	system("pause");
	//system("read -p \"Pressione enter para continuar\" Saindo");	
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
			if((first == NULL) || (first->left == l->left) || (first->left == l->right))
			{
				if((first == NULL) && (first->left == l->left))
					inverter_info(l);
				trocar(table,boot,l->right,l->left,true);
				break;
			}
		*player_turn = true;
	}
	else if(pode_jogar(*boot,*table,false))
	{
		piece *l,*last=buscar_ultimo(*table);
		for(l=*boot;l!=NULL;l=l->prox)
			if((l->right == last->right) || (last->right == l->left) || (last->right == l->right))
			{
				if((last == NULL) && (last->right == l->right))
					inverter_info(l);
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

bool testar_vencedor(piece *player,piece *boot)
{
	if((player == NULL) || (boot == NULL))
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
		mostrar(*table,false);
		mostrar(*player,true);
		printf("\n//---------------------------------------------//");
		printf("\n// Digite qual peca quer jogar ou 0 pra voltar //");
		printf("\n//---------------------------------------------//");
		printf("\n\n  Sua escolha: ");
		scanf("%i",&escolha);
		if(escolha == 0)
			ativo = false;
		else if(escolha <= contar(*player)+1)
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
				mostrar(*table,false);
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
							if(first->left == p->left)
								inverter_info(p);
							trocar(table,player,p->right,p->left,true);
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
							if(first->right == p->right)
								inverter_info(p);
							trocar(table,player,p->right,p->left,false);
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


void inverter_info(piece * p)
{
	int aux = p->right;
	p->right = p->left;
	p->left = aux;
}