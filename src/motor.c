#include "motor.h"

int main(int argc, char* argv[], char* envp[])
{
  if(argc != 1)
  {
    printf("[ERRO]: Argumento invalido.\n");
    exit(1);
  }

  char map[ROWS][COLS];
  readMap(map, 1);

  char* command = (char *) malloc(sizeof(char) * MAXLEN);
  do
  {
    printf("Introduza um comando: ");
    fgets(command, MAXLEN, stdin);
    commands(command, map);
  
  } while(strcmp(command, "end"));

  free(command);
}

void commands(char* command, char map[ROWS][COLS])
{
  char* commandAux = strtok(command, " \n");
  char* arg;

  if (commandAux == NULL)
  {
    return;
  }
  

  printf("\nComando: [%s]\n", commandAux);
  if(!strcmp(commandAux, "users"))
  {
    usersCommand();
  }
  else if(!strcmp(commandAux, "kick"))
  {
    arg = strtok(NULL, " ");
    if(arg == NULL)
      printf("[ERRO] Syntax: kick <username>\n");
    else
    {
      printf("username: %s\n", arg);
      kickCommand(arg);
    }
  }
  else if(!strcmp(commandAux, "bots"))
  {
    botsCommand();
  }
  else if(!strcmp(commandAux, "bmov"))
  {
    bmovCommand();
  }
  else if(!strcmp(commandAux, "rbm"))
  {
    rbmCommand();
  }
  else if(!strcmp(commandAux, "begin"))
  {
    beginCommand();
  }
  else if (!strcmp(commandAux, "curses"))
  {
    initscr(); // Obrigatorio e sempre a primeira operação de ncurses
    raw();  // desativa o buffer de input, cada tecla é lida imediatamente
    noecho();  // desliga o echo no ecrã, para voltar ativar devem utilizar a função echo();
    keypad(stdscr, TRUE);  // habilita as teclas  especiais, exemplo -> <-
    mvprintw(1, 10, "___________________________________________________");  // mensagem fora da janela, na linha 1, coluna 10 do ecrã
    //mvprintw(1, 10, "[ Up,Down,Right e Left para mover o jogador ]");  // mensagem fora da janela, na linha 1, coluna 10 do ecrã
    mvprintw(2, 10, "[ space - muda para o teste de funcoes / q - sair ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
    mvprintw(3, 10, "___________________________________________________"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
    WINDOW *janelaTopo = newwin(17, 40, 4, 16);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
    WINDOW *janelaBaixo = newwin(5, 40, 21, 1);  
    WINDOW *janelaBot = newwin(3, 40, 26, 1);
    desenhaMapa(janelaTopo, 2, map);  // função exemplo que desenha o janela no ecrã
    desenhaMapa(janelaBaixo, 1, map);  // função exemplo que desenha o janela no ecrã
    desenhaMapa(janelaBot, 1, map);  // função exemplo que desenha o janela no ecrã
    trataTeclado(janelaTopo, janelaBaixo,janelaBot, map); // função exemplo que trata o teclado
    wclear(janelaTopo); // função que limpa o ecrã
    wrefresh(janelaTopo);  // função que faz atualização o ecrã com as operações realizadas anteriormente
    delwin(janelaTopo);  // apaga a janela.
    wclear(janelaBaixo); // função que limpa o ecrã
    wrefresh(janelaBaixo); // função que faz atualiza o ecrã com as operações realizadas anteriormente
    delwin(janelaBaixo);  // apaga a janela.
    wclear(janelaBot); // função que limpa o ecrã
    wrefresh(janelaBot); // função que faz atualiza o ecrã com as operações realizadas anteriormente
    delwin(janelaBot);  // apaga a janela.
    endwin();  // encerra a utilização do ncurses. Muito importante senão o terminal fica inconsistente (idem se sair por outras vias)
  }
  else if(!strcmp(commandAux, "end"))
  {
    endCommand();
    strcpy(command, "end");
  }
  else if(!strcmp(commandAux, "test_bot"))
  {
    char* interval  = strtok(NULL, " ");
    char* duration = strtok(NULL, " ");
    if(interval == NULL || duration == NULL)
      printf("[ERRO] Syntax: test_bot <interval> <duration>\n");
    else
    {
      printf("interval: %s\n", interval);
      printf("duration: %s\n", duration);
      testBotCommand(interval, duration);
    }
  }
  else printf(("[ERRO]: Comando invalido.\n"));
}

void usersCommand()
{
  printf("\nComando [users] nao implementado.\n");
}

void kickCommand(char *username)
{
  printf("\nComando [kick] nao implementado.\n");
}

void botsCommand()
{
  printf("\nComando [bots] nao implementado.\n");
}

void bmovCommand()
{
  printf("\nComando [bmov] nao implementado.\n");
}

void rbmCommand()
{
  printf("\nComando [rbm] nao implementado.\n");
}

void beginCommand()
{
  printf("\nComando [begin] nao implementado.\n");
}

void endCommand()
{
  printf("\nComando [end] nao implementado.\n");
}

void testBotCommandCurses(char* interval, char* duration, WINDOW *janelaBaixo, WINDOW *janelaTopo, WINDOW *janelaBot, int tecla) {
  int PID_bot, nBytes, state;
  int pipeBotMotor[2];
  

  // Verificação da correta criação do pipeBotMotor.
  if (pipe(pipeBotMotor) == -1)
  {
    wprintw(janelaBaixo,"[ERRO]: Nao foi possível criar o pipe para comunicacao com o Bot.\n");
    exit(1);
  }

  // Verificação da correta execução do fork.
  PID_bot = fork();
  if (PID_bot == -1)
  {
    wprintw(janelaBaixo,"[ERRO]: O fork falhou.\n");
    exit(2);
  }

  if (PID_bot == 0)
  {
    // Bot (Child Process)

    // Fecho dos terminais dos pipes que não serão utilizados.
    close(pipeBotMotor[0]);

    // Fecho e duplicação dos descritores de ficheiros.
    close(STDOUT_FILENO);
    dup(pipeBotMotor[1]);

    // Execução do bot
    execl("bot", "bot", interval, duration, NULL);

    // Verificação da correta execução do execl. Se for bem-sucedido, este código será substituído.
    wprintw(janelaBaixo,"[ERRO]: bot nao encontrado.\n");
    exit(3);
  }
  else
  {
    char* botInfo = (char *) malloc(MAXLEN);
    // Motor (Parent process)
    wprintw(janelaBaixo,"Bot PID: %d\n\n", PID_bot);
    // Fecho do terminal do pipe que não será utilizado.
    close(pipeBotMotor[1]);
    while(1) {
      // Recebe a informação do bot.
      nBytes = read(pipeBotMotor[0], botInfo, MAXLEN);
      
      if(nBytes == 0){
        break;}

      //wprintw(janelaBaixo,"Recebi %d bytes do bot.\n", nBytes);
      botInfo[nBytes + 1] = '\0';

      char* xChar = strtok(botInfo, " ");
      char* yChar  = strtok(NULL, " ");
      char* dChar = strtok(NULL, "\n");

      int x = atoi(xChar), y = atoi(yChar), d = atoi(dChar);
      wprintw(janelaBot,"Recebi: %d %d %d\n\n", x, y, d);
      wrefresh(janelaBaixo);
      wrefresh(janelaBot);
    }
    free(botInfo);
    wrefresh(janelaBaixo);
    wclear(janelaBot); 
    wrefresh(janelaBot);
    
    // printf("\nO bot terminou com %d.\n", WEXITSTATUS(state));
  }
}

void testBotCommand(char* interval, char* duration) {
  int PID_bot, nBytes, state;
  int pipeBotMotor[2];

  // Verificação da correta criação do pipeBotMotor.
  if (pipe(pipeBotMotor) == -1)
  {
    printf("[ERRO]: Nao foi possível criar o pipe para comunicacao com o Bot.\n");
    exit(1);
  }

  // Verificação da correta execução do fork.
  PID_bot = fork();
  if (PID_bot == -1)
  {
    printf("[ERRO]: O fork falhou.\n");
    exit(2);
  }

  if (PID_bot == 0)
  {
    // Bot (Child Process)

    // Fecho dos terminais dos pipes que não serão utilizados.
    close(pipeBotMotor[0]);

    // Fecho e duplicação dos descritores de ficheiros.
    close(STDOUT_FILENO);
    dup(pipeBotMotor[1]);

    // Execução do bot
    execl("bot", "bot", interval, duration, NULL);

    // Verificação da correta execução do execl. Se for bem-sucedido, este código será substituído.
    printf("[ERRO]: bot nao encontrado.\n");
    exit(3);
  }
  else
  {
    char* botInfo = (char *) malloc(MAXLEN);
    // Motor (Parent process)
    printf("Bot PID: %d\n\n", PID_bot);
    // Fecho do terminal do pipe que não será utilizado.
    close(pipeBotMotor[1]);
    while(1) {
      // Recebe a informação do bot.
      nBytes = read(pipeBotMotor[0], botInfo, MAXLEN);
      if(nBytes == 0)
        break;
      printf("Recebi %d bytes do bot.\n", nBytes);
      botInfo[nBytes + 1] = '\0';

      char* xChar = strtok(botInfo, " ");
      char* yChar  = strtok(NULL, " ");
      char* dChar = strtok(NULL, "\n");

      int x = atoi(xChar), y = atoi(yChar), d = atoi(dChar);
      printf("Recebi: %d %d %d\n\n", x, y, d);
    
    }
    free(botInfo);
  }
}

void readMap(char map[ROWS][COLS], int level) {
  char filename[20];
  sprintf(filename, "%s%d%s", "../levels/map", level, ".txt");

  int fileD = open(filename, O_RDONLY);
  if(fileD == -1) {
    printf("[ERRO]: Erro ao abrir o ficheiro %s", filename);
    perror("");
    exit(1);
  }
  char c;
  for(int row = 0; row < ROWS; row++) {
    for(int col = 0; col < COLS; col ++) {
      read(fileD, &map[row][col], 1);
    }
    read(fileD, &c, 1);
  }
}

void desenhaMapa(WINDOW *janela, int tipo, char map[ROWS][COLS])
{

    // quando temos o scroll ativo, não deveremos ter a borda desenhada na janela para não termos o problema escrever em cima das bordas
    if (tipo == 1)
    {
        scrollok(janela, TRUE); // liga o scroll na "janela".
        //wprintw(janela, "\n #> ");
    }
    else if (tipo == 2)
    {
      keypad(janela, TRUE); // para ligar as teclas de direção (aplicar à janela)
      wclear(janela);// limpa a janela
      //wborder(janela, '|', '|', '-', '-', '+', '+', '+', '+'); // Desenha uma borda. Nota importante: tudo o que escreverem, devem ter em conta a posição da borda
      //wprintw(janela,"\n");

      for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col ++) {
          wprintw(janela,"%c", map[row][col]);
        }
      //wprintw(janela,"\n");
      }
    }
    else if (tipo == 3)
    {
        scrollok(janela, TRUE); // liga o scroll na "janela".
        wprintw(janela, "\n #> ");
    }
    refresh(); // necessário para atualizar a janela
    wrefresh(janela); // necessário para atualizar a janela
}

