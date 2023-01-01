#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "structures.c"
#include "tablePrinter.c"
#include "selectionMenuPrinter.c"


/*
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}
*/

void sleep_ms(int milliseconds){Sleep(milliseconds);};
void printStock(stock *st);
product* getProduct(stock*st,int id);
void exportStock(stock*st);
stock* search(stock* st, int choice, char key[]);
void addProduct(stock * st, product p);
void copyProduct(product* prod1, product* prod2);
void deleteProduct(stock*st,int id);
int input_product(stock*temp_stock,_MessageFormat*msg);
int input_id(_MessageFormat*msg,_MessageFormat*error_msg);
record newRecord(int opType,product*prod,int quantity);
void appendHistory(history * ht);
void importHistory(history * ht);
void addRecord(history * ht, record p);

// these two variables are shared across all functions in order to reduce memory usage
char tempFileString[65536]; //a string variable that can be used by any function when needed 
char userInput[128]; //a string variable that can be used by any function to read user input
char LOGO_FILE[] = "Database\\logo.txt";
char MENU_FILE[] = "Database\\menu.txt";
char STOCK_FILE[] = "Database\\stock.csv";
char HISTORY_FILE[] = "Database\\history.csv";
char STONKS_FILE[] = "Database\\stonks.txt";
char FRAME_FILE[] = "Database\\frame.txt";
void resetSelectionMenuFormat(){
	for (int i = SelectionMenuFormat.current_selection;i>=1;i-- ){
		SelectionMenuFormat.current_selection --;
		printSelectionMenu(SelectionMenuFormat);
		Sleep(50);
			}
}

void show_printHistory(char nextScreen[],history*hs){
	
	HistoryTableFormat.pre_tab = 4;HistoryTableFormat.row = 4;
	HistoryTableFormat.post_tab = 1;
	printHistory(hs);
	printf("\n %58s","Appuyer sur ENTRE pour Continuer ...");
	getchar();
	strcpy(nextScreen,"main");

	
}


void show_Search(char nextScreen[],stock*st){
	
	strcpy(nextScreen,"main");
	// temp product and stock variables
	stock * temp_stock;
	temp_stock = (stock*)malloc(sizeof(stock));
	temp_stock->next = NULL;
	product * prod = &(temp_stock->value);
	prod->id = 0;prod->price = 0.0;prod->quantity = 0;
	strcpy(prod->name,"");strcpy(prod->description,"");

	// stock table settings
	StockTableFormat.pre_tab = 6;StockTableFormat.row = 9;
	StockTableFormat.post_tab = 1;
	
	// selection menu settings
	
	SelectionMenuFormat.row = 13;
	SelectionMenuFormat.pre_tab = 8;
	SelectionMenuFormat.post_tab = 4;
	strcpy(SelectionMenuFormat.selected_prefix , "> ");
	SelectionMenuFormat.selected_color = YELLOW_TXT;
	SelectionMenuFormat.normal_color = WHITE_TXT * 10;
	SelectionMenuFormat.current_selection = 0;
	SelectionMenuFormat.numberOfOptions = NumberOfaddProductOptions;
	for(int i=0;i<SelectionMenuFormat.numberOfOptions;i++)
		SelectionMenuFormat.optionsText[i] = addProductOptions[i];
	// Error Message settings
	_MessageFormat error_msg;
	char* error1="This ID already exist please try another one.";
	char* error2="Please Specify an ID";
	error_msg.text = error1;
	error_msg.row = 14;
	error_msg.pre_tab = 42;
	error_msg.color = RED_TXT*10;

	// Success message 
	_MessageFormat success_msg;
	//success_msg.text  = " Product Added Successfully.\nPress ENTER to continue ...";
	success_msg.text = " Produit Ajoute avec succes.\nAppuyez sur ENTER pour continuer...";
	
	success_msg.row = 14;
	success_msg.pre_tab = 42;
	success_msg.color = GREEN_TXT*10;
	// Title Message 
	// temp str variables
	strcpy(nextScreen,"main");
	tempFileString[0] = '\0';
	readFile(FRAME_FILE,tempFileString);
	
	char num[MAX_PRICE_DIGITS*10];
	stock*search_inp;
	stock*search_res;
	product*filter;
	while (1)
	{	
		search_res =(stock*)malloc(sizeof(stock));
		search_res->value.id = -1;
		search_res->next = NULL;
		search_inp = st;
		filter = &temp_stock->value;
		clearScreen();
		printf("%s",tempFileString);
		moveTo(4,28);setTextColorBright(GREEN_TXT);printf("CHERCHER UN PRODUIT");resetColor();
		printSelectionMenu(SelectionMenuFormat);
		
		int res = input_product(temp_stock,&error_msg);
		if (res == -1) return;

		if (filter->id >0){
			memset(num,'\0',MAX_PRICE_DIGITS*10);
			itoa(prod->id,num,10);
			search_res=search(search_inp,1,num);
			search_inp = search_res;
		}
		

		if (strlen(filter->name) != 0){
			
			search_res=search(search_inp,2,(char*)filter->name);
			search_inp = search_res;
		}
		

		if (strlen(filter->description) != 0){
			search_res=search(search_inp,5,(char*)filter->description);
			search_inp = search_res;
		}
		
		if (filter->price >0){
			memset(num,'\0',MAX_PRICE_DIGITS*10);
			itoa(prod->price,num,10);
			search_res=search(search_inp,3,num);
			search_inp = search_res;
		}
		
		if (filter->quantity >0){
			memset(num,'\0',MAX_PRICE_DIGITS*10);
			itoa(prod->quantity,num,10);
			search_res=search(search_inp,4,num);
			search_inp = search_res;
		}
		clearScreen();StockTableFormat.row = 4;
		printStock(search_res);
		StockTableFormat.row = 9;
		if (search_res->value.id == -1){
			setTextColor(RED_TXT),
			printf(" %53s","Aucun valeur trouve");
			resetColor();
		}
		printf("\n %58s","Appuyer sur ENTRE pour Continuer ...");
		getchar();
		free(search_res);
	
	}
	
	
}

