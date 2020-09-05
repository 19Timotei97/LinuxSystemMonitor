#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay
{
  void Display (System&, int = 10);

  void DisplaySystem (System&, WINDOW*);
  
  void DisplayProcesses (std::vector<Process>&, WINDOW*, int);
  
  std::string ProgressBar (float);
};

#endif