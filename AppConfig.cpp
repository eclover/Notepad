#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>



AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    m_isValid = read();
}

AppConfig::AppConfig( QSize size,QPoint point,QFont font, bool sbVisible, bool tbVisible,bool autoWrap,QObject *parent)
    :QObject(parent)
{
    m_editorSize = size;
    m_editorPoint = point;
    m_editorFont = font;
    m_isStatusBarVisible = sbVisible;
    m_isToolBarVisible = tbVisible;
    m_isAutoWrap = autoWrap;
    m_isValid = true;
}

bool AppConfig::isStatusBarVisible()
{
    return m_isStatusBarVisible;
}

bool AppConfig::isToolBarVisible()
{
    return m_isToolBarVisible;
}

bool AppConfig::isAutoWrap()
{
    return m_isAutoWrap;
}

QFont AppConfig::editorFont()
{
    return m_editorFont;
}

bool AppConfig::isValid()
{
    return m_isValid;
}

QSize AppConfig::mainWindowSize()
{
    return m_editorSize;
}

QPoint AppConfig::mainWindowPoint()
{
    return m_editorPoint;
}

bool AppConfig::write()
{
    bool ret = false;

    QFile file(QApplication::applicationDirPath() + "/app.config");
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_8);
        out << m_isStatusBarVisible;
        out << m_isToolBarVisible;
        out << m_editorFont;
        out << m_isAutoWrap;
        out << m_editorSize;
        out << m_editorPoint;
        file.close();
        ret = true;
    }
    return ret;
}

bool AppConfig::read()
{
    bool ret = false;
    QFile file(QApplication::applicationDirPath()+"/app.config");
    if(file.open(QIODevice::ReadOnly ))
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_8);
        in >> m_isStatusBarVisible;
        in >> m_isToolBarVisible;
        in >> m_editorFont;
        in >> m_isAutoWrap;
        in >> m_editorSize;
        in >> m_editorPoint;
        file.close();
        ret = true;
    }
    return ret;
}
