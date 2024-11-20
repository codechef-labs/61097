#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>

using namespace std;

const int MAX_EXPR = 100;

// Stack implementation for expression evaluation
double numberStack[MAX_EXPR];
char operatorStack[MAX_EXPR];
int numTop = -1;
int opTop = -1;

// Stack operations
void pushNumber(double num) {
    numberStack[++numTop] = num;
}

void pushOperator(char op) {
    operatorStack[++opTop] = op;
}

double popNumber() {
    return numberStack[numTop--];
}

char popOperator() {
    return operatorStack[opTop--];
}

// Function to get operator precedence
int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
}

// Function to perform operation
double performOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                cout << "Error: Division by zero!\n";
                exit(1);
            }
            return a / b;
        case '%': 
            if (b == 0) {
                cout << "Error: Modulo by zero!\n";
                exit(1);
            }
            return static_cast<int>(a) % static_cast<int>(b);
        default: return 0;
    }
}

// Function to evaluate expression
double evaluateExpression(const string& expr) {
    numTop = -1;
    opTop = -1;
    
    string token;
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (isspace(expr[i])) continue;
        
        if (isdigit(expr[i]) || expr[i] == '.') {
            token += expr[i];
        } else {
            if (!token.empty()) {
                pushNumber(stod(token));
                token.clear();
            }
            
            if (expr[i] == '(') {
                pushOperator(expr[i]);
            }
            else if (expr[i] == ')') {
                while (opTop >= 0 && operatorStack[opTop] != '(') {
                    char op = popOperator();
                    double b = popNumber();
                    double a = popNumber();
                    pushNumber(performOperation(a, b, op));
                }
                if (opTop >= 0) popOperator(); // Remove '('
            }
            else if (expr[i] == '+' || expr[i] == '-' || 
                     expr[i] == '*' || expr[i] == '/' || expr[i] == '%') {
                while (opTop >= 0 && operatorStack[opTop] != '(' &&
                       getPrecedence(operatorStack[opTop]) >= getPrecedence(expr[i])) {
                    char op = popOperator();
                    double b = popNumber();
                    double a = popNumber();
                    pushNumber(performOperation(a, b, op));
                }
                pushOperator(expr[i]);
            }
        }
    }
    
    if (!token.empty()) {
        pushNumber(stod(token));
    }
    
    while (opTop >= 0) {
        char op = popOperator();
        double b = popNumber();
        double a = popNumber();
        pushNumber(performOperation(a, b, op));
    }
    
    return numberStack[numTop];
}

int main() {
    int choice;
    double num1, num2;
    string expression;
    
    while (true) {
        cout << "\nCalculator Menu:\n"
             << "1. Addition\n"
             << "2. Subtraction\n"
             << "3. Multiplication\n"
             << "4. Division\n"
             << "5. Modulo\n"
             << "6. Evaluate Expression\n"
             << "7. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        
        if (choice == 7) break;
        
        if (choice >= 1 && choice <= 5) {
            cout << "Enter two numbers: ";
            cin >> num1 >> num2;
            
            cout << fixed << setprecision(2);
            
            switch (choice) {
                case 1:
                    cout << "Result: " << num1 + num2 << endl;
                    break;
                case 2:
                    cout << "Result: " << num1 - num2 << endl;
                    break;
                case 3:
                    cout << "Result: " << num1 * num2 << endl;
                    break;
                case 4:
                    if (num2 == 0) {
                        cout << "Error: Division by zero!\n";
                    } else {
                        cout << "Result: " << num1 / num2 << endl;
                    }
                    break;
                case 5:
                    if (num2 == 0) {
                        cout << "Error: Modulo by zero!\n";
                    } else {
                        cout << "Result: " << static_cast<int>(num1) % static_cast<int>(num2) << endl;
                    }
                    break;
            }
        }
        else if (choice == 6) {
            cout << "Enter expression (e.g., 4 + 3 * (2 + 4) / 2): ";
            cin.ignore(); // Clear input buffer
            getline(cin, expression);
            
            cout << fixed << setprecision(2);
            cout << "Result: " << evaluateExpression(expression) << endl;
        }
        else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
    
    return 0;
}
