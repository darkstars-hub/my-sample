#include "C:\Users\maila\AppData\Local\Programs\Python\Python312\include\Python.h"


int main() {
    // Pythonの初期化
    Py_Initialize();

    // Pythonコードを実行
    PyRun_SimpleString("print('Hello from Python!')");

    // Pythonの終了
    Py_Finalize();
    return 0;
}
