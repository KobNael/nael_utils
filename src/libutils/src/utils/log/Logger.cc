#include "utils/log/Logger.hh"

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
{
	open_stream(file, *this->_stream);
}
//FileLogger destructor
FileLogger::~FileLogger()
{
	if(nullptr != this->_stream)
	{
		close_stream(*this->_stream);
		delete this->_stream;
	}
}

//TeeLogger constructor
TeeLogger::TeeLogger(std::string const& file, io::LogLevel level)
 : Logger<TeeStream>(file, level)
{
	open_stream(file, _fstream);
	_teeDevice = new TeeDevice(std::cout, _fstream);
	this->_stream = new TeeStream(*_teeDevice);
}
//TeeLogger destructor
TeeLogger::~TeeLogger()
{
	close_stream(_fstream);
	if(nullptr != this->_stream)
	{
		this->_stream->flush();
		this->_stream->close();
		delete this->_stream;
	}
	if(nullptr != _teeDevice)
	{
		delete _teeDevice;
	}
}

} //namespace io