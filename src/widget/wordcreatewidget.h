#ifndef WORDCREATEWIDGET_H
#define WORDCREATEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include "wmessage.h"
#include <QMutex>

class WordCreateWidget : public QWidget
{
    Q_OBJECT
public:
    enum CREATE_MODE
    {
        CREATE, MODIFY
    };

    explicit WordCreateWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void recoveryInterface();
    void setCreateMode(CREATE_MODE mode);
    bool loadWordInfo(QString name);

public slots:

private:
    void loadStyleSheet();
    QString inputCheck();

signals:
    void sendMessageSignal(WMessage message);
    void wordTimeIncreaseSignal(QString name);

private slots:
    void slot_btnCancel_clicked();
    void slot_btnConfirm_clicked();

private:
    int m_mode;
    int m_times;
    int m_remember;

    QScrollArea *scrollArea;
    QWidget *widget;

    QLabel *label_word;
    QLabel *label_phoneticSymbol;
    QLabel *label_adj_Chinese;
    QLabel *label_adj_English;
    QLabel *label_adv_Chinese;
    QLabel *label_adv_English;
    QLabel *label_vt_Chinese;
    QLabel *label_vt_English;
    QLabel *label_vi_Chinese;
    QLabel *label_vi_English;
    QLabel *label_v_pastTense;
    QLabel *label_v_pastParticiple;
    QLabel *label_v_presentParticiple;
    QLabel *label_v_thirdPersonSingular;
    QLabel *label_noun_Chinese;
    QLabel *label_noun_English;
    QLabel *label_prep_Chinese;
    QLabel *label_prep_English;
    QLabel *label_conj_Chinese;
    QLabel *label_conj_English;
    QLabel *label_pron_Chinese;
    QLabel *label_pron_English;
    QLabel *label_art_Chinese;
    QLabel *label_art_English;
    QLabel *label_exampleSentence;
    QLabel *label_synonym;
    QLabel *label_antonym;

    QLineEdit *lineEdit_word;
    QLineEdit *lineEdit_phoneticSymbol;
    QLineEdit *lineEdit_adj_Chinese;
    QLineEdit *lineEdit_adj_English;
    QLineEdit *lineEdit_adv_Chinese;
    QLineEdit *lineEdit_adv_English;
    QLineEdit *lineEdit_vt_Chinese;
    QLineEdit *lineEdit_vt_English;
    QLineEdit *lineEdit_vi_Chinese;
    QLineEdit *lineEdit_vi_English;
    QLineEdit *lineEdit_v_pastTense;
    QLineEdit *lineEdit_v_pastParticiple;
    QLineEdit *lineEdit_v_presentParticiple;
    QLineEdit *lineEdit_v_thirdPersonSingular;
    QLineEdit *lineEdit_noun_Chinese;
    QLineEdit *lineEdit_noun_English;
    QLineEdit *lineEdit_prep_Chinese;
    QLineEdit *lineEdit_prep_English;
    QLineEdit *lineEdit_conj_Chinese;
    QLineEdit *lineEdit_conj_English;
    QLineEdit *lineEdit_pron_Chinese;
    QLineEdit *lineEdit_pron_English;
    QLineEdit *lineEdit_art_Chinese;
    QLineEdit *lineEdit_art_English;
    QLineEdit *lineEdit_synonym;
    QLineEdit *lineEdit_antonym;

    QCheckBox *checkBox_phrase;

    QTextEdit *textEdit_exampleSentence[6];

    QPushButton *btn_cancel;
    QPushButton *btn_confirm;
};

#endif // WORDCREATEWIDGET_H
