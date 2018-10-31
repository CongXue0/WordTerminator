#ifndef WORDSETTINGWIDGET_H
#define WORDSETTINGWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

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
    QLabel *label_memLevel;
    QLabel *label_commonShow;
    QLabel *label_range1;
    QLabel *label_range2;
    QLabel *label_range3;
    QLabel *label_range4;
    QLabel *label_range1_f;
    QLabel *label_range2_f;
    QLabel *label_range3_f;
    QLabel *label_range4_plus;
    QLabel *label_timesSet1;
    QLabel *label_timesSet2;

    QLineEdit *lineEdit_leastFoTi;
    QLineEdit *lineEdit_memInterval;
    QLineEdit *lineEdit_times[10];
    QLineEdit *lineEdit_range1Left;
    QLineEdit *lineEdit_range1Right;
    QLineEdit *lineEdit_range2Left;
    QLineEdit *lineEdit_range2Right;
    QLineEdit *lineEdit_range3Left;
    QLineEdit *lineEdit_range3Right;
    QLineEdit *lineEdit_range4Left;
    QLineEdit *lineEdit_timesSet1;
    QLineEdit *lineEdit_timesSet2;

    QComboBox *combox_memLevel;

};

#endif // WORDSETTINGWIDGET_H
