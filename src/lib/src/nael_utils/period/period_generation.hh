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
 * @param temporal_mesh the corresponding mesh
 * @param offset the offset to apply to the period
 * @pre end_horizon > start_horizon
 * @pre offset > -12:00:00
 * @pre offset < 12:00:00
 */
LTimePeriod
generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, TemporalMesh const &temporal_mesh, boost::posix_time::time_duration const &offset);
