#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int NUM_COACHES = 4;
const string coachNames[NUM_COACHES] = {
    "AC Sleeper", "Non-AC Sleeper", "AC Seater", "Non-AC Seater"
};

struct Coach {
    int totalSeats;
    int availableSeats;
    int price;
};

struct Train {
    int number;
    string name;
    string source;
    string destination;
    string departure;
    Coach coaches[NUM_COACHES];
    Train* next;
};

Train* head = nullptr;

Train* findTrain(int number) {
    Train* current = head;
    while (current != nullptr) {
        if (current->number == number) return current;
        current = current->next;
    }
    return nullptr;
}

void addTrain(int number, string name, string source, string destination,
              string departure, int seats[], int prices[]) {
    if (findTrain(number) != nullptr) {
        cout << "A train with number " << number << " already exists.\n";
        return;
    }

    Train* newTrain = new Train;
    newTrain->number = number;
    newTrain->name = name;
    newTrain->source = source;
    newTrain->destination = destination;
    newTrain->departure = departure;
    for (int i = 0; i < NUM_COACHES; i++) {
        newTrain->coaches[i].totalSeats = seats[i];
        newTrain->coaches[i].availableSeats = seats[i];
        newTrain->coaches[i].price = prices[i];
    }
    newTrain->next = nullptr;

    if (head == nullptr) {
        head = newTrain;
    } else {
        Train* last = head;
        while (last->next != nullptr) last = last->next;
        last->next = newTrain;
    }
    cout << "Train " << number << " added successfully.\n";
}

void printTrain(Train* t) {
    cout << "\nTrain " << t->number << " - " << t->name << "\n";
    cout << "  Route     : " << t->source << " -> " << t->destination << "\n";
    cout << "  Departure : " << t->departure << "\n";
    for (int i = 0; i < NUM_COACHES; i++) {
        if (t->coaches[i].totalSeats > 0) {
            cout << "  " << coachNames[i] << " : Rs " << t->coaches[i].price
                 << " per seat, " << t->coaches[i].availableSeats << "/"
                 << t->coaches[i].totalSeats << " seats free\n";
        }
    }
}

void displayAllTrains() {
    if (head == nullptr) {
        cout << "No trains available.\n";
        return;
    }
    for (Train* current = head; current != nullptr; current = current->next) {
        printTrain(current);
    }
}

void searchByNumber(int number) {
    Train* t = findTrain(number);
    if (t == nullptr) cout << "Train " << number << " not found.\n";
    else printTrain(t);
}

void searchByRoute(string source, string destination) {
    bool found = false;
    for (Train* current = head; current != nullptr; current = current->next) {
        if (current->source == source && current->destination == destination) {
            printTrain(current);
            found = true;
        }
    }
    if (!found) cout << "No trains found from " << source << " to " << destination << ".\n";
}

void updateTrain(int number, string name, string source,
                 string destination, string departure) {
    Train* t = findTrain(number);
    if (t == nullptr) {
        cout << "Train " << number << " not found.\n";
        return;
    }
    t->name = name;
    t->source = source;
    t->destination = destination;
    t->departure = departure;
    cout << "Train " << number << " updated.\n";
}

void deleteTrain(int number) {
    if (head == nullptr) {
        cout << "The list is empty.\n";
        return;
    }

    if (head->number == number) {
        Train* toDelete = head;
        head = head->next;
        delete toDelete;
        cout << "Train " << number << " deleted.\n";
        return;
    }

    Train* previous = head;
    Train* current = head->next;
    while (current != nullptr && current->number != number) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Train " << number << " not found.\n";
        return;
    }

    previous->next = current->next;
    delete current;
    cout << "Train " << number << " deleted.\n";
}

void reverseList() {
    Train* previous = nullptr;
    Train* current = head;
    while (current != nullptr) {
        Train* nextNode = current->next;
        current->next = previous;
        previous = current;
        current = nextNode;
    }
    head = previous;
    cout << "List order reversed.\n";
}

int countTrains() {
    int count = 0;
    for (Train* current = head; current != nullptr; current = current->next) count++;
    return count;
}

