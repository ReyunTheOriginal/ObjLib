#include "../include/Core.hpp"

int main(int argc, char *argv[]){
    bool running = true;
    obj::Init();
    
    // ===== WINDOW & SCENE SETUP =====
    obj::window* Win = obj::CreateWindow("ObjLib Transform Hierarchy Test", obj::vector2(1400, 800));
    obj::FPS::SetTargetFrameRate(60);

    obj::scene* Scene = obj::CreateScene();
    Win->SetScene(Scene);
    Win->ActiveCamera->SetResolution({800, 600});
    Win->ActiveCamera->Position = {0, 0};

    // ===== SPRITE SETUP =====
    obj::sprite* spCircle = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Circle.png");
    obj::sprite* spSquare = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Square.png");
    obj::sprite* spTriangle = obj::CreateSprite(obj::exePath() + "/Sprites/Default/Triangle.png");

    // ===== UI SETUP =====
    obj::UI::canvas* Canvas = obj::UI::CreateCanvas(Win->ActiveCamera);
    obj::Font* Font = obj::CreateFont(obj::exePath() + "/Fonts/Times New Roman.ttf", 20);

    // Title
    obj::UI::screenObject* titleDisplay = obj::UI::CreateScreenObject(Canvas);
    titleDisplay->UITransform->SetWorldPosition({10, 10});
    obj::UI::Text* titleText = titleDisplay->AddComponent<obj::UI::Text>();
    titleText->SetFont(Font);
    titleText->SetText("Transform Hierarchy Test");

    // Info Display
    obj::UI::screenObject* infoDisplay = obj::UI::CreateScreenObject(Canvas);
    infoDisplay->UITransform->SetWorldPosition({10, 40});
    obj::UI::Text* infoText = infoDisplay->AddComponent<obj::UI::Text>();
    infoText->SetFont(Font);
    infoText->SetText("Controls: Q=Spawn root  W=Spawn child  E=Rotate parent  R=Scale parent");

    obj::UI::screenObject* posDisplay = obj::UI::CreateScreenObject(Canvas);
    posDisplay->UITransform->SetWorldPosition({10, 70});
    obj::UI::Text* posText = posDisplay->AddComponent<obj::UI::Text>();
    posText->SetFont(Font);
    posText->SetText("Parent Pos: (0, 0)  Child Pos: (0, 0)");

    // ===== GAME OBJECTS =====
    obj::gameObject* parentObj = nullptr;
    obj::gameObject* childObj = nullptr;

    // ===== MAIN LOOP =====
    while(running){
        obj::Update();

        // ===== SPAWN ROOT OBJECT (Q) =====
        if (obj::Input::KeyPressed(obj::KeyCode::Q)){
            if (parentObj) obj::Destroy(parentObj);
            
            parentObj = obj::CreateGameObject(Scene);
            parentObj->Transform->SetWorldPosition({-1, 0});
            parentObj->Transform->SetWorldScale({1, 1});
            
            obj::spriteRenderer* sr = parentObj->AddComponent<obj::spriteRenderer>();
            sr->Sprite = spCircle;
            sr->Color = {255, 0, 0, 255};
            
            childObj = nullptr;
        }

        // ===== SPAWN CHILD OBJECT (W) =====
        if (obj::Input::KeyPressed(obj::KeyCode::W)){
            if (!parentObj) {
                infoText->SetText("Create parent first! (Q)");
            } else {
                if (childObj) obj::Destroy(childObj);
                
                childObj = obj::CreateGameObject(Scene, parentObj->Transform);
                childObj->Transform->SetWorldPosition({1, 0});
                childObj->Transform->SetWorldScale({0.5, 0.5});
                
                obj::spriteRenderer* sr = childObj->AddComponent<obj::spriteRenderer>();
                sr->Sprite = spSquare;
                sr->Color = {0, 255, 0, 255};
            }
        }

        // ===== ROTATE PARENT (E) =====
        if (obj::Input::KeyHeld(obj::KeyCode::E)){
            if (parentObj){
                parentObj->Transform->LocalRotation += 100 * obj::Time::DeltaTime;
            }
        }

        // ===== SCALE PARENT (R) =====
        if (obj::Input::KeyHeld(obj::KeyCode::R)){
            if (parentObj){
                float scaleAmount = 0.5f * obj::Time::DeltaTime;
                parentObj->Transform->LocalScale += obj::vector2(scaleAmount, scaleAmount);
            }
        }

        // ===== DRAG PARENT WITH MOUSE =====
        if (obj::Input::MouseButtonHeld(1) && parentObj){
            parentObj->Transform->SetWorldPosition(obj::Input::WorldMousePosition);
        }

        // ===== UPDATE DISPLAY TEXT =====
        if (parentObj){
            std::string posInfo = "Parent Pos: (" + std::to_string((int)parentObj->Transform->GetWorldPosition().x) + ", " + 
                                  std::to_string((int)parentObj->Transform->GetWorldPosition().y) + ")";
            if (childObj){
                posInfo += "  Child Pos: (" + std::to_string((int)childObj->Transform->GetWorldPosition().x) + ", " + 
                           std::to_string((int)childObj->Transform->GetWorldPosition().y) + ")";
            }
            posText->SetText(posInfo);
        } else {
            posText->SetText("Press Q to spawn parent object");
        }

        // ===== QUIT (Escape) =====
        if (obj::Input::KeyPressed(obj::KeyCode::Escape)){
            running = false;
        }

        obj::Apply();
        obj::Render();
    }

    obj::Quit();
    return 0;
}



/*using namespace obj;

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
}*/


