#include "linklabel.h"
#include <QDebug>

LinkLabel::LinkLabel(QWidget *parent) : QLineEdit(parent)
{
    QLineEdit::setReadOnly(true);
}

QSize LinkLabel::sizeHint() const
{
    auto size = this->minimumSizeHint();
    auto size2 = QLineEdit::sizeHint();
    auto sp = this->sizePolicy();
    if (sp.horizontalPolicy() == QSizePolicy::Expanding)
    {
        if (size2.width() > size.width())
            size.setWidth(size2.width());
    }
    if (sp.verticalPolicy() == QSizePolicy::Expanding)
    {
        if (size2.height() > size.height())
            size.setHeight(size2.height());
    }
    return size;
}

QSize LinkLabel::minimumSizeHint() const
{
    auto size = this->getIdealSize();
    if (minimumSize().width() > size.width())
        size.setWidth(minimumSize().width());
    if (minimumSize().height() > size.height())
        size.setHeight(minimumSize().height());
    return size;
}

void LinkLabel::setText(const QString &txt)
{
    QLineEdit::setText(txt);
    this->adjustSize();
}

QSize LinkLabel::getIdealSize() const
{
    QFontMetrics fm(this->font());
    int h = 0;
    if (this->text().isEmpty())
        h = fm.boundingRect(" ").size().height();
    QSize size = fm.boundingRect(this->text()).size();
    if (h > 0)
        size.setHeight(h);
    auto margin = this->textMargins();
    size.setWidth(size.width() + margin.left() + margin.right() + 6);
    size.setHeight(size.height() + margin.top() + margin.bottom());
    return size;
}

void LinkLabel::enterEvent(QEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    QLineEdit::enterEvent(event);
}

void LinkLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->selectedText().isEmpty())
        emit released();
    QLineEdit::mouseReleaseEvent(event);
}
