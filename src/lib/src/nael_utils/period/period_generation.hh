/**
 * @file period_generation.hh
 * @brief Toolbox for generation of time periods.
 */

#pragma once

#include <nael_utils/period/period_utils.hh>

/**
 * @brief Generate the time period defined by a couple start/end on an horizon
 * @param start_horizon the starting date time of the horizon
 * @param end_horizon the ending date time of the horizon
 * @param start_period the starting time of the period
 * @param end_period the ending time of the period
 * @pre end_horizon > start_horizon
 * @pre end_horizon and start_horizon are valid date times
 * @pre 00:00:00 <= start_period <= 23:59:59
 * @pre 00:00:00 <= end_period <= 23:59:59
 */
LTimePeriod
generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, boost::posix_time::time_duration const &start_period, boost::posix_time::time_duration const &end_period);

/**
 * @brief Generate the time period defined by a couple start/end on an horizon
 * @param start_horizon the starting date time of the horizon
 * @param end_horizon the ending date time of the horizon
 * @param temporal_mesh the corresponding mesh
 * @pre end_horizon > start_horizon
 */
LTimePeriod
generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, TemporalMesh const &temporal_mesh);

/**
 * @brief Generate the capacitated time period defined by a couple start/end on an horizon
 * @param start_horizon the starting date time of the horizon
 * @param end_horizon the ending date time of the horizon
 * @param start_period the starting time of the period
 * @param end_period the ending time of the period
 * @param capa the capacity of the periods
 * @pre start_horizon < end_horizon
 * @pre end_horizon and start_horizon are valid date times
 * @pre 00:00:00 <= start_period <= 23:59:59
 * @pre 00:00:00 <= end_period <= 23:59:59
 */
LCapaPeriod
generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, boost::posix_time::time_duration const &start_period, boost::posix_time::time_duration const &end_period, int64_t capa);

