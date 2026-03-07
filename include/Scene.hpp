#pragma once

#include <unordered_map>
#include "GlobalLists.hpp"
#include "Gameobject.hpp"
#include "GlobalTypes.hpp"

namespace obj{
    struct window;

    struct scene{
        window* Window = nullptr;
        int GID = 0;
        std::unordered_map<int, gameobject*> GameObjects;
        color BackGroundColor = color::black;


        scene(){};

        ~scene(){
            for (auto obj : GameObjects){
                if (obj.second)DestroyGameObject(obj.second);
            }
        }
    };

    scene* CreateScene();
    void DestroyScene(scene* Scene);
}