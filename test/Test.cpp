#include "../include/Core.hpp"

using namespace obj;

void ChangeAllColors(gameObject* obj, color Color){
    spriteRenderer* ren = obj->GetComponent<spriteRenderer>();
    if (ren)ren->Color = Color;

    for (Internal::transform* child : obj->Transform->GetChildren()){
        ChangeAllColors(child->GetGameObject(), Color);
    }

};

gameObject* GetClosestObjectTo(scene* Scene, vector2 Pos, float MaxDis, std::unordered_set<gameObject*> Excluded = std::unordered_set<gameObject*>()){
    float currentDis = 999999;
    gameObject* CurrentObj = nullptr;

    for (gameObject* obj : Scene->GetGameObjects()){
        float dis = Math::Distance(Pos, obj->Transform->GetWorldPosition());
        if (dis < currentDis && dis <= MaxDis && !Excluded.contains(obj)){
            currentDis = dis;
            CurrentObj = obj;
        }
    }

    return CurrentObj;
}

struct MoveWithWASDObj : component{
    float Speed = 1;
    void Run() override{
        GetGameObject()->Transform->LocalPosition += Input::DirectionalInput * 2.5 * Speed * Time::DeltaTime;
    }
};

struct MoveWithWASDCam : cameraComponent{
    float Speed = 1;
    void Run() override{
        GetCamera()->Position += Input::DirectionalInput * 2.5 * Speed * Time::DeltaTime;
    }
};

struct MoveWithWASDScren : UI::screenComponent{
    float Speed = 1;
    void Run() override{
        vector2 move = Input::DirectionalInput * 2.5 * Speed * Time::DeltaTime * PixelsPerUnit;
        move.y *= -1;
        GetScreenObject()->UITransform->LocalPosition += move;
    }
};

sprite* SquareSprite = CreateSprite(exePath() + "/Sprites/Default/Square.png");
sprite* CircleSprite = CreateSprite(exePath() + "/Sprites/Default/Circle.png");
sprite* TriangleSprite = CreateSprite(exePath() + "/Sprites/Default/Triangle.png");

gameObject* HeldObject = nullptr;

