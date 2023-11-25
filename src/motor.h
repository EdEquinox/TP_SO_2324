#include "utils.h"

#define NBOTS 1

// Vars. globais
WINDOW *janelaMapa, *janelaComandos, *janelaBot;
char map[ROWS][COLS]; // matriz que representa o mapa do jogo
int botPID[NBOTS];

void testBotCommandCurses(char* interval, char* duration, int tecla);
void desenhaJanela(WINDOW *janela, int tipo);
void trataTeclado();
int commandsCurses(char* command, int tecla);
void usersCommandCurses();
void kickCommandCurses(char *username);
void botsCommandCurses();
void bmovCommandCurses();
void rbmCommandCurses();
void beginCommandCurses();
void endCommandCurses();
