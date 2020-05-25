#ifndef WTBUTTON_H
#define WTBUTTON_H

#include <QPushButton>

class WTButton : public QPushButton
{
    Q_OBJECT
public:
    explicit WTButton(QWidget *parent = nullptr);

    void setText(const QString &text);
    QString text() const;
    void setCheckedText(const QString& checkedTxt);
    void setUncheckedText(const QString& uncheckedTxt);
    void setCheckedTip(const QString& checkedTip);
    void setUncheckedTip(const QString &uncheckedTip);

protected slots:
    void slot_buttonClicked(bool check);

protected:
    QString m_checkedTxt;
    QString m_uncheckedTxt;
    QString m_checkedTip;
    QString m_uncheckedTip;
};

#endif // WTBUTTON_H
