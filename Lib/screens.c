#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "structures.c"

#include "tablePrinter.c"
#include "selectionMenuPrinter.c"
#include <ctype.h>
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
void printStock(stock *st);
product* getProduct(stock*st,int id);
void exportStock(stock*st);
stock* search(stock* st, int choice, char key[]);
void addProduct(stock * st, product p);

// these two variables are shared across all functions in order to reduce memory usage
char tempFileString[65536]; //a string variable that can be used by any function when needed 
char userInput[128]; //a string variable that can be used by any function to read user input
char LOGO_FILE[] = "Database\\logo.txt";
char MENU_FILE[] = "Database\\menu.txt";
char STOCK_FILE[] = "Database\\stock.csv";
char HISTORY_FILE[] = "Database\\history.csv";
char STONKS_FILE[] = "Database\\stonks.txt";
char FRAME_FILE[] = "Database\\frame.txt";

void show_historique(char nextScreen[]){
	
}

//TODO: controle saisie 
//TODO: 
void show_update(char nextScreen[], stock * st){
	printf("\n2. Update Name of the product? ");
    printf("\n3. Update Price of the product?");
    printf("\n4. Update Quantitiy of the product?");
    printf("\n5. Update Discription of the product?");
    printf("\nEnter your choice:");
	int choice ;
    scanf("%d", &choice);
	
	printf("Input the id of the item you want to update from the stock: ");
	int temp_id;
	scanf("%d", &temp_id);
	char new_name[64];
	double new_price;
	int new_quantity;
	char new_description[256];

	product* temp_prod = getProduct(st,temp_id);
	if (temp_prod == NULL) return;
	switch (choice) {
		case 2:
			printf("Enter new Name: ");
			gets(new_name);gets(new_name);
			strcpy(temp_prod->name,new_name);
			//updateProductName(st,temp_id,new_name);
			break;
		case 3:
			printf("Enter the new price: ");
			scanf("%lf",&new_price);
			printf(">>>>>>>>>>>< new zebi zebi %f",new_price);
			getchar();getchar();
			temp_prod->price = new_price;
			
			//updateProductPrice(st,temp_id,new_price);
			break;
		case 4:
			printf("Enter the new quantity: ");
			scanf("%d",&new_quantity);
			printf(">>>>>>>>>>>< new zebi %d",new_quantity);
			temp_prod->quantity = new_quantity;
			//updateProductQuantity(st,temp_id,new_quantity);
			break;
		case 5:
			printf("Enter new Description: ");
			gets(new_description);gets(new_description);
			strcpy(temp_prod->description,new_description);
			//updateProductDescription(st,temp_id,new_description);
			break;			
	}
	clearScreen();
	exportStock(st);
	printf("------------ %s ----------\n",STOCK_FILE);printStock(st);
	getchar();
	
	strcpy(nextScreen,"main");
	
}

