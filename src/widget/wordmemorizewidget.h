#ifndef WORDMEMORIZEWIDGET_H
#define WORDMEMORIZEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include "copylabel.h"
#include "wordadmin.h"
#include "linklabel.h"
#include "wtool.h"

namespace Ui
{
    class WordMemorizeWidget;
}

class WordMemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    enum MODE {
        WELCOME, MEMORY
    };
    enum TEST {
        NONE = -1, EXPLORATE, RECALL, DICTATE
    };
    enum TEST_STRATEGY {
        STRATEGY0, STRATEGY1
    };

    explicit WordMemorizeWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void updateWordStatistics();
    int getMode();

private:
    void setMode(int mode);
    void testListInit();
    void loadTestInfo();
    void setWordInfo(bool isShield);
    void reloadLayout();
    void bottomLayoutSet(QVector<QVector<QWidget *>> w_set);
    void wordInfoShow(int mode);
    void chooseTestWord();
    void chooseStrategy_level0();
    void chooseStrategy_level1();

private slots:
    void slot_radioButtonClicked();
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_btnStart_Clicked();
    void slot_btnKnow_Clicked();
    void slot_btnNotKnow_Clicked();
    void slot_btnForever_Clicked();
    void slot_btnNotForever_Clicked();
    void slot_btnNext_Clicked();
    void slot_btnSubmit_Clicked();
//    void slot_wordCanMemorize(QString name);
//    void slot_wordTimeDecline(QString name);
    void slot_stopWordMemorize(bool *ret);

signals:
    void wordTimeIncreaseSignal(QString name);

private:
    Ui::WordMemorizeWidget *ui;
    int m_mode;
    int m_test;
    int m_propertyNum;
    int m_exampleNum;
    int m_synonymNum;
    int m_antonymNum;
    int m_lineNum;
    int m_container_height_;

    int m_testModeNum;
    int m_testNum;
    int m_pastNum;
    int m_curIndex;
    QString m_lastWord;
    int m_spacing;

    bool m_reloadFlag;
    int m_curGroupId;

    QList<WordTest> m_testList;
    QStringList m_nameList;
    WordInfo m_word;

    QMutex m_mutex;

    CopyLabel *copyLabel_property[PROPERTY_NUM];
    CopyLabel *copyLabel_explain[PROPERTY_NUM];
    QWidget *widget_explain[PROPERTY_NUM];
    CopyLabel *copyLabel_example[EXAMPLE_NUM];

    LinkLabel *linkLabel_synonym[RELATED_NUM];
    LinkLabel *linkLabel_antonym[RELATED_NUM];
};

#endif // WORDMEMORIZEWIDGET_H
