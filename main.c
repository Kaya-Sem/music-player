
#include "glib.h"
#include "settings.h"
#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data) {
  g_print("Hello World\n");
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
