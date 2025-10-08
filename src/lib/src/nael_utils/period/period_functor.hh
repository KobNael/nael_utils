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
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        time_period operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the union between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the union between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the union between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a capa_period with the sum of the capacity (default value being 0)
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the union between two optional ratio_periods on a given period
         * @param p1 the first ratio_period
         * @param p2 the second ratio_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a ratio_period with the sum of the ratios (the default value being 0.)
         */
        std::optional<ratio_period> operator()(std::optional<ratio_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the union between an optional ratio_period and an optional time_period on a given period
         * @param p1 the first ratio_period
         * @param p2 the second time_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a ratio_period with the ratio of p1 if defined, a ratio_period with ratio 1. if p2 is defined, else nothing
         */
        std::optional<ratio_period> operator()(std::optional<ratio_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
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
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the difference between an optional time_period and an optional ratio_period on a given period
         * @param p1 the time_period
         * @param p2 the ratio_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the difference between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the difference between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a capacity period with the difference between the first capa and the second one (default value being 0)
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the difference between an optional capa_period and an optional time_period on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a capa_period with the capacity of p1 if p2 is not defined
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the difference between an optional ratio_period and an optional time_period on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a ratio_period with the ratio of p1 if p2 is not defined
         */
        std::optional<ratio_period> operator()(std::optional<ratio_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
    };

    /**
     * @struct MakeInter
     * @brief Functor making the intersection of two (capa) periods
     */
    struct MakeInter
    {
        /**
         * @brief Should we keep the period with only one time stamp
         */
        bool _keep_empty;

        /**
         * @brief Constructor
         * @param keep_empty Should we keep the period with only one time stamp
         */
        explicit MakeInter(bool keep_empty) : _keep_empty(keep_empty)
        {
        }

        /**
         * @brief Compute the intersection between two optional periods on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between an optional time_period and an optional capa_period on a given period
         * @param p1 the time_period
         * @param p2 the capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between an optional time_period and an optional ratio_period on a given period
         * @param p1 the time_period
         * @param p2 the ratio_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return the resulting time_period
         */
        std::optional<time_period> operator()(std::optional<time_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between two optional capa_periods on a given period
         * @param p1 the first capa_period
         * @param p2 the second capa_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a capa_period with the minimal capacity (default value being 0)
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between two optional ratio_periods on a given period
         * @param p1 the first ratio_period
         * @param p2 the second ratio_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a ratio period with the minimal ratio (default value being 0.)
         */
        std::optional<ratio_period> operator()(std::optional<ratio_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between an optional capa_period and an optional time_period on a given period
         * @param p1 the first period
         * @param p2 the second period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a capa period with the capacity of p1 if p2 is defined, nullopt otherwise
         */
        std::optional<capa_period> operator()(std::optional<capa_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
        /**
         * @brief Compute the intersection between an optional ratio_period and an optional time_period on a given period
         * @param p1 the ratio_period
         * @param p2 the time_period
         * @param from the starting ptime of the period to consider
         * @param to the ending ptime of the period to consider
         * @return a ratio period with the ratio of p1 if p2 is defined, nullopt otherwise
         */
        std::optional<ratio_period> operator()(std::optional<ratio_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const;
    };

} // namespace details
