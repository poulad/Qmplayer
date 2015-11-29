#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

#include "qmplayerwidget.h"

namespace Ui {
   class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();



private slots:
   void on_buttonPlayPause_clicked();
   void on_buttonLog_clicked();
   void on_positionSlider_sliderPressed();
   void on_positionSlider_sliderReleased();



private:
   Ui::MainWindow *ui;
   QTextEdit m_TextEdit;
};

#endif // MAINWINDOW_H
