#include "qmplayerwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   QmplayerWidget w;
   w.show();

   return a.exec();
}
