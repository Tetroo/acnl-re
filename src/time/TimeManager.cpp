#include "TimeManager.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>
#include <cmath>

namespace acnl::time {

TimeManager::TimeManager() {
    initializeWithSystemTime();
}

bool TimeManager::isLeapYear(uint16_t year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return (year % 4 == 0);
}

bool TimeManager::isPast6AmBoundary() const {
    return m_currentTime.hour >= 6;
}

uint8_t TimeManager::daysInMonth(uint16_t year, uint8_t month) {
    static const uint8_t daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    if (month >= 1 && month <= 12) return daysPerMonth[month - 1];
    return 30;
}

void TimeManager::initialize(const GameDateTime& initialTime) {
    m_currentTime = initialTime;
    m_wasBefore6Am = (m_currentTime.hour < 6);
    generateWeeklyTurnipForecast(0x12345678);
}

void TimeManager::initializeWithSystemTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm tmNow{};
#ifdef _WIN32
    localtime_s(&tmNow, &nowTime);
#else
    localtime_r(&nowTime, &tmNow);
#endif

    m_currentTime.year = static_cast<uint16_t>(tmNow.tm_year + 1900);
    // Clamp to ACNL boundaries [2012, 2050]
    if (m_currentTime.year < 2012) m_currentTime.year = 2012;
    if (m_currentTime.year > 2050) m_currentTime.year = 2050;

    m_currentTime.month = static_cast<uint8_t>(tmNow.tm_mon + 1);
    m_currentTime.day = static_cast<uint8_t>(tmNow.tm_mday);
    m_currentTime.day_of_week = static_cast<uint8_t>(tmNow.tm_wday);
    m_currentTime.hour = static_cast<uint8_t>(tmNow.tm_hour);
    m_currentTime.minute = static_cast<uint8_t>(tmNow.tm_min);
    m_currentTime.second = static_cast<uint8_t>(tmNow.tm_sec);
    m_currentTime.subsecond = 0;

    m_wasBefore6Am = (m_currentTime.hour < 6);
    generateWeeklyTurnipForecast(static_cast<uint32_t>(nowTime));
}

void TimeManager::tick(double deltaSeconds) {
    m_accumulatedSubseconds += deltaSeconds;
    if (m_accumulatedSubseconds >= 1.0) {
        int64_t fullSeconds = static_cast<int64_t>(m_accumulatedSubseconds);
        m_accumulatedSubseconds -= fullSeconds;
        advanceClockBySeconds(fullSeconds);
    }
}

void TimeManager::advanceClockBySeconds(int64_t seconds) {
    int64_t totalSec = m_currentTime.second + seconds;
    m_currentTime.second = static_cast<uint8_t>(totalSec % 60);
    int64_t totalMin = m_currentTime.minute + (totalSec / 60);
    m_currentTime.minute = static_cast<uint8_t>(totalMin % 60);
    int64_t totalHours = m_currentTime.hour + (totalMin / 60);

    bool hourChanged = (m_currentTime.hour != (totalHours % 24));
    uint8_t prevHour = m_currentTime.hour;
    m_currentTime.hour = static_cast<uint8_t>(totalHours % 24);

    // Check 6:00 AM boundary crossing
    if (hourChanged) {
        if (prevHour == 5 && m_currentTime.hour == 6) {
            triggerDailyRollover();
        }
    }

    int64_t extraDays = totalHours / 24;
    while (extraDays > 0) {
        uint8_t maxDays = daysInMonth(m_currentTime.year, m_currentTime.month);
        m_currentTime.day++;
        m_currentTime.day_of_week = (m_currentTime.day_of_week + 1) % 7;
        if (m_currentTime.day > maxDays) {
            m_currentTime.day = 1;
            m_currentTime.month++;
            if (m_currentTime.month > 12) {
                m_currentTime.month = 1;
                m_currentTime.year++;
                if (m_currentTime.year > 2050) {
                    m_currentTime.year = 2050; // ACNL hard clamp
                }
            }
        }
        extraDays--;
    }
}

