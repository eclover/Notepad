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
#include "Pub.h"

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



bool MainWindow::preEditorChange(MainEditor* edit,QString tabTitle)
{
    bool ret = true;
    if(edit->isTextChanged())
    {
        int r = -1;
        if(edit->filePath() == "")
        {
            r = showQueryMessage(tr("Do you want to save the changes to the file \"%1\"?").arg(tabTitle));
        }
        else
        {
            r = showQueryMessage(tr("Do you want to save the changes to the file \"%1\"?").arg(edit->filePath()));
        }

        switch(r)
        {
        case QMessageBox::Save:
            saveCurrentFile(edit);
            break;
        case QMessageBox::No:
          //  edit->setTextChanged(false);
            break;
        case QMessageBox::Cancel:
            ret = false;
            break;
        }
    }
    return ret;
}



void MainWindow::openFileToTabWidget(QString path)
{
    MainEditor* edit = NULL;
    if(createEdit(edit))
    {
        edit->openFile(path);
        QFileInfo fi(path);
        addNewTab(edit,fi.fileName());
        edit->setTitle("[" + path +"] - eNotePad");
        this->setWindowTitle(edit->title());
    }
    else
    {
         showErrorMessage(tr("Open file failed!\n\n")+"\""+path+"\"");
    }
}

bool MainWindow::saveCurrentFile(MainEditor* edit)
{
    bool ret = true;

    QString path = edit->filePath();
    if(path == "")
    {
        path = showFileDialog(QFileDialog::AcceptSave,tr("Save"));

    }
    if(path != "")
    {
        if(edit->saveFile(path))
        {
            this->setWindowTitle(edit->title());
            QFileInfo fi(path);
            m_tabWidget.setTabText(m_tabWidget.currentIndex(),fi.fileName());
        }
        else
        {
            ret = false;
            showErrorMessage(tr("Save File Error\n\n")+"\""+ret+"\"");
        }
    }
    return ret;
}



void MainWindow::onFileOpen()
{
    //preEditorChange();

    QString path = showFileDialog(QFileDialog::AcceptOpen,tr("Open"));
    if(path != "")
    {
        if(mainEdit->toPlainText() == "" && !mainEdit->isTextChanged() )
        {
            onTabCloseRequested(m_tabWidget.currentIndex());
        }
        openFileToTabWidget(path);
        onCursorPositionChanged();
    }


}

void MainWindow::onFileSave()
{
    saveCurrentFile(mainEdit);

}

void MainWindow::onFileSaveAs()
{
    QString path = showFileDialog(QFileDialog::AcceptSave,tr("Save As"));

    if(path != "")
    {
        mainEdit->setFilePath(path);
        saveCurrentFile(mainEdit);
    }

}

void MainWindow::onFileSaveAll()
{
    for(int i=0;i < m_tabWidget.count();i++)
    {
        MainEditor* edit = dynamic_cast<MainEditor*>(m_tabWidget.widget(i));
        if(edit != NULL)
        {
            preEditorChange(edit,m_tabWidget.tabText(i));
        }
    }
}

void MainWindow::onFileNew()
{
/*    preEditorChange();

    if(!m_isTextChanged)
    {
        mainEdit->clear();
        this->setWindowTitle("NotePad - [ New ]");
        m_filePath = "";
        m_isTextChanged = false;
    }*/
    MainEditor* edit = NULL;
    if(createEdit(edit))
    {

        addNewTab(edit,"New");
        onCursorPositionChanged();
        this->setWindowTitle(edit->title());
    }
}

void MainWindow::onTextChanged()
{
    this->setWindowTitle(mainEdit->title());
 /*   if(!mainEdit->isTextChanged())
    {
        mainEdit->setTitle("*" + mainEdit->title());
        this->setWindowTitle(mainEdit->title());
    }
    mainEdit->setTextChanged(true);*/
}

