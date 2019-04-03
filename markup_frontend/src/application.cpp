#include <QApplication>
#include "markup_frontend/markupwidget.h"
#include "markup_frontend/application.h"

int run_application(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MarkupWidget markup;
    markup.setGeometry(100,100,1080, 720);
    markup.show();

    return a.exec();
}

