#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <string.h>

char ofilename[100];
GtkTextBuffer* tb;
GtkWidget* win;
GFile* file;

int isnew = 1;
/* Define TfeTextView Widget which is the child object of GtkTextView */

#define TFE_TYPE_TEXT_VIEW tfe_text_view_get_type ()
G_DECLARE_FINAL_TYPE (TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)

struct _TfeTextView
{
  GtkTextView parent;
  GFile *file;
};

G_DEFINE_TYPE (TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW);

static void tfe_text_view_init (TfeTextView *tv) {
}

static void tfe_text_view_class_init (TfeTextViewClass *class) {
}

void tfe_text_view_set_file (TfeTextView *tv, GFile *f) {
  tv -> file = f;
}

GFile* tfe_text_view_get_file (TfeTextView *tv) {
  return tv -> file;
}

GtkWidget* tfe_text_view_new (void) {
  return GTK_WIDGET (g_object_new (TFE_TYPE_TEXT_VIEW, NULL));
}

/* Fim da definicao de GtkTextView*/

static void click_new (GtkButton* btn, gpointer used_data){
    
    isnew = 1;
    gtk_text_buffer_set_text (tb, "", -1);
    tfe_text_view_set_file (TFE_TEXT_VIEW (used_data), NULL);
    strcpy (ofilename,"");
    file = NULL;
}

static void click_compile (GtkButton* btn, gpointer used_data){
    if(isnew){
        gtk_text_buffer_set_text (used_data, "Arquivo de programa nao salvo", -1);        
    }
    else{
    char cfilename[100];
    strcpy(cfilename,ofilename);
    int len = strlen(cfilename);
    cfilename[len-4] = '\0';
    pid_t cpid;
    int stat;
    if(fork()==0){
        char* args[] = {"./c",ofilename,cfilename,NULL};
        execv("./c",args);
    }
    else{
        cpid = wait(&stat);    
    }
    if (WIFEXITED(stat)){
        FILE* flog;
        if ((flog = fopen("log.txt", "r")) == NULL) {
          printf("Deu ruim!!!");
          exit(1);
        }
        char string[100];
        fgets(string,100,flog);
        gtk_text_buffer_set_text (used_data, string, -1);        
        
    }
    else
    if (WIFSIGNALED(stat))
        psignal(WTERMSIG(stat), "Exit signal");
    }
    
}

static void on_response_open (GtkNativeDialog *native, int response, gpointer used_data){
    char* contents;
    gsize length;    
    if (response == GTK_RESPONSE_ACCEPT){
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
      file = gtk_file_chooser_get_file (chooser);
      strcpy(ofilename, g_file_get_path(file));
      if(g_file_load_contents (file, NULL, &contents, &length, NULL, NULL)){
        gtk_text_buffer_set_text (tb, contents, length);
        g_free (contents);
      }
    }
    tfe_text_view_set_file(TFE_TEXT_VIEW (used_data),file);
    g_object_unref (native);
}

static void click_open (GtkButton* btn, gpointer used_data){
    isnew = 0;
    GtkFileChooserNative *native;
    tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (used_data));
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (win),
                                        action,
                                        "_Open",
                                        "_Cancel");

    g_signal_connect (native, "response", G_CALLBACK (on_response_open), used_data);
    gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
    
}

static void save_to_file(GFile* file, gpointer used_data){
    GtkWidget* tv;
    char* pathname;
    GtkTextIter start_iter;
    GtkTextIter end_iter;
    char* contents;
    unsigned int n;
    unsigned int i;
    
    tv = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (used_data));
    file = tfe_text_view_get_file (TFE_TEXT_VIEW (tv));
    tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
    gtk_text_buffer_get_bounds (tb, &start_iter, &end_iter);
    contents = gtk_text_buffer_get_text (tb, &start_iter, &end_iter, FALSE);
    if(! g_file_replace_contents (file, contents, strlen(contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, NULL)){
        pathname = g_file_get_path (file);
        g_print ("ERRO: nao foi possivel salvar em %s",pathname);
        g_free (pathname);        
    }
    g_free (contents);
}

static void on_response_save (GtkNativeDialog *native, int response, gpointer used_data){
    GtkWidget* tv;
    if (response == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
        file = gtk_file_chooser_get_file (chooser);
        tv = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (used_data));
        tfe_text_view_set_file(TFE_TEXT_VIEW (tv), file);
        save_to_file (file,used_data);

        g_object_unref (file);
        isnew = 0;
    }

    g_object_unref (native);
}

