#ifndef CONTROLLER
#define CONTROLLER

#include <stdlib.h>
#include "../library/library.h"

/* The controller provides a way for the User (UI) to interact with the system, and alter it's state.
 * The controller gets new data, or changes existing data, thereby manipulating the library model.
 * The library model with emit an event, to notify UI components. 
 * The UI components will update themselves with the model's data.
 * */



typedef struct Controller Controller;

typedef void (*ctrl_func_0) (Controller*);
typedef void (*ctrl_func_1) (Controller*, void*);
typedef void (*ctrl_func_2) (Controller*, void*, void*);


struct Controller{
  LibraryModel* model;

  ctrl_func_0 full_library_scan;
};



Controller* controller_new() {
  Controller* controller = malloc(sizeof(Controller));
  if (!controller) {
    return NULL;
  }


  return controller;
}

#endif // !CONTROLLER
