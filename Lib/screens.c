#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.c"
// these two variables are shared across all functions in order to reduce memory usage
char tempFileString[999999999]; //a string variable that can be used by any function when needed 
char userInput[128]; //a string variable that can be used by any function to read user input
char LOGO_FILE[] = "Database\\logo.txt";
char MENU_FILE[] = "Database\\menu.txt";
char STOCK_FILE[] = "Database\\stock.csv";
char STONKS_FILE[] = "Database\\stonks.txt";
//#______ Table Priter _______#//
void _printTableTopLine(int width,int cols,int n) {
	printf("┏");
	for (int i=0; i<width-2; i++){
		
	}
	printf("┓");
}
void _printTableInsideLine(int width) {
	printf("┣");for (int i=0; i<width-2; i++)printf("━");printf("┫");
}
void _printTableBottomLine(int width) {
	printf("┗");for (int i=0; i<width-2; i++)printf("━");printf("┛");
}
void _printTableStr(int width,int before_tab_size,char str[]){
	int len = strlen(str);
	printf("┃"); 
	for(int i=0; i<before_tab_size;i++) printf(" ");
	printf("%s",str); 
	printf("┃"); 
	for(int i=0; i<(width-before_tab_size-len-3);i++) printf(" ");

	printf("┃");
}

//#____ End of Table Printer _____#//



void printProduct(product prod){
	//convinience method to print a product variable
	printf("%-3d, %-11s, %-9.3f,%5d,%s",prod.id,prod.name,prod.price,prod.quantity,prod.description);
}
void show_historique(char nextScreen[]){

	
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
	else if (strcmp(userInput,"6")== 0) strcpy(nextScreen,"logo");
	else if (strcmp(userInput,"7")== 0) strcpy(nextScreen,"STONKS");
	else if (strcmp(userInput,"8")== 0) strcpy(nextScreen,"exit");
}


void show_Screen1(char nextScreen[]){
	// Screen 1
	printf("This is Screen 1");
	gets(userInput);
	strcpy(nextScreen,"main"); // nextScreen = "main"
}
