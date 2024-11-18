//Jackson Knapp Group 13 Hashtable.h
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Item { // rep. all items in auction
public:
    string name;
    int currentHighBid;
    string highestBidder;

    Item(string name) : name(name), currentHighBid(0), highestBidder("") {} // initialize the item with a name

    void bidUpdate(int bidAmount, string bidder) { //update bid if new bid is larger than last
        if (bidAmount > currentHighBid) {
            currentHighBid = bidAmount;
            highestBidder = bidder;
        }
    }

    void displayItem() const { //display an item's details in auction
        cout << "Item: " << name << ", Current High Bid: " << currentHighBid
                  << ", Highest Bidder: " << highestBidder << endl;
    }
};

class HashTable { // manage items in auction as seen in original code
private:
    struct HtItem {
        string key; // key-item pair (item name and Item object are linked)
        Item* value;
    };

    vector<HtItem*> table; // vector of ptrs to HtItem structs (bulk of hashtable)
    int itemCountHT; //items in table

    int hashFunction(string name) { // generate an index for an item based on name
        int hash = 0;
        for (char ch : name) {
            hash = hash * 31 + ch; // hash function to diff.
        }
        return hash % table.size();  // index returned within table
    }

    int linearProbe(std::string name, int index) { // resolve hash collisions
        return (index + 1) % table.size();  // try next index in case of collision
    }

public:
    HashTable() : table(100, nullptr), itemCountHT(0) {} // create hash table (vector of ptrs to HtItem, 100 slots each with a nullptr (arbitrary slot #), then keeps track of how many items in table

    ~HashTable() { // clean up memory when window is closed
        for (auto& item : table) { //range based for loop, iterates over each element in table vector. auto& -> deduces type of Item and access items
            delete item->value;
            delete item;
        }
    }

    void insertItem(string name) { //insert item into table
        int index = hashFunction(name);  // hash function to get index
        table[index] = new HtItem{ name, new Item(name) };  // create new item, insert at index
        itemCountHT++; //increase number of items in table
    }

    void placeBid(string name, string bidder, int bidAmount) { // place a bid on item
        int index = searchItem(name);
        if (index != -1) {
            Item* item = table[index]->value;
            item->bidUpdate(bidAmount, bidder);  // update the bid if the amount is higher
        } else {
            cerr << "Item not found!" << endl;  // error handling, happens right as things happen
        }
    }

    int searchItem(string name) { // search for an item in the hash table
        int index = hashFunction(name);  // hash the item name to get index
        while (table[index] != nullptr) {
            if (table[index]->key == name) {
                return index;  // index returned where item is
            }
            index = linearProbe(name, index);  // in case of collision
        }
        return -1;  // not found
    }

    void displayAllItems(ostream& os) { // display all items in the table
        for (const auto& item : table) {
            if (item != nullptr) {
                item->value->displayItem();
                os << endl;
            }
        }
    }

    void removeItem(string name) { // remove an item from the hash table
        int index = searchItem(name);
        if (index != -1) {
            delete table[index]->value;  // Item object deleted
            delete table[index];         // HtItem struct deleted
            table[index] = nullptr;      // nullptr set, deletes item
        } else {
            cerr << "Item not found!" << endl;  // Item not found
        }
    }
};

#endif // HASHTABLE_H
