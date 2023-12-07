#include "motor.h"

#pragma region Curses Commands

int commandsCurses(char *command, int tecla) {

  char *commandAux = strtok(command, " \n");
  char *arg;

  if (commandAux == NULL) // Verificação da correta utilização do programa.
  {
    return 0;
  }

  if (!strcmp(commandAux, "users")) // Comando users.
  {
    usersCommandCurses();
  } else if (!strcmp(commandAux, "kick")) // Comando kick.
  {
    arg = strtok(NULL, " ");
    if (arg == NULL) // Verificação da correta utilização do comando kick.
      wprintw(janelaComandos, "[ERRO] Syntax: kick <username>\n");
    else {
      wprintw(janelaComandos, "username: %s\n", arg);
      kickCommandCurses(arg);
    }
  } else if (!strcmp(commandAux, "bots")) // Comando bots.
  {
    botsCommandCurses();
  } else if (!strcmp(commandAux, "bmov")) // Comando bmov.
  {
    bmovCommandCurses();
  } else if (!strcmp(commandAux, "rbm")) // Comando rbm.
  {
    rbmCommandCurses();
  } else if (!strcmp(commandAux, "begin")) // Comando begin.
  {
    beginCommandCurses();
  } else if (!strcmp(commandAux, "end")) // Comando end.
  {
    endCommandCurses();
  } else if (!strcmp(commandAux, "test_bot")) // Comando test_bot.
  {
    char *interval = strtok(NULL, " ");
    char *duration = strtok(NULL, " ");
    if (interval == NULL || duration == NULL)
      wprintw(janelaComandos, "[ERRO] Syntax: test_bot <interval> <duration>\n");
    else {
      wprintw(janelaComandos, "interval: %s\n", interval);
      wprintw(janelaComandos, "duration: %s\n", duration);

      testBotCommandCurses(interval, duration, tecla);
    }
  } else wprintw(janelaComandos, ("[ERRO]: Comando invalido.\n")); // Comando inválido.
}

// Função que trata do comando test_bot.
void testBotCommandCurses(char *interval, char *duration, int tecla) {
  int nBytes, state; // nBytes: número de bytes recebidos do bot
  int pipeBotMotor[2]; // pipeBotMotor: pipe para comunicação entre o bot e o motor

  // Verificação da correta criação do pipeBotMotor.
  if (pipe(pipeBotMotor) == -1) {
    wprintw(janelaComandos, "[ERRO]: Nao foi possível criar o pipe para comunicacao com o Bot.\n");
    exit(1);
  }

  // Verificação da correta execução do fork.
  botPID[0] = fork();
  if (botPID[0] == -1) {
    wprintw(janelaComandos, "[ERRO]: O fork falhou.\n");
    exit(2);
  }

  if (botPID[0] == 0) {
    // Bot (Child Process)

    // Fecho dos terminais dos pipes que não serão utilizados.
    close(pipeBotMotor[0]);

    // Fecho e duplicação dos descritores de ficheiros.
    close(STDOUT_FILENO);
    dup(pipeBotMotor[1]);
    close(pipeBotMotor[1]);

    // Execução do bot
    execl("bot", "bot", interval, duration, NULL);

    // Verificação da correta execução do execl. Se for bem-sucedido, este código será substituído.
    wprintw(janelaComandos, "[ERRO]: bot nao encontrado.\n");
    exit(3);
  } else {
    // Motor (Parent process)
    char *botInfo = (char *) malloc(MAXLEN);
    union sigval result;

    wprintw(janelaComandos, "Bot PID: %d\n\n", botPID[0]);

    // Fecho do terminal do pipe que não será utilizado.
    close(pipeBotMotor[1]);
    for (int i = 0; i < 10; i++) {
      // Recebe a informação do bot.
      nBytes = read(pipeBotMotor[0], botInfo, MAXLEN);

      if (nBytes == 0) {
        botPID[0] = -1;
        break;
      }

      botInfo[nBytes + 1] = '\0';

      char *xChar = strtok(botInfo, " ");
      char *yChar = strtok(NULL, " ");
      char *dChar = strtok(NULL, "\n");

      int x = atoi(xChar), y = atoi(yChar), d = atoi(dChar);
      wprintw(janelaBot, "Recebi: %d %d %d\n\n", x, y, d);
      wrefresh(janelaComandos);
      wrefresh(janelaBot);
    }

    sigqueue(botPID[0], SIGINT, result);

    // wait(&state);

    free(botInfo);          // Libertação da memória alocada para a string botInfo.
    wrefresh(janelaComandos);  // Atualização da janelaComandos.
    wclear(janelaBot);      // Limpeza da janelaBot.
    wrefresh(janelaBot);    // Atualização da janelaBot.
  }
}

