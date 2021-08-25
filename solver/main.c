#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// specify a current verion
#define VERSION "1.0"

// check if char is a number
bool isNumber(char number) {
    return '0' <= number && number <= '9';
}

// function which will be implemented in future update
int getPower(int pointer, char equation[]) {
    return 0;
}

// parse incomplete buffer string to double without pain
double parseDouble(char buffer[], char pointer) {
    const int bufferSize = pointer;
    char temp[bufferSize];
    for (int i = 0; i < bufferSize; i++) temp[i] = buffer[i];
    return atof(temp);
}

bool checkDouble(char buffer[], char pointer) {
//    for (int i = 0; i < pointer + 1; i++) if (!isNumber(buffer[i]) && !buffer[i] == '.' && );
    return true;
}

// gets the sum of simple summands such as -2, 2.5, 90
double* getSummands(char equation[], char variable) {
    // in this array we will store sums for different types of summands {simple: 90, linear: 2.5x, quadratic: -2x^2}
    static double sums[] = {0, 0, 0};
    // buffer to store recent characters from parsed string
    char buffer[64];
    char bufferEnd = 0;
    // variable to detect is equality sign passed or not
    bool isRight = false;
    
    // current summands states
    bool isPower = false, isLinear = false, isSimple = true;
    
    // is value negative state
    bool isNegative = false;
    
    for (unsigned char pointer = 0; pointer < strlen(equation); pointer++) {
        const char current = equation[pointer];
        switch (current) {
            case '=':
                // passed the equals sign
                isRight = true;
            case '-':
                // check if minus is not the first character
                if (bufferEnd == 0) isNegative = !isNegative;
            case '+':
            addSummand:
                // check if plus is not the first character
                if (bufferEnd > 0) {
                    double number = parseDouble(buffer, bufferEnd);
                    char modifier = 1;
                    if (isNegative) modifier = -1;
                    sums[isPower + isLinear + isSimple - 1] += number * modifier;
                    bufferEnd = 0;
                    isPower = false; isLinear = false; isSimple = true; isNegative = false;
                    pointer--;
                }
                break;
            case '.':
                buffer[bufferEnd] = current;
                bufferEnd++;
                break;
            case '^':
                isPower = true;
                break;
            case '*':
            case '/':
                puts("This operation is not supported yet.");
                puts("It is going to be introduced in future version.");
                break;
            default:
                // check if current char is number or variable
                if (isNumber(current)) {
                    if (!isPower) {
                        buffer[bufferEnd] = current;
                        bufferEnd++;
                    } else if (current != '2') puts("Different power operations support will be introduced in future update.");
                } else if (current == variable) {
                    // if variable without coefficient then set coefficient to one
                    if (bufferEnd == 0) {
                        buffer[bufferEnd] = '1';
                        bufferEnd++;
                    }
                    // set summand is linear
                    isLinear = true;
                } else {
                    // if unsupported character is provided ignore it
                }
                break;
        }
    }
    
    // if still have buffer push it
    if (bufferEnd > 0) goto addSummand;
    
    return sums;
}

// if a == 0 and b == 0 but c != 0 then the equation is unsolvable
bool isSolvable(double a, double b, double c) {
    return a != 0 || b != 0 || c == 0;
}

// if
bool isAnySolution(double a, double b, double c) {
    return a == 0 && b == 0 && c == 0;
}

int main() {
    const char DEFAULT_VARIABLE = 'x';
    
    // %s will be replaced by current version of program.
    const char WELCOME_MESSAGE[] = "Quadratic equation solver ver. %s",
        SELECT_VARIABLE[] = "Please select variable to use in equation (enter for %c): ",
        PROVIDE_EQUATION[] = "Please provide an equation to solve (ex. %c^2 - 5 = 10%c):",
        NO_SOLUTION[] = "No solutions, try your luck next time!",
        ANY_SOLUTION[] = "You won't believe what I have just investigated! You can substitute any number and you will solve the problem.",
        ONE_SOLUTION[] = "Only one solution for your problem: %c = %f",
        TWO_SOLUTIONS[] = "Excellent! Unbelievable! It's two solutions: %c1=%f, %c2=%f";
    
    printf(WELCOME_MESSAGE, VERSION);
    putchar('\n');
    
    printf(SELECT_VARIABLE, DEFAULT_VARIABLE);
    char variable = getchar();
    
    // if provided empty string use default character
    if (variable == '\n') variable = DEFAULT_VARIABLE;
    else getchar();
    
    printf(PROVIDE_EQUATION, variable, variable);
    putchar('\n');
    
    char equation[64];
    scanf("%[^\n]", equation);
    double* summands = getSummands(equation, 'x');
    
    // parameters for equation ax^2 + bx + c = 0
    double c = *(summands + 0), b = *(summands + 1), a = *(summands + 2);
    
    if (isSolvable(a, b, c)) {
        if (!isAnySolution(a, b, c)) {
            // check if it is linear equation
            if (a == 0) {
                printf(ONE_SOLUTION, variable, c / -b);
                putchar('\n');
            }
            else {
                // get the discriminant with magic formula: D = b^2 - 4ac
                const double discriminant = pow(b, 2) - 4 * a * c;
                if (discriminant > 0) {
                    // if equation has two solutions
                    printf(TWO_SOLUTIONS, variable, (-b + pow(discriminant, 0.5)) / (2 * a), variable, (-b - pow(discriminant, 0.5)) / (2 * a));
                    putchar('\n');
                } else if (discriminant < 0) {
                    // if equation has no solutions
                    puts(NO_SOLUTION);
                } else {
                    // if equation has one solutions
                    printf(ONE_SOLUTION, variable, -b / (2 * a));
                    putchar('\n');
                }
            }
        } else puts(ANY_SOLUTION);
    } else puts(NO_SOLUTION);


    return 0;
}
