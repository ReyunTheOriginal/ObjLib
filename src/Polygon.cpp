#include "Polygon.hpp"

#include <stack>
#include <limits>

namespace obj{
    bool BoundingBoxOverlap(const std::vector<vector2>& A, const std::vector<vector2>& B){
        return (
            A[0].x < B[1].x &&
            A[1].x > B[0].x &&
            A[0].y < B[1].y &&
            A[1].y > B[0].y
        );
    }

    namespace Internal{
        bool isReflex(const std::vector<vector2>& verts, int i) {
            int n = verts.size();
            vector2 a = verts[(i - 1 + n) % n];
            vector2 b = verts[i];
            vector2 c = verts[(i + 1) % n];
            return Math::cross(a, b, c) > 0;
        }

        bool onSegment(vector2 a, vector2 b, vector2 q) {
            return std::min(a.x, b.x) <= q.x && q.x <= std::max(a.x, b.x)
                && std::min(a.y, b.y) <= q.y && q.y <= std::max(a.y, b.y);
        }

        bool segmentsIntersect(vector2 p1, vector2 p2, vector2 p3, vector2 p4) {
            float d1 = Math::cross(p3, p4, p1);
            float d2 = Math::cross(p3, p4, p2);
            float d3 = Math::cross(p1, p2, p3);
            float d4 = Math::cross(p1, p2, p4);

            if (d1 * d2 < 0 && d3 * d4 < 0)
                return true;

            if (d1 == 0 && onSegment(p3, p4, p1)) return true;
            if (d2 == 0 && onSegment(p3, p4, p2)) return true;
            if (d3 == 0 && onSegment(p1, p2, p3)) return true;
            if (d4 == 0 && onSegment(p1, p2, p4)) return true;

            return false;
        }

        bool diagonalClear(const polygon* poly, int i, int j) {
            int n = poly->Vertices.size();
            vector2 a = poly->Vertices[i];
            vector2 b = poly->Vertices[j];

            for (int k = 0; k < n; k++) {
                int next = (k + 1) % n;
                if (k == i || k == j || next == i || next == j) continue;
                if (segmentsIntersect(a, b, poly->Vertices[k], poly->Vertices[next]))
                    return false;
            }
            return true;
        }

        // Simplified: rely on midpoint-inside test + diagonalClear.
        // The old wedge test was too strict and winding-dependent.
        bool diagonalValid(const polygon* poly, int i, int j) {
            if (!diagonalClear(poly, i, j)) return false;

            vector2 mid = {
                (poly->Vertices[i].x + poly->Vertices[j].x) * 0.5f,
                (poly->Vertices[i].y + poly->Vertices[j].y) * 0.5f
            };
            return pointInPolygon(poly->Vertices, mid);
        }

        int findSplitTarget(const polygon* poly, int reflexIdx) {
            int n = poly->Vertices.size();
            int best = -1;

            for (int j = 0; j < n; j++) {
                if (j == reflexIdx) continue;
                if (j == (reflexIdx - 1 + n) % n) continue;
                if (j == (reflexIdx + 1) % n) continue;

                if (!diagonalValid(poly, reflexIdx, j)) continue;

                if (best == -1) { best = j; continue; }

                // Bug 3 fix: prefer convex targets over reflex ones
                bool jIsReflex    = isReflex(poly->Vertices, j);
                bool bestIsReflex = isReflex(poly->Vertices, best);
                if (!jIsReflex && bestIsReflex) best = j;
            }

            return best;
        }

