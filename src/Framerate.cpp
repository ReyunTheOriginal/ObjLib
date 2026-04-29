#include "Framerate.hpp"

namespace obj{
    namespace FPS{
        double ExactFPS = 0;
        int FPS = 0;
        
        namespace Internal{
            float TargetFrameTime = 0; // 0 = uncapped
        }//Internal

        void SetTargetFrameRate(int fps){
            if (fps <= 0){
                Internal::TargetFrameTime = 0; // Uncapped
            } else {
                Internal::TargetFrameTime = 1.0f / fps;
            }
        }

    }//FPS
}//obj