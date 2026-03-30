/**
 * @file piecewise_function.cc
 */
#include <nael_utils/piecewise_function/piecewise_function.hh>

#include <nael_utils/safe_comp/safe_comp.hh>

#include <algorithm>
#include <cassert>
#include <ranges>
#include <iostream>

//================//
// Dot definition //
//================//
bool Dot::operator==(const Dot& rhs) const
{
    return safecomp::eq(_x, rhs._x) && safecomp::eq(_y, rhs._y);
}

//====================//
// Segment definition //
//====================//
// Return the slope of a linear segment
double Segment::get_slope() const
{
    if(safecomp::eq(_from._x, _to._x))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return (_to._y - _from._y) / (_to._x - _from._x);
}

// Check if a segment is vertical
bool Segment::is_vertical() const
{
    return std::isnan(get_slope());
}

// Check if a segment is horizontal
bool Segment::is_horizontal() const
{
    return safecomp::eq(_from._y, _to._y);
}

// Check if a dot is on the segment
bool Segment::contains(Dot const &dot) const
{
    // special case of vertical segment
    if(is_vertical())
    {
            // x must be equal to the segment x
        return safecomp::eq(dot._x, _from._x)
            // and y within segment bounds
            && y_in_range(dot._y);
    }

    // non vertical segment
        // x within segment bounds
    return x_in_range(dot._x)
        // and y on the line defined by the segment
        && safecomp::eq(dot._y, get_y(dot._x));
}

// Check if a x coordinate is within the segment bounds
bool Segment::x_in_range(double x) const
{
        // horizontal segment on the good value
    return (is_horizontal() && safecomp::eq(_from._x, x))
        // or x in the bounds of the segment
        || (safecomp::le(_from._x, x) && safecomp::lt(x, _to._x));
}

// Check if a y coordinate is within the segment bounds
bool Segment::y_in_range(double y) const
{
        // vertical segment on the good value
    return (is_vertical() && safecomp::eq(_from._y, y))
        // or y in the bounds of the increasing segment
        || (safecomp::le(_from._y, y) && safecomp::lt(y, _to._y))
        // or y in the bounds of the decreasing segment
        || (safecomp::ge(_from._y, y) && safecomp::gt(y, _to._y));
}

// Compute the x coordinate for a given y coordinate on the segment
double Segment::get_x(double y) const
{
    // out of bounds or horizontal segment
    if(is_horizontal())
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    // special case for vertical segment
    if(is_vertical())
    {
        // return the origin of the segment, as all x coordinates are the same
        return _from._x;
    }
    // normal case: non vertical segment
    return _from._x + (y - _from._y) / get_slope();
}

// Compute the y coordinate for a given x coordinate on the segment
double Segment::get_y(double x) const
{
    // special case of vertical segment
    if(is_vertical())
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    // special case for horizontal segment
    if(is_horizontal())
    {
        // return the origin of the segment, as all y coordinates are the same
        return _from._y;
    }
    // normal case: non vertical segment
    return _from._y + get_slope() * (x - _from._x);
}

