#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//ZMIENNE GLOBALNE
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
const char* DEFAULT_ERROR_MESSAGE = "";
const char* EMPTY_STR = "";
const char* SUCCESS_MESSAGE = "Wyrazenie jest zgodne z gramatyka\n";
const char* FAILED_MESSAGE = "Wyrazenie jest NIE zgodne z gramatyka\n";

//PARAMETRY PROGRAMU
const int ROZMIAR_BUFFORA = 100;
char BUFFOR[100];
char BUFFOR_OF_ERROR[100];
char PUSTY_SYMBOL = '\0';
const int INDEKS_W_TABLICY_ASCI_ZNAKU_NOWEJ_LINI = 10;
char biezacy_symbol;
char BLEDNY_SYMBOL = 0;
bool BLAD = false;
int i = 0;
int indeks = 0;
enum Pierwsze {
	PierwszeProdukcjaS = 0,
	PierwszeProdukcjaZ = 1,
	PierwszeProdukcjaW = 2,
	PierwszeProdukcjaP = 3,
	PierwszeProdukcjaR = 4,
	PierwszeProdukcjaL = 5,
	PierwszeProdukcjaC = 6,
	PierwszeProcukcjaO = 7,
	PierwszeProdukcjaWprim = 8,
	PierwszeProdukcjaRprim = 9,
	PierwszeProdukcjaLprim = 10
};

char* NAME_OF_PRODUCTIONS[] = {
"FIRST_PRODUCTION_S",
"FIRST_PRODUCTION_Z",
"FIRST_PRODUCTION_W",
"FIRST_PRODUCTION_P",
"FIRST_PRODUCTION_R",
"FIRST_PRODUCTION_L",
"FIRST_PRODUCTION_C",
"FIRST_PRODUCTION_O",
"FIRST_PRODUCTION_Wprim",
"FIRST_PRODUCTION_Rprim",
"FIRST_PRODUCTION_Lprim"
};

