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

class WordLibraryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordLibraryWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void recoveryInterface();
    void clearSearch();
    void updateWordList();
    void updateWordStatistics();

public slots:

private:
    void loadStyleSheet();

signals:
    void sendMessageSignal(WMessage message);

private slots:
    void slot_btnSearch_clicked();
    void slot_btnCreate_clicked();
    void slot_itemDoubleClicked(QModelIndex index);
    void slot_radioButtonClicked();
    void slot_wordTimeDecline(QString name);
    void slot_wordTimeIncrease(QString name);

private:
    QLabel *label_bg;
    QLabel *label_statistics;

    QLineEdit *lineEdit_search;

    QPushButton *btn_search;
    QPushButton *btn_create;

    QListView *wordList;
    QStringList m_wordList;
    QStringListModel *m_model;

    QRadioButton *radioBtn_range[4];
    QRadioButton *radioBtn_forever;

    QComboBox *combox_search;

};

#endif // WORDLIBRARYWIDGET_H
