#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
//#include <math.h>

#define SIZE 101
#define ERROR -1

typedef enum{
    displayall,
    simplecheck,
    checkwithspaces,
    levenshteinmethod
} selectedMode;

int findContacts(char searchedElement[], selectedMode mode, int countOfMistakes);
int getLine(char *arr);
bool checkifNumber(char str[]);
bool checkIfName(char str[]);
bool CheckWithSpace(int startPoint, char str[], int startPoint2, char wanted[]);
bool levenshteinMethod(char str[], char wanted[], int countOfMistakes);
int convertStringToInt(char numToConvert[]);
char toLower(char c);

//number to the group of letter relation


int main(int argc, char* argv[])
{       
    int result = 0;

    if(argc == 1){
        result = findContacts(NULL, displayall, 0);
    }
    else if(argc == 2){
        if(checkifNumber(argv[1])){
            result = findContacts(argv[1], simplecheck, 0);
        }else{
            fprintf(stderr ,"Wrong input!!!\nThe argument(searchedElement) you sent to the application is not a number!!!");
            return EXIT_FAILURE;
        }
    }
    else if(argc == 3){
        if(strcmp(argv[1], "-s") == 0 && checkifNumber(argv[2])){
            result = findContacts(argv[2], checkwithspaces, 0);
        }
        else{
            fprintf(stderr, "Wrong input!!!\nFormat with this count of arguments must be -s [searchedElement]");
            return EXIT_FAILURE;
        }
    }
    else if(argc == 4){
        if(checkifNumber(argv[1]) && strcmp(argv[2], "-l") == 0 && checkifNumber(argv[3])){
            int mistakesCount = convertStringToInt(argv[3]);
            if(mistakesCount < 100){
                result = findContacts(argv[1], levenshteinmethod, mistakesCount);
            }
            else{
                fprintf(stderr, "Wrong input!!!\nAn argument [MaximalCountOFUserMistakes] can\'t be larger than 99");
                return EXIT_FAILURE;
            }
        }
        else{
            fprintf(stderr, "Wrong input!!!\nFormat with this count of arguments must be [searchedElement] -l [MaximalCountOFUserMistakes]");
            return EXIT_FAILURE;
        }
    }
    else{
        fprintf(stderr, "Wrong input!!!\nYou sent the program too many arguments");
        return EXIT_FAILURE;
    }

    if(result == 0){
        printf("Not found");
    }
    else if(result == ERROR){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int findContacts(char searchedElement[], selectedMode mode, int countOfMistakes)//basic function for both cases
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

        //Calling appropriate method in dependacy on given arguments in the launching of the program
        if(mode == displayall){
            printf("%s, %s\n", name, number);
            isFound = true;
        }else if(mode == simplecheck){
            if(levenshteinMethod(number, searchedElement, 0) ||
            levenshteinMethod(name, searchedElement, 0)){
                printf("%s, %s\n", name, number);
                isFound = true;
            }
        }else if(mode == checkwithspaces){
            if(CheckWithSpace(0,number, 0, searchedElement) ||
            CheckWithSpace(0,name, 0, searchedElement)){
                printf("%s, %s\n", name, number);
                isFound = true;
            }
        }else if(mode == levenshteinmethod){
            if(levenshteinMethod(number, searchedElement, countOfMistakes) ||
            levenshteinMethod(name, searchedElement, countOfMistakes)){
                printf("%s %s\n", name, number);
                isFound = true;
            }
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


bool isSymbolMatched(char symbol, char wanted){//checking here if a symbol in the provided argument is in the name/number
    char dictionary[10][6] = {"0+", "1", "2abc", "3def", "4ghi", "5jkl", "6mno", "7pqrs", "8tuv", "9wxyz"};
    //Finding possible searched list of chars
    char *possibleFindings = dictionary[wanted - '0'];
    for(int j = 0; j < strlen(possibleFindings); j++){
        if(symbol == possibleFindings[j]){
            return true;
        }
    }
    return false;
}

bool CheckWithSpace(int startPoint, char str[], int startPoint2, char wanted[]){
    for(int i = startPoint; i < strlen(str); i++){
        if(isSymbolMatched(str[i], wanted[startPoint2])){

            //Checking if we already checked all chars in searched element
            if(startPoint2 == strlen(wanted) - 1){
                return true;
            }
            if(CheckWithSpace(i+1, str, startPoint2+1, wanted)){ //going to the next etap of recursion
                return true;
            }
        }

    }
    return false;
}

int Pow(int number, int power){
    if(power > 0){
        return number * Pow(number, power - 1);
    }
    return 1;
}

int convertStringToInt(char numToConvert[]){
    int result = 0;
    for(int i = 0; i < strlen(numToConvert); i++){
        result += (numToConvert[i]-'0') * Pow(10, strlen(numToConvert) - 1 - i);
    }
    return result;
}

bool levenshteinMethod(char str[], char wanted[], int countOfMistakes){
    int initialMistakesCount = countOfMistakes;
     for(int i = 0; i < strlen(str); i++){
        int extra = 0;
        int extraArg = 0;
        for(int j = 0; i + j + extra < strlen(str) && j + extraArg < strlen(wanted); j++){
            if(isSymbolMatched(str[i+j+extra], wanted[j + extraArg])){
                if(j + extraArg == strlen(wanted) - 1){
                    return true;
                }
            }
            else{
                if(countOfMistakes > 0){
                    countOfMistakes--;
                    if(isSymbolMatched(str[i + j + extra + 1], wanted[j + extraArg])){
                        extra++;
                    }else if(isSymbolMatched(str[i + j + extra], wanted[j+extraArg+1])){
                        extraArg++;
                    }
                    if(j + extraArg >= strlen(wanted) - 1){
                        return true;
                    }
                }else{
                    countOfMistakes = initialMistakesCount;
                    break;
                }
                if(i + j + extra >= strlen(str) - 1){
                    if(strlen(wanted) + i + extra - extraArg - strlen(str) <= countOfMistakes){
                        return true;
                    }
                }
            }

        }
    }
    return false;
}