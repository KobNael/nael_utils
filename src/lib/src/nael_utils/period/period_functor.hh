/**
 * @file period_functor.hh
 */
#include <optional>
#include <nael_utils/period/period_typedef.hh>

namespace details
{
    /**
     * @struct MakeUnion
     * @brief Functor making the union of two (capa) periods
     */
    struct MakeUnion
    {
        /**
         * @brief Compute the union between two optional periods on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        time_period operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the union between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param period the time_period to consider
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the union between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
    };

    /**
     * @struct MakeDiff
     * @brief Functor making the difference of two (capa) periods
     */
    struct MakeDiff
    {
        /**
         * @brief Compute the difference between two optional periods on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the difference between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param period the time_period to consider
         * @return the resulting capa_period
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the difference between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the difference between an optional capa_period and an optional time_period on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param period the time_period to consider
         * @return the resulting capa_period
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const;
    };

    /**
     * @struct MakeInter
     * @brief Functor making the intersection of two (capa) periods
     */
    struct MakeInter
    {
        /**
         * @brief Compute the intersection between two optional periods on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the intersection between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param period the time_period to consider
         * @return the resulting capa_period
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the intersection between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param period the time_period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const;
        /**
         * @brief Compute the intersection between an optional capa_period and an optional time_period on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param period the time_period to consider
         * @return the resulting capa_period
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const;
    };

}//namespace details
