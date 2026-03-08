#include "../include/Core.hpp"

obj::gameobject* HeldObj = nullptr;

int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    obj::window* Win = obj::CreateWindow("hello world", obj::vector2(800, 600));
    obj::window* Win2 = obj::CreateWindow("hello Earth", obj::vector2(800, 600));
    obj::scene* Scene = obj::CreateScene();
    Win->SetScene(Scene);
    Win2->SetScene(Scene);
    Scene->BackGroundColor = obj::color::black;

    obj::gameobject* gam = obj::CreateGameObject(Scene);
    obj::gameobject* gam2 = obj::CreateGameObject(Scene);
    gam2->Position = {400, 300};
    gam2->Size = {0.5, 2};

    obj::spriteRenderer* sr = gam->AddComponent<obj::spriteRenderer>();
    obj::spriteRenderer* sr2 = gam2->AddComponent<obj::spriteRenderer>();
    obj::sprite* sp = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Triangle.png");
    obj::sprite* sp2 = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Circle.png");
    sr->Sprite = sp;
    sr2->Sprite = sp2;

    sr->Color = {255,0,0,128};
    sr2->Color = {0, 255, 0, 255};

    while(running){
        obj::Update();

        if (obj::Input.MouseButtonPressed(1)){
            float dis = MAXFLOAT;
            for (obj::gameobject* obj : Win->GetScene()->GameObjects){
                float newdis = obj::Math::Distance(obj::Input.MousePosition, obj->Position);
                if (newdis < dis){
                    dis = newdis;
                    HeldObj = obj;
                }
            }
        }

        if (obj::Input.MouseButtonReleased(1)){
            HeldObj = nullptr;
        }

        if (HeldObj){
            HeldObj->Position = obj::Input.MousePosition;
        }

        //SDL_Delay(16);
        obj::Render();
    }

    obj::Quit();
    return 0;
}
