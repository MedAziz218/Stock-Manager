
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

//Imports stock from STOCK_FILE.
void importStock(stock * st){
	FILE* ptr; char ch; product temp_prod;
	ptr = fopen(STOCK_FILE, "r"); //opening file
	printf("here");
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

//Exports the stock to STOCK_FILE. (write_mode: w)
void exportStock(stock * st){
	FILE* ptr;
	fopen(STOCK_FILE,"w");
	stock *temp = st;
	while(temp != NULL) {
	fprintf(ptr,"%i;%c;%d;%i;%c",temp->value.id,temp->value.name,
	temp->value.price,temp->value.quantity,temp->value.description);
	temp = temp->next;
	}
	fclose(ptr);
}

void search(stock* st, int choice, char key[]){
	// function that searches for specific items from the stock based on an input provided by the user
	stock *aux, *temp_st;
	temp_st = (stock*)malloc(sizeof(stock)); // temporary linked list o show the resulting items after the search
	temp_st->value.id = -1;
	int choice;	// choice of the product attribute that will be used for the search
	char key[64]; // variable to store the key that will be used for the search
	aux = st;
	// while(1){
	// 	printf("Would you like to search by: \n"
	// 	   "1- id \n"
	// 	   "2- name \n"
	// 	   "3- price \n"
	// 	   "4- quantity \n"
	// 	   "5- description \n"
	// 	   );
	// 		scanf("%d", &choice);
	// 	if (choice >=1 && choice <=5){
	// 		break;
	// 	}
	// 	else{
	// 		scanf("The input you've provided is not valid, please try again\n");
	// 	}
	// }
	// printf("search for: ");
	// scanf("%s", &key);
	//strstr(s1, s2) returns true if s1 contains s2
	//strlwr(s) return the string s in lowercase
	switch (choice){
		case 1: 
			while(aux->next != NULL){
				if(aux->value.id == atoi(key)){
					addProduct(temp_st, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 2:
			while(aux != NULL){
				if(strstr(strlwr(aux->value.name), strlwr(key))){
					addProduct(temp_st, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 3: 
			while(aux != NULL){
				if(atof(key) <= aux->value.price + 50 && atof(key) >= aux->value.price - 50){
					addProduct(temp_st, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 4: 
			while(aux != NULL){
				if(aux->value.quantity == atoi(key)){
					addProduct(temp_st, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 5:
			while(aux != NULL){
				if(strstr(strlwr(aux->value.description), strlwr(key))){
					addProduct(temp_st, aux->value);
				}
				aux = aux->next;
			}
			break;
	}
	if(temp_st->value.id == -1){
		printf("Your search for '%s' did not match any product.\nMake sure that your input is spelled correctly");
	}
	else{

	}
}
