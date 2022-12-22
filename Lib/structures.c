

 
typedef struct history {
	int operation_type; // 1 means + and 0 means negative
	long double date; //self explanatory
	int product_id; //self explanatory
	int quantity; //self explanatory
	float unit_price; //self explanatory
	float operation_price; //total positive/negative per unit
} history;





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
