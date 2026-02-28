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
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);

    PrintList(head);
    int val = 0;
    head = insertAtFront(head, val);
    PrintList(head);
    return 0;
}
