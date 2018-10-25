#ifndef WTBUTTON_H
#define WTBUTTON_H

#include <QCheckBox>
#include <QPushButton>

class WTButton : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(QString activeColor READ getActiveColor WRITE setActiveColor DESIGNABLE true)
    Q_PROPERTY(QString inactiveColor READ getInactiveColor WRITE setInactiveColor DESIGNABLE true)
public:
    explicit WTButton(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *obj, QEvent *e);

    void setText(QString text);
    QString text() const;
    void setActive(bool active);
    bool isActive() const;
    void setActiveText(const QString& activeTxt);
    void setInactiveText(const QString& inactiveTxt);
    void setActiveTip(const QString& activeTip);
    void setInactiveTip(const QString& inactiveTip);
    QString getActiveColor();
    void setActiveColor(QString color);
    QString getInactiveColor();
    void setInactiveColor(QString color);
    QPushButton *getBgBtn();

    void updateColor();

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
    QString m_activeColor;
    QString m_inactiveColor;

    QPushButton *btn_bg;
};

#endif // WTBUTTON_H
