#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// specify a current verion
const char VERSION[] = "1.0";

const char DEFAULT_VARIABLE = 'x';
char variable = DEFAULT_VARIABLE;

const int NUMBER_OF_COFFICIENTS = 3;

// %s will be replaced by current version of program.
const char WELCOME_MESSAGE[] = "Quadratic equation solver ver. %s\n";
const char SELECT_INPUT_METHOD[] = "Please, select an input method (1 for simplified, 2 for prettified (BETA)): ";
const char INPUT_COEFFICIENT[] = "Please, input a coeffient %c: ";
const char SELECT_VARIABLE[] = "Please, select variable to use in equation (enter for %c): ";
const char PROVIDE_EQUATION[] = "Please, provide an equation to solve (ex. %c^2 - 5 = 10%c):\n";
const char NO_SOLUTION[] = "No solutions, try your luck next time!\n";
const char ANY_SOLUTION[] = "You won't believe what I have just investigated! You can substitute any number and you will solve the problem.\n";
const char ONE_SOLUTION[] = "Only one solution for your problem: %c = %f\n";
const char TWO_SOLUTIONS[] = "Excellent! Unbelievable! It's two solutions: %c1=%f, %c2=%f\n";

const int INPUT_METHOD_SIMPLIFIED = 1;
const int INPUT_METHOD_PRETTIFIED = 2;

const int EQUATION_TYPE_TRUE = -1;
const int EQUATION_TYPE_STATEMENT = 0;
const int EQUATION_TYPE_LINEAR = 1;
const int EQUATION_TYPE_QUADRATIC = 2;

const bool _DEBUG = true;

// check if char is a number
bool isDigit(char number) {
    return '0' <= number && number <= '9';
}

bool isLetter(char letter) {
    return ('a' <= letter && letter <= 'z') || ('A' <= letter && letter <= 'Z');
}

