#ifndef WORDSETTINGWIDGET_H
#define WORDSETTINGWIDGET_H

#include <QWidget>

class WordSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordSettingWidget(QWidget *parent = 0);
    void recoveryInterface();

private:
    void loadStyleSheet();

};

#endif // WORDSETTINGWIDGET_H
