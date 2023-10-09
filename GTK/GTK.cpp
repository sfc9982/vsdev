#include <cstdio>
#include <gtk/gtk.h>
#include <windows.h>


void get_current_time(GtkButton *btn, GtkLabel *label) {
    SYSTEMTIME st, lt;
    printf("Button Clicked!\n");

    GetSystemTime(&st);
    GetLocalTime(&lt);

    char buf[1024];
    sprintf(buf, "%02d:%02d:%02d %d/%d/%d", st.wHour, st.wMinute, st.wSecond, st.wDay, st.wMonth, st.wYear);
    gtk_label_set_label(label, buf);
}

static void on_activate(GtkApplication *app) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);

    GtkWidget *label = gtk_label_new("Hello GTK!");
    GtkWidget *btn   = gtk_button_new_with_label("Get Current Time");
    gtk_widget_set_margin_start(GTK_WIDGET(btn), 4);
    gtk_widget_set_margin_end(GTK_WIDGET(btn), 4);
    g_signal_connect_after(btn, "clicked", (GCallback)get_current_time, label);

    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), btn);

    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    // Create a new application
    GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