// Função que trata do comando users.
void usersCommandCurses() {
  wprintw(janelaComandos, "\nComando [users] nao implementado.\n");
}

// Função que trata do comando kick.
void kickCommandCurses(char *username) {
  wprintw(janelaComandos, "\nComando [kick] nao implementado.\n");
}

// Função que trata do comando bots.
void botsCommandCurses() {
  wprintw(janelaComandos, "\nComando [bots] nao implementado.\n");
}

// Função que trata do comando bmov.
void bmovCommandCurses() {
  wprintw(janelaComandos, "\nComando [bmov] nao implementado.\n");
}

// Função que trata do comando rbm.

void rbmCommandCurses() {
  wprintw(janelaComandos, "\nComando [rbm] nao implementado.\n");
}

// Função que trata do comando begin.
void beginCommandCurses() {
  wprintw(janelaComandos, "\nComando [begin] nao implementado.\n");
}

// Função que trata do comando end.
void endCommandCurses() {
  unlink(SERVER_FIFO);
  wprintw(janelaComandos, "\nComando [end] nao implementado.\n");
}

#pragma endregion

#pragma region Auxiliar Functions

// Função que trata o sinal de resize do terminal
void handler_saSIGWINCH(int sigNum, siginfo_t *info, void *old) {
  int newHeight, newWidth;
  getmaxyx(stdscr, newHeight, newWidth);
  endwin();
  refresh();
  clear();

  mvprintw(1, 10,
           "____________________________________________________");  // mensagem fora da janela, na linha 1, coluna 10 do ecrã
  mvprintw(2, 10,
           "[ space - muda para o teste de comandos / q - sair ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
  mvprintw(3, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 3, coluna 10 do ecrã

  janelaMapa = newwin(17, 40, 4,
                      16);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
  janelaComandos = newwin(5, 40, 21, 1);

  desenhaJanela(janelaMapa, 2);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaComandos, 3);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaBot, 1);  // função exemplo que desenha o janela no ecrã

  wrefresh(janelaComandos);
  wrefresh(janelaMapa);
  wrefresh(janelaBot);
  refresh();
}

// Função que lê o mapa do ficheiro.
void readMap(int level) {
  char filename[20];
  sprintf(filename, "%s%d%s", "../levels/map", level, ".txt");

  int fileD = open(filename, O_RDONLY); // Abrir o ficheiro para leitura.
  if (fileD == -1) { // Verificação da correta abertura do ficheiro.
    printf("[ERRO]: Erro ao abrir o ficheiro %s", filename);
    perror("");
    exit(1);
  }
  char c;
  for (int row = 0; row < ROWS; row++) { // Ler o mapa do ficheiro.
    for (int col = 0; col < COLS; col++) {
      read(fileD, &gameInfo.map[row][col], 1);
    }
    read(fileD, &c, 1);
  }
}

// Função que desenha o mapa.
void desenhaJanela(WINDOW *janela, int tipo) {
  if (tipo == 1) {
    scrollok(janela, TRUE); // liga o scroll na "janela".
  } else if (tipo == 2) {
    keypad(janela, TRUE); // para ligar as teclas de direção (aplicar à janela)
    wclear(janela);// limpa a janela

    for (int row = 0; row < ROWS; row++) { // Imprime o mapa na janela.
      for (int col = 0; col < COLS; col++) {
        wprintw(janela, "%c", gameInfo.map[row][col]);
      }
    }
  } else if (tipo == 3) {
    scrollok(janela, TRUE); // liga o scroll na "janela".
    if (botPID[0] != -1)
      wprintw(janelaComandos, "Bot PID: %d\n\n", botPID[0]);
  }
  refresh(); // necessário para atualizar a janela
  wrefresh(janela); // necessário para atualizar a janela
}

