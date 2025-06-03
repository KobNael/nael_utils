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
#include <type_traits>

//Converteur for boost date / time
namespace boost
{

    namespace gregorian
    {
        /**
         * @brief convert a gregorian date to a json::value
         * @param[out] jv the json::value
         * @param d the date
         */
        void tag_invoke( const json::value_from_tag&, json::value& jv, date const& d );

        /**
         * @brief convert json::value to a date
         * @param jv the json::value
         * @return the date
         */
        date tag_invoke( const json::value_to_tag< date >&, json::value const& jv );
    }//gregorian

    namespace posix_time
    {
        /**
         * @brief convert a time_duration to json value
         * @param[out] jv the json::value
         * @param td the time_duration
         */
        void tag_invoke( const json::value_from_tag&, json::value& jv, time_duration const& td );
        /**
         * @brief convert json::value to a time_duration
         * @param jv the json::value
         * @return the time_duration
         */
        time_duration tag_invoke( const json::value_to_tag< time_duration >&, json::value const& jv );

        /**
         * @brief convert a ptime to json value
         * @param[out] jv the json::value
         * @param pt the ptime
         */
        void tag_invoke( const json::value_from_tag&, json::value& jv, ptime const& pt );
        /**
         * @brief convert json::value to a ptime
         * @param jv the json::value
         * @return the ptime
         */
        ptime tag_invoke( const json::value_to_tag< ptime >&, json::value const& jv );

    }//posix_time

}//boost

/**
 * @brief Extract a type from a boost json object
 * @tparam T the type of of object to extract
 * @param obj the json object
 * @param name the attribute name
 * @param[out] value reference to the object
 */
template<class T> void extract( boost::json::object const & obj, char const * name, T & value )
{
    boost::json::value const *obj_val = obj.if_contains( name );
    if(nullptr != obj_val)
    {
        value = boost::json::value_to<T>( *obj_val );
    }
}

namespace dto
{

    /**
     * @brief Convert a json value to an object T
     * @tparam T the type of object
     * @param v
     */
    template<class T,
        class D1 = boost::describe::describe_members<T,
            boost::describe::mod_public | boost::describe::mod_protected>,
        class D2 = boost::describe::describe_members<T, boost::describe::mod_private>,
        class En = std::enable_if_t<boost::mp11::mp_empty<D2>::value && !std::is_union<T>::value> >

    T tag_invoke( boost::json::value_to_tag<T> const&, boost::json::value const& v )
    {
        auto const& obj = v.as_object();

        T t{};

        boost::mp11::mp_for_each<D1>([&](auto D){

            extract( obj, D.name, t.*D.pointer );

        });

        return t;
    }

} //namespace dto
