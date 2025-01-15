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
}//namespace

//FileLogger constructor
FileLogger::FileLogger(std::string const& file, io::LogLevel level)
 : Logger<std::ofstream>(file, level)
{
    open_stream(file, *_internal_stream);
    this->setLog( *_internal_stream );
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
    this->setLog( *_teeStream );
}
//TeeLogger destructor
TeeLogger::~TeeLogger()
{}

} //namespace io