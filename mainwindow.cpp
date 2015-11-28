#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   ui->widget->loadMedia("/home/yuzer/Videos/av/av.mp4");
   connect(ui->volumeSlider, SIGNAL(valueChanged(int)), ui->widget, SLOT(setVolume(int)));
}

MainWindow::~MainWindow()
{
   delete ui;
}

void MainWindow::on_buttonPlayPause_clicked()
{
    ui->widget->playPause();
}


void MainWindow::on_buttonLog_clicked()
{
   m_TextEdit.setPlainText( ui->widget->log() );
   m_TextEdit.show();
}


//////////////////////////////////////////////////////////// Public Slots ////////////////////////////////////////////////////////////


