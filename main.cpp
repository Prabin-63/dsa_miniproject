#include <QApplication>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set a clean system font
    QFont font("Segoe UI", 10);
    font.setStyleHint(QFont::SansSerif);
    app.setFont(font);

    app.setApplicationName("Election System");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("DSA Mini Project");

    MainWindow w;
    w.show();
    return app.exec();
}
