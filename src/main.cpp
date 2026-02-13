#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app_anilex(argc, argv);

    QWidget widget;
    widget.show();

    return app_anilex.exec();
}
