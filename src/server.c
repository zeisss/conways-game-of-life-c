#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void server_populate_random(Server *server, int population);
#define FIELD(s, x, y) s->fields[(x) + (y) * s->width]
#define ALIVE 1
#define DEAD 0

Server * server_create(int w, int h, int population) {
  Server * s = (Server*) malloc(sizeof(Server));
  s->width = w;
  s->height = h;
  s->fields = (int*) malloc(sizeof(int) * w * h);
  memset(s->fields, DEAD, sizeof(int) * w * h);
  if (population > 0) 
    server_populate_random(s, population);
  return s;
}

void server_populate_random(Server *server, int population) {
  while (population > 0) {
    int i = rand() % (server->height * server->width);
    
    // printf("%d\n", i);

    server->fields[i] = 1;

    population--;
  }
}

void server_calc_new_fields(Server *server, int *new_field) {
  int x, y, neighbours, value;

  for (x = 0; x < server->width; x++) {
    for (y = 0; y < server->height; y++) {
      neighbours = 0;
      value = FIELD(server, x, y);

      // Left
      if (x > 0) {
        if (FIELD(server, x - 1, y) == ALIVE) neighbours++;
      }
      // Top Left
      if (x > 0 && y > 0) {
        if (FIELD(server, x - 1, y - 1) == ALIVE) neighbours++;
      }
      // Top
      if (y > 0) {
        if (FIELD(server, x, y - 1) == ALIVE) neighbours++;
      }

      // Top Right
      if (y > 0 && x < server->width - 1) {
        if (FIELD(server, x + 1, y - 1) == ALIVE) neighbours++;
      }
      // Right
      if (x < server->width - 1) {
        if (FIELD(server, x + 1, y) == ALIVE) neighbours++;
      }
      // Bottom Right
      if (y < server->height - 1 && x < server->width - 1) {
        if (FIELD(server, x + 1, y + 1) == ALIVE) neighbours++;
      }
      // Bottom
      if (y < server->height - 1) {
        if (FIELD(server, x, y + 1) == ALIVE) neighbours++;
      }
      // Bottom Left
      if (x > 0 && y < server->height - 1) {
        if (FIELD(server, x - 1, y + 1) == ALIVE) neighbours++;
      }
      
      // if (neighbours > 0) printf("x: %d y: %d neigbours: %d\n", x, y, neighbours);

      if (value == ALIVE) {
        if (neighbours == 2 || neighbours == 3) // Exactly the right amount - all else is under- or overpopulation
          new_field[x + y * server->width] = ALIVE;
      } else {
        if (neighbours == 3) {
          new_field[x + y * server->width] = ALIVE;
        }
      }
      
    }
  }
  
}

void server_update(Server *server) {
  // Create a new field
  int * new_field = (int *)malloc(sizeof(int) * server->width * server->height);
  memset(new_field, DEAD, sizeof(int) * server->width * server->height);

  // Calculate the next state
  server_calc_new_fields(server, new_field);

  // Update the structure to the new 
  free (server->fields);
  server->fields = new_field;
  server->generation++;
}

void server_close(Server *s) {
  free(s);
}