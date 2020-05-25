#ifndef WORDFUNCTIONWIDGET_H
#define WORDFUNCTIONWIDGET_H

#include <QWidget>

namespace Ui
{
    class WordFunctionWidget;
}

class WordFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordFunctionWidget(QWidget *parent = nullptr);
    void recoveryInterface();
    void reloadGlobalValue();
    void saveGlobalValue();

private:
    void exportWord(int t1, int t2, int group,int remember);

private slots:
    void slot_btnRefresh_clicked();
    void slot_btnExport_clicked();
    void slot_comboxScript_currentIndexChanged(int index);

private:
    Ui::WordFunctionWidget *ui;
};

#endif // WORDFUNCTIONWIDGET_H
