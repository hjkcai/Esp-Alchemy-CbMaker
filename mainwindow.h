#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "highlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QTextEdit *txt, *txterr;
    highlighter *h;
    QString fileName;

private slots:
    void checkError();
    void save();
};

#endif // MAINWINDOW_H
