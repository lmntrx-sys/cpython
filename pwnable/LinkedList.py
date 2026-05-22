class Node:
    def __init__(self, val):
        self.val = val
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    def insert(self, val):
        new_node = Node(val)
        new_node.next = self.head
        self.head = new_node

    def printList(self):
        current = self.head
        while current:
            print(current.val, end=' -> ')
            current = current.next
        print("None")

    def insertAfter(self, prev_node, val):
        curr = self.head
        while curr:
            if curr.val == prev_node:
                break
            curr = curr.next
            
        if not prev_node:
            return None
        
        newNode = Node(val)
        newNode.next = prev_node.next


    def reverse(self):
        prev = None
        current = self.head
        while current:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node
        self.head = prev


ll = LinkedList()
ll.insert(10)
ll.insert(20)
ll.insert(30)
print("Linked List:")

ll.printList()        

print("reversed")
ll.reverse()
ll.printList()