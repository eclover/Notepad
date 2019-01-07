#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QObjectList>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <AboutDialog.h>
#include <QFontDialog>
#include "AppConfig.h"
#include <QInputDialog>
#include <QTextStream>
#include <QTextCodec>

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode,QString title)
{
    QString ret = "";
    QFileDialog fd(this);
    fd.setAcceptMode(mode);
    fd.setWindowTitle(title);

    QStringList filters;
    QMap<QString,QString>map;
    const char* filterArray[][2] =
    {
        {"Text Files(*.txt)",".txt"},
        {"All Files(*.*)","*"},
        {NULL,NULL}
    };
    for(int i=0;filterArray[i][0]!=NULL;i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0],filterArray[i][1]);
    }
    fd.setNameFilters(filters);

    if(mode == QFileDialog::AcceptOpen)
    {
        fd.setFileMode(QFileDialog::ExistingFiles);
    }

    if(fd.exec() == QFileDialog::Accepted)
    {
        ret = fd.selectedFiles()[0];
        if(mode == QFileDialog::AcceptSave)
        {
            QString postfix = map[fd.selectedNameFilter()];
            if(postfix != "*" && !ret.endsWith(postfix))
            {
                ret += postfix;
            }
        }
    }
    return ret;
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);
    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

int MainWindow::showQueryMessage(QString message)
{
    int ret = -1;
    QMessageBox msg(this);
    msg.setWindowTitle("Query");
    msg.setIcon(QMessageBox::Question);
    msg.setText(message);
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel );

    ret = msg.exec();
    return ret;
}

void MainWindow::preEditorChange()
{
    if(m_isTextChanged)
    {
        int ret = showQueryMessage(tr("Do you want to save the changes to the file?"));

        switch(ret)
        {
        case QMessageBox::Save:
            saveCurrentFile(m_filePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}

void MainWindow::openFileToEditor(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextCodec* codec = QTextCodec::codecForName("GBK");
        mainEditor.setPlainText(codec->toUnicode(file.readAll()));
        file.close();
        m_isTextChanged = false;
        m_filePath = path;
        this->setWindowTitle("NotePad - ["+path+"]");

    }
    else
    {
        this->showErrorMessage(tr("Open file failed!\n\n")+"\""+path+"\"");
    }
}

QString MainWindow::saveCurrentFile(QString path)
{
    QString ret = path;
    if(path == "")
    {
        path = showFileDialog(QFileDialog::AcceptSave,tr("Save"));
    }
    if(path != "")
    {
        QFile file(path);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out<<mainEditor.toPlainText();

            m_isTextChanged = false;
            file.close();
            this->setWindowTitle("NotePad - ["+path+"]");
        }
        else
        {
            this->showErrorMessage(tr("Save File Error\n\n")+"\""+path+"\"");
            ret = "";
        }

    }
    return ret;
}



void MainWindow::onFileOpen()
{
    preEditorChange();
    if(!m_isTextChanged)
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen,tr("Open"));
        if(path != "")
        {
           openFileToEditor(path);
        }
    }

}

void MainWindow::onFileSave()
{
    if(m_filePath == "")
    {
        m_filePath = saveCurrentFile(m_filePath);
    }


}

void MainWindow::onFileSaveAs()
{
    QString path = showFileDialog(QFileDialog::AcceptSave,tr("Save As"));

    if(path != "")
    {

        m_filePath = saveCurrentFile(path);
    }

}

void MainWindow::onNewFile()
{
    preEditorChange();

    if(!m_isTextChanged)
    {
        mainEditor.clear();
        this->setWindowTitle("NotePad - [ New ]");
        m_filePath = "";
        m_isTextChanged = false;
    }
}

void MainWindow::onTextChanged()
{
    if(!m_isTextChanged)
    {
        this->setWindowTitle("*" + this->windowTitle());
    }
    m_isTextChanged = true;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    preEditorChange();
    if(!m_isTextChanged)
    {
        bool autoWrap = (mainEditor.lineWrapMode()==QPlainTextEdit::WidgetWidth);
        AppConfig config(this->size(),this->pos(),mainEditor.font(),toolBar()->isVisible(),statusBar()->isVisible(),autoWrap);
        config.write();
        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    if(e->mimeData()->hasUrls())
    {
        QList<QUrl> urls = e->mimeData()->urls();

        QString path = urls.at(0).toLocalFile();
        QFileInfo fi(path);
        if(fi.isFile())
        {
            preEditorChange();
            if(!m_isTextChanged)
            {
                openFileToEditor(path);
            }

        }

    }
    else
    {
        e->ignore();
    }

}

QAction *MainWindow::findMenuBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = menuBar()->children();

    for(int i=0;i<list.count();i++)
    {
        QMenu* menu = dynamic_cast<QMenu*>(list[i]);
        if(menu !=  NULL)
        {

            QList<QAction*> actions = menu->actions();
            for(int j=0;j<actions.count();j++)
            {
                if(actions[j]->text().startsWith(text))
                {
                    ret = actions[j];
                    break;
                }
            }
        }
    }
    return ret;
}

