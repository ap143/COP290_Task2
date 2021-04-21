#include "start.hpp"

GtkApplication *static_app;
GtkWidget *static_window;

GtkWidget *static_user_name;
GtkWidget *static_code;
GtkWidget *static_button;

pthread_t th;

std::string code;
int response_code = 0;

void start_server(GtkApplication *app, gpointer user_data)
{
    user_name = gtk_entry_get_text(GTK_ENTRY(static_user_name));
    if (user_name.length() == 0)
    {
        return;
    }
    srand(time(0));

    code = "000000";

    for (int i = 0; i < 6; i++)
    {
        code[i] = (char) (rand()%26 + (int) 'A');
    }

    gtk_entry_set_text(GTK_ENTRY(static_code), code.c_str());

    gtk_widget_set_sensitive(static_button, FALSE);
    gtk_button_set_label(GTK_BUTTON(static_button), "Waiting...");

    response_code = 1;

    pthread_create(&th, NULL, initial_connection, NULL);
} 

void start_client(GtkApplication *app, gpointer user_data)
{
    user_name = gtk_entry_get_text(GTK_ENTRY(static_user_name));
    if (user_name.length() == 0)
    {
        return;
    }

    code = gtk_entry_get_text(GTK_ENTRY(static_code));

    gtk_widget_set_sensitive(static_button, FALSE);
    gtk_button_set_label(GTK_BUTTON(static_button), "Waiting...");

    response_code = 2;

    pthread_create(&th, NULL, initial_connection, NULL);
} 

void host_game(GtkApplication *app, gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(static_window));

    GtkWidget *window;
    GtkWidget *fixed;

    GtkWidget *button;

    GtkWidget *user_name, *code;
    GtkWidget *user_name_label, *code_label;

    int width = 300;
    int height = 200;
    int button_width = 130;
    int button_height = 20;

    window = gtk_application_window_new(static_app);
    gtk_window_set_title(GTK_WINDOW(window), "MazeWars");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    button = gtk_button_new_with_label("Host");
    gtk_fixed_put(GTK_FIXED(fixed), button, width / 2 - button_width / 2, height - button_height - 20);
    gtk_widget_set_size_request(button, button_width, button_height);

    user_name = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), user_name, 10, 40);
    gtk_widget_set_size_request(user_name, button_width * 4 / 5, button_height);

    user_name_label = gtk_label_new("Player name");
    gtk_fixed_put((GTK_FIXED(fixed)), user_name_label, 10, 10);

    code = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), code, 10, 110);
    gtk_widget_set_size_request(code, button_width * 4 / 5, button_height);
    gtk_editable_set_editable(GTK_EDITABLE(code), FALSE);

    code_label = gtk_label_new("Code");
    gtk_fixed_put((GTK_FIXED(fixed)), code_label, 10, 80);

    g_signal_connect(button, "clicked", G_CALLBACK(start_server), NULL);

    static_user_name = user_name;
    static_code = code;
    static_window = window;
    static_button = button;

    gtk_widget_show_all(window);
}

void join_game(GtkApplication *app, gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(static_window));

    GtkWidget *window;
    GtkWidget *fixed;

    GtkWidget *button;

    GtkWidget *user_name, *code;
    GtkWidget *user_name_label, *code_label;

    int width = 300;
    int height = 200;
    int button_width = 130;
    int button_height = 20;

    window = gtk_application_window_new(static_app);
    gtk_window_set_title(GTK_WINDOW(window), "MazeWars");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    button = gtk_button_new_with_label("Join");
    gtk_fixed_put(GTK_FIXED(fixed), button, width / 2 - button_width / 2, height - button_height - 20);
    gtk_widget_set_size_request(button, button_width, button_height);

    user_name = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), user_name, 10, 40);
    gtk_widget_set_size_request(user_name, button_width * 4 / 5, button_height);

    user_name_label = gtk_label_new("Player name");
    gtk_fixed_put((GTK_FIXED(fixed)), user_name_label, 10, 10);

    code = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), code, 10, 110);
    gtk_widget_set_size_request(code, button_width * 4 / 5, button_height);

    code_label = gtk_label_new("Code");
    gtk_fixed_put((GTK_FIXED(fixed)), code_label, 10, 80);

    g_signal_connect(button, "clicked", G_CALLBACK(start_client), NULL);

    static_user_name = user_name;
    static_code = code;
    static_window = window;
    static_button = button;

    gtk_widget_show_all(window);
}

void host_join(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *fixed;

    GtkWidget *host_button, *join_button;
    GtkWidget *main_title;

    int width = 300;
    int height = 80;
    int button_width = 130;
    int button_height = 20;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MazeWars");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    host_button = gtk_button_new_with_label("Host a game");
    join_button = gtk_button_new_with_label("Join a game");

    gtk_fixed_put(GTK_FIXED(fixed), host_button, 10, height / 2);
    gtk_fixed_put(GTK_FIXED(fixed), join_button, width - (10 + button_width), height / 2);
    gtk_widget_set_size_request(host_button, button_width, button_height);
    gtk_widget_set_size_request(join_button, button_width, button_height);

    g_signal_connect(host_button, "clicked", G_CALLBACK(host_game), NULL);
    g_signal_connect(join_button, "clicked", G_CALLBACK(join_game), NULL);

    main_title = gtk_label_new("Welcome to maze wars");
    gtk_fixed_put((GTK_FIXED(fixed)), main_title, width / 2 - 100, 10);

    static_app = app;
    static_window = window;

    gtk_widget_show_all(window);
}

int start(int argc, char *argv[])
{
    GtkApplication *app;

    app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(host_join), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    
    pthread_join(th, NULL);
    
    g_object_unref(app);

    return response_code;
}

void* initial_connection(void* a)
{
    // arg = "1"; Host
    // arg = "0"; Join

    if (response_code == 1)
    {
        serv = new Server("127.0.0.1");

        std::string user_code;

        do
        {
            user_code = serv->get();
            if (user_code == code)
            {
                serv->send("1");
                serv->send(user_name);
                oponent_name = serv->get();
                break;
            }
            else
            {
                serv->send("0");
            }
        } while (true);
    }
    else if (response_code == 2)
    {
        client = new Client("127.0.0.1");

        do
        {
            client->send(code);

            std::string response = client->get();

            if (response == "1")
            {
                oponent_name = client->get();
                client->send(user_name);
                break;
            }
        } while (true);
    }

    response_code = 1;
    gtk_widget_destroy(GTK_WIDGET(static_window));

    return 0;
}
