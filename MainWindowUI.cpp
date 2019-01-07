#include "MainWindow.h"
#include <QMenu>
#include <QKeySequence>
#include <QIcon>
#include <QLabel>
#include "AppConfig.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pFindDlg(new FindDialog(this,&mainEditor)),
      m_pReplaceDlg(new ReplaceDialog(this,&mainEditor))
{
    this->setWindowTitle("eNotePad - [ New ]");
    this->setAcceptDrops(true);;
    this->setWindowIcon(QIcon(QPixmap(":/images/icon.png")));
    m_filePath = "";
    m_isTextChanged = false;
}

MainWindow::~MainWindow()
{

}

void MainWindow::openFile(QString path)
{
    preEditorChange();
    if(!m_isTextChanged)
    {
       openFileToEditor(path);

    }

}

bool MainWindow::construct()
{
    bool ret = true;
    AppConfig config;
    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEdit();

    if(config.isValid())
    {
        QFont font = config.editorFont();
        mainEditor.setFont(font);

        bool isToolBarVisible = config.isToolBarVisible();
        QToolBar* tb = toolBar();
        if(tb != NULL)
        {
            tb->setVisible(isToolBarVisible);
            findToolBarAction("ToolBar")->setChecked(isToolBarVisible);
            findMenuBarAction("ToolBar")->setChecked(isToolBarVisible);
        }

        bool isStatusBarVisible = config.isStatusBarVisible();
        statusBar()->setVisible(isStatusBarVisible);
        findToolBarAction("StatusBar")->setChecked(isToolBarVisible);
        findMenuBarAction("StatusBar")->setChecked(isToolBarVisible);


        bool isAutoWrap = config.isAutoWrap();
        if(isAutoWrap)
        {
            mainEditor.setLineWrapMode(QPlainTextEdit::WidgetWidth);
        }
        else
        {
            mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);
        }

        findMenuBarAction("AutoWrap")->setChecked(isToolBarVisible);
        resize(config.mainWindowSize());
        move(config.mainWindowPoint());


    }
    return ret;
}

MainWindow* MainWindow::NewInstance()
{
    MainWindow* ret = new MainWindow;
    if(ret == NULL || !ret->construct())
    {
        delete ret;
        ret = NULL;
    }
    ret->resize(600,400);
    return ret;
}

bool MainWindow::initMenuBar()
{
    bool ret = true;
    QMenuBar* mb = menuBar();
    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initSearchMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initHelpMenu(mb);

    return ret;
}


bool MainWindow::initToolBar()
{
    bool ret = true;
    QToolBar* tb = addToolBar(tr("Tool Bar"));
    tb->setMovable(false);
    tb->setIconSize(QSize(16,16));
    tb->setFloatable(false);
    ret = ret && initToolItem(tb);

    return ret;
}

bool MainWindow::initStatusBar()
{
    bool ret = true;
    QStatusBar* sb = statusBar();
    sb->setVisible(true);
    ret = (sb != NULL);
    if(ret)
    {
       ret = ret && initStatusItem(sb);
    }
    return ret;
}

