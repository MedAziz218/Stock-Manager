
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