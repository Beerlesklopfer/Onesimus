#include "baculadirector.h"
#include <QCoreApplication>
#include <QDebug>

/*
 * BEISPIEL: Verwendung der BaculaDirector-Klasse
 * 
 * Dieses Beispiel zeigt, wie Sie die BaculaDirector-Klasse
 * in Ihren eigenen Qt-Anwendungen verwenden können.
 */

class BaculaExample : public QObject
{
    Q_OBJECT

public:
    BaculaExample() : director(new BaculaDirector(this))
    {
        // Verbinde Signals
        connect(director, &BaculaDirector::connected, this, &BaculaExample::onConnected);
        connect(director, &BaculaDirector::disconnected, this, &BaculaExample::onDisconnected);
        connect(director, &BaculaDirector::connectionError, this, &BaculaExample::onError);
        connect(director, &BaculaDirector::jobsReceived, this, &BaculaExample::onJobsReceived);
        connect(director, &BaculaDirector::clientsReceived, this, &BaculaExample::onClientsReceived);
    }

    void run()
    {
        // Beispiel 1: Bconsole-Verbindung
        qDebug() << "Verbinde mit Bacula Director über Bconsole...";
        director->connectBConsole("localhost", 9101, "bacula-dir", "YourPassword");
        
        // Beispiel 2: REST-API-Verbindung (auskommentiert)
        // director->connectRestAPI("http://localhost:9101", "admin", "password");
    }

private slots:
    void onConnected()
    {
        qDebug() << "Erfolgreich verbunden!";
        qDebug() << "Verbindungstyp:" << 
            (director->connectionType() == BaculaDirector::BConsole ? "Bconsole" : "REST-API");
        
        // Daten abrufen
        if (director->connectionType() == BaculaDirector::RestAPI) {
            director->restGetJobs();
            director->restGetClients();
        } else {
            director->listJobs(50);
            director->listClients();
        }
    }

    void onDisconnected()
    {
        qDebug() << "Verbindung getrennt.";
        QCoreApplication::quit();
    }

    void onError(const QString &error)
    {
        qDebug() << "FEHLER:" << error;
        QCoreApplication::quit();
    }

    void onJobsReceived(const QList<BaculaDirector::JobInfo> &jobs)
    {
        qDebug() << "\n=== Jobs (" << jobs.size() << ") ===";
        
        for (const auto &job : jobs) {
            qDebug() << QString("Job %1: %2 (%3) - Status: %4 - Bytes: %5")
                .arg(job.jobId)
                .arg(job.name)
                .arg(job.clientName)
                .arg(job.status)
                .arg(job.jobBytes);
        }
        
        // Beispiel: Job starten
        // director->runJob("MyBackupJob");
        
        // Beispiel: Job abbrechen
        // director->cancelJob(123);
    }

    void onClientsReceived(const QList<BaculaDirector::ClientInfo> &clients)
    {
        qDebug() << "\n=== Clients (" << clients.size() << ") ===";
        
        for (const auto &client : clients) {
            qDebug() << QString("Client: %1 @ %2:%3 (%4)")
                .arg(client.name)
                .arg(client.address)
                .arg(client.port)
                .arg(client.os);
        }
        
        // Beispiel: Client-Status abfragen
        if (!clients.isEmpty()) {
            // director->statusClient(clients.first().name);
        }
        
        // Beende nach Datenempfang
        QTimer::singleShot(2000, QCoreApplication::quit);
    }

private:
    BaculaDirector *director;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    BaculaExample example;
    example.run();
    
    return app.exec();
}

#include "example_usage.moc"


/*
 * WEITERE BEISPIELE:
 * 
 * 1. Job mit Parametern ausführen (REST-API):
 * 
 *    QJsonObject params;
 *    params["level"] = "Full";
 *    params["pool"] = "Default";
 *    director->restRunJob("BackupJob", params);
 * 
 * 
 * 2. Alle laufenden Jobs abrufen (REST-API):
 * 
 *    director->restGetJobs("status=R");
 * 
 * 
 * 3. Restore-Job starten (Bconsole):
 * 
 *    director->restoreFiles("client-fd", "Full Set");
 * 
 * 
 * 4. Storage-Status abfragen (Bconsole):
 * 
 *    director->statusStorage("File-Storage");
 * 
 * 
 * 5. Benutzerdefinierten Befehl senden (Bconsole):
 * 
 *    director->sendCommand("list pools");
 *    // Antwort kommt über commandResponse-Signal
 * 
 * 
 * 6. Job-Status überwachen:
 * 
 *    connect(director, &BaculaDirector::jobStatusChanged, 
 *            [](int jobId, BaculaDirector::JobStatus status) {
 *        qDebug() << "Job" << jobId << "Status:" << status;
 *    });
 */
