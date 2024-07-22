/**
 * @file mainwindow.h
 * @brief The application's main window class.
 * @version 1.0
 * @date 22/07/2024
 * @author https://github.com/4g3nt47
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
#include <QLabel>
#include <QLineEdit>
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

    QAction *newAction, *openAction, *saveAction, *saveAsAction, *findAction, *exitAction;
    QAction *lineWrapAction;
    QAction *themeActions[3];
    QAction *aboutAction, *aboutQtAction;
    QWidget *findAndReplaceWidget;
    QLineEdit *findLineEdit, *replaceLineEdit;
    QVBoxLayout *findLayout;
    Editor *editor;

    /**
     * @brief Used by the constructor to setup the application window.
     */
    void setupWindow();

    /**
     * @brief Save application settings to persistent storage.
     */
    void saveSettings();

    /**
     * @brief Load application settings to persistent storage.
     */
    void loadSettings();

  private slots:

    /**
     * @brief Spawns a new instance of the application window.
     */
    void createNewDocument();

    /**
     * @brief Open a new document. Spawns a new window if the current document has unsaved changes.
     * @return true on success.
     */
    bool openFile();

    /**
     * @brief Set the application theme by name.
     * @param themeName The name of the theme, e.g: "OLED"
     * @return true on success
     */
    bool setThemeByName(const QString &themeName);

  public slots:

    /**
     * @brief Display a message in the status bar. Connected to the Editor::showStatusMessage() signal.
     * @param msg The message to display
     * @param delay The duration in milliseconds.
     */
    void showStatusMessage(const QString &msg, int delay = 2000);

    /**
     * @brief Enable/disable line wrapping in the text editor.
     * @param checked true to enable line wrapping.
     */
    void toggleLineWrap(bool checked);

    /**
     * @brief Handles QAction::toggled signal for available theme actions.
     * @param checked true if the theme QAction sending the event has been checked.
     */
    void changeTheme(bool checked);

    /**
     * @brief Show/hide the find-and-replace widget.
     */
    void toggleFind();

    /**
     * @brief Execute a find-and-replace on the current document.
     */
    void findAndReplace();

  protected:

    /**
     * @brief Called when the close() event of the main window is called. Used to ensure user got a chance to save changes before application exit.
     * @param event The close event
     */
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
