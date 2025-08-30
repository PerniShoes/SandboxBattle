#pragma once
#include <SDL.h>
#include <vector>
#include <memory>
#include <array>

#include "utils.h"
#include "PrettyColors.h"

// HOW TO CHANGE PROJECT NAME:
// -Replace all "ProjectName" in ProjectName.h and in ProjectName.cpp (make sure #include in .cpp changes too)
// -Rename both files 
// -Replace all "ProjectName" in Game.h and in Game.cpp             .h->(Forward declaration, unique_ptr<ProjectName>) .cpp->(#include, make_unique<ProjectName>)
// -Replace all "m_ProjectName" in Game.h and in Game.cpp (optional)
// In case of issues all relevant lines, in Game.h/cpp files, are marked with "// Project"

class Time;
class Texture;

class ProjectName
{

public:
    ProjectName();
    ProjectName(const ProjectName& gol) = delete;
    ProjectName& operator=(const ProjectName& gol) = delete;
    ProjectName(ProjectName&& gol) = delete;
    ProjectName& operator=(ProjectName&& gol) = delete;

    ~ProjectName();

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

