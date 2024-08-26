#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class InsufficientBalanceException : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient balance.";
    }
};

class InvalidBidException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid bid amount.";
    }
};

class AuctionClosedException : public exception {
public:
    const char* what() const noexcept override {
        return "Auction is closed.";
    }
};

class Participant {
protected:
    string name;
    string phoneNumber;
    string accountDetails;
    string gender;
    double balance;

public:
    Participant(const string& name, double balance, const string& phoneNumber, const string& accountDetails, const string& gender)
        : name(name), balance(balance), phoneNumber(phoneNumber), accountDetails(accountDetails), gender(gender) {}

    Participant() : name(""), balance(0), phoneNumber(""), accountDetails(""), gender("") {}

    string getName() const {
        return name;
    }

    double getBalance() const {
        return balance;
    }

    void deductBalance(double amount) {
        if (amount > balance) {
            throw InsufficientBalanceException();
        }
        balance -= amount;
    }

    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Account Details: " << accountDetails << endl;
        cout << "Gender: " << gender << endl;
        cout << "Balance: " << balance << endl;
    }
};

class Auction {
protected:
    string title;
    double startingBid;
    double currentBid;
    string winner;
    bool isOpen;

public:
    Auction(const string& title, double startingBid)
        : title(title), startingBid(startingBid), currentBid(startingBid), isOpen(true) {}

    Auction() : title(""), startingBid(0), currentBid(0), isOpen(true) {}

    virtual ~Auction() {}

    void placeBid(Participant& bidder, double amount) {
        if (!isOpen) {
            throw AuctionClosedException();
        }

        if (amount <= currentBid) {
            throw InvalidBidException();
        }

        if (amount > bidder.getBalance()) {
            throw InsufficientBalanceException();
        }

        bidder.deductBalance(amount);
        currentBid = amount;
        winner = bidder.getName();
        cout << "Bid placed successfully!" << endl;
    }

    void closeAuction() {
        isOpen = false;
        if (winner.empty()) {
            cout << "Auction closed. No winner." << endl;
        } else {
            cout << "Auction closed. Winner: " << winner << endl;
        }
    }

    bool isAuctionOpen() const {
        return isOpen;
    }

    virtual void displayInfo() const = 0;

    string getTitle() const {
        return title;
    }

    string getWinner() const {
        return winner;
    }

    double getCurrentBid() const {
        return currentBid;
    }
};

class ItemAuction : public Auction {
private:
    string description;

public:
    ItemAuction(const string& title, double startingBid, const string& description)
        : Auction(title, startingBid), description(description) {}

    ItemAuction() : Auction(), description("") {}

    void displayInfo() const override {
        cout << "Item Auction: " << getTitle() << endl;
        cout << "Description: " << description << endl;
        cout << "Starting Bid: " << startingBid << endl;
        cout << "Current Bid: " << getCurrentBid() << endl;
        cout << "Auction Status: " << (isAuctionOpen() ? "Open" : "Closed") << endl;
    }
};

class ArtAuction : public Auction {
private:
    string artist;

public:
    ArtAuction(const string& title, double startingBid, const string& artist)
        : Auction(title, startingBid), artist(artist) {}

    ArtAuction() : Auction(), artist("") {}

    void displayInfo() const override {
        cout << "Art Auction: " << getTitle() << endl;
        cout << "Artist: " << artist << endl;
        cout << "Starting Bid: " << startingBid << endl;
        cout << "Current Bid: " << getCurrentBid() << endl;
        cout << "Auction Status: " << (isAuctionOpen() ? "Open" : "Closed") << endl;
    }
};

class AuctionHouse {
private:
    static const int MAX_AUCTIONS = 10;
    static const int MAX_PARTICIPANTS = 10;

    Auction* auctions[MAX_AUCTIONS];
    Participant* participants[MAX_PARTICIPANTS];
    int numAuctions;
    int numParticipants;

public:
    AuctionHouse() : numAuctions(0), numParticipants(0) {}

    void addAuction(Auction* auction) {
        if (numAuctions >= MAX_AUCTIONS) {
            cout << "Cannot add more auctions. Auction house is full." << endl;
            return;
        }

        auctions[numAuctions] = auction;
        ++numAuctions;
    }

    void addParticipant(Participant* participant) {
        if (numParticipants >= MAX_PARTICIPANTS) {
            cout << "Cannot add more participants. Auction house is full." << endl;
            return;
        }

        participants[numParticipants] = participant;
        ++numParticipants;
    }

    void displayAuctionsInfo() const {
        cout << "Auction House: Available Auctions" << endl;
        for (int i = 0; i < numAuctions; ++i) {
            auctions[i]->displayInfo();
            cout << "---------------------------" << endl;
        }
    }

    void displayParticipantsInfo() const {
        cout << "Auction House: Participants" << endl;
        for (int i = 0; i < numParticipants; ++i) {
            participants[i]->displayInfo();
            cout << "---------------------------" << endl;
        }
    }

