// Logan Schneider, Aniket Sarkar, Jackson Knapp, Final Project Bidding auction system using hash tables
// Date: 10/24/2024

#include <iostream>
#include <string>
#include <vector>
#include <limits>  // Used in numeric_limits for the error handling cin.ignore lines

using namespace std;

// Define a constant maximum load for the table
const int MAX_TABLE_SIZE = 10;

// Class for items in the auction system
class Item {
public:
    string name;             
    int currentHighBid;      
    string highestBidder;    // Name of the highest bidder

    // Constructor that initializes an item name, starting bid of 0 and no bidder
    Item(string name) {
        this->name = name;
        currentHighBid = 0;
        highestBidder = "None";
    }

    // Places a bid on the item
    void bidUpdate(int bidAmount, string bidder) {
        if (bidAmount > currentHighBid) {
            currentHighBid = bidAmount;
            highestBidder = bidder;
            cout << bidder << " placed a bid of $" << bidAmount << " on " << name << endl;
        } else {
            cout << "Bid must be higher than the current bid of $" << currentHighBid << endl;
        }
    }

    // Displays an item's current highest bid and highest bidder
    void displayItem() const {
        cout << "Item: " << name << " | Highest Bid: $" << currentHighBid << " by " << highestBidder << endl;
    }
};

// Represents the auction system using a hash table with open addressing (linear probing)
class HashTable {
private:
    struct HtItem {
        string key;  // The name of the item (used as the key)
        Item* value; // The actual Item object stored in the table
    };

    vector<HtItem*> table;  // Array of HtItem pointers (representing the hash table)
    int itemCountHT;          // Total number of items in the hash table

    // Hash function to compute index based on the item's name. Distributes items across the table evenly to minimize collisions 
    int hashFunction(string name) {
        int hashValue = 0;
        // Sums the ASCII values of the characters in the name
        for (char ch : name) { // iterates through each character in 'name'
            hashValue += ch; 
        }
        return hashValue % MAX_TABLE_SIZE; // Taking the modulus with the maximum table size gives a viable index
    }

