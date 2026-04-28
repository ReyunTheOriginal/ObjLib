#include "Camera.hpp"

namespace obj{
    camera* CreateCamera(){
        camera* Camera = new camera();
        return Camera;
    }
    void DestroyCamera(camera* Camera){
        delete Camera;
    }

    vector2 camera::SetResolution(const vector2& res){
        Resolution = res; 

        if (ActiveScene && !ActiveScene->Windows.empty()){
            for (window* win : ActiveScene->Windows){
                if (win && win->SDLrenderer) {
                    SDL_SetRenderLogicalPresentation(win->SDLrenderer, res.x, res.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
                }
            }
        }

        return Resolution;
    }

    vector2 Rotate(const vector2& RotateAround, float angle){
        float c = cos(angle);
        float s = sin(angle);

        return {
            RotateAround.x * c - RotateAround.y * s,
            RotateAround.x * s + RotateAround.y * c
        };
    }

    vector2 camera::ScreenToWorldPosition(const vector2& PosToTranslate) {
        if (Zoom <= 0) Zoom = 0.01f;
        vector2 result = PosToTranslate;

        vector2 res = GetResolution();
        result -= (res / 2);

        result.y *= -1;
        result /= Zoom;
        result = Rotate(result, Math::Deg2Rad(Rotation));
        result = result + Position;
        return result;
    }

    vector2 camera::WorldToScreenPosition(const vector2& PosToTranslate) {
        vector2 result = PosToTranslate - Position;
        result = Rotate(result, -Math::Deg2Rad(Rotation));
        result = result * Zoom;
        result.y *= -1;

        vector2 res = GetResolution();
        result += (res / 2);

        return result;
    }
}