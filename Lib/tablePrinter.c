#include <math.h>

//#______ Table Printer _______#//
const struct {
    char upper_left[4] ;
	char upper_cross[4] ;
	char upper_right[4] ;
	char left_cross[4] ;
	char middle_cross[4];
	char right_cross[4] ;
	char lower_left[4] ;
	char lower_cross[4] ;
	char lower_right[4] ;
	char horizontal[4] ;
	char vertical[4] ;
//} TABLE = {"╭","┳","╮","┣","▒","┫","╰","┻","╯","▒","┃"};	
//}TABLE = {"╔","╦","╗","╠","╬","╣","╚","╩","╝","═","║"};
} TABLE = {"┏","┳","┓","┣","╋","┫","┗","┻","┛","━","┃"};
void calibrate_HistoryTableFormat(history *st, int sizes[7]);
typedef struct _TableFormat {
    //char* attributes[10];
    //char* typesOfAttributes[10]; //str,int or float
    int sizeOfAttributes[10];
    int numberOfAttributes;
    int pre_tab,post_tab;
	int row;
    int width;
    int crosses[10];

} _TableFormat;
_TableFormat StockTableFormat = {{4,20,0,16,20},5,8,6,4};
_TableFormat HistoryTableFormat = {{1,24,MAX_QUANTITY_DIGITS,MAX_NAME_LENGTH
						,4,4,4},7,8,6,4};

int digit_count(int n){
	int count = 0;
	do {
    n /= 10;
    ++count;
 	} while (n != 0);
 	return count;
}

//convinience method to print a product variable
void printProduct(product prod){
	printf("%-3d, %-11s, %-9.3f, %-5d, %s",prod.id,prod.name,prod.price,prod.quantity,prod.description);
}
int appartient(int x, int list[],int n){
	for (int i = 0; i<n;i++)
		if (list[i] == x) return 1;
	return 0;
}
void printTableTitles(int pre_tab,int post_tab,int width,int crosses[]){}
void printTableHeader(_TableFormat tab){
    int crosses_len = tab.numberOfAttributes -1;
	moveTo(tab.row,0);
	moveRight(tab.pre_tab);
	printf(TABLE.upper_left);
	for (int i=0;i<tab.width;i++) {
		if (appartient(i,tab.crosses,crosses_len)) printf(TABLE.upper_cross);
		else printf(TABLE.horizontal);	
	}
	printf(TABLE.upper_right);printf("%*s",tab.post_tab,"");printf("\n");
}
void printTableLine(_TableFormat tab){
	int crosses_len = tab.numberOfAttributes -1;
	moveRight(tab.pre_tab);
	printf(TABLE.left_cross);
	for (int i=0;i<tab.width;i++) {
		if (appartient(i,tab.crosses,crosses_len)) printf(TABLE.middle_cross);
		else printf(TABLE.horizontal);	
	}
	printf(TABLE.right_cross);printf("%*s",tab.post_tab,"");printf("\n");
}
void printTableColser(_TableFormat tab){
	int crosses_len = tab.numberOfAttributes -1;
	moveRight(tab.pre_tab);
	printf(TABLE.lower_left);
	for (int i=0;i<tab.width;i++) {
		if (appartient(i,tab.crosses,crosses_len)) printf(TABLE.lower_cross);
		else printf(TABLE.horizontal);	
	}
	printf(TABLE.lower_right);printf("%*s",tab.post_tab,"");printf("\n");
}


void __init_table__(_TableFormat*tab){
    int width = 0;//id_space+name_space+price_space+quantity_space+description_space+4;
    int p_sum = -1;
	//int width = id_space+name_space+price_space+quantity_space+description_space+4;
	//int crosses[] = {id_space, 1+id_space+name_space, 2+id_space+name_space+price_space, 3+id_space+name_space+price_space+quantity_space};
    for (int i=0;i<tab->numberOfAttributes;i++){
        width += tab->sizeOfAttributes[i];
        p_sum += tab->sizeOfAttributes[i] +1;
        if (i <= tab->numberOfAttributes -1) tab->crosses[i] = p_sum;
    };
    width = width +tab->numberOfAttributes -1;
    tab->width = width;
}
void printBeforeTableItems(_TableFormat tab){
    moveRight(tab.pre_tab);printf(TABLE.vertical);
}
void printBetweenTableItems(_TableFormat tab){
    printf(TABLE.vertical);
}
void printAfterTableItems(_TableFormat tab){
    printf("%*s\n",tab.post_tab,"");
}