//ZDEFINIOWANE ZBIORY SYMBOLI NALEZACYCH DO ZBIOROW FIRST DLA POSZCZEGOLNYCH PRODUKCJI
char FIRST_PRODUCTION_S[] = { '(','0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_Z[] = { '(','0','1','2','3','4','5','6','7','8','9','\0' };
char FIRST_PRODUCTION_W[] = { '(','0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_P[] = { '(','0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_R[] = { '0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_L[] = { '0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_C[] = { '0','1','2','3','4','5','6','7','8','9' };
char FIRST_PRODUCTION_O[] = { '*','+','-',':','^' };
char FIRST_PRODUCTION_Wprim[] = { '*','+','-',':','^','\0' };
char FIRST_PRODUCTION_Rprim[] = { '.',' ' };
char FIRST_PRODUCTION_Lprim[] = { '0','1','2','3','4','5','6','7','8','9','\0' };

//FUNCKJE POMOCNICZE
int rozmiarTablicyZnakow(char **array);
void wyczyscBuffor();
void wyczyscBufforError();
bool znajdzSymbolWProdukcji(char symbol, int production);
char nastepnySymbolZInkrementacja();
char nastepnySymbolBezInkrementacja();
void sygnalizujBlad();
void wczytaj();
void print_result(bool wynik);
bool sprawdzWyrazenie(char* expression);
void reset_form();
void reset_data_to_validation();
//FUNKCJE REALIZUJACE OBSLUGE ZNAKU NALEZACEGO DO DANEJ PRODUKCJI
void S();
void Z();
void W();
void Wprim();
void P();
void R();
void Rprim();
void L();
void Lprim();
void C();
void O();

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

bool sprawdzWyrazenie(char* expression){
	reset_data_to_validation();
	sprintf(BUFFOR , "%s\n\n", expression);
	biezacy_symbol = BUFFOR[indeks];
	S();
	int diff = strcmp(BUFFOR_OF_ERROR, EMPTY_STR);
	if(diff == 0)
	{
		return true;
	}else{
		return false;
	}
}

void validation(GtkWidget *widget, gpointer data) {
  const gchar* expression = gtk_entry_get_text(GTK_ENTRY(entry_for_input_expression));
	bool isSuccess = sprawdzWyrazenie((char*)expression);
	if(isSuccess)
	{
		gtk_label_set_text(GTK_LABEL(label_on_result_message),SUCCESS_MESSAGE);
	}else
	{
		gtk_label_set_text(GTK_LABEL(label_on_result_message),FAILED_MESSAGE);
	}
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

void reset_data_to_validation(){
	i = 0;
	indeks = 0;
	wyczyscBuffor();
	wyczyscBufforError();
	BLAD = false;
}

void reset_form() {
	reset_data_to_validation();
	gtk_entry_set_text(GTK_ENTRY(entry_for_input_expression),EMPTY_STR);
	gtk_entry_set_text(GTK_ENTRY(entry_for_input_expression),EMPTY_STR);
	gtk_label_set_text(GTK_LABEL(label_on_result_message),DEFAULT_RESULT_MESSAGE);
	gtk_label_set_text(GTK_LABEL(label_on_error_message),DEFAULT_ERROR_MESSAGE);
}

//PONIZEJ ZNAJDUJE SIE IMPLEMENTACJA FUNCKI POMOCNICZYCH
int rozmiarTablicyZnakow(char **array)
{
	return sizeof(array) - 1;
}

void wyczyscBuffor()
{
	for (i = 0; i < ROZMIAR_BUFFORA; i++)
	{
		BUFFOR[i] = '\0';
	}
}

void wyczyscBufforError()
{
	for (i = 0; i < ROZMIAR_BUFFORA; i++)
	{
		BUFFOR_OF_ERROR[i] = '\0';
	}
}

bool znajdzSymbolWProdukcji(char symbol, int production)
{
	char *prod = NULL;
	int size = 0;
	if(symbol != INDEKS_W_TABLICY_ASCI_ZNAKU_NOWEJ_LINI)
	{		
		switch (production)
		{
			case PierwszeProdukcjaS:
				{
					prod = FIRST_PRODUCTION_S;
					size = sizeof(FIRST_PRODUCTION_S);
					break;
				}
			case PierwszeProdukcjaZ:
				{
					prod = FIRST_PRODUCTION_Z;
					size = sizeof(FIRST_PRODUCTION_Z);
					break;
				}
			case PierwszeProdukcjaW:
				{
					prod = FIRST_PRODUCTION_W;
					size = sizeof(FIRST_PRODUCTION_W);
					break;
				}
			case PierwszeProdukcjaP:
				{
					prod = FIRST_PRODUCTION_P;
					size = sizeof(FIRST_PRODUCTION_P);
					break;
				}
			case PierwszeProdukcjaR:
				{
					prod = FIRST_PRODUCTION_R;
					size = sizeof(FIRST_PRODUCTION_R);
					break;
				}
			case PierwszeProdukcjaL:
				{
					prod = FIRST_PRODUCTION_L;
					size = sizeof(FIRST_PRODUCTION_L);
					break;
				}
			case PierwszeProdukcjaC:
				{
					prod = FIRST_PRODUCTION_C;
					size = sizeof(FIRST_PRODUCTION_C);
					break;
				}
			case PierwszeProcukcjaO:
				{
					prod = FIRST_PRODUCTION_O;
					size = sizeof(FIRST_PRODUCTION_O);
					break;
				}
			case PierwszeProdukcjaWprim:
				{
					prod = FIRST_PRODUCTION_Wprim;
					size = sizeof(FIRST_PRODUCTION_Wprim);
					break;
				}
			case PierwszeProdukcjaRprim:
				{
					prod = FIRST_PRODUCTION_Rprim;
					size = sizeof(FIRST_PRODUCTION_Rprim);
					break;
				}
			case PierwszeProdukcjaLprim:
				{
					prod = FIRST_PRODUCTION_Lprim;
					size = sizeof(FIRST_PRODUCTION_Lprim);
					break;
				}
		}

		for (i = 0; i < size; i++)
		{
			if (prod[i] == symbol)
			{
				return true;
			}
		}	
	}
	return false;
}

char nastepnySymbolZInkrementacja()
{
	if (indeks + 1 < ROZMIAR_BUFFORA)
	{
		indeks++;
	}
	else
	{
		g_print("Indeks: %d", indeks);
	}
	return BUFFOR[indeks];
}

char nastepnySymbolBezInkrementacja()
{
	return BUFFOR[indeks + 1];
}

void sygnalizujBlad()
{
	BLAD = true;
	BLEDNY_SYMBOL = biezacy_symbol;
	//g_print("%s", BUFFOR_OF_ERROR);
}

void wczytaj()
{
	biezacy_symbol = nastepnySymbolZInkrementacja();
	if(BLAD)
	{
		sprintf(BUFFOR_OF_ERROR, "Syntax BLAD!");
		gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
	}
}
//PONIZEJ ZNAJDUJĄ SIE IMPLEMENTACJE FUNkCJI REALIZUJACYCH POSZCZEGOLNE PRODUKCJE

//PRODUKCJA S
void S()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaW))
	{
		W();
		if (biezacy_symbol != PUSTY_SYMBOL && biezacy_symbol == ';')
		{
			wczytaj();
			Z();
		}
		else
		{
			if (biezacy_symbol != PUSTY_SYMBOL)
			{
				sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji S] Oczekiwano: ';', znaleziono: \'%c\'\n", biezacy_symbol);
				gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
				sygnalizujBlad();
			}
		}
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji S] Oczekiwano: FIRST(W), znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUCKJA Z
void Z()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaW))
	{
		W();
		if ( biezacy_symbol == ';')
		{
			wczytaj();
			if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaZ))
			{
				Z();
			}
			else
			{
				if (biezacy_symbol != PUSTY_SYMBOL && biezacy_symbol != INDEKS_W_TABLICY_ASCI_ZNAKU_NOWEJ_LINI)
				{
					sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji Z] Oczekiwano: FIRST(Z), znaleziono: \'%c\'\n", biezacy_symbol);
					gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
					sygnalizujBlad();
				}
			}
		}
		else
		{
			if (biezacy_symbol != PUSTY_SYMBOL)
			{
				sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji Z] Oczekiwano: ';', znaleziono: \'%c\'\n", biezacy_symbol);
				gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
				sygnalizujBlad();
			}
		}
	}
}

