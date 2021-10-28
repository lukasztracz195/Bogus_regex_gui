#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//GLOBAL VARIABLES
const int WIDTH_OF_WINDOW = 500;
const int HEIGHT_OF_WINDOW = 500;
const char* NAME_OF_WINDOW = "Boguslaw Kowalski REGEX";
const char* CONTENT_OF_BANNER = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\
      Analizator skladniowy LL(1) wykozystujacy rozbior        \n\
      generacyjny zstepujacy z wyprzedzeniem o jeden symbol    \n\
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\
      KAZDE WYRAZENIE MUSI BYC ZAKONCZONE ZNAKIEM ';'         \n\
 Wprowadz w poniższe pole wyrazenie mateamtyczne do walidacji\n\
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
const char* PLACEHOLDER_ON_EXPRESSION_ENTRY = "This is please for enter your mathemathic formula";
const char* DEFAULT_RESULT_MESSAGE = "This is please on result of validation";
const char* DEFAULT_ERROR_MESSAGE = "This is please on error of validation";
const char* EMPTY_STR = "";
const char* SUCCESS_MESSAGE = "Wyrazenie jest zgodne z gramatyka\n";
const char* FAILED_MESSAGE = "Wyrazenie jest NIE zgodne z gramatyka\n";

//Uchwyty na widgety
GtkWidget *window;
GtkWidget *validation_button;
GtkWidget *reset_button;
GtkWidget *baner;
GtkWidget *label_on_result_message;
GtkWidget *label_on_error_message;
GtkWidget *vbox;
GtkWidget *entry_for_input_expression;

//Funkcje do inicjowania i obsługi widgetów
void validation(GtkWidget *widget, gpointer data) {
  const gchar* expression = gtk_entry_get_text(GTK_ENTRY(entry_for_input_expression));
	g_print("Expression = %s\n", expression);
	/*
	wynik = sprawdzWyrazenie(( char * )expression);
	g_print("WYNIK WALIDACJI: %d", wynik.isSuccess);
	g_print("ERROR WALIDACJI: %s", wynik.error);
	if(wynik.isSuccess)
	{
		gtk_label_set_text(GTK_LABEL(label_on_result_message),SUCCESS_MESSAGE);
		gtk_label_set_text(GTK_LABEL(label_on_error_message),EMPTY_STR);
	}else{
		gtk_label_set_text(GTK_LABEL(label_on_result_message),FAILED_MESSAGE);
		gtk_label_set_text(GTK_LABEL(label_on_error_message),wynik.error);
	}
	*/
}

GtkWidget* init_window(){
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH_OF_WINDOW, HEIGHT_OF_WINDOW);
  gtk_window_set_title(GTK_WINDOW(window), NAME_OF_WINDOW);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  return window;
}

GtkWidget* init_baner(){
	GtkWidget *baner;
	baner = gtk_label_new(CONTENT_OF_BANNER);
	gtk_label_set_justify(GTK_LABEL(baner), GTK_JUSTIFY_CENTER);
  return baner;
}

GtkWidget* init_entry_for_input_expression(){
	GtkWidget *entry_for_input_expression;
	entry_for_input_expression = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(entry_for_input_expression), PLACEHOLDER_ON_EXPRESSION_ENTRY);
	gtk_entry_set_visibility(GTK_ENTRY(entry_for_input_expression), TRUE);
	return entry_for_input_expression;
}

GtkWidget* init_label_on_result_of_validation(){
	GtkWidget *label_on_result_message;
	label_on_result_message = gtk_label_new(DEFAULT_RESULT_MESSAGE);
	gtk_label_set_justify(GTK_LABEL(label_on_result_message), GTK_JUSTIFY_CENTER);
  return label_on_result_message;
}

GtkWidget* init_label_on_error_of_validation(){
	GtkWidget *label_on_error_message;
	label_on_error_message = gtk_label_new(DEFAULT_ERROR_MESSAGE);
	gtk_label_set_justify(GTK_LABEL(label_on_error_message), GTK_JUSTIFY_CENTER);
  return label_on_error_message;
}

void reset_form() {
	g_print("RESET\n");
	gtk_entry_set_text(GTK_ENTRY(entry_for_input_expression),EMPTY_STR);
	gtk_entry_set_text(GTK_ENTRY(entry_for_input_expression),EMPTY_STR);
	gtk_label_set_text(GTK_LABEL(label_on_result_message),DEFAULT_RESULT_MESSAGE);
	gtk_label_set_text(GTK_LABEL(label_on_error_message),DEFAULT_ERROR_MESSAGE);
}

//PARAMETRY PROGRAMU
const int ROZMIAR_BUFFORA = 100;
char BUFFOR[100];
int indeks = 0;
char PUSTY_SYMBOL = '\0';
const int INDEKS_W_TABLICY_ASCI_ZNAKU_NOWEJ_LINI = 10;
char biezacy_symbol;
char BLEDNY_SYMBOL = 0;
bool BLAD = false;
int i = 0;

int main(int argc, char *argv[]) {
    

  gtk_init(&argc, &argv);
	
	//Initailizacja obiektów view
  window = init_window();
  baner = init_baner();
  label_on_result_message = init_label_on_result_of_validation();
  label_on_error_message = init_label_on_error_of_validation();
  entry_for_input_expression = init_entry_for_input_expression();
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  validation_button = gtk_button_new_with_label("Validate");
  reset_button = gtk_button_new_with_label("Reset Form");
  
  //Pozycjonowanie elementów
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start(GTK_BOX(vbox), baner, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry_for_input_expression, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), validation_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), validation_button, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label_on_result_message, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label_on_error_message, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), reset_button, TRUE, TRUE, 0);
  
  //Przypisanie akcji do widgetów
  g_signal_connect(G_OBJECT(validation_button), "clicked", G_CALLBACK(validation), NULL);
  
    g_signal_connect(G_OBJECT(reset_button), "clicked", G_CALLBACK(reset_form), NULL);
    
  //Pokazanie okna 
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
