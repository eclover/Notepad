#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QFont>
#include <QSize>
#include <QPoint>


class AppConfig : public QObject
{
    Q_OBJECT
    QFont m_editorFont;
    bool m_isStatusBarVisible;
    bool m_isToolBarVisible;
    bool m_isAutoWrap;
    bool m_isValid;
    QSize m_editorSize;
    QPoint m_editorPoint;
    bool read();

public:
    explicit AppConfig(QObject *parent = 0);
    explicit AppConfig(QSize size,QPoint point,QFont font,bool sbVisible,bool tbVisible,bool autoWrap,QObject *parent = 0);
    bool isStatusBarVisible();
    bool isToolBarVisible();
    bool isAutoWrap();
    QFont editorFont();
    bool isValid();
    QSize mainWindowSize();
    QPoint mainWindowPoint();

    bool write();
signals:

public slots:
};

#endif // APPCONFIG_H
