#ifndef WORDSETTINGWIDGET_H
#define WORDSETTINGWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>

class WordSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordSettingWidget(QWidget *parent = 0);
    void recoveryInterface();
    void reloadGlobalValue();
    void saveGlobalValue();

private:
    void loadStyleSheet();





private:
    QTabWidget *tabWidget;

    QWidget *widget_forgetSet;
    QWidget *widget_libSet;
    QWidget *widget_groupSet;
    QWidget *widget_help;

    QLabel *label_leastFoTi;
    QLabel *label_memInterval;
    QLabel *label_declineTimes;
    QLabel *label_times[10];


    QLineEdit *lineEdit_leastFoTi;
    QLineEdit *lineEdit_memInterval;
    QLineEdit *lineEdit_times[10];



};

#endif // WORDSETTINGWIDGET_H
