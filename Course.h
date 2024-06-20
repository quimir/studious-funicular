#ifndef COURSE_H
#define COURSE_H

#include <QString>

/**
 * 课程的属性
 */
struct Course {
  // 课程的ID，唯一索引如果重复会进行检查
  int course_id;
  // 课程的名字，会检查名字是否重复。如果名字重复则不会进行录入。
  QString course_name;
  // 课程的时长，可以进行重复
  int course_duration;
};

#endif // COURSE_H
