#include "student.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>

Student::Student() {}

Student::Student(QString user_name, QString user_id,
				 QString user_password,
				 bool deregistered) :
	id_(user_id),
	name_(user_name),
	password_(user_password),
	deregistered_(deregistered) {
}

QVector<int> Student::GetEnrolledCourses() const {
  return this->enrolled_courses_;
}

void Student::AddEnrolledCourses(int course_id) {
  if (!this->enrolled_courses_.contains(course_id))
	this->enrolled_courses_.append(course_id);
}

void Student::SetStudentGrade(int course_id, double grades) {
  grades_[course_id] = grades;
}

void Student::UnenrollCourse(int course_id) {
  this->enrolled_courses_.removeAll(course_id);
  this->grades_.remove(course_id);
}

double Student::GetStudentGrade(int course_id) const {
  return grades_.value(course_id, -1);
}

QList<int> Student::GetAllGradesKeys() const {
  return this->grades_.keys();
}

QList<double> Student::GetAllGradeValue() const {
  return this->grades_.values();
}
void Student::EnrollCourse(int course_id) {
  if (!grades_.contains(course_id)) {
	grades_[course_id] = -1.0f;
  }
}
