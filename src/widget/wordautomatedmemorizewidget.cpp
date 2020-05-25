#include "wordautomatedmemorizewidget.h"
#include "ui_wordautomatedmemorizewidget.h"
#include "wtool.h"

WordAutomatedMemorizeWidget::WordAutomatedMemorizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordAutomatedMemorizeWidget)
{
    setStyleSheet(WTool::getStyleQss("WordAutomatedMemorizeWidget"));
    ui->setupUi(this);
}

WordAutomatedMemorizeWidget::~WordAutomatedMemorizeWidget()
{
    delete ui;
}
