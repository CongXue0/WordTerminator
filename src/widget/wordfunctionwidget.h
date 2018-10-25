#ifndef WORDFUNCTIONWIDGET_H
#define WORDFUNCTIONWIDGET_H

#include <QWidget>

class WordFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordFunctionWidget(QWidget *parent = 0);
    void recoveryInterface();

private:
    void loadStyleSheet();

};

#endif // WORDFUNCTIONWIDGET_H
