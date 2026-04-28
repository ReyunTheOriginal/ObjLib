#include "../include/Core.hpp"

obj::gameObject* HeldObj = nullptr;

int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    obj::window* Win = obj::CreateWindow("hello Earth", obj::vector2(1200, 600));
    obj::scene* Scene = obj::CreateScene();
    Win->SetScene(Scene);
    
    Scene->ActiveCamera->ActiveScene = Scene;
    Scene->ActiveCamera->SetResolution({800,600});

    Scene->ActiveCamera->Position = {0,0};

    obj::gameObject* gam = obj::CreateGameObject(Scene);
    obj::gameObject* gam2 = obj::CreateGameObject(Scene);
    obj::gameObject* gam3 = obj::CreateGameObject(Scene);
    gam2->Position = {400, 300};
    gam2->Size = {0.5, 2};

    gam3->Size = {0.5, 0.5};
    gam3->Position = {0,0};

    obj::spriteRenderer* sr = gam->AddComponent<obj::spriteRenderer>();
    obj::spriteRenderer* sr2 = gam2->AddComponent<obj::spriteRenderer>();
    obj::spriteRenderer* sr3 = gam3->AddComponent<obj::spriteRenderer>();

    obj::sprite* sp = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Triangle.png");
    obj::sprite* sp2 = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Circle.png");
    obj::sprite* sp3 = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Square.png");

    sr->Sprite = sp;
    sr2->Sprite = sp2;
    sr3->Sprite = sp3;

    sr3->RenderLayer.Order = 2;

    sr->Color = {255,0,0,128};
    sr2->Color = {0, 255, 0, 255};

    while(running){
        obj::Update();

        if (obj::Input.MouseButtonPressed(1)){
            float dis = MAXFLOAT;
            for (obj::gameObject* obj : Win->GetScene()->GameObjects){
                float newdis = obj::Math::Distance(obj::Input.WorldMousePosition, obj->Position);
                if (newdis < dis){
                    dis = newdis;
                    HeldObj = obj;
                }
            }
        }

        if (obj::Input.KeyHeld(obj::KeyCode::R)){
            if (obj::Input.KeyHeld(obj::KeyCode::LShift)){
                Scene->ActiveCamera->Rotation++;
            }else{
                Scene->ActiveCamera->Rotation--;
            }
        }

        if (obj::Input.KeyHeld(obj::KeyCode::Space)){
            if (obj::Input.KeyHeld(obj::KeyCode::LShift)){
                Scene->ActiveCamera->Zoom+= 0.01;
            }else{
                Scene->ActiveCamera->Zoom-= 0.01;
            }
        }

        Scene->ActiveCamera->Position += obj::Input.DirectionalInput;

        //obj::Print(Scene->GetActiveCamera()->Position);

        if (obj::Input.MouseButtonReleased(1)){
            HeldObj = nullptr;
        }

        if (HeldObj){
            HeldObj->Position = obj::Input.WorldMousePosition;
        }

        //SDL_Delay(16);
        obj::Render();
    }

    obj::Quit();
    return 0;
}
