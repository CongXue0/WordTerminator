#include "dtcp_input_dialog.h"
#include "wtool.h"
#include "dtcp_tool_button.h"
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QTimeEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QApplication>
#include <QDesktopWidget>

namespace dtcp
{
    InputDialog::InputDialog(InputDialog::InputDialogOption option) :
        QDialog(WTool::topWidget_)
    {
        setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

        option_ = option;

        quickLineEdit_ = nullptr;

        quickLabel_ = "Input your value:";
        okText_ = "Ok";
        cancelText_ = "Cancel";
    }

    void InputDialog::initiate()
    {
        switch (option_)
        {
        case UserCustomize:
            initiateUserCustomize();
            break;
        case QuickInput:
            initiateQuickInput();
            break;
        }
    }

    QString InputDialog::getText(const QString &title, const QString &label, QLineEdit::EchoMode echo, const QString &text, bool *ok)
    {
        InputDialog dlg(InputDialog::QuickInput);
        dlg.setWindowTitle(title);
        dlg.setQuickInputLabel(label);
        dlg.initiate();
        QDesktopWidget *desktop = QApplication::desktop();
        dlg.move((desktop->width() - dlg.width()) / 2, (desktop->height() - dlg.height()) / 2);
        QLineEdit* quickLine = dlg.getQuickLineEdit();
        quickLine->setEchoMode(echo);
        quickLine->setText(text);
        int ret = dlg.exec();
        if (ok)
            *ok = (ret == QDialog::Accepted);
        if (ret == QDialog::Accepted)
            return quickLine->text();
        else
            return "";
    }

    void InputDialog::addLine(const InputDialog::LinePair &line)
    {
        if (option_ != UserCustomize)
            return;
        content_.append(line);
    }

    QWidget *InputDialog::getWidget(int row, int col, int *type)
    {
        if (row < 0 || row > inputWidgets_.size() - 1)
            return nullptr;
        if (col < 0 || col > inputWidgets_[row].size() - 1)
            return nullptr;
        if (type)
        {
            if (QWidget *w = inputWidgets_[row][col])
                *type = w->property("type").toInt();
            else
                *type = -1;
        }
        return inputWidgets_[row][col];
    }

    void InputDialog::setQuickInputLabel(QString label)
    {
        if (option_ != QuickInput && !label.isEmpty())
            return;
        quickLabel_ = label;
    }

    QString InputDialog::getQuickText()
    {
        if (quickLineEdit_)
            return quickLineEdit_->text();
        else
            return "";
    }

    QLineEdit *InputDialog::getQuickLineEdit()
    {
        return quickLineEdit_;
    }

    void InputDialog::setOkText(QString text)
    {
        if (!text.isEmpty())
            okText_ = text;
    }

    void InputDialog::setCancelText(QString text)
    {
        if (!text.isEmpty())
            cancelText_ = text;
    }

    QWidget *InputDialog::createWidget(InputDialog::InputType type, QWidget *parent)
    {
        QWidget *input = nullptr;
        switch (type)
        {
        case LineEdit:
            input = new QLineEdit(parent);
            break;
        case ComboBox:
            input = new QComboBox(parent);
            break;
        case RadioButton:
            input = new QRadioButton(parent);
            break;
        case CheckBox:
            input = new QCheckBox(parent);
            break;
        case TimeEdit:
            input = new QTimeEdit(parent);
            break;
        case DateEdit:
            input = new QDateEdit(parent);
            break;
        case DateTimeEdit:
            input = new QDateTimeEdit(parent);
            break;
        case SpinBox:
            input = new QSpinBox(parent);
            break;
        case DoubleSpinBox:
            input = new QDoubleSpinBox(parent);
            break;
        case PushButton:
            input = new QPushButton(parent);
            break;
        }
        if (input)
        {
            QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
            sizePolicy1.setHorizontalStretch(0);
            sizePolicy1.setVerticalStretch(0);
            sizePolicy1.setHeightForWidth(input->sizePolicy().hasHeightForWidth());
            input->setSizePolicy(sizePolicy1);
            input->setProperty("type", (int)type);
        }
        return input;
    }

