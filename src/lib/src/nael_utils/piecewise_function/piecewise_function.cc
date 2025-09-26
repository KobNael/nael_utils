/**
 * @file piecewise_function.cc
 */
#include <nael_utils/piecewise_function/piecewise_function.hh>

#include <nael_utils/safe_comp/safe_comp.hh>

#include <algorithm>
#include <cassert>
#include <ranges>
#include <iostream>

bool Dot::operator==(const Dot& rhs) const
{
    return safecomp::eq(_x, rhs._x) && safecomp::eq(_y, rhs._y);
}


// Return the slope of a linear segment
double Segment::get_slope() const
{
    if(safecomp::eq(_from._x, _to._x))
    {
        return std::numeric_limits<long double>::quiet_NaN();
    }
    return (_to._y - _from._y) / (_to._x - _from._x);
}

// Check if a dot is on the segment
bool Segment::contains(Dot const &dot) const
{
    // special case of vertical segment
    if(safecomp::eq(_from._x, _to._x))
    {
            // x must be equal to the segment x
        return safecomp::eq(dot._x, _from._x)
            // and y within segment bounds
            && safecomp::le(_from._y, dot._y) && safecomp::le(dot._y, _to._y);
    }
    // normal case: non vertical segment
    assert(!safecomp::eq(_from._x, _to._x));
        // x within segment bounds
    return safecomp::le(_from._x, dot._x) && safecomp::le(dot._x, _to._x)
        // and y on the line defined by the segment
        && safecomp::eq(dot._y, _from._y + get_slope() * (dot._x - _from._x));
}

// Check if a x coordinate is within the segment bounds
bool Segment::x_in_range(double x) const
{
    return safecomp::le(_from._x, x) && safecomp::le(x, _to._x);
}

// Check if a y coordinate is within the segment bounds
bool Segment::y_in_range(double y) const
{
    return (safecomp::le(_from._y, y) && safecomp::le(y, _to._y)) || (safecomp::le(_to._y, y) && safecomp::le(y, _from._y));
}

// Compute the x coordinate for a given y coordinate on the segment
double Segment::get_x(double y) const
{
    assert(y_in_range(y));
    // special case of horizontal segment
    if(safecomp::eq(_from._y, _to._y))
    {
        return std::numeric_limits<long double>::quiet_NaN();
    }
    // special case for vertical segment
    if(safecomp::eq(_from._x, _to._x))
    {
        return _from._x;
    }
    // normal case: non vertical segment
    return _from._x + (y - _from._y) / get_slope();
}

// Compute the y coordinate for a given x coordinate on the segment
double Segment::get_y(double x) const
{
    assert(safecomp::le(_from._x, x) && safecomp::le(x, _to._x));
    // special case of vertical segment
    if(safecomp::eq(_from._x, _to._x))
    {
        return std::numeric_limits<long double>::quiet_NaN();
    }
    // normal case: non vertical segment
    return _from._y + get_slope() * (x - _from._x);
}

/**
 * @brief Merges consecutive segments with the same slope in a piece-wise linear function.
 * @param[out] pwf The piece-wise linear function to merge.
 */
void merge(Piecewise_linear_function &pwf)
{
    if(pwf.size() < 3)
    {
        return;
    }
    auto from = pwf.begin();
    auto dot = std::next(from);
    auto to = std::next(dot);
    while(to != pwf.end())
    {
        // on vertical segments, remove the intermediate point to clean yoyo effects
        if(safecomp::eq(from->_x, to->_x) || Segment(*from, *to).contains(*dot))
        {
            // remove it
            dot = pwf.erase(dot);
            to = std::next(dot);
        }
        // otherwise, move to the next segment
        else
        {
            ++from;
            ++dot;
            ++to;
        }
    }
}

/**
 * @brief Sum two segments of a piecewise linear function
 * @param segment The original segment.
 * @param variation The variation segment to apply.
 * @return A piece-wise linear function resulting from the sum.
 */
