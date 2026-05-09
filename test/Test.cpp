#include "../include/Core.hpp"

using namespace obj;

void CreateScenes(std::vector<scene*>& Scenes){
    scene* Scene1 = CreateScene();
    Scene1->Name = "Scene1";
    Scene1->BackGroundColor = {64, 0 , 0};
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


int main(){
    Init();

    window* Window = CreateWindow("ObjLib Test", {800, 600});
    std::vector<scene*> Scenes;
    CreateScenes(Scenes);

    camera* Camera = Window->GetCamera();

    UI::canvas* Canvas = UI::CreateCanvas();

    Camera->SetCanvas(Canvas);

    UI::screenObject* ObjectsText = UI::CreateScreenObject(Canvas);

    font* Font = CreateFont(exePath() + "/Fonts/Times New Roman.ttf");

    UI::Text* TextUI = ObjectsText->AddComponent<UI::Text>();
    ObjectsText->AddComponent<MoveWithWASDScren>();
    TextUI->Pivot = pivot::TopLeft;

    TextUI->SetFont(Font);

    Window->SetScene(Scenes[0]);
    Window->SetTitle(Scenes[0]->Name);

    sprite* SquareSprite = CreateSprite(exePath() + "/Sprites/Default/Square.png");
    sprite* CircleSprite = CreateSprite(exePath() + "/Sprites/Default/Circle.png");
    sprite* TriangleSprite = CreateSprite(exePath() + "/Sprites/Default/Triangle.png");

    gameObject* A = CreateGameObject(Scenes[0]);
    gameObject* B = CreateGameObject(Scenes[0]);
    gameObject* C = CreateGameObject(Scenes[0]);
    
    A->AddComponent<spriteRenderer>()->Sprite = SquareSprite;
    B->AddComponent<spriteRenderer>()->Sprite = CircleSprite;
    C->AddComponent<spriteRenderer>()->Sprite = TriangleSprite;

    A->AddComponent<MoveWithWASDObj>();
    B->AddComponent<MoveWithWASDObj>();
    C->AddComponent<MoveWithWASDObj>();

    A->Transform->LocalPosition = {0,0};
    B->Transform->LocalPosition = {0,1};
    C->Transform->LocalPosition = {0,1};

    A->Transform->SetChild(B->Transform);
    B->Transform->SetChild(C->Transform);
    C->Transform->SetChild(A->Transform);


    int CurrentSceneIndex = 0;

    while (true){
        Update();

        if (Input::KeyPressed(KeyCode::Escape)){
            CurrentSceneIndex = (CurrentSceneIndex + 1) % Scenes.size();
            Window->SetScene(Scenes[CurrentSceneIndex]);
            Window->SetTitle(Scenes[CurrentSceneIndex]->Name);
        }

        TextUI->SetText("Objects Count: " + std::to_string(Scenes[CurrentSceneIndex]->GetGameObjects().size()));

        if (Input::KeyPressed(KeyCode::F1)){
            Window->Debug = !Window->Debug;
        }

        if (Input::KeyPressed(KeyCode::Q)){
            gameObject* n = CreateGameObject(Scenes[CurrentSceneIndex]);
            n->Transform->LocalPosition = Input::WorldMousePosition;

            n->AddComponent<spriteRenderer>()->Sprite = TriangleSprite;
        }

        Window->GetCamera()->AddComponent<MoveWithWASDCam>();

        Apply();
        Render();
    }

    Quit();
    return 0;
}