void TimeManager::triggerDailyRollover() {
    m_dayNumber++;
    // If Sunday 6:00 AM, re-roll turnip price pattern
    if (m_currentTime.day_of_week == 0) {
        generateWeeklyTurnipForecast(0x56780000 + m_dayNumber);
    }

    // Notify all registered systems (flora growth, shop restock, daily fossil spawns)
    for (const auto& listener : m_rolloverListeners) {
        if (listener) {
            listener(m_dayNumber);
        }
    }
}

void TimeManager::generateWeeklyTurnipForecast(uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint16_t> buyDist(90, 110);
    std::uniform_int_distribution<uint32_t> patternDist(0, 3);

    m_turnipForecast.buy_price = buyDist(rng);
    m_turnipForecast.pattern = static_cast<TurnipPattern>(patternDist(rng));

    // Base price multiplier simulations based on reverse engineered patterns
    switch (m_turnipForecast.pattern) {
        case TurnipPattern::Fluctuating: {
            for (int i = 0; i < 12; ++i) {
                std::uniform_int_distribution<uint16_t> priceDist(50, 140);
                m_turnipForecast.sell_prices[i] = priceDist(rng);
            }
            break;
        }
        case TurnipPattern::LargeSpike: {
            int spikeIndex = 3 + (rng() % 5);
            uint16_t curr = m_turnipForecast.buy_price;
            for (int i = 0; i < 12; ++i) {
                if (i < spikeIndex) {
                    curr = static_cast<uint16_t>(curr * 0.95);
                    m_turnipForecast.sell_prices[i] = curr;
                } else if (i == spikeIndex) {
                    m_turnipForecast.sell_prices[i] = static_cast<uint16_t>(m_turnipForecast.buy_price * 2.0);
                } else if (i == spikeIndex + 1) {
                    m_turnipForecast.sell_prices[i] = static_cast<uint16_t>(m_turnipForecast.buy_price * 4.5); // Peak up to ~500-600
                } else {
                    m_turnipForecast.sell_prices[i] = static_cast<uint16_t>(m_turnipForecast.buy_price * 0.7);
                }
            }
            break;
        }
        case TurnipPattern::Decreasing: {
            uint16_t curr = m_turnipForecast.buy_price;
            for (int i = 0; i < 12; ++i) {
                curr = static_cast<uint16_t>(curr * 0.92);
                m_turnipForecast.sell_prices[i] = curr;
            }
            break;
        }
        case TurnipPattern::SmallSpike: {
            int spikeIndex = 4 + (rng() % 4);
            uint16_t curr = m_turnipForecast.buy_price;
            for (int i = 0; i < 12; ++i) {
                if (i == spikeIndex) {
                    m_turnipForecast.sell_prices[i] = static_cast<uint16_t>(m_turnipForecast.buy_price * 1.8); // Peak ~180-200
                } else {
                    curr = static_cast<uint16_t>(curr * 0.94);
                    m_turnipForecast.sell_prices[i] = curr;
                }
            }
            break;
        }
    }
}

uint16_t TimeManager::getCurrentTurnipPrice() const {
    if (m_currentTime.day_of_week == 0) {
        return m_turnipForecast.buy_price; // Sunday buy price
    }
    // Days 1..6 (Mon..Sat). Index: (day_of_week - 1)*2 + (hour >= 12 ? 1 : 0)
    int dayIdx = m_currentTime.day_of_week - 1;
    int halfDayIdx = dayIdx * 2 + (m_currentTime.hour >= 12 ? 1 : 0);
    if (halfDayIdx >= 0 && halfDayIdx < 12) {
        return m_turnipForecast.sell_prices[halfDayIdx];
    }
    return 0;
}

std::string TimeManager::formatDateTime() const {
    static const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    std::ostringstream ss;
    ss << m_currentTime.year << "-"
       << std::setfill('0') << std::setw(2) << static_cast<int>(m_currentTime.month) << "-"
       << std::setfill('0') << std::setw(2) << static_cast<int>(m_currentTime.day) << " ("
       << dayNames[m_currentTime.day_of_week % 7] << ") "
       << std::setfill('0') << std::setw(2) << static_cast<int>(m_currentTime.hour) << ":"
       << std::setfill('0') << std::setw(2) << static_cast<int>(m_currentTime.minute) << ":"
       << std::setfill('0') << std::setw(2) << static_cast<int>(m_currentTime.second);
    return ss.str();
}

} // namespace acnl::time
