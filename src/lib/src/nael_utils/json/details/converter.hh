/**
 * @file converter.hh
 */
#pragma once

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/json.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <format>
#include <type_traits>

// Converteur for boost date / time
namespace boost
{

    namespace gregorian
    {
        /**
         * @brief convert a gregorian date to a json::value
         * @param[out] jv the json::value
         * @param d the date
         */
        void tag_invoke(const json::value_from_tag &, json::value &jv, date const &d);

        /**
         * @brief convert json::value to a date
         * @param jv the json::value
         * @return the date
         */
        date tag_invoke(const json::value_to_tag<date> &, json::value const &jv);
    } // gregorian

    namespace posix_time
    {
        /**
         * @brief convert a time_duration to json value
         * @param[out] jv the json::value
         * @param td the time_duration
         */
        void tag_invoke(const json::value_from_tag &, json::value &jv, time_duration const &td);
        /**
         * @brief convert json::value to a time_duration
         * @param jv the json::value
         * @return the time_duration
         */
        time_duration tag_invoke(const json::value_to_tag<time_duration> &, json::value const &jv);

        /**
         * @brief convert a ptime to json value
         * @param[out] jv the json::value
         * @param pt the ptime
         */
        void tag_invoke(const json::value_from_tag &, json::value &jv, ptime const &pt);
        /**
         * @brief convert json::value to a ptime
         * @param jv the json::value
         * @return the ptime
         */
        ptime tag_invoke(const json::value_to_tag<ptime> &, json::value const &jv);

    } // posix_time

} // boost

/**
 * @brief Extract a type from a boost json object
 * @tparam T the type of of object to extract
 * @param obj the json object
 * @param name the attribute name
 * @param[out] value reference to the object
 */
template <class T>
void extract(boost::json::object const &obj, char const *name, T &value)
{
    boost::json::value const *obj_val = obj.if_contains(name);
    std::cerr << "parse " << std::string(name) << std::endl;
    if (nullptr != obj_val)
    {
        try
        {
            value = boost::json::value_to<T>(*obj_val);
        }
        catch(boost::system::system_error& e)
        {
            throw std::runtime_error("Could not parse attribute `" + std::string(name) + "`: " + std::string(e.what()));
        }
    }
}

namespace dto
{
    /**
     * @brief Concept to check if a type is suitable for JSON conversion
     * @tparam T the type to check
     */
    template<typename T>
    concept JsonConvertible = !std::is_union_v<T> && // Type must not be a union
                             boost::mp11::mp_empty<boost::describe::describe_members<T, boost::describe::mod_private>>::value && // Type must have no private members described by boost::describe
                             std::is_constructible_v<T>; // Type must be constructible

    /**
     * @brief Convert a json value to an object T
     * @tparam T the type of object
     * @param v the json value
     * @return T the converted object
     */
    template <JsonConvertible T>
    T tag_invoke(boost::json::value_to_tag<T> const &, boost::json::value const &v)
    {
        auto const &obj = v.as_object();

        T t{};

        using members_t = boost::describe::describe_members<T, boost::describe::mod_public | boost::describe::mod_protected>;

        boost::mp11::mp_for_each<members_t>([&t, &obj](auto member_descriptor)
        {
            extract(obj, member_descriptor.name, t.*member_descriptor.pointer);
        });

        return t;
    }

} // namespace dto
