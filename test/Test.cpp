#include "../include/Core.hpp"
/*
obj::gameObject* HeldObj = nullptr;

int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    
    obj::window* Win = obj::CreateWindow("hello Earth", obj::vector2(1200, 600));
    //SDL_SetRenderVSync(Win->SDLrenderer, 1);

    obj::scene* Scene = obj::CreateScene();

    Win->SetScene(Scene);

    Win->ActiveCamera->SetResolution({800,600});

    Win->ActiveCamera->Position = {0,0};

    obj::gameObject* gam = obj::CreateGameObject(Scene);
    obj::gameObject* gam2 = obj::CreateGameObject(Scene);
    obj::gameObject* gam3 = obj::CreateGameObject(Scene);
    gam2->Transform->Position = {400, 300};
    gam2->Transform->Scale = {0.5, 2};

    gam3->Transform->Scale = {0.5, 0.5};
    gam3->Transform->Position = {0,0};

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
                float newdis = obj::Math::Distance(obj::Input::WorldMousePosition, obj->Transform->Position);
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
            Win->ActiveCamera->SetResolution(Win->ActiveCamera->GetResolution() + obj::vector2(move, move));
        }
        if (obj::Input::KeyHeld(obj::KeyCode::Down)){
            Win->ActiveCamera->SetResolution(Win->ActiveCamera->GetResolution() + obj::vector2(-move, -move));
        }

        if (obj::Input::KeyHeld(obj::KeyCode::Space)){
            if (obj::Input::KeyHeld(obj::KeyCode::LShift)){
                Win->ActiveCamera->Zoom+= (obj::Time::DeltaTime);
            }else{
                Win->ActiveCamera->Zoom-= (obj::Time::DeltaTime);
            }
        }

        Win->ActiveCamera->Position += obj::Input::DirectionalInput * obj::Time::DeltaTime;

        if (obj::Input::MouseButtonReleased(1)){
            HeldObj = nullptr;
        }

        if (HeldObj){
            HeldObj->Transform->Position = obj::Input::WorldMousePosition;

            if (obj::Input::KeyHeld(obj::KeyCode::R)){
                if (obj::Input::KeyHeld(obj::KeyCode::LShift)){
                    HeldObj->Transform->Rotation += (150 * obj::Time::DeltaTime);
                }else{
                    HeldObj->Transform->Rotation -= (150 * obj::Time::DeltaTime);
                }
            }
        }

        obj::Render();
    }

    obj::Quit();
    return 0;
}*/


using namespace obj;

struct obst{
    gameObject* obj1 = nullptr;
    gameObject* obj2 = nullptr;

    bool GavePoint = false;

    ~obst(){
        Destroy(obj1);
        Destroy(obj2);
    }
};

std::vector<obst*> Pillers;

void CreatePiller(scene* Scene, sprite* sp){

    obst* newobst = new obst();

    float openingSize = RandomRange((float)0, (float)70);
    float openingloc = RandomRange((float)-120, (float)120);

    gameObject* TopObj = CreateGameObject(Scene);
    spriteRenderer* TopRen = TopObj->AddComponent<spriteRenderer>();

    TopObj->Transform->Position = {550/64, (350 + openingSize + openingloc)/64};
    TopObj->Transform->Scale = {0.3 * 3.33, 2 * 3.33};

    TopRen->Sprite = sp;

    TopRen->Color = {255, 0, 0};

    newobst->obj1 = TopObj;

    gameObject* DownObj = CreateGameObject(Scene);
    spriteRenderer* DownRen = DownObj->AddComponent<spriteRenderer>();

    DownObj->Transform->Position = {550/64, (-350 - openingSize + openingloc)/64};

    DownObj->Transform->Scale = {0.3 * 3.33, 2 * 3.33};

    DownRen->Sprite = sp;

    DownRen->Color = {255, 0, 0};

    newobst->obj2 = DownObj;

    Pillers.push_back(newobst);

}

