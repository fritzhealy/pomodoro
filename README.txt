Only tested on linux (ubuntu 16.04)
compiled on my system with g++ -o pomodoro -std=c++14 pomodoro.cpp `pkg-config --cflags --libs libnotify`
need libnotify-dev from package manager
_______________________________________________
USE
pomodoro start -- runs 25 minute pomodoro timer
pomodoro start short-break -- runs 5 minute break timer
pomodoro start long-break -- runs 10 minute break timer
when running the cursor won't show
- type quit + enter to stop pomodoro
- type pause + enter to pause pomodoro (although you shouldn't)
- type resume + enter to resume pomodoro

DESCRIPTION
_______________________________________________
Pomodoro was just described to me by a friend. The
idea is to close out all your distractions during your pomodoro 
in order to get more work done.
You should take 1 pomodoro then 1 short break 3 times.
Then another pomodoro and a long break. 
Repeat for work day.
This system should notify you for all your increments
as well as count down your remaining time.