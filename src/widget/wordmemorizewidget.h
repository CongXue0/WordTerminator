#ifndef WORDMEMORIZEWIDGET_H
#define WORDMEMORIZEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include "copylabel.h"
#include <QScrollArea>
#include "wordadmin.h"
#include <QLineEdit>
#include "linklabel.h"

class WordMemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    enum MODE
    {
        WELCOME, MEMORY
    };
    enum TEST
    {
        NONE = -1, EXPLORATE, RECALL, DICTATE
    };
    enum TEST_STRATEGY
    {
        STRATEGY0, STRATEGY1
    };

    explicit WordMemorizeWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void recoveryInterface();
    void reloadGlobalValue();
    void updateWordStatistics();
    int getMode();

private:
    void loadStyleSheet();
    void loadJsonRect();
    void setMode(int mode);
    void testListInit();
    void loadTestInfo();
    void clearWordInfo();
    void setWordInfo(bool isShield);
    void setViewPosition();
    void setViewPosition_14();
    void setViewPosition_15();
    void showWordInfo(bool showWord = true);
    void chooseTestWord();
    void chooseStrategy_level0();
    void chooseStrategy_level1();

signals:
    void wordTimeIncreaseSignal(QString name);

private slots:
    void slot_radioButtonClicked();
    void slot_btnStart_Clicked();
    void slot_btnKnow_Clicked();
    void slot_btnNotKnow_Clicked();
    void slot_btnForever_Clicked();
    void slot_btnNotForever_Clicked();
    void slot_btnNext_Clicked();
    void slot_btnSubmit_Clicked();
    void slot_wordCanMemorize(QString name);
    void slot_wordTimeDecline(QString name);
    void slot_stopWordMemorize(bool *ret);

private:
    int m_mode;
    int m_test;
    int m_propertyNum;
    int m_exampleNum;
    int m_synonymNum;
    int m_antonymNum;
    int m_testModeNum;
    int m_testNum;
    int m_pastNum;
    int m_curIndex;
    QString m_lastWord;
    int m_spacing;

    QList<WordTest> m_testList;
    QStringList m_nameList;
    WordInfo m_word;

    QMutex m_mutex;

    QRect rect_explorate_left;
    QRect rect_explorate_right;
    QRect rect_explorate_center;
    QRect rect_recall_left;
    QRect rect_recall_right;
    QRect rect_recall_center;

    QLabel *label_bg_welcome;
    QLabel *label_bg_start;
    QLabel *label_statistics;
    QLabel *label_info;
    QLabel *label_cha;
    QLabel *label_gou;
    QLabel *label_input;
    QLabel *label_line;

    QRadioButton *radioBtn_test[2];
    QRadioButton *radioBtn_range[4];
    QRadioButton *radioBtn_forever;

    QPushButton *btn_start;
    QPushButton *btn_know;
    QPushButton *btn_notKnow;
    QPushButton *btn_forever;
    QPushButton *btn_notforever;
    QPushButton *btn_next;
    QPushButton *btn_submit;

    QScrollArea *scrollArea;
    QWidget *widget_start;

    CopyLabel *copyLabel_WORD;
    CopyLabel *copyLabel_word;
    CopyLabel *copyLabel_phoneticSymbol;
    CopyLabel *copyLabel_property[20];
    CopyLabel *copyLabel_explain[20];
    CopyLabel *copyLabel_exampleSentence;
    CopyLabel *copyLabel_example[6];
    CopyLabel *copyLabel_synonym;
    CopyLabel *copyLabel_antonym;

    LinkLabel *linkLabel_synonym[8];
    LinkLabel *linkLabel_antonym[8];

    QLineEdit *lineEdit_input;
};

#endif // WORDMEMORIZEWIDGET_H
