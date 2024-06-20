#include "student_window.h"
#include "qdialogbuttonbox.h"
#include "qlabel.h"
#include "ui_studentwindow.h"
#include "login_window.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QStringListModel>
#include <QStringList>
#include <QDialog>

StudentWindow::StudentWindow(Admin *admin, Student &student,
							 CourseManager *course_manager,
							 QWidget *parent)
	: QWidget(parent), ui(new Ui::StudentWindow),
	  admin_(admin), current_student_(student), course_manager_(course_manager) {
  ui->setupUi(this);
  this->setWindowTitle("学生界面");
  ui->frame->setFrameShape(QFrame::Box);
  ui->frame->setLineWidth(2);

  ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

  ui->frame->setLayout(ui->verticalLayout);

  ui->frame->setGeometry(0, 0, width(), height());
  this->UpdateCourseView();

  if (current_student_.deregistered_) {
	ui->add_programmes_button->setEnabled(false);
	ui->delete_programmes_button->setEnabled(false);
	ui->logout_button->setEnabled(false);
  }
  connect(ui->add_programmes_button,
		  &QPushButton::clicked,
		  this,
		  &StudentWindow::AddCourse);
  connect(ui->delete_programmes_button,
		  &QPushButton::clicked,
		  this,
		  &StudentWindow::RemoveCourse);
  connect(ui->select_programmes_button,
		  &QPushButton::clicked,
		  this,
		  &StudentWindow::QueryCourses);
  connect(ui->logout_button, &QPushButton::clicked, this, &StudentWindow::Logout);
  connect(ui->exit_button, &QPushButton::clicked, this, [=]() {
	LoginWindow *login_window = new LoginWindow(this->admin_);
	this->close();
	login_window->show();
  });
}

StudentWindow::~StudentWindow() {
  delete ui;
}

void StudentWindow::UpdateCourseView() {
  if (this->current_student_.grades_.isEmpty()) {
	ui->student_table_view->setModel(nullptr);
	return;
  }
  Student student = this->admin_->GetStudent(this->current_student_.id_);

  QStringList headers = {"课程ID", "课程名字", "时长", "成绩"};
  QStandardItemModel *display_model = new QStandardItemModel();
  display_model->setHorizontalHeaderLabels(headers);

  auto grades_keys = student.GetAllGradesKeys();
  auto grades_value = student.GetAllGradeValue();

  for (int row = 0; row < student.grades_.size(); row++) {
	int course_id = grades_keys[row];
	double grade = grades_value[row];
	Course course = this->course_manager_->GetCourse(course_id);

	QList<QStandardItem *> items;
	items.append(new QStandardItem(QString::number(course.course_id)));
	items.append(new QStandardItem(course.course_name));
	items.append(new QStandardItem(QString::number(course.course_duration)));
	items.append(new QStandardItem(grade == -1 ? "N/A" : QString::number(grade)));

	display_model->appendRow(items);
  }
  ui->student_table_view->setModel(display_model);
}

void StudentWindow::AddCourse() {
  if (this->course_manager_->GetAllCourses().isEmpty()) {
	QMessageBox::warning(this, "警告！", "现在还没有课程请通知管理员添加课程后再做选择！");
	return;
  }

  if (this->current_student_.GetAllGradesKeys().size() <= 18) {
	QMessageBox::warning(this,
						 "警告！",
						 "您的课程选择已经超出限制，请删除部分课程后再选择!");
	return;
  }

  QVector<Course> enrolled_course = this->course_manager_->GetAllCourses();
  QStringList course_names;
  for (const auto &course_id : enrolled_course) {
	course_names << course_id.course_name;
  }

  while (true) {
	QString course_name =
		QInputDialog::getItem(this, "移除课程", "添加的名字： ", course_names, 0, false);
	if (course_name.isEmpty())
	  return;

	Course selected_course;
	for (const auto &course_id : enrolled_course) {
	  if (course_id.course_name == course_name) {
		selected_course = course_id;
		break;
	  }
	}

	int selected_course_id = selected_course.course_id;
	if (std::any_of(this->current_student_.GetAllGradesKeys().cbegin(),
					this->current_student_.GetAllGradesKeys().cend(),
					[&selected_course_id](int value) {
					  return selected_course_id == value;
					})) {
	  int result = QMessageBox::warning(this,
										"Confirm",
										"发现重复输入，是否要重新选择?",
										QMessageBox::Yes | QMessageBox::No
											| QMessageBox::Close);
	  if (result == QMessageBox::Yes)
		continue;
	  else {
		return;
	  }
	}

	int result = QMessageBox::question(this,
									   "Confirm",
									   "你确定要添加该课程吗？",
									   QMessageBox::Yes | QMessageBox::No
										   | QMessageBox::Close);
	if (result == QMessageBox::No)
	  continue;
	else if (result == QMessageBox::Close) {
	  return;
	}

	this->admin_->RegisterGrade(this->current_student_.id_, selected_course.course_id);
	this->current_student_ = this->admin_->GetStudent(this->current_student_.id_);

	this->UpdateCourseView();
	break;
  }
}

