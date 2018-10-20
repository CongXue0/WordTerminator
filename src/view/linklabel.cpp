#include "linklabel.h"
#include <QDebug>

LinkLabel::LinkLabel(QWidget *parent) : QLineEdit(parent)
{
    this->setFrame(false);
    this->setReadOnly(true);
}

void LinkLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (this->selectedText().isEmpty() == true)
        emit released();
    QLineEdit::mouseReleaseEvent(e);
}

void LinkLabel::enterEvent(QEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    QLineEdit::enterEvent(event);
}

void LinkLabel::setReadOnly(bool read)
{
    QLineEdit::setReadOnly(read);
}
