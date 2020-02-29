#include "ncurses_display.h"
#include "system.h"

#include <iostream>
#include "linux_parser.h" 

int main() {
  System system;
  for(auto p: system.Processes()){
    std::cout << p.User() << std::endl;
  }
  //NCursesDisplay::Display(system);
}