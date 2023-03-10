
product* getProduct(stock*st,int id){
	stock * aux; //product* temp_prod; 
	aux = st;
	while (aux != NULL){
		if (aux->value.id == id){
			return &(aux->value);
		}
		aux = aux->next;
	}
	product*prod;
	prod = (product*)malloc(sizeof(product));
	prod->id = -1;
	return prod;
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

// Deletes an existing product from the stock
void deleteProduct(stock * st, int id){
	stock *ptr, *aux;
	int cond = 0;
	//ptr = (stock*)malloc(sizeof(stock));

	// If the stock is empty then there's nothing to delete
	if (st->value.id == -1){
		//printf("The stock is already empty\n");
		return;
	}

	// If the stock isn' t empty
	// If the item to delete is in the first node
	if (st->value.id == id){
		cond = 1;
		// If the stock has one item
		if (st->next == NULL) {
			st->value.id = -1;
			
		// If the stock has more than one item
		} else {
			st->value = st->next->value;
			ptr = st->next;
			st->next = st->next->next;
			free(ptr);
		}
		
	}
	else{
		// else iterate through the list to find the item and delete it
		aux = st;
		while (aux->next != NULL)
		{	
			if (aux->next->value.id == id) {
				ptr = aux->next;
				aux->next = aux->next->next;
				cond = 1;
				break;	
			}
			aux = aux->next;
		}
	}

	//TODO: remove this block when project is finished.
	if (cond) {
		//printf("The item with the Id %d ",id);
		//setTextColorBright(GREEN_TXT);printf("was deleted successfully");resetColor();printf(".\n");
	}
	else {
		//printf("The item with the Id %d ",id);
		//setTextColor(RED_TXT);printf("was not found");resetColor();printf(".\n");
	}
}


//Imports stock from STOCK_FILE.
void importStock(stock * st){
	FILE* ptr; char ch; product temp_prod;
	ptr = fopen(STOCK_FILE, "r"); //opening file
	
	char temp_str1[400],temp_str2[400],temp_str3[400],temp_str4[400],temp_str5[400];
	int i=0; int scan_result;
	do {
		memset(temp_str5,'\0',sizeof(char)*400);
		scan_result = fscanf(ptr,"%400[^;];%400[^;];%400[^;];%400[^;];%400[^\n]", 
								temp_str1, temp_str2, temp_str3,temp_str4, temp_str5);
		ch = fgetc(ptr);
		if (i>0 && scan_result >= 4){
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
	ptr = fopen(STOCK_FILE,"w");
	stock *temp = st;
	fprintf(ptr,"id;name;price;quantity;description\n");
	if (ptr == NULL){
		setTextColor(RED_TXT);printf("Error");resetColor();
		//printf(": File Access denied.\n");
		return;
	}
	while(temp != NULL) {
		if (strlen(temp->value.name)==0) strcpy(temp->value.name," ");
		fprintf(ptr,"%i;%s;%.3f;%i;%s\n",temp->value.id,temp->value.name,
				temp->value.price,temp->value.quantity,temp->value.description);
		temp = temp->next;
		//if (strlen(temp->value.name)) strcpy(temp->value.name," ");
	}
	//setTextColorBright(GREEN_TXT);printf("Exported successfully");resetColor();printf(".\n");
	fclose(ptr);
}

// function that searches for specific items from the stock based on an input provided as parameter
// "choice" designates the search type (1 for id , 2 for name, 3 for price, 4 for quantity, 5 for description)
// "key" designates the key words or values we re searching for 
stock* search(stock* st, int choice, char key[]){
	
	int range = 50; // range for searching by price.
	stock *aux, *temp_stock;
	char *temp_key;
	
	temp_key = (char*)malloc(sizeof(char)*strlen(key));
	strcpy(temp_key,key); strlwr(temp_key);
	
	temp_stock = (stock*)malloc(sizeof(stock)); // temporary linked list to show the resulting items after the search
	temp_stock->value.id = -1;
	temp_stock->next = NULL;
	// 	Would you like to search by:
	// 	   "1- id \n"
	// 	   "2- name \n"
	// 	   "3- price \n"
	// 	   "4- quantity \n"
	// 	   "5- description \n"
	//strstr(s1, s2) returns true if s1 contains s2
	//strlwr(s) turn the string s to all lowercase caracters
	
	aux = st;
	switch (choice){
		case 1: 
			//by id (will always return one value since each id is unique )
			while(aux != NULL){
				
				if(aux->value.id == atoi(temp_key)){
					addProduct(temp_stock, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 2:
			//by name (will return all product with name containing the given key)
			while(aux != NULL){
				char temp_str[64];
				strcpy(temp_str,aux->value.name);
				
				if(strstr(strlwr(temp_str), temp_key)){
					addProduct(temp_stock, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 3: 
			//by price (will return all product with  key-range<= price <=  key+range )
			
			while(aux != NULL){
				if(atof(key) <= aux->value.price + range && atof(key) >= aux->value.price - range){
					addProduct(temp_stock, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 4: 
			//by quantity (will return all products with quantity == key)
			while(aux != NULL){
				if(aux->value.quantity == atoi(key)){
					addProduct(temp_stock, aux->value);
				}
				aux = aux->next;
			}
			break;
		case 5:
			//by description (same as searching by name ....)
			while(aux != NULL){
				char temp_str[256];
				strcpy(temp_str,aux->value.description);

				if(strstr(strlwr(temp_str), temp_key)){
					addProduct(temp_stock, aux->value);
				}
				aux = aux->next;
			}
			break;
	}
	//printStock(temp_stock);
	free(temp_key);
	return temp_stock;
}
