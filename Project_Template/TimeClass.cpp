#include "TimeClass.h"

TimeClass::TimeClass(float timeGoal)
    :m_IsPaused{false}
    ,m_IsDone{false}
    ,m_AccumulatedTimeClass{0.0f}
    ,m_TimeClassGoal{timeGoal}
{
    if (m_TimeClassGoal < 0.0f)
    {
        m_TimeClassGoal = 0.0f;
    }
    else if (m_TimeClassGoal > 0.0f)
    {
        m_IsPaused = true;
    }
}

void TimeClass::Update(float elapsedTime)
{
    if (m_IsPaused == true)
        return;

    m_AccumulatedTimeClass += elapsedTime;
    if (m_AccumulatedTimeClass >= m_TimeClassGoal 
        && m_TimeClassGoal != 0.0f)
    {
        m_IsDone = true;
        m_IsPaused = true;
    }

}

void TimeClass::Start()
{
    m_IsPaused = false;
}

void TimeClass::Pause()
{
    m_IsPaused = true;
}

void TimeClass::Restart() 
{
    m_AccumulatedTimeClass = 0.0f;
    m_IsDone = false;
}

void TimeClass::RestartAndPause()
{
    Pause();
    Restart();
}

float TimeClass::GetTime() const
{
    return m_AccumulatedTimeClass;
}

float TimeClass::GetTimeClassrPercentage() const
{
    if (m_TimeClassGoal == 0.0f)
        return 0.0f;

    if (m_IsDone == true)
        return 1.0f;
   
    return m_AccumulatedTimeClass / m_TimeClassGoal;
}

void TimeClass::SetGoal(float timeGoal)
{
    if (timeGoal < 0.0f)
    {
        timeGoal = 0.0f;
    }
    m_TimeClassGoal = timeGoal;
}

bool TimeClass::IsDone() const
{
    return m_IsDone;
}