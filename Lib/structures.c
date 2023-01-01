
int NumberOfMainMenuOptions = 9;
char *MainMenuOptions[]={
						 "1 - Ajouter un produit.\n",
						 "2 - Modifier un produit.\n",
						 "3 - Supprimer un produit.\n",
						 "4 - Afficher tout le Stock.\n",
						 "5 - Recherche d’un produit dans le stock.\n",
						 "6 - Afficher l'historique\n",
						 "7 - Show Logo.\n",  
						 "8 - STONKS ? \n",
						 "9 - Exit.\n"
						 };

int NumberOfaddProductOptions = 7;
char *addProductOptions[]={
						 "1 - Identifiant\n",
					     "2 - Nom\n",
						 "3 - Prix\n",
						 "4 - Quantite\n",
						 "5 - Description du produit  \n\n",
						 "**-CONFIRMER.   ", "**-ANNULER.  "
						 };

enum key_codes{
	ENTER_KEY= 13, ARROW_START = -32, ARROW_UP = 72, ARROW_RIGHT = 75,ARROW_LEFT=77 ,
	ARROW_DOWN = 80,
	DELETE_KEY = 8, ESCAPE_KEY = 27,
};
enum program_constants{
	SCREEN_WIDTH = 84,
	MAX_ID_DIGITS = 6, MAX_NAME_LENGTH = 20,MAX_PRICE_DIGITS = 6,PRICE_DECIMAL_DIGITS = 3,
	MAX_QUANTITY_DIGITS = 6, MAX_DESCRIPTION_LENGTH = 40,
};
//Record: A record concerning a certain operation.
typedef struct record {
	int operation_type; // 1 means [+]positive and 0 means [-]negative 
						// 3 means modified (always comes in two lines)
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
