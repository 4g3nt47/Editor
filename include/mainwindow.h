#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QFont>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QKeySequence>
#include <QCloseEvent>
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
    QAction *aboutAction, *aboutQtAction;
    Editor *editor;

    /**
     * @brief Setup the main window and load a fresh document.
     */
    void setupWindow();

  private slots:

    void createNewDocument();
    bool openFile();

  public slots:

    void showStatusMessage(const QString &msg, int delay = 2000);

  protected:

    /**
     * @brief Called when the close() event of the main window is called. Used to ensure user got a chance to save changes before application exit.
     * @param event The close event.
     */
    void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
