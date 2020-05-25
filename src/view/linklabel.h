#ifndef LINKLABEL_H
#define LINKLABEL_H

#include <QLineEdit>

class LinkLabel : public QLineEdit
{
    Q_OBJECT
public:
    explicit LinkLabel(QWidget *parent = nullptr);

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

    void setText(const QString &txt);
    QSize getIdealSize() const;

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    void setReadOnly(bool read) = delete;

signals:
    void released();
};

#endif // LINKLABEL_H
