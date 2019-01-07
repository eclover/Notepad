#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT
    QLabel m_logoLbl;
    QPushButton m_closeBtn;
    QPlainTextEdit m_infoEditor;

protected:
    void initControls();

public:
    explicit AboutDialog(QWidget* parent=0);
};

#endif // ABOUTDIALOG_H