void show_STONKS(char nextScreen[]){
	readFile(STONKS_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n %58s","Press Enter to Continue ...");
	getchar();
	clearScreen();
	strcpy(nextScreen,"main");
}
void show_logo(char nextScreen[]){
	readFile(LOGO_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n %58s","Press Enter to Continue ...");
	getchar();
	clearScreen();
	strcpy(nextScreen,"main");
}
int NumberOfaddProductOptions = 7;
char *addProductOptions[]={
						 "1 - Identifiant\n",
					     "2 - Nom\n",
						 "3 - Prix\n",
						 "4 - Quantite\n",
						 "5 - Description du produit  \n\n",
						 "**-CONFIRMER.   ", "**-ANNULER.  "
						 };

void show_addProduct(char nextScreen[],stock*st){
	// print the screen border
	tempFileString[0] = '\0';
	readFile(FRAME_FILE,tempFileString);
	printf("%s",tempFileString);

	// temp product and stock variables
	product * prod;
	stock * temp_stock;
	temp_stock = (stock*)malloc(sizeof(stock));
	temp_stock->next = NULL;
	prod = &(temp_stock->value);prod->id = 0;prod->price = 0.0;prod->quantity = 0;
	strcpy(prod->name,"");strcpy(prod->description,"");

	// stock table settings
	StockTableFormat.pre_tab = 6;StockTableFormat.row = 9;
	StockTableFormat.post_tab = 1;
	printStock(temp_stock);

	// selection menu settings
	int current_selection = 1;
	SelectionMenuFormat.row = 13;
	SelectionMenuFormat.pre_tab = 8;
	SelectionMenuFormat.post_tab = 4;
	strcpy(SelectionMenuFormat.selected_prefix , "> ");
	SelectionMenuFormat.selected_color = YELLOW_TXT;
	SelectionMenuFormat.normal_color = WHITE_TXT * 10;
	SelectionMenuFormat.current_selection = current_selection -1;
	SelectionMenuFormat.numberOfOptions = NumberOfaddProductOptions;
	for(int i=0;i<SelectionMenuFormat.numberOfOptions;i++)
		SelectionMenuFormat.optionsText[i] = addProductOptions[i];
	printSelectionMenu(SelectionMenuFormat);
	
	// Error Message settings
	_MessageFormat error_msg;
	error_msg.text = "This ID already exist please try another one.";
	error_msg.row = 14;
	error_msg.pre_tab = 42;
	error_msg.color = RED_TXT*10;

	// Success message 
	_MessageFormat success_msg;
	success_msg.text = " Product Added Successfully.\nPress ENTER to continue ...";
	success_msg.row = 14;
	success_msg.pre_tab = 42;
	success_msg.color = GREEN_TXT*10;
	// Title Message 
	moveTo(4,28);setTextColorBright(GREEN_TXT);printf("AJOUTER UN PRODUIT");resetColor();
	// temp str variables
	int temp_str_len;
	char* temp_str[NumberOfaddProductOptions] ;
	
	for (int i=0;i<NumberOfaddProductOptions;i++) {
		temp_str[i] = (char*)malloc(sizeof(char)*100);
		memset(temp_str[i],'\0',sizeof(char)*100);
	}
	
	while(1){

	hideCursor();
	sleep_ms(16);//60 fps;
	char ch = '\0';
	if(_kbhit() )ch = getch(); 	
	if (ch == 13){
		if (current_selection == 7){
			strcpy(nextScreen,"main");
			break;
		}
		if (current_selection == 6){
			char num[50];
			itoa(prod->id,num,10);
			
			stock * res = search(st,1,num);
			moveRight(8);
			//moveTo(CURSOR.upper_border+NumberOfaddProductOptions+2,CURSOR.col);
			if (res->value.id == -1){
				addProduct(st,*prod);
				printMessage(success_msg);
				getchar();
				strcpy(nextScreen,"main");
				break;

				
			}else {
				printMessage(error_msg);
				for (int i = current_selection;i>=1;i-- ){
					current_selection = i;
					SelectionMenuFormat.current_selection = current_selection-1;
					printSelectionMenu(SelectionMenuFormat);
					Sleep(50);
				}
			}	
		}
	}
	else if (ch == -32){
		ch = getch();
		wipeMessage(error_msg);
		if (ch == 77 && current_selection>=6){//moveleft
			current_selection --;
			if (current_selection <6) current_selection = 7;
		}
		if (ch == 75 && current_selection>=6){//moveright
			current_selection ++;
			if (current_selection > 7) current_selection =6;
		}
		if (ch == 72 && current_selection>=1){//moveUp
			current_selection --;
			if (current_selection == 0) current_selection = NumberOfaddProductOptions-1;
		}
		if (ch == 80 && (current_selection <= NumberOfaddProductOptions)){//moveDown
			current_selection ++;
			if (current_selection == NumberOfaddProductOptions) current_selection = 1;
		}
		int svc = SelectionMenuFormat.selected_color;
		if (current_selection == 6) SelectionMenuFormat.selected_color = GREEN_TXT*10;
		else if (current_selection == 7) SelectionMenuFormat.selected_color = RED_TXT;
		SelectionMenuFormat.current_selection = current_selection-1;
		printSelectionMenu(SelectionMenuFormat);
		SelectionMenuFormat.selected_color = svc;

	} else if (ch){
		wipeMessage(error_msg);
		temp_str_len = strlen(temp_str[current_selection-1]);
		if (  (current_selection == 1 && (isdigit(ch) && temp_str_len < 8 ))
		   || (current_selection == 2 && (isalnum(ch)|| ch == ' ') && temp_str_len < 20 )
		   || (current_selection == 3 && (isdigit(ch)|| (ch == '.' && !strstr(temp_str[current_selection-1], "."))) && temp_str_len < 8 )
		   || (current_selection == 4 && (isdigit(ch) && temp_str_len < 8 ) )
		   || (current_selection == 5 && (ch != 8 || ch == ' ')&& temp_str_len < 60) )
		{
			temp_str[current_selection-1][temp_str_len] = ch;
		}
		if (ch ==8 && temp_str_len >0){
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

//#-#-# Main Menu #-#-#//
int NumberOfMainMenuOptions = 8;
char *MainMenuOptions[]={
						 "1 - Ajouter un produit.\n",
					     "2 - Afficher la description d’un produit.\n",
						 "3 - Supprimer un produit.\n",
						 "4 - Modifier un produit.\n",
						 "5 - Recherche d’un produit dans le stock.\n",
						 "6 - Show Logo.\n",  
						 "7 - STONKS ? \n",
						 "8 - Exit.\n"
						 };

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
		sleep_ms(16);//60 fps;
		hideCursor();
		char ch = '\0';
		if(_kbhit() )ch = getch(); 
		if (ch){
			if (ch == 13){
				break;
			}
			else if (ch == -32){
				ch = getch();
				if (ch == 72 && current_selection>=1){//moveUp
					current_selection --; if (current_selection == 0) current_selection = NumberOfMainMenuOptions;
					//showMainMenuSelection(current_selection);	
					SelectionMenuFormat.current_selection = current_selection-1;
					printSelectionMenu(SelectionMenuFormat);
				}
				if (ch == 80 && (current_selection <= NumberOfMainMenuOptions)){//moveDown
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
		case 2: strcpy(nextScreen,"main");break;
		case 3: strcpy(nextScreen,"main");break;
		case 4: strcpy(nextScreen,"update");break;
		case 5: strcpy(nextScreen,"main");break;
		case 6: strcpy(nextScreen,"logo");break;
		case 7: strcpy(nextScreen,"STONKS");break;
		case 8: strcpy(nextScreen,"exit");break;
	}
	
	
}



