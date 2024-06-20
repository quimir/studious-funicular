#include "registration_window.h"
#include "qsqlquery.h"
#include "ui_registrationwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "login_window.h"

QRegularExpression RegistrationWindow::re_("^[A-Za-z0-9@￥]+$");

RegistrationWindow::RegistrationWindow(Admin *admin, QWidget *parent)
	: QWidget(parent), admin_(admin), ui(new Ui::RegistrationWindow) {
  ui->setupUi(this);
  this->setWindowTitle("注册界面");
  /**
   * 设定界面的风格，并且绑定用户将界面进行移动或者放大缩小。
   */
  ui->user_password_line_edit->setEchoMode(QLineEdit::Password);

  ui->frame->setFrameShape(QFrame::Box);
  ui->frame->setLineWidth(2);

  ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

  ui->frame->setLayout(ui->verticalLayout);

  ui->frame->setGeometry(0, 0, width(), height());
  connect(ui->registration_button, &QPushButton::clicked,
		  this, &RegistrationWindow::AttempRegistration);

  connect(ui->exit_button, &QPushButton::clicked, this, [=]() {
	LoginWindow *login_window = new LoginWindow(this->admin_);
	login_window->setGeometry(geometry());
	this->close();
	login_window->show();
  });
}

RegistrationWindow::~RegistrationWindow() {
  delete ui;
}

void RegistrationWindow::AttempRegistration() {
  QString user_name = ui->user_name_line_edit->text();
  QString user_id = ui->user_id_line_edit->text();
  QString user_password = ui->user_password_line_edit->text();

  if (user_id.isEmpty() || user_name.isEmpty() || user_password.isEmpty()) {
	QMessageBox::warning(this, "注册错误！",
						 "您有一项为空，请重新检查输入是否正确！",
						 QMessageBox::Ok | QMessageBox::Close);
	return;
  }

  if (!re_.match(user_id).hasMatch() ||
	  !re_.match(user_password).hasMatch()) {
	QMessageBox::warning(this,
						 "注册错误！",
						 "您的账号或密码不符合纯英文/纯数字/英文+数字/英文+数字+特殊字符(@$)的组合，"
						 "请您重试!",
						 QMessageBox::Ok | QMessageBox::Close);
	return;
  }

  QSqlQuery query;
  query.prepare("SELECT id FROM students WHERE id = ?");
  query.addBindValue(user_id);
  if (query.exec() && query.next()) {
	QMessageBox::warning(this, "注册错误!",
						 "查询到注册的账号已重复，请重新输入账号！",
						 QMessageBox::Ok | QMessageBox::Close);
	return;
  }

  Student new_student(user_name, user_id, user_password, false);
  this->admin_->RegisterStudent(new_student);
  int ret = QMessageBox::information(this, "注册消息",
									 "注册成功，是否要前往登入?",
									 QMessageBox::Yes, QMessageBox::Close);

  if (ret == QMessageBox::Yes) {
	LoginWindow *login_window = new LoginWindow(this->admin_);
	login_window->setGeometry(this->geometry());
	this->close();
	login_window->show();
  }
}

void RegistrationWindow::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
	  foreach (QObject *child, children()) {
	  if (auto *frame = qobject_cast<QFrame *>(child))
		frame->setGeometry(0, 0, width(), height());

	}
}
