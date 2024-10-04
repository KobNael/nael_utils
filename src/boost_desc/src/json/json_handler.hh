#pragma once
#include <fstream>
#include "details/converter.hh"

namespace json
{
    /**
     * @brief Load a context from a file
     * @param filePath path to the file
     * @param context the context to fill
     */
    template<typename T>
    void import_from_file(std::string const &filePath, T &context)
    {
        std::ifstream istream(filePath, std::ios::in);
        if (!istream.is_open() || istream.eof()) {
            throw;
        }
        boost::json::value j = boost::json::parse(istream);
        istream.close();
        context = boost::json::value_to<T>(j);
    }

     /**
     * @brief Export a context to a file
     * @param filePath path to the file
     * @param context the context to export
     */
    template<typename T>
    void export_to_file(std::string const &filePath, T const &context)
    {
        std::ofstream ostream(filePath, std::ios::out);
        ostream << boost::json::value_from( context );
        ostream.close();
    }
}
