#include <QDebug>
#include <QFile>
#include <QString>
#include <QByteArray>

#include <string>
#include <cstdio>

#include "windowattr.h"

WindowAttr::WindowAttr(int WId) :
    m_WId(WId), m_PID(0)
{
    findPid(WId);
    findWName(WId);
    findAppName();
}

WindowAttr::WindowAttr() :
    m_WId(0), m_PID(0)
{
    findActiveWindowWId();
    findPid(m_WId);
    findWName(m_WId);
    findAppName();
}

QString WindowAttr::exec(QString cmd)
{
    QByteArray ba = cmd.toLocal8Bit();
    FILE* pipe = popen(ba.data(), "r");

    if (!pipe) return "ERROR";
    char buffer[128];
    QString result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);

    return result;
}

int WindowAttr::getWId() const
{
    return m_WId;
}

int WindowAttr::getPid() const
{
    return m_PID;
}

void WindowAttr::findActiveWindowWId()
{
    QString cmd = QString("xprop -root | awk '/_NET_ACTIVE_WINDOW\\"
                          "(WINDOW\\)/{print $NF}'");
    QString activeWId = exec(cmd);

    bool ok = true;
    int awid = activeWId.toInt(&ok, 16);
    if ( !ok )
        qDebug() << "Nieudana konwersja QString->int";
    m_WId = awid;
}

void WindowAttr::findPid(int WId)
{
    QString cmd = QString("xprop -id %1 | awk '/_NET_WM_PID\\(CARDINAL\\)"
                          "/{print $NF}'")
                          .arg(WId);
    QString pid = exec(cmd);
    m_PID = pid.toInt();
}

QString WindowAttr::getWName() const
{
    return m_WName;
}

QString WindowAttr::getAppName() const
{
    return m_AppName;
}

void WindowAttr::findWName(int WId)
{
    /* Prawdopodobnie WM_NAME jest najpewniejsze (wystepowalo jak do tej
     * pory we wszystkich uruchamianych programach) */
    QString cmd = QString("xprop -id %1 | grep WM_NAME\\(STRING\\)")
                            .arg(WId);
    m_WName = exec(cmd);
    m_WName.remove(0, 19);                  // usuwanie 19 pierwszych znakow
    m_WName.chop(2);                        // usuwanie 2 ostatnich znakow
}

void WindowAttr::findAppName()
{
    if (m_WId == 0 || m_PID == 0)
        return; 

    QString proc_file_name = QString("/proc/%1/comm").arg(m_PID);

    QFile file(proc_file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return; 

    m_AppName = file.readLine();
    m_AppName.chop(1);
}

