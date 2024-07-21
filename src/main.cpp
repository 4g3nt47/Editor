#include "mainwindow.h"
#include "editor.h"

int main(int argc, char *argv[]){

  QApplication app(argc, argv);
  app.setFont(QFont("helvetica", 11));

  QSplashScreen *splash = new QSplashScreen();
  splash->setPixmap(QPixmap(":/images/splash.png"));
  splash->setFixedSize(800, 550);
  QFont splashFont("helvetica", 13);
  splashFont.setItalic(true);
  splash->setFont(splashFont);
  splash->show();
  // Show some dynamic messages in the flash screen.
  Qt::Alignment align = Qt::AlignTop | Qt::AlignRight;
  splash->showMessage(QObject::tr("Loading resources..."), align, Qt::white);
  QThread::msleep(1000); // Simulate a load delay so user can see the splash screen
  splash->showMessage("Starting the editor...", align, Qt::white);
  QThread::msleep(1000);

  MainWindow *mainWindow = new MainWindow();
  mainWindow->show();

  splash->finish(mainWindow);
  delete splash;
  return app.exec();
}