//PRODUKCJA W
void W()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaP))
	{
		P();
		Wprim();
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji W] Oczekiwano: FIRST(P), znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUKCJA W'
void Wprim()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProcukcjaO))
	{
		O();
		if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaW))
		{
			W();
		}
		else
		{
			if (biezacy_symbol != PUSTY_SYMBOL || biezacy_symbol == INDEKS_W_TABLICY_ASCI_ZNAKU_NOWEJ_LINI)
			{
				sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji W_PRIM] Oczekiwano: FIRST(W), znaleziono: \'%c\'\n", biezacy_symbol);
				gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
				sygnalizujBlad();
			}
		}
	}
}

//PRODUKCJA P
void P()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaR))
	{
		
		R();
	}
	else if (biezacy_symbol != PUSTY_SYMBOL && biezacy_symbol == '(')
	{
		wczytaj();
		if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaW))
		{
			W();
			if (biezacy_symbol != PUSTY_SYMBOL && biezacy_symbol == ')')
			{
				wczytaj();
			}
			else
			{
				if (biezacy_symbol != PUSTY_SYMBOL)
				{
					sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji P] Oczekiwano: ')', znaleziono: \'%c\'\n", biezacy_symbol);
					gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
					sygnalizujBlad();
				}
			}
		}
		else
		{
			if (biezacy_symbol != PUSTY_SYMBOL)
			{
				sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji P] Oczekiwano: FIRST(W), znaleziono: \'%c\'\n", biezacy_symbol);
				gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
				sygnalizujBlad();
			}
		}
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji P] Oczekiwano: FIRST(R) lub '(', znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUKCJA R
void R()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaL))
	{
		
		L();
		Rprim();
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji R] Oczekiwano: FIRST(L), znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUKCJA R'
void Rprim()
{
	if (biezacy_symbol != PUSTY_SYMBOL && biezacy_symbol == '.' )
	{
		wczytaj();
		if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaL))
		{
			L();
		}
		else
		{
			if (biezacy_symbol != PUSTY_SYMBOL )
			{
				sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji R'] Oczekiwano: FRIST(L), znaleziono: \'%c\'\n", biezacy_symbol);
				gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
				sygnalizujBlad();
			}
		}
	}
}

//PRODUKCJA L
void L()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaC))
	{
		C();
		Lprim();
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji L] Oczekiwano: FIRST(C), znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUKCJA L'
void Lprim()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaL))
	{
		L();
	}
}

//PRODUKCJA C
void C()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProdukcjaC)) 
	{
		wczytaj();
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji C] Oczekiwano: cyfra, znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

//PRODUKCJA O
void O()
{
	if (znajdzSymbolWProdukcji(biezacy_symbol, PierwszeProcukcjaO))
	{
		wczytaj();
	}
	else
	{
		if (biezacy_symbol != PUSTY_SYMBOL)
		{
			sprintf(BUFFOR_OF_ERROR, "[Blad w produkcji O] Oczekiwano: operator, znaleziono: \'%c\'\n", biezacy_symbol);
			gtk_label_set_text(GTK_LABEL(label_on_error_message),BUFFOR_OF_ERROR);
			sygnalizujBlad();
		}
	}
}

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
  
  wyczyscBuffor();
	wyczyscBufforError();
  //Przypisanie akcji do widgetów
  g_signal_connect(G_OBJECT(validation_button), "clicked", G_CALLBACK(validation), NULL);
  
    g_signal_connect(G_OBJECT(reset_button), "clicked", G_CALLBACK(reset_form), NULL);
    
  //Pokazanie okna 
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}