void zerofy(double array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

// parse incomplete buffer string to double without pain
double parseDouble(char buffer[], char bufferEnd) {
    const int bufferSize = bufferEnd;
    char temp[bufferSize];
    for (int i = 0; i < bufferSize; i++) {
        temp[i] = buffer[i];
    }
    return atof(temp);
}

// count non-space characters in string and return them
int countNonSpaceCharacters(char string[]) {
    int counter = 0;
    unsigned long stringLength = strlen(string);
    for (int i = 0; i < stringLength; i++) {
        if (string[i] != ' ') {
            counter++;
        }
    }
    return counter;
}

// remove spaces from string and store it into newString
void removeSpaces(char string[], char newString[]) {
    int cursor = 0;
    const unsigned long stringLength = strlen(string);
    for (int i = 0; i < stringLength; i++) {
        if (string[i] != ' ') {
            newString[cursor] = string[i];
            cursor++;
        }
    }
}

void reverse(double array[], int size) {
    for (int low = 0, high = size - 1; low < high; low++, high--) {
        double temp = array[low];
        array[low] = array[high];
        array[high] = temp;
    }
}

// gets the coefficients of summands such as -2x, 2.5, 90x^2
int getSummands(char equation[], double coefficients[]) {
    zerofy(coefficients, NUMBER_OF_COFFICIENTS);

    // remove all spaces from equation
    const unsigned long nonSpaceCharacters = countNonSpaceCharacters(equation);
    char formattedEquation[nonSpaceCharacters];
    removeSpaces(equation, formattedEquation);

    // coefficients - in this array we will store coefficients for different types of summands {simple: 90, linear: 2.5x, quadratic: -2x^2}

    // numberBuffer to store recent characters from parsed string
    char numberBuffer[64];
    int numberBufferEnd = 0;

    // powerBuffer is needed to store recent characters from parsed string after character «^»
    char powerBuffer[64];
    int powerBufferEnd = 0;

    // variable to detect is equality sign passed or not
    bool isRight = false;
    
    // is value negative state
    bool isNumberNegative = false;
    bool isPowerNegative = false;
    bool isPower = false;

    double power = 0;

    for (unsigned char i = 0; i < nonSpaceCharacters; i++) {
        const char current = formattedEquation[i];
        switch (current) {
            case '=':
                // passed the equals sign
                isRight = true;
            case '-':
                // check if minus is not the first character
                if (numberBufferEnd == 0) {
                    if (isPower) {
                        isPowerNegative = !isPowerNegative;
                    } else {
                        isNumberNegative = !isNumberNegative;
                    }
                }
            case '+':
                // check if plus is not the first character of currently parsed number
                if (numberBufferEnd > 0) {
                    if (powerBufferEnd > 0) {
                        int modifier = 1;
                        if (isPowerNegative) modifier = -1;
                        power += parseDouble(powerBuffer, powerBufferEnd) * modifier - 1;
                    }
                    if (power != 2 && power != 1 && power != 0) {
                        printf("Power %f is not supported yet.\n", power);
                        return -1;
                    }
                    double n = parseDouble(numberBuffer, numberBufferEnd);
                    int modifier = 1;
                    if (isNumberNegative) modifier = -1;
                    coefficients[(int) power] += n * modifier;
                    numberBufferEnd = powerBufferEnd = power = 0;
                    isPower = isPowerNegative = isNumberNegative = false;
                    i--;
                }
                break;
            case '.':
                if (isPower) {
                    // add char to power buffer
                    powerBuffer[powerBufferEnd] = current;
                    powerBufferEnd++;
                } else {
                    // add char to number buffer
                    numberBuffer[numberBufferEnd] = current;
                    numberBufferEnd++;
                }
                break;
            case '^':
                isPower = true;
                break;
            case '*':
            case '/':
                printf("This operation is not supported yet.\n");
                printf("It is going to be introduced in future version.\n");
                return -1;
            default:
                // check if current char is numberBuffer or variable
                if (isDigit(current)) {
                    if (isPower) {
                        // add char to power buffer
                        powerBuffer[powerBufferEnd] = current;
                        powerBufferEnd++;
                    } else {
                        // add char to number buffer
                        numberBuffer[numberBufferEnd] = current;
                        numberBufferEnd++;
                    }
                } else if (current == variable) {
                    if (isPower) {
                        printf("You can't use variable as power value.\n");
                        return -1;
                    }
                    // if variable without coefficient then set coefficient to one
                    if (numberBufferEnd == 0) {
                        // add char to number buffer
                        numberBuffer[numberBufferEnd] = '1';
                        numberBufferEnd++;
                    }
                    // add power value
                    power++;
                } else {
                    printf("Detected unknown character >> %c\n", current);
                    return -1;
                }
                break;
        }
    }
    
    if (!isRight) {
        printf("Equation is not valid.\n");
        return -1;
    }
    
    if (numberBufferEnd > 0) {
        if (powerBufferEnd > 0) {
            int modifier = 1;
            if (isPowerNegative) modifier = -1;
            power += parseDouble(powerBuffer, powerBufferEnd) * modifier - 1;
        }
        double n = parseDouble(numberBuffer, numberBufferEnd);
        int modifier = 1;
        if (isNumberNegative) modifier = -1;
        coefficients[(int) power] += n * modifier;
    }
    
    return 0;
}

double getDiscriminant(double a, double b, double c) {
    return (b * b) - 4 * a * c;
}

void getQuadraticSolutions(double a, double b, double discriminant, double solutions[]) {
    solutions[0] = (-b + sqrt(discriminant)) / (2 * a);
    solutions[1] = (-b - sqrt(discriminant)) / (2 * a);
}

int selectInputMethod(void) {
    while (true) {
        puts(SELECT_INPUT_METHOD);
        int input;
        scanf("%d", &input);
        getchar();
        if (input == INPUT_METHOD_SIMPLIFIED || input == INPUT_METHOD_PRETTIFIED) {
            return input;
        } else {
            puts("Unknown input value.\n");
        }
    }
    return 0;
}

void simplifiedInput(double coefficients[]) {
    for (int i = 0; i < NUMBER_OF_COFFICIENTS; i++) {
        printf(INPUT_COEFFICIENT, 'a' + i);
        int coeficient;
        if (!scanf("%d", &coeficient)) {
            getchar();
            i--;
            continue;
        }
        coefficients[i] = coeficient;
    }
    
    reverse(coefficients, NUMBER_OF_COFFICIENTS);
}

char inputVariable() {
    while (true) {
        printf(SELECT_VARIABLE, DEFAULT_VARIABLE);
        char variable = getchar();
        
        // if provided empty string use default character
        if (variable == '\n') variable = DEFAULT_VARIABLE;
        else getchar();

        if (isLetter(variable)) {
            return variable;
        } else {
            printf("%c is not a valid variable for equation.\n", variable);
        }
    }
}

void prettifiedInput(double coeficients[]) {
    variable = inputVariable();

    while (true) {
        printf(PROVIDE_EQUATION, variable, variable);
        char equation[64];
        scanf("%64[^\n]", equation);
        getchar();

        int code = getSummands(equation, coeficients);

        if (code == 0) {
            break;
        }
    }
}

int getEquationType(double coefficients[]) {
    double a = coefficients[2], b = coefficients[1], c = coefficients[0];
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                return EQUATION_TYPE_TRUE;
            }
            return EQUATION_TYPE_STATEMENT;
        }
        return EQUATION_TYPE_LINEAR;
    }
    return EQUATION_TYPE_QUADRATIC;
}

