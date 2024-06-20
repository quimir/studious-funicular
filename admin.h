#ifndef ADMIN_H
#define ADMIN_H

#include "student.h"
#include <QVector>
#include <QString>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "course_manager.h"

/**
 * 数据中心类，本数据全部使用QSQLLite存储，所有的数据由该类进行处理。
 */
class Admin {
 public:
  Admin();

  ~Admin();

  /**
   * 插入学生信息，其会将输入的学生信息插入到表当中，如果发生错误则会显示在日志文件当中。
   * @param student 学生信息
   */
  void RegisterStudent(const Student &student);

  void RegisterStudent(const QString &user_id,
					   const QString &user_name,
					   const QString &user_password,
					   bool deregistered = false);

  /**
   * 修改学生的注销信息，通过学生id作为索引，如果找到了该id那么就会进行更改，并且写入到数据库当中。
   * @param student_id 学生ID
   */
  void DeregisterStudent(const QString &student_id);

  /**
   * 更新学生信息，如果可以在学生信息当中找到，则在数据库和本类当中更新信息。
   * @param student 需要更新的学生信息
   */
  void UpdateStudent(const Student &student);

  /**
   * 通过学生id来获取学生信息
   * @param student_id 学生id
   * @return 学生信息
   */
  Student GetStudent(const QString &student_id) const;

  void RegisterGrade(const QString &student_id,
					 const QString &course_name,
					 double grade = -1);

  /**
   * 依据学生id索引更改该学生课程的成绩。
   * @param student_id 学生id 
   * @param course_id 课程id
   * @param grade 学生成绩
   */
  void RegisterGrade(const QString &student_id, int course_id, double grade = -1);

  /**
   * 依据学生id删除该学生的课程。通过学生id索引到学生的信息，如果索引得到则进行课程的查找，如果该课程存在则
   * 将其进行删除。
   * @param student_id 学生id
   * @param course_id 课程id
   */
  void UnenrollCourse(const QString &student_id, int course_id);

  /**
   * 将课程添加到该学生信息当中。
   * @param student_id 学生id
   * @param course_id 课程id
   */
  void UnerollCourse(const QString &student_id, int course_id);

  /**
   * 获取学生订阅的全部课程。
   * @param student_id 学生id
   * @return 输出的格式为：学生id+学生姓名+课程+课程ID+课程名字+课程成绩。
   */
  QString GenerateReport(const QString &student_id) const;

  /**
   * 获取学生在该课程下的全部信息。
   * @param student_id 学生id
   * @param course_id 课程id
   * @return 课程ID+课程名字+课程时长+成绩
   */
  QString GenerateReport(const QString &student_id, int course_id);

  QSqlTableModel *GetStudentModel() const;

  QSqlDatabase GetDb() const;

  CourseManager *GetCourseManager();

 private:
  /**
   * 初始化数据库里的数据，默认数据库的路径为{用户名}/{local}/{项目名称}/student_management.db。
   * 其首先会检查该数据库是否存在，如果不存在那么就调用初始化数据库，一般有三个表，student存储学生消息，
   * grades存储学生id对应的学生所选课程的id和成绩，另外一个则是enrollments存储学生id-课程消息，该部分
   * 一一对应不可修改！默认选择的表是students。
   */
  void InitializeDatabase();

  /**
   * 读取数据库当中的学生信息，其会在三个表当中提取全部信息放入在成员student_当中。
   */
  void LoadStudents();

 public:
  const QVector<Student> &GetAllStudent() const;

 private:
  QVector<Student> student_;
  QSqlDatabase db_;
  QSqlTableModel *student_model_{};
  CourseManager *course_manager_;
};

#endif // ADMIN_H
