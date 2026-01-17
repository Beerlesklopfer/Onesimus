#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Bacula Qt UI");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Bacula");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
