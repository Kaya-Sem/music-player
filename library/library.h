#ifndef LIBRARY_MODEL
#define LIBRARY_MODEL

#include "glib-object.h"
#include "glib.h"
#include <gtk/gtk.h>
#include <sys/types.h>

// gobject setup
typedef struct _LibraryModel LibraryModel;
typedef struct _LibraryModelClass LibraryModelClass;

/* functions and their arity */
typedef void (*func_0) (LibraryModel*);
typedef void (*func_1) (LibraryModel*, void*);
typedef void (*func_2) (LibraryModel*, void*, void*);

struct _LibraryModel {
  GObject parent_instance;
  int count;

  GArray tracks;

  func_0 reset; // resets both in-memory library and the sqlite metadata library
  func_0 prune;  // removes all dead or stale entries

  func_1 remove_song; 
  func_1 remove_album; 
  func_1 remove_artist; 
  func_1 add_tracks;

  /* In addition to local removal, also deletes on filesystem */
  
  func_1 delete_artist; 
  func_1 song_artist; 
  func_1 album_artist; 
};

static void add_all_tracks(LibraryModel* model, void* track_array) {
  GArray* array = (GArray*) track_array;

  if (array->len == 0) {
    g_print("no tracks found in array");
    return;
  }

  for (uint i = 0; i < array->len; i++) {
    g_array_append_val(&model->tracks, array[i] );
  }

  return;
}

struct _LibraryModelClass {
  GObjectClass parent_class;
};

G_DEFINE_TYPE(LibraryModel, library_model, G_TYPE_OBJECT)

enum { LIBRARY_CHANGED, LAST_SIGNAL };

static guint signals[LAST_SIGNAL] = {0};

static void library_model_class_init(LibraryModelClass *klass) {
  signals[LIBRARY_CHANGED] =
      g_signal_new("library-changed", G_TYPE_FROM_CLASS(klass),
                   G_SIGNAL_RUN_FIRST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void library_model_init(LibraryModel *self) {}

static void library_model_notify_change(LibraryModel *self) {
  g_signal_emit(self, signals[LIBRARY_CHANGED], 0);
}

LibraryModel* library_model_new() {
  LibraryModel* model = g_object_new(library_model_get_type(), NULL);

  model->add_tracks = add_all_tracks;

  return model;
}

#endif // !LIBRARY_MODEL
