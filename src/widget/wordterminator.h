#ifndef WORDTERMINATOR_H
#define WORDTERMINATOR_H

#include <QDialog>
#include <QLabel>
#include "wtbutton.h"
#include <QStackedWidget>
#include "wordlibrarywidget.h"
#include "wordcreatewidget.h"
#include "wordshowwidget.h"
#include "wmessage.h"
#include <QVector>
#include "wordmemorizewidget.h"

namespace Ui
{
    class WordTerminator;
}

class WordTerminator : public QDialog
{
    Q_OBJECT

public:
    enum VIEW
    {
        Widget_WordLibrary, Widget_WordCreate, Widget_WordShow, WIdget_WordMemorize,
        Widget_Setting
    };

    explicit WordTerminator(QWidget *parent = 0);
    ~WordTerminator();
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *event);

    int getCurrentWidgetIndex();

private:
    void loadStyleSheet();
    int topWidgetIndex();
    void pushWidgetIndex(int index);
    void popWidgetIndex();
    void clearWidgetIndex();
    void exportWord(QString msg);

signals:
    void stopWordMemorizeSignal(bool *ret);

private slots:
    void slot_btnRefresh_pressed();
    void slot_btnExport_pressed();
    void slot_wtbuttonPressed();
    void slot_handleMessage(WMessage message);

private:
    Ui::WordTerminator *ui;
    bool m_first;

    QVector<int> m_widgets;

    QLabel *label_menubg;

    QPushButton *btn_refresh;
    QPushButton *btn_export;
    WTButton *btn_lib;
    WTButton *btn_mem;
    WTButton *btn_set;

    QStackedWidget *stackedWidget;

    WordLibraryWidget *wordLibrary;
    WordCreateWidget *wordCreate;
    WordShowWidget *wordShow;
    WordMemorizeWidget *wordMemorize;
};

#endif // WORDTERMINATOR_H