bool MainWindow::initMainEdit()
{
    bool ret = true;
    mainEditor.setParent(this);
    mainEditor.setAcceptDrops(false);
    connect(&mainEditor,&QPlainTextEdit::textChanged,this,&MainWindow::onTextChanged);
    connect(&mainEditor,&QPlainTextEdit::copyAvailable,this,&MainWindow::onCopyAvailable);
    connect(&mainEditor,&QPlainTextEdit::undoAvailable,this,&MainWindow::onUndoAvailable);
    connect(&mainEditor,&QPlainTextEdit::redoAvailable,this,&MainWindow::onRedoAvailable);
    connect(&mainEditor,&QPlainTextEdit::cursorPositionChanged,this,&MainWindow::onCursorPositionChanged);
    setCentralWidget(&mainEditor);

    QPalette p = mainEditor.palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    mainEditor.setPalette(p);
    return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("File(&F)"),mb);

    ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("New(&N)"),Qt::CTRL+Qt::Key_N);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onNewFile);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Open(&O)"),Qt::CTRL+Qt::Key_O);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onFileOpen);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Save(&S)"),Qt::CTRL+Qt::Key_S);
        if(ret)
        {

            connect(action,&QAction::triggered,this,&MainWindow::onFileSave);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Save As(&A)"),Qt::CTRL+Qt::ALT+Qt::Key_A);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onFileSaveAs);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Save All"),0);
        if(ret)
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,tr("Close"),Qt::CTRL+Qt::Key_W);
        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Close All(&L)"),Qt::CTRL+Qt::SHIFT+Qt::Key_W);
        if(ret)
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,tr("Print"),Qt::CTRL+Qt::Key_P);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onFilePrint);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("立即打印"),0);
        if(ret)
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,tr("Exit(&X)"),Qt::ALT+Qt::Key_F4);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onFileExit);
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initEditMenu(QMenuBar *mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("Edit(&E)"),mb);
    ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("Undo(&U)"),Qt::CTRL+Qt::Key_Z);
        if(ret)
        {
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::undo);
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Redo(&R)"),Qt::CTRL+Qt::Key_Y);
        if(ret)
        {
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::redo);
            action->setEnabled(false);
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,tr("Cut(&R)"),Qt::CTRL+Qt::Key_X);
        if(ret)
        {
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::cut);
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Copy(&C)"),Qt::CTRL+Qt::Key_C);
        if(ret)
        {
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::copy);
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Paste(&P)"),Qt::CTRL+Qt::Key_V);
        if(ret)
        {
            connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::paste);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Delete(&D)"),Qt::Key_Delete);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onEditDelete);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Select All(&L)"),Qt::CTRL+Qt::Key_A);
        if(ret)
        {
            menu->addAction(action);
        }
        menu->addSeparator();
        ret = ret && makeAction(action,tr("Set ReadOnly"),0);
        if(ret)
        {
            action->setCheckable(true);
            connect(action,&QAction::triggered,this,&MainWindow::onEditSetReadOnly);
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initSearchMenu(QMenuBar *mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("Search(&S)"),mb);
    ret = (menu != NULL);
    if(ret )
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("Find(&F)"),Qt::CTRL+Qt::Key_F);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onSearchFind);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Replace(&H)"),Qt::CTRL+Qt::Key_H);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onSearchReplace);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("Goto(&G)"),Qt::CTRL+Qt::Key_G);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onSearchGoto);
            menu->addAction(action);
        }
    }
    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar *mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("Format"),mb);
    ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("Font(&F)"),0);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onFormatFont);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("AutoWrap(&W)"),0);
        if(ret)
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action,&QAction::triggered,this,&MainWindow::onFormatAutoWrap);
            menu->addAction(action);
        }
    }
    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initViewMenu(QMenuBar *mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("View(&O)"),mb);
    ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("StatusBar(&S)"),0);
        if(ret)
        {
            action->setCheckable(true);
            action->setChecked(true);

            connect(action,&QAction::triggered,this,&MainWindow::onViewStatusBar);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,tr("ToolBar"),0);
        if(ret)
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action,&QAction::triggered,this,&MainWindow::onViewToolBar);
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar *mb)
{
    bool ret = true;
    QMenu* menu = new QMenu(tr("Help(&H)"),mb);
    ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,tr("Help"),0);
        if(ret)
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,tr("About eNotePad"),0);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&MainWindow::onHelpAbout);
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;
    ret = ret && makeAction(action,tr("New"),":/images/new.png");
    if(ret)
    {

        connect(action,&QAction::triggered,this,&MainWindow::onNewFile);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Open"),":/images/open.png");
    if(ret)
    {
        connect(action,&QAction::triggered,this,&MainWindow::onFileOpen);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Save"),":/images/save.png");
    if(ret)
    {
        connect(action,&QAction::triggered,this,&MainWindow::onFileSave);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("SaveAs"),":/images/saveas.png");
    if(ret)
    {

        connect(action,&QAction::triggered,this,&MainWindow::onFileSaveAs);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Print"),":/images/print.png");
    if(ret)
    {
        connect(action,&QAction::triggered,this,&MainWindow::onFilePrint);
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tr("Undo"),":/images/undo.png");
    if(ret)
    {
        connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::undo);
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Redo"),":/images/redo.png");
    if(ret)
    {
        connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::redo);
        action->setEnabled(false);
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tr("Cut"),":/images/cut.png");
    if(ret)
    {
        connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::cut);
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Copy"),":/images/copy.png");
    if(ret)
    {
        connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::copy);
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Paste"),":/images/paste.png");
    if(ret)
    {
        connect(action,&QAction::triggered,&mainEditor,&QPlainTextEdit::paste);
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tr("Find"),":/images/find.png");
    if(ret)
    {
        connect(action,&QAction::triggered,this,&MainWindow::onSearchFind);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("Replace"),":/images/replace.png");
    if(ret)
    {

        connect(action,&QAction::triggered,this,&MainWindow::onSearchReplace);
        tb->addAction(action);
    }


    ret = ret && makeAction(action,tr("Goto"),":/images/goto.png");
    if(ret)
    {
        connect(action,&QAction::triggered,this,&MainWindow::onSearchGoto);
        tb->addAction(action);
    }



    tb->addSeparator();

    ret = ret && makeAction(action,tr("Font"),":/images/font.png");
    if(ret)
    {

        connect(action,&QAction::triggered,this,&MainWindow::onFormatFont);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tr("AutoWrap"),":/images/wrap.png");
    if(ret)
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action,&QAction::triggered,this,&MainWindow::onFormatAutoWrap);
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tr("StatusBar"),":/images/status.png");
    if(ret)
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action,&QAction::triggered,this,&MainWindow::onViewStatusBar);
        tb->addAction(action);

    }

    ret = ret && makeAction(action,tr("ToolBar"),":/images/tool.png");
    if(ret)
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action,&QAction::triggered,this,&MainWindow::onViewToolBar);
        tb->addAction(action);
    }


    return ret;
}

bool MainWindow::initStatusItem(QStatusBar *sb)
{
    bool ret = true;
    QLabel* author = new QLabel("eClover");
    ret = (author != NULL);
    if(ret)
    {
        statusLbl.setMinimumWidth(200);
        statusLbl.setAlignment(Qt::AlignCenter);
        statusLbl.setText("Line:1    Col:1");

        author->setMinimumWidth(200);
        author->setAlignment(Qt::AlignCenter);

        sb->addPermanentWidget(new QLabel());
        sb->addPermanentWidget(&statusLbl);
        sb->addPermanentWidget(author);
    }
    return ret;
}

bool MainWindow::makeAction(QAction*& action, QString title, int st)
{
    bool ret = true;
    action = new QAction(title);
    if(action != NULL)
    {
        if(st > 0)
        {
            action->setShortcut(QKeySequence(st));
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}



bool MainWindow::makeAction(QAction*& action,QString tip,QString icon)
{
    bool ret = true;
    action = new QAction("",NULL);
    if(action != NULL)
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
    }
    else
    {
        ret = false;
    }
    return ret;
}
