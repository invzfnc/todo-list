#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <regex>

using namespace std;

// Platform specific commands
#ifdef __MINGW32__ // Windows
    #define CLEAR_SCREEN "cls"
#else // Linux or MacOS
    #define CLEAR_SCREEN "clear"
#endif

// Path to save file
#define DATA_PATH "./save.csv"

// Single item unit
struct TodoItem {
    string title;
    string description;
    string due_date;
};

// List containing item units
typedef vector<TodoItem> TodoItems;

// Function declarations
// Commands functions
void add();
void view();
void mark();
void edit();
void remove();

// File IO functions
void save_data();
TodoItems retrieve_data();

// Global storage object
TodoItems todo_items;

int main() 
{
    char command;

    while (true)
    {
        // Instructions
        cout << "1. Add Task" << endl;
        cout << "2. View Tasks" << endl;
        cout << "3. Mark Task as Completed" << endl;
        cout << "4. Edit Task" << endl;
        cout << "5. Delete Task" << endl;
        cout << "6. Exit" << endl << endl;

        cout << "Enter a number 1-6: ";

        while (true)
        {
            cin >> command;
            cin.ignore(INT_MAX, '\n'); // Flush stdin stream
            if (command >= '1' && command <= '6')
                break;
            cout << "Invalid input. Please enter a number within range 1-6: ";
        }

        cout << endl;

        system(CLEAR_SCREEN);

        switch (command)
        {
            case '1':
                add();
                break;
            case '2':
                view();
                break;
            case '3':
                mark();
                break;
            case '4':
                edit();
                break;
            case '5':
                remove();
                break;
            case '6':
                save_data();
                return 0;
        }
    }
}

void add()
{
    cout << "Add" << endl;
}

void view()
{
    cout << "View" << endl;

}

void mark()
{
    cout << "Mark" << endl;

}

void edit()
{
    cout << "Edit" << endl;

}

void remove()
{
    cout << "Remove" << endl;

}

void save_data()
{
    ofstream fout;

    fout.open(DATA_PATH, ios::out | ios::app);

    for (const auto &item : todo_items)
    {
        fout << "\"" << item.title << "\","
            << "\"" << item.description << "\","
            << "\"" << item.due_date << "\""
            << endl;
    }

    fout.close();
}

TodoItems retrieve_data()
{
    TodoItem item;
    TodoItems items;

    ifstream file(DATA_PATH);
    string line;

    regex pattern("\"([^\"]*)\",\"([^\"]*)\",\"([^\"]*)\"");
    smatch matches;

    while (getline(file, line))
    {
        regex_search(line, matches, pattern);
        item.title = matches.str(1);
        item.description = matches.str(2);
        item.due_date = matches.str(3);
        items.push_back(item);
    }

    file.close();

    return items;
}