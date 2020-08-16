#pragma once
#include <QPushButton>

class DtcpToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DtcpToolButton(QWidget *parent = nullptr);

    bool isAutoCheck() const;
    void setAutoCheck(bool autoCheck);

    void setText(const QString &text);
    QString text() const;
    void setCheckedText(const QString& checkedTxt);
    void setUncheckedText(const QString& uncheckedTxt);
    void setCheckedTip(const QString& checkedTip);
    void setUncheckedTip(const QString &uncheckedTip);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

protected slots:
    void OnButtonToggled(bool checked);

protected:
    bool autoCheck_;
    QString m_checkedTxt;
    QString m_uncheckedTxt;
    QString m_checkedTip;
    QString m_uncheckedTip;
};

class DtcpImageButton : public DtcpToolButton
{
    Q_OBJECT
public:
    explicit DtcpImageButton(QWidget *parent = nullptr);
};

class DtcpTextButton : public DtcpToolButton
{
    Q_OBJECT
public:
    explicit DtcpTextButton(QWidget *parent = nullptr);
    explicit DtcpTextButton(const QString &text, QWidget *parent = nullptr);

    virtual QSize minimumSizeHint() const override;
};
