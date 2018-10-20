#ifndef WTBUTTON_H
#define WTBUTTON_H

#include <QCheckBox>

class WTButton : public QCheckBox
{
    Q_OBJECT
public:
    explicit WTButton(QWidget *parent = 0);

    void setActive(bool active);
    bool isActive() const;
    void setActiveText(QString activeTxt);
    void setInactiveText(QString inactiveTxt);
    void setActiveTip(QString activeTip);
    void setInactiveTip(QString inactiveTip);

signals:

private:
    void setChecked(bool check);
    bool isChecked() const;
    void setToolTip(const QString &tip);

private slots:
    void slot_buttonStateChanged(int status);


private:
    QString m_activeTxt;
    QString m_inactiveTxt;
    QString m_activeTip;
    QString m_inactiveTip;
};

#endif // WTBUTTON_H
