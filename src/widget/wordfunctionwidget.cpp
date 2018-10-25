#include "wordfunctionwidget.h"
#include "wtool.h"

WordFunctionWidget::WordFunctionWidget(QWidget *parent) : QWidget(parent)
{

    loadStyleSheet();
}

void WordFunctionWidget::recoveryInterface()
{

}

void WordFunctionWidget::loadStyleSheet()
{
    setStyleSheet(WTool::getWordMemorizeWidgetQss());
}
