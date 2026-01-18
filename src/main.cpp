#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WINDOWS
    // Force OpenSSL backend statt Schannel
    qputenv("QT_SSL_USE_OPENSSL", "1");
    QSslSocket::setActiveBackend("openssl");
    qDebug() << "SSL Support:" << QSslSocket::supportsSsl();
    qDebug() << "SSL Library Build Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "SSL Library Version:" << QSslSocket::sslLibraryVersionString();
    qDebug() << "Available Backends:" << QSslSocket::availableBackends();
    qDebug() << "Active Backend:" << QSslSocket::activeBackend();
    qDebug() << "Using TLS backend:" << QSslSocket::activeBackend();
    qDebug() << "SSL Library:" << QSslSocket::sslLibraryVersionString();
#endif

    QCoreApplication::addLibraryPath(
        QCoreApplication::applicationDirPath() + "/plugins");

    QApplication app(argc, argv);

    app.setApplicationName("Onesimus");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Bacula");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
