#include "highlighter.h"

highlighter::highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

QString highlighter::subString(const QString &s,const QString &after, const QString &before)
{
    int start = after.isNull() ? 0 : s.indexOf(after) + after.length();
    if (start == -1) start = 0;

    int end = before.isNull() ? s.length() : s.lastIndexOf(before) - 1;
    if (end == -1) end = s.length();

    return s.mid(start, end - start + 1);
}

void highlighter::check(const QString &text)
{
    items.clear();
    QStringList datal = text.split('\n', QString::SkipEmptyParts);

    // 第一轮：检查元素
    for (int i = 0; i < datal.count(); i++)
    {
        QString data = datal[i];

        int colon = data.indexOf(':');
        int equal = data.indexOf('=');
        bool basic = data[0] == '#';
        bool star = data[0] == '*';
        int idstart = basic || star ? 1 : 0;

        if (basic && equal != -1)
        {
            /* #id:tr=...
                     ^~~~
               基本元素不需要组合 */

            highlightItem hi = highlightItem(datal[i], i, equal, data.length() - equal);
            hi.describe = tr("error #1");
            items << hi;
        }
        else if (!basic && equal == -1)
        {
            /* id:tr
                    ^
               没有组合 */

            highlightItem hi = highlightItem(datal[i], i, 0, 0);
            hi.describe = tr("error #2");
            items << hi;
        }

        if (colon == -1)
        {
            /* id =...
                 ^
               没有名称 */

            highlightItem hi = highlightItem(datal[i], i, idstart, equal - idstart);
            hi.describe = tr("error #3");
            items << hi;
        }
        else
        {
            QString id = data.mid(idstart, colon - idstart).trimmed();
            QString name = data.mid(colon + 1, equal - colon - 1).trimmed();

            if (id.isEmpty())
            {
                // id为空
                highlightItem hi = highlightItem(datal[i], i, idstart, equal - idstart);
                hi.describe = tr("error #4");
                items << hi;
            }
            else
            {
                 QRegExp regexp("[A-Za-z0-9_]*");
                 int index = regexp.indexIn(id);

                 if (index != 0 || regexp.matchedLength() != id.length())
                 {
                     // id不符合规则
                     highlightItem hi = highlightItem(datal[i], i, idstart, equal - idstart);
                     hi.describe = tr("error #5");
                     items << hi;
                 }
                 else elements << id;
            }

            if (name.isEmpty())
            {
                // 名称为空
                highlightItem hi = highlightItem(datal[i], i, colon + 1, equal - colon - 1);
                hi.describe = tr("error #6");
                items << hi;
            }
        }
    }

    // 第二轮：检查组合
    for (int j = 0; j < datal.count(); j++)
    {
        QString data = datal[j];
        int first = 0;

        if (data[0] == '*' || data[0] == '#')
        {
            if (data[0] == '#') continue;
            data = data.mid(1);
            first = 1;
        }

        QStringList product = subString(data, QString::null, "=").split('+', QString::SkipEmptyParts);
        QStringList source = subString(data, "=").split('+', QString::SkipEmptyParts);
        int equal = data.indexOf('=');

        int l1 = 0;
        for (int i = 0; i < product.length(); i++)
        {
            QString s = subString(product[i], QString::null, ":");
            if (elements.indexOf(s) == -1)
            {
                // id无效
                highlightItem hi = highlightItem(datal[j], j, first + l1, s.length());
                hi.describe = tr("error #7");
                items << hi;
            }

            l1 += product[i].length() + 1;
        }

        if (product.length() == 0)
        {
            // 没有目标组合
            highlightItem hi = highlightItem(datal[j], j, 0, 0);
            hi.describe = tr("error #8");
            items << hi;
        }

        int l2 = 0;
        for (int i = 0; i < source.length(); i++)
        {
            if (elements.indexOf(source[i]) == -1)
            {
                // id无效
                highlightItem hi = highlightItem(datal[j], j, first + l1 + l2, source[i].length());
                hi.describe = tr("error #7");
                items << hi;
            }

            l2 += source[i].length() + 1;
        }

        if (source.length() == 0)
        {
            // 没有源组合
            highlightItem hi = highlightItem(datal[j], j, equal + 1, 0);
            hi.describe = tr("error #9");
            items << hi;
        }
    }

    this->rehighlight();
}

void highlighter::highlightBlock(const QString &text)
{
    int i;
    for (i = 0; i < items.count(); i++)
    {
        if (text == items[i].match)
            this->setFormat(items[i].start, items[i].length, Qt::red);
    }

//    if (i == items.count())
//        this->setFormat(0, text.length(), Qt::black);
}
