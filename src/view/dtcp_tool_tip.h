#pragma once
#include <QDialog>
#include <QLabel>

class DtcpToolTip
{
public:
    DtcpToolTip() = delete;

    static void showText(const QPoint &pos, const QString &text);
    static inline void hideText() { showText(QPoint(), QString()); }
    static bool isVisible();
    static QString text();

private:
    static QDialog *m_dlg;
    static QLabel *m_label;
};
