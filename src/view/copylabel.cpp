#include "copylabel.h"
#include "wtool.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QTimer>

CopyLabel::CopyLabel(QWidget *parent) : QTextEdit(parent)
{
    m_borderWidth = 0;

    m_align = this->alignment();
    minSize_ = QSize(0, 0);

    QTextEdit::setReadOnly(true);
    QTextEdit::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QTextEdit::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QSize CopyLabel::sizeHint() const
{
    auto size = this->minimumSizeHint();
    auto size2 = maximumViewportSize();
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

QSize CopyLabel::minimumSizeHint() const
{
    auto size = this->getIdealSize();
    if (minSize_.width() > size.width())
        size.setWidth(minSize_.width());
    if (minSize_.height() > size.height())
        size.setHeight(minSize_.height());
    return size;
}

void CopyLabel::setText(const QString &txt)
{
    QTextEdit::setPlainText(txt);
    if (m_align != this->alignment())
        QTextEdit::setAlignment(m_align);
    if (this->isVisible())
    {
        auto size = this->getIdealSize();
        if (size.height() < minSize_.height())
            size.setHeight(minSize_.height());
        this->setMinimumHeight(size.height());
    }
//    this->adjustSize();
//    this->updateSize();
//    this->resize(this->sizeHint());
}

QString CopyLabel::text() const
{
    return QTextEdit::toPlainText();
}

void CopyLabel::setAlignment(Qt::Alignment a)
{
    QTextEdit::setAlignment(a);
    m_align = a;
}

void CopyLabel::setMinimumSize(const QSize &size)
{
    minSize_ = size;
    QTextEdit::setMinimumSize(minSize_);
}

QSize CopyLabel::getIdealSize() const
{
    QTextDocument *doc = this->document()->clone();
    if (doc->textWidth() < 100)
        doc->setTextWidth(100);
    auto sp = this->sizePolicy();
    if (sp.horizontalPolicy() == QSizePolicy::Expanding && doc->textWidth() < maximumViewportSize().width())
        doc->setTextWidth(maximumViewportSize().width());
    QSize size = QSize(int(doc->idealWidth()), int(doc->size().height()));
    size.setWidth(size.width() + 2 * m_borderWidth);
    size.setHeight(size.height() + 2 * m_borderWidth);
    delete doc;
    return size;
}

void CopyLabel::setBorderWidth(int width)
{
    m_borderWidth = width;
}

int CopyLabel::getBorderWidth() const
{
    return m_borderWidth;
}

void CopyLabel::showEvent(QShowEvent *)
{
    auto size = this->getIdealSize();
    if (size.height() < minSize_.height())
        size.setHeight(minSize_.height());
    this->setMinimumHeight(size.height());

//    this->adjustSize();
}
