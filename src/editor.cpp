#include "editor.h"
#include <QDebug>

QStringList Editor::openedFiles;

Editor::Editor(QWidget *parent) : QTextEdit(parent){

}

Editor::~Editor(){

  documentClosed();
  qInfo() << "Editor closed!";
}

void Editor::setupEditor(){

  setAcceptRichText(true);
  setAcceptRichText(true);
  setFont(QFont("monospace", 14));
  setCurrentFile("");
  connect(this, &QTextEdit::textChanged, this, &Editor::textChanged);
}

void Editor::setCurrentFile(const QString &filename){

  if (!currentFile.isEmpty())
    documentClosed(); // Reusing a window. Remove the current filename from the list of opened files.
  currentFile = filename;
  if (!currentFile.isEmpty()){
    openedFiles.append(currentFile);
    emit updateWindowTitle(tr("%1[*] - Editor").arg(getBaseFilename(currentFile)));
  }else{
    emit updateWindowTitle(tr("Untitled[*] - Editor"));
  }
}

void Editor::setDocumentModified(bool modified){

  setWindowModified(modified);
  emit documentModified(modified);
}

bool Editor::canCloseDocument(){

  if (!isWindowModified())
    return true;
  int choice = QMessageBox::warning(this, tr("Editor"), tr("You have made some unsaved changes.\nWould you like to save?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
  if (choice == QMessageBox::Yes)
    return saveFile();
  else if (choice == QMessageBox::No)
    return true;
  return false;
}

QString Editor::getBaseFilename(const QString &filename){
  return QFileInfo(filename).fileName();
}

QString Editor::getCurrentFile(){
  return currentFile;
}

void Editor::textChanged(){
  setDocumentModified(true);
}

bool Editor::openFile(){

  if (!canCloseDocument())
    return false;
  QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text files (*.txt)\n"
                                                                                 "All files (*.*)"));
  if (!filename.isEmpty()){
    if (openedFiles.contains(filename)){
      QMessageBox::warning(this, tr("Editor"), tr("File is already open!"));
      return false;
    }
    emit showStatusMessage("Opening file...");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
      QMessageBox::warning(this, tr("Editor"), tr("Error reading file: %1\nReason: %2").arg(getBaseFilename(filename)).arg(file.errorString()));
      return false;
    }
    setCurrentFile(filename);
    QByteArray bytes = file.readAll();
    setText(QString::fromStdString(bytes.toStdString()));
    emit showStatusMessage(tr("File opened: %1").arg(getBaseFilename(filename)));
    setDocumentModified(false);
    return true;
  }
  return false;
}

bool Editor::saveFile(){

  QString filename = currentFile;
  if (filename.isEmpty())
    filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".", tr("Text files (*.txt)"));
  if (!filename.isEmpty()){
    emit showStatusMessage("Saving file...");
    if (!writeToFile(filename))
      return false;
    setCurrentFile(filename);
    setDocumentModified(false);
    emit showStatusMessage("File saved successfully!");
    return true;
  }
  return false;
}

bool Editor::saveFileAs(){

  QString filename = QFileDialog::getSaveFileName(this, tr("Save file as"), ".", tr("Text files (*.txt)"));
  if (!filename.isEmpty()){
    emit showStatusMessage("Saving file...");
    if (!writeToFile(filename))
      return false;
    setCurrentFile(filename);
    setDocumentModified(false);
    emit showStatusMessage("File saved successfully!");
    return true;
  }
  return false;
}

bool Editor::writeToFile(const QString &filename){

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)){
    QMessageBox::warning(this, "Editor", tr("Error writing to file: %1\nReason: %2").arg(getBaseFilename(filename)).arg(file.errorString()));
    return false;
  }
  file.write(toPlainText().toStdString().c_str());
  file.close();
  return true;
}

void Editor::documentClosed(){
  openedFiles.removeAll(currentFile);
}

void Editor::about(){
  QMessageBox::about(this, tr("About Editor"), tr("<h2>Editor v1.0</h2>"
                                                  "<p>Editor is a simple text editor written in Qt5"
                                                  "<p>Copyright &copy; 2024 <a href='https://github.com/4g3nt47'>Umar Abdul</a></p>"));
}
