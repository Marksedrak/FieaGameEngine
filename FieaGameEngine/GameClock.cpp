#include "pch.h"
#include "GameClock.h"
#include <assert.h>

using namespace std::chrono;

namespace Fiea::GameEngine
{
    GameClock::GameClock(now_func now) : _now(now)
    {
        // if this was not overridden in the member initialization, use the provided
        //  system clock, by default (typical case)
        if (_now == nullptr) {
            _now = high_resolution_clock::now;
        }
        assert(_now != nullptr);

        _startTime = _now();
    }

    GameTime GameClock::Current() const {
        GameTime time;
        time._start = time._start = _startTime;
        time._current = _now();
        return time;
    }

    GameTime::Millis GameClock::Elapsed(const GameTime& time) const
    {
        return duration_cast<milliseconds>(_now() - time._last).count();
    }

    void GameClock::Update(GameTime& time) const
    {
        time._last = time._current;
        time._current = _now();
    }
}