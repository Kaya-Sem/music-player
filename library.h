#ifndef LIBRARY
#define LIBRARY

#define _XOPEN_SOURCE 500

#include "glib.h"
#include <gtk/gtk.h>
#include <ftw.h>
#include "settings.h"

const char *LIBRARY_PATH = "/home/kayasem/Music/Library/";
const char *exts[] = {".mp3", ".flac", ".wav", ".ogg", ".aac"};

typedef struct LibraryModel LibraryModel;

/* functions and their arity */
typedef gboolean (*func_0) (LibraryModel*);
typedef gboolean (*func_1) (LibraryModel*, void*);
typedef gboolean (*func_2) (LibraryModel*, void*, void*);

struct LibraryModel {
char ** sources;

    char ** songs;
    int count;

  /* functions */

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

gboolean library_full_scan(LibraryModel *lm) {
  // should  loop over all sources and scan them for songs. It should then add them to the library model, and somehow notify the UI or update it via g_idle_add
  return FALSE;
}

typedef struct {
  char **songs;
  int count;
} SongList;

typedef struct {
  GPtrArray *array;
} ScanContext;

int is_song(const char *filename) {
  size_t len = strlen(filename);
  for (int i = 0; i < sizeof(exts) / sizeof(exts[0]); ++i) {
    size_t ext_len = strlen(exts[i]);
    if (len >= ext_len && strcmp(filename + len - ext_len, exts[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

static ScanContext *current_context = NULL;

int find_song(const char *fpath, const struct stat *sb, int typeflag,
              struct FTW *ftwbuf) {
  if (typeflag == FTW_F && is_song(fpath)) {
    g_ptr_array_add(current_context->array, g_strdup(fpath)); // add song
  }
  return 0;
}

void *scan_library(gpointer data) {
  SongList *result = g_new0(SongList, 1);
  GPtrArray *array = g_ptr_array_new_with_free_func(g_free);

  ScanContext context = { .array = array };
  current_context = &context;

  nftw(LIBRARY_PATH, find_song, max_allowed_file_descriptors, FTW_PHYS);

  result->count = array->len;
  result->songs = (char **)g_ptr_array_free(array, FALSE); // take ownership

  // DEBUG: print result
  for (int i = 0; i < result->count; i++) {
    g_print("Scanned song: %s\n", result->songs[i]);
  }

  return result;
}

#endif // !LIBRARY
