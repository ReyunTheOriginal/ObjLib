#include "Camera/Camera.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "EssentialProcesses.hpp"
#include "UI/Canvas.hpp"
#include "Rendering/Rendering.hpp"

namespace obj{
    void camera::OnCanvasChange(){}

    camera* CreateCamera(window* Window){
        camera* cam = new camera();
        cam->ActiveWindow = Window;
        return cam;
    }

    UI::canvas* camera::SetCanvas(UI::canvas* CanvasToSet){
        if (CanvasToSet != ActiveCanvas){
            // Unset and unload old canvas if it exists
            if (ActiveCanvas){
                ActiveCanvas->OnCanvasUnSet();
                ActiveCanvas->Camera = nullptr;
                ActiveCanvas->OnCanvasUnLoad();
            }

            // Load and set new canvas if it exists
            if (CanvasToSet){
                if (!CanvasToSet->Camera)
                    CanvasToSet->OnCanvasLoad();

                CanvasToSet->Camera = this;
                CanvasToSet->OnCanvasSet();
            }

            OnCanvasChange();
        }

        ActiveCanvas = CanvasToSet;
        return CanvasToSet;
    }

    vector2 Rotate(const vector2& RotateAround, float angle){
        angle = -angle;  // Negate for clockwise rotation
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

        vector2 res = Internal::Renderer->GetResolution(ActiveWindow);
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

        vector2 res = Internal::Renderer->GetResolution(ActiveWindow);
        result += (res / 2);

        return result;
    }
}