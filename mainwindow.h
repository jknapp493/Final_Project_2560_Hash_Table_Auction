//Jackson Knapp Group 13 mainwindow.h
#ifndef MAINWINDOW_H // "include guards" stating that this ONE instance of mainwindow.h should be used
#define MAINWINDOW_H

#include <QMainWindow> // create the actual windows of QT
#include <QString> // manipulate strings in QT code and ui
#include <QInputDialog>// create dialog requests (enter nums, names, etc.)
#include "HashTable.h" // hash table header

QT_BEGIN_NAMESPACE // BEGIN and END NAMESPACEs are used to define and enclose namespaces. These macros ensure all code is compatible with each other (within QT)
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // called to allow system to enable signals to widgets and slots to different functions. Basically, required for things to actually happen

public:
    explicit MainWindow(QWidget *parent = nullptr); // constructor for mainWindow class (initializes), nullptr makes the parent widget top-level (nothing else to have prioirty)
    ~MainWindow(); // destructor for MainWindow if it's destroyed to free up resources. EVERYTHING is destroyed

private slots:
    void on_btn_add_clicked(); // slot for adding items
    void on_btn_placebid_clicked(); // slot for placing a bid
    void on_btn_search_clicked(); // slot for searching an item
    void on_btn_display_clicked(); // slot for displaying all items
    void on_btn_remove_clicked(); // slot for removing an item
    void on_btn_exit_clicked(); // slot for exiting the application

private:
    void getNextInput();            // method for inputs in a sequence

    enum InputState { // define set of named consts that represent different stages of process
        ITEM_NAME,
        BIDDER_NAME,
        BID_AMOUNT,
        COMPLETE
    };

    Ui::MainWindow *ui;
    HashTable auction;  // hash table to store items

    InputState currentState; // required "states" when making an item for the auction, amkes it easy to check states to see if errors occur
    QString currentItemName;
    QString currentBidderName;
    int currentBidAmount;
};

#endif // ends the MAINWINDOW_H "include guard" designated space

