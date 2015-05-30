#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <fstream>
#include <sstream>

enum class LogLevel : int { NONE, DEBUG, INFO, WARNING, ERROR, ALL };

class Logger
{
public:
  Logger(std::string fileName, LogLevel loggingLevel);
  Logger();
  virtual ~Logger();
  void Open(std::string fileName);
  void Close();
  void AddTimeStamps(bool stamp);

  void AddLine(LogLevel level, const std::string &text);


  void SetLoggingLevel(LogLevel val);
private:

  void WriteLogLevel( LogLevel level );
  LogLevel m_loggingLevel;
  std::string CreateStamp(bool extendedTimeStamp = false);
  bool m_addTimeStamps;
  std::ofstream m_logFile;
};

#endif // LOGGER_HPP
