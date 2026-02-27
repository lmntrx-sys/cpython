#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct node{
    int data;
    struct node *next;
};
struct node *head = NULL; 

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

void reverseListRecursive(struct node **head){
    if (*head == NULL || (*head)->next == NULL)
        return;
    
    struct node *temp = (*head)->next;
    reverseListRecursive(&temp);
    
    temp->next = *head;
    (*head)->next = NULL;
    *head = temp;
}

void PrintList(struct node *node){
    struct node *temp = node;

    while (temp != NULL){
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
    
}

void freeList(struct node *head){
    struct node *temp;
    
    while (head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(){
    struct node *head = NULL;
    
    addFirst(&head, 10);
    addFirst(&head, 20);
    addFirst(&head, 30);
    PrintList(head);

    reverseListRecursive(&head);
    PrintList(head);

    freeList(head);

    return 0;
}

