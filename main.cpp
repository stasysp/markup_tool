#include <iostream>

#include "markup_backend/markup.h"
#include "markup_frontend/application.h"

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MarkupWidget markup;
    markup.setGeometry(100,100,1080, 720);
    markup.show();

    return a.exec();*/
    run_application(argc, argv);

    return 0;
}


/*int main() {
    // run_pipeline();

    std::cout << "Hello World!" << std::endl;

    return 0;
}*/

