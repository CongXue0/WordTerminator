#ifndef WORDCREATEWIDGET_H
#define WORDCREATEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMutex>
#include <QComboBox>
#include "wmessage.h"
#include "wordadmin.h"

namespace Ui
{
    class WordCreateWidget;
}

class WordCreateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordCreateWidget(QWidget *parent = nullptr);
    ~WordCreateWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void setEdit(bool edit);
    bool loadWordInfo(QString name);

private:
    void keyPressEvent(QKeyEvent *event);
    QString inputCheck();

private slots:
    void slot_wordCheck();
    void slot_btnCancel_clicked();
    void slot_btnConfirm_clicked();

private:
    Ui::WordCreateWidget *ui;
    bool m_reloadFlag;
    bool m_edit;
    int m_times;
    int m_remember;
    QStringList m_groupList;
    QTextEdit *textEdit_exampleSentence[EXAMPLE_NUM];
};

#endif // WORDCREATEWIDGET_H
