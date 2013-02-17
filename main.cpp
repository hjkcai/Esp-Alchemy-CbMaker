#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QTranslator *t = new QTranslator();
    t->load(QApplication::applicationDirPath() + "/CbMaker.qm");
    app.installTranslator(t);

    MainWindow *mw = new MainWindow();
    mw->show();

    return app.exec();
}
