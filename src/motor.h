#include "utils.h"

#define MAX_BOTS 10
#define MAX_ROCKS 50
#define MAX_BLOCKS 5
#define MAX_LEVELS 3
#define N_THREADS 2

typedef struct {
  int pid;
  char username[MAXLEN];
  int x;
  int y;
} Player;

typedef struct {
  int id;
  int x;
  int y;
} Block;

typedef struct {
  int id;
  int x;
  int y;
} Rock;

typedef struct {
  int level;
  char map[ROWS][COLS];
  Player players[MAX_PLAYERS];
  int nPlayers;
  Block blocks[MAX_BLOCKS];
  int nBlocks;
  Rock rocks[MAX_ROCKS];
  int nRocks;
} GameInfo;

typedef struct {
  pthread_mutex_t mutex;
  pthread_t tid;
  int stop;
  int level;
  void * retval;
} TData;

// Vars. globais
WINDOW *janelaMapa, *janelaComandos, *janelaOutput;
int botPID[MAX_BOTS];
GameInfo gameInfo;

void processMessage(Message message);
void* comms();
int max(int pipes[][2], int nBots);
void getEnvVars(int* inscricao, int* nPlayers, int* duracao, int* decremento);
void* testBotCommandCurses(void* arg);
void desenhaJanela(WINDOW *janela, int tipo);
void trataTeclado();
void commandsCurses(char* command, int tecla);
void usersCommandCurses();
void kickCommandCurses(char *username);
void botsCommandCurses();
void bmovCommandCurses();
void rbmCommandCurses();
void beginCommandCurses();
void endCommandCurses();
