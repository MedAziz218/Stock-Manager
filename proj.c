#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cmdFunctions.c"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#define DIV 1048576 
#define WIDTH 7
#define OsWindows 1
#endif

#ifdef linux
#define OsWindows 0
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif




// these two variables are shared across all functions in order to reduce memory usage
char tempFileString[20000]; //a string variable that can be used by any function when needed 
char userInput[128]; //a string variable that can be used by any function to read user input
char LOGO_FILE[] = "logo.txt";
char MENU_FILE[] = "menu.txt";
char STOCK_FILE[] = "stock.csv";

//TODO: last modified date
typedef struct product{
	int id;
	char name[64];
	double price;
	int quantity;
	char description[256];
}  product;

typedef struct stock{
	product value;
	struct stock* next;
}stock;
void printProduct(product prod){
	//convinience method to print a product variable
	printf("%-3d, %-11s, %-9.3f,%5d,%s",prod.id,prod.name,prod.price,prod.quantity,prod.description);
}

void show_logo(char nextScreen[]){
	readFile(LOGO_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n %58s","Press Enter to Continue ...");
	getchar();
	clearScreen();
	strcpy(nextScreen,"main");
}
void show_MainMenu(char nextScreen[]){
	// Main Menu and Navigation Screen
	tempFileString[0] = '\0'; // clear 
	readFile(MENU_FILE,tempFileString);
	printf("%s",tempFileString);
	gets(userInput);
	if (strcmp(userInput,"1")==0) strcpy(nextScreen,"1");
	else if (strcmp(userInput,"6")== 0) strcpy(nextScreen,"exit");
}

void show_Screen1(char nextScreen[]){
	// Screen 1
	printf("This is Screen 1");
	gets(userInput);
	strcpy(nextScreen,"main"); // nextScreen = "main"
}

void copyProduct(product *prod1,product *prod2){
	// Convinience function to copy product variables
	prod1->id = prod2->id; strcpy(prod1->name,prod2->name);
	prod1->price = prod2->price; prod1->quantity = prod2->quantity;
	strcpy(prod1->description,prod2->description);	
}

void addProduct(stock * st, product p){
	// Adds a new product to the stock
	stock * ptr,*aux;	
	
	if (st->value.id == -1){
		// If the stock is empty then just set the value of the first element 
		copyProduct(&(st->value),&p); //printProduct((st->value));printf("\n");
		
	} else {
		// If Not iterate throught the list and add a new element at the end
		ptr = (stock*)malloc(sizeof(stock));	
		ptr->next = NULL;
		aux = st;
		copyProduct(&(ptr->value),&p);
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = ptr; //printProduct((st->next->value));printf("\n");
	}
	
}

int importStock(stock * st){
	// This imports data from the csv file STOCK_FILE provided
	FILE* ptr; char ch; product temp_prod;
	ptr = fopen(STOCK_FILE, "r"); //opening file
	char temp_str1[400],temp_str2[400],temp_str3[400],temp_str4[400],temp_str5[400];
	int i=0; int scan_result;
	do {
		scan_result = fscanf(ptr, "%400[^;];%400[^;];%400[^;];%400[^;];%400[^\n]", 
					  temp_str1, temp_str2, temp_str3,temp_str4, temp_str5);
		ch = fgetc(ptr);
		if (i>0 && scan_result == 5){
			temp_prod.id = atoi(temp_str1);
			strcpy(temp_prod.name,temp_str2);
			temp_prod.price = atof(temp_str3);
			temp_prod.quantity = atoi(temp_str4);
			strcpy(temp_prod.description,temp_str5);
			addProduct(st,temp_prod);
		}
		i++;
	}while(ch != EOF);
	fclose(ptr);
}


int main(){
	char nextScreen[10]; // this variable tells what screen should be shown next
	//SetConsoleOutputCP(1252); //Set console encoding to Windows 1252
	if (OsWindows){
		SetConsoleOutputCP(65001);  //Set console encoding to utf8
		system("color");
	}
	clearScreen() ;//system("cls");


	/*______importStock Test ______*/
	stock *head,*current;
	head = (stock*)malloc(sizeof(stock));
	// Making the stock list so that id=-1 means that the stock is empty
	head->value.id = -1;
	head->next = NULL;
	// Importing stock data from STOCK_FILE
	importStock(head);
	printf("------------ %s ----------\n",STOCK_FILE);
	current = head;
	int i = 0;
	while(current != NULL){
			printf("product %-3d:  ",i);
			printProduct(current->value);printf("\n");
			current = current->next;
			i++;
		}
	printf("\n--importing data from STOCK_FILE done\n");
	printf("--Test Completed\n");
	printf( "--press Enter to proceed to main program ... ");
	getchar();
	/*______ End of Test_______*/

	

	/*______ Main Program ______*/
	

	strcpy(nextScreen,"logo"); //set current screen to main Menu
	while(1){
		
		clearScreen();
		//strcmp(str1,str2) -> return (0 if str1 == str2 else 1)
		//Each Screen should set tell us what screen should be displayed next
		if (strcmp(nextScreen,"logo") == 0){
			show_logo(nextScreen);
		}
		if (strcmp(nextScreen,"main") == 0){
			show_MainMenu(nextScreen);
		}
		else if ( strcmp(nextScreen,"1") == 0){
			show_Screen1(nextScreen);
		}
		else if ( strcmp(nextScreen,"exit")== 0) break;
	}
	
	resetColor();
	/*______ End of Main Program _______*/
}
