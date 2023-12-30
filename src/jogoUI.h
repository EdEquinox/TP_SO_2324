#include "utils.h"

#define N_THREADS 2

typedef struct {
  int stop;
  pthread_t tid;
  void * retval;
} TData;

// Vars. globais
WINDOW *janelaMapa, *janelaComandos, *janelaOutput;
char* playerName;

void closeClient();
void* comms(void* arg);
int commands(char* command, int tecla);
void playersCommand();
void msgCommand(char *username, char* msg);
void exitCommand();
void desenhaJanela(WINDOW *janela, int tipo);
void* trataTeclado(void* arg);