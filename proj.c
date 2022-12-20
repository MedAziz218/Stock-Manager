#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "cmdFunctions.c"
// these two variables are shared across all functions in order to reduce memory usage
char tempFileString[4096]; //a string variable that can be used by any function when needed 
char userInput[128]; //a string variable that can be used by any function to read user input
char LOGO_FILE[] = "logo.txt";
char MENU_FILE[] = "menu.txt";
char STOCK_FILE[] = "stock.csv";

//TODO: last modified date
//TODO: ajouterproduct
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


void show_MainMenu(char nextScreen[]){
	tempFileString[0] = '\0'; // clear 
	readFile(MENU_FILE,tempFileString);
	printf("%s",tempFileString);

	gets(userInput);
	if (strcmp(userInput,"1")==0) strcpy(nextScreen,"1");
	else if (strcmp(userInput,"5")== 0) strcpy(nextScreen,"exit");
}

void show_Screen1(char nextScreen[]){
	printf("This is Screen 1");

	gets(userInput);
	
	strcpy(nextScreen,"main"); // nextScreen = "main"
}
void addProduct(stock * st, product p){
	stock * ptr,*aux;
	ptr = (stock*)malloc(sizeof(stock));
	ptr->value = p;
	
	if (st == NULL){
		st = ptr;
	} else {
		aux = st;
		while(aux->next != NULL){
			aux = aux->next;
		}

		aux->next = ptr;
	}
	
}
void importStock(stock * st){
	FILE* ptr; char ch;
	ptr = fopen(STOCK_FILE, "r");
	product temp; 
	char temp_str[256]; int temp_i = 0;
	
	int i = 0;
	ch = fgetc(ptr);
    while (ch != EOF) {
		//printf("%c",ch);
		if (ch == ';' || ch == '\n') {
			printf(">>> %s \n",temp_str);
			switch (i){
				case 0: temp.id = atoi(temp_str); break;
				case 1: strcpy(temp.name,temp_str); break;
				case 2: temp.price = atof(temp_str); break;
				case 3: temp.quantity = atoi(temp_str); break;
				case 4: strcpy(temp.description, temp_str); break;
			}
			
			temp_i = 0;
			
		}
		i++;
		if (i == 5)
			{
				//addProduct(st,temp);
				printf("holaaa %s\n",temp.id);
				i = 0;
				strcpy(temp_str,"");
				temp_str[0] = '\0';
		}
		temp_str[temp_i] = ch;
		ch = fgetc(ptr);
		
		
    }
	
}



int main(){
	/*
	product p1,p2;
	stock* st;
	strcpy(p1.name, "hola");
	strcpy(p2.name , "amigos");
	//product *ptr;
	//ptr = (product*)malloc(sizeof(product));
	st = (stock *)malloc(sizeof(stock));
	getchar();
	
	stock *head,tail;
	importStock(head);
	for(int i=0; i<3;i++){
		product p = head->value;
		printf(">>%d \n",p.id);
		printf(">>%s \n\n",p.name);
		//head = head->next;
	}
	getchar();
	*/
	char nextScreen[10]; // this variable tells what screen should be shown next
	//SetConsoleOutputCP(1252); //Set console encoding to Windows 1252
	SetConsoleOutputCP(65001); //Set console encoding to utf8
	system("color");clearScreen();

	
	readFile(LOGO_FILE,tempFileString);
	printf("%s",tempFileString);
	printf("\n\nPress Enter to Continue ...");
	
	getchar();
	clearScreen();

	strcpy(nextScreen,"main"); //set current screen to main Menu
	while(1){
		
		clearScreen();
		//instead of gets we can define our own function to control the input 
		//strcmp(str1,str2) -> return (0 if str1 == str2 else 1)
		// 
		if (strcmp(nextScreen,"main") == 0){
			show_MainMenu(nextScreen);
		}
		

		else if ( strcmp(nextScreen,"1") == 0){
			show_Screen1(nextScreen);
		}
		else if ( strcmp(nextScreen,"exit")== 0) break;
		

	}
	
	
	
	//printf("%s",str);
}