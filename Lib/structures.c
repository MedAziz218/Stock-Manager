
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
