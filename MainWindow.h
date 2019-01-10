#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSharedPointer>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFileDialog>
#include "FindDialog.h"
#include "MainEditor.h"
#include <QPalette>
#include "ReplaceDialog.h"
#include <QTabWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    bool construct();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initTabWidget();

    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initSearchMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);

    bool initToolItem(QToolBar* tb);
    bool initStatusItem(QStatusBar* sb);


    void addNewTab(MainEditor* edit,QString title);
    bool createEdit(MainEditor*& edit);
    bool makeAction(QAction*& action,QString title,int st);
    bool makeAction(QAction*& action,QString tip,QString icon);

    QString showFileDialog(QFileDialog::AcceptMode mode,QString title);

    bool preEditorChange(MainEditor* edit,QString tabTitle);
    bool saveCurrentFile(MainEditor* edit);
    void closeEvent(QCloseEvent* e);
    void closeTab(int index);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);
    QToolBar* toolBar();
    void updateMainEditor(MainEditor* edit);

  //  QPlainTextEdit mainEditor;
    MainEditor* mainEdit;
    QTabWidget m_tabWidget;
    QLabel statusLbl;

    bool m_isTextChanged;
    QSharedPointer<FindDialog> m_pFindDlg;
    QSharedPointer<ReplaceDialog> m_pReplaceDlg;

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileSaveAll();
    void onFileExit();
    void onFileNew();
    void onFileCloseAll();
    void onFileClose();
    void onTextChanged();
    void onCopyAvailable(bool available);
    void onRedoAvailable(bool available);
    void onUndoAvailable(bool available);
    void onFilePrint();
    void onCursorPositionChanged();
    void onEditDelete();
    void onSearchFind();
    void onSearchReplace();
    void onViewStatusBar();
    void onViewToolBar();
    void onHelpAbout();
    void onFormatAutoWrap();
    void onFormatFont();
    void onSearchGoto();
    void onEditSetReadOnly();
    void onTabCloseRequested(int index);
    void onCurrentTabChanged(int index);

    void onCopy();
    void onCut();
    void onUndo();
    void onRedo();
    void onPaste();
    void onSelectAll();


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openFileToTabWidget(QString path);

    static MainWindow* NewInstance();
};

#endif // MAINWINDOW_H