    void InputDialog::setOkCancelButton(QVBoxLayout *mainLayout)
    {
        QHBoxLayout *hLayout = new QHBoxLayout();
        DtcpTextButton *pushButton_ok = new DtcpTextButton(this);
        pushButton_ok->setText(okText_);
        DtcpTextButton *pushButton_cancel = new DtcpTextButton(this);
        pushButton_cancel->setText(cancelText_);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_ok->sizePolicy().hasHeightForWidth());
        pushButton_ok->setSizePolicy(sizePolicy2);
        pushButton_cancel->setSizePolicy(sizePolicy2);
        pushButton_ok->setMaximumSize(QSize(120, 16777215));
        pushButton_cancel->setMaximumSize(QSize(120, 16777215));
        hLayout->addWidget(pushButton_ok);
        hLayout->addWidget(pushButton_cancel);
        mainLayout->addLayout(hLayout);
        connect(pushButton_ok, SIGNAL(clicked()), this, SLOT(slot_pushButtonOk_clicked()));
        connect(pushButton_cancel, SIGNAL(clicked()), this, SLOT(slot_pushButtonCancel_clicked()));
    }

    void InputDialog::moveToCenter()
    {
        QDesktopWidget *deskdop = QApplication::desktop();
        this->move((deskdop->width() - this->width()) / 2, (deskdop->height() - this->height()) / 2);
    }

    void InputDialog::initiateUserCustomize()
    {
        if (inputWidgets_.size() > 0)
            return;

        int maxRow = 0, maxCol = 0;
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setMargin(9);
        mainLayout->setSpacing(6);
        for (auto &line : content_)
        {
            if (!line.label.isEmpty() || line.inputs.size() > 0)
            {
                inputWidgets_.append(QVector<QWidget *>{});
                QHBoxLayout *hLayout = new QHBoxLayout();
                hLayout->setMargin(0);
                hLayout->setSpacing(0);
                if (!line.label.isEmpty())
                {
                    QLabel *label = new QLabel(this);
                    label->setObjectName(QString("label_grey_%1").arg(maxRow));
                    label->setText(line.label);
                    label->setMinimumSize(QSize(150, 0));
                    hLayout->addWidget(label);
                }
                if (line.inputs.size() > 0)
                {
                    QWidget *widget = new QWidget(this);
                    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                    sizePolicy.setHorizontalStretch(0);
                    sizePolicy.setVerticalStretch(0);
                    sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
                    widget->setSizePolicy(sizePolicy);

                    QHBoxLayout *hLayout_2 = new QHBoxLayout(widget);
                    hLayout_2->setMargin(0);
                    hLayout_2->setSpacing(6);
                    for (auto &type : line.inputs)
                    {
                        if (QWidget *w = createWidget(type, widget))
                        {
                            hLayout_2->addWidget(w);
                            inputWidgets_[maxRow].append(w);
                        }
                    }
                    hLayout->addWidget(widget);
                    if (maxCol < line.inputs.size())
                        maxCol = line.inputs.size();
                }
                mainLayout->addLayout(hLayout);
                maxRow++;
            }
        }

        setOkCancelButton(mainLayout);
        maxRow++;

        if (maxRow > 1)
        {
            int width = 18 + 150 + 200;
            int height = 18 + 6 * (maxRow - 1) + 30 * maxRow;
            if (maxCol > 0)
            {
                width += 150 * (maxCol - 1);
            }
            this->setGeometry(this->x(), this->y(), width, height);
        }
        moveToCenter();
    }

    void InputDialog::initiateQuickInput()
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setMargin(9);
        mainLayout->setSpacing(6);

        QHBoxLayout *hLayout1 = new QHBoxLayout();
        hLayout1->setMargin(0);
        hLayout1->setSpacing(0);
        QLabel *label = new QLabel(this);
        label->setText(quickLabel_);
        label->setMinimumSize(QSize(150, 0));
        hLayout1->addWidget(label);
        mainLayout->addLayout(hLayout1);

        QHBoxLayout *hLayout2 = new QHBoxLayout();
        hLayout2->setMargin(0);
        hLayout2->setSpacing(0);
        quickLineEdit_ = dynamic_cast<QLineEdit *>(createWidget(LineEdit, this));
        hLayout2->addWidget(quickLineEdit_);
        mainLayout->addLayout(hLayout2);

        setOkCancelButton(mainLayout);

        int width = 18 + 150 + 200;
        int height = 18 + 6 * 2 + 30 * 3;
        this->setGeometry(this->x(), this->y(), width, height);
        moveToCenter();
    }

    void InputDialog::slot_pushButtonOk_clicked()
    {
        if (accpet_callback_ && !accpet_callback_())
        {
            return;
        }
        this->accept();
    }

    void InputDialog::slot_pushButtonCancel_clicked()
    {
        this->reject();
    }
}
