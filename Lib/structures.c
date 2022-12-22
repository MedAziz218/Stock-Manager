
//Record: A record concerning a certain operation.
typedef struct record {
	int operation_type; // 1 means [+]positive and 0 means [-]negative 
						//(convention: -1 means empty history record)
	char date[32]; //A string with this format: DDD MMM DD HH:MM:SS YYYY
	int product_id; //self explanatory
	char product_name[64]; //Product's name
	int quantity; //self explanatory
	float unit_price; //self explanatory
	float operation_price; //total positive/negative per unit
} record;

//HISTORY: A linked list of operation records.
typedef struct history{
	record value;
	struct history* next;
}history;



//Product: A product being operated with inside the stock. 
typedef struct product{
	int id;
	char name[64];
	double price;
	int quantity;
	char description[256];
	
}  product;

//Stock: A stock of products (Linked List)
typedef struct stock{
	product value;
	struct stock* next;
}stock;