struct Scene1Stuff : component{
    void Run() override{
        if (Input::KeyPressed(InputCode::Q)){
            gameObject* n = CreateGameObject(GetGameObject()->GetScene());
            n->Transform->LocalPosition = Input::WorldMousePosition;

            int RandomInt = RandomRange(0,2);

            std::vector<sprite*> sprites = {SquareSprite, CircleSprite, TriangleSprite};

            n->AddComponent<spriteRenderer>()->Sprite = sprites[RandomInt];
            collider* col = n->AddComponent<collider>();
            col->SetPolygon(ImportPolygon(exePath() + "/Polygon.json"));
        }

        if (Input::KeyPressed(InputCode::T)){
            gameObject* n = CreateGameObject(GetGameObject()->GetScene());
            n->Transform->LocalPosition = Input::WorldMousePosition;

            int RandomInt = RandomRange(0,2);

            std::vector<sprite*> sprites = {SquareSprite, CircleSprite, TriangleSprite};

            n->AddComponent<spriteRenderer>()->Sprite = sprites[RandomInt];
            n->AddComponent<collider>();
        }

        if (!HeldObject && Input::MouseButtonPressed(InputCode::LeftMouseButton)){
            HeldObject = GetClosestObjectTo(GetGameObject()->GetScene(), Input::WorldMousePosition, 1);
        }
        if (Input::MouseButtonReleased(InputCode::LeftMouseButton)){
            HeldObject = nullptr;
        }

        if (HeldObject){
            HeldObject->Transform->SetWorldPosition(Input::WorldMousePosition);

            if (Input::KeyHeld(InputCode::R)){
                HeldObject->Transform->LocalRotation += Time::DeltaTime * (Input::KeyHeld(InputCode::LShift)? -60 : 60);
            }

            if (Input::KeyPressed(InputCode::C)){
                if (!HeldObject->Transform->GetParent()){
                    ChangeAllColors(HeldObject, {
                            (float)(rand() % 256), 
                            (float)(rand() % 256), 
                            (float)(rand() % 256)
                        }
                    );
                }
            }

            if (Input::KeyPressed(InputCode::N)){
                if (HeldObject->Transform->GetChildren().size() > 0){
                    ChangeAllColors(HeldObject, {
                            (float)(rand() % 256), 
                            (float)(rand() % 256), 
                            (float)(rand() % 256)
                        }
                    );
                }else{
                    ChangeAllColors(HeldObject, {255,255,255,255});
                }

                HeldObject->Transform->SetParent(nullptr);
            }

            if (Input::MouseButtonPressed(InputCode::RightMouseButton)){
                gameObject* NewParent = GetClosestObjectTo(GetGameObject()->GetScene(), HeldObject->Transform->GetWorldPosition(), 2, {HeldObject});

                if (NewParent){
                    HeldObject->Transform->SetParent(NewParent->Transform);
                    HeldObject->GetComponent<spriteRenderer>()->Color = NewParent->GetComponent<spriteRenderer>()->Color;

                    if (HeldObject->GetComponent<spriteRenderer>()->Color == color(255,255,255,255)){
                        ChangeAllColors(NewParent, {
                                (float)(rand() % 256), 
                                (float)(rand() % 256), 
                                (float)(rand() % 256)
                            }
                        );
                    }else{
                        ChangeAllColors(NewParent, HeldObject->GetComponent<spriteRenderer>()->Color);
                    }

                }else{
                    HeldObject->Transform->SetParent(nullptr);

                    if (HeldObject->Transform->GetChildren().size() > 0){
                        ChangeAllColors(HeldObject, {
                                (float)(rand() % 256), 
                                (float)(rand() % 256), 
                                (float)(rand() % 256)
                            }
                        );
                    }
                    
                }
            }
            HeldObject->Transform->LocalScale += Input::MouseScroll.y * Time::DeltaTime * 5;
        }else{
            GetGameObject()->GetScene()->GetWindows()[0]->GetCamera()->Zoom += Input::MouseScroll.y * Time::DeltaTime * 5;
        }
        for (gameObject* parentless : GetGameObject()->GetScene()->GetParentlessGameObjects()){
            float t = Time::ElapsedTime * 20 + parentless->GetID() * 1.5;
            color Color = {
                (float)(Math::Sin(t) * 127.5f + 127.5f),
                (float)(Math::Sin(t + 2.09f) * 127.5f + 127.5f),
                (float)(Math::Sin(t + 4.18f) * 127.5f + 127.5f),
                255
            };

            spriteRenderer* renderer = parentless->GetComponent<spriteRenderer>();

            if (renderer)
                renderer->Color = Color;
        }
    }
};

void CreateScenes(std::vector<scene*>& Scenes){
    scene* Scene1 = CreateScene();
    Scene1->Name = "Scene1";
    Scene1->BackGroundColor = {64, 0 , 0};
    gameObject* sc = CreateGameObject(Scene1);
    sc->Transform->LocalPosition = {9999,999};
    sc->AddComponent<Scene1Stuff>();
    Scenes.push_back(Scene1);

    scene* Scene2 = CreateScene();
    Scene2->Name = "Scene2";
    Scene2->BackGroundColor = {0, 64 , 0};
    Scenes.push_back(Scene2);

    scene* Scene3 = CreateScene();
    Scene3->Name = "Scene3";
    Scene3->BackGroundColor = {0, 0 , 64};
    Scenes.push_back(Scene3);

    scene* Scene4 = CreateScene();
    Scene4->Name = "Scene4";
    Scene4->BackGroundColor = {0, 0 , 0};
    Scenes.push_back(Scene4);
}

int main(){
    Init();

    window* Window = CreateWindow("ObjLib Test", {800, 600});
    std::vector<scene*> Scenes;
    CreateScenes(Scenes);

    Window->SetScene(Scenes[0]);
    Window->SetTitle(Scenes[0]->Name);

    int CurrentSceneIndex = 0;

    FPS::SetTargetFrameRate(70);

    while (true){
        Update();

        if (Input::KeyPressed(InputCode::Escape)){
            CurrentSceneIndex = (CurrentSceneIndex + 1) % Scenes.size();
            Window->SetScene(Scenes[CurrentSceneIndex]);
            Window->SetTitle(Scenes[CurrentSceneIndex]->Name);
        }

        if (Input::KeyPressed(InputCode::F1)){
            Window->Debug = !Window->Debug;
        }

        Window->GetCamera()->AddComponent<MoveWithWASDCam>();

        Apply();
        Render();
    }

    Quit();
    return 0;
}