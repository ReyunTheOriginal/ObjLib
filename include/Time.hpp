#pragma once

#include "SDL3/SDL.h"
#include "Framerate.hpp"

namespace obj{
    namespace Time{
        extern float TimeScale;
        extern float DeltaTime;
        extern float RealTime;
        extern float ElapsedTime;
        
        void Update();
    }//Time
}//obj