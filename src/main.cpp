#include <iostream>
#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    /* code */
    std::cout << "henlo\n";

    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}
