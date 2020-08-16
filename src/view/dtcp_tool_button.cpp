#include "dtcp_tool_button.h"
#include <QEvent>
#include <QDebug>

DtcpToolButton::DtcpToolButton(QWidget *parent) : QPushButton(parent)
{
    autoCheck_ = true;
    this->installEventFilter(this);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(OnButtonToggled(bool)));
}

bool DtcpToolButton::isAutoCheck() const
{
    return autoCheck_;
}

void DtcpToolButton::setAutoCheck(bool autoCheck)
{
    autoCheck_ = autoCheck;
}

void DtcpToolButton::setText(const QString &text)
{
    auto tmpText = text;
    if (tmpText.contains("&"))
        tmpText.replace("&", "&&");
    QPushButton::setText(tmpText);
}

QString DtcpToolButton::text() const
{
    QString txt = QPushButton::text();
    if (txt.contains("&"))
        txt.replace("&&", "&");
    return txt;
}

void DtcpToolButton::setCheckedText(const QString& checkedTxt)
{
    if (m_checkedTxt == checkedTxt)
        return;
    m_checkedTxt = checkedTxt;
    if (this->isChecked() && !m_checkedTxt.isEmpty())
        this->setText(m_checkedTxt);
}

void DtcpToolButton::setUncheckedText(const QString& uncheckedTxt)
{
    if (m_uncheckedTxt == uncheckedTxt)
        return;
    m_uncheckedTxt = uncheckedTxt;
    if (!this->isChecked() && !m_uncheckedTxt.isEmpty())
        this->setText(m_uncheckedTxt);
}

void DtcpToolButton::setCheckedTip(const QString& checkedTip)
{
    if (m_checkedTip == checkedTip)
        return;
    m_checkedTip = checkedTip;
    if (this->isChecked() && !m_checkedTip.isEmpty())
        this->setToolTip(m_checkedTip);
}

void DtcpToolButton::setUncheckedTip(const QString &uncheckedTip)
{
    if (m_uncheckedTip == uncheckedTip)
        return;
    m_uncheckedTip = uncheckedTip;
    if (!this->isChecked() && !m_uncheckedTip.isEmpty())
        this->setToolTip(m_uncheckedTip);
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

void DtcpToolButton::OnButtonToggled(bool checked)
{
    if (this->isChecked())
    {
        if (!m_checkedTxt.isEmpty())
            this->setText(m_checkedTxt);
        if (!m_checkedTip.isEmpty())
            this->setToolTip(m_checkedTip);
    }
    else
    {
        if (!m_uncheckedTxt.isEmpty())
            this->setText(m_uncheckedTxt);
        if (!m_uncheckedTip.isEmpty())
            this->setToolTip(m_uncheckedTip);
    }
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
