#include "utils.h"

void commands(char* command, char map[ROWS][COLS]);
void usersCommand();
void kickCommand(char *username);
void botsCommand();
void bmovCommand();
void rbmCommand();
void beginCommand();
void endCommand();
void testBotCommand(char* interval, char* duration);
void readMap(char map[ROWS][COLS], int level);
void testBotCommandCurses(char* interval, char* duration, WINDOW *janelaBaixo, WINDOW *janelaTopo, WINDOW *janelaBot, int tecla);
void desenhaMapa(WINDOW *janela, int tipo, char map[ROWS][COLS]);
void trataTeclado(WINDOW *janelaTopo, WINDOW *janelaBaixo, WINDOW *janelaBot, char map[ROWS][COLS]);
void commandsCurses(char* command, WINDOW *janelaBaixo, WINDOW *janelaTopo, WINDOW *janelaBot, int tecla);
void usersCommandCurses(WINDOW *janelaBaixo);
void kickCommandCurses(char *username, WINDOW *janelaBaixo);
void botsCommandCurses(WINDOW *janelaBaixo);
void bmovCommandCurses(WINDOW *janelaBaixo);
void rbmCommandCurses(WINDOW *janelaBaixo);
void beginCommandCurses(WINDOW *janelaBaixo);
void endCommandCurses(WINDOW *janelaBaixo);
