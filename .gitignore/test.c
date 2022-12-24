#include <stdlib.h>
#include<stdio.h>
#include <string.h>
char* test(char * str){
  char * temp_key;
  temp_key = (char*)malloc(sizeof(str));
  strcpy(temp_key,str);
  strlwr(temp_key);
  printf("\n%s\n",temp_key);
  return temp_key;
}  
int main()
{
  char str[64] = "Alu";
  
  printf("Given string is: %s\n",str);


  printf("\nString after converting to the "
             "lowercase is: %s<<<\n\n",test("Alu"));
  
  return 0;
}