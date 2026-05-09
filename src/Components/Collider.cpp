#include "Components/Collider.hpp"

#include "Window.hpp"
#include "GameObject.hpp"
#include "Components/Transform.hpp"
#include "Camera/Camera.hpp"

#include <HelperFunctions.hpp>
#include "Math.hpp"

namespace obj{
    std::vector<vector2> collider::GetWorldVertices(){
        std::vector<vector2> result;

        for (vector2 vert : Vertices){
            // Apply scale
            vector2 scaled = vert * GetGameObject()->Transform->GetWorldScale();
            
            float cos_rot = std::cos(Math::Deg2Rad(GetGameObject()->Transform->GetWorldRotation()));
            float sin_rot = std::sin(Math::Deg2Rad(GetGameObject()->Transform->GetWorldRotation()));
            vector2 rotated = {
                scaled.x * cos_rot - scaled.y * sin_rot,
                scaled.x * sin_rot + scaled.y * cos_rot
            };
            
            // Apply position and offset
            vector2 worldVert = rotated + GetGameObject()->Transform->GetWorldPosition() + Offsit;

            result.push_back(worldVert);
        }

        return result;
    }

    void collider::DebugDraw(window* Window){
        std::vector<vector2> WorldVerts = GetWorldVertices();

        for (vector2& vert : WorldVerts){
            vert = Window->GetCamera()->WorldToScreenPosition(vert);
        }

        int size = WorldVerts.size();

        for (int i=0; i<size; i++){
            SDL_SetRenderDrawColor(Window->GetSDLRenderer(), DebugColor.r, DebugColor.g, DebugColor.b, DebugColor.a);
            SDL_RenderLine(Window->GetSDLRenderer(), WorldVerts[i].x, WorldVerts[i].y,  WorldVerts[(i + 1) % size].x, WorldVerts[(i + 1) % size].y);
        }
    }
}