Piecewise_linear_function sum_segments(Segment const &segment, Segment const &variation)
{
    double variation_slope{ variation.get_slope() };
    double segment_slope{ segment.get_slope() };
    double variation_delta_y{ variation._to._y - variation._from._y };

    // if on vertial variation
    if(std::isnan(variation_slope))
    {
        // if on a vertical segment move the target point
        if(std::isnan(segment_slope))
        {
            assert(safecomp::eq(segment._from._x, variation._from._x));
            return Piecewise_linear_function{{segment._from._x, segment._from._y},
                                            {segment._to._x, segment._to._y + variation_delta_y}};
        }
        // otherwise split the segment in two parts
        else
        {
            assert( segment._from._x <= variation._from._x && segment._to._x >= variation._to._x );
            double y_at_variation = segment.get_y(variation._from._x);
            return Piecewise_linear_function{{segment._from._x, segment._from._y},
                                            {variation._from._x, y_at_variation},
                                            {variation._to._x, y_at_variation + variation_delta_y},
                                            {segment._to._x, segment._to._y + variation_delta_y}};
        }
    }

    Piecewise_linear_function result;
    // first part (if any)
    if(safecomp::lt(segment._from._x, variation._from._x))
    {
        result.emplace_back(segment._from._x, segment._from._y);
        result.emplace_back(variation._from._x, segment.get_y(segment._from._x));
    }
    // common part
    double from_x = std::max(segment._from._x, variation._from._x);
    double to_x = std::min(segment._to._x, variation._to._x);
    double y_from_variation = variation.get_y(from_x);
    double y_to_variation = variation.get_y(to_x);
    double from_y = segment.get_y(from_x) + y_from_variation;
    double to_y = segment.get_y(to_x) + y_to_variation;
    result.emplace_back(from_x, from_y);
    result.emplace_back(to_x, to_y);
    // last part (if any)
    if(safecomp::gt(segment._to._x, variation._to._x))
    {
        result.emplace_back(segment._to._x, segment._to._y + variation_delta_y);
    }

    return result;
}

// Adds a non vertical variation to a piece-wise linear function
Piecewise_linear_function add_variation(Piecewise_linear_function const &pwf, Segment const &variation)
{
    assert( pwf.size() > 1 );
    assert( safecomp::isnull(variation._from._y) );
    // get the variation characteristics
    double variation_delta_y{ variation._to._y - variation._from._y };

    // Create a new piecewise linear function to hold the result
    Piecewise_linear_function result;
    auto cur_it = std::next(pwf.begin());
    do
    {
        auto prev_it = std::prev(cur_it);
        // segment fully before variation
        if(safecomp::lt(cur_it->_x, variation._from._x))
        {
            result.emplace_back(prev_it->_x, prev_it->_y);
            result.emplace_back(cur_it->_x, cur_it->_y);
        }
        // segment fully after variation
        else if(safecomp::gt(prev_it->_x, variation._to._x))
        {
            result.emplace_back(prev_it->_x, prev_it->_y + variation_delta_y);
            result.emplace_back(cur_it->_x, cur_it->_y + variation_delta_y);
        }
        // intersection
        else
        {
            auto const &sum = sum_segments({*prev_it, *cur_it}, variation);
            result.insert(result.end(), sum.begin(), sum.end());
        }
    } while (++cur_it != pwf.end());

    // merge the segments if needed and return the result
    merge(result);
    return result;
}

// Adds a list of variation to a piece-wise linear function
Piecewise_linear_function add_variation(Piecewise_linear_function const &pwf, std::list<Segment> const &variations)
{
    Piecewise_linear_function result = pwf;
    for(auto const &variation : variations)
    {
        result = add_variation(result, variation);
    }
    return result;
}

// Analyse a piece-wise linear function and return the first dot with the highest priority according to a comparison function in an interval
template<typename Fun>
Dot get_dot(Piecewise_linear_function const &pwf, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    Dot res({std::numeric_limits<long double>::quiet_NaN(), std::numeric_limits<long double>::quiet_NaN()});
    auto cur_it = std::next(pwf.begin());
    do
    {
        auto const &segment = Segment{*std::prev(cur_it), *cur_it};
        // segment fully before interval
        if(safecomp::lt(segment._to._x, x_start))
        {
            continue;
        }
        // after the interval
        if(safecomp::lt(x_end, segment._from._x))
        {
            break;
        }

        // spercial case: vertical segment
        Segment intersection = (safecomp::eq(segment._from._x, segment._to._x))
            ? segment
            : Segment( {std::max(segment._from._x, x_start), segment.get_y(std::max(segment._from._x, x_start))}, {std::min(segment._to._x, x_end), segment.get_y(std::min(segment._to._x, x_end))} );
        // get the best dot among the segment bounds
        if(comp(res, intersection._from))
        {
            res = intersection._from;
        }
        if(comp(res, intersection._to))
        {
            res = intersection._to;
        }
    }while(++cur_it != pwf.end());

    return res;
}
// Analyse a piece-wise linear function and return the lowest dot in an interval
Dot get_lowest_dot(Piecewise_linear_function const &pwf, double x_start, double x_end)
{
    return get_dot(pwf, x_start, x_end, [](Dot const &ref, Dot const& candidate){return std::isnan(ref._y) || (safecomp::lt(candidate._y, ref._y));});
}
// Analyse a piece-wise linear function and return the highest dot in an interval
Dot get_highest_dot(Piecewise_linear_function const &pwf, double x_start, double x_end)
{
    return get_dot(pwf, x_start, x_end, [](Dot const &ref, Dot const& candidate){return std::isnan(ref._y) || (safecomp::lt(ref._y, candidate._y));});
}

