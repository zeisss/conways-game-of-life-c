#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "server.h"

Server *server;
int args_w = 80, args_h = 44, args_p = 30;

char *alive = "x", *dead = " ";
/**
 * Prints the usage table.
 */
void usage(char *name) {
  printf("Usage: %s [options]\n", name);
  printf("\n");
  printf("\t-w WIDTH (default 80)\n");
  printf("\t-H HEIGHT (default 44)\n");
  printf("\t-p PERCENTAGE (default 30)\n");
  printf("\t-n negative output\n");
  printf("\t-h Shows this help\n");
  printf("\n");
}

/**
 * Parses arguments. Returns 0 to abort processing.
 */
int parse_args(int argc, char* argv[]) {
  for (;argc > 1; argc--) {
    if (strncmp("-h", argv[argc - 1], 2) == 0 ||
        strncmp("--help", argv[argc - 1], 6) == 0) {
      usage(argv[0]);
      return 0;
    }
    else if (strncmp("-w", argv[argc - 1], 2) == 0) {
      args_w = atoi(argv[argc]);
    }
    else if (strncmp("-H", argv[argc - 1], 2) == 0) {
      args_h = atoi(argv[argc]);
    }
    else if (strncmp("-p", argv[argc - 1], 2) == 0) {
      args_p = atoi(argv[argc]);
    }
    else if(strncmp("-n",  argv[argc - 1], 2) == 0) {
      alive = " ";
      dead = "o";
    }
  }
  return 1;
}

void render_fields(Server *s) {
  int x, y, value;
  for (y = 0; y < s->height; y++) {
    printf("|");
    for (x = 0; x < s->width; x++) {
      value = s->fields[x + y * s->width];
      if (value > 0) {
        printf("%s ", alive);
      } else {
        printf("%s ", dead);
      }
    }
    printf("|\n");
  }
  printf("Generation %ld\n", server->generation);
}

void shutdown_hook(int sig) {
  server_close(server);
  printf("Bye bye.\n");
  exit(0);
}

int main(int argc, char* argv[]) {
  if (parse_args(argc, argv) == 0) {
    exit(0);
  }

  printf("Game of Life\n");
  printf("============\n");
  printf("Starting up ...\n");

  // Initialization
  srand(time(0));
  signal(SIGTERM, shutdown_hook);
  signal(SIGINT, shutdown_hook);
  server = server_create(args_w, args_h, (int)(args_w * args_h * args_p  / 100));

  while (1) {
    render_fields(server);
    server_update(server);
    usleep (100 * 1000);
  }
  return 0;
}