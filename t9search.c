#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void findContacts(/*char value[]*/);
void getString(char *arr);
void clearString(char str[]);

int main(int argc, char* argv[])
{
    findContacts();
        
    //Doing smth in dependacy with count of arguments
    /*if(argc == 2){
        findContacts();
    }
    else if(argc == 3){
        findContacts();
    }
    else{
        return -1;
    }*/
}


void findContacts(/*char value[]*/)//basic function for both cases
{
    char name[100];
    char number[100];

    while(1){
        
        getString(name);
        getString(number);
        
        printf("\n%s %s\n", name, number);

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


/*bool checkIfAppropriate(char value[]){
    while()
}*/

