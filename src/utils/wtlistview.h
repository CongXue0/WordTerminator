#ifndef WTLISTVIEW_H
#define WTLISTVIEW_H

#include <QWidget>
#include <QListView>
#include <QEvent>

class WTListView : public QListView
{
    Q_OBJECT
public:
    explicit WTListView(QWidget *parent = nullptr);
//    void mousePressEvent(QMouseEvent *e);
//    void mouseReleaseEvent(QMouseEvent *e);
//    QModelIndexList selectedIndexes() const Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // WTLISTVIEW_H
