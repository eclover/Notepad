#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H


#include <FindDialog.h>


class ReplaceDialog : public FindDialog
{
    Q_OBJECT
    QLabel m_replaceLbl;
    QLineEdit m_replaceEdit;
    QPushButton m_replaceBtn;
    QPushButton m_replaceAllBtn;

protected:
    void initControls();
    void connectSlots();

protected slots:
    void onReplaceClicked();
    void onReplaceAllClicked();
public:
    explicit ReplaceDialog(QWidget *parent=0,QPlainTextEdit *pText=0);


};

#endif // REPLACEDIALOG_H
