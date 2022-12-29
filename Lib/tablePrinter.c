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
//} TABLE = {"╭","┳","╮","┣","╋","┫","╰","┻","╯","▒","┃"};	
//}TABLE = {"╔","╦","╗","╠","╬","╣","╚","╩","╝","═","║"};
} TABLE = {"┏","┳","┓","┣","╋","┫","┗","┻","┛","━","┃"};


//convinience method to print a product variable
void printProduct(product prod){
	printf("%-3d, %-11s, %-9.3f, %-5d, %s",prod.id,prod.name,prod.price,prod.quantity,prod.description);
}
typedef struct  {
    //char* attributes[10];
    //char* typesOfAttributes[10]; //str,int or float
    int sizeOfAttributes[10];
    int numberOfAttributes;
    int pre_tab,post_tab;
	int row;
    int width;
    int crosses[10];

} _TableFormat;

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


//{4,20,0,16,0,0,0}
_TableFormat StockTableFormat = {{4,20,0,16,20},5,8,6,4};

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
	printf(" %-*d ",id_space-2,prod.id);resetColor();printf(TABLE.vertical);
	
	
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
	//printf("%*s",post_tab,"");
	//printf("\n");
}

int digit_count(int n){
	int count = 0;
	do {
    n /= 10;
    ++count;
 	} while (n != 0);
 	return count;
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
	
	cur_name_space = fmin(cur_name_space,20);
	cur_description_space = fmin(cur_description_space,80);
	
	//StockTableFormat.sizeOfAttributes[]
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
		setTextColor(YELLOW_TXT); printf("it s empty"); resetColor();printf("\n");
		
	}
	memset(StockTableFormat.sizeOfAttributes,0,sizeof(int)*5);
	calibrate_StockTableFormat(st,StockTableFormat.sizeOfAttributes);
	__init_table__(&StockTableFormat);
	if (StockTableFormat.width >=84)
		{
			StockTableFormat.sizeOfAttributes[4] -= StockTableFormat.width -84;
			__init_table__(&StockTableFormat);
			//cur_description_space = cur_description_space - (StockTableFormat.width -70);
		}
	//header 
	printTableHeader(StockTableFormat);
	//printTableTitles(pre_tab,width,crosses);
	//printTableLine(pre_tab,width,crosses);

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


//#______ History Printer _______#//
//TODO:
void printTableHistory(){}
void printHistory(){}
//#____ End of History Printer _____#//
