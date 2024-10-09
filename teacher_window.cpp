#include "teacher_window.h"
#include "qinputdialog.h"
#include "qstandarditemmodel.h"
#include "ui_teacherwindow.h"
#include <QFrame>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include "login_window.h"
#include "function_return_setting.h"

TeacherWindow::TeacherWindow(Admin *admin, CourseManager *course_manager, QWidget *parent)
	: QWidget(parent),
	  ui(new Ui::TeacherWindow),
	  admin_(admin),
	  course_manager_(course_manager) {
  ui->setupUi(this);
  this->setWindowTitle("教师界面");
  ui->frame->setFrameShape(QFrame::Box);
  ui->frame->setLineWidth(2);

  ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

  ui->frame->setLayout(ui->verticalLayout);

  ui->frame->setGeometry(0, 0, width(), height());
  this->UpdateCourseView();

  connect(ui->add_programmes_button,
		  &QPushButton::clicked,
		  this,
		  &TeacherWindow::AddCourse);
  connect(ui->delete_programmes_button,
		  &QPushButton::clicked,
		  this,
		  &TeacherWindow::DeleteCourse);
  connect(ui->add_grade_button, &QPushButton::clicked, this, &TeacherWindow::AddGrade);
  connect(ui->exit_button, &QPushButton::clicked, this, [=]() {
	auto *login_window = new LoginWindow(this->admin_);
	login_window->setGeometry(this->geometry());
	this->close();
	login_window->show();
  });
}

TeacherWindow::~TeacherWindow() {
  delete ui;
}

void TeacherWindow::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
	  foreach (QObject *child, children()) {
	  if (auto *frame = qobject_cast<QFrame *>(child))
		frame->setGeometry(0, 0, width(), height());

	}
}

void TeacherWindow::UpdateCourseView() {
  QVector<Course> courses = this->course_manager_->GetAllCourses();
  QStringList headers = {"课程ID", "课程名字", "课程课时"};
  auto *model = new QStandardItemModel(courses.size(), headers.size());

  model->setHorizontalHeaderLabels(headers);
  for (int row = 0; row < courses.size(); row++) {
	model->setItem(row, 0, new QStandardItem(QString::number(courses[row].course_id)));
	model->setItem(row, 1, new QStandardItem(courses[row].course_name));
	model->setItem(row,
				   2,
				   new QStandardItem(QString::number(courses[row].course_duration)));
  }

  ui->student_table_view->setModel(model);
}

