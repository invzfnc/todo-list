#include <iostream>

using namespace std;

void add();
void view();
void mark();
void edit();
void remove();

struct TodoItem {
    string title, description, due_date;
};

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

    return 0;
}

void add()
{

}

void view()
{

}

void mark()
{

}

void edit()
{

}

void remove()
{

}