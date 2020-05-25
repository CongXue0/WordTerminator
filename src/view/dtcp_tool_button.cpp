#include "dtcp_tool_button.h"
#include <QEvent>
#include <QDebug>

DtcpToolButton::DtcpToolButton(QWidget *parent) : QPushButton(parent)
{
    autoCheck_ = true;
    this->installEventFilter(this);
}

bool DtcpToolButton::isAutoCheck() const
{
    return autoCheck_;
}

void DtcpToolButton::setAutoCheck(bool autoCheck)
{
    autoCheck_ = autoCheck;
}

bool DtcpToolButton::eventFilter(QObject *obj, QEvent *event)
{
    if (!autoCheck_ && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) && obj == this)
    {
        if (this->isEnabled())
            emit this->clicked();
        return true;
    }
    return QPushButton::eventFilter(obj, event);
}

DtcpImageButton::DtcpImageButton(QWidget *parent) : DtcpToolButton(parent)
{
}

DtcpTextButton::DtcpTextButton(QWidget *parent) : DtcpToolButton(parent)
{
}

DtcpTextButton::DtcpTextButton(const QString &text, QWidget *parent) : DtcpToolButton(parent)
{
    this->setText(text);
}

QSize DtcpTextButton::minimumSizeHint() const
{
    QString txt = this->text();
    QSize size(75, 23);
    if (!txt.isEmpty())
    {
        QSize txtSize = QFontMetrics(this->font()).boundingRect(txt).size();
        int h = txtSize.height(), w = txtSize.width();
        int addw = int(h / 2.0);
        if (addw < w / 5)
            addw = w / 5;
        if (addw > 40)
            addw = 40;
        int addh = int(h / 5.0);
        if (addw < 2)
            addw = 2;
        txtSize += QSize(addw, addh);
        if (size.width() < txtSize.width())
            size.setWidth(txtSize.width());
        if (size.height() < txtSize.height())
            size.setHeight(txtSize.height());
    }
    return size;
}
