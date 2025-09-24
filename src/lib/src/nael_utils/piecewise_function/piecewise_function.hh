/**
 * @file piecewise_function.hh
 * @brief Interface for piecewise function
 */
#pragma once

#include <list>
#include <ostream>

/**
 * @struct Dot
 * @brief Structure defining a point (dot) in a piece-wise function
 */
struct Dot
{
    long double _x;
    long double _y;

    // default comparison operator
    bool operator==(const Dot&) const;
    // basic stream operator
    friend std::ostream &operator<<(std::ostream &os, Dot const &obj)
    {
        os << "(" << obj._x << "," << obj._y << ")";
        return os;
    }

};

/**
 * @typedef segment
 * @brief A segment defined by two dots
 */
struct Segment
{
    Dot _from;
    Dot _to;
    /**
     * @brief Check if a dot is on the segment
     * @param dot The dot to check.
     * @return True if the dot is on the segment, false otherwise.
     */
    bool contains(Dot const &dot) const;
    /**
     * @brief Compute the y coordinate for a given x coordinate on the segment
     * @param x The x coordinate.
     * @return The corresponding y coordinate.
     * @pre x must be within the segment bounds
     */
    long double get_y(long double x) const;

    /**
     * @brief Returns the slope of the segment
     * @return The slope of the segment (nan if vertical segment)
     */
    long double get_slope() const;

    // default comparison operator
    bool operator==(const Segment&) const = default;
};

/**
 * @typedef piecewise_linear_function
 * @brief list of segments defining a piece-wise linear function
 */
using Piecewise_linear_function = std::list<Dot>;

/**
 * @brief Adds a variation to a piece-wise linear function
 * @param pwf The original piece-wise linear function.
 * @param variation The variation to add.
 * @pre the x coordinates of the variation must be within the bounds of the piece-wise function
 * @pre the y coordinate of the variation start must be zero
 * @return A new piece-wise linear function with the added segment.
 */
Piecewise_linear_function add_variation(Piecewise_linear_function const &pwf, Segment const &variation);

/**
 * @brief Adds a list of variation to a piece-wise linear function
 * @param pwf The original piece-wise linear function.
 * @param variations The list of variations to add.
 * @pre the x coordinates of the variations must be within the bounds of the piece-wise function
 * @pre the y coordinate of the variation start must be zero
 * @return A new piece-wise linear function with the added segment.
 */
Piecewise_linear_function add_variation(Piecewise_linear_function const &pwf, std::list<Segment> const &variations);
