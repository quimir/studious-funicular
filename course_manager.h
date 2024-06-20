#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include "course.h"
#include <QString>
#include <QVector>

/**
 * 课程管理中心，课程当中所有的信息都通过该类进行。
 */
class CourseManager {
 public:
  CourseManager();

  ~CourseManager();

  /**
   * 添加课程信息，其在进行添加完之后会调用SaveCourses进行课程信息的保存
   * @param id 课程id
   * @param name 课程名字
   * @param duration 课程时长 
   */
  void AddCourse(int id, const QString &name, int duration);

  /**
   * 移除课程信息，因为课程id为唯一索引，所以只需要输入课程id即可移除该id对应的课程信息
   * @param id 课程id
   */
  void RemoveCourse(int id);

  QVector<Course> GetAllCourses() const;

  /**
   * 获取课程id对应的课程信息
   * @param id 课程id
   * @return 课程信息
   */
  Course GetCourse(int id) const;

 private:
  /**
   * 初始化文件路径，默认的路径为{用户名}/{local}/{项目名称}/programmes.txt，
   * 所有的课程消息都存储在其中。
   */
  void InitializeFilePath();

  /**
   * 从课程文件当中读取所有的课程文件信息，其的读取形式和SaveCourses()息息相关，
   * 因此如果更改了SaveCourses请务必修改此函数，否则课程信息将不会完全读取，
   * 所有的课程信息都会存储在courses_成员当中。
   */
  void LoadCourses();

  /**
   * 将课程信息写入到课程文件当中，写入的格式为:课程ID,课程名字,课程的时长。如果没有
   * 创建课程文件那么其会首先创建该文件。
   */
  void SaveCourses() const;
 private:
  // 存储课程信息
  QVector<Course> courses_;
  // 课程信息文件
  QString file_path_;
};

#endif // COURSEMANAGER_H
