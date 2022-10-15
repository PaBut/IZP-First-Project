#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>


bool findContacts(char value[]);
bool getLine(char *arr);
bool checkifNumber(char str[]);
bool checkIfName(char str[]);
bool checkIfAppropriate(int startPoint, char str[], int startPoint2, char wanted[]);
void clearString(char str[]);
char toUpper(char c);
char toLower(char c);

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
    if(argc == 1){
        findContacts(NULL);
    }
    else if(argc == 2){
        if(checkifNumber(argv[1])){
            if(!findContacts(argv[1])){
               printf("Not found");
            }
        }
        else{
            fprintf(stderr ,"The argument you send to the application is not number!!!");
            return -1;
        }
        
    }
    else{
        fprintf(stderr ,"You have given wrong amout of arguments");
        return -1;
    }
}


bool findContacts(char value[])//basic function for both cases
{
    char name[100];
    char number[100];
    bool isLastLine;
    bool isFound = false;

    do{
        if(!getLine(name)){    
            fprintf(stderr ,"You forgot to add a number after the name");
            exit(-1);
        }
        isLastLine = getLine(number);

        if(!checkIfName(name) || !checkifNumber(number)){
            fprintf(stderr ,"You missed the order of names and numbers");
            exit(-1);
        }

        if(value == NULL){
            printf("%s %s\n", name, number);
            isFound = true;
        }
        else if(checkIfAppropriate(0,number, 0, value) || checkIfAppropriate(0,name, 0, value)){
            printf("%s %s\n", name, number);
            isFound = true;
        }

        //clearString(name);
        //clearString(number);
    }while(isLastLine);

    return isFound;
}
bool getLine(char arr[])//Get name or number of a contact. Also checking here, if the list is full read.
{
    for(int i = 0; i < 100 ; i++ ){
        
        arr[i] = getc(stdin);

        if(arr[i] == '\n'){
            arr[i] = '\0';
            return true;
        }
        if(arr[i] == EOF){
            arr[i] = '\0';
            return false;
        }
    }    
    fprintf(stderr ,"The line is higher than 100");
    exit(-1);
}

bool checkifNumber(char str[]){//checking if the current line is a number, not a name of the contact
    bool isNumber = true;
    for(int i = 0; i < strlen(str); i++){
        if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '+')){
            isNumber = false;
        }
    }
    return isNumber;
}

bool checkIfName(char str[]){
    bool isName = false;
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            isName = true;
        }
    }
    return isName;
}


/*void clearString(char str[])
{
    for(int i = 0; str[i] != '\0'; i++){
        str[i] = '\0';
    }
}*/

/*char toLower(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + ('a'-'A');
    }
    return c;
}
char toUpper(char c){
    if(c >= 'a' && c <= 'z'){
        return c - ('a'-'A');
    }
    return c;
}*/


bool isSymbotMatched(char symbol, char wanted){//checking here if a symbol in provided argument is in the name/numbers
    if(symbol>='0' && symbol <= '9' ){
        return symbol == wanted;
    }else if(symbol >= 'a' && symbol <= 'z'){
    int index = wanted - ('0' - '\0');
        if(dictionary[index] != NULL)
        {
            for(int j = 0; dictionary[index][j] != '\0'; j++)
            {
                if(symbol == dictionary[index][j])
                {
                    return true;
                }
            }
        } 
    }
    return false;
}

bool checkIfAppropriate(int startPoint, char str[], int startPoint2, char wanted[]){
    for(int i = startPoint; i < strlen(str); i++){

        

        if(isSymbotMatched(str[i], wanted[startPoint2])){

            if(startPoint2+1 == strlen(wanted)){//every symbol given by user is found in name/number
                //str[i] = toUpper(str[i]);
                return true;
            }
            if(checkIfAppropriate(i+1, str, startPoint2+1, wanted)){ //going to the next etap of recursion
                //str[i] = toUpper(str[i]);
                return true;
            }
        }
    }
    return false;
}

