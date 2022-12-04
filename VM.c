#include <gtk/gtk.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TamanhoDosQuadros 600
#define TAMANHO_DE_MEMORIA 50

GFile* file;
GtkTextBuffer* tb2;
GtkTextBuffer* tb3;
GtkTextBuffer* tb4;
GtkEntryBuffer* bufent;
GtkWidget *window;
GtkWidget* entry;
GtkWidget* tv1;
GtkWidget* tv4;

GtkWidget* btno;
GtkWidget* btnsbs;
GtkWidget* btnrun;

FILE* fptr;
int M[TAMANHO_DE_MEMORIA];
int s;
int i;
char P[100][26];
int stepbystep = 0;
int wait = 0;

static void enter_text (GtkEntry* , gpointer );
static void click_open (GtkButton* , gpointer );

void update_tb2 (){
    int j = TAMANHO_DE_MEMORIA-1;
    gtk_text_buffer_set_text(tb2,"",-1);
    for(;j>=0;j--){
        GtkTextIter end;
		char linha [50];
        sprintf(linha,"%d\t%d\n",j+1,M[j]);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER (tb2),&end);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER (tb2),&end,linha,-1);
	}
    return;
}

void update_tb3 (int num){
    GtkTextIter end;
    char text [20];
    sprintf(text,"%d",num);
    strcat(text,"\n");
    g_print("oi");
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER (tb3),&end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER (tb3),&end,text,-1);
    return;   
}

void update_tb4 (){
    char text[30];
    sprintf(text,"i:%d\ts:%d",i,s);
    gtk_text_buffer_set_text(tb4,text,-1);
    return;
}
int LoadFile(char* filename){
	if ((fptr = fopen(filename, "r")) == NULL) {
    	printf("Deu ruim!!!");
    	exit(1);
	}
	char currentchar;
	currentchar = fgetc(fptr);
	int r = 0;
	int c = 0;
	while(currentchar != EOF){
		if(currentchar == '\n'){
			P[r][c] = '\0';
			r++;
			c=0;
		}
		else{
			P[r][c] = currentchar;
			c++;
		}
		currentchar = fgetc(fptr);
	}
    fclose(fptr);
	return r;
}

int FetchLabel(int label){
	char rotulo [5];
	char comando [9];
	int j,k;
	k=0;
	for(j=0;j<4;j++){
		rotulo[j] = P[k][j];
	}
	rotulo[4] = '\0';
	
	for(j=0;j<8;j++){
		comando[j] = P[k][j+4];	
	}
	comando[8] = '\0';
	int flag = 0;
	
	while(strcmp(comando,"HLT     ") && atoi(rotulo) != label){
		
		k++;
		for(j=0;j<4;j++){
			rotulo[j] = P[k][j];
		}
		rotulo[4] = '\0';
		
		for(j=0;j<8;j++){
			comando[j] = P[k][j+4];	
		}
		comando[8] = '\0';
		
		if(!strcmp(comando,"HLT     ")){
			flag = 1;
		}
	}
	if(flag){
		return -1;
	}
	else{
		return k;
	}
}

