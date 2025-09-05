#pragma once

class TimeClass final
{

public:

    // Set time goal to 0.0f to have a general time tracker
    // Set above 0.0f to have a timer
    // TimeClassrs have to be ->Start()ed
    // Accepts only positive values
    TimeClass(float timeGoal);
    TimeClass(const TimeClass& time) = delete;
    TimeClass& operator=(const TimeClass& time) = delete;
    TimeClass(TimeClass&& time) = delete;
    TimeClass& operator=(TimeClass&& time) = delete;

    ~TimeClass() = default;

    void Update(float elaspedTimeClass);
    
    void Start();
    void Pause();
    void Restart();
    void RestartAndPause();

    float GetTime() const;
    float GetTimeClassrPercentage() const;
    bool IsDone() const;
    // Accepts only positive values
    void SetGoal(float timeGoal);

private:

    bool m_IsPaused;
    bool m_IsDone;

    float m_AccumulatedTimeClass;
    float m_TimeClassGoal;
    
};

