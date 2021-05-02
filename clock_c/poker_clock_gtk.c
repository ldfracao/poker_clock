#include <gtk/gtk.h>

// prototypes
static gboolean _label_update(gpointer data);
static void _pause_timer(GtkWidget *button, gpointer data);
static void _start_timer (GtkWidget *button, gpointer data);
static void activate (GtkApplication *app, gpointer user_data);

static gboolean start_timer = FALSE;
static gboolean continue_timer = FALSE;

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

static gboolean
_label_update(gpointer data)
{
  GtkLabel *label = (GtkLabel*)data;

  static short seconds = 0;
  static short minutes = 0;
  char *buf = malloc(200 * sizeof(char));

  snprintf(buf, 200, "%.2d:%.2d", minutes, ++seconds);
  gtk_label_set_label(label, buf);
  free(buf);

  return continue_timer;
}

static void
_pause_timer(GtkWidget *button, gpointer data)
{
  GtkWidget *label = data;
  if(start_timer)
  {
    continue_timer = !continue_timer;
    if(continue_timer)
    {
      g_timeout_add_seconds(1, _label_update, label);
      start_timer = TRUE;
      continue_timer = TRUE;
    }
  }
  g_signal_connect (G_OBJECT(button), "clicked", G_CALLBACK (_start_timer), label);
}

static void
_start_timer (GtkWidget *button, gpointer data)
{

  GtkWidget *label = data;

  if(!start_timer)
  {
    g_timeout_add_seconds(1, _label_update, label);
    start_timer = TRUE;
    continue_timer = TRUE;
  }
  g_signal_connect (G_OBJECT(button), "clicked", G_CALLBACK (_pause_timer), label);
}

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
  
  // box inside window as a base widget
  GtkWidget *box;

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), box);

  // menu bar
  GtkWidget *menu_bar;
  GtkWidget *menu;
  GtkWidget *menu_item;
  GtkWidget *file_menu_open;
  GtkWidget *file_menu_save;
  GtkWidget *file_menu_save_as;
  GtkWidget *file_menu_close;

  // accelerators
  GtkAccelGroup *accel_group = NULL;

  accel_group = gtk_accel_group_new ();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  // empty menu bar
  menu_bar = gtk_menu_bar_new ();
  gtk_container_add (GTK_CONTAINER(box), menu_bar);

  // empty menu
  menu = gtk_menu_new ();

  // menu items
  // File
  menu_item = gtk_menu_item_new_with_mnemonic ("_File");
  gtk_container_add (GTK_CONTAINER(menu_bar), menu_item);

    // File sub menu items
    file_menu_open = gtk_menu_item_new_with_label ("Open");
    file_menu_save = gtk_menu_item_new_with_label ("Save");
    file_menu_save_as = gtk_menu_item_new_with_label ("Save as");
    file_menu_close = gtk_menu_item_new_with_label ("Close");

    // set and populate file menu
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item), menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_open);
    gtk_widget_add_accelerator(file_menu_open, "activate", accel_group, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_save);
    gtk_widget_add_accelerator(file_menu_save, "activate", accel_group, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_save_as);
    gtk_widget_add_accelerator(file_menu_save_as, "activate", accel_group, GDK_KEY_Shift_L, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_shell_append (GTK_MENU_SHELL (menu), file_menu_close);
    gtk_widget_add_accelerator(file_menu_close, "activate", accel_group, GDK_KEY_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    g_signal_connect_swapped (file_menu_close, "activate", G_CALLBACK (gtk_widget_destroy), window);    

  // About
  menu_item = gtk_menu_item_new_with_mnemonic ("_About");
  gtk_container_add (GTK_CONTAINER(menu_bar), menu_item);

  // MAIN AREA
  GtkWidget *main_area;
  
  main_area = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (box), main_area);

  // left column
  GtkWidget *left_labels;
  left_labels = gtk_label_new ("Level 1");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 0, 1, 1);
  left_labels = gtk_label_new ("Entries");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 1, 1, 1);
  left_labels = gtk_label_new ("Players");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 2, 1, 1);
  left_labels = gtk_label_new ("Rebuys");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 3, 1, 1);
  left_labels = gtk_label_new ("Add ons");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 4, 1, 1);
  left_labels = gtk_label_new ("Total prize");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 5, 1, 1);

  // center column
  GtkWidget *center_labels;

  center_labels = gtk_label_new ("Next blinds");
  gtk_grid_attach (GTK_GRID (main_area), center_labels, 1, 0, 1, 1);
  center_labels = gtk_label_new ("Blinds");
  gtk_grid_attach (GTK_GRID (main_area), center_labels, 1, 1, 1, 1);
  center_labels = gtk_label_new ("00:00");
  gtk_grid_attach (GTK_GRID (main_area), center_labels, 1, 2, 1, 1);
 
  // right column
  GtkWidget *right_labels;
  right_labels = gtk_label_new ("Elapsed time");
  gtk_grid_attach (GTK_GRID (main_area), right_labels, 2, 0, 1, 1);
  right_labels = gtk_label_new ("Next break");
  gtk_grid_attach (GTK_GRID (main_area), right_labels, 2, 1, 1, 1);
  right_labels = gtk_label_new ("Chip count");
  gtk_grid_attach (GTK_GRID (main_area), right_labels, 2, 2, 1, 1);
  right_labels = gtk_label_new ("Average stack");
  gtk_grid_attach (GTK_GRID (main_area), right_labels, 2, 3, 1, 1);

  GtkWidget *start_button;
  
  start_button = gtk_button_new_with_label ("Start/Pause");
  gtk_grid_attach (GTK_GRID (main_area), start_button, 1, 6, 1, 1);
  g_signal_connect (G_OBJECT(start_button), "clicked", G_CALLBACK (_start_timer), center_labels);

  // STYLESHEET START
  GtkCssProvider *provider = gtk_css_provider_new ();
  GtkStyleContext *context;
  GtkStyleContext *context1;
  gtk_css_provider_load_from_path (provider, "poker_clock_gtk_c.css", NULL);
  context = gtk_widget_get_style_context (center_labels);
  context1 = gtk_widget_get_style_context (window);

  gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider (context1, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  // STYLESHEET END

  // This call recursively calls gtk_widget_show() on all widgets that are contained in the window, directly or indirectly.
  gtk_widget_show_all (window);

}
