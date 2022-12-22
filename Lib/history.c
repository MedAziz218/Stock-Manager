
#include<time.h> //Needed to get the current time of operation aka ctime().

void addRecord(history * ht, record p){
	// Adds a new record to history.
	stock * ptr,*aux;	
	if (ht->value.operation_type == -1){
		// If the history of records is empty then just set the value of the first element 
		copyProduct(&(ht->value),&p); 
	} else {
		ptr = (stock*)malloc(sizeof(stock));	
		ptr->next = NULL;
		aux = ht;
		copyProduct(&(ptr->value),&p);
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = ptr; 
	}	
}

void exportHistory(history * ht){
    FILE* ptr; char ch; record tempRecord;
	ptr = fopen(STOCK_FILE, "r"); //opening file
	char t1[2],t2[32],t3[8],t4[32],t5[32],t6[32];
	int i=0; int scan_result;
	do {
		scan_result = fscanf(ptr, "%400[^;];%400[^;];%400[^;];%400[^;];%400[^;];%400[^\n]", 
								t1,t2,t3,t4,t5,t6);
		ch = fgetc(ptr);
		if (i>0 && scan_result == 6){
			tempRecord.operation_type = atoi(t1);
			strcpy(tempRecord.date,t2);
			tempRecord.product_id = atoi(t3);
			tempRecord.quantity = atoi(t4);
			tempRecord.unit_price = atof(t5);
			tempRecord.operation_price = atof(t6);
			addRecord(ht,tempRecord);
		}
		i++;
	}while(ch != EOF);
	fclose(ptr);
}

