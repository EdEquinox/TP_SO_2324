#include "utils.h"

typedef struct {
  int stop;
  pthread_t tid;
  void * retval;
} TData;

// Vars. globais
WINDOW *janelaMapa, *janelaComandos, *janelaOutput;
char* playerName;

void* comms(void* arg);
int commands(char* command, int tecla);
void playersCommand();
void msgCommand(char *username, char* msg);
void exitCommand();
void desenhaJanela(WINDOW *janela, int tipo);
void trataTeclado();