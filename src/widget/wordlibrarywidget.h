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
#include <QMenu>

namespace Ui
{
    class WordLibraryWidget;
}

class WordLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordLibraryWidget(QWidget *parent = nullptr);
    ~WordLibraryWidget();
    void recoveryInterface();
    void reloadGlobalValue();
    void setReloadFlag(bool flag);
    void clearSearch();
    void updateWordList();
    void updateWordStatistics();

private:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void slot_menu1Triggered(QAction *act);
    void slot_menu2Triggered(QAction *act);
    void slot_btnSearch_clicked();
    void slot_btnCreate_clicked();
    void slot_itemDoubleClicked(QModelIndex index);
    void slot_checkBoxClicked();
    void slot_comboxGroup_currentIndexChanged(int index);
    void slot_wordTimeDecline(QString name);
    void slot_wordTimeIncrease(QString name);

private:
    Ui::WordLibraryWidget *ui;
    bool m_reloadFlag;
    int m_curGroupId;
    QModelIndexList m_modelList;

    QMenu *m_menu1;
    QMenu *m_menu2;

    QStringList m_wordList;
    QStringListModel *m_model;
};

#endif // WORDLIBRARYWIDGET_H
