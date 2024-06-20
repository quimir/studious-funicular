#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QWidget>
#include "admin.h"
#include "course_manager.h"

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QWidget {
 Q_OBJECT

 public:
  explicit TeacherWindow(Admin *admin,
						 CourseManager *course_manager,
						 QWidget *parent = nullptr);
  ~TeacherWindow();

  // QWidget interface
 protected:
  void resizeEvent(QResizeEvent *event) override;

 private:
  /**
   * 更新CourseManager当中的课程信息显示到QTabelView当中。
   */
  void UpdateCourseView();

  /**
   * 设置提示窗口消息。该函数会读取CourseManager类当中的所有消息，不允许添加相同的课程id和课程名字。
   * @param dialog_title 提示窗口标题
   * @param label_text 提示文本
   * @param int_value 需要被更改的变量
   * @return 参见function_return_setting.h
   */
  int SetIntDialog(const QString &dialog_title,
				   const QString &label_text,
				   int &int_value);

  int SetStringDialog(const QString &dialog_title,
					  const QString &label_text,
					  QString &string_value);

  QList<Course>::iterator GetCourseManagerBegin() const;

  QList<Course>::iterator GetCourseManagerEnd() const;

 private slots:
  /**
   * 添加课程信息，其会调用SetIntDialog和SetStringDialog两个作为辅助。
   */
  void AddCourse();

  /**
   * 删除课程信息。其底层调度的是void CourseManager::RemoveCourse(int id)函数。其不允许在没有课程的
   * 时候进行删除操作。
   */
  void DeleteCourse();

  /**
   * 添加学生成绩。不允许没有学生、没有课程、学生没有进行选课的时候添加。在添加时其会获取学生的id作为索引，
   * 并且获取课程的id将其全部带入到void Admin::RegisterGrade(const QString &student_id, 
   * int course_id, double grade)当作作为实现。
   */
  void AddGrade();

 private:
  Ui::TeacherWindow *ui;
  Admin *admin_;
  CourseManager *course_manager_;
};

#endif // TEACHERWINDOW_H
