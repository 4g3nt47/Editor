#include "mainwindow.h"

int main(int argc, char *argv[]){

  QApplication a(argc, argv);
  a.setFont(QFont("helvetica", 11));
  MainWindow w;
  w.show();
  return a.exec();
}
