#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *next;
};
struct head *head = NULL; 

void addFirst(struct node **head, int val){
    // 1. Declare a head pointer and make it as NULL.
    struct node *New_node = malloc(sizeof(struct node));
    // 2. Create a new node with the given data.
    New_node->data = val;
    // 3. Make the new node points to the head node.
    New_node->next = *head;
    // 4. Finally, make the new node as the head node.
    *head = New_node;
}

int main(){
    return 0;
}
