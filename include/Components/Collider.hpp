#pragma once

#include "Component.hpp"
#include "GlobalTypes.hpp"
#include "Polygon.hpp"

namespace obj{
    struct collider : component{
        private:
        Internal::polygon Polygon;
        std::vector<Internal::polygon> ConvexPolygons;

        public:
        vector2 Offsit = {0,0};

        color DebugColor = {0,255,0,120};
        color DebugBoundingBoxColor = {0,0,255,120};

        std::vector<Internal::polygon> GetConvexSegments(){return ConvexPolygons;}

        std::vector<vector2> GetPolygon(){return Polygon.Vertices;}
        std::vector<vector2> SetPolygon(std::vector<vector2> poly);

        std::vector<vector2> GetWorldVertices();
        std::vector<vector2> GetSegmentWorldVertices(int SegmentIndex);
        std::vector<vector2> GetBoundingBox();

        void DebugDraw(window* Window) override;

        void Run() override;

        collider(){
            SetPolygon({
                {-0.5, -0.5},
                {-0.5,  0.5},
                { 0.5,  0.5},
                { 0.5, -0.5} 
            });
        }
    };

    struct collisionInfo{
        collider* Collider = nullptr;
        collider* OtherCollider = nullptr;

        vector2 CollisionNormal = {0,0};
        float CollisionDepth = 0;

    };
}

