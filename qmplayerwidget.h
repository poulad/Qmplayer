#ifndef QMPLAYERWIDGET_H
#define QMPLAYERWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QString>
#include <QTime>

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
   void pause();
   void seekTo(int deciSeconds);
   void seekTo(const QTime&);
   void setVolume(int);

signals:
   void videoPosChanged(int deciSeconds);
   void videoPosChanged(QTime);
   void audioPosChanged(int deciSeconds);
   void audioPosChanged(QTime);
   void volumeChanged(int percent);

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

   QRegExp m_PositionRegex;
   QRegExp m_VolumeRegex;

   bool m_IsPlaying;
   int m_VideoPos;
   int m_AudioPos;
   int m_Volume;
};

#endif // QMPLAYERWIDGET_H
