#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPointer>

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = 0,QPlainTextEdit *pText = 0);

    QPlainTextEdit* getPlainTextEdit();

protected slots:
    void onFindClicked();

protected:
    QLabel m_findLbl;
    QLineEdit m_findEdit;
    QPushButton m_findBtn;
    QPushButton m_closeBtn;
    QCheckBox m_matchChkBx;
    QRadioButton m_forwardBtn;
    QRadioButton m_backwardBtn;
    QGroupBox m_radioGrpBx;

    QGridLayout m_layout;
    QHBoxLayout m_hbLayout;
    QPointer<QPlainTextEdit> m_pText;

    void initControls();
    void connectSlots();
    bool event(QEvent *event);
    void setPlainTextEdit(QPlainTextEdit* pText);
};

#endif // FINDDIALOG_H