void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo, WINDOW *janelaBot, char map[ROWS][COLS]) 
{
    curs_set(0); // faz com que o cursor não seja visivel

    keypad(janelaTopo, TRUE);       // para ligar as teclas de direção (aplicar à janelaTopo)
    wmove(janelaTopo, 1, 1);        // posiciona o cursor,visualmente, na posicao 1,1 da janelaTopo
    //nota a posição é relativa à janelaTopo e não ao ecrã.
    int tecla = wgetch(janelaTopo); // MUITO importante: o input é feito sobre a janela em questão, neste caso na janelaTopo
    char* command = (char *) malloc(sizeof(char) * MAXLEN);

    while (tecla != 113) // trata as tecla até introduzirem a letra q. O código asci de q é 113
    {

        /*if (tecla == KEY_UP) // quando o utilizador introduz a seta para cima
        {
            desenhaMapa(janelaTopo, 2, map); // atualiza toda a janela
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla UP na posição 1,1 "); // escreve a palavra UP na posição  1,1. 
            //Nota: não escreve na posição 0,0 porque está lá a borda da janela que foi criada anteriormente
            wrefresh(janelaTopo);// // necessário para atualizar a janelaTopo. Nota: é apenas esta janela que pretendemos atualizar
        }
        else if (tecla == KEY_RIGHT)
        {
            desenhaMapa(janelaTopo, 2, map);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla RIGHT na posição 1,1");
            wrefresh(janelaTopo);
        }
        else if (tecla == KEY_LEFT)
        {
            desenhaMapa(janelaTopo, 2, map);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla LEFT na posição 1,1");
            wrefresh(janelaTopo);
        }
        else if (tecla == KEY_DOWN)
        {
            desenhaMapa(janelaTopo, 2, map);
            mvwprintw(janelaTopo, 1, 1, "Estou a carregar na tecla DOWN na posição 1,1");
            wrefresh(janelaTopo);
        }*/
       
       if (tecla == ' ') // trata a tecla espaço
        {  // a tecla espaço ativa a janela inferior e tem o scroll ativo
          //  o wprintw e o wgetstr devem ser utilizados em janelas que tem o scroll ativo.
          curs_set(1); // faz com que o cursor seja visivel
          echo();                         // habilita a maneira como o utilizador visualiza o que escreve
                                            // ou seja volta a ligar o echo para se ver o que se está a escrever
          wprintw(janelaBaixo, "\n #> "); // utilizada para imprimir. 
                                          //nota como a janelaBaixo tem o scroll ativo, ele vai imprimindo linha a linha
          wgetstr(janelaBaixo, command);  // para receber do teclado uma string na "janelaBaixo" para a variavel comando
          commandsCurses(command, janelaBaixo, janelaTopo, janelaBot, tecla);
          noecho(); //voltar a desabilitar o que o utilizador escreve
          wrefresh(janelaBaixo); //sempre que se escreve numa janela, tem de se fazer refresh
            
        }

        wclear(janelaBaixo);
        curs_set(0); // faz com que o cursor não seja visivel
        wmove(janelaTopo, 1, 1); // posiciona o cursor (visualmente) na posicao 1,1 da janelaTopo
        tecla = wgetch(janelaTopo); //espera que o utilizador introduza um inteiro. Importante e como já referido anteriormente introduzir a janela onde queremos receber o input
    }
}

