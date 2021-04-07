#include "dtcp_tool_tip.h"
#include "wtool.h"
#include <QVBoxLayout>
#include <QDebug>

QDialog *DtcpToolTip::m_dlg = nullptr;
QLabel *DtcpToolTip::m_label = nullptr;

void DtcpToolTip::showText(const QPoint &pos, const QString &text)
{
    if (!m_dlg)
    {
        m_dlg = new QDialog(WTool::topWidget_);
        m_dlg->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        m_dlg->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        QVBoxLayout *layout = new QVBoxLayout(m_dlg);
        layout->setMargin(0);
        layout->setSpacing(0);
        m_label = new QLabel(m_dlg);
        m_label->setStyleSheet(QString::fromUtf8(u8R"(
QLabel {
    color: #00677c;
    background-color: white;
    padding: 3px;
}
)"));
        layout->addWidget(m_label);
        m_label->show();
    }
    if (text.isEmpty())
    {
        m_dlg->hide();
    }
    else
    {
        QFontMetrics fm(m_label->font());
        QStringList list = text.split("\n");
        QString tmp;
        for (int i = 0; i < list.size(); ++i)
        {
            tmp.append(fm.elidedText(list[i], Qt::ElideRight, 800));
            if (i != list.size() - 1)
                tmp.append("\n");
        }
        m_label->setText(tmp);
        m_dlg->show();
        QSize size = m_label->sizeHint();
        QSize screenSize = WTool::getScreenGeometry().size();
        m_dlg->setFixedSize(size);
        if (pos.x() + 1 + size.width() > screenSize.width())
        {
            m_dlg->setGeometry(pos.x() - 1 - size.width(), pos.y() + 19, size.width(), size.height());
        }
        else
        {
            m_dlg->setGeometry(pos.x() + 1, pos.y() + 19, size.width(), size.height());
        }
    }
}

bool DtcpToolTip::isVisible()
{
    if (m_dlg)
        return m_dlg->isVisible();
    else
        return false;
}

QString DtcpToolTip::text()
{
    if (m_label)
        return m_label->text();
    else
        return "";
}




