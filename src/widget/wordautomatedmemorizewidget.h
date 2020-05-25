#ifndef WORDAUTOMATEDMEMORIZEWIDGET_H
#define WORDAUTOMATEDMEMORIZEWIDGET_H

#include <QWidget>

namespace Ui {
class WordAutomatedMemorizeWidget;
}

class WordAutomatedMemorizeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WordAutomatedMemorizeWidget(QWidget *parent = nullptr);
    ~WordAutomatedMemorizeWidget();

private:
    Ui::WordAutomatedMemorizeWidget *ui;
};

#endif // WORDAUTOMATEDMEMORIZEWIDGET_H
