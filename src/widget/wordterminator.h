#ifndef WORDTERMINATOR_H
#define WORDTERMINATOR_H

#include "wmessage.h"
#include "dtcp_tool_button.h"
#include <QDialog>
#include <QVector>
#include <QStack>

namespace Ui
{
    class WordTerminator;
}

class WordTerminator : public QDialog
{
    Q_OBJECT
public:
    enum VIEW {
        Widget_WordLibrary, Widget_WordCreate, Widget_WordShow, Widget_AutomatedMemorize,
        Widget_WordMemorize, Widget_Function, Widget_Setting
    };
    explicit WordTerminator(QWidget *parent = nullptr);
    ~WordTerminator();
    static WordTerminator *instance() { return m_self; }

    int getCurrentWidgetIndex();

protected:
    virtual void showEvent(QShowEvent *) override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

    int topWidgetIndex();
    void pushWidgetIndex(int index);
    void popWidgetIndex();
    void clearWidgetIndex();

protected slots:
    void slot_saveBtn_clicked();
    void slot_switchButtonClicked();
    void slot_handleMessage(WMessage message);

protected:
    static WordTerminator *m_self;
    Ui::WordTerminator *ui;
    bool m_first;

    QVector<int> m_widgets;
    QPushButton *m_btn_group[5];
    QStack<QString> m_showWordStack;
};

#endif // WORDTERMINATOR_H
