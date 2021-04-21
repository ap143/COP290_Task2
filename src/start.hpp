#ifndef START
#define START

#include "network.hpp"
#include <gtk/gtk.h>
#include <pthread.h>
#include <X11/Xlib.h>

extern Server* serv;
extern Client* client;

extern std::string user_name;
extern std::string oponent_name;

void host_game(GtkApplication *app, gpointer user_data);
void join_game(GtkApplication *app, gpointer user_data);

void host_join(GtkApplication *app, gpointer user_data);

int start(int argc, char* argv[]);

void* initial_connection(void* a);

#endif