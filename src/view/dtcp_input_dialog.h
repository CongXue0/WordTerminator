#pragma once
#include <QDialog>
#include <QLayout>
#include <QLineEdit>

namespace dtcp
{
    class InputDialog : public QDialog
    {
        Q_OBJECT
    public:
        enum InputDialogOption {
            UserCustomize, QuickInput
        };
        enum InputType {
            LineEdit, ComboBox, RadioButton, CheckBox, TimeEdit, DateEdit, DateTimeEdit, SpinBox, DoubleSpinBox, PushButton
        };
        struct LinePair {
            QString label;
            QVector<InputType> inputs;
        };

        explicit InputDialog(InputDialog::InputDialogOption option);
        void initiate();

        static QString getText(const QString &title, const QString &label, QLineEdit::EchoMode echo = QLineEdit::Normal, const QString &text = QString(), bool *ok = nullptr);

        void addLine(const InputDialog::LinePair &line);
        QWidget *getWidget(int row, int col, int *type = nullptr);

        void setQuickInputLabel(QString label);
        QString getQuickText();
        QLineEdit *getQuickLineEdit();

        void setOkText(QString text);
        void setCancelText(QString text);

        void set_accpet_callback(std::function<bool(void)> func) { accpet_callback_ = func; }

    private:
        QWidget *createWidget(InputDialog::InputType type, QWidget *parent);
        void setOkCancelButton(QVBoxLayout *mainLayout);
        void moveToCenter();
        void initiateUserCustomize();
        void initiateQuickInput();

    private slots:
        void slot_pushButtonOk_clicked();
        void slot_pushButtonCancel_clicked();

    private:
        InputDialogOption option_;
        QVector<LinePair> content_;
        QVector<QVector<QWidget *>> inputWidgets_;

        QLineEdit *quickLineEdit_;

        QString quickLabel_;
        QString okText_;
        QString cancelText_;

        std::function<bool(void)> accpet_callback_;
    };
}
