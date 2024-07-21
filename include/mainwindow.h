#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QFont>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QKeySequence>
#include <QCloseEvent>
#include <QSplashScreen>
#include <QThread>
#include "editor.h"

/**
 * @brief The MainWindow class.
 */
class MainWindow : public QMainWindow{

  Q_OBJECT

  public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:

    QAction *newAction, *openAction, *saveAction, *saveAsAction, *exitAction;
    QAction *lineWrapAction;
    QAction *themeActions[3];
    QAction *aboutAction, *aboutQtAction;
    Editor *editor;

    /**
     * @brief Setup the main window and load a fresh document.
     */
    void setupWindow();

    void saveSettings();

    void loadSettings();

  private slots:

    void createNewDocument();
    bool openFile();
    bool setThemeByName(const QString &themeName);

  public slots:

    void showStatusMessage(const QString &msg, int delay = 2000);
    void toggleLineWrap(bool checked);
    void changeTheme(bool checked);

  protected:

    /**
     * @brief Called when the close() event of the main window is called. Used to ensure user got a chance to save changes before application exit.
     * @param event The close event.
     */
    void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
