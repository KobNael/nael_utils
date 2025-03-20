#include <nael_utils/json/details/converter.hh>

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
        void tag_invoke( const json::value_from_tag&, json::value& jv, date const& d )
        {
            jv = { to_iso_extended_string(d) };
        }

        /**
         * @brief convert json::value to a date
         * @param jv the json::value
         * @return the date
         */
        date tag_invoke( const json::value_to_tag< date >&, json::value const& jv )
        {
            date d =  from_string(boost::json::value_to<std::string>(jv));
            return d;
        }
    }//gregorian

    namespace posix_time
    {
        /**
         * @brief convert json::value to a time_duration
         * @param jv the json::value
         * @return the time_duration
         */
        time_duration tag_invoke( const json::value_to_tag< time_duration >&, json::value const& jv )
        {
            return duration_from_string(boost::json::value_to<std::string>(jv));
        }

        /**
         * @brief convert json::value to a ptime
         * @param jv the json::value
         * @return the ptime
         */
        ptime tag_invoke( const json::value_to_tag< ptime >&, json::value const& jv )
        {
            return time_from_string(boost::json::value_to<std::string>(jv));
        }
    }//posix_time

}//boost
