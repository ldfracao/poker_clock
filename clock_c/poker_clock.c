#include <gtk/gtk.h>

void end_program (void)
{
    gtk_main_quit ();
}

int main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *btn = gtk_button_new_with_label ("Close window");
    g_signal_connect (btn, "clicked", G_CALLBACK (end_program), NULL);
    g_signal_connect (win, "delete_event", G_CALLBACK (end_program), NULL);

    gtk_container_add (GTK_CONTAINER (win), btn);

    gtk_widget_show_all (win);

    gtk_widget_show (win);

    gtk_main ();
    gtk_main_quit();

    return 0;
}