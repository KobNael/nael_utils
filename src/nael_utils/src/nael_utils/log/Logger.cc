/**
 * @file Logger.cc
 */
#include <nael_utils/log/Logger.hh>

namespace io
{

namespace
{
	/**
	 * @brief Open a stream
	 */
	void open_stream(std::string const &file, std::ofstream &stream)
	{
		if( file != ""){
			stream.open(file.c_str() , std::ios::out);
			if( stream.fail() ){
				throw std::runtime_error("Could not create log file " + file);
			}else{
				stream.imbue(std::locale(std::locale::classic()));
			}
		}else{
			throw std::runtime_error("Invalid name of log file") ;
		}
	}
	/**
	 * @brief Close a stream
	 */
	void close_stream(std::ofstream &stream)
	{
		if( stream.is_open() )
		{
			stream.flush();
			stream.close();
		}
	}
}//namespace

//FileLogger constructor
FileLogger::FileLogger(std::string const& file, io::LogLevel level)
 : Logger<std::ofstream>(file, level)
 , _internal_stream(std::make_unique<std::ofstream>())
{
    open_stream(file, *_internal_stream);
    this->_stream = _internal_stream.get();
}
//FileLogger destructor
FileLogger::~FileLogger()
{}

//TeeLogger constructor
TeeLogger::TeeLogger(std::string const& file, io::LogLevel level)
 : Logger<TeeStream>(file, level)
{
	open_stream(file, _fstream);
	_teeDevice = std::make_unique<TeeDevice>(std::cout, _fstream);
    _teeStream = std::make_unique<TeeStream>(*_teeDevice);
	this->_stream = _teeStream.get();
}
//TeeLogger destructor
TeeLogger::~TeeLogger()
{
	close_stream(_fstream);
	if(nullptr != this->_stream)
	{
        try
        {
            this->_stream->flush();
            this->_stream->close();
        }
        catch( ... )
        {}
	}
}

} //namespace io