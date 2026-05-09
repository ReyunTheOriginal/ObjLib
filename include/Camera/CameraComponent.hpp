#pragma once

namespace obj{
    struct camera;
    struct window;

    struct cameraComponent{
        private:
        camera* Camera = nullptr;

        public:
        bool Enabled = true;
        bool DidInit = false;

        camera* GetCamera(){return Camera;}

        virtual void Run() {} //any updating that needs to be done
        virtual void Draw(window* Window) {} //any rendering
        virtual void DebugDraw(window* Window) {} //any rendering in the window debug mode
        virtual void Init() {} //any init that needs to be ran once
        virtual void OnDestroy(){}

        void CStart(camera* cam){
            Camera = cam;
        }
        ~cameraComponent(){
            OnDestroy();
        }; //destructor
    };

}