void StudentWindow::RemoveCourse() {
  if (this->current_student_.grades_.isEmpty()) {
	QMessageBox::warning(this,
						 "警告！",
						 "您现在还没有选择课程，请先选择课程之后再进行移除!");
	return;
  }

  QVector<int> grade_name = this->current_student_.GetAllGradesKeys();
  QStringList course_names;
  for (const auto &course_id : grade_name) {
	Course course = this->course_manager_->GetCourse(course_id);
	course_names << course.course_name;
  }

  while (true) {
	QString course_name =
		QInputDialog::getItem(this, "移除课程", "课程的名字： ", course_names, 0, false);
	if (course_name.isEmpty())
	  return;

	Course selected_course;
	for (const auto &course_id : grade_name) {
	  Course course = this->course_manager_->GetCourse(course_id);
	  if (course.course_name == course_name) {
		selected_course = course;
		break;
	  }
	}

	int result = QMessageBox::question(this,
									   "Confirm",
									   "你确定要取消该课程吗？",
									   QMessageBox::Yes | QMessageBox::No
										   | QMessageBox::Close);

	if (result == QMessageBox::No)
	  continue;
	else if (result == QMessageBox::Close) {
	  return;
	}

	this->admin_->UnenrollCourse(this->current_student_.id_, selected_course.course_id);
	this->current_student_ = this->admin_->GetStudent(this->current_student_.id_);

	QMessageBox::information(this, "Confirm", "删除成功！");

	this->UpdateCourseView();
	break;
  }
}

void StudentWindow::QueryCourses() {
  QDialog dialog;
  dialog.setWindowTitle("查询窗口");
  QVBoxLayout *layout = new QVBoxLayout(&dialog);
  QLabel *label = new QLabel("请输入查询的ID： ", &dialog);

  layout->addWidget(label);
  QLineEdit *line_edit = new QLineEdit(&dialog);
  layout->addWidget(line_edit);

  QDialogButtonBox *button_box = new QDialogButtonBox(
	  QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

  layout->addWidget(button_box);

  connect(button_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, &dialog,
		  [&dialog, this, &line_edit]() {
			bool ok;
			int course_id = line_edit->text().toInt(&ok);
			QVector<int>
				enrolled_courses_index = this->current_student_.GetEnrolledCourses();
			if (!std::any_of(enrolled_courses_index.cbegin(),
							 enrolled_courses_index.cend(),
							 [course_id](int value) {
							   return course_id == value;
							 })) {
			  QMessageBox::warning(this,
								   "警告！",
								   "您没有添加该课程，请添加该课程后再进行查询！");
			  return;
			}
			if (ok) {
			  QString string =
				  this->admin_->GenerateReport(this->current_student_.id_, course_id);
			  QMessageBox::information(this, "查询到的成绩： ", string);
			  dialog.accept();
			}
		  });

  connect(button_box->button(QDialogButtonBox::Cancel),
		  &QPushButton::clicked,
		  &dialog,
		  [&dialog]() {
			dialog.reject();
		  });

  dialog.exec();
}

void StudentWindow::Logout() {
  if ((QMessageBox::warning(this,
							"警告！",
							"您现在正在进行注销操作是否真的要注销？",
							QMessageBox::Yes | QMessageBox::No | QMessageBox::Close))
	  != QMessageBox::Yes)
	return;
  QMessageBox::information(this, "消息", "注销成功！");
  // Disable buttons for adding and removing courses
  ui->add_programmes_button->setEnabled(false);
  ui->delete_programmes_button->setEnabled(false);
  ui->logout_button->setEnabled(false);

  this->current_student_.deregistered_ = true;
  this->admin_->DeregisterStudent(this->current_student_.id_);
  this->current_student_ = this->admin_->GetStudent(this->current_student_.id_);
}

void StudentWindow::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
	  foreach(QObject *child, children()) {
	  if (auto *frame = qobject_cast<QFrame *>(child))
		frame->setGeometry(0, 0, width(), height());
	}
}