void MainWindow::closeEvent(QCloseEvent* e)
{
//    preEditorChange();
 /*   if(!m_isTextChanged)
    {
        bool autoWrap = (mainEdit->lineWrapMode()==MainEditor::WidgetWidth);
        AppConfig config(this->size(),this->pos(),mainEdit->font(),toolBar()->isVisible(),statusBar()->isVisible(),autoWrap);
        config.write();
        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }*/

    for(int i=m_tabWidget.count()-1;i>=0;i--)
    {
        closeTab(i);
    }

    if(m_tabWidget.count() == 0)
    {
        bool autoWrap = (mainEdit->lineWrapMode()==MainEditor::WidgetWidth);
        AppConfig config(this->size(),this->pos(),mainEdit->font(),toolBar()->isVisible(),statusBar()->isVisible(),autoWrap);
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

        for(int i=0;i<urls.length();i++)
        {
            QString path = urls.at(i).toLocalFile();
            QFileInfo fi(path);
            if(fi.isFile())
            {
                //preEditorChange();
                if(!m_isTextChanged)
                {
                    openFileToTabWidget(path);
                }

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

void MainWindow::updateMainEditor(MainEditor *edit)
{
    mainEdit = edit;
    m_pFindDlg->setPlainTextEdit(edit);
    m_pReplaceDlg->setPlainTextEdit(edit);
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
        mainEdit->document()->print(p);
    }


}

void MainWindow::onCursorPositionChanged()
{
    int pos = mainEdit->textCursor().position();
    QString text = mainEdit->toPlainText();
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

    QApplication::sendEvent(mainEdit,&keyPress);
    QApplication::sendEvent(mainEdit,&keyRelease);
}

void MainWindow::onSearchFind()
{
    m_pFindDlg->setPlainTextEdit(mainEdit);
    m_pFindDlg->show();
}

void MainWindow::onSearchReplace()
{
    m_pReplaceDlg->setPlainTextEdit(mainEdit);
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
    if(mainEdit->lineWrapMode() == MainEditor::WidgetWidth)
    {
        mainEdit->setLineWrapMode(MainEditor::NoWrap);
        findToolBarAction("AutoWrap")->setChecked(false);
        findMenuBarAction("AutoWrap")->setChecked(false);
    }
    else
    {
        mainEdit->setLineWrapMode(MainEditor::WidgetWidth);
        findToolBarAction("AutoWrap")->setChecked(true);
        findMenuBarAction("AutoWrap")->setChecked(true);

    }
}

void MainWindow::onFormatFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mainEdit->font(),this,tr("Font"));
    mainEdit->setFont(font);
}

void MainWindow::onSearchGoto()
{
    bool ok = false;
    int line = QInputDialog::getInt(this,tr("Goto"),tr("Goto:"),1,1,mainEdit->document()->lineCount(),1,&ok,Qt::WindowCloseButtonHint | Qt::Drawer);
    if(ok)
    {
        QString text = mainEdit->toPlainText();
        QTextCursor c = mainEdit->textCursor();
        int pos = 0;
        int next = -1;
        for(int i=0;i<line;i++)
        {
            pos = next + 1;
            next = text.indexOf('\n',pos);
        }
        c.setPosition(pos);
        mainEdit->setTextCursor(c);
    }
}

void MainWindow::onEditSetReadOnly()
{
    bool readOnly = mainEdit->isReadOnly();

    mainEdit->setReadOnly(!readOnly);
    findMenuBarAction("Set ReadOnly")->setChecked(!readOnly);
}


void MainWindow::closeTab(int index)
{
    MainEditor* edit = dynamic_cast<MainEditor*>(m_tabWidget.widget(index));
    if(index > -1)
    {
        if(preEditorChange(edit,m_tabWidget.tabText(index)))
        {
            m_tabWidget.removeTab(index);
            if(m_tabWidget.count() > 0)
            {
                edit = dynamic_cast<MainEditor*>(m_tabWidget.currentWidget());

                onCursorPositionChanged();
                this->setWindowTitle(edit->title());
            }
        }
    }
}

void MainWindow::onTabCloseRequested(int index)
{
    closeTab(index);
    if(m_tabWidget.count() == 0)
    {
        onFileNew();
    }

}

void MainWindow::onCurrentTabChanged(int index)
{
    if(index > -1)
    {
        MainEditor* edit = dynamic_cast<MainEditor*>(m_tabWidget.widget(index));
        if(edit != NULL)
        {
            updateMainEditor(edit);
            QTextDocument* doc = edit->document();
            bool redoAvailable = doc->isRedoAvailable();
            bool undoAvailable = doc->isUndoAvailable();
            onRedoAvailable(redoAvailable);
            onUndoAvailable(undoAvailable);
        }
        this->setWindowTitle(mainEdit->title());
        onCursorPositionChanged();



    }
}

void MainWindow::onCopy()
{
    mainEdit->copy();
}

void MainWindow::onCut()
{
    mainEdit->cut();
}


void MainWindow::onPaste()
{
    mainEdit->paste();
}

void MainWindow::onSelectAll()
{
    mainEdit->selectAll();
}

void MainWindow::onFileCloseAll()
{
    QCloseEvent e;
    QApplication::sendEvent(this,&e);
}

void MainWindow::onFileClose()
{
    int index = m_tabWidget.currentIndex();
    closeTab(index);
}

void MainWindow::onUndo()
{
    mainEdit->undo();
}

void MainWindow::onRedo()
{
    mainEdit->redo();
}


void MainWindow::onFileExit()
{
    close();
}
