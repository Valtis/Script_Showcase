
#include <ctime>
#include "Utility/Logger.h"


Logger::Logger(std::string fileName, LogLevel loggingLevel) : m_loggingLevel(loggingLevel), m_addTimeStamps(false), m_logFile()
{
    if (m_loggingLevel > LogLevel::NONE)
        Open(fileName);
}
Logger::Logger() : m_loggingLevel(LogLevel::NONE), m_addTimeStamps(false), m_logFile()
{

}
Logger::~Logger()
{
    Close();
}

void Logger::Open(std::string fileName)
{
    m_logFile.open(fileName);
    m_logFile << CreateStamp(true) << "Opening new log file\n\n";
}

void Logger::Close()
{
    if (m_logFile.is_open())
    {
        
        m_logFile << "\n" << CreateStamp(true) << "Closing log file" << std::endl;
        m_logFile.close();
    }
}

void Logger::AddTimeStamps(bool stamp)
{
    m_addTimeStamps = stamp;
}

std::string Logger::CreateStamp(bool extendedTimeStamp)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	 
    time_t currentTime;
    time(&currentTime);
    tm *timeInfo = localtime(&currentTime);

    std::ostringstream stamp;
    stamp << "[";

    if (extendedTimeStamp == true)
    {
        stamp << timeInfo->tm_year + 1900 << "." <<  timeInfo->tm_mon + 1 << "." << timeInfo->tm_mday << " ";
    }

    stamp << (timeInfo->tm_hour < 10 ? "0" : "") <<  timeInfo->tm_hour << ":" << (timeInfo->tm_min < 10 ? "0" : "") <<  timeInfo->tm_min << ":" << (timeInfo->tm_sec < 10 ? "0" : "")  << timeInfo->tm_sec << "]";
    stamp << " ";
#pragma warning(pop)

	return stamp.str();
}

void Logger::AddLine( LogLevel level, std::string text )
{
  if (!m_logFile.is_open())
  {
    return;
  }

	if (m_loggingLevel == LogLevel::NONE || (m_loggingLevel != LogLevel::ALL && level < m_loggingLevel))
  {
		return;
  }

	if (m_addTimeStamps == true)
  {
		m_logFile << CreateStamp() << " ";
  }

  WriteLogLevel(level);
	m_logFile << text << std::endl;
}

void Logger::WriteLogLevel( LogLevel level )
{
  switch (level)
  {
  case LogLevel::NONE:
  case LogLevel::ALL:
    break;
  case LogLevel::ERROR:
    m_logFile << "Error: ";
    break;
  case LogLevel::WARNING:
    m_logFile << "Warning: ";
    break;
  case LogLevel::INFO:
    m_logFile << "Info: ";
    break;
  case LogLevel::DEBUG:
    m_logFile << "Debug: ";
    break;
  default:
    m_logFile << "Logger error: Level " << static_cast<int>(level) << " given which is undefined. Following message attached: ";
  }
}

void Logger::SetLoggingLevel( LogLevel val )
{
  m_loggingLevel = val;
}
