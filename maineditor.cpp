#include "MainEditor.h"
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include "Pub.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QPainter>
#include <QTextBlock>
#include <QPalette>

MainEditor::MainEditor(QWidget* parent)
    :QPlainTextEdit(parent),
      m_isTextChanged(false),
      m_filePath(""),
      m_title("[New] - eNotePad")
{
    lineNumberArea = new LineNumberArea(this);
    initEditors();
    initConnections();
}

MainEditor::MainEditor(QString path, QWidget *parent)
    :QPlainTextEdit(parent)
{
    m_filePath = path;
}

bool MainEditor::isTextChanged()
{
    return m_isTextChanged;
}

QString MainEditor::filePath()
{
    return m_filePath;
}

void MainEditor::setTextChanged(bool change)
{
    m_isTextChanged = change;
}

void MainEditor::setFilePath(QString path)
{
    m_filePath = path;
}

bool MainEditor::saveFile(QString path)
{
   bool ret = true;

    if(path != "")
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out<<this->toPlainText();

            m_isTextChanged = false;
            file.close();
            m_filePath = path;
            m_title = "[" +m_filePath+"] - eNotePad";
        }
        else
        {
            ret = false;
        }

    }
    return ret;
}

bool MainEditor::openFile(QString path)
{
    bool ret = true;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextCodec* codec = QTextCodec::codecForName("GBK");
        this->setPlainText(codec->toUnicode(file.readAll()));
        file.close();
        m_isTextChanged = false;
        m_filePath = path;
    }
    else
    {
        ret = false;

    }
    return ret;
}

void MainEditor::setTitle(QString title)
{
    m_title = title;
}

QString MainEditor::title()
{
    return m_title;
}

void MainEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::blue).lighter(190);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);

    }
    setExtraSelections(extraSelections);
}

void MainEditor::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(),0,0,0);
}

void MainEditor::updateLineNumberArea(const QRect rect, int count)
{
    if (count)
        lineNumberArea->scroll(0, count);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void MainEditor::onTextChanged()
{
    if(!m_isTextChanged)
    {
        setTextChanged(true);
        setTitle("*" + m_title);
    }
    m_isTextChanged = true;
}

void MainEditor::initConnections()
{
    connect(this,&MainEditor::cursorPositionChanged,this,&MainEditor::highlightCurrentLine);
    connect(this,&MainEditor::blockCountChanged,this,&MainEditor::updateLineNumberAreaWidth);
    connect(this,&MainEditor::updateRequest,this,&MainEditor::updateLineNumberArea);
    connect(this,&MainEditor::textChanged,this,&MainEditor::onTextChanged);
}

void MainEditor::initEditors()
{
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    QPalette p = this->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight));
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));
    this->setPalette(p);
}

void MainEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));
 }

int MainEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1,blockCount());
    while(max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void MainEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0,top,lineNumberArea->width(),fontMetrics().height(),Qt::AlignRight,number);

        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
