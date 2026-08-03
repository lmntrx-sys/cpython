#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct 
{
    double* data;
    size_t size;
    size_t capacity;
} Stack;

typedef struct 
{
    char op;
} Opstack;


char* getExpression()
{
    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;

    nread = getline(&line, &cap, stdin);

    if (nread == -1)
    {
        free(line);
        return NULL;
    }

    line[strcspn(line, "\n")] = '\0';
    return line;
}

int32_t* eval(int32_t* stack, char op) {

    if (op == '+') { stack[*stack - 1] += stack[*stack]; }
    else if (op == '-') { stack[*stack - 1] -= stack[*stack]; }
    else if (op == '/') { stack[*stack - 1] /= stack[*stack]; }
    else if (op == '*') { stack[*stack - 1] *= stack[*stack]; }
    else if (op == '%'){ stack[*stack-1] %= stack[*stack]; }
    *stack -= 1; 
    return stack;
}

void push(Stack* stack, double value){
    
    if (stack->size == stack->capacity){
        stack->capacity = stack->capacity ? stack->capacity * 2 : 8;
        double *new_data = realloc(stack->data, stack->capacity * sizeof(*stack->data));

        if (new_data == NULL){
            fprintf(stderr, "Out of memory! Sorry");
            free(stack->data);
            exit(1);
        }
        stack->data = new_data;

    }
    stack->data[stack->size++] = value;
}

// FIX: this took `Stack stack` by value. C passes structs by copy — any
// change made to `stack` inside this function was invisible to main() the
// moment parseExpression returned. On top of that, the very next line
// declared a *second*, completely separate `Stack st = {0}` and used that
// instead — so the parameter wasn't just ineffective, it was fully ignored.
// Two independent Stacks existed: the one main() built (permanently empty),
// and this local one (built correctly, then thrown away when the function
// returned — its heap-allocated `.data` leaking, unreachable forever).
// A pointer fixes both problems at once: no copy, no shadow variable.
int32_t parseExpression(char *line, Stack* stack, Opstack* op_stack)
{
    char *p = line;
    // (local `Stack st = {0};` removed — `stack` IS the caller's Stack now)

    while (*p != '\n' && *p != '\0')
    {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (isdigit((unsigned char)*p)) {
            char *endptr;
            double value = strtod(p, &endptr);

            push(stack, value);
            p = endptr;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%') {

            // FIX: `sizeof(op_stack)` is the size of the POINTER
            // (always 4 or 8 bytes) — it can never be 0, so this could
            // never actually detect "nothing pending yet." Since main
            // zero-initializes with `Opstack op_st = {0};`, `.op` starts
            // as '\0' — that's your real sentinel to check instead:
            if (op_stack->op == '\0'){
                // TODO: still yours — record *p as pending here
            } else {
                switch (*p)   // FIX: was `switch ()` — empty parens don't
                              // compile, a switch needs an expression
                {
                    case '*': case '/': case '%':
                        if (op_stack->op == '+' || op_stack->op == '-'){

                            
                        }
                    
                }
            }

            // TODO: whichever branch above runs, you still need
            // `p++; continue;` here — right now control always falls
            // through into "Invalid character" below, even for a
            // character that WAS a valid operator.
        }

        fprintf(stderr, "Invalid character in input\n");
        return 1;
    }
    return 0;

}

int main(){
    char* expression = getExpression();

    if (expression == NULL){ return -1; };
    Stack st = {0};
    Opstack op_st = {0};

    parseExpression(expression, &st, &op_st);
    free(expression);
    return 0;
}