#include "jogoUI.h"

#pragma region Funções auxiliares

void handler_saSIGWINCH(int sigNum, siginfo_t *info, void *old) {
  int newHeight, newWidth;
  getmaxyx(stdscr, newHeight, newWidth);
  endwin();
  refresh();
  clear();

  mvprintw(1, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 1, coluna 10 do ecrã
  mvprintw(2, 10,
           "[                     Bem-vindo                    ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
  mvprintw(3, 10,
           "[                   Testa as setas!                ]"); // mensagem fora da janela, na linha 3, coluna 10 do ecrã
  mvprintw(4, 10,
           "[ space - muda para o teste de comandos / q - sair ]"); // mensagem fora da janela, na linha 4, coluna 10 do ecrã
  mvprintw(5, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 5, coluna 10 do ecrã

  janelaMapa = newwin(17, 40, 6,
                      16);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
  janelaComandos = newwin(5, 40, 25, 1);

  desenhaJanela(janelaMapa, 2);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaComandos, 1);  // função exemplo que desenha o janela no ecrã

  wrefresh(janelaComandos);
  wrefresh(janelaMapa);
  refresh();
}

// Função que desenha o mapa
void desenhaJanela(WINDOW *janela, int tipo) {
  if (tipo == 1) {
    wprintw(janela, "Bem-vindo %s!", playerName); // escreve a palavra UP na janelaTopo
    scrollok(janela, TRUE); // liga o scroll na "janela".
  } else if(tipo == 2) {
    scrollok(janela, TRUE); // liga o scroll na "janela".
  } else {
    keypad(janela, TRUE); // para ligar as teclas de direção (aplicar à janela)
    wclear(janela);// limpa a janela
    wborder(janela, '|', '|', '-', '-', '+', '+', '+',
            '+'); // Desenha uma borda. Nota importante: tudo o que escreverem, devem ter em conta a posição da borda
  }
  refresh(); // necessário para atualizar a janela
  wrefresh(janela); // necessário para atualizar a janela
}

void trataTeclado() {
  keypad(janelaMapa, TRUE);       // para ligar as teclas de direção (aplicar à janelaTopo)
  wmove(janelaMapa, 1, 1);        // posiciona o cursor,visualmente, na posicao 1,1 da janelaTopo
  int tecla = wgetch(
    janelaMapa); // MUITO importante: o input é feito sobre a janela em questão, neste caso na janelaTopo
  char *command = (char *) malloc(sizeof(char) * MAXLEN);

  while (tecla != 113) // trata as tecla até introduzirem a letra q. O código asci de q é 113
  {
    if (tecla == KEY_UP) // quando o utilizador introduz a seta para cima
    {
      desenhaJanela(janelaMapa, 2); // atualiza toda a janela
      mvwprintw(janelaMapa, 1, 1, "Estou a carregar na tecla UP"); // escreve a palavra UP na janelaTopo
      wrefresh(
        janelaMapa);// // necessário para atualizar a janelaTopo. Nota: é apenas esta janela que pretendemos atualizar
    } else if (tecla == KEY_RIGHT) {
      desenhaJanela(janelaMapa, 2);
      mvwprintw(janelaMapa, 1, 1, "Estou a carregar na tecla RIGHT");
      wrefresh(janelaMapa);
    } else if (tecla == KEY_LEFT) {
      desenhaJanela(janelaMapa, 2);
      mvwprintw(janelaMapa, 1, 1, "Estou a carregar na tecla LEFT");
      wrefresh(janelaMapa);
    } else if (tecla == KEY_DOWN) {
      desenhaJanela(janelaMapa, 2);
      mvwprintw(janelaMapa, 1, 1, "Estou a carregar na tecla DOWN");
      wrefresh(janelaMapa);
    } else if (tecla == ' ') // trata a tecla espaço
    {
      curs_set(1); // ativa o cursor
      echo();                        // habilita o echo, para que o que o utilizador escreve apareça no ecrã
      wprintw(janelaComandos, "\n #> "); // utilizada para imprimir.
      wgetstr(janelaComandos, command);  // para receber do teclado uma string na "janelaBaixo" para a variavel comando
      if (commands(command, tecla) == 1) return;

      noecho(); //voltar a desabilitar o que o utilizador escreve
      wrefresh(janelaComandos); //sempre que se escreve numa janela, tem de se fazer refresh
    }
    wclear(janelaComandos); // função que limpa o ecrã
    curs_set(0); // faz com que o cursor não seja visivel
    wmove(janelaMapa, 1, 1); // posiciona o cursor (visualmente) na posicao 1,1 da janelaTopo
    tecla = wgetch(
      janelaMapa); //espera que o utilizador introduza um inteiro. Importante e como já referido anteriormente introduzir a janela onde queremos receber o input
  }
  exitCommand();
}

# pragma endregion

#pragma region Comandos

int commands(char *command, int tecla) {
  char *commandAux = strtok(command, " \n");
  char *username, *msg;

  if (commandAux == NULL) // Verificação da correta utilização do programa.
  {
    return 0;
  }

  if (!strcmp(commandAux, "players"))  // Comando players
  {
    playersCommand();
  } else if (!strcmp(commandAux, "msg")) // Comando msg
  {
    username = strtok(NULL, " ");
    msg = strtok(NULL, " ");
    if (username == NULL || msg == NULL) // Verificação da correta utilização do comando.
      wprintw(janelaComandos, "[ERRO] Syntax: msg <username> <message>\n");
    else {
      wprintw(janelaComandos, "username: %s\n", username);
      wprintw(janelaComandos, "msg: %s\n", msg);
      msgCommand(username, msg);
    }
  } else if (!strcmp(commandAux, "exit"))  // Comando exit
  {
    exitCommand();
    strcpy(command, "exit");
    return 1;
  } else wprintw(janelaComandos, ("[ERRO]: Comando invalido.\n"));
}

void* comms(void* arg) {

  TData* tData = (TData*) arg;

  Message message;
  char clientFIFO_Name[22];
  sprintf(clientFIFO_Name, CLIENT_FIFO, getpid());
  int fdClientFIFO = open(clientFIFO_Name, O_RDONLY);
  while(!tData->stop) {
    int nBytes = read(fdClientFIFO, &message, sizeof(Message));
    wprintw(janelaOutput, "PID: %d\tMID: %d\tMSG: %s\n", message.pid, message.messageID, message.message);
  }
  close(fdClientFIFO);
  return NULL;
}

// Comando players
void playersCommand() {
  Message message;
  message.pid = getpid();
  message.messageID = CLIENT_PLAYERS;
  strcpy(message.message, "players");

  int fdServerFIFO = open(SERVER_FIFO, O_WRONLY);
  int nBytes = write(fdServerFIFO, &message, sizeof(Message));
  close(fdServerFIFO);
}

// Comando msg
void msgCommand(char *username, char *msg) {
  Message message;
  message.pid = getpid();
  message.messageID = CLIENT_MSG;
  sprintf(message.message, "%s %s", username, msg);

  int fdServerFIFO = open(SERVER_FIFO, O_WRONLY);
  int nBytes = write(fdServerFIFO, &message, sizeof(Message));
  close(fdServerFIFO);
}

// Comando exit
void exitCommand() {
  // TODO Change this
  char clientFIFO_Name[22];
  sprintf(clientFIFO_Name, CLIENT_FIFO, getpid());
  unlink(clientFIFO_Name);
  wclear(janelaMapa); // função que limpa o ecrã
  wrefresh(janelaMapa);  // função que faz atualização o ecrã com as operações realizadas anteriormente
  delwin(janelaMapa);  // apaga a janela.
  wclear(janelaComandos); // função que limpa o ecrã
  wrefresh(janelaComandos); // função que faz atualiza o ecrã com as operações realizadas anteriormente
  delwin(janelaComandos);  // apaga a janela.
  endwin(); // Obrigatorio e sempre a ultima operação de ncurses
}

#pragma endregion

int main(int argc, char *argv[], char *envp[]) {
  if (argc != 2) {
    printf("[ERRO]: Introduza o nome do jogador como argumento.\n");
    exit(1);
  }

  // Verificar se o SERVER_FIFO existe
  if(access(SERVER_FIFO, F_OK) != 0) {
    printf("[ERRO] O FIFO do Servidor não existe.\n");
    exit(1);
  }

  playerName = argv[1];

  // Criação do FIFO do Cliente
  char clientFIFO_Name[22];
  sprintf(clientFIFO_Name, CLIENT_FIFO, getpid());
  int result = mkfifo(clientFIFO_Name, 0777);
  if (result == -1) {
    printf("[ERRO]: Erro ao criar FIFO do Cliente %d", getpid());
    exit(-1);
  }

  struct sigaction saSIGWINCH;
  saSIGWINCH.sa_sigaction = handler_saSIGWINCH;
  saSIGWINCH.sa_flags = SA_RESTART | SA_SIGINFO;
  sigaction(SIGWINCH, &saSIGWINCH, NULL);

  Message message;
  message.pid = getpid();
  message.messageID = CLIENT_CONNECT;
  strcpy(message.message, playerName);

  int fdServerFIFO = open(SERVER_FIFO, O_WRONLY);
  int nBytes = write(fdServerFIFO, &message, sizeof(Message));
  close(fdServerFIFO);

  initscr(); // Obrigatorio e sempre a primeira operação de ncurses
  raw();  // desativa o buffer de input, cada tecla é lida imediatamente
  noecho();  // desliga o echo no ecrã, para voltar ativar devem utilizar a função echo();
  keypad(stdscr, TRUE);  // habilita as teclas  especiais, exemplo -> <-

  mvprintw(1, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 1, coluna 10 do ecrã
  mvprintw(2, 10,
           "[                      Bem-vindo                   ]"); // mensagem fora da janela, na linha 2, coluna 10 do ecrã
  mvprintw(3, 10,
           "[                   testa as setas!                ]"); // mensagem fora da janela, na linha 3, coluna 10 do ecrã
  mvprintw(4, 10,
           "[ space - muda para o teste de comandos / q - sair ]"); // mensagem fora da janela, na linha 4, coluna 10 do ecrã
  mvprintw(5, 10,
           "____________________________________________________"); // mensagem fora da janela, na linha 5, coluna 10 do ecrã

  janelaMapa = newwin(17, 40, 6, 16);  // Criar janela para a matriz de jogo, tendo os parametro numero de linhas,numero de colunas, posição onde começa a janela  e posição onde termina
  janelaComandos = newwin(5, 40, 25, 1);
  janelaOutput = newwin(9, 40, 30, 1);
  desenhaJanela(janelaMapa, 3);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaComandos, 1);  // função exemplo que desenha o janela no ecrã
  desenhaJanela(janelaOutput, 2);  // função exemplo que desenha o janela no ecrã

  // Threads
  TData thread;
  thread.stop = 0; // comms server

  result = pthread_create(&thread.tid, NULL, comms, (void*) &thread);

  if(result != 0) {
    printf("Erro a criar threads");
    exit(-1);
  }

  trataTeclado();

  thread.stop = 1;
  pthread_kill(thread.tid, SIGUSR1);
  pthread_join(thread.tid, &thread.retval);
}