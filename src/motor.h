#include "utils.h"

#define NBOTS 1
#define MAX_PLAYERS 5
#define MAX_BOTS 10
#define MAX_ROCKS 50
#define MAX_BLOCKS 5
#define MAX_LEVELS 3

typedef struct {
  int pid;
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
  Block blocks[MAX_BLOCKS];
  Rock rocks[MAX_ROCKS];
} GameInfo;

// Vars. globais
WINDOW *janelaMapa, *janelaComandos, *janelaBot;
int botPID[MAX_BOTS];
GameInfo gameInfo;

void comms();
int max(int pipes[][2], int nBots);
void getEnvVars(int* inscricao, int* nPlayers, int* duracao, int* decremento);
void testBotCommandCurses(int level);
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
