#include "wtbutton.h"

WTButton::WTButton(QWidget *parent) : QCheckBox(parent)
{
    m_activeTxt = "";
    m_inactiveTxt = "";
    m_activeTip = "";
    m_inactiveTip = "";
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(slot_buttonStateChanged(int)));
}

void WTButton::setActive(bool active)
{
    this->setChecked(active);
}

bool WTButton::isActive() const
{
    return this->isChecked();
}

void WTButton::setActiveText(QString activeTxt)
{
    if (m_activeTxt == activeTxt)
        return;
    m_activeTxt = activeTxt;
    if (this->isChecked() == true && m_activeTxt.isEmpty() == false)
        this->setText(m_activeTxt);
}

void WTButton::setInactiveText(QString inactiveTxt)
{
    if (m_inactiveTxt == inactiveTxt)
        return;
    m_inactiveTxt = inactiveTxt;
    if (this->isChecked() == false && m_inactiveTxt.isEmpty() == false)
        this->setText(m_inactiveTxt);
}

void WTButton::setActiveTip(QString activeTip)
{
    if (m_activeTip == activeTip)
        return;
    m_activeTip = activeTip;
    if (this->isChecked() == true && m_activeTip.isEmpty() == false)
        this->setToolTip(m_activeTip);
}

void WTButton::setInactiveTip(QString inactiveTip)
{
    if (m_inactiveTip == inactiveTip)
        return;
    m_inactiveTip = inactiveTip;
    if (this->isChecked() == false && m_inactiveTip.isEmpty() == false)
        this->setToolTip(m_inactiveTip);
}

void WTButton::setChecked(bool check)
{
    QCheckBox::setChecked(check);
}

bool WTButton::isChecked() const
{
    return QCheckBox::isChecked();
}

void WTButton::setToolTip(const QString &tip)
{
    QCheckBox::setToolTip(tip);
}

void WTButton::slot_buttonStateChanged(int status)
{
    if (status == 0)
    {
        if (m_inactiveTxt.isEmpty() == false)
            this->setText(m_inactiveTxt);
        if (m_inactiveTip.isEmpty() == false)
            this->setToolTip(m_inactiveTip);
    }
    else if (status == 2)
    {
        if (m_activeTxt.isEmpty() == false)
            this->setText(m_activeTxt);
        if (m_activeTip.isEmpty() == false)
            this->setToolTip(m_activeTip);
    }
}
