#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "admin.h"
#include <QRegularExpression>

namespace Ui {
class LoginWindow;
}

/**
 * 登入界面，此界面用来处理登入信息。
 */
class LoginWindow : public QMainWindow {
 Q_OBJECT

 public:
  explicit LoginWindow(Admin *admin, QWidget *parent = nullptr);
  ~LoginWindow();

  // QWidget interface
 protected:
  void resizeEvent(QResizeEvent *event) override;

 private slots:
  /**
   * 登入消息处理函数，触发该函数时会进行判断，如果输入的内容为管理员则会创建管理员的窗口进行管理员，
   * 否则进行数据库的查询，数据库当中如果存在其的ID就对密码进行判断，当ID和密码通过时就会进行学生界面的创建。
   */
  void AttempLogin();

 private:
  Admin *admin_;
  Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
