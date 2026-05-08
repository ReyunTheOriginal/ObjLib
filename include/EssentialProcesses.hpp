#pragma once

#include <vector>
#include <variant>

const float PixelsPerUnit = 64;

namespace obj{
    // Forward declarations
    struct gameObject;
    struct font;
    struct sprite;
    struct component;

    namespace UI{
        struct canvas;
        struct screenObject;
    }
    
    extern bool ObjMessages;
    int  Init(); //Start up all necessary code to prepare the program
    void Apply(); //Apply things like Phyiscs, Hiararchies, and Delete things
    void Update(); //update processes like Input, Time, and FPS
    void Quit(); //safely quit the entire program

    //IT IS HIGHLY RECOMMENDED TO NEVER USE THIS, USE Destroy() instead
    void DestroyInstantly(auto& Instance){
        delete Instance;
    }

    using AnyType = std::variant<
        component*,
        gameObject*,
        font*,
        UI::canvas*,
        UI::screenObject*,
        sprite*
    >;
    
    extern std::vector<AnyType*> QueuedForDestruction;
    
    template<typename T>
    void Destroy(T& Instance){
        QueuedForDestruction.push_back(new AnyType(Instance));
    }
}
