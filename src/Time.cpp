#include "Time.hpp"

namespace obj{
    namespace Time{
        float TimeScale = 1;
        float DeltaTime = 0.016;
        float RealTime = 0.016;

        Uint64 last = SDL_GetPerformanceCounter();
        Uint64 perfFrequency = SDL_GetPerformanceFrequency();

        namespace Internal{
            double FrameTimer = 0;
            int FrameCount = 0;
        }
        
        void Update(){
            Uint64 now = SDL_GetPerformanceCounter();

            RealTime = ((double)(now - last) / perfFrequency);
            DeltaTime = RealTime * TimeScale;

            // Frame rate limiting
            if (::obj::FPS::Internal::TargetFrameTime > 0 && RealTime < ::obj::FPS::Internal::TargetFrameTime){
                float sleepTime = ::obj::FPS::Internal::TargetFrameTime - RealTime;
                SDL_DelayNS((Uint64)(sleepTime * 1e9)); // Convert to nanoseconds
                
                // Recalculate after sleep
                now = SDL_GetPerformanceCounter();
                RealTime = ((double)(now - last) / perfFrequency);
                DeltaTime = RealTime * TimeScale;
            }

            ::obj::FPS::ExactFPS = 1/DeltaTime;
            
            Internal::FrameCount++;
            Internal::FrameTimer += DeltaTime;

            if (Internal::FrameTimer >= 1.0){
                ::obj::FPS::FPS = Internal::FrameCount;
                Internal::FrameCount = 0;
                Internal::FrameTimer = 0;
            }

            last = now;
        }
    }//Time
}//obj