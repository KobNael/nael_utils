/**
 * @file piecewise_function.hh
 * @brief Interface for piecewise function
 */
#pragma once

#include <limits>
#include <list>
#include <ostream>

/**
 * @struct Dot
 * @brief Structure defining a point (dot) in a piece-wise function
 */
struct Dot
{
    double _x= {0.};
    double _y= {0.};

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
    Dot _from={};
    Dot _to={};
    /**
     * @brief Check if a dot is on the segment
     * @param dot The dot to check.
     * @return True if the dot is on the segment, false otherwise.
     */
    bool contains(Dot const &dot) const;

    /**
     * @brief Check if a y coordinate is within the segment bounds
     * @param y The y coordinate to check.
     * @return True if the y coordinate is within the segment bounds, false otherwise.
     */
    bool y_in_range(double y) const;
    /**
     * @brief Check if a x coordinate is within the segment bounds
     * @param x The x coordinate to check.
     * @return True if the x coordinate is within the segment bounds, false otherwise.
     */
    bool x_in_range(double x) const;
    /**
     * @brief Compute the y coordinate for a given x coordinate on the segment
     * @param x The x coordinate.
     * @return The corresponding y coordinate.
     * @pre x must be within the segment bounds
     */
    double get_y(double x) const;
    /**
     * @brief Compute the x coordinate for a given y coordinate on the segment
     * @param y The y coordinate.
     * @return The corresponding x coordinate.
     * @pre y must be within the segment bounds
     */
    double get_x(double y) const;

    /**
     * @brief Returns the slope of the segment
     * @return The slope of the segment (nan if vertical segment)
     */
    double get_slope() const;

    // default comparison operator
    bool operator==(const Segment&) const = default;
    // basic stream operator
    friend std::ostream &operator<<(std::ostream &os, Segment const &obj)
    {
        os << "[" << obj._from << " -> " << obj._to << "]";
        return os;
    }
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

/**
 * @brief Return the list of intersection points between a piece-wise linear function and an horizontal segment
 * @param pwf The original piece-wise linear function.
 * @param y the y coordinate of the horizontal segment.
 * @return The list of intersection points.
 */
Piecewise_linear_function get_intersection(Piecewise_linear_function const &pwf, double y);

/**
 * @brief Analyse a piece-wise linear function and return the lowest dot in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_lowest_dot(Piecewise_linear_function const &pwf, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());

/**
 * @brief Analyse a piece-wise linear function and return the highest dot in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_highest_dot(Piecewise_linear_function const &pwf, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());

/**
 * @brief Analyse a piece-wise linear function and return the first dot above a given y in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_first_dot_above(Piecewise_linear_function const &pwf, double y, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());
/**
 * @brief Analyse a piece-wise linear function and return the last dot above a given y in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_last_dot_above(Piecewise_linear_function const &pwf, double y, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());
/**
 * @brief Analyse a piece-wise linear function and return the first dot below a given y in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_first_dot_below(Piecewise_linear_function const &pwf, double y, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());
/**
 * @brief Analyse a piece-wise linear function and return the last dot below a given y in an interval
 * @param pwf The piece-wise linear function to analyze.
 * @param x_start the starting x coordinate of the interval (inclusive)
 * @param x_end the ending x coordinate of the interval (inclusive)
 * @return the corresponding dot, or (nan,nan) if none found
 */
Dot get_last_dot_below(Piecewise_linear_function const &pwf, double y, double x_start = -std::numeric_limits<long double>::infinity(), double x_end = std::numeric_limits<long double>::infinity());
