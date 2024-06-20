#ifndef LOGGERSYSTEM_H
#define LOGGERSYSTEM_H

#include <QString>
#include <QFile>
#include <QTextStream>

/**
 * 日志文件类，建议用该类替换Qt或者C++的默认输出类，因为该类是单例模式，所以使用该类之前请务必使用
 * GetInstance()获取其的唯一实例。
 */
class LoggerSystem {
 public:
  /**
   * 日志消息的级别
   */
  enum class LogLevel {
	// 消息
	kInfo,
	// 警告
	kWarning,
	// 错误
	kError
  };
 public:
  static LoggerSystem &GetInstance();

  /**
   * 写入消息到日志文件，该函数会将消息写入到日志文件当中，写入样式为：[日志消息的级别]-[发生的事件]-[日志消息];
   * @param level 日志消息的级别，详细请查看LogLevel
   * @param message 日志消息.
   */
  void LogMessage(LogLevel level, const QString &message);
 private:
  LoggerSystem();
  ~LoggerSystem();

  /**
   * 将日志消息级别转为字符串
   * @param level 日志消息界别，详细请查看LogLevel
   * @return 形式为取消k的全部大写
   */
  QString LogLevelToString(LogLevel level);

  /**
   * 建立日志消息的文件和日志消息的目录，默认为{用户名}/{local}/{项目名称}//application.log。
   * 每次程序启动时都会检查该目录是否存在，如果不存在则会创建。但是如果是在程序运行的时候就删除，
   * 那么就无法写入日志消息。
   */
  void EnsureLogFile();

 private:
  // 日志文件，默认路径为{用户名}/{local}/{项目名称}//application.log
  QFile log_file_;
  QTextStream log_stream_;
};

#endif // LOGGERSYSTEM_H
