#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

  editor = new Editor(this);
  connect(editor, &Editor::updateWindowTitle, this, &MainWindow::setWindowTitle);
  connect(editor, &Editor::documentModified, this, &MainWindow::setWindowModified);
  connect(editor, &Editor::showStatusMessage, this, &MainWindow::showStatusMessage);
  editor->setupEditor();

  setupWindow();
  setWindowIcon(QIcon(":/images/icon.png"));
  setMinimumSize(800, 550);

  QLabel *findLabel = new QLabel(tr("&Find:"), this);
  findLabel->setMinimumWidth(80);
  findLabel->setAlignment(Qt::AlignRight);
  findLineEdit = new QLineEdit(this);
  findLineEdit->setPlaceholderText(tr("Keyword..."));
  findLabel->setBuddy(findLineEdit);
  QLabel *replaceLabel = new QLabel(tr("&Replace:"), this);
  replaceLabel->setMinimumWidth(findLabel->minimumWidth());
  replaceLabel->setAlignment(Qt::AlignRight);
  replaceLineEdit = new QLineEdit(this);
  replaceLineEdit->setPlaceholderText(tr("Replace with..."));
  replaceLabel->setBuddy(replaceLineEdit);

  QHBoxLayout *l1 = new QHBoxLayout();
  l1->addWidget(findLabel);
  l1->addWidget(findLineEdit, 1);
  QHBoxLayout *l2 = new QHBoxLayout();
  l2->addWidget(replaceLabel);
  l2->addWidget(replaceLineEdit, 1);
  QVBoxLayout *l3 = new QVBoxLayout();
  l3->addLayout(l1);
  l3->addLayout(l2);

  findAndReplaceWidget = new QWidget(this);
  findAndReplaceWidget->setLayout(l3);
  findAndReplaceWidget->setFont(QFont("helvetica", 9));
  findAndReplaceWidget->hide();
  QVBoxLayout *l4 = new QVBoxLayout();
  l4->addWidget(editor);
  l4->addWidget(findAndReplaceWidget);
  QWidget *mainWidget = new QWidget(this);
  mainWidget->setLayout(l4);

  connect(findLineEdit, &QLineEdit::returnPressed, this, &MainWindow::findAndReplace);
  connect(replaceLineEdit, &QLineEdit::returnPressed, this, &MainWindow::findAndReplace);

  setCentralWidget(mainWidget);

  // Set this window to auto-delete itself (and all child widgets) when it's closed.
  // Very important for multi-window apps to avoid hogging memory.
  // WARNING: The widget will call `delete this` on itself when closed, so if the MainWindow object is created on stack,
  // and not dynamically allocated, the program will SEGFAULT as it tries to free a stack address.
  setAttribute(Qt::WA_DeleteOnClose);
}

MainWindow::~MainWindow(){
  qDebug() << "Main window closed!";
}

