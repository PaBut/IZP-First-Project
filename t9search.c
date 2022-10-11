#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void findContacts(char value[]);
void getString(char *arr);
bool checkIfAppropriate(int startPoint, char str[], int startPoint2, char wanted[]);
void clearString(char str[]);

char dictionary[10][5] = {
    "+",
    "",
    "abc",
    "def",
    "ghi",
    "jkl",
    "mno",
    "pqrs",
    "tuv",
    "wxyz"
};

int main(int argc, char* argv[])
{       
    //Doing smth in dependacy with count of arguments
    printf("%d", argc);
    if(argc == 1){
        findContacts(NULL);
    }
    else if(argc == 2){
        //printf("\n%s\n", argv[1]);  
        findContacts(argv[1]);
    }
    else{
        return -1;
    }
}


void findContacts(char value[])//basic function for both cases
{
    char name[100];
    char number[100];

    while(1){
        
        getString(name);                            //Getting name and number of each contact
        getString(number);
        if(value == NULL){
            printf("\n%s %s\n", name, number);
        }
        else if(checkIfAppropriate(0,number, 0, value) || checkIfAppropriate(0,name, 0, value)){
            printf("\n%s %s\n", name, number);

        }

        if(number[strlen(number)-1] == EOF){        
            break;
        }
        clearString(name);
        clearString(number);
    }
}
void getString(char arr[])//Get name or number of a contact. Also checking here, if the list is full read.
{
    for(int i = 0;  ; i++ ){

        if(i == 100){
            exit(-1);
        }
        
        arr[i] = getc(stdin);
        
        if(arr[i] == EOF){
            return;
        }

        if(arr[i] == '\n'){
            arr[i] = '\0';
            return;
        }
    }    
}


void clearString(char str[])
{
    for(int i = 0; str[i] != '\0'; i++){
        str[i] = '\0';
    }
}

char toLow(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + ('a'-'A');
    }
    return c;
}


bool checkIfAppropriate(int startPoint, char str[], int startPoint2, char wanted[]){
    for(int i = startPoint; i < strlen(str)-1; i++){
        if(startPoint2 == strlen(wanted)){ //base case of recurze
            return true;
        }

        bool conditions = false;
        char ch;

        if(str[i]>='0' && str[i] <= '9' ){
            conditions = str[i] == wanted[startPoint2];
        }else if((ch = toLow(str[i])) >= 'a' && ch <= 'z'){
            int index = wanted[startPoint2] - ('0' - '\0');
            if(dictionary[index] != NULL)
            {
                for(int j = 0; dictionary[index][j] != '\0'; j++)
                {
                    if(ch == dictionary[index][j])
                    {
                        conditions = true;
                        break;
                    }
                }
            }
            
        }

        if(conditions){
            if(checkIfAppropriate(i+1, str, startPoint2+1, wanted)){
                return true;
            }
        }
    }
    return false;
}

