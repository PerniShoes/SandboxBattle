#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include <array>

#include "utils.h"
#include "PrettyColors.h"

// HOW TO CHANGE PROJECT NAME:
// -Replace all "SandoxBattler" in SandoxBattler.h and in SandoxBattler.cpp (make sure #include in .cpp changes too)
// -Rename both files 
// -Replace all "SandoxBattler" in Game.h and in Game.cpp             .h->(Forward declaration, unique_ptr<SandoxBattler>) .cpp->(#include, make_unique<SandoxBattler>)
// -Replace all "m_SandoxBattler" in Game.h and in Game.cpp (optional)
// In case of issues all relevant lines, in Game.h/cpp files, are marked with "// Project"

class Time;
class Texture;

class SandoxBattler
{

public:
    SandoxBattler();
    SandoxBattler(const SandoxBattler& gol) = delete;
    SandoxBattler& operator=(const SandoxBattler& gol) = delete;
    SandoxBattler(SandoxBattler&& gol) = delete;
    SandoxBattler& operator=(SandoxBattler&& gol) = delete;

    ~SandoxBattler();

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

};