void show_deleteProduct(char nextScreen[],stock*st ){
	// print the screen border
	tempFileString[0] = '\0';
	readFile(FRAME_FILE,tempFileString);
	printf("%s",tempFileString);
	// Title Message 
	moveTo(4,28);setTextColorBright(GREEN_TXT);printf("SUPPRIMER UN PRODUIT");resetColor();
	
	
	// Error Message settings
	_MessageFormat error_msg;
	error_msg.text = "This ID does not exist please try another one.";
	error_msg.row = 14;
	error_msg.pre_tab = 42;
	error_msg.color = RED_TXT*10;

	// Success message 
	_MessageFormat success_msg;
	success_msg.text = " Produit Supprime avec succes.\nAppuyez sur ENTER pour continuer...";
	//success_msg.text  = " Product Deleted Successfully.\nPress ENTER to continue ...";
	success_msg.row = 14;
	success_msg.pre_tab = 42;
	success_msg.color = GREEN_TXT*10;

	_MessageFormat input_msg;
	input_msg.text = "Entrer l'id du produit a supprimer: ";
	input_msg.row = 10;
	input_msg.pre_tab = 6;
	input_msg.color = WHITE_TXT*10;

	_MessageFormat input_int;
	input_int.text = (char*)malloc(sizeof(char)*MAX_ID_DIGITS+2);
	input_int.row = input_msg.row ;
	input_int.pre_tab = input_msg.pre_tab + strlen(input_msg.text);
	input_int.color = WHITE_TXT*10;

	strcpy(nextScreen,"main");
	int inp = 0; 
	product*product_to_edit;
	printMessage(input_msg);
	memset(input_int.text,'\0',MAX_ID_DIGITS+2);
	while(1){
		inp = input_id(&input_int,&error_msg);
		if (inp == -1) return;
		product_to_edit = getProduct(st,inp);
		if (product_to_edit->id != -1) break;
		else {
			printMessage(error_msg);
			printMessage(input_int);	
		}
	}
	deleteProduct(st,inp);
	exportStock(st);
	printMessage(success_msg);
	getchar();

	//HERE

	//Add operation to history.
	record temp_record = newRecord(0,product_to_edit,product_to_edit->quantity); 
	history * temp_history;
	temp_history = (history*)malloc(sizeof(history));
	temp_history->next = NULL;
	temp_history->value.operation_type = -1;
	addRecord(temp_history,temp_record);
	appendHistory(temp_history);
}

