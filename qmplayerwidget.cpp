#include "qmplayerwidget.h"

QmplayerWidget::QmplayerWidget(QWidget *widget)
   :  QWidget(widget)
{
   setAcceptDrops(true);

   m_VolumeRegex.setPattern("ANS_volume=(\\d+).(\\d+)");

   m_Process = new QProcess(this);
   connect(m_Process, SIGNAL(readyReadStandardError()), this, SLOT(readStdErr()));
   connect(m_Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOut()));

   m_Process->setProgram("mplayer");
   m_ArgumentsList
         << "-slave"
         << "-idle"
         << "-wid" << QString::number(winId())
         << "-noconfig" << "all"
         << "-ass"
            ;
   m_Process->setArguments(m_ArgumentsList);
   m_Process->start(QIODevice::ReadWrite);
   if (m_Process->waitForStarted(5000) == true)
      setVolume(100);
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


void QmplayerWidget::loadMedia(const QString & media)
{
   m_Command = "loadfile \"" + media + "\" 0";
   sendCommand();
}


void QmplayerWidget::playPause()
{
   if(m_Process->isWritable() == false)
      return;

   m_Command = "pause";
   sendCommand();
}


void QmplayerWidget::setVolume(int v)
{
   if(m_Process->isWritable() == false)
      return;
   m_Command = "volume " + QString::number(v) + " 1";
   sendCommand();
}


//////////////////////////////////////////////////////////// Public Slots ////////////////////////////////////////////////////////////


void QmplayerWidget::sendCommand()
{
   m_Command += "\n";
   m_Process->write(m_Command.toLatin1());
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
   if(file.exists() == true)
   {
      loadMedia(file.fileName());
   }
}



//////////////////////////////////////////////////////////// Private Slots ////////////////////////////////////////////////////////////


void QmplayerWidget::readStdErr()
{
   m_Log += m_Process->readAllStandardError();
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
