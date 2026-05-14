#include <iostream>
#include <vector>
#include <list>
#include <functional>

int findValue(int hashTable)
{
    return 0;
}

int main()
{
    /* {1,2,3} calculate the position to insert a value through the modulo operator 
    e.g value = 4, position = 4 % 3 = 1 where 3 is the size of the hash table. This means that value 4 will be inserted at index 1 in the hash table.
    */
    // Create a hash table with 3 buckets
    std::vector<std::list<int>> hashTable(6);
    for (int i = 0; i < hashTable.size(); i++) {
       std::cout<<"Bucket "<<i<< ": ";
       for (auto& val : hashTable[i]) {
           std::cout<<val<< " ";
       }
       std::cout<<std::endl;
    }

    // Insert values into the hash table
    std::vector<int> values = {11, 12, 73, 4, 35, 68};

    for (int value : values) {
        int index = value % hashTable.size();
        hashTable[index].push_back(value);
    }


    for (int i = 0; i < hashTable.size(); i++)
    {
        std::cout<<"Bucket "<<i<<":";
        for (auto& val : hashTable[i])
        {
            std::cout<<val<<" ";
        }

        std::cout<<std::endl;
    }

    return 0;
}