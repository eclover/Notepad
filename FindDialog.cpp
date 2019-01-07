#include "FindDialog.h"
#include <QEvent>
#include <QMessageBox>
#include <QDebug>

FindDialog::FindDialog(QWidget *parent,QPlainTextEdit *pText )
    :QDialog(parent,Qt::WindowCloseButtonHint | Qt::Drawer)
{
    initControls();
    connectSlots();
    setPlainTextEdit(pText);
}

void FindDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;
}

QPlainTextEdit *FindDialog::getPlainTextEdit()
{
    return m_pText;
}

void FindDialog::onFindClicked()
{
    QString findText = m_findEdit.text();
    int index = -1;
    if(m_pText != NULL && findText != "")
    {
        QString text = m_pText->toPlainText();
        QTextCursor c = m_pText->textCursor() ;
        if(m_backwardBtn.isChecked())
        {
            index = text.indexOf(findText,c.position(),m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
            if(index >= 0)
            {
                c.setPosition(index);
                c.setPosition(index+findText.length(),QTextCursor::KeepAnchor);
                m_pText->setTextCursor(c);
            }
        }

        if(m_forwardBtn.isChecked())
        {
            index = text.lastIndexOf(findText,c.position()-text.length()-1,m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);

            if(index >= 0)
            {
                c.setPosition(index+findText.length());
                c.setPosition(index,QTextCursor::KeepAnchor);
                m_pText->setTextCursor(c);
            }
        }

        if(index < 0)
        {
            QMessageBox msg(this);
            msg.setText(tr("Can not find \"%1\" anymore......").arg(findText));
            msg.setWindowTitle(tr("Error"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.exec();
        }

    }
}


void FindDialog::initControls()
{
    this->setWindowTitle(tr("Find"));
    m_findLbl.setText(tr("Find Text(N):"));
    m_forwardBtn.setText(tr("Forward(U)"));
    m_backwardBtn.setChecked(true);
    m_backwardBtn.setText(tr("Backward(D)"));
    m_radioGrpBx.setTitle(tr("Direction"));
    m_matchChkBx.setText(tr("Match Case(C)"));
    m_findBtn.setText(tr("Find Next(F)"));
    m_closeBtn.setText(tr("Close"));

    m_hbLayout.addWidget(&m_forwardBtn);
    m_hbLayout.addWidget(&m_backwardBtn);
    m_radioGrpBx.setLayout(&m_hbLayout);

    m_layout.addWidget(&m_findLbl,0,0);
    m_layout.addWidget(&m_findEdit,0,1);
    m_layout.addWidget(&m_findBtn,0,2);
    m_layout.addWidget(&m_matchChkBx,1,0);
    m_layout.addWidget(&m_radioGrpBx,1,1);
    m_layout.addWidget(&m_closeBtn,1,2);


    this->setLayout(&m_layout);

}

void FindDialog::connectSlots()
{
   connect(&m_closeBtn,&QPushButton::clicked,this,&FindDialog::close);
   connect(&m_findBtn,&QPushButton::clicked,this,&FindDialog::onFindClicked);
}

bool FindDialog::event(QEvent *event)
{
    if(event->type() ==  QEvent::Close)
    {
        hide();
        return true;
    }

    return QDialog::event(event);

}
