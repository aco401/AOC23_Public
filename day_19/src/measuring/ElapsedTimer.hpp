#ifndef AOC_D19_ELAPSEDTIMER_H
#define AOC_D19_ELAPSEDTIMER_H

#include <cassert>
#include <chrono>

namespace d19 {

class ElapsedTimer {
    std::chrono::steady_clock::time_point point_start;
    std::chrono::steady_clock::time_point point_stop;

    size_t start_cnt;
    size_t stop_cnt;

  public:
    ElapsedTimer();

    /**
     * @brief To start a time measurement. 
     * 
     */
    void start();

    /**
     * @brief To end a time measurement.
     * 
     */
    void stop();

    /**
     * @brief To reset the object for a new measurement.
     * 
     */
    void clear();

    /**
     * @brief Returns the time (in milliseconds) between the start and stop call.
     * 
     * @return std::chrono::milliseconds
     * 
     * @exception std::domain_error if the start and stop functions were not used correctly. 
     * Correct usage means, calling @a start and @a stop only once and calling @a start first.
     * The second and onwards measurement has to be initiated with @a clear().
     */
    [[nodiscard]] std::chrono::milliseconds getElapsedTime();
};

} // namespace d19

#endif // AOC_D19_ELAPSEDTIMER_H