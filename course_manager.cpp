#include "course_manager.h"
#include "qdir.h"
#include "qstandardpaths.h"
#include "logger_system.h"

CourseManager::~CourseManager() {
  SaveCourses();
}

void CourseManager::AddCourse(int id, const QString &name, int duration) {
  this->courses_.append(Course{id, name, duration});
  SaveCourses();
}

void CourseManager::RemoveCourse(int id) {
  auto it = std::remove_if(courses_.begin(), courses_.end(), [id](const Course &course) {
	return course.course_id == id;
  });
  courses_.erase(it, courses_.end());
  SaveCourses();
}

QVector<Course> CourseManager::GetAllCourses() const {
  return courses_;
}

Course CourseManager::GetCourse(int id) const {
  for (const auto &course : courses_) {
	if (course.course_id == id)
	  return course;
  }

  return Course{-1, "", 0};
}

CourseManager::CourseManager() {
  InitializeFilePath();
  LoadCourses();
}

void CourseManager::InitializeFilePath() {
  QString data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkdir(data_dir);
  file_path_ = data_dir + "/programmes.txt";
}

void CourseManager::LoadCourses() {
  QFile file(file_path_);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	QTextStream in(&file);
	while (!in.atEnd()) {
	  QStringList fields = in.readLine().split(",");
	  if (fields.size() == 3) {
		int id = fields[0].toInt();
		QString name = fields[1];
		int duration = fields[2].toInt();
		courses_.append(Course{id, name, duration});
	  }
	}
	file.close();
  }
}

void CourseManager::SaveCourses() const {
  QFile file(file_path_);
  QFileInfo file_info(file_path_);
  if (!file_info.exists()) {
	// 假设没有则创建该目录
	QDir().mkdir(file_info.dir().path());
  }
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
	QTextStream out(&file);
	for (const auto &course : courses_) {
	  out << course.course_id << "," << course.course_name << ","
		  << course.course_duration << "\n";
	}
	file.close();
  } else {
	LoggerSystem::GetInstance().LogMessage(LoggerSystem::LogLevel::kError,
										   "Serious mistake! Discovered that the course file "
										   "was not generated and could not be written "
										   "to either.");
  }
}
