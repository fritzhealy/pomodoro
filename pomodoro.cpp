#include <libnotify/notify.h>
#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>
#include <termios.h>
using namespace std;

const int POMODORO_MINUTES = 25;
const int POMODORO_SECONDS = 25*60; //25 minutes in seconds
const int POMODORO_SHORT_BREAK_MINUTES = 5;
const int POMODORO_SHORT_BREAK_SECONDS = 5*60; //25 minutes in seconds
const int POMODORO_LONG_BREAK_MINUTES = 10;
const int POMODORO_LONG_BREAK_SECONDS = 10*60; //25 minutes in seconds
const chrono::milliseconds MINUTE(1000*60); //unit for 1 minute in milliseconds
const chrono::milliseconds SECOND(1000); //unit for 1 minute in milliseconds
atomic<bool> done(false);
atomic<bool> paused(false);

void countdown_pomodoro(int argc, char* argv[]){
    time_t t = time(0);
    int seconds = 0;
    int minutes = 0;
    if(argc==3){
        if((string)argv[2]=="long-break"){
            cout << "starting long break" << endl;
            seconds = POMODORO_LONG_BREAK_SECONDS;
            minutes = POMODORO_LONG_BREAK_MINUTES;
        } else if((string)argv[2]=="short-break"){
            cout << "starting short break" << endl;
            seconds = POMODORO_SHORT_BREAK_SECONDS;
            minutes = POMODORO_SHORT_BREAK_MINUTES;
        } else {
            cerr << "unrecognized command" << endl;
            return;
        }
    } else {
        cout << "Starting pomodoro!"<<endl;
        seconds = POMODORO_SECONDS;
        minutes = POMODORO_MINUTES;
    }
    t += seconds;
    struct tm *  then = localtime(&t);
    int hours = then->tm_hour;
    if(then->tm_hour>12){
        hours = then->tm_hour%12;
    }
    cout << "Ending at: "<< hours <<":"<< then->tm_min <<endl<<endl;
    for(int i = minutes; i>0;i--){ //minute loop
        for(int j = 60; j > 0 ; j--){ //second loop
            if(paused){ //every second check if paused
                cout << "PAUSING TIMER" <<endl;
                do { //endless loop till unpause
                    this_thread::sleep_for(SECOND);
                } while(paused && !done);
                cout << "RESUMING TIMER" <<endl<<endl;
            }
            if(done){ //if done then we're over
               cout << "QUITING TIMER" <<endl<<"press any key to exit"<<endl<<endl;
                return;
            }
            cout << "\e[A" <<"\rTime remaining: "<< i << "                           " << endl;
            this_thread::sleep_for(SECOND); //else sleep for second
        }
    }
    notify_init("Pomodoro");
    NotifyNotification* n = notify_notification_new ("Pomodoro", 
        "++++++++++++FINISHED++++++++++++",
        0);
    notify_notification_set_timeout(n, 1000); // 10 seconds

    if (!notify_notification_show(n, 0)) 
    {
        cerr << "couldn't notify you, but it's done" << endl;
        return ;
    }
    cout << "\e[A" << "\r+++++++++++++++FINISHED+++++++++++++++++" << endl << "press enter to exit"<<endl;
    done = true;
}

int main(int argc, char *argv[]){
    if(argc<2){
        cerr << "Need more args" << endl;
        return 1;
    }
    if(((string)argv[1])=="start"){
        thread th = thread(countdown_pomodoro,argc,argv);
        // thanks to/from https://stackoverflow.com/questions/13694170/how-do-i-hide-user-input-with-cin-in-c
        termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        /* we want to disable echo */
        tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        do {
            string input;
            cin >> input;
            if(input == "quit"){
                done = true;
            } else if(input =="pause"){
                paused = true;
            } else if(input =="resume"){
                paused = false;
            }
        } while(!done);

        // thanks to/from https://stackoverflow.com/questions/13694170/how-do-i-hide-user-input-with-cin-in-c
        tcgetattr(STDIN_FILENO, &tty);
        /* we want to reenable echo */
        tty.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);

        th.join();
    } else if(((string)argv[1])=="end"){
        cout << "ending" << endl;
    } else {
        cerr << "unrecognized command" << endl;
        return 1;
    }
    return 0;
}