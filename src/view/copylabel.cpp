#include "copylabel.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFocusEvent>

CopyLabel::CopyLabel(QWidget *parent) : QTextEdit(parent)
{
    m_selectFlag = false;

    this->setReadOnly(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CopyLabel::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        m_selectFlag = true;
    }
    QTextEdit::mousePressEvent(e);
}

void CopyLabel::focusOutEvent(QFocusEvent *e)
{
    if (!m_selectFlag)
    {
        this->moveCursor(QTextCursor::End);
    }
    else
    {
        m_selectFlag = false;
    }
    QTextEdit::focusOutEvent(e);
}

void CopyLabel::setText(const QString &txt)
{
    QTextEdit::setPlainText(txt);
}

QString CopyLabel::text() const
{
    return QTextEdit::toPlainText();
}

void CopyLabel::setReadOnly(bool read)
{
    QTextEdit::setReadOnly(read);
}

void CopyLabel::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    QTextEdit::setHorizontalScrollBarPolicy(policy);
}

void CopyLabel::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    QTextEdit::setVerticalScrollBarPolicy(policy);
}
