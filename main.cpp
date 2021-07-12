#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>

enum Command
{
    BEGIN = 1,
    STATUS = 2,
    END = 3,
    EXIT = 4,
    NOT_DEFINED = 99
};

struct Task
{
    bool isOngoing;
    std::time_t startTime;
    std::time_t endTime;
    std::string taskTitle;

    explicit Task(std::string& taskTitle)
    {
        this->taskTitle = taskTitle;
        startTime = std::time(nullptr);
        endTime = -1;
        isOngoing = true;
        std::tm* local = std::localtime(&startTime);
        std::cout << "\t";
        std::cout << std::put_time(local, "%d.%m.%Y %H:%M:%S");
        std::cout << " [task is started]: " << taskTitle << "\n";
    }

    void endTask()
    {
        if (isOngoing)
        {
            endTime = std::time(nullptr);
            isOngoing = false;
            std::tm* local = std::localtime(&endTime);
            std::cout << "\t";
            std::cout << std::put_time(local, "%d.%m.%Y %H:%M:%S");
            std::cout << "[task is finished]: " << taskTitle << "\n";
        }
        else
        {
            std::cout << "\t";
            std::cout << "Task \"" << taskTitle << "\" is already ended.\n";
        }
    }

    double getDuration() const
    {
        if (!isOngoing)
        {
            return std::difftime(endTime, startTime);
        }
        else
        {
            std::cout << "The task is ongoing.\n";
            return (std::difftime(std::time(nullptr), startTime));
        }
    }

    void printStatus()
    {
        std::cout << "\t";
        std::tm* startLocal = std::localtime(&startTime);
        std::cout << std::put_time(startLocal, "%d.%m.%Y %H:%M:%S") << " - ";
        if (!isOngoing)
        {
            std::tm* endLocal = std::localtime(&endTime);
            std::cout << std::put_time(endLocal, "%d.%m.%Y %H:%M:%S");
            double duration = getDuration();
            int hoursDuration = (int)duration / 3600;
            int minutesDuration = ((int)duration - hoursDuration * 3600) / 60;
            int secondsDuration = ((int)duration - hoursDuration * 3600 - minutesDuration * 60);
            std::cout << " (" << hoursDuration << ":" << minutesDuration << ":" << secondsDuration << ") - ";
        }
        else
        {
            std::cout << " ongoing - ";
        }
        std::cout << taskTitle << "\n";
    }
};

/**
 * @function Get the user input from keyboard.
 * @param [in] input String
 * @return Command
 * @throws invalid_argument exception.
 */
Command getCommandFromInput(std::string& input)
{
    Command cmd = NOT_DEFINED;
    try
    {
        cmd = (Command)std::stoi(input);
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << "Can't understand your input." << "\n";
    }
    return cmd;
}

int main() {
    Command cmd;
    std::vector<Task> tasks;
    do {
        std::cout << "Please enter the command (1-4)\n";
        std::cout << "1 - to begin new task;\n";
        std::cout << "2 - to print status;\n";
        std::cout << "3 - to end current task;\n";
        std::cout << "4 - to exit;\n";
        std::cout << "Your command: ";
        std::string input;
        std::getline(std::cin, input);
        cmd = getCommandFromInput(input);

        std::string taskTitle;
        Task* task;
        switch (cmd)
        {
            case EXIT:
                std::cout << "Exiting the program...\n";
                break;
            case BEGIN:
                if (!tasks.empty()) tasks.back().endTask();
                std::cout << "Please enter the task title: ";
                std::getline(std::cin, taskTitle);
                task = new Task(taskTitle);
                tasks.push_back(*task);
                delete task;
                break;
            case END:
                if (!tasks.empty()) tasks.back().endTask();
                break;
            case STATUS:
                std::cout << "Current status:\n";
                for (Task t : tasks) t.printStatus();
                break;
            default:
                std::cerr << "Unknown command. Try again.\n";
        }
    } while (cmd != EXIT);

    std::cout << "Program is finished\n";

    return 0;
}