#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//Maximal size of line(100symbols + char '\0')
#define SIZE 101
//An integer value returned by any function other than main in case of error 
#define ERROR -1

//possible modes of searching in the program
typedef enum{
    displayall,
    simplecheck,
    checkwithspaces,
    levenshteinmethod
} selectedMode;

int findContacts(char searchedElement[], selectedMode mode, int countOfMistakes);
int getLine(char line[]);
bool checkifNumber(char line[]);
bool checkIfName(char line[]);
bool CheckWithSpace(int startPoint, char line[], int startPoint2, char wanted[]);
bool levenshteinMethod(char line[], char wanted[], int countOfMistakes);
int stringLength(char str[]);
bool areEqualStrings(char str1[], char str2[]);
int convertStringToInt(char numToConvert[]);
char toLower(char c);
int returnError(int returnValue, char msg[]);

int main(int argc, char* argv[])
{       
    //Variable that indicates if any contacts were found and if there's an error in input file
    int result = 0;

    //Checking if the arguments were proper and setting the search mode in dependancy on it
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
            if(stringLength(argv[3]) > 2){
                return returnError(EXIT_FAILURE ,"Wrong input!!!\nAn argument [MaximalCountOFUserMistakes] can\'t be larger than 99");
            }
            result = findContacts(argv[1], levenshteinmethod, convertStringToInt(argv[3]));
        }
        else{
            return returnError(EXIT_FAILURE ,"Wrong input!!!\nFormat with this count of arguments must be [searchedElement] -l [MaximalCountOFUserMistakes]");
        }
    }
    else{
        return returnError(EXIT_FAILURE ,"Wrong input!!!\nYou sent the program too many arguments");
    }

    if(result == false){
        printf("Not found");
    }
    else if(result == ERROR){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//basic function for all modes of searching
int findContacts(char searchedElement[], selectedMode mode, int countOfMistakes)
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
            //That function with a 0 as countOfMistakes works like is needed in the first task
            //it finds all lines, where all the numbers in the searched elelment appear in a row(without an interruption)
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
//Also checking here, if the list is full read and if the line is bigger than allowed maximum
//and if there's a empty line
int getLine(char line[])
{
    for(int i = 0; i < SIZE ; i++ ){

        int symbolASCII = getc(stdin);
        if(symbolASCII == EOF){
            if(i == 0){
                return returnError(ERROR ,"Wrong file input!!!\nEmpty line at the end of the list of contacts");
            }
            line[i] = '\0';
            return false;
        }
        line[i] = toLower((char)symbolASCII);
        if(line[i] == '\n'){
            if(i == 0){
                return returnError(ERROR ,"Wrong file input!!!\nEmpty line in the list of contacts");
            }
            line[i] = '\0';
            return true;
        }        
    }    
    return returnError(ERROR ,"Wrong file input!!!\nThe line is higher than 100 chars");
}

//checking if the current line is a number, not a name of the contact
bool checkifNumber(char line[]){
    bool isNumber = true;
    for(int i = 0; i < stringLength(line); i++){
        if(!((line[i] >= '0' && line[i] <= '9') || line[i] == '-' || line[i] == '+')){
            isNumber = false;
        }
    }
    return isNumber;
}

//checking if the current line is a name, not the number of the contact
bool checkIfName(char line[]){
    bool isName = false;
    for(int i = 0; i < stringLength(line); i++){
        if(line[i] >= 'a' && line[i] <= 'z'){
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

//Returning an error result accompanied with text sent to stderr
int returnError(int returnValue, char msg[]){
    fprintf(stderr, msg);
    return returnValue;
}

bool isSymbolMatched(char symbol, char wanted){//checking here if a symbol in the provided argument is in the name/number
    char dictionary[10][6] = {"0+", "1", "2abc", "3def", "4ghi", "5jkl", "6mno", "7pqrs", "8tuv", "9wxyz"};
    //Finding possible searched list of chars
    char *possibleFindings = dictionary[wanted - '0'];
    //checking if any symbol of the list equals to 
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

            //Checking if we already checked all chars in searched element(base state)
            if(startPoint2 == stringLength(wanted) - 1){
                return true;
            }
            //Checking the next symbol in a searched element(going to the next etap of recursion)
            if(CheckWithSpace(i+1, str, startPoint2+1, wanted)){ 
                return true;
            }
        }

    }
    return false;
}

bool levenshteinMethod(char line[], char wanted[], int countOfMistakes){
    
    int initialMistakesCount = countOfMistakes;
    //iterating the start of checking line(name/number) 
    for(int i = 0; i < stringLength(line); i++){
        
        int extra = 0;
        int extraArg = 0;

        for(int j = 0; i + j + extra < stringLength(line) && j + extraArg < stringLength(wanted); j++){
            
            int indexArg = j + extraArg;
            int indexLine = i + j + extra;

            //if symbols matches then iteration goes on, else it will satisfy the condition below
            if(!isSymbolMatched(line[indexLine], wanted[indexArg])){
                if(countOfMistakes > 0){
                    countOfMistakes--;
                    //if neither of the conditions below is true, then the number(symbol) is just mistaken
                    //checking with for loop if one or more number were missed/added extra in the searched element
                    for(int m = 1; m <= countOfMistakes+1 && (indexArg + m < stringLength(wanted) && indexLine + m < stringLength(line)); m++){
                        if(indexArg + m < stringLength(wanted)){
                            //extra number(s) was/were added in the searched element
                            //From now on checking every time symbol in the searched element with incremented on count of extra numbers index
                            if(isSymbolMatched(line[indexLine], wanted[indexArg+m])){
                                extraArg+=m;
                                indexArg+=m;
                                countOfMistakes-=m-1;
                                break;
                            }
                        }
                        if(indexLine + m < stringLength(line)){
                            //a number(s) was/were missed in the searched element
                            //From now on every time checking a symbol in the line with incremented on count of missed numbers index
                            if(isSymbolMatched(line[indexLine + m], wanted[indexArg])){
                                extra+=m;
                                indexLine+=m;
                                countOfMistakes-=m-1;
                                break;
                            }  
                        }
                    }
                }
                else{
                    //Not a single mistake left. Starting from the beginning with the next symbol in the line
                    //Setting countOfMistakes to the default value
                    countOfMistakes = initialMistakesCount;
                    break;
                }
            }
            //If the last element in the searched element was checked and it matches the symbol in the searched element
            //or count of possible mistakes cover it, than line suits
            if(indexArg >= stringLength(wanted) - 1){
                return true;
            }
            //The checked symbol in line was last
            if(indexLine >= stringLength(line) - 1){
                //if iteration can go outside of the boundaries of the line(which is being checked)
                //then check if the difference of maximal possible position of iteration and the length of line
                //is lower than the count of possible mistakes, then current contact is suitable
                if(stringLength(wanted) + i + extra - extraArg - stringLength(line) <= countOfMistakes){
                    return true;
                }
            }
        }
    }
    return false;
}