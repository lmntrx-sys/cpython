#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct 
{
    double*  data;
    size_t   size;
    size_t   capacity;
} Stack;

typedef struct 
{
    char*   data;
    size_t  size;
    size_t  capacity;
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

double* eval(Stack* stack, char op) {

    if (stack->size < 2) {
        fprintf(stderr, "Not enough values on stack to perform operation\n");
        exit(1);
    }

    double *left  = &stack->data[stack->size - 2];
    double *right = &stack->data[stack->size - 1];

    if (op == '+') { *left += *right; }
    else if (op == '-') { *left -= *right; }
    else if (op == '/') { *left /= *right; }
    else if (op == '*') { *left *= *right; }

    else if (op == '%') { *left = (double)((long)*left % (long)*right); }

    // TODO — still open, not touched: nothing here checks stack->size
    // is at least 2 before this runs. Left/right above will read out of
    // bounds (and *left is written back into memory you don't own) if
    // eval() is ever called with fewer than 2 values pushed. You had a
    // `stack->size < 2` guard in an earlier version — it's missing here
    // and needs to go back in, before this function is called.

    stack->size -= 1;
    return stack->data;
}

void push(Stack* stack, double op){
    
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
    stack->data[stack->size++] = op;
}

void pushOp(Opstack* op_stack, char op){
    if (op_stack->size == op_stack->capacity){
        op_stack->capacity = op_stack->capacity ? op_stack->capacity * 2 : 8;
        char* new_data = realloc(op_stack->data, op_stack->capacity * sizeof(*op_stack->data));
        if (new_data == NULL){
            fprintf(stderr, "Out of memory! Sorry");
            free(op_stack->data);
            exit(1);
        }
        op_stack->data = new_data;
    }
    op_stack->data[op_stack->size++] = op;
}

char popOp(Opstack* op_stack){
    return op_stack->data[--op_stack->size];   
}

char peekOp(Opstack* op_stack){
    return op_stack->data[op_stack->size - 1]; 
}

bool opStackEmpty(Opstack* op_stack){
    return op_stack->size == 0;
}

int32_t precedence(char op) {
    switch (op) {
        case '-':
        case '+':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
}

int32_t parseExpression(char *line, Stack* stack, Opstack* op_stack)
{
    char      *p = line;
    int32_t   op_index = 0;
    int32_t   stack_index = 0; 

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

            while (!opStackEmpty(op_stack) && precedence(peekOp(op_stack)) >= precedence(*p)) {

                // if what is at the stack has higher preceedence than the current operator, pop it and evaluate it
                eval(stack, popOp(op_stack));
                

            }
            // Push what is currently in the operator stack to the operator stack
            pushOp(op_stack, *p);


            p++;
            continue;
        }

        fprintf(stderr, "Invalid character in input\n");
        return 1;
    }

    // flush the remaining operator in the op_stack
    if (!opStackEmpty(op_stack)) {

        while (!opStackEmpty(op_stack))
        {
            /* code */
            eval(stack, popOp(op_stack));
        }
        
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

    printf("Result: %f\n", st.data[0]);
    // free(st.data);
    // free(op_st.data);

    return 0;
}