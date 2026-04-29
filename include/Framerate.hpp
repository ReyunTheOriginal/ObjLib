#pragma once

namespace obj{
    namespace FPS{
        extern double ExactFPS;
        extern int FPS;
        
        namespace Internal{
            extern float TargetFrameTime; // Target frame time in seconds (0 = uncapped)
        }

        void SetTargetFrameRate(int fps); // Set target FPS (0 = uncapped)
    }//FPS
}//obj