// Função que trata do teclado.
void trataTeclado() {
  curs_set(0); // faz com que o cursor não seja visivel
  int tecla = wgetch(
    janelaMapa); // MUITO importante: o input é feito sobre a janela em questão, neste caso na janelaMapa
  char *command = (char *) malloc(sizeof(char) * MAXLEN);

  while (tecla != 113) // trata as tecla até introduzirem a letra q. O código asci de q é 113
  {

    if (tecla == ' ') // trata a tecla espaço
    {
      curs_set(1); // faz com que o cursor seja visivel
      echo();                         // habilita o echo no ecrã, para voltar desativar devem utilizar a função noecho();
      wprintw(janelaComandos, "\n #> "); // utilizada para imprimir.
      wgetstr(janelaComandos,
              command);  // para receber do teclado uma string na "janelaComandos" para a variavel comando

      if (commandsCurses(command, tecla) == 1) return;

      noecho(); //voltar a desabilitar o que o utilizador escreve
      wrefresh(janelaComandos); //sempre que se escreve numa janela, tem de se fazer refresh
    }

    wclear(janelaComandos); // função que limpa o ecrã
    curs_set(0); // faz com que o cursor não seja visivel
    wmove(janelaMapa, 1, 1); // posiciona o cursor (visualmente) na posicao 1,1 da janelaMapa
    tecla = wgetch(
      janelaMapa); //espera que o utilizador introduza um inteiro. Importante e como já referido anteriormente introduzir a janela onde queremos receber o input
  }
  endCommandCurses();
}

#pragma endregion

int main(int argc, char *argv[], char *envp[]) {
  if (argc != 1) // Verificação da correta utilização do programa.
  {
    printf("[ERRO]: Argumento invalido.\n");
    exit(1);
  }

  // Ler o mapa do ficheiro map1.txt
  readMap(1);

  // Tratamento do sinal de resize da janela do terminal
  struct sigaction saSIGWINCH;
  saSIGWINCH.sa_sigaction = handler_saSIGWINCH;
  saSIGWINCH.sa_flags = SA_RESTART | SA_SIGINFO;
  sigaction(SIGWINCH, &saSIGWINCH, NULL);

  // Criação do FIFO do Servidor
  int result = mkfifo(SERVER_FIFO, 0777);
  if (result == -1) {
    printf("[ERRO]: Erro ao criar FIFO do Servidor");
    exit(-1);
  }

  initscr(); // Obrigatorio e sempre a primeira operação de ncurses
  raw();  // desativa o buffer de input, cada tecla é lida imediatamente
  noecho();  // desliga o echo no ecrã, para voltar ativar devem utilizar a função echo();
  keypad(stdscr, TRUE);  // habilita as teclas  especiais, exemplo -> <-

  mvprintw(1, 10,
           "____________________________________________________");  // mensagem fora da janela, na linha 1, coluna 10 do ecrã
  mvprintw(2, 10,
           "[ space - muda para o teste de comandos / q - sair ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
  mvprintw(3, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 3, coluna 10 do ecrã

  janelaMapa = newwin(17, 40, 4,
                      16);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
  janelaComandos = newwin(5, 40, 21, 1);
  janelaBot = newwin(3, 40, 26, 1);
  desenhaJanela(janelaMapa, 2);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaComandos, 1);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaBot, 1);  // função exemplo que desenha o janela no ecrã

  trataTeclado(); // função que trata o teclado

  wclear(janelaMapa); // função que limpa o ecrã
  wrefresh(janelaMapa);  // função que faz atualização o ecrã com as operações realizadas anteriormente
  delwin(janelaMapa);  // apaga a janela.
  wclear(janelaComandos); // função que limpa o ecrã
  wrefresh(janelaComandos); // função que faz atualiza o ecrã com as operações realizadas anteriormente
  delwin(janelaComandos);  // apaga a janela.
  wclear(janelaBot); // função que limpa o ecrã
  wrefresh(janelaBot); // função que faz atualiza o ecrã com as operações realizadas anteriormente
  delwin(janelaBot);  // apaga a janela.
  endwin();  // encerra a utilização do ncurses. Muito importante senão o terminal fica inconsistente (idem se sair por outras vias)

}