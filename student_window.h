
#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QWidget>
#include <QSqlTableModel>
#include <QStringListModel>
#include "student.h"
#include "admin.h"

namespace Ui {
class StudentWindow;
}

/**
 * 学生界面窗口。该窗口会显示学生的所有信息和进行学生的操作。包括添加、删除、查询课程和注销操作。
 */
class StudentWindow : public QWidget {
 Q_OBJECT

 public:
  explicit StudentWindow(Admin *admin, Student &student,
						 CourseManager *course_manager,
						 QWidget *parent = nullptr);
  ~StudentWindow();

 private:
  /**
   * 更新学生订阅课程的视图窗口。其的核心在于将数据传递给QTableView，通过Admin进行学生信息的获取和索引。
   */
  void UpdateCourseView();

 private slots:
  /**
   * 添加课程函数。当函数触发时其首先回去询问是否有课程信息，如果没有则不进行添加。再次会查找是否超出课程限制，
   * 超出则不能添加，在添加的时候会进行索引是否已经添加过该课程，添加过则不能进行添加。
   */
  void AddCourse();

  /**
   * 移除课程函数。当函数触发时其首先会去询问是否有课程信息，如果没有则不进行删除。当用户选择删除时会同步给
   * QTableView
   */
  void RemoveCourse();

  /**
   * 查询课程函数。当函数触发时其首先会去询问是否有课程信息，如果没有则不进行查询。其底层调度的是：
   * QString Admin::GenerateReport(const QString &student_id, int course_id)该函数。
   */
  void QueryCourses();

  /**
   * 注销课程函数。其触发的时候会停用添加课程，删除课程和注销按钮。
   */
  void Logout();

 protected:
  void resizeEvent(QResizeEvent *event) override;

 private:
  Ui::StudentWindow *ui;
  Admin *admin_;
  Student current_student_;
  CourseManager *course_manager_;
};

#endif // STUDENTWINDOW_H
