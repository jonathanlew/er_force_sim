/***************************************************************************
 *   Copyright 2020 Michael Eischer, Andreas Wendler                       *
 *   Robotics Erlangen e.V.                                                *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef COMBINEDLOGWRITER_H
#define COMBINEDLOGWRITER_H

#include "src/protobuf/robot.pb.h"
#include "src/protobuf/status.h"
#include "src/protobuf/command.h"

#include <QString>
#include <QObject>
#include <QList>

class LogFileWriter;
class BacklogWriter;
class QThread;
class QDateTime;
class QLabel;
class StatusSource;
namespace CombinedLogWriterInternal {
    class SignalSource;
}

class CombinedLogWriter : public QObject
{
    Q_OBJECT
public:
    CombinedLogWriter(bool replay, int backlogLength);
    ~CombinedLogWriter();
    CombinedLogWriter(const CombinedLogWriter&) = delete;
    CombinedLogWriter& operator=(const CombinedLogWriter&) = delete;
    std::shared_ptr<StatusSource> makeStatusSource();
    void sendBacklogStatus(int lastNPackets);
    Status getTeamStatus();
    static QString dateTimeToString(const QDateTime & dt);

signals:
    void sendStatus(const Status& s);
    void resetBacklog();

public slots:
    void handleStatus(Status status);
    void enableLogging(bool enable); // enables or disables both record and backlog
    void handleCommand(Command command);

private:
    QString createLogFilename() const;
    void startLogfile();
    void saveBackLog();
    void recordButtonToggled(bool enabled, QString overwriteFilename);
    void useLogfileLocation(bool enabled);
    void sendIsLogging(bool log);

private:
    enum class LogState {
        PENDING,
        LOGGING,
        BACKLOG
    } m_logState;
    const bool m_isReplay;
    bool m_useSettingLocation = false;
    BacklogWriter *m_backlogWriter;
    QThread *m_backlogThread;
    LogFileWriter *m_logFile;
    QThread *m_logFileThread;

    robot::Team m_yellowTeam;
    robot::Team m_blueTeam;
    QString m_yellowTeamName;
    QString m_blueTeamName;

    qint64 m_lastTime;

    bool m_isLoggingEnabled;

    CombinedLogWriterInternal::SignalSource *m_signalSource;

    QList<Status> m_zeroTimeStatus;
};

#endif // COMBINEDLOGWRITER_H
