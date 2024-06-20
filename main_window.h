#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * 开始界面
 */
class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  // QWidget interface
 protected:
  void resizeEvent(QResizeEvent *event) override;

 private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
