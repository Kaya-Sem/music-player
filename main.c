#define _XOPEN_SOURCE 500
#include "glib.h"
#include "settings.h"
#include <ftw.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "library.h"

// Global LibraryModel for demonstration
LibraryModel global_library = {0};
GtkWidget *status_label = NULL;

// Idle callback to update UI after scan
static gboolean update_ui_with_scan_results(gpointer data) {
    SongList *result = (SongList *)data;
    // Update the global library model
    if (global_library.songs) {
        g_strfreev(global_library.songs);
    }
    global_library.songs = result->songs;
    global_library.count = result->count;
    // Update the status label
    if (status_label) {
        char buf[128];
        snprintf(buf, sizeof(buf), "Songs in library: %d", result->count);
        gtk_label_set_text(GTK_LABEL(status_label), buf);
    }
    g_free(result); // Only free the struct, not the songs array
    return FALSE;
}

// Thread function to scan library and schedule UI update
void *threaded_scan_library(gpointer data) {
    SongList *result = scan_library(NULL);
    g_idle_add(update_ui_with_scan_results, result);
    return NULL;
}

// Button callback to start scan
static void on_scan_button_clicked(GtkButton *button, gpointer user_data) {
    g_thread_new("library-scan", threaded_scan_library, NULL);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  status_label = gtk_label_new("Songs in library: 0");
  GtkWidget *scan_button = gtk_button_new_with_label("Scan Library");
  g_signal_connect(scan_button, "clicked", G_CALLBACK(on_scan_button_clicked), NULL);
  GtkWidget *label = gtk_label_new("Here come my audio controls!");

  window = gtk_application_window_new(app);
  gtk_window_set_title(
      GTK_WINDOW(window),
      "My super sexy music player - Now playing: Easy Go ~ Nils Petter "
      "Molvaer"); // Allow this to be changed by the user in settings
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
  gtk_window_maximize(GTK_WINDOW(window));
  // gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // Allow this to be
  // changed by the user in settings

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
  gtk_box_append((GtkBox *)hbox, status_label);
  gtk_box_append((GtkBox *)hbox, scan_button);
  gtk_box_append((GtkBox *)hbox, label);

  gtk_window_set_child(GTK_WINDOW(window), hbox);

  gtk_window_present(GTK_WINDOW(window));

  // Event bus or MVC? Also, give players the following options:
  // - Disable scanning on startup
  // - Allow scanning on startup
  // - start manual rescan
  // g_thread_new("library-scan", scan_library, NULL);
}

int main(int argc, char **argv) {

  g_print("\nMade with love by Kaya-Sem\n");
  g_print("Version: %s\n", version);
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
