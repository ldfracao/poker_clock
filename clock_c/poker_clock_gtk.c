#include <gtk/gtk.h>
#include <stdlib.h>

typedef struct player{
gchar *name;
gboolean buy_in;
gint rebuy_amount;
gboolean add_on;
} Player;

// prototypes
void add_player(GtkWidget *button, gpointer data);
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

  // lauches the app and sends activate signal, takes command line arguments, the parsed arguments will be removed from the array
  // pressing X on the main window stores the int returned by g_application_run in status
  status = g_application_run (G_APPLICATION (app), argc, argv);

  // frees the app
  g_object_unref (app);

  return status;
}

void add_player(GtkWidget *button, gpointer data) {

GtkWindow *window;
GtkWidget *dialog;
GtkWidget *grid;
GtkWidget *name_label;
GtkWidget *name_entry;
GtkWidget *buy_in_check;
GtkWidget *buy_in_label;
GtkWidget *rebuy_label;
GtkWidget *rebuy_entry;
GtkWidget *add_on_check;
GtkWidget *add_on_label;
Player *player = (Player*) data;

window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
dialog = gtk_dialog_new_with_buttons("Add Player",
GTK_WINDOW(window),
GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
"OK",
GTK_RESPONSE_ACCEPT,
"Cancel",
GTK_RESPONSE_REJECT,
NULL);

grid = gtk_grid_new();
gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

name_label = gtk_label_new("Name:");
name_entry = gtk_entry_new();
gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);

buy_in_check = gtk_check_button_new();
buy_in_label = gtk_label_new("Buy-in");
gtk_grid_attach(GTK_GRID(grid), buy_in_check, 0, 1, 1, 1);
gtk_grid_attach(GTK_GRID(grid), buy_in_label, 1, 1, 1, 1);

rebuy_label = gtk_label_new("Rebuy amount:");
rebuy_entry = gtk_entry_new();
gtk_grid_attach(GTK_GRID(grid), rebuy_label, 0, 2, 1, 1);
gtk_grid_attach(GTK_GRID(grid), rebuy_entry, 1, 2, 1, 1);

add_on_check = gtk_check_button_new();
add_on_label = gtk_label_new("Add-on");
gtk_grid_attach(GTK_GRID(grid), add_on_check, 0, 3, 1, 1);
gtk_grid_attach(GTK_GRID(grid), add_on_label, 1, 3, 1, 1);

gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);

gtk_widget_show_all(dialog);
}

static gboolean
_label_update(gpointer data)
{
  GtkLabel *label = (GtkLabel*)data;

  static short seconds = 0;
  static short minutes = 0;
  gchar *buf = g_malloc(20 * sizeof(gchar));

  snprintf(buf, 20, "%.2d:%.2d", minutes, seconds);
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes++;
  }
  if (minutes == 60) {
    minutes = 0;
  }
  gtk_label_set_label(label, buf);
  g_free(buf);

  return continue_timer;
}

static void
_pause_timer(GtkWidget *button, gpointer data)
{
  GtkWidget *label = data;
  g_source_remove_by_user_data(label);
  if(start_timer)
  {
    continue_timer = !continue_timer;
    start_timer = !start_timer;
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
    continue_timer = !continue_timer;
    start_timer = !start_timer;
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
  // gtk_window_set_default_size (GTK_WINDOW (window), 800, 400);
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
  left_labels = gtk_label_new ("Prize 1st 2nd 3rd");
  gtk_grid_attach (GTK_GRID (main_area), left_labels, 0, 6, 3, 1);

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

  // bottom button box

  GtkWidget *button_box;
  GtkWidget *gear_button;
  GtkWidget *image;

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gear_button = gtk_button_new ();
  image = gtk_image_new_from_file ("icons/gear-icon.png");

  gtk_grid_attach (GTK_GRID (main_area), button_box, 0, 7, 1, 1);
  gtk_container_add (GTK_CONTAINER (button_box), gear_button);
  gtk_button_set_image (GTK_BUTTON (gear_button), image);

  GtkWidget *start_button;
  
  start_button = gtk_button_new_with_label ("Start/Pause");
  gtk_grid_attach (GTK_GRID (main_area), start_button, 1, 7, 1, 1);
  g_signal_connect (G_OBJECT(start_button), "clicked", G_CALLBACK (_start_timer), center_labels);

  // Add player button
  GtkWidget *add_player_button;
  Player *player;

  // Create add player button and position it in the grid
  add_player_button = gtk_button_new_with_label("Add Player");
  gtk_grid_attach(GTK_GRID(main_area), add_player_button, 2, 7, 1, 1);

  // Connect add player button to the add_player function
  g_signal_connect(G_OBJECT(add_player_button), "clicked", G_CALLBACK(add_player), player);

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
