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
        std::vector<gameObject*> ActiveGameObjects;
        std::vector<gameObject*> InactiveGameObjects;
        std::vector<gameObject*> ParentlessGameObjects;

        public:
        std::string Name = "Scene";

        color BackGroundColor = color::DarkGray;
        
        int GetID(){return ID;}

        virtual void OnSceneSet(); // runs when the scene is set into a window
        virtual void OnSceneUnSet();// runs when a window running this scene runs another one
        virtual void OnSceneLoad();// runs when runs when the scene gets set in a window when it previously had no window
        virtual void OnSceneUnLoad(); // runs when a scene loses all windows running it

        std::vector<window*> GetWindows(){return Windows;}
        std::vector<gameObject*> GetGameObjects(){return GameObjects;}
        std::vector<gameObject*> GetParentlessGameObjects(){return ParentlessGameObjects;}
        std::vector<gameObject*> GetActiveGameObjects(){return ActiveGameObjects;}
        std::vector<gameObject*> GetInactiveGameObjects(){return InactiveGameObjects;}

        void PushActiveGameObjects(gameObject* obj){ActiveGameObjects.push_back(obj);}
        void PushInactiveGameObjects(gameObject* obj){InactiveGameObjects.push_back(obj);}
        void ClearActiveGameObjects(){ActiveGameObjects.clear();}
        void ClearInactiveGameObjects(){InactiveGameObjects.clear();}
        
        scene(){}
        ~scene();
    };

    scene* CreateScene();
}