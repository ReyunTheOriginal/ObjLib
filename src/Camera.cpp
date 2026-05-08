#include "Camera.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "EssentialProcesses.hpp"
#include "UI/Canvas.hpp"

namespace obj{
    camera* CreateCamera(window* Window){
        camera* cam = new camera();
        cam->ActiveWindow = Window;
        return cam;
    }

    UI::canvas* camera::SetCanvas(UI::canvas* Canvas){
        if (!Canvas)return nullptr;

        Canvas->Camera = this;
        ActiveCanvas = Canvas;
        return Canvas;
    }

    vector2 camera::SetResolution(const vector2& res){
        //if (Resolution.x == res.x && Resolution.y == res.y) return Resolution; // Skip if unchanged
        
        Resolution = res;

        if (ActiveWindow && ActiveWindow->GetSDLRenderer()) {
            SDL_SetRenderLogicalPresentation(ActiveWindow->GetSDLRenderer(), res.x, res.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);
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
        result /= (Zoom * PixelsPerUnit);
        result = Rotate(result, Math::Deg2Rad(Rotation));
        result = result + Position;
        return result;
    }

    vector2 camera::WorldToScreenPosition(const vector2& PosToTranslate) {
        vector2 result = PosToTranslate - Position;
        result = Rotate(result, -Math::Deg2Rad(Rotation));
        result *= (Zoom * PixelsPerUnit);
        result.y *= -1;

        vector2 res = GetResolution();
        result += (res / 2);

        return result;
    }
}