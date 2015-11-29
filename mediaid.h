#ifndef MEDIAID_H
#define MEDIAID_H

#include <QObject>
#include <QProcess>
#include <QStringList>

class MediaID : public QObject
{
   Q_OBJECT
public:
   explicit MediaID(QObject *parent = 0);

signals:

public slots:

private:
   QProcess *m_Process;
   QStringList m_Args;
};

#endif // MEDIA_H
