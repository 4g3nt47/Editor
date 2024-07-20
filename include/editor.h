#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>


class Editor : public QTextEdit{

  Q_OBJECT

  private:

    static QStringList *openedFiles;
    QString currentFile;
    void setCurrentFile(const QString &filename);

  public:

    Editor(QWidget *parent = nullptr);
    ~Editor();

    void setDocumentModified(bool modified);

    bool canCloseDocument();

    QString getBaseFilename(const QString &filename);

    QString getCurrentFile();

  public slots:

    void textChanged();

    bool openFile();

    bool saveFile();

    bool saveFileAs();

    bool writeToFile(const QString &filename);

    void documentClosed();

    void about();

  signals:

    void documentModified(bool modified);
    void showStatusMessage(const QString &msg, int delay = 2000);


};

#endif // EDITOR_H
