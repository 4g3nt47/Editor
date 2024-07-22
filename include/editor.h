/**
 * @file editor.h
 * @brief Header file for the Editor class.
 * @version 1.0
 * @date 22/07/2024
 * @author https://github.com/4g3nt47
 */

#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>

/**
 * @brief The Editor class. Extends the QTextEdit class and implements the editor's application logic.
 */
class Editor : public QTextEdit{

  Q_OBJECT

  private:

    static QStringList openedFiles; // List of opened files to avoid editing same file in different windows.
    QString currentFile; // The full name of the current file being edited.

    /**
     * @brief Set the current file being edited. Adds it to the openedFiles list and emits updateWindowTitle()
     * @param filename The full name of the file being edited
     */
    void setCurrentFile(const QString &filename);

  public:

    /**
     * @brief Creates an new text editor that's not assigned to any file.
     * @param parent The parent widget
     */
    Editor(QWidget *parent = nullptr);

    /**
     * @brief The class destructor. Ensures documentClosed() is called after the editor has been deleted.
     */
    ~Editor();

    /**
     * @brief Configures the QTextEdit widget used by the editor, and any internal signal-slot connection used.
     */
    void setupEditor();

    /**
     * @brief Marks the current document as modified or not. Emits the documentModified(bool) signal.
     * @param modified true if the current document has just been modified
     */
    void setDocumentModified(bool modified);

    /**
     * @brief Called when user performs any action that will lead to the current document being edited to be discarded.
     * This function ensures user does not accidentally discard any unsaved changes to the application by prompting the
     * user to save the changes.
     * @return true if the current document can be safely discarded
     */
    bool canCloseDocument();

    /**
     * @brief Get the basename of a given filename. E.g: "/home/user/file.txt" => "file.txt"
     * @param filename Filename to get the basename of.
     * @return The basename of the given filename
     */
    QString getBaseFilename(const QString &filename);

    /**
     * @brief Get the name of the current file being edited.
     * @return The name of the current file being edited
     */
    QString getCurrentFile();

  public slots:

    /**
     * @brief Called whenever the editor's text is changed. Calls setDocumentModified(true) to update state.
     */
    void textChanged();

    /**
     * @brief Load a file into the editor from disk.
     * @return true on success
     */
    bool openFile();

    /**
     * @brief Save the current document being edited. If no filename is associated with the editor, user will be promted to to provide one.
     * @return true on success
     */
    bool saveFile();

    /**
     * @brief Save the current document being edited under a different name.
     * @return
     */
    bool saveFileAs();

    /**
     * @brief Write the document being edited to disk.
     * @param filename The filename to write to
     * @return true on success
     */
    bool writeToFile(const QString &filename);

    /**
     * @brief Find and replace some text in the document.
     * @param findStr The string to find
     * @param replaceStr The string to replace it with
     */
    void findAndReplace(const QString &findStr, const QString &replaceStr);

    /**
     * @brief Called when the current document have been closed.
     */
    void documentClosed();

    /**
     * @brief Displays the program's "about" dialog.
     */
    void about();

  signals:

    /**
     * @brief Signals the new window title to use. "<Filename> - Editor"
     * @param title The new window title to use.
     */
    void updateWindowTitle(const QString &title);

    /**
     * @brief Signals whether the document has unsaved changes, or not.
     * @param modified true if document has unsaved changes.
     */
    void documentModified(bool modified);

    /**
     * @brief Signals the new message that should be displayed in the status bar.
     * @param msg The message to display
     * @param delay The message duration, in milliseconds.
     */
    void showStatusMessage(const QString &msg, int delay = 2000);

};

#endif // EDITOR_H