int main(){
    Init();

    //FPS::SetTargetFrameRate(2);

    sprite* SquareSprite = CreateSprite(exePath() + "/Sprites/Default/Square.png");

    FPS::SetTargetFrameRate(60);

    window* Window = CreateWindow("Flappy Bird Test", {800, 600});

    scene* Scene = CreateScene();
    Window->SetScene(Scene);

    gameObject* flappy = CreateGameObject(Scene);

    spriteRenderer* renderer = flappy->AddComponent<spriteRenderer>();

    gameObject* testobj = CreateGameObject(Scene);
    collider* col = testobj->AddComponent<collider>();

    col->Vertices = ImportPolygon(exePath() + "/Polygon.json", 0.5);
    
    renderer->Sprite = SquareSprite;

    renderer->Color = {0, 255, 0};

    flappy->Transform->Scale = {0.3 * 3.33, 0.3 * 3.33};
    flappy->Transform->Position.x = -200/64;

    vector2 Velocity = {0,0};


    bool running = true;

    float Gravity = 720/64;
    float JumpForce = 320/64;
    float PillerSpeed = 250/64;

    float pillerTimer = 0;

    float pillerCoolDown = 1;

    UI::canvas* Canvas = UI::CreateCanvas(Window->ActiveCamera);

    sprite* CircleSprite = CreateSprite(exePath() + "/Sprites/Default/Circle.png");

    UI::screenObject* TextScren = UI::CreateScreenObject(Canvas);
    UI::screenObject* TextScren2 = UI::CreateScreenObject(Canvas, TextScren->UITransform);
    UI::Text* text = TextScren->AddComponent<UI::Text>();
    UI::Text* text2 = TextScren2->AddComponent<UI::Text>();
    Font* font = CreateFont(exePath() + "/Fonts/Times New Roman.ttf", 64);

    text->SetFont(font);
    text2->SetFont(font);

    TextScren2->UITransform->Position = {400, 300};

    int points = 0;

    Window->Debug = true;

    FPS::SetTargetFrameRate(60);

    while (running){
        Update();

        if (Input::KeyPressed(KeyCode::J)){
            TextScren->Enabled = !TextScren->Enabled;
        }

        testobj->Transform->Position = Input::WorldMousePosition;

        if (Input::KeyHeld(KeyCode::R)){
            if (Input::KeyHeld(KeyCode::LShift)){
                testobj->Transform->Rotation -= 1;
            }else{
                testobj->Transform->Rotation += 1;
            }
        }

        if (Input::KeyPressed(KeyCode::L)){
            testobj->Transform->Rotation += 90;
        }

        pillerTimer += Time::DeltaTime;

        Velocity.y -= Gravity * Time::DeltaTime;

        if (Input::KeyPressed(KeyCode::Space)){
            Velocity.y = JumpForce;
        }

        text->SetText("Score: " + std::to_string(points));
        TextScren->UITransform->Position = {(float)(text->GetTexture()->w /2), (float)(text->GetTexture()->h /2)};

        if (pillerTimer >= pillerCoolDown){
            pillerCoolDown = RandomRange((float)0.8, (float)3);
            CreatePiller(Scene, SquareSprite);
            pillerTimer = 0;
        }

        for (int i = (int)Pillers.size() - 1; i >= 0; i--){
            if (Pillers[i] && Pillers[i]->obj1 && Pillers[i]->obj2){

                Pillers[i]->obj1->Transform->Position.x -= PillerSpeed * Time::DeltaTime;
                Pillers[i]->obj2->Transform->Position.x -= PillerSpeed * Time::DeltaTime;

                if (Pillers[i]->obj1->Transform->Position.x < flappy->Transform->Position.x){
                    if (!Pillers[i]->GavePoint){
                        points++;
                        Pillers[i]->GavePoint = true;
                    }
                }

                if (Pillers[i]->obj1->Transform->Position.x <= -25){
                    delete Pillers[i];
                    Pillers.erase(Pillers.begin() + i);
                }
            }
        }

        flappy->Transform->Position += Velocity * Time::DeltaTime;
        
        Render();
    }
    return 0;
}


