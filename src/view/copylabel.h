#ifndef COPYLABEL_H
#define COPYLABEL_H

#include <QTextEdit>

class CopyLabel : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
public:
    explicit CopyLabel(QWidget *parent = nullptr);

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

    void setText(const QString &txt);
    QString text() const;
    void setAlignment(Qt::Alignment a);
    void setMinimumSize(const QSize &size);
    QSize getIdealSize() const;

protected:
    void setBorderWidth(int width);
    int getBorderWidth() const;

    virtual void showEvent(QShowEvent *) override;

    void setReadOnly(bool) = delete;
    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy) = delete;
    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy) = delete;

protected:
    int m_borderWidth;

    Qt::Alignment m_align;
    QSize minSize_;
    bool ignoreResize_;
};

#endif // COPYLABEL_H
