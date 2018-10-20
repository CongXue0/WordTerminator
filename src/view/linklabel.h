#ifndef LINKLABEL_H
#define LINKLABEL_H

#include <QLineEdit>

class LinkLabel : public QLineEdit
{
    Q_OBJECT
public:
    explicit LinkLabel(QWidget *parent = 0);
    void mouseReleaseEvent(QMouseEvent *e);
    void enterEvent(QEvent *event);

signals:
    void released();

public slots:

private:
    void setReadOnly(bool read);

private:

};

#endif // LINKLABEL_H
