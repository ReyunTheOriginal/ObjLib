#pragma once

#include "Component.hpp"
#include "GlobalTypes.hpp"
#include "Polygon.hpp"
#include <memory>

namespace obj{
    struct collider : component{
        private:
        Internal::polygon* Polygon = nullptr;
        std::vector<Internal::polygon*> ConvexPolygons;

        public:
        vector2 Offsit = {0,0};
        bool IsTrigger = false;

        color DebugColor = {0,255,0,120};
        color DebugBoundingBoxColor = {0,0,255,120};

        std::vector<std::vector<vector2>> GetConvexSegments();

        std::vector<vector2> GetPolygon(){return Polygon->Vertices;}
        std::vector<vector2> SetPolygon(std::vector<vector2> poly);

        std::vector<vector2> GetWorldVertices();
        std::vector<vector2> GetSegmentWorldVertices(int SegmentIndex);
        std::vector<vector2> GetBoundingBox();

        void DebugDraw(window* Window) override;

        collider(){
            SetPolygon({
                {-0.5, -0.5},
                {-0.5,  0.5},
                { 0.5,  0.5},
                { 0.5, -0.5} 
            });
        }

        ~collider(){
            if (Polygon) delete Polygon;

            for (Internal::polygon* poly : ConvexPolygons)
                if (poly) delete poly;
        }
    };

    struct collisionInfo {
        collider* Collider = nullptr;
        collider* OtherCollider = nullptr;

        vector2 CollisionNormal = {0, 0}; // The facing direction of the hit between colliders
        float CollisionDepth = 0.0f;
        vector2 SeparationVector = {0,0};

        std::vector<vector2> ContactPoints; // Exact world space location(s) of impact

        // State Flag
        bool IsTrigger = false;    
    };

}

