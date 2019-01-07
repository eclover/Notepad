#include "AboutDialog.h"
#include <QPalette>
#include <QPixmap>

AboutDialog::AboutDialog(QWidget* parent)
    :QDialog(parent,Qt::WindowCloseButtonHint),
      m_logoLbl(this),m_closeBtn(this),m_infoEditor(this)
{
    initControls();
}

void AboutDialog::initControls()
{
    setWindowTitle(tr("About"));
    QPixmap pm(":/images/logo.png");
    pm.scaled(115,115,Qt::KeepAspectRatio);
    m_logoLbl.setPixmap(pm);
    m_logoLbl.resize(115,115);
    m_logoLbl.move(20,20);


    QPalette p = m_infoEditor.palette();
    p.setColor(QPalette::Active,QPalette::Base,p.color(QPalette::Active,QPalette::Background));
    p.setColor(QPalette::Inactive,QPalette::Base,p.color(QPalette::Inactive,QPalette::Background));

    m_infoEditor.setPalette(p);
    m_infoEditor.resize(150,130);
    m_infoEditor.move(165,20);
    m_infoEditor.setFrameStyle(QFrame::NoFrame);
    m_infoEditor.setReadOnly(true);
    m_infoEditor.setPlainText("NotePad Project\n\nPlatform: Qt 5.8\n\nVersion: 1.0.0\n\nCopyright: eClover");


    m_closeBtn.setText(tr("Close"));
    m_closeBtn.move(205,150);
    m_closeBtn.resize(80,30);
    connect(&m_closeBtn,&QPushButton::clicked,this,&AboutDialog::close);
}
