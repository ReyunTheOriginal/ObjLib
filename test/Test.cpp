#include "../include/Core.hpp"

/*obj::gameObject* HeldObj = nullptr;

int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    
    obj::window* Win = obj::CreateWindow("hello Earth", obj::vector2(1200, 600));
    //SDL_SetRenderVSync(Win->SDLrenderer, 1);

    obj::scene* Scene = obj::CreateScene();

    Win->SetScene(Scene);

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


    sr3->RenderLayer->Order = 2;

    sr->Color = {255,0,0,128};
    sr2->Color = {0, 255, 0, 255};

    while(running){
        obj::Update();

        Win->SetTitle(std::to_string(obj::FPS::FPS));

        if (obj::Input::MouseButtonPressed(1)){
            float dis = MAXFLOAT;
            for (obj::gameObject* obj : obj::Input::FocusedWindow->GetScene()->GameObjects){
                float newdis = obj::Math::Distance(obj::Input::WorldMousePosition, obj->Position);
                if (newdis < dis){
                    dis = newdis;
                    HeldObj = obj;
                }
            }
        }

        if (obj::Input::KeyPressed(obj::KeyCode::G)){
            sr->FlipVertical = !sr->FlipVertical;
        }

        float move = 5 * obj::Time::DeltaTime;

        if (obj::Input::KeyHeld(obj::KeyCode::Up)){
            Scene->ActiveCamera->SetResolution(Scene->ActiveCamera->GetResolution() + obj::vector2(move, move));
        }
        if (obj::Input::KeyHeld(obj::KeyCode::Down)){
            Scene->ActiveCamera->SetResolution(Scene->ActiveCamera->GetResolution() + obj::vector2(-move, -move));
        }

        if (obj::Input::KeyHeld(obj::KeyCode::Space)){
            if (obj::Input::KeyHeld(obj::KeyCode::LShift)){
                Scene->ActiveCamera->Zoom+= (obj::Time::DeltaTime);
            }else{
                Scene->ActiveCamera->Zoom-= (obj::Time::DeltaTime);
            }
        }

        Scene->ActiveCamera->Position += obj::Input::DirectionalInput;

        if (obj::Input::MouseButtonReleased(1)){
            HeldObj = nullptr;
        }

        if (HeldObj){
            HeldObj->Position = obj::Input::WorldMousePosition;

            if (obj::Input::KeyHeld(obj::KeyCode::R)){
                if (obj::Input::KeyHeld(obj::KeyCode::LShift)){
                    HeldObj->Rotation += (150 * obj::Time::DeltaTime);
                }else{
                    HeldObj->Rotation -= (150 * obj::Time::DeltaTime);
                }
            }
        }

        obj::Render();
    }

    obj::Quit();
    return 0;
}*/

using namespace obj;

sprite* SquareSprite = CreateSprite(exePath() + "/Sprites/Default/Square.png");

std::vector<gameObject*> Pillers;

void CreatePiller(scene* Scene){
    float openingSize = RandomRange((float)0, (float)70/32);
    float openingloc = RandomRange((float)-120/32, (float)120/32);

    gameObject* TopObj = CreateGameObject(Scene);
    spriteRenderer* TopRen = TopObj->AddComponent<spriteRenderer>();

    TopObj->Transform->Position = {550/32, (350 + openingSize + openingloc)/32};
    TopObj->Transform->Scale = {0.3, 2};

    TopRen->Sprite = SquareSprite;

    TopRen->Color = {255, 0, 0};

    Pillers.push_back(TopObj);

    gameObject* DownObj = CreateGameObject(Scene);
    spriteRenderer* DownRen = DownObj->AddComponent<spriteRenderer>();

    DownObj->Transform->Position = {550/32, (-350 - openingSize + openingloc)/32};

    DownObj->Transform->Scale = {0.3, 2};

    DownRen->Sprite = SquareSprite;

    DownRen->Color = {255, 0, 0};

    Pillers.push_back(DownObj);

}

int main(){
    Init();

    FPS::SetTargetFrameRate(60);

    window* Window = CreateWindow("Flappy Bird Test", {800, 600});
    scene* Scene = CreateScene();
    Window->SetScene(Scene);
    gameObject* flappy = CreateGameObject(Scene);

    spriteRenderer* renderer = flappy->AddComponent<spriteRenderer>();
    
    renderer->Sprite = SquareSprite;
    renderer->Color = {0, 255, 0};

    flappy->Transform->Scale = {0.3, 0.3};
    flappy->Transform->Position.x = -200/32;

    vector2 Velocity = {0,0};


    bool running = true;

    float Gravity = 740/32;
    float JumpForce = 320/32;
    float PillerSpeed = 250/32;

    float pillerTimer = 0;

    float pillerCoolDown = 3;

    UI::canvas* Canvas = UI::CreateCanvas(Scene->ActiveCamera);

    UI::screenObject* scren = UI::CreateScreenObject(Canvas);

    sprite* CircleSprite = CreateSprite(exePath() + "/Sprites/Default/Triangle.png");

    UI::image* i = scren->AddComponent<UI::image>();

    i->Sprite = CircleSprite;

    while (running){
        Update();

        pillerTimer += Time::DeltaTime;

        Velocity.y -= Gravity * Time::DeltaTime;

        if (Input::KeyPressed(KeyCode::Space)){
            Velocity.y = JumpForce;
        }

        scren->UITransform->Position = Input::ScreenMousePosition;

        if (pillerTimer >= pillerCoolDown){
            pillerCoolDown = RandomRange((float)0.8, (float)3);
            CreatePiller(Scene);
            pillerTimer = 0;
        }

        for (int i = (int)Pillers.size() - 1; i >= 0; i--){
            gameObject* piller = Pillers[i];
            piller->Transform->Position.x -= PillerSpeed * Time::DeltaTime;

            if (piller->Transform->Position.x <= -500){
                Pillers.erase(Pillers.begin() + i);
                Destroy(piller);
            }
        }

        flappy->Transform->Position += Velocity * Time::DeltaTime;
        
        Render();
    }
    return 0;
}


