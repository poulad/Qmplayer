#include "qmplayerwidget.h"

QmplayerWidget::QmplayerWidget(QWidget *widget)
   :  QWidget(widget)
{
   setAcceptDrops(true);
   m_mplayerAddress = "mplayer";

   m_PositionRegex.setPattern("A:\\s*(\\d+).(\\d+)\\s*V:\\s*(\\d+).(\\d+)\\s*");
   m_VolumeRegex.setPattern("ANS_volume=(\\d+).(\\d+)");

   m_Process = new QProcess(this);
   connect(m_Process, SIGNAL(readyReadStandardError()), this, SLOT(readStdErr()));
   connect(m_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOut()));

   m_MediaID = new MediaID(m_mplayerAddress, this);
   connect(m_MediaID, SIGNAL(mediaIdentified()), this, SLOT(readMediaId()));

   m_Process->setProgram(m_mplayerAddress);
   m_ArgumentsList
         << "-slave"
         << "-idle"
         << "-identify"
         << "-wid" << QString::number(winId())
         << "-noconfig" << "all"
         << "-nosub"
         << "-ass"
         << "-volume" << "100"
            ;
   m_Process->setArguments(m_ArgumentsList);
   m_Process->start(QIODevice::ReadWrite);
}


QmplayerWidget::~QmplayerWidget()
{
   if(m_Process->isOpen() == true)
      m_Process->close();
   delete m_Process;
}


int QmplayerWidget::volume() const
{
   return m_Volume;
}


const QString& QmplayerWidget::log() const
{
   return m_Log;
}



//////////////////////////////////////////////////////////// Public Slots ////////////////////////////////////////////////////////////



void QmplayerWidget::loadMedia(const QString &media)
{
   setEnabled(false);
   m_MediaID->setMedia(media);
}


void QmplayerWidget::loadSubtitle(const QString &sub)
{
   m_Command = "pausing_keep sub_remove 0";
   sendCommand();
   m_Command = "pausing_keep sub_load \"" + sub + "\"";
   sendCommand();
   m_Command = "pausing_keep sub_select 0";
   sendCommand();
}


void QmplayerWidget::playPause()
{
   if(m_Process->isWritable() == false)
      return;

   m_Command = "pause";
   sendCommand();
   m_IsPlaying = !m_IsPlaying;
}


void QmplayerWidget::pause()
{
   if(m_Process->isWritable() == false || m_IsPlaying == false)
      return;
   playPause();
}


void QmplayerWidget::seekTo(int deciSeconds)
{
   m_Command = "pausing_keep seek " + QString::number(deciSeconds/10) + "." + QString::number(deciSeconds%10) + " 2";
   sendCommand();
}


void QmplayerWidget::seekTo(const QTime &time)
{
   if(m_Process->isWritable() == false)
      return;
   seekTo( time.msecsSinceStartOfDay() / 100 );
}


void QmplayerWidget::setVolume(int v)
{
   if(m_Process->isWritable() == false)
      return;
   m_Command = "volume " + QString::number(v) + " 1";
   sendCommand();
}



///////////////////////////////////////////////////////// Protected Functions //////////////////////////////////////////////////////////



void QmplayerWidget::dragEnterEvent(QDragEnterEvent *event)
{
   if(event->mimeData()->hasFormat("text/uri-list"))
      event->acceptProposedAction();
}


void QmplayerWidget::dropEvent(QDropEvent *event)
{
   QList<QUrl> urlList = event->mimeData()->urls();
   QFile file( urlList.first().toLocalFile() );
   QFileInfo fileInfo(file);
   if( fileInfo.suffix().contains("srt", Qt::CaseInsensitive) || fileInfo.suffix().contains("ass", Qt::CaseInsensitive) || fileInfo.suffix().contains("ssa", Qt::CaseInsensitive) )
      loadSubtitle(file.fileName());
   else
      loadMedia(file.fileName());
}



///////////////////////////////////////////////////////////// Private Slots /////////////////////////////////////////////////////////////



void QmplayerWidget::readStdErr()
{
   m_StdErr = m_Process->readAllStandardError();
   m_Log += m_StdErr;
   if(m_StdErr.contains(m_PositionRegex))
   {
      m_AudioPos = m_PositionRegex.cap(1).toInt() * 10 + m_PositionRegex.cap(2).toInt();
      m_VideoPos = m_PositionRegex.cap(3).toInt() * 10 + m_PositionRegex.cap(4).toInt();
      emit audioPosChanged(m_AudioPos);
      emit videoPosChanged(m_VideoPos);
      QTime time(0,0,0);
      emit audioPosChanged(time.addMSecs(m_AudioPos * 100));
      emit videoPosChanged(time.addMSecs(m_VideoPos * 100));
   }
}


void QmplayerWidget::readStdOut()
{
   m_StdOut = m_Process->readAllStandardOutput();
   m_Log += m_StdOut;
   if(m_StdOut.contains(m_VolumeRegex))
   {
      m_Volume = m_VolumeRegex.cap(1).toInt();
      emit volumeChanged(m_Volume);
   }
}


void QmplayerWidget::readMediaId()
{
   if( m_MediaID->hasVideo() || m_MediaID->hasAudtio() )
   {
      emit lengthChanged(m_MediaID->length());
      emit lengthChanged(QTime::fromMSecsSinceStartOfDay(m_MediaID->length() * 10));
      m_Command = "pausing_keep loadfile \"" + m_MediaID->fileName() + "\" 0";
      sendCommand();
   }

   setEnabled(true);

}




//////////////////////////////////////////////////////////// Private Functions ////////////////////////////////////////////////////////////




void QmplayerWidget::sendCommand()
{
   m_Command += "\n";
   m_Process->write(m_Command.toLatin1());
}

