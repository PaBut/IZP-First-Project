#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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
int stringLength(char str[]);
bool areEqualStrings(char str1[], char str2[]);
int convertStringToInt(char numToConvert[]);
char toLower(char c);
int returnError(int returnValue, char msg[]);


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
            return returnError(EXIT_FAILURE ,"Wrong input!!!\nThe argument(searchedElement) you sent to the application is not a number!!!");
        }
    }
    else if(argc == 3){
        if(areEqualStrings(argv[1], "-s") && checkifNumber(argv[2])){
            result = findContacts(argv[2], checkwithspaces, 0);
        }
        else{
            return returnError(EXIT_FAILURE ,"Wrong input!!!\nFormat with this count of arguments must be -s [searchedElement]");
        }
    }
    else if(argc == 4){
        if(checkifNumber(argv[1]) && areEqualStrings(argv[2], "-l") && checkifNumber(argv[3])){
            int mistakesCount = convertStringToInt(argv[3]);
            if(mistakesCount < 100){
                result = findContacts(argv[1], levenshteinmethod, mistakesCount);
            }
            else{
                return returnError(EXIT_FAILURE ,"Wrong input!!!\nAn argument [MaximalCountOFUserMistakes] can\'t be larger than 99");
            }
        }
        else{
            return returnError(EXIT_FAILURE ,"Wrong input!!!\nFormat with this count of arguments must be [searchedElement] -l [MaximalCountOFUserMistakes]");
        }
    }
    else{
        return returnError(EXIT_FAILURE ,"Wrong input!!!\nYou sent the program too many arguments");
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
        //Checking in the following lines if there's an error in the file input
        if(!(isValidName = getLine(name))){  
            return returnError(ERROR ,"Wrong file input!!!\nYou forgot to add a number after the name");
        }
        if(isValidName == ERROR || (isLastLine = getLine(number)) == ERROR){
            return ERROR;
        }

        if(!checkIfName(name) || !checkifNumber(number)){
            return returnError(ERROR ,"Wrong file input!!!\nMissed the order of names and numbers");
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
                printf("%s, %s\n", name, number);
                isFound = true;
            }
        }
    }while(isLastLine);

    return isFound;
}

//Get a name or a number of the contact
//Also checking here, if the list is full read and the line is bigger than allowed maximum
//and if there's a empty line
int getLine(char arr[])
{
    for(int i = 0; i < SIZE ; i++ ){

        int symbolASCII = getc(stdin);
        if(symbolASCII == EOF){
            if(i == 0){
                return returnError(ERROR ,"Wrong file input!!!\nEmpty line at the end of the list of contacts");
            }
            arr[i] = '\0';
            return false;
        }
        arr[i] = toLower((char)symbolASCII);
        if(arr[i] == '\n'){
            if(i == 0){
                return returnError(ERROR ,"Wrong file input!!!\nEmpty line in the list of contacts");
            }
            arr[i] = '\0';
            return true;
        }        
    }    
    return returnError(ERROR ,"Wrong file input!!!\nThe line is higher than 100 chars");
}

bool checkifNumber(char str[]){//checking if the current line is a number, not a name of the contact
    bool isNumber = true;
    for(int i = 0; i < stringLength(str); i++){
        if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '+')){
            isNumber = false;
        }
    }
    return isNumber;
}

bool checkIfName(char str[]){//checking if the current line is a name, not the number of the contact
    bool isName = false;
    for(int i = 0; i < stringLength(str); i++){
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

int Pow(int number, int power){
    if(power > 0){
        return number * Pow(number, power - 1);
    }
    return 1;
}

int stringLength(char str[]){
    int i = 0;
    for(; str[i] != '\0'; i++){}
    return i;
}

bool areEqualStrings(char str1[], char str2[]){
    if(stringLength(str1) == stringLength(str2)){
        for(int i = 0; i < stringLength(str1); i++){
            if(str1[i] != str2[i]){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

int convertStringToInt(char numToConvert[]){
    int result = 0;
    for(int i = 0; i < stringLength(numToConvert); i++){
        result += (numToConvert[i]-'0') * Pow(10, stringLength(numToConvert) - 1 - i);
    }
    return result;
}

int returnError(int returnValue, char msg[]){
    fprintf(stderr, msg);
    return returnValue;
}

bool isSymbolMatched(char symbol, char wanted){//checking here if a symbol in the provided argument is in the name/number
    char dictionary[10][6] = {"0+", "1", "2abc", "3def", "4ghi", "5jkl", "6mno", "7pqrs", "8tuv", "9wxyz"};
    //Finding possible searched list of chars
    char *possibleFindings = dictionary[wanted - '0'];
    for(int j = 0; j < stringLength(possibleFindings); j++){
        if(symbol == possibleFindings[j]){
            return true;
        }
    }
    return false;
}

bool CheckWithSpace(int startPoint, char str[], int startPoint2, char wanted[]){
    for(int i = startPoint; i < stringLength(str); i++){
        if(isSymbolMatched(str[i], wanted[startPoint2])){

            //Checking if we already checked all chars in searched element
            if(startPoint2 == stringLength(wanted) - 1){
                return true;
            }
            if(CheckWithSpace(i+1, str, startPoint2+1, wanted)){ //going to the next etap of recursion
                return true;
            }
        }

    }
    return false;
}

bool levenshteinMethod(char str[], char wanted[], int countOfMistakes){
    int initialMistakesCount = countOfMistakes;
     for(int i = 0; i < stringLength(str); i++){
        int extra = 0;
        int extraArg = 0;
        for(int j = 0; i + j + extra < stringLength(str) && j + extraArg < stringLength(wanted); j++){
            if(isSymbolMatched(str[i+j+extra], wanted[j + extraArg])){
                if(j + extraArg >= stringLength(wanted) - 1){
                    return true;
                }
            }
            else{
                if(countOfMistakes > 0){
                    countOfMistakes--;
                    if(isSymbolMatched(str[i + j + extra], wanted[j+extraArg+1])){
                        extraArg++;
                    }
                    else if(isSymbolMatched(str[i + j + extra + 1], wanted[j + extraArg])){
                        extra++;
                    }
                    if(j + extraArg >= stringLength(wanted) - 1){
                        return true;
                    }
                }else{
                    countOfMistakes = initialMistakesCount;
                    break;
                }
            }
            if(i + j + extra >= stringLength(str) - 1){
                if(stringLength(wanted) + i + extra - extraArg - stringLength(str) <= countOfMistakes){
                    return true;
                }
            }
        }
    }
    return false;
}
