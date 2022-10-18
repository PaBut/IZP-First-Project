
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define SIZE 101
#define ERROR -1

int findContacts(char searchedElement[], int countOfMistakes);
int getLine(char *arr);
bool checkifNumber(char str[]);
bool checkIfName(char str[]);
bool checkIfAppropriate(int startPoint, char str[], int startPoint2, char wanted[]);
bool levenshteinMethod(char str[], char wanted[], int countOfMistakes);
int convertStringToInt(char numToConvert[]);
char toLower(char c);

//number to the group of letter relation
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
    int result = 0;
    //Doing smth in dependancy on count of arguments
    if(argc == 1){
        result = findContacts(NULL, 0);
    }
    else if(argc == 2 || argc == 3){
        if(!checkifNumber(argv[1])){
            fprintf(stderr ,"The argument(searched element) you sent to the application is not a number!!!");
            return EXIT_FAILURE;
        }
        if(argc == 2){
            result = findContacts(argv[1], 0);
        }
        if(argc == 3){
            if(!checkifNumber(argv[2])){
                fprintf(stderr ,"The argument(maximal count of possible mistakes) you sent the application is not a number!!!");
                return EXIT_FAILURE;
            }
            if(strlen(argv[2]) > 2){
                fprintf(stderr ,"The second argument can't be higher than 99\n(the maximal length of the line is 100 chars)");
                return EXIT_FAILURE;
            }
            result = findContacts(argv[1], convertStringToInt(argv[2]));
        }
    }
    else{
        fprintf(stderr ,"You have given wrong amount of arguments");
        return EXIT_FAILURE;
    }

    if(result == 0){
        printf("Not found");
    }
    else if(result == ERROR){
        return EXIT_FAILURE;
    }

    return 0;
}


int findContacts(char searchedElement[], int countOfMistakes)//basic function for both cases
{
    char name[SIZE];
    char number[SIZE];
    int isLastLine;
    int isValidName;

    bool isFound = false;

    do{
        //Checking in the following lines if there's an error in the input
        if(!(isValidName = getLine(name))){    
            fprintf(stderr ,"You forgot to add a number after the name");
            return ERROR;
        }
        if(isValidName == ERROR || (isLastLine = getLine(number)) == ERROR){
            fprintf(stderr, "The line was higher than 100 symbols");
            return ERROR;
        }

        if(!checkIfName(name) || !checkifNumber(number)){
            fprintf(stderr ,"You missed the order of names and numbers");
            return ERROR;
        }

        //Calling appropriate method in dependacy of given arguments in the launching of the program
        if(searchedElement == NULL){
            printf("%s %s\n", name, number);
            isFound = true;
        }
        else if(countOfMistakes > 0){
            if(levenshteinMethod(number, searchedElement, countOfMistakes) ||
            levenshteinMethod(name, searchedElement, countOfMistakes)){
                printf("%s %s\n", name, number);
                isFound = true;
            }
        }
        else if(checkIfAppropriate(0,number, 0, searchedElement) ||
         checkIfAppropriate(0,name, 0, searchedElement)){
            printf("%s %s\n", name, number);
            isFound = true;
        }
    }while(isLastLine);

    return isFound;
}

//Get a name or a number of the contact
//Also checking here, if the list is full read and the line is bigger than allowed maximum
int getLine(char arr[])
{
    for(int i = 0; i < SIZE ; i++ ){
        
        arr[i] = toLower(getc(stdin));

        if(arr[i] == '\n'){
            arr[i] = '\0';
            return true;
        }
        if(arr[i] == EOF){
            arr[i] = '\0';
            return false;
        }
    }    
    //the line is higher than 100 chars
    return ERROR;
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

//To lower case mathod
char toLower(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + ('a'-'A');
    }
    return c;
}


bool isSymbotMatched(char symbol, char wanted){//checking here if a symbol in the provided argument is in the name/number
    if(symbol>='0' && symbol <= '9' ){
        return symbol == wanted;
    }else if(symbol >= 'a' && symbol <= 'z'){
        //Calculating needed row in the dictionary
        int index = wanted - '0';
        if(dictionary[index] != NULL){
            for(int j = 0; dictionary[index][j] != '\0'; j++){
                if(symbol == dictionary[index][j]){
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

            //Checking if we already checked all chars in searched element
            if(startPoint2+1 == strlen(wanted)){
                return true;
            }
            if(checkIfAppropriate(i+1, str, startPoint2+1, wanted)){ //going to the next etap of recursion
                return true;
            }
        }

        /*if(i + 1 == strlen(str)){
           if(countOfMistakes > 0){
                if(strlen(wanted) - startPoint2-1 <= countOfMistakes){
                    printf("\t\t%s - %c - %c - %d - %d\n", str, str[i], wanted[startPoint2], countOfMistakes-1, startPoint2);
                    // printf("\n%d - %d\n", startPoint2+2, strlen(wanted));
                    str[i] = toUpper(str[i]);
                    return true;
                }
                // printf("%s - %c - %c - %d - %d\n", str, str[i], wanted[startPoint2], countOfMistakes-1, startPoint2);
                startPoint2++;
                i = startPoint;
                countOfMistakes--;
            }
        }*/
    }
    return false;
}

int convertStringToInt(char numToConvert[]){
    int result = 0;
    for(int i = 0; i < strlen(numToConvert); i++){
        result += (numToConvert[i]-'0') * pow(10, strlen(numToConvert) - 1 - i);
    }
    return result;
}

bool levenshteinMethod(char str[], char wanted[], int countOfMistakes){
    bool isLineMatched = false;
    int initialCountOfMistakes = countOfMistakes;
    //checking here if the line which is being searched contains searched element
    //(including maximal amount of user made mistakes)
    for(int i = 0; i < strlen(str); i++){
        for(int j = 0; j < strlen(wanted) && i+j < strlen(str); j++){
            isLineMatched = false;
            if(isSymbotMatched(str[j+i], wanted[j])){
                isLineMatched = true;
            }
            else{
                if(countOfMistakes > 0){
                    isLineMatched = true;
                    countOfMistakes--;
                }else{
                    countOfMistakes = initialCountOfMistakes;
                    break;
                }
            }
            //if iteration can go outside of the boundaries of the line(which is being checked)
            //then check if the maximal possible position of iteration
            //(considering it like the sum of the length of searched element and the counter of higher iteration)
            //is lower than the count of possible mistakes, then current contact is suitable
            if(i+j == strlen(str) - 1){
                if(strlen(wanted) + i - strlen(str) <= countOfMistakes){
                    isLineMatched = true;
                }
            }
        }
    }
    return isLineMatched;
}
