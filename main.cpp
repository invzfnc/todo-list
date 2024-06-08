#include <iostream>
#include <fstream>
#include <sstream>
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
// Main command functions
void add();
void view();
void mark();
void edit();
void remove();

// Tools
int get_item_position(string);
bool is_leap_year(int);
bool is_valid_date(string);
string get_date_input();

// File IO functions
void save_data();
TodoItems retrieve_data();

// Global storage object
TodoItems todo_items;

int main() 
{
    char command;
    // Retrieve saved data from previous run, if any
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

        // Map commands to main function calls
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
                // Save all data before program termination
                save_data();
                return 0;
        }

        // Allow user to read the output before the program continues
        cout << "\nPress enter to continue ...";
        cin.ignore(INT_MAX, '\n');

        // Remove clutter from previous output to improve readability
        // and to focus on the next instructions
        system(CLEAR_SCREEN);
    }
}

void add()
{
    // Create object to store new task details
    TodoItem task;

    cout << "Enter task details (Empty to abort operation): " << endl;

    // Title of new task
    cout << "Title: ";
    getline(cin, task.title);

    // Allow user to back out of operation if they no longer 
    // wish to continue
    if (task.title.empty())
    {
        cout << "Abort task." << endl;
        return;
    }

    // Description of new task
    cout << "Description: ";
    getline(cin, task.description);

    // Due date of new task
    cout << "Due Date (YYYY-MM-DD): ";
    task.due_date = get_date_input();

    // Add new task to todo_items vector
    todo_items.push_back(task);

    cout << "Task added successfully" << endl;
}

void view()
{
    // Set counter
    // Will be printed onto screen beside the task titles
    int count = 1;

    // Print title of output
    cout << "All Tasks" << endl;

    // Iterate over all items in todo_items vector
    // and print their details onto screen
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
    // Prompt user for position of task in todo_item
    // vector to be marked
    int item_position = get_item_position("mark");

    // Status code for operation abortion
    if (item_position == -1)
        return;

    // Check if item is already marked before
    if (todo_items[item_position].completed)
        cout << "Task is already marked as completed." << endl;
    else
    {
        // Change the state of .completed attribute
        todo_items[item_position].completed = true;
        cout << "Task marked as completed." << endl;
    }
}

void edit()
{
    // Prompt user for position of task in todo_item
    // vector to be edited
    int item_position = get_item_position("edit");

    // Status code for operation abortion
    if (item_position == -1)
        return;
        
    // Get selected task from todo_items vector
    auto task = todo_items[item_position];
    
    cout << "Enter task details (Empty to abort operation): " << endl;

    // Print initial title of task for user reference
    cout << "Title " << "(was " << task.title <<  "): ";
    getline(cin, task.title);

    // Allow user to back out of operation if they no longer 
    // wish to continue
    if (task.title.empty())
    {
        cout << "Abort task." << endl;
        return;
    }

    // Get updated decsription for selected task
    cout << "Description: " << "(was " << task.description << "): ";
    getline(cin, task.description);

    // Get updated due date for selected task
    cout << "Due Date (YYYY-MM-DD, was " << task.due_date << "): ";

    // Get and validate date input
    task.due_date = get_date_input();

    // Replace original task details with updated task details
    todo_items[item_position] = task;

    cout << "Task edited successfully" << endl;
}

void remove()
{
    // Prompt user for position of task in todo_item
    // vector to be removed
    int item_position = get_item_position("remove");

    // Status code for operation abortion
    if (item_position == -1)
        return;

    // Character to hold user's decision ([y]es or [n]o)
    char choice;

    // Prompts user to enter choice
    cout << "Confirm to delete \"" << todo_items[item_position].title << "\"? [y/n]: ";
    cin >> choice;

    // Consider both lower and upper case inputs
    // Proceed as long as the choice starts with
    // the 'y' character
    if (choice == 'y' || choice == 'Y')
    {
        // Delete the selected task from todo_items vector
        todo_items.erase(todo_items.begin() + item_position);
        cout << "Task deleted successfully." << endl;
    }
    // Confirmation failed
    else
    {
        cout << "Delete operation cancelled." << endl;
    }

    // Clear buffer for next input to prevent input issues
    // (Ignore leftover input from this point)
    cin.ignore(INT_MAX, '\n');
}

