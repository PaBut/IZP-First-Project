## Project description
The goal of the project is to create a program that emulates a simplified algorithm for finding contacts given a sequence of digits. The input to the program is a sequence of digits. The output of the program will contain a list of contacts that match the specified numeric filter.
## Usage
The contact list is given in form of consecutive names and numbers(separated with a new line) and must be given to the program using stdin. The contact list example is given down below:
```
Petr Dvorak
603123456\n
Jana Novotna
777987654
Bedrich Smetana ml.
541141120
```
The input sequence are numbers that correspond to specific letters and itself:  
2 (abc), 3 (def), 4 (ghi), 5 (jkl), 6 (mno), 7 (pqrs), 8 (tuv), 9 (wxyz), 0 (+)

- To find a contact which name or number corrensponds to the consecutive sequence of numbers following command should be used as an example ```./t9search 656 <list_file```
- In case of finding a contact where name or number has nonconsecutive sequence of the given numbers use the ```-s``` flag
- Search for similar contacts. If the user entered a wrong search string (wrong number), the program will offer similar results. User errors could be: wrong digit, missing digit, extra digit entered. The program should be configured with the program argument ```-l L```, where L represents the maximal number of user errors .
