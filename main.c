#include <gtk/gtk.h>
#include <time.h>

// Global pointers and variables
GtkListStore *transaction_store;
GtkBuilder *builder;
double budget = 0;       // Store the set budget
double remaining_budget = 0;  // Track the remaining budget

// Function to handle "Set Budget" button click
void on_set_budget_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *budget_entry = GTK_ENTRY(user_data);
    const gchar *budget_text = gtk_entry_get_text(budget_entry);

    // Convert budget amount to double
    budget = g_ascii_strtod(budget_text, NULL);
    remaining_budget = budget;  // Initialize remaining budget

    if (budget > 0) {
        g_print("Budget set to: %.2f\n", budget);
        gtk_widget_set_visible(GTK_WIDGET(budget_entry), FALSE);
    } else {
        g_print("Invalid budget amount entered.\n");
    }
}

// Function to handle "Transaction" button click
void on_transaction_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *transaction_box = GTK_WIDGET(user_data);
    gboolean visible = gtk_widget_get_visible(transaction_box);
    gtk_widget_set_visible(transaction_box, !visible);
}

// Function to handle "View Report" button click
void on_view_report_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *report_scrolled_window = GTK_WIDGET(user_data);
    gboolean visible = gtk_widget_get_visible(report_scrolled_window);
    gtk_widget_set_visible(report_scrolled_window, !visible);
}

// Function to show a warning dialog if budget is exceeded
void show_budget_warning() {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_builder_get_object(builder, "main_window")),
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_WARNING,
                                    GTK_BUTTONS_OK,
                                    "Warning: Expenses have exceeded the budget!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to add a transaction and update remaining budget
void add_transaction(GtkButton *button, gpointer user_data) {
    // Retrieve transaction data widgets
    GtkComboBoxText *type_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "income_expense_combobox"));
    GtkComboBoxText *description_combobox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "description_combobox"));
    GtkEntry *amount_entry = GTK_ENTRY(gtk_builder_get_object(builder, "amount_entry"));

    // Get values
    const gchar *type = gtk_combo_box_text_get_active_text(type_combobox);
    const gchar *description = gtk_combo_box_text_get_active_text(description_combobox);
    const gchar *amount_text = gtk_entry_get_text(amount_entry);
    double amount = g_ascii_strtod(amount_text, NULL);

    if (amount <= 0) {
        g_print("Please enter a valid amount.\n");
        return;
    }

    // Update remaining budget if it's an expense
    if (g_strcmp0(type, "Expense") == 0) {
        remaining_budget -= amount;
    }

    // Check if budget exceeded
    if (remaining_budget < 0) {
        show_budget_warning();
    }

    // Add the transaction to the ListStore (model for the TreeView)
    GtkTreeIter iter;
    gtk_list_store_append(transaction_store, &iter);

    // Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    gchar date_str[11];
    g_snprintf(date_str, sizeof(date_str), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    gtk_list_store_set(transaction_store, &iter,
                       0, date_str,
                       1, type,
                       2, description,
                       3, amount_text,
                       -1);

    g_print("Transaction added: Date=%s, Type=%s, Description=%s, Amount=%.2f\n", date_str, type, description, amount);
    g_print("Remaining budget: %.2f\n", remaining_budget);

    // Clear the amount entry for the next input
    gtk_entry_set_text(amount_entry, "");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "Unsaved 1.glade", NULL)) {
        g_printerr("Error loading file\n");
        return 1;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    GtkWidget *transaction_box = GTK_WIDGET(gtk_builder_get_object(builder, "transaction_box"));
    GtkWidget *budget_entry = GTK_WIDGET(gtk_builder_get_object(builder, "budget_entry"));
    GtkWidget *report_scrolled_window = GTK_WIDGET(gtk_builder_get_object(builder, "report_scrolled_window"));

    // Set up the ListStore for transactions
    transaction_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkTreeView *report_treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "report_treeview"));
    gtk_tree_view_set_model(report_treeview, GTK_TREE_MODEL(transaction_store));
    g_object_unref(transaction_store);

    // Connect signals
    GtkWidget *transaction_button = GTK_WIDGET(gtk_builder_get_object(builder, "transaction_button"));
    g_signal_connect(transaction_button, "clicked", G_CALLBACK(on_transaction_button_clicked), transaction_box);

    GtkWidget *set_budget_button = GTK_WIDGET(gtk_builder_get_object(builder, "set_budget_button"));
    g_signal_connect(set_budget_button, "clicked", G_CALLBACK(on_set_budget_button_clicked), budget_entry);

    GtkWidget *view_report_button = GTK_WIDGET(gtk_builder_get_object(builder, "view_report_button"));
    g_signal_connect(view_report_button, "clicked", G_CALLBACK(on_view_report_button_clicked), report_scrolled_window);

    GtkWidget *add_transaction_button = GTK_WIDGET(gtk_button_new_with_label("Add Transaction"));
    gtk_box_pack_start(GTK_BOX(transaction_box), add_transaction_button, FALSE, FALSE, 0);
    g_signal_connect(add_transaction_button, "clicked", G_CALLBACK(add_transaction), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
