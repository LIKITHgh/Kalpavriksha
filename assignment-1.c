#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 1000

// Global variables for expression parsing
const char *expr;
int pos = 0;

// Function to skip whitespaces
void skip_spaces() {
    while (isspace(expr[pos])) pos++;
}

// Forward declarations
int parse_expression();

// Error handling
void error(const char *msg) {
    printf("%s\n", msg);
    exit(0);
}

// Parse numbers (integers)
int parse_number() {
    skip_spaces();
    if (!isdigit(expr[pos])) {
        error("Error: Invalid expression.");
    }

    int num = 0;
    while (isdigit(expr[pos])) {
        num = num * 10 + (expr[pos] - '0');
        pos++;
    }
    return num;
}

// Parse factors (numbers only for now)
int parse_factor() {
    skip_spaces();
    return parse_number();
}

// Parse terms (handles * and / with precedence)
int parse_term() {
    int value = parse_factor();
    skip_spaces();

    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos++];
        int rhs = parse_factor();

        if (op == '*') {
            value *= rhs;
        } else if (op == '/') {
            if (rhs == 0) {
                error("Error: Division by zero.");
            }
            value /= rhs;
        }
        skip_spaces();
    }
    return value;
}

// Parse full expression (handles + and -)
int parse_expression() {
    int value = parse_term();
    skip_spaces();

    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos++];
        int rhs = parse_term();

        if (op == '+') {
            value += rhs;
        } else {
            value -= rhs;
        }
        skip_spaces();
    }
    return value;
}

int main() {
    char input[MAX];

    // Read full line as input
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;

    expr = input;
    pos = 0;

    int result = parse_expression();
    skip_spaces();

    // If extra invalid characters remain after parsing
    if (expr[pos] != '\0') {
        printf("Error: Invalid expression.\n");
        return 0;
    }

    printf("%d\n", result);
    return 0;
}