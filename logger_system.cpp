#include "logger_system.h"
#include <QStandardPaths>
#include <QDir>

LoggerSystem &LoggerSystem::GetInstance() {
  static LoggerSystem instance;
  return instance;
}

void LoggerSystem::LogMessage(LogLevel level, const QString &message) {
  QString log_entry = QString("[%1]-->[%2]--[%3]")
	  .arg(LogLevelToString(level))
	  .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd:HH-mm-ss"))
	  .arg(message);
  this->log_stream_ << log_entry << "\n";
  this->log_stream_.flush();
}

LoggerSystem::LoggerSystem() {
  EnsureLogFile();
}

LoggerSystem::~LoggerSystem() {
  log_file_.close();
}

QString LoggerSystem::LogLevelToString(LogLevel level) {
  switch (level) {
	case LogLevel::kInfo:return "INFO";
	case LogLevel::kWarning:return "WARNING";
	case LogLevel::kError:return "ERROR";
	default:return "UNKNOWW";
  }
}

void LoggerSystem::EnsureLogFile() {
  QString data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir;
  if (!dir.exists(data_dir)) {
	dir.mkdir(data_dir);
  }

  this->log_file_.setFileName(data_dir + "/application.log");
  if (!log_file_.exists()) {
	this->log_file_.open(QIODevice::WriteOnly | QIODevice::Text);
	this->log_file_.close();
  }

  this->log_file_.open(QIODevice::Append | QIODevice::Text);
  log_stream_.setDevice(&log_file_);
}
