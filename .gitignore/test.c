#include <stdio.h>
#include <string.h>
#include<conio.h>
#include <ctype.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (milliseconds >= 1000)
      sleep(milliseconds / 1000);
    usleep((milliseconds % 1000) * 1000);
#endif
}
struct test
{
    char a[5];
    int b;
};
const struct test foo = {"gf",10};

int main()
{
    
    while (1){
        sleep_ms(16);
        char ch = '\0';
	    if( _kbhit() )ch = getch(); 

        if (ch  ){
            if (ch == 8)
                printf("ch = %c , ord = %d\n",ch,ch);

        }
    }
    /*
    printf("Hello World");
    printf("Hello world\nHow are you?");
    printf("Hello \"World\", How are you?\n"); 
    printf ("Hey I got 84.20%% in my final exams\n");
    printf("num in octal format: %o\n", num);
    printf ("num in hexadecimal format(lowercase) : %x\n", num);
    printf ("num in hexadecimal format(uppercase) : %X\n", num);
    printf ("Hello world, how are you?\
    I love C programing language.\n");
    printf ("The file is store at c:\\files\\word_files\n");
    len = printf ("Hello\n");
    printf ("Length: %d\n", len);
    printf ("num (padded): %05d\n", num);
    printf ("str1=\"%20s\", str2=\"%-20s\"\n", "Hello", "World");
    printf("val = %.2f\n", val);
    printf("num = %i \n", num);
    printf("Address of num is: %p\n", &num);
    */
    return 0;
}