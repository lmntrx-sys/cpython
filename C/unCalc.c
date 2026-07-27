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

// FIX: this took `Stack stack` by value. C passes structs by copy — any
// change made to `stack` inside this function was invisible to main() the
// moment parseExpression returned. On top of that, the very next line
// declared a *second*, completely separate `Stack st = {0}` and used that
// instead — so the parameter wasn't just ineffective, it was fully ignored.
// Two independent Stacks existed: the one main() built (permanently empty),
// and this local one (built correctly, then thrown away when the function
// returned — its heap-allocated `.data` leaking, unreachable forever).
// A pointer fixes both problems at once: no copy, no shadow variable.
void parseExpression(char *line, Stack* stack)
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

            if (stack->size == stack->capacity)
            {
                stack->capacity = stack->capacity ? stack->capacity * 2 : 8;
                stack->data = realloc(stack->data, stack->capacity * sizeof(*stack->data));
            }

            stack->data[stack->size++] = value;
            // this push is correct — growable, bounds-respecting. nice.

            p = endptr;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%') {
            // if (stack->size < 2) {
            //     fprintf(stderr, "Input malformed\n");
            //     fflush(stderr);
            //     // FIX: originally fell through to the pops below even
            //     // after printing the error. stack->size can be 0 or 1
            //     // here — `--stack->size` on an already-0 size_t wraps to
            //     // SIZE_MAX (unsigned, can't go negative), and the next
            //     // line would read stack->data[SIZE_MAX] — wildly out of
            //     // bounds. Your check was the right idea; it just needed
            //     // to actually stop execution once it fired.
            
            // }

            // TODO — you're here: `op` tells you which operation to run
            // on `left` and `right`, but nothing does that yet, and
            // nothing pushes a result back onto the stack. Without a push
            // here, every operator call only removes values — the stack
            // will end up empty (or underflow) instead of holding a
            // running result. (The push you need is the same shape as
            // the one in the number branch above — same capacity-doubling
            // check and everything. Might be worth factoring that into a
            // push(Stack*, double) helper you call from both spots.)

            double right = stack->data[--stack->size];
            double left = stack->data[--stack->size];
            double result = 0.0;

            switch (*p)
            {
                case '+': result = left + right; break;
                case '-': result = left - right; break;
                case '*': result = left * right; break;
                case '/':
                    if (right == 0.0) {
                        fprintf(stderr, "Division by zero not allowed\n");
                        fflush(stderr);
                        return;
                    }
                    result = left / right;
                    break;
                case '%':
                    result = left - (int)(left / right) * right;
                    break;
            }

            if (stack->size == stack->capacity) {
                stack->capacity = stack->capacity ? stack->capacity * 2 : 8;
                stack->data = realloc(stack->data, stack->capacity * sizeof(*stack->data));
            }
            stack->data[stack->size++] = result;
            p++;
            continue;
        }

        fprintf(stderr, "Invalid character in input\n");
        fflush(stderr);
        return;
    }
}

int main(){
    char* expression = getExpression();

    if (expression == NULL){ return -1; };
    Stack st = {0};

    parseExpression(expression, &st);
    // FIX: was `parseExpression(expression, st)` — now that the function
    // takes a Stack*, this needs `&st` so it operates on main's real
    // stack instead of a copy that then gets thrown away.

    // TODO: once operators push results correctly, st.size should end up
    // 1 — that value is your answer. Decide what to do if it isn't (e.g.
    // "3 4" with no operator, or "3 4 + 5" with a leftover value), print
    // the result, and since st.data was realloc'd on the heap, it needs a
    // free(st.data) somewhere before main returns — nothing does that yet.

    free(expression);
    return 0;
}