int get_item_position(string action)
{
    // Hold user input for task position in todo_items vector
    int input_num;

    while (true)
    {
        // Print customised prompt
        cout << "Enter task number to " << action << " (0 to abort operation): ";

        cin >> input_num;
        // Clear buffer to prevent input issues
        cin.ignore(INT_MAX, '\n');

        // If read successfully
        if (cin)
        {
            // User chose to abort operation
            if (input_num == 0)
            {
                cout << "Abort task." << endl;

                // Return status code for operation abortion
                return -1;
            }

            // Input number is in valid range
            if (input_num >= 1 && input_num <= todo_items.size())
                break; // Move on to next step
    
            cout << "Task number is out of range." << endl;

            // Continue prompting for input
            continue;
        }
        // User entered non-numeric values
        else 
        {
            // Clear error flag of cin
            // to enable next input
            cin.clear();

            // Clear buffer
            cin.ignore(INT_MAX, '\n');

            // Inform user about incorrect input
            cout << "What you've entered is not a number." << endl;

            // Continue prompting for input
            continue;
        }
    }

    // Adjust offset by 1 before returning
    // (User input starts at 1 while
    // position in vectors starts at 0)
    return input_num - 1;
}

bool is_valid_date(string date_str)
{
    int day, month, year;

    // Extract day, month, year from date_str
    int matches = sscanf(date_str.c_str(), "%d /%d /%d", &day, &month, &year);

    // Proceed only if there are exactly three matches
    if (matches != 3)
        return false;

    // Conditions: 
    // 1. Year should not be a negative number
    // 2. Month should be in range [1, 12]
    // 3. Date should be over 0
    // 4. Date should not exceed maximum days in that particular month
    // 5. Rule 3, with an exception for second month if it is a leap year (28 -> 29)

    if (year < 1) {
        return false;
    }

    if (month < 1 || month > 12) {
        return false;
    }

    // Array to match month with respective days
    const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1)
        return false;

    if (month == 2 && is_leap_year(year))
    {
        if (day > 29)
            return false;
    }
    else
    {
        if (day > days_in_month[month - 1])
            return false;
    }

    // Valid if all conditions are fulfilled
    return true;
}


bool is_leap_year(int year)
{
    // Condition 1:
    // Leap year if perfectly divisible by 400
    // OR
    // Condition 2:
    // Leap year if not divisible by 100, AND
    // Divisible by 4

    if (year % 400 == 0)
        return true;

    else if (year % 100 != 0 && year % 4 == 0)
        return true;

    else
        return false;
}

string get_date_input()
{
    string date_str;
    int day, month, year;
    stringstream result;

    // Prompt until receives valid input
    while (true)
    {
        // Get the whole line to parse
        getline(cin, date_str);

        // Input has passed validation checking
        if (is_valid_date(date_str))
            break; // Continue onto next step

        // Prompt for reenter
        cout << "Please enter a valid date: ";
    }

    // Arguments breakdown for sscanf function call
    // 1. Convert date_str from std::string to char* (C style string).
    // 2. C style conversion specification: %d, stands for digits/numbers.
    //    Second argument stands for "Match a digit, followed by zero or
    //    more spaces, a slash character, space character(s), a single
    //    slash, and a digit". This allows extraction of wanted information
    //    from a string.
    // 3. 3 pointers to 3 variables, holding the extracted day, month, and
    //    year. Extracted digits will be assigned to these variables by
    //    sscanf function.
    //
    sscanf(date_str.c_str(), "%d /%d /%d", &day, &month, &year);

    // Perform string concatenation for final result
    result << day << "/" << month << "/" << year;

    // Convert to C++ style std::string and return
    return result.str();
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