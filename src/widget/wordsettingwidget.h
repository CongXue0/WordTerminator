#ifndef WORDSETTINGWIDGET_H
#define WORDSETTINGWIDGET_H

#include "wmessage.h"
#include <QLineEdit>

namespace Ui
{
    class WordSettingWidget;
}

class WordSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordSettingWidget(QWidget *parent = nullptr);
    ~WordSettingWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void saveGlobalValue();

private slots:
    void slot_btnReset_clicked();

private:
    Ui::WordSettingWidget *ui;
    QLineEdit *lineEdit_times[10];
    QLineEdit *lineEdit_groupName[21];
};

#endif // WORDSETTINGWIDGET_H
