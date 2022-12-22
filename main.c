#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Lib\cmdFunctions.c"
#include "Lib\screens.c"
#include "Lib\stonks.c"
#include "Lib\history.c"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define OsWindows 1
#endif

#ifdef linux
#define OsWindows 0
#include <unistd.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif



int main(){
	char nextScreen[10]; // this variable tells what screen should be shown next
	//SetConsoleOutputCP(1252); //Set console encoding to Windows 1252
	if (OsWindows){
		SetConsoleOutputCP(65001);  //Set console encoding to utf8
		system("color");
		system("cls");
	}
	clearScreen() ;


	/*______importStock Test ______*/
	stock *head,*current;
	head = (stock*)malloc(sizeof(stock));
	// Making the stock list so that id=-1 means that the stock is empty
	head->value.id = -1;
	head->next = NULL;
	// Importing stock data from STOCK_FILE
	importStock(head);
	/*
		product test;
		test.id = 27;
		strcpy(test.name,"hola");
		test.price = 27.5;
		test.quantity = 1;
		strcpy(test.description,"nyes");
		addProduct(head,test);
	*/
	printf("------------ %s ----------\n",STOCK_FILE);
	current = head;
	int i = 0;
	while(current != NULL ){
		printf("product %-3d:  ",i);
		printProduct(current->value);printf("\n");
		current = current->next;
		i++;
	}
	printf("\n--importing data from STOCK_FILE done\n");
	printf("--Test Completed\n");
	printf( "--press Enter to proceed to main program ... ");
	printf("\n");
	
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
		else if (strcmp(nextScreen,"STONKS") == 0){
			show_STONKS(nextScreen);
		}
		else if ( strcmp(nextScreen,"exit")== 0) break;
	}
	
	resetColor();
	/*______ End of Main Program _______*/
}