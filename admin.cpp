#include "admin.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qstandardpaths.h"
#include <QDir>
#include "logger_system.h"

Admin::Admin() : course_manager_(new CourseManager()) {
  InitializeDatabase();
  if (this->db_.isOpen()) {
	this->LoadStudents();
  }
}

Admin::~Admin() {
  delete this->student_model_; // 确保释放模型对象
  if (this->db_.isOpen()) {
	this->db_.close();
  }
  delete this->course_manager_;
}

void Admin::RegisterStudent(const Student &student) {
  if (!this->db_.isOpen()) {
	LoggerSystem::GetInstance().
		LogMessage(LoggerSystem::LogLevel::kWarning,
				   "Database not open. Unable to register student.");
  }
  QSqlQuery query;
  query.prepare(
	  "INSERT INTO students (id, name, password, is_deregistered) VALUES (?, ?, ?, ?)");
  query.addBindValue(student.id_);
  query.addBindValue(student.name_);
  query.addBindValue(student.password_);
  query.addBindValue(student.deregistered_ ? 1 : 0);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to register student - " +
											   query.lastError().text());
  }

  this->student_.append(student);
  this->student_model_->select();
}

void Admin::DeregisterStudent(const QString &student_id) {
  bool is_open = false;
  for (Student &student : student_) {
	if (student.id_ == student_id) {
	  student.deregistered_ = true;
	  is_open = true;
	  break;
	}
  }

  if (!is_open) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "The student's ID was not found, please try again.");
  }

  QSqlQuery query;
  query.prepare("UPDATE students SET is_deregistered = 1 WHERE id = ?");
  query.addBindValue(student_id);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to deregister student - "
											   + query.lastError().text());
  }
}

void Admin::UpdateStudent(const Student &student) {
  bool is_open = false;
  for (Student &students : this->student_) {
	if (students.id_ == student.id_) {
	  students = student;
	  is_open = true;
	  break;
	}
  }

  if (!is_open) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Failed to update student information because the student was not found with the updated student ID: "
											   + student.id_);
	return;
  }
  QSqlQuery query;
  query.prepare(
	  "UPDATE students SET name = ?, password = ?, is_deregistered = ? WHERE id = ?");
  query.addBindValue(student.name_);
  query.addBindValue(student.password_);
  query.addBindValue(student.deregistered_ ? 1 : 0);
  query.addBindValue(student.id_);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to update student - " +
											   query.lastError().text());
  }
  this->student_model_->select();
}

Student Admin::GetStudent(const QString &student_id) const {
  for (const Student &student : this->student_) {
	if (student.id_ == student_id) {
	  return student;
	}
  }

  LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										 "Error: Student not found - " + student_id);
  return Student("", "", "");
}

void Admin::RegisterGrade(const QString &student_id,
						  const QString &course_name,
						  double grade) {
  int course_id = -1;
  for (const Course &course : this->course_manager_->GetAllCourses()) {
	if (course.course_name == course_name) {
	  course_id = course.course_id;
	  break;
	}
  }

  if (course_id == -1) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: course not found - " + course_name);
	return;
  }

  RegisterGrade(student_id, course_id, grade);
}

QString Admin::GenerateReport(const QString &student_id) const {
  QString report;
  Student student = this->GetStudent(student_id);
  report += "Student ID: " + student.id_ + "\n";
  report += "Name: " + student.name_ + "\n";

  QSqlQuery query;
  query.prepare("SELECT course_id, grade FROM grades WHERE student_id = ?");
  query.addBindValue(student_id);
  if (query.exec()) {
	while (query.next()) {
	  int course_id = query.value("course_id").toInt();
	  double grade = query.value("grade").toDouble();
	  Course course = this->course_manager_->GetCourse(course_id);
	  report +=
		  QString("课程ID： ") + QString::number(course_id) + " - " + QString("课程名字： ")
			  + course.course_name + ": " + QString("课程成绩： ") + QString::number(grade)
			  + "\n";
	}
  }

  return report;
}

QSqlTableModel *Admin::GetStudentModel() const {
  return this->student_model_;
}

void Admin::InitializeDatabase() {
  QString data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(data_dir);
  QString db_path = data_dir + "/student_management.db";

  if (QSqlDatabase::contains("qt_sql_default_connection")) {
	this->db_ = QSqlDatabase::database("qt_sql_default_connection");
  } else {
	this->db_ = QSqlDatabase::addDatabase("QSQLITE");
	this->db_.setDatabaseName(db_path);
  }

  if (!this->db_.open()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kError,
										   "Error: Unable to open database");
	return;
  }

  QSqlQuery query;
  query.exec(
	  "CREATE TABLE IF NOT EXISTS students (id TEXT PRIMARY KEY, name TEXT, password TEXT, is_deregistered INTEGER)");
  query.exec(
	  "CREATE TABLE IF NOT EXISTS grades (id INTEGER PRIMARY KEY AUTOINCREMENT, student_id TEXT, course_id INTEGER, grade REAL)");
  query.exec(
	  "CREATE TABLE IF NOT EXISTS enrollments (student_id TEXT, course_id INTEGER, PRIMARY KEY(student_id, course_id))");

  this->student_model_ = new QSqlTableModel(nullptr, this->db_);
  this->student_model_->setTable("students");
  this->student_model_->select();
}