        int findSteinerPoint(const polygon* poly, int reflexIdx, vector2& outPoint) {
            int n = poly->Vertices.size();
            vector2 origin = poly->Vertices[reflexIdx];

            float bestT = std::numeric_limits<float>::max();
            int bestEdge = -1;
            vector2 bestPt = {0, 0};

            // Cast ray in +X direction from the reflex vertex
            vector2 dir = {1.0f, 0.0f};

            for (int k = 0; k < n; k++) {
                int kn = (k + 1) % n;
                if (k == reflexIdx || kn == reflexIdx) continue;
                if (k == (reflexIdx - 1 + n) % n) continue;
                if (kn == (reflexIdx + 1) % n) continue;

                vector2 e0 = poly->Vertices[k];
                vector2 e1 = poly->Vertices[kn];

                float ex = e1.x - e0.x;
                float ey = e1.y - e0.y;
                float denom = dir.x * ey - dir.y * ex;
                if (std::abs(denom) < 1e-9f) continue;

                float t = ((e0.x - origin.x) * ey - (e0.y - origin.y) * ex) / denom;
                float s = ((e0.x - origin.x) * dir.y - (e0.y - origin.y) * dir.x) / denom;

                if (t > 1e-6f && s >= 0.0f && s <= 1.0f) {
                    if (t < bestT) {
                        bestT = t;
                        bestEdge = k;
                        bestPt = {origin.x + t * dir.x, origin.y + t * dir.y};
                    }
                }
            }

            if (bestEdge == -1) {
                // +X failed, try -X
                dir = {-1.0f, 0.0f};
                for (int k = 0; k < n; k++) {
                    int kn = (k + 1) % n;
                    if (k == reflexIdx || kn == reflexIdx) continue;
                    if (k == (reflexIdx - 1 + n) % n) continue;
                    if (kn == (reflexIdx + 1) % n) continue;

                    vector2 e0 = poly->Vertices[k];
                    vector2 e1 = poly->Vertices[kn];

                    float ex = e1.x - e0.x;
                    float ey = e1.y - e0.y;
                    float denom = dir.x * ey - dir.y * ex;
                    if (std::abs(denom) < 1e-9f) continue;

                    float t = ((e0.x - origin.x) * ey - (e0.y - origin.y) * ex) / denom;
                    float s = ((e0.x - origin.x) * dir.y - (e0.y - origin.y) * dir.x) / denom;

                    if (t > 1e-6f && s >= 0.0f && s <= 1.0f) {
                        if (t < bestT) {
                            bestT = t;
                            bestEdge = k;
                            bestPt = {origin.x + t * dir.x, origin.y + t * dir.y};
                        }
                    }
                }
            }

            if (bestEdge == -1) return -1;

            // Prefer an endpoint of the hit edge if it forms a valid diagonal,
            // to avoid creating unnecessary extra vertices
            int kn = (bestEdge + 1) % n;
            if (diagonalValid(poly, reflexIdx, bestEdge))
                return bestEdge; // signal to use existing vertex instead
            if (diagonalValid(poly, reflexIdx, kn))
                return kn;

            outPoint = bestPt;
            return bestEdge + n; // offset to signal "insert new point" vs "use existing vertex"
        }

        // Insert a Steiner point after edge index `edgeIdx`, return new polygon
        polygon* insertVertex(const polygon* poly, int edgeIdx, vector2 pt) {
            polygon* result = new polygon;
            int n = poly->Vertices.size();
            for (int i = 0; i <= edgeIdx; i++)
                result->Vertices.push_back(poly->Vertices[i]);
            result->Vertices.push_back(pt);
            for (int i = edgeIdx + 1; i < n; i++)
                result->Vertices.push_back(poly->Vertices[i]);
            return result;
        }

        std::pair<polygon*, polygon*> splitPolygon(const polygon* poly, int i, int j) {
            int n = poly->Vertices.size();
            polygon* A = new polygon;
            polygon* B = new polygon;

            A->Vertices.clear();
            B->Vertices.clear();

            // A: from i to j (inclusive)
            for (int k = i; ; k = (k + 1) % n) {
                A->Vertices.push_back(poly->Vertices[k]);
                if (k == j) break;
            }

            // B: from j to i (inclusive)
            for (int k = j; ; k = (k + 1) % n) {
                B->Vertices.push_back(poly->Vertices[k]);
                if (k == i) break;
            }

            return {A, B};
        }

        bool polygon::isConvex(){
            int n = Vertices.size();
            for (int i = 0; i < n; i++) {
                if (isReflex(Vertices, i)) return false;
            }
            return true;
        }

        float signedArea(const polygon* poly) {
            float area = 0;
            int n = poly->Vertices.size();
            for (int i = 0; i < n; i++) {
                vector2 a = poly->Vertices[i];
                vector2 b = poly->Vertices[(i + 1) % n];
                area += (b.x - a.x) * (b.y + a.y);
            }
            return area;
        }

        std::vector<polygon*> SplitIntoConvex(polygon* Poly) {
            std::vector<polygon*> result;
            std::stack<polygon*> toProcess;
            toProcess.push(Poly);

            while (!toProcess.empty()) {
                polygon* current = toProcess.top(); toProcess.pop();

                if (current->isConvex()) {
                    result.push_back(current);
                    continue;
                }

                // Find first reflex vertex
                int reflexIdx = -1;
                for (int i = 0; i < (int)current->Vertices.size(); i++) {
                    if (isReflex(current->Vertices, i)) { reflexIdx = i; break; }
                }

                int target = findSplitTarget(current, reflexIdx);

                if (target != -1) {
                    auto [A, B] = splitPolygon(current, reflexIdx, target);
                    toProcess.push(A);
                    toProcess.push(B);
                    delete current;
                } else {
                    // No existing vertex works — insert a Steiner point on the boundary
                    vector2 steinerPt;
                    int edgeIdx = findSteinerPoint(current, reflexIdx, steinerPt);

                    if (edgeIdx != -1) {
                        if (edgeIdx < (int)current->Vertices.size()) {
                            // An existing endpoint works — split directly to it
                            auto [A, B] = splitPolygon(current, reflexIdx, edgeIdx);
                            toProcess.push(A);
                            toProcess.push(B);
                            delete current;
                        } else {
                            // Insert a new vertex
                            int realEdge = edgeIdx - (int)current->Vertices.size();
                            polygon* expanded = insertVertex(current, realEdge, steinerPt);
                            toProcess.push(expanded);
                            delete current;
                        }
                    } else {
                        result.push_back(current); // truly degenerate
                    }
                }
            }
            return result;
        }
    }
}