#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   connect(ui->volumeSlider, SIGNAL(valueChanged(int)), ui->mplayerWidget, SLOT(setVolume(int)));
   connect(ui->mplayerWidget, SIGNAL(videoPosChanged(int)), ui->positionSlider, SLOT(setValue(int)));
   connect(ui->mplayerWidget, SIGNAL(videoPosChanged(QTime)), ui->positionTimeEdit, SLOT(setTime(QTime)));
}


MainWindow::~MainWindow()
{
   delete ui;
}



//////////////////////////////////////////////////////////// Private Slots ////////////////////////////////////////////////////////////



void MainWindow::on_buttonPlayPause_clicked()
{
    ui->mplayerWidget->playPause();
}


void MainWindow::on_buttonLog_clicked()
{
   m_TextEdit.setPlainText( ui->mplayerWidget->log() );
   m_TextEdit.show();
}


void MainWindow::on_positionSlider_sliderPressed()
{
   disconnect(ui->mplayerWidget, SIGNAL(videoPosChanged(int)), ui->positionSlider, SLOT(setValue(int)));
   connect(ui->positionSlider, SIGNAL(valueChanged(int)), ui->mplayerWidget, SLOT(seekTo(int)));
}


void MainWindow::on_positionSlider_sliderReleased()
{
   disconnect(ui->positionSlider, SIGNAL(valueChanged(int)), ui->mplayerWidget, SLOT(seekTo(int)));
   connect(ui->mplayerWidget, SIGNAL(videoPosChanged(int)), ui->positionSlider, SLOT(setValue(int)));
}


void MainWindow::on_mplayerWidget_lengthChanged(int deciSeconds)
{
   ui->positionSlider->setMaximum(deciSeconds/10);
}


void MainWindow::on_mplayerWidget_lengthChanged(QTime time)
{
   ui->LengthTimeEdit->setTime(time);
}
