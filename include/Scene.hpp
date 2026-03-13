#pragma once

#include <unordered_set>
#include "GlobalLists.hpp"
#include "GlobalTypes.hpp"
#include "Gameobject.hpp"

namespace obj{
    struct window;
    struct camera;

    struct scene{
        private:
        camera* ActiveCamera = nullptr;

        friend scene* CreateScene();

        public:
        std::unordered_set<window*> Windows;
        std::unordered_set<gameobject*> GameObjects;

        color BackGroundColor = color::DarkGray;

        camera* GetActiveCamera(){return ActiveCamera;}
        camera* SetActiveCamera(camera* Camera);

        scene(){};
        ~scene();
    };

    scene* CreateScene();
    void DestroyScene(scene* Scene);
}