#pragma once

namespace obj{
    extern bool ObjMessages;
    int  Init(); //Start up all necessary code to prepare the program
    void Update(); //update processes like physics, positions, and math
    void Quit(); //safely quit the entire program
}