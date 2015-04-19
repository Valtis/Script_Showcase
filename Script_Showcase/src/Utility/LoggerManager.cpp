#include "Utility/LoggerManager.h"

std::string LoggerManager::m_logFolder = "";
std::unordered_map<std::string, std::unique_ptr<Logger>> LoggerManager::m_logs;
LogLevel LoggerManager::m_globalLogLevel = LogLevel::ALL;

std::string LoggerManager::GetFullName( const std::string &name )
{
  if (m_logFolder.empty())
  {
    return name;
  }
  return m_logFolder + "/" + name;
}

Logger & LoggerManager::GetLog( std::string name )
{
  name = GetFullName(name);

  if (m_logs.count(name) == 0)
  {
    m_logs[name].reset(new Logger());
    m_logs[name]->AddTimeStamps(true);
    m_logs[name]->SetLoggingLevel(m_globalLogLevel);

    if (m_globalLogLevel != LogLevel::NONE)
    {
      m_logs[name]->Open(name);
    }
  }

  return *m_logs[name];
}

void LoggerManager::Release()
{
  m_logs.clear();
}

LoggerManager::LoggerManager()
{

}

void LoggerManager::SetLogFolder( std::string folder )
{
  m_logFolder = folder;
}

LoggerManager::~LoggerManager()
{
  m_logs.clear();
}

void LoggerManager::SetGlobalLogLevel(LogLevel level)
{
  m_globalLogLevel = level;
  for (auto &log : m_logs)
  {
    log.second->SetLoggingLevel(m_globalLogLevel);
  }
}