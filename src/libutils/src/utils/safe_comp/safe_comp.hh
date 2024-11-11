#pragma once

/**
 *@file safe_comp.hh
 *@brief (safe) floating point comparator
 */

#include <type_traits>
#include <cmath>
#include <limits>
#include <stdexcept>

/**
 * @namespace safecomp
 * @brief Contains a complete list of safe numercial comparison, eg. :
 * @code{cpp}
 * ASSERT_TRUE( safecomp::eq(1e-6, 0.) );
 * ASSERT_FALSE( safecomp::gt(1e-6, 0.) );
 * @endcode
 */
namespace safecomp
{
/////////////////
// Basic types //
/////////////////
    /**\return lhs_p is < rhs_p -\f$\epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool lt(T lhs_p, T rhs_p)
    {
        return lhs_p < rhs_p;
    }

    /**\return lhs_p is > rhs_p +\f$\epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool gt(T lhs_p, T rhs_p)
    {
        return lhs_p > rhs_p;
    }

    /**\return |lhs_p - rhs_p| \f$\leq \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool eq(T lhs_p, T rhs_p)
    {
        return lhs_p == rhs_p;
    }

    /**\return lhs_p + rhs_p \f$\leq \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool le(T lhs_p, T rhs_p)
    {
        return lhs_p <= rhs_p;
    }

    /**\return lhs_p - rhs_p \f$\geq - \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool ge(T lhs_p, T rhs_p)
    {
        return lhs_p >= rhs_p;
    }

    /**\return lhs_p - rhs_p > \f$ \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<!std::is_floating_point<T>::value, T>::type>
    inline bool neq(T lhs_p, T rhs_p)
    {
        return !eq(lhs_p, rhs_p);
    }

///////////////////
// Numeric types //
///////////////////
    /**\return the closest integer value of p */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline T round(T val_p) {return floor(val_p + 0.5);}

    /**\return lhs_p is < rhs_p -\f$\epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool lt(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return lhs_p + epsilon_p < rhs_p;
    }

    /**\return lhs_p is > rhs_p +\f$\epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool gt(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return lhs_p - epsilon_p > rhs_p;
    }

    /**\return |lhs_p - rhs_p| \f$\leq \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool eq(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return fabs(lhs_p - rhs_p) <= epsilon_p;
    }

    /**\return lhs_p + rhs_p \f$\leq \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool le(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return lhs_p <= rhs_p + epsilon_p;
    }

    /**\return lhs_p - rhs_p \f$\geq - \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool ge(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return lhs_p >= rhs_p - epsilon_p;
    }

    /**\return lhs_p - rhs_p > \f$ \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool neq(T lhs_p, T rhs_p, T epsilon_p=1e-6)
    {
        return !eq(lhs_p,rhs_p, epsilon_p);
    }

    /**\return lhs_p \f$\in [-\epsilon, \epsilon ]\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool isnull(T val_p, T epsilon_p=1e-6)
    {
        return (val_p >= -epsilon_p) && (val_p <= epsilon_p);
    }

    /**\return lhs_p < \f$ -\epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool isneg(T val_p, T epsilon_p=1e-6)
    {
        return lt(val_p, 0, epsilon_p);
    }

    /**\return lhs_p > \f$ \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool ispos(T val_p, T epsilon_p=1e-6)
    {
        return gt(val_p, 0, epsilon_p);
    }

    /**\return is the difference between lhs_p and the closest integer \f$\leq \epsilon\f$ */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool isint(T val_p, T epsilon_p=1e-6)
    {
        return eq(round(val_p), val_p, epsilon_p);
    }

    /**\return true if val_p is not a number */
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline bool isnan(T val_p) {
        volatile double tmp_l = val_p;
        return val_p == std::numeric_limits<T>::quiet_NaN() || tmp_l != val_p;
    }

    /**\return true if val_p represents infinity*/
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    inline int isinf(T val_p) {
        volatile double tmp_l = val_p;
        if ((tmp_l == val_p) && ((tmp_l - val_p) != 0.0))
            return (val_p < 0.0 ? -1 : 1);
        else return 0;
    }
}
