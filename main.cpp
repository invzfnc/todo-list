#include <iostream>
#include <fstream>
#include <climits>
#include <cstdio>
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
    bool completed = false;
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

// Tools
int get_item_position(string);

// File IO functions
void save_data();
TodoItems retrieve_data();

// Global storage object
TodoItems todo_items;

int main() 
{
    char command;
    todo_items = retrieve_data();

    // Clear screen on first run
    system(CLEAR_SCREEN);

    while (true)
    {
        // Show instructions
        cout << "-To Do List-" << endl;
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
            /* Flush stdin stream to remove input "residue".
               Only first character will be read, the rest 
               of the line will be discarded. */
            cin.ignore(INT_MAX, '\n');
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
                // Save all data before terminating program
                save_data();
                return 0;
        }

        cout << "\nPress enter to continue ...";
        cin.ignore(INT_MAX, '\n');

        system(CLEAR_SCREEN);
    }
}

void add()
{
    TodoItem task;

    cout << "Enter task details (Empty to abort operation): " << endl;

    cout << "Title: ";
    getline(cin, task.title);
    if (task.title.empty())
    {
        cout << "Abort task." << endl;
        return;
    }
    cout << "Description: ";
    getline(cin, task.description);
    cout << "Due Date (YYYY-MM-DD): ";
    getline(cin, task.due_date);

    todo_items.push_back(task);

    cout << "Task added successfully" << endl;
}

void view()
{
    int count = 1;

    cout << "All Tasks" << endl;
    for (const auto &item : todo_items)
    {
        cout << endl;
        printf("%-3d", count++);
        cout << "Title: " << item.title << endl;
        cout << "   Desc: " << item.description << endl;
        cout << "   Due Date: " << item.due_date << endl;
        cout << "   Completed: " << (item.completed ? "Yes" : "No") << endl;
    }
}

void mark()
{
    int item_position = get_item_position("mark");

    if (item_position == -1) // Operation aborted
        return;

    if (todo_items[item_position].completed)
        cout << "Task is already marked as completed." << endl;
    else
    {
        todo_items[item_position].completed = true;
        cout << "Task marked as completed." << endl;
    }
}

void edit()
{
    int item_position = get_item_position("edit");

    if (item_position == -1) // Operation aborted
        return;
        
    auto task = todo_items[item_position];
    
    cout << "Enter task details (Empty to abort operation): " << endl;

    cout << "Title " << "(was " << task.title <<  "): ";
    getline(cin, task.title);
    if (task.title.empty())
    {
        cout << "Abort task." << endl;
        return;
    }
    cout << "Description: " << "(was " << task.description << "): ";
    getline(cin, task.description);
    cout << "Due Date (YYYY-MM-DD, was " << task.due_date << "): ";
    getline(cin, task.due_date);

    todo_items[item_position] = task;

    cout << "Task edited successfully" << endl;
}

void remove()
{
    cout << "Remove" << endl;

}

int get_item_position(string action)
{
    int input_num;

    while (true)
    {
        cout << "Enter task number to " << action << " (0 to abort operation): ";

        cin >> input_num;
        cin.ignore(INT_MAX, '\n');

        if (cin)
        {
            if (input_num == 0)
            {
                cout << "Abort task." << endl;
                return -1;
            }

            if (input_num >= 1 && input_num <= todo_items.size())
                break;
    
            cout << "Task number is out of range." << endl;
            continue;
        }
        else 
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "What you've entered is not a number." << endl;
            continue;
        }
    }

    return input_num - 1;
}

void save_data()
{
    ofstream fout;

    fout.open(DATA_PATH, ios::out);

    for (const auto &item : todo_items)
    {
        fout << "\"" << item.title << "\","
            << "\"" << item.description << "\","
            << "\"" << item.due_date << "\","
            << "\"" << item.completed<< "\""
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

    regex pattern("\"([^\"]*)\",\"([^\"]*)\",\"([^\"]*)\",\"([^\"]*)\"");
    smatch matches;

    while (getline(file, line))
    {
        regex_search(line, matches, pattern);
        item.title = matches.str(1);
        item.description = matches.str(2);
        item.due_date = matches.str(3);
        
        item.completed = matches.str(4) == "1" ? true : false;

        items.push_back(item);
    }

    file.close();

    return items;
}