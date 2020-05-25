#include "wtbutton.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>

WTButton::WTButton(QWidget *parent) : QPushButton(parent)
{
    m_checkedTxt = "";
    m_uncheckedTxt = "";
    m_checkedTip = "";
    m_uncheckedTip = "";

    this->setCheckable(true);

    connect(this, SIGNAL(clicked(bool)), this, SLOT(slot_buttonClicked(bool)));
}

void WTButton::setText(const QString &text)
{
    auto tmpText = text;
    if (tmpText.contains("&"))
        tmpText.replace("&", "&&");
    QPushButton::setText(tmpText);
}

QString WTButton::text() const
{
    QString txt = QPushButton::text();
    if (txt.contains("&"))
        txt.replace("&&", "&");
    return txt;
}

void WTButton::setCheckedText(const QString& checkedTxt)
{
    if (m_checkedTxt == checkedTxt)
        return;
    m_checkedTxt = checkedTxt;
    if (this->isChecked() && !m_checkedTxt.isEmpty())
        this->setText(m_checkedTxt);
}

void WTButton::setUncheckedText(const QString& uncheckedTxt)
{
    if (m_uncheckedTxt == uncheckedTxt)
        return;
    m_uncheckedTxt = uncheckedTxt;
    if (!this->isChecked() && !m_uncheckedTxt.isEmpty())
        this->setText(m_uncheckedTxt);
}

void WTButton::setCheckedTip(const QString& checkedTip)
{
    if (m_checkedTip == checkedTip)
        return;
    m_checkedTip = checkedTip;
    if (this->isChecked() && !m_checkedTip.isEmpty())
        this->setToolTip(m_checkedTip);
}

void WTButton::setUncheckedTip(const QString &uncheckedTip)
{
    if (m_uncheckedTip == uncheckedTip)
        return;
    m_uncheckedTip = uncheckedTip;
    if (!this->isChecked() && !m_uncheckedTip.isEmpty())
        this->setToolTip(m_uncheckedTip);
}

void WTButton::slot_buttonClicked(bool check)
{
    if (check)
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