//#____ End of Table Printer _____#//

//#______ History Printer _______#//
//TODO:

void printTableRecord(record rec,_TableFormat tab ){
	printBeforeTableItems(tab);
	int color1,color2;
	if (rec.operation_type == 1){
		color1 = GREEN_TXT;
		color2 = GREEN_BKG;
	}else if(rec.operation_type == 0){
		color1 = RED_TXT;
		color2 = RED_BKG;
	}else {
		color1 = YELLOW_TXT;
		color2 = YELLOW_BKG;
	}
	setTextColorBright(color1);
	char c ;
	switch (rec.operation_type){
		case 0: c='-';break;
		case 1: c='+';break;
		case 3: c='M';break;
	}
	printf(" %c ",c);
	resetColor();printf(TABLE.vertical);

	setTextColorBright(color2);
	printf(" %*s ",tab.sizeOfAttributes[1]-2,rec.date);
	resetColor();printf(TABLE.vertical);

	setBackgroundColor(color1);
	printf(" %*d ",tab.sizeOfAttributes[2]-2,rec.product_id);
	resetColor();printf(TABLE.vertical);

    printf(" %*s ",tab.sizeOfAttributes[3]-2,rec.product_name);
	resetColor();printf(TABLE.vertical);

	
	setTextColorBright(color2);
    
	printf(" %*d ",tab.sizeOfAttributes[4]-2,rec.quantity);
	resetColor();printf(TABLE.vertical);

	setTextColorBright(color1);
	printf(" %*.3f $ ",tab.sizeOfAttributes[5]-4,rec.unit_price);
	resetColor();printf(TABLE.vertical);
	setTextColor(color1);
	if (rec.operation_type == 0)
		printf(" %*.3f $ ",tab.sizeOfAttributes[6]-4,rec.operation_price*-1);
	else if(rec.operation_type == 1) 
		printf(" %-*s+%.3f $ ",tab.sizeOfAttributes[6]-4-5-digit_count(rec.operation_price/1),"",rec.operation_price*1);
	else 
		printf(" %*.3f $ ",tab.sizeOfAttributes[6]-4,rec.operation_price);
	resetColor();
	printBetweenTableItems(tab);
	printAfterTableItems(tab);
}
void printHistory(history * hist){
	//convinience method to print the stock 
	history * aux; record rec;
	// If table is empty stop
	if (hist->value.operation_type == -1){
		//setTextColor(YELLOW_TXT); printf("it s empty"); resetColor();printf("\n");
		return;
	}
	memset(HistoryTableFormat.sizeOfAttributes,0,sizeof(int)*7);
	calibrate_HistoryTableFormat(hist,HistoryTableFormat.sizeOfAttributes);
	__init_table__(&HistoryTableFormat);
	
	
	int x = HistoryTableFormat.row -1;
	int p = HistoryTableFormat.pre_tab+3;
	moveTo(x,HistoryTableFormat.crosses[0]+p);
	printf("  DATE");
	moveTo(x,HistoryTableFormat.crosses[1]+p);
	printf(" ID");
	moveTo(x,HistoryTableFormat.crosses[2]+p);
	printf(" NOM");
	moveTo(x,HistoryTableFormat.crosses[3]+p);
	printf(" QUANTITE");
	moveTo(x,HistoryTableFormat.crosses[4]+p);
	printf(" PRIX UNITAIRE");
	moveTo(x,HistoryTableFormat.crosses[5]+p);
	printf("  TOTALE ");
	
	//header 
	printTableHeader(HistoryTableFormat);

	//printTableTitles(pre_tab,width,crosses);
	//printTableLine(pre_tab,width,crosses);
	int is_modify = 0;
	aux = hist;
	do
	{	
		// -----------------------------------------
		rec = aux->value;
		printTableRecord(rec,HistoryTableFormat);
		if (rec.operation_type == 3 && is_modify == 0){
			is_modify = 1;
		} else if (rec.operation_type == 3) is_modify = 0;
		if (aux->next != NULL && is_modify == 0){
			printTableLine(HistoryTableFormat);
		}
		// -----------------------------------------
		aux = aux->next;
	}while (aux!= NULL);	
	printTableColser(HistoryTableFormat);
	
	
}
//#____ End of History Printer _____#//

