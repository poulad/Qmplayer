#ifndef QMPLAYERWIDGET_H
#define QMPLAYERWIDGET_H

#include <QWidget>
#include <QProcess>

class QmplayerWidget : public QWidget, public QProcess
{
   Q_OBJECT

public:
   QmplayerWidget(QWidget *parent = 0);
   ~QmplayerWidget();
};

#endif // QMPLAYERWIDGET_H
