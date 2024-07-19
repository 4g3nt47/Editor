# Editor

A basic text editor written in Qt to demonstrate working with;
1. An application main window (QMainWindow)
2. Menu bar (QMenu and QMenuBar)
3. Tool bars (QToolBar) and status bar (QStatusBar)
4. Actions (QActions)
5. Some useful inbuilt dialogs, including for file selection (QFileDialog) and messages (QMessageBox)
6. How to use embedded image files/resources as icons.

## Build

This application was built with Qt `5.15.2`.

1. Clone the repo;

```sh
git clone https://github.com/4g3nt47/Editor.git
```

2. Compile;

```sh
cd Editor
mkdir build
cd build
qmake ..
make
```

3. Run;

```sh
./Editor
```
