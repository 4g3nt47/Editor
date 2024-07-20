#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

  editor = new Editor(this);
  connect(editor, &Editor::documentModified, this, &MainWindow::setWindowModified);
  connect(editor, &Editor::showStatusMessage, this, &MainWindow::showStatusMessage);

  setupWindow();
  setWindowIcon(QIcon(":/images/icon.png"));
  setWindowTitle(tr("Untitled[*] - Editor"));
  setMinimumSize(800, 550);
  setCentralWidget(editor);
}

MainWindow::~MainWindow(){

}

void MainWindow::setupWindow(){

  // Create the actions.
  newAction = new QAction(tr("&New"), this);
  newAction->setShortcut(QKeySequence::New);
  newAction->setIcon(QIcon(":/images/new.png"));
  newAction->setStatusTip("Create a new file");
  connect(newAction, &QAction::triggered, this, &MainWindow::createNewDocument);

  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setIcon(QIcon(":/images/open.png"));
  openAction->setStatusTip("Open an existing file");
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setIcon(QIcon(":/images/save.png"));
  saveAction->setStatusTip("Save changes to file");
  connect(saveAction, &QAction::triggered, editor, &Editor::saveFile);

  saveAsAction = new QAction(tr("Save as"), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setIcon(QIcon(":/images/save.png"));
  saveAsAction->setStatusTip("Save changes to another file");
  connect(saveAsAction, &QAction::triggered, editor, &Editor::saveFileAs);

  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(tr("Ctrl+X"));
  exitAction->setStatusTip("Close application");
  connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  aboutAction = new QAction(tr("&About Editor"), this);
  aboutAction->setStatusTip("About the Editor");
  connect(aboutAction, &QAction::triggered, editor, &Editor::about);

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

void MainWindow::createNewDocument(){
  MainWindow *mw = new MainWindow(this);
  mw->show();
}

bool MainWindow::openFile(){

  if (editor->isWindowModified()){ // Unsaved changes. Create a new window.
    MainWindow *mw = new MainWindow(this);
    if (mw->openFile()){
      mw->show(); // Successful load. Show the window.
      return true;
    }
    // Failed to load file. Delete the new window.
    delete mw;
    return false;
  }
  // Working with unchanged document. Reuse the current window.
  return editor->openFile();
}

void MainWindow::showStatusMessage(const QString &msg, int delay){
  statusBar()->showMessage(msg, delay);
}

void MainWindow::closeEvent(QCloseEvent *event){

  if (editor->canCloseDocument()){
    editor->documentClosed();
    event->accept();
  }else{
    event->ignore();
  }
}