void show_printStock(char nextScreen[],stock*st){
	StockTableFormat.pre_tab = 4;StockTableFormat.row = 4;
	StockTableFormat.post_tab = 1;
	
	printStock(st);
	strcpy(nextScreen,"main");
	printf("\n %58s","Appuyer sur ENTRE pour Continuer ...");
	getchar();
}

void show_STONKS(char nextScreen[]){
	readFile(STONKS_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n %58s","Appuyer sur ENTRE pour Continuer ...");
	getchar();
	clearScreen();
	strcpy(nextScreen,"main");
}
void show_logo(char nextScreen[]){
	readFile(LOGO_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n %58s","Appuyer sur ENTRE pour Continuer ...");
	getchar();
	clearScreen();
	strcpy(nextScreen,"main");
}

void show_addProduct(char nextScreen[],stock*st){
	// print the screen border
	tempFileString[0] = '\0';
	readFile(FRAME_FILE,tempFileString);
	printf("%s",tempFileString);

	// temp product and stock variables
	stock * temp_stock;
	temp_stock = (stock*)malloc(sizeof(stock));
	temp_stock->next = NULL;
	product * prod = &(temp_stock->value);
	prod->id = 0;prod->price = 0.0;prod->quantity = 0;
	strcpy(prod->name,"");strcpy(prod->description,"");

	// stock table settings
	StockTableFormat.pre_tab = 6;StockTableFormat.row = 9;
	StockTableFormat.post_tab = 1;
	
	// selection menu settings
	
	SelectionMenuFormat.row = 13;
	SelectionMenuFormat.pre_tab = 8;
	SelectionMenuFormat.post_tab = 4;
	strcpy(SelectionMenuFormat.selected_prefix , "> ");
	SelectionMenuFormat.selected_color = YELLOW_TXT;
	SelectionMenuFormat.normal_color = WHITE_TXT * 10;
	SelectionMenuFormat.current_selection = 0;
	SelectionMenuFormat.numberOfOptions = NumberOfaddProductOptions;
	for(int i=0;i<SelectionMenuFormat.numberOfOptions;i++)
		SelectionMenuFormat.optionsText[i] = addProductOptions[i];
	printSelectionMenu(SelectionMenuFormat);
	
	// Error Message settings
	_MessageFormat error_msg;
	char* error1="This ID already exist please try another one.";
	char* error2="Please Specify an ID";
	error_msg.text = error1;
	error_msg.row = 14;
	error_msg.pre_tab = 42;
	error_msg.color = RED_TXT*10;

	// Success message 
	_MessageFormat success_msg;
	success_msg.text = " Produit Ajoute avec succes.\nAppuyez sur ENTER pour continuer...";
	//success_msg.text = " Product Added Successfully.\nPress ENTER to continue ...";
	
	success_msg.row = 14;
	success_msg.pre_tab = 42;
	success_msg.color = GREEN_TXT*10;
	// Title Message 
	moveTo(4,28);setTextColorBright(GREEN_TXT);printf("AJOUTER UN PRODUIT");resetColor();
	// temp str variables
	strcpy(nextScreen,"main");

	while (1)
	{
		int res = input_product(temp_stock,&error_msg);
		if (res == -1) return;
		else if (res == 0){
			wipeMessage(error_msg);
			error_msg.text = error2;
			printMessage(error_msg);
			resetSelectionMenuFormat();	
		}
		else if (getProduct(st,temp_stock->value.id)->id != -1) {
			wipeMessage(error_msg);
			error_msg.text = error1;
			printMessage(error_msg);
			resetSelectionMenuFormat();	
		}
		else {
			printMessage(success_msg);break;
		}
		    
		
	}

	addProduct(st,temp_stock->value);
	exportStock(st);
	getchar();
	//Add operation to history.
	record temp_record = newRecord(1,&temp_stock->value,temp_stock->value.quantity); 
	history * temp_history;
	temp_history = (history*)malloc(sizeof(history));
	temp_history->next = NULL;
	temp_history->value.operation_type = -1;

	addRecord(temp_history,temp_record);
	
	appendHistory(temp_history);


}
void show_updateProduct(char nextScreen[],stock*st){
	
	// print the screen border
	tempFileString[0] = '\0';
	readFile(FRAME_FILE,tempFileString);
	printf("%s",tempFileString);
	// Title Message 
	moveTo(4,28);setTextColorBright(GREEN_TXT);printf("MODIFIER UN PRODUIT");resetColor();
	
	
	// Error Message settings
	char* error1="Cet ID n'existe pas, Veuillez en essayer un autre.";
	char* error2="veuillez spécifier un Identifiant";
	_MessageFormat error_msg;
	error_msg.text = error1;//"This ID does exist please try another one.";
	error_msg.row = 14;
	error_msg.pre_tab = 42;
	error_msg.color = RED_TXT*10;

	// Success message 
	_MessageFormat success_msg;
	
	success_msg.text = " Produit Modifie avec succes.\nAppuyez sur ENTER pour continuer...";
	success_msg.row = 14;
	success_msg.pre_tab = 42;
	success_msg.color = GREEN_TXT*10;

	_MessageFormat input_msg;
	input_msg.text = "Entrer l'id du produit a modifier: ";
	input_msg.row = 10;
	input_msg.pre_tab = 6;
	input_msg.color = WHITE_TXT*10;

	_MessageFormat input_int;
	input_int.text = (char*)malloc(sizeof(char)*MAX_ID_DIGITS+2);
	input_int.row = input_msg.row ;
	input_int.pre_tab = input_msg.pre_tab + strlen(input_msg.text);
	input_int.color = WHITE_TXT*10;
		
	// temp product and stock variables
	
	stock * temp_stock;
	temp_stock = (stock*)malloc(sizeof(stock));
	//product * temp_prod = &(temp_stock->value);
	temp_stock->next = NULL;
	product * prod = &(temp_stock->value);
	prod->id = 0;prod->price = 0.0;prod->quantity = 0;
	strcpy(prod->name,"");strcpy(prod->description,"");
	
	// stock table settings
	StockTableFormat.pre_tab = 6;StockTableFormat.row = 9;
	StockTableFormat.post_tab = 1;
	
	// selection menu settings
	
	SelectionMenuFormat.row = 13;
	SelectionMenuFormat.pre_tab = 8;
	SelectionMenuFormat.post_tab = 4;
	strcpy(SelectionMenuFormat.selected_prefix , "> ");
	SelectionMenuFormat.selected_color = YELLOW_TXT;
	SelectionMenuFormat.normal_color = WHITE_TXT * 10;
	SelectionMenuFormat.current_selection = 0;
	SelectionMenuFormat.numberOfOptions = NumberOfaddProductOptions;
	for(int i=0;i<SelectionMenuFormat.numberOfOptions;i++)
		SelectionMenuFormat.optionsText[i] = addProductOptions[i];

	// temp str variables
	strcpy(nextScreen,"main");
	int inp = 0; 
	product*product_to_edit;
	printMessage(input_msg);
	memset(input_int.text,'\0',MAX_ID_DIGITS+2);
	while(1){
		inp = input_id(&input_int,&error_msg);
		if (inp == -1) return;
		product_to_edit = getProduct(st,inp);
		if (product_to_edit->id != -1) break;
		else  {
			printMessage(error_msg);
			printMessage(input_int);	
		}
	}
	wipeMessage(input_int);
	wipeMessage(input_msg);
	copyProduct(&temp_stock->value,product_to_edit);
	
	record edited_record = newRecord(3,product_to_edit, product_to_edit->quantity); 

	while (1)
	{
		int res = input_product(temp_stock,&error_msg);
		int search_res = getProduct(st,temp_stock->value.id)->id ;
		if (res == -1) return;
		else if (res == 0){
			wipeMessage(error_msg);
			error_msg.text = error2;
			printMessage(error_msg);
			resetSelectionMenuFormat();	
		}
		else if (search_res != -1 && search_res != product_to_edit->id ){
			wipeMessage(error_msg);
			error_msg.text = error1;
			printMessage(error_msg);
			resetSelectionMenuFormat();	
		}
		else {
			printMessage(success_msg);break;
		}	
	}
	strcpy(nextScreen,"main");
	copyProduct(product_to_edit,&temp_stock->value);
	exportStock(st);
	getchar();

	//Add operation to history.
	record temp_record = newRecord(3,&temp_stock->value,temp_stock->value.quantity); 
	history * temp_history, * next_history;

	temp_history = (history*)malloc(sizeof(history));
	temp_history->value.operation_type = -1;
	temp_history->next = NULL;
	
	addRecord(temp_history,edited_record);
	addRecord(temp_history,temp_record);
	
	appendHistory(temp_history);


}

//#-#-# Main Menu #-#-#//

void show_MainMenu(char nextScreen[]){
	// Main Menu and Navigation Screen
	tempFileString[0] = '\0';
	readFile(MENU_FILE,tempFileString);
	printf("%s",tempFileString);
	
	// selection menu
	int current_selection = 1;
	SelectionMenuFormat.row = 16;
	SelectionMenuFormat.pre_tab = 8;
	strcpy(SelectionMenuFormat.selected_prefix , "> ");
	SelectionMenuFormat.selected_color = MAGENTA_TXT*10;
	SelectionMenuFormat.normal_color = WHITE_TXT * 10;
	SelectionMenuFormat.current_selection = current_selection -1;
	SelectionMenuFormat.numberOfOptions = NumberOfMainMenuOptions;
	
	for (int i=0;i<SelectionMenuFormat.numberOfOptions;i++)
		SelectionMenuFormat.optionsText[i] = MainMenuOptions[i];
	printSelectionMenu(SelectionMenuFormat);
	
	while(1)
	{	
		sleep_ms(16);
		hideCursor();
		char ch = '\0';
		if(_kbhit() )ch = getch(); 
		if (ch){
			if (ch == ENTER_KEY){
				break;
			}
			else if (ch == ARROW_START){
				ch = getch();
				if (ch == ARROW_UP && current_selection>=1){//moveUp
					current_selection --; if (current_selection == 0) current_selection = NumberOfMainMenuOptions;
					SelectionMenuFormat.current_selection = current_selection-1;
					printSelectionMenu(SelectionMenuFormat);
				}
				if (ch == ARROW_DOWN && (current_selection <= NumberOfMainMenuOptions)){//moveDown
					current_selection ++;if (current_selection == NumberOfMainMenuOptions+1) current_selection = 1;
					SelectionMenuFormat.current_selection = current_selection-1;
					printSelectionMenu(SelectionMenuFormat);
				}
			}else {
				current_selection = 0; break;
			}
		}
	}
	showCursor();
	switch (current_selection)
	{
		case 0: strcpy(nextScreen,"main");break;
		case 1: strcpy(nextScreen,"add");break;
		case 2: strcpy(nextScreen,"update");break;
		case 3: strcpy(nextScreen,"delete");break;
		case 4: strcpy(nextScreen,"printstock");break;
		case 5: strcpy(nextScreen,"search");break;
		case 6: strcpy(nextScreen,"printhistory");break;
		case 7: strcpy(nextScreen,"logo");break;
		case 8: strcpy(nextScreen,"STONKS");break;
		case 9: strcpy(nextScreen,"exit");break;
	}
}

// -1 cancel
// else return id
int input_id(_MessageFormat*input_int_msg,_MessageFormat*error_msg){
	char* temp_ch = input_int_msg->text;
	strcpy(temp_ch,input_int_msg->text);
	product* prod ;
	int inp;
	while(1){
		char ch = '\0';
		sleep_ms(16);
		if(_kbhit() )ch = getch(); 
		if (ch) wipeMessage(*error_msg);
		if (ch == ESCAPE_KEY) return -1;//cancel
		if(isdigit(ch) && strlen(temp_ch)<MAX_ID_DIGITS) {
			temp_ch[strlen(temp_ch)] = ch;			
		}else if (ch ==DELETE_KEY && strlen(temp_ch)){ 
			wipeMessage(*input_int_msg);
			temp_ch[strlen(temp_ch)-1] = '\0';
		}else if (ch == ENTER_KEY && strlen(temp_ch)){

			inp = atoi(temp_ch);
			return inp;
		}
		if (ch) printMessage(*input_int_msg);
	}	
}
// -2 product already exists
// -1 cancel
// 0 id == 0
// 1 confirm
int input_product(stock*temp_stock,_MessageFormat*msg){
	
	product* prod = &(temp_stock->value);
	
	int current_selection = SelectionMenuFormat.current_selection+1;
	int temp_str_len;
	char* temp_str[NumberOfaddProductOptions] ;
	
	for (int i=0;i<NumberOfaddProductOptions;i++) {
		temp_str[i] = (char*)malloc(sizeof(char)*100);
		memset(temp_str[i],'\0',sizeof(char)*100);
	}

	itoa(prod->id,temp_str[0],10);
	strcpy(temp_str[1],prod->name);
	itoa(prod->price,temp_str[2],10); 
	itoa(prod->quantity,temp_str[3],10);
	strcpy(temp_str[4],prod->description);
	printStock(temp_stock);
	printSelectionMenu(SelectionMenuFormat);
	while(1){

	hideCursor();
	sleep_ms(16);//60 fps;
	char ch = '\0';
	if(_kbhit() )ch = getch(); 
	if (ch){
		wipeMessage(*msg);
	}
	if (ch == ESCAPE_KEY) return -1;//cancel
	if (ch == ENTER_KEY){
		if (current_selection == 7){
			return -1; // cancel
		}
		if (current_selection == 6){
			if (temp_stock->value.id == 0)
				return 0; // confirm
			//if (getProduct(st,temp_stock->value.id)->id == -1)
			//	return -2;
			return 1;
		}
	}
	else if (ch == ARROW_START){
		ch = getch();
		if (ch == ARROW_LEFT && current_selection>=6){//moveleft
			current_selection --;
			if (current_selection <6) current_selection = 7;
		}
		if (ch == ARROW_RIGHT && current_selection>=6){//moveright
			current_selection ++;
			if (current_selection > 7) current_selection =6;
		}
		if (ch == ARROW_UP && current_selection>=1){//moveUp
			current_selection --;
			if (current_selection == 7) current_selection = 5;
			if (current_selection == 0) current_selection = 6;
			
		}
		if (ch == ARROW_DOWN && (current_selection <= NumberOfaddProductOptions)){//moveDown
			current_selection ++;
			
			if (current_selection >= 7) current_selection = 1;
		}
		int svc = SelectionMenuFormat.selected_color;
		if (current_selection == 6) SelectionMenuFormat.selected_color = GREEN_TXT*10;
		else if (current_selection == 7) SelectionMenuFormat.selected_color = RED_TXT;
		SelectionMenuFormat.current_selection = current_selection-1;
		printSelectionMenu(SelectionMenuFormat);
		SelectionMenuFormat.selected_color = svc;

	} else if (ch){
		temp_str_len = strlen(temp_str[current_selection-1]);
		if (  (current_selection == 1 && (isdigit(ch) && temp_str_len < MAX_ID_DIGITS ))
		   || (current_selection == 2 && (isalnum(ch)|| ch == ' ') && temp_str_len < MAX_NAME_LENGTH )
		   || (current_selection == 3 
		   	  && (  (isdigit(ch)&& temp_str_len < MAX_PRICE_DIGITS && !strstr(temp_str[current_selection-1], "."))
			  	 || (isdigit(ch)&& strstr(temp_str[current_selection-1], ".") && temp_str_len < MAX_PRICE_DIGITS+1+PRICE_DECIMAL_DIGITS)
				 || (ch == '.' && !strstr(temp_str[current_selection-1], "."))
				 ) 
			  )
		   || (current_selection == 4 && (isdigit(ch) && temp_str_len < MAX_QUANTITY_DIGITS ) )
		   || (current_selection == 5 && (ch != DELETE_KEY || ch == ' ')&& temp_str_len < MAX_DESCRIPTION_LENGTH) )
		{
			temp_str[current_selection-1][temp_str_len] = ch;
		}
		if (ch ==DELETE_KEY && temp_str_len >0){
			temp_str[current_selection-1][temp_str_len-1] = '\0';
		}
		prod->id=atoi(temp_str[0]);
		strcpy(prod->name,temp_str[1]); //prod->name
		prod->price=atof(temp_str[2]);
		prod->quantity=atoi(temp_str[3]);
		strcpy(prod->description,temp_str[4]); //prod->description
		printStock(temp_stock);
		}
	}
}




