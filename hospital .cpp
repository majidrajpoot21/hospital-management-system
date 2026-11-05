#include <iostream>
#include <limits>
#include <string>

using namespace std;

struct PatientNode {
    int id;
    string name;
    string ward;
    PatientNode* left;
    PatientNode* right;

    PatientNode(int patientId, const string& patientName, const string& patientWard)
        : id(patientId), name(patientName), ward(patientWard), left(nullptr), right(nullptr) {}
};

class PatientRoster {
private:
    PatientNode* head;
    PatientNode* tail;
    PatientNode* current;

public:
    PatientRoster() : head(nullptr), tail(nullptr), current(nullptr) {}

    PatientRoster(const PatientRoster&) = delete;
    PatientRoster& operator=(const PatientRoster&) = delete;

    ~PatientRoster() {
        PatientNode* node = head;
        while (node != nullptr) {
            PatientNode* next = node->right;
            delete node;
            node = next;
        }
    }

    void addPatientAtEnd(int id, const string& name, const string& ward) {
        PatientNode* node = new PatientNode(id, name, ward);
        if (tail == nullptr) {
            head = tail = current = node;
            return;
        }

        node->left = tail;
        tail->right = node;
        tail = node;
    }

    bool addPatientAfterCurrent(int id, const string& name, const string& ward) {
        if (current == nullptr) {
            return false;
        }

        PatientNode* node = new PatientNode(id, name, ward);
        node->left = current;
        node->right = current->right;

        if (current->right != nullptr) {
            current->right->left = node;
        } else {
            tail = node;
        }

        current->right = node;
        return true;
    }

    bool moveNext() {
        if (current == nullptr || current->right == nullptr) {
            return false;
        }
        current = current->right;
        return true;
    }

    bool movePrevious() {
        if (current == nullptr || current->left == nullptr) {
            return false;
        }
        current = current->left;
        return true;
    }

    bool removeCurrent() {
        if (current == nullptr) {
            return false;
        }

        PatientNode* removed = current;
        PatientNode* previous = removed->left;
        PatientNode* next = removed->right;

        if (previous != nullptr) {
            previous->right = next;
        } else {
            head = next;
        }

        if (next != nullptr) {
            next->left = previous;
        } else {
            tail = previous;
        }

        current = (next != nullptr) ? next : previous;
        delete removed;
        return true;
    }

    void printCurrent() const {
        if (current == nullptr) {
            cout << "There is no selected patient.\n";
            return;
        }
        printPatient(current);
    }

    void printForward() const {
        if (head == nullptr) {
            cout << "The patient roster is empty.\n";
            return;
        }

        cout << "\nHospital patient roster (first to last):\n";
        for (PatientNode* node = head; node != nullptr; node = node->right) {
            cout << (node == current ? "-> " : "   ");
            printPatient(node);
        }
    }

    void printBackward() const {
        if (tail == nullptr) {
            cout << "The patient roster is empty.\n";
            return;
        }

        cout << "\nHospital patient roster (last to first):\n";
        for (PatientNode* node = tail; node != nullptr; node = node->left) {
            cout << (node == current ? "-> " : "   ");
            printPatient(node);
        }
    }

private:
    static void printPatient(const PatientNode* patient) {
        cout << "Patient ID: " << patient->id
             << " | Name: " << patient->name
             << " | Ward: " << patient->ward << '\n';
    }
};

bool readInteger(const string& prompt, int& value) {
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        if (cin.eof()) {
            return false;
        }
        cout << "Please enter a valid whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool readNonEmptyText(const string& prompt, string& value) {
    while (true) {
        cout << prompt;
        if (!getline(cin, value)) {
            return false;
        }
        if (value.find_first_not_of(" \t") != string::npos) {
            return true;
        }
        cout << "This field cannot be empty.\n";
    }
}

int main() {
    PatientRoster roster;
    roster.addPatientAtEnd(201, "Alex Morgan", "Cardiology");
    roster.addPatientAtEnd(202, "Jamie Lee", "Orthopedics");
    roster.addPatientAtEnd(203, "Taylor Brown", "Neurology");

    int choice = 0;
    while (true) {
        cout << "\nHospital Patient Roster\n"
             << "1. Display Selected Patient\n"
             << "2. Move to Next Patient\n"
             << "3. Move to Previous Patient\n"
             << "4. Add Patient at the End\n"
             << "5. Add Patient After Selected Patient\n"
             << "6. Remove Selected Patient\n"
             << "7. Display Roster from First to Last\n"
             << "8. Display Roster from Last to First\n"
             << "9. Exit\n";

        if (!readInteger("Choose an option: ", choice)) {
            cout << '\n';
            break;
        }

        int id = 0;
        string name;
        string ward;
        switch (choice) {
        case 1:
            roster.printCurrent();
            break;
        case 2:
            cout << (roster.moveNext() ? "Moved to the next patient.\n"
                                       : "There is no next patient.\n");
            break;
        case 3:
            cout << (roster.movePrevious() ? "Moved to the previous patient.\n"
                                           : "There is no previous patient.\n");
            break;
        case 4:
            if (!readInteger("Enter patient ID: ", id) ||
                !readNonEmptyText("Enter patient name: ", name) ||
                !readNonEmptyText("Enter ward or department: ", ward)) {
                return 0;
            }
            roster.addPatientAtEnd(id, name, ward);
            cout << "Patient added to the end of the roster.\n";
            break;
        case 5:
            if (!readInteger("Enter patient ID: ", id) ||
                !readNonEmptyText("Enter patient name: ", name) ||
                !readNonEmptyText("Enter ward or department: ", ward)) {
                return 0;
            }
            if (roster.addPatientAfterCurrent(id, name, ward)) {
                cout << "Patient added after the selected patient.\n";
            } else {
                cout << "There is no selected patient. Add a patient at the end first.\n";
            }
            break;
        case 6:
            cout << (roster.removeCurrent() ? "Selected patient removed.\n"
                                            : "There is no selected patient to remove.\n");
            break;
        case 7:
            roster.printForward();
            break;
        case 8:
            roster.printBackward();
            break;
        case 9:
            cout << "Exiting the hospital roster.\n";
            return 0;
        default:
            cout << "Choose a number from 1 to 9.\n";
        }
    }

    return 0;
}