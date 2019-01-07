#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFileDialog>
#include "FindDialog.h"
#include <QSharedPointer>
#include <QPalette>
#include "ReplaceDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    bool construct();
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEdit();

    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initSearchMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);

    bool initToolItem(QToolBar* tb);
    bool initStatusItem(QStatusBar* sb);

    bool makeAction(QAction*& action,QString title,int st);
    bool makeAction(QAction*& action,QString tip,QString icon);

    QString showFileDialog(QFileDialog::AcceptMode mode,QString title);
    void showErrorMessage(QString message);
    int showQueryMessage(QString message);

    void preEditorChange();
    void openFileToEditor(QString path);
    QString saveCurrentFile(QString path);
    void closeEvent(QCloseEvent* e);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);

    QToolBar* toolBar();

    QPlainTextEdit mainEditor;
    QLabel statusLbl;

    bool m_isTextChanged;
    QString m_filePath;
    QSharedPointer<FindDialog> m_pFindDlg;
    QSharedPointer<ReplaceDialog> m_pReplaceDlg;

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onFileExit();
    void onNewFile();
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

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openFile(QString path);

    static MainWindow* NewInstance();
};

#endif // MAINWINDOW_H