    void displayEndOfDaySummary() const {
        cout << "End of Day Summary:" << endl;

        cout << "Participants: " << endl;
        for (int i = 0; i < numParticipants; ++i) {
            participants[i]->displayInfo();
            cout << "---------------------------" << endl;
        }

        cout << "Auctions: " << endl;
        for (int i = 0; i < numAuctions; ++i) {
            auctions[i]->displayInfo();
            cout << "---------------------------" << endl;
            cout << "Bids: " << endl;
            for (int j = 0; j < numParticipants; ++j) {
                Participant* participant = participants[j];
                if (participant->getBalance() >= auctions[i]->getCurrentBid()) {
                    cout << "Participant: " << participant->getName() << endl;
                    cout << "Bid Amount: " << auctions[i]->getCurrentBid() << endl;
                    cout << "---------------------------" << endl;
                }
            }
            cout << endl;
        }

        cout << "Auction Winners and Items Won: " << endl;
        for (int i = 0; i < numAuctions; ++i) {
            Auction* auction = auctions[i];
            if (!auction->isAuctionOpen()) {
                cout << "Auction: " << auction->getTitle() << endl;
                if (!auction->getWinner().empty()) {
                    cout << "Winner: " << auction->getWinner() << endl;
                    for (int j = 0; j < numParticipants; ++j) {
                        if (participants[j]->getName() == auction->getWinner()) {
                            cout << "Participant " << participants[j]->getName() << " won the item." << endl;
                            break;
                        }
                    }
                } else {
                    cout << "No winner for this auction." << endl;
                }
                cout << "---------------------------" << endl;
            }
        }
    }
};
class PatternPrinter {
private:
    string message;
    public:
    PatternPrinter(string& msg) : message(msg) {}

    // Function to print the pattern
    void printPattern() {
        int width = message.length() + 4;
        int height = 5;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    // Print asterisks for the border
                    cout << "*";
                }
                else if (i == height / 2 && j == (width - message.length()) / 2) {
                    // Print the message at the center of the pattern
                    cout << message;
                    j += message.length() - 1;
                }
                else {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }
};


int main() {
    string welcomeMessage = "Welcome to Auction House";
    PatternPrinter printer(welcomeMessage);
    printer.printPattern();
    AuctionHouse auctionHouse;

    // Creating Participant objects
    Participant* participants[10];
    participants[0] = new Participant("John", 1000, "1234567890", "A123456789", "Male");
    participants[1] = new Participant("Alice", 2000, "9876543210", "B987654321", "Female");
    participants[2] = new Participant("Robert", 0, "7894561230", "C789456123", "Male");
    participants[3] = new Participant("Tom", 1500, "2345678901", "D234567890", "Male");
    participants[4] = new Participant("Emma", 1800, "8765432109", "E876543210", "Female");
    participants[5] = new Participant("Alex", 1200, "3456789012", "F345678901", "Male");
    participants[6] = new Participant("Sarah", 2500, "7654321098", "G765432109", "Female");
    participants[7] = new Participant("Mike", 3000, "4567890123", "H456789012", "Male");
    participants[8] = new Participant("Emily", 1400, "6543210987", "I654321098", "Female");
    participants[9] = new Participant("Daniel", 1600, "5678901234", "J567890123", "Male");

    // Adding Participants to the AuctionHouse
    for (int i = 0; i < 10; ++i) {
        auctionHouse.addParticipant(participants[i]);
    }

    // Creating Auction objects
    Auction* auctions[2];
    auctions[0] = new ItemAuction("Laptop", 500, "Brand new laptop");
    auctions[1] = new ArtAuction("Painting", 2000, "Renowned artist");

    // Adding Auctions to the AuctionHouse
    for (int i = 0; i < 2; ++i) {
        auctionHouse.addAuction(auctions[i]);
    }

    int choice;
    do {
        cout << "\n----- Menu -----" << endl;
        cout << "1. Place Bid" << endl;
        cout << "2. Close Auction" << endl;
        cout << "3. Display Auctions" << endl;
        cout << "4. Display Participants" << endl;
        cout << "5. End of Day Summary" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            int auctionIndex, participantIndex;
            double amount;
            cout << "Enter the auction index (0 or 1): ";
            cin >> auctionIndex;
            cout << "Enter the participant index (0 to 9): ";
            cin >> participantIndex;
            cout << "Enter the bid amount: ";
            cin >> amount;
            try {
                auctions[auctionIndex]->placeBid(*participants[participantIndex], amount);
            } catch (InsufficientBalanceException& e) {
                cout << "Bid placement failed: " << e.what() << endl;
            } catch (InvalidBidException& e) {
                cout << "Bid placement failed: " << e.what() << endl;
            } catch (AuctionClosedException& e) {
                cout << "Bid placement failed: " << e.what() << endl;
            } catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            break;
        case 2:
            cout << "Enter the auction index (0 or 1) to close: ";
            cin >> auctionIndex;
            auctions[auctionIndex]->closeAuction();
            break;
        case 3:
            auctionHouse.displayAuctionsInfo();
            break;
        case 4:
            auctionHouse.displayParticipantsInfo();
            break;
        case 5:
            auctionHouse.displayEndOfDaySummary();
            break;
        case 6:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);

    // Clean up dynamically allocated memory
    for (int i = 0; i < 10; ++i) {
        delete participants[i];
    }
    for (int i = 0; i < 2; ++i) {
        delete auctions[i];
    }

    return 0;
}
