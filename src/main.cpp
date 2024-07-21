#include "mainwindow.h"
#include "editor.h"

int main(int argc, char *argv[]){

  QApplication a(argc, argv);
  a.setFont(QFont("helvetica", 11));

  MainWindow *mainWindow = new MainWindow();
  mainWindow->show();

  int exit_code = a.exec();
  return exit_code;
}
