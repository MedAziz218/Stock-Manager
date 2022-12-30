
typedef struct {
    char* optionsText[50];
    int numberOfOptions;
    char selected_prefix[5];
    int row,pre_tab,post_tab;
    int selected_color;
    int normal_color;
    int current_selection;
} _SelectionMenuFormat;

_SelectionMenuFormat SelectionMenuFormat = {{"1-option1","2-option2","3-option3"},3,">",4,4,4,2,RED_TXT,WHITE_TXT*10};

void wipeSelectionMenu(_SelectionMenuFormat menu){
    moveTo(menu.row,0);
    int len = 0;
    for (int i=0; i< menu.numberOfOptions;i++){
        moveRight(menu.pre_tab);
        len = strlen(menu.optionsText[i]);
        if (menu.optionsText[i][len-1] == '\n') printf("%*s\n",strlen(menu.optionsText[i])+menu.post_tab,""); 
        else printf("%*s",strlen(menu.optionsText[i])+strlen(menu.selected_prefix)+menu.pre_tab,""); 
    }
}
void printSelectionMenu(_SelectionMenuFormat menu){
    moveTo(menu.row,0);
    char temp[SCREEN_WIDTH];int last_index,temp_int;
    int horizontal = 0;
    memset(temp,'\0',SCREEN_WIDTH);
    
    for (int i=0; i< menu.numberOfOptions;i++){
        last_index = strlen(menu.optionsText[i])-1;
        //if (menu.optionsText[i][last_index] == '\n') horizontal = 0;
        //else horizontal = 1;
        if (! horizontal) moveRight(menu.pre_tab);
        
        if (i == menu.current_selection){
            if (menu.selected_color >100) setTextColorBright(menu.selected_color/10);
            else setTextColor(menu.selected_color);
            printf(menu.selected_prefix);
            
        }
        else {
            if (menu.normal_color >100) setTextColorBright(menu.normal_color/10);
            else setTextColor(menu.normal_color);
        }
        
        if (menu.optionsText[i][last_index] == '\n'){
            //menu.optionsText[i][last_index] = '\0';
            horizontal = 0;
            memset(temp,'\0',50);
            strncpy(temp,menu.optionsText[i],last_index);
            printf("%s%*s\n",temp,menu.post_tab,"");
           // menu.optionsText[i][last_index] = '\n';
        }
        else {
            horizontal = 1;
            printf("%s",menu.optionsText[i]);   
        }
        resetColor();
    }
    
}

typedef struct {
    char* text;
    int row;
    int pre_tab,pos_tab;
    int color;
} _MessageFormat;
_MessageFormat MessageFormat = {"Message",0,4,4,RED_TXT};

void printMessage(_MessageFormat msg){
    moveTo(msg.row,0); moveRight(msg.pre_tab);
    if (msg.color >100) setTextColorBright(msg.color/10);
    else setTextColor(msg.color);
    for (int i=0; i< strlen(msg.text);i++){
        printf("%c",msg.text[i]);
        if (msg.text[i] == '\n') moveRight(msg.pre_tab);
    }
    
    resetColor();
}
void wipeMessage(_MessageFormat msg){
    moveTo(msg.row,0); moveRight(msg.pre_tab);
    for (int i=0; i< strlen(msg.text);i++){
        printf(" ");
        if (msg.text[i] == '\n') moveRight(msg.pre_tab);
    }
}