int TeacherWindow::SetIntDialog(const QString &dialog_title,
								const QString &label_text,
								int &int_value) {
  QDialog dialog;
  dialog.setWindowTitle(dialog_title);

  auto *layout = new QVBoxLayout(&dialog);
  auto *label = new QLabel(label_text, &dialog);

  layout->addWidget(label);

  auto *line_edit = new QLineEdit(&dialog);
  layout->addWidget(line_edit);

  auto *button_box = new QDialogButtonBox(
	  QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

  layout->addWidget(button_box);

  int is_value = functionreturnsetting::kIntMax;

  connect(button_box->button(QDialogButtonBox::Ok),
		  &QPushButton::clicked,
		  &dialog,
		  [&int_value, &line_edit, this, &is_value, &dialog]() {
			bool ok;
			int value = line_edit->text().toInt(&ok);
			if (!ok || !(value > 0 && value < 10000000)) {
			  int button_click = QMessageBox::warning(this,
													  "提示！",
													  "发现输入的值有错误是否要重新输入？",
													  QMessageBox::Ok
														  | QMessageBox::Close);
			  if (button_click == QMessageBox::Ok)
				is_value = functionreturnsetting::kDialogFailed;
			  else if (button_click == QMessageBox::Close)
				is_value = functionreturnsetting::kDialogClean;
			} else {
              auto course_manager_ids=this->course_manager_->GetAllCourses();
              if (std::any_of(course_manager_ids.cbegin(),
                              course_manager_ids.cend(),
							  [value](const Course &course) {
								return course.course_id == value;
							  })) {
				int message_click = QMessageBox::warning(this,
														 "提示！",
														 "发现输入的值已经重复否要重新输入？",
														 QMessageBox::Ok
															 | QMessageBox::Close);
				if (message_click == QMessageBox::Yes)
				  is_value = functionreturnsetting::kDialogFailed;
				else if (message_click == QMessageBox::Close)
				  is_value = functionreturnsetting::kDialogClean;
			  } else {
				int_value = value;
				is_value = functionreturnsetting::kDialogSuccess;
				dialog.accept();
			  }
			};
		  });

  connect(button_box->button(QDialogButtonBox::Cancel),
		  &QPushButton::clicked,
		  &dialog,
		  [&is_value, &dialog]() {
			is_value = functionreturnsetting::kDialogClean;
			dialog.reject();
		  });

  dialog.exec();
  return is_value;
}

void TeacherWindow::AddCourse() {
  int dialog_value;
  int course_id, course_duration;
  QString course_name;

  while (true) {
	dialog_value =
		SetIntDialog("添加课程", "请添加课程ID,数值限定在0-10000000之内： ", course_id);
	if (dialog_value == functionreturnsetting::kDialogSuccess)
	  break;
	else if (dialog_value == functionreturnsetting::kDialogClean)
	  return;
  }

  while (true) {
	dialog_value = SetStringDialog("添加课程", "请输入课程的名字： ", course_name);
	if (dialog_value == functionreturnsetting::kDialogSuccess)
	  break;
	else if (dialog_value == functionreturnsetting::kDialogClean)
	  return;
  }

  while (true) {
    dialog_value = SetIntDialog("添加课程", "请输入课程的课时： ", course_duration);
	if (dialog_value == functionreturnsetting::kDialogSuccess)
	  break;
	else if (dialog_value == functionreturnsetting::kDialogClean)
	  return;
  }

  this->course_manager_->AddCourse(course_id, course_name, course_duration);
  this->UpdateCourseView();
}

void TeacherWindow::DeleteCourse() {
  QVector<Course> courses = this->course_manager_->GetAllCourses();
  if (courses.isEmpty()) {
	QMessageBox::warning(this,
						 "错误！",
						 "您现在还没有添加课程请添加课程之后再进行删除的选择!");
	return;
  }

  QStringList course_ids;
  for (const auto &course : courses) {
	course_ids << QString::number(course.course_id);
  }
  bool ok;
  while (true) {
	QString course_id_str = QInputDialog::getItem(this,
                                                  "删除课程",
												  "请选择需要删除课程的ID： ",
												  course_ids,
												  0,
												  false,
												  &ok);
	if (!ok || course_id_str.isEmpty()) {
	  int result = QMessageBox::warning(this,
										"错误！",
										"选择错误或者没有进行选择，是否需要重新选择？",
										QMessageBox::Yes | QMessageBox::Close);
	  if (result == QMessageBox::Yes) {
		continue;
	  } else if (result == QMessageBox::Close) {
		return;
	  }
	}

	int course_id = course_id_str.toInt();
	Course course = this->course_manager_->GetCourse(course_id);
	if (course.course_id == -1) {
	  QMessageBox::warning(this, "错误！", "发生删除错误！该ID不存在请重新选择！");
        return;
	}

	int reuslt = QMessageBox::question(this,
									   "Delete Course",
									   "你确定真的要删除"
										   + this->course_manager_->GetCourse(course_id).course_name
										   + "该课程吗？",
									   QMessageBox::Yes | QMessageBox::No
										   | QMessageBox::Close);
	if (reuslt == QMessageBox::No)
	  continue;
	else if (reuslt == QMessageBox::Close)
	  return;

	QMessageBox::information(this,
                             "删除课程消息",
							 "删除成功，课程ID："
                                 + QString::number(
                                     course_id) + "课程名字： " +
								 this->course_manager_->GetCourse(course_id).course_name
								 + "课程时长： " +
								 QString::number(this->course_manager_->GetCourse(
									 course_id).course_duration));
	this->course_manager_->RemoveCourse(course_id);
	this->UpdateCourseView();
	break;
  }
}

void TeacherWindow::AddGrade() {
  if (this->admin_->GetAllStudent().isEmpty()) {
	QMessageBox::warning(this,
						 "警告！",
						 "没有学生进行注册，请学生注册之后再进行成绩的添加！");
	return;
  }

  if (this->course_manager_->GetAllCourses().isEmpty()) {
	QMessageBox::warning(this, "警告！", "当前没有课程，请添加完课程之后再进行修改成绩！");
	return;
  }

  QDialog dialog(this);
  dialog.setWindowTitle("添加成绩");

  QVBoxLayout layout(&dialog);

  QComboBox student_combo_box(&dialog);
  QLabel student_name_label(&dialog);
  QVector<Student> students = admin_->GetAllStudent();
  for (const Student &student : students) {
	student_combo_box.addItem(student.id_);
  }

  student_combo_box.setCurrentIndex(0);// Trigger initial update
  QString student_id = student_combo_box.currentText();
  QComboBox course_combo_box(&dialog);
  QVector<int> courses = this->admin_->GetStudent(student_id).GetAllGradesKeys();
  for (const int &course : courses) {
	course_combo_box.addItem(this->course_manager_->GetCourse(course).course_name);
  }

  connect(&student_combo_box,
		  &QComboBox::currentIndexChanged,
		  this,
		  [&student_id, &student_name_label, &students, &student_combo_box, &courses, this, &course_combo_box]() {
			student_id = student_combo_box.currentText();
			for (const Student &student : students) {
			  if (student.id_ == student_id) {
				student_name_label.setText(student.name_);
				break;
			  }
			}

			course_combo_box.clear();
			courses = this->admin_->GetStudent(student_id).GetAllGradesKeys();
			for (const int &course : courses) {
			  course_combo_box.addItem(this->course_manager_->GetCourse(course).course_name);
			}
		  });

  QLineEdit grade_edit(&dialog);
  grade_edit.setValidator(new QDoubleValidator(0, 100, 2, &dialog));

  QDialogButtonBox button_box(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

  auto *student_id_label = new QLabel("学生ID： ", &dialog);
  auto *name_label = new QLabel("学生姓名： ", &dialog);
  auto *programmes_name = new QLabel("课程名字： ", &dialog);
  auto *programmes_grade = new QLabel("课程成绩： ", &dialog);

  layout.addWidget(student_id_label);
  layout.addWidget(&student_combo_box);
  layout.addWidget(name_label);
  layout.addWidget(&student_name_label);
  layout.addWidget(programmes_name);
  layout.addWidget(&course_combo_box);
  layout.addWidget(programmes_grade);
  layout.addWidget(&grade_edit);
  layout.addWidget(&button_box);

  connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  if (dialog.exec() == QDialog::Accepted) {
	QString student_id = student_combo_box.currentText();
	QString course_name = course_combo_box.currentText();
	double grade = grade_edit.text().toDouble();

	int course_id = -1;
	for (int value = 0; value < courses.size(); value++) {
	  if (this->course_manager_->GetCourse(courses[value]).course_name == course_name) {
		course_id = courses[value];
		break;
	  }
	}

	if (course_id == -1) {
      QMessageBox::warning(this, "添加成绩警告！", "没有找到该课程的ID！");
	  return;
	}

	admin_->RegisterGrade(student_id, course_id, grade);
	QMessageBox::information(this, "添加成功", "您已成功添加学生信息！");
  }
}
QList<Course>::iterator TeacherWindow::GetCourseManagerBegin() const {
  return this->course_manager_->GetAllCourses().begin();
}
QList<Course>::iterator TeacherWindow::GetCourseManagerEnd() const {
  return this->course_manager_->GetAllCourses().end();
}
int TeacherWindow::SetStringDialog(const QString &dialog_title,
								   const QString &label_text,
								   QString &string_value) {
  QDialog dialog;
  dialog.setWindowTitle(dialog_title);

  auto *layout = new QVBoxLayout(&dialog);
  auto *label = new QLabel(label_text, &dialog);

  layout->addWidget(label);

  auto *line_edit = new QLineEdit(&dialog);
  layout->addWidget(line_edit);

  auto *button_box = new QDialogButtonBox(
	  QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
  layout->addWidget(button_box);

  int is_value = functionreturnsetting::kIntMax;

  connect(button_box->button(QDialogButtonBox::Ok),
		  &QPushButton::clicked,
		  &dialog,
		  [&string_value, line_edit, this, &is_value, &dialog]() {
			bool ok;
			QString value = line_edit->text().toUtf8();
			ok = value.isEmpty();
			if (ok) {
			  int button_click = QMessageBox::warning(this,
													  "提示！",
													  "发现输入的值有错误是否要重新输入？",
													  QMessageBox::Ok
														  | QMessageBox::Close);
			  if (button_click == QMessageBox::Ok)
				is_value = functionreturnsetting::kDialogFailed;
			  else if (button_click == QMessageBox::Close)
				is_value = functionreturnsetting::kDialogClean;
			} else {
			  auto course_manager_begin = this->course_manager_->GetAllCourses().cbegin();
			  auto course_manager_end = this->course_manager_->GetAllCourses().cend();
			  if (std::any_of(course_manager_begin,
							  course_manager_end,
							  [&value](const Course &course) {
								return course.course_name == value;
							  })) {
				int message_click = QMessageBox::warning(this,
														 "提示！",
														 "发现输入的值已经重复否要重新输入？",
														 QMessageBox::Ok
															 | QMessageBox::Close);
				if (message_click == QMessageBox::Yes)
				  is_value = functionreturnsetting::kDialogFailed;
				else if (message_click == QMessageBox::Close)
				  is_value = functionreturnsetting::kDialogClean;
			  } else {
				string_value = value;
				is_value = functionreturnsetting::kDialogSuccess;
				dialog.accept();
			  }
			};
		  });

  connect(button_box->button(QDialogButtonBox::Cancel),
		  &QPushButton::clicked,
		  &dialog,
		  [&is_value, &dialog]() {
			is_value = functionreturnsetting::kDialogClean;
			dialog.reject();
		  });
  dialog.exec();
  return is_value;
}
