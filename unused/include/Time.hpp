#pragma once

#include <SDL3/SDL.h>

namespace obj{
    namespace Internal{
        struct time{
            double DeltaTime;
            float TimeScale = 1;

            Uint64 now = SDL_GetPerformanceCounter();
            Uint64 last = SDL_GetPerformanceCounter();

            void Update();

            void CalculateDeltatime();
        };
    }//Internal

    extern Internal::time Time;
}//obl
    