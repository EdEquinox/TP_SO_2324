#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <ncurses.h>

#define MAXLEN 50
#define COLS 40
#define ROWS 16

#define SERVER_FIFO "../tmp/serverFIFO"
#define CLIENT_FIFO "../tmp/clientFIFO%d"

typedef struct {
  int pid;
  int messageID;
  char message[MAXLEN];
} Message;

