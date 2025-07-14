#define _XOPEN_SOURCE 500
#include "glib.h"
#include "settings.h"
#include <ftw.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

const char *LIBRARY_PATH = "/home/kayasem/Music/PPM/";

int is_song(const char *filename) {
  const char *exts[] = {".mp3", ".flac", ".wav", ".ogg", ".aac"};
  size_t len = strlen(filename);
  for (int i = 0; i < sizeof(exts) / sizeof(exts[0]); ++i) {
    size_t ext_len = strlen(exts[i]);
    if (len >= ext_len && strcmp(filename + len - ext_len, exts[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

int find_song(const char *fpath, const struct stat *sb, int typeflag,
              struct FTW *ftwbuf) {
  if (typeflag == FTW_F && is_song(fpath)) {
    g_print("Found song: %s\n", fpath);
  }
  return 0; // Continue
}

void *scan_library(gpointer data) {
  nftw(LIBRARY_PATH, find_song, 16, FTW_PHYS);
  return NULL;
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *label = gtk_label_new("Here come my audio controls!");

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "My super sexy music player");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  GtkWidget *hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  GtkWidget *frame1 = gtk_frame_new(NULL);
  GtkWidget *frame2 = gtk_frame_new(NULL);

  gtk_widget_set_size_request(hpaned, 200, -1);

  gtk_paned_set_start_child(GTK_PANED(hpaned), frame1);
  gtk_paned_set_resize_start_child(GTK_PANED(hpaned), TRUE);
  gtk_paned_set_shrink_start_child(GTK_PANED(hpaned), FALSE);
  gtk_widget_set_size_request(frame1, 50, -1);

  gtk_paned_set_end_child(GTK_PANED(hpaned), frame2);
  gtk_paned_set_resize_end_child(GTK_PANED(hpaned), FALSE);
  gtk_paned_set_shrink_end_child(GTK_PANED(hpaned), FALSE);
  gtk_widget_set_size_request(frame2, 50, -1);

  GtkWidget *sidebar_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_box_append(GTK_BOX(sidebar_content), gtk_entry_new());
  gtk_box_append(GTK_BOX(sidebar_content),
                 gtk_label_new("This is the sidebar!"));

  gtk_widget_set_vexpand(hpaned, TRUE);
  gtk_paned_set_start_child(GTK_PANED(hpaned), sidebar_content);
  gtk_paned_set_end_child(GTK_PANED(hpaned),
                          gtk_label_new("This is the main view!"));

  gtk_box_append(GTK_BOX(hbox), hpaned);
  gtk_box_append((GtkBox *)hbox, hpaned);
  gtk_box_append((GtkBox *)hbox, label);

  gtk_window_set_child(GTK_WINDOW(window), hbox);

  gtk_window_present(GTK_WINDOW(window));

  g_thread_new("library-scan", scan_library, NULL);
}

int main(int argc, char **argv) {

  g_print("%s\n", version);
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
