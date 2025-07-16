#ifndef LIBRARY_MODEL
#define LIBRARY_MODEL

#include <gtk/gtk.h>

// gobject setup
typedef struct _LibraryModel LibraryModel;
typedef struct _LibraryModelClass LibraryModelClass;

/* functions and their arity */
typedef gboolean (*func_0) (LibraryModel*);
typedef gboolean (*func_1) (LibraryModel*, void*);
typedef gboolean (*func_2) (LibraryModel*, void*, void*);

struct _LibraryModel {
  GObject parent_instance;
  int count;

  func_0 full_scan; // does a full rescan of the provided libraries
  func_0 reset; // resets both in-memory library and the sqlite metadata library
  func_0 prune;  // removes all dead or stale entries
  

  func_1 partial_scan; // takes in a string for a partial rescan of that library
  func_1 remove_song; 
  func_1 remove_album; 
  func_1 remove_artist; 

  /* In addition to local removal, also deletes on filesystem */
  
  func_1 delete_artist; 
  func_1 song_artist; 
  func_1 album_artist; 
};

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

#endif // !LIBRARY_MODEL
