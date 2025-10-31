#pragma once

namespace obl{
    namespace Internal{
        struct frames{
            private:
            double FrameCount = 0;
            double TimeAdd = 0;

            public:
            int FPS = 0;
            double QuickFPS = 0;
            
            void Update();
        };
    }//Internal
    extern Internal::frames Frames;
}//obl