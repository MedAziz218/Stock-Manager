
#include<time.h> //Needed to get the current time of operation aka ctime().


void copyRecord(record *rec1,record *rec2){
	// Convinience function to copy product variables
	rec1->operation_type = rec2->operation_type;
	strcpy(rec1->date,rec2->date);
	rec1->product_id = rec2->product_id;
	strcpy(rec1->product_name,rec2->product_name);
	rec1->quantity = rec2->quantity;
	rec1->operation_price = rec2->operation_price;
	rec1->unit_price = rec2->unit_price;
}

//Adds a new record to history.
void addRecord(history * ht, record p){
	// Adds a new record to history.
	history * ptr,*aux;	
	if (ht->value.operation_type == -1){
		// If the history of records is empty then just set the value of the first element 
		copyRecord(&(ht->value),&p); 
	} else {
		ptr = (history*)malloc(sizeof(history));	
		ptr->next = NULL;
		aux = ht;
		copyRecord(&(ptr->value),&p);
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = ptr; 
	}	
}

//Import the History of records from HISTORY_FILE
void importHistory(history * ht){
    FILE* ptr; char ch; record tempRecord;
	ptr = fopen(HISTORY_FILE, "r"); //opening file
	fseek(ptr,0,0);
	char t1[100],t2[100],t3[100],t4[100],t5[100],t6[100],t7[100];
	int i=0; int scan_result;
	do {
		memset(t1,'\0',sizeof(char)*100);
		memset(t2,'\0',sizeof(char)*100);
		memset(t3,'\0',sizeof(char)*100);
		memset(t4,'\0',sizeof(char)*100);
		memset(t5,'\0',sizeof(char)*100);
		memset(t6,'\0',sizeof(char)*100);
		memset(t7,'\0',sizeof(char)*100);

		scan_result = fscanf(ptr,"%100[^;];%100[^;];%100[^;];%100[^;];%100[^;];%100[^;];%100[^\n]", 
								t1,t2,t3,t4,t5,t6,t7);
		ch = fgetc(ptr);
		
		if (i>0 &&scan_result==7 ){
			tempRecord.operation_type = atoi(t1);
			strcpy(tempRecord.date,t2);
			tempRecord.product_id = atoi(t3);
			strcpy(tempRecord.product_name,t4);
			tempRecord.quantity = atoi(t5);
			tempRecord.unit_price = atof(t6);
			tempRecord.operation_price = atof(t7);
			addRecord(ht,tempRecord);
		}
		i++;
	}while(ch != EOF);
	fclose(ptr);
	
}



//USE: 
// Returns a record type object from relevant input.
// NewRecord() is to be used whenever a product is added/removed from the stock.
// It registers the date of the operation (all you need to do is just call it).
// optype: 0 is [-] and 1 is [+]
// Just provide the product ID, name, quantity and price.
// Example:
// record foo = newRecord(1,139,"bar",20,4.5)
record newRecord(int opType,product*prod,int quantity){
	record result;
	time_t current_date; //Date of creation of the object.
	time(&current_date);
	strcpy(result.date,ctime(&current_date));
	result.operation_type=opType;
	result.product_id=prod->id;
	strcpy(result.product_name,prod->name);
	result.unit_price=prod->price;
	result.quantity=quantity;
	result.operation_price=prod->price*quantity;
	return result;
}

//Writes to HISTORY_FILE. (mode: w)
void exportHistory(history * ht){
	FILE* ptr;
	fopen(HISTORY_FILE,"w");
	history *temp = ht;
	if (strlen(temp->value.product_name) == 0) {
		strcpy(temp->value.product_name," ");
	}
	while(temp != NULL) {
	fprintf(ptr,"%i;%c;%i;%c;%i;%f;%f",temp->value.operation_type,temp->value.date,
	temp->value.product_id,temp->value.product_name,temp->value.quantity,
	temp->value.unit_price,temp->value.operation_price);
	temp = temp->next;
	}
	fclose(ptr);
}

//Same as exportHistory() but appends to file instead
//of rewriting it. (mode: a)
void appendHistory(history * ht){
	FILE* ptr;
	ptr = fopen(HISTORY_FILE,"a");
	history *temp = ht;
	//fprintf(ptr,"id;name;price;quantity;description\n");
	if (ptr == NULL){
		//setTextColor(RED_TXT);printf("Error");resetColor();
		//printf(": File Access denied.\n");
		return;
	}
	if (strlen(temp->value.product_name) == 0) {
		strcpy(temp->value.product_name," ");
	}
		
	while(temp != NULL) {
		//if (strlen(temp->value.name)==0) strcpy(temp->value.name," ");
		temp->value.date[strlen(temp->value.date)-1] = '\0';
		fprintf(ptr,"%i;%s;%i;%s;%i;%.3f;%.3f\n",
                        temp->value.operation_type,
                        temp->value.date,
                        temp->value.product_id,
                        temp->value.product_name,
                        temp->value.quantity,
                        temp->value.unit_price,
                        temp->value.operation_price);
		temp = temp->next;
	}
	//setTextColorBright(GREEN_TXT);printf("Exported successfully");resetColor();printf(".\n");
	fclose(ptr);
}
