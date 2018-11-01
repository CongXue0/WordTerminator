#ifndef WORDLIBRARYWIDGET_H
#define WORDLIBRARYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QLineEdit>
#include <QRadioButton>
#include "wmessage.h"
#include <QComboBox>
#include "wtool.h"
#include "wtlistview.h"
#include <QMenu>

class WordLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordLibraryWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *e);
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void clearSearch();
    void updateWordList();
    void updateWordStatistics();

public slots:

private:
    void loadStyleSheet();

signals:
    void sendMessageSignal(WMessage message);
    void wordTimeIncreaseSignal(QString name);

private slots:
    void slot_menu1Triggered(QAction *act);
    void slot_menu2Triggered(QAction *act);
    void slot_btnSearch_clicked();
    void slot_btnCreate_clicked();
    void slot_itemDoubleClicked(QModelIndex index);
    void slot_radioButtonClicked();
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_wordTimeDecline(QString name);
    void slot_wordTimeIncrease(QString name);

private:
    bool m_reloadFlag;
    int m_curGroupId;
    QModelIndexList m_modelList;

    QMenu *m_menu1;
    QMenu *m_menu2;

    QLabel *label_bg;
    QLabel *label_statistics;

    QLineEdit *lineEdit_search;

    QPushButton *btn_search;
    QPushButton *btn_create;

    WTListView *wordList;
    QStringList m_wordList;
    QStringListModel *m_model;

    QRadioButton *radioBtn_range[4];
    QRadioButton *radioBtn_forever;

    QComboBox *combox_group;
    QComboBox *combox_search;

};

#endif // WORDLIBRARYWIDGET_H
