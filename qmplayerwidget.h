#ifndef QMPLAYERWIDGET_H
#define QMPLAYERWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QString>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include <QDebug>

class QmplayerWidget : public QWidget
{
   Q_OBJECT

public:
   QmplayerWidget(QWidget *parent);
   ~QmplayerWidget();

   int volume() const;
   const QString& log() const;

public slots:
   void loadMedia(const QString&);
   void playPause();
   void setVolume(int);

signals:
   void volumeChanged(int);

protected:
   void dragEnterEvent(QDragEnterEvent *event);
   void dropEvent(QDropEvent *event);

private slots:
   void readStdErr();
   void readStdOut();


private:
   inline void sendCommand();



   QProcess *m_Process;
   QStringList m_ArgumentsList;
   QString m_StdOut;
   QString m_StdErr;
   QString m_Log;

   QString m_mplayerAddress;
   QString m_Command;

   QRegExp m_VolumeRegex;


   int m_Volume;
};

#endif // QMPLAYERWIDGET_H
