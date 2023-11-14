#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#define MAXLEN 50
#define COLS 40
#define ROWS 16
