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
#include <QComboBox>

namespace Ui
{
    class WordCreateWidget;
}

class WordCreateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordCreateWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void setEdit(bool edit);
    bool loadWordInfo(QString name);

private:
    QString inputCheck();

private slots:
    void slot_btnCancel_clicked();
    void slot_btnConfirm_clicked();

signals:
    void sendMessageSignal(WMessage message);
    void wordTimeIncreaseSignal(QString name);

private:
    Ui::WordCreateWidget *ui;
    bool m_reloadFlag;
    bool m_edit;
    int m_times;
    int m_remember;
    QStringList m_groupList;
    QVector<QTextEdit *> textEdit_exampleSentence;
};

#endif // WORDCREATEWIDGET_H
