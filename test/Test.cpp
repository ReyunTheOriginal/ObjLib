#include "../include/Core.hpp"


int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    obj::window* Win = obj::CreateWindow("hello world", obj::vector2(800, 600));
    obj::scene* Scene = obj::CreateScene();
    Win->SetScene(Scene);
    Scene->BackGroundColor = obj::color::black;

    obj::gameobject* gam = obj::CreateGameObject(Scene);
    obj::gameobject* gam2 = obj::CreateGameObject(Scene);
    gam2->Position = {800, 600};

    obj::spriteRenderer* sr = gam->AddComponent<obj::spriteRenderer>();
    obj::spriteRenderer* sr2 = gam2->AddComponent<obj::spriteRenderer>();
    obj::sprite* sp = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Triangle.png");
    obj::sprite* sp2 = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Circle.png");
    sr->Sprite = sp;
    sr2->Sprite = sp2;

    while(running){
        obj::Update();

        if (obj::Input.KeyPressed(obj::KeyCode::A)){
            std::cerr << "PressedKey A\n";
        }
        if (obj::Input.KeyReleased(obj::KeyCode::S)){
            std::cerr << "ReleasedKey S\n";
        }
        if (obj::Input.KeyHeld(obj::KeyCode::D)){
            std::cerr << "HeldKey D\n";
        }

        if (obj::Input.MouseButtonPressed(1)){
            std::cerr << "PressedButton LEFT\n";
        }
        if (obj::Input.MouseButtonReleased(2)){
            std::cerr << "ReleasedButton MIDDLE\n";
        }
        if (obj::Input.MouseButtonHeld(3)){
            std::cerr << "HeldButton RIGHT\n";
        }


        //SDL_Delay(16);
        obj::Render();
    }

    obj::Quit();
    return 0;
}
