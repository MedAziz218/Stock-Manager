#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define OsWindows 1
#define clrscr() system("cls")
#endif

#ifdef linux
#define OsWindows 0
#include <unistd.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif
enum Colors {
    RESET_COLOR, 
    BLACK_TXT = 30, RED_TXT,GREEN_TXT,YELLOW_TXT,BLUE_TXT,MAGENTA_TXT,CYAN_TXT,WHITE_TXT,
    BLACK_BKG = 40, RED_BKG, GREEN_BKG, YELLOW_BKG, BLUE_BKG, MAGENTA_BKG, CYAN_BKG, WHITE_BKG    
};

enum ClearCodes {
    CLEAR_FROM_CURSOR_TO_END,
    CLEAR_FROM_CURSOR_TO_BEGIN,
    CLEAR_ALL
};

struct {
    int col,row;
    int lower_border,upper_border;
    int left_border,right_border;
}CURSOR = {0,0,0,10,0,10};

static inline void hideCursor() {
    printf("\033[?25l");
}
static inline void showCursor() {
    printf("\033[?25h");
}
static inline void setTextColorBright(int code) {
    printf("\x1b[%d;1m", code);
}
static inline void setTextColor(int code) {
    printf("\x1b[%dm", code);
}
static inline void setBackgroundColor(int code) {
    printf("\x1b[%dm", code);
}
static inline void setBackgroundColorBright(int code) {
    printf("\x1b[%d;1m", code);
}

static inline void resetColor(void) {
    printf("\x1b[%dm", RESET_COLOR);
}


static inline void moveDown(int positions) {
    printf("\x1b[%dB", positions);
}
static inline void moveUp(int positions) {
    printf("\x1b[%dA", positions);
}
static inline void moveLeft(int positions) {
    printf("\x1b[%dD", positions);
}
static inline void moveRight(int positions) {
    printf("\x1b[%dC", positions);
}
static inline void moveTo(int row, int col) {
    printf("\x1b[%d;%df", row, col);
    
}

static inline void clearScreen(void) {
    //printf("\x1b[%dJ", CLEAR_ALL);
    clrscr();//moveTo(0,0);
   
}
static inline void saveCursorPosition(void) {
    printf("\x1b%d", 7);
}
static inline void restoreCursorPosition(void) {
    printf("\x1b%d", 8);
}

void readFile(char filename[],char str[])
{	
    FILE* ptr; char ch; int i=0;
    // Opening file in reading mode
    ptr = fopen(filename, "r");
	
	// Show error message if something goes wrong
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

    // Reading what is written in file
    // character by character using loop.
	// If it is EOF stop reading.
	ch = fgetc(ptr);
    while (ch != EOF) {
        str[i] = ch; 
		i++;
		ch = fgetc(ptr); 
    }
	// Mark the end of the string (important to make the str variable reusable)
	str[i] = '\0'; 
	
	// Closing the file
    fclose(ptr);
}
