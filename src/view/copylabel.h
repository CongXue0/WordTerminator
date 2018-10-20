#ifndef COPYLABEL_H
#define COPYLABEL_H

#include <QTextEdit>

class CopyLabel : public QTextEdit
{
    Q_OBJECT
public:
    explicit CopyLabel(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void setText(const QString &txt);
    QString text() const;

signals:

public slots:

private:
    void setReadOnly(bool read);
    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);
    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);

private:
    bool m_selectFlag;

};

#endif // COPYLABEL_H
