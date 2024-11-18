//Jackson Knapp Group 13 mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTextEdit>
#include <QMessageBox> //library to show messages to user, enables some popup messages to be displayed
#include <iostream>
#include <sstream>

using namespace std; //******

class QTextStreamBuffer : public std::streambuf {
public:
    QTextStreamBuffer(QTextEdit* textEdit) : textEdit(textEdit) {} //initializer for QTextEdit widget ptr

protected:
    virtual int overflow(int c) override { // function called when data is put into the stream
        if (c != EOF) { //if int is not end of file/line
            currentOutput.append(QChar(c));  // Append character to the string buffer
            if (c == '\n') { //if newline,output the entire string to QTextEdit
                textEdit->append(currentOutput);
                currentOutput.clear();  // Clear the buffer for the next line
            }
        }
        return c;
    }

private:
    QTextEdit* textEdit;  // ptr to the QTextEdit widget
    QString currentOutput;  // buffer holding the output before appending to QTextEdit
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentState(ITEM_NAME), currentBidAmount(0)
{
    ui->setupUi(this);  // UI components initialized
    QTextStreamBuffer* buffer = new QTextStreamBuffer(ui->Console);  // console in original IDE appears in console on window
    cout.rdbuf(buffer);  //redirect output stream to use the buffer provided. Required to output text to QTextEdit objects in the UI
}

MainWindow::~MainWindow() //cleans up UI components when needed
{
    delete ui;  // Clean up the UI when the window is destroyed
}

void MainWindow::on_btn_add_clicked()// process of adding a new item with details
{
    currentState = ITEM_NAME; //state1, initalize vals
    currentItemName.clear();
    currentBidderName.clear();
    currentBidAmount = 0;
    getNextInput();
}

void MainWindow::getNextInput() // get inputs for object to auction
{
    bool ok;
    QString input;

    switch (currentState) {
    case ITEM_NAME: //state2, item name is taken in
        input = QInputDialog::getText(this, "Enter Item Name", "Item Name:", QLineEdit::Normal, "", &ok);
        if (ok && !input.isEmpty()) {
            currentItemName = input;
            currentState = BIDDER_NAME;
            getNextInput();
        } else {
            QMessageBox::warning(this, "Error", "Item name cannot be empty!"); // error handle1
        }
        break;

    case BIDDER_NAME: //state3, name of bidder is taken in
        input = QInputDialog::getText(this, "Enter Bidder Name", "Bidder Name:", QLineEdit::Normal, "", &ok);
        if (ok && !input.isEmpty()) {
            currentBidderName = input;
            currentState = BID_AMOUNT;
            getNextInput();
        } else {
            QMessageBox::warning(this, "Error", "Bidder name cannot be empty!");// error handle2
        }
        break;

    case BID_AMOUNT:
        currentBidAmount = QInputDialog::getInt(this, "Enter Bid Amount", "Bid Amount:", 0, 0, 1000000, 1, &ok);
        if (ok) {
            auction.insertItem(currentItemName.toStdString());
            auction.placeBid(currentItemName.toStdString(), currentBidderName.toStdString(), currentBidAmount);

            cout << "Item added and bid placed successfully!" << endl;  // confirmation printed
            currentState = COMPLETE; // process of taking in an item FULLY done
        } else {
            QMessageBox::warning(this, "Error", "Bid amount must be a valid number!");// error handle3
        }
        break;

    case COMPLETE:
        currentState = ITEM_NAME; // reset for future items to be added
        break;
    }
}

void MainWindow::on_btn_placebid_clicked() //take in details to make a bid on an item
{
    QString itemName = QInputDialog::getText(this, "Place Bid", "Enter Item Name:");
    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Item name cannot be empty!");
        return;
    }

    QString bidderName = QInputDialog::getText(this, "Place Bid", "Enter Bidder Name:");
    if (bidderName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Bidder name cannot be empty!");
        return;
    }

    bool ok;
    int bidAmount = QInputDialog::getInt(this, "Place Bid", "Enter Bid Amount:", 0, 0, 1000000000, 1, &ok); //bid amount max is arbitrarily set
    if (!ok) {
        QMessageBox::warning(this, "Error", "Bid amount must be a valid number!");
        return;
    }

    auction.placeBid(itemName.toStdString(), bidderName.toStdString(), bidAmount); //bid is placed on object. only updates if bid is higher than current bid

    cout << "Bid placed successfully!" << endl;  // console output
}

void MainWindow::on_btn_search_clicked() //see if an item is within the auction (confirmation of yes and no, don't have to see the display)
{
    QString itemName = QInputDialog::getText(this, "Search Item", "Enter Item Name:");
    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Item name cannot be empty!");
        return;
    }

    int index = auction.searchItem(itemName.toStdString()); // search must return an actual index for the item in question
    if (index != -1) {
        QMessageBox::information(this, "Found", "Item found in the auction!");
    } else {
        QMessageBox::warning(this, "Not Found", "Item not found in the auction.");
    }

    cout << "Search for item: " << itemName.toStdString() << endl;  // console print
}

void MainWindow::on_btn_display_clicked() //display all items
{
    stringstream output;
    auction.displayAllItems(output);

    cout << "Auction Items displayed!" << endl;  //console confirm

    QMessageBox::information(this, "Auction Items", QString::fromStdString(output.str()));
}

void MainWindow::on_btn_remove_clicked()
{
    QString itemName = QInputDialog::getText(this, "Remove Item", "Enter Item Name:");
    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Item name cannot be empty!");
        return;
    }

    auction.removeItem(itemName.toStdString()); //remove item from hashtable
    cout << "Item removed from the auction." << endl;  // console confirm

    QMessageBox::information(this, "Success", "Item removed from the auction.");
}

void MainWindow::on_btn_exit_clicked()
{
    QApplication::quit();  // exit the app
}