//Executa o comando na linha definida
int Exec(int l){
	char rotulo [5];
	char comando [9];
	char param1 [5];
	char param2 [5];
	
	int j;
	for(j=0;j<4;j++){
		rotulo[j] = P[l][j];
		param1[j] = P[l][j+12];
		param2[j] =	P[l][j+16];
	}
	rotulo[4] = '\0';
	param1[4] = '\0';
	param2[4] = '\0';
	
	for(j=0;j<8;j++){
		comando[j] = P[l][j+4];	
	}
	comando[8] = '\0';
	//Se o comando for START
	if(!strcmp(comando,"START   ")){
		s=-1;
        gtk_text_buffer_set_text(tb3,"",-1);
		i++;
	}
	else{
		//Se for alocacao
		if(!strcmp(comando,"ALLOC   ")){
			int m,n,k;
			m=atoi(param1);
			n=atoi(param2);
			for(k=0;k<n;k++){
				s++;
				M[s]=M[m+k];
			}
			i++;
		}
		else{
			//Se for dealocacao
			if(!strcmp(comando,"DALLOC  ")){
				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("dalloc %d %d\n",m,n);
				for(k=n-1;k>=0;k--){
					M[m+k]= M[s];
					s--;
				}
				i++;
			}
			else{
				//Se for chamada
				if(!strcmp(comando,"CALL    ")){
					int p;
					s++;
					M[s]=i+1;
					p=atoi(param1);
					i = FetchLabel(p);
					printf("call %d\n",p);
				}
				else{
					//Se for retorno
					if(!strcmp(comando,"RETURN  ")){
						printf("return %d\n",s);
						i=M[s];
						s--;
					}
					else{
						//Se for HALT
						if(!strcmp(comando,"HLT     ")){
							printf("Halt\n");
                            gtk_widget_set_sensitive(GTK_WIDGET(btno),TRUE);
							return -1;
						}
						else{
							//Se for leitura
							if(!strcmp(comando,"RD      ")){
                                
								GtkTextIter end;
                                char text [20]= "Digite um numero:";
                                strcat(text,"\n");
                                gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER (tb3),&end);

                                gtk_text_buffer_insert(GTK_TEXT_BUFFER (tb3),&end,text,-1);
								s++;
								wait = 1;
                                gtk_widget_set_sensitive(GTK_WIDGET(entry),TRUE);
                                gtk_widget_set_sensitive(GTK_WIDGET(btnrun),FALSE);
                                gtk_widget_set_sensitive(GTK_WIDGET(btnsbs),FALSE);
                                update_tb2();
							}
							else{
								//Se for escrita
								if(!strcmp(comando,"PRN     ")){
									update_tb3(M[s]);
									s--;
									i++;
								}
								else{
									//Se for carregar constante
									if(!strcmp(comando,"LDC     ")){
										int k = atoi(param1);
										printf("%d",k);
										s++;
										M[s] = k;
										i++;
									}
									else{
										//Se for carregar variavel
										if(!strcmp(comando,"LDV     ")){
											int n = atoi(param1);
											s++;
											M[s] = M[n];
											i++;
										}
										else{
											//Se for adicao
											if(!strcmp(comando,"ADD     ")){
												M[s-1] = M[s-1] + M[s];
												s--;
												i++;
											}
											else{
												//Se for subtracao
												if(!strcmp(comando,"SUB     ")){
													M[s-1] = M[s-1] - M[s];
													s--;
													i++;
												}
												else{
													//Se for multiplicacao
													if(!strcmp(comando,"MULT    ")){
														M[s-1] = M[s-1] * M[s];
														s--;
														i++;
													}
													else{
														//Se for divisao
														if(!strcmp(comando,"DIVI    ")){
															M[s-1] = M[s-1] / M[s];
															s--;
															i++;
														}
														else{
															//Se for inverter sinal
															if(!strcmp(comando,"INV     ")){
																M[s] = -(M[s]);
																i++;
															}
															else{
																//Se for "e"
																if(!strcmp(comando,"AND     ")){
																	if(M[s-1] == 1 && M[s] == 1){
																		M[s-1] = 1;
																	}
																	else{
																		M[s-1] = 0;
																	}
																	s--;
																	i++;
																}
																else{
																	//Se for "ou"
																	if(!strcmp(comando,"OR      ")){
																		if(M[s-1] == 1 || M[s] == 1){
																			M[s-1] = 1;
																		}
																		else{
																			M[s-1] = 0;
																		}
																		s--;
																		i++;
																	}
																	else{
																		//Se for negacao
																		if(!strcmp(comando,"NEG     ")){
																			M[s] = 1-M[s];
																			i++;
																		}
																		else{
																			//Se for menor
																			if(!strcmp(comando,"CME     ")){
																				if(M[s-1] < M[s]){
																					M[s-1] = 1;
																				}
																				else{
																					M[s-1] = 0;
																				}
																				s--;
																				i++;
																			}
																			else{
																				//Se for maior
																				if(!strcmp(comando,"CMA     ")){
																					if(M[s-1] > M[s]){
																						M[s-1] = 1;
																					}
																					else{
																						M[s-1] = 0;
																					}
																					s--;
																					i++;
																				}
																				else{
																					//Se for igual
																					if(!strcmp(comando,"CEQ     ")){
																						if(M[s-1] == M[s]){
																							M[s-1] = 1;
																						}
																						else{
																							M[s-1] = 0;
																						}
																						s--;
																						i++;
																					}
																					else{
																						//Se for nao igual
																						if(!strcmp(comando,"CDIF    ")){
																							if(M[s-1] != M[s]){
																								M[s-1] = 1;
																							}
																							else{
																								M[s-1] = 0;
																							}
																							s--;
																							i++;
																						}
																						else{
																							//Se for menor ou igual
																							if(!strcmp(comando,"CMEQ    ")){
																								if(M[s-1] <= M[s]){
																									M[s-1] = 1;
																								}
																								else{
																									M[s-1] = 0;
																								}
																								s--;
																								i++;
																							}
																							else{
																								//Se for maior ou igual
																								if(!strcmp(comando,"CMAQ    ")){
																									if(M[s-1] >= M[s]){
																										M[s-1] = 1;
																									}
																									else{
																										M[s-1] = 0;
																									}
																									s--;
																									i++;
																								}
																								else{
																									//Se for armazenamento
																									if(!strcmp(comando,"STR     ")){
																										int n;
																										n = atoi(param1);
																										M[n] = M[s];
																										s--;
																										i++;
																									}
																									else{
																										//Se for pulo
																										if(!strcmp(comando,"JMP     ")){
																											int p;
																											p = atoi(param1);
																											i = FetchLabel(p);
																										}
																										else{
																											//Se for pulo quando false
																											if(!strcmp(comando,"JMPF    ")){
																												if(M[s] == 0){
																													int p = atoi(param1);
																													i = FetchLabel(p);
																												}
																												else{
																													i++;
																												}
																												s--;
																											}
																											else{
																												//Se nao for null
																												if(strcmp(comando,"NULL    ")){
																													return 1;
																												}
																												else{
																													i++;
																												}
																											}
																										}
																									}
																								}	
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
    if(stepbystep){
        update_tb2();   
    }
    update_tb4();
	return 0;	
}

int run_all (){
    int halt = 0;
    stepbystep = 0;
    do{
        halt=Exec(i);
    }while(halt == 0 && wait == 0);
    if(halt){    
        update_tb2();
        GtkTextIter end;
        char text [20]= "Fim do Programa";
        strcat(text,"\n");
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER (tb3),&end);
        gtk_text_buffer_insert(GTK_TEXT_BUFFER (tb3),&end,text,-1);
        i=0;
        int j;
        for(j=0;j<TAMANHO_DE_MEMORIA;j++){
		    M[j]=0;
	    }
        gtk_widget_set_sensitive(btno,TRUE);
    }
    g_print("xau");
    return halt;
}




static void on_response_open (GtkNativeDialog *native, int response, gpointer used_data){
    char* contents;
    gsize length; 
    GtkTextBuffer* tb;
    tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW (used_data));   
    if (response == GTK_RESPONSE_ACCEPT){
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
      file = gtk_file_chooser_get_file (chooser);
      LoadFile (g_file_get_path(file));
      if(g_file_load_contents (file, NULL, &contents, &length, NULL, NULL)){
        gtk_text_buffer_set_text (tb, contents, length);
        int lines = gtk_text_buffer_get_line_count (tb);
        g_print("%d\n",lines);
        int row;        
        for(row = 0; row<lines;row++ ){
            GtkTextIter startline;
            gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(tb),&startline,row);
            char numero [6];
            sprintf(numero,"%d\t",row+1);
            gtk_text_buffer_insert(GTK_TEXT_BUFFER (tb),&startline,numero,-1);
        }
        g_free (contents);
      }
    }
    g_object_unref (native);
    return;
}

static void click_open (GtkButton* btn, gpointer used_data){
    GtkFileChooserNative *native;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    
    native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (window),
                                        action,
                                        "_Open",
                                        "_Cancel");

    g_signal_connect (native, "response", G_CALLBACK (on_response_open), used_data);
    gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
    return;
}

static void click_run (GtkButton* btn, gpointer used_data){
    gtk_widget_set_sensitive(GTK_WIDGET(btno),FALSE);
    run_all();
    return;
}

static void click_sbs (GtkButton* btn, gpointer used_data){
    stepbystep = 1;
    g_print("%d\n",i);    
    gtk_widget_set_sensitive(GTK_WIDGET(btno),FALSE);
    Exec(i);
   return; 
}

static void enter_text (GtkEntry* ent, gpointer used_data){
    GtkEntryBuffer* buf;
    buf = gtk_entry_get_buffer(GTK_ENTRY(used_data)) ;   
    char* text = gtk_entry_buffer_get_text (GTK_ENTRY_BUFFER(buf));
    int k = atoi(text);
    M[s] = k;
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buf),"",-1);
    wait = 0;
    gtk_widget_set_sensitive(GTK_WIDGET(btnsbs),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(btnrun),TRUE);
    i++;
    if(!stepbystep){
        //run_all();        
    }
    return;
}

static void app_open (GtkApplication* app, gpointer user_data){
    GtkWidget* boxv;    
    GtkWidget* boxv2;
    GtkWidget* boxv3;
    GtkWidget* boxh;
    GtkWidget* boxh2;
    GtkWidget* boxh3;
    GtkWidget* dmy1;
    GtkWidget* dmy2;
    GtkWidget* dmy21;
    GtkWidget* dmy22;
    GtkWidget* dmy23;
    GtkWidget* dmy31;
    GtkWidget* dmy32;
    GtkWidget* dmy33;
    GtkWidget* tv2;
    GtkWidget* tv3;
    GtkWidget* scr1;
    GtkWidget* scr2;
    GtkWidget* scr3;
    GtkTextBuffer* tb1; 
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Virtual Machine");
    gtk_window_maximize (GTK_WINDOW (window));
    
    boxv = gtk_box_new(GTK_ORIENTATION_VERTICAL,5); 

    //Linha de cima
    gtk_window_set_child(GTK_WINDOW (window), boxv);
    boxh = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_append(GTK_BOX(boxv),boxh);
    btno = gtk_button_new_with_label("Open");
    btnsbs = gtk_button_new_with_label("Step by Step");
    btnrun = gtk_button_new_with_label("Run");
    dmy1 = gtk_label_new (NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy1), 1);
    dmy2 = gtk_label_new (NULL);
    gtk_widget_set_hexpand(dmy2,TRUE);
    gtk_box_append(GTK_BOX(boxh),dmy1);
    gtk_box_append(GTK_BOX(boxh),btno);    
    gtk_box_append(GTK_BOX(boxh),dmy2);

    //Centro
    boxh2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
    dmy21 = gtk_label_new(NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy21), 3);
    dmy22 = gtk_label_new(NULL);
    gtk_widget_set_hexpand(dmy22,TRUE);
    dmy23 = gtk_label_new(NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy23), 3);
    
    scr1 = gtk_scrolled_window_new();
    tv1 = gtk_text_view_new();
    tb1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(tv1));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tv1), FALSE);    
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv1), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr1), tv1);    
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW (scr1),TamanhoDosQuadros);    
    

    gtk_text_view_set_monospace(GTK_TEXT_VIEW(tv1),TRUE);

    scr2 = gtk_scrolled_window_new();
    tv2 = gtk_text_view_new();
    tb2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(tv2));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tv2), FALSE);    
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv2), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr2), tv2);   
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW (scr2),TamanhoDosQuadros);    
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(tv2),TRUE);
    gtk_text_buffer_set_text(tb2,"",-1);

    gtk_widget_set_vexpand(scr1,TRUE);
    gtk_widget_set_vexpand(scr2,TRUE);


    gtk_box_append(GTK_BOX(boxh2),dmy21);
    gtk_box_append(GTK_BOX(boxh2),scr1);
    gtk_box_append(GTK_BOX(boxh2),dmy22);
    gtk_box_append(GTK_BOX(boxh2),scr2);
    gtk_box_append(GTK_BOX(boxh2),dmy23);
    


    gtk_box_append (GTK_BOX (boxv),boxh2); 
    //Baixo
    boxh3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    boxv2 = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
    boxv3 = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);    
    
    dmy31 = gtk_label_new(NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy31), 3);
    dmy32 = gtk_label_new(NULL);
    gtk_widget_set_hexpand(dmy32,TRUE);   
    dmy33 = gtk_label_new(NULL);
    gtk_label_set_width_chars (GTK_LABEL (dmy33), 3);

    scr3 = gtk_scrolled_window_new();
    tv3 = gtk_text_view_new();
    tb3 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(tv3));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tv3), FALSE);    
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv3), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr3), tv3);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW (scr3),100);    
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW (scr3),TamanhoDosQuadros);    
    gtk_text_buffer_set_text(tb3,"",-1);

    entry = gtk_entry_new();
    bufent = gtk_entry_get_buffer (GTK_ENTRY (entry));
    gtk_entry_set_activates_default(GTK_ENTRY (entry), TRUE);
    gtk_entry_set_input_purpose(GTK_ENTRY (entry),GTK_INPUT_PURPOSE_DIGITS);
    gtk_widget_set_sensitive(GTK_WIDGET(entry),FALSE);
    

    
    tv4 = gtk_text_view_new();
    tb4 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(tv4));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (tv4), FALSE);    
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv4), GTK_WRAP_WORD_CHAR);
    gtk_text_buffer_set_text(tb4,"",-1);


    
    gtk_box_append (GTK_BOX (boxv),boxh3);
    
    
    gtk_box_append (GTK_BOX (boxh3),dmy31);
    gtk_box_append (GTK_BOX (boxh3),boxv2);
    gtk_box_append (GTK_BOX (boxh3),dmy32);
    gtk_box_append (GTK_BOX (boxh3),boxv3);
    gtk_box_append (GTK_BOX (boxh3),dmy33);
    gtk_box_append (GTK_BOX (boxv2),scr3);
    gtk_box_append (GTK_BOX (boxv2),entry);
    gtk_box_append (GTK_BOX (boxv3),tv4);
    gtk_box_append (GTK_BOX (boxv3),btnsbs);
    gtk_box_append (GTK_BOX (boxv3),btnrun);
    



    //Signals
    g_signal_connect (entry,"activate",G_CALLBACK (enter_text),entry);
    g_signal_connect (btno,"clicked",G_CALLBACK(click_open),tv1);
    g_signal_connect (btnrun,"clicked", G_CALLBACK(click_run),NULL);
    g_signal_connect (btnsbs,"clicked", G_CALLBACK(click_sbs),NULL);
    gtk_widget_show (window);
 } 

int main (int argc, char** argv){
    
    GtkApplication* app;
    int stat;
    int j;
    for(j=0;j<TAMANHO_DE_MEMORIA;j++){
		M[j]=0;
	}
    app = gtk_application_new ("VM.c",G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (app_open), NULL);
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return stat;
}