//======================//
// Segment manipulation //
//======================//
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
        if(safecomp::eq(from->_x, to->_x) || Segment{*from, *to}.contains(*dot))
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
    double variation_delta_y{ variation._to._y - variation._from._y };

    // if on vertical variation
    if(variation.is_vertical())
    {
        // if on a vertical segment move the target point
        if(segment.is_vertical())
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

//===================//
// Function Analysis //
//===================//
// Analyse a piece-wise linear function and return the first dot with the highest priority according to a comparison function in an interval
template<typename Fun>
Dot get_dot(Piecewise_linear_function const &pwf, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    Dot res({std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()});
    auto cur_it = std::next(pwf.begin());
    do
    {
        auto const &segment = Segment{*std::prev(cur_it), *cur_it};
        // segment fully before interval
        if(safecomp::le(segment._to._x, x_start) && safecomp::lt(segment._from._x, x_start))
        {
            continue;
        }
        // after the interval
        if(safecomp::lt(x_end, segment._from._x))
        {
            break;
        }

        // special case: vertical segment : just look at the from dot (the to dot will be processed in the next segment if any)
        if(segment.is_vertical())
        {
            //if the from dot is ok
            if(comp(res, segment._from))
            {
                // limit case : if we are on the x_start, we should check the to dot to take the tendancy into account
                if(safecomp::eq(segment._from._x, x_start))
                {
                    continue;
                }
                // otherwise, take the from dot
                res = segment._from;
            }
            continue;
        }
        else
        {
            double x1 = std::max(x_start, segment._from._x);
            // always test the from dot
            if(Dot intersection_from = {x1, segment.get_y(x1)};comp(res, intersection_from))
            {
                res = intersection_from;
            }
            // test the to dot only if it is strictly contained in the current segment
            double x2 = std::min(x_end, segment._to._x);
            if(safecomp::lt(x_end, segment._to._x))
            {
                if(Dot intersection_to = {x2, segment.get_y(x2)};comp(res, intersection_to))
                {
                    res = intersection_to;
                }
            }
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
        auto const segment = Segment{*std::prev(cur_it), *cur_it};
        // segment fully before interval
        if(safecomp::le(segment._to._x, x_start) && safecomp::lt(segment._from._x, x_start))
        {
            continue;
        }
        // segment fully after interval => not found
        if(safecomp::lt(x_end, segment._from._x))
        {
            return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
        }
        // special case for vertical segment
        if(segment.is_vertical())
        {
            // to be valid, the segment must follow the tendancy
            // hence we should check the to dot, it will be processed in the next iteration
            continue;
        }
        //get the intersection in [x_start, x_end]
        double x1 = std::max(x_start, segment._from._x);
        double x2 = std::min(x_end, segment._to._x);
        Segment intersection = {{x1, segment.get_y(x1)}, {x2, segment.get_y(x2)}};
        // always test the from dot
        if(comp(intersection._from._y, y))
        {
            return intersection._from;
        }
        // if y is in the range of the segment
        if(segment.y_in_range(y))
        {
            // compute the corresponding x coordinate
            double x = intersection.get_x(y);
            // if it's a valid value for the segment, return it
            if(segment.x_in_range(x))
            {
                return {x, y};
            }
        }
        // the intersection is not valid => go on
        continue;

    }while(++cur_it != pwf.end());

    // out of bound => not found
    return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
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
// Analyse a piece-wise linear function and return the first dot of the last piece fully above a given y in an interval st. every dot after is still above y
Dot get_first_dot_of_last_piece_above(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    assert( pwf.size() > 1 );
    double cur_x_start = x_start;
    do
    {
        // search the first dot satisfying the condition
        Dot dot = get_first_dot_above(pwf, y, cur_x_start, x_end);
        // if not found, return (nan,nan)
        if(std::isnan(dot._x))
        {
            return dot;
        }
        // search the lowest dot in the interval [dot._x, x_end]
        Dot lowest_dot = get_lowest_dot(pwf, dot._x, x_end);
        // if the lowest dot is still above y, we are in the last piece above y => return the first dot of this piece
        if(safecomp::ge(lowest_dot._y, y))
        {
            // if the lowest dot is still above y, we are in the last piece above y => return the first dot of this piece
            return dot;
        }
        // otherwise, try again after the lowest dot
        // if the whole segment after the dot is satisfying the condition, return it
        cur_x_start = lowest_dot._x;
    } while (true);
}
// Analyse a piece-wise linear function and return the first dot of the last piece fully below a given y in an interval st. every dot after is still below y
Dot get_first_dot_of_last_piece_below(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    assert( pwf.size() > 1 );
    double cur_x_start = x_start;
    do
    {
        // search the first dot satisfying the condition
        Dot dot = get_first_dot_below(pwf, y, cur_x_start, x_end);
        // if not found, return (nan,nan)
        if(std::isnan(dot._x))
        {
            return dot;
        }
        // search the highest dot in the interval [dot._x, x_end]
        Dot highest_dot = get_highest_dot(pwf, dot._x, x_end);
        // if the highest dot is still below y, we are in the last piece below y => return the first dot of this piece
        if(safecomp::le(highest_dot._y, y))
        {
            // if the highest dot is still below y, we are in the last piece below y => return the first dot of this piece
            return dot;
        }
        // otherwise, try again after the highest dot
        // if the whole segment after the dot is satisfying the condition, return it
        cur_x_start = highest_dot._x;
    } while (true);
}

// Analyse a piece-wise linear function and return the last dot satisfying a condition in an interval
template<typename Fun>
Dot get_last_dot(Piecewise_linear_function const &pwf, double y, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    auto cur_it = pwf.rbegin();
    do
    {
        auto const segment = Segment{*std::next(cur_it), *cur_it};
        // segment fully before interval => not found
        if(safecomp::le(segment._to._x, x_start) && safecomp::lt(segment._from._x, x_start))
        {
            return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
        }
        // segment fully after interval => not found
        if(safecomp::lt(x_end, segment._from._x))
        {
            continue;
        }
        // special case for vertical segment
        if(segment.is_vertical())
        {
            // to be valid, the segment must follow the tendancy
            // hence we should have check the to dot in the previous iteration
            continue;
        }
        //get the intersection in [x_start, x_end]
        double x1 = std::max(x_start, segment._from._x);
        double x2 = std::min(x_end, segment._to._x);
        Segment intersection = {{x1, segment.get_y(x1)}, {x2, segment.get_y(x2)}};
        // test the 'to' dot only if it is strictly contained in the current segment
        if(segment.x_in_range(intersection._to._x) && comp(intersection._to._y, y))
        {
            return intersection._to;
        }
        // if first point is valid => ok
        if(comp(intersection._from._y, y))
        {
            return intersection._from;
        }
        // the intersection is not valid => go on
        continue;
    }while(++cur_it != --pwf.rend());

    // out of bound => not found
    return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
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
// Analyse a piece-wise linear function and return the last dot satisfying a condition in an interval
template<typename Fun>
Dot get_last_dot_of_first_piece(Piecewise_linear_function const &pwf, double y, double x_start, double x_end, Fun comp)
{
    assert( pwf.size() > 1 );
    // Init the result with (nan,nan) to be able to detect if we found a valid dot or not
    Dot res({std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()});
    // Iterate on segments
    auto cur_it = std::next(pwf.begin());
    do
    {
        auto const segment = Segment{*std::prev(cur_it), *cur_it};
        // segment fully before interval
        if(safecomp::le(segment._to._x, x_start) && safecomp::lt(segment._from._x, x_start))
        {
            continue;
        }
        // segment fully after interval => exit
        if(safecomp::lt(x_end, segment._from._x))
        {
            break;
        }
        // special case for vertical segment
        if(segment.is_vertical())
        {
            // since we are looking for the last dot of the first piece, if we are on a vertical segment
            // we may consider the from dot as a candidate if it is valid, but we should not consider the to dot as it may be on the next piece
            if(comp(segment._from._y))
            {
                res = segment._from;
            }
            continue;
        }
        //get the intersection in [x_start, x_end]
        double x1 = std::max(x_start, segment._from._x);
        double x2 = std::min(x_end, segment._to._x);
        Segment intersection = {{x1, segment.get_y(x1)}, {x2, segment.get_y(x2)}};
        // if the from dot is not valid, break
        if(!comp(intersection._from._y))
        {
            break;
        }
        // the from dot is valid, take it as a candidate
        res = intersection._from;
        // if the to dot is ok
        if(comp(intersection._to._y))
        {
            // if the dot is strictly contained in the segment, take it as a candidate
            if(safecomp::lt(intersection._to._x, segment._to._x))
            {
                res = intersection._to;
            }
            // otherwise, we may take the dot into account on the next iteration if it is valid
            continue;
        }
        // the to dot is not valid, but the from dot is valid => search the intersection as the last valid dot
        assert(intersection.y_in_range(y));
        res = {intersection.get_x(y), y};
        break;
    }while(++cur_it != pwf.end());

    // out of bound => not found
    return res;
}
// Analyse a piece-wise linear function and return the last dot below a given y in an interval [x_start, x_end] st. every dot before is below y
Dot get_last_dot_of_first_piece_below(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    return get_last_dot_of_first_piece(pwf, y, x_start, x_end, [y](double y_cand){return safecomp::le(y_cand, y);});
}
// Analyse a piece-wise linear function and return the last dot above a given y in an interval [x_start, x_end] st. every dot before is above y
Dot get_last_dot_of_first_piece_above(Piecewise_linear_function const &pwf, double y, double x_start, double x_end)
{
    return get_last_dot_of_first_piece(pwf, y, x_start, x_end, [y](double y_cand){return safecomp::ge(y_cand, y);});
}
