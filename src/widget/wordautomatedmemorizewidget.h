#ifndef WORDAUTOMATEDMEMORIZEWIDGET_H
#define WORDAUTOMATEDMEMORIZEWIDGET_H

#include <QWidget>
#include "wordadmin.h"

namespace Ui {
    class WordAutomatedMemorizeWidget;
}

class WordAutomatedMemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    enum MODE {
        WELCOME, MEMORY
    };
    explicit WordAutomatedMemorizeWidget(QWidget *parent = nullptr);
    ~WordAutomatedMemorizeWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void updateWordStatistics();
    int getMode();

private:
    void setMode(int mode);

private slots:
    void slot_radioButtonClicked();
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_btnStart_Clicked();
    void slot_memorizeFinished();
    void slot_stopWordMemorize(bool *ret);

private:
    Ui::WordAutomatedMemorizeWidget *ui;
    int m_mode;
    int m_curGroupId;
    bool m_reloadFlag;

    QList<WordTest> m_testList;
};

#endif // WORDAUTOMATEDMEMORIZEWIDGET_H
