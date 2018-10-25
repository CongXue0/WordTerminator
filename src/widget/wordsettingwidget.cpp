#include "wordsettingwidget.h"
#include "wtool.h"

WordSettingWidget::WordSettingWidget(QWidget *parent) : QWidget(parent)
{

    loadStyleSheet();
}

void WordSettingWidget::recoveryInterface()
{

}

void WordSettingWidget::loadStyleSheet()
{
     setStyleSheet(WTool::getWordMemorizeWidgetQss());
}
