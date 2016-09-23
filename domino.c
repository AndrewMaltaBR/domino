#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct piece
{
    int right;
    int left;
    struct piece* ante;
    struct piece* prox;
}piece;

piece * init_lot(piece * p);
piece * init_player(piece * p,piece ** l);
piece * inserir_ini(piece * p,int i,int j);
piece * inserir_fim(piece * p,int i,int j);
void trocar(piece ** p,piece ** l,int i,int j,bool ini);
void trocar_aleatoriamente(piece ** p,piece ** l);
piece * deletar(piece * p,int i,int j);
void limpar(piece * p);
void imprimir(piece *l);
void imprimirh(piece *l, char v[27][300], int i);
void imprimirplayer(piece *l);
int contar(piece * p);
piece * buscar_ultimo(piece * p);
bool pode_jogar(piece * p,piece * t,bool ini,piece *d);
void abrir_menu();
void jogar();
void bot_ia(piece **bot, piece **table, piece **lot,bool *comprar,bool *player_turn);
bool testar_vencedor(piece *player,piece *bot,piece *table,piece *lot);
void inverter_info(piece ** p);
void clear();
void pause();

int main()
{
    abrir_menu();
    clear();
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
    piece * new = (piece *) calloc(1,sizeof(piece));
    if(new == NULL)
    {
        printf("\n**Erro ao alocar**\n");
        getchar();
    }
    new->left = i;
    new->right = j;
    new->prox = NULL;
    new->ante = last;
    last->prox = new;
    return p;
}

piece * inserir_ini(piece * p,int i,int j)
{
    piece * new = (piece *) calloc(1,sizeof(piece));
    if(new == NULL)
    {
        printf("\n**Erro ao alocar**\n");
        getchar();
    }
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

void imprimir(piece *l)
{
    char v[27][300];
    piece *p = l;
    int i,j,k,a,sobe,c;
    for (i = 0; i < 27; i++) {
        strcpy(v[i]," ");
    }

    j = 0;
    a = 0;
    sobe = 0;
    c= 0;

    for (p = p; p != NULL; p = p->prox)
    {
        if (sobe == 1 && j == 0)
        {
            j = 18;
        }

        if (sobe == 1 && j == 18 && c == 1)
        {
            j = 0;
        }

        if (sobe == 1 && j == 27 && c == 0)
        {
            j = 9;
            c = 1;
        }

        if ((j % 27) == 0 && a != 1 && j != 0)
        {
            if (sobe == 0)
            {
                imprimirh(p,v,i-5);
                sobe = 1;
            }
            else
            {
                imprimirh(p,v,i-27);
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
                    k = p->left;
                }
                else
                {
                    k = p->right;
                }
            }
            else
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
            i = 27;
        }
        if (sobe == 1 && i == 18 && c == 1)
        {
            i = 0;
        }

        j = i;
    }

    for (i = 0; i < 27; i++)
    {
        printf("%s\n",v[i]);
    }
}

void imprimirh (piece *l, char v[27][300], int a)
{
    int j,i,k;

    if (a == 22)
    {
        for (i = 0; i < 22; ++i)
        {
            strcat(v[i],"               ");
        }
        for (i = 22; i < 27; ++i)
        {
            strcat(v[i]," ");
        }
    }

    if (a == 0)
    {
        for (i = 5; i < 27; ++i)
        {
            strcat(v[i],"               ");
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
            strcat(v[i]," ----- -----  ");
        }
        else
        {
            k = l->left;
            for (j = 0; j < 2; j++)
            {
                if (i == (a+1))
                {
                    if (k == 6)
                    {
                        strcat(v[i],"|0 0 0");
                    }
                    else
                    {
                        if (k == 5 || k == 4)
                        {
                            strcat(v[i],"|0   0");
                        }
                        else
                        {
                            if (k == 3 || k == 2)
                            {
                                strcat(v[i],"|    0");
                            }
                            else
                            {
                                strcat(v[i],"|     ");
                            }
                        }
                    }
                }

                if (i == (a+2))
                {
                    if (k == 6 || k == 0 || k == 4 || k == 2)
                    {
                        strcat(v[i],"|     ");
                    }
                    else
                    {
                        strcat(v[i],"|  0  ");
                    }
                }

                if (i == (a+3))
                {
                    if (k == 6)
                    {
                        strcat(v[i],"|0 0 0");
                    }
                    else
                    {
                        if (k == 5 || k == 4)
                        {
                            strcat(v[i],"|0   0");
                        }
                        else
                        {
                            if (k == 3 || k == 2)
                            {
                                strcat(v[i],"|0    ");
                            }
                            else
                            {
                                strcat(v[i],"|     ");
                            }
                        }
                    }
                }
                k = l->right;
            }
            strcat(v[i],"| ");
        }
    }
}

