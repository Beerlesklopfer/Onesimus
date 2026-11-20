<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE" sourcelanguage="en">
<context>
    <name>AuthMethodPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1589" />
        <source>How should we authenticate?</source>
        <translation>Wie soll die Anmeldung erfolgen?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1593" />
        <source>TLS-PSK (recommended)</source>
        <translation>TLS-PSK (empfohlen)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1598" />
        <source>TLS with certificates</source>
        <translation>TLS mit Zertifikaten</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1615" />
        <source>TLS Cipher List (optional):</source>
        <translation>TLS-Cipher-Liste (optional):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1622" />
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>z. B. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1623" />
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Click Preset to use recommended ciphers.</source>
        <translation>Durch Doppelpunkte getrennte Liste von TLS-PSK-Ciphers.
Leer lassen für automatische Erkennung.
Auf „Vorgabe“ klicken, um die empfohlenen Ciphers zu verwenden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1628" />
        <source>Preset</source>
        <translation>Vorgabe</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1629" />
        <source>Fill with recommended PSK ciphers</source>
        <translation>Mit empfohlenen PSK-Ciphers füllen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1638" />
        <source>TLS-PSK is the default for Bareos 18.2+.</source>
        <translation>TLS-PSK ist der Standard ab Bareos 18.2.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1696" />
        <source>Server supports TLS encryption.</source>
        <translation>Der Server unterstützt TLS-Verschlüsselung.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1704" />
        <source>TLS-PSK - not available on this server</source>
        <translation>TLS-PSK – auf diesem Server nicht verfügbar</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1705" />
        <source>This server doesn't appear to support TLS-PSK. Consider upgrading to Bareos 18.2 or later.</source>
        <translation>Dieser Server scheint TLS-PSK nicht zu unterstützen. Erwägen Sie ein Upgrade auf Bareos 18.2 oder neuer.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1713" />
        <source>Server check failed: %1</source>
        <translation>Serverprüfung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1730" />
        <source>TLS-PSK uses your password to encrypt the connection.</source>
        <translation>TLS-PSK verschlüsselt die Verbindung mit Ihrem Passwort.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1734" />
        <source>You'll need CA, client certificate, and private key.</source>
        <translation>Sie benötigen CA, Client-Zertifikat und privaten Schlüssel.</translation>
    </message>
</context>
<context>
    <name>B321Dialog</name>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="88" />
        <location filename="../src/tools/b321dialog.cpp" line="575" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="109" />
        <location filename="../src/tools/b321dialog.cpp" line="632" />
        <source>%1 (not in the volume list)</source>
        <translation>%1 (nicht in der Volume-Liste)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="111" />
        <location filename="../src/tools/b321dialog.cpp" line="649" />
        <source>media type unknown</source>
        <translation>Medientyp unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="112" />
        <source>storage unknown</source>
        <translation>Storage unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="115" />
        <source>off site (marked)</source>
        <translation>extern (markiert)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="117" />
        <source>off site (location %1)</source>
        <translation>extern (Standort %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="119" />
        <source>on site</source>
        <translation>vor Ort</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="120" />
        <source>off site unknown</source>
        <translation>extern unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="122" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="131" />
        <location filename="../src/tools/b321dialog.cpp" line="132" />
        <source>3-2-1 Verifier</source>
        <translation>3-2-1-Prüfung</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="133" />
        <source>Checks for every backup job whether there are 3 copies of the data, on 2 media types, with 1 copy off site. Read only.</source>
        <translation>Prüft für jeden Backup-Job, ob es 3 Kopien der Daten auf 2 Medientypen gibt, davon 1 Kopie extern. Nur lesend.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="154" />
        <source>Data state</source>
        <translation>Datenstand</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="156" />
        <source>&amp;Refresh</source>
        <translation>&amp;Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="158" />
        <source>Reload the jobs, the volumes of the jobs and the volume list (read only)</source>
        <translation>Jobs, die Volumes der Jobs und die Volume-Liste neu laden (nur lesend)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="170" />
        <source>&amp;Backup jobs:</source>
        <translation>&amp;Backup-Jobs:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174" />
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174" />
        <source>Copies</source>
        <translation>Kopien</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174" />
        <source>Media Types</source>
        <translation>Medientypen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Off Site</source>
        <translation>Extern</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Verdict</source>
        <translation>Bewertung</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Reason</source>
        <translation>Grund</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177" />
        <source>Copies of the data a restore would use: the chain, its complete copies and, if set, the original data. 3 are needed.</source>
        <translation>Kopien der Daten, die eine Wiederherstellung nutzen würde: die Kette, ihre vollständigen Kopien und, falls eingestellt, die Originaldaten. 3 werden benötigt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="180" />
        <source>Distinct media types of the volumes of the chain and its complete copies. 2 are needed.</source>
        <translation>Verschiedene Medientypen der Volumes der Kette und ihrer vollständigen Kopien. 2 werden benötigt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="182" />
        <source>Copies off site, by your marking of the storages or a location of the volume. 1 is needed.</source>
        <translation>Externe Kopien, gemäß Ihrer Markierung der Storages oder einem Standort des Volumes. 1 wird benötigt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="185" />
        <source>3-2-1 check per backup job</source>
        <translation>3-2-1-Prüfung je Backup-Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="186" />
        <source>One row per backup job: copies, media types, off site, verdict and reason. The details of the selected job follow below the table.</source>
        <translation>Eine Zeile je Backup-Job: Kopien, Medientypen, extern, Bewertung und Grund. Die Details des ausgewählten Jobs folgen unter der Tabelle.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="221" />
        <source>C&amp;hain and copies:</source>
        <translation>&amp;Kette und Kopien:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="226" />
        <source>Chain and copies of the selected job</source>
        <translation>Kette und Kopien des ausgewählten Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="227" />
        <source>The jobs a restore would use and the copies of them, with the volumes they lie on</source>
        <translation>Die Jobs, die eine Wiederherstellung nutzen würde, und ihre Kopien, mit den Volumes, auf denen sie liegen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="237" />
        <source>&amp;Why, and what to do:</source>
        <translation>&amp;Warum, und was tun:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="241" />
        <source>Reasons, suggestions and hints for the selected job</source>
        <translation>Gründe, Vorschläge und Hinweise zum ausgewählten Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="250" />
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="252" />
        <source>Count the &amp;original data as a copy</source>
        <translation>&amp;Originaldaten als Kopie zählen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="253" />
        <source>The 3-2-1 rule is usually stated as the production data plus 2 backups. Off: 3 backups are needed.</source>
        <translation>Die 3-2-1-Regel wird meist als Produktivdaten plus 2 Backups formuliert. Aus: 3 Backups werden benötigt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="258" />
        <source>O&amp;ff-site storages:</source>
        <translation>E&amp;xterne Storages:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="260" />
        <source>Bareos does not record where a storage stands. Tick the storages that are at another site.</source>
        <translation>Bareos speichert nicht, wo ein Storage steht. Haken Sie die Storages an, die an einem anderen Standort stehen.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="267" />
        <source>Off-site storages</source>
        <translation>Externe Storages</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="268" />
        <source>A checked storage is off site. Space toggles.</source>
        <translation>Ein angehakter Storage ist extern. Leertaste schaltet um.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="271" />
        <source>&amp;Unmarked storages are on site</source>
        <translation>&amp;Nicht markierte Storages sind vor Ort</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="272" />
        <source>Without this, a storage that is not ticked is "unknown", not "on site".</source>
        <translation>Ohne diese Option ist ein nicht angehakter Storage „unbekannt“, nicht „vor Ort“.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="282" />
        <source>&amp;Close</source>
        <translation>&amp;Schließen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="319" />
        <location filename="../src/tools/b321dialog.cpp" line="435" />
        <source>%1, off site</source>
        <translation>%1, extern</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="320" />
        <location filename="../src/tools/b321dialog.cpp" line="436" />
        <source>%1, not marked off site</source>
        <translation>%1, nicht als extern markiert</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="441" />
        <source>No storage known yet</source>
        <translation>Noch kein Storage bekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="468" />
        <location filename="../src/tools/b321dialog.cpp" line="517" />
        <source>Jobs: %1</source>
        <translation>Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="487" />
        <location filename="../src/tools/b321dialog.cpp" line="520" />
        <source>Volumes of the jobs: %1</source>
        <translation>Volumes der Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="502" />
        <location filename="../src/tools/b321dialog.cpp" line="523" />
        <source>Volume list: %1</source>
        <translation>Volume-Liste: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="554" />
        <source>%1: %2. %3</source>
        <translation>%1: %2. %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="558" />
        <source>%1/3</source>
        <translation>%1/3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="561" />
        <source>Copies: %1 of 3, %2</source>
        <translation>Kopien: %1 von 3, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="565" />
        <source>?/2</source>
        <translation>?/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="565" />
        <source>%1/2</source>
        <translation>%1/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="568" />
        <source>Media types: %1 of 2%2, %3</source>
        <translation>Medientypen: %1 von 2%2, %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="576" />
        <source>%1/1</source>
        <translation>%1/1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="580" />
        <source>Off site: unknown</source>
        <translation>Extern: unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="581" />
        <source>Off site: %1 of 1, %2</source>
        <translation>Extern: %1 von 1, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="587" />
        <source>Verdict: %1</source>
        <translation>Bewertung: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="611" />
        <source>No job selected.</source>
        <translation>Kein Job ausgewählt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="623" />
        <source>volumes not loaded</source>
        <translation>Volumes nicht geladen</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="625" />
        <source>no volume recorded</source>
        <translation>kein Volume verzeichnet</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="630" />
        <source>%1 (%2, %3, %4)</source>
        <translation>%1 (%2, %3, %4)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="640" />
        <source>Chain (the backup itself): %n job(s)</source>
        <translation>
            <numerusform>Kette (das Backup selbst): %n Job</numerusform>
            <numerusform>Kette (das Backup selbst): %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642" />
        <source>Copy in pool %1: complete</source>
        <translation>Kopie in Pool %1: vollständig</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642" />
        <location filename="../src/tools/b321dialog.cpp" line="645" />
        <source>(not reported)</source>
        <translation>(nicht gemeldet)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="644" />
        <source>Copy in pool %1: partial, %2 of %3 jobs</source>
        <translation>Kopie in Pool %1: teilweise, %2 von %3 Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="653" />
        <source>last job ended %1</source>
        <translation>letzter Job endete %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="654" />
        <source>copied %1</source>
        <translation>kopiert %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="655" />
        <source>%1 — %2</source>
        <translation>%1 — %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="662" />
        <source>%1 %2, started %3: %4</source>
        <translation>%1 %2, gestartet %3: %4</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="665" />
        <source> (migrated from job %1)</source>
        <translation> (migriert von Job %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="669" />
        <source>Copy %1 of %2 %3, copied %4: %5</source>
        <translation>Kopie %1 von %2 %3, kopiert %4: %5</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="678" />
        <source>No copy of %1 %2</source>
        <translation>Keine Kopie von %1 %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="684" />
        <source>No restorable chain in the job history</source>
        <translation>Keine wiederherstellbare Kette in der Job-Historie</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="689" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="696" />
        <source>Why</source>
        <translation>Warum</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="697" />
        <source>What to do</source>
        <translation>Was tun</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="698" />
        <source>Hints (not part of the verdict)</source>
        <translation>Hinweise (nicht Teil der Bewertung)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="707" />
        <source>Not everything could be loaded: %1</source>
        <translation>Nicht alles konnte geladen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="711" />
        <source>jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="712" />
        <source>volumes of the jobs</source>
        <translation>Volumes der Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="713" />
        <source>volume list</source>
        <translation>Volume-Liste</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="714" />
        <source>Loading: %1 …</source>
        <translation>Lade: %1 …</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="717" />
        <source>No job data yet. Refresh loads it (read only).</source>
        <translation>Noch keine Job-Daten. „Aktualisieren“ lädt sie (nur lesend).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="718" />
        <source>Not connected to a Director.</source>
        <translation>Nicht mit einem Director verbunden.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="726" />
        <source>%n job(s) checked: %1 pass, %2 fail, %3 unknown. </source>
        <translation>
            <numerusform>%n Job geprüft: %1 bestanden, %2 nicht bestanden, %3 unbekannt. </numerusform>
            <numerusform>%n Jobs geprüft: %1 bestanden, %2 nicht bestanden, %3 unbekannt. </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="728" />
        <source>From %n job record(s) of the last %1 days</source>
        <translation>
            <numerusform>Aus %n Job-Eintrag der letzten %1 Tage</numerusform>
            <numerusform>Aus %n Job-Einträgen der letzten %1 Tage</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="729" />
        <source>, %n volume(s)</source>
        <translation>
            <numerusform>, %n Volume</numerusform>
            <numerusform>, %n Volumes</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="730" />
        <source>, the volumes of %n job(s)</source>
        <translation>
            <numerusform>, die Volumes von %n Job</numerusform>
            <numerusform>, die Volumes von %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="732" />
        <source>. Read only.</source>
        <translation>. Nur lesend.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="13" />
        <source>3-2-1 &amp;Verifier...</source>
        <translation>3-2-1-&amp;Prüfung...</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="14" />
        <source>Check for every backup job: 3 copies, 2 media types, 1 copy off site (read only)</source>
        <translation>Für jeden Backup-Job prüfen: 3 Kopien, 2 Medientypen, 1 Kopie extern (nur lesend)</translation>
    </message>
</context>
<context>
    <name>B321Verifier</name>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="90" />
        <source>unknown time</source>
        <translation>unbekannte Zeit</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="96" />
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="109" />
        <source>(pool not reported)</source>
        <translation>(Pool nicht gemeldet)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="411" />
        <source>No successful Full backup of %1 in the job history of the last %n day(s): there is no chain to restore.</source>
        <translation>
            <numerusform>Kein erfolgreiches Full-Backup von %1 in der Job-Historie des letzten %n Tages: Es gibt keine Kette zum Wiederherstellen.</numerusform>
            <numerusform>Kein erfolgreiches Full-Backup von %1 in der Job-Historie der letzten %n Tage: Es gibt keine Kette zum Wiederherstellen.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="413" />
        <source>No successful Full backup of %1 in the job history: there is no chain to restore.</source>
        <translation>Kein erfolgreiches Full-Backup von %1 in der Job-Historie: Es gibt keine Kette zum Wiederherstellen.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="415" />
        <source>Run a Full backup of %1 and check that it terminates normally.</source>
        <translation>Führen Sie ein Full-Backup von %1 aus und prüfen Sie, ob es normal endet.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="417" />
        <source>No successful Full backup in the job history</source>
        <translation>Kein erfolgreiches Full-Backup in der Job-Historie</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="419" />
        <source>Only the original data on the client counts: 1 of 3 copies.</source>
        <translation>Nur die Originaldaten auf dem Client zählen: 1 von 3 Kopien.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="432" />
        <source>The chain a restore would use today is %1</source>
        <translation>Die Kette, die eine Wiederherstellung heute nutzen würde, ist %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="433" />
        <source>, a Differential</source>
        <translation>, ein Differential</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="434" />
        <source> and %n Incremental(s) after it</source>
        <translation>
            <numerusform> und %n Incremental danach</numerusform>
            <numerusform> und %n Incrementals danach</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="435" />
        <source>; it counts as 1 copy.</source>
        <translation>; sie zählt als 1 Kopie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="438" />
        <source>The original data on the client counts as 1 copy (setting "Count the original data as a copy").</source>
        <translation>Die Originaldaten auf dem Client zählen als 1 Kopie (Einstellung „Originaldaten als Kopie zählen“).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="441" />
        <source>The original data on the client is not counted (setting "Count the original data as a copy" is off).</source>
        <translation>Die Originaldaten auf dem Client werden nicht gezählt (Einstellung „Originaldaten als Kopie zählen“ ist aus).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="447" />
        <source>job %1 from job %2</source>
        <translation>Job %1 aus Job %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="449" />
        <source>Migrated chain jobs (%1): a Migrate moves the data and adds no copy; the old job is not restored any more.</source>
        <translation>Migrierte Jobs der Kette (%1): Ein Migrate verschiebt die Daten und fügt keine Kopie hinzu; der alte Job wird nicht mehr wiederhergestellt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="455" />
        <source>Pool %1 holds a copy of every job of the chain; it counts as 1 copy.</source>
        <translation>Pool %1 enthält eine Kopie jedes Jobs der Kette; sie zählt als 1 Kopie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="459" />
        <source>Pool %1 holds copies of %2 of the %3 jobs of the chain (missing: %4); a partial copy does not count.</source>
        <translation>Pool %1 enthält Kopien von %2 der %3 Jobs der Kette (fehlend: %4); eine Teilkopie zählt nicht.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="465" />
        <source>No Copy job has copied the chain.</source>
        <translation>Kein Copy-Job hat die Kette kopiert.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="467" />
        <source>Copies: %1 of 3.</source>
        <translation>Kopien: %1 von 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="473" />
        <source>The volumes of the chain are not known, so neither are their media types.</source>
        <translation>Die Volumes der Kette sind nicht bekannt, ebenso wenig ihre Medientypen.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="474" />
        <source>The volumes of the jobs (JobMedia) were not loaded: media types unknown.</source>
        <translation>Die Volumes der Jobs (JobMedia) wurden nicht geladen: Medientypen unbekannt.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="476" />
        <source>Media types of the chain and its complete copies: %1.</source>
        <translation>Medientypen der Kette und ihrer vollständigen Kopien: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="479" />
        <source>A copy that spans several media types counts once; copies on media types of their own: %1.</source>
        <translation>Eine Kopie, die mehrere Medientypen umfasst, zählt einmal; Kopien auf eigenen Medientypen: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="483" />
        <source>They lie on the storages %1; the same media type on several storages counts as one type.</source>
        <translation>Sie liegen auf den Storages %1; derselbe Medientyp auf mehreren Storages zählt als ein Typ.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="488" />
        <source>Some volumes of these jobs are not in the volume list or have no media type.</source>
        <translation>Einige Volumes dieser Jobs sind nicht in der Volume-Liste oder haben keinen Medientyp.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="510" />
        <source>storage %1 marked off site</source>
        <translation>Storage %1 als extern markiert</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="511" />
        <source>volume %1 has location %2 in the catalog</source>
        <translation>Volume %1 hat im Katalog den Standort %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="515" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="516" />
        <source>the chain</source>
        <translation>die Kette</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="517" />
        <source>the copy in pool %1</source>
        <translation>die Kopie in Pool %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="520" />
        <source>Off site: %1.</source>
        <translation>Extern: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="524" />
        <source>Off site is unknown: no storage of the chain or its copies is marked off site and no volume has a location in the catalog. Bareos itself does not record whether a storage is off site.</source>
        <translation>Extern ist unbekannt: Kein Storage der Kette oder ihrer Kopien ist als extern markiert, und kein Volume hat einen Standort im Katalog. Bareos selbst speichert nicht, ob ein Storage extern ist.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="529" />
        <source>Nothing is off site: every copy has a volume on a storage that is on site.</source>
        <translation>Nichts ist extern: Jede Kopie hat ein Volume auf einem Storage vor Ort.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="535" />
        <source>the present one</source>
        <translation>die aktuelle</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="538" />
        <source>Pool %1 lacks copies of the jobs %2: run its Copy job again (Selection Type = PoolUncopiedJobs copies what is missing) so that the whole chain is copied.</source>
        <translation>Pool %1 fehlen Kopien der Jobs %2: Führen Sie seinen Copy-Job erneut aus (Selection Type = PoolUncopiedJobs kopiert, was fehlt), damit die ganze Kette kopiert wird.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="546" />
        <source>Add a Copy job to a second storage with a different media type than %1 (for example tape, a removable disk or cloud storage with a Media Type of its own) (needed: %2 more).</source>
        <translation>Fügen Sie einen Copy-Job auf einen zweiten Storage mit einem anderen Medientyp als %1 hinzu (zum Beispiel Band, eine Wechselplatte oder Cloud-Speicher mit eigenem Media Type) (benötigt: %2 weitere).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="551" />
        <source>Add a Copy job to a second storage (needed: %1 more).</source>
        <translation>Fügen Sie einen Copy-Job auf einen zweiten Storage hinzu (benötigt: %1 weitere).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="555" />
        <source>Write one of the copies to a storage with another media type than %1.</source>
        <translation>Schreiben Sie eine der Kopien auf einen Storage mit einem anderen Medientyp als %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="559" />
        <source>Refresh: the volumes of the jobs and the volume list are needed to tell the media types.</source>
        <translation>Aktualisieren: Die Volumes der Jobs und die Volume-Liste werden benötigt, um die Medientypen zu bestimmen.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="564" />
        <source>Mark storage %1 as off site if it is at another site; if it is not, tick "Unmarked storages are on site".</source>
        <translation>Markieren Sie Storage %1 als extern, wenn er an einem anderen Standort steht; wenn nicht, haken Sie „Nicht markierte Storages sind vor Ort“ an.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="569" />
        <source>Refresh: the storages of the volumes are needed to tell where the copies are.</source>
        <translation>Aktualisieren: Die Storages der Volumes werden benötigt, um zu bestimmen, wo die Kopien liegen.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="573" />
        <source>Copy the chain to a storage at another site and mark that storage as off site (or take a volume off site and record its location).</source>
        <translation>Kopieren Sie die Kette auf einen Storage an einem anderen Standort und markieren Sie diesen Storage als extern (oder bringen Sie ein Volume nach extern und verzeichnen Sie seinen Standort).</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="593" />
        <source>The newest Full (job %1) is %n day(s) old</source>
        <translation>
            <numerusform>Das neueste Full (Job %1) ist %n Tag alt</numerusform>
            <numerusform>Das neueste Full (Job %1) ist %n Tage alt</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="595" />
        <source>; this job's Fulls ran every %n day(s) (median of the loaded history).</source>
        <translation>
            <numerusform>; die Fulls dieses Jobs liefen alle %n Tag (Median der geladenen Historie).</numerusform>
            <numerusform>; die Fulls dieses Jobs liefen alle %n Tage (Median der geladenen Historie).</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="598" />
        <source> It is overdue.</source>
        <translation> Es ist überfällig.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="601" />
        <source>The newest Full (job %1) is %n day(s) old; it is the only successful Full in the loaded history.</source>
        <translation>
            <numerusform>Das neueste Full (Job %1) ist %n Tag alt; es ist das einzige erfolgreiche Full in der geladenen Historie.</numerusform>
            <numerusform>Das neueste Full (Job %1) ist %n Tage alt; es ist das einzige erfolgreiche Full in der geladenen Historie.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="628" />
        <source>The newest copy of this job (job %1 in pool %2, a copy of job %3) was made %n day(s) ago.</source>
        <translation>
            <numerusform>Die neueste Kopie dieses Jobs (Job %1 in Pool %2, eine Kopie von Job %3) wurde vor %n Tag erstellt.</numerusform>
            <numerusform>Die neueste Kopie dieses Jobs (Job %1 in Pool %2, eine Kopie von Job %3) wurde vor %n Tagen erstellt.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="638" />
        <source>That copy belongs to an earlier chain: the current chain (from job %1) has no complete copy in pool %2.</source>
        <translation>Diese Kopie gehört zu einer früheren Kette: Die aktuelle Kette (ab Job %1) hat keine vollständige Kopie in Pool %2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="667" />
        <source>The chain's %1 (job %2)</source>
        <translation>%1 der Kette (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="669" />
        <source>The copy (job %1, pool %2) of the chain's %3</source>
        <translation>Die Kopie (Job %1, Pool %2) von %3 der Kette</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="671" />
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="673" />
        <source>%1 is on volume %2, which is in status Error.</source>
        <translation>%1 liegt auf Volume %2, das im Status Error ist.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="676" />
        <source>The chain's Full is on volume %1, which is in Error: run a new Full backup and check the volume.</source>
        <translation>Das Full der Kette liegt auf Volume %1, das im Status Error ist: Führen Sie ein neues Full-Backup aus und prüfen Sie das Volume.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="681" />
        <source>%1 is on volume %2, which is %3: Bareos may overwrite it.</source>
        <translation>%1 liegt auf Volume %2, das %3 ist: Bareos kann es überschreiben.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="690" />
        <source>The job list hit its row limit: older jobs and copies may be missing from this check.</source>
        <translation>Die Job-Liste hat ihr Zeilenlimit erreicht: Ältere Jobs und Kopien fehlen in dieser Prüfung möglicherweise.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="696" />
        <source>%1 of 3 copies</source>
        <translation>%1 von 3 Kopien</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="697" />
        <source>partial copies: %1</source>
        <translation>Teilkopien: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="699" />
        <source>media types unknown</source>
        <translation>Medientypen unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="701" />
        <source>media: %1 of 2 (%2)</source>
        <translation>Medien: %1 von 2 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="704" />
        <source>off site: %1 of 1</source>
        <translation>extern: %1 von 1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="705" />
        <source>off site unknown</source>
        <translation>extern unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="706" />
        <source>nothing off site</source>
        <translation>nichts extern</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="859" />
        <source>met</source>
        <translation>erfüllt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="860" />
        <source>not met</source>
        <translation>nicht erfüllt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="863" />
        <location filename="../src/tools/b321verifier.cpp" line="895" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="869" />
        <source>Passes</source>
        <translation>Bestanden</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="870" />
        <source>Fails</source>
        <translation>Nicht bestanden</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="873" />
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="879" />
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="880" />
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="881" />
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="882" />
        <source>Virtual Full</source>
        <translation>Virtual Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="885" />
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="891" />
        <source>off site</source>
        <translation>extern</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="892" />
        <source>on site</source>
        <translation>vor Ort</translation>
    </message>
</context>
<context>
    <name>BAclMatrix</name>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="54" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="55" />
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="56" />
        <source>Storages</source>
        <translation>Storages</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="57" />
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="58" />
        <source>Pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="59" />
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="60" />
        <source>Catalogs</source>
        <translation>Catalogs</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="61" />
        <source>Commands</source>
        <translation>Befehle</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="68" />
        <source>Where</source>
        <translation>Where</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="69" />
        <source>Plugin Options</source>
        <translation>Plugin Options</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="83" />
        <source>The Director sent no reply.</source>
        <translation>Der Director hat nicht geantwortet.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="84" />
        <source>The Director did not answer with ACLs: %1</source>
        <translation>Der Director hat nicht mit ACLs geantwortet: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="93" />
        <source>The Director refused ".acl list": %1</source>
        <translation>Der Director hat ".acl list" abgelehnt: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="98" />
        <source>The reply holds no list of consoles ("acl").</source>
        <translation>Die Antwort enthält keine Liste der Konsolen ("acl").</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="309" />
        <location filename="../src/director/baclmatrix.cpp" line="381" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="316" />
        <source>Locked: the catalog ACL of the console could not be read; it may run status, .status, version and .api only.</source>
        <translation>Gesperrt: Die Katalog-ACL der Konsole konnte nicht gelesen werden; sie darf nur status, .status, version und .api ausführen.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325" />
        <source>Allowed (decided by the Director).</source>
        <translation>Erlaubt (vom Director entschieden).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325" />
        <location filename="../src/director/baclmatrix.cpp" line="338" />
        <source>Allowed by %1.</source>
        <translation>Erlaubt durch %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="328" />
        <source>The file ACL allows it (%1), the catalog ACL does not.</source>
        <translation>Die Datei-ACL erlaubt es (%1), die Katalog-ACL nicht.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="330" />
        <location filename="../src/director/baclmatrix.cpp" line="339" />
        <source>Denied by %1.</source>
        <translation>Verboten durch %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="333" />
        <location filename="../src/director/baclmatrix.cpp" line="341" />
        <source>The console has no ACL of this type: nothing is allowed.</source>
        <translation>Die Konsole hat keine ACL dieses Typs: nichts ist erlaubt.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="334" />
        <location filename="../src/director/baclmatrix.cpp" line="342" />
        <source>No entry of the ACL allows it.</source>
        <translation>Kein Eintrag der ACL erlaubt es.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="360" />
        <source>Console %1</source>
        <translation>Konsole %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="361" />
        <source>Profiles: none</source>
        <translation>Profile: keine</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="362" />
        <source>Profiles: %1</source>
        <translation>Profile: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="364" />
        <source>Locked: its catalog ACL could not be read.</source>
        <translation>Gesperrt: Ihre Katalog-ACL konnte nicht gelesen werden.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="367" />
        <source>loaded</source>
        <translation>geladen</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="368" />
        <source>off</source>
        <translation>aus</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="369" />
        <source>table missing</source>
        <translation>Tabelle fehlt</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="370" />
        <source>failed</source>
        <translation>fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="371" />
        <source>Catalog ACL: %1</source>
        <translation>Katalog-ACL: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="372" />
        <source>Catalog ACL: %1 (%2)</source>
        <translation>Katalog-ACL: %1 (%2)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrix.cpp" line="377" />
        <source>%n catalog ACL entry(s) narrow the file ACL.</source>
        <translation>
            <numerusform>%n Eintrag der Katalog-ACL schränkt die Datei-ACL ein.</numerusform>
            <numerusform>%n Einträge der Katalog-ACL schränken die Datei-ACL ein.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382" />
        <source>none</source>
        <translation>keine</translation>
    </message>
</context>
<context>
    <name>BAclMatrixDialog</name>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="56" />
        <source>Change the catalog ACL</source>
        <translation>Katalog-ACL ändern</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="58" />
        <source>These changes of the catalog ACL are sent to the Director, one after the other. The file ACL is not changed. The first change the Director refuses stops the rest.</source>
        <translation>Diese Änderungen der Katalog-ACL werden nacheinander an den Director gesendet. Die Datei-ACL bleibt unverändert. Die erste Änderung, die der Director ablehnt, stoppt den Rest.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="70" />
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="79" />
        <source>(no entries: no restriction)</source>
        <translation>(keine Einträge: keine Einschränkung)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="107" />
        <source>ACL matrix of the consoles</source>
        <translation>ACL-Matrix der Konsolen</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="113" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="115" />
        <source>Filter resources</source>
        <translation>Ressourcen filtern</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="118" />
        <source>Only rows where the consoles differ</source>
        <translation>Nur Zeilen, in denen sich die Konsolen unterscheiden</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="121" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="122" />
        <source>Reads the ACLs of every console again (.acl list)</source>
        <translation>Liest die ACLs aller Konsolen neu (.acl list)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="124" />
        <source>Reading the ACLs of the consoles...</source>
        <translation>Die ACLs der Konsolen werden gelesen...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="131" />
        <source>ACL matrix: resources by console</source>
        <translation>ACL-Matrix: Ressourcen je Konsole</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="147" />
        <source>✓ allowed   ✗ denied by an entry with "!"   · not allowed   ⊘ allowed by the file ACL, narrowed by the catalog ACL   — console locked. A cell's tool tip says why; a console's tool tip has its profiles, its catalog ACL and its Where and Plugin Options ACLs.</source>
        <translation>✓ erlaubt   ✗ verboten durch einen Eintrag mit "!"   · nicht erlaubt   ⊘ von der Datei-ACL erlaubt, von der Katalog-ACL eingeschränkt   — Konsole gesperrt. Der Tooltip einer Zelle sagt warum; der Tooltip einer Konsole zeigt ihre Profile, ihre Katalog-ACL und ihre Where- und Plugin-Options-ACLs.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="154" />
        <source>A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows it again. Changed cells are underlined until they are applied. What the file ACL denies is changed in the editor.</source>
        <translation>Ein Doppelklick (oder die Leertaste) auf ✓ verbietet es über die Katalog-ACL, auf ⊘ erlaubt es wieder. Geänderte Zellen sind unterstrichen, bis sie angewendet sind. Was die Datei-ACL verbietet, wird im Editor geändert.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="164" />
        <source>Discard</source>
        <translation>Verwerfen</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="165" />
        <source>Forgets the changes that were not applied</source>
        <translation>Vergisst die Änderungen, die nicht angewendet wurden</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="168" />
        <source>Apply...</source>
        <translation>Anwenden...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="169" />
        <source>Shows every change of the catalog ACL once and sends them after your confirmation (.acl set / .acl delete)</source>
        <translation>Zeigt jede Änderung der Katalog-ACL einmal und sendet sie nach deiner Bestätigung (.acl set / .acl delete)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="209" />
        <source>%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).</source>
        <translation>
            <numerusform>%n Konsole. Erlaubt ist, was der Director entschieden hat (Datei-ACL und Katalog-ACL).</numerusform>
            <numerusform>%n Konsolen. Erlaubt ist, was der Director entschieden hat (Datei-ACL und Katalog-ACL).</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="211" />
        <source>%n console(s). This Director reports the file ACL only; the GUI decided the cells from its entries.</source>
        <translation>
            <numerusform>%n Konsole. Dieser Director meldet nur die Datei-ACL; die GUI hat die Zellen aus ihren Einträgen bestimmt.</numerusform>
            <numerusform>%n Konsolen. Dieser Director meldet nur die Datei-ACL; die GUI hat die Zellen aus ihren Einträgen bestimmt.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="219" />
        <source>The ACLs could not be read: %1</source>
        <translation>Die ACLs konnten nicht gelesen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="258" />
        <source>Not applied yet: allowed again through the catalog ACL.</source>
        <translation>Noch nicht angewendet: über die Katalog-ACL wieder erlaubt.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="259" />
        <source>Not applied yet: denied through the catalog ACL.</source>
        <translation>Noch nicht angewendet: über die Katalog-ACL verboten.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="262" />
        <source>Double click: deny it through the catalog ACL.</source>
        <translation>Doppelklick: über die Katalog-ACL verbieten.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="263" />
        <source>Double click: allow it again.</source>
        <translation>Doppelklick: wieder erlauben.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="277" />
        <source>Console %1 is locked: its catalog ACL cannot be changed until it can be read.</source>
        <translation>Konsole %1 ist gesperrt: Ihre Katalog-ACL kann erst geändert werden, wenn sie gelesen werden kann.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="278" />
        <source>%1 is not allowed by the file ACL of console %2: that is changed in the editor, not here.</source>
        <translation>%1 ist von der Datei-ACL der Konsole %2 nicht erlaubt: Das wird im Editor geändert, nicht hier.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="311" />
        <source>The changes were discarded.</source>
        <translation>Die Änderungen wurden verworfen.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="325" />
        <source>Nothing was sent: the entry "%1" contains a comma, which the Director cannot store in a catalog ACL.</source>
        <translation>Nichts gesendet: Der Eintrag "%1" enthält ein Komma, das der Director in einer Katalog-ACL nicht speichern kann.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="335" />
        <source>Console %1, %2:</source>
        <translation>Konsole %1, %2:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="336" />
        <source>  now: %1</source>
        <translation>  jetzt: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="337" />
        <source>  new: %1</source>
        <translation>  neu: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="339" />
        <source>%1 of %2</source>
        <translation>%1 von %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="344" />
        <source>Nothing was sent: the changes were not confirmed.</source>
        <translation>Nichts gesendet: Die Änderungen wurden nicht bestätigt.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="356" />
        <source>Changed: %1.</source>
        <translation>Geändert: %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="359" />
        <source>Changing %1...</source>
        <translation>%1 wird geändert...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="368" />
        <source>refused</source>
        <translation>abgelehnt</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="379" />
        <source>nothing</source>
        <translation>nichts</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="380" />
        <source>Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.</source>
        <translation>Ändern von %1 fehlgeschlagen: %2. Vorher geändert: %3. Danach wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="423" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>BBvfsModel</name>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="234" />
        <source>Directory</source>
        <translation>Verzeichnis</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="235" />
        <source>Symlink</source>
        <translation>Symlink</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="236" />
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="299" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="300" />
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="301" />
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="302" />
        <source>Modified</source>
        <translation>Geändert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="468" />
        <source>JSON parse error: %1</source>
        <translation>JSON-Parse-Fehler: %1</translation>
    </message>
</context>
<context>
    <name>BCapabilities</name>
    <message>
        <location filename="../src/bcapabilities.cpp" line="33" />
        <source>Change an existing schedule</source>
        <translation>Bestehenden Schedule ändern</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="36" />
        <source>%1 cannot change an existing schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keinen bestehenden Schedule ändern: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="40" />
        <source>Nothing was changed. A schedule can also be changed in its configuration file on the Director.</source>
        <translation>Es wurde nichts geändert. Ein Schedule kann auch in seiner Konfigurationsdatei auf dem Director geändert werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="49" />
        <source>Prune and purge all volumes</source>
        <translation>Prune und Purge aller Volumes</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="52" />
        <source>%1 has no command that purges all volumes at once; purge works on named volumes only and ignores retention periods.</source>
        <translation>%1 hat keinen Befehl, der alle Volumes auf einmal purged; purge wirkt nur auf benannte Volumes und ignoriert Aufbewahrungsfristen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="55" />
        <source>"Prune only" applies the retention period of every volume; a single volume can be purged by its name with the purge command.</source>
        <translation>„Nur Prune“ wendet die Aufbewahrungsfrist jedes Volumes an; ein einzelnes Volume kann mit dem purge-Befehl über seinen Namen gepurged werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="62" />
        <source>Send a FileSet to the Director</source>
        <translation>FileSet an den Director senden</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="65" />
        <source>%1 cannot take a FileSet through the console, because its configure command does not accept nested blocks (Include, Options, Exclude).</source>
        <translation>%1 kann kein FileSet über die Konsole annehmen, da sein configure-Befehl keine verschachtelten Blöcke (Include, Options, Exclude) akzeptiert.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="68" />
        <source>You can copy the configuration text and place it on the Director yourself.</source>
        <translation>Sie können den Konfigurationstext kopieren und selbst auf dem Director ablegen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="75" />
        <source>Delete a schedule</source>
        <translation>Schedule löschen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="78" />
        <source>%1 cannot delete a schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keinen Schedule löschen: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="82" />
        <source>Nothing was deleted. A schedule can also be removed from the configuration files on the Director.</source>
        <translation>Es wurde nichts gelöscht. Ein Schedule kann auch aus den Konfigurationsdateien auf dem Director entfernt werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="91" />
        <source>Create a schedule</source>
        <translation>Schedule anlegen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="94" />
        <source>%1 cannot create a schedule.</source>
        <translation>%1 kann keinen Schedule anlegen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="106" />
        <source>Change an existing job</source>
        <translation>Bestehenden Job ändern</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="109" />
        <source>%1 cannot change an existing job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keinen bestehenden Job ändern: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="113" />
        <source>Nothing was changed. A job can also be changed in its configuration file on the Director.</source>
        <translation>Es wurde nichts geändert. Ein Job kann auch in seiner Konfigurationsdatei auf dem Director geändert werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="122" />
        <source>Delete a job</source>
        <translation>Job löschen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="125" />
        <source>%1 cannot delete a job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keinen Job löschen: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="128" />
        <source>Nothing was deleted. A job can also be removed from the configuration files on the Director.</source>
        <translation>Es wurde nichts gelöscht. Ein Job kann auch aus den Konfigurationsdateien auf dem Director entfernt werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="137" />
        <source>Create a job</source>
        <translation>Job anlegen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="140" />
        <source>%1 cannot create a job.</source>
        <translation>%1 kann keinen Job anlegen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="148" />
        <source>Change an existing JobDefs</source>
        <translation>Bestehende JobDefs ändern</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="151" />
        <source>%1 cannot change an existing JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keine bestehenden JobDefs ändern: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="155" />
        <source>Nothing was changed. A JobDefs can also be changed in its configuration file on the Director.</source>
        <translation>Es wurde nichts geändert. JobDefs können auch in ihrer Konfigurationsdatei auf dem Director geändert werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="164" />
        <source>Delete a JobDefs</source>
        <translation>JobDefs löschen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="167" />
        <source>%1 cannot delete a JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 kann auf dieser Verbindung keine JobDefs löschen: Dafür werden die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird) benötigt.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="171" />
        <source>Nothing was deleted. A JobDefs can also be removed from the configuration files on the Director.</source>
        <translation>Es wurde nichts gelöscht. JobDefs können auch aus den Konfigurationsdateien auf dem Director entfernt werden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="180" />
        <source>Create a JobDefs</source>
        <translation>JobDefs anlegen</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="183" />
        <source>%1 cannot create a JobDefs.</source>
        <translation>%1 kann keine JobDefs anlegen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="309" />
        <source>This Director (Bareos)</source>
        <translation>Dieser Director (Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="310" />
        <source>This Director (Bareos %1)</source>
        <translation>Dieser Director (Bareos %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="314" />
        <source>This Director (Onesimus Director)</source>
        <translation>Dieser Director (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="316" />
        <source>This Director (Onesimus Director %1)</source>
        <translation>Dieser Director (Onesimus Director %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="322" />
        <source>This Director</source>
        <translation>Dieser Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="334" />
        <source>%1 does not offer this to this session: its configuration capabilities do not list it. The console needs the command ACL "configure" and the ACL of the resource type, or the Director is older than its configuration methods.</source>
        <translation>%1 bietet dies dieser Sitzung nicht an: Seine Konfigurationsfähigkeiten führen es nicht auf. Die Console benötigt die Befehls-ACL „configure“ und die ACL des Ressourcentyps, oder der Director ist älter als seine Konfigurationsmethoden.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="343" />
        <source>The capabilities query failed: %1.</source>
        <translation>Die Abfrage der Fähigkeiten ist fehlgeschlagen: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="347" />
        <source>The capabilities were not received for this session.</source>
        <translation>Die Fähigkeiten wurden für diese Sitzung nicht empfangen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="356" />
        <source>The Director lists for this session: %1.</source>
        <translation>Der Director führt für diese Sitzung auf: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="372" />
        <source>The Onesimus Director supports this over its gRPC interface.</source>
        <translation>Der Onesimus Director unterstützt dies über seine gRPC-Schnittstelle.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="375" />
        <source>Connect to it over gRPC to use this.</source>
        <translation>Verbinden Sie sich über gRPC mit ihm, um dies zu nutzen.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="382" />
        <source>The Onesimus Director supports this.</source>
        <translation>Der Onesimus Director unterstützt dies.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="387" />
        <source>Support for this is planned for a later version of the Onesimus Director.</source>
        <translation>Die Unterstützung dafür ist für eine spätere Version des Onesimus Directors geplant.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="390" />
        <source>The Onesimus Director is planned to support this.</source>
        <translation>Der Onesimus Director soll dies künftig unterstützen.</translation>
    </message>
</context>
<context>
    <name>BCapabilityNotice</name>
    <message>
        <location filename="../src/bcapabilitynotice.cpp" line="65" />
        <source>Do not show again</source>
        <translation>Nicht mehr anzeigen</translation>
    </message>
</context>
<context>
    <name>BCatalogResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>DB Driver</source>
        <translation>DB-Treiber</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>DB Name</source>
        <translation>DB-Name</translation>
    </message>
</context>
<context>
    <name>BCertificateGenerator</name>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="36" />
        <source>Generate TLS Certificates</source>
        <translation>TLS-Zertifikate erzeugen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="48" />
        <source>Generate TLS certificates for secure Bareos communication.
This will create a CA certificate, client certificate, and private key.</source>
        <translation>TLS-Zertifikate für sichere Bareos-Kommunikation erzeugen.
Dabei werden ein CA-Zertifikat, ein Client-Zertifikat und ein privater Schlüssel erstellt.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="56" />
        <source>Certificate Settings</source>
        <translation>Zertifikatseinstellungen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="64" />
        <source>Browse...</source>
        <translation>Durchsuchen...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="67" />
        <source>Output Directory:</source>
        <translation>Ausgabeverzeichnis:</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="72" />
        <source>e.g., onesimus-client</source>
        <translation>z. B. onesimus-client</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="73" />
        <source>Common Name (CN):</source>
        <translation>Common Name (CN):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="78" />
        <source>e.g., My Company</source>
        <translation>z. B. Meine Firma</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="79" />
        <source>Organization (O):</source>
        <translation>Organisation (O):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="84" />
        <source>Days (e.g., 3650 = 10 years)</source>
        <translation>Tage (z. B. 3650 = 10 Jahre)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="85" />
        <source>Valid for (days):</source>
        <translation>Gültig für (Tage):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="88" />
        <source>Overwrite existing files</source>
        <translation>Vorhandene Dateien überschreiben</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="113" />
        <source>Generate Certificates</source>
        <translation>Zertifikate erzeugen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="115" />
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="129" />
        <source>Select Output Directory</source>
        <translation>Ausgabeverzeichnis wählen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="164" />
        <source>Failed to create key context: %1</source>
        <translation>Schlüsselkontext konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="169" />
        <source>Failed to init keygen: %1</source>
        <translation>Schlüsselerzeugung konnte nicht initialisiert werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="175" />
        <source>Failed to set key bits: %1</source>
        <translation>Schlüssellänge konnte nicht gesetzt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="181" />
        <source>Failed to generate key: %1</source>
        <translation>Schlüssel konnte nicht erzeugt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="197" />
        <source>Failed to create X509: %1</source>
        <translation>X509 konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="203" />
        <source>Failed to set X509 version</source>
        <translation>X509-Version konnte nicht gesetzt werden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="211" />
        <source>Failed to create serial number</source>
        <translation>Seriennummer konnte nicht erstellt werden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="225" />
        <source>Failed to set public key</source>
        <translation>Öffentlicher Schlüssel konnte nicht gesetzt werden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="233" />
        <source>Failed to create X509_NAME</source>
        <translation>X509_NAME konnte nicht erstellt werden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="244" />
        <source>Failed to add Organization to subject: %1</source>
        <translation>Organisation konnte nicht zum Subject hinzugefügt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="252" />
        <source>Failed to add Common Name to subject: %1</source>
        <translation>Common Name konnte nicht zum Subject hinzugefügt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="260" />
        <source>Failed to set subject name: %1</source>
        <translation>Subject-Name konnte nicht gesetzt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="270" />
        <location filename="../src/bcertificategenerator.cpp" line="278" />
        <source>Failed to set issuer name: %1</source>
        <translation>Issuer-Name konnte nicht gesetzt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="311" />
        <source>Failed to sign certificate: %1</source>
        <translation>Zertifikat konnte nicht signiert werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="325" />
        <location filename="../src/bcertificategenerator.cpp" line="346" />
        <location filename="../src/bcertificategenerator.cpp" line="384" />
        <source>Cannot open file for writing: %1</source>
        <translation>Datei kann nicht zum Schreiben geöffnet werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="333" />
        <source>Failed to write private key: %1</source>
        <translation>Privater Schlüssel konnte nicht geschrieben werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="354" />
        <source>Failed to write certificate: %1</source>
        <translation>Zertifikat konnte nicht geschrieben werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="376" />
        <source>Failed to create PKCS12: %1</source>
        <translation>PKCS12 konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="394" />
        <source>Failed to write PFX file: %1</source>
        <translation>PFX-Datei konnte nicht geschrieben werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="409" />
        <source>Generating...</source>
        <translation>Erzeuge...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="418" />
        <source>Please fill in all required fields</source>
        <translation>Bitte füllen Sie alle Pflichtfelder aus</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="420" />
        <location filename="../src/bcertificategenerator.cpp" line="434" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="432" />
        <source>Failed to create output directory</source>
        <translation>Ausgabeverzeichnis konnte nicht erstellt werden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="450" />
        <source>Certificate files already exist. Enable 'Overwrite' to replace.</source>
        <translation>Zertifikatsdateien sind bereits vorhanden. Aktivieren Sie „Überschreiben“, um sie zu ersetzen.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="452" />
        <source>Files exist</source>
        <translation>Dateien vorhanden</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="461" />
        <source>Generating CA private key (4096 bit RSA)...</source>
        <translation>Erzeuge privaten CA-Schlüssel (4096 Bit RSA)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="465" />
        <location filename="../src/bcertificategenerator.cpp" line="476" />
        <location filename="../src/bcertificategenerator.cpp" line="487" />
        <location filename="../src/bcertificategenerator.cpp" line="496" />
        <location filename="../src/bcertificategenerator.cpp" line="509" />
        <location filename="../src/bcertificategenerator.cpp" line="522" />
        <location filename="../src/bcertificategenerator.cpp" line="535" />
        <location filename="../src/bcertificategenerator.cpp" line="546" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="472" />
        <source>Saving CA private key...</source>
        <translation>Speichere privaten CA-Schlüssel...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="482" />
        <source>Generating CA certificate...</source>
        <translation>Erzeuge CA-Zertifikat...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="503" />
        <source>Generating client private key (4096 bit RSA)...</source>
        <translation>Erzeuge privaten Client-Schlüssel (4096 Bit RSA)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="516" />
        <source>Saving client private key...</source>
        <translation>Speichere privaten Client-Schlüssel...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="528" />
        <source>Generating client certificate...</source>
        <translation>Erzeuge Client-Zertifikat...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="554" />
        <source>Creating PFX file for Windows...</source>
        <translation>Erstelle PFX-Datei für Windows...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="556" />
        <source>PFX creation failed (non-critical)</source>
        <translation>PFX-Erstellung fehlgeschlagen (nicht kritisch)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="569" />
        <source>Certificates generated successfully!</source>
        <translation>Zertifikate erfolgreich erzeugt!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="570" />
        <source>CA Certificate: %1</source>
        <translation>CA-Zertifikat: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="571" />
        <source>Client Certificate: %1</source>
        <translation>Client-Zertifikat: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="572" />
        <source>Private Key: %1</source>
        <translation>Privater Schlüssel: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="574" />
        <source>PFX File: %1</source>
        <translation>PFX-Datei: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="575" />
        <source>PFX Password: (empty - no password)</source>
        <translation>PFX-Passwort: (leer – kein Passwort)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="578" />
        <source>Success!</source>
        <translation>Erfolgreich!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="581" />
        <source>Use Certificates</source>
        <translation>Zertifikate verwenden</translation>
    </message>
</context>
<context>
    <name>BCheckableHeaderView</name>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="118" />
        <source>Show All Columns</source>
        <translation>Alle Spalten anzeigen</translation>
    </message>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="128" />
        <source>Reset Columns</source>
        <translation>Spalten zurücksetzen</translation>
    </message>
</context>
<context>
    <name>BCleanupDialog</name>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="39" />
        <source>Database Cleanup</source>
        <translation>Datenbankbereinigung</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="98" />
        <source>This tool helps maintain your Bareos database by removing old or unnecessary data.
⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding.</source>
        <translation>Dieses Werkzeug hilft bei der Pflege Ihrer Bareos-Datenbank, indem es alte oder unnötige Daten entfernt.
⚠️ Warnung: Bereinigungen können nicht rückgängig gemacht werden. Prüfen Sie die Optionen sorgfältig, bevor Sie fortfahren.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="108" />
        <source>Cleanup Options</source>
        <translation>Bereinigungsoptionen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="113" />
        <source>Remove old full backups</source>
        <translation>Alte Full-Backups entfernen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="121" />
        <source>Keep last:</source>
        <translation>Behalte die letzten:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="128" />
        <source> full backups</source>
        <translation> Full-Backups</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="136" />
        <source>Remove successful jobs with no files</source>
        <translation>Erfolgreiche Jobs ohne Dateien entfernen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="138" />
        <source>Delete jobs that completed successfully but backed up 0 files</source>
        <translation>Jobs löschen, die erfolgreich abgeschlossen wurden, aber 0 Dateien gesichert haben</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="143" />
        <source>Prune volumes</source>
        <translation>Volumes prunen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="145" />
        <source>Apply retention policy to volumes (removes expired job records)</source>
        <translation>Aufbewahrungsrichtlinie auf Volumes anwenden (entfernt abgelaufene Job-Einträge)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="148" />
        <source>Action:</source>
        <translation>Aktion:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="152" />
        <source>Prune only</source>
        <translation>Nur Prune</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="153" />
        <source>Prune and purge</source>
        <translation>Prune und Purge</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="180" />
        <source>Remove failed jobs older than</source>
        <translation>Fehlgeschlagene Jobs entfernen, älter als</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="182" />
        <source>Delete jobs that failed or were canceled</source>
        <translation>Jobs löschen, die fehlgeschlagen sind oder abgebrochen wurden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="189" />
        <source> days</source>
        <translation> Tage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="211" />
        <source>Select cleanup options and click 'Analyze' to preview</source>
        <translation>Wählen Sie Bereinigungsoptionen und klicken Sie auf „Analysieren“ für eine Vorschau</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="216" />
        <source>Analysis / Log</source>
        <translation>Analyse / Protokoll</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="234" />
        <source>Analyze</source>
        <translation>Analysieren</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="235" />
        <source>Preview what would be cleaned up</source>
        <translation>Vorschau, was bereinigt würde</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="239" />
        <source>Cleanup</source>
        <translation>Bereinigen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="241" />
        <source>Perform cleanup operations (requires analysis first)</source>
        <translation>Bereinigung ausführen (erfordert vorherige Analyse)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="394" />
        <source>- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)
</source>
        <translation>- %1 alte Full-Backup-Jobs löschen (%2 je Client, Job und FileSet bleiben erhalten)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="400" />
        <source>- Delete %1 empty jobs (successful with 0 files)
</source>
        <translation>- %1 leere Jobs löschen (erfolgreich mit 0 Dateien)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="405" />
        <source>- Delete %1 failed jobs older than %2 days
</source>
        <translation>- %1 fehlgeschlagene Jobs löschen, die älter als %2 Tage sind
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="412" />
        <source>  Job records to delete in total: %1
</source>
        <translation>  Insgesamt zu löschende Job-Einträge: %1
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="416" />
        <source>- Prune all volumes (command: %1)
</source>
        <translation>- Alle Volumes prunen (Befehl: %1)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="432" />
        <source>The job list changed in the meantime. Nothing was deleted; please run the analysis again.</source>
        <translation>Die Job-Liste hat sich inzwischen geändert. Es wurde nichts gelöscht; bitte führen Sie die Analyse erneut aus.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="445" />
        <source>Stopping: no further commands are sent after the answer that is still awaited.</source>
        <translation>Wird angehalten: Nach der noch ausstehenden Antwort werden keine weiteren Befehle gesendet.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="475" />
        <source>⚠ No job data available!</source>
        <translation>⚠ Keine Job-Daten verfügbar!</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="476" />
        <source>Please load the job list first (click 'Refresh' in the main window).</source>
        <translation>Bitte laden Sie zuerst die Job-Liste (im Hauptfenster auf „Aktualisieren“ klicken).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="478" />
        <source>No job data. Please refresh job list first.</source>
        <translation>Keine Job-Daten. Bitte aktualisieren Sie zuerst die Job-Liste.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="485" />
        <source>Looking at the %1 jobs currently loaded in the job list. Jobs that are not loaded there are not considered.</source>
        <translation>Betrachtet werden die %1 Jobs, die derzeit in der Job-Liste geladen sind. Jobs, die dort nicht geladen sind, werden nicht berücksichtigt.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="502" />
        <source>✓ Will prune all volumes (apply each volume's retention period)</source>
        <translation>✓ Alle Volumes werden geprunt (Aufbewahrungsfrist jedes Volumes wird angewendet)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="503" />
        <source>  Command: %1</source>
        <translation>  Befehl: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="505" />
        <source>  'Prune and purge' is not available and will not be sent.</source>
        <translation>  „Prune und Purge“ ist nicht verfügbar und wird nicht gesendet.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="510" />
        <source>=== Analysis Summary ===</source>
        <translation>=== Zusammenfassung der Analyse ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="514" />
        <source>Total jobs to delete: %1</source>
        <translation>Zu löschende Jobs insgesamt: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="516" />
        <source>  - Old full backups: %1 (%2)</source>
        <translation>  - Alte Full-Backups: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="521" />
        <source>  - Empty jobs: %1</source>
        <translation>  - Leere Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="524" />
        <source>  - Failed jobs: %1</source>
        <translation>  - Fehlgeschlagene Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="527" />
        <source>No jobs found matching cleanup criteria.</source>
        <translation>Keine Jobs gefunden, die den Bereinigungskriterien entsprechen.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="536" />
        <source>Analysis complete. Nothing to clean up.</source>
        <translation>Analyse abgeschlossen. Nichts zu bereinigen.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="550" />
        <source>Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...</source>
        <translation>Analysiere alte Full-Backups (die %1 neuesten je Client, Job und FileSet bleiben erhalten)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="558" />
        <source>at least one full backup must be kept</source>
        <translation>mindestens ein Full-Backup muss behalten werden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="561" />
        <source>%1 job rows have no job ID</source>
        <translation>%1 Job-Zeilen haben keine Job-ID</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="564" />
        <source>%1 job rows have no job type, so backups cannot be told from other jobs</source>
        <translation>%1 Job-Zeilen haben keinen Job-Typ, daher können Backups nicht von anderen Jobs unterschieden werden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="568" />
        <source>%1 backup jobs have no client or job name</source>
        <translation>%1 Backup-Jobs haben keinen Client- oder Job-Namen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="573" />
        <source>  ⚠ No old full backups are selected: %1.</source>
        <translation>  ⚠ Es sind keine alten Full-Backups ausgewählt: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="577" />
        <source>  Note: the FileSet of a job is taken from its current job configuration, not from the catalog record of each run.</source>
        <translation>  Hinweis: Das FileSet eines Jobs wird aus seiner aktuellen Job-Konfiguration übernommen, nicht aus dem Katalogeintrag jedes Laufs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="584" />
        <source>%1 runs have no known FileSet</source>
        <translation>%1 Läufe haben kein bekanntes FileSet</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="587" />
        <source>%1 runs have no status</source>
        <translation>%1 Läufe haben keinen Status</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="590" />
        <source>%1 runs have a level other than Full, Differential or Incremental</source>
        <translation>%1 Läufe haben ein anderes Level als Full, Differential oder Incremental</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="593" />
        <source>%1 runs have no readable start time</source>
        <translation>%1 Läufe haben keine lesbare Startzeit</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="596" />
        <source>  ⚠ Client '%1', job '%2': skipped, nothing selected (%3)</source>
        <translation>  ⚠ Client „%1“, Job „%2“: übersprungen, nichts ausgewählt (%3)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="602" />
        <source>  Client '%1', job '%2', FileSet '%3': %4 full backups, will delete %5</source>
        <translation>  Client „%1“, Job „%2“, FileSet „%3“: %4 Full-Backups, %5 werden gelöscht</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="609" />
        <source>    ... and %1 more</source>
        <translation>    ... und %1 weitere</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="613" />
        <source>    → Job %1: %2 (%3, %4)</source>
        <translation>    → Job %1: %2 (%3, %4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="616" />
        <source>      its dependent jobs %1 are deleted by this cleanup as well</source>
        <translation>      seine abhängigen Jobs %1 werden durch diese Bereinigung ebenfalls gelöscht</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="622" />
        <source>    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it</source>
        <translation>    ⚠ Job %1 (%2) wird behalten: Incremental-/Differential-Jobs %3 hängen davon ab</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="629" />
        <source>✓ Found %1 old full backups to delete (%2)</source>
        <translation>✓ %1 alte Full-Backups zum Löschen gefunden (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="633" />
        <source>  No old full backups found to delete</source>
        <translation>  Keine alten Full-Backups zum Löschen gefunden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="639" />
        <source>Analyzing empty jobs (successful with 0 files)...</source>
        <translation>Analysiere leere Jobs (erfolgreich mit 0 Dateien)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="644" />
        <source>  → Job %1: %2 [%3] (%4)</source>
        <translation>  → Job %1: %2 [%3] (%4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="649" />
        <location filename="../src/bcleanupdialog.cpp" line="687" />
        <source>  ... and %1 more</source>
        <translation>  ... und %1 weitere</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="653" />
        <source>  %1 successful jobs have no file count and are not selected</source>
        <translation>  %1 erfolgreiche Jobs haben keine Dateianzahl und werden nicht ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="658" />
        <source>✓ Found %1 empty jobs to delete</source>
        <translation>✓ %1 leere Jobs zum Löschen gefunden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="660" />
        <source>  No empty jobs found</source>
        <translation>  Keine leeren Jobs gefunden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="668" />
        <source>Analyzing failed jobs older than %1 days (before %2)...</source>
        <translation>Analysiere fehlgeschlagene Jobs, die älter als %1 Tage sind (vor %2)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="677" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="678" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="679" />
        <source>Canceled</source>
        <translation>Abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="680" />
        <source>Non-fatal error</source>
        <translation>Nicht schwerwiegender Fehler</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="682" />
        <source>  → Job %1: %2 [%3] (%4) - %5</source>
        <translation>  → Job %1: %2 [%3] (%4) - %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="691" />
        <source>✓ Found %1 failed jobs to delete</source>
        <translation>✓ %1 fehlgeschlagene Jobs zum Löschen gefunden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="693" />
        <source>  No failed jobs found matching criteria</source>
        <translation>  Keine fehlgeschlagenen Jobs gefunden, die den Kriterien entsprechen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702" />
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="727" />
        <source>Cleanup running: commands are sent one at a time...</source>
        <translation>Bereinigung läuft: Befehle werden einzeln gesendet...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="737" />
        <source>Sent: %1</source>
        <translation>Gesendet: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="749" />
        <source>(empty answer)</source>
        <translation>(leere Antwort)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="754" />
        <source>✓ Job %1: deletion confirmed by the Director</source>
        <translation>✓ Job %1: Löschung vom Director bestätigt</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="757" />
        <source>✗ Job %1: the Director reported an error: %2</source>
        <translation>✗ Job %1: Der Director meldete einen Fehler: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="760" />
        <source>? Job %1: the Director did not confirm the deletion. Answer: %2</source>
        <translation>? Job %1: Der Director hat die Löschung nicht bestätigt. Antwort: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="764" />
        <source>? Job %1: result unknown (%2)</source>
        <translation>? Job %1: Ergebnis unbekannt (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="772" />
        <source>Prune volumes: the Director answered without an error: %1</source>
        <translation>Volumes prunen: Der Director antwortete ohne Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="775" />
        <source>✗ Prune volumes: the Director reported an error: %1</source>
        <translation>✗ Volumes prunen: Der Director meldete einen Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="778" />
        <source>? Prune volumes: result unknown (%1)</source>
        <translation>? Volumes prunen: Ergebnis unbekannt (%1)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="781" />
        <source>? Prune volumes: no usable answer: %1</source>
        <translation>? Volumes prunen: keine verwertbare Antwort: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="791" />
        <source>no answer from the Director within the time limit</source>
        <translation>keine Antwort vom Director innerhalb der Zeitgrenze</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="806" />
        <source>=== Cleanup Result ===</source>
        <translation>=== Ergebnis der Bereinigung ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="807" />
        <source>Job deletions confirmed by the Director: %1</source>
        <translation>Vom Director bestätigte Job-Löschungen: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="810" />
        <source>Prune volumes: answered without an error (see the answer above)</source>
        <translation>Volumes prunen: ohne Fehler beantwortet (siehe Antwort oben)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="813" />
        <source>Commands the Director answered with an error: %1</source>
        <translation>Befehle, die der Director mit einem Fehler beantwortet hat: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="816" />
        <source>Commands with an unknown result: %1. Refresh the job list to see the actual state.</source>
        <translation>Befehle mit unbekanntem Ergebnis: %1. Aktualisieren Sie die Job-Liste, um den tatsächlichen Stand zu sehen.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="820" />
        <source>Commands not sent because the run was stopped: %1</source>
        <translation>Befehle, die nicht gesendet wurden, weil der Lauf angehalten wurde: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="824" />
        <source>Cleanup finished: the Director confirmed all %1 commands.</source>
        <translation>Bereinigung abgeschlossen: Der Director hat alle %1 Befehle bestätigt.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="827" />
        <source>Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. See the log.</source>
        <translation>Bereinigung nicht abgeschlossen: %1 bestätigt, %2 fehlgeschlagen, %3 unbekannt, %4 nicht gesendet. Siehe Protokoll.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="353" />
        <location filename="../src/bcleanupdialog.cpp" line="714" />
        <source>Not Connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="76" />
        <source>The connection to the Director was lost</source>
        <translation>Die Verbindung zum Director wurde unterbrochen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="116" />
        <source>Delete old successful full backups of a job if newer full backups of the same client, job and FileSet exist. A full backup that incremental or differential backups still depend on is kept.</source>
        <translation>Alte erfolgreiche Full-Backups eines Jobs löschen, wenn neuere Full-Backups desselben Clients, Jobs und FileSets existieren. Ein Full-Backup, von dem noch Incremental- oder Differential-Backups abhängen, wird behalten.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="129" />
        <source>Number of recent full backups to keep per client, job and FileSet</source>
        <translation>Anzahl der neuesten Full-Backups, die je Client, Job und FileSet behalten werden</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="154" />
        <source>Prune removes the expired job records of all volumes according to their retention period</source>
        <translation>Prune entfernt die abgelaufenen Job-Einträge aller Volumes gemäß ihrer Aufbewahrungsfrist</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="164" />
        <source>Not available: Bareos has no command that purges all volumes at once. Purge works on named volumes only and ignores retention periods, so it is not sent from this dialog.</source>
        <translation>Nicht verfügbar: Bareos hat keinen Befehl, der alle Volumes auf einmal purged. Purge wirkt nur auf benannte Volumes und ignoriert Aufbewahrungsfristen, daher wird es von diesem Dialog nicht gesendet.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="247" />
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="333" />
        <source>The options were changed. The analysis above is no longer valid; click 'Analyze' again.</source>
        <translation>Die Optionen wurden geändert. Die obige Analyse ist nicht mehr gültig; klicken Sie erneut auf „Analysieren“.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="342" />
        <source>The job list has changed. The analysis above is no longer valid; click 'Analyze' again.</source>
        <translation>Die Job-Liste hat sich geändert. Die obige Analyse ist nicht mehr gültig; klicken Sie erneut auf „Analysieren“.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="354" />
        <location filename="../src/bcleanupdialog.cpp" line="715" />
        <source>Please connect to Bareos Director first.</source>
        <translation>Bitte verbinden Sie sich zuerst mit dem Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="360" />
        <source>No Options Selected</source>
        <translation>Keine Optionen ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="361" />
        <source>Please select at least one cleanup option.</source>
        <translation>Bitte wählen Sie mindestens eine Bereinigungsoption.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="369" />
        <source>=== Analysis ===</source>
        <translation>=== Analyse ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="382" />
        <location filename="../src/bcleanupdialog.cpp" line="431" />
        <source>Analysis Required</source>
        <translation>Analyse erforderlich</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="383" />
        <source>Please run analysis before cleanup.</source>
        <translation>Bitte führen Sie vor der Bereinigung eine Analyse aus.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="390" />
        <source>⚠️ This will permanently delete data from the Bareos catalog.

</source>
        <translation>⚠️ Dadurch werden Daten dauerhaft aus dem Bareos-Katalog gelöscht.

</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="391" />
        <source>Operations to perform:
</source>
        <translation>Auszuführende Vorgänge:
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="419" />
        <source>
Do you want to continue?</source>
        <translation>
Möchten Sie fortfahren?</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="422" />
        <source>Confirm Cleanup</source>
        <translation>Bereinigung bestätigen</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="470" />
        <source>Analyzing cleanup options...</source>
        <translation>Analysiere Bereinigungsoptionen...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="534" />
        <source>Analysis complete. Click 'Cleanup' to proceed.</source>
        <translation>Analyse abgeschlossen. Klicken Sie auf „Bereinigen“, um fortzufahren.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702" />
        <source>Stop</source>
        <translation>Anhalten</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="720" />
        <source>=== Starting Cleanup ===</source>
        <translation>=== Bereinigung wird gestartet ===</translation>
    </message>
</context>
<context>
    <name>BClientDetailsDialog</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="189" />
        <source>Client Details - %1</source>
        <translation>Client Details - %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="330" />
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="216" />
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="316" />
        <source>General Information</source>
        <translation>Allgemeine Informationen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="323" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="325" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="395" />
        <source>N/A</source>
        <translation>k. A.</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="331" />
        <source>Address:</source>
        <translation>Adresse:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="332" />
        <source>Port:</source>
        <translation>Port:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="333" />
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="334" />
        <source>Operating System:</source>
        <translation>Betriebssystem:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="339" />
        <source>Connection Information</source>
        <translation>Verbindungsinformationen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="353" />
        <source>Recent (%1)</source>
        <translation>Aktuell (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="358" />
        <source>Stale (%1)</source>
        <translation>Veraltet (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="362" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="370" />
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="365" />
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="383" />
        <source>Last backup:</source>
        <translation>Letztes Backup:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="384" />
        <source>Last Connection:</source>
        <translation>Letzte Verbindung:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="389" />
        <source>Retention Policies</source>
        <translation>Aufbewahrungsrichtlinien</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394" />
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394" />
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="399" />
        <source>Auto Prune:</source>
        <translation>Auto Prune:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="400" />
        <source>File Retention:</source>
        <translation>File Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="401" />
        <source>Job Retention:</source>
        <translation>Job Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="407" />
        <source>Information</source>
        <translation>Informationen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="426" />
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="459" />
        <source>Recent jobs for this client:</source>
        <translation>Letzte Jobs dieses Clients:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="463" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="502" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="525" />
        <source>Statistics</source>
        <translation>Statistik</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="511" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="512" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="514" />
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="516" />
        <source>Total Jobs:</source>
        <translation>Jobs gesamt:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="517" />
        <source>Successful Jobs:</source>
        <translation>Erfolgreiche Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="518" />
        <source>Failed Jobs:</source>
        <translation>Fehlgeschlagene Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="519" />
        <source>Total Data:</source>
        <translation>Daten gesamt:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="520" />
        <source>Total Files:</source>
        <translation>Dateien gesamt:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="494" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
</context>
<context>
    <name>BClientJobsModel</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="79" />
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="80" />
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="81" />
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="90" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="91" />
        <source>Running</source>
        <translation>Läuft</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="92" />
        <source>Blocked</source>
        <translation>Blockiert</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="93" />
        <source>Created</source>
        <translation>Erstellt</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="94" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="95" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="96" />
        <source>Canceled</source>
        <translation>Abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="97" />
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="126" />
        <source>Job ID</source>
        <translation>Job-ID</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="127" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="128" />
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="129" />
        <source>Duration</source>
        <translation>Dauer</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="130" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="131" />
        <source>Files</source>
        <translation>Dateien</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="132" />
        <source>Bytes</source>
        <translation>Bytes</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="133" />
        <source>Status</source>
        <translation>Status</translation>
    </message>
</context>
<context>
    <name>BClientResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196" />
        <source>Catalog</source>
        <translation>Catalog</translation>
    </message>
</context>
<context>
    <name>BClientWidget</name>
    <message>
        <location filename="../ui/bclientwidget.ui" line="14" />
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="29" />
        <source>Show state</source>
        <translation>Zustand anzeigen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="30" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
</context>
<context>
    <name>BClientsModel</name>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="132" />
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="193" />
        <source>Client: %1</source>
        <translation>Client: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="197" />
        <source>Address: %1:%2</source>
        <translation>Adresse: %1:%2</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="207" />
        <source>%1 bytes</source>
        <translation>%1 Bytes</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="226" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="228" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="19" />
        <source>No connection to Director</source>
        <translation>Keine Verbindung zum Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="24" />
        <source>Loading clients...</source>
        <translation>Lade Clients...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="201" />
        <source>FD Port: %1</source>
        <translation>FD-Port: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="230" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="232" />
        <source>Last Backup</source>
        <translation>Letztes Backup</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="234" />
        <source>Operating System</source>
        <translation>Betriebssystem</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="236" />
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="238" />
        <source>Last Connection</source>
        <translation>Letzte Verbindung</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="240" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="242" />
        <source>Total Data</source>
        <translation>Gesamtdaten</translation>
    </message>
</context>
<context>
    <name>BClientsWidget</name>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="35" />
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="36" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="57" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="60" />
        <source>Filter by client...</source>
        <translation>Nach Client filtern...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="67" />
        <source>Last backup:</source>
        <translation>Letztes Backup:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="68" />
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="69" />
        <source>Recent</source>
        <translation>Aktuell</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="70" />
        <source>Stale</source>
        <translation>Veraltet</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="71" />
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="72" />
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="79" />
        <source>Enable automatic refresh</source>
        <translation>Automatische Aktualisierung aktivieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="92" />
        <source>Statistics</source>
        <translation>Statistik</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="95" />
        <source>Total:</source>
        <translation>Gesamt:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="116" />
        <source>Recent:</source>
        <translation>Aktuell:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="117" />
        <source>Clients whose last backup or contact is younger than the limit in the settings</source>
        <translation>Clients, deren letztes Backup oder letzter Kontakt jünger ist als die Grenze in den Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="119" />
        <source>Stale:</source>
        <translation>Veraltet:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="120" />
        <source>Clients whose last backup or contact is older than the limit in the settings</source>
        <translation>Clients, deren letztes Backup oder letzter Kontakt älter ist als die Grenze in den Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="122" />
        <source>Never:</source>
        <translation>Nie:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="123" />
        <source>Clients without any known backup or contact</source>
        <translation>Clients ohne bekanntes Backup oder bekannten Kontakt</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="125" />
        <source>Unknown:</source>
        <translation>Unbekannt:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="126" />
        <source>Clients whose last backup time cannot be read</source>
        <translation>Clients, deren letzte Backup-Zeit nicht gelesen werden kann</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="387" />
        <source>Error parsing client data</source>
        <translation>Fehler beim Auswerten der Client-Daten</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientswidget.cpp" line="428" />
        <source>%n client(s) loaded</source>
        <translation>
            <numerusform>%n Client geladen</numerusform>
            <numerusform>%n Clients geladen</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="538" />
        <source>Details / Export...</source>
        <translation>Details / Export...</translation>
    </message>
</context>
<context>
    <name>BConfigImportDialog</name>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="34" />
        <source>Import Director Configuration</source>
        <translation>Director-Konfiguration importieren</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="60" />
        <source>Configuration Source</source>
        <translation>Konfigurationsquelle</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="64" />
        <source>Select directory or ZIP file...</source>
        <translation>Verzeichnis oder ZIP-Datei wählen...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="68" />
        <source>Directory...</source>
        <translation>Verzeichnis...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="73" />
        <source>ZIP File...</source>
        <translation>ZIP-Datei...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="78" />
        <source>Parse</source>
        <translation>Einlesen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="101" />
        <source>Connection Preview</source>
        <translation>Verbindungsvorschau</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="105" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="171" />
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="110" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="200" />
        <source>Address:</source>
        <translation>Adresse:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="114" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="207" />
        <source>Port:</source>
        <translation>Port:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="118" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="185" />
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="122" />
        <source>TLS:</source>
        <translation>TLS:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="138" />
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="145" />
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="168" />
        <source>Connection Settings</source>
        <translation>Verbindungseinstellungen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="202" />
        <source>hostname or IP address</source>
        <translation>Hostname oder IP-Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="203" />
        <source>Enter the hostname or IP address of the Director to connect to</source>
        <translation>Geben Sie den Hostnamen oder die IP-Adresse des Directors ein, mit dem verbunden werden soll</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="211" />
        <source>Port number (default: 9101)</source>
        <translation>Portnummer (Standard: 9101)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="214" />
        <source>Create connection profile</source>
        <translation>Verbindungsprofil anlegen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="219" />
        <source>Copy Debug Info</source>
        <translation>Debug-Info kopieren</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="220" />
        <source>Copy parsed resource details to clipboard for debugging</source>
        <translation>Eingelesene Ressourcendetails zur Fehlersuche in die Zwischenablage kopieren</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="253" />
        <source>Debug Info Copied</source>
        <translation>Debug-Info kopiert</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="254" />
        <source>Debug information has been copied to clipboard.

Paste it somewhere to view.</source>
        <translation>Die Debug-Informationen wurden in die Zwischenablage kopiert.

Fügen Sie sie irgendwo ein, um sie anzusehen.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="259" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="464" />
        <source>Director</source>
        <translation>Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="264" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="465" />
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="267" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="466" />
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="270" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="467" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="273" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="468" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="276" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="469" />
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="279" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="470" />
        <source>Pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="282" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="471" />
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="285" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="472" />
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="288" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="473" />
        <source>Catalogs</source>
        <translation>Catalogs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="299" />
        <source>Select Director Configuration Directory</source>
        <translation>Director-Konfigurationsverzeichnis wählen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="306" />
        <source>Ready to parse directory</source>
        <translation>Bereit zum Einlesen des Verzeichnisses</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="320" />
        <source>Select Configuration ZIP Archive</source>
        <translation>Konfigurations-ZIP-Archiv wählen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="322" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-Archive (*.zip);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="327" />
        <source>Ready to parse ZIP archive</source>
        <translation>Bereit zum Einlesen des ZIP-Archivs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="342" />
        <source>Parsing...</source>
        <translation>Lese ein...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="356" />
        <source>Failed to create temporary directory</source>
        <translation>Temporäres Verzeichnis konnte nicht erstellt werden</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="360" />
        <source>Extracting ZIP archive...</source>
        <translation>Entpacke ZIP-Archiv...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="368" />
        <source>Failed to extract ZIP archive (timeout)</source>
        <translation>ZIP-Archiv konnte nicht entpackt werden (Zeitüberschreitung)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="373" />
        <source>Failed to extract ZIP archive: %1</source>
        <translation>ZIP-Archiv konnte nicht entpackt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="393" />
        <source>Parsing extracted files...</source>
        <translation>Lese entpackte Dateien ein...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="403" />
        <source>Parsed %1: %2</source>
        <translation>%1 eingelesen: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="410" />
        <source>Parsing complete: %1 resources found</source>
        <translation>Einlesen abgeschlossen: %1 Ressourcen gefunden</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="440" />
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="443" />
        <source>Parsing Error</source>
        <translation>Fehler beim Einlesen</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="502" />
        <source>(multiple addresses)</source>
        <translation>(mehrere Adressen)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="509" />
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="509" />
        <source>Disabled</source>
        <translation>Deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="568" />
        <source>Invalid Selection</source>
        <translation>Ungültige Auswahl</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="569" />
        <source>Please select both a Director and a Console.</source>
        <translation>Bitte wählen Sie sowohl einen Director als auch eine Console.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="574" />
        <source>Invalid Address</source>
        <translation>Ungültige Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="575" />
        <source>Please enter the hostname or IP address of the Director.</source>
        <translation>Bitte geben Sie den Hostnamen oder die IP-Adresse des Directors ein.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="613" />
        <source>%1 '%2': Missing required field '%3'</source>
        <translation>%1 „%2“: Pflichtfeld „%3“ fehlt</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="634" />
        <source>&lt;b&gt;Validation Warnings (%1 total, showing first 10):&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Validierungswarnungen (%1 insgesamt, die ersten 10 werden angezeigt):&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="636" />
        <source>&lt;b&gt;Validation Warnings:&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Validierungswarnungen:&lt;/b&gt;&lt;br&gt;</translation>
    </message>
</context>
<context>
    <name>BConnectionWizard</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="57" />
        <source>Connection Setup</source>
        <translation>Verbindungseinrichtung</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="84" />
        <source>New Connection</source>
        <translation>Neue Verbindung</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="194" />
        <source>SHA-256 fingerprint of the server certificate:</source>
        <translation>SHA-256-Fingerabdruck des Serverzertifikats:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="201" />
        <source>Server certificate changed</source>
        <translation>Serverzertifikat geändert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="202" />
        <source>The certificate of the Director at %1:%2 has changed since the last connection.</source>
        <translation>Das Zertifikat des Directors unter %1:%2 hat sich seit der letzten Verbindung geändert.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="204" />
        <source>Previously trusted:</source>
        <translation>Bisher vertraut:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="206" />
        <source>This is expected if the certificate was regenerated on the Director. Otherwise someone may be intercepting the connection.</source>
        <translation>Das ist zu erwarten, wenn das Zertifikat auf dem Director neu erzeugt wurde. Andernfalls fängt möglicherweise jemand die Verbindung ab.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="209" />
        <source>Trust server certificate?</source>
        <translation>Serverzertifikat vertrauen?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="210" />
        <source>The Director at %1:%2 uses a self-signed certificate that has not been trusted yet.</source>
        <translation>Der Director unter %1:%2 verwendet ein selbstsigniertes Zertifikat, dem noch nicht vertraut wird.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="216" />
        <source>The same fingerprint is announced in the local network (mDNS). This is not proof of the Director's identity: any device in the network can send such an announcement.</source>
        <translation>Derselbe Fingerabdruck wird im lokalen Netzwerk (mDNS) angekündigt. Das ist kein Beweis für die Identität des Directors: Jedes Gerät im Netzwerk kann eine solche Ankündigung senden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="221" />
        <source>The fingerprint does NOT match the one announced in the network (mDNS)!</source>
        <translation>Der Fingerabdruck stimmt NICHT mit dem im Netzwerk (mDNS) angekündigten überein!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="224" />
        <source>Before trusting it, compare the fingerprint with the one of the Director's gRPC certificate, obtained from the Director's administrator or read on the Director itself.</source>
        <translation>Vergleichen Sie den Fingerabdruck vor dem Vertrauen mit dem des gRPC-Zertifikats des Directors, den Sie vom Administrator des Directors erhalten oder auf dem Director selbst ablesen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="231" />
        <source>Trust certificate</source>
        <translation>Zertifikat vertrauen</translation>
    </message>
</context>
<context>
    <name>BConsoleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>ACLs</source>
        <translation>ACLs</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122" />
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122" />
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="130" />
        <source>None</source>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>BDatabase</name>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="37" />
        <source>Failed to create database directory: %1</source>
        <translation>Datenbankverzeichnis konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="47" />
        <source>Failed to open database: %1</source>
        <translation>Datenbank konnte nicht geöffnet werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="68" />
        <source>Failed to create database schema: %1</source>
        <translation>Datenbankschema konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="88" />
        <source>Failed to apply database migrations: %1</source>
        <translation>Datenbankmigrationen konnten nicht angewendet werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="100" />
        <source>Failed to repair database schema: %1</source>
        <translation>Datenbankschema konnte nicht repariert werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="206" />
        <source>No migration script for database version %1</source>
        <translation>Kein Migrationsskript für Datenbankversion %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="221" />
        <source>Failed to apply migration to version %1: %2</source>
        <translation>Migration auf Version %1 konnte nicht angewendet werden: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="226" />
        <source>Migration to version %1 did not record its version</source>
        <translation>Die Migration auf Version %1 hat ihre Version nicht eingetragen</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="257" />
        <source>Table %1 is still missing after the repair</source>
        <translation>Tabelle %1 fehlt nach der Reparatur weiterhin</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="270" />
        <source>Failed to open SQL resource: %1</source>
        <translation>SQL-Ressource konnte nicht geöffnet werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="286" />
        <source>Failed to start transaction: %1</source>
        <translation>Transaktion konnte nicht gestartet werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="317" />
        <source>SQL execution failed: %1
Statement: %2</source>
        <translation>SQL-Ausführung fehlgeschlagen: %1
Anweisung: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="331" />
        <source>Failed to commit transaction: %1</source>
        <translation>Transaktion konnte nicht abgeschlossen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="482" />
        <source>Database not ready</source>
        <translation>Datenbank nicht bereit</translation>
    </message>
</context>
<context>
    <name>BDirectiveSchema</name>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="259" />
        <source>Required directive '%1' is missing</source>
        <translation>Pflicht-Direktive „%1“ fehlt</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="271" />
        <source>'%1' must be an integer</source>
        <translation>„%1“ muss eine ganze Zahl sein</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="274" />
        <source>'%1' must be &gt;= %2</source>
        <translation>„%1“ muss &gt;= %2 sein</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="277" />
        <source>'%1' must be &lt;= %2</source>
        <translation>„%1“ muss &lt;= %2 sein</translation>
    </message>
</context>
<context>
    <name>BDirectorModel</name>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="23" />
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="24" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="25" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="26" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="27" />
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="28" />
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="29" />
        <source>DIR Port</source>
        <translation>DIR-Port</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="30" />
        <source>Query File</source>
        <translation>Query File</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="31" />
        <source>Working Dir</source>
        <translation>Working Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="32" />
        <source>PID Dir</source>
        <translation>PID Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="33" />
        <source>Scripts Dir</source>
        <translation>Scripts Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="34" />
        <source>Plugin Dir</source>
        <translation>Plugin Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="35" />
        <source>SubSys Dir</source>
        <translation>SubSys Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="36" />
        <source>Max Jobs</source>
        <translation>Max Jobs</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="37" />
        <source>TLS Enable</source>
        <translation>TLS Enable</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="38" />
        <source>TLS Require</source>
        <translation>TLS Require</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="39" />
        <source>TLS Verify</source>
        <translation>TLS Verify</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="40" />
        <source>TLS CA Cert</source>
        <translation>TLS CA Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="41" />
        <source>TLS Cert</source>
        <translation>TLS Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="42" />
        <source>TLS Key</source>
        <translation>TLS Key</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="43" />
        <source>TLS Allowed CN</source>
        <translation>TLS Allowed CN</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="44" />
        <source>System</source>
        <translation>System</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="45" />
        <source>Created</source>
        <translation>Erstellt</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="46" />
        <source>Updated</source>
        <translation>Aktualisiert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="47" />
        <source>Last Connected</source>
        <translation>Zuletzt verbunden</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="48" />
        <source>Active</source>
        <translation>Aktiv</translation>
    </message>
</context>
<context>
    <name>BDirectorResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19" />
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="36" />
        <source>(multiple)</source>
        <translation>(mehrere)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45" />
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45" />
        <source>No</source>
        <translation>Nein</translation>
    </message>
</context>
<context>
    <name>BEditableListModel</name>
    <message>
        <location filename="../src/config/beditablelistmodel.cpp" line="120" />
        <source>Add %1 items</source>
        <translation>%1 Einträge hinzufügen</translation>
    </message>
</context>
<context>
    <name>BEditableListWidget</name>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="77" />
        <location filename="../src/config/beditablelistwidget.cpp" line="110" />
        <source>Add...</source>
        <translation>Hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="78" />
        <location filename="../src/config/beditablelistwidget.cpp" line="123" />
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="115" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="246" />
        <source>Add item</source>
        <translation>Eintrag hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="251" />
        <source>Remove selected</source>
        <translation>Ausgewählte entfernen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="390" />
        <location filename="../src/config/beditablelistwidget.cpp" line="447" />
        <source>Select Directory</source>
        <translation>Verzeichnis wählen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="392" />
        <location filename="../src/config/beditablelistwidget.cpp" line="449" />
        <source>Select File</source>
        <translation>Datei wählen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="397" />
        <source>Add Item</source>
        <translation>Eintrag hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="398" />
        <source>Enter value:</source>
        <translation>Wert eingeben:</translation>
    </message>
</context>
<context>
    <name>BFileSetDocument</name>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="755" />
        <source>FileSet name is required</source>
        <translation>Der FileSet-Name ist erforderlich</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="761" />
        <source>At least one Include block is required</source>
        <translation>Mindestens ein Include-Block ist erforderlich</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="769" />
        <source>Include block %1 has no file paths</source>
        <translation>Include-Block %1 hat keine Dateipfade</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="774" />
        <source>At least one Include block must have file paths</source>
        <translation>Mindestens ein Include-Block muss Dateipfade enthalten</translation>
    </message>
</context>
<context>
    <name>BFileSetExcludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1008" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1023" />
        <source>Global Exclude</source>
        <translation>Globaler Ausschluss</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1009" />
        <source>Configure paths and patterns to exclude from all Include blocks.</source>
        <translation>Pfade und Muster konfigurieren, die aus allen Include-Blöcken ausgeschlossen werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1019" />
        <source>Exclude Paths</source>
        <translation>Ausgeschlossene Pfade</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1024" />
        <source>Enter path to exclude...</source>
        <translation>Auszuschließenden Pfad eingeben...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1025" />
        <source>+ Add Path</source>
        <translation>+ Pfad hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1031" />
        <source>These paths will be excluded from all backup jobs using this FileSet.
Use absolute paths (e.g., /tmp, /var/cache) or patterns.</source>
        <translation>Diese Pfade werden von allen Backup-Jobs ausgeschlossen, die dieses FileSet verwenden.
Verwenden Sie absolute Pfade (z. B. /tmp, /var/cache) oder Muster.</translation>
    </message>
</context>
<context>
    <name>BFileSetIncludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="901" />
        <source>Include Blocks</source>
        <translation>Include-Blöcke</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="902" />
        <source>Configure which files and directories to include in the backup.</source>
        <translation>Konfigurieren, welche Dateien und Verzeichnisse in das Backup aufgenommen werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="915" />
        <source>+ Add Include Block</source>
        <translation>+ Include-Block hinzufügen</translation>
    </message>
</context>
<context>
    <name>BFileSetPreviewPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1064" />
        <source>Preview &amp; Deploy</source>
        <translation>Vorschau &amp; Bereitstellung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1065" />
        <source>Review the generated FileSet configuration. Copy or export it for manual deployment.</source>
        <translation>Prüfen Sie die erzeugte FileSet-Konfiguration. Kopieren oder exportieren Sie sie zur manuellen Bereitstellung.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1077" />
        <source>Configuration:</source>
        <translation>Konfiguration:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1083" />
        <source>Copy configuration</source>
        <translation>Konfiguration kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1141" />
        <source># NOTE: Bareos Director does not support 'configure add fileset' with
# nested Include/Exclude/Options blocks (confirmed Bareos limitation).
# Deploy this configuration manually:
#   1. Copy to /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Run: systemctl reload bareos-dir
#
# Use 'Copy configuration' to copy the text below.

</source>
        <translation># HINWEIS: Der Bareos Director unterstützt 'configure add fileset' mit
# verschachtelten Include/Exclude/Options-Blöcken nicht (bestätigte Bareos-Einschränkung).
# Stellen Sie diese Konfiguration manuell bereit:
#   1. Kopieren nach /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Ausführen: systemctl reload bareos-dir
#
# Mit „Konfiguration kopieren“ kopieren Sie den Text unten.

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1176" />
        <source>Configuration is valid</source>
        <translation>Konfiguration ist gültig</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1190" />
        <source>Close Wizard</source>
        <translation>Assistent schließen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1191" />
        <source>The configuration has not been sent to the Director.
Use 'Copy configuration' to save it before closing.

Do you want to close the wizard?</source>
        <translation>Die Konfiguration wurde nicht an den Director gesendet.
Speichern Sie sie vor dem Schließen mit „Konfiguration kopieren“.

Möchten Sie den Assistenten schließen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1204" />
        <source>Executing configure command...</source>
        <translation>Führe configure-Befehl aus...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1242" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1278" />
        <source>FileSet created successfully!</source>
        <translation>FileSet erfolgreich angelegt!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1254" />
        <source>Unknown error</source>
        <translation>Unbekannter Fehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1256" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1288" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1296" />
        <source>Timeout</source>
        <translation>Zeitüberschreitung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1297" />
        <source>The configure command timed out.
Do you want to retry?</source>
        <translation>Beim configure-Befehl ist eine Zeitüberschreitung aufgetreten.
Möchten Sie es erneut versuchen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1303" />
        <source>Command timed out</source>
        <translation>Zeitüberschreitung beim Befehl</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1313" />
        <source>Configuration copied to clipboard</source>
        <translation>Konfiguration in die Zwischenablage kopiert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1325" />
        <source>ZIP Archives (*.zip);;Configuration Files (*.conf);;All Files (*)</source>
        <translation>ZIP-Archive (*.zip);;Konfigurationsdateien (*.conf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1328" />
        <source>Export FileSet Configuration</source>
        <translation>FileSet-Konfiguration exportieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1348" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1380" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1394" />
        <source>Export Error</source>
        <translation>Exportfehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1349" />
        <source>Cannot create zip file: %1</source>
        <translation>ZIP-Datei kann nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1381" />
        <source>Error writing zip file</source>
        <translation>Fehler beim Schreiben der ZIP-Datei</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1385" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1402" />
        <source>Exported to: %1</source>
        <translation>Exportiert nach: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1395" />
        <source>Cannot write file: %1</source>
        <translation>Datei kann nicht geschrieben werden: %1</translation>
    </message>
</context>
<context>
    <name>BFileSetResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354" />
        <source>Source File</source>
        <translation>Quelldatei</translation>
    </message>
</context>
<context>
    <name>BFileSetSettingsPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="616" />
        <source>FileSet Settings</source>
        <translation>FileSet-Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="617" />
        <source>Configure the FileSet name and basic options.</source>
        <translation>FileSet-Namen und grundlegende Optionen konfigurieren.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="631" />
        <source>Select FileSet:</source>
        <translation>FileSet wählen:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="634" />
        <source>Loading FileSets...</source>
        <translation>Lade FileSets...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="651" />
        <source>Basic Settings</source>
        <translation>Grundeinstellungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="655" />
        <source>e.g., LinuxFullSystem</source>
        <translation>z. B. LinuxFullSystem</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="656" />
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="659" />
        <source>Optional description</source>
        <translation>Optionale Beschreibung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="660" />
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="665" />
        <source>Global Options</source>
        <translation>Globale Optionen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="668" />
        <source>Enable VSS</source>
        <translation>VSS aktivieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="669" />
        <source>Enable Volume Shadow Copy Service (Windows)</source>
        <translation>Volume Shadow Copy Service aktivieren (Windows)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="672" />
        <source>Ignore FileSet Changes</source>
        <translation>FileSet-Änderungen ignorieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="673" />
        <source>Don't trigger Full backup when FileSet changes</source>
        <translation>Bei Änderungen am FileSet kein Full-Backup auslösen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="676" />
        <source>Enable Snapshot</source>
        <translation>Snapshot aktivieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="677" />
        <source>Enable snapshot support (Linux LVM/btrfs)</source>
        <translation>Snapshot-Unterstützung aktivieren (Linux LVM/btrfs)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="699" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="755" />
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="775" />
        <source>No Director connection</source>
        <translation>Keine Director-Verbindung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="829" />
        <source>-- Select FileSet --</source>
        <translation>-- FileSet wählen --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="835" />
        <source>(%1 FileSets)</source>
        <translation>(%1 FileSets)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="877" />
        <source>Loaded</source>
        <translation>Geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="891" />
        <source>Config not cached</source>
        <translation>Konfiguration nicht zwischengespeichert</translation>
    </message>
</context>
<context>
    <name>BFileSetTemplatePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="165" />
        <source>Select FileSet Template</source>
        <translation>FileSet-Vorlage wählen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="166" />
        <source>Choose a preset template to start with, or mix multiple templates together.</source>
        <translation>Wählen Sie eine Vorlage als Ausgangspunkt oder kombinieren Sie mehrere Vorlagen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="175" />
        <source>Template:</source>
        <translation>Vorlage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="178" />
        <source>Mix Templates...</source>
        <translation>Vorlagen kombinieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="180" />
        <source>Combine paths from multiple templates</source>
        <translation>Pfade aus mehreren Vorlagen zusammenführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="192" />
        <source>Template Description</source>
        <translation>Vorlagenbeschreibung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="206" />
        <source>Paths Preview</source>
        <translation>Pfadvorschau</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="211" />
        <source>&lt;b&gt;Include:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Include:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="224" />
        <source>&lt;b&gt;Exclude:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Exclude:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="393" />
        <source>Platform: Windows</source>
        <translation>Plattform: Windows</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="395" />
        <source>Platform: Linux/Unix</source>
        <translation>Plattform: Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="397" />
        <source>Platform: macOS</source>
        <translation>Plattform: macOS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="399" />
        <source>Platform: Any</source>
        <translation>Plattform: beliebig</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="412" />
        <source>Mix Templates</source>
        <translation>Vorlagen kombinieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="417" />
        <source>Select templates to combine:</source>
        <translation>Zu kombinierende Vorlagen wählen:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="435" />
        <source>%1
Platform: %2</source>
        <translation>%1
Plattform: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="442" />
        <source>Combined Paths Preview</source>
        <translation>Vorschau der kombinierten Pfade</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="491" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="493" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="601" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="605" />
        <source>(none)</source>
        <translation>(keine)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="570" />
        <source>Mixed_%1</source>
        <translation>Mixed_%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="571" />
        <source>Combined FileSet from %1 templates</source>
        <translation>Kombiniertes FileSet aus %1 Vorlagen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="573" />
        <source>Mixed: %1 templates selected</source>
        <translation>Kombiniert: %1 Vorlagen ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="577" />
        <source>Templates: %1</source>
        <translation>Vorlagen: %1</translation>
    </message>
</context>
<context>
    <name>BFileSetWizard</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80" />
        <source>New FileSet Wizard</source>
        <translation>Assistent für neues FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80" />
        <source>Edit FileSet</source>
        <translation>FileSet bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="103" />
        <source>Finish</source>
        <translation>Fertigstellen</translation>
    </message>
</context>
<context>
    <name>BGrpcAddressProbe</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="167" />
        <source>no TLS connection within %1 s</source>
        <translation>keine TLS-Verbindung innerhalb von %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="204" />
        <source>Cannot resolve the host name %1: %2</source>
        <translation>Der Hostname %1 kann nicht aufgelöst werden: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="205" />
        <source>no address</source>
        <translation>keine Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="229" />
        <source>Cannot resolve the host name %1: no address</source>
        <translation>Der Hostname %1 kann nicht aufgelöst werden: keine Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="242" />
        <source>Cannot reach the gRPC endpoint %1:%2 (%3)</source>
        <translation>Der gRPC-Endpunkt %1:%2 ist nicht erreichbar (%3)</translation>
    </message>
</context>
<context>
    <name>BGrpcConnect</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="83" />
        <source>%1 - the Director no longer knows this session (it ends after 8 hours without a request or when the Director restarts). Connect again.</source>
        <translation>%1 – der Director kennt diese Sitzung nicht mehr (sie endet nach 8 Stunden ohne Anfrage oder bei einem Neustart des Directors). Verbinden Sie sich erneut.</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="90" />
        <source>the call was cancelled</source>
        <translation>der Aufruf wurde abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="94" />
        <source>no connection or no reply within %1 s</source>
        <translation>keine Verbindung oder keine Antwort innerhalb von %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="95" />
        <source>no connection or no reply in time</source>
        <translation>keine Verbindung oder keine rechtzeitige Antwort</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="98" />
        <source>this console is not allowed to do that</source>
        <translation>diese Console darf das nicht</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="101" />
        <source>the Director does not offer this method (an older version?)</source>
        <translation>der Director bietet diese Methode nicht an (ältere Version?)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="104" />
        <source>the gRPC service of the Director cannot be reached</source>
        <translation>der gRPC-Dienst des Directors ist nicht erreichbar</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="107" />
        <source>not logged in, or the session has expired</source>
        <translation>nicht angemeldet, oder die Sitzung ist abgelaufen</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="111" />
        <source>the Director reported an error without a message</source>
        <translation>der Director meldete einen Fehler ohne Meldungstext</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="114" />
        <source>no reason given</source>
        <translation>kein Grund angegeben</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="126" />
        <source>The Director does not offer the expected gRPC service (protocol version mismatch: this build speaks %1; the Director may run an older or newer build). Use BNET or update the Director or this application.</source>
        <translation>Der Director bietet den erwarteten gRPC-Dienst nicht an (Protokollversion passt nicht: dieser Build spricht %1; der Director läuft möglicherweise mit einem älteren oder neueren Build). Verwenden Sie BNET oder aktualisieren Sie den Director oder diese Anwendung.</translation>
    </message>
</context>
<context>
    <name>BGrpcTransport</name>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="195" />
        <location filename="../src/director/bgrpctransport.cpp" line="228" />
        <source>gRPC login failed: %1</source>
        <translation>gRPC-Anmeldung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="203" />
        <source>gRPC login failed: no challenge received</source>
        <translation>gRPC-Anmeldung fehlgeschlagen: keine Challenge empfangen</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="236" />
        <source>Authentication failed: %1</source>
        <translation>Authentifizierung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="237" />
        <source>invalid response</source>
        <translation>ungültige Antwort</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="295" />
        <source>Client %1 connected</source>
        <translation>Client %1 verbunden</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="297" />
        <source>Client %1 disconnected</source>
        <translation>Client %1 getrennt</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="300" />
        <source>Client %1 is alive</source>
        <translation>Client %1 ist erreichbar</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="301" />
        <source>Client %1 not reachable</source>
        <translation>Client %1 nicht erreichbar</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="303" />
        <source>Storage %1 connected (job %2)</source>
        <translation>Storage %1 verbunden (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="306" />
        <source>Storage %1 disconnected (job %2)</source>
        <translation>Storage %1 getrennt (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="310" />
        <source>Storage %1 is alive</source>
        <translation>Storage %1 ist erreichbar</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="311" />
        <source>Storage %1 not reachable</source>
        <translation>Storage %1 nicht erreichbar</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="421" />
        <source>Invalid response</source>
        <translation>Ungültige Antwort</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="481" />
        <source>Not connected to the gRPC service of the Director</source>
        <translation>Nicht mit dem gRPC-Dienst des Directors verbunden</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="542" />
        <source>The reply of the Director could not be read</source>
        <translation>Die Antwort des Directors konnte nicht gelesen werden</translation>
    </message>
</context>
<context>
    <name>BIncludeBlockWidget</name>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="60" />
        <source>Include Block #%1</source>
        <translation>Include-Block #%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="68" />
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="95" />
        <source>Add Path...</source>
        <translation>Pfad hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="96" />
        <source>Enter file or directory path</source>
        <translation>Datei- oder Verzeichnispfad eingeben</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="99" />
        <source>Files</source>
        <translation>Dateien</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="106" />
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="120" />
        <source>Exclude Files</source>
        <translation>Dateien ausschließen</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="126" />
        <source>Add File...</source>
        <translation>Datei hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="127" />
        <source>Enter file path to exclude</source>
        <translation>Auszuschließenden Dateipfad eingeben</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="133" />
        <source>Directory Patterns (WildDir)</source>
        <translation>Verzeichnismuster (WildDir)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="139" />
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="152" />
        <source>Add Pattern...</source>
        <translation>Muster hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="140" />
        <source>e.g., */__pycache__</source>
        <translation>z. B. */__pycache__</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="146" />
        <source>File Patterns (WildFile)</source>
        <translation>Dateimuster (WildFile)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="153" />
        <source>e.g., *.pyc, *.log</source>
        <translation>z. B. *.pyc, *.log</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="158" />
        <source>Exclude</source>
        <translation>Exclude</translation>
    </message>
</context>
<context>
    <name>BIncludeOptionsForm</name>
    <message>
        <location filename="../src/config/bincludeoptionsform.cpp" line="87" />
        <source>Failed to load include options schema</source>
        <translation>Schema der Include-Optionen konnte nicht geladen werden</translation>
    </message>
</context>
<context>
    <name>BJobAdvancedPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="633" />
        <source>Advanced Directives</source>
        <translation>Erweiterte Direktiven</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="634" />
        <source>Optional settings for fine-tuning (can be skipped).</source>
        <translation>Optionale Einstellungen zur Feinabstimmung (können übersprungen werden).</translation>
    </message>
</context>
<context>
    <name>BJobBasicsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="244" />
        <location filename="../src/jobs/bjobwizard.cpp" line="249" />
        <source>Basic Settings</source>
        <translation>Grundeinstellungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="245" />
        <source>Configure the identity of the resource.</source>
        <translation>Die Identität der Ressource konfigurieren.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="253" />
        <source>e.g., BackupServer1</source>
        <translation>z. B. BackupServer1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="254" />
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="258" />
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="263" />
        <source>Inheritance</source>
        <translation>Vererbung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="270" />
        <source>JobDefs:</source>
        <translation>JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="282" />
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="294" />
        <source>Inherits default settings from JobDefs "%1". Resource fields on the next page become optional.</source>
        <translation>Übernimmt Standardeinstellungen von JobDefs „%1“. Die Ressourcenfelder auf der nächsten Seite werden optional.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="308" />
        <source>Basic Settings (JobDefs)</source>
        <translation>Grundeinstellungen (JobDefs)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitions</name>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="112" />
        <source>(block)</source>
        <translation>(Block)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitions.cpp" line="125" />
        <source>(%n block(s))</source>
        <translation>
            <numerusform>(%n Block)</numerusform>
            <numerusform>(%n Blöcke)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="299" />
        <source>(see the text)</source>
        <translation>(siehe Text)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitionsWidget</name>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="157" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="158" />
        <source>Resource</source>
        <translation>Ressource</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="159" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="435" />
        <source>JobDefs</source>
        <translation>JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="160" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="161" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="162" />
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="163" />
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="164" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="165" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="166" />
        <source>Priority</source>
        <translation>Priorität</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="167" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202" />
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200" />
        <source>Disabled</source>
        <translation>Deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202" />
        <source>Disabled, the scheduler does not start it</source>
        <translation>Deaktiviert, der Scheduler startet ihn nicht</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="235" />
        <source>Set in this %1</source>
        <translation>In diesem %1 gesetzt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="238" />
        <source>Inherited from JobDefs %1</source>
        <translation>Geerbt von JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="240" />
        <source>Not known: a JobDefs of the chain was not reported</source>
        <translation>Nicht bekannt: Ein JobDefs der Kette wurde nicht gemeldet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="241" />
        <source>Not set: the default of the Director applies</source>
        <translation>Nicht gesetzt: Es gilt die Vorgabe des Directors</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="265" />
        <source>Copy %1 %2</source>
        <translation>%1 %2 kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="266" />
        <source>New %1</source>
        <translation>Neu: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="271" />
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="286" />
        <source>&amp;JobDefs:</source>
        <translation>&amp;JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="288" />
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="292" />
        <source>&amp;Level:</source>
        <translation>&amp;Level:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="295" />
        <source>&amp;Client:</source>
        <translation>&amp;Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="297" />
        <source>&amp;FileSet:</source>
        <translation>&amp;FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="299" />
        <source>&amp;Schedule:</source>
        <translation>&amp;Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="301" />
        <source>St&amp;orage:</source>
        <translation>St&amp;orage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="303" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="305" />
        <source>&amp;Messages:</source>
        <translation>&amp;Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="311" />
        <source>Create it &amp;disabled (Enabled = no)</source>
        <translation>&amp;Deaktiviert anlegen (Enabled = no)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="313" />
        <source>A new job with a schedule is started by the scheduler of the Director. Created disabled, it does not run until you enable it.</source>
        <translation>Ein neuer Job mit einem Schedule wird vom Scheduler des Directors gestartet. Deaktiviert angelegt, läuft er erst, wenn Sie ihn aktivieren.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="320" />
        <source>The copy is the text of %1 with the new name, in a file of its own. Everything else stays as it is.</source>
        <translation>Die Kopie ist der Text von %1 mit dem neuen Namen, in einer eigenen Datei. Alles andere bleibt, wie es ist.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="324" />
        <source>Fields left empty are not written; a job takes them from its JobDefs. The Director checks the new resource before anything is written.</source>
        <translation>Leer gelassene Felder werden nicht geschrieben; ein Job übernimmt sie aus seinen JobDefs. Der Director prüft die neue Ressource, bevor etwas geschrieben wird.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="334" />
        <source>&amp;Check and review...</source>
        <translation>&amp;Prüfen und ansehen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="351" />
        <source>A name is needed.</source>
        <translation>Ein Name ist erforderlich.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="353" />
        <source>The name must not contain quotes, slashes, backslashes or "..".</source>
        <translation>Der Name darf keine Anführungszeichen, Schrägstriche, Backslashes oder ".." enthalten.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="355" />
        <source>A resource of this name exists already.</source>
        <translation>Eine Ressource mit diesem Namen existiert bereits.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="422" />
        <source>&amp;Refresh</source>
        <translation>&amp;Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="423" />
        <source>Reads the Job and JobDefs resources from the Director again</source>
        <translation>Liest die Job- und JobDefs-Ressourcen erneut vom Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="424" />
        <source>New &amp;Job...</source>
        <translation>Neuer &amp;Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="425" />
        <source>New Job&amp;Defs...</source>
        <translation>Neue Job&amp;Defs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="426" />
        <source>&amp;Copy...</source>
        <translation>&amp;Kopieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="427" />
        <source>De&amp;lete...</source>
        <translation>&amp;Löschen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="429" />
        <source>Filter</source>
        <translation>Filter</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="431" />
        <source>Filter the resources</source>
        <translation>Ressourcen filtern</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="433" />
        <source>Jobs and JobDefs</source>
        <translation>Jobs und JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="434" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="436" />
        <source>Show</source>
        <translation>Anzeigen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="457" />
        <source>Job and JobDefs resources</source>
        <translation>Job- und JobDefs-Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Directive</source>
        <translation>Direktive</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Value</source>
        <translation>Wert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Set in</source>
        <translation>Gesetzt in</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="505" />
        <source>Directives and where they are set</source>
        <translation>Direktiven und wo sie gesetzt sind</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="512" />
        <source>Directives</source>
        <translation>Direktiven</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="519" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="526" />
        <source>Resource text</source>
        <translation>Ressourcentext</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="528" />
        <source>Edit the &amp;text directly (for directives the form does not have)</source>
        <translation>&amp;Text direkt bearbeiten (für Direktiven, die das Formular nicht enthält)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="529" />
        <source>The text is sent as you write it, after the Director has checked it and you have seen the difference. The fields of the form are not used then.</source>
        <translation>Der Text wird so gesendet, wie Sie ihn schreiben, nachdem der Director ihn geprüft und Sie den Unterschied gesehen haben. Die Felder des Formulars werden dann nicht verwendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="533" />
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="537" />
        <source>Re&amp;vert</source>
        <translation>&amp;Zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="538" />
        <source>Drops the changes made here</source>
        <translation>Verwirft die hier vorgenommenen Änderungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="539" />
        <source>&amp;Save...</source>
        <translation>&amp;Speichern...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="540" />
        <source>Lets the Director check the change, shows it, and writes it after you confirm</source>
        <translation>Lässt den Director die Änderung prüfen, zeigt sie an und schreibt sie nach Ihrer Bestätigung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="626" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1032" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1201" />
        <source>(not set here)</source>
        <translation>(hier nicht gesetzt)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="627" />
        <source>yes</source>
        <translation>ja</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="628" />
        <source>no</source>
        <translation>nein</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="645" />
        <source>JobDefs:</source>
        <translation>JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="646" />
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="649" />
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="653" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="654" />
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="655" />
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="656" />
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="657" />
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="658" />
        <source>Messages:</source>
        <translation>Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="659" />
        <source>Priority:</source>
        <translation>Priorität:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="660" />
        <source>Enabled:</source>
        <translation>Aktiviert:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="661" />
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="776" />
        <source>Unsaved changes</source>
        <translation>Ungespeicherte Änderungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="777" />
        <source>%1 has changes that were not written. Drop them?</source>
        <translation>%1 hat Änderungen, die nicht geschrieben wurden. Verwerfen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="805" />
        <source>No resource selected</source>
        <translation>Keine Ressource ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="806" />
        <source>Choose a Job or JobDefs in the table to see its directives and where they are set.</source>
        <translation>Wählen Sie in der Tabelle einen Job oder JobDefs, um seine Direktiven zu sehen und wo sie gesetzt sind.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="840" />
        <source>Uses no JobDefs: every directive is set here or has its default.</source>
        <translation>Verwendet keine JobDefs: Jede Direktive ist hier gesetzt oder hat ihren Standardwert.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="841" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="849" />
        <source>Inherits from JobDefs %1.</source>
        <translation>Erbt von JobDefs %1.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="844" />
        <source>No job uses this JobDefs.</source>
        <translation>Kein Job verwendet diese JobDefs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="845" />
        <source>Used by %n job(s): %1.</source>
        <translation>
            <numerusform>Verwendet von %n Job: %1.</numerusform>
            <numerusform>Verwendet von %n Jobs: %1.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="890" />
        <source>the text the Director sent does not match its SHA-256</source>
        <translation>Der vom Director gesendete Text passt nicht zu seinem SHA-256</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="923" />
        <source>this %1</source>
        <translation>diesem %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="932" />
        <source>JobDefs %1</source>
        <translation>JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="934" />
        <source>not known (a JobDefs was not reported)</source>
        <translation>nicht bekannt (ein JobDefs wurde nicht gemeldet)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="936" />
        <source>not set (default)</source>
        <translation>nicht gesetzt (Standard)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1009" />
        <source>Set here (as the Director reports it)</source>
        <translation>Hier gesetzt (wie vom Director gemeldet)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1011" />
        <source>Inherited: %1 (JobDefs %2). Leave empty to keep it.</source>
        <translation>Geerbt: %1 (JobDefs %2). Leer lassen, um es beizubehalten.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1013" />
        <source>Not known: a JobDefs of the chain was not reported.</source>
        <translation>Nicht bekannt: Ein JobDefs der Kette wurde nicht gemeldet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1014" />
        <source>Not set: the default of the Director applies.</source>
        <translation>Nicht gesetzt: Es gilt die Vorgabe des Directors.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1016" />
        <source>Written in a way the form cannot change (several times, a list, or with other directives on its line). Use the text.</source>
        <translation>So geschrieben, dass das Formular es nicht ändern kann (mehrfach, als Liste oder mit anderen Direktiven in derselben Zeile). Verwenden Sie den Text.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1033" />
        <source>(not set here: %1)</source>
        <translation>(hier nicht gesetzt: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1057" />
        <source>Read-only on this connection: changing Job and JobDefs resources needs the configuration methods of the gRPC interface of the Onesimus Director. New Job and New JobDefs use the job wizard ("configure add").</source>
        <translation>Auf dieser Verbindung nur lesend: Das Ändern von Job- und JobDefs-Ressourcen erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors. „Neuer Job“ und „Neue JobDefs“ verwenden den Job-Assistenten ("configure add").</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1061" />
        <source>Read-only: this session is not offered changes of %1 resources (the console needs the command ACL "configure" and the ACL of the resource type).</source>
        <translation>Nur lesend: Dieser Sitzung werden keine Änderungen an %1-Ressourcen angeboten (die Console benötigt die Befehls-ACL "configure" und die ACL des Ressourcentyps).</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1065" />
        <source>Reading the text of the resource from the Director...</source>
        <translation>Lese den Text der Ressource vom Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1067" />
        <source>Read-only: the text of the resource could not be read: %1</source>
        <translation>Nur lesend: Der Text der Ressource konnte nicht gelesen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1069" />
        <source>The form is locked: the text of the resource cannot be changed field by field safely (%1). Edit the text directly instead.</source>
        <translation>Das Formular ist gesperrt: Der Text der Ressource kann nicht sicher Feld für Feld geändert werden (%1). Bearbeiten Sie stattdessen den Text direkt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1100" />
        <source>Creates a resource with the text of this one and a new name</source>
        <translation>Erstellt eine Ressource mit dem Text dieser und einem neuen Namen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1101" />
        <source>Copying needs the configuration methods of the gRPC interface of the Onesimus Director (read and create) for this resource type</source>
        <translation>Kopieren erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (Lesen und Anlegen) für diesen Ressourcentyp</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1199" />
        <source>%1: %2 → %3</source>
        <translation>%1: %2 → %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1202" />
        <source>(not set here, inherited)</source>
        <translation>(hier nicht gesetzt, geerbt)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1262" />
        <source>%1 was not written: changing it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 wurde nicht geschrieben: Das Ändern erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors. Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1267" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1317" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1361" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1394" />
        <source>Another change is being written.</source>
        <translation>Eine andere Änderung wird gerade geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1275" />
        <source>The text as edited</source>
        <translation>Der Text in bearbeiteter Form</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1280" />
        <source>%1 cannot be written: %2</source>
        <translation>%1 kann nicht geschrieben werden: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1281" />
        <source>%1 was not written</source>
        <translation>%1 wurde nicht geschrieben</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1282" />
        <source>The change could not be written into the text of the resource safely: %1

Nothing was sent.</source>
        <translation>Die Änderung konnte nicht sicher in den Text der Ressource geschrieben werden: %1

Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1292" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1323" />
        <source>Jobs and JobDefs that use this JobDefs</source>
        <translation>Jobs und JobDefs, die diese JobDefs verwenden</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1293" />
        <source>A JobDefs gives its values to every job that does not set them itself; the change applies to those of the %n resource(s) below.</source>
        <translation>
            <numerusform>Eine JobDefs gibt ihre Werte an jeden Job weiter, der sie nicht selbst setzt; die Änderung gilt für die unten aufgeführte %n Ressource.</numerusform>
            <numerusform>Eine JobDefs gibt ihre Werte an jeden Job weiter, der sie nicht selbst setzt; die Änderung gilt für die unten aufgeführten %n Ressourcen.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1297" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1326" />
        <source>Jobs affected</source>
        <translation>Betroffene Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1298" />
        <source>Lines that were not edited stay as they are in the file.</source>
        <translation>Nicht bearbeitete Zeilen bleiben in der Datei unverändert.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1300" />
        <source>The Director checks the change of %1...</source>
        <translation>Der Director prüft die Änderung von %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1312" />
        <source>%1 was not deleted: deleting it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 wurde nicht gelöscht: Das Löschen erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors. Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1327" />
        <source>The job's records in the catalog stay; only its definition is removed. A running job is not touched.</source>
        <translation>Die Einträge des Jobs im Katalog bleiben erhalten; nur seine Definition wird entfernt. Ein laufender Job wird nicht berührt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1330" />
        <source>The Director checks whether %1 can be deleted...</source>
        <translation>Der Director prüft, ob %1 gelöscht werden kann...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1372" />
        <source>A new job. The Director checks that everything it needs is set, here or in its JobDefs.</source>
        <translation>Ein neuer Job. Der Director prüft, ob alles Nötige gesetzt ist, hier oder in seinen JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1374" />
        <source>A new JobDefs. No job uses it until a job names it.</source>
        <translation>Eine neue JobDefs. Kein Job verwendet sie, bis ein Job sie nennt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1376" />
        <source>The Director checks the new %1 %2...</source>
        <translation>Der Director prüft die neue Ressource %1 %2...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1388" />
        <source>%1 %2 was not copied: copying needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 %2 wurde nicht kopiert: Das Kopieren erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors. Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1408" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1417" />
        <source>%1 %2 was not copied: %3</source>
        <translation>%1 %2 wurde nicht kopiert: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1421" />
        <source>A copy of %1 %2 with the new name.</source>
        <translation>Eine Kopie von %1 %2 mit dem neuen Namen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1427" />
        <source>The Director checks the copy %1...</source>
        <translation>Der Director prüft die Kopie %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1442" />
        <source>The Director reloaded its configuration.</source>
        <translation>Der Director hat seine Konfiguration neu geladen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1444" />
        <source>Follow-up commands: %1</source>
        <translation>Folgebefehle: %1</translation>
    </message>
</context>
<context>
    <name>BJobDetailsDialog</name>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="31" />
        <source>Job Details: %1 (ID: %2) - Client: %3</source>
        <translation>Job-Details: %1 (ID: %2) - Client: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="70" />
        <source>Files</source>
        <translation>Dateien</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="79" />
        <source>Job Information</source>
        <translation>Job-Informationen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="82" />
        <source>Job ID:</source>
        <translation>Job-ID:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="83" />
        <source>Job Name:</source>
        <translation>Job-Name:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="84" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="88" />
        <source>Backup</source>
        <translation>Backup</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="89" />
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="93" />
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="94" />
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="95" />
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="97" />
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="101" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="102" />
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="108" />
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="236" />
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="116" />
        <source>Timing</source>
        <translation>Zeiten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="123" />
        <source>Start Time:</source>
        <translation>Startzeit:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="127" />
        <source>Duration:</source>
        <translation>Dauer:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="132" />
        <source>Statistics</source>
        <translation>Statistik</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="136" />
        <source>Files Backed Up:</source>
        <translation>Gesicherte Dateien:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="143" />
        <source>Data Size:</source>
        <translation>Datengröße:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="148" />
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="165" />
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="159" />
        <source>Job Status:</source>
        <translation>Job-Status:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="179" />
        <source>Loading job log...</source>
        <translation>Lade Job-Protokoll...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="183" />
        <source>Logs</source>
        <translation>Protokolle</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="196" />
        <source>No connection to the Director.</source>
        <translation>Keine Verbindung zum Director.</translation>
    </message>
</context>
<context>
    <name>BJobLogDialog</name>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="27" />
        <source>Job Log: %1 (ID: %2) - %3</source>
        <translation>Job-Protokoll: %1 (ID: %2) - %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="42" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="43" />
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="44" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="45" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="48" />
        <source>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</source>
        <translation>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="65" />
        <source>Copy Selection</source>
        <translation>Auswahl kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="71" />
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
</context>
<context>
    <name>BJobLogView</name>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="67" />
        <source>No log entries for this job.</source>
        <translation>Keine Protokolleinträge für diesen Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="71" />
        <source>Could not load the job log: %1</source>
        <translation>Das Job-Protokoll konnte nicht geladen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="101" />
        <source>No connection to the Director.</source>
        <translation>Keine Verbindung zum Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="105" />
        <source>Loading job log...</source>
        <translation>Lade Job-Protokoll...</translation>
    </message>
</context>
<context>
    <name>BJobPreviewPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="697" />
        <source>Preview &amp; Deploy</source>
        <translation>Vorschau &amp; Bereitstellung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="698" />
        <source>Review the configuration and deploy to the Director.</source>
        <translation>Prüfen Sie die Konfiguration und stellen Sie sie auf dem Director bereit.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="703" />
        <source>Configuration:</source>
        <translation>Konfiguration:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="715" />
        <source>Copy Configuration</source>
        <translation>Konfiguration kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="722" />
        <source>Director Command:</source>
        <translation>Director-Befehl:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="734" />
        <source>Copy Command</source>
        <translation>Befehl kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="787" />
        <source>Note: RunScript blocks cannot be sent via 'configure add'. The command above creates the job without RunScript blocks. Deploy the full configuration manually using 'Copy Configuration' to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.</source>
        <translation>Hinweis: RunScript-Blöcke können nicht über 'configure add' gesendet werden. Der obige Befehl legt den Job ohne RunScript-Blöcke an. Stellen Sie die vollständige Konfiguration manuell bereit, indem Sie sie mit „Konfiguration kopieren“ nach /etc/bareos/bareos-dir.d/job/%1.conf übernehmen, und laden Sie den Director neu.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="810" />
        <source>Deploy to Director</source>
        <translation>Auf dem Director bereitstellen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="811" />
        <source>Send 'configure add' to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>'configure add' an den Director senden?

Klicken Sie auf Ja zum Ausführen, auf Nein zum Schließen ohne Bereitstellung.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="833" />
        <source>Sending configure command...</source>
        <translation>Sende configure-Befehl...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="868" />
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="877" />
        <source>Resource created successfully.</source>
        <translation>Ressource erfolgreich angelegt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="885" />
        <source>Resource created. Reloading Director...</source>
        <translation>Ressource angelegt. Director wird neu geladen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="908" />
        <source>Resource created and Director reloaded successfully.</source>
        <translation>Ressource angelegt und Director erfolgreich neu geladen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="911" />
        <source>Resource created, but reloading the Director failed. Run 'reload' on the Director.</source>
        <translation>Ressource angelegt, aber das Neuladen des Directors ist fehlgeschlagen. Führen Sie 'reload' auf dem Director aus.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="924" />
        <source>Resource created, but the connection to the Director was lost before the reload was confirmed.</source>
        <translation>Ressource angelegt, aber die Verbindung zum Director ging verloren, bevor das Neuladen bestätigt wurde.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="928" />
        <source>The connection to the Director was lost before it answered. Reconnect and check whether the resource exists.</source>
        <translation>Die Verbindung zum Director ging verloren, bevor er antwortete. Verbinden Sie sich neu und prüfen Sie, ob die Ressource existiert.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="940" />
        <source>Resource created, but the Director did not confirm the reload in time. Run 'reload' on the Director if the job does not appear.</source>
        <translation>Ressource angelegt, aber der Director hat das Neuladen nicht rechtzeitig bestätigt. Führen Sie 'reload' auf dem Director aus, falls der Job nicht erscheint.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="943" />
        <source>Timeout waiting for Director response. The command may still be processing.</source>
        <translation>Zeitüberschreitung beim Warten auf die Antwort des Directors. Der Befehl wird möglicherweise noch verarbeitet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="952" />
        <source>Configuration copied to clipboard.</source>
        <translation>Konfiguration in die Zwischenablage kopiert.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="959" />
        <source>Command copied to clipboard.</source>
        <translation>Befehl in die Zwischenablage kopiert.</translation>
    </message>
</context>
<context>
    <name>BJobResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
</context>
<context>
    <name>BJobResourcesPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="339" />
        <source>Resource References</source>
        <translation>Ressourcenverweise</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="340" />
        <source>Select the resources this job uses.</source>
        <translation>Wählen Sie die Ressourcen, die dieser Job verwendet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="356" />
        <source>Required Resources</source>
        <translation>Erforderliche Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="361" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="365" />
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="369" />
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="373" />
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="377" />
        <source>Messages:</source>
        <translation>Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="382" />
        <source>Job Settings</source>
        <translation>Job-Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="388" />
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="392" />
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="396" />
        <source>Catalog:</source>
        <translation>Catalog:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="401" />
        <source>Priority:</source>
        <translation>Priorität:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="406" />
        <source>Max Concurrent Jobs:</source>
        <translation>Max Concurrent Jobs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="411" />
        <source>Restore Settings</source>
        <translation>Restore-Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="415" />
        <source>/tmp/bareos-restores</source>
        <translation>/tmp/bareos-restores</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="416" />
        <source>Where:</source>
        <translation>Where:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="420" />
        <source>Replace:</source>
        <translation>Replace:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="425" />
        <source>Pool Overrides</source>
        <translation>Pool-Überschreibungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="432" />
        <source>Full Backup Pool:</source>
        <translation>Full Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="436" />
        <source>Diff Backup Pool:</source>
        <translation>Diff Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="440" />
        <source>Inc Backup Pool:</source>
        <translation>Inc Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="445" />
        <source>Simple Scripts</source>
        <translation>Einfache Skripte</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="449" />
        <location filename="../src/jobs/bjobwizard.cpp" line="453" />
        <location filename="../src/jobs/bjobwizard.cpp" line="457" />
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="450" />
        <source>Run Before Job:</source>
        <translation>Run Before Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="454" />
        <source>Run After Job:</source>
        <translation>Run After Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="458" />
        <source>Run After Failed Job:</source>
        <translation>Run After Failed Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="461" />
        <location filename="../src/jobs/bjobwizard.cpp" line="465" />
        <source>/path/to/client-script.sh</source>
        <translation>/path/to/client-script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="462" />
        <source>Client Run Before:</source>
        <translation>Client Run Before:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="466" />
        <source>Client Run After:</source>
        <translation>Client Run After:</translation>
    </message>
</context>
<context>
    <name>BJobScriptsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="607" />
        <source>RunScript Blocks</source>
        <translation>RunScript-Blöcke</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="608" />
        <source>Define complex script execution blocks (optional). For simple scripts, use the fields on the previous page.</source>
        <translation>Komplexe Skript-Ausführungsblöcke definieren (optional). Für einfache Skripte verwenden Sie die Felder auf der vorherigen Seite.</translation>
    </message>
</context>
<context>
    <name>BJobWidget</name>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="48" />
        <source>Successful (T)</source>
        <translation>Erfolgreich (T)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="49" />
        <source>Warning (W)</source>
        <translation>Warnung (W)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="51" />
        <source>Error (E)</source>
        <translation>Fehler (E)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="56" />
        <source>Enable Date Filter</source>
        <translation>Datumsfilter aktivieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="400" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1660" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1663" />
        <source>Filter</source>
        <translation>Filter</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="459" />
        <source>Job Name:</source>
        <translation>Job-Name:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="460" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="513" />
        <source>Backup Level</source>
        <translation>Backup-Stufe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="545" />
        <source>FileSet:</source>
        <translation>Dateisatz:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="546" />
        <source>Storage:</source>
        <translation>Speicher:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="547" />
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="533" />
        <source>Date Range</source>
        <translation>Datumsbereich</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="39" />
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="40" />
        <source>Clear History</source>
        <translation>Verlauf löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="50" />
        <source>Failed (F)</source>
        <translation>Fehlgeschlagen (F)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="52" />
        <source>Running (R)</source>
        <translation>Läuft (R)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="53" />
        <source>Canceled (A)</source>
        <translation>Abgebrochen (A)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="54" />
        <location filename="../src/jobs/bjobwidget.cpp" line="356" />
        <source>Zero Bytes</source>
        <translation>Null Bytes</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="81" />
        <source>When enabled, only shows jobs with 0 bytes transferred.
Warning: This filter persists across restarts and may
cause an empty table if no zero-byte jobs exist.</source>
        <translation>Wenn aktiviert, werden nur Jobs mit 0 übertragenen Bytes angezeigt.
Warnung: Dieser Filter bleibt über Neustarts hinweg erhalten und kann
zu einer leeren Tabelle führen, wenn es keine Jobs mit null Bytes gibt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="89" />
        <source>When enabled, only shows jobs within the selected date range.
Warning: This filter persists across restarts and may
cause an empty or limited table if the range is too narrow.</source>
        <translation>Wenn aktiviert, werden nur Jobs im gewählten Zeitraum angezeigt.
Warnung: Dieser Filter bleibt über Neustarts hinweg erhalten und kann
zu einer leeren oder eingeschränkten Tabelle führen, wenn der Zeitraum zu eng ist.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="105" />
        <source>Filter by job name...</source>
        <translation>Nach Job-Name filtern...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="109" />
        <source>Filter by client...</source>
        <translation>Nach Client filtern...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="114" />
        <location filename="../src/jobs/bjobwidget.cpp" line="117" />
        <location filename="../src/jobs/bjobwidget.cpp" line="120" />
        <source>(no job selected)</source>
        <translation>(kein Job ausgewählt)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="133" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Load error</source>
        <translation>&lt;b&gt;Job-Protokoll&lt;/b&gt; - Job: %1 (ID: %2) - Ladefehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="157" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1710" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - No job selected</source>
        <translation>&lt;b&gt;Job-Protokoll&lt;/b&gt; - Kein Job ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="161" />
        <source>-- Select Job Log --</source>
        <translation>-- Job-Protokoll wählen --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="163" />
        <source>Select from previously loaded job logs</source>
        <translation>Aus zuvor geladenen Job-Protokollen wählen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="165" />
        <source>Copy selected lines to clipboard</source>
        <translation>Ausgewählte Zeilen in die Zwischenablage kopieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="168" />
        <source>Clear job log history</source>
        <translation>Verlauf der Job-Protokolle löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="171" />
        <source>Search in log...</source>
        <translation>Im Protokoll suchen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="173" />
        <source>Filter log lines by search text</source>
        <translation>Protokollzeilen nach Suchtext filtern</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="206" />
        <source>Cancelling job...</source>
        <translation>Breche Job ab...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="209" />
        <source>Deleting job...</source>
        <translation>Lösche Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="212" />
        <source>Running job again...</source>
        <translation>Führe Job erneut aus...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="215" />
        <source>Loading job log...</source>
        <translation>Lade Job-Protokoll...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="219" />
        <source>Pruning expired jobs...</source>
        <translation>Prune abgelaufener Jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="260" />
        <source>⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. This filter persists across restarts!</source>
        <translation>⚠ Null-Bytes-Filter aktiv — nur Jobs mit 0 Bytes werden angezeigt. Dieser Filter bleibt über Neustarts hinweg erhalten!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="270" />
        <source>⚠ Date filter active — only jobs within the selected range are shown. This filter persists across restarts!</source>
        <translation>⚠ Datumsfilter aktiv — nur Jobs im gewählten Zeitraum werden angezeigt. Dieser Filter bleibt über Neustarts hinweg erhalten!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="359" />
        <source>Date Range (%1 – %2)</source>
        <translation>Zeitraum (%1 – %2)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="372" />
        <source>Status (hidden: %1)</source>
        <translation>Status (ausgeblendet: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="377" />
        <source>⚠ Active filters from last session: %1 — results may be limited</source>
        <translation>⚠ Aktive Filter aus der letzten Sitzung: %1 — Ergebnisse sind möglicherweise eingeschränkt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="403" />
        <source>Show/hide filter</source>
        <translation>Filter ein-/ausblenden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="409" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="413" />
        <source>Auto-Refresh</source>
        <translation>Automatisch aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="415" />
        <source>5 s</source>
        <translation>5 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="416" />
        <source>10 s</source>
        <translation>10 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="417" />
        <source>30 s</source>
        <translation>30 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="418" />
        <source>1 min</source>
        <translation>1 min</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="419" />
        <source>5 min</source>
        <translation>5 min</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="464" />
        <source>&lt;b&gt;Status:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Status:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="480" />
        <source>Basic Filters</source>
        <translation>Grundfilter</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="502" />
        <source>Loading...</source>
        <translation>Wird geladen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="527" />
        <source>From:</source>
        <translation>Von:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="528" />
        <source>To:</source>
        <translation>Bis:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="552" />
        <source>Job Details</source>
        <translation>Job-Details</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="557" />
        <source>Reset filter</source>
        <translation>Filter zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="678" />
        <source>Updated: %1 - %2 jobs loaded</source>
        <translation>Aktualisiert: %1 - %2 Jobs geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="723" />
        <source>No selection</source>
        <translation>Keine Auswahl</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="724" />
        <source>Please select a job to cancel.</source>
        <translation>Bitte wählen Sie einen Job zum Abbrechen aus.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="735" />
        <source>Cancel Job</source>
        <translation>Job abbrechen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="736" />
        <source>Do you really want to cancel job ID %1?</source>
        <translation>Möchten Sie den Job mit der ID %1 wirklich abbrechen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="744" />
        <source>Job Cancelled</source>
        <translation>Job abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="745" />
        <source>Job %1 was cancelled.</source>
        <translation>Job %1 wurde abgebrochen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1142" />
        <source>Loading page %1...</source>
        <translation>Lade Seite %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1155" />
        <source>Refreshing job list...</source>
        <translation>Aktualisiere Job-Liste...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1280" />
        <source>%1 jobs loaded</source>
        <translation>%1 Jobs geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1515" />
        <source>Auto-refresh on (%1)</source>
        <translation>Auto-Refresh aktiviert (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1520" />
        <source>Auto-refresh off</source>
        <translation>Auto-Refresh deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1615" />
        <source>Filtered: %1 of %2 jobs</source>
        <translation>Gefiltert: %1 von %2 Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1661" />
        <source>Hide filter</source>
        <translation>Filter ausblenden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1664" />
        <source>Show filter</source>
        <translation>Filter einblenden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1692" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Loading...</source>
        <translation>&lt;b&gt;Job-Protokoll&lt;/b&gt; - Job: %1 (ID: %2) - Lädt...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1730" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1867" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines</source>
        <translation>&lt;b&gt;Job-Protokoll&lt;/b&gt; - Job: %1 (ID: %2) - %3 Zeilen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="589" />
        <source>History:</source>
        <translation>Verlauf:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="609" />
        <source>Job Log</source>
        <translation>Job-Protokoll</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="612" />
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1194" />
        <source>Error parsing JSON data</source>
        <translation>Fehler beim Auswerten der JSON-Daten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1546" />
        <source>Created</source>
        <translation>Erstellt</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1547" />
        <source>Running</source>
        <translation>Läuft</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1548" />
        <source>Blocked</source>
        <translation>Blockiert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1549" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1550" />
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1551" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1552" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1553" />
        <source>Non-fatal Error</source>
        <translation>Nicht schwerwiegender Fehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1554" />
        <source>Canceled</source>
        <translation>Abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1608" />
        <source>⚠ No jobs visible — 'Zero Bytes' filter is active!</source>
        <translation>⚠ Keine Jobs sichtbar — Filter „Null Bytes“ ist aktiv!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1610" />
        <source>⚠ No jobs visible — check your filter settings (%1 jobs loaded)</source>
        <translation>⚠ Keine Jobs sichtbar — prüfen Sie Ihre Filtereinstellungen (%1 Jobs geladen)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1619" />
        <source>%1 Jobs</source>
        <translation>%1 Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1725" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines, job is running</source>
        <translation>&lt;b&gt;Job-Protokoll&lt;/b&gt; - Job: %1 (ID: %2) - %3 Zeilen, Job läuft</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1891" />
        <source>Copied %1 log lines to clipboard</source>
        <translation>%1 Protokollzeilen in die Zwischenablage kopiert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1906" />
        <source>Copied %1 selected lines to clipboard</source>
        <translation>%1 ausgewählte Zeilen in die Zwischenablage kopiert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1920" />
        <source>Job log history cleared</source>
        <translation>Verlauf der Job-Protokolle gelöscht</translation>
    </message>
</context>
<context>
    <name>BJobWizard</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="990" />
        <source>New Job Wizard</source>
        <translation>Assistent für neuen Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="991" />
        <source>New JobDefs Wizard</source>
        <translation>Assistent für neue JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1001" />
        <source>Finish</source>
        <translation>Fertigstellen</translation>
    </message>
</context>
<context>
    <name>BJobsModel</name>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="253" />
        <source>Job ID</source>
        <translation>Job-ID</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="254" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="255" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="256" />
        <source>Start Time</source>
        <translation>Startzeit</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="257" />
        <source>End Time</source>
        <translation>Endzeit</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="258" />
        <source>Duration</source>
        <translation>Dauer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="259" />
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="260" />
        <source>Level</source>
        <translation>Stufe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="261" />
        <source>Files</source>
        <translation>Dateien</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="262" />
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="263" />
        <source>Status</source>
        <translation>Status</translation>
    </message>
</context>
<context>
    <name>BJobsStatisticsWidget</name>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="19" />
        <source>Job Summary</source>
        <translation>Job-Übersicht</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="25" />
        <source>Total Jobs:</source>
        <translation>Jobs gesamt:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="28" />
        <source>Selected:</source>
        <translation>Ausgewählt:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="33" />
        <source>Status Breakdown</source>
        <translation>Aufschlüsselung nach Status</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="40" />
        <source>Successful:</source>
        <translation>Erfolgreich:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="55" />
        <source>Warnings:</source>
        <translation>Warnungen:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="70" />
        <source>Failed:</source>
        <translation>Fehlgeschlagen:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="84" />
        <source>Data Summary</source>
        <translation>Datenübersicht</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="88" />
        <source>Total Files:</source>
        <translation>Dateien gesamt:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="91" />
        <source>Total Size:</source>
        <translation>Größe gesamt:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="93" />
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="181" />
        <source>N/A</source>
        <translation>k. A.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="94" />
        <source>Date Range:</source>
        <translation>Zeitraum:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="176" />
        <source>%1 to %2</source>
        <translation>%1 bis %2</translation>
    </message>
</context>
<context>
    <name>BJsonJobView</name>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="526" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="612" />
        <source>Delete %1 Jobs</source>
        <translation>%1 Jobs löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="157" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="526" />
        <source>Delete Job</source>
        <translation>Job löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="146" />
        <source>Show Details...</source>
        <translation>Details anzeigen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="151" />
        <source>Retry Job</source>
        <translation>Job wiederholen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="154" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="720" />
        <source>Cancel Job</source>
        <translation>Job abbrechen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="162" />
        <source>View Log...</source>
        <translation>Protokoll anzeigen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="165" />
        <source>Restore Files...</source>
        <translation>Dateien wiederherstellen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="172" />
        <source>Export to JSON...</source>
        <translation>Als JSON exportieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="176" />
        <source>Export to CSV...</source>
        <translation>Als CSV exportieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="183" />
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="187" />
        <source>Invert Selection</source>
        <translation>Auswahl umkehren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="191" />
        <source>Clear Selection</source>
        <translation>Auswahl aufheben</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272" />
        <source>Export Selected Jobs to JSON</source>
        <translation>Ausgewählte Jobs als JSON exportieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272" />
        <source>Export All Jobs to JSON</source>
        <translation>Alle Jobs als JSON exportieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="285" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="318" />
        <source>Export Error</source>
        <translation>Exportfehler</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="286" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="319" />
        <source>Could not write to file: %1</source>
        <translation>Datei konnte nicht geschrieben werden: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="293" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="326" />
        <source>Export Successful</source>
        <translation>Export erfolgreich</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjsonjobview.cpp" line="294" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="327" />
        <source>Exported %n job(s) to %1</source>
        <translation>
            <numerusform>%n Job nach %1 exportiert</numerusform>
            <numerusform>%n Jobs nach %1 exportiert</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305" />
        <source>Export Selected Jobs to CSV</source>
        <translation>Ausgewählte Jobs als CSV exportieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305" />
        <source>Export All Jobs to CSV</source>
        <translation>Alle Jobs als CSV exportieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="339" />
        <source>Total Jobs: %1
</source>
        <translation>Jobs gesamt: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="340" />
        <source>Successful: %1
</source>
        <translation>Erfolgreich: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="341" />
        <source>Warnings: %1
</source>
        <translation>Warnungen: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="342" />
        <source>Failed: %1

</source>
        <translation>Fehlgeschlagen: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="344" />
        <source>Total Files: %1
</source>
        <translation>Dateien gesamt: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="345" />
        <source>Total Size: %1 bytes

</source>
        <translation>Größe gesamt: %1 Bytes

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="348" />
        <source>Date Range:
</source>
        <translation>Zeitraum:
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="349" />
        <source>  From: %1
</source>
        <translation>  Von: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="350" />
        <source>  To: %1

</source>
        <translation>  Bis: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="353" />
        <source>Selected: %1</source>
        <translation>Ausgewählt: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="355" />
        <source>Job Statistics</source>
        <translation>Job-Statistik</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="420" />
        <source>Export All to JSON...</source>
        <translation>Alle als JSON exportieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="421" />
        <source>Export All to CSV...</source>
        <translation>Alle als CSV exportieren...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="536" />
        <source>ID %1: %2 (%3)</source>
        <translation>ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="542" />
        <source>&lt;b&gt;%1 jobs selected for deletion:&lt;/b&gt;&lt;br&gt;%2</source>
        <translation>&lt;b&gt;%1 Jobs zum Löschen ausgewählt:&lt;/b&gt;&lt;br&gt;%2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="551" />
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="552" />
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="553" />
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="555" />
        <source>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Level:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Start:&lt;/b&gt; %5</source>
        <translation>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Level:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Start:&lt;/b&gt; %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="569" />
        <source>&lt;div style='background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'&gt;&lt;b&gt;⚠ Attention:&lt;/b&gt; %1 dependent Incremental/Differential job(s) found!&lt;br&gt;&lt;br&gt;Deleting Full backups may make dependent backups unusable for restore.&lt;/div&gt;</source>
        <translation>&lt;div style='background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'&gt;&lt;b&gt;⚠ Achtung:&lt;/b&gt; %1 abhängige(r) Incremental-/Differential-Job(s) gefunden!&lt;br&gt;&lt;br&gt;Das Löschen von Full-Backups kann abhängige Backups für die Wiederherstellung unbrauchbar machen.&lt;/div&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="580" />
        <source>Delete Mode</source>
        <translation>Löschmodus</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="583" />
        <source>Delete job record only</source>
        <translation>Nur Job-Eintrag löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="584" />
        <source>Removes the job entry from the catalog database.
The backup data on the storage media remains intact.</source>
        <translation>Entfernt den Job-Eintrag aus der Katalogdatenbank.
Die Backup-Daten auf den Speichermedien bleiben erhalten.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="593" />
        <source>Prune (remove expired jobs by retention policy)</source>
        <translation>Prune (abgelaufene Jobs gemäß Aufbewahrungsrichtlinie entfernen)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="594" />
        <source>Removes job records that have exceeded their configured retention period.
Only expired jobs for the affected client(s) will be removed.</source>
        <translation>Entfernt Job-Einträge, deren konfigurierte Aufbewahrungsfrist überschritten ist.
Nur abgelaufene Jobs der betroffenen Clients werden entfernt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="603" />
        <source>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warning:&lt;/b&gt; This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warnung:&lt;/b&gt; Diese Aktion kann nicht rückgängig gemacht werden!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="612" />
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="626" />
        <source>  ID %1: %2 (%3)</source>
        <translation>  ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="631" />
        <source>Confirm Delete</source>
        <translation>Löschen bestätigen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="632" />
        <source>Are you sure you want to delete %1 job(s)?

%2

%3 dependent Incremental/Differential backup(s) may become unusable for restore.</source>
        <translation>Möchten Sie %1 Job(s) wirklich löschen?

%2

%3 abhängige Incremental-/Differential-Backup(s) können für die Wiederherstellung unbrauchbar werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="688" />
        <source>Rerun Job</source>
        <translation>Job erneut ausführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="689" />
        <source>Do you want to run job %1 (%2) again?</source>
        <translation>Möchten Sie den Job %1 (%2) erneut ausführen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="721" />
        <source>Do you really want to cancel the running job %1 (%2)?</source>
        <translation>Möchten Sie den laufenden Job %1 (%2) wirklich abbrechen?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="750" />
        <source>Not connected</source>
        <translation>Keine Verbindung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="751" />
        <source>No connection to the Director.</source>
        <translation>Keine Verbindung zum Director verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="772" />
        <source>Not Connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="773" />
        <source>No director connection available.</source>
        <translation>Keine Director-Verbindung verfügbar.</translation>
    </message>
</context>
<context>
    <name>BMainWindow</name>
    <message>
        <location filename="../ui/bmainwindow.ui" line="14" />
        <source>Bacula Qt UI</source>
        <translation>Bacula Qt UI</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="142" />
        <source>Interface zoom: %1 %</source>
        <translation>Oberflächenzoom: %1 %</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="202" />
        <source>Connecting...</source>
        <translation>Verbinde...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="205" />
        <source>Authenticating...</source>
        <translation>Authentifiziere...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="208" />
        <source>Setting API mode...</source>
        <translation>Setze API-Modus...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="211" />
        <source>Loading resources...</source>
        <translation>Lade Ressourcen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="214" />
        <source>Connected and ready</source>
        <translation>Verbunden und bereit</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="217" />
        <source>Connection error</source>
        <translation>Verbindungsfehler</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="220" />
        <location filename="../src/bmainwindow.cpp" line="1694" />
        <source>Disconnected</source>
        <translation>Getrennt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="227" />
        <source>Loading resources... %1/%2</source>
        <translation>Lade Ressourcen... %1/%2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="249" />
        <location filename="../src/bmainwindow.cpp" line="686" />
        <location filename="../src/bmainwindow.cpp" line="2525" />
        <source>Ready</source>
        <translation>Bereit</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="400" />
        <location filename="../src/bmainwindow.cpp" line="2875" />
        <source>Auto-refresh enabled (%1s)</source>
        <translation>Auto-Refresh aktiviert (%1 s)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="404" />
        <location filename="../src/bmainwindow.cpp" line="2880" />
        <source>Auto-refresh disabled</source>
        <translation>Auto-Refresh deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="472" />
        <source>Job: %1</source>
        <translation>Job: %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="533" />
        <location filename="../src/bmainwindow.cpp" line="690" />
        <location filename="../src/bmainwindow.cpp" line="2441" />
        <source>Not connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="609" />
        <source>Storage/Volumes</source>
        <translation>Storage/Volumes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="667" />
        <source>Statistics</source>
        <translation>Statistiken</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="700" />
        <location filename="../src/bmainwindow.cpp" line="717" />
        <location filename="../src/bmainwindow.cpp" line="1629" />
        <location filename="../src/bmainwindow.cpp" line="1886" />
        <source>Connect</source>
        <translation>Verbinden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="705" />
        <location filename="../src/bmainwindow.cpp" line="723" />
        <location filename="../src/bmainwindow.cpp" line="1887" />
        <source>Reconnect</source>
        <translation>Neu verbinden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="711" />
        <source>Disconnect</source>
        <translation>Trennen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="719" />
        <location filename="../src/bmainwindow.cpp" line="1551" />
        <location filename="../src/bmainwindow.cpp" line="2340" />
        <source>Connect to Director</source>
        <translation>Mit Director verbinden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="725" />
        <source>Reconnect to last used connection</source>
        <translation>Mit der zuletzt verwendeten Verbindung neu verbinden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="729" />
        <location filename="../src/bmainwindow.cpp" line="1888" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="735" />
        <location filename="../src/bmainwindow.cpp" line="1889" />
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="740" />
        <location filename="../src/bmainwindow.cpp" line="1890" />
        <source>Exit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="745" />
        <location filename="../src/bmainwindow.cpp" line="1719" />
        <source>About Onesimus</source>
        <translation>Über Onesimus</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="749" />
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="753" />
        <source>Online Documentation</source>
        <translation>Online-Dokumentation</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="758" />
        <source>Report a Bug...</source>
        <translation>Fehler melden...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="762" />
        <location filename="../src/bmainwindow.cpp" line="1866" />
        <location filename="../src/bmainwindow.cpp" line="1872" />
        <location filename="../src/bmainwindow.cpp" line="1953" />
        <source>Keyboard Shortcuts</source>
        <translation>Tastenkürzel</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="767" />
        <location filename="../src/bmainwindow.cpp" line="2287" />
        <source>My Permissions</source>
        <translation>Meine Berechtigungen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="773" />
        <location filename="../src/bmainwindow.cpp" line="1903" />
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="779" />
        <location filename="../src/bmainwindow.cpp" line="1904" />
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="785" />
        <source>Clear Selection</source>
        <translation>Auswahl aufheben</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="790" />
        <source>Find...</source>
        <translation>Suchen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="797" />
        <location filename="../src/bmainwindow.cpp" line="1918" />
        <source>Show Statistics</source>
        <translation>Statistik anzeigen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="811" />
        <location filename="../src/bmainwindow.cpp" line="1919" />
        <source>Show Job Log</source>
        <translation>Job-Protokoll anzeigen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="826" />
        <location filename="../src/bmainwindow.cpp" line="1921" />
        <source>Interface Zoom In</source>
        <translation>Oberfläche vergrößern</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="832" />
        <location filename="../src/bmainwindow.cpp" line="1922" />
        <source>Interface Zoom Out</source>
        <translation>Oberfläche verkleinern</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="844" />
        <source>Toggle Dark/Light Theme</source>
        <translation>Dunkles/helles Design umschalten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="858" />
        <source>Run Job</source>
        <translation>Job ausführen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="863" />
        <source>Cancel Job</source>
        <translation>Job abbrechen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="868" />
        <source>Show Details</source>
        <translation>Details anzeigen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="873" />
        <location filename="../src/bmainwindow.cpp" line="1936" />
        <source>Refresh Jobs</source>
        <translation>Jobs aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="879" />
        <source>Export Jobs as JSON...</source>
        <translation>Jobs als JSON exportieren...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="884" />
        <source>Export Jobs as CSV...</source>
        <translation>Jobs als CSV exportieren...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="890" />
        <source>Add Job...</source>
        <translation>Job hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="909" />
        <source>Edit Job...</source>
        <translation>Job bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="915" />
        <source>Edit Job</source>
        <translation>Job bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="916" />
        <source>No Jobs available.</source>
        <translation>Keine Jobs verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="922" />
        <source>Job Resources</source>
        <translation>Job-Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="975" />
        <source>Delete Job...</source>
        <translation>Job löschen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="983" />
        <source>Add JobDefs...</source>
        <translation>JobDefs hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1002" />
        <source>Edit JobDefs...</source>
        <translation>JobDefs bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1008" />
        <source>JobDefs Resources</source>
        <translation>JobDefs-Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1063" />
        <source>Delete JobDefs...</source>
        <translation>JobDefs löschen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074" />
        <location filename="../src/bmainwindow.cpp" line="1078" />
        <source>Delete JobDefs</source>
        <translation>JobDefs löschen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074" />
        <source>No JobDefs available.</source>
        <translation>Keine JobDefs verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1078" />
        <source>JobDefs to delete:</source>
        <translation>Zu löschende JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1092" />
        <source>Jobs that use this JobDefs</source>
        <translation>Jobs, die diese JobDefs verwenden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1098" />
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1108" />
        <source>Add FileSet...</source>
        <translation>FileSet hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1116" />
        <source>Edit FileSet...</source>
        <translation>FileSet bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1124" />
        <source>Delete FileSet...</source>
        <translation>FileSet löschen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1133" />
        <location filename="../src/bmainwindow.cpp" line="1139" />
        <location filename="../src/bmainwindow.cpp" line="1183" />
        <location filename="../src/bmainwindow.cpp" line="1188" />
        <source>Delete FileSet</source>
        <translation>FileSet löschen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1134" />
        <source>No FileSets available.</source>
        <translation>Keine FileSets verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1144" />
        <source>Select the FileSet to delete:</source>
        <translation>Zu löschendes FileSet wählen:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1154" />
        <source>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warning:&lt;/b&gt; This will remove the FileSet resource from the Director configuration. This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warnung:&lt;/b&gt; Dadurch wird die FileSet-Ressource aus der Director-Konfiguration entfernt. Diese Aktion kann nicht rückgängig gemacht werden!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1164" />
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1176" />
        <source>Confirm Delete</source>
        <translation>Löschen bestätigen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1177" />
        <source>Are you sure you want to delete FileSet "%1"?</source>
        <translation>Möchten Sie das FileSet „%1“ wirklich löschen?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1184" />
        <source>Not connected to a Director. FileSet "%1" was not deleted.</source>
        <translation>Nicht mit einem Director verbunden. FileSet „%1“ wurde nicht gelöscht.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1189" />
        <source>FileSet "%1" no longer exists on the Director. Nothing was deleted.</source>
        <translation>FileSet „%1“ existiert auf dem Director nicht mehr. Es wurde nichts gelöscht.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1197" />
        <source>Add New Client...</source>
        <translation>Neuen Client hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1205" />
        <location filename="../src/bmainwindow.cpp" line="1937" />
        <source>Refresh Clients</source>
        <translation>Clients aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1211" />
        <source>Show Client Details</source>
        <translation>Client-Details anzeigen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1221" />
        <location filename="../src/bmainwindow.cpp" line="1938" />
        <source>Refresh Storage</source>
        <translation>Storage aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1228" />
        <source>Add Schedule...</source>
        <translation>Schedule hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1250" />
        <location filename="../src/bmainwindow.cpp" line="1939" />
        <source>Refresh Schedules</source>
        <translation>Schedules aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1257" />
        <source>Database Cleanup...</source>
        <translation>Datenbankbereinigung...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1259" />
        <source>Clean up old backups and free disk space</source>
        <translation>Alte Backups bereinigen und Speicherplatz freigeben</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1263" />
        <source>Connection Wizard...</source>
        <translation>Verbindungsassistent...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1265" />
        <source>Set up a new director connection</source>
        <translation>Neue Director-Verbindung einrichten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1268" />
        <source>Import Config...</source>
        <translation>Konfiguration importieren...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1270" />
        <source>Import Director configuration from files or ZIP archive</source>
        <translation>Director-Konfiguration aus Dateien oder ZIP-Archiv importieren</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1279" />
        <location filename="../src/bmainwindow.cpp" line="1319" />
        <location filename="../src/bmainwindow.cpp" line="1335" />
        <source>Language</source>
        <translation>Sprache</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1285" />
        <source>System (%1)</source>
        <translation>System (%1)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1320" />
        <source>Onesimus will speak %1 after a restart.</source>
        <translation>Onesimus spricht nach einem Neustart %1.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1323" />
        <source>Restart now? The connection to the Director is closed and opened again if automatic connection is on.</source>
        <translation>Jetzt neu starten? Die Verbindung zum Director wird getrennt und, wenn die automatische Verbindung eingeschaltet ist, wieder aufgebaut.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1325" />
        <source>Restart now?</source>
        <translation>Jetzt neu starten?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1326" />
        <source>Restart now</source>
        <translation>Jetzt neu starten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1327" />
        <source>Later</source>
        <translation>Später</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1336" />
        <source>Onesimus could not be started again. The new language takes effect at the next start.</source>
        <translation>Onesimus konnte nicht neu gestartet werden. Die neue Sprache gilt beim nächsten Start.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1346" />
        <location filename="../src/bmainwindow.cpp" line="1894" />
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1360" />
        <location filename="../src/bmainwindow.cpp" line="1909" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1369" />
        <location filename="../src/bmainwindow.cpp" line="1927" />
        <source>View</source>
        <translation>Ansicht</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1438" />
        <source>ACL matrix of the consoles...</source>
        <translation>ACL-Matrix der Konsolen...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1439" />
        <source>Which console may use which job, client, storage, schedule, pool, fileset, catalog and command (Onesimus Director)</source>
        <translation>Welche Konsole welchen Job, Client, Storage, Schedule, Pool, FileSet, Catalog und Befehl verwenden darf (Onesimus Director)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bmainwindow.cpp" line="2799" />
        <source>%n row(s) copied</source>
        <translation>
            <numerusform>%n Zeile kopiert</numerusform>
            <numerusform>%n Zeilen kopiert</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="601" />
        <location filename="../src/bmainwindow.cpp" line="1380" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1388" />
        <source>Modify Jobs</source>
        <translation>Jobs ändern</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1394" />
        <source>Modify JobDefs</source>
        <translation>JobDefs ändern</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1400" />
        <source>Filesets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="605" />
        <location filename="../src/bmainwindow.cpp" line="1410" />
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1417" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="638" />
        <location filename="../src/bmainwindow.cpp" line="1421" />
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="97" />
        <source>Onesimus - Bacula Backup Management</source>
        <translation>Onesimus - Bacula-Backup-Verwaltung</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="99" />
        <source>Onesimus - Bareos Backup Management</source>
        <translation>Onesimus - Bareos-Backup-Verwaltung</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1427" />
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1443" />
        <location filename="../src/bmainwindow.cpp" line="1957" />
        <source>Help</source>
        <translation>Hilfe</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1472" />
        <source>Main Toolbar</source>
        <translation>Hauptwerkzeugleiste</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1501" />
        <location filename="../src/bmainwindow.cpp" line="1515" />
        <location filename="../src/bmainwindow.cpp" line="2414" />
        <source>Statistics ▼</source>
        <translation>Statistik ▼</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1504" />
        <source>Show/Hide Statistics</source>
        <translation>Statistik ein-/ausblenden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1515" />
        <location filename="../src/bmainwindow.cpp" line="2414" />
        <location filename="../src/bmainwindow.cpp" line="2450" />
        <source>Statistics ▶</source>
        <translation>Statistik ▶</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1543" />
        <source>No Connection Profiles</source>
        <translation>Keine Verbindungsprofile</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1544" />
        <source>No connection profiles found.

Please create a connection profile in Settings first.</source>
        <translation>Keine Verbindungsprofile gefunden.

Bitte legen Sie zuerst in den Einstellungen ein Verbindungsprofil an.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1558" />
        <source>Select Connection Profile</source>
        <translation>Verbindungsprofil wählen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1564" />
        <source>Connection Profile</source>
        <translation>Verbindungsprofil</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1613" />
        <source>Manage Profiles...</source>
        <translation>Profile verwalten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1641" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1642" />
        <source>Selected profile is invalid.</source>
        <translation>Das ausgewählte Profil ist ungültig.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1680" />
        <location filename="../src/bmainwindow.cpp" line="2539" />
        <location filename="../src/bmainwindow.cpp" line="2761" />
        <source>Connecting to %1...</source>
        <translation>Verbinde mit %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1739" />
        <source>&lt;p style='color:#666; margin:0;'&gt;Version %1 (Build %2)&lt;/p&gt;</source>
        <translation>&lt;p style='color:#666; margin:0;'&gt;Version %1 (Build %2)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1745" />
        <source>&lt;p style='margin:0;'&gt;Modern Qt6 GUI for Bacula Backup&lt;/p&gt;</source>
        <translation>&lt;p style='margin:0;'&gt;Moderne Qt6-Oberfläche für Bacula-Backups&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1747" />
        <source>&lt;p style='margin:0;'&gt;Modern Qt6 GUI for Bareos Backup&lt;/p&gt;</source>
        <translation>&lt;p style='margin:0;'&gt;Moderne Qt6-Oberfläche für Bareos-Backups&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1831" />
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1847" />
        <source>Documentation</source>
        <translation>Dokumentation</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1848" />
        <location filename="../src/bmainwindow.cpp" line="1858" />
        <source>Could not open browser. Please visit:
%1</source>
        <translation>Browser konnte nicht geöffnet werden. Bitte besuchen Sie:
%1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1857" />
        <source>Report Bug</source>
        <translation>Fehler melden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1905" />
        <source>Find</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1920" />
        <source>Toggle Theme</source>
        <translation>Design umschalten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1923" />
        <source>Reset Interface Zoom</source>
        <translation>Oberflächenzoom zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1943" />
        <source>Data</source>
        <translation>Daten</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1952" />
        <source>Help Contents</source>
        <translation>Hilfeinhalte</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2288" />
        <source>Not connected to a Director.</source>
        <translation>Nicht mit einem Director verbunden.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2310" />
        <source>Settings saved</source>
        <translation>Einstellungen gespeichert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2337" />
        <source>Disconnect from Director</source>
        <translation>Vom Director trennen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2400" />
        <source>Connected to %1 version %2</source>
        <translation>Verbunden mit %1 Version %2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2402" />
        <source>Connected: version %1</source>
        <translation>Verbunden: Version %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2408" />
        <source>Connected - Loading data...</source>
        <translation>Verbunden – lade Daten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2473" />
        <source>Error: </source>
        <translation>Fehler: </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2480" />
        <source>Connection Error</source>
        <translation>Verbindungsfehler</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2495" />
        <source>Refreshing data...</source>
        <translation>Aktualisiere Daten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2534" />
        <source>No Saved Connection</source>
        <translation>Keine gespeicherte Verbindung</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2535" />
        <source>No previous connection found. Please use 'Connect' to establish a new connection.</source>
        <translation>Keine frühere Verbindung gefunden. Bitte verwenden Sie „Verbinden“, um eine neue Verbindung herzustellen.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2715" />
        <source>Auto-connecting to %1...</source>
        <translation>Automatische Verbindung mit %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2742" />
        <source>Connection cancelled: server certificate not trusted</source>
        <translation>Verbindung abgebrochen: Serverzertifikat nicht vertrauenswürdig</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2805" />
        <source>Copying client data is not implemented yet</source>
        <translation>Client-Daten kopieren noch nicht implementiert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2818" />
        <source>All jobs selected</source>
        <translation>Alle Jobs ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2821" />
        <source>Selecting all clients is not implemented yet</source>
        <translation>Alle Clients auswählen noch nicht implementiert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2833" />
        <source>Selection cleared</source>
        <translation>Auswahl aufgehoben</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2836" />
        <source>Clearing the client selection is not implemented yet</source>
        <translation>Auswahl für Clients aufheben noch nicht implementiert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2847" />
        <location filename="../src/bmainwindow.cpp" line="2857" />
        <source>Search</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2848" />
        <source>Use the filter options in the Job view
to search for specific jobs.

Available filters:
• Job Name
• Client Name
• Status (Success, Warning, Failed)
• Level (Full, Incremental, Differential)
• Date Range</source>
        <translation>Verwenden Sie die Filteroptionen in der Job-Ansicht,
um nach bestimmten Jobs zu suchen.

Verfügbare Filter:
• Job-Name
• Client-Name
• Status (Erfolgreich, Warnung, Fehlgeschlagen)
• Level (Full, Incremental, Differential)
• Zeitraum</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2858" />
        <source>Use the filter combo box in the Client view
to search for specific clients.</source>
        <translation>Verwenden Sie das Filter-Auswahlfeld in der Client-Ansicht,
um nach bestimmten Clients zu suchen.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2890" />
        <source>Reset Interface Zoom (now %1 %)</source>
        <translation>Oberflächenzoom zurücksetzen (derzeit %1 %)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2904" />
        <source>Switched to Light Theme</source>
        <translation>Zu hellem Design gewechselt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2908" />
        <source>Switched to Dark Theme</source>
        <translation>Zu dunklem Design gewechselt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2921" />
        <location filename="../src/bmainwindow.cpp" line="2933" />
        <location filename="../src/bmainwindow.cpp" line="2945" />
        <source>Not Connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2921" />
        <location filename="../src/bmainwindow.cpp" line="2933" />
        <source>Please connect to the Director first.</source>
        <translation>Bitte verbinden Sie sich zuerst mit dem Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2927" />
        <source>Jobs exported as JSON</source>
        <translation>Jobs als JSON exportiert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2939" />
        <source>Jobs exported as CSV</source>
        <translation>Jobs als CSV exportiert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2946" />
        <source>Please connect to Bareos Director first.</source>
        <translation>Bitte verbinden Sie sich zuerst mit dem Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2955" />
        <source>Loading job data...</source>
        <translation>Lade Job-Daten...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2990" />
        <source>Connection profile '%1' saved</source>
        <translation>Verbindungsprofil „%1“ gespeichert</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3075" />
        <source>Password Missing</source>
        <translation>Passwort fehlt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3076" />
        <source>The imported Console '%1' does not contain a password.

You will need to enter the password manually in the connection settings.</source>
        <translation>Die importierte Console „%1“ enthält kein Passwort.

Sie müssen das Passwort manuell in den Verbindungseinstellungen eingeben.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3097" />
        <source>Created connection profile '%1' for Director '%2'</source>
        <translation>Verbindungsprofil „%1“ für Director „%2“ angelegt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3102" />
        <source>Connect Now?</source>
        <translation>Jetzt verbinden?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3103" />
        <source>Connection profile '%1' has been created.

Do you want to connect now?</source>
        <translation>Das Verbindungsprofil „%1“ wurde angelegt.

Möchten Sie sich jetzt verbinden?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3119" />
        <source>Imported Director '%1' with Console '%2'</source>
        <translation>Director „%1“ mit Console „%2“ importiert</translation>
    </message>
</context>
<context>
    <name>BMdnsBrowser</name>
    <message>
        <location filename="../src/bmdnsbrowser.cpp" line="197" />
        <source>mDNS: cannot open UDP socket: %1</source>
        <translation>mDNS: UDP-Socket kann nicht geöffnet werden: %1</translation>
    </message>
</context>
<context>
    <name>BMessagesResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428" />
        <source>Source File</source>
        <translation>Quelldatei</translation>
    </message>
</context>
<context>
    <name>BMessagesWidget</name>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="232" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="238" />
        <source>Clear</source>
        <translation>Leeren</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="245" />
        <source>0 messages</source>
        <translation>0 Meldungen</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="280" />
        <source>Polling every %1s</source>
        <translation>Abfrage alle %1 s</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="307" />
        <source>Messages cleared</source>
        <translation>Meldungen geleert</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="322" />
        <source>Not connected</source>
        <translation>Nicht verbunden</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="363" />
        <source>Refreshing messages...</source>
        <translation>Aktualisiere Meldungen...</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/messages/bmessageswidget.cpp" line="369" />
        <source>%n message(s)</source>
        <translation>
            <numerusform>%n Meldung</numerusform>
            <numerusform>%n Meldungen</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BNewClientDirectorPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="86" />
        <source>Director Information</source>
        <translation>Director-Informationen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="87" />
        <source>The new client will connect to this Director. These settings are used to generate the File Daemon configuration.</source>
        <translation>Der neue Client verbindet sich mit diesem Director. Diese Einstellungen werden zum Erzeugen der File-Daemon-Konfiguration verwendet.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="94" />
        <source>Director Name:</source>
        <translation>Director-Name:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="98" />
        <source>Director Address:</source>
        <translation>Director-Adresse:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="102" />
        <source>Director Port:</source>
        <translation>Director-Port:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="106" />
        <source>TLS Mode:</source>
        <translation>TLS-Modus:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="114" />
        <source>The Director name above will be used in the File Daemon's Director resource. The client authenticates against this Director using a shared password.</source>
        <translation>Der obige Director-Name wird in der Director-Ressource des File Daemons verwendet. Der Client authentifiziert sich bei diesem Director mit einem gemeinsamen Passwort.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="134" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="141" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="148" />
        <source>&lt;i&gt;(not available)&lt;/i&gt;</source>
        <translation>&lt;i&gt;(nicht verfügbar)&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="135" />
        <source>Director name could not be retrieved from the connection.</source>
        <translation>Der Director-Name konnte nicht aus der Verbindung ermittelt werden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="142" />
        <source>Director address could not be retrieved.</source>
        <translation>Die Director-Adresse konnte nicht ermittelt werden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="149" />
        <source>Director port could not be retrieved.</source>
        <translation>Der Director-Port konnte nicht ermittelt werden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="154" />
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="159" />
        <source>&lt;b style='color:red;'&gt;Cannot proceed:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Make sure you are connected to a Director.</source>
        <translation>&lt;b style='color:red;'&gt;Fortfahren nicht möglich:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Stellen Sie sicher, dass Sie mit einem Director verbunden sind.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="176" />
        <source>&lt;span style='color:orange;'&gt;&lt;b&gt;Warning:&lt;/b&gt; The current console may not have permission to run 'configure add client'. Check the Command ACL in the Console resource on the Director.&lt;/span&gt;&lt;br&gt;You can still generate and export the FD configuration files.</source>
        <translation>&lt;span style='color:orange;'&gt;&lt;b&gt;Warnung:&lt;/b&gt; Die aktuelle Console hat möglicherweise keine Berechtigung, 'configure add client' auszuführen. Prüfen Sie die Command ACL in der Console-Ressource auf dem Director.&lt;/span&gt;&lt;br&gt;Sie können die FD-Konfigurationsdateien trotzdem erzeugen und exportieren.</translation>
    </message>
</context>
<context>
    <name>BNewClientPreviewPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="473" />
        <source>Preview &amp; Export</source>
        <translation>Vorschau &amp; Export</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="474" />
        <source>Review generated configuration files, validate, and export.</source>
        <translation>Erzeugte Konfigurationsdateien prüfen, validieren und exportieren.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="485" />
        <source>FD: Director</source>
        <translation>FD: Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="488" />
        <source>FD: Client</source>
        <translation>FD: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="491" />
        <source>FD: Messages</source>
        <translation>FD: Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="494" />
        <source>Dir: Client</source>
        <translation>Dir: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="513" />
        <source>Director Command</source>
        <translation>Director-Befehl</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="525" />
        <source>Copy Current Tab</source>
        <translation>Aktuellen Tab kopieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="529" />
        <source>Copy All</source>
        <translation>Alles kopieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="533" />
        <source>Export ZIP...</source>
        <translation>ZIP exportieren...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="766" />
        <source>FD Director</source>
        <translation>FD Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="767" />
        <source>FD Client</source>
        <translation>FD Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="768" />
        <source>FD Messages</source>
        <translation>FD Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="769" />
        <source>Dir Client</source>
        <translation>Dir Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="775" />
        <source>%1: Config syntax could not be parsed.</source>
        <translation>%1: Die Konfigurationssyntax konnte nicht ausgewertet werden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="786" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="795" />
        <source>Client name '%1' does not end with '-fd'. Convention: use '&lt;hostname&gt;-fd'.</source>
        <translation>Der Client-Name „%1“ endet nicht auf „-fd“. Konvention: '&lt;hostname&gt;-fd' verwenden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="800" />
        <source>Password MD5 hash has invalid length (%1, expected 32).</source>
        <translation>Der MD5-Hash des Passworts hat eine ungültige Länge (%1, erwartet 32).</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="806" />
        <source>Validation passed.</source>
        <translation>Validierung erfolgreich.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="899" />
        <source>Copied '%1' to clipboard.</source>
        <translation>„%1“ in die Zwischenablage kopiert.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="924" />
        <source>All configuration files copied to clipboard.</source>
        <translation>Alle Konfigurationsdateien in die Zwischenablage kopiert.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="936" />
        <source>Export Client Configuration</source>
        <translation>Client-Konfiguration exportieren</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="938" />
        <source>ZIP Archives (*.zip)</source>
        <translation>ZIP-Archive (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="944" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="1017" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="1023" />
        <source>Export Error</source>
        <translation>Exportfehler</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="945" />
        <source>Failed to create temporary directory.</source>
        <translation>Temporäres Verzeichnis konnte nicht erstellt werden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1018" />
        <source>ZIP creation timed out. Is 'zip' installed?</source>
        <translation>Zeitüberschreitung beim Erstellen des ZIP-Archivs. Ist 'zip' installiert?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1024" />
        <source>ZIP creation failed:
%1</source>
        <translation>ZIP-Erstellung fehlgeschlagen:
%1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1029" />
        <source>Exported to: %1</source>
        <translation>Exportiert nach: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1038" />
        <source>ERROR: No director connection available.</source>
        <translation>FEHLER: Keine Director-Verbindung verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1047" />
        <source>Sending configure command...</source>
        <translation>Sende configure-Befehl...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1088" />
        <source>Failed to add client to Director: %1
Go back to change the settings, or click Finish to send the command again.
You can still export the FD config files manually.</source>
        <translation>Client konnte nicht zum Director hinzugefügt werden: %1
Gehen Sie zurück, um die Einstellungen zu ändern, oder klicken Sie auf „Fertigstellen“, um den Befehl erneut zu senden.
Sie können die FD-Konfigurationsdateien weiterhin manuell exportieren.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1101" />
        <source>Client '%1' added to Director.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client „%1“ zum Director hinzugefügt.
Stellen Sie die FD-Konfigurationsdateien auf dem Client-Rechner bereit und starten Sie bareos-fd neu.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1111" />
        <source>Client '%1' added to Director. Reloading Director...</source>
        <translation>Client „%1“ zum Director hinzugefügt. Director wird neu geladen...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1138" />
        <source>Client '%1' added and Director reloaded successfully.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client „%1“ hinzugefügt und Director erfolgreich neu geladen.
Stellen Sie die FD-Konfigurationsdateien auf dem Client-Rechner bereit und starten Sie bareos-fd neu.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1145" />
        <source>Client '%1' added to Director, but reload failed.
Run 'reload' manually on the Director or restart bareos-dir.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client „%1“ zum Director hinzugefügt, aber das Neuladen ist fehlgeschlagen.
Führen Sie 'reload' manuell auf dem Director aus oder starten Sie bareos-dir neu.
Stellen Sie die FD-Konfigurationsdateien auf dem Client-Rechner bereit und starten Sie bareos-fd neu.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1164" />
        <source>Client '%1' added to Director, but the connection was lost before the reload was confirmed.
Reconnect and run 'reload' on the Director if the client does not appear.</source>
        <translation>Client „%1“ zum Director hinzugefügt, aber die Verbindung ging verloren, bevor das Neuladen bestätigt wurde.
Verbinden Sie sich neu und führen Sie 'reload' auf dem Director aus, falls der Client nicht erscheint.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1173" />
        <source>The connection to the Director was lost before it answered.
Reconnect and check whether client '%1' exists before sending the command again.</source>
        <translation>Die Verbindung zum Director ging verloren, bevor er antwortete.
Verbinden Sie sich neu und prüfen Sie, ob Client „%1“ existiert, bevor Sie den Befehl erneut senden.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1191" />
        <source>Client '%1' added to Director, but the Director did not confirm the reload within 30 seconds.
Run 'reload' manually on the Director if the client does not appear.</source>
        <translation>Client „%1“ zum Director hinzugefügt, aber der Director hat das Neuladen nicht innerhalb von 30 Sekunden bestätigt.
Führen Sie 'reload' manuell auf dem Director aus, falls der Client nicht erscheint.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1201" />
        <source>Command Timeout</source>
        <translation>Zeitüberschreitung beim Befehl</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1202" />
        <source>The 'configure add client' command did not respond within 30 seconds.

The Director may be busy or the connection may have been lost.

Retry the command?</source>
        <translation>Der Befehl 'configure add client' hat nicht innerhalb von 30 Sekunden geantwortet.

Der Director ist möglicherweise ausgelastet, oder die Verbindung wurde unterbrochen.

Befehl erneut versuchen?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1214" />
        <source>Configure command timed out. The client may or may not have been created.
You can export the FD config files and configure the Director manually.</source>
        <translation>Zeitüberschreitung beim configure-Befehl. Der Client wurde möglicherweise angelegt, möglicherweise auch nicht.
Sie können die FD-Konfigurationsdateien exportieren und den Director manuell konfigurieren.</translation>
    </message>
</context>
<context>
    <name>BNewClientSettingsPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="200" />
        <source>Client Settings</source>
        <translation>Client-Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="201" />
        <source>Configure the new client's identity, password, TLS, and network settings.</source>
        <translation>Identität, Passwort, TLS und Netzwerkeinstellungen des neuen Clients konfigurieren.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="206" />
        <source>Basic Settings</source>
        <translation>Grundeinstellungen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="210" />
        <source>e.g. server1-fd</source>
        <translation>z. B. server1-fd</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="211" />
        <source>Client Name:</source>
        <translation>Client-Name:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="214" />
        <source>e.g. 192.168.1.100 or server1.example.com</source>
        <translation>z. B. 192.168.1.100 oder server1.example.com</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="215" />
        <source>Address:</source>
        <translation>Adresse:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="220" />
        <source>FD Port:</source>
        <translation>FD-Port:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="225" />
        <source>Client password</source>
        <translation>Client-Passwort</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="227" />
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="229" />
        <source>Password:</source>
        <translation>Passwort:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="235" />
        <source>MD5 Hash:</source>
        <translation>MD5-Hash:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="245" />
        <source>Passive mode (client behind NAT/firewall)</source>
        <translation>Passiver Modus (Client hinter NAT/Firewall)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="247" />
        <source>Enable if the client is behind a firewall/NAT.
The Director and Storage Daemon will initiate connections to the client instead.

Firewall ports to open on the client side:
  - Inbound to File Daemon: TCP 9102</source>
        <translation>Aktivieren, wenn sich der Client hinter einer Firewall/NAT befindet.
Director und Storage Daemon bauen dann die Verbindungen zum Client auf.

Auf der Client-Seite zu öffnende Firewall-Ports:
  - Eingehend zum File Daemon: TCP 9102</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="256" />
        <source>Connection wait time:</source>
        <translation>Wartezeit für Verbindung:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="260" />
        <source> sec</source>
        <translation> s</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="261" />
        <source>Time in seconds to wait for client-initiated connection</source>
        <translation>Wartezeit in Sekunden für eine vom Client aufgebaute Verbindung</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="270" />
        <source>Execute "configure add client" on Director</source>
        <translation>"configure add client" auf dem Director ausführen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="273" />
        <source>If checked, the wizard will send the configure command to the Director
on the Preview page to add this client automatically.</source>
        <translation>Wenn aktiviert, sendet der Assistent auf der Vorschauseite den configure-Befehl
an den Director, um diesen Client automatisch hinzuzufügen.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="409" />
        <source>(enter password)</source>
        <translation>(Passwort eingeben)</translation>
    </message>
</context>
<context>
    <name>BNewClientWizard</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="51" />
        <source>Add New Client</source>
        <translation>Neuen Client hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="65" />
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="67" />
        <source>TLS Certificate</source>
        <translation>TLS-Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="70" />
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
</context>
<context>
    <name>BNewJobDialog</name>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="24" />
        <source>Run New Job</source>
        <translation>Neuen Job ausführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="50" />
        <source>Run Backup Job</source>
        <translation>Backup-Job ausführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="60" />
        <source>Command Preview</source>
        <translation>Befehlsvorschau</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="79" />
        <source>Estimate</source>
        <translation>Schätzen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="81" />
        <source>Calculate estimated files and size</source>
        <translation>Voraussichtliche Dateien und Größe berechnen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="91" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="665" />
        <source>Run Job</source>
        <translation>Job ausführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="113" />
        <source>Job Configuration</source>
        <translation>Job-Konfiguration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="118" />
        <source>Select job...</source>
        <translation>Job wählen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="121" />
        <source>Job:</source>
        <translation>Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="124" />
        <source>Select client...</source>
        <translation>Client wählen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="127" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="133" />
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="138" />
        <source>Resources</source>
        <translation>Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="143" />
        <source>Select FileSet...</source>
        <translation>FileSet wählen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="146" />
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="149" />
        <source>Select Pool...</source>
        <translation>Pool wählen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="152" />
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="155" />
        <source>Select Storage...</source>
        <translation>Storage wählen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="158" />
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="163" />
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="169" />
        <source>Lower values = higher priority. Only sent if changed here; otherwise the job's own priority applies.</source>
        <translation>Niedrigere Werte = höhere Priorität. Wird nur gesendet, wenn hier geändert; sonst gilt die eigene Priorität des Jobs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="176" />
        <source>Priority:</source>
        <translation>Priorität:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="182" />
        <source>Basic</source>
        <translation>Grundlegend</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="192" />
        <source>Scheduling</source>
        <translation>Zeitplanung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="196" />
        <source>e.g. "2024-01-15 14:30:00" or empty for immediate</source>
        <translation>z. B. "2024-01-15 14:30:00" oder leer für sofort</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="197" />
        <source>Start time in the form YYYY-MM-DD HH:MM:SS</source>
        <translation>Startzeit im Format YYYY-MM-DD HH:MM:SS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="199" />
        <source>When:</source>
        <translation>Wann:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="204" />
        <source>Bootstrap Options</source>
        <translation>Bootstrap-Optionen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="207" />
        <source>Use Bootstrap file</source>
        <translation>Bootstrap-Datei verwenden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="212" />
        <source>Path to Bootstrap file</source>
        <translation>Pfad zur Bootstrap-Datei</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="222" />
        <source>Replace</source>
        <translation>Ersetzen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="225" />
        <source>Restore mode: Replace existing files</source>
        <translation>Restore-Modus: Vorhandene Dateien ersetzen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="229" />
        <source>Always</source>
        <translation>Immer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="230" />
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="231" />
        <source>If Newer</source>
        <translation>Wenn neuer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="232" />
        <source>If Older</source>
        <translation>Wenn älter</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="242" />
        <source>Advanced</source>
        <translation>Erweitert</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="248" />
        <source>⚠ No JobWidget connection available</source>
        <translation>⚠ Keine JobWidget-Verbindung verfügbar</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="253" />
        <source>Loading configuration data from JobWidget...</source>
        <translation>Lade Konfigurationsdaten aus JobWidget...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="291" />
        <source>✓ %1 jobs, %2 clients loaded</source>
        <translation>✓ %1 Jobs, %2 Clients geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="297" />
        <source>⚠ No jobs available - check connection</source>
        <translation>⚠ Keine Jobs verfügbar – Verbindung prüfen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="383" />
        <source>✓ Configuration loaded</source>
        <translation>✓ Konfiguration geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="422" />
        <source>Loading job defaults...</source>
        <translation>Lade Job-Vorgaben...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="450" />
        <source>Failed to load job defaults</source>
        <translation>Job-Vorgaben konnten nicht geladen werden</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="561" />
        <source>✓ Job defaults loaded</source>
        <translation>✓ Job-Vorgaben geladen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="592" />
        <source>No job selected.</source>
        <translation>Kein Job ausgewählt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="594" />
        <source>The value for '%1' contains a line break or control character.</source>
        <translation>Der Wert für „%1“ enthält einen Zeilenumbruch oder ein Steuerzeichen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="596" />
        <source>'When' must be a valid time in the form YYYY-MM-DD HH:MM:SS.</source>
        <translation>„Wann“ muss eine gültige Zeit im Format YYYY-MM-DD HH:MM:SS sein.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="598" />
        <source>The priority must be a positive number.</source>
        <translation>Die Priorität muss eine positive Zahl sein.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="600" />
        <source>The command could not be built.</source>
        <translation>Der Befehl konnte nicht erstellt werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="618" />
        <source>Nothing will be sent: %1</source>
        <translation>Es wird nichts gesendet: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="645" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="686" />
        <source>Missing Input</source>
        <translation>Fehlende Eingabe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="646" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="687" />
        <source>Please select a job.</source>
        <translation>Bitte wählen Sie einen Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="654" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="694" />
        <source>Invalid Input</source>
        <translation>Ungültige Eingabe</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="666" />
        <source>Do you want to run the following job?

%1</source>
        <translation>Möchten Sie den folgenden Job ausführen?

%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="705" />
        <source>Calculating estimate...</source>
        <translation>Berechne Schätzung...</translation>
    </message>
</context>
<context>
    <name>BPFXConverter</name>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="27" />
        <source>Convert to PFX Format</source>
        <translation>In PFX-Format konvertieren</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="35" />
        <source>Convert separate PEM/DER certificate and key files into a single PFX file for Windows.
PFX files are password-protected and contain both certificate and private key.</source>
        <translation>Separate PEM/DER-Zertifikats- und Schlüsseldateien in eine einzelne PFX-Datei für Windows konvertieren.
PFX-Dateien sind passwortgeschützt und enthalten sowohl Zertifikat als auch privaten Schlüssel.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="44" />
        <source>Input Files</source>
        <translation>Eingabedateien</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="50" />
        <source>Select certificate file...</source>
        <translation>Zertifikatsdatei wählen...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="51" />
        <location filename="../src/bpfxconverter.cpp" line="61" />
        <location filename="../src/bpfxconverter.cpp" line="71" />
        <location filename="../src/bpfxconverter.cpp" line="102" />
        <source>Browse...</source>
        <translation>Durchsuchen...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="55" />
        <source>Certificate:</source>
        <translation>Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="60" />
        <source>Select private key file...</source>
        <translation>Datei mit privatem Schlüssel wählen...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="65" />
        <source>Private Key:</source>
        <translation>Privater Schlüssel:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="70" />
        <source>Optional: CA certificate</source>
        <translation>Optional: CA-Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="75" />
        <source>CA Cert (Optional):</source>
        <translation>CA-Zertifikat (optional):</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="80" />
        <source>PFX Password Protection</source>
        <translation>PFX-Passwortschutz</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="85" />
        <source>Enter password for PFX file...</source>
        <translation>Passwort für die PFX-Datei eingeben...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="86" />
        <source>Password:</source>
        <translation>Passwort:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="90" />
        <source>Confirm password...</source>
        <translation>Passwort bestätigen...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="91" />
        <source>Confirm:</source>
        <translation>Bestätigen:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="96" />
        <source>Output</source>
        <translation>Ausgabe</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="101" />
        <source>Select where to save PFX file...</source>
        <translation>Speicherort der PFX-Datei wählen...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="105" />
        <source>Save PFX File</source>
        <translation>PFX-Datei speichern</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="107" />
        <source>PFX Files (*.pfx);;All Files (*)</source>
        <translation>PFX-Dateien (*.pfx);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="117" />
        <source>Save As:</source>
        <translation>Speichern unter:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="122" />
        <source>Conversion Log:</source>
        <translation>Konvertierungsprotokoll:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="133" />
        <source>Convert to PFX</source>
        <translation>In PFX konvertieren</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="137" />
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="148" />
        <source>Select Certificate File</source>
        <translation>Zertifikatsdatei wählen</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="150" />
        <source>Certificates (*.pem *.crt *.cert *.der);;All Files (*)</source>
        <translation>Zertifikate (*.pem *.crt *.cert *.der);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="153" />
        <source>✓ Certificate selected: %1</source>
        <translation>✓ Zertifikat ausgewählt: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="160" />
        <source>Select Private Key File</source>
        <translation>Datei mit privatem Schlüssel wählen</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="162" />
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Private Schlüssel (*.pem *.key);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="165" />
        <source>✓ Private key selected: %1</source>
        <translation>✓ Privater Schlüssel ausgewählt: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="172" />
        <source>Select CA Certificate</source>
        <translation>CA-Zertifikat wählen</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="174" />
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="177" />
        <source>✓ CA certificate selected: %1</source>
        <translation>✓ CA-Zertifikat ausgewählt: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="185" />
        <location filename="../src/bpfxconverter.cpp" line="192" />
        <location filename="../src/bpfxconverter.cpp" line="199" />
        <source>Missing Input</source>
        <translation>Fehlende Eingabe</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="186" />
        <source>Please select a certificate file.</source>
        <translation>Bitte wählen Sie eine Zertifikatsdatei.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="193" />
        <source>Please select a private key file.</source>
        <translation>Bitte wählen Sie eine Datei mit privatem Schlüssel.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="200" />
        <source>Please enter a password to protect the PFX file.</source>
        <translation>Bitte geben Sie ein Passwort zum Schutz der PFX-Datei ein.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="206" />
        <source>Password Mismatch</source>
        <translation>Passwörter stimmen nicht überein</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="207" />
        <source>The passwords do not match. Please try again.</source>
        <translation>Die Passwörter stimmen nicht überein. Bitte versuchen Sie es erneut.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="213" />
        <source>Missing Output</source>
        <translation>Fehlende Ausgabe</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="214" />
        <source>Please specify where to save the PFX file.</source>
        <translation>Bitte geben Sie an, wo die PFX-Datei gespeichert werden soll.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="224" />
        <location filename="../src/bpfxconverter.cpp" line="226" />
        <location filename="../src/bpfxconverter.cpp" line="233" />
        <location filename="../src/bpfxconverter.cpp" line="236" />
        <source>========================================</source>
        <translation>========================================</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="225" />
        <source>Starting PFX Conversion</source>
        <translation>Starte PFX-Konvertierung</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="234" />
        <source>✓ Conversion successful!</source>
        <translation>✓ Konvertierung erfolgreich!</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="235" />
        <source>PFX file created: %1</source>
        <translation>PFX-Datei erstellt: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="240" />
        <source>Success</source>
        <translation>Erfolg</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="241" />
        <source>PFX file created successfully!

%1

You can now use this file in the certificate settings.</source>
        <translation>PFX-Datei erfolgreich erstellt!

%1

Sie können diese Datei jetzt in den Zertifikatseinstellungen verwenden.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="251" />
        <source>Loading certificate from: %1</source>
        <translation>Lade Zertifikat aus: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="256" />
        <source>✗ ERROR: Cannot open certificate file</source>
        <translation>✗ FEHLER: Zertifikatsdatei kann nicht geöffnet werden</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257" />
        <location filename="../src/bpfxconverter.cpp" line="271" />
        <location filename="../src/bpfxconverter.cpp" line="284" />
        <location filename="../src/bpfxconverter.cpp" line="301" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257" />
        <source>Cannot open certificate file.</source>
        <translation>Zertifikatsdatei kann nicht geöffnet werden.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="270" />
        <source>✗ ERROR: Cannot parse certificate (not valid PEM or DER)</source>
        <translation>✗ FEHLER: Zertifikat kann nicht gelesen werden (kein gültiges PEM oder DER)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="272" />
        <source>Cannot parse certificate file. Make sure it's a valid PEM or DER certificate.</source>
        <translation>Zertifikatsdatei kann nicht gelesen werden. Stellen Sie sicher, dass es sich um ein gültiges PEM- oder DER-Zertifikat handelt.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="276" />
        <source>✓ Certificate loaded: %1</source>
        <translation>✓ Zertifikat geladen: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="279" />
        <source>Loading private key from: %1</source>
        <translation>Lade privaten Schlüssel aus: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="283" />
        <source>✗ ERROR: Cannot open private key file</source>
        <translation>✗ FEHLER: Datei mit privatem Schlüssel kann nicht geöffnet werden</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="284" />
        <source>Cannot open private key file.</source>
        <translation>Datei mit privatem Schlüssel kann nicht geöffnet werden.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="300" />
        <source>✗ ERROR: Cannot parse private key</source>
        <translation>✗ FEHLER: Privater Schlüssel kann nicht gelesen werden</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="302" />
        <source>Cannot parse private key file. Make sure it's a valid PEM or DER key.</source>
        <translation>Datei mit privatem Schlüssel kann nicht gelesen werden. Stellen Sie sicher, dass es sich um einen gültigen PEM- oder DER-Schlüssel handelt.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="306" />
        <source>✓ Private key loaded</source>
        <translation>✓ Privater Schlüssel geladen</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="309" />
        <source>Creating PFX (PKCS#12) file...</source>
        <translation>Erstelle PFX-Datei (PKCS#12)...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="313" />
        <source>⚠ Note: Full PFX conversion requires OpenSSL PKCS#12 API</source>
        <translation>⚠ Hinweis: Die vollständige PFX-Konvertierung erfordert die OpenSSL-PKCS#12-API</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="314" />
        <source>  The certificate and key files are valid and can be loaded</source>
        <translation>  Die Zertifikats- und Schlüsseldateien sind gültig und können geladen werden</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="321" />
        <source>✓ Files validated successfully</source>
        <translation>✓ Dateien erfolgreich validiert</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="322" />
        <source>  Certificate: %1</source>
        <translation>  Zertifikat: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="323" />
        <source>  Private Key: %1</source>
        <translation>  Privater Schlüssel: %1</translation>
    </message>
</context>
<context>
    <name>BPaginationWidget</name>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="26" />
        <source>Enable Pagination</source>
        <translation>Seitenweise Anzeige aktivieren</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="32" />
        <source>Items per page:</source>
        <translation>Einträge pro Seite:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="46" />
        <source>First Page</source>
        <translation>Erste Seite</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="52" />
        <source>Previous Page</source>
        <translation>Vorherige Seite</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="58" />
        <location filename="../src/jobs/bpaginationwidget.cpp" line="311" />
        <location filename="../src/jobs/bpaginationwidget.cpp" line="334" />
        <source>Page 0 of 0</source>
        <translation>Seite 0 von 0</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="64" />
        <source>Next Page</source>
        <translation>Nächste Seite</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="70" />
        <source>Last Page</source>
        <translation>Letzte Seite</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="78" />
        <source>Go to page:</source>
        <translation>Gehe zu Seite:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="85" />
        <source>Go</source>
        <translation>Los</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="345" />
        <source>Page %1 of %2</source>
        <translation>Seite %1 von %2</translation>
    </message>
</context>
<context>
    <name>BPermissionsDialog</name>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1985" />
        <source>My Permissions</source>
        <translation>Meine Berechtigungen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1998" />
        <source>Connection</source>
        <translation>Verbindung</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2000" />
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2001" />
        <source>Host:</source>
        <translation>Host:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2008" />
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2010" />
        <source>TLS Certificate</source>
        <translation>TLS-Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2012" />
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2014" />
        <source>Encryption:</source>
        <translation>Verschlüsselung:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2020" />
        <source>Backup &amp; Restore</source>
        <translation>Backup &amp; Restore</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2021" />
        <source>Job Control</source>
        <translation>Job-Steuerung</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2022" />
        <source>Administration</source>
        <translation>Administration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2024" />
        <source>Media</source>
        <translation>Medien</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2035" />
        <source>Command Permissions</source>
        <translation>Befehlsberechtigungen</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2042" />
        <source>Checking...</source>
        <translation>Prüfe...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2051" />
        <source>Restore Details</source>
        <translation>Restore-Details</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2053" />
        <location filename="../src/bmainwindow.cpp" line="2062" />
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2060" />
        <source>Other Available Commands</source>
        <translation>Weitere verfügbare Befehle</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2069" />
        <source>Raw Director Response</source>
        <translation>Rohantwort des Directors</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2087" />
        <source>Querying Director...</source>
        <translation>Frage Director ab...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2176" />
        <source>N/A</source>
        <translation>k. A.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2179" />
        <source>Authorized</source>
        <translation>Berechtigt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2182" />
        <source>Not authorized</source>
        <translation>Nicht berechtigt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2207" />
        <source>&lt;b&gt;Permission:&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;Berechtigung:&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2211" />
        <source>&lt;b&gt;where= (restore path):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;where= (Restore-Pfad):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2215" />
        <source>&lt;b&gt;client= (target client):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;client= (Ziel-Client):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2219" />
        <source>&lt;b&gt;replace= (replace policy):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;replace= (Ersetzungsrichtlinie):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2225" />
        <source>&lt;br&gt;&lt;b&gt;Available parameters:&lt;/b&gt; %1</source>
        <translation>&lt;br&gt;&lt;b&gt;Verfügbare Parameter:&lt;/b&gt; %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2246" />
        <source>Total: %1 commands (%2 authorized, %3 denied)</source>
        <translation>Gesamt: %1 Befehle (%2 berechtigt, %3 verweigert)</translation>
    </message>
</context>
<context>
    <name>BPoolResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374" />
        <source>Pool Type</source>
        <translation>Pool Type</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374" />
        <source>Recycle</source>
        <translation>Recycle</translation>
    </message>
</context>
<context>
    <name>BProfileSettingsDialog</name>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="23" />
        <source>Advanced Profile Settings - %1</source>
        <translation>Erweiterte Profileinstellungen – %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="40" />
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="41" />
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="42" />
        <source>ACL</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="65" />
        <source>Optional description for this connection</source>
        <translation>Optionale Beschreibung für diese Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="66" />
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="71" />
        <source> seconds</source>
        <translation> Sekunden</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="72" />
        <source>Disabled</source>
        <translation>Deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="73" />
        <source>Keepalive interval (0 = disabled)</source>
        <translation>Keepalive-Intervall (0 = deaktiviert)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="74" />
        <source>Heartbeat Interval:</source>
        <translation>Heartbeat Interval:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="78" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (Bareos-Konsolenprotokoll)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="79" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="85" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Dieser Build von Onesimus hat keine gRPC-Unterstützung.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="87" />
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="178" />
        <source>Protocol:</source>
        <translation>Protokoll:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="92" />
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>gRPC-Port des Directors (Standard: BNET-Port + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="93" />
        <source>gRPC port:</source>
        <translation>gRPC-Port:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="102" />
        <source>Forget certificate</source>
        <translation>Zertifikat vergessen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="103" />
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Das gespeicherte Zertifikat entfernen. Bei der nächsten Verbindung wird erneut gefragt, ob dem Serverzertifikat vertraut wird.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="106" />
        <source>Server certificate:</source>
        <translation>Serverzertifikat:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="118" />
        <source>e.g., operator or readonly</source>
        <translation>z. B. operator oder readonly</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="119" />
        <source>Reference to a Profile resource on the Director.
The Profile defines ACLs for this console.</source>
        <translation>Verweis auf eine Profile-Ressource auf dem Director.
Das Profile legt die ACLs für diese Console fest.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="122" />
        <source>Director Profile:</source>
        <translation>Director-Profile:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="126" />
        <source>&lt;b&gt;Note:&lt;/b&gt; The Director Profile is a server-side resource that defines permissions. If set, the ACLs in this profile are ignored in favor of the server-side Profile.</source>
        <translation>&lt;b&gt;Hinweis:&lt;/b&gt; Das Director-Profile ist eine serverseitige Ressource, die Berechtigungen festlegt. Wenn es gesetzt ist, werden die ACLs in diesem Profil zugunsten des serverseitigen Profile ignoriert.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="146" />
        <source>TLS Options</source>
        <translation>TLS-Optionen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="149" />
        <source>TLS Required</source>
        <translation>TLS Required</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="150" />
        <source>Require TLS for all connections (recommended)</source>
        <translation>TLS für alle Verbindungen verlangen (empfohlen)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="153" />
        <source>TLS Authenticate Only</source>
        <translation>TLS Authenticate Only</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="154" />
        <source>Use TLS only for authentication, not for data encryption</source>
        <translation>TLS nur zur Authentifizierung verwenden, nicht zur Datenverschlüsselung</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="160" />
        <source>Cipher Configuration</source>
        <translation>Cipher-Konfiguration</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="164" />
        <source>TLSv1.2 ciphers (colon-separated)</source>
        <translation>TLSv1.2-Ciphers (durch Doppelpunkte getrennt)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="165" />
        <source>OpenSSL cipher list for TLSv1.2 connections
Example: HIGH:!aNULL:!MD5</source>
        <translation>OpenSSL-Cipher-Liste für TLSv1.2-Verbindungen
Beispiel: HIGH:!aNULL:!MD5</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="167" />
        <source>Cipher List (TLSv1.2):</source>
        <translation>Cipher List (TLSv1.2):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="170" />
        <source>TLSv1.3 cipher suites (colon-separated)</source>
        <translation>TLSv1.3-Cipher-Suites (durch Doppelpunkte getrennt)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="171" />
        <source>OpenSSL cipher suites for TLSv1.3 connections
Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</source>
        <translation>OpenSSL-Cipher-Suites für TLSv1.3-Verbindungen
Beispiel: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="173" />
        <source>Cipher Suites (TLSv1.3):</source>
        <translation>Cipher Suites (TLSv1.3):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="176" />
        <source>e.g., -ALL,TLSv1.2,TLSv1.3</source>
        <translation>z. B. -ALL,TLSv1.2,TLSv1.3</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="177" />
        <source>OpenSSL protocol string</source>
        <translation>OpenSSL-Protokollzeichenkette</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="183" />
        <source>Certificate Files</source>
        <translation>Zertifikatsdateien</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="187" />
        <source>DH Parameters:</source>
        <translation>DH-Parameter:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="189" />
        <source>Path to Diffie-Hellman parameters file</source>
        <translation>Pfad zur Diffie-Hellman-Parameterdatei</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="191" />
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="207" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="194" />
        <source>Select DH Parameters File</source>
        <translation>DH-Parameterdatei wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="195" />
        <source>PEM Files (*.pem);;All Files (*)</source>
        <translation>PEM-Dateien (*.pem);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="203" />
        <source>CRL File:</source>
        <translation>CRL-Datei:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="205" />
        <source>Path to Certificate Revocation List file</source>
        <translation>Pfad zur Zertifikatssperrlisten-Datei (CRL)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="210" />
        <source>Select CRL File</source>
        <translation>CRL-Datei wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="211" />
        <source>CRL Files (*.crl *.pem);;All Files (*)</source>
        <translation>CRL-Dateien (*.crl *.pem);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="221" />
        <source>Allowed Certificate Common Names</source>
        <translation>Erlaubte Common Names von Zertifikaten</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="225" />
        <source>One Common Name per line</source>
        <translation>Ein Common Name pro Zeile</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="226" />
        <source>List of allowed certificate Common Names.
Leave empty to allow all valid certificates.</source>
        <translation>Liste der erlaubten Common Names von Zertifikaten.
Leer lassen, um alle gültigen Zertifikate zu erlauben.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="249" />
        <source>&lt;b&gt;Access Control Lists (ACLs)&lt;/b&gt;&lt;br&gt;Define which resources this console can access. One entry per line. Use &lt;code&gt;*all*&lt;/code&gt; to allow all resources of that type. Regex patterns are supported for Job ACL.</source>
        <translation>&lt;b&gt;Zugriffskontrolllisten (ACLs)&lt;/b&gt;&lt;br&gt;Legen Sie fest, auf welche Ressourcen diese Console zugreifen darf. Ein Eintrag pro Zeile. Verwenden Sie &lt;code&gt;*all*&lt;/code&gt;, um alle Ressourcen dieses Typs zu erlauben. Für die Job ACL werden reguläre Ausdrücke unterstützt.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="271" />
        <source>Job ACL:</source>
        <translation>Job ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272" />
        <source>Allowed jobs</source>
        <translation>Erlaubte Jobs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272" />
        <source>List of allowed job names (regex supported)</source>
        <translation>Liste der erlaubten Job-Namen (reguläre Ausdrücke möglich)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="275" />
        <source>Client ACL:</source>
        <translation>Client ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276" />
        <source>Allowed clients</source>
        <translation>Erlaubte Clients</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276" />
        <source>List of allowed client names</source>
        <translation>Liste der erlaubten Client-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280" />
        <source>Storage ACL:</source>
        <translation>Storage ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281" />
        <source>Allowed storages</source>
        <translation>Erlaubte Storages</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281" />
        <source>List of allowed storage names</source>
        <translation>Liste der erlaubten Storage-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="284" />
        <source>Pool ACL:</source>
        <translation>Pool ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285" />
        <source>Allowed pools</source>
        <translation>Erlaubte Pools</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285" />
        <source>List of allowed pool names</source>
        <translation>Liste der erlaubten Pool-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289" />
        <source>Schedule ACL:</source>
        <translation>Schedule ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290" />
        <source>Allowed schedules</source>
        <translation>Erlaubte Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290" />
        <source>List of allowed schedule names</source>
        <translation>Liste der erlaubten Schedule-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="293" />
        <source>FileSet ACL:</source>
        <translation>FileSet ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294" />
        <source>Allowed filesets</source>
        <translation>Erlaubte FileSets</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294" />
        <source>List of allowed fileset names</source>
        <translation>Liste der erlaubten FileSet-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298" />
        <source>Catalog ACL:</source>
        <translation>Catalog ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299" />
        <source>Allowed catalogs</source>
        <translation>Erlaubte Catalogs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299" />
        <source>List of allowed catalog names</source>
        <translation>Liste der erlaubten Catalog-Namen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="302" />
        <source>Command ACL:</source>
        <translation>Command ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303" />
        <source>Allowed commands</source>
        <translation>Erlaubte Befehle</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303" />
        <source>List of allowed commands</source>
        <translation>Liste der erlaubten Befehle</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307" />
        <source>Where ACL:</source>
        <translation>Where ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308" />
        <source>Allowed restore locations</source>
        <translation>Erlaubte Restore-Ziele</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308" />
        <source>List of allowed restore locations</source>
        <translation>Liste der erlaubten Restore-Ziele</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="311" />
        <source>Plugin Options ACL:</source>
        <translation>Plugin Options ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312" />
        <source>Allowed plugin options</source>
        <translation>Erlaubte Plugin-Optionen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312" />
        <source>List of allowed plugin options</source>
        <translation>Liste der erlaubten Plugin-Optionen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="412" />
        <source>Not used with gRPC: the connection is always encrypted with TLS, the server certificate is confirmed on first use.</source>
        <translation>Bei gRPC nicht verwendet: Die Verbindung ist immer mit TLS verschlüsselt, das Serverzertifikat wird bei der ersten Verwendung bestätigt.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="427" />
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(nicht gespeichert – wird bei der nächsten Verbindung bestätigt)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="433" />
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="445" />
        <source>Reset to Defaults</source>
        <translation>Auf Standardwerte zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="446" />
        <source>Are you sure you want to reset all advanced settings to their default values?</source>
        <translation>Möchten Sie wirklich alle erweiterten Einstellungen auf ihre Standardwerte zurücksetzen?</translation>
    </message>
</context>
<context>
    <name>BRecycleForecast</name>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="62" />
        <source>now</source>
        <translation>jetzt</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="63" />
        <source>never</source>
        <translation>nie</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="66" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="74" />
        <source>The Director did not report the status of this volume.</source>
        <translation>Der Director hat den Status dieses Volumes nicht gemeldet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="79" />
        <source>Archived: Bareos never prunes or recycles an archived volume.</source>
        <translation>Archived: Bareos prunt oder recycelt ein archiviertes Volume nie.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="82" />
        <source>Disabled: Bareos neither writes to nor recycles a disabled volume.</source>
        <translation>Disabled: Bareos schreibt weder auf ein deaktiviertes Volume noch recycelt es es.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="86" />
        <source>Error: Bareos never prunes a volume in Error, so it keeps its space until its status is changed. The job records on it still expire.</source>
        <translation>Error: Bareos prunt ein Volume im Status Error nie, daher behält es seinen Platz, bis sein Status geändert wird. Die Job-Einträge darauf laufen trotzdem ab.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="90" />
        <source>Read-Only: Bareos does not prune or recycle a read-only volume.</source>
        <translation>Read-Only: Bareos prunt oder recycelt ein schreibgeschütztes Volume nicht.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="93" />
        <source>Cleaning tape: never used for data.</source>
        <translation>Reinigungsband: wird nie für Daten verwendet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="98" />
        <source>Recycle = no: the volume is never reused; its data stays until it is relabeled or deleted.</source>
        <translation>Recycle = no: Das Volume wird nie wiederverwendet; seine Daten bleiben erhalten, bis es neu gelabelt oder gelöscht wird.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="102" />
        <source>Purged: no job needs it any more, reusable now.</source>
        <translation>Purged: Kein Job benötigt es mehr, jetzt wiederverwendbar.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="106" />
        <source>It still holds %1 on disk until it is recycled or truncated.</source>
        <translation>Es belegt noch %1 auf der Festplatte, bis es recycelt oder gekürzt (truncate) wird.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="112" />
        <source>Marked for recycling: reused at the next need of its pool.</source>
        <translation>Zum Recyceln markiert: wird beim nächsten Bedarf seines Pools wiederverwendet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="115" />
        <source>Busy: in use by a job right now.</source>
        <translation>Busy: wird gerade von einem Job verwendet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="118" />
        <source>Unlabeled: not yet written.</source>
        <translation>Unlabeled: noch nicht beschrieben.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="124" />
        <source>Append: the short volume list does not report the Volume Use Duration, so it is not known whether this volume stays open until it is full.</source>
        <translation>Append: Die kurze Volume-Liste meldet die Volume Use Duration nicht, daher ist nicht bekannt, ob dieses Volume offen bleibt, bis es voll ist.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="128" />
        <source>Append without Volume Use Duration: it stays open until it is full</source>
        <translation>Append ohne Volume Use Duration: Es bleibt offen, bis es voll ist</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="130" />
        <source>(%1)</source>
        <translation>(%1)</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="133" />
        <source>or holds %1 jobs</source>
        <translation>oder %1 Jobs enthält</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="135" />
        <source>; nothing on it is freed before that.</source>
        <translation>; vorher wird darauf nichts freigegeben.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="141" />
        <source>Append with a use duration of %1, but the first write or the retention is not reported.</source>
        <translation>Append mit einer Nutzungsdauer von %1, aber der erste Schreibvorgang oder die Aufbewahrung wird nicht gemeldet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="150" />
        <source>Append until %1 (use duration %2), then %3 of volume retention.</source>
        <translation>Append bis %1 (Nutzungsdauer %2), danach %3 Volume Retention.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="158" />
        <source>%1: the last write or the volume retention is not reported.</source>
        <translation>%1: Der letzte Schreibvorgang oder die Volume Retention wird nicht gemeldet.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="163" />
        <source>%1: last written %2, volume retention %3.</source>
        <translation>%1: zuletzt geschrieben %2, Volume Retention %3.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="174" />
        <source>%1: the job retention of %2 ends before the volume retention of %3; the jobs are pruned first and the volume becomes Purged.</source>
        <translation>%1: Die Job Retention von %2 endet vor der Volume Retention von %3; die Jobs werden zuerst geprunt und das Volume wird Purged.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="182" />
        <source>The date has passed: it is pruned when its pool next needs a volume.</source>
        <translation>Das Datum ist überschritten: Es wird geprunt, wenn sein Pool das nächste Mal ein Volume benötigt.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="187" />
        <source>Status "%1" is not known to this version.</source>
        <translation>Der Status „%1“ ist dieser Version nicht bekannt.</translation>
    </message>
</context>
<context>
    <name>BResourceDialog</name>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="49" />
        <source>Select %1:</source>
        <translation>%1 wählen:</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="67" />
        <source>Edit %1 Resource</source>
        <translation>%1-Ressource bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="77" />
        <source>Show advanced directives</source>
        <translation>Erweiterte Direktiven anzeigen</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="86" />
        <location filename="../src/director/bresourcedialog.cpp" line="248" />
        <source>Save .conf</source>
        <translation>.conf speichern</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="91" />
        <location filename="../src/director/bresourcedialog.cpp" line="325" />
        <source>Save ZIP</source>
        <translation>ZIP speichern</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="223" />
        <source>Save %1 Configuration</source>
        <translation>%1-Konfiguration speichern</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="224" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Konfigurationsdateien (*.conf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="230" />
        <location filename="../src/director/bresourcedialog.cpp" line="238" />
        <location filename="../src/director/bresourcedialog.cpp" line="273" />
        <location filename="../src/director/bresourcedialog.cpp" line="281" />
        <source>Save Failed</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="231" />
        <location filename="../src/director/bresourcedialog.cpp" line="274" />
        <source>%1 "%2" cannot be written. %3</source>
        <translation>%1 „%2“ kann nicht geschrieben werden. %3</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="239" />
        <source>Could not write to %1</source>
        <translation>Konnte nicht nach %1 schreiben</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="246" />
        <location filename="../src/director/bresourcedialog.cpp" line="323" />
        <source>Saved!</source>
        <translation>Gespeichert!</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="263" />
        <source>Save %1 Configuration as ZIP</source>
        <translation>%1-Konfiguration als ZIP speichern</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="264" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-Archive (*.zip);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="282" />
        <source>Could not create ZIP file: %1</source>
        <translation>ZIP-Datei konnte nicht erstellt werden: %1</translation>
    </message>
</context>
<context>
    <name>BResourceForm</name>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="153" />
        <source>No schema available for resource type '%1'</source>
        <translation>Kein Schema für den Ressourcentyp „%1“ verfügbar</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="181" />
        <location filename="../src/config/bresourceform.cpp" line="193" />
        <location filename="../src/config/bresourceform.cpp" line="194" />
        <location filename="../src/config/bresourceform.cpp" line="197" />
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="378" />
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="386" />
        <source>Show/Hide password</source>
        <translation>Passwort anzeigen/verbergen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="412" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414" />
        <source>Browse for file</source>
        <translation>Datei suchen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414" />
        <source>Browse for directory</source>
        <translation>Verzeichnis suchen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="418" />
        <source>Select File</source>
        <translation>Datei wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="420" />
        <source>Select Directory</source>
        <translation>Verzeichnis wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="449" />
        <source>&lt;%1 name&gt;</source>
        <translation>&lt;%1-Name&gt;</translation>
    </message>
</context>
<context>
    <name>BResourcePersistence</name>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="33" />
        <source>schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="34" />
        <source>job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="35" />
        <source>JobDefs resource</source>
        <translation>JobDefs-Ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="36" />
        <source>client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="37" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="38" />
        <source>pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="39" />
        <source>storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="42" />
        <source>resource</source>
        <translation>Ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="49" />
        <source>%1 %2 was not created</source>
        <translation>%1 %2 wurde nicht angelegt</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="50" />
        <source>%1 %2 was not deleted</source>
        <translation>%1 %2 wurde nicht gelöscht</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="53" />
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 wurde nicht geändert</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="105" />
        <source>There is no connection to the configuration methods of the Director. Nothing was sent.</source>
        <translation>Es besteht keine Verbindung zu den Konfigurationsmethoden des Directors. Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="118" />
        <source>Nothing was written.</source>
        <translation>Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="139" />
        <source>Nothing to write</source>
        <translation>Nichts zu schreiben</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="140" />
        <source>The text is the one the Director has.</source>
        <translation>Der Text ist derjenige, den der Director hat.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="151" />
        <source>The text the change was made in does not match its SHA-256. Reload the %1 and make the change again.</source>
        <translation>Der Text, in dem die Änderung vorgenommen wurde, passt nicht zu seinem SHA-256. Laden Sie %1 neu und nehmen Sie die Änderung erneut vor.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="286" />
        <source>%1 %2 created</source>
        <translation>%1 %2 angelegt</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="287" />
        <source>%1 %2 deleted</source>
        <translation>%1 %2 gelöscht</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="288" />
        <source>%1 %2 changed</source>
        <translation>%1 %2 geändert</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="290" />
        <source>The Director wrote the configuration and reloaded it.</source>
        <translation>Der Director hat die Konfiguration geschrieben und neu geladen.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="291" />
        <source>The Director wrote the configuration but did not report a reload; the change takes effect with its next reload.</source>
        <translation>Der Director hat die Konfiguration geschrieben, aber kein Neuladen gemeldet; die Änderung wird mit dem nächsten Neuladen wirksam.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="294" />
        <source>The reload does not adjust everything; the Director names these follow-up commands:</source>
        <translation>Das Neuladen übernimmt nicht alles; der Director nennt diese Folgebefehle:</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="299" />
        <source>%1 %2 was changed on the Director</source>
        <translation>%1 %2 wurde auf dem Director geändert</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="300" />
        <source>The Director has another state of the %1 than the one this change started from. Nothing was written and nothing is overwritten. Compare the two texts, reload the %1 and make the change again.</source>
        <translation>Der Director hat einen anderen Stand von %1 als den, von dem diese Änderung ausging. Es wurde nichts geschrieben und nichts überschrieben. Vergleichen Sie die beiden Texte, laden Sie %1 neu und nehmen Sie die Änderung erneut vor.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="307" />
        <source>The Director checked the new configuration and rejected it. Nothing was written.</source>
        <translation>Der Director hat die neue Konfiguration geprüft und abgelehnt. Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="311" />
        <source>The %1 is still used. The Director does not delete a %1 that other resources name; change them first. Nothing was deleted.</source>
        <translation>%1 wird noch verwendet. Der Director löscht kein Objekt vom Typ %1, das von anderen Ressourcen genannt wird; ändern Sie diese zuerst. Es wurde nichts gelöscht.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="317" />
        <source>The console of this session may not change this %1 (the ACL of its resource type). Nothing was written.</source>
        <translation>Die Console dieser Sitzung darf %1 nicht ändern (ACL ihres Ressourcentyps). Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="319" />
        <source>The console of this session may not change this %1 (its %2 ACL). Nothing was written.</source>
        <translation>Die Console dieser Sitzung darf %1 nicht ändern (ihre %2-ACL). Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="324" />
        <source>The Director has no %1 of this name (any more). Nothing was written.</source>
        <translation>Der Director hat (nicht mehr) %1 dieses Namens. Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="328" />
        <source>The Director already has a resource of this name. Nothing was written.</source>
        <translation>Der Director hat bereits eine Ressource dieses Namens. Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="332" />
        <source>The Director wrote the file, but reloading the configuration failed, so it put the old file back. The %1 is as it was.</source>
        <translation>Der Director hat die Datei geschrieben, aber das Neuladen der Konfiguration ist fehlgeschlagen, daher hat er die alte Datei wiederhergestellt. %1 ist unverändert.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="338" />
        <source>This connection has no configuration methods. Nothing was sent.</source>
        <translation>Diese Verbindung hat keine Konfigurationsmethoden. Es wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="344" />
        <source>The Director refused the call (%1). The console needs the command ACL "configure".</source>
        <translation>Der Director hat den Aufruf abgelehnt (%1). Die Console benötigt die Befehls-ACL "configure".</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="346" />
        <source>The call failed (%1). Nothing was written.</source>
        <translation>Der Aufruf ist fehlgeschlagen (%1). Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="350" />
        <source>The Director sent no status. Nothing is known to have been written.</source>
        <translation>Der Director hat keinen Status gesendet. Es ist nicht bekannt, dass etwas geschrieben wurde.</translation>
    </message>
</context>
<context>
    <name>BResourceReportDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="229" />
        <source>file %1</source>
        <translation>Datei %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="231" />
        <source>line %1</source>
        <translation>Zeile %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="233" />
        <source>column %1</source>
        <translation>Spalte %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="237" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="269" />
        <source>This was found when the Director checked the change; nothing was sent to be written.</source>
        <translation>Dies wurde gefunden, als der Director die Änderung prüfte; es wurde nichts zum Schreiben gesendet.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="275" />
        <source>The Director says: %1</source>
        <translation>Der Director meldet: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="279" />
        <source>Where the configuration is invalid</source>
        <translation>Wo die Konfiguration ungültig ist</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="288" />
        <location filename="../src/config/bresourcereviewdialog.cpp" line="291" />
        <source>Still referenced by</source>
        <translation>Noch referenziert von</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="299" />
        <source>Follow-up commands the Director names</source>
        <translation>Folgebefehle, die der Director nennt</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="306" />
        <source>They are not run by this application.</source>
        <translation>Sie werden von dieser Anwendung nicht ausgeführt.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="320" />
        <source>On the Director now</source>
        <translation>Jetzt auf dem Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="322" />
        <source>What was to be written (not written)</source>
        <translation>Was geschrieben werden sollte (nicht geschrieben)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="328" />
        <source>&amp;Reload the %1</source>
        <translation>%1 neu &amp;laden</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="330" />
        <source>Reads the %1 again from the Director; your changes are offered to be kept or discarded</source>
        <translation>Liest %1 erneut vom Director; Ihre Änderungen können behalten oder verworfen werden</translation>
    </message>
</context>
<context>
    <name>BResourceReviewDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="80" />
        <source>Change %1 %2</source>
        <translation>%1 %2 ändern</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="81" />
        <source>Change %1 %2 on the Director?</source>
        <translation>%1 %2 auf dem Director ändern?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="82" />
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Der Director hat die neue Konfiguration geprüft und akzeptiert sie. Es wird nichts geschrieben, bis Sie "Auf den Director schreiben" wählen.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="86" />
        <source>Create %1 %2</source>
        <translation>%1 %2 anlegen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="87" />
        <source>Create %1 %2 on the Director?</source>
        <translation>%1 %2 auf dem Director anlegen?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="88" />
        <source>The Director checked the new %1 and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Der Director hat %1 geprüft und akzeptiert es. Es wird nichts geschrieben, bis Sie "Auf den Director schreiben" wählen.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="92" />
        <source>Delete %1 %2</source>
        <translation>%1 %2 löschen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="93" />
        <source>Delete %1 %2 on the Director?</source>
        <translation>%1 %2 auf dem Director löschen?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="94" />
        <source>The Director checked that nothing refers to the %1. Nothing is deleted until you choose "Delete on the Director".</source>
        <translation>Der Director hat geprüft, dass nichts auf %1 verweist. Es wird nichts gelöscht, bis Sie "Auf dem Director löschen" wählen.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="107" />
        <location filename="../src/config/bresourcereviewdialog.cpp" line="110" />
        <source>Changed directives</source>
        <translation>Geänderte Direktiven</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="121" />
        <source>Text before and after (- removed, + added)</source>
        <translation>Text vorher und nachher (- entfernt, + hinzugefügt)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="123" />
        <source>Text of the new resource</source>
        <translation>Text der neuen Ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="124" />
        <source>Text that is deleted</source>
        <translation>Text, der gelöscht wird</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="139" />
        <source>Affected</source>
        <translation>Betroffen</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bresourcereviewdialog.cpp" line="143" />
        <source>The change applies to every one of these %n resource(s).</source>
        <translation>
            <numerusform>Die Änderung gilt für diese %n Ressource.</numerusform>
            <numerusform>Die Änderung gilt für jede dieser %n Ressourcen.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="152" />
        <source>(nothing else, as far as this application knows)</source>
        <translation>(sonst nichts, soweit diese Anwendung weiß)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="156" />
        <source>The Director names as referencing it: %1</source>
        <translation>Der Director nennt als verweisend: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="166" />
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Datei auf dem Director: %1 (sie enthält auch andere Ressourcen; nur diese Ressource wird ersetzt).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="168" />
        <source>File on the Director: %1.</source>
        <translation>Datei auf dem Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="170" />
        <source>The Director reloads its configuration after writing. Jobs that are running or already queued keep the configuration they started with.</source>
        <translation>Der Director lädt seine Konfiguration nach dem Schreiben neu. Jobs, die laufen oder bereits eingereiht sind, behalten die Konfiguration, mit der sie gestartet wurden.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="175" />
        <source>&amp;Delete on the Director</source>
        <translation>Auf dem Director &amp;löschen</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="176" />
        <source>&amp;Write to the Director</source>
        <translation>Auf den Director &amp;schreiben</translation>
    </message>
</context>
<context>
    <name>BResourceText</name>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="223" />
        <source>The text does not start with a resource (line %1).</source>
        <translation>Der Text beginnt nicht mit einer Ressource (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="225" />
        <source>Directives on the line of "%1 {" cannot be edited safely (line %2).</source>
        <translation>Direktiven in der Zeile von "%1 {" können nicht sicher bearbeitet werden (Zeile %2).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="235" />
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Text nach der schließenden Klammer der Ressource (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="246" />
        <source>The closing brace of the resource shares its line with a nested block (line %1).</source>
        <translation>Die schließende Klammer der Ressource teilt sich ihre Zeile mit einem verschachtelten Block (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="260" />
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Die schließende Klammer teilt sich ihre Zeile mit einer Direktive (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="333" />
        <source>The text contains no resource.</source>
        <translation>Der Text enthält keine Ressource.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="335" />
        <source>The resource has no closing brace.</source>
        <translation>Der Ressource fehlt die schließende Klammer.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="385" />
        <source>A change without a directive name.</source>
        <translation>Eine Änderung ohne Direktivennamen.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="387" />
        <source>Directive %1 is changed twice.</source>
        <translation>Direktive %1 wird zweimal geändert.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="391" />
        <source>The value of %1 contains a line break.</source>
        <translation>Der Wert von %1 enthält einen Zeilenumbruch.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="393" />
        <source>An empty value for %1; remove the directive instead.</source>
        <translation>Ein leerer Wert für %1; entfernen Sie stattdessen die Direktive.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="396" />
        <source>Directive %1 is on a line that cannot be edited safely (several directives on one line, a list or a nested block). Change it in the text.</source>
        <translation>Direktive %1 steht in einer Zeile, die nicht sicher bearbeitet werden kann (mehrere Direktiven in einer Zeile, eine Liste oder ein verschachtelter Block). Ändern Sie sie im Text.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="400" />
        <source>Directive %1 appears more than once. Change it in the text.</source>
        <translation>Direktive %1 kommt mehr als einmal vor. Ändern Sie sie im Text.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="453" />
        <source>The new text could not be read back: %1</source>
        <translation>Der neue Text konnte nicht zurückgelesen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="458" />
        <source>The new text still has directive %1.</source>
        <translation>Der neue Text enthält noch Direktive %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="460" />
        <source>The new text does not have the intended value of %1.</source>
        <translation>Der neue Text hat nicht den beabsichtigten Wert von %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="464" />
        <source>The new text would change more than the edited directives.</source>
        <translation>Der neue Text würde mehr ändern als die bearbeiteten Direktiven.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="466" />
        <source>The new text would change the type of the resource.</source>
        <translation>Der neue Text würde den Typ der Ressource ändern.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="468" />
        <source>The new text would change the name of the resource.</source>
        <translation>Der neue Text würde den Namen der Ressource ändern.</translation>
    </message>
</context>
<context>
    <name>BResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="46" />
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="48" />
        <source>Toggle config file preview</source>
        <translation>Vorschau der Konfigurationsdatei umschalten</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="60" />
        <source>Edit...</source>
        <translation>Bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="66" />
        <source>Export .conf</source>
        <translation>.conf exportieren</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="72" />
        <source>Export ZIP</source>
        <translation>ZIP exportieren</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85" />
        <source>Source File</source>
        <translation>Quelldatei</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="136" />
        <source>%1: %2 resource(s)</source>
        <translation>%1: %2 Ressource(n)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="225" />
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 wurde nicht geändert</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="243" />
        <source>Not written, because the form cannot write lists or nested blocks: %1. Change them in the configuration file.</source>
        <translation>Nicht geschrieben, weil das Formular keine Listen oder verschachtelten Blöcke schreiben kann: %1. Ändern Sie diese in der Konfigurationsdatei.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="248" />
        <source>Nothing was changed.</source>
        <translation>Es wurde nichts geändert.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="249" />
        <source>Nothing that can be written was changed. %1</source>
        <translation>Es wurde nichts geändert, das geschrieben werden kann. %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="266" />
        <source>Another change is still being written. Try again when it is done.</source>
        <translation>Eine andere Änderung wird noch geschrieben. Versuchen Sie es erneut, wenn sie abgeschlossen ist.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="276" />
        <source>The text the Director sent does not match its SHA-256.</source>
        <translation>Der vom Director gesendete Text passt nicht zu seinem SHA-256.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="278" />
        <source>%1 Nothing was written.</source>
        <translation>%1 Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="285" />
        <source>The change cannot be written into the text of the resource safely: %1 Nothing was written.</source>
        <translation>Die Änderung kann nicht sicher in den Text der Ressource geschrieben werden: %1 Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="504" />
        <source>Schedule "%1" is not written here.</source>
        <translation>Schedule "%1" wird hier nicht geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="604" />
        <source>Export %1 Configuration</source>
        <translation>%1-Konfiguration exportieren</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="605" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Konfigurationsdateien (*.conf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="610" />
        <location filename="../src/director/bresourcewidget.cpp" line="665" />
        <source>Export Failed</source>
        <translation>Export fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="611" />
        <source>Could not write to %1</source>
        <translation>Konnte nicht nach %1 schreiben</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="656" />
        <source>Export %1 Configuration as ZIP</source>
        <translation>%1-Konfiguration als ZIP exportieren</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="657" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-Archive (*.zip);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="666" />
        <source>Could not create ZIP file: %1</source>
        <translation>ZIP-Datei konnte nicht erstellt werden: %1</translation>
    </message>
</context>
<context>
    <name>BRestoreBrowsePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="479" />
        <source>Select Files</source>
        <translation>Dateien auswählen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="480" />
        <source>Browse and select files/directories to restore.</source>
        <translation>Dateien/Verzeichnisse zur Wiederherstellung durchsuchen und auswählen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="492" />
        <source>Loading files...</source>
        <translation>Dateien werden geladen...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="518" />
        <location filename="../src/jobs/brestorewizard.cpp" line="699" />
        <source>No files selected</source>
        <translation>Keine Dateien ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="531" />
        <source>Step %1 of %2 — Browse and select files/directories to restore.</source>
        <translation>Schritt %1 von %2 — Dateien/Verzeichnisse zur Wiederherstellung durchsuchen und auswählen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="534" />
        <source>Browse files — Client: %1, FileSet: %2</source>
        <translation>Dateien durchsuchen — Client: %1, FileSet: %2</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="708" />
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n Verzeichnis</numerusform>
            <numerusform>%n Verzeichnisse</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="710" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n Datei</numerusform>
            <numerusform>%n Dateien</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="711" />
        <source>Selected: %1</source>
        <translation>Ausgewählt: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="713" />
        <source>%n item(s) selected</source>
        <translation>
            <numerusform>%n Element ausgewählt</numerusform>
            <numerusform>%n Elemente ausgewählt</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BRestoreExecutePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1170" />
        <source>Execute Restore</source>
        <translation>Wiederherstellung ausführen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1171" />
        <source>Sending commands to the Director...</source>
        <translation>Befehle werden an den Director gesendet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1175" />
        <source>Preparing...</source>
        <translation>Vorbereitung...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1205" />
        <source>Step %1 of %2 — Sending commands to the Director...</source>
        <translation>Schritt %1 von %2 — Befehle werden an den Director gesendet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1214" />
        <source>Step %v of %m</source>
        <translation>Schritt %v von %m</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1240" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1259" />
        <source>No director connection</source>
        <translation>Keine Director-Verbindung</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1247" />
        <source>Creating restore table...</source>
        <translation>Restore-Tabelle wird erstellt...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1248" />
        <source>&gt;&gt;&gt; .bvfs_restore %1</source>
        <translation>&gt;&gt;&gt; .bvfs_restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1266" />
        <source>Executing restore command...</source>
        <translation>Restore-Befehl wird ausgeführt...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1267" />
        <source>&gt;&gt;&gt; restore %1</source>
        <translation>&gt;&gt;&gt; restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1281" />
        <source>Cleaning up restore table...</source>
        <translation>Restore-Tabelle wird aufgeräumt...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1284" />
        <source>&gt;&gt;&gt; .bvfs_cleanup %1</source>
        <translation>&gt;&gt;&gt; .bvfs_cleanup %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1300" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1416" />
        <source>.bvfs_restore failed: %1</source>
        <translation>.bvfs_restore fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1304" />
        <source>Restore table created successfully.</source>
        <translation>Restore-Tabelle erfolgreich erstellt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1320" />
        <source>Restore command failed: %1</source>
        <translation>Restore-Befehl fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1324" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1378" />
        <source>Hint: The Bareos Director restricts the "where" path via WhereACL. Ask your administrator to add the path "%1" to the WhereACL of your console configuration, or use a permitted restore path.</source>
        <translation>Hinweis: Der Bareos Director beschränkt den "where"-Pfad über die WhereACL. Bitten Sie Ihren Administrator, den Pfad "%1" zur WhereACL Ihrer Console-Konfiguration hinzuzufügen, oder verwenden Sie einen erlaubten Wiederherstellungspfad.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1347" />
        <source>Restore job submitted successfully (Job ID: %1).</source>
        <translation>Restore-Job erfolgreich übermittelt (Job-ID: %1).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1349" />
        <source>Restore command accepted.</source>
        <translation>Restore-Befehl angenommen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1373" />
        <source>Restore command failed.</source>
        <translation>Restore-Befehl fehlgeschlagen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1392" />
        <source>Restore job submitted successfully.</source>
        <translation>Restore-Job erfolgreich übermittelt.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1402" />
        <source>WARNING: Unexpected restore response.</source>
        <translation>WARNUNG: Unerwartete Antwort auf den Restore-Befehl.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1420" />
        <source>Restore table created (text response).</source>
        <translation>Restore-Tabelle erstellt (Textantwort).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1435" />
        <source>Cleanup complete.</source>
        <translation>Aufräumen abgeschlossen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1445" />
        <source>Complete</source>
        <translation>Abgeschlossen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1448" />
        <source>Restore job %1 submitted successfully!</source>
        <translation>Restore-Job %1 erfolgreich übermittelt!</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1449" />
        <source>The restore job (ID: %1) is now running on the Director.
You can monitor its progress in the Jobs view.
Refresh the job list to see the restore job status.</source>
        <translation>Der Restore-Job (ID: %1) läuft jetzt auf dem Director.
Sie können seinen Fortschritt in der Job-Ansicht verfolgen.
Aktualisieren Sie die Job-Liste, um den Status des Restore-Jobs zu sehen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1454" />
        <source>Restore completed.</source>
        <translation>Wiederherstellung abgeschlossen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1455" />
        <source>The restore commands have been executed.
Check the log above for details.</source>
        <translation>Die Restore-Befehle wurden ausgeführt.
Details finden Sie im Protokoll oben.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1467" />
        <source>ERROR: %1</source>
        <translation>FEHLER: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1470" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1471" />
        <source>Restore failed</source>
        <translation>Wiederherstellung fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1499" />
        <source>command</source>
        <translation>Befehl</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1500" />
        <source>Timeout waiting for %1 response (no response within %2 seconds)</source>
        <translation>Zeitüberschreitung beim Warten auf die Antwort auf %1 (keine Antwort innerhalb von %2 Sekunden)</translation>
    </message>
</context>
<context>
    <name>BRestoreOptionsPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="726" />
        <source>Restore Options</source>
        <translation>Wiederherstellungsoptionen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="727" />
        <source>Configure where and how to restore the selected files.</source>
        <translation>Legen Sie fest, wohin und wie die ausgewählten Dateien wiederhergestellt werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="733" />
        <source>Source:</source>
        <translation>Quelle:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="736" />
        <source>Target Client:</source>
        <translation>Ziel-Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="739" />
        <source>Restore Where:</source>
        <translation>Wiederherstellen nach:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="742" />
        <source>Always</source>
        <translation>Immer</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="743" />
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="744" />
        <source>If Newer</source>
        <translation>Wenn neuer</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="745" />
        <source>If Older</source>
        <translation>Wenn älter</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="746" />
        <source>Replace Policy:</source>
        <translation>Ersetzungsrichtlinie:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="770" />
        <source>Step %1 of %2 — Configure where and how to restore the selected files.</source>
        <translation>Schritt %1 von %2 — Legen Sie fest, wohin und wie die ausgewählten Dateien wiederhergestellt werden.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="775" />
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n Verzeichnis</numerusform>
            <numerusform>%n Verzeichnisse</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="777" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n Datei</numerusform>
            <numerusform>%n Dateien</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="779" />
        <source>nothing</source>
        <translation>nichts</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="785" />
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="786" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="788" />
        <source>Job %1: %2 — %3 selected (estimated size: %4)</source>
        <translation>Job %1: %2 — %3 ausgewählt (geschätzte Größe: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="822" />
        <source>Always overwrite existing files on the target, regardless of timestamps.</source>
        <translation>Vorhandene Dateien auf dem Ziel immer überschreiben, unabhängig von Zeitstempeln.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="826" />
        <source>Never overwrite. Skip files that already exist on the target.</source>
        <translation>Nie überschreiben. Dateien, die auf dem Ziel bereits existieren, überspringen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="830" />
        <source>Only overwrite if the backed-up file is newer than the existing file.</source>
        <translation>Nur überschreiben, wenn die gesicherte Datei neuer als die vorhandene Datei ist.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="834" />
        <source>Only overwrite if the backed-up file is older than the existing file.</source>
        <translation>Nur überschreiben, wenn die gesicherte Datei älter als die vorhandene Datei ist.</translation>
    </message>
</context>
<context>
    <name>BRestorePreviewPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="872" />
        <source>Command Preview</source>
        <translation>Befehlsvorschau</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="873" />
        <source>Review the commands that will be sent to the Director.</source>
        <translation>Prüfen Sie die Befehle, die an den Director gesendet werden.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="883" />
        <source>Permission Checks</source>
        <translation>Berechtigungsprüfungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="890" />
        <source>Restore command:</source>
        <translation>Restore-Befehl:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="891" />
        <source>Target client:</source>
        <translation>Ziel-Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="892" />
        <source>Where path:</source>
        <translation>Where-Pfad:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="914" />
        <source>Click "Commit" to execute these commands on the Director.</source>
        <translation>Klicken Sie auf "Commit", um diese Befehle auf dem Director auszuführen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="927" />
        <source>Step %1 of %2 — Review the commands that will be sent to the Director.</source>
        <translation>Schritt %1 von %2 — Prüfen Sie die Befehle, die an den Director gesendet werden.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="970" />
        <location filename="../src/jobs/brestorewizard.cpp" line="973" />
        <source>%n directory(ies) (including all contents)</source>
        <translation>
            <numerusform>%n Verzeichnis (mit gesamtem Inhalt)</numerusform>
            <numerusform>%n Verzeichnisse (mit gesamtem Inhalt)</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="971" />
        <source>%n individual file(s)</source>
        <translation>
            <numerusform>%n einzelne Datei</numerusform>
            <numerusform>%n einzelne Dateien</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="975" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n Datei</numerusform>
            <numerusform>%n Dateien</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="977" />
        <source>No files or directories selected</source>
        <translation>Keine Dateien oder Verzeichnisse ausgewählt</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="984" />
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="985" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="987" />
        <source>Restore summary:
• Selection: %1
• Estimated size: %2
• Target client: %3
• Restore to: %4
• Replace policy: %5</source>
        <translation>Zusammenfassung der Wiederherstellung:
• Auswahl: %1
• Geschätzte Größe: %2
• Ziel-Client: %3
• Wiederherstellen nach: %4
• Ersetzungsrichtlinie: %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1014" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1015" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1016" />
        <source>Checking...</source>
        <translation>Prüfe...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1017" />
        <source>Querying Director for permissions...</source>
        <translation>Berechtigungen werden beim Director abgefragt...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1029" />
        <source>No Director connection</source>
        <translation>Keine Director-Verbindung</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1087" />
        <source>restore command lacks client= parameter</source>
        <translation>Dem restore-Befehl fehlt der Parameter client=</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1092" />
        <source>Available (subject to WhereACL)</source>
        <translation>Verfügbar (vorbehaltlich WhereACL)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1095" />
        <source>Not available</source>
        <translation>Nicht verfügbar</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1098" />
        <source>N/A (restore not authorized)</source>
        <translation>N/A (Wiederherstellung nicht autorisiert)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1102" />
        <source>Permission check completed.</source>
        <translation>Berechtigungsprüfung abgeschlossen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1119" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1121" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1123" />
        <source>Unknown (timeout)</source>
        <translation>Unbekannt (Zeitüberschreitung)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1125" />
        <source>Permission check timed out — you can still proceed.</source>
        <translation>Zeitüberschreitung bei der Berechtigungsprüfung — Sie können trotzdem fortfahren.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1153" />
        <source>Authorized</source>
        <translation>Autorisiert</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1156" />
        <source>Not authorized</source>
        <translation>Nicht autorisiert</translation>
    </message>
</context>
<context>
    <name>BRestoreSelectJobPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="146" />
        <source>Restore Conditions</source>
        <translation>Wiederherstellungsbedingungen</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="147" />
        <source>Configure the restore scope before browsing files.</source>
        <translation>Legen Sie den Umfang der Wiederherstellung fest, bevor Sie Dateien durchsuchen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="154" />
        <source>Source Job:</source>
        <translation>Quell-Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="158" />
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="162" />
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="166" />
        <source>All related backups (full restore chain)</source>
        <translation>Alle zugehörigen Backups (vollständige Wiederherstellungskette)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="175" />
        <source>Restore Scope:</source>
        <translation>Umfang der Wiederherstellung:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="181" />
        <source>Before Date:</source>
        <translation>Vor Datum:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="208" />
        <source>Step %1 of %2 — Configure the restore scope before browsing files.</source>
        <translation>Schritt %1 von %2 — Legen Sie den Umfang der Wiederherstellung fest, bevor Sie Dateien durchsuchen.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="211" />
        <location filename="../src/jobs/brestorewizard.cpp" line="416" />
        <source>Job %1: %2 (Client: %3, FileSet: %4)</source>
        <translation>Job %1: %2 (Client: %3, FileSet: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="217" />
        <source>Only this specific job (ID: %1)</source>
        <translation>Nur dieser bestimmte Job (ID: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="266" />
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="267" />
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="270" />
        <source>Warning: Failed to load %1 from Director within timeout. The Director may be busy or the connection may be slow.</source>
        <translation>Warnung: %1 konnte nicht innerhalb der Zeitbegrenzung vom Director geladen werden. Der Director ist möglicherweise ausgelastet oder die Verbindung langsam.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="272" />
        <source> and </source>
        <translation> und </translation>
    </message>
</context>
<context>
    <name>BRestoreWizard</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="114" />
        <source>Restore Wizard — Job %1: %2</source>
        <translation>Wiederherstellungsassistent — Job %1: %2</translation>
    </message>
</context>
<context>
    <name>BRunOverridesEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="39" />
        <source>not set</source>
        <translation>nicht gesetzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="40" />
        <source>yes</source>
        <translation>ja</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="41" />
        <source>no</source>
        <translation>nein</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="83" />
        <source>(job's setting)</source>
        <translation>(Einstellung des Jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="87" />
        <source>The Director has not reported any %1 yet; refresh the view after connecting. Not set: the setting of the job applies.</source>
        <translation>Der Director hat noch keine %1 gemeldet; aktualisieren Sie die Ansicht nach dem Verbinden. Nicht gesetzt: Die Einstellung des Jobs gilt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="89" />
        <source>The %1 the Director reported. Not set: the setting of the job applies.</source>
        <translation>Die vom Director gemeldeten %1. Nicht gesetzt: Die Einstellung des Jobs gilt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="109" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="110" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="111" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="112" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="113" />
        <source>pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="114" />
        <source>storages</source>
        <translation>Storages</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="117" />
        <source>set to</source>
        <translation>setzen auf</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="123" />
        <source>Level of the job</source>
        <translation>Level des Jobs</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="132" />
        <source>The level the Run line names. "Level of the job": the line names none and the level of the job applies.</source>
        <translation>Das Level, das die Run-Zeile nennt. "Level des Jobs": Die Zeile nennt keines, und das Level des Jobs gilt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="138" />
        <source>Priority</source>
        <translation>Priorität</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="139" />
        <source>Override the priority</source>
        <translation>Priorität überschreiben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="140" />
        <source>a time period, e.g. 8 hours</source>
        <translation>ein Zeitraum, z. B. 8 hours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="141" />
        <source>Empty: the setting of the job applies</source>
        <translation>Leer: Die Einstellung des Jobs gilt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="167" />
        <source>&amp;Level:</source>
        <translation>&amp;Level:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="168" />
        <source>&amp;Storage:</source>
        <translation>&amp;Storage:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="170" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="171" />
        <source>&amp;Messages:</source>
        <translation>&amp;Messages:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="173" />
        <source>&amp;Full pool:</source>
        <translation>&amp;Full-Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="174" />
        <source>P&amp;riority:</source>
        <translation>P&amp;riorität:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="176" />
        <source>&amp;Incremental pool:</source>
        <translation>&amp;Incremental-Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="177" />
        <source>Sp&amp;ool data:</source>
        <translation>Daten s&amp;poolen:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="179" />
        <source>&amp;Differential pool:</source>
        <translation>&amp;Differential-Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="180" />
        <source>&amp;Accurate:</source>
        <translation>&amp;Accurate:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="182" />
        <source>&amp;Next pool:</source>
        <translation>&amp;Next Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="183" />
        <source>Max run sched &amp;time:</source>
        <translation>Max Run Sched &amp;Time:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="271" />
        <source>%1 (not reported)</source>
        <translation>%1 (nicht gemeldet)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="273" />
        <source>%1 is named by the Run line, but the Director did not report it. It is kept as it is.</source>
        <translation>%1 wird von der Run-Zeile genannt, aber der Director hat es nicht gemeldet. Es bleibt unverändert.</translation>
    </message>
</context>
<context>
    <name>BRunParser</name>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="113" />
        <location filename="../src/schedules/model/brunparser.cpp" line="145" />
        <source>A Run line cannot contain a line break.</source>
        <translation>Eine Run-Zeile darf keinen Zeilenumbruch enthalten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="134" />
        <source>Unexpected character "%1".</source>
        <translation>Unerwartetes Zeichen "%1".</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="163" />
        <source>A quoted value is not closed.</source>
        <translation>Ein Wert in Anführungszeichen ist nicht abgeschlossen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="393" />
        <source>The Run line is empty.</source>
        <translation>Die Run-Zeile ist leer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="408" />
        <source>Expected an equals, got: %1</source>
        <translation>Gleichheitszeichen erwartet, erhalten: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="412" />
        <location filename="../src/schedules/model/brunparser.cpp" line="457" />
        <source>Expected a value after "%1=".</source>
        <translation>Wert nach "%1=" erwartet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="423" />
        <source>Expect a YES or NO, got: %1</source>
        <translation>YES oder NO erwartet, erhalten: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="433" />
        <source>Job level field: %1 not found in run record</source>
        <translation>Job-Level-Feld: %1 nicht im Run-Eintrag gefunden</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="440" />
        <source>expected a positive integer, got: %1</source>
        <translation>positive Ganzzahl erwartet, erhalten: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="446" />
        <source>expected a time period, got: %1</source>
        <translation>Zeitraum erwartet, erhalten: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="497" />
        <source>Unexpected token: %1</source>
        <translation>Unerwartetes Token: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="502" />
        <source>Day number out of range (1-31)</source>
        <translation>Tagesnummer außerhalb des Bereichs (1-31)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="509" />
        <source>Unexpected token: "%1"</source>
        <translation>Unerwartetes Token: "%1"</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="519" />
        <location filename="../src/schedules/model/brunparser.cpp" line="653" />
        <source>Week number out of range (0-53)</source>
        <translation>Wochennummer außerhalb des Bereichs (0-53)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="525" />
        <source>Job type field: %1 in run record not found</source>
        <translation>Job-Typ-Feld: %1 im Run-Eintrag nicht gefunden</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="555" />
        <source>Time must be preceded by keyword AT.</source>
        <translation>Vor der Uhrzeit muss das Schlüsselwort AT stehen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="571" />
        <location filename="../src/schedules/model/brunparser.cpp" line="576" />
        <location filename="../src/schedules/model/brunparser.cpp" line="585" />
        <source>Bad time specification.</source>
        <translation>Ungültige Zeitangabe.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="610" />
        <source>Bad day specification in modulo.</source>
        <translation>Ungültige Tagesangabe im Modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="612" />
        <source>Bad day specification, offset must always be &lt;= than modulo.</source>
        <translation>Ungültige Tagesangabe, der Offset muss immer &lt;= dem Modulo sein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="622" />
        <source>Week number out of range (0-53) in modulo</source>
        <translation>Wochennummer außerhalb des Bereichs (0-53) im Modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="624" />
        <source>Week interval out of range (1-53) in modulo</source>
        <translation>Wochenintervall außerhalb des Bereichs (1-53) im Modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="626" />
        <source>Bad week number specification in modulo, offset must always be &lt;= than modulo.</source>
        <translation>Ungültige Wochennummer im Modulo, der Offset muss immer &lt;= dem Modulo sein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="633" />
        <source>Bad modulo time specification. Format for weekdays is '01/02', for yearweeks is 'w01/w02'.</source>
        <translation>Ungültige Modulo-Zeitangabe. Das Format für Wochentage ist '01/02', für Kalenderwochen 'w01/w02'.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="646" />
        <source>Bad day range specification.</source>
        <translation>Ungültige Angabe des Tagesbereichs.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="661" />
        <source>Invalid month, week or position day range</source>
        <translation>Ungültiger Bereich für Monat, Woche oder Positionstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="665" />
        <source>Invalid month, weekday or position range</source>
        <translation>Ungültiger Bereich für Monat, Wochentag oder Position</translation>
    </message>
</context>
<context>
    <name>BRunRuleDialog</name>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="94" />
        <source>Only on these da&amp;ys of the month</source>
        <translation>Nur an diesen &amp;Tagen des Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="95" />
        <source>Only in these &amp;weeks of the year</source>
        <translation>Nur in diesen &amp;Wochen des Jahres</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="96" />
        <source>last</source>
        <translation>letzte</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="97" />
        <source>&amp;Every hour</source>
        <translation>&amp;Jede Stunde</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="104" />
        <source>Calendar rule</source>
        <translation>Kalenderregel</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="107" />
        <source>A calendar rule runs by day of the month or by week of the year. It does not belong to a weekday and is not painted in the grid.</source>
        <translation>Eine Kalenderregel läuft nach Tag des Monats oder nach Woche des Jahres. Sie gehört zu keinem Wochentag und wird nicht im Raster gezeichnet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="114" />
        <source>What runs</source>
        <translation>Was läuft</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="122" />
        <source>Not checked: every day of the month</source>
        <translation>Nicht angehakt: jeder Tag des Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="127" />
        <source>Day %1</source>
        <translation>Tag %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="132" />
        <source>Not checked: every week of the year. The weeks are numbered w00 to w53 as in a Run line.</source>
        <translation>Nicht angehakt: jede Woche des Jahres. Die Wochen sind wie in einer Run-Zeile von w00 bis w53 nummeriert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="138" />
        <source>Week %1</source>
        <translation>Woche %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="144" />
        <source>And only</source>
        <translation>Und nur</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="146" />
        <source>on these weekdays:</source>
        <translation>an diesen Wochentagen:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="162" />
        <source>in these weeks of the month:</source>
        <translation>in diesen Wochen des Monats:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="177" />
        <source>The last seven days of a month</source>
        <translation>Die letzten sieben Tage eines Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="183" />
        <source>in these months:</source>
        <translation>in diesen Monaten:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="200" />
        <source>Time</source>
        <translation>Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="205" />
        <source>%1 hours</source>
        <translation>%1 Uhr</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="208" />
        <source>The run starts in every hour of the day ("hourly")</source>
        <translation>Der Lauf startet in jeder Stunde des Tages ("hourly")</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="210" />
        <source>&amp;Minute of the hour:</source>
        <translation>&amp;Minute der Stunde:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="213" />
        <source>Minute of the hour</source>
        <translation>Minute der Stunde</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="226" />
        <source>Run line</source>
        <translation>Run-Zeile</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="291" />
        <source>Odd</source>
        <translation>Ungerade</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="292" />
        <source>Even</source>
        <translation>Gerade</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="293" />
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="387" />
        <source>Choose days of the month or weeks of the year. A run without them belongs to the grid and is painted there.</source>
        <translation>Wählen Sie Tage des Monats oder Wochen des Jahres. Ein Lauf ohne sie gehört zum Raster und wird dort gezeichnet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="420" />
        <source>Run = %1</source>
        <translation>Run = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="434" />
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
</context>
<context>
    <name>BRunScriptDialog</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="45" />
        <source>Edit RunScript</source>
        <translation>RunScript bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="54" />
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="55" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="58" />
        <source>Select Script</source>
        <translation>Skript auswählen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="65" />
        <source>Command:</source>
        <translation>Befehl:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="70" />
        <source>Runs When:</source>
        <translation>Läuft wann:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="75" />
        <source>Runs On Client</source>
        <translation>Läuft auf dem Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="79" />
        <source>Runs On Failure</source>
        <translation>Läuft bei Fehlschlag</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="82" />
        <source>Abort Job On Error</source>
        <translation>Job bei Fehler abbrechen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="86" />
        <source>Fail Job On Error</source>
        <translation>Job bei Fehler fehlschlagen lassen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97" />
        <source>Validation</source>
        <translation>Prüfung</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97" />
        <source>Command is required.</source>
        <translation>Befehl ist erforderlich.</translation>
    </message>
</context>
<context>
    <name>BRunScriptEditor</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>Command</source>
        <translation>Befehl</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>Runs When</source>
        <translation>Läuft wann</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>On Client</source>
        <translation>Auf Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="154" />
        <source>+ Add Script</source>
        <translation>+ Skript hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="155" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="156" />
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231" />
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231" />
        <source>No</source>
        <translation>Nein</translation>
    </message>
</context>
<context>
    <name>BRunWriter</name>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="177" />
        <source>The run has an empty hour, day, month or week set and cannot be written as a Run line.</source>
        <translation>Der Lauf hat eine leere Menge an Stunden, Tagen, Monaten oder Wochen und kann nicht als Run-Zeile geschrieben werden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="182" />
        <source>The minute %1 is out of range.</source>
        <translation>Die Minute %1 liegt außerhalb des Bereichs.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="246" />
        <source>Run line "%1" is not understood (%2) and is never rewritten.</source>
        <translation>Run-Zeile "%1" wird nicht verstanden (%2) und wird nie neu geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="265" />
        <source>The generated Run line "%1" does not read back: %2</source>
        <translation>Die erzeugte Run-Zeile "%1" lässt sich nicht zurücklesen: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="270" />
        <source>The generated Run line "%1" does not say what was intended.</source>
        <translation>Die erzeugte Run-Zeile "%1" sagt nicht das Beabsichtigte aus.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="286" />
        <source>Run line %1: %2</source>
        <translation>Run-Zeile %1: %2</translation>
    </message>
</context>
<context>
    <name>BScheduleBasicsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="516" />
        <source>Schedule Basics</source>
        <translation>Schedule-Grundlagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="517" />
        <source>Configure the schedule name and select an optional template.</source>
        <translation>Legen Sie den Namen des Schedules fest und wählen Sie optional eine Vorlage.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="522" />
        <source>Basic Settings</source>
        <translation>Grundeinstellungen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="526" />
        <source>e.g., WeeklyCycle</source>
        <translation>z. B. WeeklyCycle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="527" />
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="530" />
        <source>e.g., Full Sunday, incremental daily</source>
        <translation>z. B. Full am Sonntag, täglich Incremental</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="531" />
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="533" />
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="540" />
        <source>Template</source>
        <translation>Vorlage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="544" />
        <source>(Empty — build from scratch)</source>
        <translation>(Leer — von Grund auf erstellen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="550" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="596" />
        <source>Select a template to pre-fill the schedule with common run directives.</source>
        <translation>Wählen Sie eine Vorlage, um den Schedule mit gängigen Run-Direktiven vorzubelegen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="609" />
        <source>&lt;b&gt;%1&lt;/b&gt;: %2&lt;br&gt;%3 run directive(s)</source>
        <translation>&lt;b&gt;%1&lt;/b&gt;: %2&lt;br&gt;%3 Run-Direktive(n)</translation>
    </message>
</context>
<context>
    <name>BScheduleBrushDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="293" />
        <source>New brush</source>
        <translation>Neuer Pinsel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="296" />
        <source>A brush is what painting puts into a cell: a level, the minute of the hour and, if wanted, settings that replace those of the job.</source>
        <translation>Ein Pinsel ist das, was das Malen in eine Zelle setzt: ein Level, die Minute der Stunde und, falls gewünscht, Einstellungen, die die des Jobs ersetzen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="305" />
        <source>Minute of the hour</source>
        <translation>Minute der Stunde</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="306" />
        <source>The run starts at this minute of every painted hour</source>
        <translation>Der Lauf startet zu dieser Minute jeder gemalten Stunde</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="307" />
        <source>Minute of the &amp;hour:</source>
        <translation>Minute der &amp;Stunde:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="310" />
        <source>Level and overrides</source>
        <translation>Level und Überschreibungen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="366" />
        <source>Brush: %1</source>
        <translation>Pinsel: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleEntry</name>
    <message>
        <location filename="../src/models/bresourcemodels.cpp" line="679" />
        <source>The entry does not describe a run.</source>
        <translation>Der Eintrag beschreibt keinen Lauf.</translation>
    </message>
</context>
<context>
    <name>BScheduleGanttWidget</name>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="405" />
        <source>(no client)</source>
        <translation>(kein Client)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="408" />
        <source>(no storage)</source>
        <translation>(kein Storage)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="435" />
        <source>Not used by any job</source>
        <translation>Von keinem Job verwendet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="551" />
        <source>No runs in this week</source>
        <translation>Keine Läufe in dieser Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="552" />
        <source>No runs on this date</source>
        <translation>Keine Läufe an diesem Datum</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Monday</source>
        <translation>Montag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Tuesday</source>
        <translation>Dienstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Wednesday</source>
        <translation>Mittwoch</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Thursday</source>
        <translation>Donnerstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Friday</source>
        <translation>Freitag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Saturday</source>
        <translation>Samstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Sunday</source>
        <translation>Sonntag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Mon</source>
        <translation>Mo</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Tue</source>
        <translation>Di</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Wed</source>
        <translation>Mi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Thu</source>
        <translation>Do</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Fri</source>
        <translation>Fr</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Sat</source>
        <translation>Sa</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Sun</source>
        <translation>So</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1076" />
        <source>%1, if it turns out against it</source>
        <translation>%1, falls es ungünstig ausgeht</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1146" />
        <source>Utilization</source>
        <translation>Auslastung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1373" />
        <source>Not used by any enabled job: nothing starts at this time.</source>
        <translation>Von keinem aktivierten Job verwendet: Zu dieser Zeit startet nichts.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1374" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1391" />
        <source>Time</source>
        <translation>Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1377" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1389" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1379" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1398" />
        <source>Run</source>
        <translation>Lauf</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1390" />
        <source>(level of the job)</source>
        <translation>(Level des Jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1402" />
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1405" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1411" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1414" />
        <source>Predicted wait</source>
        <translation>Erwartete Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1411" />
        <source>none</source>
        <translation>keine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1414" />
        <source>Predicted wait: none</source>
        <translation>Erwartete Wartezeit: keine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1425" />
        <source>the jobs of the Director are not loaded: it is not known which job runs this schedule</source>
        <translation>Die Jobs des Directors sind nicht geladen: Es ist nicht bekannt, welcher Job diesen Schedule ausführt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1430" />
        <source>no successful run of this job in the last %1 days</source>
        <translation>kein erfolgreicher Lauf dieses Jobs in den letzten %1 Tagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1434" />
        <source>No history</source>
        <translation>Keine Historie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1435" />
        <source>Duration unknown: the outline marks the start only.</source>
        <translation>Dauer unbekannt: Der Umriss markiert nur den Start.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1438" />
        <source>Median duration</source>
        <translation>Mediane Dauer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1440" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1447" />
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1442" />
        <source>runs</source>
        <translation>Läufe</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1445" />
        <source>Median wait</source>
        <translation>Mediane Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450" />
        <source>Wait</source>
        <translation>Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1454" />
        <source>Estimate: no run at this level in the last %1 days, figures from all levels of the job.</source>
        <translation>Schätzung: kein Lauf auf diesem Level in den letzten %1 Tagen, Werte aus allen Levels des Jobs.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1460" />
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1463" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1472" />
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1474" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1478" />
        <source>Priority</source>
        <translation>Priorität</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1508" />
        <source>job(s) running</source>
        <translation>Job(s) laufen</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1512" />
        <source>%n run(s) predicted to wait</source>
        <translation>
            <numerusform>%n Lauf wird voraussichtlich warten</numerusform>
            <numerusform>%n Läufe werden voraussichtlich warten</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1517" />
        <source>%1 run(s) without history start here (not counted: duration unknown)</source>
        <translation>%1 Lauf/Läufe ohne Historie starten hier (nicht gezählt: Dauer unbekannt)</translation>
    </message>
</context>
<context>
    <name>BScheduleGridEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="29" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="76" />
        <source>Undone: %1</source>
        <translation>Rückgängig gemacht: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="40" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="85" />
        <source>Redone: %1</source>
        <translation>Wiederhergestellt: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="203" />
        <source>Eraser</source>
        <translation>Radierer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="256" />
        <source>This schedule contains a Run line that is not understood. It is locked: nothing in it can be changed here.</source>
        <translation>Dieser Schedule enthält eine Run-Zeile, die nicht verstanden wird. Er ist gesperrt: Hier kann nichts daran geändert werden.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="283" />
        <source>%n cell(s)</source>
        <translation>
            <numerusform>%n Zelle</numerusform>
            <numerusform>%n Zellen</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="285" />
        <source>%1 of the scope %2</source>
        <translation>%1 des Bereichs %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="297" />
        <source>Paint %1 on %2</source>
        <translation>%1 auf %2 malen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="306" />
        <source>Add %1 to %2</source>
        <translation>%1 zu %2 hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="314" />
        <source>Erase %1</source>
        <translation>%1 radieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="322" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="348" />
        <source>No cell is selected.</source>
        <translation>Keine Zelle ausgewählt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="330" />
        <source>No brush is chosen.</source>
        <translation>Kein Pinsel gewählt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="374" />
        <source>A scope needs at least one month, and a week of the month or "last".</source>
        <translation>Ein Bereich benötigt mindestens einen Monat und eine Woche des Monats oder "letzte".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="384" />
        <source>Add the scope %1</source>
        <translation>Bereich %1 hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="399" />
        <source>The scope "%1" is always there.</source>
        <translation>Der Bereich "%1" ist immer vorhanden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="404" />
        <source>The scope %1 still has Run lines. Erase its cells first.</source>
        <translation>Der Bereich %1 hat noch Run-Zeilen. Radieren Sie zuerst seine Zellen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="410" />
        <source>Remove the scope %1</source>
        <translation>Bereich %1 entfernen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="420" />
        <source>Add a calendar rule</source>
        <translation>Kalenderregel hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="431" />
        <source>Change the calendar rule %1</source>
        <translation>Kalenderregel %1 ändern</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="443" />
        <source>Remove the calendar rule %1</source>
        <translation>Kalenderregel %1 entfernen</translation>
    </message>
</context>
<context>
    <name>BScheduleGridModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="715" />
        <source>Run line %1 ("%2") is not understood: %3</source>
        <translation>Run-Zeile %1 ("%2") wird nicht verstanden: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="724" />
        <source>Run line %1: %2</source>
        <translation>Run-Zeile %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1177" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1226" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1251" />
        <source>There is no Run line %1.</source>
        <translation>Es gibt keine Run-Zeile %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1179" />
        <source>Run line %1 is not a line of the grid and is not changed here.</source>
        <translation>Run-Zeile %1 ist keine Zeile des Rasters und wird hier nicht geändert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1207" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1230" />
        <source>A run without days of the month and without weeks of the year is not a calendar rule; it is painted in the grid.</source>
        <translation>Ein Lauf ohne Tage des Monats und ohne Wochen des Jahres ist keine Kalenderregel; er wird im Raster gezeichnet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1228" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1253" />
        <source>Run line %1 is not a calendar rule.</source>
        <translation>Run-Zeile %1 ist keine Kalenderregel.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1288" />
        <source>The generated Run line "%1" does not say what was intended.</source>
        <translation>Die erzeugte Run-Zeile "%1" sagt nicht das Beabsichtigte aus.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridText</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="74" />
        <source>Monday</source>
        <translation>Montag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="75" />
        <source>Tuesday</source>
        <translation>Dienstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="76" />
        <source>Wednesday</source>
        <translation>Mittwoch</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="77" />
        <source>Thursday</source>
        <translation>Donnerstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="78" />
        <source>Friday</source>
        <translation>Freitag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="79" />
        <source>Saturday</source>
        <translation>Samstag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="80" />
        <source>Sunday</source>
        <translation>Sonntag</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="88" />
        <source>Mon</source>
        <translation>Mo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="89" />
        <source>Tue</source>
        <translation>Di</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="90" />
        <source>Wed</source>
        <translation>Mi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="91" />
        <source>Thu</source>
        <translation>Do</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="92" />
        <source>Fri</source>
        <translation>Fr</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="93" />
        <source>Sat</source>
        <translation>Sa</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="94" />
        <source>Sun</source>
        <translation>So</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="102" />
        <source>January</source>
        <translation>Januar</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="103" />
        <source>February</source>
        <translation>Februar</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="104" />
        <source>March</source>
        <translation>März</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="105" />
        <source>April</source>
        <translation>April</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="106" />
        <source>May</source>
        <translation>Mai</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="107" />
        <source>June</source>
        <translation>Juni</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="108" />
        <source>July</source>
        <translation>Juli</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="109" />
        <source>August</source>
        <translation>August</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="110" />
        <source>September</source>
        <translation>September</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="111" />
        <source>October</source>
        <translation>Oktober</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="112" />
        <source>November</source>
        <translation>November</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="113" />
        <source>December</source>
        <translation>Dezember</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="122" />
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="128" />
        <source>%1th</source>
        <translation>%1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="124" />
        <source>%1st</source>
        <translation>%1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="125" />
        <source>%1nd</source>
        <translation>%1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="126" />
        <source>%1rd</source>
        <translation>%1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="142" />
        <source>Every week</source>
        <translation>Jede Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="157" />
        <source>all</source>
        <translation>alle</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="187" />
        <source>last week only</source>
        <translation>nur letzte Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="190" />
        <source>first week only</source>
        <translation>nur erste Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="191" />
        <source>second week only</source>
        <translation>nur zweite Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="192" />
        <source>third week only</source>
        <translation>nur dritte Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="193" />
        <source>fourth week only</source>
        <translation>nur vierte Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="194" />
        <source>fifth week only</source>
        <translation>nur fünfte Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="199" />
        <source>only %1</source>
        <translation>nur %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="207" />
        <source>level of the job</source>
        <translation>Level des Jobs</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="250" />
        <source>pool %1</source>
        <translation>Pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="251" />
        <source>full pool %1</source>
        <translation>Full-Pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="252" />
        <source>incremental pool %1</source>
        <translation>Incremental-Pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="253" />
        <source>differential pool %1</source>
        <translation>Differential-Pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="254" />
        <source>next pool %1</source>
        <translation>Next Pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="255" />
        <source>storage %1</source>
        <translation>Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="256" />
        <source>messages %1</source>
        <translation>Messages %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="257" />
        <source>priority %1</source>
        <translation>Priorität %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258" />
        <source>spool data yes</source>
        <translation>Daten spoolen: ja</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258" />
        <source>spool data no</source>
        <translation>Daten spoolen: nein</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259" />
        <source>accurate yes</source>
        <translation>Accurate: ja</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259" />
        <source>accurate no</source>
        <translation>Accurate: nein</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="260" />
        <source>max run sched time %1</source>
        <translation>Max Run Sched Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="266" />
        <source>%1 at :%2</source>
        <translation>%1 um :%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="276" />
        <source>%1 at %2</source>
        <translation>%1 um %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="308" />
        <source>%1 %2 hours</source>
        <translation>%1 %2 Uhr</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="316" />
        <source>%1: no run</source>
        <translation>%1: kein Lauf</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="325" />
        <source>the Run line names no time</source>
        <translation>die Run-Zeile nennt keine Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="338" />
        <source>no run in this scope</source>
        <translation>kein Lauf in diesem Bereich</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="342" />
        <source>in other scopes: %1</source>
        <translation>in anderen Bereichen: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="369" />
        <source>and</source>
        <translation>und</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="370" />
        <source>or</source>
        <translation>oder</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="378" />
        <source>%1 to %2</source>
        <translation>%1 bis %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="400" />
        <source>on every %1 day (starting with the %2)</source>
        <translation>an jedem %1 Tag (beginnend mit dem %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="402" />
        <source>on the %1</source>
        <translation>am %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="405" />
        <source>%1 of every month</source>
        <translation>%1 jedes Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="406" />
        <source>%1 of %2</source>
        <translation>%1 im %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="408" />
        <source>in %1</source>
        <translation>im %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="417" />
        <source>in every %1 week of the year (starting with week %2)</source>
        <translation>in jeder %1 Woche des Jahres (beginnend mit Woche %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="425" />
        <source>in week %1 of the year</source>
        <translation>in Woche %1 des Jahres</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="426" />
        <source>in weeks %1 of the year</source>
        <translation>in den Wochen %1 des Jahres</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="433" />
        <source>in the last seven days of the month</source>
        <translation>in den letzten sieben Tagen des Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="435" />
        <source>in the %1 week of the month</source>
        <translation>in der %1 Woche des Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="438" />
        <source>or in its last seven days</source>
        <translation>oder in seinen letzten sieben Tagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="452" />
        <source>only if it is a %1</source>
        <translation>nur wenn es ein %1 ist</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="454" />
        <source>on %1</source>
        <translation>am %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="462" />
        <source>every hour at :00 (the line names no time)</source>
        <translation>jede Stunde um :00 (die Zeile nennt keine Uhrzeit)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="464" />
        <source>every hour at :%1</source>
        <translation>jede Stunde um :%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="472" />
        <source>at %1</source>
        <translation>um %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridView</name>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="255" />
        <source>Undo: %1 (Ctrl+Z)</source>
        <translation>Rückgängig: %1 (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="256" />
        <source>Nothing to undo (Ctrl+Z)</source>
        <translation>Nichts rückgängig zu machen (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="257" />
        <source>Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Wiederherstellen: %1 (Ctrl+Shift+Z oder Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="258" />
        <source>Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Nichts wiederherzustellen (Ctrl+Shift+Z oder Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="293" />
        <source>No schedule chosen. Select a schedule in the list on the left to see it as a grid of weekdays and hours.</source>
        <translation>Kein Schedule gewählt. Wählen Sie links in der Liste einen Schedule, um ihn als Raster aus Wochentagen und Stunden zu sehen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="334" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="335" />
        <source>Undo</source>
        <translation>Rückgängig</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="337" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="338" />
        <source>Redo</source>
        <translation>Wiederherstellen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="340" />
        <source>Revert all</source>
        <translation>Alles zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="341" />
        <source>Undoes every change of this schedule. Redo brings them back.</source>
        <translation>Macht jede Änderung dieses Schedules rückgängig. Wiederherstellen bringt sie zurück.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="343" />
        <source>&amp;Save...</source>
        <translation>&amp;Speichern...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="345" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="788" />
        <source>Writes the changed Run lines to the Director. The Director checks them first, and you see old and new lines and the jobs that use the schedule before anything is written.</source>
        <translation>Schreibt die geänderten Run-Zeilen auf den Director. Der Director prüft sie zuerst, und Sie sehen alte und neue Zeilen sowie die Jobs, die den Schedule verwenden, bevor etwas geschrieben wird.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="350" />
        <source>Delete...</source>
        <translation>Löschen...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="351" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="792" />
        <source>Deletes this schedule on the Director, after a check that no job uses it and your confirmation</source>
        <translation>Löscht diesen Schedule auf dem Director, nach einer Prüfung, dass kein Job ihn verwendet, und Ihrer Bestätigung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="366" />
        <source>No description</source>
        <translation>Keine Beschreibung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="367" />
        <source>The Description of the schedule. Saved with the Run lines; empty removes it from the file.</source>
        <translation>Die Description des Schedules. Wird mit den Run-Zeilen gespeichert; leer entfernt sie aus der Datei.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="399" />
        <source>&amp;Keep my changes</source>
        <translation>Meine Änderungen &amp;behalten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="400" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="861" />
        <source>&amp;Discard and reload</source>
        <translation>&amp;Verwerfen und neu laden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="410" />
        <source>Scopes</source>
        <translation>Bereiche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="414" />
        <source>Scope:</source>
        <translation>Bereich:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="415" />
        <source>The weeks of the month and the months a Run line is restricted to. The grid shows the runs of the chosen scope; runs of the other scopes are hatched.</source>
        <translation>Die Wochen des Monats und die Monate, auf die eine Run-Zeile beschränkt ist. Das Raster zeigt die Läufe des gewählten Bereichs; Läufe der anderen Bereiche sind schraffiert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="422" />
        <source>Add scope</source>
        <translation>Bereich hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="423" />
        <source>Add scope: weeks of the month, and months, to paint a grid of their own for</source>
        <translation>Bereich hinzufügen: Wochen des Monats und Monate, für die ein eigenes Raster gezeichnet wird</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="448" />
        <source>Legend</source>
        <translation>Legende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="469" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="473" />
        <source>Run lines</source>
        <translation>Run-Zeilen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <source>No.</source>
        <translation>Nr.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Run line</source>
        <translation>Run-Zeile</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <source>Shown in</source>
        <translation>Angezeigt in</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="479" />
        <source>The Run lines of the schedule in their order. Choosing a line outlines its cells in the grid and switches to its scope.</source>
        <translation>Die Run-Zeilen des Schedules in ihrer Reihenfolge. Die Wahl einer Zeile umrandet ihre Zellen im Raster und wechselt zu ihrem Bereich.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="495" />
        <source>&amp;Add rule...</source>
        <translation>Regel &amp;hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="496" />
        <source>Add calendar rule</source>
        <translation>Kalenderregel hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="497" />
        <source>Adds a calendar rule: a Run line by day of the month or by week of the year, such as "on 1 at 21:00"</source>
        <translation>Fügt eine Kalenderregel hinzu: eine Run-Zeile nach Tag des Monats oder Woche des Jahres, etwa "on 1 at 21:00"</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499" />
        <source>&amp;Edit rule...</source>
        <translation>Regel &amp;bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="500" />
        <source>Edit calendar rule</source>
        <translation>Kalenderregel bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="501" />
        <source>Opens the form of the calendar rule that is chosen in the list</source>
        <translation>Öffnet das Formular der in der Liste gewählten Kalenderregel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="502" />
        <source>&amp;Remove rule</source>
        <translation>Regel &amp;entfernen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="503" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="1360" />
        <source>Remove calendar rule</source>
        <translation>Kalenderregel entfernen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="504" />
        <source>Removes the calendar rule that is chosen in the list</source>
        <translation>Entfernt die in der Liste gewählte Kalenderregel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="514" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="518" />
        <source>Calendar rules</source>
        <translation>Kalenderregeln</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Mark</source>
        <translation>Markierung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Runs</source>
        <translation>Läufe</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="524" />
        <source>Run lines by day of the month or week of the year. They do not belong to a weekday and are marked in the band below the grid. Enter or a double click opens the form of a rule, Del removes it.</source>
        <translation>Run-Zeilen nach Tag des Monats oder Woche des Jahres. Sie gehören zu keinem Wochentag und werden im Band unter dem Raster markiert. Enter oder ein Doppelklick öffnet das Formular einer Regel, Entf entfernt sie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="542" />
        <source>Next five weeks</source>
        <translation>Nächste fünf Wochen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="548" />
        <source>The levels that run on each date</source>
        <translation>Die Levels, die an jedem Datum laufen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="553" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="554" />
        <source>Previous month</source>
        <translation>Vorheriger Monat</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="557" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="558" />
        <source>Next month</source>
        <translation>Nächster Monat</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="560" />
        <source>Today</source>
        <translation>Heute</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="561" />
        <source>Back to the current week</source>
        <translation>Zurück zur aktuellen Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="721" />
        <source>Modified: the description, not written to the Director.</source>
        <translation>Geändert: die Beschreibung, nicht an den Director geschrieben.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="736" />
        <source>%n changed</source>
        <translation>
            <numerusform>%n geändert</numerusform>
            <numerusform>%n geändert</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="738" />
        <source>%n new</source>
        <translation>
            <numerusform>%n neu</numerusform>
            <numerusform>%n neu</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="740" />
        <source>%n removed</source>
        <translation>
            <numerusform>%n entfernt</numerusform>
            <numerusform>%n entfernt</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="742" />
        <source>Modified: %1 Run line(s) and the description, not written to the Director.</source>
        <translation>Geändert: %1 Run-Zeile(n) und die Beschreibung, nicht an den Director geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="744" />
        <source>Modified: %1 Run line(s), not written to the Director.</source>
        <translation>Geändert: %1 Run-Zeile(n), nicht auf den Director geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="723" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="747" />
        <source>The Director reports other Run lines since.</source>
        <translation>Der Director meldet inzwischen andere Run-Zeilen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="362" />
        <source>Descr&amp;iption:</source>
        <translation>Beschre&amp;ibung:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="790" />
        <source>This connection cannot change a schedule; the click says why.</source>
        <translation>Diese Verbindung kann keinen Schedule ändern; ein Klick zeigt den Grund.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="794" />
        <source>This connection cannot delete a schedule; the click says why.</source>
        <translation>Diese Verbindung kann keinen Schedule löschen; ein Klick zeigt den Grund.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="858" />
        <source>The Director now reports other Run lines for this schedule than the ones your changes started from. Your changes are kept and nothing was replaced.</source>
        <translation>Der Director meldet für diesen Schedule jetzt andere Run-Zeilen als die, von denen Ihre Änderungen ausgingen. Ihre Änderungen bleiben erhalten, und es wurde nichts ersetzt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="863" />
        <source>The schedule %1 has changes that are not written. Showing %2 discards them.</source>
        <translation>Der Schedule %1 hat Änderungen, die nicht geschrieben sind. Das Anzeigen von %2 verwirft sie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="865" />
        <source>&amp;Discard and show %1</source>
        <translation>&amp;Verwerfen und %1 anzeigen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="892" />
        <source>Schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="897" />
        <source>(jobs not loaded yet)</source>
        <translation>(Jobs noch nicht geladen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="899" />
        <source>Not used by any job</source>
        <translation>Von keinem Job verwendet</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="901" />
        <source>Used by %n job(s)</source>
        <translation>
            <numerusform>Von %n Job verwendet</numerusform>
            <numerusform>Von %n Jobs verwendet</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="907" />
        <source>Disabled: the Director does not run it.</source>
        <translation>Deaktiviert: Der Director führt ihn nicht aus.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="912" />
        <source>Contains %n Run line(s) that are not understood. The schedule is locked: it cannot be edited or written here. Change it in its configuration file on the Director.</source>
        <translation>
            <numerusform>Enthält %n Run-Zeile, die nicht verstanden wird. Der Schedule ist gesperrt: Er kann hier weder bearbeitet noch geschrieben werden. Ändern Sie ihn in seiner Konfigurationsdatei auf dem Director.</numerusform>
            <numerusform>Enthält %n Run-Zeilen, die nicht verstanden werden. Der Schedule ist gesperrt: Er kann hier weder bearbeitet noch geschrieben werden. Ändern Sie ihn in seiner Konfigurationsdatei auf dem Director.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="941" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="1164" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="944" />
        <source>%1: no Run line in this scope. Del or the context menu removes it.</source>
        <translation>%1: keine Run-Zeile in diesem Bereich. Entf oder das Kontextmenü entfernt ihn.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="945" />
        <source>%1: no Run line in this scope</source>
        <translation>%1: keine Run-Zeile in diesem Bereich</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="946" />
        <source>%1: %n Run line(s) in this scope</source>
        <translation>
            <numerusform>%1: %n Run-Zeile in diesem Bereich</numerusform>
            <numerusform>%1: %n Run-Zeilen in diesem Bereich</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="952" />
        <source>Scope %1, %n Run line(s)</source>
        <translation>
            <numerusform>Bereich %1, %n Run-Zeile</numerusform>
            <numerusform>Bereich %1, %n Run-Zeilen</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="989" />
        <source>other scope, may run on the same day</source>
        <translation>anderer Bereich, kann am selben Tag laufen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="993" />
        <source>other scope, never on the same day</source>
        <translation>anderer Bereich, nie am selben Tag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1007" />
        <source>no time in the Run line: runs every hour at :00</source>
        <translation>keine Uhrzeit in der Run-Zeile: läuft jede Stunde um :00</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1015" />
        <source>calendar rule, not a cell</source>
        <translation>Kalenderregel, keine Zelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1045" />
        <source>changed</source>
        <translation>geändert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1048" />
        <source>new</source>
        <translation>neu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1051" />
        <source>removed</source>
        <translation>entfernt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1063" />
        <source>Reported by the Director as "%1", a spelling it does not read back.</source>
        <translation>Vom Director als "%1" gemeldet, eine Schreibweise, die er nicht zurückliest.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1070" />
        <source>not understood: %1</source>
        <translation>nicht verstanden: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1071" />
        <source>Not understood and not shown in the grid: %1</source>
        <translation>Nicht verstanden und nicht im Raster angezeigt: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1072" />
        <source>This schedule cannot be written by the application.</source>
        <translation>Dieser Schedule kann von der Anwendung nicht geschrieben werden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1081" />
        <source>calendar rule %1</source>
        <translation>Kalenderregel %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1086" />
        <source>scope %1</source>
        <translation>Bereich %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1108" />
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1137" />
        <source>Nothing selected. Click a cell or drag over cells; click a day or an hour to select all of it.</source>
        <translation>Nichts ausgewählt. Klicken Sie auf eine Zelle oder ziehen Sie über Zellen; klicken Sie auf einen Tag oder eine Stunde, um sie ganz auszuwählen.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1172" />
        <source>%1 in %n cell(s)</source>
        <translation>
            <numerusform>%1 in %n Zelle</numerusform>
            <numerusform>%1 in %n Zellen</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1176" />
        <source>%n cell(s) selected:</source>
        <translation>
            <numerusform>%n Zelle ausgewählt:</numerusform>
            <numerusform>%n Zellen ausgewählt:</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1177" />
        <source>no run in this scope</source>
        <translation>kein Lauf in diesem Bereich</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1180" />
        <source>Other scopes: %1</source>
        <translation>Andere Bereiche: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1181" />
        <source>They stay when this scope is painted or erased</source>
        <translation>Sie bleiben erhalten, wenn in diesem Bereich gemalt oder radiert wird</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1286" />
        <source>Remove the scope "%1"</source>
        <translation>Bereich "%1" entfernen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1289" />
        <source>Add scope...</source>
        <translation>Bereich hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1328" />
        <source>Calendar rule %1</source>
        <translation>Kalenderregel %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1331" />
        <source>New calendar rule</source>
        <translation>Neue Kalenderregel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1359" />
        <source>Edit calendar rule...</source>
        <translation>Kalenderregel bearbeiten...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1364" />
        <source>Add calendar rule...</source>
        <translation>Kalenderregel hinzufügen...</translation>
    </message>
</context>
<context>
    <name>BScheduleGridWidget</name>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="68" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="412" />
        <source>%1 hours</source>
        <translation>%1 Uhr</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="69" />
        <source>Select all</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="73" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="199" />
        <source>Selects the whole day</source>
        <translation>Wählt den ganzen Tag aus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="75" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="201" />
        <source>Selects this hour on every day</source>
        <translation>Wählt diese Stunde an jedem Tag aus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="77" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="202" />
        <source>Selects all days and hours</source>
        <translation>Wählt alle Tage und Stunden aus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="187" />
        <source>Apply the tool</source>
        <translation>Werkzeug anwenden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="189" />
        <source>Erase</source>
        <translation>Radieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="197" />
        <source>Selects this cell</source>
        <translation>Wählt diese Zelle aus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="205" />
        <source>Lists the runs of this cell</source>
        <translation>Listet die Läufe dieser Zelle auf</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="207" />
        <source>Applies the tool in hand (%1) to this cell</source>
        <translation>Wendet das aktuelle Werkzeug (%1) auf diese Zelle an</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="211" />
        <source>Removes the runs of the scope on display from this cell</source>
        <translation>Entfernt die Läufe des angezeigten Bereichs aus dieser Zelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="310" />
        <source>Schedule grid of %1, scope %2</source>
        <translation>Schedule-Raster von %1, Bereich %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="314" />
        <source>Weekdays by hours. Arrow keys move, Shift extends the selection, Enter lists the runs of a cell.</source>
        <translation>Wochentage nach Stunden. Pfeiltasten bewegen, Shift erweitert die Auswahl, Enter listet die Läufe einer Zelle auf.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="317" />
        <source>Space applies the tool to the selected cells, Delete erases them, F, D, I and V paint a level.</source>
        <translation>Leertaste wendet das Werkzeug auf die ausgewählten Zellen an, Entf radiert sie, F, D, I und V malen ein Level.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="319" />
        <source>Tool: %1.</source>
        <translation>Werkzeug: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="321" />
        <source>Paint mode is on.</source>
        <translation>Malmodus ist an.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="325" />
        <source>Calendar rule %1: %2, %3</source>
        <translation>Kalenderregel %1: %2, %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="59" />
        <source>Schedule grid</source>
        <translation>Schedule-Raster</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="281" />
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="631" />
        <source>Calendar rules</source>
        <translation>Kalenderregeln</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1239" />
        <source>%1, %2 to %3</source>
        <translation>%1, %2 bis %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1249" />
        <source>the Run line names no time</source>
        <translation>die Run-Zeile nennt keine Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1252" />
        <source>Run line %1: %2</source>
        <translation>Run-Zeile %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1259" />
        <source>No run in the scope "%1"</source>
        <translation>Kein Lauf im Bereich "%1"</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1266" />
        <source>Other scopes:</source>
        <translation>Andere Bereiche:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1270" />
        <source>%1: may run on the same day as this scope</source>
        <translation>%1: kann am selben Tag wie dieser Bereich laufen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1271" />
        <source>%1: never on the same day as this scope</source>
        <translation>%1: nie am selben Tag wie dieser Bereich</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1277" />
        <source>Painting or erasing here changes only the scope "%1"; the runs of other scopes stay.</source>
        <translation>Malen oder Radieren ändert hier nur den Bereich "%1"; die Läufe anderer Bereiche bleiben erhalten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1284" />
        <source>Expected load: %1 runs at the same time</source>
        <translation>Erwartete Last: %1 Läufe gleichzeitig</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1299" />
        <source>%1: click to select the whole day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1: Klicken, um den ganzen Tag auszuwählen. Ctrl fügt ihn hinzu oder entfernt ihn, Shift erweitert die Auswahl von ihrem Anker aus.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1303" />
        <source>%1 to %2: click to select this hour on every day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1 bis %2: Klicken, um diese Stunde an jedem Tag auszuwählen. Ctrl fügt sie hinzu oder entfernt sie, Shift erweitert die Auswahl von ihrem Anker aus.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1308" />
        <source>Click to select all days and hours (Ctrl+A)</source>
        <translation>Klicken, um alle Tage und Stunden auszuwählen (Ctrl+A)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1310" />
        <source>Run lines by day of the month or week of the year do not belong to a weekday. They are marked here at their hours and listed under "Calendar rules"; this band cannot be selected or painted. A double click on a mark opens the form of its rule.</source>
        <translation>Run-Zeilen nach Tag des Monats oder Woche des Jahres gehören zu keinem Wochentag. Sie werden hier zu ihren Stunden markiert und unter "Kalenderregeln" aufgeführt; dieses Band kann nicht ausgewählt oder bemalt werden. Ein Doppelklick auf eine Markierung öffnet das Formular ihrer Regel.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1391" />
        <source>%1, %2 to %3: %n run(s)</source>
        <translation>
            <numerusform>%1, %2 bis %3: %n Lauf</numerusform>
            <numerusform>%1, %2 bis %3: %n Läufe</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1399" />
        <source>Runs of the cell</source>
        <translation>Läufe der Zelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Time</source>
        <translation>Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Overrides</source>
        <translation>Überschreibungen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Scope</source>
        <translation>Bereich</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Run line</source>
        <translation>Run-Zeile</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1417" />
        <source>no time in the Run line</source>
        <translation>keine Uhrzeit in der Run-Zeile</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1423" />
        <source>%1 (other scope, may run on the same day)</source>
        <translation>%1 (anderer Bereich, kann am selben Tag laufen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1424" />
        <source>%1 (other scope, never on the same day)</source>
        <translation>%1 (anderer Bereich, nie am selben Tag)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1431" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1443" />
        <source>Enter or a double click shows the Run line in the list. Esc closes.</source>
        <translation>Enter oder ein Doppelklick zeigt die Run-Zeile in der Liste. Esc schließt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1544" />
        <source>Tool: %1</source>
        <translation>Werkzeug: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleLegend</name>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="40" />
        <source>Full backup. The length of the bar is the median duration of the job's measured runs at this level.</source>
        <translation>Full-Backup. Die Länge des Balkens ist die mediane Dauer der gemessenen Läufe des Jobs auf diesem Level.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="42" />
        <source>Differential backup.</source>
        <translation>Differential-Backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="44" />
        <source>Incremental backup.</source>
        <translation>Incremental-Backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="46" />
        <source>Virtual full backup.</source>
        <translation>VirtualFull-Backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="48" />
        <source>Pale and dashed, "~" in front of the letter: no run at this level was measured; the length is estimated from the job's runs at other levels.</source>
        <translation>Blass und gestrichelt, "~" vor dem Buchstaben: Auf diesem Level wurde kein Lauf gemessen; die Länge ist aus den Läufen des Jobs auf anderen Levels geschätzt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="51" />
        <source>Dashed outline with "?": the job never ran, so there is no duration. Only the start is shown.</source>
        <translation>Gestrichelter Umriss mit "?": Der Job ist nie gelaufen, daher gibt es keine Dauer. Nur der Start wird angezeigt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="53" />
        <source>Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).</source>
        <translation>Hellere Verlängerung mit einem Strich am Ende: Neun von zehn Läufen waren bis dahin beendet (90. Perzentil).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="55" />
        <source>Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.</source>
        <translation>Gefüllte Sanduhr: Der Lauf wird warten. Die farbige Linie dahinter ist die geplante Zeit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="57" />
        <source>Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).</source>
        <translation>Sanduhr-Umriss: Der Lauf wartet möglicherweise, die Vorhersage ist nicht sicher (siehe die Liste der erwarteten Wartezeiten).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="59" />
        <source>Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.</source>
        <translation>Schraffierter Streifen: die Wartezeit, von der geplanten Zeit bis zum erwarteten Start. Ein gestrichelter Umriss: nicht sicher.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="61" />
        <source>"?" in front of a bar: the prediction for this run is not certain, it may have to wait.</source>
        <translation>"?" vor einem Balken: Die Vorhersage für diesen Lauf ist nicht sicher, er muss möglicherweise warten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="63" />
        <source>"»" at the right edge: the run starts after the last day on display.</source>
        <translation>"»" am rechten Rand: Der Lauf startet nach dem letzten angezeigten Tag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="65" />
        <source>Open diamond: no job uses this schedule. It marks where a run would start.</source>
        <translation>Offene Raute: Kein Job verwendet diesen Schedule. Sie markiert, wo ein Lauf starten würde.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="67" />
        <source>Arrow (day view only): the run at the head waits for the run at the tail, from the moment that one lets it go. The label names the limit; dotted: priority; dashed: only if something unknown turns out against the run. Point at a run to see its arrows.</source>
        <translation>Pfeil (nur Tagesansicht): Der Lauf an der Spitze wartet auf den Lauf am Ende, ab dem Moment, in dem dieser ihn freigibt. Die Beschriftung nennt die Grenze; gepunktet: Priorität; gestrichelt: nur falls etwas Unbekanntes ungünstig für den Lauf ausgeht. Zeigen Sie auf einen Lauf, um seine Pfeile zu sehen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="71" />
        <source>Red line: now.</source>
        <translation>Rote Linie: jetzt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="259" />
        <source>&lt;b&gt;What the marks mean&lt;/b&gt;</source>
        <translation>&lt;b&gt;Was die Markierungen bedeuten&lt;/b&gt;</translation>
    </message>
</context>
<context>
    <name>BScheduleMiniCalendar</name>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="85" />
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="110" />
        <source>Calendar of runs, %1</source>
        <translation>Kalender der Läufe, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="116" />
        <source>%1 to %2</source>
        <translation>%1 bis %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="125" />
        <source>%1: no run</source>
        <translation>%1: kein Lauf</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="128" />
        <source>%1 %2 (Run line %3)</source>
        <translation>%1 %2 (Run-Zeile %3)</translation>
    </message>
</context>
<context>
    <name>BSchedulePalette</name>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="124" />
        <source>Brushes</source>
        <translation>Pinsel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="136" />
        <location filename="../src/schedules/bschedulepalette.cpp" line="137" />
        <source>Eraser</source>
        <translation>Radierer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="138" />
        <source>Removes the runs of the scope on display from cells. Del erases the selected cells with any tool in hand.</source>
        <translation>Entfernt die Läufe des angezeigten Bereichs aus Zellen. Entf radiert die ausgewählten Zellen mit jedem Werkzeug.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="141" />
        <source>New brush...</source>
        <translation>Neuer Pinsel...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="143" />
        <source>Defines a brush: level, minute of the hour and overrides</source>
        <translation>Definiert einen Pinsel: Level, Minute der Stunde und Überschreibungen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="145" />
        <source>Paint mode</source>
        <translation>Malmodus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="147" />
        <source>On: pressing a cell and dragging over cells applies the tool at once. Off: the mouse selects, and Space applies the tool to the selection. The key P switches it in the grid.</source>
        <translation>An: Drücken auf eine Zelle und Ziehen über Zellen wendet das Werkzeug sofort an. Aus: Die Maus wählt aus, und die Leertaste wendet das Werkzeug auf die Auswahl an. Die Taste P schaltet ihn im Raster um.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="151" />
        <source>Keep other runs</source>
        <translation>Andere Läufe behalten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="153" />
        <source>On: painting adds the brush to the runs a cell already has, so a cell can have several runs in one hour. Off: the cell ends up with the brush only. Shift+Space adds once without this switch.</source>
        <translation>An: Malen fügt den Pinsel zu den Läufen hinzu, die eine Zelle bereits hat, sodass eine Zelle mehrere Läufe in einer Stunde haben kann. Aus: Die Zelle enthält danach nur den Pinsel. Shift+Leertaste fügt einmalig hinzu, ohne diesen Schalter.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="198" />
        <source>%1 = %2</source>
        <translation>%1 = %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="246" />
        <source>Brush %1</source>
        <translation>Pinsel %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="247" />
        <source>Takes this brush in hand: %1. Space applies it to the selected cells.</source>
        <translation>Nimmt diesen Pinsel zur Hand: %1. Die Leertaste wendet ihn auf die ausgewählten Zellen an.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="308" />
        <source>In hand: the eraser.</source>
        <translation>Zur Hand: der Radierer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="310" />
        <source>In hand: the brush %1.</source>
        <translation>Zur Hand: der Pinsel %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="314" />
        <source>Paint mode: press a cell or drag over cells to apply it.</source>
        <translation>Malmodus: Drücken Sie auf eine Zelle oder ziehen Sie über Zellen, um es anzuwenden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="316" />
        <source>Space applies it to the selected cells, Del erases them.</source>
        <translation>Die Leertaste wendet es auf die ausgewählten Zellen an, Entf radiert sie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="319" />
        <source>Other runs of a painted cell stay.</source>
        <translation>Andere Läufe einer bemalten Zelle bleiben erhalten.</translation>
    </message>
</context>
<context>
    <name>BSchedulePersistence</name>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="103" />
        <source>The text of schedule %1 arrived changed</source>
        <translation>Der Text von Schedule %1 kam verändert an</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="104" />
        <source>The SHA-256 of the text the Director sent does not match the text. Nothing was written.</source>
        <translation>Der SHA-256 des vom Director gesendeten Textes passt nicht zum Text. Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="117" />
        <source>Schedule %1 has changed on the Director</source>
        <translation>Schedule %1 wurde auf dem Director geändert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="118" />
        <source>The configuration file of the schedule does not say what was loaded here, so the change was not sent. Reload the schedule and make the change again. %1</source>
        <translation>Die Konfigurationsdatei des Schedules entspricht nicht dem, was hier geladen wurde, daher wurde die Änderung nicht gesendet. Laden Sie den Schedule neu und nehmen Sie die Änderung erneut vor. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="146" />
        <source>Schedule %1 cannot be written</source>
        <translation>Schedule %1 kann nicht geschrieben werden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="147" />
        <source>Run line %1 is not understood by this application (%2). A schedule with such a line is not changed here; change it in its configuration file.</source>
        <translation>Run-Zeile %1 wird von dieser Anwendung nicht verstanden (%2). Ein Schedule mit einer solchen Zeile wird hier nicht geändert; ändern Sie ihn in seiner Konfigurationsdatei.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="156" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="189" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="276" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="317" />
        <source>Schedule %1 was not changed</source>
        <translation>Schedule %1 wurde nicht geändert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="157" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="376" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="447" />
        <source>There is no connection to the configuration methods of the Director.</source>
        <translation>Es besteht keine Verbindung zu den Konfigurationsmethoden des Directors.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="190" />
        <source>The change could not be written into the configuration text safely: %1</source>
        <translation>Die Änderung konnte nicht sicher in den Konfigurationstext geschrieben werden: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="198" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="264" />
        <source>Nothing to write</source>
        <translation>Nichts zu schreiben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="199" />
        <source>The Run lines and the description are the ones the Director has.</source>
        <translation>Die Run-Zeilen und die Beschreibung sind die, die der Director hat.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="265" />
        <source>The edited text is the one the Director has.</source>
        <translation>Der bearbeitete Text ist derjenige, den der Director hat.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="277" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="417" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="493" />
        <source>Nothing was written.</source>
        <translation>Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="316" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="446" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="492" />
        <source>Schedule %1 was not created</source>
        <translation>Schedule %1 wurde nicht angelegt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="318" />
        <source>The edited text was not sent to the Director: %1</source>
        <translation>Der bearbeitete Text wurde nicht an den Director gesendet: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="326" />
        <source>the name of the schedule has to stay %1.</source>
        <translation>der Name des Schedules muss %1 bleiben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="338" />
        <source>The text was edited in the review; the Director checked the edited text.</source>
        <translation>Der Text wurde in der Überprüfung bearbeitet; der Director hat den bearbeiteten Text geprüft.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="375" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="416" />
        <source>Schedule %1 was not deleted</source>
        <translation>Schedule %1 wurde nicht gelöscht</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="573" />
        <source>Run line %1 cannot be written: %2</source>
        <translation>Run-Zeile %1 kann nicht geschrieben werden: %2</translation>
    </message>
</context>
<context>
    <name>BSchedulePreviewPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="674" />
        <source>Preview &amp; Deploy</source>
        <translation>Vorschau &amp; Bereitstellung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="675" />
        <source>Review the schedule configuration and deploy to the Director.</source>
        <translation>Prüfen Sie die Schedule-Konfiguration und stellen Sie sie auf dem Director bereit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="680" />
        <source>Configuration:</source>
        <translation>Konfiguration:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="691" />
        <source>Copy Configuration</source>
        <translation>Konfiguration kopieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="698" />
        <source>Director Command:</source>
        <translation>Director-Befehl:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="709" />
        <source>Copy Command</source>
        <translation>Befehl kopieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="756" />
        <source>CreateResource (gRPC) with the configuration text above</source>
        <translation>CreateResource (gRPC) mit dem obigen Konfigurationstext</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="767" />
        <source>This schedule cannot be written. %1</source>
        <translation>Dieser Schedule kann nicht geschrieben werden. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="780" />
        <source>Schedule cannot be written</source>
        <translation>Schedule kann nicht geschrieben werden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="781" />
        <source>%1

Go back and remove or replace this run, or cancel the wizard.</source>
        <translation>%1

Gehen Sie zurück und entfernen oder ersetzen Sie diesen Lauf, oder brechen Sie den Assistenten ab.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="796" />
        <source>Deploy to Director</source>
        <translation>Auf dem Director bereitstellen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="797" />
        <source>Send 'configure add' to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>'configure add' an den Director senden?

Klicken Sie auf Ja zum Ausführen, auf Nein zum Schließen ohne Bereitstellung.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="819" />
        <source>Sending configure command...</source>
        <translation>configure-Befehl wird gesendet...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="846" />
        <source>The Director checks the schedule...</source>
        <translation>Der Director prüft den Schedule...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="853" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="884" />
        <source>Schedule created successfully! Click Finish to close.</source>
        <translation>Schedule erfolgreich angelegt! Klicken Sie auf Fertigstellen zum Schließen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="856" />
        <source>Nothing was written.</source>
        <translation>Es wurde nichts geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="859" />
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="887" />
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="897" />
        <source>The connection to the Director was lost before it answered. Reconnect and check with 'show schedules' whether the schedule exists.</source>
        <translation>Die Verbindung zum Director ging verloren, bevor er antwortete. Verbinden Sie sich erneut und prüfen Sie mit 'show schedules', ob der Schedule existiert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="905" />
        <source>Timeout waiting for Director response. The schedule may have been created — check with 'show schedules'.</source>
        <translation>Zeitüberschreitung beim Warten auf die Antwort des Directors. Der Schedule wurde möglicherweise angelegt — prüfen Sie mit 'show schedules'.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="922" />
        <source>Configuration copied to clipboard.</source>
        <translation>Konfiguration in die Zwischenablage kopiert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="929" />
        <source>Command copied to clipboard.</source>
        <translation>Befehl in die Zwischenablage kopiert.</translation>
    </message>
</context>
<context>
    <name>BScheduleResource</name>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="97" />
        <source>The text does not start with a Schedule resource (line %1).</source>
        <translation>Der Text beginnt nicht mit einer Schedule-Ressource (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="101" />
        <source>Directives on the line of "Schedule {" cannot be edited safely (line %1).</source>
        <translation>Direktiven in der Zeile von "Schedule {" können nicht sicher bearbeitet werden (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="107" />
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Text nach der schließenden Klammer der Ressource (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="111" />
        <source>A nested block in a Schedule resource (line %1).</source>
        <translation>Ein verschachtelter Block in einer Schedule-Ressource (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="114" />
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Die schließende Klammer teilt sich ihre Zeile mit einer Direktive (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="145" />
        <source>A quoted Run value does not end on its line (line %1).</source>
        <translation>Ein Run-Wert in Anführungszeichen endet nicht in seiner Zeile (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="148" />
        <source>Something follows the Run value on its line (line %1).</source>
        <translation>Auf den Run-Wert folgt etwas in seiner Zeile (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="156" />
        <source>A Run directive shares its line with another directive (line %1).</source>
        <translation>Eine Run-Direktive teilt sich ihre Zeile mit einer anderen Direktive (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="163" />
        <source>A Run directive without a value (line %1).</source>
        <translation>Eine Run-Direktive ohne Wert (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="173" />
        <source>A Run directive that does not start its line (line %1).</source>
        <translation>Eine Run-Direktive, die nicht am Zeilenanfang steht (Zeile %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="184" />
        <source>The text contains no Schedule resource.</source>
        <translation>Der Text enthält keine Schedule-Ressource.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="186" />
        <source>The Schedule resource has no closing brace.</source>
        <translation>Der Schedule-Ressource fehlt die schließende Klammer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="217" />
        <source>The configuration file has %1 Run line(s), the schedule as it was loaded %2.</source>
        <translation>Die Konfigurationsdatei hat %1 Run-Zeile(n), der Schedule, wie er geladen wurde, %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="227" />
        <source>Run line %1 differs: the file says "%2", the schedule as it was loaded "%3".</source>
        <translation>Run-Zeile %1 weicht ab: Die Datei sagt "%2", der Schedule, wie er geladen wurde, "%3".</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="249" />
        <source>A new Run line without text.</source>
        <translation>Eine neue Run-Zeile ohne Text.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="253" />
        <source>A Run line refers to line %1, which the file does not have.</source>
        <translation>Eine Run-Zeile verweist auf Zeile %1, die die Datei nicht hat.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="255" />
        <source>The Run lines would change their order; that is not written.</source>
        <translation>Die Run-Zeilen würden ihre Reihenfolge ändern; das wird nicht geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="258" />
        <source>A changed Run line without text.</source>
        <translation>Eine geänderte Run-Zeile ohne Text.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="262" />
        <source>A Run line contains a line break.</source>
        <translation>Eine Run-Zeile enthält einen Zeilenumbruch.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="364" />
        <source>The new text could not be read back: %1</source>
        <translation>Der neue Text konnte nicht zurückgelesen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="369" />
        <source>The new text does not have the intended Run lines.</source>
        <translation>Der neue Text hat nicht die beabsichtigten Run-Zeilen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="371" />
        <source>The new text would change more than the Run lines.</source>
        <translation>Der neue Text würde mehr ändern als die Run-Zeilen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="373" />
        <source>The new text would change the name of the schedule.</source>
        <translation>Der neue Text würde den Namen des Schedules ändern.</translation>
    </message>
</context>
<context>
    <name>BScheduleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408" />
        <source>Source File</source>
        <translation>Quelldatei</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewDialog</name>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="54" />
        <source>(no Run line)</source>
        <translation>(keine Run-Zeile)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="91" />
        <source>changed: </source>
        <translation>geändert: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="92" />
        <source>new: </source>
        <translation>neu: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="93" />
        <source>removed: </source>
        <translation>entfernt: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="108" />
        <source>Change schedule %1</source>
        <translation>Schedule %1 ändern</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="109" />
        <source>Change schedule %1 on the Director?</source>
        <translation>Schedule %1 auf dem Director ändern?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="110" />
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Der Director hat die neue Konfiguration geprüft und akzeptiert sie. Es wird nichts geschrieben, bis Sie "Auf den Director schreiben" wählen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="114" />
        <source>Create schedule %1</source>
        <translation>Schedule %1 anlegen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="115" />
        <source>Create schedule %1 on the Director?</source>
        <translation>Schedule %1 auf dem Director anlegen?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="116" />
        <source>The Director checked the new schedule and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Der Director hat den neuen Schedule geprüft und akzeptiert ihn. Es wird nichts geschrieben, bis Sie "Auf den Director schreiben" wählen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="120" />
        <source>Delete schedule %1</source>
        <translation>Schedule %1 löschen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="121" />
        <source>Delete schedule %1 on the Director?</source>
        <translation>Schedule %1 auf dem Director löschen?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="122" />
        <source>The Director checked that nothing refers to the schedule. Nothing is deleted until you choose "Delete on the Director".</source>
        <translation>Der Director hat geprüft, dass nichts auf den Schedule verweist. Es wird nichts gelöscht, bis Sie "Auf dem Director löschen" wählen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="140" />
        <source>Run lines now (in the file)</source>
        <translation>Run-Zeilen jetzt (in der Datei)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="143" />
        <source>Run lines now</source>
        <translation>Run-Zeilen jetzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="148" />
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="151" />
        <source>Run lines after writing</source>
        <translation>Run-Zeilen nach dem Schreiben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="159" />
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="168" />
        <source>Jobs that use this schedule</source>
        <translation>Jobs, die diesen Schedule verwenden</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="162" />
        <source>The change applies to every one of these %n job(s), not only to the one on display. To move a single job, copy the schedule and assign the copy to that job instead.</source>
        <translation>
            <numerusform>Die Änderung gilt für diesen %n Job, nicht nur für den angezeigten. Um einen einzelnen Job zu verschieben, kopieren Sie den Schedule und weisen Sie stattdessen die Kopie diesem Job zu.</numerusform>
            <numerusform>Die Änderung gilt für jeden dieser %n Jobs, nicht nur für den angezeigten. Um einen einzelnen Job zu verschieben, kopieren Sie den Schedule und weisen Sie stattdessen die Kopie diesem Job zu.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="173" />
        <source>(no job of the Director uses it, as far as this application knows)</source>
        <translation>(kein Job des Directors verwendet ihn, soweit diese Anwendung weiß)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="177" />
        <source>The Director names as referencing it: %1</source>
        <translation>Der Director nennt als verweisend: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="189" />
        <source>Text after writing, editable</source>
        <translation>Text nach dem Schreiben, bearbeitbar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="190" />
        <source>This text can be edited. An edited text is checked by the Director and shown again before anything is written.</source>
        <translation>Dieser Text kann bearbeitet werden. Ein bearbeiteter Text wird vom Director geprüft und erneut angezeigt, bevor etwas geschrieben wird.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="193" />
        <source>Text after writing</source>
        <translation>Text nach dem Schreiben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="196" />
        <source>Text now</source>
        <translation>Text jetzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="200" />
        <source>The text was edited. "Check with the Director" sends it for a check only; the review then shows it again, and only "Write to the Director" there writes it.</source>
        <translation>Der Text wurde bearbeitet. "Beim Director prüfen" sendet ihn nur zur Prüfung; die Überprüfung zeigt ihn dann erneut, und erst "Auf den Director schreiben" dort schreibt ihn.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="210" />
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Datei auf dem Director: %1 (sie enthält auch andere Ressourcen; nur diese Ressource wird ersetzt).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="212" />
        <source>File on the Director: %1.</source>
        <translation>Datei auf dem Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="215" />
        <source>The Director reloads its configuration after writing. Runs it has already queued may still start at the old time.</source>
        <translation>Der Director lädt seine Konfiguration nach dem Schreiben neu. Läufe, die er bereits eingereiht hat, können noch zur alten Zeit starten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="221" />
        <source>&amp;Delete on the Director</source>
        <translation>Auf dem Director &amp;löschen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="222" />
        <source>&amp;Write to the Director</source>
        <translation>Auf den Director &amp;schreiben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="253" />
        <source>&amp;Check with the Director</source>
        <translation>Beim Director &amp;prüfen</translation>
    </message>
</context>
<context>
    <name>BScheduleRunDialog</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="48" />
        <source>Edit Run Directive</source>
        <translation>Run-Direktive bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="56" />
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="61" />
        <source>Time:</source>
        <translation>Uhrzeit:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="65" />
        <source>(every week)</source>
        <translation>(jede Woche)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="66" />
        <source>Week of Month:</source>
        <translation>Woche des Monats:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="71" />
        <source>Days of Week</source>
        <translation>Wochentage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Sun</source>
        <translation>So</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Mon</source>
        <translation>Mo</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Tue</source>
        <translation>Di</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Wed</source>
        <translation>Mi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Thu</source>
        <translation>Do</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Fri</source>
        <translation>Fr</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Sat</source>
        <translation>Sa</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="89" />
        <source>Daily</source>
        <translation>Täglich</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="93" />
        <source>Mon-Fri</source>
        <translation>Mo-Fr</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="98" />
        <source>Clear</source>
        <translation>Leeren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="111" />
        <source>Optional Overrides</source>
        <translation>Optionale Überschreibungen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="117" />
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="122" />
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="127" />
        <source>(default)</source>
        <translation>(Standard)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="128" />
        <source>Priority:</source>
        <translation>Priorität:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="138" />
        <source>Preview:</source>
        <translation>Vorschau:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="159" />
        <source>Validation</source>
        <translation>Prüfung</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="160" />
        <source>Please select at least one day of the week.</source>
        <translation>Bitte wählen Sie mindestens einen Wochentag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="179" />
        <source>(level of the job)</source>
        <translation>(Level des Jobs)</translation>
    </message>
</context>
<context>
    <name>BScheduleRunEditor</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Days</source>
        <translation>Tage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Time</source>
        <translation>Uhrzeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="323" />
        <source>+ Add Run</source>
        <translation>+ Lauf hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="324" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="325" />
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="326" />
        <source>Duplicate</source>
        <translation>Duplizieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="381" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="396" />
        <source>Edit Run Directive</source>
        <translation>Run-Direktive bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="382" />
        <source>This Run line is not understood and cannot be edited:
%1

%2

Remove it to continue.</source>
        <translation>Diese Run-Zeile wird nicht verstanden und kann nicht bearbeitet werden:
%1

%2

Entfernen Sie sie, um fortzufahren.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="397" />
        <source>This Run line uses forms that this dialog cannot show, for example months, days of the month, several times or other overrides:
%1

It is kept exactly as it is. To replace it, remove it and add a new run.</source>
        <translation>Diese Run-Zeile verwendet Formen, die dieser Dialog nicht anzeigen kann, zum Beispiel Monate, Tage des Monats, mehrere Uhrzeiten oder andere Überschreibungen:
%1

Sie bleibt genau so erhalten, wie sie ist. Um sie zu ersetzen, entfernen Sie sie und fügen Sie einen neuen Lauf hinzu.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="453" />
        <source>Not understood: %1</source>
        <translation>Nicht verstanden: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="454" />
        <source>Not understood</source>
        <translation>Nicht verstanden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="469" />
        <source>Kept exactly as written: %1</source>
        <translation>Genau wie geschrieben beibehalten: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="479" />
        <source>(level of the job)</source>
        <translation>(Level des Jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="484" />
        <source>daily</source>
        <translation>täglich</translation>
    </message>
</context>
<context>
    <name>BScheduleRunsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="636" />
        <source>Run Directives</source>
        <translation>Run-Direktiven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="637" />
        <source>Define when this schedule runs backups. Add one or more Run entries.</source>
        <translation>Legen Sie fest, wann dieser Schedule Backups ausführt. Fügen Sie einen oder mehrere Run-Einträge hinzu.</translation>
    </message>
</context>
<context>
    <name>BScheduleScopeDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="379" />
        <source>&amp;last (the last seven days of a month)</source>
        <translation>&amp;letzte (die letzten sieben Tage eines Monats)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="383" />
        <source>Add scope</source>
        <translation>Bereich hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="386" />
        <source>A scope is the part of the calendar a grid is painted for: weeks of the month, and months if not all. What is painted in it becomes Run lines such as "1st sat at 21:00".</source>
        <translation>Ein Bereich ist der Teil des Kalenders, für den ein Raster gezeichnet wird: Wochen des Monats und, falls nicht alle, Monate. Was darin gemalt wird, wird zu Run-Zeilen wie "1st sat at 21:00".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="392" />
        <source>Weeks of the month</source>
        <translation>Wochen des Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="398" />
        <source>Days %1 to %2 of a month</source>
        <translation>Tage %1 bis %2 eines Monats</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="408" />
        <source>Months</source>
        <translation>Monate</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="417" />
        <source>&amp;All months</source>
        <translation>&amp;Alle Monate</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="474" />
        <source>Choose at least one month.</source>
        <translation>Wählen Sie mindestens einen Monat.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="476" />
        <source>Choose at least one week of the month, or "last".</source>
        <translation>Wählen Sie mindestens eine Woche des Monats oder "letzte".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="482" />
        <source>The scope "%1" exists already; it will be shown.</source>
        <translation>Der Bereich "%1" existiert bereits; er wird angezeigt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="483" />
        <source>New scope: %1</source>
        <translation>Neuer Bereich: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleWaitModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1068" />
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1092" />
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1070" />
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1091" />
        <source>%1 min</source>
        <translation>%1 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1075" />
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1090" />
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1113" />
        <source>%1 h</source>
        <translation>%1 h</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1089" />
        <source>%1 d</source>
        <translation>%1 d</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1100" />
        <source>Max Run Time %1</source>
        <translation>Max Run Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1102" />
        <source>Full Max Runtime %1</source>
        <translation>Full Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1104" />
        <source>Incremental Max Runtime %1</source>
        <translation>Incremental Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1106" />
        <source>Differential Max Runtime %1</source>
        <translation>Differential Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1108" />
        <source>Max Run Sched Time %1</source>
        <translation>Max Run Sched Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1113" />
        <source>no Max Run Time set, %1 assumed</source>
        <translation>keine Max Run Time gesetzt, %1 angenommen</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1119" />
        <source>%n run(s) wait, longest %1</source>
        <translation>
            <numerusform>%n Lauf wartet, längster %1</numerusform>
            <numerusform>%n Läufe warten, längster %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1120" />
        <source>No waiting predicted</source>
        <translation>Keine Wartezeit erwartet</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1122" />
        <source>%1 uncertain</source>
        <translation>%1 unsicher</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1131" />
        <source>Director limit: the Director runs %1 job(s) at a time (Maximum Concurrent Jobs = %1) and all of them are taken.</source>
        <translation>Director-Grenze: Der Director führt %1 Job(s) gleichzeitig aus (Maximum Concurrent Jobs = %1), und alle Plätze sind belegt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1133" />
        <source>Director limit: Maximum Concurrent Jobs of the Director is not known. With the Bareos default of 1 every job slot would be taken.</source>
        <translation>Director-Grenze: Maximum Concurrent Jobs des Directors ist nicht bekannt. Mit dem Bareos-Standardwert 1 wäre jeder Job-Platz belegt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1137" />
        <source>Storage limit: storage %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Storage-Grenze: Storage %1 nimmt %2 Job(s) gleichzeitig an (Maximum Concurrent Jobs = %2), und alle Plätze sind belegt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1141" />
        <source>Storage limit: the storage of this job is not known, and so is its Maximum Concurrent Jobs. With one shared storage and the Bareos default of 1 it would be full.</source>
        <translation>Storage-Grenze: Der Storage dieses Jobs ist nicht bekannt und damit auch nicht sein Maximum Concurrent Jobs. Mit einem gemeinsamen Storage und dem Bareos-Standardwert 1 wäre er voll.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1144" />
        <source>Storage limit: Maximum Concurrent Jobs of storage %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Storage-Grenze: Maximum Concurrent Jobs von Storage %1 ist nicht bekannt. Mit dem Bareos-Standardwert 1 wäre er voll.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1148" />
        <source>Client limit: client %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Client-Grenze: Client %1 nimmt %2 Job(s) gleichzeitig an (Maximum Concurrent Jobs = %2), und alle Plätze sind belegt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1151" />
        <source>Client limit: Maximum Concurrent Jobs of client %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Client-Grenze: Maximum Concurrent Jobs von Client %1 ist nicht bekannt. Mit dem Bareos-Standardwert 1 wäre er voll.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1155" />
        <source>Job limit: job %1 may run %2 time(s) at once (Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.</source>
        <translation>Job-Grenze: Job %1 darf %2-mal gleichzeitig laufen (Maximum Concurrent Jobs = %2), und ein früherer Start davon ist noch nicht beendet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1158" />
        <source>Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos default of 1 an earlier start of it would be in the way.</source>
        <translation>Job-Grenze: Maximum Concurrent Jobs von Job %1 ist nicht bekannt. Mit dem Bareos-Standardwert 1 stünde ein früherer Start davon im Weg.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1162" />
        <source>Priority: this run has priority %1 and runs of priority %2 are running. The Director starts a run of a lower priority (a higher number) only when no run of another priority is running.</source>
        <translation>Priorität: Dieser Lauf hat Priorität %1, und Läufe mit Priorität %2 laufen gerade. Der Director startet einen Lauf niedrigerer Priorität (höhere Zahl) erst, wenn kein Lauf einer anderen Priorität läuft.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1167" />
        <source>Priority: this run has priority %1 and runs of priority %2 are running. It starts beside them only if it and all of them have Allow Mixed Priority.</source>
        <translation>Priorität: Dieser Lauf hat Priorität %1, und Läufe mit Priorität %2 laufen gerade. Er startet nur dann neben ihnen, wenn er und alle von ihnen Allow Mixed Priority haben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1171" />
        <source>Queue order: a run of priority %1 is waiting ahead in the queue and cannot start yet. The Director starts no run that is behind it in the queue.</source>
        <translation>Warteschlangenreihenfolge: Ein Lauf mit Priorität %1 wartet weiter vorn in der Warteschlange und kann noch nicht starten. Der Director startet keinen Lauf, der in der Warteschlange dahinter steht.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1195" />
        <source>from %1, duration unknown</source>
        <translation>ab %1, Dauer unbekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1205" />
        <source>%1 to about %2, estimated</source>
        <translation>%1 bis etwa %2, geschätzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1207" />
        <source>%1 to %2</source>
        <translation>%1 bis %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1216" />
        <source>and %1 more</source>
        <translation>und %1 weitere</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1229" />
        <source>%1: it may run until %2 (%3)</source>
        <translation>%1: kann bis %2 laufen (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1230" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1235" />
        <source>Maximum Concurrent Jobs of the Director is not known</source>
        <translation>Maximum Concurrent Jobs des Directors ist nicht bekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1238" />
        <source>the storage of the job is not known (the Job and its JobDefs name none)</source>
        <translation>der Storage des Jobs ist nicht bekannt (der Job und seine JobDefs nennen keinen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1239" />
        <source>Maximum Concurrent Jobs of storage %1 is not known</source>
        <translation>Maximum Concurrent Jobs von Storage %1 ist nicht bekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1241" />
        <source>Maximum Concurrent Jobs of client %1 is not known</source>
        <translation>Maximum Concurrent Jobs von Client %1 ist nicht bekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1243" />
        <source>Maximum Concurrent Jobs of job %1 is not known</source>
        <translation>Maximum Concurrent Jobs von Job %1 ist nicht bekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1245" />
        <source>Allow Mixed Priority of job %1 is not known</source>
        <translation>Allow Mixed Priority von Job %1 ist nicht bekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1247" />
        <source>the duration of %1 was not measured at this level</source>
        <translation>die Dauer von %1 wurde auf diesem Level nicht gemessen</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1249" />
        <source>the duration of %1 was never measured</source>
        <translation>die Dauer von %1 wurde nie gemessen</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1252" />
        <source>the number of parallel writers of the storage device is not known (the job names no storage; the Director cannot read it)</source>
        <translation>die Anzahl paralleler Schreiber des Storage-Geräts ist nicht bekannt (der Job nennt keinen Storage; der Director kann sie nicht lesen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1254" />
        <source>the number of parallel writers of the device of storage %1 is not known (the Director cannot read it; set it in the list of predicted waits)</source>
        <translation>die Anzahl paralleler Schreiber des Geräts von Storage %1 ist nicht bekannt (der Director kann sie nicht lesen; legen Sie sie in der Liste der erwarteten Wartezeiten fest)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1318" />
        <source>Director limit (Maximum Concurrent Jobs = %1)</source>
        <translation>Director-Grenze (Maximum Concurrent Jobs = %1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1321" />
        <source>Storage limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Storage-Grenze (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1325" />
        <source>Client limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Client-Grenze (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1329" />
        <source>Job limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Job-Grenze (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1333" />
        <source>Priority %1, runs of priority %2 are running</source>
        <translation>Priorität %1, Läufe mit Priorität %2 laufen gerade</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1337" />
        <source>Queue order: a run of priority %1 waits ahead</source>
        <translation>Warteschlangenreihenfolge: Ein Lauf mit Priorität %1 wartet weiter vorn</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1344" />
        <source>depends on values that are not known</source>
        <translation>hängt von unbekannten Werten ab</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1347" />
        <source>May wait: %1</source>
        <translation>Wartet möglicherweise: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1349" />
        <source>may be longer: %1</source>
        <translation>kann länger sein: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1371" />
        <source>%1 to %2: %3</source>
        <translation>%1 bis %2: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1377" />
        <source>Waits for: %1</source>
        <translation>Wartet auf: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1382" />
        <source>Predicted start: %1, %2 after the scheduled time (%3)</source>
        <translation>Erwarteter Start: %1, %2 nach der geplanten Zeit (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1391" />
        <source>it depends on values that are not known</source>
        <translation>es hängt von unbekannten Werten ab</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1394" />
        <source>Uncertain, the wait may be longer: %1.</source>
        <translation>Unsicher, die Wartezeit kann länger sein: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1396" />
        <source>Uncertain, this run may have to wait: %1.</source>
        <translation>Unsicher, dieser Lauf muss möglicherweise warten: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1400" />
        <source>If so: %1</source>
        <translation>Falls ja: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1423" />
        <source>Inside the run: waits %1 in the storage daemon for a device of storage %2 (%3 parallel writer(s); after two waits for a release it tries again every %4 s)%5.</source>
        <translation>Innerhalb des Laufs: wartet %1 im Storage Daemon auf ein Gerät von Storage %2 (%3 parallele(r) Schreiber; nach zwei Wartevorgängen auf eine Freigabe versucht er es alle %4 s erneut)%5.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1430" />
        <source>, held by %1</source>
        <translation>, belegt von %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1433" />
        <source>Predicted end: %1</source>
        <translation>Erwartetes Ende: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1436" />
        <source>Start overhead %1 (from the Director starting the job to its recorded start: connections, Client Run Before Job)%2.</source>
        <translation>Startaufwand %1 (vom Start des Jobs durch den Director bis zu seinem aufgezeichneten Start: Verbindungen, Client Run Before Job)%2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1445" />
        <source>Measured wait at this level: %1; predicted %2.</source>
        <translation>Gemessene Wartezeit auf diesem Level: %1; erwartet %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1472" />
        <source>not measured</source>
        <translation>nicht gemessen</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1473" />
        <source>median %1, max %2 (%3 run(s))</source>
        <translation>Median %1, Max. %2 (%3 Lauf/Läufe)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1484" />
        <source>Prediction too low: measured median %1, predicted %2 - a limit the GUI does not know, e.g. the storage device (parallel writers), or a Client Run Before Job that takes long.</source>
        <translation>Vorhersage zu niedrig: gemessener Median %1, erwartet %2 - eine Grenze, die die GUI nicht kennt, z. B. das Storage-Gerät (parallele Schreiber) oder ein lange dauerndes Client Run Before Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1489" />
        <source>Prediction too high: measured median %1, predicted %2 - a limit is set lower here than it is, or the durations used are too long.</source>
        <translation>Vorhersage zu hoch: gemessener Median %1, erwartet %2 - eine Grenze ist hier niedriger gesetzt, als sie ist, oder die verwendeten Dauern sind zu lang.</translation>
    </message>
</context>
<context>
    <name>BScheduleWidget</name>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="125" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="860" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3301" />
        <source>Jobs:</source>
        <translation>Jobs:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="134" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="208" />
        <source>Timeline</source>
        <translation>Zeitachse</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="209" />
        <source>Grid</source>
        <translation>Raster</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="210" />
        <source>Measured waits</source>
        <translation>Gemessene Wartezeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="211" />
        <source>View:</source>
        <translation>Ansicht:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="217" />
        <source>Day</source>
        <translation>Tag</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="218" />
        <source>Week</source>
        <translation>Woche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="227" />
        <source>Today</source>
        <translation>Heute</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="229" />
        <source>Show today or the current week</source>
        <translation>Heute bzw. die aktuelle Woche anzeigen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="239" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1042" />
        <source>The date on display; the calendar opens from the arrow</source>
        <translation>Das angezeigte Datum; der Kalender öffnet sich über den Pfeil</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="252" />
        <source>Snap:</source>
        <translation>Einrasten:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="259" />
        <source>Zoom:</source>
        <translation>Zoom:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="264" />
        <source>Zoom level (pixels per hour)</source>
        <translation>Zoomstufe (Pixel pro Stunde)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="302" />
        <source>Legend</source>
        <translation>Legende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="304" />
        <source>What the bars, hourglasses, question marks and diamonds of the timeline mean</source>
        <translation>Was die Balken, Sanduhren, Fragezeichen und Rauten der Zeitachse bedeuten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="325" />
        <source>Schedules:</source>
        <translation>Schedules:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1102" />
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1103" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1105" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1104" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1143" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1858" />
        <source>Median duration</source>
        <translation>Mediane Dauer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="373" />
        <source>Clients (FD)</source>
        <translation>Clients (FD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="374" />
        <source>Storage (SD)</source>
        <translation>Storage (SD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="668" />
        <source>%1 schedules loaded</source>
        <translation>%1 Schedules geladen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="758" />
        <source>%1 schedules loaded (%2 Run lines)</source>
        <translation>%1 Schedules geladen (%2 Run-Zeilen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="791" />
        <source>Refreshing schedule list...</source>
        <translation>Schedule-Liste wird aktualisiert...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="865" />
        <source>Jobs (%1):</source>
        <translation>Jobs (%1):</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="867" />
        <source>Jobs (%1 Schedules):</source>
        <translation>Jobs (%1 Schedules):</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="888" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="892" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="904" />
        <source>-</source>
        <translation>-</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="906" />
        <source>Median of %1 measured runs at all levels in the last %2 days</source>
        <translation>Median von %1 gemessenen Läufen auf allen Levels in den letzten %2 Tagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="908" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1371" />
        <source>No history</source>
        <translation>Keine Historie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1041" />
        <source>The week on display: pick any day of it; the calendar opens from the arrow</source>
        <translation>Die angezeigte Woche: einen beliebigen Tag daraus wählen; der Kalender öffnet sich über den Pfeil</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Mon</source>
        <translation>Mo</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Tue</source>
        <translation>Di</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Wed</source>
        <translation>Mi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Thu</source>
        <translation>Do</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Fri</source>
        <translation>Fr</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Sat</source>
        <translation>Sa</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Sun</source>
        <translation>So</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="440" />
        <source>Measured history</source>
        <translation>Gemessene Historie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="451" />
        <source>Close the area below the timeline</source>
        <translation>Bereich unter der Zeitachse schließen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1074" />
        <source>Click a run in the timeline: its measured history appears here.</source>
        <translation>Klicken Sie auf einen Lauf in der Zeitachse: Seine gemessene Historie erscheint hier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1106" />
        <source>Priority</source>
        <translation>Priorität</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1144" />
        <source>25% - 75%</source>
        <translation>25% - 75%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1145" />
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1152" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857" />
        <source>Runs</source>
        <translation>Läufe</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1158" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>Median wait</source>
        <translation>Mediane Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1159" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>90% wait</source>
        <translation>90%-Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <source>Duration</source>
        <translation>Dauer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Wait</source>
        <translation>Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1311" />
        <source>no job uses this schedule</source>
        <translation>kein Job verwendet diesen Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1315" />
        <source>no successful run of this job in the last %1 days</source>
        <translation>kein erfolgreicher Lauf dieses Jobs in den letzten %1 Tagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1318" />
        <source>No history: %1.</source>
        <translation>Keine Historie: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1330" />
        <source>Estimate: no run at this level in the last %1 days. The figures are from all levels of the job.</source>
        <translation>Schätzung: kein Lauf auf diesem Level in den letzten %1 Tagen. Die Werte stammen aus allen Levels des Jobs.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1346" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1347" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1366" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2195" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2902" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1400" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1406" />
        <source>1 hour</source>
        <translation>1 Stunde</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1401" />
        <source>2 hours</source>
        <translation>2 Stunden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1402" />
        <source>4 hours</source>
        <translation>4 Stunden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1404" />
        <source>15 min</source>
        <translation>15 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1405" />
        <source>30 min</source>
        <translation>30 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1473" />
        <source>Run line %1 "%2": %3</source>
        <translation>Run-Zeile %1 "%2": %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1488" />
        <source>Not understood and not shown in the views:</source>
        <translation>Nicht verstanden und nicht in den Ansichten angezeigt:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1490" />
        <source>This schedule cannot be written by the application.</source>
        <translation>Dieser Schedule kann von der Anwendung nicht geschrieben werden.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="1496" />
        <source>%n Run line(s) not understood</source>
        <translation>
            <numerusform>%n Run-Zeile nicht verstanden</numerusform>
            <numerusform>%n Run-Zeilen nicht verstanden</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1498" />
        <source>These Run lines are not shown in the views, and their schedules cannot be written by the application:</source>
        <translation>Diese Run-Zeilen werden in den Ansichten nicht angezeigt, und ihre Schedules können von der Anwendung nicht geschrieben werden:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1608" />
        <source>Schedule %1 was not changed: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird), which are not available on this connection. %2 stays at %3 (dropped at %4).</source>
        <translation>Schedule %1 wurde nicht geändert: Das Ändern eines Schedules erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (onesimus-dird), die auf dieser Verbindung nicht verfügbar sind. %2 bleibt bei %3 (abgelegt bei %4).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1635" />
        <source>Schedule %1 was not changed: another change is being written.</source>
        <translation>Schedule %1 wurde nicht geändert: Eine andere Änderung wird gerade geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1649" />
        <source>Run line %1 is not understood, so the schedule is locked.</source>
        <translation>Run-Zeile %1 wird nicht verstanden, daher ist der Schedule gesperrt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1652" />
        <source>the dragged Run line runs at several hours of the day; change it in the grid.</source>
        <translation>die gezogene Run-Zeile läuft zu mehreren Stunden des Tages; ändern Sie sie im Raster.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1656" />
        <source>the dragged run could not be matched to exactly one Run line; refresh and try again, or change it in the grid.</source>
        <translation>der gezogene Lauf konnte nicht genau einer Run-Zeile zugeordnet werden; aktualisieren Sie und versuchen Sie es erneut, oder ändern Sie ihn im Raster.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1660" />
        <source>its Run lines are not loaded yet.</source>
        <translation>seine Run-Zeilen sind noch nicht geladen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1663" />
        <source>%1 is not a time of day.</source>
        <translation>%1 ist keine Uhrzeit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1667" />
        <source>the new Run line did not read back as intended (%1).</source>
        <translation>die neue Run-Zeile ließ sich nicht wie beabsichtigt zurücklesen (%1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1670" />
        <source>Schedule %1 was not changed: %2</source>
        <translation>Schedule %1 wurde nicht geändert: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1675" />
        <source>Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). Only this line is written anew; the other lines stay as they are in the file.</source>
        <translation>Run-Zeile %1 von Schedule %2 wird von %3 auf %4 verschoben (in der Zeitachse gezogen: %5). Nur diese Zeile wird neu geschrieben; die anderen Zeilen bleiben, wie sie in der Datei stehen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1680" />
        <source>The Director checks the change of schedule %1...</source>
        <translation>Der Director prüft die Änderung von Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1726" />
        <source>Schedule %1 is locked: it contains a Run line that is not understood.</source>
        <translation>Schedule %1 ist gesperrt: Er enthält eine Run-Zeile, die nicht verstanden wird.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1732" />
        <source>Schedule %1 was not written: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director. Your changes are kept in the grid.</source>
        <translation>Schedule %1 wurde nicht geschrieben: Das Ändern eines Schedules erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors. Ihre Änderungen bleiben im Raster erhalten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1738" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1768" />
        <source>Another change is being written.</source>
        <translation>Eine andere Änderung wird gerade geschrieben.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1744" />
        <source>Schedule %1 cannot be written: %2</source>
        <translation>Schedule %1 kann nicht geschrieben werden: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1747" />
        <source>The Director checks the changes of schedule %1...</source>
        <translation>Der Director prüft die Änderungen von Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1750" />
        <source>The changes made in the grid. Lines that were not touched stay as they are in the file.</source>
        <translation>Die im Raster vorgenommenen Änderungen. Unberührte Zeilen bleiben, wie sie in der Datei stehen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1762" />
        <source>Schedule %1 was not deleted: deleting a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Schedule %1 wurde nicht gelöscht: Das Löschen eines Schedules erfordert die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1771" />
        <source>The Director checks whether schedule %1 can be deleted...</source>
        <translation>Der Director prüft, ob Schedule %1 gelöscht werden kann...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1786" />
        <source>The Director reloaded its configuration.</source>
        <translation>Der Director hat seine Konfiguration neu geladen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1788" />
        <source>Follow-up commands: %1</source>
        <translation>Folgebefehle: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1847" />
        <source>Refresh history</source>
        <translation>Historie aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1850" />
        <source>Ask the Director again for the jobs of the last days (llist jobs)</source>
        <translation>Den Director erneut nach den Jobs der letzten Tage fragen (llist jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1858" />
        <source>90% duration</source>
        <translation>90%-Dauer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>Last run</source>
        <translation>Letzter Lauf</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1861" />
        <source>Runs in the window that terminated normally or with warnings</source>
        <translation>Läufe im Zeitfenster, die normal oder mit Warnungen beendet wurden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1863" />
        <source>Wait: start of the job minus its scheduled time</source>
        <translation>Wartezeit: Start des Jobs minus geplante Zeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1865" />
        <source>Nine of ten runs waited no longer than this</source>
        <translation>Neun von zehn Läufen warteten nicht länger als dies</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1981" />
        <source>the request got no reply (%1)</source>
        <translation>die Anfrage erhielt keine Antwort (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2011" />
        <source>the Director refused the request (%1)</source>
        <translation>der Director hat die Anfrage abgelehnt (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2012" />
        <source>no reason given</source>
        <translation>kein Grund angegeben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2015" />
        <source>the Director's reply could not be read (%1)</source>
        <translation>die Antwort des Directors konnte nicht gelesen werden (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2039" />
        <source>Schedules: no job history, %1</source>
        <translation>Schedules: keine Job-Historie, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2073" />
        <source>the job history is being loaded</source>
        <translation>die Job-Historie wird geladen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2077" />
        <source>the job history has not been requested yet</source>
        <translation>die Job-Historie wurde noch nicht angefordert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2108" />
        <source>Not connected.</source>
        <translation>Nicht verbunden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2110" />
        <source>The job history has not been requested yet.</source>
        <translation>Die Job-Historie wurde noch nicht angefordert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2112" />
        <source>No job history: %1.</source>
        <translation>Keine Job-Historie: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2113" />
        <source>The schedule views work without it: runs have no duration and waits are unknown.</source>
        <translation>Die Schedule-Ansichten funktionieren auch ohne sie: Läufe haben keine Dauer, und Wartezeiten sind unbekannt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2117" />
        <source>Requesting the jobs of the last %1 days from the Director...</source>
        <translation>Die Jobs der letzten %1 Tage werden beim Director angefordert...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2123" />
        <source>Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs terminated normally or with warnings and were measured.</source>
        <translation>Jobs, die in den letzten %1 Tagen geplant waren, abgerufen %2: %3 Zeilen, davon %4 Läufe normal oder mit Warnungen beendet und gemessen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2132" />
        <source>The row limit (%1) was reached and this Director returned the oldest rows: the newest runs of the window are missing.</source>
        <translation>Das Zeilenlimit (%1) wurde erreicht, und dieser Director hat die ältesten Zeilen geliefert: Die neuesten Läufe des Zeitfensters fehlen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2134" />
        <source>The row limit (%1) was reached: the oldest runs of the window are missing.</source>
        <translation>Das Zeilenlimit (%1) wurde erreicht: Die ältesten Läufe des Zeitfensters fehlen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2138" />
        <source>This Director did not deliver the scheduled time: all waits are unknown.</source>
        <translation>Dieser Director hat die geplante Zeit nicht geliefert: Alle Wartezeiten sind unbekannt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2140" />
        <source>%1 runs came without a scheduled time: their wait is unknown.</source>
        <translation>%1 Läufe kamen ohne geplante Zeit: Ihre Wartezeit ist unbekannt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2144" />
        <source>%1 rows were left out because they start before their scheduled time: copied, migrated and virtual full jobs carry the times of another job.</source>
        <translation>%1 Zeilen wurden ausgelassen, weil sie vor ihrer geplanten Zeit starten: Kopierte, migrierte und VirtualFull-Jobs tragen die Zeiten eines anderen Jobs.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2149" />
        <source>%1 rows were left out because their start or end time is missing or the end is before the start.</source>
        <translation>%1 Zeilen wurden ausgelassen, weil ihre Start- oder Endzeit fehlt oder das Ende vor dem Start liegt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2175" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2703" />
        <source>(none)</source>
        <translation>(keine)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2184" />
        <source>From %1 of %2 runs; the others came without a scheduled time.</source>
        <translation>Aus %1 von %2 Läufen; die anderen kamen ohne geplante Zeit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2193" />
        <source>The Director delivered no scheduled time for these runs.</source>
        <translation>Der Director hat für diese Läufe keine geplante Zeit geliefert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2997" />
        <source>Jobs that are scheduled for the same moment and share a limit (a storage, a client, the job slots of the Director, the parallel writers of a device) wait for each other. The proposal starts some of them later, on a quarter hour (23:15, 23:30 ...), so that they wait less on the dates on display and have room to spare. A wait for a higher priority is meant to be and is not counted; a job of a lower priority that follows a moved job moves along, so that it still runs after it. Nothing is changed on the Director.</source>
        <translation>Jobs, die zum selben Zeitpunkt geplant sind und sich eine Grenze teilen (einen Storage, einen Client, die Job-Plätze des Directors, die parallelen Schreiber eines Geräts), warten aufeinander. Der Vorschlag startet einige davon später, zur Viertelstunde (23:15, 23:30 ...), damit sie an den angezeigten Tagen weniger warten und Puffer haben. Eine Wartezeit wegen höherer Priorität ist gewollt und zählt nicht; ein Job niedrigerer Priorität, der einem verschobenen Job folgt, wandert mit, damit er weiter nach ihm läuft. Am Director wird nichts geändert.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3008" />
        <source>Replays the queue with the jobs moved quarter hour by quarter hour and keeps the smallest moves that wait least</source>
        <translation>Spielt die Warteschlange mit viertelstundenweise verschobenen Jobs durch und behält die kleinsten Verschiebungen mit der geringsten Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3162" />
        <source>Stays: a Run line of its schedule runs at several hours, is not understood or would move into the next day</source>
        <translation>Bleibt: Eine Run-Zeile seines Schedules läuft zu mehreren Stunden, wird nicht verstanden oder würde in den nächsten Tag rutschen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3165" />
        <source>Moves along to run after %1, which has a higher priority</source>
        <translation>Wandert mit, um nach %1 zu laufen, der eine höhere Priorität hat</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3007" />
        <source>Propose later start times</source>
        <translation>Spätere Startzeiten vorschlagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3016" />
        <source>Apply...</source>
        <translation>Anwenden...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Start now</source>
        <translation>Start jetzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Proposed start</source>
        <translation>Vorgeschlagener Start</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Later by</source>
        <translation>Später um</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Wait now</source>
        <translation>Wartezeit jetzt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Wait then</source>
        <translation>Wartezeit danach</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Note</source>
        <translation>Hinweis</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3026" />
        <source>Time held by a limit or waiting for a device, summed over the job's runs on the dates on display</source>
        <translation>Zeit, die eine Grenze den Job aufhält oder er auf ein Gerät wartet, summiert über seine Läufe an den angezeigten Tagen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3028" />
        <source>Proposed later start times</source>
        <translation>Vorgeschlagene spätere Startzeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3048" />
        <source>No proposal yet.</source>
        <translation>Noch kein Vorschlag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3063" />
        <source>Creates a schedule of its own for every job that starts later and sets the job's Schedule to it; everything is shown once for confirmation first</source>
        <translation>Legt für jeden später startenden Job einen eigenen Schedule an und stellt den Schedule des Jobs darauf um; alles wird vorher einmal zur Bestätigung gezeigt</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3065" />
        <source>Needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules, read and change jobs), which are not available on this connection</source>
        <translation>Benötigt die Konfigurationsmethoden der gRPC-Schnittstelle des Onesimus Directors (Schedules anlegen, Jobs lesen und ändern), die auf dieser Verbindung nicht verfügbar sind</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3101" />
        <source>Nothing to stagger: the waits are not predicted (the jobs or the history are not loaded yet).</source>
        <translation>Nichts zu entzerren: Die Wartezeiten sind nicht vorhergesagt (Jobs oder Verlauf noch nicht geladen).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3118" />
        <source>Nothing to stagger: no run on the dates on display waits for a limit or a device.</source>
        <translation>Nichts zu entzerren: Kein Lauf an den angezeigten Tagen wartet auf eine Grenze oder ein Gerät.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3121" />
        <source>No later start helps: avoidable wait %1. See the notes of the jobs.</source>
        <translation>Kein späterer Start hilft: vermeidbare Wartezeit %1. Siehe die Hinweise der Jobs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3124" />
        <source>Avoidable wait on the dates on display: now %1, with the proposal %2. %n job(s) start later.</source>
        <translation>
            <numerusform>Vermeidbare Wartezeit an den angezeigten Tagen: jetzt %1, mit dem Vorschlag %2. %n Job startet später.</numerusform>
            <numerusform>Vermeidbare Wartezeit an den angezeigten Tagen: jetzt %1, mit dem Vorschlag %2. %n Jobs starten später.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3167" />
        <source>A later start does not help</source>
        <translation>Ein späterer Start hilft nicht</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3172" />
        <source>%n min</source>
        <translation>
            <numerusform>%n min</numerusform>
            <numerusform>%n min</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3188" />
        <source>Apply the later start times</source>
        <translation>Spätere Startzeiten anwenden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3190" />
        <source>These resources are written to the Director, in this order. The Director checks each one; the first that it refuses stops the rest.</source>
        <translation>Diese Ressourcen werden in dieser Reihenfolge an den Director geschrieben. Der Director prüft jede einzeln; die erste, die er ablehnt, stoppt den Rest.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3202" />
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3248" />
        <source>Nothing was written: %1</source>
        <translation>Nichts wurde geschrieben: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3276" />
        <source>Reading the jobs that start later...</source>
        <translation>Die später startenden Jobs werden gelesen...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3297" />
        <source>schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3298" />
        <source>New schedule %1 for %2:</source>
        <translation>Neuer Schedule %1 für %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3307" />
        <source>Nothing was written: the text of job %1 has no closing brace.</source>
        <translation>Nichts wurde geschrieben: Der Text von Job %1 hat keine schließende Klammer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3314" />
        <source>(Schedule from its JobDefs)</source>
        <translation>(Schedule aus seinen JobDefs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3320" />
        <source>Nothing was written: the later start times were not confirmed.</source>
        <translation>Nichts wurde geschrieben: Die späteren Startzeiten wurden nicht bestätigt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3332" />
        <source>Written: %1. The jobs start at their new times from now on.</source>
        <translation>Geschrieben: %1. Die Jobs starten ab jetzt zu ihren neuen Zeiten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3336" />
        <source>Writing %1...</source>
        <translation>%1 wird geschrieben...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3342" />
        <source>it was changed on the Director in the meantime</source>
        <translation>er wurde inzwischen am Director geändert</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3344" />
        <source>the name is taken</source>
        <translation>der Name ist vergeben</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3345" />
        <source>nothing</source>
        <translation>nichts</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3347" />
        <source>Writing %1 failed: %2. Written before: %3. Nothing after it was sent.</source>
        <translation>Schreiben von %1 fehlgeschlagen: %2. Vorher geschrieben: %3. Danach wurde nichts gesendet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3363" />
        <source>Nothing was written: job %1 could not be read (%2).</source>
        <translation>Nichts wurde geschrieben: Job %1 konnte nicht gelesen werden (%2).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="430" />
        <source>Predicted waits</source>
        <translation>Erwartete Wartezeiten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="444" />
        <source>Stagger</source>
        <translation>Entzerren</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="450" />
        <source>Close the tabs below the timeline</source>
        <translation>Die Reiter unter der Zeitleiste schließen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2288" />
        <source>Reasons shared by several rows</source>
        <translation>Gründe, die mehrere Zeilen teilen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Scheduled</source>
        <translation>Geplant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Predicted start</source>
        <translation>Erwarteter Start</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Measured</source>
        <translation>Gemessen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Certain</source>
        <translation>Sicher</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Reason</source>
        <translation>Grund</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2295" />
        <source>What this job waited at this level in the job history (StartTime minus SchedTime): median / maximum. The catalog's start is set after the connections and Client Run Before Job, so that time is part of it.</source>
        <translation>Wie lange dieser Job auf diesem Level laut Job-Historie gewartet hat (StartTime minus SchedTime): Median / Maximum. Der Start im Catalog wird nach den Verbindungen und Client Run Before Job gesetzt, daher ist diese Zeit enthalten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2299" />
        <source>Predicted start minus scheduled time: seconds ("27 s"), minutes:seconds ("1:35 min") or, from ten minutes on, hours:minutes ("0:53 h")</source>
        <translation>Erwarteter Start minus geplante Zeit: Sekunden ("27 s"), Minuten:Sekunden ("1:35 min") oder ab zehn Minuten Stunden:Minuten ("0:53 h")</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2302" />
        <source>No: the prediction depends on a limit the Director did not report or on a duration that was not measured</source>
        <translation>Nein: Die Vorhersage hängt von einer Grenze ab, die der Director nicht gemeldet hat, oder von einer Dauer, die nicht gemessen wurde</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2304" />
        <source>Predicted waits of the dates on display</source>
        <translation>Erwartete Wartezeiten der angezeigten Daten</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2306" />
        <source>One row per run that is predicted to wait or whose prediction is uncertain. The reason of the current row is below the table.</source>
        <translation>Eine Zeile pro Lauf, der voraussichtlich warten muss oder dessen Vorhersage unsicher ist. Der Grund der aktuellen Zeile steht unter der Tabelle.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2328" />
        <source>Reason of the selected wait</source>
        <translation>Grund der ausgewählten Wartezeit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2351" />
        <source>Parallel writers of the devices</source>
        <translation>Parallele Schreiber der Geräte</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2353" />
        <source>Notes</source>
        <translation>Hinweise</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2355" />
        <source>Basis of the prediction</source>
        <translation>Grundlage der Vorhersage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2357" />
        <source>Back-test</source>
        <translation>Rückwärtstest</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2367" />
        <source>Back-test of the prediction</source>
        <translation>Rückwärtstest der Vorhersage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2383" />
        <source>Director (%1)</source>
        <translation>Director (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2385" />
        <source>Storage %1 (%2)</source>
        <translation>Storage %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2387" />
        <source>Client %1 (%2)</source>
        <translation>Client %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2389" />
        <source>Job %1 (%2)</source>
        <translation>Job %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2391" />
        <source>Priority %1</source>
        <translation>Priorität %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2393" />
        <source>Queue: priority %1 first</source>
        <translation>Warteschlange: Priorität %1 zuerst</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2401" />
        <source>The prediction has changed: propose again.</source>
        <translation>Die Vorhersage hat sich geändert: bitte neu vorschlagen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2572" />
        <source>Waiting not predicted</source>
        <translation>Keine Wartezeit erwartet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2576" />
        <source>The jobs of the Director are not loaded: it is not known which job runs which schedule.</source>
        <translation>Die Jobs des Directors sind nicht geladen: Es ist nicht bekannt, welcher Job welchen Schedule ausführt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2579" />
        <source>Predicted for the dates on display from the limits of the Director (Maximum Concurrent Jobs, priorities) and the measured durations.</source>
        <translation>Für die angezeigten Daten vorhergesagt aus den Grenzen des Directors (Maximum Concurrent Jobs, Prioritäten) und den gemessenen Dauern.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2582" />
        <source>Uncertain: the prediction depends on a limit the Director did not report or on a duration that was not measured.</source>
        <translation>Unsicher: Die Vorhersage hängt von einer Grenze ab, die der Director nicht gemeldet hat, oder von einer Dauer, die nicht gemessen wurde.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2586" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2798" />
        <source>Opens or closes the list of predicted waits below the timeline.</source>
        <translation>Öffnet oder schließt die Liste der erwarteten Wartezeiten unter der Zeitachse.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2602" />
        <source>Not predicted: the jobs of the Director are not loaded.</source>
        <translation>Nicht vorhergesagt: Die Jobs des Directors sind nicht geladen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2604" />
        <source>%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules from the day before on.</source>
        <translation>%1 angezeigte(r) Lauf/Läufe, nachgespielt mit den %2 Läufen aller aktivierten Schedules ab dem Vortag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2613" />
        <source>the storage of job %1 (the job names none)</source>
        <translation>der Storage von Job %1 (der Job nennt keinen)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2620" />
        <source>storage %1</source>
        <translation>Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2627" />
        <source>client %1</source>
        <translation>Client %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2632" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3311" />
        <source>job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2639" />
        <source>Director = %1</source>
        <translation>Director = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2641" />
        <source>the Director</source>
        <translation>der Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2643" />
        <source>storages: %1</source>
        <translation>Storages: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2645" />
        <source>clients: %1</source>
        <translation>Clients: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2647" />
        <source>Maximum Concurrent Jobs as reported: %1.</source>
        <translation>Maximum Concurrent Jobs wie gemeldet: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2649" />
        <source>Maximum Concurrent Jobs not reported by this Director for: %1. Runs it matters for are uncertain.</source>
        <translation>Maximum Concurrent Jobs wurde von diesem Director nicht gemeldet für: %1. Läufe, für die es relevant ist, sind unsicher.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2673" />
        <source>[%1] %2 rows: %3.</source>
        <translation>[%1] %2 Zeilen: %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2717" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3129" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3174" />
        <source>none</source>
        <translation>keine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2724" />
        <source>not measured</source>
        <translation>nicht gemessen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726" />
        <source>yes</source>
        <translation>ja</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726" />
        <source>no</source>
        <translation>nein</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2731" />
        <source>Prediction too low</source>
        <translation>Vorhersage zu niedrig</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2732" />
        <source>Prediction too low; %1</source>
        <translation>Vorhersage zu niedrig; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2735" />
        <source>Prediction too high</source>
        <translation>Vorhersage zu hoch</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2736" />
        <source>Prediction too high; %1</source>
        <translation>Vorhersage zu hoch; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2740" />
        <source>Waits %1 for the device inside the run</source>
        <translation>Wartet %1 innerhalb des Laufs auf das Gerät</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2754" />
        <source>No run on display is predicted to wait.</source>
        <translation>Für keinen angezeigten Lauf wird eine Wartezeit erwartet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2755" />
        <source>Not predicted.</source>
        <translation>Nicht vorhergesagt.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2795" />
        <source>Predicted waits: %1. The list is open.</source>
        <translation>Erwartete Wartezeiten: %1. Die Liste ist geöffnet.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2796" />
        <source>Predicted waits: %1. The list is closed.</source>
        <translation>Erwartete Wartezeiten: %1. Die Liste ist geschlossen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2808" />
        <source>Select a row to read its reason in full.</source>
        <translation>Wählen Sie eine Zeile, um ihren Grund vollständig zu lesen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2816" />
        <source>%1, scheduled %2:</source>
        <translation>%1, geplant %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2897" />
        <source>Storage %1, parallel writers of its device:</source>
        <translation>Storage %1, parallele Schreiber seines Geräts:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2904" />
        <source>Parallel writers of the device of storage %1</source>
        <translation>Parallele Schreiber des Geräts von Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2905" />
        <source>How many jobs the device(s) of %1 write at a time: Maximum Concurrent Jobs of the Device resource in the storage daemon (several devices: their sum). The Director cannot read it. "unknown": runs that depend on it are uncertain.</source>
        <translation>Wie viele Jobs das/die Gerät(e) von %1 gleichzeitig schreiben: Maximum Concurrent Jobs der Device-Ressource im Storage Daemon (mehrere Geräte: ihre Summe). Der Director kann es nicht lesen. "unbekannt": Läufe, die davon abhängen, sind unsicher.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2948" />
        <source>No job history: no suggestion.</source>
        <translation>Keine Job-Historie: kein Vorschlag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2950" />
        <source>History suggests %1.</source>
        <translation>Die Historie schlägt %1 vor.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2952" />
        <source>History gives no suggestion.</source>
        <translation>Die Historie liefert keinen Vorschlag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958" />
        <source>Use %1</source>
        <translation>%1 verwenden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958" />
        <source>Use</source>
        <translation>Verwenden</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2979" />
        <source>Back-test: no job history to replay.</source>
        <translation>Rückwärtstest: keine Job-Historie zum Nachspielen.</translation>
    </message>
</context>
<context>
    <name>BScheduleWizard</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="950" />
        <source>Add Schedule</source>
        <translation>Schedule hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="1016" />
        <source># This schedule cannot be written:</source>
        <translation># Dieser Schedule kann nicht geschrieben werden:</translation>
    </message>
</context>
<context>
    <name>BSettings</name>
    <message>
        <location filename="../src/config/bsettings.cpp" line="221" />
        <source>Default</source>
        <translation>Standard</translation>
    </message>
</context>
<context>
    <name>BSettingsDialog</name>
    <message>
        <location filename="../ui/bsettingsdialog.ui" line="14" />
        <location filename="../src/config/bsettingsdialog.cpp" line="64" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1387" />
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="103" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1400" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1408" />
        <source>Reset</source>
        <translation>Zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="105" />
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="107" />
        <source>Apply</source>
        <translation>Übernehmen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="140" />
        <source>🔌 Connection</source>
        <translation>🔌 Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="144" />
        <source>🎨 Appearance</source>
        <translation>🎨 Darstellung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="148" />
        <source>⚙️ Behavior</source>
        <translation>⚙️ Verhalten</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="152" />
        <source>🔧 Advanced</source>
        <translation>🔧 Erweitert</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="181" />
        <source>Connection Manager</source>
        <translation>Verbindungsverwaltung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="189" />
        <source>Saved Connections</source>
        <translation>Gespeicherte Verbindungen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="203" />
        <source>+ Add</source>
        <translation>+ Hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="204" />
        <source>Add a new connection profile</source>
        <translation>Neues Verbindungsprofil hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="205" />
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="207" />
        <source>Duplicate</source>
        <translation>Duplizieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="209" />
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="239" />
        <source>Profile</source>
        <translation>Profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="245" />
        <source>e.g. Production Server, Test Environment</source>
        <translation>z. B. Produktionsserver, Testumgebung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="246" />
        <source>Profile Name:</source>
        <translation>Profilname:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="251" />
        <source>Director Connection</source>
        <translation>Director-Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="257" />
        <source>e.g. 192.168.1.100 or bareos-dir.local</source>
        <translation>z. B. 192.168.1.100 oder bareos-dir.local</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="258" />
        <source>Host:</source>
        <translation>Host:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="263" />
        <source>Port:</source>
        <translation>Port:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="267" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (Bareos-Konsolenprotokoll)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="268" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="274" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Dieser Build von Onesimus hat keine gRPC-Unterstützung.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="276" />
        <source>Protocol:</source>
        <translation>Protokoll:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="281" />
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>gRPC-Port des Directors (Standard: BNET-Port + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="282" />
        <source>gRPC port:</source>
        <translation>gRPC-Port:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="289" />
        <source>Forget certificate</source>
        <translation>Zertifikat vergessen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="290" />
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Das gepinnte Zertifikat entfernen. Bei der nächsten Verbindung wird erneut gefragt, ob dem Serverzertifikat vertraut wird.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="293" />
        <source>Server certificate:</source>
        <translation>Serverzertifikat:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="301" />
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="302" />
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="303" />
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="304" />
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="305" />
        <source>Server operating system (affects TLS paths in exported configs)</source>
        <translation>Server-Betriebssystem (beeinflusst TLS-Pfade in exportierten Konfigurationen)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="306" />
        <source>Server OS:</source>
        <translation>Server-Betriebssystem:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="310" />
        <source>Director Name:</source>
        <translation>Director-Name:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="314" />
        <source>Console name for authentication (e.g., 'admin' or 'onesimus')</source>
        <translation>Console-Name für die Authentifizierung (z. B. 'admin' oder 'onesimus')</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="315" />
        <source>Console Name:</source>
        <translation>Console-Name:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="320" />
        <source>Password:</source>
        <translation>Passwort:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="327" />
        <location filename="../src/config/bsettingsdialog.cpp" line="333" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1668" />
        <source>(password MD5 hash will appear here)</source>
        <translation>(MD5-Hash des Passworts erscheint hier)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="328" />
        <source>MD5 Hash:</source>
        <translation>MD5-Hash:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="368" />
        <source>⚠️ TLS encrypts communication with the Director.
TLS is strongly recommended for production environments!</source>
        <translation>⚠️ TLS verschlüsselt die Kommunikation mit dem Director.
TLS wird für Produktionsumgebungen dringend empfohlen!</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="376" />
        <source>Authentication &amp; Encryption</source>
        <translation>Authentifizierung &amp; Verschlüsselung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="382" />
        <source>Authentication Method:</source>
        <translation>Authentifizierungsmethode:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="388" />
        <source>Use x509 certificates (instead of PSK)</source>
        <translation>x509-Zertifikate verwenden (statt PSK)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="390" />
        <source>By default, TLS uses Pre-Shared Key (PSK) authentication.
Enable this to use X.509 certificate-based authentication instead.
Certificate mode requires CA certificate and client certificate/key files.</source>
        <translation>Standardmäßig verwendet TLS die Authentifizierung per Pre-Shared Key (PSK).
Aktivieren Sie dies, um stattdessen die zertifikatbasierte Authentifizierung nach X.509 zu verwenden.
Der Zertifikatsmodus erfordert ein CA-Zertifikat sowie Client-Zertifikat- und Schlüsseldateien.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="408" />
        <source>Path to CA certificate (.pem)</source>
        <translation>Pfad zum CA-Zertifikat (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="409" />
        <location filename="../src/config/bsettingsdialog.cpp" line="419" />
        <location filename="../src/config/bsettingsdialog.cpp" line="429" />
        <location filename="../src/config/bsettingsdialog.cpp" line="440" />
        <source>Browse...</source>
        <translation>Durchsuchen...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="414" />
        <source>CA Certificate:</source>
        <translation>CA-Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="418" />
        <source>Path to client certificate (.pem)</source>
        <translation>Pfad zum Client-Zertifikat (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="424" />
        <source>Client Certificate:</source>
        <translation>Client-Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="428" />
        <source>Path to private key (.pem, .key)</source>
        <translation>Pfad zum privaten Schlüssel (.pem, .key)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="434" />
        <source>Private Key:</source>
        <translation>Privater Schlüssel:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="439" />
        <source>Path to client certificate (.pfx)</source>
        <translation>Pfad zum Client-Zertifikat (.pfx)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="445" />
        <source>PFX Certificate:</source>
        <translation>PFX-Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="448" />
        <source>Convert PEM/DER to PFX...</source>
        <translation>PEM/DER in PFX umwandeln...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="449" />
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Separate PEM/DER-Zertifikat- und Schlüsseldateien in eine PFX-Datei umwandeln</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="454" />
        <source>Verify server certificate (recommended)</source>
        <translation>Serverzertifikat überprüfen (empfohlen)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="463" />
        <source>TLS Cipher List (optional):</source>
        <translation>TLS-Cipher-Liste (optional):</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="469" />
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>z. B. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="470" />
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>Durch Doppelpunkte getrennte Liste von TLS-PSK-Ciphers.
Leer lassen für automatische Erkennung.
Beispiel: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="475" />
        <source>Preset</source>
        <translation>Vorgabe</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="476" />
        <source>Fill with recommended PSK ciphers</source>
        <translation>Mit empfohlenen PSK-Ciphers füllen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="487" />
        <source>gRPC is always encrypted with TLS. The server certificate is confirmed on first use and pinned; the Director authenticates with console name and password. TLS-PSK and certificate settings do not apply.</source>
        <translation>gRPC ist immer mit TLS verschlüsselt. Das Serverzertifikat wird bei der ersten Verwendung bestätigt und gepinnt; der Director authentifiziert mit Console-Name und Passwort. TLS-PSK- und Zertifikatseinstellungen gelten nicht.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="498" />
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="503" />
        <source>Save password in profile</source>
        <translation>Passwort im Profil speichern</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="507" />
        <source>Auto-connect on startup</source>
        <translation>Beim Start automatisch verbinden</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="511" />
        <source>Connection timeout:</source>
        <translation>Verbindungs-Timeout:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="515" />
        <location filename="../src/config/bsettingsdialog.cpp" line="769" />
        <source> seconds</source>
        <translation> Sekunden</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="526" />
        <source>Save Profile</source>
        <translation>Profil speichern</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="528" />
        <source>Save changes to this connection profile</source>
        <translation>Änderungen an diesem Verbindungsprofil speichern</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="540" />
        <source>Export All Profiles</source>
        <translation>Alle Profile exportieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="541" />
        <source>Import Profiles</source>
        <translation>Profile importieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="580" />
        <source>Appearance</source>
        <translation>Darstellung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="586" />
        <source>Text Size</source>
        <translation>Textgröße</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="595" />
        <source>The text follows the font of your desktop. This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0).</source>
        <translation>Der Text folgt der Schrift Ihres Desktops. Dieser Faktor vergrößert oder verkleinert ihn (Ctrl++, Ctrl+-, Ctrl+0).</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="597" />
        <source>Interface zoom:</source>
        <translation>Oberflächen-Zoom:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="602" />
        <source>User Interface</source>
        <translation>Benutzeroberfläche</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="606" />
        <source>Enable animations</source>
        <translation>Animationen aktivieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="610" />
        <source>Compact mode</source>
        <translation>Kompaktmodus</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="616" />
        <source>Language</source>
        <translation>Sprache</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="624" />
        <source>System (%1)</source>
        <translation>System (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="636" />
        <source>Application Language:</source>
        <translation>Anwendungssprache:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="639" />
        <source>Language changes require an application restart. "System" follows the language of the operating system.</source>
        <translation>Sprachänderungen erfordern einen Neustart der Anwendung. „System“ folgt der Sprache des Betriebssystems.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="648" />
        <source>Status Bar Colors</source>
        <translation>Farben der Statusleiste</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="657" />
        <source>Color shown when connected to Director</source>
        <translation>Farbe bei Verbindung zum Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="659" />
        <source>Connected:</source>
        <translation>Verbunden:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="665" />
        <source>Color shown when not connected</source>
        <translation>Farbe ohne Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="667" />
        <source>Disconnected:</source>
        <translation>Getrennt:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="672" />
        <source>Backup Level Colors</source>
        <translation>Farben der Backup-Levels</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="736" />
        <source>Behavior</source>
        <translation>Verhalten</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="742" />
        <source>Confirmations</source>
        <translation>Bestätigungen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="746" />
        <source>Confirm job cancellation</source>
        <translation>Abbruch von Jobs bestätigen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="750" />
        <source>Confirm job start</source>
        <translation>Start von Jobs bestätigen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="757" />
        <source>Auto-Refresh</source>
        <translation>Automatische Aktualisierung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="761" />
        <source>Auto-refresh</source>
        <translation>Automatisch aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="765" />
        <source>Interval:</source>
        <translation>Intervall:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="778" />
        <source>Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="786" />
        <source>Maximum Jobs:</source>
        <translation>Maximale Anzahl Jobs:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="788" />
        <source>Show newest jobs first</source>
        <translation>Neueste Jobs zuerst anzeigen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="791" />
        <source>When enabled, the job list shows the most recent jobs first.
When disabled, the oldest jobs are shown first.</source>
        <translation>Wenn aktiviert, zeigt die Job-Liste die neuesten Jobs zuerst.
Wenn deaktiviert, werden die ältesten Jobs zuerst angezeigt.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="798" />
        <source> hours</source>
        <translation> Stunden</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="800" />
        <source>The clients table marks a client as stale when its last backup or contact
is at least this old. The value is taken from the job history; it is not
a live connection check.</source>
        <translation>Die Client-Tabelle markiert einen Client als veraltet, wenn sein letztes Backup oder sein letzter Kontakt
mindestens so alt ist. Der Wert stammt aus der Job-Historie; es ist keine
Live-Verbindungsprüfung.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="803" />
        <source>Client backup is stale after:</source>
        <translation>Client-Backup ist veraltet nach:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="808" />
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="815" />
        <source> days</source>
        <translation> Tage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="817" />
        <source>The schedule view measures how long jobs run and how long they wait
from the jobs of the last days in the catalog. A longer time gives more
runs per job and a larger request to the Director. The history is only
requested while the schedule view is open.</source>
        <translation>Die Schedule-Ansicht misst anhand der Jobs der letzten Tage im Catalog,
wie lange Jobs laufen und wie lange sie warten. Ein längerer Zeitraum ergibt
mehr Läufe pro Job und eine größere Anfrage an den Director. Die Historie wird
nur angefordert, solange die Schedule-Ansicht geöffnet ist.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="821" />
        <source>Measure job history over the last:</source>
        <translation>Job-Historie messen über die letzten:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="826" />
        <source>Visible Backup Levels</source>
        <translation>Sichtbare Backup-Levels</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="831" />
        <source>Select which backup levels appear in the Run Backup dialog:</source>
        <translation>Wählen Sie, welche Backup-Levels im Dialog „Backup ausführen“ erscheinen:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="837" />
        <source>Complete backup of all files</source>
        <translation>Vollständige Sicherung aller Dateien</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="838" />
        <source>Backup files changed since last backup</source>
        <translation>Seit dem letzten Backup geänderte Dateien sichern</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="839" />
        <source>Backup files changed since last Full backup</source>
        <translation>Seit dem letzten Full-Backup geänderte Dateien sichern</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="840" />
        <source>Consolidate incremental backups into a synthetic full</source>
        <translation>Incremental-Backups zu einem synthetischen Full zusammenführen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="841" />
        <source>Data-level backup (rarely used)</source>
        <translation>Backup auf Datenebene (selten verwendet)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="889" />
        <source>Advanced Settings</source>
        <translation>Erweiterte Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="896" />
        <source>File Logging</source>
        <translation>Protokollierung in Datei</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="900" />
        <source>Enable file logging</source>
        <translation>Protokollierung in Datei aktivieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="901" />
        <source>Write debug information to a log file</source>
        <translation>Debug-Informationen in eine Protokolldatei schreiben</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="905" />
        <source>Log File:</source>
        <translation>Protokolldatei:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="915" />
        <source>Max. Log Size:</source>
        <translation>Max. Protokollgröße:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="939" />
        <source>Miscellaneous</source>
        <translation>Sonstiges</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="943" />
        <source>Show tooltips</source>
        <translation>Tooltips anzeigen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="951" />
        <source>⚠️ These settings are for advanced users.
Changes may affect application stability.</source>
        <translation>⚠️ Diese Einstellungen sind für erfahrene Benutzer.
Änderungen können die Stabilität der Anwendung beeinträchtigen.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1388" />
        <source>Settings have been saved.
Some changes require an application restart.</source>
        <translation>Die Einstellungen wurden gespeichert.
Einige Änderungen erfordern einen Neustart der Anwendung.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1401" />
        <source>Do you really want to reset all settings to default values?
This action cannot be undone.</source>
        <translation>Möchten Sie wirklich alle Einstellungen auf die Standardwerte zurücksetzen?
Diese Aktion kann nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1409" />
        <source>All settings have been reset to default values.</source>
        <translation>Alle Einstellungen wurden auf die Standardwerte zurückgesetzt.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1415" />
        <source>Select CA Certificate</source>
        <translation>CA-Zertifikat auswählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1416" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1428" />
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Zertifikate (*.pem *.crt *.cert);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1424" />
        <source>Select Client Certificate</source>
        <translation>Client-Zertifikat auswählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1426" />
        <source>Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)</source>
        <translation>Zertifikate (*.pfx *.pem *.crt *.cert *.der);;PFX-Dateien (*.pfx);;PEM-Dateien (*.pem);;DER-Dateien (*.der);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1437" />
        <source>Select Private Key</source>
        <translation>Privaten Schlüssel auswählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1438" />
        <source>Keys (*.pem *.key);;All Files (*)</source>
        <translation>Schlüssel (*.pem *.key);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1446" />
        <source>Export Settings</source>
        <translation>Einstellungen exportieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1462" />
        <source>Export</source>
        <translation>Exportieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1462" />
        <source>Settings exported successfully.</source>
        <translation>Einstellungen erfolgreich exportiert.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1469" />
        <source>Import Settings</source>
        <translation>Einstellungen importieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1484" />
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1484" />
        <source>Settings imported successfully.</source>
        <translation>Einstellungen erfolgreich importiert.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1493" />
        <source>No Profiles</source>
        <translation>Keine Profile</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1494" />
        <source>There are no connection profiles to delete.</source>
        <translation>Es gibt keine Verbindungsprofile zum Löschen.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1498" />
        <source>Clear All Profiles</source>
        <translation>Alle Profile löschen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1499" />
        <source>Do you really want to delete all %1 connection profile(s)?

This includes all passwords and certificate paths.
This action cannot be undone.</source>
        <translation>Möchten Sie wirklich alle %1 Verbindungsprofil(e) löschen?

Dies umfasst alle Passwörter und Zertifikatspfade.
Diese Aktion kann nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1518" />
        <source>Deleted</source>
        <translation>Gelöscht</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1519" />
        <source>All connection profiles have been deleted.</source>
        <translation>Alle Verbindungsprofile wurden gelöscht.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1526" />
        <source>Choose Color for Full Backup</source>
        <translation>Farbe für Full-Backup wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1540" />
        <source>Choose Color for Incremental Backup</source>
        <translation>Farbe für Incremental-Backup wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1554" />
        <source>Choose Color for Differential Backup</source>
        <translation>Farbe für Differential-Backup wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1568" />
        <source>Choose Color for Virtual Full Backup</source>
        <translation>Farbe für VirtualFull-Backup wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1582" />
        <source>Choose Color for Connected Status</source>
        <translation>Farbe für den Status „Verbunden“ wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1596" />
        <source>Choose Color for Disconnected Status</source>
        <translation>Farbe für den Status „Getrennt“ wählen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1660" />
        <source>••••••••  (password saved)</source>
        <translation>••••••••  (Passwort gespeichert)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1666" />
        <source>Enter password</source>
        <translation>Passwort eingeben</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1708" />
        <source>New Connection</source>
        <translation>Neue Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1746" />
        <source>Delete Profile</source>
        <translation>Profil löschen</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1747" />
        <source>Do you really want to delete the connection profile "%1"?

This action cannot be undone.</source>
        <translation>Möchten Sie das Verbindungsprofil "%1" wirklich löschen?

Diese Aktion kann nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1782" />
        <source>%1 (Copy)</source>
        <translation>%1 (Kopie)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1806" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1896" />
        <source>Unnamed Connection</source>
        <translation>Unbenannte Verbindung</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1880" />
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(nicht gepinnt - wird bei der nächsten Verbindung bestätigt)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1885" />
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1897" />
        <source>Profile Saved</source>
        <translation>Profil gespeichert</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1898" />
        <source>Connection profile "%1" has been saved.</source>
        <translation>Verbindungsprofil "%1" wurde gespeichert.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1912" />
        <source>PFX File Created</source>
        <translation>PFX-Datei erstellt</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1913" />
        <source>PFX file has been created successfully.

The certificate path has been updated in the current profile.</source>
        <translation>Die PFX-Datei wurde erfolgreich erstellt.

Der Zertifikatspfad wurde im aktuellen Profil aktualisiert.</translation>
    </message>
</context>
<context>
    <name>BStaggerPlan</name>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="32" />
        <source>Schedule %1 has no Run lines.</source>
        <translation>Schedule %1 hat keine Run-Zeilen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="36" />
        <source>Run line %1 of schedule %2 is not understood.</source>
        <translation>Run-Zeile %1 von Schedule %2 wird nicht verstanden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="39" />
        <source>Run line %1 of schedule %2 runs at several hours.</source>
        <translation>Run-Zeile %1 von Schedule %2 läuft zu mehreren Stunden.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="42" />
        <source>Run line %1 of schedule %2 would move into the next day.</source>
        <translation>Run-Zeile %1 von Schedule %2 würde in den nächsten Tag rutschen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="52" />
        <source>Run line %1 of schedule %2 did not read back as written (%3).</source>
        <translation>Run-Zeile %1 von Schedule %2 las sich nicht so zurück, wie sie geschrieben wurde (%3).</translation>
    </message>
</context>
<context>
    <name>BStatus</name>
    <message>
        <location filename="../src/bstatuschip.cpp" line="25" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="25" />
        <source>Terminated normally</source>
        <translation>Normal beendet</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26" />
        <source>Warnings</source>
        <translation>Warnungen</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26" />
        <source>Terminated with warnings</source>
        <translation>Mit Warnungen beendet</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27" />
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27" />
        <source>Terminated in error</source>
        <translation>Mit Fehler beendet</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28" />
        <source>Errors</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28" />
        <source>Non-fatal error</source>
        <translation>Nicht schwerwiegender Fehler</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29" />
        <source>Fatal</source>
        <translation>Schwerwiegend</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29" />
        <source>Fatal error</source>
        <translation>Schwerwiegender Fehler</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30" />
        <source>Canceled</source>
        <translation>Abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30" />
        <source>Canceled by user</source>
        <translation>Vom Benutzer abgebrochen</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="31" />
        <source>Running</source>
        <translation>Läuft</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32" />
        <source>Committing</source>
        <translation>Festschreiben</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32" />
        <source>Committing data (last despool)</source>
        <translation>Daten werden festgeschrieben (letztes Despooling)</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33" />
        <source>Despooling</source>
        <translation>Despooling</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33" />
        <source>Despooling data</source>
        <translation>Daten werden despoolt</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34" />
        <source>Created</source>
        <translation>Erstellt</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34" />
        <source>Created, not yet running</source>
        <translation>Erstellt, läuft noch nicht</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35" />
        <source>Wait FD</source>
        <translation>Wartet auf FD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35" />
        <source>Waiting on the File daemon</source>
        <translation>Wartet auf den File Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36" />
        <source>Wait SD</source>
        <translation>Wartet auf SD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36" />
        <source>Waiting on the Storage daemon</source>
        <translation>Wartet auf den Storage Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37" />
        <source>Wait media</source>
        <translation>Wartet auf Medium</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37" />
        <source>Waiting for new media</source>
        <translation>Wartet auf ein neues Medium</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38" />
        <source>Wait mount</source>
        <translation>Wartet auf Mount</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38" />
        <source>Waiting for mount</source>
        <translation>Wartet auf das Mounten</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39" />
        <source>Wait storage</source>
        <translation>Wartet auf Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39" />
        <source>Waiting for a storage resource</source>
        <translation>Wartet auf eine Storage-Ressource</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40" />
        <source>Wait job</source>
        <translation>Wartet auf Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40" />
        <source>Waiting for a job resource</source>
        <translation>Wartet auf eine Job-Ressource</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41" />
        <source>Wait client</source>
        <translation>Wartet auf Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41" />
        <source>Waiting for a client resource</source>
        <translation>Wartet auf eine Client-Ressource</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42" />
        <source>Wait max jobs</source>
        <translation>Wartet auf max. Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42" />
        <source>Waiting for maximum jobs</source>
        <translation>Wartet auf maximale Anzahl Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43" />
        <source>Wait start</source>
        <translation>Wartet auf Start</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43" />
        <source>Waiting for the start time</source>
        <translation>Wartet auf die Startzeit</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44" />
        <source>Wait priority</source>
        <translation>Wartet auf Priorität</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44" />
        <source>Waiting for higher priority jobs to finish</source>
        <translation>Wartet auf das Ende von Jobs mit höherer Priorität</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45" />
        <source>Wait device</source>
        <translation>Wartet auf Gerät</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45" />
        <source>Queued, waiting for a device</source>
        <translation>Eingereiht, wartet auf ein Gerät</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46" />
        <source>Despool attr.</source>
        <translation>Despool Attr.</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46" />
        <source>Storage daemon despooling attributes</source>
        <translation>Storage Daemon despoolt Attribute</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47" />
        <source>Inserting</source>
        <translation>Einfügen</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47" />
        <source>Doing batch insert of file records</source>
        <translation>Batch-Einfügen von Dateieinträgen</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48" />
        <source>Blocked</source>
        <translation>Blockiert</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49" />
        <source>Differences</source>
        <translation>Unterschiede</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49" />
        <source>Verify found differences</source>
        <translation>Verify hat Unterschiede gefunden</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50" />
        <source>Incomplete</source>
        <translation>Unvollständig</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50" />
        <source>Incomplete job</source>
        <translation>Unvollständiger Job</translation>
    </message>
</context>
<context>
    <name>BStorageResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319" />
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319" />
        <source>Device</source>
        <translation>Device</translation>
    </message>
</context>
<context>
    <name>BTableModel</name>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="215" />
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="215" />
        <source>No</source>
        <translation>Nein</translation>
    </message>
</context>
<context>
    <name>BWaitCalibration</name>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="190" />
        <source>this job was not measured: median of all %1 runs on this Director</source>
        <translation>dieser Job wurde nicht gemessen: Median aller %1 Läufe auf diesem Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="195" />
        <source>nothing measured: %1 assumed</source>
        <translation>nichts gemessen: %1 angenommen</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="203" />
        <source>Duration %1: median of %2 measured run(s). It includes any wait for a device inside the storage daemon (the parallel writers of %3 are not known, so it is not taken out).</source>
        <translation>Dauer %1: Median von %2 gemessenen Läufen. Sie enthält jede Wartezeit auf ein Gerät innerhalb des Storage Daemons (die parallelen Schreiber von %3 sind nicht bekannt, daher wird sie nicht herausgerechnet).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="207" />
        <source>its storage</source>
        <translation>sein Storage</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="186" />
        <source>median of %n measured run(s) at this level</source>
        <translation>
            <numerusform>Median von %n gemessenem Lauf auf diesem Level</numerusform>
            <numerusform>Median von %n gemessenen Läufen auf diesem Level</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="188" />
        <source>median of %n measured run(s) of this job at other levels</source>
        <translation>
            <numerusform>Median von %n gemessenem Lauf dieses Jobs auf anderen Levels</numerusform>
            <numerusform>Median von %n gemessenen Läufen dieses Jobs auf anderen Levels</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="210" />
        <source>Duration %1: median of %n measured run(s); they did not wait for the device.</source>
        <translation>
            <numerusform>Dauer %1: Median von %n gemessenem Lauf; er hat nicht auf das Gerät gewartet.</numerusform>
            <numerusform>Dauer %1: Median von %n gemessenen Läufen; sie haben nicht auf das Gerät gewartet.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="214" />
        <source>Duration %1 without the wait for the device (measured %2, median of %3 run(s)): counted from the end of the run that held the device before it (%4 parallel writer(s)). It may still hold up to 30 s of the storage daemon's tries.</source>
        <translation>Dauer %1 ohne die Wartezeit auf das Gerät (gemessen %2, Median von %3 Läufen): gezählt ab dem Ende des Laufs, der das Gerät vorher belegte (%4 parallele(r) Schreiber). Sie kann noch bis zu 30 s Wiederholungsversuche des Storage Daemons enthalten.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="310" />
        <source>%1 ended %2 after %3 on %4; alone it takes at least %5</source>
        <translation>%1 endete %2 nach %3 am %4; allein dauert es mindestens %5</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="327" />
        <source>%1 night(s) with runs on storage %2; on %3 more than one of them was queued or running at the same time; on %4 the runs' recorded start to end overlapped.</source>
        <translation>%1 Nacht/Nächte mit Läufen auf Storage %2; in %3 war mehr als einer davon gleichzeitig eingereiht oder laufend; in %4 überschnitten sich die aufgezeichneten Start- und Endzeiten der Läufe.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="334" />
        <source>No two runs ever overlapped although several were queued: the storage wrote one job at a time.</source>
        <translation>Nie haben sich zwei Läufe überschnitten, obwohl mehrere eingereiht waren: Der Storage schrieb jeweils einen Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="340" />
        <source>Overlapping runs do not prove parallel writing: with Just In Time Reservation a job waits for the device inside its run.</source>
        <translation>Sich überschneidende Läufe beweisen kein paralleles Schreiben: Mit Just In Time Reservation wartet ein Job innerhalb seines Laufs auf das Gerät.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="344" />
        <source>%1 of %2 telling pair(s) ended sooner after each other than one writer allows (e.g. %3): it writes more than one job at a time.</source>
        <translation>%1 von %2 aussagekräftigen Paaren endeten schneller nacheinander, als ein einzelner Schreiber erlaubt (z. B. %3): Er schreibt mehr als einen Job gleichzeitig.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="351" />
        <source>In all %1 telling pair(s) on %2 night(s) the later run went on after the other ended for at least as long as it takes alone (e.g. %3): consistent with one writer at a time.</source>
        <translation>In allen %1 aussagekräftigen Paaren in %2 Nacht/Nächten lief der spätere Lauf nach dem Ende des anderen mindestens so lange weiter, wie er allein dauert (z. B. %3): passt zu einem Schreiber zur Zeit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="358" />
        <source>Not enough telling pairs (%1; a pair tells when the later run started well before the other ended and was also seen running alone): no suggestion.</source>
        <translation>Nicht genügend aussagekräftige Paare (%1; ein Paar ist aussagekräftig, wenn der spätere Lauf deutlich vor dem Ende des anderen startete und auch allein laufend beobachtet wurde): kein Vorschlag.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="446" />
        <source>Back-test: no measured night to replay.</source>
        <translation>Rückwärtstest: keine gemessene Nacht zum Nachspielen.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="447" />
        <source>Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 (%5, %6; %7).</source>
        <translation>Rückwärtstest, letzte %1 Nacht/Nächte (%2 Läufe): medianer Fehler %3, schlechtester %4 (%5, %6; %7).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453" />
        <source>predicted too late</source>
        <translation>zu spät vorhergesagt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453" />
        <source>predicted too early</source>
        <translation>zu früh vorhergesagt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="455" />
        <source>Replayed without a device limit for %1 (not set).</source>
        <translation>Ohne Gerätegrenze für %1 nachgespielt (nicht gesetzt).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="458" />
        <source>Durations and overheads come from the same history.</source>
        <translation>Dauern und Startaufwände stammen aus derselben Historie.</translation>
    </message>
</context>
<context>
    <name>BareosAuth</name>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="142" />
        <source>Starting Bareos authentication...</source>
        <translation>Starte Bareos-Authentifizierung...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="149" />
        <source>Setting up TLS-PSK...</source>
        <translation>Richte TLS-PSK ein...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="168" />
        <source>Starting TLS-PSK handshake...</source>
        <translation>Starte TLS-PSK-Handshake...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="175" />
        <source>Setting up certificate-based TLS...</source>
        <translation>Richte zertifikatbasiertes TLS ein...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="191" />
        <source>Starting TLS handshake...</source>
        <translation>Starte TLS-Handshake...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="199" />
        <source>Starting legacy authentication (no TLS)...</source>
        <translation>Starte Legacy-Authentifizierung (ohne TLS)...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="628" />
        <source>TLS established, sending Hello packet...</source>
        <translation>TLS hergestellt, sende Hello-Paket...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="652" />
        <source>Waiting for Director challenge...</source>
        <translation>Warte auf Challenge des Directors...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="749" />
        <source>Director rejected connection: %1</source>
        <translation>Director hat die Verbindung abgelehnt: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="756" />
        <source>

Hint: The Director requires TLS encryption. Please use PSK or Certificate authentication instead of Legacy mode.</source>
        <translation>

Hinweis: Der Director erfordert TLS-Verschlüsselung. Bitte verwenden Sie PSK- oder Zertifikatsauthentifizierung statt des Legacy-Modus.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="762" />
        <source>Director sent empty response.

This usually means the Director requires TLS encryption but Legacy (unencrypted) mode was used.
Please try PSK or Certificate authentication.</source>
        <translation>Director hat eine leere Antwort gesendet.

Das bedeutet meist, dass der Director TLS-Verschlüsselung erfordert, aber der Legacy-Modus (unverschlüsselt) verwendet wurde.
Bitte versuchen Sie PSK- oder Zertifikatsauthentifizierung.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="770" />
        <source>Unexpected response from Director:
%1</source>
        <translation>Unerwartete Antwort vom Director:
%1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="832" />
        <source>CRAM-MD5 response sent successfully</source>
        <translation>CRAM-MD5-Antwort erfolgreich gesendet</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1228" />
        <source>The Director announced a message of %1 bytes during authentication (limit: %2 bytes). Connection closed.</source>
        <translation>Der Director hat während der Authentifizierung eine Nachricht von %1 Bytes angekündigt (Grenze: %2 Bytes). Verbindung geschlossen.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1374" />
        <location filename="../src/director/bareosauth.cpp" line="1488" />
        <source>Connected</source>
        <translation>Verbunden</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1669" />
        <source>TLS-PSK authentication failed: %1

This usually means:
• Wrong password
• Wrong console name
• Console not configured on Director
• TLS-PSK not enabled on Director</source>
        <translation>TLS-PSK-Authentifizierung fehlgeschlagen: %1

Das bedeutet meist:
• Falsches Passwort
• Falscher Console-Name
• Console nicht auf dem Director konfiguriert
• TLS-PSK auf dem Director nicht aktiviert</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1678" />
        <source>TLS handshake failed: %1

Possible causes:
• No compatible cipher suites
• TLS version mismatch
• Director doesn't support PSK</source>
        <translation>TLS-Handshake fehlgeschlagen: %1

Mögliche Ursachen:
• Keine kompatiblen Cipher-Suites
• Abweichende TLS-Versionen
• Director unterstützt kein PSK</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1686" />
        <source>Socket error: %1</source>
        <translation>Socket-Fehler: %1</translation>
    </message>
</context>
<context>
    <name>BareosDirector</name>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="183" />
        <source>Changing the configuration needs the gRPC interface of the Onesimus Director</source>
        <translation>Das Ändern der Konfiguration erfordert die gRPC-Schnittstelle des Onesimus Directors</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="235" />
        <source>Connection to the Director was closed before the reply arrived</source>
        <translation>Die Verbindung zum Director wurde geschlossen, bevor die Antwort eintraf</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="294" />
        <source>Disconnected from the Director before the reply arrived</source>
        <translation>Vom Director getrennt, bevor die Antwort eintraf</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="461" />
        <source>No response from Director within %1 seconds</source>
        <translation>Keine Antwort vom Director innerhalb von %1 Sekunden</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="471" />
        <source>Timeout while loading resources: %1</source>
        <translation>Zeitüberschreitung beim Laden der Ressourcen: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="910" />
        <source>Connection to the Director was lost before the reply arrived</source>
        <translation>Die Verbindung zum Director ging verloren, bevor die Antwort eintraf</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1006" />
        <source>The Director did not confirm the API mode within %1 seconds. The connection was closed.</source>
        <translation>Der Director hat den API-Modus nicht innerhalb von %1 Sekunden bestätigt. Die Verbindung wurde geschlossen.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1127" />
        <source>The Director announced a message of %1 bytes (limit: %2 MiB). The connection was closed.</source>
        <translation>Der Director hat eine Nachricht von %1 Bytes angekündigt (Grenze: %2 MiB). Die Verbindung wurde geschlossen.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1559" />
        <source>TLS is enabled for this connection, but the session with the Director is not encrypted. The connection was closed.</source>
        <translation>TLS ist für diese Verbindung aktiviert, aber die Sitzung mit dem Director ist nicht verschlüsselt. Die Verbindung wurde geschlossen.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1822" />
        <source>Unknown error</source>
        <translation>Unbekannter Fehler</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1877" />
        <location filename="../src/director/bareosdirector.cpp" line="2323" />
        <source>Not ready</source>
        <translation>Nicht bereit</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1929" />
        <source>Error sending command</source>
        <translation>Fehler beim Senden des Befehls</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1976" />
        <source>The command was not sent: no ready connection to the Director</source>
        <translation>Der Befehl wurde nicht gesendet: keine bereite Verbindung zum Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2312" />
        <source>This build of Onesimus has no gRPC support</source>
        <translation>Dieser Build von Onesimus hat keine gRPC-Unterstützung</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2403" />
        <source>Server certificate not trusted</source>
        <translation>Serverzertifikat nicht vertrauenswürdig</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2419" />
        <source>Command failed</source>
        <translation>Befehl fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2447" />
        <source>Connection to Director lost: %1</source>
        <translation>Verbindung zum Director verloren: %1</translation>
    </message>
</context>
<context>
    <name>CommandLine</name>
    <message>
        <location filename="../src/main.cpp" line="391" />
        <source>--tls needs --host. Without --host the TLS settings of the saved connection profile are used.</source>
        <translation>--tls erfordert --host. Ohne --host werden die TLS-Einstellungen des gespeicherten Verbindungsprofils verwendet.</translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="397" />
        <source>--tls needs the console password: use --password-file, the environment variable ONESIMUS_PASSWORD or --password.</source>
        <translation>--tls erfordert das Console-Passwort: Verwenden Sie --password-file, die Umgebungsvariable ONESIMUS_PASSWORD oder --password.</translation>
    </message>
</context>
<context>
    <name>ConfigPreviewPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2020" />
        <source>Server Configuration Preview</source>
        <translation>Vorschau der Serverkonfiguration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2022" />
        <source>Copy this configuration to your Bareos Director</source>
        <translation>Kopieren Sie diese Konfiguration auf Ihren Bareos Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2032" />
        <source>Console resource for the Director:</source>
        <translation>Console-Ressource für den Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2036" />
        <source>Path: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</source>
        <translation>Pfad: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2048" />
        <location filename="../src/bconnectionwizard.cpp" line="2089" />
        <location filename="../src/bconnectionwizard.cpp" line="2130" />
        <location filename="../src/bconnectionwizard.cpp" line="2179" />
        <location filename="../src/bconnectionwizard.cpp" line="2467" />
        <location filename="../src/bconnectionwizard.cpp" line="2506" />
        <location filename="../src/bconnectionwizard.cpp" line="2517" />
        <location filename="../src/bconnectionwizard.cpp" line="2528" />
        <source>Copy to Clipboard</source>
        <translation>In die Zwischenablage kopieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2053" />
        <location filename="../src/bconnectionwizard.cpp" line="2094" />
        <location filename="../src/bconnectionwizard.cpp" line="2184" />
        <location filename="../src/bconnectionwizard.cpp" line="2495" />
        <location filename="../src/bconnectionwizard.cpp" line="2555" />
        <location filename="../src/bconnectionwizard.cpp" line="2632" />
        <source>Save .conf</source>
        <translation>.conf speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2058" />
        <location filename="../src/bconnectionwizard.cpp" line="2099" />
        <location filename="../src/bconnectionwizard.cpp" line="2189" />
        <location filename="../src/bconnectionwizard.cpp" line="2606" />
        <location filename="../src/bconnectionwizard.cpp" line="2671" />
        <source>Save ZIP</source>
        <translation>ZIP speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2066" />
        <source>Console Config</source>
        <translation>Console-Konfiguration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2073" />
        <source>Director resource TLS settings (if needed):</source>
        <translation>TLS-Einstellungen der Director-Ressource (falls nötig):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2077" />
        <location filename="../src/bconnectionwizard.cpp" line="2118" />
        <source>Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Pfad: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2107" />
        <source>Director TLS</source>
        <translation>Director-TLS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2114" />
        <source>Enable gRPC in the Director resource:</source>
        <translation>gRPC in der Director-Ressource aktivieren:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2137" />
        <source>Director (gRPC)</source>
        <translation>Director (gRPC)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2145" />
        <source>Configuration for command-line consoles on the client:</source>
        <translation>Konfiguration für Kommandozeilen-Consoles auf dem Client:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2149" />
        <source>Path: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf or oconsole -c &amp;lt;file&amp;gt; (oconsole)</source>
        <translation>Pfad: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf oder oconsole -c &amp;lt;file&amp;gt; (oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2157" />
        <source>Contains the console password: restrict the file to mode 0640 root:bareos (or 0600 owned by the user running the console).</source>
        <translation>Enthält das Console-Passwort: Beschränken Sie die Datei auf Modus 0640 root:bareos (oder 0600 im Besitz des Benutzers, der die Console ausführt).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2197" />
        <source>Console client (bconsole / oconsole)</source>
        <translation>Console-Client (bconsole / oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2201" />
        <location filename="../src/bconnectionwizard.cpp" line="2229" />
        <source>After copying, paste this into your Bareos Director config and restart bareos-dir.</source>
        <translation>Fügen Sie dies nach dem Kopieren in Ihre Bareos-Director-Konfiguration ein und starten Sie bareos-dir neu.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2227" />
        <source>Add the Console resource and enable gRPC in the Director resource, then reload the Director.</source>
        <translation>Fügen Sie die Console-Ressource hinzu, aktivieren Sie gRPC in der Director-Ressource und laden Sie dann den Director neu.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2324" />
        <source>Default gRPC port: BNET port + 1000 (%1 + 1000 = %2).</source>
        <translation>Standard-gRPC-Port: BNET-Port + 1000 (%1 + 1000 = %2).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2326" />
        <source>The Console resource provides name and password for the login.</source>
        <translation>Die Console-Ressource liefert Name und Passwort für die Anmeldung.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2327" />
        <source>After adding the Console, reload the Director:</source>
        <translation>Laden Sie nach dem Hinzufügen der Console den Director neu:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2329" />
        <source>(enabling gRPC for the first time requires a Director restart)</source>
        <translation>(das erstmalige Aktivieren von gRPC erfordert einen Neustart des Directors)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2416" />
        <source>Password in "[md5]&lt;hex&gt;" form: accepted by oconsole and by C++ bconsole.</source>
        <translation>Passwort in der Form "[md5]&lt;hex&gt;": wird von oconsole und vom C++-bconsole akzeptiert.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2426" />
        <source>not used when the Console resource below matches</source>
        <translation>nicht verwendet, wenn die Console-Ressource unten passt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2434" />
        <source>Grpc Fingerprint = &lt;SHA-256 of the Director's gRPC certificate&gt;</source>
        <translation>Grpc Fingerprint = &lt;SHA-256 des gRPC-Zertifikats des Directors&gt;</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2465" />
        <location filename="../src/bconnectionwizard.cpp" line="2504" />
        <location filename="../src/bconnectionwizard.cpp" line="2515" />
        <location filename="../src/bconnectionwizard.cpp" line="2526" />
        <source>Copied!</source>
        <translation>Kopiert!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2478" />
        <source>Save Console Client Configuration</source>
        <translation>Console-Client-Konfiguration speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2479" />
        <location filename="../src/bconnectionwizard.cpp" line="2539" />
        <location filename="../src/bconnectionwizard.cpp" line="2616" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Konfigurationsdateien (*.conf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2485" />
        <location filename="../src/bconnectionwizard.cpp" line="2545" />
        <location filename="../src/bconnectionwizard.cpp" line="2575" />
        <location filename="../src/bconnectionwizard.cpp" line="2622" />
        <location filename="../src/bconnectionwizard.cpp" line="2651" />
        <source>Save Failed</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2486" />
        <location filename="../src/bconnectionwizard.cpp" line="2546" />
        <location filename="../src/bconnectionwizard.cpp" line="2623" />
        <source>Could not write to %1</source>
        <translation>Konnte nicht nach %1 schreiben</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2493" />
        <location filename="../src/bconnectionwizard.cpp" line="2553" />
        <location filename="../src/bconnectionwizard.cpp" line="2604" />
        <location filename="../src/bconnectionwizard.cpp" line="2630" />
        <location filename="../src/bconnectionwizard.cpp" line="2669" />
        <source>Saved!</source>
        <translation>Gespeichert!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2538" />
        <source>Save Console Configuration</source>
        <translation>Console-Konfiguration speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2565" />
        <source>Save Console Configuration as ZIP</source>
        <translation>Console-Konfiguration als ZIP speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2566" />
        <location filename="../src/bconnectionwizard.cpp" line="2642" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-Archive (*.zip);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2576" />
        <location filename="../src/bconnectionwizard.cpp" line="2652" />
        <source>Could not create ZIP file: %1</source>
        <translation>ZIP-Datei konnte nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2615" />
        <source>Save Director TLS Configuration</source>
        <translation>Director-TLS-Konfiguration speichern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2641" />
        <source>Save Director TLS Configuration as ZIP</source>
        <translation>Director-TLS-Konfiguration als ZIP speichern</translation>
    </message>
</context>
<context>
    <name>ConsoleSetupPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2948" />
        <source>Console configuration</source>
        <translation>Console-Konfiguration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2956" />
        <source>Modify existing console</source>
        <translation>Vorhandene Console ändern</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2969" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2979" />
        <source>Select a console to view its configuration...</source>
        <translation>Wählen Sie eine Console, um ihre Konfiguration anzuzeigen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2987" />
        <source>Create new console</source>
        <translation>Neue Console anlegen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2998" />
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3005" />
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3009" />
        <source>Password:</source>
        <translation>Passwort:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3017" />
        <source>Security notice: Consoles with overly wide ACLs (e.g. *all*) grant full access to all backup operations, including deletion. Ask your backup administrator to create a Console resource with restricted ACLs tailored to your needs.</source>
        <translation>Sicherheitshinweis: Consoles mit zu weit gefassten ACLs (z. B. *all*) gewähren vollen Zugriff auf alle Backup-Vorgänge, einschließlich Löschen. Bitten Sie Ihren Backup-Administrator, eine Console-Ressource mit eingeschränkten, auf Ihre Bedürfnisse zugeschnittenen ACLs anzulegen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3033" />
        <location filename="../src/bconnectionwizard.cpp" line="3058" />
        <source>Modify an existing console or create a new one on the director.</source>
        <translation>Ändern Sie eine vorhandene Console oder legen Sie eine neue auf dem Director an.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3056" />
        <source>gRPC requires 'Enable Grpc = yes' in the Director resource (default port: BNET port + 1000). After adding a console, reload the Director.</source>
        <translation>gRPC erfordert 'Enable Grpc = yes' in der Director-Ressource (Standardport: BNET-Port + 1000). Laden Sie nach dem Hinzufügen einer Console den Director neu.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068" />
        <source>No Console</source>
        <translation>Keine Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068" />
        <source>Please select a console.</source>
        <translation>Bitte wählen Sie eine Console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076" />
        <source>Missing</source>
        <translation>Fehlt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076" />
        <source>Please enter a name and password for the new console.</source>
        <translation>Bitte geben Sie einen Namen und ein Passwort für die neue Console ein.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3179" />
        <location filename="../src/bconnectionwizard.cpp" line="3275" />
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3197" />
        <source>Loading details for %1...</source>
        <translation>Details für %1 werden geladen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3206" />
        <source>Waiting for connection...</source>
        <translation>Warte auf Verbindung...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3216" />
        <location filename="../src/bconnectionwizard.cpp" line="3257" />
        <source>Loading consoles...</source>
        <translation>Consoles werden geladen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3222" />
        <source>Connecting to Director...</source>
        <translation>Verbinde mit dem Director...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3246" />
        <source>Loading resources...</source>
        <translation>Ressourcen werden geladen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3248" />
        <source>Authentication failed: %1</source>
        <translation>Authentifizierung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3266" />
        <source>Connection lost</source>
        <translation>Verbindung verloren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3299" />
        <source>No named consoles found. Ask your backup administrator to create a personalized Console resource on the Director.</source>
        <translation>Keine benannten Consoles gefunden. Bitten Sie Ihren Backup-Administrator, eine persönliche Console-Ressource auf dem Director anzulegen.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="3303" />
        <source>Found %n console(s)</source>
        <translation>
            <numerusform>%n Console gefunden</numerusform>
            <numerusform>%n Consoles gefunden</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3320" />
        <source>No details found for %1</source>
        <translation>Keine Details für %1 gefunden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3325" />
        <source>Name: %1
</source>
        <translation>Name: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3327" />
        <source>Description: %1
</source>
        <translation>Beschreibung: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3329" />
        <source>Profile: %1
</source>
        <translation>Profile: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3334" />
        <source>TLS Enabled: %1
</source>
        <translation>TLS Enabled: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3336" />
        <source>TLS Required: %1
</source>
        <translation>TLS Required: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3338" />
        <source>TLS Verify Peer: %1
</source>
        <translation>TLS Verify Peer: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3340" />
        <source>Job ACL: %1
</source>
        <translation>Job ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3342" />
        <source>Client ACL: %1
</source>
        <translation>Client ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3344" />
        <source>Storage ACL: %1
</source>
        <translation>Storage ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3346" />
        <source>Command ACL: %1
</source>
        <translation>Command ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3348" />
        <source>Catalog ACL: %1
</source>
        <translation>Catalog ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3362" />
        <source>
--- WARNING ---
This console has full access (*all*). Consider restricting ACLs to prevent accidental deletion of backups or other critical operations.
</source>
        <translation>
--- WARNUNG ---
Diese Console hat vollen Zugriff (*all*). Erwägen Sie, die ACLs einzuschränken, um versehentliches Löschen von Backups oder andere kritische Vorgänge zu verhindern.
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3397" />
        <source>Creating console '%1'...</source>
        <translation>Console '%1' wird angelegt...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3408" />
        <source>Console created successfully!</source>
        <translation>Console erfolgreich angelegt!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3411" />
        <source>Failed to create console</source>
        <translation>Console konnte nicht angelegt werden</translation>
    </message>
</context>
<context>
    <name>CredentialsPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1418" />
        <source>Enter your credentials</source>
        <translation>Geben Sie Ihre Zugangsdaten ein</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1424" />
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1428" />
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1436" />
        <source>Generate</source>
        <translation>Erzeugen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1437" />
        <source>Generate a secure random password</source>
        <translation>Ein sicheres Zufallspasswort erzeugen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1442" />
        <source>Password:</source>
        <translation>Passwort:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1449" />
        <location filename="../src/bconnectionwizard.cpp" line="1455" />
        <source>(password MD5 hash will appear here)</source>
        <translation>(MD5-Hash des Passworts erscheint hier)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1450" />
        <source>MD5 Hash:</source>
        <translation>MD5-Hash:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1488" />
        <source>Remember password</source>
        <translation>Passwort merken</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1497" />
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console config: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</source>
        <translation>Director-Konfiguration: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console-Konfiguration: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1503" />
        <source>These must match your Bareos Director configuration.</source>
        <translation>Diese müssen mit Ihrer Bareos-Director-Konfiguration übereinstimmen.</translation>
    </message>
</context>
<context>
    <name>ImportConsoleSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="723" />
        <source>Select Director and Console from imported configuration</source>
        <translation>Director und Console aus der importierten Konfiguration auswählen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="728" />
        <source>The configuration will be parsed from your ZIP file. Select which Director and Console to use for the connection.</source>
        <translation>Die Konfiguration wird aus Ihrer ZIP-Datei gelesen. Wählen Sie, welcher Director und welche Console für die Verbindung verwendet werden sollen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="734" />
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="737" />
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="758" />
        <source>Select a console to view its details...</source>
        <translation>Wählen Sie eine Console, um ihre Details anzuzeigen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="800" />
        <source>No ZIP file specified.</source>
        <translation>Keine ZIP-Datei angegeben.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="806" />
        <source>Extracting and parsing ZIP archive...</source>
        <translation>ZIP-Archiv wird entpackt und gelesen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="814" />
        <source>Failed to create temporary directory.</source>
        <translation>Temporäres Verzeichnis konnte nicht erstellt werden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="823" />
        <source>Cannot read ZIP file: %1</source>
        <translation>ZIP-Datei kann nicht gelesen werden: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="830" />
        <source>Failed to extract ZIP archive.</source>
        <translation>ZIP-Archiv konnte nicht entpackt werden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="867" />
        <source>No configuration resources found in the archive.</source>
        <translation>Keine Konfigurationsressourcen im Archiv gefunden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="892" />
        <source>Parsed %1 resources: %2 Director(s), %3 Console(s)</source>
        <translation>%1 Ressourcen gelesen: %2 Director(s), %3 Console(s)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="899" />
        <source>Warning: Need at least one Director and one Console resource.</source>
        <translation>Warnung: Mindestens eine Director- und eine Console-Ressource erforderlich.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="920" />
        <source>=== Director: %1 ===
</source>
        <translation>=== Director: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="925" />
        <source>(multiple addresses)</source>
        <translation>(mehrere Adressen)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928" />
        <source>  Address: %1
</source>
        <translation>  Address: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928" />
        <source>(not set)</source>
        <translation>(nicht gesetzt)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="929" />
        <source>  Port: %1
</source>
        <translation>  Port: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="930" />
        <location filename="../src/bconnectionwizard.cpp" line="952" />
        <source>  TLS Enable: %1
</source>
        <translation>  TLS Enable: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="937" />
        <source>=== Console: %1 ===
</source>
        <translation>=== Console: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="943" />
        <source>  Password: [md5]***
</source>
        <translation>  Password: [md5]***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="945" />
        <source>  Password: ***
</source>
        <translation>  Password: ***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="948" />
        <source>  Password: (not set)
</source>
        <translation>  Password: (nicht gesetzt)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="953" />
        <source>  TLS Require: %1
</source>
        <translation>  TLS Require: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="958" />
        <source>  Profile: %1
</source>
        <translation>  Profile: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="974" />
        <source>
  --- ACLs ---
</source>
        <translation>
  --- ACLs ---
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="977" />
        <source>  %1: %2
</source>
        <translation>  %1: %2
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="982" />
        <source>
  (No ACLs defined on this console)
</source>
        <translation>
  (Keine ACLs für diese Console definiert)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1119" />
        <source>Host/Address</source>
        <translation>Host/Adresse</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1120" />
        <source>Director Name</source>
        <translation>Director-Name</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1121" />
        <source>Console Name</source>
        <translation>Console-Name</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1122" />
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1125" />
        <source>Incomplete Configuration</source>
        <translation>Unvollständige Konfiguration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1126" />
        <source>The imported configuration is missing the following required fields:

  %1

You can fill them in on the following pages.</source>
        <translation>In der importierten Konfiguration fehlen die folgenden Pflichtfelder:

  %1

Sie können sie auf den folgenden Seiten ausfüllen.</translation>
    </message>
</context>
<context>
    <name>JobWidget</name>
    <message>
        <location filename="../ui/jobwidget.ui" line="14" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
</context>
<context>
    <name>ProfileNamePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3422" />
        <source>Name this connection</source>
        <translation>Diese Verbindung benennen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3425" />
        <source>e.g., Production Server</source>
        <translation>z. B. Produktionsserver</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3428" />
        <source>Set as default</source>
        <translation>Als Standard festlegen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3432" />
        <source>Connect now</source>
        <translation>Jetzt verbinden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3442" />
        <source>Advanced Settings...</source>
        <translation>Erweiterte Einstellungen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3443" />
        <source>Configure TLS options, ACLs, and other advanced settings</source>
        <translation>TLS-Optionen, ACLs und weitere erweiterte Einstellungen konfigurieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3448" />
        <source>Export Config...</source>
        <translation>Konfiguration exportieren...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3449" />
        <source>Export Bareos configuration files as zip archive</source>
        <translation>Bareos-Konfigurationsdateien als ZIP-Archiv exportieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3456" />
        <source>This name helps identify the connection.</source>
        <translation>Dieser Name hilft, die Verbindung zu erkennen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3536" />
        <source>Console only (for existing Director)</source>
        <translation>Nur Console (für vorhandenen Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3537" />
        <source>Console + Profile (with ACLs)</source>
        <translation>Console + Profile (mit ACLs)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3538" />
        <source>Full configuration (new installation)</source>
        <translation>Vollständige Konfiguration (Neuinstallation)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3542" />
        <location filename="../src/bconnectionwizard.cpp" line="3557" />
        <source>Export Configuration</source>
        <translation>Konfiguration exportieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3543" />
        <source>Select export mode:</source>
        <translation>Exportmodus wählen:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3559" />
        <source>Zip Archives (*.zip)</source>
        <translation>ZIP-Archive (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3564" />
        <source>Export Successful</source>
        <translation>Export erfolgreich</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3565" />
        <source>Configuration exported to:
%1

Extract to your Bareos server's root directory and restart the Director.</source>
        <translation>Konfiguration exportiert nach:
%1

Entpacken Sie sie in das Wurzelverzeichnis Ihres Bareos-Servers und starten Sie den Director neu.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3569" />
        <source>Export Failed</source>
        <translation>Export fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3570" />
        <source>Failed to export configuration:
%1</source>
        <translation>Konfiguration konnte nicht exportiert werden:
%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="30" />
        <source>No files to export</source>
        <translation>Keine Dateien zum Exportieren</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="203" />
        <source>Cannot create zip file: %1</source>
        <translation>ZIP-Datei kann nicht erstellt werden: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="234" />
        <source>Error writing zip file</source>
        <translation>Fehler beim Schreiben der ZIP-Datei</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="295" />
        <source>Not set</source>
        <translation>Nicht gesetzt</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="299" />
        <source>%n year(s)</source>
        <translation>
            <numerusform>%n Jahr</numerusform>
            <numerusform>%n Jahre</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="301" />
        <source>%n day(s)</source>
        <translation>
            <numerusform>%n Tag</numerusform>
            <numerusform>%n Tage</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="305" />
        <source>%n hour(s)</source>
        <translation>
            <numerusform>%n Stunde</numerusform>
            <numerusform>%n Stunden</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="307" />
        <source>%n second(s)</source>
        <translation>
            <numerusform>%n Sekunde</numerusform>
            <numerusform>%n Sekunden</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="27" />
        <source>Add "%1"</source>
        <translation>"%1" hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="61" />
        <source>Remove "%1"</source>
        <translation>"%1" entfernen</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="63" />
        <source>Remove %1 items</source>
        <translation>%1 Einträge entfernen</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="96" />
        <source>Edit item</source>
        <translation>Eintrag bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="137" />
        <source>Move item</source>
        <translation>Eintrag verschieben</translation>
    </message>
</context>
<context>
    <name>ServerPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1154" />
        <source>Where is your Bareos Director?</source>
        <translation>Wo befindet sich Ihr Bareos Director?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1159" />
        <source>Found in the network:</source>
        <translation>Im Netzwerk gefunden:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1161" />
        <source>Search again</source>
        <translation>Erneut suchen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1171" />
        <source>Onesimus Directors announce themselves in the local network. Select one to fill in the connection data.</source>
        <translation>Onesimus Directors melden sich im lokalen Netzwerk an. Wählen Sie einen aus, um die Verbindungsdaten auszufüllen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1186" />
        <source>Searching...</source>
        <translation>Suche...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1197" />
        <source>e.g., bareos.example.com or 192.168.1.100</source>
        <translation>z. B. bareos.example.com oder 192.168.1.100</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1198" />
        <source>Host:</source>
        <translation>Host:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1204" />
        <source>Port:</source>
        <translation>Port:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1207" />
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1208" />
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1209" />
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1210" />
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1211" />
        <source>Select the operating system of the Bareos Director server.
This affects certificate paths in exported configurations.</source>
        <translation>Wählen Sie das Betriebssystem des Bareos-Director-Servers.
Dies beeinflusst die Zertifikatspfade in exportierten Konfigurationen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1213" />
        <source>Server OS:</source>
        <translation>Server-Betriebssystem:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1217" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (Bareos-Konsolenprotokoll)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1218" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1224" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Dieser Build von Onesimus hat keine gRPC-Unterstützung.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1226" />
        <source>Protocol:</source>
        <translation>Protokoll:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1232" />
        <source>gRPC port:</source>
        <translation>gRPC-Port:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1246" />
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Director-Konfiguration: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1249" />
        <location filename="../src/bconnectionwizard.cpp" line="1308" />
        <source>The default port is 9101.</source>
        <translation>Der Standardport ist 9101.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1286" />
        <source>Searching for Onesimus Directors in the network...</source>
        <translation>Suche nach Onesimus Directors im Netzwerk...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1292" />
        <source>No Onesimus Director found - enter the server manually.</source>
        <translation>Kein Onesimus Director gefunden - geben Sie den Server manuell ein.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1307" />
        <source>gRPC is encrypted with TLS; the server certificate is confirmed on first use.</source>
        <translation>gRPC ist mit TLS verschlüsselt; das Serverzertifikat wird bei der ersten Verwendung bestätigt.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1327" />
        <source>(gRPC %1)</source>
        <translation>(gRPC %1)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="1340" />
        <source>%n Director(s) found. Select one to use it.</source>
        <translation>
            <numerusform>%n Director gefunden. Wählen Sie ihn aus, um ihn zu verwenden.</numerusform>
            <numerusform>%n Directors gefunden. Wählen Sie einen aus, um ihn zu verwenden.</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>StorageWidget</name>
    <message>
        <location filename="../src/storagewidget.cpp" line="27" />
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="53" />
        <source>(due)</source>
        <translation>(fällig)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="85" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="88" />
        <source>Pool filter</source>
        <translation>Pool-Filter</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="94" />
        <source>&amp;Status:</source>
        <translation>&amp;Status:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="97" />
        <source>Status filter</source>
        <translation>Statusfilter</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="103" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="114" />
        <source>Statistics</source>
        <translation>Statistik</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="117" />
        <source>Volumes:</source>
        <translation>Volumes:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="125" />
        <source>Used:</source>
        <translation>Belegt:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="451" />
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="452" />
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="455" />
        <source>Used</source>
        <translation>Belegt</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="456" />
        <source>Limit</source>
        <translation>Grenze</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="457" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Last Written</source>
        <translation>Zuletzt geschrieben</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Retention</source>
        <translation>Aufbewahrung</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Reusable From</source>
        <translation>Wiederverwendbar ab</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Reason</source>
        <translation>Grund</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149" />
        <source>Maximum Volume Bytes of the volume; "none" = no limit</source>
        <translation>Maximum Volume Bytes des Volumes; "keine" = keine Grenze</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="151" />
        <source>From when Bareos may reuse the volume. It does so only when its pool needs a volume, so this is the earliest date, not the day of recycling.</source>
        <translation>Ab wann Bareos das Volume wiederverwenden darf. Es tut dies nur, wenn sein Pool ein Volume benötigt; daher ist dies das früheste Datum, nicht der Tag des Recyclings.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="153" />
        <source>Volumes</source>
        <translation>Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="155" />
        <source>One row per volume. Enter opens the details of the selected volume.</source>
        <translation>Eine Zeile pro Volume. Enter öffnet die Details des ausgewählten Volumes.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="212" />
        <source>All pools (%1)</source>
        <translation>Alle Pools (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="215" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="221" />
        <source>All statuses (%1)</source>
        <translation>Alle Status (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="224" />
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="247" />
        <source>Error parsing volume data</source>
        <translation>Fehler beim Lesen der Volume-Daten</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="254" />
        <source>The Director sent the short volume list: limits, job counts and use durations are unknown, and so is when an Append volume will be closed.</source>
        <translation>Der Director hat die kurze Volume-Liste gesendet: Grenzen, Job-Anzahlen und Nutzungsdauern sind unbekannt, ebenso wann ein Append-Volume geschlossen wird.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/storagewidget.cpp" line="268" />
        <source>%n volume(s) loaded</source>
        <translation>
            <numerusform>%n Volume geladen</numerusform>
            <numerusform>%n Volumes geladen</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="315" />
        <source>%1, %2, %3, last written %4, reusable from %5</source>
        <translation>%1, %2, %3, zuletzt geschrieben %4, wiederverwendbar ab %5</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="343" />
        <location filename="../src/storagewidget.cpp" line="456" />
        <location filename="../src/storagewidget.cpp" line="458" />
        <location filename="../src/storagewidget.cpp" line="464" />
        <location filename="../src/storagewidget.cpp" line="466" />
        <source>none</source>
        <translation>keine</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="351" />
        <source>%1 of %2</source>
        <translation>%1 von %2</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="408" />
        <source>Refreshing the volume list...</source>
        <translation>Volume-Liste wird aktualisiert...</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="436" />
        <source>unknown (not in this list)</source>
        <translation>unbekannt (nicht in dieser Liste)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="453" />
        <source>Media type</source>
        <translation>Medientyp</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="458" />
        <source>Maximum jobs</source>
        <translation>Maximale Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="459" />
        <source>Errors</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="460" />
        <source>First written</source>
        <translation>Zuerst geschrieben</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="461" />
        <source>Last written</source>
        <translation>Zuletzt geschrieben</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="462" />
        <source>Labeled</source>
        <translation>Beschriftet</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="463" />
        <source>Volume retention</source>
        <translation>Volume-Aufbewahrung</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="464" />
        <source>Use duration</source>
        <translation>Nutzungsdauer</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>Recycle</source>
        <translation>Recycling</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>yes</source>
        <translation>ja</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>no</source>
        <translation>nein</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="466" />
        <source>Recycle pool</source>
        <translation>Recycling-Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="467" />
        <source>Times recycled</source>
        <translation>Anzahl Recyclings</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468" />
        <source>Reusable from</source>
        <translation>Wiederverwendbar ab</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="478" />
        <source>Volume Details</source>
        <translation>Volume-Details</translation>
    </message>
    <message>
        <location filename="../ui/storagewidget.ui" line="14" />
        <location filename="../src/storagewidget.cpp" line="454" />
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
</context>
<context>
    <name>TLSPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1768" />
        <source>Where are your TLS certificates?</source>
        <translation>Wo befinden sich Ihre TLS-Zertifikate?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1774" />
        <source>/etc/bareos/ssl/ca.pem</source>
        <translation>/etc/bareos/ssl/ca.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1775" />
        <location filename="../src/bconnectionwizard.cpp" line="1786" />
        <location filename="../src/bconnectionwizard.cpp" line="1807" />
        <location filename="../src/bconnectionwizard.cpp" line="1816" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1779" />
        <source>CA Cert:</source>
        <translation>CA-Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1785" />
        <source>Path to PFX certificate file</source>
        <translation>Pfad zur PFX-Zertifikatsdatei</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1790" />
        <source>PFX File:</source>
        <translation>PFX-Datei:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1794" />
        <source>PFX file password (if encrypted)</source>
        <translation>Passwort der PFX-Datei (falls verschlüsselt)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1795" />
        <source>PFX Password:</source>
        <translation>PFX-Passwort:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1806" />
        <source>/etc/bareos/ssl/client.pem</source>
        <translation>/etc/bareos/ssl/client.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1811" />
        <source>Client Cert:</source>
        <translation>Client-Zertifikat:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1815" />
        <source>/etc/bareos/ssl/client.key</source>
        <translation>/etc/bareos/ssl/client.key</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1820" />
        <source>Private Key:</source>
        <translation>Privater Schlüssel:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1830" />
        <source>Convert PEM/DER to PFX...</source>
        <translation>PEM/DER in PFX umwandeln...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1831" />
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Separate PEM/DER-Zertifikat- und Schlüsseldateien in eine PFX-Datei umwandeln</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1836" />
        <source>Generate Certificates...</source>
        <translation>Zertifikate erzeugen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1839" />
        <source>No certificates? Click Generate to create them.</source>
        <translation>Keine Zertifikate? Klicken Sie auf Erzeugen, um sie zu erstellen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875" />
        <location filename="../src/bconnectionwizard.cpp" line="1886" />
        <location filename="../src/bconnectionwizard.cpp" line="1898" />
        <source>Missing</source>
        <translation>Fehlt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875" />
        <source>Please provide a CA certificate file.</source>
        <translation>Bitte geben Sie eine CA-Zertifikatsdatei an.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879" />
        <location filename="../src/bconnectionwizard.cpp" line="1890" />
        <location filename="../src/bconnectionwizard.cpp" line="1902" />
        <source>Not Found</source>
        <translation>Nicht gefunden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879" />
        <source>CA certificate file not found.</source>
        <translation>CA-Zertifikatsdatei nicht gefunden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1886" />
        <source>Please provide a PFX certificate file.</source>
        <translation>Bitte geben Sie eine PFX-Zertifikatsdatei an.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1890" />
        <source>PFX certificate file not found.</source>
        <translation>PFX-Zertifikatsdatei nicht gefunden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1898" />
        <source>Please provide all certificate files.</source>
        <translation>Bitte geben Sie alle Zertifikatsdateien an.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1902" />
        <source>One or more certificate files not found.</source>
        <translation>Eine oder mehrere Zertifikatsdateien nicht gefunden.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1936" />
        <source>CA Certificate</source>
        <translation>CA-Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1937" />
        <location filename="../src/bconnectionwizard.cpp" line="1958" />
        <source>Certificates (*.pem *.crt *.cer);;All Files (*)</source>
        <translation>Zertifikate (*.pem *.crt *.cer);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1954" />
        <source>Select PFX Certificate</source>
        <translation>PFX-Zertifikat auswählen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1955" />
        <source>PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)</source>
        <translation>PFX-Dateien (*.pfx);;Zertifikate (*.pem *.crt);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1957" />
        <source>Client Certificate</source>
        <translation>Client-Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1975" />
        <source>Private Key</source>
        <translation>Privater Schlüssel</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1976" />
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Private Schlüssel (*.pem *.key);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2008" />
        <source>PFX File Created</source>
        <translation>PFX-Datei erstellt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2009" />
        <source>PFX file has been created successfully.

The certificate path has been updated.</source>
        <translation>Die PFX-Datei wurde erfolgreich erstellt.

Der Zertifikatspfad wurde aktualisiert.</translation>
    </message>
</context>
<context>
    <name>TemplateSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="375" />
        <source>How would you like to create the connection?</source>
        <translation>Wie möchten Sie die Verbindung erstellen?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="377" />
        <source>You can start from scratch, use a template from an existing connection, or import from a ZIP file.</source>
        <translation>Sie können von Grund auf beginnen, eine Vorlage aus einer vorhandenen Verbindung verwenden oder aus einer ZIP-Datei importieren.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="381" />
        <source>Create new connection from scratch</source>
        <translation>Neue Verbindung von Grund auf erstellen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="382" />
        <source>Use template from existing connection</source>
        <translation>Vorlage aus vorhandener Verbindung verwenden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="383" />
        <source>Import from ZIP file</source>
        <translation>Aus ZIP-Datei importieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="405" />
        <source>Refresh</source>
        <translation>Aktualisieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="428" />
        <source>Select ZIP file...</source>
        <translation>ZIP-Datei auswählen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="431" />
        <source>Browse...</source>
        <translation>Durchsuchen...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="542" />
        <source>Director: %1</source>
        <translation>Director: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="543" />
        <source>Console: %1</source>
        <translation>Console: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>TLS: %1</source>
        <translation>TLS: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <source>PSK</source>
        <translation>PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <source>Certificate</source>
        <translation>Zertifikat</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>Disabled</source>
        <translation>Deaktiviert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>Enabled</source>
        <translation>Aktiviert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="601" />
        <source>System: %1</source>
        <translation>System: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="707" />
        <source>Select Director Configuration ZIP</source>
        <translation>ZIP mit Director-Konfiguration auswählen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="709" />
        <source>ZIP Files (*.zip);;All Files (*)</source>
        <translation>ZIP-Dateien (*.zip);;Alle Dateien (*)</translation>
    </message>
</context>
<context>
    <name>TestPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2682" />
        <location filename="../src/bconnectionwizard.cpp" line="2699" />
        <location filename="../src/bconnectionwizard.cpp" line="2729" />
        <location filename="../src/bconnectionwizard.cpp" line="2838" />
        <source>Test Connection</source>
        <translation>Verbindung testen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2686" />
        <source>Let's test your connection.</source>
        <translation>Testen wir Ihre Verbindung.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2690" />
        <location filename="../src/bconnectionwizard.cpp" line="2726" />
        <source>Click the button to test.</source>
        <translation>Klicken Sie auf die Schaltfläche, um zu testen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2749" />
        <source>Connecting...</source>
        <translation>Verbinde...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2752" />
        <source>Starting test...</source>
        <translation>Test wird gestartet...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2791" />
        <source>Mode: TLS-Cert</source>
        <translation>Modus: TLS-Cert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2803" />
        <source>Mode: TLS-PSK with cipher list: %1</source>
        <translation>Modus: TLS-PSK mit Cipher-Liste: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2805" />
        <source>Mode: TLS-PSK</source>
        <translation>Modus: TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2818" />
        <source>Mode: gRPC (TLS) on port %1</source>
        <translation>Modus: gRPC (TLS) auf Port %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2819" />
        <location filename="../src/bconnectionwizard.cpp" line="2821" />
        <source>Connecting to %1:%2...</source>
        <translation>Verbinde mit %1:%2...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2857" />
        <source>Cancelled.</source>
        <translation>Abgebrochen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2865" />
        <source>Connecting</source>
        <translation>Verbinden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2866" />
        <source>Authenticating</source>
        <translation>Authentifizieren</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2867" />
        <source>Setting API</source>
        <translation>API einstellen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2868" />
        <source>Loading</source>
        <translation>Laden</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2869" />
        <source>Ready</source>
        <translation>Bereit</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2891" />
        <source>Server certificate trusted.</source>
        <translation>Serverzertifikat vertrauenswürdig.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2895" />
        <source>Server certificate not trusted - connection cancelled.</source>
        <translation>Serverzertifikat nicht vertrauenswürdig - Verbindung abgebrochen.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2896" />
        <location filename="../src/bconnectionwizard.cpp" line="2909" />
        <location filename="../src/bconnectionwizard.cpp" line="2915" />
        <source>Failed</source>
        <translation>Fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2908" />
        <source>Auth OK</source>
        <translation>Authentifizierung OK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2909" />
        <source>Auth failed: %1</source>
        <translation>Authentifizierung fehlgeschlagen: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2914" />
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2925" />
        <source>Success!</source>
        <translation>Erfolg!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2926" />
        <source>Connection successful!</source>
        <translation>Verbindung erfolgreich!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2929" />
        <source>Test Again</source>
        <translation>Erneut testen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2937" />
        <source>Timeout</source>
        <translation>Zeitüberschreitung</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2938" />
        <source>Timed out</source>
        <translation>Zeitüberschreitung</translation>
    </message>
</context>
<context>
    <name>WelcomePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="353" />
        <source>Welcome</source>
        <translation>Willkommen</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="359" />
        <source>Let's set up a connection to your Bareos Director.

You'll need:
  - The hostname or IP of your Director
  - Your console name and password

Click Next to begin.</source>
        <translation>Richten wir eine Verbindung zu Ihrem Bareos Director ein.

Sie benötigen:
  - Den Hostnamen oder die IP-Adresse Ihres Directors
  - Ihren Console-Namen und Ihr Passwort

Klicken Sie auf Weiter, um zu beginnen.</translation>
    </message>
</context>
</TS>