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
    findWName();
}

WindowAttr::WindowAttr() :
    m_WId(0), m_PID(0)
{
    findActiveWindowWId();
    findPid(m_WId);
    findWName();
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

void WindowAttr::findWName()
{
    if (m_WId == 0 || m_PID == 0)
        return; 

    QString proc_file_name = QString("/proc/%1/cmdline").arg(m_PID);

    QFile file(proc_file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return; 

    m_WName = file.readLine();
}
