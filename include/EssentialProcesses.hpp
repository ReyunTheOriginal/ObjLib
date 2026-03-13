#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "GlobalLists.hpp"
#include "Scene.hpp"
#include "GlobalTypes.hpp"
#include "Sprite.hpp"
#include "Components.hpp"
#include "Gameobject.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Camera.hpp"

namespace obj{
    int  Init(); //Start up all necessary code to prepare the program
    void Update(); //update processes like physics, positions, and math
    void Render(); //Render all renderable objects
    void Quit(); //safely quit the entire program
}