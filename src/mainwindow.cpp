#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

  setupWindow();
  setWindowIcon(QIcon(":/images/icon.png"));
  setMinimumSize(600, 450);

  textEdit = new QTextEdit(this);
  textEdit->setAcceptRichText(true);
  textEdit->setFont(QFont("monospace", 14));
  textEdit->setStyleSheet("background-color: #1f1e2f; color: #F8F853;");
  connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::textChanged);
  setCentralWidget(textEdit);
  setCurrentFile("");
}

MainWindow::~MainWindow(){

}

void MainWindow::setupWindow(){

  // Create the actions.
  newAction = new QAction(tr("&New"), this);
  newAction->setShortcut(QKeySequence::New);
  newAction->setIcon(QIcon(":/images/new.png"));
  newAction->setStatusTip("Create a new file");
  connect(newAction, &QAction::triggered, this, &MainWindow::createNewFile);

  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setIcon(QIcon(":/images/open.png"));
  openAction->setStatusTip("Open an existing file");
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setIcon(QIcon(":/images/save.png"));
  saveAction->setStatusTip("Save changes to file");
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

  saveAsAction = new QAction(tr("Save as"), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setIcon(QIcon(":/images/save.png"));
  saveAsAction->setStatusTip("Save changes to another file");
  connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(tr("Ctrl+X"));
  exitAction->setStatusTip("Close application");
  connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  aboutAction = new QAction(tr("&About Editor"), this);
  aboutAction->setStatusTip("About the Editor");
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

  aboutQtAction = new QAction(tr("About Qt"), this);
  aboutQtAction->setStatusTip("About the Qt library");
  connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

  // Create menus

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));
  aboutMenu->addAction(aboutAction);
  aboutMenu->addAction(aboutQtAction);

  // Create tool bar.

  QToolBar *fileToolBar = addToolBar(tr("&File"));
  fileToolBar->addAction(newAction);
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(saveAction);

  // Create status bar.

  statusBar();
}

void MainWindow::setCurrentFile(const QString &filename){

  this->currentFile = filename;
  setWindowTitle(tr("%1[*] - Editor").arg(getBaseFilename(this->currentFile.isEmpty() ? "Untitled" : this->currentFile)));
}

bool MainWindow::createNewFile(){

  if (!canCloseDocument())
    return false;
  textEdit->setText("");
  setCurrentFile("");
  setWindowModified(false);
  return true;
}

bool MainWindow::openFile(){

  if (!canCloseDocument())
    return false;
  QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text files (*.txt)"));
  if (!filename.isEmpty()){
    statusBar()->showMessage("Opening file...", 2000);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
      QMessageBox::warning(this, "Editor", tr("Error reading file: %1\nReason: %2").arg(getBaseFilename(filename)).arg(file.errorString()));
      return false;
    }
    setCurrentFile(filename);
    QByteArray bytes = file.readAll();
    textEdit->setText(QString::fromStdString(bytes.toStdString()));
    statusBar()->showMessage(tr("File opened: %1").arg(getBaseFilename(filename)));
    setWindowModified(false);
  }
  return false;
}

bool MainWindow::saveFile(){

  QString filename = currentFile;
  if (filename.isEmpty())
    filename = QFileDialog::getSaveFileName(this, tr("Save file"), ".", tr("Text files (*.txt)"));
  if (!filename.isEmpty()){
    statusBar()->showMessage("Saving file...", 2000);
    if (!writeToFile(filename))
      return false;
    setCurrentFile(filename);
    setWindowModified(false);
    statusBar()->showMessage("File saved successfully!", 2000);
    return true;
  }
  return false;
}

bool MainWindow::saveFileAs(){

  QString filename = QFileDialog::getSaveFileName(this, tr("Save file as"), ".", tr("Text files (*.txt)"));
  if (!filename.isEmpty()){
    statusBar()->showMessage("Saving file...", 2000);
    if (!writeToFile(filename))
      return false;
    setCurrentFile(filename);
    setWindowModified(false);
    statusBar()->showMessage("File saved successfully!", 2000);
  }
  return false;
}

bool MainWindow::writeToFile(const QString &filename){

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)){
    QMessageBox::warning(this, "Editor", tr("Error writing to file: %1\nReason: %2").arg(getBaseFilename(filename)).arg(file.errorString()));
    return false;
  }
  file.write(textEdit->toPlainText().toStdString().c_str());
  file.close();
  return true;
}

void MainWindow::textChanged(){
  setWindowModified(true);
}

bool MainWindow::canCloseDocument(){

  if (!isWindowModified())
    return true;
  int choice = QMessageBox::warning(this, tr("Editor"), tr("You have made some unsaved changes.\nWould you like to save?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
  if (choice == QMessageBox::Yes)
    return saveFile();
  else if (choice == QMessageBox::No)
    return true;
  return false;
}

QString MainWindow::getBaseFilename(const QString &filename){
  return QFileInfo(filename).fileName();
}

void MainWindow::about(){
  QMessageBox::about(this, tr("About Editor"), tr("<h2>Editor v1.0</h2>"
                                                  "<p>Editor is a simple text editor written in Qt5"
                                                  "<p>Copyright &copy; 2024 <a href='https://github.com/4g3nt47'>Umar Abdul</a></p>"));
}

void MainWindow::closeEvent(QCloseEvent *event){

  if (canCloseDocument())
    event->accept();
  else
    event->ignore();
}
