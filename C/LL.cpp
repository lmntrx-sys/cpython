#include <bits/stdc++.h>

using namespace std;

class Node {
    public:
        int data;
        Node* next;

        Node(int val){
            data = val;
            next = nullptr;
        }
        
};

Node* insertAtFront(Node* head, int val){

    Node* new_node = new Node(val);
    new_node->next = head;
    head = new_node;
    return head;
};

Node* insertAfter(Node* head, int key, int new_data){
    // Set the curr Node
    Node* curr = head;
    // Treverse the entire List
    while (curr != nullptr){
        if (curr->data == key){
            break;
        }
        curr = curr->next;
    }


    if (curr == nullptr){
        return head;
    }

    // Allocate space for the new node
    Node* new_node = new Node(new_data);
    // set the pointer of the new node to the pointer of the given node
    new_node->next = curr->next;
    // set the pointer of the given node to the new node
    curr->next = new_node;
    // return the head
    return head;

}

void PrintList(Node* head){
    Node* curr = head;
    while (curr != NULL){
        cout << " -> " << curr->data;
        curr = curr->next;
    }
    cout << endl;
}

int main(){
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(4);
    head->next->next->next = new Node(5);

    cout << "Initial LL \n";
    PrintList(head);
    
    cout << "Insertion at front\n";
    int val = 0;
    head = insertAtFront(head, val);
    PrintList(head);

    cout << "Insertion at a specific point\n";
    int key = 2, new_data = 3;
    head = insertAfter(head, key, new_data);
    PrintList(head);
    return 0;
}
