#ifndef WORDFUNCTIONWIDGET_H
#define WORDFUNCTIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>

class WordFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordFunctionWidget(QWidget *parent = 0);
    void recoveryInterface();
    void reloadGlobalValue();
    void saveGlobalValue();

private:
    void loadStyleSheet();
    void exportWord(int t1, int t2, int group,int remember);

private slots:
    void slot_btnRefresh_pressed();
    void slot_btnExport_pressed();
    void slot_comboxScript_currentIndexChanged(int index);

private:
    QLabel *label_bg_func;
    QLabel *label_refresh;
    QLabel *label_export;
    QLabel *line1;
    QLabel *line2;

    QPushButton *btn_refresh;
    QPushButton *btn_export;

    QComboBox *combox_script;

    QLineEdit *lineEdit_script;


};

#endif // WORDFUNCTIONWIDGET_H
