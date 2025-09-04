#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include <array>

#include "utils.h"
#include "PrettyColors.h"

class Time;
class Texture;

class SandboxBattler
{

public:
    SandboxBattler();
    SandboxBattler(const SandboxBattler& other) = delete;
    SandboxBattler& operator=(const SandboxBattler& other) = delete;
    SandboxBattler(SandboxBattler&& other) = delete;
    SandboxBattler& operator=(SandboxBattler&& other) = delete;

    ~SandboxBattler();

    void Draw() const;
    void DrawUI(Rectf viewport) const; 

    void Update(float elapsedTime);
    void UpdateTextures(); 

    void OnMouseUp();
    void OnMouseDown();
    void OnMouseMotion(const Point2f& mousePos);  
    // Instead of having functions fire off at key/button press, have them fire off at release 
    // (like Pyke Q, press = start charge, hold = keep charging, release = fire off)
    // If you don't need charging/holding then you can just do it on press

private:
    // Add FIX to things that need to be fixed, later check FIX in search tool
    // Same with DELETE

    void Cleanup();

    // Mouse
    Point2f m_LastMousePos; 
    std::unique_ptr<Time> m_LBHeldTimer; 


    // New code


};

