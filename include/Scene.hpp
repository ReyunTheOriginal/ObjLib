#pragma once

#include <vector>
#include "GlobalLists.hpp"
#include "GlobalTypes.hpp"
#include "Window.hpp"
#include "GameObject.hpp"
#include "Components/Transform.hpp"

namespace obj{
    struct camera;
    
    namespace Internal{
        struct component;
    }

    struct scene{
        private:
        int ID = 0;

        friend scene* CreateScene();
        friend window::~window();
        friend scene* window::SetScene(scene* SceneToSet);
        friend void window::DebugDisplay();
        friend gameObject::~gameObject();
        friend gameObject* CreateGameObject(scene* Scene, Internal::transform* Parent);
        friend void gameObject::SetScene(scene* SceneToSendTo);
        friend void Internal::transform::SetParent(Internal::transform* ParentToSet);

        std::vector<window*> Windows;
        std::vector<gameObject*> GameObjects;
        std::vector<gameObject*> ParentlessGameObjects;

        public:
        std::string Name = "Scene";

        color BackGroundColor = color::DarkGray;
        
        int GetID(){return ID;}

        std::vector<window*> GetWindows(){return Windows;}
        std::vector<gameObject*> GetGameObjects(){
            return GameObjects;
        }
        std::vector<gameObject*> GetParentlessGameObjects(){
            return ParentlessGameObjects;
        }
        
        scene(){}
        ~scene();
    };

    scene* CreateScene();
}