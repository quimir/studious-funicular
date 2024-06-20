#include "login_window.h"
#include "qsqlquery.h"
#include "ui_loginwindow.h"
#include "registration_window.h"
#include <QMessageBox>
#include <QPushButton>
#include "student_window.h"
#include "student.h"
#include <QWidget>
#include "teacher_window.h"

LoginWindow::LoginWindow(Admin *admin, QWidget *parent)
	: QMainWindow(parent), admin_(admin), ui(new Ui::LoginWindow) {
  ui->setupUi(this);
  this->setWindowTitle("登入界面");
  // ui->frame->setStyleSheet("QWidget { border: 1px solid black; }");
  /**
   * 设定界面的风格，并且绑定用户将界面进行移动或者放大缩小。
   */
  ui->frame->setFrameShape(QFrame::Box);
  ui->frame->setLineWidth(2);
  ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

  ui->frame->setLayout(ui->verticalLayout);

  ui->frame->setGeometry(0, 0, ui->centralwidget->width(), ui->centralwidget->height());

  connect(ui->exit_button, &QPushButton::clicked, this, [=]() {
	this->close();
  });
  ui->password_line_edit->setEchoMode(QLineEdit::Password);

  connect(ui->register_button, &QPushButton::clicked, this, [=]() {
	auto *registration_window = new RegistrationWindow(this->admin_);
	registration_window->setGeometry(this->geometry());
	this->close();
	registration_window->show();
  });

  connect(ui->sign_in_button, &QPushButton::clicked, this, &LoginWindow::AttempLogin);
}

LoginWindow::~LoginWindow() {
  delete ui;
}

void LoginWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  QWidget *central_widget = this->centralWidget();
  if (central_widget) {
		foreach (QObject *child, central_widget->children()) {
		if (auto *frame = qobject_cast<QFrame *>(child)) {
		  frame->setGeometry(0, 0, central_widget->width(), central_widget->height());
		}
	  }
  }
}

void LoginWindow::AttempLogin() {
  QString user_name = ui->user_name_line_edit->text();
  QString password = ui->password_line_edit->text();

  if (user_name == "root" && password == "1234") {
	auto *teacher_window =
		new TeacherWindow(this->admin_, this->admin_->GetCourseManager());
	teacher_window->setGeometry(geometry());
	this->close();
	teacher_window->show();
	return;
  }

  QSqlQuery query;
  query.prepare("SELECT id, name, password FROM students WHERE id = ?");
  query.addBindValue(user_name);
  if (!query.exec() || !query.next()) {
	QMessageBox::warning(this, "登录失败！", "该用户名或者密码没有初始化！");
	return;
  }

  Student student = this->admin_->GetStudent(user_name);
  if (student.id_.isEmpty() || student.password_ != password) {
	QMessageBox::warning(this, "登录错误！", "账号或者密码错误！");
	return;
  }

  auto *student_window =
	  new StudentWindow(admin_, student, this->admin_->GetCourseManager());
  student_window->setGeometry(geometry());
  this->close();
  student_window->show();
}