QAction *MainWindow::findToolBarAction(QString text)
{
    QAction* ret = NULL;

    QList<QAction*> actions = toolBar()->actions();
    for(int j=0;j<actions.count();j++)
    {
        if(actions[j]->toolTip().startsWith(text))
        {
            ret = actions[j];
            break;
        }
    }


    return ret;
}

QToolBar *MainWindow::toolBar()
{
    QToolBar* ret = NULL;
    const QObjectList& list = children();
    for(int i=0;i<list.count();i++)
    {
        QToolBar* tb = dynamic_cast<QToolBar*>(list[i]);
        if(tb != NULL)
        {
            ret = tb;
            break;
        }
    }
    return ret;
}


void MainWindow::onCopyAvailable(bool available)
{
    findMenuBarAction("Copy")->setEnabled(available);
    findMenuBarAction("Cut")->setEnabled(available);
    findToolBarAction("Copy")->setEnabled(available);
    findToolBarAction("Cut")->setEnabled(available);
}

void MainWindow::onRedoAvailable(bool available)
{
    findMenuBarAction("Redo")->setEnabled(available);
    findToolBarAction("Redo")->setEnabled(available);
}

void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction("Undo")->setEnabled(available);
    findToolBarAction("Undo")->setEnabled(available);
}

void MainWindow::onFilePrint()
{
    QPrintDialog dlg(this);
    dlg.setWindowTitle(tr("Print"));

    if(dlg.exec() == QDialog::Accepted)
    {
        QPrinter* p = dlg.printer();
        mainEditor.document()->print(p);
    }


}

void MainWindow::onCursorPositionChanged()
{
    int pos = mainEditor.textCursor().position();
    QString text = mainEditor.toPlainText();
    int ln = 0 ,col=0;
    int flag = -1;
    for(int i=0;i<pos;i++)
    {
        if(text[i] == '\n')
        {

            ln++;
            flag = i;
        }
    }
    flag++;
    col = pos - flag;
    this->statusLbl.setText(QString("Line:%1    Col:%2").arg(ln+1).arg(col+1));

}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier);
    QKeyEvent keyRelease(QEvent::KeyRelease,Qt::Key_Delete,Qt::NoModifier);

    QApplication::sendEvent(&mainEditor,&keyPress);
    QApplication::sendEvent(&mainEditor,&keyRelease);
}

void MainWindow::onSearchFind()
{
    m_pFindDlg->show();
}

void MainWindow::onSearchReplace()
{
    m_pReplaceDlg->show();
}

void MainWindow::onViewStatusBar()
{
    QStatusBar* sb = statusBar();
    bool visible = sb->isVisible();

    sb->setVisible(!visible);

    findMenuBarAction("StatusBar")->setChecked(!visible);
    findToolBarAction("StatusBar")->setChecked(!visible);

}

void MainWindow::onViewToolBar()
{
    const QObjectList& list = this->children();
    QToolBar* tb ;
    for(int i=0;i<list.count();i++)
    {
        tb = dynamic_cast<QToolBar*>(list[i]);
        if(tb != NULL)
        {
            break;
        }
    }
    bool visible = tb->isVisible();
    tb->setVisible(!visible);
    findToolBarAction("ToolBar")->setChecked(!visible);
    findMenuBarAction("ToolBar")->setChecked(!visible);
}

void MainWindow::onHelpAbout()
{
    AboutDialog(this).exec();

}

void MainWindow::onFormatAutoWrap()
{
    if(mainEditor.lineWrapMode() == QPlainTextEdit::WidgetWidth)
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);
        findToolBarAction("AutoWrap")->setChecked(false);
        findMenuBarAction("AutoWrap")->setChecked(false);
    }
    else
    {
        mainEditor.setLineWrapMode(QPlainTextEdit::WidgetWidth);
        findToolBarAction("AutoWrap")->setChecked(true);
        findMenuBarAction("AutoWrap")->setChecked(true);

    }
}

void MainWindow::onFormatFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mainEditor.font(),this,tr("Font"));
    mainEditor.setFont(font);
}

void MainWindow::onSearchGoto()
{
    bool ok = false;
    int line = QInputDialog::getInt(this,tr("Goto"),tr("Goto:"),1,1,mainEditor.document()->lineCount(),1,&ok,Qt::WindowCloseButtonHint | Qt::Drawer);
    if(ok)
    {
        QString text = mainEditor.toPlainText();
        QTextCursor c = mainEditor.textCursor();
        int pos = 0;
        int next = -1;
        for(int i=0;i<line;i++)
        {
            pos = next + 1;
            next = text.indexOf('\n',pos);
        }
        c.setPosition(pos);
        mainEditor.setTextCursor(c);
    }
}

void MainWindow::onEditSetReadOnly()
{
    bool readOnly = mainEditor.isReadOnly();

    mainEditor.setReadOnly(!readOnly);
    findMenuBarAction("Set ReadOnly")->setChecked(!readOnly);
}

void MainWindow::onFileExit()
{
    close();
}
