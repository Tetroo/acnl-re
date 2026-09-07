#pragma once

#include <cstdint>
#include <chrono>
#include <string>
#include <vector>
#include <functional>
#include "../../types/TimeEngine.h"

namespace acnl::time {

class TimeManager {
public:
    TimeManager();

    // Initialize with standard epoch (2012-01-01) or specific datetime
    void initialize(const GameDateTime& initialTime);
    void initializeWithSystemTime();

    // Per-frame tick update (advances clock and checks rollover)
    void tick(double deltaSeconds);

    // Manual time travel adjustment
    bool setTime(const GameDateTime& newTime);
    void applyOffsetSeconds(int64_t offsetSeconds);

    // Getters
    [[nodiscard]] const GameDateTime& getCurrentTime() const { return m_currentTime; }
    [[nodiscard]] int64_t getUserOffsetSeconds() const { return m_userOffsetSeconds; }
    [[nodiscard]] bool isPast6AmBoundary() const;
    [[nodiscard]] uint32_t getDayNumber() const { return m_dayNumber; }

    // Turnip system
    [[nodiscard]] const TurnipForecast& getWeeklyTurnipForecast() const { return m_turnipForecast; }
    void generateWeeklyTurnipForecast(uint32_t seed);
    [[nodiscard]] uint16_t getCurrentTurnipPrice() const;

    // Daily 6:00 AM Rollover callbacks
    using RolloverCallback = std::function<void(uint32_t newDayNumber)>;
    void registerRolloverListener(RolloverCallback callback) {
        m_rolloverListeners.push_back(std::move(callback));
    }

    // Format time to string
    [[nodiscard]] std::string formatDateTime() const;

private:
    void triggerDailyRollover();
    void advanceClockBySeconds(int64_t seconds);
    static bool isLeapYear(uint16_t year);
    static uint8_t daysInMonth(uint16_t year, uint8_t month);

    GameDateTime m_currentTime{};
    GameDateTime m_lastSavedTime{};
    int64_t      m_userOffsetSeconds{0};
    double       m_accumulatedSubseconds{0.0};
    uint32_t     m_dayNumber{0};
    bool         m_wasBefore6Am{false};

    TurnipForecast m_turnipForecast{};
    std::vector<RolloverCallback> m_rolloverListeners;
};

} // namespace acnl::time
