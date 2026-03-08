#pragma once

#include <unordered_set>
#include "GlobalLists.hpp"
#include "Gameobject.hpp"
#include "GlobalTypes.hpp"

namespace obj{
    struct window;

    struct scene{
        std::unordered_set<window*> Windows;
        int GID = 0;
        std::unordered_set<gameobject*> GameObjects;
        color BackGroundColor = color::black;


        scene(){};

        ~scene(){
            for (auto obj : GameObjects){
                if (obj)DestroyGameObject(obj);
            }
        }
    };

    scene* CreateScene();
    void DestroyScene(scene* Scene);
}