#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include <QPlainTextEdit>

class MainEditor : public QPlainTextEdit
{
    Q_OBJECT
    bool m_isTextChanged;
    QString m_filePath;
    QString m_title;
    void initConnections();
    void initEditors();
    QWidget* lineNumberArea;

    void resizeEvent(QResizeEvent *event) override;
public:
    explicit MainEditor(QWidget* parent = 0);
    explicit MainEditor(QString path,QWidget *parent = 0);
    bool isTextChanged();
    QString filePath();
    void setTextChanged(bool change);
    void setFilePath(QString path);
    bool saveFile(QString path);
    bool openFile(QString path);
    void setTitle(QString title);
    QString title();

    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent* event);

private slots:
    void highlightCurrentLine();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect rect,int count);
    void onTextChanged();


};


class LineNumberArea : public QWidget
  {
  public:
      LineNumberArea(MainEditor *editor) : QWidget(editor) {
          codeEditor = editor;
      }

      QSize sizeHint() const override {
          return QSize(codeEditor->lineNumberAreaWidth(), 0);
      }

  protected:
      void paintEvent(QPaintEvent *event) override {
          codeEditor->lineNumberAreaPaintEvent(event);
      }

  private:
      MainEditor *codeEditor;
  };


#endif // MAINEDITOR_H