static void click_save (GtkButton* btn, gpointer used_data){
    GtkWidget* tv;
    if(isnew){
        GtkFileChooserNative *native;
        GtkFileChooser *chooser;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

        native = gtk_file_chooser_native_new ("Save File",
                                        GTK_WINDOW (win),
                                        action,
                                        "_Save",
                                        "_Cancel");
        chooser = GTK_FILE_CHOOSER (native);

        if (isnew)
            gtk_file_chooser_set_current_name (chooser, "Untitled document");
        else
            gtk_file_chooser_set_file (chooser, file, NULL);

        g_signal_connect (native, "response", G_CALLBACK (on_response_save), used_data);
        gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
    }
    if(!isnew){
        tv = gtk_scrolled_window_get_child (GTK_SCROLLED_WINDOW (used_data));
        file = tfe_text_view_get_file (TFE_TEXT_VIEW (tv));
        save_to_file (file,used_data);
    }
}
static void app_activate (GApplication* app, gpointer used_data){
    g_print (" Voce precisa de um nome de arquivo como argumento\n");
}

static void app_open (GApplication* app, GFile** files, gint n_files, gchar* hint, gpointer used_data){
    
    GtkWidget* nb;
    GtkWidget* lab;
    GtkNotebookPage *nbp;
    GtkWidget* scr;
    GtkWidget* scr2;
    GtkWidget* tv;
    char* contents;
    gsize length;
    char* filename;
    int i;
    
    GtkWidget* boxv;
    GtkWidget* boxh;
    GtkWidget* tv1;
    GtkTextBuffer* tb1;
    GtkWidget* dmy1;
    GtkWidget* dmy2;
    GtkWidget* dmy3;
    GtkWidget* btnn;    //Botao para novo
    GtkWidget* btno;    //Botao para abrir
    GtkWidget* btns;    //Botao para salvar
    GtkWidget* btnc;    //Botao para compilar

    win = gtk_application_window_new (GTK_APPLICATION (app));
    gtk_window_set_title (GTK_WINDOW (win), "Compilador");
    gtk_window_maximize (GTK_WINDOW (win));

    boxv = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);
    gtk_window_set_child (GTK_WINDOW (win), boxv);
    
    boxh = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append (GTK_BOX (boxv), boxh);
    
    
    dmy1 = gtk_label_new (NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy1), 1);
    dmy2 = gtk_label_new (NULL);
    gtk_widget_set_hexpand (dmy2, TRUE);
    dmy3 = gtk_label_new (NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy3), 10);
    btnn = gtk_button_new_with_label ("New");
    btno = gtk_button_new_with_label ("Open");
    btns = gtk_button_new_with_label ("Save");
    btnc = gtk_button_new_with_label ("Compile");

    gtk_box_append (GTK_BOX (boxh), dmy1);
    gtk_box_append (GTK_BOX (boxh), btnn);
    gtk_box_append (GTK_BOX (boxh), btno);
    gtk_box_append (GTK_BOX (boxh), btns);
    gtk_box_append (GTK_BOX (boxh), dmy2);
    gtk_box_append (GTK_BOX (boxh), btnc);
    gtk_box_append (GTK_BOX (boxh), dmy3);

    scr = gtk_scrolled_window_new ();
    tv = tfe_text_view_new ();
    tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);
            
    gtk_text_buffer_set_text(tb,"",-1);
    gtk_widget_set_hexpand (tv,TRUE);
    gtk_widget_set_vexpand (tv,TRUE);        
    //file = g_file_dup(files[0]);
    //tfe_text_view_set_file (TFE_TEXT_VIEW (tv), g_file_dup (files[0]));
          
    gtk_box_append (GTK_BOX (boxv), scr);
     
            
    char* text = "";
    scr2 = gtk_scrolled_window_new();
    gtk_scrolled_window_set_has_frame (GTK_SCROLLED_WINDOW (scr2), TRUE);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW (scr2),150);
    gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW (scr2),TRUE);
    tv1 = gtk_text_view_new ();
    tb1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv1));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tv1), FALSE);
    gtk_text_buffer_set_text(tb1, text, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv1), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr2), tv1);
    gtk_box_append (GTK_BOX (boxv), scr2);
        
    gtk_widget_show (win);
    
    g_signal_connect(btnc, "clicked", G_CALLBACK (click_compile), tb1);   
    g_signal_connect(btno, "clicked", G_CALLBACK (click_open), tv);
    g_signal_connect(btns, "clicked", G_CALLBACK (click_save), scr);
    g_signal_connect(btnn, "clicked", G_CALLBACK (click_new), tv);
}

int main (int argc, char** argv){
    GtkApplication* app;
    int stat;
    app = gtk_application_new ("tfe2.c",G_APPLICATION_HANDLES_OPEN);
    g_signal_connect (app, "activate", G_CALLBACK (app_open), NULL);
    g_signal_connect (app, "open", G_CALLBACK (app_open), NULL);
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return stat;
}

