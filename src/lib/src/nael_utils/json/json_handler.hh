/**
 * @file json_handler.hh
 */
#pragma once

#include <nael_utils/json/details/converter.hh>
#include <nael_utils/exception/exception.hh>
#include <fstream>

namespace json
{
    /**
     * @brief Load a context from a stream
     * @tparam T the type of context
     * @param istream the input stream
     * @param context the context to fill
     * @pre T has been declared using #MAKE_DTO_STRUCT
     */
    template <typename T>
    void import_from_stream(std::istream &istream, T &context)
    {
        boost::json::value j = boost::json::parse(istream);
        context = boost::json::value_to<T>(j);
    }

    /**
     * @brief Load a context from a file
     * @tparam T the type of context
     * @param filePath path to the file
     * @param context the context to fill
     * @pre T has been declared using #MAKE_DTO_STRUCT
     */
    template <typename T>
    void import_from_file(std::string const &filePath, T &context)
    {
        std::ifstream istream(filePath, std::ios::in);
        if (!istream.is_open() || istream.eof())
        {
            throw io::access_error("Could not open file " + filePath);
        }
        import_from_stream(istream, context);
        istream.close();
    }

    /**
     * @brief Export a context to a stream
     * @tparam T the type of context
     * @param ostream the output stream
     * @param context the context to export
     * @pre T has been declared using #MAKE_DTO_STRUCT
     */
    template <typename T>
    void export_to_stream(std::ostream &ostream, T const &context)
    {
        ostream << boost::json::value_from(context);
    }

    /**
     * @brief Export a context to a file
     * @tparam T the type of context
     * @param filePath path to the file
     * @param context the context to export
     * @pre T has been declared using #MAKE_DTO_STRUCT
     */
    template <typename T>
    void export_to_file(std::string const &filePath, T const &context)
    {
        std::ofstream ostream(filePath, std::ios::out);
        export_to_stream(ostream, context);
        ostream.close();
    }
} // namespace json
