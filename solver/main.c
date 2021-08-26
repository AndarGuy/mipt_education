#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// specify a current verion
#define VERSION "1.0"

// function which will be implemented in future update
int getPower(int pointer, char equation[]) {
    return 0;
}

// function which will be implemented in future update
bool checkDouble(char buffer[], char pointer) {
    return true;
}

// check if char is a number
bool isNumber(char number) {
    return '0' <= number && number <= '9';
}

// parse incomplete buffer string to double without pain
double parseDouble(char buffer[], char pointer) {
    const int bufferSize = pointer;
    char temp[bufferSize];
    for (int i = 0; i < bufferSize; i++) temp[i] = buffer[i];
    return atof(temp);
}

// gets the sums of summands such as -2x, 2.5, 90x^2
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

double getDiscriminant(double a, double b, double c) {
    return pow(b, 2) - 4 * a * c;
}

double getLinearSolution(double b, double c) {
    return c / -b;
}

double* getQuadraticSolutions(double a, double b, double discriminant) {
    static double solutions[2];
    solutions[0] = (-b + pow(discriminant, 0.5)) / (2 * a);
    solutions[1] = (-b - pow(discriminant, 0.5)) / (2 * a);
    return solutions;
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
    double* summands = getSummands(equation, variable);
    
    // parameters for equation ax^2 + bx + c = 0
    double c = *(summands + 0), b = *(summands + 1), a = *(summands + 2);
    
    if (isSolvable(a, b, c)) {
        if (!isAnySolution(a, b, c)) {
            // check if it is linear equation
            if (a == 0) {
                printf(ONE_SOLUTION, variable, getLinearSolution(b, c));
                putchar('\n');
            }
            else {
                // get the discriminant with magic formula: D = b^2 - 4ac
                const double discriminant = getDiscriminant(a, b, c);
                if (discriminant > 0) {
                    // if equation has two solutions
                    double* solutions = getQuadraticSolutions(a, b, discriminant);
                    double solution1 = *(solutions + 0), solution2 = *(solutions + 1);
                    printf(TWO_SOLUTIONS, variable, solution1, variable, solution2);
                    putchar('\n');
                } else if (discriminant < 0) {
                    // if equation has no solutions
                    puts(NO_SOLUTION);
                } else {
                    // if equation has one solutions
                    double solution = *(getQuadraticSolutions(a, b, discriminant) + 0);
                    printf(ONE_SOLUTION, variable, solution);
                    putchar('\n');
                }
            }
        } else puts(ANY_SOLUTION);
    } else puts(NO_SOLUTION);


    return 0;
}