// Analyse a piece-wise linear function and return the first dot satisfying a condition in an interval
template<typename Fun>
Dot get_first_dot(Piecewise_linear_function const &pwf, double y, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    auto cur_it = std::next(pwf.begin());
    do
    {
        auto const &segment = Segment{*std::prev(cur_it), *cur_it};
        std::cerr << "on " << segment << std::endl;
        // segment fully before interval
        if(safecomp::lt(segment._to._x, x_start))
        {
        std::cerr << "before interval" << std::endl;
            continue;
        }
        // segment fully after interval => not found
        if(safecomp::lt(x_end, segment._from._x))
        {
            std::cerr << "after interval" << std::endl;
            return {std::numeric_limits<long double>::quiet_NaN(), std::numeric_limits<long double>::quiet_NaN()};
        }
        // special case for vertical segment
        if(std::isnan(segment.get_slope()))
        {
            // first dot valid
            if(comp(segment._from._y, y))
            {
                return segment._from;
            }
            // second dot valid
            else if(comp(segment._to._y, y))
            {
                return Dot(segment._to._x, y);
            }
            // invalid segment
            else
            {
                continue;
            }
        }
        //get the intersection in [x_start, x_end]
        double x1 = std::max(x_start, segment._from._x);
        double x2 = std::min(x_end, segment._to._x);
        Segment intersection = {{x1, segment.get_y(x1)}, {x2, segment.get_y(x2)}};
        // if first point is valid => ok
        if(comp(intersection._from._y, y))
        {
            return intersection._from;
        }
        // if y is in the range => compute x
        else if(intersection.y_in_range(y))
        {
            return {intersection.get_x(y), y};
        }
        // the intersection is not valid => go on
        else
        {
            continue;
        }

    }while(++cur_it != pwf.end());

    // out of bound => not found
    return {std::numeric_limits<long double>::quiet_NaN(), std::numeric_limits<long double>::quiet_NaN()};
}
// Analyse a piece-wise linear function and return the first dot above a given y in an interval
Dot get_first_dot_above(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    return get_first_dot(pwf, y, x_start, x_end, [](double y_cand, double y_ref){return safecomp::ge(y_cand, y_ref);});
}
// Analyse a piece-wise linear function and return the first dot below a given y in an interval
Dot get_first_dot_below(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    return get_first_dot(pwf, y, x_start, x_end, [](double y_cand, double y_ref){return safecomp::le(y_cand, y_ref);});
}

// Analyse a piece-wise linear function and return the last dot satisfying a condition in an interval
template<typename Fun>
Dot get_last_dot(Piecewise_linear_function const &pwf, double y, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    auto cur_it = pwf.rbegin();
    do
    {
        auto const &segment = Segment{*std::next(cur_it), *cur_it};
        std::cout << "on " << segment << std::endl;
        // segment fully after interval
        if(safecomp::gt(segment._from._x, x_end))
        {
            std::cout << "too far, continue" << std::endl;
            continue;
        }
        // segment fully before interval => not found
        if(safecomp::gt(x_start, segment._to._x))
        {
            std::cout << "too early, not found" << std::endl;
            return {std::numeric_limits<long double>::quiet_NaN(), std::numeric_limits<long double>::quiet_NaN()};
        }
        // special case for vertical segment
        if(std::isnan(segment.get_slope()))
        {
            // last dot valid
            if(comp(segment._to._y, y))
            {
                return segment._to;
            }
            // second dot valid
            else if(comp(segment._from._y, y))
            {
                return Dot(segment._from._x, y);
            }
            // invalid segment
            else
            {
                continue;
            }
        }
        //get the intersection in [x_start, x_end]
        double x1 = std::max(x_start, segment._from._x);
        double x2 = std::min(x_end, segment._to._x);
        Segment intersection = {{x1, segment.get_y(x1)}, {x2, segment.get_y(x2)}};
        // if last point is valid => ok
        if(comp(intersection._to._y, y))
        {
            return intersection._to;
        }
        // if y is in the range => compute x
        else if(intersection.y_in_range(y))
        {
            return {intersection.get_x(y), y};
        }
        // the intersection is not valid => go on
        else
        {
            continue;
        }
    }while(++cur_it != pwf.rend());

    // out of bound => not found
    return {std::numeric_limits<long double>::quiet_NaN(), std::numeric_limits<long double>::quiet_NaN()};
}
// Analyse a piece-wise linear function and return the last dot above a given y in an interval
Dot get_last_dot_above(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    return get_last_dot(pwf, y, x_start, x_end, [](double y_cand, double y_ref){return safecomp::ge(y_cand, y_ref);});
}
// Analyse a piece-wise linear function and return the last dot below a given y in an interval
Dot get_last_dot_below(Piecewise_linear_function const &pwf, double y, double x_start, double x_end )
{
    return get_last_dot(pwf, y, x_start, x_end, [](double y_cand, double y_ref){return safecomp::le(y_cand, y_ref);});
}
