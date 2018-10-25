#include "wtbutton.h"
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>

WTButton::WTButton(QWidget *parent) : QCheckBox(parent)
{
    QCheckBox::setText("");
    m_activeTxt = "";
    m_inactiveTxt = "";
    m_activeTip = "";
    m_inactiveTip = "";
    m_activeColor = "#ffffff";
    m_inactiveColor = "#ffffff";

    btn_bg = new QPushButton(this);
    btn_bg->setFlat(true);
    btn_bg->setStyleSheet("QPushButton{border-style:none;}");
    btn_bg->installEventFilter(this);

    connect(this, SIGNAL(stateChanged(int)), this, SLOT(slot_buttonStateChanged(int)));
}

void WTButton::showEvent(QShowEvent *)
{
    btn_bg->setGeometry(0, 0, this->width(), this->height());
    btn_bg->setFont(this->font());
    updateColor();
}

bool WTButton::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == btn_bg && e->type() == QEvent::MouseButtonPress)
    {
        this->click();
        return true;
    }
    return QCheckBox::eventFilter(obj, e);
}

void WTButton::setText(QString text)
{
    if (text.contains("&"))
        text.replace("&", "&&");
    btn_bg->setText(text);
}

QString WTButton::text() const
{
    QString txt = btn_bg->text();
    if (txt.contains("&"))
        txt.replace("&&", "&");
    return txt;
}

void WTButton::setActive(bool active)
{
    this->setChecked(active);
}

bool WTButton::isActive() const
{
    return this->isChecked();
}

void WTButton::setActiveText(const QString& activeTxt)
{
    if (m_activeTxt == activeTxt)
        return;
    m_activeTxt = activeTxt;
    if (this->isChecked() && !m_activeTxt.isEmpty())
        this->setText(m_activeTxt);
}

void WTButton::setInactiveText(const  QString & inactiveTxt)
{
    if (m_inactiveTxt == inactiveTxt)
        return;
    m_inactiveTxt = inactiveTxt;
    if (!this->isChecked() && !m_inactiveTxt.isEmpty())
        this->setText(m_inactiveTxt);
}

void WTButton::setActiveTip(const QString& activeTip)
{
    if (m_activeTip == activeTip)
        return;
    m_activeTip = activeTip;
    if (this->isChecked() && !m_activeTip.isEmpty())
        this->setToolTip(m_activeTip);
}

void WTButton::setInactiveTip(const QString &inactiveTip)
{
    if (m_inactiveTip == inactiveTip)
        return;
    m_inactiveTip = inactiveTip;
    if (!this->isChecked()&& !m_inactiveTip.isEmpty())
        this->setToolTip(m_inactiveTip);
}

QString WTButton::getActiveColor()
{
    return m_activeColor;
}

void WTButton::setActiveColor(QString color)
{
    m_activeColor = color;
}

QString WTButton::getInactiveColor()
{
    return m_inactiveColor;
}

void WTButton::setInactiveColor(QString color)
{
    m_inactiveColor = color;
}

QPushButton *WTButton::getBgBtn()
{
    return btn_bg;
}

void WTButton::updateColor()
{
    if (this->isActive() == true)
    {
        btn_bg->setStyleSheet(QString("QPushButton{color:%1;}").arg(m_activeColor));
    }
    else
    {
        btn_bg->setStyleSheet(QString("QPushButton{color:%1;}").arg(m_inactiveColor));
    }
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
        if (!m_inactiveTxt.isEmpty())
            this->setText(m_inactiveTxt);
        if (!m_inactiveTip.isEmpty())
            this->setToolTip(m_inactiveTip);
    }
    else if (status == 2)
    {
        if (!m_activeTxt.isEmpty())
            this->setText(m_activeTxt);
        if (!m_activeTip.isEmpty())
            this->setToolTip(m_activeTip);
    }
    updateColor();
}
