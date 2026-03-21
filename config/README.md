# baReos Verzeichnisstruktur

Spiegelt die Bareos FHS-konforme Verzeichnisstruktur:

```
/etc/bareos/                     → config/
  bareos-dir.d/                  Director-Konfiguration
    catalog/                     Datenbank-Katalog
    client/                      File-Daemon Definitionen
    console/                     Console/Admin Definitionen
    director/                    Director Resource
    fileset/                     FileSet Definitionen
    job/                         Job Definitionen
    jobdefs/                     Job Defaults
    messages/                    Message Routing
    pool/                        Storage Pool Definitionen
    profile/                     ACL Profile
    schedule/                    Job Schedules
    storage/                     Storage Definitionen
    user/                        User Definitionen
  bareos-fd.d/                   File Daemon Konfiguration
  bareos-sd.d/                   Storage Daemon Konfiguration

/var/lib/bareos/                 → (Runtime, nicht im Repo)
  storage/                       Backup-Volumes (Default)

/var/log/bareos/                 → (Logs, nicht im Repo)

/usr/lib/bareos/
  plugins/                       → plugins/
  scripts/                       → scripts/
    ddl/                         → scripts/ddl/ (Schema aus catalog/schema/)
  defaultconfigs/                → defaultconfigs/
```
