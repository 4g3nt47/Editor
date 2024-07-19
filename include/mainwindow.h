/**
 * @file mainwindow.h
 * @brief A basic text editor written in Qt to demonstrate working with;
 * 1. An application main window (QMainWindow)
 * 2. Menu bar (QMenu and QMenuBar)
 * 3. Tool bars (QToolBar) and status bar (QStatusBar)
 * 4. Actions (QActions)
 * 5. Some useful inbuilt dialogs, including for file selection (QFileDialog) and messages (QMessageBox)
 * @author https://github.com/4g3nt47
 * @date 19/Jul/2024
 */

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
#include <QMessageBox>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>

/**
 * @brief The MainWindow class. Since this is a small application, both the application logic and display are implemented in this class.
 */
class MainWindow : public QMainWindow{

  Q_OBJECT

  public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:

    QAction *newAction, *openAction, *saveAction, *saveAsAction, *exitAction;
    QAction *aboutAction, *aboutQtAction;

    QTextEdit *textEdit;
    QString currentFile;

    /**
     * @brief Setup the main window and load a fresh document.
     */
    void setupWindow();

  private slots:

    /**
     * @brief Set the name of the current document being edited.
     * @param filename The document filename.
     */
    void setCurrentFile(const QString &filename);

    /**
     * @brief Create a new document for editing.
     * @return true on success.
     */
    bool createNewFile();

    /**
     * @brief Open a file from disk for editing.
     * @return true on success.
     */
    bool openFile();

    /**
     * @brief Save the current document.
     * @return
     */
    bool saveFile();

    /**
     * @brief Save the current document with another name.
     * @return
     */
    bool saveFileAs();

    /**
     * @brief Write the document buffer to local file.
     * @param filename The file to write to.
     * @return true on success.
     */
    bool writeToFile(const QString &filename);

    /**
     * @brief Called whenever text document is modifed.
     */
    void textChanged();

    /**
     * @brief Check if the current document can be discarded. Will prompt user to save changes, if need be.
     * @return true if the document can be discarded.
     */
    bool canCloseDocument();

    /**
     * @brief Obtain the base name of a file path/name.
     * @param filename The filename
     * @return The basename of the file. E.g: '/home/file.txt' => 'file.txt'
     */
    QString getBaseFilename(const QString &filename);

    /**
     * @brief Display the program's about page
     */
    void about();

  protected:

    /**
     * @brief Called when the close() event of the main window is called. Used to ensure user got a chance to save changes before application exit.
     * @param event The close event.
     */
    void closeEvent(QCloseEvent *event);

};
#endif // MAINWINDOW_H
