#ifndef SERVER_H_
#define SERVER_H_ 1

struct server_struct_ {
  int width;
  int height;
  int * fields;
  long int generation;
};

typedef struct server_struct_ Server;

Server * server_create(int w, int h, int population);

void server_update(Server *server);
void server_close(Server *s);

#endif