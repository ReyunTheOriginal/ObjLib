#include "Components/SpriteRenderer.hpp"

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Camera/Camera.hpp"
#include "Components/Transform.hpp"
#include "EssentialProcesses.hpp"
#include "Rendering/Rendering.hpp"

#include <unordered_map>

namespace obj{

    void spriteRenderer::Draw(window* Window){
        if (Internal::Renderer && GetGameObject() && GetGameObject()->GetScene()){
            auto Texture = Sprite->Textures[Window];
            if (Texture){
                camera* ActiveCamera = Window->GetCamera();

                //scale the rect
                float zoom = ActiveCamera->Zoom;
                vector2 scaled = {(GetGameObject()->Transform->GetWorldScale().x * PixelsPerUnit) * zoom,
                                (GetGameObject()->Transform->GetWorldScale().y * PixelsPerUnit) * zoom};

                vector2 ScreenPos = ActiveCamera->WorldToScreenPosition(GetGameObject()->Transform->GetWorldPosition());

                //set the texture colors
                Texture->SetTextureColorMod(Color);

                SDL_FlipMode Flip = SDL_FLIP_NONE;
                if (FlipHorizontal){
                    Flip = SDL_FLIP_HORIZONTAL;
                    if (FlipVertical){
                        Flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
                    }
                }else if (FlipVertical){
                    Flip = SDL_FLIP_VERTICAL;
                    if (FlipHorizontal){
                        Flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
                    }
                }
                
                //render it rotated as necessary
                Internal::Renderer->DrawSpriteRotated(
                    Window, 
                    Sprite, 
                    ScreenPos, 
                    scaled, 
                    (GetGameObject()->Transform->GetWorldRotation() + ActiveCamera->Rotation),
                    Pivot,
                    Flip
                );
            }else{
                std::cout << "Sprite Has No Texture for:" + GetGameObject()->Name << "\n";
            }
        }
    }
}