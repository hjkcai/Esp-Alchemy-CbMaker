#include "mainwindow.h"

const QString black = "<font style=\"color: black\">%0</font>";
const QString blue = "<font style=\"color: blue\">%0</font>";
const QString green = "<font style=\"color: #008000\">%0</font>";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    txterr = new QTextEdit(this);
    txterr->setFont(QFont("Courier New"));
    txterr->setHtml(blue.arg(tr("Translation missing (txterr_intro)")));
    txterr->setReadOnly(true);

    txt = new QTextEdit(this);
    txt->setFont(QFont("Courier New"));
    h = new highlighter(txt->document());

    QMenu *file = this->menuBar()->addMenu(tr("&File"));

    connect(file->addAction(tr("Check for &Errors")), SIGNAL(triggered()), this, SLOT(checkError()));
    connect(file->addAction(tr("E&xit...")), SIGNAL(triggered()), this, SLOT(close()));

    fileName = QString(
            #ifndef Q_OS_MAC
                                  "%0/res/combinations"
            #else
                                  "%0/../../../Alchemy.app/Contents/Resources/combinations"
            #endif
                                ).arg(QApplication::applicationDirPath());

    QFile *f = new QFile(fileName);
    if (f->open(QFile::ReadOnly | QFile::Text))
    {
        txt->setText(f->readAll());
        f->close();
    }
    delete f;

    this->resize(650, 400);
    this->setWindowTitle(tr("CbMaker"));
}

void MainWindow::closeEvent(QCloseEvent *)
{
    save();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    txt->setGeometry(0, 0, this->width(), this->height() - 201);
    txterr->setGeometry(0, this->height() - 200, this->width(), 200);
}

void MainWindow::checkError()
{
    h->check(txt->toPlainText());
    txterr->setHtml(blue.arg(tr("Translation missing (txterr)")));

    for (int i = 0; i < h->items.count(); i++)
    {
        txterr->append(black.arg(QString("%0 %1: %2").arg(tr("line")).arg(h->items[i].line + 1).arg(h->items[i].describe)));
    }

    if (h->items.count() == 0)
    {
        txterr->append(green.arg(tr("Everything is correct!")));
        save();
    }
}

void MainWindow::save()
{
    QFile *saves = new QFile(fileName);

#ifndef Q_OS_MAC
    QDir d(QApplication::applicationDirPath());
    if (!d.exists("res")) d.mkdir("res");
#endif

    saves->open(QFile::WriteOnly);
    saves->write(txt->toPlainText().toAscii());
    saves->waitForBytesWritten(3000);
    saves->flush();
    saves->close();
    delete saves;
}
