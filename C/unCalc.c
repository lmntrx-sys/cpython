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
    char* data;
    size_t size;
    size_t capacity;
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

// FIX: this whole function was modeled on the ORIGINAL calc.c's eval(),
// where `pool` was an int32_t* and pool[0] was a dedicated counter slot —
// *pool literally WAS the count, stored inside the data array itself.
// Your design already separates that out properly: stack->size IS the
// count, stack->data is just values, no slot is secretly metadata. So
// `stack[*stack - 1]` here was trying to use a real pushed VALUE (a
// double) as an array index — which doesn't even compile: you can't do
// pointer arithmetic with a double operand (`ptr + double` is a type
// error in C, not just a logic bug). Fixed to take the whole Stack* and
// use stack->size the way it's actually meant to be used:
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

// typedef struct 
// {
//     char* data;
//     size_t size;
//     size_t capacity;
// } Opstack;

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

int32_t parseExpression(char *line, Stack* stack, Opstack* op_stack)
{
    char *p = line;
    int32_t op_index = 0;
    int32_t stack_index = 0; 

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

            if (opStackEmpty(op_stack)) {
                pushOp(op_stack, *p);
            } else {
                switch (*p)
                {
                    case '*': case '/': case '%':
                        if (*op_stack->data == '+' || *op_stack->data == '-'){
                            // TODO — still open, and this is the one to
                            // think hardest about: Opstack only holds a
                            // single `char op`. Overwriting it here
                            // doesn't "defer" the pending '+'/'-' — it
                            // ERASES it. "1+2*3" will lose the '+'
                            // entirely: op becomes '*', and whatever the
                            // '+' was supposed to do never happens. Real
                            // deferral needs somewhere to hold MORE than
                            // one pending operator at once — this struct
                            // can't do that yet as written.

                            pushOp(op_stack, *p);
                            op_stack->data = p;
                            eval(stack, *op_stack->data);
                        } else {
                            popOp(op_stack);
                            eval(stack, *op_stack->data);
                            op_stack->data = p;
                        }
                    break;

                    case '+': case '-':
                        eval(stack, *op_stack->data);
                        op_stack->data = p;
                    break;

                    default:
                        eval(stack, *op_stack->data);
                        op_stack->data = p;
                }
            }

            // TODO: still missing — p++; continue; here. Without it,
            // control falls through to the error line below even after
            // successfully handling a valid operator.
            p++;
            continue;
        }

        fprintf(stderr, "Invalid character in input\n");
        return 1;
    }

    // flush the remaining operator in the op_stack
    if (!opStackEmpty(op_stack)) {
        eval(stack, *op_stack->data);
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

    // TODO: still missing — nothing prints st.data[0] (your final
    // answer, once size == 1) or checks that size actually IS 1 (catch
    // "3 4" with no operator, or "3+4 5" leaving something extra
    // behind). And st.data itself is heap memory — needs a free(st.data)
    // before main returns, once you're done reading the result out of it.

    return 0;
}