void printTableProduct(product prod,_TableFormat tab ){
	int pre_tab = tab.pre_tab, post_tab = tab.post_tab;
	int id_space = tab.sizeOfAttributes[0], name_space = tab.sizeOfAttributes[1];
	int price_space = tab.sizeOfAttributes[2];
	int quantity_space=tab.sizeOfAttributes[3] ,  description_space = tab.sizeOfAttributes[4];
	char temp_char[256];
	//pre tab 
	//printf("%*s",pre_tab,"");
	printBeforeTableItems(tab);
	// id
	setBackgroundColorBright(RED_BKG);setTextColorBright(WHITE_TXT);
	if (prod.id == -2)
		 printf(" %-*s ",id_space-2,"");
	else printf(" %-*d ",id_space-2,prod.id);
	resetColor();printf(TABLE.vertical);
	
	
	// name
	strcpy(temp_char,prod.name);
	if (strlen(prod.name)>name_space){
		temp_char[name_space-2] = '\0';
		temp_char[name_space-3] = '.';
		temp_char[name_space-4] = '.';
		temp_char[name_space-5] = '.';}
	printf(" %-*s ",name_space-2,temp_char);printf(TABLE.vertical);
	// price
	setTextColorBright(GREEN_TXT); printf(" %*.3f $ ",price_space-1-3,prod.price);
	resetColor(); printf(TABLE.vertical);
	// quantity
	setTextColorBright(BLUE_TXT); printf(" %*d ",quantity_space-2,prod.quantity);
	resetColor(); printf(TABLE.vertical);
	// description
	strcpy(temp_char,prod.description);
	if (strlen(prod.description)>description_space-2){
		temp_char[description_space-2] = '\0';
		temp_char[description_space-3] = '.';
		temp_char[description_space-4] = '.';
		temp_char[description_space-5] = '.';}
	printf(" %-*s ",description_space-2,temp_char);
	printBetweenTableItems(tab);
	printAfterTableItems(tab);

}




void calibrate_HistoryTableFormat(history *st, int sizes[7]){
	history* aux;
	int cur_optype_space = 0;
	int cur_date_space = 0;
	int cur_id_space = 0;
	int cur_name_space = 0;
	int cur_quantity_space = 0;
	int cur_unitPrice_space = 0;
	int cur_totalPrice_space = 0;
	
	aux = st;
	do {  
		record rec = aux->value;
		cur_optype_space = 1;
		cur_date_space = fmax(cur_date_space,strlen(rec.date));
		cur_id_space = fmax(cur_id_space,digit_count(rec.product_id));
		cur_name_space = fmax(cur_name_space,strlen(rec.product_name));
		cur_quantity_space = fmax(cur_quantity_space,digit_count(rec.quantity));
		cur_unitPrice_space = fmax(cur_unitPrice_space,digit_count(rec.unit_price));
		cur_totalPrice_space = fmax(cur_totalPrice_space,digit_count(rec.operation_price));
		
		aux = aux->next;
	} while( aux != NULL);
	
	cur_name_space = fmin(cur_name_space,MAX_NAME_LENGTH);
	cur_quantity_space = fmax(cur_quantity_space,9);
	cur_unitPrice_space = fmax(cur_unitPrice_space,7);

	HistoryTableFormat.sizeOfAttributes[0] = fmax(HistoryTableFormat.sizeOfAttributes[0],cur_optype_space+2);
	HistoryTableFormat.sizeOfAttributes[1] = fmax(HistoryTableFormat.sizeOfAttributes[1],cur_date_space+2);
	HistoryTableFormat.sizeOfAttributes[2] = fmax(HistoryTableFormat.sizeOfAttributes[2],cur_id_space+2);
	HistoryTableFormat.sizeOfAttributes[3] = fmax(HistoryTableFormat.sizeOfAttributes[3],cur_name_space+2);
	HistoryTableFormat.sizeOfAttributes[4] = fmax(HistoryTableFormat.sizeOfAttributes[4],cur_quantity_space+2);
	HistoryTableFormat.sizeOfAttributes[5] = fmax(HistoryTableFormat.sizeOfAttributes[5],cur_unitPrice_space+1+PRICE_DECIMAL_DIGITS+4);
	HistoryTableFormat.sizeOfAttributes[6] = fmax(HistoryTableFormat.sizeOfAttributes[6],cur_totalPrice_space+1+PRICE_DECIMAL_DIGITS+5);
	
}

