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

void tag_invoke( const json::value_from_tag&, json::value& jv, date const& d )
{
    jv = { to_iso_extended_string(d) };
}

date tag_invoke( const json::value_to_tag< date >&, json::value const& jv )
{
    date d =  from_string(boost::json::value_to<std::string>(jv));
    return d;
}

}//gregorian
namespace posix_time
{

time_duration tag_invoke( const json::value_to_tag< time_duration >&, json::value const& jv )
{
    return duration_from_string(boost::json::value_to<std::string>(jv));
}

ptime tag_invoke( const json::value_to_tag< ptime >&, json::value const& jv )
{
    return time_from_string(boost::json::value_to<std::string>(jv));
}

template<typename time_ao_duration>
void tag_invoke( const json::value_from_tag&, json::value& jv, time_ao_duration const& td )
{
    jv = { to_simple_string(td) };
}

}//posix_time

}//boost

template<class T> void extract( boost::json::object const & obj, char const * name, T & value )
{
    value = boost::json::value_to<T>( obj.at( name ) );
}

namespace dto
{

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

}