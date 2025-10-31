#include "../include/Core.hpp"

int main(int argc, char *argv[]){
    obl::Init();

    obl::window* Window = obl::CreateWindow("Hello World", 
        {800, 600}, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
        SDL_RENDERER_ACCELERATED
    );

    obl::window* Window2 = obl::CreateWindow("Hello World", 
        {800, 600}, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
        SDL_RENDERER_ACCELERATED
    );
    obl::window* Window3 = obl::CreateWindow("Hello World", 
        {800, 600}, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, 
        SDL_RENDERER_ACCELERATED
    );

    obl::Input.GetFocusedWindow()->Debug = true;

    bool running = true;

    obl::gameObject* HeldObject = nullptr;
    bool Dragging = false;

    float time = 0;

    while (running){
        obl::ClearWindows();
        obl::Update();

        //Corrent Problem:
        /*Object Hierarchy*/

        time += obl::Time.DeltaTime;

        obl::Input.GetFocusedWindow()->ActiveCamera->Position += obl::Input.DirectionalInput * (10 * obl::Time.DeltaTime);
        
        if (HeldObject){
            if (obl::Input.ScrollY > 0){
                HeldObject->GetComponent<obl::components::renderLayer>()->RenderOrder += 1;
            }else if (obl::Input.ScrollY < 0){
                HeldObject->GetComponent<obl::components::renderLayer>()->RenderOrder -= 1;
            }

            if (obl::Input.KeyPressed(SDLK_g)){
                auto* ren = HeldObject->GetComponent<obl::components::renderLayer>();
                ren->Enabled = !ren->Enabled;
            }

            if (obl::Input.KeyHeld(SDLK_r)){
                if (obl::Input.KeyHeld(SDLK_LSHIFT)){
                    HeldObject->Transform->Rotation -= (60 * obl::Time.DeltaTime);
                }else{
                    HeldObject->Transform->Rotation += (60 * obl::Time.DeltaTime);
                }
            }
        }else{
            obl::Input.GetFocusedWindow()->ActiveCamera->ZoomIn(obl::Input.ScrollY * (5 * obl::Time.DeltaTime));

            if (obl::Input.KeyHeld(SDLK_r)){
                if (obl::Input.KeyHeld(SDLK_LSHIFT)){
                    obl::Input.GetFocusedWindow()->ActiveCamera->Rotation -= (60 * obl::Time.DeltaTime);
                }else{
                    obl::Input.GetFocusedWindow()->ActiveCamera->Rotation += (60 * obl::Time.DeltaTime);
                }
            }
        }


        if (obl::Input.MousePressed(3)){
            int Ran = obl::RandomRange<int>(0,4);
            std::string sprite = "";
            switch (Ran){
                case 1:
                    sprite = "Circle";
                    break;
                case 2:
                    sprite = "Triangle";
                    break;
                case 3:
                    sprite = "Square";
                    break;
                default:
                    sprite = "Error";
                    break;
            }
            obl::gameObject* NewObj = obl::CreateGameObject(*obl::Input.GetFocusedWindow());

            auto& ren = NewObj->AddComponent<obl::components::SpriteRenderer>();
            ren.SetSprite(sprite);
            ren.Color = {obl::RandomRange<int>(0,256),obl::RandomRange<int>(0,256),obl::RandomRange<int>(0,256),255};
            
            NewObj->Transform->Position = obl::Input.GetFocusedWindow()->ScreenToWorldPosition({(float)(obl::RandomRange<int>(0,obl::Input.GetFocusedWindow()->GetResulotion().x + 1)), (float)(obl::RandomRange<int>(0,obl::Input.GetFocusedWindow()->GetResulotion().y + 1))});
        
            Dragging = true;
            obl::Input.GetFocusedWindow()->Cursor->ToggleCursorLock(true);
        }

        if (obl::Input.KeyPressed(SDLK_t)){
            obl::Input.GetFocusedWindow()->Debug = !obl::Input.GetFocusedWindow()->Debug;
            obl::Print(obl::Input.GetFocusedWindow()->Debug);
        }

        if (obl::Input.MouseReleased(1)){
            HeldObject = nullptr;
        }
        if (obl::Input.MouseReleased(3)){
            Dragging = false;
            obl::Input.GetFocusedWindow()->Cursor->ToggleCursorLock(false);
        }

        if (obl::Input.MousePressed(1)){
            float CurrentDistance = MAXFLOAT;
            obl::gameObject* pointer = nullptr;
            for (auto& obj : obl::Input.GetFocusedWindow()->WindowObjects){
                float dis = obl::Math::Distance(obl::Input.GetFocusedWindow()->ScreenToWorldPosition(obl::Input.GetMousePos()), obj->Transform->Position);
                if (dis < CurrentDistance){
                    CurrentDistance = dis;
                    pointer = obj.get();
                }
            }
            if (CurrentDistance < 4.0)HeldObject = pointer;
        }

        if (HeldObject){
            HeldObject->Transform->Position = obl::Input.GetFocusedWindow()->ScreenToWorldPosition(obl::Input.GetMousePos());
        }

        if (Dragging){
            obl::Input.GetFocusedWindow()->ActiveCamera->Position += (obl::Input.MouseMotion()) * ((10 * obl::Time.DeltaTime) / obl::Input.GetFocusedWindow()->ActiveCamera->GetZoom());
        }

        obl::RenderWindows();
    }

    obl::Quit();
    return 0;
}