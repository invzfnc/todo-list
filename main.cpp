#include <iostream>
#include <vector>

// Platform specific commands
#ifdef __MINGW32__ // Windows
    #define CLEAR_SCREEN "cls"
#else // Linux or MacOS
    #define CLEAR_SCREEN "clear"
#endif

using namespace std;

void add();
void view();
void mark();
void edit();
void remove();

struct TodoItem {
    string title;
    string description;
    string due_date;
};

vector<TodoItem> todo_items;

int main() 
{
    int command;

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
        if (command >= 1 && command <= 6)
            break;
        cout << "Please enter a number within range 1-6: ";
    }

    cout << endl;
    system(CLEAR_SCREEN);
    switch (command)
    {
        case 1:
            add();
            break;
        case 2:
            view();
            break;
        case 3:
            mark();
            break;
        case 4:
            edit();
            break;
        case 5:
            remove();
            break;
    }

    return 0;
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