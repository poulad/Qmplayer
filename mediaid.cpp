#include "mediaid.h"

MediaID::MediaID(QString mplayer, QObject *parent)
   :  QObject(parent)
{
   m_mplayerAddress = mplayer;
   //initialize member variables
   m_Length = 0;
   m_AudioBitrate = 0;
   m_AudioRate = 0;
   m_AudioChannels = 0;
   m_Width = 0;
   m_Height = 0;
   m_fps = 0;
   m_VideoBitrate = 0;
   m_AspectRatio = 0;

   m_Process = new QProcess(this);
   m_Process->setProgram(m_mplayerAddress);
   connect(m_Process, SIGNAL(finished(int)), this, SLOT(identifyMedia(int)));
}


void MediaID::setMedia(const QString &media)
{
   m_Address = media;
   QString command;
   command = m_mplayerAddress + " -identify -endpos 0 " + "\"" + m_Address + "\"";
   m_Process->start(command);
}


void MediaID::identifyMedia(int exitCode)
{
   m_StdOutErr = m_Process->readAll();
   if(exitCode != 0)
   {
      emit failed(false);
      return;
   }

   QStringList lines = m_StdOutErr.split("\n");
   QRegExp idRegex("^ID_([A-Z|0-9|_]+)=([A-Z|0-9|_|:|-|.|\\s|\\w]+)$", Qt::CaseInsensitive);
   if(lines.contains("ID_AUDIO_ID=\\d+", Qt::CaseInsensitive))
   {
      m_HasAudio = true;
   }
   if(lines.contains("ID_VIDEO_ID=\\d+", Qt::CaseInsensitive))
   {
      m_HasVideo = true;
   }
   if(lines.contains("ID_FILE_SUB_ID=\\d+", Qt::CaseInsensitive))
   {
      m_HasSubtitle = true;
   }
   foreach(QString str, lines)
   {
      str.contains(idRegex);

      if(idRegex.cap(1) == "LENGTH")
      {
         QString length = idRegex.cap(2).remove(".");
         m_Length = length.toInt();
      }
      else if(idRegex.cap(1) == "AUDIO_FORMAT")
         m_AudioFormat = idRegex.cap(2);
      else if(idRegex.cap(1) == "AUDIO_CODEC")
         m_AudioCodec = idRegex.cap(2);
      else if(idRegex.cap(1) == "AUDIO_BITRATE")
         m_AudioBitrate = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "AUDIO_RATE")
         m_AudioRate = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "AUDIO_NCH")
         m_AudioChannels = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "VIDEO_FORMAT")
         m_VideoFormat = idRegex.cap(2);
      else if(idRegex.cap(1) == "VIDEO_CODEC")
         m_VideoCodec = idRegex.cap(2);
      else if(idRegex.cap(1) == "VIDEO_WIDTH")
         m_Width = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "VIDEO_HEIGHT")
         m_Height = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "VIDEO_FPS")
         m_fps = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "VIDEO_BITRATE")
         m_VideoBitrate = idRegex.cap(2).toInt();
      else if(idRegex.cap(1) == "VIDEO_ASPECT")
         m_AspectRatio = idRegex.cap(2).toDouble();
   }
   //qDebug() << "Audio:" << m_AudioFormat << m_AudioCodec << QString::number(m_AudioBitrate) << QString::number(m_AudioRate) << QString::number(m_AudioChannels);
   //qDebug() << "Video:" << m_VideoFormat << m_VideoCodec << QString::number(m_Width) << QString::number(m_Height) << QString::number(m_fps) << QString::number(m_VideoBitrate) << QString::number(m_AspectRatio);
   emit mediaIdentified();
}


