#include <gtk/gtk.h>

static void
activate (GtkApplication *app, gpointer user_data)
{

  // create a new window and set its title size and border
  // GTK_WINDOW will check if the pointer is an instance of the GtkWindow class, before casting, and emit a warning if the check fails.
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Doom clock");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 400);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  // gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  // GdkPixbuf *icon;

  // icon = create_pixbuf("icon.jpg");
  gtk_window_set_icon_from_file (GTK_WINDOW (window), "icon.jpg", NULL);

  // box inside window as a base widget
  GtkWidget *box;
  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add (GTK_CONTAINER (window), box);

  GtkWidget *menu_bar;
  GtkWidget *menu;
  GtkWidget *menu_item;
  GtkWidget *file_menu_item1;
  GtkWidget *file_menu_item2;
  GtkWidget *file_menu_item3;
  GtkWidget *file_menu_item4;

  // menu bar
  menu_bar = gtk_menu_bar_new ();
  gtk_container_add (GTK_CONTAINER(box), menu_bar);

  // empty menu
  menu = gtk_menu_new ();

  // menu items
  // File
  menu_item = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_container_add (GTK_CONTAINER(menu_bar), menu_item);

    // File sub menu items
    // Open
    file_menu_item1 = gtk_menu_item_new_with_label ("Open");
    file_menu_item2 = gtk_menu_item_new_with_label ("Save");
    file_menu_item3 = gtk_menu_item_new_with_label ("Save as");
    file_menu_item4 = gtk_menu_item_new_with_label ("Close");
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_item1);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_item2);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_item3);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_item4);
    g_signal_connect_swapped (file_menu_item4, "activate", G_CALLBACK (gtk_widget_destroy), window);    

  // About
  menu_item = gtk_menu_item_new_with_mnemonic ("_About");
  gtk_container_add (GTK_CONTAINER(menu_bar), menu_item);

  // This call recursively calls gtk_widget_show() on all widgets
  // that are contained in the window, directly or indirectly.
  gtk_widget_show_all (window);

}

// in a GTK+ application, the purpose of the main() function is to create a GtkApplication object and run it.
int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  // app pointer initilization
  app = gtk_application_new ("org.gtk.doomclock", G_APPLICATION_FLAGS_NONE);

  // connects activate signal to activate function
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  // lauches the app aand sends activate signal, takes command line arguments, the parsed arguments will be removed from the array
  // pressing X on the main window stores the int returned by g_application_run in status
  status = g_application_run (G_APPLICATION (app), argc, argv);

  // frees the app
  g_object_unref (app);

  return status;
}