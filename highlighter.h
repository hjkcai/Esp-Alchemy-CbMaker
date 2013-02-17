#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QtGui>

class highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    highlighter(QTextDocument *parent = 0);
    void check(const QString &text);

    struct highlightItem
    {
    public:
        highlightItem() { }

        highlightItem(const QString &m, const int &li, const int &s, const int &l)
        {
            match = m;
            line = li;
            start = s;
            length = l;
        }

        QString match;
        int line;
        int start;
        int length;
        QString describe;
    };

    QVector<highlightItem> items;

protected:
    void highlightBlock(const QString &text);

private:
    QVector<QString> elements;
    QString subString(const QString &s, const QString &after = QString::null, const QString &before = QString::null);
};

#endif // HIGHLIGHTER_H
