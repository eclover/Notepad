#include "ReplaceDialog.h"


ReplaceDialog::ReplaceDialog(QWidget* parent,MainEditor* pText)
    :FindDialog(parent,pText)
{
    initControls();
    connectSlots();
}

void ReplaceDialog::initControls()
{
    this->setWindowTitle(tr("Replace"));
    m_replaceLbl.setText(tr("ReplaceTo:"));
    m_replaceBtn.setText(tr("Replace"));
    m_replaceAllBtn.setText("ReplaceAll");

    m_layout.removeWidget(&m_matchChkBx);
    m_layout.removeWidget(&m_radioGrpBx);
    m_layout.removeWidget(&m_closeBtn);

    m_layout.addWidget(&m_replaceLbl,1,0);
    m_layout.addWidget(&m_replaceEdit,1,1);
    m_layout.addWidget(&m_replaceBtn,1,2);
    m_layout.addWidget(&m_matchChkBx,2,0);
    m_layout.addWidget(&m_radioGrpBx,2,1);
    m_layout.addWidget(&m_replaceAllBtn,2,2);
    m_layout.addWidget(&m_closeBtn,3,2);
}

void ReplaceDialog::connectSlots()
{
    connect(&m_replaceBtn,&QPushButton::clicked,this,&ReplaceDialog::onReplaceClicked);
    connect(&m_replaceAllBtn,&QPushButton::clicked,this,&ReplaceDialog::onReplaceAllClicked);
}

void ReplaceDialog::onReplaceClicked()
{
    QString target = m_findEdit.text();
    QString replaceTxt = m_replaceEdit.text();
    if(m_pText != NULL && target != "")
    {
        QString selTxt = m_pText->textCursor().selectedText();
        if(selTxt == target)
        {
            m_pText->insertPlainText(replaceTxt);
        }
        onFindClicked();
    }
}

void ReplaceDialog::onReplaceAllClicked()
{
    QString target = m_findEdit.text();
    QString replaceTxt = m_replaceEdit.text();
    if(m_pText != NULL && target != "")
    {
        QString text = m_pText->toPlainText();
        text.replace(target,replaceTxt,m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        m_pText->setPlainText(text);
    }
}