    // Linear probing function to find the index of an item
    int linearProbe(string name, int index) {
        while (table[index] != nullptr) { // While the index exists
            if (table[index]->key == name) { // If the index key is the name we are looking for
                return index; // Return this index of the item as it is found 
            }
            index = (index + 1) % MAX_TABLE_SIZE; // Move to the next index using the modulous operator *linear probing* to continue searching
        }
        return -1; // Return -1 if the item is not found and that will be the "false" value
    }

public:
    // Constructor that initializes the hash table. MAX_TABLE_SIZE is the maximum number of items that can be stored in the table and everything is initialized to nullptr 
    HashTable() : table(MAX_TABLE_SIZE, nullptr), itemCountHT(0) {}

    
// Creates and inserts an item into the hash table
    void insertItem(string name) {
        // First makes sure the table is not full
        if (itemCountHT == MAX_TABLE_SIZE) {
            cout << "Insert Error: Hash Table is full\n";
            return;
        }

        int index = hashFunction(name);  // Get the index using hashFunction

        // Linear probing to handle collisions
        while (table[index] != nullptr && table[index]->key != name) { // If the index is already occupied (not nullptr and has a key which is not this name)
            index = (index + 1) % MAX_TABLE_SIZE;  // Move to the next index using the modulous operator again. *linear probing*
        }

        // If the item already exists tell the user and return
        if (table[index] != nullptr && table[index]->key == name) {   // same logic as above; if the index is not nullptr and has the same key as the name
            cout << "Item already exists in auction." << endl;
            return;
        }
        // ****************************************************
        // Actual insert logic

        // Create a new item and insert it into the table
        Item* newItem = new Item(name);
        table[index] = new HtItem{ name, newItem };
        itemCountHT++; // increment the HTitem counter
        cout << "Item " << name << " added to auction." << endl;
    }

// Places a bid on an item in the hash table. Calls the bidUpdate function from the Item class to verify that it is a higher bid than the current highest bid
    void placeBid(string name, string bidder, int bidAmount) {
        int index = hashFunction(name);

        // Linear probing to find the item. (this is so if the original one when inputted got moved to a different index from a collision)
        index = linearProbe(name, index);

        // If the item is found, place the bid
        if (index != -1) {   // if the index is found
            table[index]->value->Item::bidUpdate(bidAmount, bidder); // The value hash table values at the index are updated with the bidUpdate function from the Item class and its associated logic
        } else {
            cout << "Item " << name << " not found in auction." << endl;
        }
    }

// Searches for an item in the hash table (what items are in the auction)
    void searchItem(string name) {
        int index = hashFunction(name);

        // Finds the item if it went through the linear probing collision handling
        index = linearProbe(name, index);

        // If the item is found, place the bid
        if (index != -1) {   // if the index is found
            cout << "Item " << name << " found in auction!" << endl;
        } else {
            cout << "Item " << name << " not found in auction." << endl;
        }
    }

// Remove an item from the hash table 
    void removeItem(string name) {
        int index = hashFunction(name);

        // Finds the item if it went through the linear probing collision handling
        index = linearProbe(name, index);

        // If the item is found, remove it
        if (index != -1) {   // if the index is found
            delete table[index]->value;  // Free memory of the Item
            delete table[index];         // Free the HtItem structure
            table[index] = nullptr;  // reset the index to nullptr
            itemCountHT--; // decrement the item counter
            cout << "Item " << name << " removed from auction." << endl;
        } else {
            cout << "Item " << name << " not found in auction." << endl;
        }
    }

// Checks if the hash table is empty True or False (Might be totally unnecessary. In the video I watched it had it and maybe it could help with debugging but I am really not sure lol)
    bool isEmpty() const {
        return itemCountHT == 0;
    }

// Displays all items in the hash table (auction)
    void displayAllItems() {
        for (int i = 0; i < MAX_TABLE_SIZE; i++) { // iterates through the entire table
            if (table[i] != nullptr) {
                table[i]->value->Item::displayItem();  // Displays the items bidder and bid amount through the displayItem function from the Item class
            }
        }
    }

// Menu for the auction system
    void auctionMenu() {
        int option;  // Variable to store the user's menu option

        do {
            // Display the menu options
            cout << "\nMenu:\n";
            cout << "1. Add an item\n";
            cout << "2. Place a bid\n";
            cout << "3. Search for an item\n";
            cout << "4. Display all items\n";
            cout << "5. Remove an item\n";
            cout << "6. Exit\n";
            cout << "Please choose an option: ";

            // Check error handling for the input option of menu
            if (!(cin >> option)) {
                cout << "Invalid input. Please enter a number between 1 and 6.\n";
                cin.clear(); // Clear the error flag if therer is any
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard an invalid input
                continue; // Restart the dowhile loop recalling the menu
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover newline
            cout << endl; // New line for formatting

            // Menu functionality
            // Made a switch statement for the menu options instead of the if else statements
            switch (option) {
                case 1: { // Add an item
                    string name;
                    cout << "Enter item name: ";
                    getline(cin, name);
                    if (name.empty()) {
                        cout << "Item name cannot be empty.\n";
                    } else {
                        insertItem(name);
                    }
                    break;
                }
                case 2: { // Place a bid
                    string itemName, bidder;
                    int bidAmount;

                    cout << "Enter item name to bid on: ";
                    getline(cin, itemName);
                    if (itemName.empty()) {
                        cout << "Item name cannot be empty.\n";
                        break;
                    }

                    cout << "Enter your name: ";
                    getline(cin, bidder);
                    if (bidder.empty()) {
                        cout << "Bidder name cannot be empty.\n";
                        break;
                    }

                    // Error handling for bid amount as this and the option need to be integers
                    cout << "Enter bid amount: ";
                    if (!(cin >> bidAmount) || bidAmount <= 0) {
                        cout << "Invalid bid amount. Please enter a positive integer.\n"; // Error message for invalid bid amount
                        cin.clear(); // Clear the error flag if there is any
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover newline
                        break;
                    }

                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover newline
                    placeBid(itemName, bidder, bidAmount); // Place the bid
                    break;
                }
                case 3: { // Search for an item
                    string name;
                    cout << "Enter item name to search for: ";
                    getline(cin, name);
                    if (name.empty()) {
                        cout << "Item name cannot be empty.\n";
                    } else {
                        searchItem(name);
                    }
                    break;
                }
                case 4: // Display all items
                    displayAllItems();
                    break;
                case 5: { // Remove an item
                    string name;
                    cout << "Enter item name to remove: ";
                    getline(cin, name);
                    if (name.empty()) {
                        cout << "Item name cannot be empty.\n";
                    } else {
                        removeItem(name);
                    }
                    break;
                }
                case 6: // Exit
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid option. Please enter a number between 1 and 6.\n";
            }
        } while (option != 6); // Repeat until the user chooses to exit
    }

// Destructor cleans up dynamically allocated memory
    ~HashTable() {
        for (int i = 0; i < MAX_TABLE_SIZE; i++) { // iterates through the entire table
            if (table[i] != nullptr) {
                delete table[i]->value;  // Free memory for each Item
                delete table[i];         // Free the HtItem structure
            }
        }
    }
};

int main() {
    HashTable testAuction;  // create an instance of the HashTable for the test auction

    // *****************************************************************
    // Test Case Examples
    // *****************************************************************

    // Check if the table is empty (should be empty at the start)
    if (testAuction.isEmpty()) {
        cout << "Auction is empty!" << endl;
    } else {
        cout << "Items are still available in auction." << endl;
    }

    // Add items
    testAuction.insertItem("Shopping Cart");
    testAuction.insertItem("Gucci Watch");
    testAuction.insertItem("Mona Lisa");

    // place bids
    testAuction.placeBid("Mona Lisa", "Alice", 10);
    testAuction.placeBid("Mona Lisa", "Bob", 150);
    testAuction.placeBid("Mona Lisa", "Charlie", 100);  // Should fail, as bid is lower than $150

    // Search for an item
    testAuction.searchItem("Mona Lisa");

    // Remove an item 
    testAuction.removeItem("Painting");

    // Check if the table is empty (should not be empty after adding items)
    if (testAuction.isEmpty()) {
        cout << "Auction is empty!" << endl;
    } else {
        cout << "Items are still available in auction." << endl;
    }

    // Display all items in the auction
    testAuction.displayAllItems();
    cout << endl;
    cout << endl;

    // *****************************************************************
    // ONLINE AUCTION MENU
    // *****************************************************************
    cout << "Welcome to the Online Auction System! Input answers to the menu below" << endl;
    HashTable Auction;  // Create the online Auction

    Auction.auctionMenu();


    return 0;
}
