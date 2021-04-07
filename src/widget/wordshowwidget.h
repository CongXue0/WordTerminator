#ifndef WORDSHOWWIDGET_H
#define WORDSHOWWIDGET_H

#include "copylabel.h"
#include "linklabel.h"
#include "wordadmin.h"
#include "wmessage.h"
#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>

namespace Ui
{
    class WordShowWidget;
}

class WordShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordShowWidget(QWidget *parent = nullptr);
    ~WordShowWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    bool loadWordInfo(QString name);
    WordInfo currentWordInfo();

private:
    void keyPressEvent(QKeyEvent *event);
    void setWordInfo();
    void reloadLayout();
    void setWordTimes(int times);

private slots:
    void slot_btnReturn_Clicked();
    void slot_btnMin_Clicked();
    void slot_btnMid_Clicked();
    void slot_btnMax_Clicked();
    void slot_btnAdd_Clicked();
    void slot_btnDelete_Clicked();
    void slot_btnEdit_Clicked();
    void slot_btnReset_Clicked();
    void slot_btnRemember_Clicked(bool active);
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_wordLinkPressed();
    void slot_wordTimeDecline(QString name);
    void slot_wordTimeIncrease(QString name);

private:
    Ui::WordShowWidget *ui;
    WordInfo m_word;
    int m_propertyNum;
    int m_exampleNum;
    int m_synonymNum;
    int m_antonymNum;
    int m_derivativeNum;
    int m_lineNum;

    QStringList m_groupList;

    bool m_reloadFlag;

    CopyLabel *copyLabel_property[PROPERTY_NUM];
    CopyLabel *copyLabel_explain[PROPERTY_NUM];
    QWidget *widget_explain[PROPERTY_NUM];

    CopyLabel *copyLabel_example[EXAMPLE_NUM];

    LinkLabel *linkLabel_synonym[RELATED_NUM];
    LinkLabel *linkLabel_antonym[RELATED_NUM];
    LinkLabel *linkLabel_derivative[RELATED_NUM];
};

#endif // WORDSHOWWIDGET_H
