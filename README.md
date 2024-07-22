# Editor

**Editor** is a basic text editor that is written in C++ with Qt5. It serves as an example to demonstrate how to implement the following;

- A splash screen for applications with long startup routine.
- An application main window (QMainWindow).
- Menu bar (QMenu and QMenuBar).
- Tool bars (QToolBar) and status bar (QStatusBar).
- Actions (QActions).
- QTextEdit for text inputs.
- Some useful inbuilt dialogs, including for file selection (QFileDialog) and messages (QMessageBox).
- How to use embedded image files/resources as icons.
- Signal-slot operations.

And more!

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
