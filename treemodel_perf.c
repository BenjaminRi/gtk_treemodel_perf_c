#include <stdio.h>
#include <time.h>

#include <gtk/gtk.h>

enum {
  U64_COLUMN = 0,
  NUM_COLS
};


static GtkTreeStore* create_store(const unsigned size) {
	GtkTreeStore* treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_UINT64);
	//Note: If we add a sorted model, the runtime explodes even more
	//GtkTreeModel* sortmodel = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(treestore));
	
	{
		GtkTreeIter iter;
		unsigned size_it = size;
		while(size_it--) {
			gtk_tree_store_append(treestore, &iter, NULL);
			//gtk_tree_store_set(treestore, &iter, U64_COLUMN, (guint64) size_it, -1);
		}
	}
	
	
	{
		GtkTreeIter iter;
		gtk_tree_model_get_iter_first(GTK_TREE_MODEL(treestore), &iter);
		const unsigned long time_before = (unsigned long) time(NULL);
		do {
			gtk_tree_store_set(treestore, &iter, U64_COLUMN, (guint64) 99, -1);
		} while(gtk_tree_model_iter_next (GTK_TREE_MODEL(treestore), &iter));
		const unsigned long time_after = (unsigned long) time(NULL);
		fprintf(stdout, "%u,%lu\n", size, time_after - time_before); 
	}
	
	return treestore;
}

static void activate (GtkApplication* app, gpointer user_data) {
	GtkTreeStore* treestore = create_store(15000);
	
	GtkTreeModel* model = GTK_TREE_MODEL(treestore);
	GtkWidget* view = gtk_tree_view_new();

	GtkTreeViewColumn* col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "U64 column");

	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", U64_COLUMN);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	
	GtkWidget* scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		GTK_POLICY_AUTOMATIC,
		GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scrolled_window), FALSE);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	
	GtkWidget* window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "TreeModel Perf");
	gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
	gtk_container_add (GTK_CONTAINER (window), scrolled_window);
	gtk_widget_show_all (window);
}

int main (int argc, char **argv) {
	GtkApplication *app;
	int status;
	
	app = gtk_application_new ("com.github.BenjaminRi.treemodel_perf", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	
	return status;
}

