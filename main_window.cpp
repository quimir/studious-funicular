#include "main_window.h"
#include "./ui_mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include "login_window.h"
#include "admin.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setWindowTitle("学生管理系统");

  ui->main_vbox_layout->setContentsMargins(0, 0, 0, 0);
  ui->main_vbox_layout->setSpacing(0);

  ui->centralwidget->setLayout(ui->main_vbox_layout);

  connect(ui->exit_button, &QPushButton::clicked, this, [=]() {
	this->close();
  });
  connect(ui->sign_in_button, &QPushButton::clicked, this, [=]() {
	Admin *admin = new Admin();
	LoginWindow *login_window = new LoginWindow(admin);
	login_window->setGeometry(geometry());
	this->close();
	login_window->show();
  });
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {

}
