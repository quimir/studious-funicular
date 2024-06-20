// #include "TestClass.h"
// #include "qdialogbuttonbox.h"
// #include <QPushButton>
// void TestClass::InitTestCase() {
//     admin_=Admin::GetInstance();
//     course_manager_=CourseManager::GetInstance();
//     teacher_window_=new TeacherWindow(admin_,course_manager_);

//     //Test
//     course_manager_.AddCourse(1,"Math",30);
//     course_manager_.AddCourse(2,"Science",45);

//     //Test Student
//     admin_.RegisterStudent("Alice","alice123","password");
//     admin_.RegisterStudent("Bob","bob456","password");
// }

// void TestClass::CleanupTestCase() {
//     delete teacher_window_;
// }

// void TestClass::TestRegisterStudent() {
//     Student new_student("Charlie","charlie789","password");
//     admin_.RegisterStudent(new_student);

//     Student retrievedStudent = admin_.GetStudent("charlie789");
//     QCOMPARE(retrievedStudent.name_, QString("Charlie"));
//     QCOMPARE(retrievedStudent.id_, QString("charlie789"));
//     QCOMPARE(retrievedStudent.password_, QString("password"));
// }

// void TestClass::TestRegisterOrUpdateGrade() {
//     // 添加测试成绩
//     admin_.RegisterGrade("alice123","Math",95);
//     Student alice=admin_.GetStudent("alice123");
//     QCOMPARE(alice.GetStudentGrade(1), 95.0);

//     // 更新测试成绩
//     admin_.RegisterGrade("alice123","Math",85);
//     alice=admin_.GetStudent("alice123");
//     QCOMPARE(alice.GetStudentGrade(1),85.0);

//     // 测试添加新的课程成绩
//     admin_.RegisterGrade("alice123","Science",90);
//     alice=admin_.GetStudent("alice123");
//     QCOMPARE(alice.GetStudentGrade(2),90);
// }

// void TestClass::TestAddCourse() {
//     QPushButton* add_course_button=teacher_window_->findChild<QPushButton*>("add_programmes_button");
//     QVERIFY(add_course_button!=nullptr);
//     QTest::mouseClick(add_course_button,Qt::LeftButton);

//     QLineEdit *course_id_edit = teacher_window_->findChild<QLineEdit*>();
//     QLineEdit *course_name_edit = teacher_window_->findChild<QLineEdit*>();
//     QLineEdit *course_duration_edit = teacher_window_->findChild<QLineEdit*>();
//     QDialogButtonBox *button_box = teacher_window_->findChild<QDialogButtonBox*>();

//     QVERIFY(course_id_edit!=nullptr);
//     QVERIFY(course_name_edit!=nullptr);
//     QVERIFY(course_duration_edit!=nullptr);
//     QVERIFY(button_box!=nullptr);
// }

// void TestClass::TestDeleteCourse() {

// }
