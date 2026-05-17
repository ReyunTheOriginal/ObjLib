#pragma once

#include <vector>
#include "Math.hpp"

namespace obj{
    namespace Internal{
        struct boundingBox{
            vector2 Min;
            vector2 Max;

            boundingBox(vector2 min = {0,0}, vector2 max = {0,0}){
                Min = min;
                Max = max;
            }

        };

        struct polygon{
            std::vector<vector2> Vertices;

            bool isConvex();

            polygon(std::vector<vector2> Verts = { {-0.5,-0.5}, {-0.5,0.5}, {0.5,0.5}, {0.5,-0.5} }){
                Vertices = Verts;
            }
        };

        std::vector<polygon*> SplitIntoConvex(Internal::polygon* Poly);
    }

    inline bool pointInPolygon(const std::vector<vector2>& poly, vector2 p) {
        int n = poly.size();
        int crossings = 0;
        for (int i = 0; i < n; i++) {
            vector2 a = poly[i];
            vector2 b = poly[(i + 1) % n];
            if ((a.y <= p.y && b.y > p.y) || (b.y <= p.y && a.y > p.y)) {
                float t = (p.y - a.y) / (b.y - a.y);
                if (p.x < a.x + t * (b.x - a.x))
                    crossings++;
            }
        }
        return crossings % 2 == 1;
    }

    bool BoundingBoxOverlap(const std::vector<vector2>& A, const std::vector<vector2>& B);
}