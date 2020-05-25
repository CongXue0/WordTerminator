#ifndef WORDTERMINATOR_H
#define WORDTERMINATOR_H

#include "wmessage.h"
#include <QDialog>
#include <QVector>

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

    int getCurrentWidgetIndex();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void closeEvent(QCloseEvent *event);

    int topWidgetIndex();
    void pushWidgetIndex(int index);
    void popWidgetIndex();
    void clearWidgetIndex();

protected slots:
    void slot_saveBtn_clicked();
    void slot_wtbuttonPressed();
    void slot_handleMessage(WMessage message);

signals:
    void stopWordMemorizeSignal(bool *ret);

protected:
    Ui::WordTerminator *ui;
    bool m_first;

    QVector<int> m_widgets;
};

#endif // WORDTERMINATOR_H
