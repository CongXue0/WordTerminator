#include "wtlistview.h"
#include "wtool.h"

WTListView::WTListView(QWidget *parent) : QListView(parent)
{
}

void WTListView::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);
    QWidget::mousePressEvent(e);
}

void WTListView::mouseReleaseEvent(QMouseEvent *e)
{
    QListView::mouseReleaseEvent(e);
    QWidget::mouseReleaseEvent(e);
}

QModelIndexList WTListView::selectedIndexes() const
{
    return QListView::selectedIndexes();
}