void MainWindow::setupWindow(){

  // Create the actions.
  newAction = new QAction(tr("&New"), this);
  newAction->setShortcut(QKeySequence::New);
  newAction->setIcon(QIcon(":/images/new.png"));
  newAction->setStatusTip(tr("Create a new file"));
  connect(newAction, &QAction::triggered, this, &MainWindow::createNewDocument);

  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setIcon(QIcon(":/images/open.png"));
  openAction->setStatusTip(tr("Open an existing file"));
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setIcon(QIcon(":/images/save.png"));
  saveAction->setStatusTip(tr("Save changes to file"));
  connect(saveAction, &QAction::triggered, editor, &Editor::saveFile);

  saveAsAction = new QAction(tr("Save as"), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setIcon(QIcon(":/images/save.png"));
  saveAsAction->setStatusTip(tr("Save changes to another file"));
  connect(saveAsAction, &QAction::triggered, editor, &Editor::saveFileAs);

  findAction = new QAction(tr("&Find and Replace"), this);
  findAction->setShortcut(QKeySequence::Find);
  findAction->setIcon(QIcon(":/images/search.png"));
  findAction->setStatusTip(tr("Find and replace"));
  connect(findAction, &QAction::triggered, this, &MainWindow::toggleFind);

  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(tr("Ctrl+X"));
  exitAction->setStatusTip(tr("Close application"));
  connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  lineWrapAction = new QAction(tr("Line wrap"), this);
  lineWrapAction->setCheckable(true);
  lineWrapAction->setChecked(true);
  lineWrapAction->setStatusTip(tr("Enable/disable line wrapping"));
  connect(lineWrapAction, &QAction::toggled, this, &MainWindow::toggleLineWrap);

  const char *themeNames[] = {"default", "Purple Shades", "OLED"};
  for (int i = 0; i < 3; i++){
    QAction *action = new QAction(tr(themeNames[i]), this);
    action->setCheckable(true);
    action->setChecked(i == 0);
    action->setData(themeNames[i]);
    action->setStatusTip(tr("Enable %1 theme").arg(themeNames[i]));
    connect(action, &QAction::toggled, this, &MainWindow::changeTheme);
    themeActions[i] = action;
  }

  aboutAction = new QAction(tr("&About Editor"), this);
  aboutAction->setStatusTip(tr("About Editor"));
  connect(aboutAction, &QAction::triggered, editor, &Editor::about);

  aboutQtAction = new QAction(tr("About Qt"), this);
  aboutQtAction->setStatusTip(tr("About the Qt library"));
  connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

  // Create menus

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(findAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  QMenu *settingsMenu = menuBar()->addMenu(tr("&Settings"));
  settingsMenu->addAction(lineWrapAction);
  QMenu *themesMenu = settingsMenu->addMenu("Themes");
  for (int i = 0; i < 3; i++)
    themesMenu->addAction(themeActions[i]);

  menuBar()->addSeparator();
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

  // Load application settings.
  loadSettings();

}

void MainWindow::saveSettings(){

  qDebug() << "Saving application settings...";
  QSettings settings("Umar Abdul", "Editor");
  settings.setValue("line wrap", lineWrapAction->isChecked());
  for (int i = 0; i < 3; i++){
    if (themeActions[i]->isChecked()){
      settings.setValue("theme", themeActions[i]->data().toString());
      break;
    }
  }
}

void MainWindow::loadSettings(){

  qDebug() << "Loading application settings...";
  QSettings settings("Umar Abdul", "Editor");
  lineWrapAction->setChecked(settings.value("line wrap", true).toBool());
  setThemeByName(settings.value("theme", "default").toString());
}

void MainWindow::createNewDocument(){
  MainWindow *mw = new MainWindow();
  mw->show();
}

bool MainWindow::openFile(){

  if (editor->isWindowModified()){ // Unsaved changes. Create a new window.
    MainWindow *mw = new MainWindow();
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

bool MainWindow::setThemeByName(const QString &themeName){

  // Uncheck all other theme actions.
  for (int i = 0; i < 3; i++){
    QAction *action = themeActions[i];
    if (action->data().toString() != themeName){
      action->setChecked(false);
    }else{
      if (!action->isChecked()){ // Selected theme is unchecked. Happens on startup when we load selected theme from saved application settings.
        disconnect(action, &QAction::toggled, this, &MainWindow::changeTheme); // Disconnect the handler temporarily to avoid a double call.
        action->setChecked(true);
        connect(action, &QAction::toggled, this, &MainWindow::changeTheme); // Reconnect :)
      }
    }

  }
  // Load and apply the selected theme.
  QString themeFile = ":/themes/default.qss";
  if (themeName == "OLED")
    themeFile = ":/themes/OLED.qss";
  else if (themeName == "Purple Shades")
    themeFile = ":/themes/PurpleShades.qss";
  QFile file(themeFile);
  file.open(QIODevice::ReadOnly);
  if (!file.isOpen()){
    QMessageBox::warning(this, "Editor", "Error loading theme!\nCheck your application resource file.");
    return false;
  }
  setStyleSheet(QString::fromStdString(file.readAll().toStdString()));
  file.close();
  return true;
}

void MainWindow::showStatusMessage(const QString &msg, int delay){
  statusBar()->showMessage(msg, delay);
}

void MainWindow::toggleLineWrap(bool checked){

  editor->setLineWrapMode(checked ? QTextEdit::WidgetWidth :  QTextEdit::NoWrap);
  showStatusMessage(tr("Word wrapping %1!").arg(lineWrapAction->isChecked() ? "enabled" : "disabled"));
}

void MainWindow::changeTheme(bool checked){

  if (!checked) // Theme was unchecked. Simply ignore (better way is to go with radio buttons).
    return;
  QAction *senderAction = qobject_cast<QAction *>(sender()); // Grab the event sender, which will be the QAction of the theme clicked.
  if (senderAction){ // Slots can be used as normal functions, in which case they won't have an event sender.
    QString themeName = senderAction->data().toString();
    if (setThemeByName(themeName))
      showStatusMessage("Theme loaded successfully!");
    else
      showStatusMessage("Error loading theme!");
  }
}

void MainWindow::toggleFind(){

  findAndReplaceWidget->setHidden(!findAndReplaceWidget->isHidden());
  if (!findAndReplaceWidget->isHidden())
    findLineEdit->setFocus();
}

void MainWindow::findAndReplace(){
  editor->findAndReplace(findLineEdit->text(), replaceLineEdit->text());
}

void MainWindow::closeEvent(QCloseEvent *event){

  if (editor->canCloseDocument()){
    saveSettings();
    qInfo() << "Closing main window...";
    event->accept();
  }else{
    event->ignore();
  }
}
