#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include "admin.h"

namespace Ui {
class RegistrationWindow;
}

/**
 * 注册界面，用来处理用户的注册。默认对用户的ID和密码进行正则表达式的判断，用户的ID和密码
 * 限定为: 纯英文/纯数字/英文+数字/英文+数字+特殊字符(@￥)
 */
class RegistrationWindow : public QWidget {
 Q_OBJECT

 public:
  explicit RegistrationWindow(Admin *admin, QWidget *parent = nullptr);
  ~RegistrationWindow();

 private slots:
  /**
   * 注册消息处理函数，当触发该函数的是时候会对用户的输入进行判断，如果其不符合正则表达式
   * 那么就提示用户重新输入。如果用户的ID进行了重复那么就提示其的ID已被占用需要重新进行输入。
   */
  void AttempRegistration();

 private:
  // 正则表达式成员，用来限制ID和密码，限制为类的说明。
  static QRegularExpression re_;
  Admin *admin_;
  Ui::RegistrationWindow *ui;

  // QWidget interface
 protected:
  void resizeEvent(QResizeEvent *event);
};

#endif // REGISTRATIONWINDOW_H
