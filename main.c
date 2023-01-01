#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include "Lib\cmdFunctions.c"
#include "Lib\screens.c"
#include "Lib\stonks.c"
#include "Lib\history.c"
#define OsWindows 1
#endif

#ifdef linux
#define OsWindows 0
#include <unistd.h>
#include "Lib/cmdFunctions.c"
#include "Lib/screens.c"
#include "Lib/stonks.c"
#include "Lib/history.c"
#endif



int main(){
	char nextScreen[20]; // this variable tells what screen should be shown next
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
	
	/*______ Main Program ______*/
	

	strcpy(nextScreen,"logo"); //set current screen to main Menu
	while(1){
		
		clearScreen();
		//strcmp(str1,str2) -> return (0 if str1 == str2 else 1)
		//Each Screen should set tell us what screen should be displayed next
		if (strcmp(nextScreen,"logo") == 0){
			show_logo(nextScreen);
		}
		else if ( strcmp(nextScreen,"add") == 0){
			show_addProduct(nextScreen,head);
		}
		else if (strcmp(nextScreen, "update") == 0) {
			show_updateProduct(nextScreen,head);
		}
		else if (strcmp(nextScreen,"delete") == 0){
			show_deleteProduct(nextScreen,head);
		}
		else if (strcmp(nextScreen,"printstock") == 0){
			show_printStock(nextScreen,head);
		}
		else if (strcmp(nextScreen,"search") == 0){
			show_Search(nextScreen,head);
		}
		else if (strcmp(nextScreen,"printhistory") == 0){
			history*hist;
			hist = (history*)malloc(sizeof(history));
			hist->value.operation_type = -1;
			hist->next = NULL;
			importHistory(hist);
			show_printHistory(nextScreen,hist);
		}
		else if (strcmp(nextScreen,"main") == 0){
			show_MainMenu(nextScreen);
		}
		
		else if (strcmp(nextScreen,"STONKS") == 0){
			show_STONKS(nextScreen);
		}
		else if ( strcmp(nextScreen,"exit")== 0) break;
	}
	
	resetColor();
	/*______ End of Main Program _______*/
}