void calibrate_StockTableFormat(stock *st, int sizes[5]){
	stock* aux;
	int cur_id_space = 0;
	int cur_name_space = 0;
	int cur_price_space = 0;
	int cur_quantity_space = 0;
	int cur_description_space = 0;
	aux = st;
	do {
		product prod = aux->value;
		cur_id_space = fmax(cur_id_space,digit_count(prod.id));
		cur_name_space = fmax(cur_name_space,strlen(prod.name));
		cur_price_space = fmax(cur_price_space,digit_count(prod.price));
		cur_quantity_space = fmax(cur_quantity_space,digit_count(prod.quantity));
		cur_description_space = fmax(cur_description_space,strlen(prod.description));
		
		aux = aux->next;
	} while( aux != NULL);
	
	cur_name_space = fmin(cur_name_space,MAX_NAME_LENGTH);
	cur_description_space = fmin(cur_description_space,MAX_DESCRIPTION_LENGTH);
	
	cur_id_space = fmax(cur_id_space,1);
	cur_name_space = fmax(cur_name_space,4);
	cur_price_space = fmax(cur_price_space,1);
	cur_quantity_space = fmax(cur_quantity_space,8);
	cur_description_space = fmax(cur_description_space,11);
	
	StockTableFormat.sizeOfAttributes[0] = fmax(StockTableFormat.sizeOfAttributes[0],cur_id_space+2);
	StockTableFormat.sizeOfAttributes[1] = fmax(StockTableFormat.sizeOfAttributes[1],cur_name_space+2);
	StockTableFormat.sizeOfAttributes[2] = fmax(StockTableFormat.sizeOfAttributes[2],cur_price_space+4+4);
	StockTableFormat.sizeOfAttributes[3] = fmax(StockTableFormat.sizeOfAttributes[3],cur_quantity_space+2);
	StockTableFormat.sizeOfAttributes[4] = fmax(StockTableFormat.sizeOfAttributes[4],cur_description_space+2);
	
}


void printStock(stock * st){
	//convinience method to print the stock 
	stock * aux; product prod;
	// If table is empty stop
	if (st->value.id == -1){
		//setTextColor(YELLOW_TXT); printf("it s empty"); resetColor();printf("\n");
		return;
	}
	memset(StockTableFormat.sizeOfAttributes,0,sizeof(int)*5);
	calibrate_StockTableFormat(st,StockTableFormat.sizeOfAttributes);
	__init_table__(&StockTableFormat);
	if (StockTableFormat.width >=84)
		{
			StockTableFormat.sizeOfAttributes[4] -= StockTableFormat.width -SCREEN_WIDTH;
			__init_table__(&StockTableFormat);
		}
	int x = StockTableFormat.row -1;
	int p = StockTableFormat.pre_tab+3;
	moveTo(x,p);
	printf("%*s",StockTableFormat.width,"");
	moveTo(x,p);
	printf("ID");
	moveTo(x,StockTableFormat.crosses[0]+p);
	printf(" NOM");
	moveTo(x,StockTableFormat.crosses[1]+p);
	printf(" PRIX");
	moveTo(x,StockTableFormat.crosses[2]+p);
	printf("QUANTITE");
	moveTo(x,StockTableFormat.crosses[3]+p);
	printf(" DESCRIPTION");

	//header 
	printTableHeader(StockTableFormat);
	aux = st;
	do
	{	
		// -----------------------------------------
		prod = aux->value;
		printTableProduct(prod,StockTableFormat);
		if (aux->next != NULL){
			printTableLine(StockTableFormat);
		}
		// -----------------------------------------
		aux = aux->next;
	}while (aux!= NULL);	
	printTableColser(StockTableFormat);
	
	
}
//#____ End of Stock Printer _____#//


