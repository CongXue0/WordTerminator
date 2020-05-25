#pragma once
#include <QPushButton>

class DtcpToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DtcpToolButton(QWidget *parent = nullptr);

    bool isAutoCheck() const;
    void setAutoCheck(bool autoCheck);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    bool autoCheck_;
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
