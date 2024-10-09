#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QVector>
#include <QMap>

/**
 * 学生消息类，其存储了学生的ID，学生的姓名，学生的密码，学生选择的课程和成绩。
 */
class Student {
 public:
  Student();

  Student(QString user_id,
		  QString user_name,
		  QString user_password,
		  bool deregistered = false);

  inline bool operator==(const Student &other) const {
	return this->id_ == other.id_;
  }

  QVector<int> GetEnrolledCourses() const;

  void AddEnrolledCourses(int course_id);

  /**
   * 设置学生的目标课程的成绩
   * @param course_id 目标课程
   * @param grades 目标成绩
   */
  void SetStudentGrade(int course_id, double grades = -1.0f);

  /**
   * 默认的添加课程的成绩，如果没有特别说明默认添加的成绩为-1，其可以被SetStudentGrade给取代，
   * 两者在逻辑上基本相同
   * @param course_id 目标课程 
   */
  void EnrollCourse(int course_id);

  void UnenrollCourse(int course_id);

  /**
   * 根据课程ID获取课程的成绩
   * @param course_id 课程ID
   * @return 课程成绩
   */
  double GetStudentGrade(int course_id) const;

  QList<int> GetAllGradesKeys() const;

  QList<double> GetAllGradeValue() const;

 public:
  // 用户ID，唯一属性，所有的学生消息都通过其为索引不能重复
  QString id_;

  // 用户名字，可以进行更改
  QString name_;

  // 用户密码，可以进行更改
  QString password_;

  // 课程索引加成绩
  QMap<int, double> grades_;

  // 表示是否已经注销
  bool deregistered_;

 private:
  // 记录添加的课程，其记录的是课程ID，但是已经被grades取代，如果在安全情况下可以被删除。
  QVector<int> enrolled_courses_;
};

#endif // STUDENT_H
