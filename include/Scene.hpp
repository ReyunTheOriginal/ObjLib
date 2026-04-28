#pragma once

#include <unordered_set>
#include "GlobalLists.hpp"
#include "GlobalTypes.hpp"
#include "GameObject.hpp"

namespace obj{
    struct window;
    struct camera;

    struct scene{
        private:
        int ID = 0;
        friend scene* CreateScene();

        public:
        std::unordered_set<window*> Windows;
        std::unordered_set<gameObject*> GameObjects;

        color BackGroundColor = color::DarkGray;

        camera* ActiveCamera = nullptr;
        
        int GetID(){return ID;}
        
        scene(){};
        ~scene();
    };

    scene* CreateScene();
    void DestroyScene(scene* Scene);
}