void commandsCurses(char* command, WINDOW *janelaBaixo, WINDOW *janelaTopo, WINDOW *janelaBot, int tecla){

  char* commandAux = strtok(command, " \n");
  char* arg;

  if (commandAux == NULL)
  {
    return;
  }

  if(!strcmp(commandAux, "users"))
  {
    usersCommandCurses(janelaBaixo);
  }
  else if(!strcmp(commandAux, "kick"))
  {
    arg = strtok(NULL, " ");
    if(arg == NULL)
      wprintw(janelaBaixo, "[ERRO] Syntax: kick <username>\n");
    else
    {
      wprintw(janelaBaixo,"username: %s\n", arg);
      kickCommandCurses(arg, janelaBaixo);
    }
  }
  else if(!strcmp(commandAux, "bots"))
  {
    botsCommandCurses(janelaBaixo);
  }
  else if(!strcmp(commandAux, "bmov"))
  {
    bmovCommandCurses(janelaBaixo);
  }
  else if(!strcmp(commandAux, "rbm"))
  {
    rbmCommandCurses(janelaBaixo);
  }
  else if(!strcmp(commandAux, "begin"))
  {
    beginCommandCurses(janelaBaixo);
  }
  else if(!strcmp(commandAux, "end"))
  {
    endCommandCurses(janelaBaixo);
    strcpy(command, "end");
  }
  else if(!strcmp(commandAux, "test"))
  {
    char* interval  = strtok(NULL, " ");
    char* duration = strtok(NULL, " ");
    if(interval == NULL || duration == NULL)
      wprintw(janelaBaixo, "[ERRO] Syntax: test_bot <interval> <duration>\n");
    else
    {
      wprintw(janelaBaixo,"interval: %s\n", interval);
      wprintw(janelaBaixo,"duration: %s\n", duration);
      
      testBotCommandCurses(interval, duration, janelaBaixo, janelaTopo, janelaBot, tecla);      
    }
  }
  else wprintw(janelaBaixo,("[ERRO]: Comando invalido.\n"));
}

void usersCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [users] nao implementado.\n");
}

void kickCommandCurses(char *username,WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [kick] nao implementado.\n");
}

void botsCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [bots] nao implementado.\n");
}

void bmovCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [bmov] nao implementado.\n");
}

void rbmCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [rbm] nao implementado.\n");
}

void beginCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [begin] nao implementado.\n");
}

void endCommandCurses(WINDOW *janelaBaixo)
{
  wprintw(janelaBaixo,"\nComando [end] nao implementado.\n");
}