int solveQuadraticEquation(double coefficients[], double solutions[]) {
    // parameters for equation ax^2 + bx + c = 0
    double a = coefficients[2], b = coefficients[1], c = coefficients[0];
    
    // get the discriminant with magic formula: D = b^2 - 4ac
    const double discriminant = getDiscriminant(a, b, c);
    if (discriminant > 0) {
        // if equation has two solutions
        getQuadraticSolutions(a, b, discriminant, solutions);
        return 2;
    } else if (discriminant < 0) {
        // if equation has no solutions
        return 0;
    } else {
        // if equation has one solutions
        getQuadraticSolutions(a, b, discriminant, solutions);
        return 1;
    }
}

void displaySolution(int roots, double solutions[]) {
    switch (roots) {
        case -1:
            printf(ANY_SOLUTION);
            break;
        case 0:
            printf(NO_SOLUTION);
            break;
        case 1:
            printf(ONE_SOLUTION, variable, solutions[0]);
            break;
        case 2:
            printf(TWO_SOLUTIONS, variable, solutions[0], variable, solutions[1]);
            break;
        default:
            break;
    }
}

int solveLinearEquation(double coefficients[], double solutions[]) {
    const double a = coefficients[0], b = coefficients[1];
    solutions[0] = -a / b;
    return 1;
}

void solveEquation(double coefficients[]) {
    const int equationType = getEquationType(coefficients);
    double solutions[2];
    int roots = 0;
    switch (equationType) {
        case EQUATION_TYPE_TRUE:
            roots = -1;
            break;
        case EQUATION_TYPE_STATEMENT:
            roots = 0;
            break;
        case EQUATION_TYPE_LINEAR:
            roots = solveLinearEquation(coefficients, solutions);
            break;
        case EQUATION_TYPE_QUADRATIC:
            roots = solveQuadraticEquation(coefficients, solutions);
            break;
        default:
            break;
    }
    displaySolution(roots, solutions);
}

void input(double coefficients[]) {
    int inputMethod = selectInputMethod();
    if (inputMethod == INPUT_METHOD_SIMPLIFIED) {
        simplifiedInput(coefficients);
    } else if (inputMethod == INPUT_METHOD_PRETTIFIED) {
        prettifiedInput(coefficients);
    } else {
        // oops...
    }
}

int main() {
    
    printf(WELCOME_MESSAGE, VERSION);

    double coefficients[NUMBER_OF_COFFICIENTS];

    input(coefficients);

    solveEquation(coefficients);

    return 0;
}
