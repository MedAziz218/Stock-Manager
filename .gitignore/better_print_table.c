void printStock(stock * st){
	//convinience method to print the stock 
	stock * aux; product prod;
	_printTableTopLine(92); printf("\n");
	if (st->value.id != -1){
		aux = st;
		do
		{
			prod = aux->value;
			//printProduct(aux->value);
			char* temp_char;
			int len = strlen(prod.description);
			int temp_len = 57, i = 0 ;
			temp_char = (char*)malloc(sizeof(char)*temp_len);
			do{
				if (i*temp_len<= len)
				     strncpy(temp_char,prod.description+(i*temp_len),temp_len);
				else strncpy(temp_char,prod.description+((i-1)*temp_len),len - temp_len+1);
				temp_char[temp_len] = '\0';
				if (i==0) printf("┃%-4d┃%-12s┃%-10.3f┃%-5d┃%-57s┃",prod.id,prod.name,prod.price,prod.quantity,temp_char);
				else      printf("\n┃%-4s┃%-12s┃%-10s┃%-5s┃%-57s┃"," "," "," "," ",temp_char);
				i++;
				
			}while(temp_len*i < len);
			//printf("\n┃%-4s┃%-12s┃%-10s┃%-5s┃%-57s┃"," "," "," "," ","");
			printf("\n");
			
			_printTableInsideLine(92);
			strncpy(temp_char,prod.description+temp_len,temp_len);
			//printf("┃%-4s┃%-12s┃%-10s┃%-5s┃"," "," "," ",temp_char);
			aux = aux->next;
			printf("\n");
		}while (aux!= NULL);	
	}
	else printf("it s empty\n");
}