void freeList() {
    while (head != nullptr) {
        Train* toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

void showCoachOptions(Train* t) {
    cout << "\nCoach types on train " << t->number << ":\n";
    for (int i = 0; i < NUM_COACHES; i++) {
        cout << "  " << (i + 1) << ". " << coachNames[i] << " - ";
        if (t->coaches[i].totalSeats == 0) {
            cout << "Not available\n";
        } else {
            cout << "Rs " << t->coaches[i].price << " per seat, "
                 << t->coaches[i].availableSeats << " seats left\n";
        }
    }
}

void bookSeats(Train* t, int coachIndex, int seatsWanted) {
    Coach& coach = t->coaches[coachIndex];

    if (seatsWanted <= 0 || seatsWanted > coach.availableSeats) {
        cout << "Cannot book " << seatsWanted << " seat(s). Free seats: "
             << coach.availableSeats << "\n";
        return;
    }

    coach.availableSeats -= seatsWanted;

    cout << "\n--- BOOKING CONFIRMED ---\n";
    cout << "Train      : " << t->number << " (" << t->name << ")\n";
    cout << "Route      : " << t->source << " -> " << t->destination << "\n";
    cout << "Coach      : " << coachNames[coachIndex] << "\n";
    cout << "Seats      : " << seatsWanted << "\n";
    cout << "Price/seat : Rs " << coach.price << "\n";
    cout << "TOTAL FARE : Rs " << seatsWanted * coach.price << "\n";
}

void cancelSeats(Train* t, int coachIndex, int seatsToCancel) {
    Coach& coach = t->coaches[coachIndex];
    int bookedSeats = coach.totalSeats - coach.availableSeats;

    if (seatsToCancel <= 0 || seatsToCancel > bookedSeats) {
        cout << "Invalid number. Only " << bookedSeats
             << " seat(s) are booked in this coach.\n";
        return;
    }

    coach.availableSeats += seatsToCancel;
    cout << seatsToCancel << " seat(s) cancelled in " << coachNames[coachIndex]
         << ". Refund: Rs " << seatsToCancel * coach.price << "\n";
}

int readInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(string prompt) {
    string text;
    cout << prompt;
    getline(cin, text);
    return text;
}

int readCoachChoice() {
    int choice = readInt("Select coach type (1-4): ");
    if (choice < 1 || choice > NUM_COACHES) {
        cout << "Invalid coach type.\n";
        return -1;
    }
    return choice - 1;
}

void menuAddTrain() {
    int number = readInt("Train number: ");
    string name = readText("Train name: ");
    string source = readText("Source station: ");
    string destination = readText("Destination station: ");
    string departure = readText("Departure time (HH:MM): ");

    int seats[NUM_COACHES];
    int prices[NUM_COACHES];
    bool hasAnyCoach = false;

    cout << "Enter seats and price for each coach type (0 seats = not available)\n";
    for (int i = 0; i < NUM_COACHES; i++) {
        seats[i] = readInt("  " + coachNames[i] + " - total seats: ");
        prices[i] = 0;
        if (seats[i] > 0) {
            prices[i] = readInt("  " + coachNames[i] + " - price per seat (Rs): ");
            hasAnyCoach = true;
        } else {
            seats[i] = 0;
        }
    }

    if (!hasAnyCoach) {
        cout << "At least one coach type must have seats.\n";
        return;
    }
    addTrain(number, name, source, destination, departure, seats, prices);
}

void menuUpdateTrain() {
    int number = readInt("Train number to update: ");
    string name = readText("New name: ");
    string source = readText("New source: ");
    string destination = readText("New destination: ");
    string departure = readText("New departure time: ");
    updateTrain(number, name, source, destination, departure);
}

void menuBookSeats() {
    int number = readInt("Train number: ");
    Train* t = findTrain(number);
    if (t == nullptr) {
        cout << "Train " << number << " not found.\n";
        return;
    }

    showCoachOptions(t);
    int coachIndex = readCoachChoice();
    if (coachIndex == -1) return;

    if (t->coaches[coachIndex].totalSeats == 0) {
        cout << coachNames[coachIndex] << " is not available on this train.\n";
        return;
    }
    int seatsWanted = readInt("Number of seats to book: ");
    bookSeats(t, coachIndex, seatsWanted);
}

void menuCancelSeats() {
    int number = readInt("Train number: ");
    Train* t = findTrain(number);
    if (t == nullptr) {
        cout << "Train " << number << " not found.\n";
        return;
    }

    showCoachOptions(t);
    int coachIndex = readCoachChoice();
    if (coachIndex == -1) return;

    int seatsToCancel = readInt("Number of seats to cancel: ");
    cancelSeats(t, coachIndex, seatsToCancel);
}

void printMenu() {
    cout << "\n===== TRAIN MANAGEMENT SYSTEM =====\n";
    cout << " 1. Add train\n";
    cout << " 2. Display all trains\n";
    cout << " 3. Search by train number\n";
    cout << " 4. Search by route\n";
    cout << " 5. Update train details\n";
    cout << " 6. Delete train\n";
    cout << " 7. Book seats\n";
    cout << " 8. Cancel seats\n";
    cout << " 9. Reverse list order\n";
    cout << "10. Count trains\n";
    cout << " 0. Exit\n";
}

int main() {
    int choice;

    do {
        printMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:  menuAddTrain(); break;
            case 2:  displayAllTrains(); break;
            case 3:  searchByNumber(readInt("Train number: ")); break;
            case 4: {
                string source = readText("Source station: ");
                string destination = readText("Destination station: ");
                searchByRoute(source, destination);
                break;
            }
            case 5:  menuUpdateTrain(); break;
            case 6:  deleteTrain(readInt("Train number to delete: ")); break;
            case 7:  menuBookSeats(); break;
            case 8:  menuCancelSeats(); break;
            case 9:  reverseList(); break;
            case 10: cout << "Total trains: " << countTrains() << "\n"; break;
            case 0:  cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    freeList();
    return 0;
}