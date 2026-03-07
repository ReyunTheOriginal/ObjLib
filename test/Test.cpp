#include "../include/Core.hpp"


int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    obj::window* Win = obj::CreateWindow("hello world", obj::vector2(800, 600));
    obj::scene* Scene = obj::CreateScene();
    Win->SetScene(Scene);
    Scene->BackGroundColor = obj::color::red;

    obj::gameobject* gam = obj::CreateGameObject(Scene);

    obj::spriteRenderer* sr = gam->AddComponent<obj::spriteRenderer>();
    obj::sprite* sp = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Triangle.png");
    sr->Sprite = sp;

    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
        }

        //SDL_Delay(16);
        obj::Update();
        obj::Render();
    }

    obj::Quit();
    return 0;
}