QSqlDatabase Admin::GetDb() const {
  return db_;
}

CourseManager *Admin::GetCourseManager() {
  return this->course_manager_;
}

void Admin::LoadStudents() {
  QSqlQuery query;
  query.exec("SELECT id, name, password, is_deregistered FROM students");
  while (query.next()) {
	QString id = query.value("id").toString();
	QString name = query.value("name").toString();
	QString password = query.value("password").toString();
	bool is_deregistered = query.value("is_deregistered").toInt() == 1;

	Student student(name, id, password, is_deregistered);

	QSqlQuery enrollmentQuery;
	enrollmentQuery.prepare("SELECT course_id FROM enrollments WHERE student_id = ?");
	enrollmentQuery.addBindValue(id);
	if (enrollmentQuery.exec()) {
	  while (enrollmentQuery.next()) {
		int course_id = enrollmentQuery.value("course_id").toInt();
		student.EnrollCourse(course_id);
	  }
	}

	QSqlQuery gradesQuery;
	gradesQuery.prepare("SELECT course_id, grade FROM grades WHERE student_id = ?");
	gradesQuery.addBindValue(id);
	if (gradesQuery.exec()) {
	  while (gradesQuery.next()) {
		int course_id = gradesQuery.value("course_id").toInt();
		double grade = gradesQuery.value("grade").toDouble();
		student.SetStudentGrade(course_id, grade);
	  }
	}

	this->student_.append(student);
  }
}
const QVector<Student> &Admin::GetAllStudent() const {
  return student_;
}

void Admin::RegisterGrade(const QString &student_id, int course_id, double grade) {
  QSqlQuery query;
  query.prepare("SELECT id FROM grades WHERE student_id = ? AND course_id = ?");
  query.addBindValue(student_id);
  query.addBindValue(course_id);

  if (query.exec() && query.next()) {
	query.prepare("UPDATE grades SET grade = ? WHERE student_id = ? AND course_id = ?");
	query.addBindValue(grade);
	query.addBindValue(student_id);
	query.addBindValue(course_id);
  } else {
	query.prepare("INSERT INTO grades (student_id, course_id, grade) VALUES (?, ?, ?)");
	query.addBindValue(student_id);
	query.addBindValue(course_id);
	query.addBindValue(grade);
  }

  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to register or update grade - "
											   + query.lastError().text());
  }

  auto it = std::find_if(this->student_.begin(),
						 this->student_.end(),
						 [&student_id](const Student &student) {
						   return student.id_ == student_id;
						 });
  if (it != this->student_.end()) {
	it->SetStudentGrade(course_id, grade);
  }
}

void Admin::RegisterStudent(const QString &user_id,
							const QString &user_name,
							const QString &user_password,
							bool deregistered) {
  QSqlQuery query;
  query.prepare("INSERT INTO students (id, name, password) VALUES (?, ?, ?)");
  query.addBindValue(user_id);
  query.addBindValue(user_name);
  query.addBindValue(user_password);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to register student - " +
											   query.lastError().text());
  }

  this->student_.append(Student(user_id, user_name, user_password, deregistered));
  this->student_model_->select();
}

void Admin::UnenrollCourse(const QString &student_id, int course_id) {
  QSqlQuery query;
  query.prepare("DELETE FROM enrollments WHERE student_id = ? AND course_id = ?");
  query.addBindValue(student_id);
  query.addBindValue(course_id);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to enroll course - "
											   + query.lastError().text());
	return;
  } else {
	for (Student &student : student_) {
	  if (student.id_ == student_id) {
		student.UnenrollCourse(course_id);
		break;
	  }
	}
  }
}

QString Admin::GenerateReport(const QString &student_id, int course_id) {
  QString report;
  Course course = this->course_manager_->GetCourse(course_id);
  Student student = this->GetStudent(student_id);
  double grade = student.GetStudentGrade(course_id);

  report += "课程ID： " + QString::number(course.course_id) + "\n" + "课程名字： "
      + course.course_name + "\n" + "课程课时： " + QString::number(course.course_duration)
	  +
		  "成绩： " + (grade == -1 ? "N/A" : QString::number(grade)) + "\n";

  return report;
}

void Admin::UnerollCourse(const QString &student_id, int course_id) {
  QSqlQuery query;
  query.prepare("INSERT INTO enrollments (student_id, course_id) VALUES (?, ?)");
  query.addBindValue(student_id);
  query.addBindValue(course_id);
  if (!query.exec()) {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kWarning,
										   "Error: Unable to enroll course - "
											   + query.lastError().text());
  } else {
	for (Student &student : student_) {
	  if (student.id_ == student_id) {
		student.EnrollCourse(course_id);
		break;
	  }
	}
  }
}
