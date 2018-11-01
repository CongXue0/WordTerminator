#ifndef WORDSHOWWIDGET_H
#define WORDSHOWWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "wtbutton.h"
#include <QScrollArea>
#include "copylabel.h"
#include "linklabel.h"
#include "wordadmin.h"
#include "wmessage.h"
#include <QComboBox>

class WordShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordShowWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);

    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    bool loadWordInfo(QString name);

private:
    void loadStyleSheet();
    void setWordInfo();
    void setViewPosition();
    void setViewPosition_14();
    void setViewPosition_15();
    void setWordTimes(int times);

signals:
    void sendMessageSignal(WMessage message);
    void wordTimeIncreaseSignal(QString name);

private slots:
    void slot_btnReturn_Clicked();
    void slot_btnMin_Clicked();
    void slot_btnMid_Clicked();
    void slot_btnMax_Clicked();
    void slot_btnAdd_Clicked();
    void slot_btnDelete_Clicked();
    void slot_btnEdit_Clicked();
    void slot_btnReset_Clicked();
    void slot_wbtnRemember_Clicked(bool active);
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_wordLinkPressed();
    void slot_wordTimeDecline(QString name);
    void slot_wordTimeIncrease(QString name);

private:
    QString m_screenSize;
    WordInfo m_word;
    int m_propertyNum;
    int m_exampleNum;
    int m_synonymNum;
    int m_antonymNum;

    QStringList m_groupList;

    bool m_reloadFlag;

    QWidget *widget_topBar;
    QWidget *widget_show;

    QPushButton *btn_return;
    QPushButton *btn_min;
    QPushButton *btn_mid;
    QPushButton *btn_max;
    QPushButton *btn_add;
    QPushButton *btn_delete;
    QPushButton *btn_edit;
    QPushButton *btn_reset;

    WTButton *wbtn_isRemember;

    QComboBox *combox_group;

    QScrollArea *scrollArea;

    CopyLabel *copyLabel_word;
    CopyLabel *copyLabel_phoneticSymbol;
    CopyLabel *copyLabel_times;
    CopyLabel *copyLabel_property[20];
    CopyLabel *copyLabel_explain[20];
    CopyLabel *copyLabel_exampleSentence;
    CopyLabel *copyLabel_example[6];
    CopyLabel *copyLabel_synonym;
    CopyLabel *copyLabel_antonym;

    LinkLabel *linkLabel_synonym[8];
    LinkLabel *linkLabel_antonym[8];

};

#endif // WORDSHOWWIDGET_H
