#pragma once

#include <unordered_set>
#include <vector>
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
        std::vector<window*> Windows;
        std::vector<gameObject*> GameObjects;

        std::vector<Internal::component*> RenderableComponenets;

        color BackGroundColor = color::DarkGray;

        camera* ActiveCamera = nullptr;
        
        int GetID(){return ID;}
        
        scene(){};
        ~scene();
    };

    scene* CreateScene();
    void DestroyScene(scene* Scene);
}