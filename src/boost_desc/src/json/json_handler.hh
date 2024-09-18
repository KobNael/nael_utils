#pragma once
#include <fstream>
#include "dto/utils/converter.hh"
#include "dto/in/DtoIn.hh"

namespace json
{
    /**
     * @brief Load a context from a file
     * @param filePath path to the file
     * @param context the context to fill
     */
    template<typename T>
    void import(std::string const &filePath, T* context)
    {
        std::ifstream istream(filePath, std::ios::in);
        if (!istream.is_open() || istream.eof()) {
            throw;
        }
        boost::json::value j = boost::json::parse(istream);

        for(auto jv : j.at_pointer("/presses").as_array())
        {
            auto a = boost::json::value_to<dto::PressDto>( jv );
            std::cout << a << std::endl;
        }
    };
}