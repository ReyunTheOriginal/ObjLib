#pragma once

#include <vector>
#include "GlobalLists.hpp"
#include "GlobalTypes.hpp"
#include "Components/Transform.hpp"

namespace obj{
    struct window;
    struct camera;
    struct gameObject;
    
    namespace Internal{
        struct component;
    }

    struct scene{
        private:
        int ID = 0;
        friend scene* CreateScene();

        public:
        std::vector<window*> Windows;
        std::vector<gameObject*> GameObjects;

        Internal::transform* ObjectParent = nullptr;
        

        color BackGroundColor = color::DarkGray;
        
        int GetID(){return ID;}
        
        scene(){ObjectParent = new Internal::transform(nullptr);}
        ~scene();
    };

    scene* CreateScene();
}