void imprimirplayer(piece *l)
{
    char v[9][250];
    piece *p = l;
    int i,k;
    for (i = 0; i < 9; i++) {
        strcpy(v[i]," ");
    }
    printf("\n//-------------Suas-pecas-------------//\n");
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
    if(p != NULL)
    {
        piece * last = p;
        while(last->prox != NULL)
            last = last->prox;
        return last;
    }
    return NULL;
}

bool pode_jogar(piece * p,piece * t,bool ini,piece *d)
{
    bool retorno = false;
    if(t == NULL)
        retorno = true;
    else
    {
        piece *m = t;
        if(!ini)
            m = buscar_ultimo(t);
        if(d != NULL)
        {
            if(ini && ((m->left == d->left) || (m->left == d->right)))
                retorno = true;
            else if(!ini && ((m->right == d->left) || (m->right == d->right)))
                retorno = true;
        }
        else
        {
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
    }

    return retorno;
}

void abrir_menu()
{
    bool menu=true;

    while(menu)
    {
        clear();
        int escolha;
        printf("\n//------------------------//");
        printf("\n//         Domino         //");
        printf("\n//------------------------//");
        printf("\n1 - Iniciar jogo\n2 - Sair");
        printf("\n\n  Sua escolha: ");
        scanf("%d",&escolha);
        switch(escolha)
        {
            case 1:
                jogar();
            break;
            case 2:
                menu = false;
                clear();
            break;
        }
    }
}

void jogar()
{
    piece *lot=NULL,*player=NULL,*bot=NULL,*table=NULL;
    bool jogando = true, player_turn=true;
    int i;
    // Inicializando //
    lot = init_lot(lot);
    player = init_player(player,&lot);
    bot = init_player(bot,&lot);
    // Iniciando com o maior double //
    for(i=6;i>=0;i--)

    {
        bool encontrou = false;
        piece *p;
        if(!encontrou)
            for(p=player;p!=NULL;p=p->prox)
                if((p->left == i) && (p->right == i))
                {
                    trocar(&table,&player,i,i,true);
                    encontrou = true;
                    player_turn = false;
                    break;
                }
        if(!encontrou)
            for(p=bot;p!=NULL;p=p->prox)
                if((p->left == i) && (p->right == i))
                {
                    trocar(&table,&bot,i,i,true);
                    encontrou = true;
                    break;
                }
        if(encontrou)
            break;
    }
    // Tela de jogo //
    while(jogando)
    {
        bool comprar = true;
        while(player_turn)
        {
            clear();
            imprimir(table);
            imprimirplayer(player);

            int escolha;
            printf("\n//------------------------//");
            printf("\n//     Acoes possiveis    //");
            printf("\n//------------------------//");
            if(pode_jogar(player,table,true,NULL) || pode_jogar(player,table,false,NULL))
                printf("\nX - Encaixar peca");
            if((lot != NULL) && comprar)
                printf("\n0 - Comprar");
            else
                printf("\n0 - Passar");
            printf("\n\n  Sua escolha: ");
            scanf("%d",&escolha);
            switch(escolha)
            {
                case 0:
                    if(comprar && (lot != NULL))
                    {
                        comprar = false;
                        trocar_aleatoriamente(&player,&lot);
                    }
                    else
                        player_turn = false;
                break;
                default:
                    if((escolha <= contar(player)) && (pode_jogar(player,table,true,NULL) || pode_jogar(player,table,false,NULL)))
                    {
                        bool jogou = false;
                        int i = 1;
                        piece * p;
                        for(p=player;p!=NULL;p=p->prox)
                        {
                            if(i == escolha)
                                break;
                            i++;
                        }
                        while(!jogou)
                        {
                            clear();
                            imprimir(table);
                            imprimirplayer(player);
                            printf("\n//------------------------//");
                            printf("\n//          Onde?         //");
                            printf("\n//------------------------//");
                            if(pode_jogar(bot,table,true,p))
                            	printf("\n1 - No inicio");
                            if(pode_jogar(bot,table,false,p))
                            	printf("\n2 - No fim");
                            printf("\nX - Voltar");
                            printf("\n\n  Sua escolha: ");
                            scanf("%d",&escolha);

                            switch(escolha)
                            {
                                case 1:
                                	if(pode_jogar(bot,table,true,p))
                                	{
                                		piece * first = table;
	                                    if((first == NULL) || (first->left == p->left) || (first->left == p->right))
	                                    {
	                                        if((first != NULL) && (first->left == p->left))
	                                            inverter_info(&p);
	                                        trocar(&table,&player,p->left,p->right,true);
	                                        jogou = true;
	                                        player_turn = false;
	                                    }
	                                    else
	                                    {
	                                        printf("\n\n  Nao e possivel jogar esta peca no inicio! \n\n");
	                                        pause();
	                                    }
                                	}
                                	else
                                		jogou = true;
                                break;
                                case 2:
                                	if(pode_jogar(bot,table,false,p))
                                	{
	                                    piece * last = buscar_ultimo(table);
	                                    if((last == NULL) || (last->right == p->left) || (last->right == p->right))
	                                    {
	                                        if((last != NULL) && (last->right == p->right))
	                                            inverter_info(&p);
	                                        trocar(&table,&player,p->left,p->right,false);
	                                        jogou = true;
	                                        player_turn = false;
	                                    }
	                                    else
	                                    {
	                                        printf("\n\n  Nao e possivel jogar esta peca no fim! \n\n");
	                                        pause();
	                                    }
	                                }
	                                else
                                		jogou = true;
                                break;
                                default:
                                    jogou = true;
                                break;
                            }
                        }
                    }
                break;
            }
        }

        if(!player_turn && jogando)
            comprar = true;
        while(!player_turn && jogando)
            bot_ia(&bot,&table,&lot,&comprar,&player_turn);

        if(jogando)
        {
            jogando = testar_vencedor(player,bot,table,lot);
            player_turn = testar_vencedor(player,bot,table,lot);
        }
    }

    clear();
    imprimir(table);
    if(player == NULL)
        printf("\n\n  Voce venceu!\n\n");
    else if(bot == NULL)
        printf("\n\n  Bot venceu!\n\n");
    else
    {
        int player_pts=0, bot_pts=0;
        piece *l;
        for(l=player;l!=NULL;l=l->prox)
            player_pts += l->left + l->right;
        for(l=bot;l!=NULL;l=l->prox)
            bot_pts += l->left + l->right;
         printf("\n\n  Jogo trancado!");
         printf("\n//----------------------//");
         printf("\n//   Voce: %i pontos    //",player_pts);
         printf("\n//----------------------//");
         printf("\n//   Bot: %i pontos     //",bot_pts);
         printf("\n//----------------------//");
        if(player_pts < bot_pts)
            printf("\n\n  Voce venceu!\n\n");
        else if(bot_pts < player_pts)
            printf("\n\n  Bot venceu!\n\n");
    }
    pause();
    // Limpando listas //
    limpar(lot);
    limpar(player);
    limpar(bot);
    limpar(table);
}

void bot_ia(piece **bot, piece **table, piece **lot,bool *comprar,bool *player_turn)
{
    // IA 0.2 //
    if(pode_jogar(*bot,*table,true,NULL) || pode_jogar(*bot,*table,false,NULL))
    {
        piece *l,*first=*table,*last=buscar_ultimo(*table);
        // CONTANDO AS PEÇAS JÁ JOGADAS E PRESENTES NA MÃO //
        int num[7],i;
        for(i=0;i<7;i++)
            num[i] = 0;
        // contando da table //
        for(l=*table;l!=NULL;l=l->prox) 
        {
            num[l->left]++;
            if(l->left != l->right)
                num[l->right]++;
        }
        // contando da mão //
        for(l=*bot;l!=NULL;l=l->prox) 
        {
            num[l->left]++;
            if(l->left != l->right)
                num[l->right]++;
        }
        // JOGANDO TRANCANDO O PLAYER //
         for(i=0;i<7;i++)
            if((num[i] > 3) && !(*player_turn))
                for(l=*bot;l!=NULL;l=l->prox)
                    if(!(*player_turn) && ((l->left == i) || (l->right == i)))
                    {
                        if((first != NULL) && (first->left == i))
                        {
                            *player_turn = true;
                            if((first != NULL) && (first->left == l->left))
                                inverter_info(&l);
                            trocar(table,bot,l->left,l->right,true);
                            break;
                        }
                        else if((last != NULL) && (last->right == i))
                        {
                            *player_turn = true;
                            if((last != NULL) && (last->right == l->right))
                                inverter_info(&l);
                            trocar(table,bot,l->left,l->right,false);
                            break;
                        }
                    }
        // JOGANDO NORMAL //
        //jogando as buchas primeiro //
        if(!(*player_turn))
            for(l=*bot;l!=NULL;l=l->prox)
                if(!(*player_turn) && (l->left == l->right))
                {
                    if((first == NULL) || pode_jogar(*bot,*table,true,l))
                    {
                        *player_turn = true;
                        trocar(table,bot,l->left,l->right,true);
                        break;
                    }
                    else if((last == NULL) || pode_jogar(*bot,*table,false,l))
                    {
                        *player_turn = true;
                        trocar(table,bot,l->left,l->right,false);
                        break;
                    }
                }
        // jogando as demais peças //
        if(!(*player_turn))
            for(l=*bot;l!=NULL;l=l->prox)
                if(!(*player_turn))
                {
                    if((first == NULL) || pode_jogar(*bot,*table,true,l))
                    {
                        *player_turn = true;
                        if((first != NULL) && (first->left == l->left))
                            inverter_info(&l);
                        trocar(table,bot,l->left,l->right,true);
                        break;
                    }
                    else if((last == NULL) || pode_jogar(*bot,*table,false,l))
                    {
                        *player_turn = true;
                        if((last != NULL) && (last->right == l->right))
                            inverter_info(&l);
                        trocar(table,bot,l->left,l->right,false);
                    }
                }
    }
    else if(*comprar && (*lot != NULL))
    {
        *comprar = false;
        trocar_aleatoriamente(bot,lot);
    }
    else
        *player_turn = true;
}

bool testar_vencedor(piece *player,piece *bot,piece *table,piece *lot)
{
    if((player == NULL) || (bot == NULL))
        return false;
    else if((lot == NULL) && (!pode_jogar(bot,table,true,NULL)) && (!pode_jogar(bot,table,false,NULL)) && (!pode_jogar(player,table,true,NULL)) && (!pode_jogar(player,table,false,NULL)))
        return false;
    return true;
}

void inverter_info(piece ** p)
{
    int aux = (*p)->right;
    (*p)->right = (*p)->left;
    (*p)->left = aux;
}

void clear()
{
    #if linux || LINUX || Linux || UNIX
        system("clear");
    #elif defined WIN32 || Win32 || win32
        system("cls");
    #else
        printf("\e[H\e[2J");
    #endif
}

void pause()
{
    #if linux || LINUX || Linux || UNIX
        system("read -p \"Pressione enter para continuar\" Saindo");
    #elif defined WIN32 || Win32 || win32
        system("pause");
    #else
        system("read -p \"Pressione enter para continuar\" Saindo");
    #endif
}