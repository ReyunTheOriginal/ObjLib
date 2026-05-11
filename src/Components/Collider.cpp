#include "Components/Collider.hpp"

#include "Window.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Components/Transform.hpp"
#include "Camera/Camera.hpp"

namespace obj{
    
    std::vector<vector2> collider::GetWorldVertices(){
        std::vector<vector2> result;

        for (Internal::polygon poly : ConvexPolygons){
            for (vector2 vert : poly.Vertices){
                // Apply scale
                vector2 scaled = vert * GetGameObject()->Transform->GetWorldScale();
                
                float angle = Math::Deg2Rad(-GetGameObject()->Transform->GetWorldRotation());  // Negate for clockwise
                float cos_rot = std::cos(angle);
                float sin_rot = std::sin(angle);
                vector2 rotated = {
                    scaled.x * cos_rot - scaled.y * sin_rot,
                    scaled.x * sin_rot + scaled.y * cos_rot
                };
                
                // Apply position and offset
                vector2 worldVert = rotated + GetGameObject()->Transform->GetWorldPosition() + Offsit;

                result.push_back(worldVert);
            }
        }

        return result;
    }

    std::vector<vector2> collider::GetSegmentWorldVertices(int SegmentIndex){
        std::vector<vector2> result;

        for (vector2 vert : ConvexPolygons[SegmentIndex].Vertices){
            // Apply scale
            vector2 scaled = vert * GetGameObject()->Transform->GetWorldScale();
            
            float angle = Math::Deg2Rad(-GetGameObject()->Transform->GetWorldRotation());  // Negate for clockwise
            float cos_rot = std::cos(angle);
            float sin_rot = std::sin(angle);
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

    std::vector<vector2> collider::GetBoundingBox(){
        Internal::boundingBox result;

        std::vector<vector2> Verts = GetWorldVertices();

        float MinX = Verts[0].x;
        float MaxX = Verts[0].x;
        float MinY = Verts[0].y;
        float MaxY = Verts[0].y;

        for (const vector2& point : Verts){
            if (point.x < MinX)
                MinX = point.x;

            if (point.x > MaxX)
                MaxX = point.x;

            if (point.y < MinY)
                MinY = point.y;

            if (point.y > MaxY)
                MaxY = point.y;
        }

        result.Min = {MinX, MinY};
        result.Max = {MaxX, MaxY};

        return {result.Min, result.Max};
    }
    
    std::vector<vector2> collider::SetPolygon(std::vector<vector2> poly){
        Polygon.Vertices.clear();
        ConvexPolygons.clear();

        Polygon.Vertices = poly;
        Internal::polygon newPolygon = Internal::polygon();
        newPolygon.Vertices = poly;

        ConvexPolygons = Internal::SplitIntoConvex(newPolygon);

        return poly;
    }   

    void collider::DebugDraw(window* Window){
        for (int i=0;i<ConvexPolygons.size();i++){
            std::vector<vector2> WorldVerts = GetSegmentWorldVertices(i);
            for (vector2& vert : WorldVerts)
                vert = Window->GetCamera()->WorldToScreenPosition(vert);

            int WorldSize = WorldVerts.size();
            SDL_SetRenderDrawColor(Window->GetSDLRenderer(), 
                (Uint8)(DebugColor.r * (i * 0.6)) % 256, 
                (Uint8)(DebugColor.g * (i * 0.3)) % 256, 
                (Uint8)(DebugColor.b * (i * 0.1)) % 256, 
                DebugColor.a
            );

            for (int j=0; j<WorldSize; j++)
                SDL_RenderLine(Window->GetSDLRenderer(), WorldVerts[j].x, WorldVerts[j].y,  WorldVerts[(j + 1) % WorldSize].x, WorldVerts[(j + 1) % WorldSize].y);
        }



        std::vector<vector2> boundingCorners = GetBoundingBox();

        Internal::boundingBox box = Internal::boundingBox(boundingCorners[0], boundingCorners[1]);

        // convert to screen space
        vector2 min = Window->GetCamera()->WorldToScreenPosition(box.Min);
        vector2 max = Window->GetCamera()->WorldToScreenPosition(box.Max);

        vector2 topLeft     = {min.x, max.y};
        vector2 topRight    = {max.x, max.y};
        vector2 bottomRight = {max.x, min.y};
        vector2 bottomLeft  = {min.x, min.y};

        SDL_SetRenderDrawColor(
            Window->GetSDLRenderer(),
            DebugBoundingBoxColor.r,
            DebugBoundingBoxColor.g,
            DebugBoundingBoxColor.b,
            DebugBoundingBoxColor.a
        );

        SDL_RenderLine(Window->GetSDLRenderer(), topLeft.x, topLeft.y, topRight.x, topRight.y);
        SDL_RenderLine(Window->GetSDLRenderer(), topRight.x, topRight.y, bottomRight.x, bottomRight.y);
        SDL_RenderLine(Window->GetSDLRenderer(), bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y);
        SDL_RenderLine(Window->GetSDLRenderer(), bottomLeft.x, bottomLeft.y, topLeft.x, topLeft.y);
    }

    void collider::Run(){
        DebugBoundingBoxColor = {0,0,255,255}; // default: no overlap

        for (gameObject* ActiveObject : GetGameObject()->GetScene()->GetActiveGameObjects()){
            collider* otherCol = ActiveObject->GetComponent<collider>();
            if (otherCol && ActiveObject != this->GetGameObject()){
                if (BoundingBoxOverlap(GetBoundingBox(), otherCol->GetBoundingBox())){
                    DebugBoundingBoxColor = {255,0,0,255};
                    break; // optional: no need to keep checking once we know we overlap
                }
            }
        }
    }
}