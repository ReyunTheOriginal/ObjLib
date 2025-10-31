#include "Core.hpp"

namespace obl{
    namespace Internal{
        void frames::Update(){
            //Calculate Accurate FPS
            TimeAdd += Time.DeltaTime;
            FrameCount++;

            if (TimeAdd >= 1.0){
                FPS = FrameCount;
                FrameCount = 0;
                TimeAdd = 0;
            }
            //Calculate QuickFPS
            QuickFPS = 1/Time.DeltaTime;
        }
    }//Internal
    Internal::frames Frames;
}//obl
