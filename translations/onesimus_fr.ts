<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR" sourcelanguage="en">
<context>
    <name>AuthMethodPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1589" />
        <source>How should we authenticate?</source>
        <translation>Comment voulez-vous vous authentifier ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1593" />
        <source>TLS-PSK (recommended)</source>
        <translation>TLS-PSK (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1598" />
        <source>TLS with certificates</source>
        <translation>TLS avec certificats</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1615" />
        <source>TLS Cipher List (optional):</source>
        <translation>Liste de chiffrements TLS (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1622" />
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>p. ex. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1623" />
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Click Preset to use recommended ciphers.</source>
        <translation>Liste de chiffrements TLS-PSK séparés par des deux-points.
Laisser vide pour la détection automatique.
Cliquez sur Préréglage pour utiliser les chiffrements recommandés.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1628" />
        <source>Preset</source>
        <translation>Préréglage</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1629" />
        <source>Fill with recommended PSK ciphers</source>
        <translation>Remplir avec les chiffrements PSK recommandés</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1638" />
        <source>TLS-PSK is the default for Bareos 18.2+.</source>
        <translation>TLS-PSK est la valeur par défaut depuis Bareos 18.2.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1696" />
        <source>Server supports TLS encryption.</source>
        <translation>Le serveur prend en charge le chiffrement TLS.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1704" />
        <source>TLS-PSK - not available on this server</source>
        <translation>TLS-PSK - non disponible sur ce serveur</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1705" />
        <source>This server doesn't appear to support TLS-PSK. Consider upgrading to Bareos 18.2 or later.</source>
        <translation>Ce serveur ne semble pas prendre en charge TLS-PSK. Envisagez une mise à niveau vers Bareos 18.2 ou ultérieur.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1713" />
        <source>Server check failed: %1</source>
        <translation>Échec de la vérification du serveur : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1730" />
        <source>TLS-PSK uses your password to encrypt the connection.</source>
        <translation>TLS-PSK utilise votre mot de passe pour chiffrer la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1734" />
        <source>You'll need CA, client certificate, and private key.</source>
        <translation>Vous aurez besoin d'une CA, d'un certificat client et d'une clé privée.</translation>
    </message>
</context>
<context>
    <name>B321Dialog</name>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="88" />
        <location filename="../src/tools/b321dialog.cpp" line="575" />
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="109" />
        <location filename="../src/tools/b321dialog.cpp" line="632" />
        <source>%1 (not in the volume list)</source>
        <translation>%1 (absent de la liste des volumes)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="111" />
        <location filename="../src/tools/b321dialog.cpp" line="649" />
        <source>media type unknown</source>
        <translation>type de support inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="112" />
        <source>storage unknown</source>
        <translation>Storage inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="115" />
        <source>off site (marked)</source>
        <translation>hors site (marqué)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="117" />
        <source>off site (location %1)</source>
        <translation>hors site (emplacement %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="119" />
        <source>on site</source>
        <translation>sur site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="120" />
        <source>off site unknown</source>
        <translation>hors site inconnu</translation>
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
        <translation>Vérificateur 3-2-1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="133" />
        <source>Checks for every backup job whether there are 3 copies of the data, on 2 media types, with 1 copy off site. Read only.</source>
        <translation>Vérifie pour chaque job de sauvegarde s'il existe 3 copies des données, sur 2 types de support, dont 1 copie hors site. Lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="154" />
        <source>Data state</source>
        <translation>État des données</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="156" />
        <source>&amp;Refresh</source>
        <translation>&amp;Actualiser</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="158" />
        <source>Reload the jobs, the volumes of the jobs and the volume list (read only)</source>
        <translation>Recharger les jobs, les volumes des jobs et la liste des volumes (lecture seule)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="170" />
        <source>&amp;Backup jobs:</source>
        <translation>Jobs de &amp;sauvegarde :</translation>
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
        <translation>Copies</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174" />
        <source>Media Types</source>
        <translation>Types de support</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Off Site</source>
        <translation>Hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Verdict</source>
        <translation>Verdict</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175" />
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177" />
        <source>Copies of the data a restore would use: the chain, its complete copies and, if set, the original data. 3 are needed.</source>
        <translation>Copies des données qu'une restauration utiliserait : la chaîne, ses copies complètes et, si activé, les données d'origine. 3 sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="180" />
        <source>Distinct media types of the volumes of the chain and its complete copies. 2 are needed.</source>
        <translation>Types de support distincts des volumes de la chaîne et de ses copies complètes. 2 sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="182" />
        <source>Copies off site, by your marking of the storages or a location of the volume. 1 is needed.</source>
        <translation>Copies hors site, d'après votre marquage des Storages ou un emplacement du volume. 1 est nécessaire.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="185" />
        <source>3-2-1 check per backup job</source>
        <translation>Vérification 3-2-1 par job de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="186" />
        <source>One row per backup job: copies, media types, off site, verdict and reason. The details of the selected job follow below the table.</source>
        <translation>Une ligne par job de sauvegarde : copies, types de support, hors site, verdict et raison. Les détails du job sélectionné suivent sous le tableau.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="221" />
        <source>C&amp;hain and copies:</source>
        <translation>C&amp;haîne et copies :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="226" />
        <source>Chain and copies of the selected job</source>
        <translation>Chaîne et copies du job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="227" />
        <source>The jobs a restore would use and the copies of them, with the volumes they lie on</source>
        <translation>Les jobs qu'une restauration utiliserait et leurs copies, avec les volumes sur lesquels ils se trouvent</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="237" />
        <source>&amp;Why, and what to do:</source>
        <translation>&amp;Pourquoi, et que faire :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="241" />
        <source>Reasons, suggestions and hints for the selected job</source>
        <translation>Raisons, suggestions et conseils pour le job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="250" />
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="252" />
        <source>Count the &amp;original data as a copy</source>
        <translation>Compter les données d'&amp;origine comme une copie</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="253" />
        <source>The 3-2-1 rule is usually stated as the production data plus 2 backups. Off: 3 backups are needed.</source>
        <translation>La règle 3-2-1 s'énonce généralement comme les données de production plus 2 sauvegardes. Désactivé : 3 sauvegardes sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="258" />
        <source>O&amp;ff-site storages:</source>
        <translation>Storages &amp;hors site :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="260" />
        <source>Bareos does not record where a storage stands. Tick the storages that are at another site.</source>
        <translation>Bareos n'enregistre pas où se trouve un Storage. Cochez les Storages situés sur un autre site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="267" />
        <source>Off-site storages</source>
        <translation>Storages hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="268" />
        <source>A checked storage is off site. Space toggles.</source>
        <translation>Un Storage coché est hors site. Espace pour basculer.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="271" />
        <source>&amp;Unmarked storages are on site</source>
        <translation>Les Storages non &amp;marqués sont sur site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="272" />
        <source>Without this, a storage that is not ticked is "unknown", not "on site".</source>
        <translation>Sans cette option, un Storage non coché est « inconnu », et non « sur site ».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="282" />
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="319" />
        <location filename="../src/tools/b321dialog.cpp" line="435" />
        <source>%1, off site</source>
        <translation>%1, hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="320" />
        <location filename="../src/tools/b321dialog.cpp" line="436" />
        <source>%1, not marked off site</source>
        <translation>%1, non marqué hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="441" />
        <source>No storage known yet</source>
        <translation>Aucun Storage connu pour l'instant</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="468" />
        <location filename="../src/tools/b321dialog.cpp" line="517" />
        <source>Jobs: %1</source>
        <translation>Jobs : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="487" />
        <location filename="../src/tools/b321dialog.cpp" line="520" />
        <source>Volumes of the jobs: %1</source>
        <translation>Volumes des jobs : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="502" />
        <location filename="../src/tools/b321dialog.cpp" line="523" />
        <source>Volume list: %1</source>
        <translation>Liste des volumes : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="554" />
        <source>%1: %2. %3</source>
        <translation>%1 : %2. %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="558" />
        <source>%1/3</source>
        <translation>%1/3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="561" />
        <source>Copies: %1 of 3, %2</source>
        <translation>Copies : %1 sur 3, %2</translation>
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
        <translation>Types de support : %1 sur 2%2, %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="576" />
        <source>%1/1</source>
        <translation>%1/1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="580" />
        <source>Off site: unknown</source>
        <translation>Hors site : inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="581" />
        <source>Off site: %1 of 1, %2</source>
        <translation>Hors site : %1 sur 1, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="587" />
        <source>Verdict: %1</source>
        <translation>Verdict : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="611" />
        <source>No job selected.</source>
        <translation>Aucun job sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="623" />
        <source>volumes not loaded</source>
        <translation>volumes non chargés</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="625" />
        <source>no volume recorded</source>
        <translation>aucun volume enregistré</translation>
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
            <numerusform>Chaîne (la sauvegarde elle-même) : %n job</numerusform>
            <numerusform>Chaîne (la sauvegarde elle-même) : %n jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642" />
        <source>Copy in pool %1: complete</source>
        <translation>Copie dans le pool %1 : complète</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642" />
        <location filename="../src/tools/b321dialog.cpp" line="645" />
        <source>(not reported)</source>
        <translation>(non indiqué)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="644" />
        <source>Copy in pool %1: partial, %2 of %3 jobs</source>
        <translation>Copie dans le pool %1 : partielle, %2 jobs sur %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="653" />
        <source>last job ended %1</source>
        <translation>dernier job terminé %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="654" />
        <source>copied %1</source>
        <translation>copié %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="655" />
        <source>%1 — %2</source>
        <translation>%1 — %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="662" />
        <source>%1 %2, started %3: %4</source>
        <translation>%1 %2, démarré %3 : %4</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="665" />
        <source> (migrated from job %1)</source>
        <translation> (migré depuis le job %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="669" />
        <source>Copy %1 of %2 %3, copied %4: %5</source>
        <translation>Copie %1 de %2 %3, copiée %4 : %5</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="678" />
        <source>No copy of %1 %2</source>
        <translation>Aucune copie de %1 %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="684" />
        <source>No restorable chain in the job history</source>
        <translation>Aucune chaîne restaurable dans l'historique des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="689" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="696" />
        <source>Why</source>
        <translation>Pourquoi</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="697" />
        <source>What to do</source>
        <translation>Que faire</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="698" />
        <source>Hints (not part of the verdict)</source>
        <translation>Conseils (ne font pas partie du verdict)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="707" />
        <source>Not everything could be loaded: %1</source>
        <translation>Tout n'a pas pu être chargé : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="711" />
        <source>jobs</source>
        <translation>jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="712" />
        <source>volumes of the jobs</source>
        <translation>volumes des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="713" />
        <source>volume list</source>
        <translation>liste des volumes</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="714" />
        <source>Loading: %1 …</source>
        <translation>Chargement : %1 …</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="717" />
        <source>No job data yet. Refresh loads it (read only).</source>
        <translation>Aucune donnée de job pour l'instant. Actualiser les charge (lecture seule).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="718" />
        <source>Not connected to a Director.</source>
        <translation>Non connecté à un Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="726" />
        <source>%n job(s) checked: %1 pass, %2 fail, %3 unknown. </source>
        <translation>
            <numerusform>%n job vérifié : %1 réussi(s), %2 échoué(s), %3 inconnu(s). </numerusform>
            <numerusform>%n jobs vérifiés : %1 réussi(s), %2 échoué(s), %3 inconnu(s). </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="728" />
        <source>From %n job record(s) of the last %1 days</source>
        <translation>
            <numerusform>À partir de %n enregistrement de job des %1 derniers jours</numerusform>
            <numerusform>À partir de %n enregistrements de jobs des %1 derniers jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="729" />
        <source>, %n volume(s)</source>
        <translation>
            <numerusform>, %n volume</numerusform>
            <numerusform>, %n volumes</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="730" />
        <source>, the volumes of %n job(s)</source>
        <translation>
            <numerusform>, les volumes de %n job</numerusform>
            <numerusform>, les volumes de %n jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="732" />
        <source>. Read only.</source>
        <translation>. Lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="13" />
        <source>3-2-1 &amp;Verifier...</source>
        <translation>&amp;Vérificateur 3-2-1...</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="14" />
        <source>Check for every backup job: 3 copies, 2 media types, 1 copy off site (read only)</source>
        <translation>Vérifier pour chaque job de sauvegarde : 3 copies, 2 types de support, 1 copie hors site (lecture seule)</translation>
    </message>
</context>
<context>
    <name>B321Verifier</name>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="90" />
        <source>unknown time</source>
        <translation>heure inconnue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="96" />
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="109" />
        <source>(pool not reported)</source>
        <translation>(pool non indiqué)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="411" />
        <source>No successful Full backup of %1 in the job history of the last %n day(s): there is no chain to restore.</source>
        <translation>
            <numerusform>Aucune sauvegarde Full réussie de %1 dans l'historique des jobs du dernier %n jour : il n'y a aucune chaîne à restaurer.</numerusform>
            <numerusform>Aucune sauvegarde Full réussie de %1 dans l'historique des jobs des %n derniers jours : il n'y a aucune chaîne à restaurer.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="413" />
        <source>No successful Full backup of %1 in the job history: there is no chain to restore.</source>
        <translation>Aucune sauvegarde Full réussie de %1 dans l'historique des jobs : il n'y a aucune chaîne à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="415" />
        <source>Run a Full backup of %1 and check that it terminates normally.</source>
        <translation>Lancez une sauvegarde Full de %1 et vérifiez qu'elle se termine normalement.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="417" />
        <source>No successful Full backup in the job history</source>
        <translation>Aucune sauvegarde Full réussie dans l'historique des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="419" />
        <source>Only the original data on the client counts: 1 of 3 copies.</source>
        <translation>Seules les données d'origine sur le client comptent : 1 copie sur 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="432" />
        <source>The chain a restore would use today is %1</source>
        <translation>La chaîne qu'une restauration utiliserait aujourd'hui est %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="433" />
        <source>, a Differential</source>
        <translation>, une Differential</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="434" />
        <source> and %n Incremental(s) after it</source>
        <translation>
            <numerusform> et %n Incremental après celle-ci</numerusform>
            <numerusform> et %n Incremental après celle-ci</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="435" />
        <source>; it counts as 1 copy.</source>
        <translation>; elle compte comme 1 copie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="438" />
        <source>The original data on the client counts as 1 copy (setting "Count the original data as a copy").</source>
        <translation>Les données d'origine sur le client comptent comme 1 copie (paramètre « Compter les données d'origine comme une copie »).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="441" />
        <source>The original data on the client is not counted (setting "Count the original data as a copy" is off).</source>
        <translation>Les données d'origine sur le client ne sont pas comptées (le paramètre « Compter les données d'origine comme une copie » est désactivé).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="447" />
        <source>job %1 from job %2</source>
        <translation>job %1 issu du job %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="449" />
        <source>Migrated chain jobs (%1): a Migrate moves the data and adds no copy; the old job is not restored any more.</source>
        <translation>Jobs de la chaîne migrés (%1) : un Migrate déplace les données et n'ajoute aucune copie ; l'ancien job n'est plus restauré.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="455" />
        <source>Pool %1 holds a copy of every job of the chain; it counts as 1 copy.</source>
        <translation>Le pool %1 contient une copie de chaque job de la chaîne ; il compte comme 1 copie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="459" />
        <source>Pool %1 holds copies of %2 of the %3 jobs of the chain (missing: %4); a partial copy does not count.</source>
        <translation>Le pool %1 contient des copies de %2 des %3 jobs de la chaîne (manquants : %4) ; une copie partielle ne compte pas.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="465" />
        <source>No Copy job has copied the chain.</source>
        <translation>Aucun job Copy n'a copié la chaîne.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="467" />
        <source>Copies: %1 of 3.</source>
        <translation>Copies : %1 sur 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="473" />
        <source>The volumes of the chain are not known, so neither are their media types.</source>
        <translation>Les volumes de la chaîne ne sont pas connus, leurs types de support non plus.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="474" />
        <source>The volumes of the jobs (JobMedia) were not loaded: media types unknown.</source>
        <translation>Les volumes des jobs (JobMedia) n'ont pas été chargés : types de support inconnus.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="476" />
        <source>Media types of the chain and its complete copies: %1.</source>
        <translation>Types de support de la chaîne et de ses copies complètes : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="479" />
        <source>A copy that spans several media types counts once; copies on media types of their own: %1.</source>
        <translation>Une copie répartie sur plusieurs types de support compte une seule fois ; copies sur des types de support propres : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="483" />
        <source>They lie on the storages %1; the same media type on several storages counts as one type.</source>
        <translation>Elles se trouvent sur les Storages %1 ; le même type de support sur plusieurs Storages compte comme un seul type.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="488" />
        <source>Some volumes of these jobs are not in the volume list or have no media type.</source>
        <translation>Certains volumes de ces jobs ne figurent pas dans la liste des volumes ou n'ont pas de type de support.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="510" />
        <source>storage %1 marked off site</source>
        <translation>Storage %1 marqué hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="511" />
        <source>volume %1 has location %2 in the catalog</source>
        <translation>le volume %1 a l'emplacement %2 dans le catalogue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="515" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="516" />
        <source>the chain</source>
        <translation>la chaîne</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="517" />
        <source>the copy in pool %1</source>
        <translation>la copie dans le pool %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="520" />
        <source>Off site: %1.</source>
        <translation>Hors site : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="524" />
        <source>Off site is unknown: no storage of the chain or its copies is marked off site and no volume has a location in the catalog. Bareos itself does not record whether a storage is off site.</source>
        <translation>Hors site inconnu : aucun Storage de la chaîne ou de ses copies n'est marqué hors site et aucun volume n'a d'emplacement dans le catalogue. Bareos lui-même n'enregistre pas si un Storage est hors site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="529" />
        <source>Nothing is off site: every copy has a volume on a storage that is on site.</source>
        <translation>Rien n'est hors site : chaque copie a un volume sur un Storage situé sur site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="535" />
        <source>the present one</source>
        <translation>l'actuelle</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="538" />
        <source>Pool %1 lacks copies of the jobs %2: run its Copy job again (Selection Type = PoolUncopiedJobs copies what is missing) so that the whole chain is copied.</source>
        <translation>Il manque au pool %1 des copies des jobs %2 : relancez son job Copy (Selection Type = PoolUncopiedJobs copie ce qui manque) afin que toute la chaîne soit copiée.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="546" />
        <source>Add a Copy job to a second storage with a different media type than %1 (for example tape, a removable disk or cloud storage with a Media Type of its own) (needed: %2 more).</source>
        <translation>Ajoutez un job Copy vers un second Storage ayant un type de support différent de %1 (par exemple une bande, un disque amovible ou un stockage cloud avec son propre Media Type) (encore %2 nécessaire(s)).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="551" />
        <source>Add a Copy job to a second storage (needed: %1 more).</source>
        <translation>Ajoutez un job Copy vers un second Storage (encore %1 nécessaire(s)).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="555" />
        <source>Write one of the copies to a storage with another media type than %1.</source>
        <translation>Écrivez l'une des copies sur un Storage ayant un autre type de support que %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="559" />
        <source>Refresh: the volumes of the jobs and the volume list are needed to tell the media types.</source>
        <translation>Actualiser : les volumes des jobs et la liste des volumes sont nécessaires pour déterminer les types de support.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="564" />
        <source>Mark storage %1 as off site if it is at another site; if it is not, tick "Unmarked storages are on site".</source>
        <translation>Marquez le Storage %1 comme hors site s'il se trouve sur un autre site ; sinon, cochez « Les Storages non marqués sont sur site ».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="569" />
        <source>Refresh: the storages of the volumes are needed to tell where the copies are.</source>
        <translation>Actualiser : les Storages des volumes sont nécessaires pour savoir où se trouvent les copies.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="573" />
        <source>Copy the chain to a storage at another site and mark that storage as off site (or take a volume off site and record its location).</source>
        <translation>Copiez la chaîne vers un Storage situé sur un autre site et marquez ce Storage comme hors site (ou emportez un volume hors site et enregistrez son emplacement).</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="593" />
        <source>The newest Full (job %1) is %n day(s) old</source>
        <translation>
            <numerusform>La Full la plus récente (job %1) date de %n jour</numerusform>
            <numerusform>La Full la plus récente (job %1) date de %n jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="595" />
        <source>; this job's Fulls ran every %n day(s) (median of the loaded history).</source>
        <translation>
            <numerusform>; les Full de ce job ont été exécutées tous les %n jour (médiane de l'historique chargé).</numerusform>
            <numerusform>; les Full de ce job ont été exécutées tous les %n jours (médiane de l'historique chargé).</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="598" />
        <source> It is overdue.</source>
        <translation> Elle est en retard.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="601" />
        <source>The newest Full (job %1) is %n day(s) old; it is the only successful Full in the loaded history.</source>
        <translation>
            <numerusform>La Full la plus récente (job %1) date de %n jour ; c'est la seule Full réussie de l'historique chargé.</numerusform>
            <numerusform>La Full la plus récente (job %1) date de %n jours ; c'est la seule Full réussie de l'historique chargé.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="628" />
        <source>The newest copy of this job (job %1 in pool %2, a copy of job %3) was made %n day(s) ago.</source>
        <translation>
            <numerusform>La copie la plus récente de ce job (job %1 dans le pool %2, une copie du job %3) a été faite il y a %n jour.</numerusform>
            <numerusform>La copie la plus récente de ce job (job %1 dans le pool %2, une copie du job %3) a été faite il y a %n jours.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="638" />
        <source>That copy belongs to an earlier chain: the current chain (from job %1) has no complete copy in pool %2.</source>
        <translation>Cette copie appartient à une chaîne antérieure : la chaîne actuelle (à partir du job %1) n'a pas de copie complète dans le pool %2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="667" />
        <source>The chain's %1 (job %2)</source>
        <translation>%1 de la chaîne (job %2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="669" />
        <source>The copy (job %1, pool %2) of the chain's %3</source>
        <translation>La copie (job %1, pool %2) de %3 de la chaîne</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="671" />
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="673" />
        <source>%1 is on volume %2, which is in status Error.</source>
        <translation>%1 se trouve sur le volume %2, qui est à l'état Error.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="676" />
        <source>The chain's Full is on volume %1, which is in Error: run a new Full backup and check the volume.</source>
        <translation>La Full de la chaîne se trouve sur le volume %1, qui est en Error : lancez une nouvelle sauvegarde Full et vérifiez le volume.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="681" />
        <source>%1 is on volume %2, which is %3: Bareos may overwrite it.</source>
        <translation>%1 se trouve sur le volume %2, qui est %3 : Bareos peut l'écraser.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="690" />
        <source>The job list hit its row limit: older jobs and copies may be missing from this check.</source>
        <translation>La liste des jobs a atteint sa limite de lignes : des jobs et copies plus anciens peuvent manquer dans cette vérification.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="696" />
        <source>%1 of 3 copies</source>
        <translation>%1 copies sur 3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="697" />
        <source>partial copies: %1</source>
        <translation>copies partielles : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="699" />
        <source>media types unknown</source>
        <translation>types de support inconnus</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="701" />
        <source>media: %1 of 2 (%2)</source>
        <translation>supports : %1 sur 2 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="704" />
        <source>off site: %1 of 1</source>
        <translation>hors site : %1 sur 1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="705" />
        <source>off site unknown</source>
        <translation>hors site inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="706" />
        <source>nothing off site</source>
        <translation>rien hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="859" />
        <source>met</source>
        <translation>respectée</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="860" />
        <source>not met</source>
        <translation>non respectée</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="863" />
        <location filename="../src/tools/b321verifier.cpp" line="895" />
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="869" />
        <source>Passes</source>
        <translation>Réussit</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="870" />
        <source>Fails</source>
        <translation>Échoue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="873" />
        <source>Unknown</source>
        <translation>Inconnu</translation>
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
        <translation>hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="892" />
        <source>on site</source>
        <translation>sur site</translation>
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
        <translation>Commandes</translation>
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
        <translation>Le Director n'a pas répondu.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="84" />
        <source>The Director did not answer with ACLs: %1</source>
        <translation>Le Director n'a pas répondu avec des ACL : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="93" />
        <source>The Director refused ".acl list": %1</source>
        <translation>Le Director a refusé ".acl list" : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="98" />
        <source>The reply holds no list of consoles ("acl").</source>
        <translation>La réponse ne contient pas de liste de consoles ("acl").</translation>
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
        <translation>Verrouillée : l'ACL du catalogue de la console n'a pas pu être lue ; elle ne peut exécuter que status, .status, version et .api.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325" />
        <source>Allowed (decided by the Director).</source>
        <translation>Autorisé (décidé par le Director).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325" />
        <location filename="../src/director/baclmatrix.cpp" line="338" />
        <source>Allowed by %1.</source>
        <translation>Autorisé par %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="328" />
        <source>The file ACL allows it (%1), the catalog ACL does not.</source>
        <translation>L'ACL du fichier l'autorise (%1), l'ACL du catalogue non.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="330" />
        <location filename="../src/director/baclmatrix.cpp" line="339" />
        <source>Denied by %1.</source>
        <translation>Refusé par %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="333" />
        <location filename="../src/director/baclmatrix.cpp" line="341" />
        <source>The console has no ACL of this type: nothing is allowed.</source>
        <translation>La console n'a pas d'ACL de ce type : rien n'est autorisé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="334" />
        <location filename="../src/director/baclmatrix.cpp" line="342" />
        <source>No entry of the ACL allows it.</source>
        <translation>Aucune entrée de l'ACL ne l'autorise.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="360" />
        <source>Console %1</source>
        <translation>Console %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="361" />
        <source>Profiles: none</source>
        <translation>Profils : aucun</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="362" />
        <source>Profiles: %1</source>
        <translation>Profils : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="364" />
        <source>Locked: its catalog ACL could not be read.</source>
        <translation>Verrouillée : son ACL du catalogue n'a pas pu être lue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="367" />
        <source>loaded</source>
        <translation>chargée</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="368" />
        <source>off</source>
        <translation>désactivée</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="369" />
        <source>table missing</source>
        <translation>table manquante</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="370" />
        <source>failed</source>
        <translation>échec</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="371" />
        <source>Catalog ACL: %1</source>
        <translation>ACL du catalogue : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="372" />
        <source>Catalog ACL: %1 (%2)</source>
        <translation>ACL du catalogue : %1 (%2)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrix.cpp" line="377" />
        <source>%n catalog ACL entry(s) narrow the file ACL.</source>
        <translation>
            <numerusform>%n entrée de l'ACL du catalogue restreint l'ACL du fichier.</numerusform>
            <numerusform>%n entrées de l'ACL du catalogue restreignent l'ACL du fichier.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382" />
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="392" />
        <source>The Director did not answer with profiles: %1</source>
        <translation>Le Director n'a pas répondu avec des profils : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="399" />
        <source>The reply holds no profiles.</source>
        <translation>La réponse ne contient pas de profils.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="415" />
        <source>Profile %1</source>
        <translation>Profile %1</translation>
    </message>
</context>
<context>
    <name>BAclMatrixDialog</name>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="63" />
        <source>Change the catalog ACL</source>
        <translation>Modifier l'ACL du catalogue</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="65" />
        <source>These changes of the catalog ACL are sent to the Director, one after the other. The file ACL is not changed. The first change the Director refuses stops the rest.</source>
        <translation>Ces modifications de l'ACL du catalogue sont envoyées au Director l'une après l'autre. L'ACL du fichier n'est pas modifiée. La première modification refusée par le Director arrête le reste.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="77" />
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="99" />
        <source>New console</source>
        <translation>Nouvelle console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="101" />
        <source>The Director creates a Console resource with this name, password and profile (configure add console). The profile's ACLs are the frame of the console; the matrix can narrow them further through the catalog ACL afterwards.</source>
        <translation>Le Director crée une ressource Console avec ce nom, ce mot de passe et ce profil (configure add console). Les ACL du profil sont le cadre de la console ; la matrice peut ensuite les restreindre davantage par l'ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="113" />
        <source>e.g. backup-viewer</source>
        <translation>p. ex. backup-viewer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="114" />
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="120" />
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="121" />
        <source>Generates another password</source>
        <translation>Génère un autre mot de passe</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="124" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="190" />
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="125" />
        <source>Copies the password to the clipboard</source>
        <translation>Copie le mot de passe dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="130" />
        <source>&amp;Password:</source>
        <translation>&amp;Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="136" />
        <source>The Profile resource whose ACLs the console gets</source>
        <translation>La ressource Profile dont la console reçoit les ACL</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="137" />
        <source>P&amp;rofile:</source>
        <translation>P&amp;rofil :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="139" />
        <source>Copy the password now: whoever uses the console needs it, and it cannot be read back here.</source>
        <translation>Copiez le mot de passe maintenant : la personne qui utilise la console en a besoin, et il ne peut pas être relu ici.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="147" />
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="151" />
        <source>Enter a name.</source>
        <translation>Saisissez un nom.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="152" />
        <source>A console of this name exists.</source>
        <translation>Une console de ce nom existe déjà.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="153" />
        <source>Choose a profile.</source>
        <translation>Choisissez un profil.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="174" />
        <source>Proposed configuration text</source>
        <translation>Texte de configuration proposé</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="176" />
        <source>Profiles are changed in the configuration files of the Director only. Put this text in place of the Profile resource in its file (the path is a suggestion), check it with the Director's test (-t) and reload; then Refresh shows the new state. Comments of the old resource are not in this text.</source>
        <translation>Les profils ne se modifient que dans les fichiers de configuration du Director. Mettez ce texte à la place de la ressource Profile dans son fichier (le chemin est une suggestion), vérifiez-le avec le test du Director (-t) et rechargez ; Actualiser montre alors le nouvel état. Les commentaires de l'ancienne ressource ne figurent pas dans ce texte.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="199" />
        <source>(no entries: no restriction)</source>
        <translation>(aucune entrée : aucune restriction)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="227" />
        <source>ACL matrix of the consoles</source>
        <translation>Matrice des ACL des consoles</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="233" />
        <source>Consoles</source>
        <translation>Consoles</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="234" />
        <source>Profiles</source>
        <translation>Profils</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="235" />
        <source>Consoles: what each console may use, the catalog ACL can be changed. Profiles: the ACLs of the Profile resources, changed as proposed configuration text.</source>
        <translation>Consoles : ce que chaque console peut utiliser, l'ACL du catalogue est modifiable. Profils : les ACL des ressources Profile, modifiées sous forme de texte de configuration proposé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="243" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="245" />
        <source>Filter resources</source>
        <translation>Filtrer les ressources</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="248" />
        <source>Only rows where the consoles differ</source>
        <translation>Seulement les lignes où les consoles diffèrent</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="251" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="252" />
        <source>Reads the ACLs of every console again (.acl list)</source>
        <translation>Relit les ACL de toutes les consoles (.acl list)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="254" />
        <source>Reading the ACLs of the consoles...</source>
        <translation>Lecture des ACL des consoles...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="258" />
        <source>New console...</source>
        <translation>Nouvelle console...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="259" />
        <source>Creates a console on the Director with a password and a profile (configure add console); it appears as a new column</source>
        <translation>Crée sur le Director une console avec mot de passe et profil (configure add console) ; elle apparaît comme nouvelle colonne</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="266" />
        <source>ACL matrix: resources by console</source>
        <translation>Matrice des ACL : ressources par console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="282" />
        <source>✓ allowed   ✗ denied by an entry with "!"   · not allowed   ⊘ allowed by the file ACL, narrowed by the catalog ACL   — console locked. A cell's tool tip says why; a console's tool tip has its profiles, its catalog ACL and its Where and Plugin Options ACLs.</source>
        <translation>✓ autorisé   ✗ refusé par une entrée avec "!"   · non autorisé   ⊘ autorisé par l'ACL du fichier, restreint par l'ACL du catalogue   — console verrouillée. L'info-bulle d'une cellule dit pourquoi ; celle d'une console montre ses profils, son ACL du catalogue et ses ACL Where et Plugin Options.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="358" />
        <source>The profiles could not be read: %1</source>
        <translation>Les profils n'ont pas pu être lus : %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="376" />
        <source>%n profile(s). Profiles are changed in the configuration files: "Config text..." proposes the new text.</source>
        <translation><numerusform>%n profil. Les profils se modifient dans les fichiers de configuration : « Texte de configuration... » propose le nouveau texte.</numerusform><numerusform>%n profils. Les profils se modifient dans les fichiers de configuration : « Texte de configuration... » propose le nouveau texte.</numerusform></translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="390" />
        <source>Config text...</source>
        <translation>Texte de configuration...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="392" />
        <source>Shows the new text of every changed profile, to put into the Director's configuration</source>
        <translation>Montre le nouveau texte de chaque profil modifié, à mettre dans la configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="396" />
        <source>A double click (or Space) on a cell turns it in the proposal: ✓ becomes denied, · or ✗ allowed. Changed cells are underlined; "Config text..." shows the new Profile resources. Nothing is sent: a person puts the text in.</source>
        <translation>Un double clic (ou Espace) sur une cellule la modifie dans la proposition : ✓ devient refusé, · ou ✗ autorisé. Les cellules modifiées sont soulignées ; « Texte de configuration... » montre les nouvelles ressources Profile. Rien n'est envoyé : une personne met le texte en place.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="399" />
        <source>A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows it again. Changed cells are underlined until they are applied. What the file ACL denies is changed in the editor.</source>
        <translation>Un double clic (ou Espace) sur ✓ le refuse par l'ACL du catalogue, sur ⊘ l'autorise à nouveau. Les cellules modifiées sont soulignées jusqu'à leur application. Ce que l'ACL du fichier refuse se modifie dans l'éditeur.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="296" />
        <source>Discard</source>
        <translation>Annuler les modifications</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="297" />
        <source>Forgets the changes that were not applied</source>
        <translation>Oublie les modifications non appliquées</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="300" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="390" />
        <source>Apply...</source>
        <translation>Appliquer...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="301" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="393" />
        <source>Shows every change of the catalog ACL once and sends them after your confirmation (.acl set / .acl delete)</source>
        <translation>Montre chaque modification de l'ACL du catalogue une fois et les envoie après votre confirmation (.acl set / .acl delete)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="305" />
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="306" />
        <source>Closes the matrix; changes that were not applied are discarded</source>
        <translation>Ferme la matrice ; les modifications non appliquées sont abandonnées</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="345" />
        <source>%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).</source>
        <translation>
            <numerusform>%n console. Autorisé est ce que le Director a décidé (ACL du fichier et du catalogue).</numerusform>
            <numerusform>%n consoles. Autorisé est ce que le Director a décidé (ACL du fichier et du catalogue).</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="347" />
        <source>%n console(s). This Director reports the file ACL only; the GUI decided the cells from its entries.</source>
        <translation>
            <numerusform>%n console. Ce Director ne signale que l'ACL du fichier ; la GUI a déterminé les cellules à partir de ses entrées.</numerusform>
            <numerusform>%n consoles. Ce Director ne signale que l'ACL du fichier ; la GUI a déterminé les cellules à partir de ses entrées.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="404" />
        <source>No profiles read yet: Refresh reads them.</source>
        <translation>Aucun profil lu : Actualiser les lit.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="429" />
        <source>Profile %1</source>
        <translation>Profil %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="431" />
        <source>Used by no console</source>
        <translation>Utilisé par aucune console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="431" />
        <source>Used by: %1</source>
        <translation>Utilisé par : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="434" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="434" />
        <source>none</source>
        <translation>aucun</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="453" />
        <source>The proposal stays until the Director has it: after the text is in place and the Director reloaded, Refresh shows it.</source>
        <translation>La proposition reste jusqu'à ce que le Director l'ait : une fois le texte en place et le Director rechargé, Actualiser la montre.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="459" />
        <source>The ACLs could not be read: %1</source>
        <translation>Les ACL n'ont pas pu être lues : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="496" />
        <source>In the proposal: allowed.</source>
        <translation>Dans la proposition : autorisé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="497" />
        <source>In the proposal: denied.</source>
        <translation>Dans la proposition : refusé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="498" />
        <source>Double click: change it in the proposed configuration text.</source>
        <translation>Double clic : le modifier dans le texte de configuration proposé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="520" />
        <source>Not applied yet: allowed again through the catalog ACL.</source>
        <translation>Pas encore appliqué : de nouveau autorisé par l'ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="521" />
        <source>Not applied yet: denied through the catalog ACL.</source>
        <translation>Pas encore appliqué : refusé par l'ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="524" />
        <source>Double click: deny it through the catalog ACL.</source>
        <translation>Double clic : le refuser par l'ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="525" />
        <source>Double click: allow it again.</source>
        <translation>Double clic : l'autoriser à nouveau.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="557" />
        <source>Console %1 is locked: its catalog ACL cannot be changed until it can be read.</source>
        <translation>La console %1 est verrouillée : son ACL du catalogue ne peut pas être modifiée tant qu'elle ne peut pas être lue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="558" />
        <source>%1 is not allowed by the file ACL of console %2: that is changed in the editor, not here.</source>
        <translation>%1 n'est pas autorisé par l'ACL du fichier de la console %2 : cela se modifie dans l'éditeur, pas ici.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="591" />
        <source>Discard the changes?</source>
        <translation>Abandonner les modifications ?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="592" />
        <source>%n change(s) of the catalog ACL were not applied. Discard them and close?</source>
        <translation>
            <numerusform>%n modification de l'ACL du catalogue n'a pas été appliquée. L'abandonner et fermer ?</numerusform>
            <numerusform>%n modifications de l'ACL du catalogue n'ont pas été appliquées. Les abandonner et fermer ?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="629" />
        <source>Creating console %1...</source>
        <translation>Création de la console %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="640" />
        <source>Console %1 was not created: %2</source>
        <translation>La console %1 n'a pas été créée : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="643" />
        <source>Console %1 was created.</source>
        <translation>La console %1 a été créée.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="653" />
        <source>The changes were discarded.</source>
        <translation>Les modifications ont été annulées.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="671" />
        <source>Nothing was sent: the entry "%1" contains a comma, which the Director cannot store in a catalog ACL.</source>
        <translation>Rien n'a été envoyé : l'entrée "%1" contient une virgule, que le Director ne peut pas enregistrer dans une ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="681" />
        <source>Console %1, %2:</source>
        <translation>Console %1, %2 :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="682" />
        <source>  now: %1</source>
        <translation>  actuel : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="683" />
        <source>  new: %1</source>
        <translation>  nouveau : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="685" />
        <source>%1 of %2</source>
        <translation>%1 de %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="690" />
        <source>Nothing was sent: the changes were not confirmed.</source>
        <translation>Rien n'a été envoyé : les modifications n'ont pas été confirmées.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="702" />
        <source>Changed: %1.</source>
        <translation>Modifié : %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="705" />
        <source>Changing %1...</source>
        <translation>Modification de %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="640" />
        <location filename="../src/director/baclmatrixdialog.cpp" line="714" />
        <source>refused</source>
        <translation>refusé</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="725" />
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="726" />
        <source>Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.</source>
        <translation>La modification de %1 a échoué : %2. Modifié avant : %3. Rien n'a été envoyé ensuite.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="776" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>BBvfsModel</name>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="234" />
        <source>Directory</source>
        <translation>Répertoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="235" />
        <source>Symlink</source>
        <translation>Lien symbolique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="236" />
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="299" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="300" />
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="301" />
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="302" />
        <source>Modified</source>
        <translation>Modifié</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="468" />
        <source>JSON parse error: %1</source>
        <translation>Erreur d'analyse JSON : %1</translation>
    </message>
</context>
<context>
    <name>BCapabilities</name>
    <message>
        <location filename="../src/bcapabilities.cpp" line="33" />
        <source>Change an existing schedule</source>
        <translation>Modifier un Schedule existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="36" />
        <source>%1 cannot change an existing schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un Schedule existant sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="40" />
        <source>Nothing was changed. A schedule can also be changed in its configuration file on the Director.</source>
        <translation>Rien n'a été modifié. Un Schedule peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="49" />
        <source>Prune and purge all volumes</source>
        <translation>Élaguer et purger tous les volumes</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="52" />
        <source>%1 has no command that purges all volumes at once; purge works on named volumes only and ignores retention periods.</source>
        <translation>%1 n'a aucune commande qui purge tous les volumes à la fois ; purge ne fonctionne que sur des volumes nommés et ignore les durées de rétention.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="55" />
        <source>"Prune only" applies the retention period of every volume; a single volume can be purged by its name with the purge command.</source>
        <translation>« Élaguer uniquement » applique la durée de rétention de chaque volume ; un volume isolé peut être purgé par son nom avec la commande purge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="62" />
        <source>Send a FileSet to the Director</source>
        <translation>Envoyer un FileSet au Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="65" />
        <source>%1 cannot take a FileSet through the console, because its configure command does not accept nested blocks (Include, Options, Exclude).</source>
        <translation>%1 ne peut pas recevoir de FileSet via la console, car sa commande configure n'accepte pas les blocs imbriqués (Include, Options, Exclude).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="68" />
        <source>You can copy the configuration text and place it on the Director yourself.</source>
        <translation>Vous pouvez copier le texte de configuration et le placer vous-même sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="75" />
        <source>Delete a schedule</source>
        <translation>Supprimer un Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="78" />
        <source>%1 cannot delete a schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un Schedule sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="82" />
        <source>Nothing was deleted. A schedule can also be removed from the configuration files on the Director.</source>
        <translation>Rien n'a été supprimé. Un Schedule peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="91" />
        <source>Create a schedule</source>
        <translation>Créer un Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="94" />
        <source>%1 cannot create a schedule.</source>
        <translation>%1 ne peut pas créer de Schedule.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="106" />
        <source>Change an existing job</source>
        <translation>Modifier un job existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="109" />
        <source>%1 cannot change an existing job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un job existant sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="113" />
        <source>Nothing was changed. A job can also be changed in its configuration file on the Director.</source>
        <translation>Rien n'a été modifié. Un job peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="122" />
        <source>Delete a job</source>
        <translation>Supprimer un job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="125" />
        <source>%1 cannot delete a job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un job sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="128" />
        <source>Nothing was deleted. A job can also be removed from the configuration files on the Director.</source>
        <translation>Rien n'a été supprimé. Un job peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="137" />
        <source>Create a job</source>
        <translation>Créer un job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="140" />
        <source>%1 cannot create a job.</source>
        <translation>%1 ne peut pas créer de job.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="148" />
        <source>Change an existing JobDefs</source>
        <translation>Modifier un JobDefs existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="151" />
        <source>%1 cannot change an existing JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un JobDefs existant sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="155" />
        <source>Nothing was changed. A JobDefs can also be changed in its configuration file on the Director.</source>
        <translation>Rien n'a été modifié. Un JobDefs peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="164" />
        <source>Delete a JobDefs</source>
        <translation>Supprimer un JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="167" />
        <source>%1 cannot delete a JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un JobDefs sur cette connexion : cela nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="171" />
        <source>Nothing was deleted. A JobDefs can also be removed from the configuration files on the Director.</source>
        <translation>Rien n'a été supprimé. Un JobDefs peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="180" />
        <source>Create a JobDefs</source>
        <translation>Créer un JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="183" />
        <source>%1 cannot create a JobDefs.</source>
        <translation>%1 ne peut pas créer de JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="309" />
        <source>This Director (Bareos)</source>
        <translation>Ce Director (Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="310" />
        <source>This Director (Bareos %1)</source>
        <translation>Ce Director (Bareos %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="314" />
        <source>This Director (Onesimus Director)</source>
        <translation>Ce Director (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="316" />
        <source>This Director (Onesimus Director %1)</source>
        <translation>Ce Director (Onesimus Director %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="322" />
        <source>This Director</source>
        <translation>Ce Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="334" />
        <source>%1 does not offer this to this session: its configuration capabilities do not list it. The console needs the command ACL "configure" and the ACL of the resource type, or the Director is older than its configuration methods.</source>
        <translation>%1 ne propose pas cela à cette session : ses capacités de configuration ne le mentionnent pas. La console a besoin de l'ACL de commande « configure » et de l'ACL du type de ressource, ou le Director est antérieur à ses méthodes de configuration.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="343" />
        <source>The capabilities query failed: %1.</source>
        <translation>La requête des capacités a échoué : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="347" />
        <source>The capabilities were not received for this session.</source>
        <translation>Les capacités n'ont pas été reçues pour cette session.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="356" />
        <source>The Director lists for this session: %1.</source>
        <translation>Le Director indique pour cette session : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="372" />
        <source>The Onesimus Director supports this over its gRPC interface.</source>
        <translation>L'Onesimus Director prend cela en charge via son interface gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="375" />
        <source>Connect to it over gRPC to use this.</source>
        <translation>Connectez-vous à lui via gRPC pour utiliser cette fonction.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="382" />
        <source>The Onesimus Director supports this.</source>
        <translation>L'Onesimus Director prend cela en charge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="387" />
        <source>Support for this is planned for a later version of the Onesimus Director.</source>
        <translation>La prise en charge est prévue dans une version ultérieure de l'Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="390" />
        <source>The Onesimus Director is planned to support this.</source>
        <translation>L'Onesimus Director doit prendre cela en charge à l'avenir.</translation>
    </message>
</context>
<context>
    <name>BCapabilityNotice</name>
    <message>
        <location filename="../src/bcapabilitynotice.cpp" line="65" />
        <source>Do not show again</source>
        <translation>Ne plus afficher</translation>
    </message>
</context>
<context>
    <name>BCatalogResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>DB Driver</source>
        <translation>Pilote de base de données</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448" />
        <source>DB Name</source>
        <translation>Nom de la base de données</translation>
    </message>
</context>
<context>
    <name>BCertificateGenerator</name>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="36" />
        <source>Generate TLS Certificates</source>
        <translation>Générer des certificats TLS</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="48" />
        <source>Generate TLS certificates for secure Bareos communication.
This will create a CA certificate, client certificate, and private key.</source>
        <translation>Générer des certificats TLS pour une communication Bareos sécurisée.
Cela créera un certificat CA, un certificat client et une clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="56" />
        <source>Certificate Settings</source>
        <translation>Paramètres du certificat</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="64" />
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="67" />
        <source>Output Directory:</source>
        <translation>Répertoire de sortie :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="72" />
        <source>e.g., onesimus-client</source>
        <translation>p. ex. onesimus-client</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="73" />
        <source>Common Name (CN):</source>
        <translation>Nom commun (CN) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="78" />
        <source>e.g., My Company</source>
        <translation>p. ex. Mon entreprise</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="79" />
        <source>Organization (O):</source>
        <translation>Organisation (O) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="84" />
        <source>Days (e.g., 3650 = 10 years)</source>
        <translation>Jours (p. ex. 3650 = 10 ans)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="85" />
        <source>Valid for (days):</source>
        <translation>Valide pendant (jours) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="88" />
        <source>Overwrite existing files</source>
        <translation>Écraser les fichiers existants</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="113" />
        <source>Generate Certificates</source>
        <translation>Générer les certificats</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="115" />
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="129" />
        <source>Select Output Directory</source>
        <translation>Sélectionner le répertoire de sortie</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="164" />
        <source>Failed to create key context: %1</source>
        <translation>Impossible de créer le contexte de clé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="169" />
        <source>Failed to init keygen: %1</source>
        <translation>Impossible d'initialiser la génération de clé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="175" />
        <source>Failed to set key bits: %1</source>
        <translation>Impossible de définir la taille de la clé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="181" />
        <source>Failed to generate key: %1</source>
        <translation>Impossible de générer la clé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="197" />
        <source>Failed to create X509: %1</source>
        <translation>Impossible de créer le X509 : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="203" />
        <source>Failed to set X509 version</source>
        <translation>Impossible de définir la version X509</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="211" />
        <source>Failed to create serial number</source>
        <translation>Impossible de créer le numéro de série</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="225" />
        <source>Failed to set public key</source>
        <translation>Impossible de définir la clé publique</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="233" />
        <source>Failed to create X509_NAME</source>
        <translation>Impossible de créer X509_NAME</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="244" />
        <source>Failed to add Organization to subject: %1</source>
        <translation>Impossible d'ajouter l'organisation au sujet : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="252" />
        <source>Failed to add Common Name to subject: %1</source>
        <translation>Impossible d'ajouter le nom commun au sujet : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="260" />
        <source>Failed to set subject name: %1</source>
        <translation>Impossible de définir le nom du sujet : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="270" />
        <location filename="../src/bcertificategenerator.cpp" line="278" />
        <source>Failed to set issuer name: %1</source>
        <translation>Impossible de définir le nom de l'émetteur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="311" />
        <source>Failed to sign certificate: %1</source>
        <translation>Impossible de signer le certificat : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="325" />
        <location filename="../src/bcertificategenerator.cpp" line="346" />
        <location filename="../src/bcertificategenerator.cpp" line="384" />
        <source>Cannot open file for writing: %1</source>
        <translation>Impossible d'ouvrir le fichier en écriture : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="333" />
        <source>Failed to write private key: %1</source>
        <translation>Impossible d'écrire la clé privée : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="354" />
        <source>Failed to write certificate: %1</source>
        <translation>Impossible d'écrire le certificat : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="376" />
        <source>Failed to create PKCS12: %1</source>
        <translation>Impossible de créer le PKCS12 : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="394" />
        <source>Failed to write PFX file: %1</source>
        <translation>Impossible d'écrire le fichier PFX : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="409" />
        <source>Generating...</source>
        <translation>Génération...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="418" />
        <source>Please fill in all required fields</source>
        <translation>Veuillez remplir tous les champs obligatoires</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="420" />
        <location filename="../src/bcertificategenerator.cpp" line="434" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="432" />
        <source>Failed to create output directory</source>
        <translation>Impossible de créer le répertoire de sortie</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="450" />
        <source>Certificate files already exist. Enable 'Overwrite' to replace.</source>
        <translation>Les fichiers de certificat existent déjà. Activez « Écraser » pour les remplacer.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="452" />
        <source>Files exist</source>
        <translation>Fichiers existants</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="461" />
        <source>Generating CA private key (4096 bit RSA)...</source>
        <translation>Génération de la clé privée CA (RSA 4096 bits)...</translation>
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
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="472" />
        <source>Saving CA private key...</source>
        <translation>Enregistrement de la clé privée CA...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="482" />
        <source>Generating CA certificate...</source>
        <translation>Génération du certificat CA...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="503" />
        <source>Generating client private key (4096 bit RSA)...</source>
        <translation>Génération de la clé privée client (RSA 4096 bits)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="516" />
        <source>Saving client private key...</source>
        <translation>Enregistrement de la clé privée client...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="528" />
        <source>Generating client certificate...</source>
        <translation>Génération du certificat client...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="554" />
        <source>Creating PFX file for Windows...</source>
        <translation>Création du fichier PFX pour Windows...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="556" />
        <source>PFX creation failed (non-critical)</source>
        <translation>Échec de la création du PFX (non critique)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="569" />
        <source>Certificates generated successfully!</source>
        <translation>Certificats générés avec succès !</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="570" />
        <source>CA Certificate: %1</source>
        <translation>Certificat CA : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="571" />
        <source>Client Certificate: %1</source>
        <translation>Certificat client : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="572" />
        <source>Private Key: %1</source>
        <translation>Clé privée : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="574" />
        <source>PFX File: %1</source>
        <translation>Fichier PFX : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="575" />
        <source>PFX Password: (empty - no password)</source>
        <translation>Mot de passe PFX : (vide - aucun mot de passe)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="578" />
        <source>Success!</source>
        <translation>Succès !</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="581" />
        <source>Use Certificates</source>
        <translation>Utiliser les certificats</translation>
    </message>
</context>
<context>
    <name>BCheckableHeaderView</name>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="118" />
        <source>Show All Columns</source>
        <translation>Afficher toutes les colonnes</translation>
    </message>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="128" />
        <source>Reset Columns</source>
        <translation>Réinitialiser les colonnes</translation>
    </message>
</context>
<context>
    <name>BCleanupDialog</name>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="39" />
        <source>Database Cleanup</source>
        <translation>Nettoyage de la base de données</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="76" />
        <source>The connection to the Director was lost</source>
        <translation>La connexion au Director a été perdue</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="98" />
        <source>This tool helps maintain your Bareos database by removing old or unnecessary data.
⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding.</source>
        <translation>Cet outil aide à entretenir votre base de données Bareos en supprimant les données anciennes ou inutiles.
⚠️ Attention : les opérations de nettoyage sont irréversibles. Examinez attentivement les options avant de continuer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="108" />
        <source>Cleanup Options</source>
        <translation>Options de nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="113" />
        <source>Remove old full backups</source>
        <translation>Supprimer les anciennes sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="116" />
        <source>Delete old successful full backups of a job if newer full backups of the same client, job and FileSet exist. A full backup that incremental or differential backups still depend on is kept.</source>
        <translation>Supprimer les anciennes sauvegardes Full réussies d'un job s'il existe des sauvegardes Full plus récentes du même client, job et FileSet. Une sauvegarde Full dont dépendent encore des sauvegardes Incremental ou Differential est conservée.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="121" />
        <source>Keep last:</source>
        <translation>Conserver les dernières :</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="128" />
        <source> full backups</source>
        <translation> sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="129" />
        <source>Number of recent full backups to keep per client, job and FileSet</source>
        <translation>Nombre de sauvegardes Full récentes à conserver par client, job et FileSet</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="136" />
        <source>Remove successful jobs with no files</source>
        <translation>Supprimer les jobs réussis sans fichiers</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="138" />
        <source>Delete jobs that completed successfully but backed up 0 files</source>
        <translation>Supprimer les jobs terminés avec succès mais ayant sauvegardé 0 fichier</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="143" />
        <source>Prune volumes</source>
        <translation>Élaguer les volumes</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="145" />
        <source>Apply retention policy to volumes (removes expired job records)</source>
        <translation>Appliquer la politique de rétention aux volumes (supprime les enregistrements de jobs expirés)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="148" />
        <source>Action:</source>
        <translation>Action :</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="152" />
        <source>Prune only</source>
        <translation>Élaguer uniquement</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="153" />
        <source>Prune and purge</source>
        <translation>Élaguer et purger</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="154" />
        <source>Prune removes the expired job records of all volumes according to their retention period</source>
        <translation>L'élagage supprime les enregistrements de jobs expirés de tous les volumes selon leur durée de rétention</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="164" />
        <source>Not available: Bareos has no command that purges all volumes at once. Purge works on named volumes only and ignores retention periods, so it is not sent from this dialog.</source>
        <translation>Non disponible : Bareos n'a aucune commande qui purge tous les volumes à la fois. La purge ne fonctionne que sur des volumes nommés et ignore les durées de rétention ; elle n'est donc pas envoyée depuis cette boîte de dialogue.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="180" />
        <source>Remove failed jobs older than</source>
        <translation>Supprimer les jobs en échec antérieurs à</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="182" />
        <source>Delete jobs that failed or were canceled</source>
        <translation>Supprimer les jobs en échec ou annulés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="189" />
        <source> days</source>
        <translation> jours</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="211" />
        <source>Select cleanup options and click 'Analyze' to preview</source>
        <translation>Sélectionnez les options de nettoyage et cliquez sur « Analyser » pour un aperçu</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="216" />
        <source>Analysis / Log</source>
        <translation>Analyse / Journal</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="234" />
        <source>Analyze</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="235" />
        <source>Preview what would be cleaned up</source>
        <translation>Prévisualiser ce qui serait nettoyé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="239" />
        <source>Cleanup</source>
        <translation>Nettoyer</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="241" />
        <source>Perform cleanup operations (requires analysis first)</source>
        <translation>Effectuer les opérations de nettoyage (nécessite une analyse préalable)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="247" />
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="333" />
        <source>The options were changed. The analysis above is no longer valid; click 'Analyze' again.</source>
        <translation>Les options ont été modifiées. L'analyse ci-dessus n'est plus valide ; cliquez de nouveau sur « Analyser ».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="342" />
        <source>The job list has changed. The analysis above is no longer valid; click 'Analyze' again.</source>
        <translation>La liste des jobs a changé. L'analyse ci-dessus n'est plus valide ; cliquez de nouveau sur « Analyser ».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="353" />
        <location filename="../src/bcleanupdialog.cpp" line="714" />
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="354" />
        <location filename="../src/bcleanupdialog.cpp" line="715" />
        <source>Please connect to Bareos Director first.</source>
        <translation>Veuillez d'abord vous connecter au Director Bareos.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="360" />
        <source>No Options Selected</source>
        <translation>Aucune option sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="361" />
        <source>Please select at least one cleanup option.</source>
        <translation>Veuillez sélectionner au moins une option de nettoyage.</translation>
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
        <translation>Analyse requise</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="383" />
        <source>Please run analysis before cleanup.</source>
        <translation>Veuillez lancer l'analyse avant le nettoyage.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="390" />
        <source>⚠️ This will permanently delete data from the Bareos catalog.

</source>
        <translation>⚠️ Cette opération supprimera définitivement des données du catalogue Bareos.

</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="391" />
        <source>Operations to perform:
</source>
        <translation>Opérations à effectuer :
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="394" />
        <source>- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)
</source>
        <translation>- Supprimer %1 anciens jobs de sauvegarde Full (en conservant %2 par client, job et FileSet)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="400" />
        <source>- Delete %1 empty jobs (successful with 0 files)
</source>
        <translation>- Supprimer %1 jobs vides (réussis avec 0 fichier)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="405" />
        <source>- Delete %1 failed jobs older than %2 days
</source>
        <translation>- Supprimer %1 jobs en échec de plus de %2 jours
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="412" />
        <source>  Job records to delete in total: %1
</source>
        <translation>  Total des enregistrements de jobs à supprimer : %1
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="416" />
        <source>- Prune all volumes (command: %1)
</source>
        <translation>- Élaguer tous les volumes (commande : %1)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="419" />
        <source>
Do you want to continue?</source>
        <translation>
Voulez-vous continuer ?</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="422" />
        <source>Confirm Cleanup</source>
        <translation>Confirmer le nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="432" />
        <source>The job list changed in the meantime. Nothing was deleted; please run the analysis again.</source>
        <translation>La liste des jobs a changé entre-temps. Rien n'a été supprimé ; veuillez relancer l'analyse.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="445" />
        <source>Stopping: no further commands are sent after the answer that is still awaited.</source>
        <translation>Arrêt : aucune autre commande n'est envoyée après la réponse encore attendue.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="470" />
        <source>Analyzing cleanup options...</source>
        <translation>Analyse des options de nettoyage...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="475" />
        <source>⚠ No job data available!</source>
        <translation>⚠ Aucune donnée de job disponible !</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="476" />
        <source>Please load the job list first (click 'Refresh' in the main window).</source>
        <translation>Veuillez d'abord charger la liste des jobs (cliquez sur « Actualiser » dans la fenêtre principale).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="478" />
        <source>No job data. Please refresh job list first.</source>
        <translation>Aucune donnée de job. Veuillez d'abord actualiser la liste des jobs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="485" />
        <source>Looking at the %1 jobs currently loaded in the job list. Jobs that are not loaded there are not considered.</source>
        <translation>Examen des %1 jobs actuellement chargés dans la liste des jobs. Les jobs qui n'y sont pas chargés ne sont pas pris en compte.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="502" />
        <source>✓ Will prune all volumes (apply each volume's retention period)</source>
        <translation>✓ Élaguera tous les volumes (applique la durée de rétention de chaque volume)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="503" />
        <source>  Command: %1</source>
        <translation>  Commande : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="505" />
        <source>  'Prune and purge' is not available and will not be sent.</source>
        <translation>  « Élaguer et purger » n'est pas disponible et ne sera pas envoyé.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="510" />
        <source>=== Analysis Summary ===</source>
        <translation>=== Résumé de l'analyse ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="514" />
        <source>Total jobs to delete: %1</source>
        <translation>Total des jobs à supprimer : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="516" />
        <source>  - Old full backups: %1 (%2)</source>
        <translation>  - Anciennes sauvegardes Full : %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="521" />
        <source>  - Empty jobs: %1</source>
        <translation>  - Jobs vides : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="524" />
        <source>  - Failed jobs: %1</source>
        <translation>  - Jobs en échec : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="527" />
        <source>No jobs found matching cleanup criteria.</source>
        <translation>Aucun job ne correspond aux critères de nettoyage.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="534" />
        <source>Analysis complete. Click 'Cleanup' to proceed.</source>
        <translation>Analyse terminée. Cliquez sur « Nettoyer » pour continuer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="536" />
        <source>Analysis complete. Nothing to clean up.</source>
        <translation>Analyse terminée. Rien à nettoyer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="550" />
        <source>Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...</source>
        <translation>Analyse des anciennes sauvegardes Full (en conservant les %1 plus récentes par client, job et FileSet)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="558" />
        <source>at least one full backup must be kept</source>
        <translation>au moins une sauvegarde Full doit être conservée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="561" />
        <source>%1 job rows have no job ID</source>
        <translation>%1 lignes de jobs n'ont pas d'ID de job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="564" />
        <source>%1 job rows have no job type, so backups cannot be told from other jobs</source>
        <translation>%1 lignes de jobs n'ont pas de type de job ; les sauvegardes ne peuvent donc pas être distinguées des autres jobs</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="568" />
        <source>%1 backup jobs have no client or job name</source>
        <translation>%1 jobs de sauvegarde n'ont ni client ni nom de job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="573" />
        <source>  ⚠ No old full backups are selected: %1.</source>
        <translation>  ⚠ Aucune ancienne sauvegarde Full n'est sélectionnée : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="577" />
        <source>  Note: the FileSet of a job is taken from its current job configuration, not from the catalog record of each run.</source>
        <translation>  Remarque : le FileSet d'un job provient de sa configuration de job actuelle, et non de l'enregistrement du catalogue de chaque exécution.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="584" />
        <source>%1 runs have no known FileSet</source>
        <translation>%1 exécutions n'ont pas de FileSet connu</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="587" />
        <source>%1 runs have no status</source>
        <translation>%1 exécutions n'ont pas de statut</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="590" />
        <source>%1 runs have a level other than Full, Differential or Incremental</source>
        <translation>%1 exécutions ont un niveau autre que Full, Differential ou Incremental</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="593" />
        <source>%1 runs have no readable start time</source>
        <translation>%1 exécutions n'ont pas d'heure de début lisible</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="596" />
        <source>  ⚠ Client '%1', job '%2': skipped, nothing selected (%3)</source>
        <translation>  ⚠ Client '%1', job '%2' : ignoré, rien de sélectionné (%3)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="602" />
        <source>  Client '%1', job '%2', FileSet '%3': %4 full backups, will delete %5</source>
        <translation>  Client '%1', job '%2', FileSet '%3' : %4 sauvegardes Full, %5 seront supprimées</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="609" />
        <source>    ... and %1 more</source>
        <translation>    ... et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="613" />
        <source>    → Job %1: %2 (%3, %4)</source>
        <translation>    → Job %1 : %2 (%3, %4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="616" />
        <source>      its dependent jobs %1 are deleted by this cleanup as well</source>
        <translation>      ses jobs dépendants %1 sont également supprimés par ce nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="622" />
        <source>    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it</source>
        <translation>    ⚠ Le job %1 (%2) est conservé : les jobs Incremental/Differential %3 en dépendent</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="629" />
        <source>✓ Found %1 old full backups to delete (%2)</source>
        <translation>✓ %1 anciennes sauvegardes Full à supprimer trouvées (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="633" />
        <source>  No old full backups found to delete</source>
        <translation>  Aucune ancienne sauvegarde Full à supprimer trouvée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="639" />
        <source>Analyzing empty jobs (successful with 0 files)...</source>
        <translation>Analyse des jobs vides (réussis avec 0 fichier)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="644" />
        <source>  → Job %1: %2 [%3] (%4)</source>
        <translation>  → Job %1 : %2 [%3] (%4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="649" />
        <location filename="../src/bcleanupdialog.cpp" line="687" />
        <source>  ... and %1 more</source>
        <translation>  ... et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="653" />
        <source>  %1 successful jobs have no file count and are not selected</source>
        <translation>  %1 jobs réussis n'ont pas de nombre de fichiers et ne sont pas sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="658" />
        <source>✓ Found %1 empty jobs to delete</source>
        <translation>✓ %1 jobs vides à supprimer trouvés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="660" />
        <source>  No empty jobs found</source>
        <translation>  Aucun job vide trouvé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="668" />
        <source>Analyzing failed jobs older than %1 days (before %2)...</source>
        <translation>Analyse des jobs en échec de plus de %1 jours (avant le %2)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="677" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="678" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="679" />
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="680" />
        <source>Non-fatal error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="682" />
        <source>  → Job %1: %2 [%3] (%4) - %5</source>
        <translation>  → Job %1 : %2 [%3] (%4) - %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="691" />
        <source>✓ Found %1 failed jobs to delete</source>
        <translation>✓ %1 jobs en échec à supprimer trouvés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="693" />
        <source>  No failed jobs found matching criteria</source>
        <translation>  Aucun job en échec ne correspond aux critères</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702" />
        <source>Stop</source>
        <translation>Arrêter</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702" />
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="720" />
        <source>=== Starting Cleanup ===</source>
        <translation>=== Démarrage du nettoyage ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="727" />
        <source>Cleanup running: commands are sent one at a time...</source>
        <translation>Nettoyage en cours : les commandes sont envoyées une par une...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="737" />
        <source>Sent: %1</source>
        <translation>Envoyé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="749" />
        <source>(empty answer)</source>
        <translation>(réponse vide)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="754" />
        <source>✓ Job %1: deletion confirmed by the Director</source>
        <translation>✓ Job %1 : suppression confirmée par le Director</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="757" />
        <source>✗ Job %1: the Director reported an error: %2</source>
        <translation>✗ Job %1 : le Director a signalé une erreur : %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="760" />
        <source>? Job %1: the Director did not confirm the deletion. Answer: %2</source>
        <translation>? Job %1 : le Director n'a pas confirmé la suppression. Réponse : %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="764" />
        <source>? Job %1: result unknown (%2)</source>
        <translation>? Job %1 : résultat inconnu (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="772" />
        <source>Prune volumes: the Director answered without an error: %1</source>
        <translation>Élagage des volumes : le Director a répondu sans erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="775" />
        <source>✗ Prune volumes: the Director reported an error: %1</source>
        <translation>✗ Élagage des volumes : le Director a signalé une erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="778" />
        <source>? Prune volumes: result unknown (%1)</source>
        <translation>? Élagage des volumes : résultat inconnu (%1)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="781" />
        <source>? Prune volumes: no usable answer: %1</source>
        <translation>? Élagage des volumes : aucune réponse exploitable : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="791" />
        <source>no answer from the Director within the time limit</source>
        <translation>aucune réponse du Director dans le délai imparti</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="806" />
        <source>=== Cleanup Result ===</source>
        <translation>=== Résultat du nettoyage ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="807" />
        <source>Job deletions confirmed by the Director: %1</source>
        <translation>Suppressions de jobs confirmées par le Director : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="810" />
        <source>Prune volumes: answered without an error (see the answer above)</source>
        <translation>Élagage des volumes : réponse sans erreur (voir la réponse ci-dessus)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="813" />
        <source>Commands the Director answered with an error: %1</source>
        <translation>Commandes auxquelles le Director a répondu par une erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="816" />
        <source>Commands with an unknown result: %1. Refresh the job list to see the actual state.</source>
        <translation>Commandes au résultat inconnu : %1. Actualisez la liste des jobs pour voir l'état réel.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="820" />
        <source>Commands not sent because the run was stopped: %1</source>
        <translation>Commandes non envoyées car l'exécution a été arrêtée : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="824" />
        <source>Cleanup finished: the Director confirmed all %1 commands.</source>
        <translation>Nettoyage terminé : le Director a confirmé les %1 commandes.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="827" />
        <source>Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. See the log.</source>
        <translation>Le nettoyage n'est pas terminé : %1 confirmé(s), %2 en échec, %3 inconnu(s), %4 non envoyé(s). Consultez le journal.</translation>
    </message>
</context>
<context>
    <name>BClientDetailsDialog</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="189" />
        <source>Client Details - %1</source>
        <translation>Détails du client - %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="330" />
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="216" />
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="316" />
        <source>General Information</source>
        <translation>Informations générales</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="323" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="325" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="395" />
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="331" />
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="332" />
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="333" />
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="334" />
        <source>Operating System:</source>
        <translation>Système d'exploitation :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="339" />
        <source>Connection Information</source>
        <translation>Informations de connexion</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="353" />
        <source>Recent (%1)</source>
        <translation>Récent (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="358" />
        <source>Stale (%1)</source>
        <translation>Ancien (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="362" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="370" />
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="365" />
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="383" />
        <source>Last backup:</source>
        <translation>Dernière sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="384" />
        <source>Last Connection:</source>
        <translation>Dernière connexion :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="389" />
        <source>Retention Policies</source>
        <translation>Politiques de rétention</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394" />
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394" />
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="399" />
        <source>Auto Prune:</source>
        <translation>Auto Prune :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="400" />
        <source>File Retention:</source>
        <translation>File Retention :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="401" />
        <source>Job Retention:</source>
        <translation>Job Retention :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="407" />
        <source>Information</source>
        <translation>Informations</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="426" />
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="459" />
        <source>Recent jobs for this client:</source>
        <translation>Jobs récents de ce client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="463" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="502" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="525" />
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="511" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="512" />
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="514" />
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="516" />
        <source>Total Jobs:</source>
        <translation>Total des jobs :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="517" />
        <source>Successful Jobs:</source>
        <translation>Jobs réussis :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="518" />
        <source>Failed Jobs:</source>
        <translation>Jobs en échec :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="519" />
        <source>Total Data:</source>
        <translation>Total des données :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="520" />
        <source>Total Files:</source>
        <translation>Total des fichiers :</translation>
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
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="92" />
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="93" />
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="94" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="95" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="96" />
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="97" />
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="126" />
        <source>Job ID</source>
        <translation>ID du job</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="127" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="128" />
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="129" />
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="130" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="131" />
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="132" />
        <source>Bytes</source>
        <translation>Octets</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="133" />
        <source>Status</source>
        <translation>Statut</translation>
    </message>
</context>
<context>
    <name>BClientResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>Afficher l'état</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="30" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="193" />
        <source>Client: %1</source>
        <translation>Client : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="197" />
        <source>Address: %1:%2</source>
        <translation>Adresse : %1:%2</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="207" />
        <source>%1 bytes</source>
        <translation>%1 octets</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="226" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="228" />
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="19" />
        <source>No connection to Director</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="24" />
        <source>Loading clients...</source>
        <translation>Chargement des clients...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="201" />
        <source>FD Port: %1</source>
        <translation>Port FD : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="230" />
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="232" />
        <source>Last Backup</source>
        <translation>Dernière sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="234" />
        <source>Operating System</source>
        <translation>Système d'exploitation</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="236" />
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="238" />
        <source>Last Connection</source>
        <translation>Dernière connexion</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="240" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="242" />
        <source>Total Data</source>
        <translation>Total des données</translation>
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
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="57" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="60" />
        <source>Filter by client...</source>
        <translation>Filtrer par client...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="67" />
        <source>Last backup:</source>
        <translation>Dernière sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="68" />
        <source>All</source>
        <translation>Tous</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="69" />
        <source>Recent</source>
        <translation>Récent</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="70" />
        <source>Stale</source>
        <translation>Ancien</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="71" />
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="72" />
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="79" />
        <source>Enable automatic refresh</source>
        <translation>Activer l'actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="92" />
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="95" />
        <source>Total:</source>
        <translation>Total :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="116" />
        <source>Recent:</source>
        <translation>Récents :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="117" />
        <source>Clients whose last backup or contact is younger than the limit in the settings</source>
        <translation>Clients dont la dernière sauvegarde ou le dernier contact est plus récent que la limite définie dans les paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="119" />
        <source>Stale:</source>
        <translation>Anciens :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="120" />
        <source>Clients whose last backup or contact is older than the limit in the settings</source>
        <translation>Clients dont la dernière sauvegarde ou le dernier contact est plus ancien que la limite définie dans les paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="122" />
        <source>Never:</source>
        <translation>Jamais :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="123" />
        <source>Clients without any known backup or contact</source>
        <translation>Clients sans aucune sauvegarde ni aucun contact connu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="125" />
        <source>Unknown:</source>
        <translation>Inconnus :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="126" />
        <source>Clients whose last backup time cannot be read</source>
        <translation>Clients dont l'heure de la dernière sauvegarde ne peut pas être lue</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="387" />
        <source>Error parsing client data</source>
        <translation>Erreur lors de l'analyse des données du client</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientswidget.cpp" line="428" />
        <source>%n client(s) loaded</source>
        <translation>
            <numerusform>%n client chargé</numerusform>
            <numerusform>%n clients chargés</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="538" />
        <source>Details / Export...</source>
        <translation>Détails / Exporter...</translation>
    </message>
</context>
<context>
    <name>BConfigImportDialog</name>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="34" />
        <source>Import Director Configuration</source>
        <translation>Importer la configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="60" />
        <source>Configuration Source</source>
        <translation>Source de la configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="64" />
        <source>Select directory or ZIP file...</source>
        <translation>Sélectionner un répertoire ou un fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="68" />
        <source>Directory...</source>
        <translation>Répertoire...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="73" />
        <source>ZIP File...</source>
        <translation>Fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="78" />
        <source>Parse</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="101" />
        <source>Connection Preview</source>
        <translation>Aperçu de la connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="105" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="171" />
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="110" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="200" />
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="114" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="207" />
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="118" />
        <location filename="../src/director/bconfigimportdialog.cpp" line="185" />
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="122" />
        <source>TLS:</source>
        <translation>TLS :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="138" />
        <source>Import</source>
        <translation>Importer</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="145" />
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="168" />
        <source>Connection Settings</source>
        <translation>Paramètres de connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="202" />
        <source>hostname or IP address</source>
        <translation>nom d'hôte ou adresse IP</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="203" />
        <source>Enter the hostname or IP address of the Director to connect to</source>
        <translation>Saisissez le nom d'hôte ou l'adresse IP du Director auquel se connecter</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="211" />
        <source>Port number (default: 9101)</source>
        <translation>Numéro de port (par défaut : 9101)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="214" />
        <source>Create connection profile</source>
        <translation>Créer un profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="219" />
        <source>Copy Debug Info</source>
        <translation>Copier les infos de débogage</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="220" />
        <source>Copy parsed resource details to clipboard for debugging</source>
        <translation>Copier les détails des ressources analysées dans le presse-papiers pour le débogage</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="253" />
        <source>Debug Info Copied</source>
        <translation>Infos de débogage copiées</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="254" />
        <source>Debug information has been copied to clipboard.

Paste it somewhere to view.</source>
        <translation>Les informations de débogage ont été copiées dans le presse-papiers.

Collez-les quelque part pour les consulter.</translation>
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
        <translation>Sélectionner le répertoire de configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="306" />
        <source>Ready to parse directory</source>
        <translation>Prêt à analyser le répertoire</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="320" />
        <source>Select Configuration ZIP Archive</source>
        <translation>Sélectionner l'archive ZIP de configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="322" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="327" />
        <source>Ready to parse ZIP archive</source>
        <translation>Prêt à analyser l'archive ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="342" />
        <source>Parsing...</source>
        <translation>Analyse...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="356" />
        <source>Failed to create temporary directory</source>
        <translation>Impossible de créer le répertoire temporaire</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="360" />
        <source>Extracting ZIP archive...</source>
        <translation>Extraction de l'archive ZIP...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="368" />
        <source>Failed to extract ZIP archive (timeout)</source>
        <translation>Impossible d'extraire l'archive ZIP (délai dépassé)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="373" />
        <source>Failed to extract ZIP archive: %1</source>
        <translation>Impossible d'extraire l'archive ZIP : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="393" />
        <source>Parsing extracted files...</source>
        <translation>Analyse des fichiers extraits...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="403" />
        <source>Parsed %1: %2</source>
        <translation>%1 analysé : %2</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="410" />
        <source>Parsing complete: %1 resources found</source>
        <translation>Analyse terminée : %1 ressources trouvées</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="440" />
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="443" />
        <source>Parsing Error</source>
        <translation>Erreur d'analyse</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="502" />
        <source>(multiple addresses)</source>
        <translation>(plusieurs adresses)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="509" />
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="509" />
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="568" />
        <source>Invalid Selection</source>
        <translation>Sélection invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="569" />
        <source>Please select both a Director and a Console.</source>
        <translation>Veuillez sélectionner à la fois un Director et une Console.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="574" />
        <source>Invalid Address</source>
        <translation>Adresse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="575" />
        <source>Please enter the hostname or IP address of the Director.</source>
        <translation>Veuillez saisir le nom d'hôte ou l'adresse IP du Director.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="613" />
        <source>%1 '%2': Missing required field '%3'</source>
        <translation>%1 '%2' : champ obligatoire '%3' manquant</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="634" />
        <source>&lt;b&gt;Validation Warnings (%1 total, showing first 10):&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Avertissements de validation (%1 au total, affichage des 10 premiers) :&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="636" />
        <source>&lt;b&gt;Validation Warnings:&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Avertissements de validation :&lt;/b&gt;&lt;br&gt;</translation>
    </message>
</context>
<context>
    <name>BConnectionWizard</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="57" />
        <source>Connection Setup</source>
        <translation>Configuration de la connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="84" />
        <source>New Connection</source>
        <translation>Nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="194" />
        <source>SHA-256 fingerprint of the server certificate:</source>
        <translation>Empreinte SHA-256 du certificat du serveur :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="201" />
        <source>Server certificate changed</source>
        <translation>Le certificat du serveur a changé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="202" />
        <source>The certificate of the Director at %1:%2 has changed since the last connection.</source>
        <translation>Le certificat du Director à l'adresse %1:%2 a changé depuis la dernière connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="204" />
        <source>Previously trusted:</source>
        <translation>Précédemment approuvé :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="206" />
        <source>This is expected if the certificate was regenerated on the Director. Otherwise someone may be intercepting the connection.</source>
        <translation>C'est normal si le certificat a été régénéré sur le Director. Sinon, quelqu'un intercepte peut-être la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="209" />
        <source>Trust server certificate?</source>
        <translation>Approuver le certificat du serveur ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="210" />
        <source>The Director at %1:%2 uses a self-signed certificate that has not been trusted yet.</source>
        <translation>Le Director à l'adresse %1:%2 utilise un certificat auto-signé qui n'a pas encore été approuvé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="216" />
        <source>The same fingerprint is announced in the local network (mDNS). This is not proof of the Director's identity: any device in the network can send such an announcement.</source>
        <translation>La même empreinte est annoncée sur le réseau local (mDNS). Ce n'est pas une preuve de l'identité du Director : n'importe quel appareil du réseau peut envoyer une telle annonce.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="221" />
        <source>The fingerprint does NOT match the one announced in the network (mDNS)!</source>
        <translation>L'empreinte NE correspond PAS à celle annoncée sur le réseau (mDNS) !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="224" />
        <source>Before trusting it, compare the fingerprint with the one of the Director's gRPC certificate, obtained from the Director's administrator or read on the Director itself.</source>
        <translation>Avant de l'approuver, comparez l'empreinte avec celle du certificat gRPC du Director, obtenue auprès de l'administrateur du Director ou lue sur le Director lui-même.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="231" />
        <source>Trust certificate</source>
        <translation>Approuver le certificat</translation>
    </message>
</context>
<context>
    <name>BConsoleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109" />
        <source>ACLs</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122" />
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122" />
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="130" />
        <source>None</source>
        <translation>Aucun</translation>
    </message>
</context>
<context>
    <name>BDatabase</name>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="37" />
        <source>Failed to create database directory: %1</source>
        <translation>Impossible de créer le répertoire de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="47" />
        <source>Failed to open database: %1</source>
        <translation>Impossible d'ouvrir la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="68" />
        <source>Failed to create database schema: %1</source>
        <translation>Impossible de créer le schéma de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="88" />
        <source>Failed to apply database migrations: %1</source>
        <translation>Impossible d'appliquer les migrations de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="100" />
        <source>Failed to repair database schema: %1</source>
        <translation>Impossible de réparer le schéma de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="206" />
        <source>No migration script for database version %1</source>
        <translation>Aucun script de migration pour la version %1 de la base de données</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="221" />
        <source>Failed to apply migration to version %1: %2</source>
        <translation>Impossible d'appliquer la migration vers la version %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="226" />
        <source>Migration to version %1 did not record its version</source>
        <translation>La migration vers la version %1 n'a pas enregistré sa version</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="257" />
        <source>Table %1 is still missing after the repair</source>
        <translation>La table %1 est toujours absente après la réparation</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="270" />
        <source>Failed to open SQL resource: %1</source>
        <translation>Impossible d'ouvrir la ressource SQL : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="286" />
        <source>Failed to start transaction: %1</source>
        <translation>Impossible de démarrer la transaction : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="317" />
        <source>SQL execution failed: %1
Statement: %2</source>
        <translation>Échec de l'exécution SQL : %1
Instruction : %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="331" />
        <source>Failed to commit transaction: %1</source>
        <translation>Impossible de valider la transaction : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="482" />
        <source>Database not ready</source>
        <translation>Base de données non prête</translation>
    </message>
</context>
<context>
    <name>BDirectiveSchema</name>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="259" />
        <source>Required directive '%1' is missing</source>
        <translation>La directive obligatoire '%1' est manquante</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="271" />
        <source>'%1' must be an integer</source>
        <translation>'%1' doit être un entier</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="274" />
        <source>'%1' must be &gt;= %2</source>
        <translation>'%1' doit être &gt;= %2</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="277" />
        <source>'%1' must be &lt;= %2</source>
        <translation>'%1' doit être &lt;= %2</translation>
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
        <translation>Nom</translation>
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
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="28" />
        <source>Description</source>
        <translation>Description</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="29" />
        <source>DIR Port</source>
        <translation>Port DIR</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="30" />
        <source>Query File</source>
        <translation>Fichier de requêtes</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="31" />
        <source>Working Dir</source>
        <translation>Répertoire de travail</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="32" />
        <source>PID Dir</source>
        <translation>Répertoire PID</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="33" />
        <source>Scripts Dir</source>
        <translation>Répertoire des scripts</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="34" />
        <source>Plugin Dir</source>
        <translation>Répertoire des plugins</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="35" />
        <source>SubSys Dir</source>
        <translation>Répertoire SubSys</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="36" />
        <source>Max Jobs</source>
        <translation>Jobs max.</translation>
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
        <translation>Système</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="45" />
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="46" />
        <source>Updated</source>
        <translation>Mis à jour</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="47" />
        <source>Last Connected</source>
        <translation>Dernière connexion</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="48" />
        <source>Active</source>
        <translation>Actif</translation>
    </message>
</context>
<context>
    <name>BDirectorResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>(plusieurs)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45" />
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45" />
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BEditableListModel</name>
    <message>
        <location filename="../src/config/beditablelistmodel.cpp" line="120" />
        <source>Add %1 items</source>
        <translation>Ajouter %1 éléments</translation>
    </message>
</context>
<context>
    <name>BEditableListWidget</name>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="77" />
        <location filename="../src/config/beditablelistwidget.cpp" line="110" />
        <source>Add...</source>
        <translation>Ajouter...</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="78" />
        <location filename="../src/config/beditablelistwidget.cpp" line="123" />
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="115" />
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="246" />
        <source>Add item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="251" />
        <source>Remove selected</source>
        <translation>Supprimer la sélection</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="390" />
        <location filename="../src/config/beditablelistwidget.cpp" line="447" />
        <source>Select Directory</source>
        <translation>Sélectionner un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="392" />
        <location filename="../src/config/beditablelistwidget.cpp" line="449" />
        <source>Select File</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="397" />
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="398" />
        <source>Enter value:</source>
        <translation>Saisir une valeur :</translation>
    </message>
</context>
<context>
    <name>BFileSetDocument</name>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="755" />
        <source>FileSet name is required</source>
        <translation>Le nom du FileSet est obligatoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="761" />
        <source>At least one Include block is required</source>
        <translation>Au moins un bloc Include est requis</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="769" />
        <source>Include block %1 has no file paths</source>
        <translation>Le bloc Include %1 n'a aucun chemin de fichier</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="774" />
        <source>At least one Include block must have file paths</source>
        <translation>Au moins un bloc Include doit avoir des chemins de fichiers</translation>
    </message>
</context>
<context>
    <name>BFileSetExcludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1008" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1023" />
        <source>Global Exclude</source>
        <translation>Exclusion globale</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1009" />
        <source>Configure paths and patterns to exclude from all Include blocks.</source>
        <translation>Configurer les chemins et motifs à exclure de tous les blocs Include.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1019" />
        <source>Exclude Paths</source>
        <translation>Chemins exclus</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1024" />
        <source>Enter path to exclude...</source>
        <translation>Saisir le chemin à exclure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1025" />
        <source>+ Add Path</source>
        <translation>+ Ajouter un chemin</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1031" />
        <source>These paths will be excluded from all backup jobs using this FileSet.
Use absolute paths (e.g., /tmp, /var/cache) or patterns.</source>
        <translation>Ces chemins seront exclus de tous les jobs de sauvegarde utilisant ce FileSet.
Utilisez des chemins absolus (p. ex. /tmp, /var/cache) ou des motifs.</translation>
    </message>
</context>
<context>
    <name>BFileSetIncludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="901" />
        <source>Include Blocks</source>
        <translation>Blocs Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="902" />
        <source>Configure which files and directories to include in the backup.</source>
        <translation>Configurer les fichiers et répertoires à inclure dans la sauvegarde.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="915" />
        <source>+ Add Include Block</source>
        <translation>+ Ajouter un bloc Include</translation>
    </message>
</context>
<context>
    <name>BFileSetPreviewPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1064" />
        <source>Preview &amp; Deploy</source>
        <translation>Aperçu &amp; déploiement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1065" />
        <source>Review the generated FileSet configuration. Copy or export it for manual deployment.</source>
        <translation>Vérifiez la configuration du FileSet générée. Copiez-la ou exportez-la pour un déploiement manuel.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1077" />
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1083" />
        <source>Copy configuration</source>
        <translation>Copier la configuration</translation>
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
        <translation># REMARQUE : le Director Bareos ne prend pas en charge 'configure add fileset' avec
# des blocs Include/Exclude/Options imbriqués (limitation Bareos confirmée).
# Déployez cette configuration manuellement :
#   1. Copiez-la dans /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Exécutez : systemctl reload bareos-dir
#
# Utilisez « Copier la configuration » pour copier le texte ci-dessous.

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1176" />
        <source>Configuration is valid</source>
        <translation>La configuration est valide</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1190" />
        <source>Close Wizard</source>
        <translation>Fermer l'assistant</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1191" />
        <source>The configuration has not been sent to the Director.
Use 'Copy configuration' to save it before closing.

Do you want to close the wizard?</source>
        <translation>La configuration n'a pas été envoyée au Director.
Utilisez « Copier la configuration » pour l'enregistrer avant de fermer.

Voulez-vous fermer l'assistant ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1204" />
        <source>Executing configure command...</source>
        <translation>Exécution de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1242" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1278" />
        <source>FileSet created successfully!</source>
        <translation>FileSet créé avec succès !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1254" />
        <source>Unknown error</source>
        <translation>Erreur inconnue</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1256" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1288" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1296" />
        <source>Timeout</source>
        <translation>Délai dépassé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1297" />
        <source>The configure command timed out.
Do you want to retry?</source>
        <translation>La commande configure a dépassé le délai.
Voulez-vous réessayer ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1303" />
        <source>Command timed out</source>
        <translation>La commande a dépassé le délai</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1313" />
        <source>Configuration copied to clipboard</source>
        <translation>Configuration copiée dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1325" />
        <source>ZIP Archives (*.zip);;Configuration Files (*.conf);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1328" />
        <source>Export FileSet Configuration</source>
        <translation>Exporter la configuration du FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1348" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1380" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1394" />
        <source>Export Error</source>
        <translation>Erreur d'exportation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1349" />
        <source>Cannot create zip file: %1</source>
        <translation>Impossible de créer le fichier zip : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1381" />
        <source>Error writing zip file</source>
        <translation>Erreur lors de l'écriture du fichier zip</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1385" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1402" />
        <source>Exported to: %1</source>
        <translation>Exporté vers : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1395" />
        <source>Cannot write file: %1</source>
        <translation>Impossible d'écrire le fichier : %1</translation>
    </message>
</context>
<context>
    <name>BFileSetResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354" />
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BFileSetSettingsPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="616" />
        <source>FileSet Settings</source>
        <translation>Paramètres du FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="617" />
        <source>Configure the FileSet name and basic options.</source>
        <translation>Configurer le nom du FileSet et les options de base.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="631" />
        <source>Select FileSet:</source>
        <translation>Sélectionner un FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="634" />
        <source>Loading FileSets...</source>
        <translation>Chargement des FileSets...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="651" />
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="655" />
        <source>e.g., LinuxFullSystem</source>
        <translation>p. ex. LinuxFullSystem</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="656" />
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="659" />
        <source>Optional description</source>
        <translation>Description facultative</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="660" />
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="665" />
        <source>Global Options</source>
        <translation>Options globales</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="668" />
        <source>Enable VSS</source>
        <translation>Activer VSS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="669" />
        <source>Enable Volume Shadow Copy Service (Windows)</source>
        <translation>Activer le service de cliché instantané de volume (Windows)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="672" />
        <source>Ignore FileSet Changes</source>
        <translation>Ignorer les modifications du FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="673" />
        <source>Don't trigger Full backup when FileSet changes</source>
        <translation>Ne pas déclencher de sauvegarde Full lorsque le FileSet change</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="676" />
        <source>Enable Snapshot</source>
        <translation>Activer les instantanés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="677" />
        <source>Enable snapshot support (Linux LVM/btrfs)</source>
        <translation>Activer la prise en charge des instantanés (Linux LVM/btrfs)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="699" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="755" />
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="775" />
        <source>No Director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="829" />
        <source>-- Select FileSet --</source>
        <translation>-- Sélectionner un FileSet --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="835" />
        <source>(%1 FileSets)</source>
        <translation>(%1 FileSets)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="877" />
        <source>Loaded</source>
        <translation>Chargé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="891" />
        <source>Config not cached</source>
        <translation>Configuration non mise en cache</translation>
    </message>
</context>
<context>
    <name>BFileSetTemplatePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="165" />
        <source>Select FileSet Template</source>
        <translation>Sélectionner un modèle de FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="166" />
        <source>Choose a preset template to start with, or mix multiple templates together.</source>
        <translation>Choisissez un modèle prédéfini pour commencer, ou combinez plusieurs modèles.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="175" />
        <source>Template:</source>
        <translation>Modèle :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="178" />
        <source>Mix Templates...</source>
        <translation>Combiner des modèles...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="180" />
        <source>Combine paths from multiple templates</source>
        <translation>Combiner les chemins de plusieurs modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="192" />
        <source>Template Description</source>
        <translation>Description du modèle</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="206" />
        <source>Paths Preview</source>
        <translation>Aperçu des chemins</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="211" />
        <source>&lt;b&gt;Include:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Include :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="224" />
        <source>&lt;b&gt;Exclude:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Exclude :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="393" />
        <source>Platform: Windows</source>
        <translation>Plateforme : Windows</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="395" />
        <source>Platform: Linux/Unix</source>
        <translation>Plateforme : Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="397" />
        <source>Platform: macOS</source>
        <translation>Plateforme : macOS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="399" />
        <source>Platform: Any</source>
        <translation>Plateforme : toutes</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="412" />
        <source>Mix Templates</source>
        <translation>Combiner des modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="417" />
        <source>Select templates to combine:</source>
        <translation>Sélectionnez les modèles à combiner :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="435" />
        <source>%1
Platform: %2</source>
        <translation>%1
Plateforme : %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="442" />
        <source>Combined Paths Preview</source>
        <translation>Aperçu des chemins combinés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="491" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="493" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="601" />
        <location filename="../src/jobs/bfilesetwizard.cpp" line="605" />
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="570" />
        <source>Mixed_%1</source>
        <translation>Mixed_%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="571" />
        <source>Combined FileSet from %1 templates</source>
        <translation>FileSet combiné à partir de %1 modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="573" />
        <source>Mixed: %1 templates selected</source>
        <translation>Combiné : %1 modèles sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="577" />
        <source>Templates: %1</source>
        <translation>Modèles : %1</translation>
    </message>
</context>
<context>
    <name>BFileSetWizard</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80" />
        <source>New FileSet Wizard</source>
        <translation>Assistant de nouveau FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80" />
        <source>Edit FileSet</source>
        <translation>Modifier le FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="103" />
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
</context>
<context>
    <name>BGrpcAddressProbe</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="167" />
        <source>no TLS connection within %1 s</source>
        <translation>aucune connexion TLS en %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="204" />
        <source>Cannot resolve the host name %1: %2</source>
        <translation>Impossible de résoudre le nom d'hôte %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="205" />
        <source>no address</source>
        <translation>aucune adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="229" />
        <source>Cannot resolve the host name %1: no address</source>
        <translation>Impossible de résoudre le nom d'hôte %1 : aucune adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="242" />
        <source>Cannot reach the gRPC endpoint %1:%2 (%3)</source>
        <translation>Impossible de joindre le point de terminaison gRPC %1:%2 (%3)</translation>
    </message>
</context>
<context>
    <name>BGrpcConnect</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="83" />
        <source>%1 - the Director no longer knows this session (it ends after 8 hours without a request or when the Director restarts). Connect again.</source>
        <translation>%1 - le Director ne connaît plus cette session (elle se termine après 8 heures sans requête ou au redémarrage du Director). Reconnectez-vous.</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="90" />
        <source>the call was cancelled</source>
        <translation>l'appel a été annulé</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="94" />
        <source>no connection or no reply within %1 s</source>
        <translation>aucune connexion ou aucune réponse en %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="95" />
        <source>no connection or no reply in time</source>
        <translation>aucune connexion ou aucune réponse à temps</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="98" />
        <source>this console is not allowed to do that</source>
        <translation>cette console n'est pas autorisée à faire cela</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="101" />
        <source>the Director does not offer this method (an older version?)</source>
        <translation>le Director ne propose pas cette méthode (version plus ancienne ?)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="104" />
        <source>the gRPC service of the Director cannot be reached</source>
        <translation>le service gRPC du Director est injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="107" />
        <source>not logged in, or the session has expired</source>
        <translation>non connecté, ou la session a expiré</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="111" />
        <source>the Director reported an error without a message</source>
        <translation>le Director a signalé une erreur sans message</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="114" />
        <source>no reason given</source>
        <translation>aucune raison indiquée</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="126" />
        <source>The Director does not offer the expected gRPC service (protocol version mismatch: this build speaks %1; the Director may run an older or newer build). Use BNET or update the Director or this application.</source>
        <translation>Le Director ne propose pas le service gRPC attendu (incompatibilité de version du protocole : cette version utilise %1 ; le Director exécute peut-être une version plus ancienne ou plus récente). Utilisez BNET ou mettez à jour le Director ou cette application.</translation>
    </message>
</context>
<context>
    <name>BGrpcTransport</name>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="195" />
        <location filename="../src/director/bgrpctransport.cpp" line="228" />
        <source>gRPC login failed: %1</source>
        <translation>Échec de la connexion gRPC : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="203" />
        <source>gRPC login failed: no challenge received</source>
        <translation>Échec de la connexion gRPC : aucun challenge reçu</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="236" />
        <source>Authentication failed: %1</source>
        <translation>Échec de l'authentification : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="237" />
        <source>invalid response</source>
        <translation>réponse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="295" />
        <source>Client %1 connected</source>
        <translation>Client %1 connecté</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="297" />
        <source>Client %1 disconnected</source>
        <translation>Client %1 déconnecté</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="300" />
        <source>Client %1 is alive</source>
        <translation>Client %1 actif</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="301" />
        <source>Client %1 not reachable</source>
        <translation>Client %1 injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="303" />
        <source>Storage %1 connected (job %2)</source>
        <translation>Storage %1 connecté (job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="306" />
        <source>Storage %1 disconnected (job %2)</source>
        <translation>Storage %1 déconnecté (job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="310" />
        <source>Storage %1 is alive</source>
        <translation>Storage %1 actif</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="311" />
        <source>Storage %1 not reachable</source>
        <translation>Storage %1 injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="421" />
        <source>Invalid response</source>
        <translation>Réponse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="481" />
        <source>Not connected to the gRPC service of the Director</source>
        <translation>Non connecté au service gRPC du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="542" />
        <source>The reply of the Director could not be read</source>
        <translation>La réponse du Director n'a pas pu être lue</translation>
    </message>
</context>
<context>
    <name>BIncludeBlockWidget</name>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="60" />
        <source>Include Block #%1</source>
        <translation>Bloc Include n°%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="68" />
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="95" />
        <source>Add Path...</source>
        <translation>Ajouter un chemin...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="96" />
        <source>Enter file or directory path</source>
        <translation>Saisir le chemin d'un fichier ou d'un répertoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="99" />
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="106" />
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="120" />
        <source>Exclude Files</source>
        <translation>Fichiers exclus</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="126" />
        <source>Add File...</source>
        <translation>Ajouter un fichier...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="127" />
        <source>Enter file path to exclude</source>
        <translation>Saisir le chemin du fichier à exclure</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="133" />
        <source>Directory Patterns (WildDir)</source>
        <translation>Motifs de répertoires (WildDir)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="139" />
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="152" />
        <source>Add Pattern...</source>
        <translation>Ajouter un motif...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="140" />
        <source>e.g., */__pycache__</source>
        <translation>p. ex. */__pycache__</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="146" />
        <source>File Patterns (WildFile)</source>
        <translation>Motifs de fichiers (WildFile)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="153" />
        <source>e.g., *.pyc, *.log</source>
        <translation>p. ex. *.pyc, *.log</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="158" />
        <source>Exclude</source>
        <translation>Exclure</translation>
    </message>
</context>
<context>
    <name>BIncludeOptionsForm</name>
    <message>
        <location filename="../src/config/bincludeoptionsform.cpp" line="87" />
        <source>Failed to load include options schema</source>
        <translation>Impossible de charger le schéma des options Include</translation>
    </message>
</context>
<context>
    <name>BJobAdvancedPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="633" />
        <source>Advanced Directives</source>
        <translation>Directives avancées</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="634" />
        <source>Optional settings for fine-tuning (can be skipped).</source>
        <translation>Paramètres facultatifs pour un réglage fin (peut être ignoré).</translation>
    </message>
</context>
<context>
    <name>BJobBasicsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="244" />
        <location filename="../src/jobs/bjobwizard.cpp" line="249" />
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="245" />
        <source>Configure the identity of the resource.</source>
        <translation>Configurer l'identité de la ressource.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="253" />
        <source>e.g., BackupServer1</source>
        <translation>p. ex. BackupServer1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="254" />
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="258" />
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="263" />
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="270" />
        <source>JobDefs:</source>
        <translation>JobDefs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="282" />
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="294" />
        <source>Inherits default settings from JobDefs "%1". Resource fields on the next page become optional.</source>
        <translation>Hérite des paramètres par défaut du JobDefs « %1 ». Les champs de ressources de la page suivante deviennent facultatifs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="308" />
        <source>Basic Settings (JobDefs)</source>
        <translation>Paramètres de base (JobDefs)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitions</name>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="112" />
        <source>(block)</source>
        <translation>(bloc)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitions.cpp" line="125" />
        <source>(%n block(s))</source>
        <translation>
            <numerusform>(%n bloc)</numerusform>
            <numerusform>(%n blocs)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="299" />
        <source>(see the text)</source>
        <translation>(voir le texte)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitionsWidget</name>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="157" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="166" />
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="167" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202" />
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200" />
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202" />
        <source>Disabled, the scheduler does not start it</source>
        <translation>Désactivé, le planificateur ne le démarre pas</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="235" />
        <source>Set in this %1</source>
        <translation>Défini dans ce %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="238" />
        <source>Inherited from JobDefs %1</source>
        <translation>Hérité du JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="240" />
        <source>Not known: a JobDefs of the chain was not reported</source>
        <translation>Inconnu : un JobDefs de la chaîne n'a pas été signalé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="241" />
        <source>Not set: the default of the Director applies</source>
        <translation>Non défini : la valeur par défaut du Director s'applique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="265" />
        <source>Copy %1 %2</source>
        <translation>Copier %1 %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="266" />
        <source>New %1</source>
        <translation>Nouveau %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="271" />
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="286" />
        <source>&amp;JobDefs:</source>
        <translation>&amp;JobDefs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="288" />
        <source>&amp;Type:</source>
        <translation>&amp;Type :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="292" />
        <source>&amp;Level:</source>
        <translation>Ni&amp;veau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="295" />
        <source>&amp;Client:</source>
        <translation>&amp;Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="297" />
        <source>&amp;FileSet:</source>
        <translation>&amp;FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="299" />
        <source>&amp;Schedule:</source>
        <translation>&amp;Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="301" />
        <source>St&amp;orage:</source>
        <translation>St&amp;orage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="303" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="305" />
        <source>&amp;Messages:</source>
        <translation>&amp;Messages :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="311" />
        <source>Create it &amp;disabled (Enabled = no)</source>
        <translation>Le créer &amp;désactivé (Enabled = no)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="313" />
        <source>A new job with a schedule is started by the scheduler of the Director. Created disabled, it does not run until you enable it.</source>
        <translation>Un nouveau job avec un Schedule est démarré par le planificateur du Director. Créé désactivé, il ne s'exécute pas tant que vous ne l'activez pas.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="320" />
        <source>The copy is the text of %1 with the new name, in a file of its own. Everything else stays as it is.</source>
        <translation>La copie est le texte de %1 avec le nouveau nom, dans un fichier séparé. Tout le reste demeure inchangé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="324" />
        <source>Fields left empty are not written; a job takes them from its JobDefs. The Director checks the new resource before anything is written.</source>
        <translation>Les champs laissés vides ne sont pas écrits ; un job les reprend de son JobDefs. Le Director vérifie la nouvelle ressource avant que quoi que ce soit ne soit écrit.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="334" />
        <source>&amp;Check and review...</source>
        <translation>&amp;Vérifier et examiner...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="351" />
        <source>A name is needed.</source>
        <translation>Un nom est requis.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="353" />
        <source>The name must not contain quotes, slashes, backslashes or "..".</source>
        <translation>Le nom ne doit pas contenir de guillemets, de barres obliques, de barres obliques inverses ni « .. ».</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="355" />
        <source>A resource of this name exists already.</source>
        <translation>Une ressource portant ce nom existe déjà.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="422" />
        <source>&amp;Refresh</source>
        <translation>&amp;Actualiser</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="423" />
        <source>Reads the Job and JobDefs resources from the Director again</source>
        <translation>Relit les ressources Job et JobDefs depuis le Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="424" />
        <source>New &amp;Job...</source>
        <translation>Nouveau &amp;Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="425" />
        <source>New Job&amp;Defs...</source>
        <translation>Nouveau Job&amp;Defs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="426" />
        <source>&amp;Copy...</source>
        <translation>&amp;Copier...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="427" />
        <source>De&amp;lete...</source>
        <translation>Su&amp;pprimer...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="429" />
        <source>Filter</source>
        <translation>Filtre</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="431" />
        <source>Filter the resources</source>
        <translation>Filtrer les ressources</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="433" />
        <source>Jobs and JobDefs</source>
        <translation>Jobs et JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="434" />
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="436" />
        <source>Show</source>
        <translation>Afficher</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="457" />
        <source>Job and JobDefs resources</source>
        <translation>Ressources Job et JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Directive</source>
        <translation>Directive</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504" />
        <source>Set in</source>
        <translation>Défini dans</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="505" />
        <source>Directives and where they are set</source>
        <translation>Directives et emplacement de leur définition</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="512" />
        <source>Directives</source>
        <translation>Directives</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="519" />
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="526" />
        <source>Resource text</source>
        <translation>Texte de la ressource</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="528" />
        <source>Edit the &amp;text directly (for directives the form does not have)</source>
        <translation>Modifier le &amp;texte directement (pour les directives absentes du formulaire)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="529" />
        <source>The text is sent as you write it, after the Director has checked it and you have seen the difference. The fields of the form are not used then.</source>
        <translation>Le texte est envoyé tel que vous l'écrivez, après que le Director l'a vérifié et que vous avez vu la différence. Les champs du formulaire ne sont alors pas utilisés.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="533" />
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="537" />
        <source>Re&amp;vert</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="538" />
        <source>Drops the changes made here</source>
        <translation>Abandonne les modifications effectuées ici</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="539" />
        <source>&amp;Save...</source>
        <translation>&amp;Enregistrer...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="540" />
        <source>Lets the Director check the change, shows it, and writes it after you confirm</source>
        <translation>Fait vérifier la modification par le Director, l'affiche et l'écrit après votre confirmation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="626" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1032" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1201" />
        <source>(not set here)</source>
        <translation>(non défini ici)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="627" />
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="628" />
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="645" />
        <source>JobDefs:</source>
        <translation>JobDefs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="646" />
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="649" />
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="653" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="654" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="655" />
        <source>Schedule:</source>
        <translation>Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="656" />
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="657" />
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="658" />
        <source>Messages:</source>
        <translation>Messages :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="659" />
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="660" />
        <source>Enabled:</source>
        <translation>Activé :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="661" />
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="776" />
        <source>Unsaved changes</source>
        <translation>Modifications non enregistrées</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="777" />
        <source>%1 has changes that were not written. Drop them?</source>
        <translation>%1 contient des modifications qui n'ont pas été écrites. Les abandonner ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="805" />
        <source>No resource selected</source>
        <translation>Aucune ressource sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="806" />
        <source>Choose a Job or JobDefs in the table to see its directives and where they are set.</source>
        <translation>Choisissez un Job ou un JobDefs dans le tableau pour voir ses directives et l'endroit où elles sont définies.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="840" />
        <source>Uses no JobDefs: every directive is set here or has its default.</source>
        <translation>N'utilise aucun JobDefs : chaque directive est définie ici ou a sa valeur par défaut.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="841" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="849" />
        <source>Inherits from JobDefs %1.</source>
        <translation>Hérite du JobDefs %1.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="844" />
        <source>No job uses this JobDefs.</source>
        <translation>Aucun job n'utilise ce JobDefs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="845" />
        <source>Used by %n job(s): %1.</source>
        <translation>
            <numerusform>Utilisé par %n job : %1.</numerusform>
            <numerusform>Utilisé par %n jobs : %1.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="890" />
        <source>the text the Director sent does not match its SHA-256</source>
        <translation>le texte envoyé par le Director ne correspond pas à son SHA-256</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="923" />
        <source>this %1</source>
        <translation>ce %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="932" />
        <source>JobDefs %1</source>
        <translation>JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="934" />
        <source>not known (a JobDefs was not reported)</source>
        <translation>inconnu (un JobDefs n'a pas été signalé)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="936" />
        <source>not set (default)</source>
        <translation>non défini (par défaut)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1009" />
        <source>Set here (as the Director reports it)</source>
        <translation>Défini ici (tel que le Director l'indique)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1011" />
        <source>Inherited: %1 (JobDefs %2). Leave empty to keep it.</source>
        <translation>Hérité : %1 (JobDefs %2). Laisser vide pour le conserver.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1013" />
        <source>Not known: a JobDefs of the chain was not reported.</source>
        <translation>Inconnu : un JobDefs de la chaîne n'a pas été signalé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1014" />
        <source>Not set: the default of the Director applies.</source>
        <translation>Non défini : la valeur par défaut du Director s'applique.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1016" />
        <source>Written in a way the form cannot change (several times, a list, or with other directives on its line). Use the text.</source>
        <translation>Écrit d'une manière que le formulaire ne peut pas modifier (plusieurs fois, une liste, ou avec d'autres directives sur la même ligne). Utilisez le texte.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1033" />
        <source>(not set here: %1)</source>
        <translation>(non défini ici : %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1057" />
        <source>Read-only on this connection: changing Job and JobDefs resources needs the configuration methods of the gRPC interface of the Onesimus Director. New Job and New JobDefs use the job wizard ("configure add").</source>
        <translation>Lecture seule sur cette connexion : la modification des ressources Job et JobDefs nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director. Nouveau Job et Nouveau JobDefs utilisent l'assistant de job (« configure add »).</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1061" />
        <source>Read-only: this session is not offered changes of %1 resources (the console needs the command ACL "configure" and the ACL of the resource type).</source>
        <translation>Lecture seule : les modifications des ressources %1 ne sont pas proposées à cette session (la console a besoin de l'ACL de commande « configure » et de l'ACL du type de ressource).</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1065" />
        <source>Reading the text of the resource from the Director...</source>
        <translation>Lecture du texte de la ressource depuis le Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1067" />
        <source>Read-only: the text of the resource could not be read: %1</source>
        <translation>Lecture seule : le texte de la ressource n'a pas pu être lu : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1069" />
        <source>The form is locked: the text of the resource cannot be changed field by field safely (%1). Edit the text directly instead.</source>
        <translation>Le formulaire est verrouillé : le texte de la ressource ne peut pas être modifié champ par champ en toute sécurité (%1). Modifiez plutôt le texte directement.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1100" />
        <source>Creates a resource with the text of this one and a new name</source>
        <translation>Crée une ressource avec le texte de celle-ci et un nouveau nom</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1101" />
        <source>Copying needs the configuration methods of the gRPC interface of the Onesimus Director (read and create) for this resource type</source>
        <translation>La copie nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (lecture et création) pour ce type de ressource</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1199" />
        <source>%1: %2 → %3</source>
        <translation>%1 : %2 → %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1202" />
        <source>(not set here, inherited)</source>
        <translation>(non défini ici, hérité)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1262" />
        <source>%1 was not written: changing it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 n'a pas été écrit : sa modification nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director. Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1267" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1317" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1361" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1394" />
        <source>Another change is being written.</source>
        <translation>Une autre modification est en cours d'écriture.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1275" />
        <source>The text as edited</source>
        <translation>Le texte tel que modifié</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1280" />
        <source>%1 cannot be written: %2</source>
        <translation>%1 ne peut pas être écrit : %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1281" />
        <source>%1 was not written</source>
        <translation>%1 n'a pas été écrit</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1282" />
        <source>The change could not be written into the text of the resource safely: %1

Nothing was sent.</source>
        <translation>La modification n'a pas pu être écrite dans le texte de la ressource en toute sécurité : %1

Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1292" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1323" />
        <source>Jobs and JobDefs that use this JobDefs</source>
        <translation>Jobs et JobDefs qui utilisent ce JobDefs</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1293" />
        <source>A JobDefs gives its values to every job that does not set them itself; the change applies to those of the %n resource(s) below.</source>
        <translation>
            <numerusform>Un JobDefs transmet ses valeurs à chaque job qui ne les définit pas lui-même ; la modification s'applique à celles de la %n ressource ci-dessous.</numerusform>
            <numerusform>Un JobDefs transmet ses valeurs à chaque job qui ne les définit pas lui-même ; la modification s'applique à celles des %n ressources ci-dessous.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1297" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1326" />
        <source>Jobs affected</source>
        <translation>Jobs concernés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1298" />
        <source>Lines that were not edited stay as they are in the file.</source>
        <translation>Les lignes non modifiées restent telles quelles dans le fichier.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1300" />
        <source>The Director checks the change of %1...</source>
        <translation>Le Director vérifie la modification de %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1312" />
        <source>%1 was not deleted: deleting it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 n'a pas été supprimé : sa suppression nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director. Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1327" />
        <source>The job's records in the catalog stay; only its definition is removed. A running job is not touched.</source>
        <translation>Les enregistrements du job dans le catalogue sont conservés ; seule sa définition est supprimée. Un job en cours d'exécution n'est pas affecté.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1330" />
        <source>The Director checks whether %1 can be deleted...</source>
        <translation>Le Director vérifie si %1 peut être supprimé...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1372" />
        <source>A new job. The Director checks that everything it needs is set, here or in its JobDefs.</source>
        <translation>Un nouveau job. Le Director vérifie que tout ce dont il a besoin est défini, ici ou dans son JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1374" />
        <source>A new JobDefs. No job uses it until a job names it.</source>
        <translation>Un nouveau JobDefs. Aucun job ne l'utilise tant qu'un job ne le nomme pas.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1376" />
        <source>The Director checks the new %1 %2...</source>
        <translation>Le Director vérifie le nouveau %1 %2...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1388" />
        <source>%1 %2 was not copied: copying needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 %2 n'a pas été copié : la copie nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director. Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1408" />
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1417" />
        <source>%1 %2 was not copied: %3</source>
        <translation>%1 %2 n'a pas été copié : %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1421" />
        <source>A copy of %1 %2 with the new name.</source>
        <translation>Une copie de %1 %2 avec le nouveau nom.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1427" />
        <source>The Director checks the copy %1...</source>
        <translation>Le Director vérifie la copie %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1442" />
        <source>The Director reloaded its configuration.</source>
        <translation>Le Director a rechargé sa configuration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1444" />
        <source>Follow-up commands: %1</source>
        <translation>Commandes de suivi : %1</translation>
    </message>
</context>
<context>
    <name>BJobDetailsDialog</name>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="31" />
        <source>Job Details: %1 (ID: %2) - Client: %3</source>
        <translation>Détails du job : %1 (ID : %2) - Client : %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="70" />
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="79" />
        <source>Job Information</source>
        <translation>Informations sur le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="82" />
        <source>Job ID:</source>
        <translation>ID du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="83" />
        <source>Job Name:</source>
        <translation>Nom du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="84" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="88" />
        <source>Backup</source>
        <translation>Backup</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="89" />
        <source>Type:</source>
        <translation>Type :</translation>
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
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="101" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="102" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="108" />
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="236" />
        <source>Schedule:</source>
        <translation>Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="116" />
        <source>Timing</source>
        <translation>Horaires</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="123" />
        <source>Start Time:</source>
        <translation>Heure de début :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="127" />
        <source>Duration:</source>
        <translation>Durée :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="132" />
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="136" />
        <source>Files Backed Up:</source>
        <translation>Fichiers sauvegardés :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="143" />
        <source>Data Size:</source>
        <translation>Taille des données :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="148" />
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="165" />
        <source>Status</source>
        <translation>Statut</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="159" />
        <source>Job Status:</source>
        <translation>Statut du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="179" />
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="183" />
        <source>Logs</source>
        <translation>Journaux</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="196" />
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director.</translation>
    </message>
</context>
<context>
    <name>BJobLogDialog</name>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="27" />
        <source>Job Log: %1 (ID: %2) - %3</source>
        <translation>Journal du job : %1 (ID : %2) - %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="42" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="43" />
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="44" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="45" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="48" />
        <source>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</source>
        <translation>&lt;b&gt;Statut :&lt;/b&gt; %1 | &lt;b&gt;Début :&lt;/b&gt; %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="65" />
        <source>Copy Selection</source>
        <translation>Copier la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="71" />
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
</context>
<context>
    <name>BJobLogView</name>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="67" />
        <source>No log entries for this job.</source>
        <translation>Aucune entrée de journal pour ce job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="71" />
        <source>Could not load the job log: %1</source>
        <translation>Impossible de charger le journal du job : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="101" />
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="105" />
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
</context>
<context>
    <name>BJobPreviewPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="697" />
        <source>Preview &amp; Deploy</source>
        <translation>Aperçu &amp; déploiement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="698" />
        <source>Review the configuration and deploy to the Director.</source>
        <translation>Vérifiez la configuration et déployez-la sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="703" />
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="715" />
        <source>Copy Configuration</source>
        <translation>Copier la configuration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="722" />
        <source>Director Command:</source>
        <translation>Commande du Director :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="734" />
        <source>Copy Command</source>
        <translation>Copier la commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="787" />
        <source>Note: RunScript blocks cannot be sent via 'configure add'. The command above creates the job without RunScript blocks. Deploy the full configuration manually using 'Copy Configuration' to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.</source>
        <translation>Remarque : les blocs RunScript ne peuvent pas être envoyés via 'configure add'. La commande ci-dessus crée le job sans blocs RunScript. Déployez manuellement la configuration complète à l'aide de « Copier la configuration » dans /etc/bareos/bareos-dir.d/job/%1.conf et rechargez le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="810" />
        <source>Deploy to Director</source>
        <translation>Déployer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="811" />
        <source>Send 'configure add' to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Envoyer 'configure add' au Director ?

Cliquez sur Oui pour exécuter, sur Non pour fermer sans déployer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="833" />
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="868" />
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="877" />
        <source>Resource created successfully.</source>
        <translation>Ressource créée avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="885" />
        <source>Resource created. Reloading Director...</source>
        <translation>Ressource créée. Rechargement du Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="908" />
        <source>Resource created and Director reloaded successfully.</source>
        <translation>Ressource créée et Director rechargé avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="911" />
        <source>Resource created, but reloading the Director failed. Run 'reload' on the Director.</source>
        <translation>Ressource créée, mais le rechargement du Director a échoué. Exécutez 'reload' sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="924" />
        <source>Resource created, but the connection to the Director was lost before the reload was confirmed.</source>
        <translation>Ressource créée, mais la connexion au Director a été perdue avant la confirmation du rechargement.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="928" />
        <source>The connection to the Director was lost before it answered. Reconnect and check whether the resource exists.</source>
        <translation>La connexion au Director a été perdue avant sa réponse. Reconnectez-vous et vérifiez si la ressource existe.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="940" />
        <source>Resource created, but the Director did not confirm the reload in time. Run 'reload' on the Director if the job does not appear.</source>
        <translation>Ressource créée, mais le Director n'a pas confirmé le rechargement à temps. Exécutez 'reload' sur le Director si le job n'apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="943" />
        <source>Timeout waiting for Director response. The command may still be processing.</source>
        <translation>Délai d'attente de la réponse du Director dépassé. La commande est peut-être encore en cours de traitement.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="952" />
        <source>Configuration copied to clipboard.</source>
        <translation>Configuration copiée dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="959" />
        <source>Command copied to clipboard.</source>
        <translation>Commande copiée dans le presse-papiers.</translation>
    </message>
</context>
<context>
    <name>BJobResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256" />
        <source>Level</source>
        <translation>Niveau</translation>
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
        <translation>Références de ressources</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="340" />
        <source>Select the resources this job uses.</source>
        <translation>Sélectionnez les ressources utilisées par ce job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="356" />
        <source>Required Resources</source>
        <translation>Ressources requises</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="361" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="365" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="369" />
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="373" />
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="377" />
        <source>Messages:</source>
        <translation>Messages :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="382" />
        <source>Job Settings</source>
        <translation>Paramètres du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="388" />
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="392" />
        <source>Schedule:</source>
        <translation>Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="396" />
        <source>Catalog:</source>
        <translation>Catalog :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="401" />
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="406" />
        <source>Max Concurrent Jobs:</source>
        <translation>Max Concurrent Jobs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="411" />
        <source>Restore Settings</source>
        <translation>Paramètres de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="415" />
        <source>/tmp/bareos-restores</source>
        <translation>/tmp/bareos-restores</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="416" />
        <source>Where:</source>
        <translation>Where :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="420" />
        <source>Replace:</source>
        <translation>Replace :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="425" />
        <source>Pool Overrides</source>
        <translation>Remplacements de pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="432" />
        <source>Full Backup Pool:</source>
        <translation>Full Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="436" />
        <source>Diff Backup Pool:</source>
        <translation>Diff Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="440" />
        <source>Inc Backup Pool:</source>
        <translation>Inc Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="445" />
        <source>Simple Scripts</source>
        <translation>Scripts simples</translation>
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
        <translation>Run Before Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="454" />
        <source>Run After Job:</source>
        <translation>Run After Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="458" />
        <source>Run After Failed Job:</source>
        <translation>Run After Failed Job :</translation>
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
        <translation>Client Run Before :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="466" />
        <source>Client Run After:</source>
        <translation>Client Run After :</translation>
    </message>
</context>
<context>
    <name>BJobScriptsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="607" />
        <source>RunScript Blocks</source>
        <translation>Blocs RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="608" />
        <source>Define complex script execution blocks (optional). For simple scripts, use the fields on the previous page.</source>
        <translation>Définissez des blocs d'exécution de scripts complexes (facultatif). Pour des scripts simples, utilisez les champs de la page précédente.</translation>
    </message>
</context>
<context>
    <name>BJobWidget</name>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="48" />
        <source>Successful (T)</source>
        <translation>Réussi (T)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="49" />
        <source>Warning (W)</source>
        <translation>Avertissement (W)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="51" />
        <source>Error (E)</source>
        <translation>Erreur (E)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="56" />
        <source>Enable Date Filter</source>
        <translation>Activer le filtre de date</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="400" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1660" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1663" />
        <source>Filter</source>
        <translation>Filtre</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="459" />
        <source>Job Name:</source>
        <translation>Nom du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="460" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="513" />
        <source>Backup Level</source>
        <translation>Niveau de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="545" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="546" />
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="547" />
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="533" />
        <source>Date Range</source>
        <translation>Plage de dates</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="39" />
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="40" />
        <source>Clear History</source>
        <translation>Effacer l'historique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="50" />
        <source>Failed (F)</source>
        <translation>Échec (F)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="52" />
        <source>Running (R)</source>
        <translation>En cours (R)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="53" />
        <source>Canceled (A)</source>
        <translation>Annulé (A)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="54" />
        <location filename="../src/jobs/bjobwidget.cpp" line="356" />
        <source>Zero Bytes</source>
        <translation>Zéro octet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="81" />
        <source>When enabled, only shows jobs with 0 bytes transferred.
Warning: This filter persists across restarts and may
cause an empty table if no zero-byte jobs exist.</source>
        <translation>Si activé, affiche uniquement les jobs ayant transféré 0 octet.
Attention : ce filtre est conservé après un redémarrage et peut
produire un tableau vide s'il n'existe aucun job à zéro octet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="89" />
        <source>When enabled, only shows jobs within the selected date range.
Warning: This filter persists across restarts and may
cause an empty or limited table if the range is too narrow.</source>
        <translation>Si activé, affiche uniquement les jobs compris dans la plage de dates sélectionnée.
Attention : ce filtre est conservé après un redémarrage et peut
produire un tableau vide ou limité si la plage est trop étroite.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="105" />
        <source>Filter by job name...</source>
        <translation>Filtrer par nom de job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="109" />
        <source>Filter by client...</source>
        <translation>Filtrer par client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="114" />
        <location filename="../src/jobs/bjobwidget.cpp" line="117" />
        <location filename="../src/jobs/bjobwidget.cpp" line="120" />
        <source>(no job selected)</source>
        <translation>(aucun job sélectionné)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="133" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Load error</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - Erreur de chargement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="157" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1710" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - No job selected</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Aucun job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="161" />
        <source>-- Select Job Log --</source>
        <translation>-- Sélectionner un journal de job --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="163" />
        <source>Select from previously loaded job logs</source>
        <translation>Sélectionner parmi les journaux de jobs chargés précédemment</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="165" />
        <source>Copy selected lines to clipboard</source>
        <translation>Copier les lignes sélectionnées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="168" />
        <source>Clear job log history</source>
        <translation>Effacer l'historique des journaux de jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="171" />
        <source>Search in log...</source>
        <translation>Rechercher dans le journal...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="173" />
        <source>Filter log lines by search text</source>
        <translation>Filtrer les lignes du journal par texte recherché</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="206" />
        <source>Cancelling job...</source>
        <translation>Annulation du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="209" />
        <source>Deleting job...</source>
        <translation>Suppression du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="212" />
        <source>Running job again...</source>
        <translation>Nouvelle exécution du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="215" />
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="219" />
        <source>Pruning expired jobs...</source>
        <translation>Élagage des jobs expirés...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="260" />
        <source>⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. This filter persists across restarts!</source>
        <translation>⚠ Filtre Zéro octet actif — seuls les jobs avec 0 octet sont affichés. Ce filtre est conservé après un redémarrage !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="270" />
        <source>⚠ Date filter active — only jobs within the selected range are shown. This filter persists across restarts!</source>
        <translation>⚠ Filtre de date actif — seuls les jobs compris dans la plage sélectionnée sont affichés. Ce filtre est conservé après un redémarrage !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="359" />
        <source>Date Range (%1 – %2)</source>
        <translation>Plage de dates (%1 – %2)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="372" />
        <source>Status (hidden: %1)</source>
        <translation>Statut (masqués : %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="377" />
        <source>⚠ Active filters from last session: %1 — results may be limited</source>
        <translation>⚠ Filtres actifs de la dernière session : %1 — les résultats peuvent être limités</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="403" />
        <source>Show/hide filter</source>
        <translation>Afficher/masquer les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="409" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="413" />
        <source>Auto-Refresh</source>
        <translation>Actualisation automatique</translation>
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
        <translation>&lt;b&gt;Statut :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="480" />
        <source>Basic Filters</source>
        <translation>Filtres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="502" />
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="527" />
        <source>From:</source>
        <translation>Du :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="528" />
        <source>To:</source>
        <translation>Au :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="552" />
        <source>Job Details</source>
        <translation>Détails du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="557" />
        <source>Reset filter</source>
        <translation>Réinitialiser les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="678" />
        <source>Updated: %1 - %2 jobs loaded</source>
        <translation>Mis à jour : %1 - %2 jobs chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="723" />
        <source>No selection</source>
        <translation>Aucune sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="724" />
        <source>Please select a job to cancel.</source>
        <translation>Veuillez sélectionner un job à annuler.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="735" />
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="736" />
        <source>Do you really want to cancel job ID %1?</source>
        <translation>Voulez-vous vraiment annuler le job d'ID %1 ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="744" />
        <source>Job Cancelled</source>
        <translation>Job annulé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="745" />
        <source>Job %1 was cancelled.</source>
        <translation>Le job %1 a été annulé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1142" />
        <source>Loading page %1...</source>
        <translation>Chargement de la page %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1155" />
        <source>Refreshing job list...</source>
        <translation>Actualisation de la liste des jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1280" />
        <source>%1 jobs loaded</source>
        <translation>%1 jobs chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1515" />
        <source>Auto-refresh on (%1)</source>
        <translation>Actualisation automatique activée (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1520" />
        <source>Auto-refresh off</source>
        <translation>Actualisation automatique désactivée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1615" />
        <source>Filtered: %1 of %2 jobs</source>
        <translation>Filtrés : %1 jobs sur %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1661" />
        <source>Hide filter</source>
        <translation>Masquer les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1664" />
        <source>Show filter</source>
        <translation>Afficher les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1692" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Loading...</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1730" />
        <location filename="../src/jobs/bjobwidget.cpp" line="1867" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - %3 lignes</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="589" />
        <source>History:</source>
        <translation>Historique :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="609" />
        <source>Job Log</source>
        <translation>Journal du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="612" />
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1194" />
        <source>Error parsing JSON data</source>
        <translation>Erreur lors de l'analyse des données JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1546" />
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1547" />
        <source>Running</source>
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1548" />
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1549" />
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1550" />
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1551" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1552" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1553" />
        <source>Non-fatal Error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1554" />
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1608" />
        <source>⚠ No jobs visible — 'Zero Bytes' filter is active!</source>
        <translation>⚠ Aucun job visible — le filtre « Zéro octet » est actif !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1610" />
        <source>⚠ No jobs visible — check your filter settings (%1 jobs loaded)</source>
        <translation>⚠ Aucun job visible — vérifiez vos paramètres de filtre (%1 jobs chargés)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1619" />
        <source>%1 Jobs</source>
        <translation>%1 jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1725" />
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines, job is running</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - %3 lignes, job en cours</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1891" />
        <source>Copied %1 log lines to clipboard</source>
        <translation>%1 lignes de journal copiées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1906" />
        <source>Copied %1 selected lines to clipboard</source>
        <translation>%1 lignes sélectionnées copiées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1920" />
        <source>Job log history cleared</source>
        <translation>Historique des journaux de jobs effacé</translation>
    </message>
</context>
<context>
    <name>BJobWizard</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="990" />
        <source>New Job Wizard</source>
        <translation>Assistant de nouveau job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="991" />
        <source>New JobDefs Wizard</source>
        <translation>Assistant de nouveau JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1001" />
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
</context>
<context>
    <name>BJobsModel</name>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="253" />
        <source>Job ID</source>
        <translation>ID du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="254" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="255" />
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="256" />
        <source>Start Time</source>
        <translation>Heure de début</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="257" />
        <source>End Time</source>
        <translation>Heure de fin</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="258" />
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="259" />
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="260" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="261" />
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="262" />
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="263" />
        <source>Status</source>
        <translation>Statut</translation>
    </message>
</context>
<context>
    <name>BJobsStatisticsWidget</name>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="19" />
        <source>Job Summary</source>
        <translation>Résumé des jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="25" />
        <source>Total Jobs:</source>
        <translation>Total des jobs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="28" />
        <source>Selected:</source>
        <translation>Sélectionnés :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="33" />
        <source>Status Breakdown</source>
        <translation>Répartition par statut</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="40" />
        <source>Successful:</source>
        <translation>Réussis :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="55" />
        <source>Warnings:</source>
        <translation>Avertissements :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="70" />
        <source>Failed:</source>
        <translation>Échoués :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="84" />
        <source>Data Summary</source>
        <translation>Résumé des données</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="88" />
        <source>Total Files:</source>
        <translation>Total des fichiers :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="91" />
        <source>Total Size:</source>
        <translation>Taille totale :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="93" />
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="181" />
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="94" />
        <source>Date Range:</source>
        <translation>Plage de dates :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="176" />
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
</context>
<context>
    <name>BJsonJobView</name>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="526" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="612" />
        <source>Delete %1 Jobs</source>
        <translation>Supprimer %1 jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="157" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="526" />
        <source>Delete Job</source>
        <translation>Supprimer le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="146" />
        <source>Show Details...</source>
        <translation>Afficher les détails...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="151" />
        <source>Retry Job</source>
        <translation>Réessayer le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="154" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="720" />
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="162" />
        <source>View Log...</source>
        <translation>Afficher le journal...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="165" />
        <source>Restore Files...</source>
        <translation>Restaurer des fichiers...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="172" />
        <source>Export to JSON...</source>
        <translation>Exporter en JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="176" />
        <source>Export to CSV...</source>
        <translation>Exporter en CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="183" />
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="187" />
        <source>Invert Selection</source>
        <translation>Inverser la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="191" />
        <source>Clear Selection</source>
        <translation>Effacer la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272" />
        <source>Export Selected Jobs to JSON</source>
        <translation>Exporter les jobs sélectionnés en JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272" />
        <source>Export All Jobs to JSON</source>
        <translation>Exporter tous les jobs en JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="285" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="318" />
        <source>Export Error</source>
        <translation>Erreur d'exportation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="286" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="319" />
        <source>Could not write to file: %1</source>
        <translation>Impossible d'écrire dans le fichier : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="293" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="326" />
        <source>Export Successful</source>
        <translation>Exportation réussie</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjsonjobview.cpp" line="294" />
        <location filename="../src/jobs/bjsonjobview.cpp" line="327" />
        <source>Exported %n job(s) to %1</source>
        <translation>
            <numerusform>%n job exporté vers %1</numerusform>
            <numerusform>%n jobs exportés vers %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305" />
        <source>Export Selected Jobs to CSV</source>
        <translation>Exporter les jobs sélectionnés en CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305" />
        <source>Export All Jobs to CSV</source>
        <translation>Exporter tous les jobs en CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="339" />
        <source>Total Jobs: %1
</source>
        <translation>Total des jobs : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="340" />
        <source>Successful: %1
</source>
        <translation>Réussis : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="341" />
        <source>Warnings: %1
</source>
        <translation>Avertissements : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="342" />
        <source>Failed: %1

</source>
        <translation>Échoués : %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="344" />
        <source>Total Files: %1
</source>
        <translation>Total des fichiers : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="345" />
        <source>Total Size: %1 bytes

</source>
        <translation>Taille totale : %1 octets

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="348" />
        <source>Date Range:
</source>
        <translation>Plage de dates :
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="349" />
        <source>  From: %1
</source>
        <translation>  De : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="350" />
        <source>  To: %1

</source>
        <translation>  À : %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="353" />
        <source>Selected: %1</source>
        <translation>Sélectionnés : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="355" />
        <source>Job Statistics</source>
        <translation>Statistiques des jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="420" />
        <source>Export All to JSON...</source>
        <translation>Tout exporter en JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="421" />
        <source>Export All to CSV...</source>
        <translation>Tout exporter en CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="536" />
        <source>ID %1: %2 (%3)</source>
        <translation>ID %1 : %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="542" />
        <source>&lt;b&gt;%1 jobs selected for deletion:&lt;/b&gt;&lt;br&gt;%2</source>
        <translation>&lt;b&gt;%1 jobs sélectionnés pour suppression :&lt;/b&gt;&lt;br&gt;%2</translation>
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
        <translation>&lt;b&gt;Job :&lt;/b&gt; %1 (ID : %2)&lt;br&gt;&lt;b&gt;Client :&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Niveau :&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Début :&lt;/b&gt; %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="569" />
        <source>&lt;div style='background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'&gt;&lt;b&gt;⚠ Attention:&lt;/b&gt; %1 dependent Incremental/Differential job(s) found!&lt;br&gt;&lt;br&gt;Deleting Full backups may make dependent backups unusable for restore.&lt;/div&gt;</source>
        <translation>&lt;div style='background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;'&gt;&lt;b&gt;⚠ Attention :&lt;/b&gt; %1 job(s) Incremental/Differential dépendant(s) trouvé(s) !&lt;br&gt;&lt;br&gt;La suppression de sauvegardes Full peut rendre les sauvegardes dépendantes inutilisables pour la restauration.&lt;/div&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="580" />
        <source>Delete Mode</source>
        <translation>Mode de suppression</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="583" />
        <source>Delete job record only</source>
        <translation>Supprimer uniquement l'enregistrement du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="584" />
        <source>Removes the job entry from the catalog database.
The backup data on the storage media remains intact.</source>
        <translation>Supprime l'entrée du job de la base de données du catalogue.
Les données de sauvegarde sur les supports de stockage restent intactes.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="593" />
        <source>Prune (remove expired jobs by retention policy)</source>
        <translation>Élaguer (supprimer les jobs expirés selon la politique de rétention)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="594" />
        <source>Removes job records that have exceeded their configured retention period.
Only expired jobs for the affected client(s) will be removed.</source>
        <translation>Supprime les enregistrements de jobs ayant dépassé leur durée de rétention configurée.
Seuls les jobs expirés du ou des clients concernés seront supprimés.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="603" />
        <source>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warning:&lt;/b&gt; This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Attention :&lt;/b&gt; cette action est irréversible !&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="612" />
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="626" />
        <source>  ID %1: %2 (%3)</source>
        <translation>  ID %1 : %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="631" />
        <source>Confirm Delete</source>
        <translation>Confirmer la suppression</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="632" />
        <source>Are you sure you want to delete %1 job(s)?

%2

%3 dependent Incremental/Differential backup(s) may become unusable for restore.</source>
        <translation>Voulez-vous vraiment supprimer %1 job(s) ?

%2

%3 sauvegarde(s) Incremental/Differential dépendante(s) peuvent devenir inutilisables pour la restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="688" />
        <source>Rerun Job</source>
        <translation>Réexécuter le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="689" />
        <source>Do you want to run job %1 (%2) again?</source>
        <translation>Voulez-vous exécuter à nouveau le job %1 (%2) ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="721" />
        <source>Do you really want to cancel the running job %1 (%2)?</source>
        <translation>Voulez-vous vraiment annuler le job en cours %1 (%2) ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="750" />
        <source>Not connected</source>
        <translation>Aucune connexion</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="751" />
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director disponible.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="772" />
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="773" />
        <source>No director connection available.</source>
        <translation>Aucune connexion au Director disponible.</translation>
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
        <translation>Zoom de l'interface : %1 %</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="202" />
        <source>Connecting...</source>
        <translation>Connexion...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="205" />
        <source>Authenticating...</source>
        <translation>Authentification...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="208" />
        <source>Setting API mode...</source>
        <translation>Définition du mode API...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="211" />
        <source>Loading resources...</source>
        <translation>Chargement des ressources...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="214" />
        <source>Connected and ready</source>
        <translation>Connecté et prêt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="217" />
        <source>Connection error</source>
        <translation>Erreur de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="220" />
        <location filename="../src/bmainwindow.cpp" line="1694" />
        <source>Disconnected</source>
        <translation>Déconnecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="227" />
        <source>Loading resources... %1/%2</source>
        <translation>Chargement des ressources... %1/%2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="249" />
        <location filename="../src/bmainwindow.cpp" line="686" />
        <location filename="../src/bmainwindow.cpp" line="2525" />
        <source>Ready</source>
        <translation>Prêt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="400" />
        <location filename="../src/bmainwindow.cpp" line="2885" />
        <source>Auto-refresh enabled (%1s)</source>
        <translation>Actualisation automatique activée (%1 s)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="404" />
        <location filename="../src/bmainwindow.cpp" line="2890" />
        <source>Auto-refresh disabled</source>
        <translation>Actualisation automatique désactivée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="472" />
        <source>Job: %1</source>
        <translation>Job : %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="533" />
        <location filename="../src/bmainwindow.cpp" line="690" />
        <location filename="../src/bmainwindow.cpp" line="2441" />
        <source>Not connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="609" />
        <source>Storage/Volumes</source>
        <translation>Storage/Volumes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="667" />
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="700" />
        <location filename="../src/bmainwindow.cpp" line="717" />
        <location filename="../src/bmainwindow.cpp" line="1629" />
        <location filename="../src/bmainwindow.cpp" line="1886" />
        <source>Connect</source>
        <translation>Se connecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="705" />
        <location filename="../src/bmainwindow.cpp" line="723" />
        <location filename="../src/bmainwindow.cpp" line="1887" />
        <source>Reconnect</source>
        <translation>Se reconnecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="711" />
        <source>Disconnect</source>
        <translation>Se déconnecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="719" />
        <location filename="../src/bmainwindow.cpp" line="1551" />
        <location filename="../src/bmainwindow.cpp" line="2340" />
        <source>Connect to Director</source>
        <translation>Se connecter au Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="725" />
        <source>Reconnect to last used connection</source>
        <translation>Se reconnecter à la dernière connexion utilisée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="729" />
        <location filename="../src/bmainwindow.cpp" line="1888" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="735" />
        <location filename="../src/bmainwindow.cpp" line="1889" />
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="740" />
        <location filename="../src/bmainwindow.cpp" line="1890" />
        <source>Exit</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="745" />
        <location filename="../src/bmainwindow.cpp" line="1719" />
        <source>About Onesimus</source>
        <translation>À propos d'Onesimus</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="749" />
        <source>About Qt</source>
        <translation>À propos de Qt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="753" />
        <source>Online Documentation</source>
        <translation>Documentation en ligne</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="758" />
        <source>Report a Bug...</source>
        <translation>Signaler un bogue...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="762" />
        <location filename="../src/bmainwindow.cpp" line="1866" />
        <location filename="../src/bmainwindow.cpp" line="1872" />
        <location filename="../src/bmainwindow.cpp" line="1953" />
        <source>Keyboard Shortcuts</source>
        <translation>Raccourcis clavier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="767" />
        <location filename="../src/bmainwindow.cpp" line="2287" />
        <source>My Permissions</source>
        <translation>Mes autorisations</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="773" />
        <location filename="../src/bmainwindow.cpp" line="1903" />
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="779" />
        <location filename="../src/bmainwindow.cpp" line="1904" />
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="785" />
        <source>Clear Selection</source>
        <translation>Effacer la sélection</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="790" />
        <source>Find...</source>
        <translation>Rechercher...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="797" />
        <location filename="../src/bmainwindow.cpp" line="1918" />
        <source>Show Statistics</source>
        <translation>Afficher les statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="811" />
        <location filename="../src/bmainwindow.cpp" line="1919" />
        <source>Show Job Log</source>
        <translation>Afficher le journal du job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="826" />
        <location filename="../src/bmainwindow.cpp" line="1921" />
        <source>Interface Zoom In</source>
        <translation>Agrandir l'interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="832" />
        <location filename="../src/bmainwindow.cpp" line="1922" />
        <source>Interface Zoom Out</source>
        <translation>Réduire l'interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="844" />
        <source>Toggle Dark/Light Theme</source>
        <translation>Basculer thème sombre/clair</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="858" />
        <source>Run Job</source>
        <translation>Exécuter le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="863" />
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="868" />
        <source>Show Details</source>
        <translation>Afficher les détails</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="873" />
        <location filename="../src/bmainwindow.cpp" line="1936" />
        <source>Refresh Jobs</source>
        <translation>Actualiser les jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="879" />
        <source>Export Jobs as JSON...</source>
        <translation>Exporter les jobs en JSON...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="884" />
        <source>Export Jobs as CSV...</source>
        <translation>Exporter les jobs en CSV...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="890" />
        <source>Add Job...</source>
        <translation>Ajouter un job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="909" />
        <source>Edit Job...</source>
        <translation>Modifier le job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="915" />
        <source>Edit Job</source>
        <translation>Modifier le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="916" />
        <source>No Jobs available.</source>
        <translation>Aucun job disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="922" />
        <source>Job Resources</source>
        <translation>Ressources Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="975" />
        <source>Delete Job...</source>
        <translation>Supprimer le job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="983" />
        <source>Add JobDefs...</source>
        <translation>Ajouter un JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1002" />
        <source>Edit JobDefs...</source>
        <translation>Modifier le JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1008" />
        <source>JobDefs Resources</source>
        <translation>Ressources JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1063" />
        <source>Delete JobDefs...</source>
        <translation>Supprimer le JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074" />
        <location filename="../src/bmainwindow.cpp" line="1078" />
        <source>Delete JobDefs</source>
        <translation>Supprimer le JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074" />
        <source>No JobDefs available.</source>
        <translation>Aucun JobDefs disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1078" />
        <source>JobDefs to delete:</source>
        <translation>JobDefs à supprimer :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1092" />
        <source>Jobs that use this JobDefs</source>
        <translation>Jobs qui utilisent ce JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1098" />
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1108" />
        <source>Add FileSet...</source>
        <translation>Ajouter un FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1116" />
        <source>Edit FileSet...</source>
        <translation>Modifier le FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1124" />
        <source>Delete FileSet...</source>
        <translation>Supprimer le FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1133" />
        <location filename="../src/bmainwindow.cpp" line="1139" />
        <location filename="../src/bmainwindow.cpp" line="1183" />
        <location filename="../src/bmainwindow.cpp" line="1188" />
        <source>Delete FileSet</source>
        <translation>Supprimer le FileSet</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1134" />
        <source>No FileSets available.</source>
        <translation>Aucun FileSet disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1144" />
        <source>Select the FileSet to delete:</source>
        <translation>Sélectionnez le FileSet à supprimer :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1154" />
        <source>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Warning:&lt;/b&gt; This will remove the FileSet resource from the Director configuration. This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style='color: #cc0000;'&gt;&lt;b&gt;Attention :&lt;/b&gt; cette opération supprimera la ressource FileSet de la configuration du Director. Cette action est irréversible !&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1164" />
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1176" />
        <source>Confirm Delete</source>
        <translation>Confirmer la suppression</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1177" />
        <source>Are you sure you want to delete FileSet "%1"?</source>
        <translation>Voulez-vous vraiment supprimer le FileSet « %1 » ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1184" />
        <source>Not connected to a Director. FileSet "%1" was not deleted.</source>
        <translation>Non connecté à un Director. Le FileSet « %1 » n'a pas été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1189" />
        <source>FileSet "%1" no longer exists on the Director. Nothing was deleted.</source>
        <translation>Le FileSet « %1 » n'existe plus sur le Director. Rien n'a été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1197" />
        <source>Add New Client...</source>
        <translation>Ajouter un nouveau client...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1205" />
        <location filename="../src/bmainwindow.cpp" line="1937" />
        <source>Refresh Clients</source>
        <translation>Actualiser les clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1211" />
        <source>Show Client Details</source>
        <translation>Afficher les détails du client</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1221" />
        <location filename="../src/bmainwindow.cpp" line="1938" />
        <source>Refresh Storage</source>
        <translation>Actualiser les Storages</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1228" />
        <source>Add Schedule...</source>
        <translation>Ajouter un Schedule...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1250" />
        <location filename="../src/bmainwindow.cpp" line="1939" />
        <source>Refresh Schedules</source>
        <translation>Actualiser les Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1257" />
        <source>Database Cleanup...</source>
        <translation>Nettoyage de la base de données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1259" />
        <source>Clean up old backups and free disk space</source>
        <translation>Nettoyer les anciennes sauvegardes et libérer de l'espace disque</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1263" />
        <source>Connection Wizard...</source>
        <translation>Assistant de connexion...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1265" />
        <source>Set up a new director connection</source>
        <translation>Configurer une nouvelle connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1268" />
        <source>Import Config...</source>
        <translation>Importer une configuration...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1270" />
        <source>Import Director configuration from files or ZIP archive</source>
        <translation>Importer la configuration du Director depuis des fichiers ou une archive ZIP</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1279" />
        <location filename="../src/bmainwindow.cpp" line="1319" />
        <location filename="../src/bmainwindow.cpp" line="1335" />
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1285" />
        <source>System (%1)</source>
        <translation>Système (%1)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1320" />
        <source>Onesimus will speak %1 after a restart.</source>
        <translation>Onesimus parlera %1 après un redémarrage.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1323" />
        <source>Restart now? The connection to the Director is closed and opened again if automatic connection is on.</source>
        <translation>Redémarrer maintenant ? La connexion au Director est fermée, puis rouverte si la connexion automatique est activée.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1325" />
        <source>Restart now?</source>
        <translation>Redémarrer maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1326" />
        <source>Restart now</source>
        <translation>Redémarrer maintenant</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1327" />
        <source>Later</source>
        <translation>Plus tard</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1336" />
        <source>Onesimus could not be started again. The new language takes effect at the next start.</source>
        <translation>Onesimus n'a pas pu être relancé. La nouvelle langue s'appliquera au prochain démarrage.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1346" />
        <location filename="../src/bmainwindow.cpp" line="1894" />
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1360" />
        <location filename="../src/bmainwindow.cpp" line="1909" />
        <source>Edit</source>
        <translation>Édition</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1369" />
        <location filename="../src/bmainwindow.cpp" line="1927" />
        <source>View</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1438" />
        <source>ACL matrix of the consoles...</source>
        <translation>Matrice des ACL des consoles...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1439" />
        <source>Which console may use which job, client, storage, schedule, pool, fileset, catalog and command (Onesimus Director)</source>
        <translation>Quelle console peut utiliser quel Job, Client, Storage, Schedule, Pool, FileSet, Catalog et commande (Onesimus Director)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bmainwindow.cpp" line="2809" />
        <source>%n row(s) copied</source>
        <translation>
            <numerusform>%n ligne copiée</numerusform>
            <numerusform>%n lignes copiées</numerusform>
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
        <translation>Modifier les jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1394" />
        <source>Modify JobDefs</source>
        <translation>Modifier les JobDefs</translation>
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
        <translation>Onesimus - Gestion des sauvegardes Bacula</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="99" />
        <source>Onesimus - Bareos Backup Management</source>
        <translation>Onesimus - Gestion des sauvegardes Bareos</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1427" />
        <source>Tools</source>
        <translation>Outils</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1443" />
        <location filename="../src/bmainwindow.cpp" line="1957" />
        <source>Help</source>
        <translation>Aide</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1472" />
        <source>Main Toolbar</source>
        <translation>Barre d'outils principale</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1501" />
        <location filename="../src/bmainwindow.cpp" line="1515" />
        <location filename="../src/bmainwindow.cpp" line="2414" />
        <source>Statistics ▼</source>
        <translation>Statistiques ▼</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1504" />
        <source>Show/Hide Statistics</source>
        <translation>Afficher/masquer les statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1515" />
        <location filename="../src/bmainwindow.cpp" line="2414" />
        <location filename="../src/bmainwindow.cpp" line="2450" />
        <source>Statistics ▶</source>
        <translation>Statistiques ▶</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1543" />
        <source>No Connection Profiles</source>
        <translation>Aucun profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1544" />
        <source>No connection profiles found.

Please create a connection profile in Settings first.</source>
        <translation>Aucun profil de connexion trouvé.

Veuillez d'abord créer un profil de connexion dans les Paramètres.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1558" />
        <source>Select Connection Profile</source>
        <translation>Sélectionner un profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1564" />
        <source>Connection Profile</source>
        <translation>Profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1613" />
        <source>Manage Profiles...</source>
        <translation>Gérer les profils...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1641" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1642" />
        <source>Selected profile is invalid.</source>
        <translation>Le profil sélectionné n'est pas valide.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1680" />
        <location filename="../src/bmainwindow.cpp" line="2539" />
        <location filename="../src/bmainwindow.cpp" line="2771" />
        <source>Connecting to %1...</source>
        <translation>Connexion à %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1739" />
        <source>&lt;p style='color:#666; margin:0;'&gt;Version %1 (Build %2)&lt;/p&gt;</source>
        <translation>&lt;p style='color:#666; margin:0;'&gt;Version %1 (Build %2)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1745" />
        <source>&lt;p style='margin:0;'&gt;Modern Qt6 GUI for Bacula Backup&lt;/p&gt;</source>
        <translation>&lt;p style='margin:0;'&gt;Interface Qt6 moderne pour les sauvegardes Bacula&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1747" />
        <source>&lt;p style='margin:0;'&gt;Modern Qt6 GUI for Bareos Backup&lt;/p&gt;</source>
        <translation>&lt;p style='margin:0;'&gt;Interface Qt6 moderne pour les sauvegardes Bareos&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1831" />
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1847" />
        <source>Documentation</source>
        <translation>Documentation</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1848" />
        <location filename="../src/bmainwindow.cpp" line="1858" />
        <source>Could not open browser. Please visit:
%1</source>
        <translation>Impossible d'ouvrir le navigateur. Veuillez consulter :
%1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1857" />
        <source>Report Bug</source>
        <translation>Signaler un bogue</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1905" />
        <source>Find</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1920" />
        <source>Toggle Theme</source>
        <translation>Changer de thème</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1923" />
        <source>Reset Interface Zoom</source>
        <translation>Réinitialiser le zoom de l'interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1943" />
        <source>Data</source>
        <translation>Données</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1952" />
        <source>Help Contents</source>
        <translation>Sommaire de l'aide</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2288" />
        <source>Not connected to a Director.</source>
        <translation>Non connecté à un Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2310" />
        <source>Settings saved</source>
        <translation>Paramètres enregistrés</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2337" />
        <source>Disconnect from Director</source>
        <translation>Se déconnecter du Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2400" />
        <source>Connected to %1 version %2</source>
        <translation>Connecté à %1 version %2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2402" />
        <source>Connected: version %1</source>
        <translation>Connecté : version %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2408" />
        <source>Connected - Loading data...</source>
        <translation>Connecté - Chargement des données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2473" />
        <source>Error: </source>
        <translation>Erreur : </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2480" />
        <source>Connection Error</source>
        <translation>Erreur de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2495" />
        <source>Refreshing data...</source>
        <translation>Actualisation des données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2534" />
        <source>No Saved Connection</source>
        <translation>Aucune connexion enregistrée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2535" />
        <source>No previous connection found. Please use 'Connect' to establish a new connection.</source>
        <translation>Aucune connexion précédente trouvée. Veuillez utiliser « Se connecter » pour établir une nouvelle connexion.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2725" />
        <source>Auto-connecting to %1...</source>
        <translation>Connexion automatique à %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2752" />
        <source>Connection cancelled: server certificate not trusted</source>
        <translation>Connexion annulée : certificat du serveur non approuvé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2815" />
        <source>Copying client data is not implemented yet</source>
        <translation>La copie des données client n'est pas encore implémentée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2828" />
        <source>All jobs selected</source>
        <translation>Tous les jobs sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2831" />
        <source>Selecting all clients is not implemented yet</source>
        <translation>La sélection de tous les clients n'est pas encore implémentée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2843" />
        <source>Selection cleared</source>
        <translation>Sélection annulée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2846" />
        <source>Clearing the client selection is not implemented yet</source>
        <translation>L'annulation de la sélection des clients n'est pas encore implémentée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2857" />
        <location filename="../src/bmainwindow.cpp" line="2867" />
        <source>Search</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2858" />
        <source>Use the filter options in the Job view
to search for specific jobs.

Available filters:
• Job Name
• Client Name
• Status (Success, Warning, Failed)
• Level (Full, Incremental, Differential)
• Date Range</source>
        <translation>Utilisez les options de filtre de la vue Jobs
pour rechercher des jobs précis.

Filtres disponibles :
• Nom du job
• Nom du client
• Statut (Réussi, Avertissement, Échec)
• Niveau (Full, Incremental, Differential)
• Plage de dates</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2868" />
        <source>Use the filter combo box in the Client view
to search for specific clients.</source>
        <translation>Utilisez la liste déroulante de filtre de la vue Clients
pour rechercher des clients précis.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2900" />
        <source>Reset Interface Zoom (now %1 %)</source>
        <translation>Réinitialiser le zoom de l'interface (actuellement %1 %)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2914" />
        <source>Switched to Light Theme</source>
        <translation>Thème clair activé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2918" />
        <source>Switched to Dark Theme</source>
        <translation>Thème sombre activé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2931" />
        <location filename="../src/bmainwindow.cpp" line="2943" />
        <location filename="../src/bmainwindow.cpp" line="2955" />
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2931" />
        <location filename="../src/bmainwindow.cpp" line="2943" />
        <source>Please connect to the Director first.</source>
        <translation>Veuillez d'abord vous connecter au Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2937" />
        <source>Jobs exported as JSON</source>
        <translation>Jobs exportés en JSON</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2949" />
        <source>Jobs exported as CSV</source>
        <translation>Jobs exportés en CSV</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2956" />
        <source>Please connect to Bareos Director first.</source>
        <translation>Veuillez d'abord vous connecter au Director Bareos.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2965" />
        <source>Loading job data...</source>
        <translation>Chargement des données des jobs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3000" />
        <source>Connection profile '%1' saved</source>
        <translation>Profil de connexion '%1' enregistré</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3085" />
        <source>Password Missing</source>
        <translation>Mot de passe manquant</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3086" />
        <source>The imported Console '%1' does not contain a password.

You will need to enter the password manually in the connection settings.</source>
        <translation>La Console importée '%1' ne contient pas de mot de passe.

Vous devrez saisir le mot de passe manuellement dans les paramètres de connexion.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3107" />
        <source>Created connection profile '%1' for Director '%2'</source>
        <translation>Profil de connexion '%1' créé pour le Director '%2'</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3112" />
        <source>Connect Now?</source>
        <translation>Se connecter maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3113" />
        <source>Connection profile '%1' has been created.

Do you want to connect now?</source>
        <translation>Le profil de connexion '%1' a été créé.

Voulez-vous vous connecter maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3129" />
        <source>Imported Director '%1' with Console '%2'</source>
        <translation>Director '%1' importé avec la Console '%2'</translation>
    </message>
</context>
<context>
    <name>BMdnsBrowser</name>
    <message>
        <location filename="../src/bmdnsbrowser.cpp" line="197" />
        <source>mDNS: cannot open UDP socket: %1</source>
        <translation>mDNS : impossible d'ouvrir le socket UDP : %1</translation>
    </message>
</context>
<context>
    <name>BMessagesResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428" />
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BMessagesWidget</name>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="232" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="238" />
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="245" />
        <source>0 messages</source>
        <translation>0 message</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="280" />
        <source>Polling every %1s</source>
        <translation>Interrogation toutes les %1 s</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="307" />
        <source>Messages cleared</source>
        <translation>Messages effacés</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="322" />
        <source>Not connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="363" />
        <source>Refreshing messages...</source>
        <translation>Actualisation des messages...</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/messages/bmessageswidget.cpp" line="369" />
        <source>%n message(s)</source>
        <translation>
            <numerusform>%n message</numerusform>
            <numerusform>%n messages</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BNewClientDirectorPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="86" />
        <source>Director Information</source>
        <translation>Informations sur le Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="87" />
        <source>The new client will connect to this Director. These settings are used to generate the File Daemon configuration.</source>
        <translation>Le nouveau client se connectera à ce Director. Ces paramètres servent à générer la configuration du File Daemon.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="94" />
        <source>Director Name:</source>
        <translation>Nom du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="98" />
        <source>Director Address:</source>
        <translation>Adresse du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="102" />
        <source>Director Port:</source>
        <translation>Port du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="106" />
        <source>TLS Mode:</source>
        <translation>Mode TLS :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="114" />
        <source>The Director name above will be used in the File Daemon's Director resource. The client authenticates against this Director using a shared password.</source>
        <translation>Le nom du Director ci-dessus sera utilisé dans la ressource Director du File Daemon. Le client s'authentifie auprès de ce Director à l'aide d'un mot de passe partagé.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="134" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="141" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="148" />
        <source>&lt;i&gt;(not available)&lt;/i&gt;</source>
        <translation>&lt;i&gt;(non disponible)&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="135" />
        <source>Director name could not be retrieved from the connection.</source>
        <translation>Le nom du Director n'a pas pu être récupéré depuis la connexion.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="142" />
        <source>Director address could not be retrieved.</source>
        <translation>L'adresse du Director n'a pas pu être récupérée.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="149" />
        <source>Director port could not be retrieved.</source>
        <translation>Le port du Director n'a pas pu être récupéré.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="154" />
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="159" />
        <source>&lt;b style='color:red;'&gt;Cannot proceed:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Make sure you are connected to a Director.</source>
        <translation>&lt;b style='color:red;'&gt;Impossible de continuer :&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Assurez-vous d'être connecté à un Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="176" />
        <source>&lt;span style='color:orange;'&gt;&lt;b&gt;Warning:&lt;/b&gt; The current console may not have permission to run 'configure add client'. Check the Command ACL in the Console resource on the Director.&lt;/span&gt;&lt;br&gt;You can still generate and export the FD configuration files.</source>
        <translation>&lt;span style='color:orange;'&gt;&lt;b&gt;Attention :&lt;/b&gt; la console actuelle n'a peut-être pas l'autorisation d'exécuter 'configure add client'. Vérifiez le Command ACL dans la ressource Console du Director.&lt;/span&gt;&lt;br&gt;Vous pouvez tout de même générer et exporter les fichiers de configuration FD.</translation>
    </message>
</context>
<context>
    <name>BNewClientPreviewPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="473" />
        <source>Preview &amp; Export</source>
        <translation>Aperçu &amp; exportation</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="474" />
        <source>Review generated configuration files, validate, and export.</source>
        <translation>Vérifiez les fichiers de configuration générés, validez-les et exportez-les.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="485" />
        <source>FD: Director</source>
        <translation>FD : Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="488" />
        <source>FD: Client</source>
        <translation>FD : Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="491" />
        <source>FD: Messages</source>
        <translation>FD : Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="494" />
        <source>Dir: Client</source>
        <translation>Dir : Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="513" />
        <source>Director Command</source>
        <translation>Commande du Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="525" />
        <source>Copy Current Tab</source>
        <translation>Copier l'onglet actuel</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="529" />
        <source>Copy All</source>
        <translation>Tout copier</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="533" />
        <source>Export ZIP...</source>
        <translation>Exporter en ZIP...</translation>
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
        <translation>%1 : la syntaxe de la configuration n'a pas pu être analysée.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="786" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="795" />
        <source>Client name '%1' does not end with '-fd'. Convention: use '&lt;hostname&gt;-fd'.</source>
        <translation>Le nom du client '%1' ne se termine pas par '-fd'. Convention : utilisez '&lt;hostname&gt;-fd'.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="800" />
        <source>Password MD5 hash has invalid length (%1, expected 32).</source>
        <translation>Le hachage MD5 du mot de passe a une longueur invalide (%1, 32 attendus).</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="806" />
        <source>Validation passed.</source>
        <translation>Validation réussie.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="899" />
        <source>Copied '%1' to clipboard.</source>
        <translation>'%1' copié dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="924" />
        <source>All configuration files copied to clipboard.</source>
        <translation>Tous les fichiers de configuration ont été copiés dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="936" />
        <source>Export Client Configuration</source>
        <translation>Exporter la configuration du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="938" />
        <source>ZIP Archives (*.zip)</source>
        <translation>Archives ZIP (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="944" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="1017" />
        <location filename="../src/clients/bnewclientdialog.cpp" line="1023" />
        <source>Export Error</source>
        <translation>Erreur d'exportation</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="945" />
        <source>Failed to create temporary directory.</source>
        <translation>Impossible de créer le répertoire temporaire.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1018" />
        <source>ZIP creation timed out. Is 'zip' installed?</source>
        <translation>La création du ZIP a dépassé le délai. 'zip' est-il installé ?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1024" />
        <source>ZIP creation failed:
%1</source>
        <translation>Échec de la création du ZIP :
%1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1029" />
        <source>Exported to: %1</source>
        <translation>Exporté vers : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1038" />
        <source>ERROR: No director connection available.</source>
        <translation>ERREUR : aucune connexion au Director disponible.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1047" />
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1088" />
        <source>Failed to add client to Director: %1
Go back to change the settings, or click Finish to send the command again.
You can still export the FD config files manually.</source>
        <translation>Impossible d'ajouter le client au Director : %1
Revenez en arrière pour modifier les paramètres, ou cliquez sur Terminer pour renvoyer la commande.
Vous pouvez tout de même exporter manuellement les fichiers de configuration FD.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1101" />
        <source>Client '%1' added to Director.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client '%1' ajouté au Director.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1111" />
        <source>Client '%1' added to Director. Reloading Director...</source>
        <translation>Client '%1' ajouté au Director. Rechargement du Director...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1138" />
        <source>Client '%1' added and Director reloaded successfully.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client '%1' ajouté et Director rechargé avec succès.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1145" />
        <source>Client '%1' added to Director, but reload failed.
Run 'reload' manually on the Director or restart bareos-dir.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client '%1' ajouté au Director, mais le rechargement a échoué.
Exécutez 'reload' manuellement sur le Director ou redémarrez bareos-dir.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1164" />
        <source>Client '%1' added to Director, but the connection was lost before the reload was confirmed.
Reconnect and run 'reload' on the Director if the client does not appear.</source>
        <translation>Client '%1' ajouté au Director, mais la connexion a été perdue avant la confirmation du rechargement.
Reconnectez-vous et exécutez 'reload' sur le Director si le client n'apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1173" />
        <source>The connection to the Director was lost before it answered.
Reconnect and check whether client '%1' exists before sending the command again.</source>
        <translation>La connexion au Director a été perdue avant sa réponse.
Reconnectez-vous et vérifiez si le client '%1' existe avant de renvoyer la commande.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1191" />
        <source>Client '%1' added to Director, but the Director did not confirm the reload within 30 seconds.
Run 'reload' manually on the Director if the client does not appear.</source>
        <translation>Client '%1' ajouté au Director, mais le Director n'a pas confirmé le rechargement dans les 30 secondes.
Exécutez 'reload' manuellement sur le Director si le client n'apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1201" />
        <source>Command Timeout</source>
        <translation>Délai de la commande dépassé</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1202" />
        <source>The 'configure add client' command did not respond within 30 seconds.

The Director may be busy or the connection may have been lost.

Retry the command?</source>
        <translation>La commande 'configure add client' n'a pas répondu dans les 30 secondes.

Le Director est peut-être occupé ou la connexion a peut-être été perdue.

Réessayer la commande ?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1214" />
        <source>Configure command timed out. The client may or may not have been created.
You can export the FD config files and configure the Director manually.</source>
        <translation>La commande configure a dépassé le délai. Le client a peut-être été créé, ou non.
Vous pouvez exporter les fichiers de configuration FD et configurer le Director manuellement.</translation>
    </message>
</context>
<context>
    <name>BNewClientSettingsPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="200" />
        <source>Client Settings</source>
        <translation>Paramètres du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="201" />
        <source>Configure the new client's identity, password, TLS, and network settings.</source>
        <translation>Configurez l'identité, le mot de passe, le TLS et les paramètres réseau du nouveau client.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="206" />
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="210" />
        <source>e.g. server1-fd</source>
        <translation>p. ex. server1-fd</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="211" />
        <source>Client Name:</source>
        <translation>Nom du client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="214" />
        <source>e.g. 192.168.1.100 or server1.example.com</source>
        <translation>p. ex. 192.168.1.100 ou server1.example.com</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="215" />
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="220" />
        <source>FD Port:</source>
        <translation>Port FD :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="225" />
        <source>Client password</source>
        <translation>Mot de passe du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="227" />
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="229" />
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="235" />
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="245" />
        <source>Passive mode (client behind NAT/firewall)</source>
        <translation>Mode passif (client derrière un NAT/pare-feu)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="247" />
        <source>Enable if the client is behind a firewall/NAT.
The Director and Storage Daemon will initiate connections to the client instead.

Firewall ports to open on the client side:
  - Inbound to File Daemon: TCP 9102</source>
        <translation>Activez cette option si le client se trouve derrière un pare-feu/NAT.
Le Director et le Storage Daemon initieront alors les connexions vers le client.

Ports du pare-feu à ouvrir côté client :
  - Entrant vers le File Daemon : TCP 9102</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="256" />
        <source>Connection wait time:</source>
        <translation>Temps d'attente de connexion :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="260" />
        <source> sec</source>
        <translation> s</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="261" />
        <source>Time in seconds to wait for client-initiated connection</source>
        <translation>Durée en secondes d'attente d'une connexion initiée par le client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="270" />
        <source>Execute "configure add client" on Director</source>
        <translation>Exécuter « configure add client » sur le Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="273" />
        <source>If checked, the wizard will send the configure command to the Director
on the Preview page to add this client automatically.</source>
        <translation>Si cette case est cochée, l'assistant enverra la commande configure au Director
sur la page Aperçu pour ajouter automatiquement ce client.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="409" />
        <source>(enter password)</source>
        <translation>(saisir le mot de passe)</translation>
    </message>
</context>
<context>
    <name>BNewClientWizard</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="51" />
        <source>Add New Client</source>
        <translation>Ajouter un nouveau client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="65" />
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="67" />
        <source>TLS Certificate</source>
        <translation>Certificat TLS</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="70" />
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
</context>
<context>
    <name>BNewJobDialog</name>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="24" />
        <source>Run New Job</source>
        <translation>Exécuter un nouveau job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="50" />
        <source>Run Backup Job</source>
        <translation>Exécuter un job de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="60" />
        <source>Command Preview</source>
        <translation>Aperçu de la commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="79" />
        <source>Estimate</source>
        <translation>Estimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="81" />
        <source>Calculate estimated files and size</source>
        <translation>Calculer le nombre de fichiers et la taille estimés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="91" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="665" />
        <source>Run Job</source>
        <translation>Exécuter le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="113" />
        <source>Job Configuration</source>
        <translation>Configuration du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="118" />
        <source>Select job...</source>
        <translation>Sélectionner un job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="121" />
        <source>Job:</source>
        <translation>Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="124" />
        <source>Select client...</source>
        <translation>Sélectionner un client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="127" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="133" />
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="138" />
        <source>Resources</source>
        <translation>Ressources</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="143" />
        <source>Select FileSet...</source>
        <translation>Sélectionner un FileSet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="146" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="149" />
        <source>Select Pool...</source>
        <translation>Sélectionner un Pool...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="152" />
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="155" />
        <source>Select Storage...</source>
        <translation>Sélectionner un Storage...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="158" />
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="163" />
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="169" />
        <source>Lower values = higher priority. Only sent if changed here; otherwise the job's own priority applies.</source>
        <translation>Valeur plus basse = priorité plus haute. Envoyée uniquement si modifiée ici ; sinon, la priorité propre du job s'applique.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="176" />
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="182" />
        <source>Basic</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="192" />
        <source>Scheduling</source>
        <translation>Planification</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="196" />
        <source>e.g. "2024-01-15 14:30:00" or empty for immediate</source>
        <translation>p. ex. "2024-01-15 14:30:00" ou vide pour immédiat</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="197" />
        <source>Start time in the form YYYY-MM-DD HH:MM:SS</source>
        <translation>Heure de début au format YYYY-MM-DD HH:MM:SS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="199" />
        <source>When:</source>
        <translation>Quand :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="204" />
        <source>Bootstrap Options</source>
        <translation>Options de bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="207" />
        <source>Use Bootstrap file</source>
        <translation>Utiliser un fichier bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="212" />
        <source>Path to Bootstrap file</source>
        <translation>Chemin du fichier bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="222" />
        <source>Replace</source>
        <translation>Remplacer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="225" />
        <source>Restore mode: Replace existing files</source>
        <translation>Mode de restauration : remplacer les fichiers existants</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="229" />
        <source>Always</source>
        <translation>Toujours</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="230" />
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="231" />
        <source>If Newer</source>
        <translation>Si plus récent</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="232" />
        <source>If Older</source>
        <translation>Si plus ancien</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="242" />
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="248" />
        <source>⚠ No JobWidget connection available</source>
        <translation>⚠ Aucune connexion JobWidget disponible</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="253" />
        <source>Loading configuration data from JobWidget...</source>
        <translation>Chargement des données de configuration depuis JobWidget...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="291" />
        <source>✓ %1 jobs, %2 clients loaded</source>
        <translation>✓ %1 jobs, %2 clients chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="297" />
        <source>⚠ No jobs available - check connection</source>
        <translation>⚠ Aucun job disponible - vérifiez la connexion</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="383" />
        <source>✓ Configuration loaded</source>
        <translation>✓ Configuration chargée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="422" />
        <source>Loading job defaults...</source>
        <translation>Chargement des valeurs par défaut du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="450" />
        <source>Failed to load job defaults</source>
        <translation>Impossible de charger les valeurs par défaut du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="561" />
        <source>✓ Job defaults loaded</source>
        <translation>✓ Valeurs par défaut du job chargées</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="592" />
        <source>No job selected.</source>
        <translation>Aucun job sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="594" />
        <source>The value for '%1' contains a line break or control character.</source>
        <translation>La valeur de '%1' contient un saut de ligne ou un caractère de contrôle.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="596" />
        <source>'When' must be a valid time in the form YYYY-MM-DD HH:MM:SS.</source>
        <translation>« Quand » doit être une heure valide au format YYYY-MM-DD HH:MM:SS.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="598" />
        <source>The priority must be a positive number.</source>
        <translation>La priorité doit être un nombre positif.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="600" />
        <source>The command could not be built.</source>
        <translation>La commande n'a pas pu être construite.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="618" />
        <source>Nothing will be sent: %1</source>
        <translation>Rien ne sera envoyé : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="645" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="686" />
        <source>Missing Input</source>
        <translation>Saisie manquante</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="646" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="687" />
        <source>Please select a job.</source>
        <translation>Veuillez sélectionner un job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="654" />
        <location filename="../src/jobs/bnewjobdialog.cpp" line="694" />
        <source>Invalid Input</source>
        <translation>Saisie invalide</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="666" />
        <source>Do you want to run the following job?

%1</source>
        <translation>Voulez-vous exécuter le job suivant ?

%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="705" />
        <source>Calculating estimate...</source>
        <translation>Calcul de l'estimation...</translation>
    </message>
</context>
<context>
    <name>BPFXConverter</name>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="27" />
        <source>Convert to PFX Format</source>
        <translation>Convertir au format PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="35" />
        <source>Convert separate PEM/DER certificate and key files into a single PFX file for Windows.
PFX files are password-protected and contain both certificate and private key.</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un seul fichier PFX pour Windows.
Les fichiers PFX sont protégés par mot de passe et contiennent à la fois le certificat et la clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="44" />
        <source>Input Files</source>
        <translation>Fichiers d'entrée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="50" />
        <source>Select certificate file...</source>
        <translation>Sélectionner le fichier de certificat...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="51" />
        <location filename="../src/bpfxconverter.cpp" line="61" />
        <location filename="../src/bpfxconverter.cpp" line="71" />
        <location filename="../src/bpfxconverter.cpp" line="102" />
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="55" />
        <source>Certificate:</source>
        <translation>Certificat :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="60" />
        <source>Select private key file...</source>
        <translation>Sélectionner le fichier de clé privée...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="65" />
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="70" />
        <source>Optional: CA certificate</source>
        <translation>Facultatif : certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="75" />
        <source>CA Cert (Optional):</source>
        <translation>Certificat CA (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="80" />
        <source>PFX Password Protection</source>
        <translation>Protection du PFX par mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="85" />
        <source>Enter password for PFX file...</source>
        <translation>Saisir le mot de passe du fichier PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="86" />
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="90" />
        <source>Confirm password...</source>
        <translation>Confirmer le mot de passe...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="91" />
        <source>Confirm:</source>
        <translation>Confirmation :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="96" />
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="101" />
        <source>Select where to save PFX file...</source>
        <translation>Choisir où enregistrer le fichier PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="105" />
        <source>Save PFX File</source>
        <translation>Enregistrer le fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="107" />
        <source>PFX Files (*.pfx);;All Files (*)</source>
        <translation>Fichiers PFX (*.pfx);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="117" />
        <source>Save As:</source>
        <translation>Enregistrer sous :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="122" />
        <source>Conversion Log:</source>
        <translation>Journal de conversion :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="133" />
        <source>Convert to PFX</source>
        <translation>Convertir en PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="137" />
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="148" />
        <source>Select Certificate File</source>
        <translation>Sélectionner le fichier de certificat</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="150" />
        <source>Certificates (*.pem *.crt *.cert *.der);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert *.der);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="153" />
        <source>✓ Certificate selected: %1</source>
        <translation>✓ Certificat sélectionné : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="160" />
        <source>Select Private Key File</source>
        <translation>Sélectionner le fichier de clé privée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="162" />
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés privées (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="165" />
        <source>✓ Private key selected: %1</source>
        <translation>✓ Clé privée sélectionnée : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="172" />
        <source>Select CA Certificate</source>
        <translation>Sélectionner le certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="174" />
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="177" />
        <source>✓ CA certificate selected: %1</source>
        <translation>✓ Certificat CA sélectionné : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="185" />
        <location filename="../src/bpfxconverter.cpp" line="192" />
        <location filename="../src/bpfxconverter.cpp" line="199" />
        <source>Missing Input</source>
        <translation>Saisie manquante</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="186" />
        <source>Please select a certificate file.</source>
        <translation>Veuillez sélectionner un fichier de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="193" />
        <source>Please select a private key file.</source>
        <translation>Veuillez sélectionner un fichier de clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="200" />
        <source>Please enter a password to protect the PFX file.</source>
        <translation>Veuillez saisir un mot de passe pour protéger le fichier PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="206" />
        <source>Password Mismatch</source>
        <translation>Les mots de passe ne correspondent pas</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="207" />
        <source>The passwords do not match. Please try again.</source>
        <translation>Les mots de passe ne correspondent pas. Veuillez réessayer.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="213" />
        <source>Missing Output</source>
        <translation>Sortie manquante</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="214" />
        <source>Please specify where to save the PFX file.</source>
        <translation>Veuillez indiquer où enregistrer le fichier PFX.</translation>
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
        <translation>Démarrage de la conversion PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="234" />
        <source>✓ Conversion successful!</source>
        <translation>✓ Conversion réussie !</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="235" />
        <source>PFX file created: %1</source>
        <translation>Fichier PFX créé : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="240" />
        <source>Success</source>
        <translation>Succès</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="241" />
        <source>PFX file created successfully!

%1

You can now use this file in the certificate settings.</source>
        <translation>Fichier PFX créé avec succès !

%1

Vous pouvez maintenant utiliser ce fichier dans les paramètres de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="251" />
        <source>Loading certificate from: %1</source>
        <translation>Chargement du certificat depuis : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="256" />
        <source>✗ ERROR: Cannot open certificate file</source>
        <translation>✗ ERREUR : impossible d'ouvrir le fichier de certificat</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257" />
        <location filename="../src/bpfxconverter.cpp" line="271" />
        <location filename="../src/bpfxconverter.cpp" line="284" />
        <location filename="../src/bpfxconverter.cpp" line="301" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257" />
        <source>Cannot open certificate file.</source>
        <translation>Impossible d'ouvrir le fichier de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="270" />
        <source>✗ ERROR: Cannot parse certificate (not valid PEM or DER)</source>
        <translation>✗ ERREUR : impossible d'analyser le certificat (PEM ou DER non valide)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="272" />
        <source>Cannot parse certificate file. Make sure it's a valid PEM or DER certificate.</source>
        <translation>Impossible d'analyser le fichier de certificat. Assurez-vous qu'il s'agit d'un certificat PEM ou DER valide.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="276" />
        <source>✓ Certificate loaded: %1</source>
        <translation>✓ Certificat chargé : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="279" />
        <source>Loading private key from: %1</source>
        <translation>Chargement de la clé privée depuis : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="283" />
        <source>✗ ERROR: Cannot open private key file</source>
        <translation>✗ ERREUR : impossible d'ouvrir le fichier de clé privée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="284" />
        <source>Cannot open private key file.</source>
        <translation>Impossible d'ouvrir le fichier de clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="300" />
        <source>✗ ERROR: Cannot parse private key</source>
        <translation>✗ ERREUR : impossible d'analyser la clé privée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="302" />
        <source>Cannot parse private key file. Make sure it's a valid PEM or DER key.</source>
        <translation>Impossible d'analyser le fichier de clé privée. Assurez-vous qu'il s'agit d'une clé PEM ou DER valide.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="306" />
        <source>✓ Private key loaded</source>
        <translation>✓ Clé privée chargée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="309" />
        <source>Creating PFX (PKCS#12) file...</source>
        <translation>Création du fichier PFX (PKCS#12)...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="313" />
        <source>⚠ Note: Full PFX conversion requires OpenSSL PKCS#12 API</source>
        <translation>⚠ Remarque : la conversion PFX complète nécessite l'API OpenSSL PKCS#12</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="314" />
        <source>  The certificate and key files are valid and can be loaded</source>
        <translation>  Les fichiers de certificat et de clé sont valides et peuvent être chargés</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="321" />
        <source>✓ Files validated successfully</source>
        <translation>✓ Fichiers validés avec succès</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="322" />
        <source>  Certificate: %1</source>
        <translation>  Certificat : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="323" />
        <source>  Private Key: %1</source>
        <translation>  Clé privée : %1</translation>
    </message>
</context>
<context>
    <name>BPaginationWidget</name>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="26" />
        <source>Enable Pagination</source>
        <translation>Activer la pagination</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="32" />
        <source>Items per page:</source>
        <translation>Éléments par page :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="46" />
        <source>First Page</source>
        <translation>Première page</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="52" />
        <source>Previous Page</source>
        <translation>Page précédente</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="58" />
        <location filename="../src/jobs/bpaginationwidget.cpp" line="311" />
        <location filename="../src/jobs/bpaginationwidget.cpp" line="334" />
        <source>Page 0 of 0</source>
        <translation>Page 0 sur 0</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="64" />
        <source>Next Page</source>
        <translation>Page suivante</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="70" />
        <source>Last Page</source>
        <translation>Dernière page</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="78" />
        <source>Go to page:</source>
        <translation>Aller à la page :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="85" />
        <source>Go</source>
        <translation>Aller</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="345" />
        <source>Page %1 of %2</source>
        <translation>Page %1 sur %2</translation>
    </message>
</context>
<context>
    <name>BPermissionsDialog</name>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1985" />
        <source>My Permissions</source>
        <translation>Mes autorisations</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1998" />
        <source>Connection</source>
        <translation>Connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2000" />
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2001" />
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2008" />
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2010" />
        <source>TLS Certificate</source>
        <translation>Certificat TLS</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2012" />
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2014" />
        <source>Encryption:</source>
        <translation>Chiffrement :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2020" />
        <source>Backup &amp; Restore</source>
        <translation>Sauvegarde &amp; restauration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2021" />
        <source>Job Control</source>
        <translation>Contrôle des jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2022" />
        <source>Administration</source>
        <translation>Administration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2024" />
        <source>Media</source>
        <translation>Supports</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2035" />
        <source>Command Permissions</source>
        <translation>Autorisations des commandes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2042" />
        <source>Checking...</source>
        <translation>Vérification...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2051" />
        <source>Restore Details</source>
        <translation>Détails de la restauration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2053" />
        <location filename="../src/bmainwindow.cpp" line="2062" />
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2060" />
        <source>Other Available Commands</source>
        <translation>Autres commandes disponibles</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2069" />
        <source>Raw Director Response</source>
        <translation>Réponse brute du Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2087" />
        <source>Querying Director...</source>
        <translation>Interrogation du Director...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2176" />
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2179" />
        <source>Authorized</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2182" />
        <source>Not authorized</source>
        <translation>Non autorisé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2207" />
        <source>&lt;b&gt;Permission:&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;Autorisation :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2211" />
        <source>&lt;b&gt;where= (restore path):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;where= (chemin de restauration) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2215" />
        <source>&lt;b&gt;client= (target client):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;client= (client cible) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2219" />
        <source>&lt;b&gt;replace= (replace policy):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;replace= (politique de remplacement) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2225" />
        <source>&lt;br&gt;&lt;b&gt;Available parameters:&lt;/b&gt; %1</source>
        <translation>&lt;br&gt;&lt;b&gt;Paramètres disponibles :&lt;/b&gt; %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2246" />
        <source>Total: %1 commands (%2 authorized, %3 denied)</source>
        <translation>Total : %1 commandes (%2 autorisées, %3 refusées)</translation>
    </message>
</context>
<context>
    <name>BPoolResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>Paramètres avancés du profil - %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="40" />
        <source>General</source>
        <translation>Général</translation>
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
        <translation>Description facultative de cette connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="66" />
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="71" />
        <source> seconds</source>
        <translation> secondes</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="72" />
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="73" />
        <source>Keepalive interval (0 = disabled)</source>
        <translation>Intervalle de keepalive (0 = désactivé)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="74" />
        <source>Heartbeat Interval:</source>
        <translation>Intervalle de heartbeat :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="78" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="79" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="85" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d'Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="87" />
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="178" />
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="92" />
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Port gRPC du Director (par défaut : port BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="93" />
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="102" />
        <source>Forget certificate</source>
        <translation>Oublier le certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="103" />
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Supprimer le certificat épinglé. La prochaine connexion demandera de nouveau si le certificat du serveur est approuvé.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="106" />
        <source>Server certificate:</source>
        <translation>Certificat du serveur :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="118" />
        <source>e.g., operator or readonly</source>
        <translation>p. ex. operator ou readonly</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="119" />
        <source>Reference to a Profile resource on the Director.
The Profile defines ACLs for this console.</source>
        <translation>Référence à une ressource Profile sur le Director.
Le Profile définit les ACL de cette console.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="122" />
        <source>Director Profile:</source>
        <translation>Profile du Director :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="126" />
        <source>&lt;b&gt;Note:&lt;/b&gt; The Director Profile is a server-side resource that defines permissions. If set, the ACLs in this profile are ignored in favor of the server-side Profile.</source>
        <translation>&lt;b&gt;Remarque :&lt;/b&gt; le Profile du Director est une ressource côté serveur qui définit les autorisations. S'il est défini, les ACL de ce profil sont ignorées au profit du Profile côté serveur.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="146" />
        <source>TLS Options</source>
        <translation>Options TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="149" />
        <source>TLS Required</source>
        <translation>TLS requis</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="150" />
        <source>Require TLS for all connections (recommended)</source>
        <translation>Exiger TLS pour toutes les connexions (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="153" />
        <source>TLS Authenticate Only</source>
        <translation>TLS pour l'authentification uniquement</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="154" />
        <source>Use TLS only for authentication, not for data encryption</source>
        <translation>Utiliser TLS uniquement pour l'authentification, pas pour le chiffrement des données</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="160" />
        <source>Cipher Configuration</source>
        <translation>Configuration des chiffrements</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="164" />
        <source>TLSv1.2 ciphers (colon-separated)</source>
        <translation>Chiffrements TLSv1.2 (séparés par des deux-points)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="165" />
        <source>OpenSSL cipher list for TLSv1.2 connections
Example: HIGH:!aNULL:!MD5</source>
        <translation>Liste de chiffrements OpenSSL pour les connexions TLSv1.2
Exemple : HIGH:!aNULL:!MD5</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="167" />
        <source>Cipher List (TLSv1.2):</source>
        <translation>Liste de chiffrements (TLSv1.2) :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="170" />
        <source>TLSv1.3 cipher suites (colon-separated)</source>
        <translation>Suites de chiffrement TLSv1.3 (séparées par des deux-points)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="171" />
        <source>OpenSSL cipher suites for TLSv1.3 connections
Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</source>
        <translation>Suites de chiffrement OpenSSL pour les connexions TLSv1.3
Exemple : TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="173" />
        <source>Cipher Suites (TLSv1.3):</source>
        <translation>Suites de chiffrement (TLSv1.3) :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="176" />
        <source>e.g., -ALL,TLSv1.2,TLSv1.3</source>
        <translation>p. ex. -ALL,TLSv1.2,TLSv1.3</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="177" />
        <source>OpenSSL protocol string</source>
        <translation>Chaîne de protocole OpenSSL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="183" />
        <source>Certificate Files</source>
        <translation>Fichiers de certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="187" />
        <source>DH Parameters:</source>
        <translation>Paramètres DH :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="189" />
        <source>Path to Diffie-Hellman parameters file</source>
        <translation>Chemin du fichier de paramètres Diffie-Hellman</translation>
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
        <translation>Sélectionner le fichier de paramètres DH</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="195" />
        <source>PEM Files (*.pem);;All Files (*)</source>
        <translation>Fichiers PEM (*.pem);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="203" />
        <source>CRL File:</source>
        <translation>Fichier CRL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="205" />
        <source>Path to Certificate Revocation List file</source>
        <translation>Chemin du fichier de liste de révocation de certificats</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="210" />
        <source>Select CRL File</source>
        <translation>Sélectionner le fichier CRL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="211" />
        <source>CRL Files (*.crl *.pem);;All Files (*)</source>
        <translation>Fichiers CRL (*.crl *.pem);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="221" />
        <source>Allowed Certificate Common Names</source>
        <translation>Noms communs de certificat autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="225" />
        <source>One Common Name per line</source>
        <translation>Un nom commun par ligne</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="226" />
        <source>List of allowed certificate Common Names.
Leave empty to allow all valid certificates.</source>
        <translation>Liste des noms communs de certificat autorisés.
Laisser vide pour autoriser tous les certificats valides.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="249" />
        <source>&lt;b&gt;Access Control Lists (ACLs)&lt;/b&gt;&lt;br&gt;Define which resources this console can access. One entry per line. Use &lt;code&gt;*all*&lt;/code&gt; to allow all resources of that type. Regex patterns are supported for Job ACL.</source>
        <translation>&lt;b&gt;Listes de contrôle d'accès (ACL)&lt;/b&gt;&lt;br&gt;Définissez les ressources auxquelles cette console peut accéder. Une entrée par ligne. Utilisez &lt;code&gt;*all*&lt;/code&gt; pour autoriser toutes les ressources de ce type. Les expressions régulières sont prises en charge pour le Job ACL.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="271" />
        <source>Job ACL:</source>
        <translation>Job ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272" />
        <source>Allowed jobs</source>
        <translation>Jobs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272" />
        <source>List of allowed job names (regex supported)</source>
        <translation>Liste des noms de jobs autorisés (expressions régulières prises en charge)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="275" />
        <source>Client ACL:</source>
        <translation>Client ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276" />
        <source>Allowed clients</source>
        <translation>Clients autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276" />
        <source>List of allowed client names</source>
        <translation>Liste des noms de clients autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280" />
        <source>Storage ACL:</source>
        <translation>Storage ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281" />
        <source>Allowed storages</source>
        <translation>Storages autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281" />
        <source>List of allowed storage names</source>
        <translation>Liste des noms de Storages autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="284" />
        <source>Pool ACL:</source>
        <translation>Pool ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285" />
        <source>Allowed pools</source>
        <translation>Pools autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285" />
        <source>List of allowed pool names</source>
        <translation>Liste des noms de pools autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289" />
        <source>Schedule ACL:</source>
        <translation>Schedule ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290" />
        <source>Allowed schedules</source>
        <translation>Schedules autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290" />
        <source>List of allowed schedule names</source>
        <translation>Liste des noms de Schedules autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="293" />
        <source>FileSet ACL:</source>
        <translation>FileSet ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294" />
        <source>Allowed filesets</source>
        <translation>FileSets autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294" />
        <source>List of allowed fileset names</source>
        <translation>Liste des noms de FileSets autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298" />
        <source>Catalog ACL:</source>
        <translation>Catalog ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299" />
        <source>Allowed catalogs</source>
        <translation>Catalogs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299" />
        <source>List of allowed catalog names</source>
        <translation>Liste des noms de Catalogs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="302" />
        <source>Command ACL:</source>
        <translation>Command ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303" />
        <source>Allowed commands</source>
        <translation>Commandes autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303" />
        <source>List of allowed commands</source>
        <translation>Liste des commandes autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307" />
        <source>Where ACL:</source>
        <translation>Where ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308" />
        <source>Allowed restore locations</source>
        <translation>Emplacements de restauration autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308" />
        <source>List of allowed restore locations</source>
        <translation>Liste des emplacements de restauration autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="311" />
        <source>Plugin Options ACL:</source>
        <translation>Plugin Options ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312" />
        <source>Allowed plugin options</source>
        <translation>Options de plugin autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312" />
        <source>List of allowed plugin options</source>
        <translation>Liste des options de plugin autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="412" />
        <source>Not used with gRPC: the connection is always encrypted with TLS, the server certificate is confirmed on first use.</source>
        <translation>Non utilisé avec gRPC : la connexion est toujours chiffrée avec TLS, le certificat du serveur est confirmé à la première utilisation.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="427" />
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(non épinglé - confirmé à la prochaine connexion)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="433" />
        <source>SHA-256: %1</source>
        <translation>SHA-256 : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="445" />
        <source>Reset to Defaults</source>
        <translation>Rétablir les valeurs par défaut</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="446" />
        <source>Are you sure you want to reset all advanced settings to their default values?</source>
        <translation>Voulez-vous vraiment rétablir tous les paramètres avancés à leurs valeurs par défaut ?</translation>
    </message>
</context>
<context>
    <name>BRecycleForecast</name>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="62" />
        <source>now</source>
        <translation>maintenant</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="63" />
        <source>never</source>
        <translation>jamais</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="66" />
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="74" />
        <source>The Director did not report the status of this volume.</source>
        <translation>Le Director n'a pas indiqué le statut de ce volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="79" />
        <source>Archived: Bareos never prunes or recycles an archived volume.</source>
        <translation>Archived : Bareos n'élague ni ne recycle jamais un volume archivé.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="82" />
        <source>Disabled: Bareos neither writes to nor recycles a disabled volume.</source>
        <translation>Disabled : Bareos n'écrit pas sur un volume désactivé et ne le recycle pas.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="86" />
        <source>Error: Bareos never prunes a volume in Error, so it keeps its space until its status is changed. The job records on it still expire.</source>
        <translation>Error : Bareos n'élague jamais un volume en Error ; il conserve donc son espace jusqu'à ce que son statut soit modifié. Les enregistrements de jobs qu'il contient expirent malgré tout.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="90" />
        <source>Read-Only: Bareos does not prune or recycle a read-only volume.</source>
        <translation>Read-Only : Bareos n'élague ni ne recycle un volume en lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="93" />
        <source>Cleaning tape: never used for data.</source>
        <translation>Bande de nettoyage : jamais utilisée pour des données.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="98" />
        <source>Recycle = no: the volume is never reused; its data stays until it is relabeled or deleted.</source>
        <translation>Recycle = no : le volume n'est jamais réutilisé ; ses données restent jusqu'à ce qu'il soit réétiqueté ou supprimé.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="102" />
        <source>Purged: no job needs it any more, reusable now.</source>
        <translation>Purged : plus aucun job n'en a besoin, réutilisable dès maintenant.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="106" />
        <source>It still holds %1 on disk until it is recycled or truncated.</source>
        <translation>Il occupe encore %1 sur le disque jusqu'à ce qu'il soit recyclé ou tronqué.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="112" />
        <source>Marked for recycling: reused at the next need of its pool.</source>
        <translation>Marqué pour recyclage : réutilisé au prochain besoin de son pool.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="115" />
        <source>Busy: in use by a job right now.</source>
        <translation>Busy : utilisé par un job en ce moment.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="118" />
        <source>Unlabeled: not yet written.</source>
        <translation>Non étiqueté : pas encore écrit.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="124" />
        <source>Append: the short volume list does not report the Volume Use Duration, so it is not known whether this volume stays open until it is full.</source>
        <translation>Append : la liste courte des volumes n'indique pas le Volume Use Duration ; on ne sait donc pas si ce volume reste ouvert jusqu'à ce qu'il soit plein.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="128" />
        <source>Append without Volume Use Duration: it stays open until it is full</source>
        <translation>Append sans Volume Use Duration : il reste ouvert jusqu'à ce qu'il soit plein</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="130" />
        <source>(%1)</source>
        <translation>(%1)</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="133" />
        <source>or holds %1 jobs</source>
        <translation>ou contient %1 jobs</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="135" />
        <source>; nothing on it is freed before that.</source>
        <translation>; rien n'y est libéré avant cela.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="141" />
        <source>Append with a use duration of %1, but the first write or the retention is not reported.</source>
        <translation>Append avec une durée d'utilisation de %1, mais la première écriture ou la rétention n'est pas indiquée.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="150" />
        <source>Append until %1 (use duration %2), then %3 of volume retention.</source>
        <translation>Append jusqu'au %1 (durée d'utilisation %2), puis %3 de rétention du volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="158" />
        <source>%1: the last write or the volume retention is not reported.</source>
        <translation>%1 : la dernière écriture ou la rétention du volume n'est pas indiquée.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="163" />
        <source>%1: last written %2, volume retention %3.</source>
        <translation>%1 : dernière écriture %2, rétention du volume %3.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="174" />
        <source>%1: the job retention of %2 ends before the volume retention of %3; the jobs are pruned first and the volume becomes Purged.</source>
        <translation>%1 : la rétention des jobs de %2 se termine avant la rétention du volume de %3 ; les jobs sont élagués en premier et le volume passe à Purged.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="182" />
        <source>The date has passed: it is pruned when its pool next needs a volume.</source>
        <translation>La date est passée : il est élagué la prochaine fois que son pool a besoin d'un volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="187" />
        <source>Status "%1" is not known to this version.</source>
        <translation>Le statut « %1 » n'est pas connu de cette version.</translation>
    </message>
</context>
<context>
    <name>BResourceDialog</name>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="49" />
        <source>Select %1:</source>
        <translation>Sélectionner %1 :</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="67" />
        <source>Edit %1 Resource</source>
        <translation>Modifier la ressource %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="77" />
        <source>Show advanced directives</source>
        <translation>Afficher les directives avancées</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="86" />
        <location filename="../src/director/bresourcedialog.cpp" line="248" />
        <source>Save .conf</source>
        <translation>Enregistrer le .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="91" />
        <location filename="../src/director/bresourcedialog.cpp" line="325" />
        <source>Save ZIP</source>
        <translation>Enregistrer le ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="223" />
        <source>Save %1 Configuration</source>
        <translation>Enregistrer la configuration %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="224" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="230" />
        <location filename="../src/director/bresourcedialog.cpp" line="238" />
        <location filename="../src/director/bresourcedialog.cpp" line="273" />
        <location filename="../src/director/bresourcedialog.cpp" line="281" />
        <source>Save Failed</source>
        <translation>Échec de l'enregistrement</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="231" />
        <location filename="../src/director/bresourcedialog.cpp" line="274" />
        <source>%1 "%2" cannot be written. %3</source>
        <translation>%1 « %2 » ne peut pas être écrit. %3</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="239" />
        <source>Could not write to %1</source>
        <translation>Impossible d'écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="246" />
        <location filename="../src/director/bresourcedialog.cpp" line="323" />
        <source>Saved!</source>
        <translation>Enregistré !</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="263" />
        <source>Save %1 Configuration as ZIP</source>
        <translation>Enregistrer la configuration %1 en ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="264" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="282" />
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
</context>
<context>
    <name>BResourceForm</name>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="153" />
        <source>No schema available for resource type '%1'</source>
        <translation>Aucun schéma disponible pour le type de ressource '%1'</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="181" />
        <location filename="../src/config/bresourceform.cpp" line="193" />
        <location filename="../src/config/bresourceform.cpp" line="194" />
        <location filename="../src/config/bresourceform.cpp" line="197" />
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="378" />
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="386" />
        <source>Show/Hide password</source>
        <translation>Afficher/masquer le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="412" />
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414" />
        <source>Browse for file</source>
        <translation>Rechercher un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414" />
        <source>Browse for directory</source>
        <translation>Rechercher un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="418" />
        <source>Select File</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="420" />
        <source>Select Directory</source>
        <translation>Sélectionner un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="449" />
        <source>&lt;%1 name&gt;</source>
        <translation>&lt;nom de %1&gt;</translation>
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
        <translation>job</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="35" />
        <source>JobDefs resource</source>
        <translation>ressource JobDefs</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="36" />
        <source>client</source>
        <translation>client</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="37" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="38" />
        <source>pool</source>
        <translation>pool</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="39" />
        <source>storage</source>
        <translation>storage</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="42" />
        <source>resource</source>
        <translation>ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="49" />
        <source>%1 %2 was not created</source>
        <translation>Création de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="50" />
        <source>%1 %2 was not deleted</source>
        <translation>Suppression de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="53" />
        <source>%1 %2 was not changed</source>
        <translation>Modification de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="105" />
        <source>There is no connection to the configuration methods of the Director. Nothing was sent.</source>
        <translation>Aucune connexion aux méthodes de configuration du Director. Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="118" />
        <source>Nothing was written.</source>
        <translation>Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="139" />
        <source>Nothing to write</source>
        <translation>Rien à écrire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="140" />
        <source>The text is the one the Director has.</source>
        <translation>Le texte est celui que possède le Director.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="151" />
        <source>The text the change was made in does not match its SHA-256. Reload the %1 and make the change again.</source>
        <translation>Le texte sur lequel la modification a été faite ne correspond pas à son SHA-256. Rechargez la ressource (%1) et refaites la modification.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="286" />
        <source>%1 %2 created</source>
        <translation>%1 %2 : création effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="287" />
        <source>%1 %2 deleted</source>
        <translation>%1 %2 : suppression effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="288" />
        <source>%1 %2 changed</source>
        <translation>%1 %2 : modification effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="290" />
        <source>The Director wrote the configuration and reloaded it.</source>
        <translation>Le Director a écrit la configuration et l'a rechargée.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="291" />
        <source>The Director wrote the configuration but did not report a reload; the change takes effect with its next reload.</source>
        <translation>Le Director a écrit la configuration mais n'a pas signalé de rechargement ; la modification prendra effet au prochain rechargement.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="294" />
        <source>The reload does not adjust everything; the Director names these follow-up commands:</source>
        <translation>Le rechargement n'ajuste pas tout ; le Director indique ces commandes complémentaires :</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="299" />
        <source>%1 %2 was changed on the Director</source>
        <translation>%1 %2 a été modifié sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="300" />
        <source>The Director has another state of the %1 than the one this change started from. Nothing was written and nothing is overwritten. Compare the two texts, reload the %1 and make the change again.</source>
        <translation>Le Director possède un autre état de la ressource (%1) que celui dont cette modification est partie. Rien n'a été écrit et rien n'est écrasé. Comparez les deux textes, rechargez la ressource (%1) et refaites la modification.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="307" />
        <source>The Director checked the new configuration and rejected it. Nothing was written.</source>
        <translation>Le Director a vérifié la nouvelle configuration et l'a rejetée. Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="311" />
        <source>The %1 is still used. The Director does not delete a %1 that other resources name; change them first. Nothing was deleted.</source>
        <translation>La ressource (%1) est encore utilisée. Le Director ne supprime pas une ressource (%1) que d'autres ressources désignent ; modifiez-les d'abord. Rien n'a été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="317" />
        <source>The console of this session may not change this %1 (the ACL of its resource type). Nothing was written.</source>
        <translation>La console de cette session n'a pas le droit de modifier cette ressource (%1) (ACL de son type de ressource). Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="319" />
        <source>The console of this session may not change this %1 (its %2 ACL). Nothing was written.</source>
        <translation>La console de cette session n'a pas le droit de modifier cette ressource (%1) (son ACL %2). Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="324" />
        <source>The Director has no %1 of this name (any more). Nothing was written.</source>
        <translation>Le Director n'a pas (ou plus) de ressource (%1) portant ce nom. Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="328" />
        <source>The Director already has a resource of this name. Nothing was written.</source>
        <translation>Le Director possède déjà une ressource portant ce nom. Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="332" />
        <source>The Director wrote the file, but reloading the configuration failed, so it put the old file back. The %1 is as it was.</source>
        <translation>Le Director a écrit le fichier, mais le rechargement de la configuration a échoué ; il a donc remis l'ancien fichier en place. La ressource (%1) est inchangée.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="338" />
        <source>This connection has no configuration methods. Nothing was sent.</source>
        <translation>Cette connexion n'a pas de méthodes de configuration. Rien n'a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="344" />
        <source>The Director refused the call (%1). The console needs the command ACL "configure".</source>
        <translation>Le Director a refusé l'appel (%1). La console a besoin de l'ACL de commande "configure".</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="346" />
        <source>The call failed (%1). Nothing was written.</source>
        <translation>L'appel a échoué (%1). Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="350" />
        <source>The Director sent no status. Nothing is known to have been written.</source>
        <translation>Le Director n'a envoyé aucun statut. Rien ne permet de savoir si quelque chose a été écrit.</translation>
    </message>
</context>
<context>
    <name>BResourceReportDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="229" />
        <source>file %1</source>
        <translation>fichier %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="231" />
        <source>line %1</source>
        <translation>ligne %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="233" />
        <source>column %1</source>
        <translation>colonne %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="237" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="269" />
        <source>This was found when the Director checked the change; nothing was sent to be written.</source>
        <translation>Ceci a été détecté lorsque le Director a vérifié la modification ; rien n'a été envoyé pour écriture.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="275" />
        <source>The Director says: %1</source>
        <translation>Le Director indique : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="279" />
        <source>Where the configuration is invalid</source>
        <translation>Où la configuration n'est pas valide</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="288" />
        <location filename="../src/config/bresourcereviewdialog.cpp" line="291" />
        <source>Still referenced by</source>
        <translation>Encore référencé par</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="299" />
        <source>Follow-up commands the Director names</source>
        <translation>Commandes complémentaires indiquées par le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="306" />
        <source>They are not run by this application.</source>
        <translation>Elles ne sont pas exécutées par cette application.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="320" />
        <source>On the Director now</source>
        <translation>Actuellement sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="322" />
        <source>What was to be written (not written)</source>
        <translation>Ce qui devait être écrit (non écrit)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="328" />
        <source>&amp;Reload the %1</source>
        <translation>&amp;Recharger la ressource (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="330" />
        <source>Reads the %1 again from the Director; your changes are offered to be kept or discarded</source>
        <translation>Relit la ressource (%1) depuis le Director ; vous pourrez conserver ou abandonner vos modifications</translation>
    </message>
</context>
<context>
    <name>BResourceReviewDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="80" />
        <source>Change %1 %2</source>
        <translation>Modifier %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="81" />
        <source>Change %1 %2 on the Director?</source>
        <translation>Modifier %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="82" />
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Le Director a vérifié la nouvelle configuration et l'accepte. Rien n'est écrit tant que vous n'avez pas choisi "Écrire sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="86" />
        <source>Create %1 %2</source>
        <translation>Créer %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="87" />
        <source>Create %1 %2 on the Director?</source>
        <translation>Créer %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="88" />
        <source>The Director checked the new %1 and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Le Director a vérifié la nouvelle ressource (%1) et l'accepte. Rien n'est écrit tant que vous n'avez pas choisi "Écrire sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="92" />
        <source>Delete %1 %2</source>
        <translation>Supprimer %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="93" />
        <source>Delete %1 %2 on the Director?</source>
        <translation>Supprimer %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="94" />
        <source>The Director checked that nothing refers to the %1. Nothing is deleted until you choose "Delete on the Director".</source>
        <translation>Le Director a vérifié que rien ne fait référence à la ressource (%1). Rien n'est supprimé tant que vous n'avez pas choisi "Supprimer sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="107" />
        <location filename="../src/config/bresourcereviewdialog.cpp" line="110" />
        <source>Changed directives</source>
        <translation>Directives modifiées</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="121" />
        <source>Text before and after (- removed, + added)</source>
        <translation>Texte avant et après (- supprimé, + ajouté)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="123" />
        <source>Text of the new resource</source>
        <translation>Texte de la nouvelle ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="124" />
        <source>Text that is deleted</source>
        <translation>Texte supprimé</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="139" />
        <source>Affected</source>
        <translation>Concerné</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bresourcereviewdialog.cpp" line="143" />
        <source>The change applies to every one of these %n resource(s).</source>
        <translation>
            <numerusform>La modification s'applique à cette ressource (%n).</numerusform>
            <numerusform>La modification s'applique à chacune de ces %n ressources.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="152" />
        <source>(nothing else, as far as this application knows)</source>
        <translation>(rien d'autre, à la connaissance de cette application)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="156" />
        <source>The Director names as referencing it: %1</source>
        <translation>Le Director indique comme y faisant référence : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="166" />
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Fichier sur le Director : %1 (il contient aussi d'autres ressources ; seule cette ressource est remplacée).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="168" />
        <source>File on the Director: %1.</source>
        <translation>Fichier sur le Director : %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="170" />
        <source>The Director reloads its configuration after writing. Jobs that are running or already queued keep the configuration they started with.</source>
        <translation>Le Director recharge sa configuration après l'écriture. Les Jobs en cours ou déjà en file d'attente conservent la configuration avec laquelle ils ont démarré.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="175" />
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Supprimer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="176" />
        <source>&amp;Write to the Director</source>
        <translation>&amp;Écrire sur le Director</translation>
    </message>
</context>
<context>
    <name>BResourceText</name>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="223" />
        <source>The text does not start with a resource (line %1).</source>
        <translation>Le texte ne commence pas par une ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="225" />
        <source>Directives on the line of "%1 {" cannot be edited safely (line %2).</source>
        <translation>Les directives situées sur la ligne de "%1 {" ne peuvent pas être modifiées de manière sûre (ligne %2).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="235" />
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Texte après l'accolade fermante de la ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="246" />
        <source>The closing brace of the resource shares its line with a nested block (line %1).</source>
        <translation>L'accolade fermante de la ressource partage sa ligne avec un bloc imbriqué (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="260" />
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>L'accolade fermante partage sa ligne avec une directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="333" />
        <source>The text contains no resource.</source>
        <translation>Le texte ne contient aucune ressource.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="335" />
        <source>The resource has no closing brace.</source>
        <translation>La ressource n'a pas d'accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="385" />
        <source>A change without a directive name.</source>
        <translation>Une modification sans nom de directive.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="387" />
        <source>Directive %1 is changed twice.</source>
        <translation>La directive %1 est modifiée deux fois.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="391" />
        <source>The value of %1 contains a line break.</source>
        <translation>La valeur de %1 contient un saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="393" />
        <source>An empty value for %1; remove the directive instead.</source>
        <translation>Valeur vide pour %1 ; supprimez plutôt la directive.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="396" />
        <source>Directive %1 is on a line that cannot be edited safely (several directives on one line, a list or a nested block). Change it in the text.</source>
        <translation>La directive %1 se trouve sur une ligne qui ne peut pas être modifiée de manière sûre (plusieurs directives sur une ligne, une liste ou un bloc imbriqué). Modifiez-la dans le texte.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="400" />
        <source>Directive %1 appears more than once. Change it in the text.</source>
        <translation>La directive %1 apparaît plusieurs fois. Modifiez-la dans le texte.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="453" />
        <source>The new text could not be read back: %1</source>
        <translation>Le nouveau texte n'a pas pu être relu : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="458" />
        <source>The new text still has directive %1.</source>
        <translation>Le nouveau texte contient encore la directive %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="460" />
        <source>The new text does not have the intended value of %1.</source>
        <translation>Le nouveau texte n'a pas la valeur prévue pour %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="464" />
        <source>The new text would change more than the edited directives.</source>
        <translation>Le nouveau texte modifierait plus que les directives éditées.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="466" />
        <source>The new text would change the type of the resource.</source>
        <translation>Le nouveau texte modifierait le type de la ressource.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="468" />
        <source>The new text would change the name of the resource.</source>
        <translation>Le nouveau texte modifierait le nom de la ressource.</translation>
    </message>
</context>
<context>
    <name>BResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="46" />
        <source>Preview</source>
        <translation>Aperçu</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="48" />
        <source>Toggle config file preview</source>
        <translation>Afficher/masquer l'aperçu du fichier de configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="60" />
        <source>Edit...</source>
        <translation>Modifier...</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="66" />
        <source>Export .conf</source>
        <translation>Exporter .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="72" />
        <source>Export ZIP</source>
        <translation>Exporter ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85" />
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="136" />
        <source>%1: %2 resource(s)</source>
        <translation>%1 : %2 ressource(s)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="225" />
        <source>%1 %2 was not changed</source>
        <translation>Modification de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="243" />
        <source>Not written, because the form cannot write lists or nested blocks: %1. Change them in the configuration file.</source>
        <translation>Non écrit, car le formulaire ne peut pas écrire de listes ni de blocs imbriqués : %1. Modifiez-les dans le fichier de configuration.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="248" />
        <source>Nothing was changed.</source>
        <translation>Rien n'a été modifié.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="249" />
        <source>Nothing that can be written was changed. %1</source>
        <translation>Rien de ce qui peut être écrit n'a été modifié. %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="266" />
        <source>Another change is still being written. Try again when it is done.</source>
        <translation>Une autre modification est encore en cours d'écriture. Réessayez lorsqu'elle sera terminée.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="276" />
        <source>The text the Director sent does not match its SHA-256.</source>
        <translation>Le texte envoyé par le Director ne correspond pas à son SHA-256.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="278" />
        <source>%1 Nothing was written.</source>
        <translation>%1 Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="285" />
        <source>The change cannot be written into the text of the resource safely: %1 Nothing was written.</source>
        <translation>La modification ne peut pas être écrite de manière sûre dans le texte de la ressource : %1 Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="504" />
        <source>Schedule "%1" is not written here.</source>
        <translation>Le Schedule "%1" n'est pas écrit ici.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="604" />
        <source>Export %1 Configuration</source>
        <translation>Exporter la configuration %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="605" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="610" />
        <location filename="../src/director/bresourcewidget.cpp" line="665" />
        <source>Export Failed</source>
        <translation>Échec de l'exportation</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="611" />
        <source>Could not write to %1</source>
        <translation>Impossible d'écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="656" />
        <source>Export %1 Configuration as ZIP</source>
        <translation>Exporter la configuration %1 en ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="657" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="666" />
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
</context>
<context>
    <name>BRestoreBrowsePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="479" />
        <source>Select Files</source>
        <translation>Sélectionner les fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="480" />
        <source>Browse and select files/directories to restore.</source>
        <translation>Parcourez et sélectionnez les fichiers/répertoires à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="492" />
        <source>Loading files...</source>
        <translation>Chargement des fichiers...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="518" />
        <location filename="../src/jobs/brestorewizard.cpp" line="699" />
        <source>No files selected</source>
        <translation>Aucun fichier sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="531" />
        <source>Step %1 of %2 — Browse and select files/directories to restore.</source>
        <translation>Étape %1 sur %2 — Parcourez et sélectionnez les fichiers/répertoires à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="534" />
        <source>Browse files — Client: %1, FileSet: %2</source>
        <translation>Parcourir les fichiers — Client : %1, FileSet : %2</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="708" />
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n répertoire</numerusform>
            <numerusform>%n répertoires</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="710" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="711" />
        <source>Selected: %1</source>
        <translation>Sélection : %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="713" />
        <source>%n item(s) selected</source>
        <translation>
            <numerusform>%n élément sélectionné</numerusform>
            <numerusform>%n éléments sélectionnés</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BRestoreExecutePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1170" />
        <source>Execute Restore</source>
        <translation>Exécuter la restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1171" />
        <source>Sending commands to the Director...</source>
        <translation>Envoi des commandes au Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1175" />
        <source>Preparing...</source>
        <translation>Préparation...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1205" />
        <source>Step %1 of %2 — Sending commands to the Director...</source>
        <translation>Étape %1 sur %2 — Envoi des commandes au Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1214" />
        <source>Step %v of %m</source>
        <translation>Étape %v sur %m</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1240" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1259" />
        <source>No director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1247" />
        <source>Creating restore table...</source>
        <translation>Création de la table de restauration...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1248" />
        <source>&gt;&gt;&gt; .bvfs_restore %1</source>
        <translation>&gt;&gt;&gt; .bvfs_restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1266" />
        <source>Executing restore command...</source>
        <translation>Exécution de la commande de restauration...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1267" />
        <source>&gt;&gt;&gt; restore %1</source>
        <translation>&gt;&gt;&gt; restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1281" />
        <source>Cleaning up restore table...</source>
        <translation>Nettoyage de la table de restauration...</translation>
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
        <translation>Échec de .bvfs_restore : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1304" />
        <source>Restore table created successfully.</source>
        <translation>Table de restauration créée avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1320" />
        <source>Restore command failed: %1</source>
        <translation>Échec de la commande de restauration : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1324" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1378" />
        <source>Hint: The Bareos Director restricts the "where" path via WhereACL. Ask your administrator to add the path "%1" to the WhereACL of your console configuration, or use a permitted restore path.</source>
        <translation>Conseil : le Director Bareos restreint le chemin "where" via WhereACL. Demandez à votre administrateur d'ajouter le chemin "%1" à la WhereACL de votre configuration de console, ou utilisez un chemin de restauration autorisé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1347" />
        <source>Restore job submitted successfully (Job ID: %1).</source>
        <translation>Job de restauration soumis avec succès (ID du Job : %1).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1349" />
        <source>Restore command accepted.</source>
        <translation>Commande de restauration acceptée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1373" />
        <source>Restore command failed.</source>
        <translation>Échec de la commande de restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1392" />
        <source>Restore job submitted successfully.</source>
        <translation>Job de restauration soumis avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1402" />
        <source>WARNING: Unexpected restore response.</source>
        <translation>AVERTISSEMENT : réponse de restauration inattendue.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1420" />
        <source>Restore table created (text response).</source>
        <translation>Table de restauration créée (réponse texte).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1435" />
        <source>Cleanup complete.</source>
        <translation>Nettoyage terminé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1445" />
        <source>Complete</source>
        <translation>Terminé</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1448" />
        <source>Restore job %1 submitted successfully!</source>
        <translation>Job de restauration %1 soumis avec succès !</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1449" />
        <source>The restore job (ID: %1) is now running on the Director.
You can monitor its progress in the Jobs view.
Refresh the job list to see the restore job status.</source>
        <translation>Le Job de restauration (ID : %1) est maintenant en cours sur le Director.
Vous pouvez suivre sa progression dans la vue Jobs.
Actualisez la liste des Jobs pour voir le statut du Job de restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1454" />
        <source>Restore completed.</source>
        <translation>Restauration terminée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1455" />
        <source>The restore commands have been executed.
Check the log above for details.</source>
        <translation>Les commandes de restauration ont été exécutées.
Consultez le journal ci-dessus pour plus de détails.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1467" />
        <source>ERROR: %1</source>
        <translation>ERREUR : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1470" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1471" />
        <source>Restore failed</source>
        <translation>Échec de la restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1499" />
        <source>command</source>
        <translation>commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1500" />
        <source>Timeout waiting for %1 response (no response within %2 seconds)</source>
        <translation>Délai dépassé en attente de la réponse %1 (aucune réponse en %2 secondes)</translation>
    </message>
</context>
<context>
    <name>BRestoreOptionsPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="726" />
        <source>Restore Options</source>
        <translation>Options de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="727" />
        <source>Configure where and how to restore the selected files.</source>
        <translation>Configurez où et comment restaurer les fichiers sélectionnés.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="733" />
        <source>Source:</source>
        <translation>Source :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="736" />
        <source>Target Client:</source>
        <translation>Client cible :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="739" />
        <source>Restore Where:</source>
        <translation>Restaurer dans :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="742" />
        <source>Always</source>
        <translation>Toujours</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="743" />
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="744" />
        <source>If Newer</source>
        <translation>Si plus récent</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="745" />
        <source>If Older</source>
        <translation>Si plus ancien</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="746" />
        <source>Replace Policy:</source>
        <translation>Politique de remplacement :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="770" />
        <source>Step %1 of %2 — Configure where and how to restore the selected files.</source>
        <translation>Étape %1 sur %2 — Configurez où et comment restaurer les fichiers sélectionnés.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="775" />
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n répertoire</numerusform>
            <numerusform>%n répertoires</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="777" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="779" />
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="785" />
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="786" />
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="788" />
        <source>Job %1: %2 — %3 selected (estimated size: %4)</source>
        <translation>Job %1 : %2 — %3 sélectionné(s) (taille estimée : %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="822" />
        <source>Always overwrite existing files on the target, regardless of timestamps.</source>
        <translation>Toujours écraser les fichiers existants sur la cible, quels que soient les horodatages.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="826" />
        <source>Never overwrite. Skip files that already exist on the target.</source>
        <translation>Ne jamais écraser. Ignorer les fichiers qui existent déjà sur la cible.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="830" />
        <source>Only overwrite if the backed-up file is newer than the existing file.</source>
        <translation>N'écraser que si le fichier sauvegardé est plus récent que le fichier existant.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="834" />
        <source>Only overwrite if the backed-up file is older than the existing file.</source>
        <translation>N'écraser que si le fichier sauvegardé est plus ancien que le fichier existant.</translation>
    </message>
</context>
<context>
    <name>BRestorePreviewPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="872" />
        <source>Command Preview</source>
        <translation>Aperçu des commandes</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="873" />
        <source>Review the commands that will be sent to the Director.</source>
        <translation>Vérifiez les commandes qui seront envoyées au Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="883" />
        <source>Permission Checks</source>
        <translation>Vérification des autorisations</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="890" />
        <source>Restore command:</source>
        <translation>Commande de restauration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="891" />
        <source>Target client:</source>
        <translation>Client cible :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="892" />
        <source>Where path:</source>
        <translation>Chemin de destination :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="914" />
        <source>Click "Commit" to execute these commands on the Director.</source>
        <translation>Cliquez sur "Valider" pour exécuter ces commandes sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="927" />
        <source>Step %1 of %2 — Review the commands that will be sent to the Director.</source>
        <translation>Étape %1 sur %2 — Vérifiez les commandes qui seront envoyées au Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="970" />
        <location filename="../src/jobs/brestorewizard.cpp" line="973" />
        <source>%n directory(ies) (including all contents)</source>
        <translation>
            <numerusform>%n répertoire (avec tout son contenu)</numerusform>
            <numerusform>%n répertoires (avec tout leur contenu)</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="971" />
        <source>%n individual file(s)</source>
        <translation>
            <numerusform>%n fichier individuel</numerusform>
            <numerusform>%n fichiers individuels</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="975" />
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="977" />
        <source>No files or directories selected</source>
        <translation>Aucun fichier ni répertoire sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="984" />
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="985" />
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="987" />
        <source>Restore summary:
• Selection: %1
• Estimated size: %2
• Target client: %3
• Restore to: %4
• Replace policy: %5</source>
        <translation>Résumé de la restauration :
• Sélection : %1
• Taille estimée : %2
• Client cible : %3
• Restaurer dans : %4
• Politique de remplacement : %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1014" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1015" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1016" />
        <source>Checking...</source>
        <translation>Vérification...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1017" />
        <source>Querying Director for permissions...</source>
        <translation>Interrogation du Director sur les autorisations...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1029" />
        <source>No Director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1087" />
        <source>restore command lacks client= parameter</source>
        <translation>la commande restore n'a pas de paramètre client=</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1092" />
        <source>Available (subject to WhereACL)</source>
        <translation>Disponible (sous réserve de WhereACL)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1095" />
        <source>Not available</source>
        <translation>Non disponible</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1098" />
        <source>N/A (restore not authorized)</source>
        <translation>N/A (restauration non autorisée)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1102" />
        <source>Permission check completed.</source>
        <translation>Vérification des autorisations terminée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1119" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1121" />
        <location filename="../src/jobs/brestorewizard.cpp" line="1123" />
        <source>Unknown (timeout)</source>
        <translation>Inconnu (délai dépassé)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1125" />
        <source>Permission check timed out — you can still proceed.</source>
        <translation>La vérification des autorisations a dépassé le délai — vous pouvez tout de même continuer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1153" />
        <source>Authorized</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1156" />
        <source>Not authorized</source>
        <translation>Non autorisé</translation>
    </message>
</context>
<context>
    <name>BRestoreSelectJobPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="146" />
        <source>Restore Conditions</source>
        <translation>Conditions de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="147" />
        <source>Configure the restore scope before browsing files.</source>
        <translation>Configurez l'étendue de la restauration avant de parcourir les fichiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="154" />
        <source>Source Job:</source>
        <translation>Job source :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="158" />
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="162" />
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="166" />
        <source>All related backups (full restore chain)</source>
        <translation>Toutes les sauvegardes associées (chaîne de restauration complète)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="175" />
        <source>Restore Scope:</source>
        <translation>Étendue de la restauration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="181" />
        <source>Before Date:</source>
        <translation>Avant la date :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="208" />
        <source>Step %1 of %2 — Configure the restore scope before browsing files.</source>
        <translation>Étape %1 sur %2 — Configurez l'étendue de la restauration avant de parcourir les fichiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="211" />
        <location filename="../src/jobs/brestorewizard.cpp" line="416" />
        <source>Job %1: %2 (Client: %3, FileSet: %4)</source>
        <translation>Job %1 : %2 (Client : %3, FileSet : %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="217" />
        <source>Only this specific job (ID: %1)</source>
        <translation>Uniquement ce Job précis (ID : %1)</translation>
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
        <translation>Avertissement : impossible de charger %1 depuis le Director dans le délai imparti. Le Director est peut-être occupé ou la connexion lente.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="272" />
        <source> and </source>
        <translation> et </translation>
    </message>
</context>
<context>
    <name>BRestoreWizard</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="114" />
        <source>Restore Wizard — Job %1: %2</source>
        <translation>Assistant de restauration — Job %1 : %2</translation>
    </message>
</context>
<context>
    <name>BRunOverridesEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="39" />
        <source>not set</source>
        <translation>non défini</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="40" />
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="41" />
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="83" />
        <source>(job's setting)</source>
        <translation>(réglage du Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="87" />
        <source>The Director has not reported any %1 yet; refresh the view after connecting. Not set: the setting of the job applies.</source>
        <translation>Le Director n'a encore signalé aucun(e) %1 ; actualisez la vue après la connexion. Non défini : le réglage du Job s'applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="89" />
        <source>The %1 the Director reported. Not set: the setting of the job applies.</source>
        <translation>Les %1 signalés par le Director. Non défini : le réglage du Job s'applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="109" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="110" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="111" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="112" />
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="113" />
        <source>pools</source>
        <translation>pools</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="114" />
        <source>storages</source>
        <translation>storages</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="117" />
        <source>set to</source>
        <translation>définir à</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="123" />
        <source>Level of the job</source>
        <translation>Niveau du Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="132" />
        <source>The level the Run line names. "Level of the job": the line names none and the level of the job applies.</source>
        <translation>Le niveau indiqué par la ligne Run. "Niveau du Job" : la ligne n'en indique aucun et le niveau du Job s'applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="138" />
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="139" />
        <source>Override the priority</source>
        <translation>Remplacer la priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="140" />
        <source>a time period, e.g. 8 hours</source>
        <translation>une durée, p. ex. 8 hours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="141" />
        <source>Empty: the setting of the job applies</source>
        <translation>Vide : le réglage du Job s'applique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="167" />
        <source>&amp;Level:</source>
        <translation>&amp;Niveau :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="168" />
        <source>&amp;Storage:</source>
        <translation>&amp;Storage :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="170" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="171" />
        <source>&amp;Messages:</source>
        <translation>&amp;Messages :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="173" />
        <source>&amp;Full pool:</source>
        <translation>Pool &amp;Full :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="174" />
        <source>P&amp;riority:</source>
        <translation>P&amp;riorité :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="176" />
        <source>&amp;Incremental pool:</source>
        <translation>Pool &amp;Incremental :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="177" />
        <source>Sp&amp;ool data:</source>
        <translation>Sp&amp;ool des données :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="179" />
        <source>&amp;Differential pool:</source>
        <translation>Pool &amp;Differential :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="180" />
        <source>&amp;Accurate:</source>
        <translation>&amp;Accurate :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="182" />
        <source>&amp;Next pool:</source>
        <translation>Pool suiva&amp;nt :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="183" />
        <source>Max run sched &amp;time:</source>
        <translation>Durée max. depuis la &amp;planification :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="271" />
        <source>%1 (not reported)</source>
        <translation>%1 (non signalé)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="273" />
        <source>%1 is named by the Run line, but the Director did not report it. It is kept as it is.</source>
        <translation>%1 est indiqué par la ligne Run, mais le Director ne l'a pas signalé. Il est conservé tel quel.</translation>
    </message>
</context>
<context>
    <name>BRunParser</name>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="113" />
        <location filename="../src/schedules/model/brunparser.cpp" line="145" />
        <source>A Run line cannot contain a line break.</source>
        <translation>Une ligne Run ne peut pas contenir de saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="134" />
        <source>Unexpected character "%1".</source>
        <translation>Caractère inattendu "%1".</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="163" />
        <source>A quoted value is not closed.</source>
        <translation>Une valeur entre guillemets n'est pas fermée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="393" />
        <source>The Run line is empty.</source>
        <translation>La ligne Run est vide.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="408" />
        <source>Expected an equals, got: %1</source>
        <translation>Signe égal attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="412" />
        <location filename="../src/schedules/model/brunparser.cpp" line="457" />
        <source>Expected a value after "%1=".</source>
        <translation>Valeur attendue après "%1=".</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="423" />
        <source>Expect a YES or NO, got: %1</source>
        <translation>YES ou NO attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="433" />
        <source>Job level field: %1 not found in run record</source>
        <translation>Champ de niveau du Job : %1 introuvable dans l'enregistrement Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="440" />
        <source>expected a positive integer, got: %1</source>
        <translation>entier positif attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="446" />
        <source>expected a time period, got: %1</source>
        <translation>durée attendue, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="497" />
        <source>Unexpected token: %1</source>
        <translation>Jeton inattendu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="502" />
        <source>Day number out of range (1-31)</source>
        <translation>Numéro de jour hors limites (1-31)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="509" />
        <source>Unexpected token: "%1"</source>
        <translation>Jeton inattendu : "%1"</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="519" />
        <location filename="../src/schedules/model/brunparser.cpp" line="653" />
        <source>Week number out of range (0-53)</source>
        <translation>Numéro de semaine hors limites (0-53)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="525" />
        <source>Job type field: %1 in run record not found</source>
        <translation>Champ de type du Job : %1 introuvable dans l'enregistrement Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="555" />
        <source>Time must be preceded by keyword AT.</source>
        <translation>L'heure doit être précédée du mot-clé AT.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="571" />
        <location filename="../src/schedules/model/brunparser.cpp" line="576" />
        <location filename="../src/schedules/model/brunparser.cpp" line="585" />
        <source>Bad time specification.</source>
        <translation>Spécification d'heure incorrecte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="610" />
        <source>Bad day specification in modulo.</source>
        <translation>Spécification de jour incorrecte dans le modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="612" />
        <source>Bad day specification, offset must always be &lt;= than modulo.</source>
        <translation>Spécification de jour incorrecte, le décalage doit toujours être &lt;= au modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="622" />
        <source>Week number out of range (0-53) in modulo</source>
        <translation>Numéro de semaine hors limites (0-53) dans le modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="624" />
        <source>Week interval out of range (1-53) in modulo</source>
        <translation>Intervalle de semaines hors limites (1-53) dans le modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="626" />
        <source>Bad week number specification in modulo, offset must always be &lt;= than modulo.</source>
        <translation>Spécification de numéro de semaine incorrecte dans le modulo, le décalage doit toujours être &lt;= au modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="633" />
        <source>Bad modulo time specification. Format for weekdays is '01/02', for yearweeks is 'w01/w02'.</source>
        <translation>Spécification de modulo incorrecte. Le format pour les jours de semaine est '01/02', pour les semaines de l'année 'w01/w02'.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="646" />
        <source>Bad day range specification.</source>
        <translation>Spécification de plage de jours incorrecte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="661" />
        <source>Invalid month, week or position day range</source>
        <translation>Plage de mois, de semaines ou de jours de position non valide</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="665" />
        <source>Invalid month, weekday or position range</source>
        <translation>Plage de mois, de jours de semaine ou de positions non valide</translation>
    </message>
</context>
<context>
    <name>BRunRuleDialog</name>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="94" />
        <source>Only on these da&amp;ys of the month</source>
        <translation>Uniquement ces &amp;jours du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="95" />
        <source>Only in these &amp;weeks of the year</source>
        <translation>Uniquement ces &amp;semaines de l'année</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="96" />
        <source>last</source>
        <translation>dernière</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="97" />
        <source>&amp;Every hour</source>
        <translation>&amp;Toutes les heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="104" />
        <source>Calendar rule</source>
        <translation>Règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="107" />
        <source>A calendar rule runs by day of the month or by week of the year. It does not belong to a weekday and is not painted in the grid.</source>
        <translation>Une règle de calendrier s'exécute selon le jour du mois ou la semaine de l'année. Elle n'appartient à aucun jour de semaine et n'est pas peinte dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="114" />
        <source>What runs</source>
        <translation>Ce qui s'exécute</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="122" />
        <source>Not checked: every day of the month</source>
        <translation>Non coché : chaque jour du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="127" />
        <source>Day %1</source>
        <translation>Jour %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="132" />
        <source>Not checked: every week of the year. The weeks are numbered w00 to w53 as in a Run line.</source>
        <translation>Non coché : chaque semaine de l'année. Les semaines sont numérotées de w00 à w53 comme dans une ligne Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="138" />
        <source>Week %1</source>
        <translation>Semaine %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="144" />
        <source>And only</source>
        <translation>Et uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="146" />
        <source>on these weekdays:</source>
        <translation>ces jours de semaine :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="162" />
        <source>in these weeks of the month:</source>
        <translation>ces semaines du mois :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="177" />
        <source>The last seven days of a month</source>
        <translation>Les sept derniers jours d'un mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="183" />
        <source>in these months:</source>
        <translation>ces mois :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="200" />
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="205" />
        <source>%1 hours</source>
        <translation>%1 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="208" />
        <source>The run starts in every hour of the day ("hourly")</source>
        <translation>L'exécution démarre à chaque heure de la journée ("hourly")</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="210" />
        <source>&amp;Minute of the hour:</source>
        <translation>&amp;Minute de l'heure :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="213" />
        <source>Minute of the hour</source>
        <translation>Minute de l'heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="226" />
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="291" />
        <source>Odd</source>
        <translation>Impaires</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="292" />
        <source>Even</source>
        <translation>Paires</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="293" />
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="387" />
        <source>Choose days of the month or weeks of the year. A run without them belongs to the grid and is painted there.</source>
        <translation>Choisissez des jours du mois ou des semaines de l'année. Une exécution sans eux appartient à la grille et y est peinte.</translation>
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
        <translation>Modifier le RunScript</translation>
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
        <translation>Sélectionner un script</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="65" />
        <source>Command:</source>
        <translation>Commande :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="70" />
        <source>Runs When:</source>
        <translation>Exécuter quand :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="75" />
        <source>Runs On Client</source>
        <translation>Exécuter sur le client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="79" />
        <source>Runs On Failure</source>
        <translation>Exécuter en cas d'échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="82" />
        <source>Abort Job On Error</source>
        <translation>Abandonner le Job en cas d'erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="86" />
        <source>Fail Job On Error</source>
        <translation>Faire échouer le Job en cas d'erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97" />
        <source>Validation</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97" />
        <source>Command is required.</source>
        <translation>La commande est obligatoire.</translation>
    </message>
</context>
<context>
    <name>BRunScriptEditor</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>Runs When</source>
        <translation>Exécuter quand</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140" />
        <source>On Client</source>
        <translation>Sur le client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="154" />
        <source>+ Add Script</source>
        <translation>+ Ajouter un script</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="155" />
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="156" />
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231" />
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231" />
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BRunWriter</name>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="177" />
        <source>The run has an empty hour, day, month or week set and cannot be written as a Run line.</source>
        <translation>L'exécution a un ensemble d'heures, de jours, de mois ou de semaines vide et ne peut pas être écrite comme ligne Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="182" />
        <source>The minute %1 is out of range.</source>
        <translation>La minute %1 est hors limites.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="246" />
        <source>Run line "%1" is not understood (%2) and is never rewritten.</source>
        <translation>La ligne Run "%1" n'est pas comprise (%2) et n'est jamais réécrite.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="265" />
        <source>The generated Run line "%1" does not read back: %2</source>
        <translation>La ligne Run générée "%1" ne se relit pas : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="270" />
        <source>The generated Run line "%1" does not say what was intended.</source>
        <translation>La ligne Run générée "%1" ne dit pas ce qui était prévu.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="286" />
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
</context>
<context>
    <name>BScheduleBasicsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="516" />
        <source>Schedule Basics</source>
        <translation>Paramètres de base du Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="517" />
        <source>Configure the schedule name and select an optional template.</source>
        <translation>Configurez le nom du Schedule et choisissez un modèle facultatif.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="522" />
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="526" />
        <source>e.g., WeeklyCycle</source>
        <translation>p. ex. WeeklyCycle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="527" />
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="530" />
        <source>e.g., Full Sunday, incremental daily</source>
        <translation>p. ex. Full le dimanche, Incremental chaque jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="531" />
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="533" />
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="540" />
        <source>Template</source>
        <translation>Modèle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="544" />
        <source>(Empty — build from scratch)</source>
        <translation>(Vide — partir de zéro)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="550" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="596" />
        <source>Select a template to pre-fill the schedule with common run directives.</source>
        <translation>Choisissez un modèle pour pré-remplir le Schedule avec des directives Run courantes.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="609" />
        <source>&lt;b&gt;%1&lt;/b&gt;: %2&lt;br&gt;%3 run directive(s)</source>
        <translation>&lt;b&gt;%1&lt;/b&gt; : %2&lt;br&gt;%3 directive(s) Run</translation>
    </message>
</context>
<context>
    <name>BScheduleBrushDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="293" />
        <source>New brush</source>
        <translation>Nouveau pinceau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="296" />
        <source>A brush is what painting puts into a cell: a level, the minute of the hour and, if wanted, settings that replace those of the job.</source>
        <translation>Un pinceau est ce que la peinture place dans une cellule : un niveau, la minute de l'heure et, si souhaité, des réglages qui remplacent ceux du Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="305" />
        <source>Minute of the hour</source>
        <translation>Minute de l'heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="306" />
        <source>The run starts at this minute of every painted hour</source>
        <translation>L'exécution démarre à cette minute de chaque heure peinte</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="307" />
        <source>Minute of the &amp;hour:</source>
        <translation>Minute de l'&amp;heure :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="310" />
        <source>Level and overrides</source>
        <translation>Niveau et remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="366" />
        <source>Brush: %1</source>
        <translation>Pinceau : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleEntry</name>
    <message>
        <location filename="../src/models/bresourcemodels.cpp" line="679" />
        <source>The entry does not describe a run.</source>
        <translation>L'entrée ne décrit pas une exécution.</translation>
    </message>
</context>
<context>
    <name>BScheduleGanttWidget</name>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="405" />
        <source>(no client)</source>
        <translation>(aucun client)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="408" />
        <source>(no storage)</source>
        <translation>(aucun storage)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="435" />
        <source>Not used by any job</source>
        <translation>Utilisé par aucun Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="551" />
        <source>No runs in this week</source>
        <translation>Aucune exécution cette semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="552" />
        <source>No runs on this date</source>
        <translation>Aucune exécution à cette date</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Monday</source>
        <translation>Lundi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Tuesday</source>
        <translation>Mardi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Wednesday</source>
        <translation>Mercredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582" />
        <source>Thursday</source>
        <translation>Jeudi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Friday</source>
        <translation>Vendredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Saturday</source>
        <translation>Samedi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583" />
        <source>Sunday</source>
        <translation>Dimanche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586" />
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587" />
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1076" />
        <source>%1, if it turns out against it</source>
        <translation>%1, si cela tombe mal</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1146" />
        <source>Utilization</source>
        <translation>Occupation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1373" />
        <source>Not used by any enabled job: nothing starts at this time.</source>
        <translation>Utilisé par aucun Job activé : rien ne démarre à cette heure.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1374" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1391" />
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1377" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1389" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1379" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1398" />
        <source>Run</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1390" />
        <source>(level of the job)</source>
        <translation>(niveau du Job)</translation>
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
        <translation>Attente prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1411" />
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1414" />
        <source>Predicted wait: none</source>
        <translation>Attente prévue : aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1425" />
        <source>the jobs of the Director are not loaded: it is not known which job runs this schedule</source>
        <translation>les Jobs du Director ne sont pas chargés : on ne sait pas quel Job utilise ce Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1430" />
        <source>no successful run of this job in the last %1 days</source>
        <translation>aucune exécution réussie de ce Job au cours des %1 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1434" />
        <source>No history</source>
        <translation>Aucun historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1435" />
        <source>Duration unknown: the outline marks the start only.</source>
        <translation>Durée inconnue : le contour ne marque que le début.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1438" />
        <source>Median duration</source>
        <translation>Durée médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1440" />
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1447" />
        <source>90%</source>
        <translation>90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1442" />
        <source>runs</source>
        <translation>exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1445" />
        <source>Median wait</source>
        <translation>Attente médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450" />
        <source>Wait</source>
        <translation>Attente</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450" />
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1454" />
        <source>Estimate: no run at this level in the last %1 days, figures from all levels of the job.</source>
        <translation>Estimation : aucune exécution à ce niveau au cours des %1 derniers jours, chiffres issus de tous les niveaux du Job.</translation>
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
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1474" />
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1478" />
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1508" />
        <source>job(s) running</source>
        <translation>Job(s) en cours</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1512" />
        <source>%n run(s) predicted to wait</source>
        <translation>
            <numerusform>%n exécution devrait attendre</numerusform>
            <numerusform>%n exécutions devraient attendre</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1517" />
        <source>%1 run(s) without history start here (not counted: duration unknown)</source>
        <translation>%1 exécution(s) sans historique démarrent ici (non comptées : durée inconnue)</translation>
    </message>
</context>
<context>
    <name>BScheduleGridEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="29" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="76" />
        <source>Undone: %1</source>
        <translation>Annulé : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="40" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="85" />
        <source>Redone: %1</source>
        <translation>Rétabli : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="203" />
        <source>Eraser</source>
        <translation>Gomme</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="256" />
        <source>This schedule contains a Run line that is not understood. It is locked: nothing in it can be changed here.</source>
        <translation>Ce Schedule contient une ligne Run qui n'est pas comprise. Il est verrouillé : rien ne peut y être modifié ici.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="283" />
        <source>%n cell(s)</source>
        <translation>
            <numerusform>%n cellule</numerusform>
            <numerusform>%n cellules</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="285" />
        <source>%1 of the scope %2</source>
        <translation>%1 de la portée %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="297" />
        <source>Paint %1 on %2</source>
        <translation>Peindre %1 sur %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="306" />
        <source>Add %1 to %2</source>
        <translation>Ajouter %1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="314" />
        <source>Erase %1</source>
        <translation>Effacer %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="322" />
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="348" />
        <source>No cell is selected.</source>
        <translation>Aucune cellule n'est sélectionnée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="330" />
        <source>No brush is chosen.</source>
        <translation>Aucun pinceau n'est choisi.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="374" />
        <source>A scope needs at least one month, and a week of the month or "last".</source>
        <translation>Une portée nécessite au moins un mois, et une semaine du mois ou "dernière".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="384" />
        <source>Add the scope %1</source>
        <translation>Ajouter la portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="399" />
        <source>The scope "%1" is always there.</source>
        <translation>La portée "%1" est toujours présente.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="404" />
        <source>The scope %1 still has Run lines. Erase its cells first.</source>
        <translation>La portée %1 contient encore des lignes Run. Effacez d'abord ses cellules.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="410" />
        <source>Remove the scope %1</source>
        <translation>Supprimer la portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="420" />
        <source>Add a calendar rule</source>
        <translation>Ajouter une règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="431" />
        <source>Change the calendar rule %1</source>
        <translation>Modifier la règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="443" />
        <source>Remove the calendar rule %1</source>
        <translation>Supprimer la règle de calendrier %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="715" />
        <source>Run line %1 ("%2") is not understood: %3</source>
        <translation>La ligne Run %1 ("%2") n'est pas comprise : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="724" />
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1177" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1226" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1251" />
        <source>There is no Run line %1.</source>
        <translation>Il n'y a pas de ligne Run %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1179" />
        <source>Run line %1 is not a line of the grid and is not changed here.</source>
        <translation>La ligne Run %1 n'est pas une ligne de la grille et n'est pas modifiée ici.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1207" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1230" />
        <source>A run without days of the month and without weeks of the year is not a calendar rule; it is painted in the grid.</source>
        <translation>Une exécution sans jours du mois et sans semaines de l'année n'est pas une règle de calendrier ; elle est peinte dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1228" />
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1253" />
        <source>Run line %1 is not a calendar rule.</source>
        <translation>La ligne Run %1 n'est pas une règle de calendrier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1288" />
        <source>The generated Run line "%1" does not say what was intended.</source>
        <translation>La ligne Run générée "%1" ne dit pas ce qui était prévu.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridText</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="74" />
        <source>Monday</source>
        <translation>Lundi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="75" />
        <source>Tuesday</source>
        <translation>Mardi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="76" />
        <source>Wednesday</source>
        <translation>Mercredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="77" />
        <source>Thursday</source>
        <translation>Jeudi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="78" />
        <source>Friday</source>
        <translation>Vendredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="79" />
        <source>Saturday</source>
        <translation>Samedi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="80" />
        <source>Sunday</source>
        <translation>Dimanche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="88" />
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="89" />
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="90" />
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="91" />
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="92" />
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="93" />
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="94" />
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="102" />
        <source>January</source>
        <translation>Janvier</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="103" />
        <source>February</source>
        <translation>Février</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="104" />
        <source>March</source>
        <translation>Mars</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="105" />
        <source>April</source>
        <translation>Avril</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="106" />
        <source>May</source>
        <translation>Mai</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="107" />
        <source>June</source>
        <translation>Juin</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="108" />
        <source>July</source>
        <translation>Juillet</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="109" />
        <source>August</source>
        <translation>Août</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="110" />
        <source>September</source>
        <translation>Septembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="111" />
        <source>October</source>
        <translation>Octobre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="112" />
        <source>November</source>
        <translation>Novembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="113" />
        <source>December</source>
        <translation>Décembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="122" />
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="128" />
        <source>%1th</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="124" />
        <source>%1st</source>
        <translation>%1er</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="125" />
        <source>%1nd</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="126" />
        <source>%1rd</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="142" />
        <source>Every week</source>
        <translation>Chaque semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="157" />
        <source>all</source>
        <translation>toutes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="187" />
        <source>last week only</source>
        <translation>dernière semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="190" />
        <source>first week only</source>
        <translation>première semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="191" />
        <source>second week only</source>
        <translation>deuxième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="192" />
        <source>third week only</source>
        <translation>troisième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="193" />
        <source>fourth week only</source>
        <translation>quatrième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="194" />
        <source>fifth week only</source>
        <translation>cinquième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="199" />
        <source>only %1</source>
        <translation>uniquement %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="207" />
        <source>level of the job</source>
        <translation>niveau du Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="250" />
        <source>pool %1</source>
        <translation>pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="251" />
        <source>full pool %1</source>
        <translation>pool Full %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="252" />
        <source>incremental pool %1</source>
        <translation>pool Incremental %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="253" />
        <source>differential pool %1</source>
        <translation>pool Differential %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="254" />
        <source>next pool %1</source>
        <translation>pool suivant %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="255" />
        <source>storage %1</source>
        <translation>storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="256" />
        <source>messages %1</source>
        <translation>messages %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="257" />
        <source>priority %1</source>
        <translation>priorité %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258" />
        <source>spool data yes</source>
        <translation>spool des données oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258" />
        <source>spool data no</source>
        <translation>spool des données non</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259" />
        <source>accurate yes</source>
        <translation>accurate oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259" />
        <source>accurate no</source>
        <translation>accurate non</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="260" />
        <source>max run sched time %1</source>
        <translation>durée max. depuis la planification %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="266" />
        <source>%1 at :%2</source>
        <translation>%1 à :%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="276" />
        <source>%1 at %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="308" />
        <source>%1 %2 hours</source>
        <translation>%1 %2 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="316" />
        <source>%1: no run</source>
        <translation>%1 : aucune exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="325" />
        <source>the Run line names no time</source>
        <translation>la ligne Run n'indique aucune heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="338" />
        <source>no run in this scope</source>
        <translation>aucune exécution dans cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="342" />
        <source>in other scopes: %1</source>
        <translation>dans d'autres portées : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="369" />
        <source>and</source>
        <translation>et</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="370" />
        <source>or</source>
        <translation>ou</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="378" />
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="400" />
        <source>on every %1 day (starting with the %2)</source>
        <translation>chaque %1 jour (à partir du %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="402" />
        <source>on the %1</source>
        <translation>le %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="405" />
        <source>%1 of every month</source>
        <translation>%1 de chaque mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="406" />
        <source>%1 of %2</source>
        <translation>%1 en %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="408" />
        <source>in %1</source>
        <translation>en %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="417" />
        <source>in every %1 week of the year (starting with week %2)</source>
        <translation>chaque %1 semaine de l'année (à partir de la semaine %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="425" />
        <source>in week %1 of the year</source>
        <translation>la semaine %1 de l'année</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="426" />
        <source>in weeks %1 of the year</source>
        <translation>les semaines %1 de l'année</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="433" />
        <source>in the last seven days of the month</source>
        <translation>les sept derniers jours du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="435" />
        <source>in the %1 week of the month</source>
        <translation>la %1 semaine du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="438" />
        <source>or in its last seven days</source>
        <translation>ou ses sept derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="452" />
        <source>only if it is a %1</source>
        <translation>uniquement si c'est un %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="454" />
        <source>on %1</source>
        <translation>le %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="462" />
        <source>every hour at :00 (the line names no time)</source>
        <translation>toutes les heures à :00 (la ligne n'indique aucune heure)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="464" />
        <source>every hour at :%1</source>
        <translation>toutes les heures à :%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="472" />
        <source>at %1</source>
        <translation>à %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridView</name>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="255" />
        <source>Undo: %1 (Ctrl+Z)</source>
        <translation>Annuler : %1 (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="256" />
        <source>Nothing to undo (Ctrl+Z)</source>
        <translation>Rien à annuler (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="257" />
        <source>Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Rétablir : %1 (Ctrl+Shift+Z ou Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="258" />
        <source>Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Rien à rétablir (Ctrl+Shift+Z ou Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="293" />
        <source>No schedule chosen. Select a schedule in the list on the left to see it as a grid of weekdays and hours.</source>
        <translation>Aucun Schedule choisi. Sélectionnez un Schedule dans la liste de gauche pour l'afficher sous forme de grille de jours de semaine et d'heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="334" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="335" />
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="337" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="338" />
        <source>Redo</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="340" />
        <source>Revert all</source>
        <translation>Tout annuler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="341" />
        <source>Undoes every change of this schedule. Redo brings them back.</source>
        <translation>Annule toutes les modifications de ce Schedule. Rétablir les ramène.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="343" />
        <source>&amp;Save...</source>
        <translation>&amp;Enregistrer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="345" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="788" />
        <source>Writes the changed Run lines to the Director. The Director checks them first, and you see old and new lines and the jobs that use the schedule before anything is written.</source>
        <translation>Écrit les lignes Run modifiées sur le Director. Le Director les vérifie d'abord, et vous voyez les anciennes et nouvelles lignes ainsi que les Jobs qui utilisent ce Schedule avant que quoi que ce soit ne soit écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="350" />
        <source>Delete...</source>
        <translation>Supprimer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="351" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="792" />
        <source>Deletes this schedule on the Director, after a check that no job uses it and your confirmation</source>
        <translation>Supprime ce Schedule sur le Director, après avoir vérifié qu'aucun Job ne l'utilise et après votre confirmation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="366" />
        <source>No description</source>
        <translation>Pas de description</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="367" />
        <source>The Description of the schedule. Saved with the Run lines; empty removes it from the file.</source>
        <translation>La Description du Schedule. Enregistrée avec les lignes Run ; vide, elle est retirée du fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="399" />
        <source>&amp;Keep my changes</source>
        <translation>&amp;Conserver mes modifications</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="400" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="861" />
        <source>&amp;Discard and reload</source>
        <translation>&amp;Abandonner et recharger</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="410" />
        <source>Scopes</source>
        <translation>Portées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="414" />
        <source>Scope:</source>
        <translation>Portée :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="415" />
        <source>The weeks of the month and the months a Run line is restricted to. The grid shows the runs of the chosen scope; runs of the other scopes are hatched.</source>
        <translation>Les semaines du mois et les mois auxquels une ligne Run est restreinte. La grille affiche les exécutions de la portée choisie ; celles des autres portées sont hachurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="422" />
        <source>Add scope</source>
        <translation>Ajouter une portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="423" />
        <source>Add scope: weeks of the month, and months, to paint a grid of their own for</source>
        <translation>Ajouter une portée : des semaines du mois, et des mois, pour lesquels peindre une grille propre</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="448" />
        <source>Legend</source>
        <translation>Légende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="469" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="473" />
        <source>Run lines</source>
        <translation>Lignes Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <source>No.</source>
        <translation>N°</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478" />
        <source>Shown in</source>
        <translation>Affiché dans</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="479" />
        <source>The Run lines of the schedule in their order. Choosing a line outlines its cells in the grid and switches to its scope.</source>
        <translation>Les lignes Run du Schedule dans leur ordre. Choisir une ligne entoure ses cellules dans la grille et passe à sa portée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="495" />
        <source>&amp;Add rule...</source>
        <translation>&amp;Ajouter une règle...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="496" />
        <source>Add calendar rule</source>
        <translation>Ajouter une règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="497" />
        <source>Adds a calendar rule: a Run line by day of the month or by week of the year, such as "on 1 at 21:00"</source>
        <translation>Ajoute une règle de calendrier : une ligne Run selon le jour du mois ou la semaine de l'année, par exemple "on 1 at 21:00"</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499" />
        <source>&amp;Edit rule...</source>
        <translation>&amp;Modifier la règle...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="500" />
        <source>Edit calendar rule</source>
        <translation>Modifier la règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="501" />
        <source>Opens the form of the calendar rule that is chosen in the list</source>
        <translation>Ouvre le formulaire de la règle de calendrier choisie dans la liste</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="502" />
        <source>&amp;Remove rule</source>
        <translation>&amp;Supprimer la règle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="503" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="1360" />
        <source>Remove calendar rule</source>
        <translation>Supprimer la règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="504" />
        <source>Removes the calendar rule that is chosen in the list</source>
        <translation>Supprime la règle de calendrier choisie dans la liste</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="514" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="518" />
        <source>Calendar rules</source>
        <translation>Règles de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Mark</source>
        <translation>Marque</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523" />
        <source>Runs</source>
        <translation>Exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="524" />
        <source>Run lines by day of the month or week of the year. They do not belong to a weekday and are marked in the band below the grid. Enter or a double click opens the form of a rule, Del removes it.</source>
        <translation>Lignes Run selon le jour du mois ou la semaine de l'année. Elles n'appartiennent à aucun jour de semaine et sont marquées dans la bande sous la grille. Entrée ou un double-clic ouvre le formulaire d'une règle, Suppr la supprime.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="542" />
        <source>Next five weeks</source>
        <translation>Cinq prochaines semaines</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="548" />
        <source>The levels that run on each date</source>
        <translation>Les niveaux exécutés à chaque date</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="553" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="554" />
        <source>Previous month</source>
        <translation>Mois précédent</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="557" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="558" />
        <source>Next month</source>
        <translation>Mois suivant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="560" />
        <source>Today</source>
        <translation>Aujourd'hui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="561" />
        <source>Back to the current week</source>
        <translation>Revenir à la semaine en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="721" />
        <source>Modified: the description, not written to the Director.</source>
        <translation>Modifié : la description, non écrite sur le Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="736" />
        <source>%n changed</source>
        <translation>
            <numerusform>%n modifiée</numerusform>
            <numerusform>%n modifiées</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="738" />
        <source>%n new</source>
        <translation>
            <numerusform>%n nouvelle</numerusform>
            <numerusform>%n nouvelles</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="740" />
        <source>%n removed</source>
        <translation>
            <numerusform>%n supprimée</numerusform>
            <numerusform>%n supprimées</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="742" />
        <source>Modified: %1 Run line(s) and the description, not written to the Director.</source>
        <translation>Modifié : %1 ligne(s) Run et la description, non écrites sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="744" />
        <source>Modified: %1 Run line(s), not written to the Director.</source>
        <translation>Modifié : %1 ligne(s) Run, non écrite(s) sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="723" />
        <location filename="../src/schedules/bschedulegridview.cpp" line="747" />
        <source>The Director reports other Run lines since.</source>
        <translation>Le Director signale depuis d'autres lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="362" />
        <source>Descr&amp;iption:</source>
        <translation>Descr&amp;iption :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="790" />
        <source>This connection cannot change a schedule; the click says why.</source>
        <translation>Cette connexion ne peut pas modifier un Schedule ; un clic en indique la raison.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="794" />
        <source>This connection cannot delete a schedule; the click says why.</source>
        <translation>Cette connexion ne peut pas supprimer un Schedule ; un clic en indique la raison.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="858" />
        <source>The Director now reports other Run lines for this schedule than the ones your changes started from. Your changes are kept and nothing was replaced.</source>
        <translation>Le Director signale maintenant pour ce Schedule d'autres lignes Run que celles dont vos modifications sont parties. Vos modifications sont conservées et rien n'a été remplacé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="863" />
        <source>The schedule %1 has changes that are not written. Showing %2 discards them.</source>
        <translation>Le Schedule %1 comporte des modifications non écrites. Afficher %2 les abandonne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="865" />
        <source>&amp;Discard and show %1</source>
        <translation>&amp;Abandonner et afficher %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="892" />
        <source>Schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="897" />
        <source>(jobs not loaded yet)</source>
        <translation>(Jobs pas encore chargés)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="899" />
        <source>Not used by any job</source>
        <translation>Utilisé par aucun Job</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="901" />
        <source>Used by %n job(s)</source>
        <translation>
            <numerusform>Utilisé par %n Job</numerusform>
            <numerusform>Utilisé par %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="907" />
        <source>Disabled: the Director does not run it.</source>
        <translation>Désactivé : le Director ne l'exécute pas.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="912" />
        <source>Contains %n Run line(s) that are not understood. The schedule is locked: it cannot be edited or written here. Change it in its configuration file on the Director.</source>
        <translation>
            <numerusform>Contient %n ligne Run non comprise. Le Schedule est verrouillé : il ne peut être ni modifié ni écrit ici. Modifiez-le dans son fichier de configuration sur le Director.</numerusform>
            <numerusform>Contient %n lignes Run non comprises. Le Schedule est verrouillé : il ne peut être ni modifié ni écrit ici. Modifiez-le dans son fichier de configuration sur le Director.</numerusform>
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
        <translation>%1 : aucune ligne Run dans cette portée. Suppr ou le menu contextuel la supprime.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="945" />
        <source>%1: no Run line in this scope</source>
        <translation>%1 : aucune ligne Run dans cette portée</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="946" />
        <source>%1: %n Run line(s) in this scope</source>
        <translation>
            <numerusform>%1 : %n ligne Run dans cette portée</numerusform>
            <numerusform>%1 : %n lignes Run dans cette portée</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="952" />
        <source>Scope %1, %n Run line(s)</source>
        <translation>
            <numerusform>Portée %1, %n ligne Run</numerusform>
            <numerusform>Portée %1, %n lignes Run</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="989" />
        <source>other scope, may run on the same day</source>
        <translation>autre portée, peut s'exécuter le même jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="993" />
        <source>other scope, never on the same day</source>
        <translation>autre portée, jamais le même jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1007" />
        <source>no time in the Run line: runs every hour at :00</source>
        <translation>aucune heure dans la ligne Run : s'exécute toutes les heures à :00</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1015" />
        <source>calendar rule, not a cell</source>
        <translation>règle de calendrier, pas une cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1045" />
        <source>changed</source>
        <translation>modifiée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1048" />
        <source>new</source>
        <translation>nouvelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1051" />
        <source>removed</source>
        <translation>supprimée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1063" />
        <source>Reported by the Director as "%1", a spelling it does not read back.</source>
        <translation>Signalée par le Director sous la forme "%1", une écriture qu'il ne relit pas.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1070" />
        <source>not understood: %1</source>
        <translation>non comprise : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1071" />
        <source>Not understood and not shown in the grid: %1</source>
        <translation>Non comprise et non affichée dans la grille : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1072" />
        <source>This schedule cannot be written by the application.</source>
        <translation>Ce Schedule ne peut pas être écrit par l'application.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1081" />
        <source>calendar rule %1</source>
        <translation>règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1086" />
        <source>scope %1</source>
        <translation>portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1108" />
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1137" />
        <source>Nothing selected. Click a cell or drag over cells; click a day or an hour to select all of it.</source>
        <translation>Rien n'est sélectionné. Cliquez sur une cellule ou faites glisser sur des cellules ; cliquez sur un jour ou une heure pour le sélectionner entièrement.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1172" />
        <source>%1 in %n cell(s)</source>
        <translation>
            <numerusform>%1 dans %n cellule</numerusform>
            <numerusform>%1 dans %n cellules</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1176" />
        <source>%n cell(s) selected:</source>
        <translation>
            <numerusform>%n cellule sélectionnée :</numerusform>
            <numerusform>%n cellules sélectionnées :</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1177" />
        <source>no run in this scope</source>
        <translation>aucune exécution dans cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1180" />
        <source>Other scopes: %1</source>
        <translation>Autres portées : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1181" />
        <source>They stay when this scope is painted or erased</source>
        <translation>Elles restent lorsque cette portée est peinte ou effacée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1286" />
        <source>Remove the scope "%1"</source>
        <translation>Supprimer la portée "%1"</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1289" />
        <source>Add scope...</source>
        <translation>Ajouter une portée...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1328" />
        <source>Calendar rule %1</source>
        <translation>Règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1331" />
        <source>New calendar rule</source>
        <translation>Nouvelle règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1359" />
        <source>Edit calendar rule...</source>
        <translation>Modifier la règle de calendrier...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1364" />
        <source>Add calendar rule...</source>
        <translation>Ajouter une règle de calendrier...</translation>
    </message>
</context>
<context>
    <name>BScheduleGridWidget</name>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="68" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="412" />
        <source>%1 hours</source>
        <translation>%1 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="69" />
        <source>Select all</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="73" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="199" />
        <source>Selects the whole day</source>
        <translation>Sélectionne toute la journée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="75" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="201" />
        <source>Selects this hour on every day</source>
        <translation>Sélectionne cette heure pour chaque jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="77" />
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="202" />
        <source>Selects all days and hours</source>
        <translation>Sélectionne tous les jours et toutes les heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="187" />
        <source>Apply the tool</source>
        <translation>Appliquer l'outil</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="189" />
        <source>Erase</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="197" />
        <source>Selects this cell</source>
        <translation>Sélectionne cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="205" />
        <source>Lists the runs of this cell</source>
        <translation>Liste les exécutions de cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="207" />
        <source>Applies the tool in hand (%1) to this cell</source>
        <translation>Applique l'outil en main (%1) à cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="211" />
        <source>Removes the runs of the scope on display from this cell</source>
        <translation>Retire de cette cellule les exécutions de la portée affichée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="310" />
        <source>Schedule grid of %1, scope %2</source>
        <translation>Grille du Schedule %1, portée %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="314" />
        <source>Weekdays by hours. Arrow keys move, Shift extends the selection, Enter lists the runs of a cell.</source>
        <translation>Jours de semaine par heures. Les flèches déplacent, Maj étend la sélection, Entrée liste les exécutions d'une cellule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="317" />
        <source>Space applies the tool to the selected cells, Delete erases them, F, D, I and V paint a level.</source>
        <translation>Espace applique l'outil aux cellules sélectionnées, Suppr les efface, F, D, I et V peignent un niveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="319" />
        <source>Tool: %1.</source>
        <translation>Outil : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="321" />
        <source>Paint mode is on.</source>
        <translation>Le mode peinture est activé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="325" />
        <source>Calendar rule %1: %2, %3</source>
        <translation>Règle de calendrier %1 : %2, %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="59" />
        <source>Schedule grid</source>
        <translation>Grille du Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="281" />
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="631" />
        <source>Calendar rules</source>
        <translation>Règles de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1239" />
        <source>%1, %2 to %3</source>
        <translation>%1, %2 à %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1249" />
        <source>the Run line names no time</source>
        <translation>la ligne Run n'indique aucune heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1252" />
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1259" />
        <source>No run in the scope "%1"</source>
        <translation>Aucune exécution dans la portée "%1"</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1266" />
        <source>Other scopes:</source>
        <translation>Autres portées :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1270" />
        <source>%1: may run on the same day as this scope</source>
        <translation>%1 : peut s'exécuter le même jour que cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1271" />
        <source>%1: never on the same day as this scope</source>
        <translation>%1 : jamais le même jour que cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1277" />
        <source>Painting or erasing here changes only the scope "%1"; the runs of other scopes stay.</source>
        <translation>Peindre ou effacer ici ne modifie que la portée "%1" ; les exécutions des autres portées restent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1284" />
        <source>Expected load: %1 runs at the same time</source>
        <translation>Charge prévue : %1 exécutions simultanées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1299" />
        <source>%1: click to select the whole day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1 : cliquez pour sélectionner toute la journée. Ctrl l'ajoute ou la retire, Maj étend la sélection depuis son ancre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1303" />
        <source>%1 to %2: click to select this hour on every day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1 à %2 : cliquez pour sélectionner cette heure pour chaque jour. Ctrl l'ajoute ou la retire, Maj étend la sélection depuis son ancre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1308" />
        <source>Click to select all days and hours (Ctrl+A)</source>
        <translation>Cliquez pour sélectionner tous les jours et toutes les heures (Ctrl+A)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1310" />
        <source>Run lines by day of the month or week of the year do not belong to a weekday. They are marked here at their hours and listed under "Calendar rules"; this band cannot be selected or painted. A double click on a mark opens the form of its rule.</source>
        <translation>Les lignes Run selon le jour du mois ou la semaine de l'année n'appartiennent à aucun jour de semaine. Elles sont marquées ici à leurs heures et listées sous "Règles de calendrier" ; cette bande ne peut être ni sélectionnée ni peinte. Un double-clic sur une marque ouvre le formulaire de sa règle.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1391" />
        <source>%1, %2 to %3: %n run(s)</source>
        <translation>
            <numerusform>%1, %2 à %3 : %n exécution</numerusform>
            <numerusform>%1, %2 à %3 : %n exécutions</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1399" />
        <source>Runs of the cell</source>
        <translation>Exécutions de la cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Overrides</source>
        <translation>Remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Scope</source>
        <translation>Portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404" />
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1417" />
        <source>no time in the Run line</source>
        <translation>aucune heure dans la ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1423" />
        <source>%1 (other scope, may run on the same day)</source>
        <translation>%1 (autre portée, peut s'exécuter le même jour)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1424" />
        <source>%1 (other scope, never on the same day)</source>
        <translation>%1 (autre portée, jamais le même jour)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1431" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1443" />
        <source>Enter or a double click shows the Run line in the list. Esc closes.</source>
        <translation>Entrée ou un double-clic affiche la ligne Run dans la liste. Échap ferme.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1544" />
        <source>Tool: %1</source>
        <translation>Outil : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleLegend</name>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="40" />
        <source>Full backup. The length of the bar is the median duration of the job's measured runs at this level.</source>
        <translation>Sauvegarde Full. La longueur de la barre est la durée médiane des exécutions mesurées du Job à ce niveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="42" />
        <source>Differential backup.</source>
        <translation>Sauvegarde Differential.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="44" />
        <source>Incremental backup.</source>
        <translation>Sauvegarde Incremental.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="46" />
        <source>Virtual full backup.</source>
        <translation>Sauvegarde VirtualFull.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="48" />
        <source>Pale and dashed, "~" in front of the letter: no run at this level was measured; the length is estimated from the job's runs at other levels.</source>
        <translation>Pâle et en pointillés, "~" devant la lettre : aucune exécution n'a été mesurée à ce niveau ; la longueur est estimée à partir des exécutions du Job aux autres niveaux.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="51" />
        <source>Dashed outline with "?": the job never ran, so there is no duration. Only the start is shown.</source>
        <translation>Contour en pointillés avec "?" : le Job ne s'est jamais exécuté, il n'y a donc pas de durée. Seul le début est affiché.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="53" />
        <source>Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).</source>
        <translation>Prolongement plus clair avec un trait à son extrémité : neuf exécutions sur dix étaient terminées à ce moment (90e centile).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="55" />
        <source>Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.</source>
        <translation>Sablier plein : l'exécution attendra. Le trait coloré derrière lui est l'heure planifiée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="57" />
        <source>Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).</source>
        <translation>Contour de sablier : l'exécution peut attendre, la prévision n'est pas certaine (voir la liste des attentes prévues).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="59" />
        <source>Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.</source>
        <translation>Bande hachurée : l'attente, de l'heure planifiée au début prévu. Un contour en pointillés : non certain.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="61" />
        <source>"?" in front of a bar: the prediction for this run is not certain, it may have to wait.</source>
        <translation>"?" devant une barre : la prévision pour cette exécution n'est pas certaine, elle pourrait devoir attendre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="63" />
        <source>"»" at the right edge: the run starts after the last day on display.</source>
        <translation>"»" au bord droit : l'exécution démarre après le dernier jour affiché.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="65" />
        <source>Open diamond: no job uses this schedule. It marks where a run would start.</source>
        <translation>Losange vide : aucun Job n'utilise ce Schedule. Il marque l'endroit où une exécution démarrerait.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="67" />
        <source>Arrow (day view only): the run at the head waits for the run at the tail, from the moment that one lets it go. The label names the limit; dotted: priority; dashed: only if something unknown turns out against the run. Point at a run to see its arrows.</source>
        <translation>Flèche (vue journalière uniquement) : l'exécution à la pointe attend l'exécution à la base, à partir du moment où celle-ci la libère. L'étiquette nomme la limite ; pointillés fins : priorité ; tirets : uniquement si un élément inconnu tourne en défaveur de l'exécution. Survolez une exécution pour voir ses flèches.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="71" />
        <source>Red line: now.</source>
        <translation>Trait rouge : maintenant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="259" />
        <source>&lt;b&gt;What the marks mean&lt;/b&gt;</source>
        <translation>&lt;b&gt;Signification des marques&lt;/b&gt;</translation>
    </message>
</context>
<context>
    <name>BScheduleMiniCalendar</name>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="85" />
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="110" />
        <source>Calendar of runs, %1</source>
        <translation>Calendrier des exécutions, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="116" />
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="125" />
        <source>%1: no run</source>
        <translation>%1 : aucune exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="128" />
        <source>%1 %2 (Run line %3)</source>
        <translation>%1 %2 (ligne Run %3)</translation>
    </message>
</context>
<context>
    <name>BSchedulePalette</name>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="124" />
        <source>Brushes</source>
        <translation>Pinceaux</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="136" />
        <location filename="../src/schedules/bschedulepalette.cpp" line="137" />
        <source>Eraser</source>
        <translation>Gomme</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="138" />
        <source>Removes the runs of the scope on display from cells. Del erases the selected cells with any tool in hand.</source>
        <translation>Retire des cellules les exécutions de la portée affichée. Suppr efface les cellules sélectionnées quel que soit l'outil en main.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="141" />
        <source>New brush...</source>
        <translation>Nouveau pinceau...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="143" />
        <source>Defines a brush: level, minute of the hour and overrides</source>
        <translation>Définit un pinceau : niveau, minute de l'heure et remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="145" />
        <source>Paint mode</source>
        <translation>Mode peinture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="147" />
        <source>On: pressing a cell and dragging over cells applies the tool at once. Off: the mouse selects, and Space applies the tool to the selection. The key P switches it in the grid.</source>
        <translation>Activé : appuyer sur une cellule et faire glisser sur des cellules applique immédiatement l'outil. Désactivé : la souris sélectionne et Espace applique l'outil à la sélection. La touche P le bascule dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="151" />
        <source>Keep other runs</source>
        <translation>Conserver les autres exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="153" />
        <source>On: painting adds the brush to the runs a cell already has, so a cell can have several runs in one hour. Off: the cell ends up with the brush only. Shift+Space adds once without this switch.</source>
        <translation>Activé : peindre ajoute le pinceau aux exécutions qu'une cellule possède déjà, une cellule peut donc avoir plusieurs exécutions dans une heure. Désactivé : la cellule ne contient plus que le pinceau. Maj+Espace ajoute une fois sans ce commutateur.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="198" />
        <source>%1 = %2</source>
        <translation>%1 = %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="246" />
        <source>Brush %1</source>
        <translation>Pinceau %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="247" />
        <source>Takes this brush in hand: %1. Space applies it to the selected cells.</source>
        <translation>Prend ce pinceau en main : %1. Espace l'applique aux cellules sélectionnées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="308" />
        <source>In hand: the eraser.</source>
        <translation>En main : la gomme.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="310" />
        <source>In hand: the brush %1.</source>
        <translation>En main : le pinceau %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="314" />
        <source>Paint mode: press a cell or drag over cells to apply it.</source>
        <translation>Mode peinture : appuyez sur une cellule ou faites glisser sur des cellules pour l'appliquer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="316" />
        <source>Space applies it to the selected cells, Del erases them.</source>
        <translation>Espace l'applique aux cellules sélectionnées, Suppr les efface.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="319" />
        <source>Other runs of a painted cell stay.</source>
        <translation>Les autres exécutions d'une cellule peinte restent.</translation>
    </message>
</context>
<context>
    <name>BSchedulePersistence</name>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="103" />
        <source>The text of schedule %1 arrived changed</source>
        <translation>Le texte du Schedule %1 est arrivé modifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="104" />
        <source>The SHA-256 of the text the Director sent does not match the text. Nothing was written.</source>
        <translation>Le SHA-256 du texte envoyé par le Director ne correspond pas au texte. Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="117" />
        <source>Schedule %1 has changed on the Director</source>
        <translation>Le Schedule %1 a été modifié sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="118" />
        <source>The configuration file of the schedule does not say what was loaded here, so the change was not sent. Reload the schedule and make the change again. %1</source>
        <translation>Le fichier de configuration du Schedule ne correspond pas à ce qui a été chargé ici ; la modification n'a donc pas été envoyée. Rechargez le Schedule et refaites la modification. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="146" />
        <source>Schedule %1 cannot be written</source>
        <translation>Le Schedule %1 ne peut pas être écrit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="147" />
        <source>Run line %1 is not understood by this application (%2). A schedule with such a line is not changed here; change it in its configuration file.</source>
        <translation>La ligne Run %1 n'est pas comprise par cette application (%2). Un Schedule contenant une telle ligne n'est pas modifié ici ; modifiez-le dans son fichier de configuration.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="156" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="189" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="276" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="317" />
        <source>Schedule %1 was not changed</source>
        <translation>Le Schedule %1 n'a pas été modifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="157" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="376" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="447" />
        <source>There is no connection to the configuration methods of the Director.</source>
        <translation>Aucune connexion aux méthodes de configuration du Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="190" />
        <source>The change could not be written into the configuration text safely: %1</source>
        <translation>La modification n'a pas pu être écrite de manière sûre dans le texte de configuration : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="198" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="264" />
        <source>Nothing to write</source>
        <translation>Rien à écrire</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="199" />
        <source>The Run lines and the description are the ones the Director has.</source>
        <translation>Les lignes Run et la description sont celles du Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="265" />
        <source>The edited text is the one the Director has.</source>
        <translation>Le texte modifié est celui que possède le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="277" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="417" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="493" />
        <source>Nothing was written.</source>
        <translation>Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="316" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="446" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="492" />
        <source>Schedule %1 was not created</source>
        <translation>Le Schedule %1 n'a pas été créé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="318" />
        <source>The edited text was not sent to the Director: %1</source>
        <translation>Le texte modifié n'a pas été envoyé au Director : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="326" />
        <source>the name of the schedule has to stay %1.</source>
        <translation>le nom du Schedule doit rester %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="338" />
        <source>The text was edited in the review; the Director checked the edited text.</source>
        <translation>Le texte a été modifié lors de la vérification ; le Director a vérifié le texte modifié.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="375" />
        <location filename="../src/schedules/bschedulepersistence.cpp" line="416" />
        <source>Schedule %1 was not deleted</source>
        <translation>Le Schedule %1 n'a pas été supprimé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="573" />
        <source>Run line %1 cannot be written: %2</source>
        <translation>La ligne Run %1 ne peut pas être écrite : %2</translation>
    </message>
</context>
<context>
    <name>BSchedulePreviewPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="674" />
        <source>Preview &amp; Deploy</source>
        <translation>Aperçu et déploiement</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="675" />
        <source>Review the schedule configuration and deploy to the Director.</source>
        <translation>Vérifiez la configuration du Schedule et déployez-la sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="680" />
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="691" />
        <source>Copy Configuration</source>
        <translation>Copier la configuration</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="698" />
        <source>Director Command:</source>
        <translation>Commande du Director :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="709" />
        <source>Copy Command</source>
        <translation>Copier la commande</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="756" />
        <source>CreateResource (gRPC) with the configuration text above</source>
        <translation>CreateResource (gRPC) avec le texte de configuration ci-dessus</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="767" />
        <source>This schedule cannot be written. %1</source>
        <translation>Ce Schedule ne peut pas être écrit. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="780" />
        <source>Schedule cannot be written</source>
        <translation>Le Schedule ne peut pas être écrit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="781" />
        <source>%1

Go back and remove or replace this run, or cancel the wizard.</source>
        <translation>%1

Revenez en arrière et supprimez ou remplacez cette exécution, ou annulez l'assistant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="796" />
        <source>Deploy to Director</source>
        <translation>Déployer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="797" />
        <source>Send 'configure add' to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Envoyer 'configure add' au Director ?

Cliquez sur Oui pour exécuter, sur Non pour fermer sans déployer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="819" />
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="846" />
        <source>The Director checks the schedule...</source>
        <translation>Le Director vérifie le Schedule...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="853" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="884" />
        <source>Schedule created successfully! Click Finish to close.</source>
        <translation>Schedule créé avec succès ! Cliquez sur Terminer pour fermer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="856" />
        <source>Nothing was written.</source>
        <translation>Rien n'a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="859" />
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="887" />
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="897" />
        <source>The connection to the Director was lost before it answered. Reconnect and check with 'show schedules' whether the schedule exists.</source>
        <translation>La connexion au Director a été perdue avant sa réponse. Reconnectez-vous et vérifiez avec 'show schedules' si le Schedule existe.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="905" />
        <source>Timeout waiting for Director response. The schedule may have been created — check with 'show schedules'.</source>
        <translation>Délai dépassé en attente de la réponse du Director. Le Schedule a peut-être été créé — vérifiez avec 'show schedules'.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="922" />
        <source>Configuration copied to clipboard.</source>
        <translation>Configuration copiée dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="929" />
        <source>Command copied to clipboard.</source>
        <translation>Commande copiée dans le presse-papiers.</translation>
    </message>
</context>
<context>
    <name>BScheduleResource</name>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="97" />
        <source>The text does not start with a Schedule resource (line %1).</source>
        <translation>Le texte ne commence pas par une ressource Schedule (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="101" />
        <source>Directives on the line of "Schedule {" cannot be edited safely (line %1).</source>
        <translation>Les directives situées sur la ligne de "Schedule {" ne peuvent pas être modifiées de manière sûre (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="107" />
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Texte après l'accolade fermante de la ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="111" />
        <source>A nested block in a Schedule resource (line %1).</source>
        <translation>Un bloc imbriqué dans une ressource Schedule (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="114" />
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>L'accolade fermante partage sa ligne avec une directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="145" />
        <source>A quoted Run value does not end on its line (line %1).</source>
        <translation>Une valeur Run entre guillemets ne se termine pas sur sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="148" />
        <source>Something follows the Run value on its line (line %1).</source>
        <translation>Quelque chose suit la valeur Run sur sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="156" />
        <source>A Run directive shares its line with another directive (line %1).</source>
        <translation>Une directive Run partage sa ligne avec une autre directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="163" />
        <source>A Run directive without a value (line %1).</source>
        <translation>Une directive Run sans valeur (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="173" />
        <source>A Run directive that does not start its line (line %1).</source>
        <translation>Une directive Run qui ne commence pas sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="184" />
        <source>The text contains no Schedule resource.</source>
        <translation>Le texte ne contient aucune ressource Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="186" />
        <source>The Schedule resource has no closing brace.</source>
        <translation>La ressource Schedule n'a pas d'accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="217" />
        <source>The configuration file has %1 Run line(s), the schedule as it was loaded %2.</source>
        <translation>Le fichier de configuration a %1 ligne(s) Run, le Schedule tel qu'il a été chargé %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="227" />
        <source>Run line %1 differs: the file says "%2", the schedule as it was loaded "%3".</source>
        <translation>La ligne Run %1 diffère : le fichier indique "%2", le Schedule tel qu'il a été chargé "%3".</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="249" />
        <source>A new Run line without text.</source>
        <translation>Une nouvelle ligne Run sans texte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="253" />
        <source>A Run line refers to line %1, which the file does not have.</source>
        <translation>Une ligne Run fait référence à la ligne %1, que le fichier ne contient pas.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="255" />
        <source>The Run lines would change their order; that is not written.</source>
        <translation>Les lignes Run changeraient d'ordre ; ce n'est pas écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="258" />
        <source>A changed Run line without text.</source>
        <translation>Une ligne Run modifiée sans texte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="262" />
        <source>A Run line contains a line break.</source>
        <translation>Une ligne Run contient un saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="364" />
        <source>The new text could not be read back: %1</source>
        <translation>Le nouveau texte n'a pas pu être relu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="369" />
        <source>The new text does not have the intended Run lines.</source>
        <translation>Le nouveau texte n'a pas les lignes Run prévues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="371" />
        <source>The new text would change more than the Run lines.</source>
        <translation>Le nouveau texte modifierait plus que les lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="373" />
        <source>The new text would change the name of the schedule.</source>
        <translation>Le nouveau texte modifierait le nom du Schedule.</translation>
    </message>
</context>
<context>
    <name>BScheduleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408" />
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408" />
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewDialog</name>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="54" />
        <source>(no Run line)</source>
        <translation>(aucune ligne Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="91" />
        <source>changed: </source>
        <translation>modifiée : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="92" />
        <source>new: </source>
        <translation>nouvelle : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="93" />
        <source>removed: </source>
        <translation>supprimée : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="108" />
        <source>Change schedule %1</source>
        <translation>Modifier le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="109" />
        <source>Change schedule %1 on the Director?</source>
        <translation>Modifier le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="110" />
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Le Director a vérifié la nouvelle configuration et l'accepte. Rien n'est écrit tant que vous n'avez pas choisi "Écrire sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="114" />
        <source>Create schedule %1</source>
        <translation>Créer le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="115" />
        <source>Create schedule %1 on the Director?</source>
        <translation>Créer le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="116" />
        <source>The Director checked the new schedule and accepts it. Nothing is written until you choose "Write to the Director".</source>
        <translation>Le Director a vérifié le nouveau Schedule et l'accepte. Rien n'est écrit tant que vous n'avez pas choisi "Écrire sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="120" />
        <source>Delete schedule %1</source>
        <translation>Supprimer le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="121" />
        <source>Delete schedule %1 on the Director?</source>
        <translation>Supprimer le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="122" />
        <source>The Director checked that nothing refers to the schedule. Nothing is deleted until you choose "Delete on the Director".</source>
        <translation>Le Director a vérifié que rien ne fait référence au Schedule. Rien n'est supprimé tant que vous n'avez pas choisi "Supprimer sur le Director".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="140" />
        <source>Run lines now (in the file)</source>
        <translation>Lignes Run actuelles (dans le fichier)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="143" />
        <source>Run lines now</source>
        <translation>Lignes Run actuelles</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="148" />
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="151" />
        <source>Run lines after writing</source>
        <translation>Lignes Run après l'écriture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="159" />
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="168" />
        <source>Jobs that use this schedule</source>
        <translation>Jobs qui utilisent ce Schedule</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="162" />
        <source>The change applies to every one of these %n job(s), not only to the one on display. To move a single job, copy the schedule and assign the copy to that job instead.</source>
        <translation>
            <numerusform>La modification s'applique à ce Job (%n), pas seulement à celui affiché. Pour déplacer un seul Job, copiez le Schedule et attribuez plutôt la copie à ce Job.</numerusform>
            <numerusform>La modification s'applique à chacun de ces %n Jobs, pas seulement à celui affiché. Pour déplacer un seul Job, copiez le Schedule et attribuez plutôt la copie à ce Job.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="173" />
        <source>(no job of the Director uses it, as far as this application knows)</source>
        <translation>(aucun Job du Director ne l'utilise, à la connaissance de cette application)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="177" />
        <source>The Director names as referencing it: %1</source>
        <translation>Le Director indique comme y faisant référence : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="189" />
        <source>Text after writing, editable</source>
        <translation>Texte après l'écriture, modifiable</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="190" />
        <source>This text can be edited. An edited text is checked by the Director and shown again before anything is written.</source>
        <translation>Ce texte peut être modifié. Un texte modifié est vérifié par le Director et affiché à nouveau avant que quoi que ce soit ne soit écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="193" />
        <source>Text after writing</source>
        <translation>Texte après l'écriture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="196" />
        <source>Text now</source>
        <translation>Texte actuel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="200" />
        <source>The text was edited. "Check with the Director" sends it for a check only; the review then shows it again, and only "Write to the Director" there writes it.</source>
        <translation>Le texte a été modifié. "Vérifier auprès du Director" l'envoie uniquement pour vérification ; la vérification l'affiche ensuite à nouveau, et seul "Écrire sur le Director" l'y écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="210" />
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Fichier sur le Director : %1 (il contient aussi d'autres ressources ; seule cette ressource est remplacée).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="212" />
        <source>File on the Director: %1.</source>
        <translation>Fichier sur le Director : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="215" />
        <source>The Director reloads its configuration after writing. Runs it has already queued may still start at the old time.</source>
        <translation>Le Director recharge sa configuration après l'écriture. Les exécutions qu'il a déjà mises en file d'attente peuvent encore démarrer à l'ancienne heure.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="221" />
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Supprimer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="222" />
        <source>&amp;Write to the Director</source>
        <translation>&amp;Écrire sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="253" />
        <source>&amp;Check with the Director</source>
        <translation>&amp;Vérifier auprès du Director</translation>
    </message>
</context>
<context>
    <name>BScheduleRunDialog</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="48" />
        <source>Edit Run Directive</source>
        <translation>Modifier la directive Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="56" />
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="61" />
        <source>Time:</source>
        <translation>Heure :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="65" />
        <source>(every week)</source>
        <translation>(chaque semaine)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="66" />
        <source>Week of Month:</source>
        <translation>Semaine du mois :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="71" />
        <source>Days of Week</source>
        <translation>Jours de la semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77" />
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78" />
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="89" />
        <source>Daily</source>
        <translation>Tous les jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="93" />
        <source>Mon-Fri</source>
        <translation>Lun-Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="98" />
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="111" />
        <source>Optional Overrides</source>
        <translation>Remplacements facultatifs</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="117" />
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="122" />
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="127" />
        <source>(default)</source>
        <translation>(par défaut)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="128" />
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="138" />
        <source>Preview:</source>
        <translation>Aperçu :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="159" />
        <source>Validation</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="160" />
        <source>Please select at least one day of the week.</source>
        <translation>Veuillez sélectionner au moins un jour de la semaine.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="179" />
        <source>(level of the job)</source>
        <translation>(niveau du Job)</translation>
    </message>
</context>
<context>
    <name>BScheduleRunEditor</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Days</source>
        <translation>Jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308" />
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="323" />
        <source>+ Add Run</source>
        <translation>+ Ajouter une exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="324" />
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="325" />
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="326" />
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="381" />
        <location filename="../src/schedules/bschedulewizard.cpp" line="396" />
        <source>Edit Run Directive</source>
        <translation>Modifier la directive Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="382" />
        <source>This Run line is not understood and cannot be edited:
%1

%2

Remove it to continue.</source>
        <translation>Cette ligne Run n'est pas comprise et ne peut pas être modifiée :
%1

%2

Supprimez-la pour continuer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="397" />
        <source>This Run line uses forms that this dialog cannot show, for example months, days of the month, several times or other overrides:
%1

It is kept exactly as it is. To replace it, remove it and add a new run.</source>
        <translation>Cette ligne Run utilise des formes que cette boîte de dialogue ne peut pas afficher, par exemple des mois, des jours du mois, plusieurs heures ou d'autres remplacements :
%1

Elle est conservée telle quelle. Pour la remplacer, supprimez-la et ajoutez une nouvelle exécution.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="453" />
        <source>Not understood: %1</source>
        <translation>Non comprise : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="454" />
        <source>Not understood</source>
        <translation>Non comprise</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="469" />
        <source>Kept exactly as written: %1</source>
        <translation>Conservée telle qu'écrite : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="479" />
        <source>(level of the job)</source>
        <translation>(niveau du Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="484" />
        <source>daily</source>
        <translation>tous les jours</translation>
    </message>
</context>
<context>
    <name>BScheduleRunsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="636" />
        <source>Run Directives</source>
        <translation>Directives Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="637" />
        <source>Define when this schedule runs backups. Add one or more Run entries.</source>
        <translation>Définissez quand ce Schedule exécute les sauvegardes. Ajoutez une ou plusieurs entrées Run.</translation>
    </message>
</context>
<context>
    <name>BScheduleScopeDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="379" />
        <source>&amp;last (the last seven days of a month)</source>
        <translation>&amp;dernière (les sept derniers jours d'un mois)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="383" />
        <source>Add scope</source>
        <translation>Ajouter une portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="386" />
        <source>A scope is the part of the calendar a grid is painted for: weeks of the month, and months if not all. What is painted in it becomes Run lines such as "1st sat at 21:00".</source>
        <translation>Une portée est la partie du calendrier pour laquelle une grille est peinte : des semaines du mois, et des mois si ce ne sont pas tous. Ce qui y est peint devient des lignes Run comme "1st sat at 21:00".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="392" />
        <source>Weeks of the month</source>
        <translation>Semaines du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="398" />
        <source>Days %1 to %2 of a month</source>
        <translation>Jours %1 à %2 d'un mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="408" />
        <source>Months</source>
        <translation>Mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="417" />
        <source>&amp;All months</source>
        <translation>&amp;Tous les mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="474" />
        <source>Choose at least one month.</source>
        <translation>Choisissez au moins un mois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="476" />
        <source>Choose at least one week of the month, or "last".</source>
        <translation>Choisissez au moins une semaine du mois, ou "dernière".</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="482" />
        <source>The scope "%1" exists already; it will be shown.</source>
        <translation>La portée "%1" existe déjà ; elle sera affichée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="483" />
        <source>New scope: %1</source>
        <translation>Nouvelle portée : %1</translation>
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
        <translation>%1 j</translation>
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
        <translation>aucun Max Run Time défini, %1 supposé</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1119" />
        <source>%n run(s) wait, longest %1</source>
        <translation>
            <numerusform>%n exécution en attente, la plus longue %1</numerusform>
            <numerusform>%n exécutions en attente, la plus longue %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1120" />
        <source>No waiting predicted</source>
        <translation>Aucune attente prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1122" />
        <source>%1 uncertain</source>
        <translation>%1 incertaine(s)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1131" />
        <source>Director limit: the Director runs %1 job(s) at a time (Maximum Concurrent Jobs = %1) and all of them are taken.</source>
        <translation>Limite du Director : le Director exécute %1 Job(s) à la fois (Maximum Concurrent Jobs = %1) et tous les emplacements sont occupés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1133" />
        <source>Director limit: Maximum Concurrent Jobs of the Director is not known. With the Bareos default of 1 every job slot would be taken.</source>
        <translation>Limite du Director : le Maximum Concurrent Jobs du Director n'est pas connu. Avec la valeur par défaut de Bareos de 1, tous les emplacements de Job seraient occupés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1137" />
        <source>Storage limit: storage %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Limite du Storage : le Storage %1 accepte %2 Job(s) à la fois (Maximum Concurrent Jobs = %2) et toutes les places sont occupées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1141" />
        <source>Storage limit: the storage of this job is not known, and so is its Maximum Concurrent Jobs. With one shared storage and the Bareos default of 1 it would be full.</source>
        <translation>Limite du Storage : le Storage de ce Job n'est pas connu, pas plus que son Maximum Concurrent Jobs. Avec un seul Storage partagé et la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1144" />
        <source>Storage limit: Maximum Concurrent Jobs of storage %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Limite du Storage : le Maximum Concurrent Jobs du Storage %1 n'est pas connu. Avec la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1148" />
        <source>Client limit: client %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Limite du Client : le Client %1 accepte %2 Job(s) à la fois (Maximum Concurrent Jobs = %2) et toutes les places sont occupées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1151" />
        <source>Client limit: Maximum Concurrent Jobs of client %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Limite du Client : le Maximum Concurrent Jobs du Client %1 n'est pas connu. Avec la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1155" />
        <source>Job limit: job %1 may run %2 time(s) at once (Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.</source>
        <translation>Limite du Job : le Job %1 peut s'exécuter %2 fois simultanément (Maximum Concurrent Jobs = %2) et un démarrage antérieur de celui-ci n'est pas terminé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1158" />
        <source>Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos default of 1 an earlier start of it would be in the way.</source>
        <translation>Limite du Job : le Maximum Concurrent Jobs du Job %1 n'est pas connu. Avec la valeur par défaut de Bareos de 1, un démarrage antérieur de celui-ci gênerait.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1162" />
        <source>Priority: this run has priority %1 and runs of priority %2 are running. The Director starts a run of a lower priority (a higher number) only when no run of another priority is running.</source>
        <translation>Priorité : cette exécution a la priorité %1 et des exécutions de priorité %2 sont en cours. Le Director ne démarre une exécution de priorité inférieure (un nombre plus élevé) que lorsqu'aucune exécution d'une autre priorité n'est en cours.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1167" />
        <source>Priority: this run has priority %1 and runs of priority %2 are running. It starts beside them only if it and all of them have Allow Mixed Priority.</source>
        <translation>Priorité : cette exécution a la priorité %1 et des exécutions de priorité %2 sont en cours. Elle ne démarre à côté d'elles que si elle et toutes celles-ci ont Allow Mixed Priority.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1171" />
        <source>Queue order: a run of priority %1 is waiting ahead in the queue and cannot start yet. The Director starts no run that is behind it in the queue.</source>
        <translation>Ordre de la file : une exécution de priorité %1 attend devant elle dans la file et ne peut pas encore démarrer. Le Director ne démarre aucune exécution placée derrière elle dans la file.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1195" />
        <source>from %1, duration unknown</source>
        <translation>à partir de %1, durée inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1205" />
        <source>%1 to about %2, estimated</source>
        <translation>%1 à environ %2, estimé</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1207" />
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1216" />
        <source>and %1 more</source>
        <translation>et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1229" />
        <source>%1: it may run until %2 (%3)</source>
        <translation>%1 : peut s'exécuter jusqu'à %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1230" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1235" />
        <source>Maximum Concurrent Jobs of the Director is not known</source>
        <translation>Le Maximum Concurrent Jobs du Director n'est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1238" />
        <source>the storage of the job is not known (the Job and its JobDefs name none)</source>
        <translation>le Storage du Job n'est pas connu (le Job et ses JobDefs n'en indiquent aucun)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1239" />
        <source>Maximum Concurrent Jobs of storage %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Storage %1 n'est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1241" />
        <source>Maximum Concurrent Jobs of client %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Client %1 n'est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1243" />
        <source>Maximum Concurrent Jobs of job %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Job %1 n'est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1245" />
        <source>Allow Mixed Priority of job %1 is not known</source>
        <translation>L'Allow Mixed Priority du Job %1 n'est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1247" />
        <source>the duration of %1 was not measured at this level</source>
        <translation>la durée de %1 n'a pas été mesurée à ce niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1249" />
        <source>the duration of %1 was never measured</source>
        <translation>la durée de %1 n'a jamais été mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1252" />
        <source>the number of parallel writers of the storage device is not known (the job names no storage; the Director cannot read it)</source>
        <translation>le nombre d'écritures parallèles du périphérique de Storage n'est pas connu (le Job n'indique aucun Storage ; le Director ne peut pas le lire)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1254" />
        <source>the number of parallel writers of the device of storage %1 is not known (the Director cannot read it; set it in the list of predicted waits)</source>
        <translation>le nombre d'écritures parallèles du périphérique du Storage %1 n'est pas connu (le Director ne peut pas le lire ; définissez-le dans la liste des attentes prévues)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1318" />
        <source>Director limit (Maximum Concurrent Jobs = %1)</source>
        <translation>Limite du Director (Maximum Concurrent Jobs = %1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1321" />
        <source>Storage limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Storage (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1325" />
        <source>Client limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Client (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1329" />
        <source>Job limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Job (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1333" />
        <source>Priority %1, runs of priority %2 are running</source>
        <translation>Priorité %1, des exécutions de priorité %2 sont en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1337" />
        <source>Queue order: a run of priority %1 waits ahead</source>
        <translation>Ordre de la file : une exécution de priorité %1 attend devant</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1344" />
        <source>depends on values that are not known</source>
        <translation>dépend de valeurs inconnues</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1347" />
        <source>May wait: %1</source>
        <translation>Peut attendre : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1349" />
        <source>may be longer: %1</source>
        <translation>peut être plus longue : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1371" />
        <source>%1 to %2: %3</source>
        <translation>%1 à %2 : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1377" />
        <source>Waits for: %1</source>
        <translation>Attend : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1382" />
        <source>Predicted start: %1, %2 after the scheduled time (%3)</source>
        <translation>Début prévu : %1, %2 après l'heure planifiée (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1391" />
        <source>it depends on values that are not known</source>
        <translation>cela dépend de valeurs inconnues</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1394" />
        <source>Uncertain, the wait may be longer: %1.</source>
        <translation>Incertain, l'attente peut être plus longue : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1396" />
        <source>Uncertain, this run may have to wait: %1.</source>
        <translation>Incertain, cette exécution pourrait devoir attendre : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1400" />
        <source>If so: %1</source>
        <translation>Si c'est le cas : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1423" />
        <source>Inside the run: waits %1 in the storage daemon for a device of storage %2 (%3 parallel writer(s); after two waits for a release it tries again every %4 s)%5.</source>
        <translation>Au sein de l'exécution : attend %1 dans le Storage Daemon un périphérique du Storage %2 (%3 écriture(s) parallèle(s) ; après deux attentes de libération, il réessaie toutes les %4 s)%5.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1430" />
        <source>, held by %1</source>
        <translation>, tenu par %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1433" />
        <source>Predicted end: %1</source>
        <translation>Fin prévue : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1436" />
        <source>Start overhead %1 (from the Director starting the job to its recorded start: connections, Client Run Before Job)%2.</source>
        <translation>Surcoût de démarrage %1 (du démarrage du Job par le Director à son début enregistré : connexions, Client Run Before Job)%2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1445" />
        <source>Measured wait at this level: %1; predicted %2.</source>
        <translation>Attente mesurée à ce niveau : %1 ; prévue %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1472" />
        <source>not measured</source>
        <translation>non mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1473" />
        <source>median %1, max %2 (%3 run(s))</source>
        <translation>médiane %1, max %2 (%3 exécution(s))</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1484" />
        <source>Prediction too low: measured median %1, predicted %2 - a limit the GUI does not know, e.g. the storage device (parallel writers), or a Client Run Before Job that takes long.</source>
        <translation>Prévision trop basse : médiane mesurée %1, prévue %2 - une limite que l'interface ne connaît pas, p. ex. le périphérique de Storage (écritures parallèles), ou un Client Run Before Job qui dure longtemps.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1489" />
        <source>Prediction too high: measured median %1, predicted %2 - a limit is set lower here than it is, or the durations used are too long.</source>
        <translation>Prévision trop haute : médiane mesurée %1, prévue %2 - une limite est définie ici plus basse qu'elle ne l'est, ou les durées utilisées sont trop longues.</translation>
    </message>
</context>
<context>
    <name>BScheduleWidget</name>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="125" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="860" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3301" />
        <source>Jobs:</source>
        <translation>Jobs :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="134" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="208" />
        <source>Timeline</source>
        <translation>Chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="209" />
        <source>Grid</source>
        <translation>Grille</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="210" />
        <source>Measured waits</source>
        <translation>Attentes mesurées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="211" />
        <source>View:</source>
        <translation>Vue :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="217" />
        <source>Day</source>
        <translation>Jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="218" />
        <source>Week</source>
        <translation>Semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="227" />
        <source>Today</source>
        <translation>Aujourd'hui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="229" />
        <source>Show today or the current week</source>
        <translation>Afficher aujourd'hui ou la semaine en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="239" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1042" />
        <source>The date on display; the calendar opens from the arrow</source>
        <translation>La date affichée ; le calendrier s'ouvre depuis la flèche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="252" />
        <source>Snap:</source>
        <translation>Aimanter :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="259" />
        <source>Zoom:</source>
        <translation>Zoom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="264" />
        <source>Zoom level (pixels per hour)</source>
        <translation>Niveau de zoom (pixels par heure)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="302" />
        <source>Legend</source>
        <translation>Légende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="304" />
        <source>What the bars, hourglasses, question marks and diamonds of the timeline mean</source>
        <translation>Signification des barres, sabliers, points d'interrogation et losanges de la chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="325" />
        <source>Schedules:</source>
        <translation>Schedules :</translation>
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
        <translation>Type</translation>
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
        <translation>Durée médiane</translation>
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
        <translation>%1 Schedules chargés</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="758" />
        <source>%1 schedules loaded (%2 Run lines)</source>
        <translation>%1 Schedules chargés (%2 lignes Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="791" />
        <source>Refreshing schedule list...</source>
        <translation>Actualisation de la liste des Schedules...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="865" />
        <source>Jobs (%1):</source>
        <translation>Jobs (%1) :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="867" />
        <source>Jobs (%1 Schedules):</source>
        <translation>Jobs (%1 Schedules) :</translation>
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
        <translation>Médiane de %1 exécutions mesurées à tous les niveaux au cours des %2 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="908" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1371" />
        <source>No history</source>
        <translation>Aucun historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1041" />
        <source>The week on display: pick any day of it; the calendar opens from the arrow</source>
        <translation>La semaine affichée : choisissez n'importe quel jour de celle-ci ; le calendrier s'ouvre par la flèche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051" />
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052" />
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="440" />
        <source>Measured history</source>
        <translation>Historique mesuré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="451" />
        <source>Close the area below the timeline</source>
        <translation>Fermer la zone sous la frise chronologique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1074" />
        <source>Click a run in the timeline: its measured history appears here.</source>
        <translation>Cliquez sur une exécution dans la frise chronologique : son historique mesuré s'affiche ici.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1106" />
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1144" />
        <source>25% - 75%</source>
        <translation>25 % - 75 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1145" />
        <source>90%</source>
        <translation>90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1152" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857" />
        <source>Runs</source>
        <translation>Exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1158" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>Median wait</source>
        <translation>Attente médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1159" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>90% wait</source>
        <translation>Attente à 90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Wait</source>
        <translation>Attente</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1311" />
        <source>no job uses this schedule</source>
        <translation>aucun Job n'utilise ce Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1315" />
        <source>no successful run of this job in the last %1 days</source>
        <translation>aucune exécution réussie de ce Job au cours des %1 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1318" />
        <source>No history: %1.</source>
        <translation>Aucun historique : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1330" />
        <source>Estimate: no run at this level in the last %1 days. The figures are from all levels of the job.</source>
        <translation>Estimation : aucune exécution à ce niveau au cours des %1 derniers jours. Les chiffres proviennent de tous les niveaux du Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1346" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1347" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1366" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2195" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2902" />
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1400" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1406" />
        <source>1 hour</source>
        <translation>1 heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1401" />
        <source>2 hours</source>
        <translation>2 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1402" />
        <source>4 hours</source>
        <translation>4 heures</translation>
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
        <translation>Ligne Run %1 "%2" : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1488" />
        <source>Not understood and not shown in the views:</source>
        <translation>Non comprises et non affichées dans les vues :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1490" />
        <source>This schedule cannot be written by the application.</source>
        <translation>Ce Schedule ne peut pas être écrit par l'application.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="1496" />
        <source>%n Run line(s) not understood</source>
        <translation>
            <numerusform>%n ligne Run non comprise</numerusform>
            <numerusform>%n lignes Run non comprises</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1498" />
        <source>These Run lines are not shown in the views, and their schedules cannot be written by the application:</source>
        <translation>Ces lignes Run ne sont pas affichées dans les vues, et leurs Schedules ne peuvent pas être écrits par l'application :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1608" />
        <source>Schedule %1 was not changed: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird), which are not available on this connection. %2 stays at %3 (dropped at %4).</source>
        <translation>Le Schedule %1 n'a pas été modifié : la modification d'un Schedule nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (onesimus-dird), qui ne sont pas disponibles sur cette connexion. %2 reste à %3 (déposé à %4).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1635" />
        <source>Schedule %1 was not changed: another change is being written.</source>
        <translation>Le Schedule %1 n'a pas été modifié : une autre modification est en cours d'écriture.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1649" />
        <source>Run line %1 is not understood, so the schedule is locked.</source>
        <translation>La ligne Run %1 n'est pas comprise, le Schedule est donc verrouillé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1652" />
        <source>the dragged Run line runs at several hours of the day; change it in the grid.</source>
        <translation>la ligne Run déplacée s'exécute à plusieurs heures de la journée ; modifiez-la dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1656" />
        <source>the dragged run could not be matched to exactly one Run line; refresh and try again, or change it in the grid.</source>
        <translation>l'exécution déplacée n'a pas pu être associée à exactement une ligne Run ; actualisez et réessayez, ou modifiez-la dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1660" />
        <source>its Run lines are not loaded yet.</source>
        <translation>ses lignes Run ne sont pas encore chargées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1663" />
        <source>%1 is not a time of day.</source>
        <translation>%1 n'est pas une heure de la journée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1667" />
        <source>the new Run line did not read back as intended (%1).</source>
        <translation>la nouvelle ligne Run ne s'est pas relue comme prévu (%1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1670" />
        <source>Schedule %1 was not changed: %2</source>
        <translation>Le Schedule %1 n'a pas été modifié : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1675" />
        <source>Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). Only this line is written anew; the other lines stay as they are in the file.</source>
        <translation>La ligne Run %1 du Schedule %2 passe de %3 à %4 (déplacée dans la chronologie : %5). Seule cette ligne est réécrite ; les autres lignes restent telles qu'elles sont dans le fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1680" />
        <source>The Director checks the change of schedule %1...</source>
        <translation>Le Director vérifie la modification du Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1726" />
        <source>Schedule %1 is locked: it contains a Run line that is not understood.</source>
        <translation>Le Schedule %1 est verrouillé : il contient une ligne Run qui n'est pas comprise.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1732" />
        <source>Schedule %1 was not written: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director. Your changes are kept in the grid.</source>
        <translation>Le Schedule %1 n'a pas été écrit : la modification d'un Schedule nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director. Vos modifications sont conservées dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1738" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="1768" />
        <source>Another change is being written.</source>
        <translation>Une autre modification est en cours d'écriture.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1744" />
        <source>Schedule %1 cannot be written: %2</source>
        <translation>Le Schedule %1 ne peut pas être écrit : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1747" />
        <source>The Director checks the changes of schedule %1...</source>
        <translation>Le Director vérifie les modifications du Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1750" />
        <source>The changes made in the grid. Lines that were not touched stay as they are in the file.</source>
        <translation>Les modifications faites dans la grille. Les lignes non touchées restent telles qu'elles sont dans le fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1762" />
        <source>Schedule %1 was not deleted: deleting a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Le Schedule %1 n'a pas été supprimé : la suppression d'un Schedule nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1771" />
        <source>The Director checks whether schedule %1 can be deleted...</source>
        <translation>Le Director vérifie si le Schedule %1 peut être supprimé...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1786" />
        <source>The Director reloaded its configuration.</source>
        <translation>Le Director a rechargé sa configuration.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1788" />
        <source>Follow-up commands: %1</source>
        <translation>Commandes complémentaires : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1847" />
        <source>Refresh history</source>
        <translation>Actualiser l'historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1850" />
        <source>Ask the Director again for the jobs of the last days (llist jobs)</source>
        <translation>Redemander au Director les Jobs des derniers jours (llist jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1858" />
        <source>90% duration</source>
        <translation>Durée à 90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859" />
        <source>Last run</source>
        <translation>Dernière exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1861" />
        <source>Runs in the window that terminated normally or with warnings</source>
        <translation>Exécutions de la période terminées normalement ou avec des avertissements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1863" />
        <source>Wait: start of the job minus its scheduled time</source>
        <translation>Attente : début du Job moins son heure planifiée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1865" />
        <source>Nine of ten runs waited no longer than this</source>
        <translation>Neuf exécutions sur dix n'ont pas attendu plus longtemps que cela</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1981" />
        <source>the request got no reply (%1)</source>
        <translation>la requête n'a reçu aucune réponse (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2011" />
        <source>the Director refused the request (%1)</source>
        <translation>le Director a refusé la requête (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2012" />
        <source>no reason given</source>
        <translation>aucune raison indiquée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2015" />
        <source>the Director's reply could not be read (%1)</source>
        <translation>la réponse du Director n'a pas pu être lue (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2039" />
        <source>Schedules: no job history, %1</source>
        <translation>Schedules : aucun historique des Jobs, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2073" />
        <source>the job history is being loaded</source>
        <translation>l'historique des Jobs est en cours de chargement</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2077" />
        <source>the job history has not been requested yet</source>
        <translation>l'historique des Jobs n'a pas encore été demandé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2108" />
        <source>Not connected.</source>
        <translation>Non connecté.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2110" />
        <source>The job history has not been requested yet.</source>
        <translation>L'historique des Jobs n'a pas encore été demandé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2112" />
        <source>No job history: %1.</source>
        <translation>Aucun historique des Jobs : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2113" />
        <source>The schedule views work without it: runs have no duration and waits are unknown.</source>
        <translation>Les vues des Schedules fonctionnent sans lui : les exécutions n'ont pas de durée et les attentes sont inconnues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2117" />
        <source>Requesting the jobs of the last %1 days from the Director...</source>
        <translation>Demande au Director des Jobs des %1 derniers jours...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2123" />
        <source>Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs terminated normally or with warnings and were measured.</source>
        <translation>Jobs planifiés au cours des %1 derniers jours, récupérés %2 : %3 lignes, dont %4 exécutions terminées normalement ou avec des avertissements ont été mesurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2132" />
        <source>The row limit (%1) was reached and this Director returned the oldest rows: the newest runs of the window are missing.</source>
        <translation>La limite de lignes (%1) a été atteinte et ce Director a renvoyé les lignes les plus anciennes : les exécutions les plus récentes de la période manquent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2134" />
        <source>The row limit (%1) was reached: the oldest runs of the window are missing.</source>
        <translation>La limite de lignes (%1) a été atteinte : les exécutions les plus anciennes de la période manquent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2138" />
        <source>This Director did not deliver the scheduled time: all waits are unknown.</source>
        <translation>Ce Director n'a pas fourni l'heure planifiée : toutes les attentes sont inconnues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2140" />
        <source>%1 runs came without a scheduled time: their wait is unknown.</source>
        <translation>%1 exécutions sont arrivées sans heure planifiée : leur attente est inconnue.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2144" />
        <source>%1 rows were left out because they start before their scheduled time: copied, migrated and virtual full jobs carry the times of another job.</source>
        <translation>%1 lignes ont été ignorées car elles démarrent avant leur heure planifiée : les Jobs Copy, Migrate et VirtualFull portent les heures d'un autre Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2149" />
        <source>%1 rows were left out because their start or end time is missing or the end is before the start.</source>
        <translation>%1 lignes ont été ignorées car leur heure de début ou de fin manque, ou la fin précède le début.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2175" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2703" />
        <source>(none)</source>
        <translation>(aucune)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2184" />
        <source>From %1 of %2 runs; the others came without a scheduled time.</source>
        <translation>Sur la base de %1 exécutions sur %2 ; les autres sont arrivées sans heure planifiée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2193" />
        <source>The Director delivered no scheduled time for these runs.</source>
        <translation>Le Director n'a fourni aucune heure planifiée pour ces exécutions.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2997" />
        <source>Jobs that are scheduled for the same moment and share a limit (a storage, a client, the job slots of the Director, the parallel writers of a device) wait for each other. The proposal starts some of them later, on a quarter hour (23:15, 23:30 ...), so that they wait less on the dates on display and have room to spare. A wait for a higher priority is meant to be and is not counted; a job of a lower priority that follows a moved job moves along, so that it still runs after it. Nothing is changed on the Director.</source>
        <translation>Les Jobs planifiés au même moment qui partagent une limite (un Storage, un Client, les places de Job du Director, les écritures parallèles d'un périphérique) s'attendent les uns les autres. La proposition démarre certains d'entre eux plus tard, au quart d'heure (23:15, 23:30 ...), afin qu'ils attendent moins aux dates affichées et aient de la marge. Une attente due à une priorité plus élevée est voulue et n'est pas comptée ; un Job de priorité inférieure qui suit un Job déplacé est déplacé avec lui, pour continuer à s'exécuter après lui. Rien n'est modifié sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3008" />
        <source>Replays the queue with the jobs moved quarter hour by quarter hour and keeps the smallest moves that wait least</source>
        <translation>Rejoue la file avec les Jobs décalés de quart d'heure en quart d'heure et garde les plus petits décalages qui attendent le moins</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3162" />
        <source>Stays: a Run line of its schedule runs at several hours, is not understood or would move into the next day</source>
        <translation>Reste : une ligne Run de son Schedule s'exécute à plusieurs heures, n'est pas comprise ou passerait au jour suivant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3165" />
        <source>Moves along to run after %1, which has a higher priority</source>
        <translation>Est déplacé pour s'exécuter après %1, qui a une priorité plus élevée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3007" />
        <source>Propose later start times</source>
        <translation>Proposer des heures de début plus tardives</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3016" />
        <source>Apply...</source>
        <translation>Appliquer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Start now</source>
        <translation>Début actuel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Proposed start</source>
        <translation>Début proposé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023" />
        <source>Later by</source>
        <translation>Plus tard de</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Wait now</source>
        <translation>Attente actuelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Wait then</source>
        <translation>Attente ensuite</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024" />
        <source>Note</source>
        <translation>Remarque</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3026" />
        <source>Time held by a limit or waiting for a device, summed over the job's runs on the dates on display</source>
        <translation>Temps retenu par une limite ou en attente d'un périphérique, additionné sur les exécutions du Job aux dates affichées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3028" />
        <source>Proposed later start times</source>
        <translation>Heures de début plus tardives proposées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3048" />
        <source>No proposal yet.</source>
        <translation>Pas encore de proposition.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3063" />
        <source>Creates a schedule of its own for every job that starts later and sets the job's Schedule to it; everything is shown once for confirmation first</source>
        <translation>Crée un Schedule propre pour chaque Job qui démarre plus tard et y règle le Schedule du Job ; tout est d'abord montré une fois pour confirmation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3065" />
        <source>Needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules, read and change jobs), which are not available on this connection</source>
        <translation>Nécessite les méthodes de configuration de l'interface gRPC de l'Onesimus Director (créer des Schedules, lire et modifier des Jobs), qui ne sont pas disponibles sur cette connexion</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3101" />
        <source>Nothing to stagger: the waits are not predicted (the jobs or the history are not loaded yet).</source>
        <translation>Rien à étaler : les attentes ne sont pas prédites (les Jobs ou l'historique ne sont pas encore chargés).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3118" />
        <source>Nothing to stagger: no run on the dates on display waits for a limit or a device.</source>
        <translation>Rien à étaler : aucune exécution aux dates affichées n'attend une limite ou un périphérique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3121" />
        <source>No later start helps: avoidable wait %1. See the notes of the jobs.</source>
        <translation>Aucun début plus tardif n'aide : attente évitable %1. Voir les remarques des Jobs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3124" />
        <source>Avoidable wait on the dates on display: now %1, with the proposal %2. %n job(s) start later.</source>
        <translation>
            <numerusform>Attente évitable aux dates affichées : maintenant %1, avec la proposition %2. %n Job démarre plus tard.</numerusform>
            <numerusform>Attente évitable aux dates affichées : maintenant %1, avec la proposition %2. %n Jobs démarrent plus tard.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3167" />
        <source>A later start does not help</source>
        <translation>Un début plus tardif n'aide pas</translation>
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
        <translation>Appliquer les heures de début plus tardives</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3190" />
        <source>These resources are written to the Director, in this order. The Director checks each one; the first that it refuses stops the rest.</source>
        <translation>Ces ressources sont écrites sur le Director, dans cet ordre. Le Director vérifie chacune ; la première qu'il refuse arrête le reste.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3202" />
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3248" />
        <source>Nothing was written: %1</source>
        <translation>Rien n'a été écrit : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3276" />
        <source>Reading the jobs that start later...</source>
        <translation>Lecture des Jobs qui démarrent plus tard...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3297" />
        <source>schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3298" />
        <source>New schedule %1 for %2:</source>
        <translation>Nouveau Schedule %1 pour %2 :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3307" />
        <source>Nothing was written: the text of job %1 has no closing brace.</source>
        <translation>Rien n'a été écrit : le texte du Job %1 n'a pas d'accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3314" />
        <source>(Schedule from its JobDefs)</source>
        <translation>(Schedule de ses JobDefs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3320" />
        <source>Nothing was written: the later start times were not confirmed.</source>
        <translation>Rien n'a été écrit : les heures de début plus tardives n'ont pas été confirmées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3332" />
        <source>Written: %1. The jobs start at their new times from now on.</source>
        <translation>Écrit : %1. Les Jobs démarrent désormais à leurs nouvelles heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3336" />
        <source>Writing %1...</source>
        <translation>Écriture de %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3342" />
        <source>it was changed on the Director in the meantime</source>
        <translation>il a été modifié sur le Director entre-temps</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3344" />
        <source>the name is taken</source>
        <translation>le nom est déjà pris</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3345" />
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3347" />
        <source>Writing %1 failed: %2. Written before: %3. Nothing after it was sent.</source>
        <translation>L'écriture de %1 a échoué : %2. Écrit avant : %3. Rien n'a été envoyé ensuite.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3363" />
        <source>Nothing was written: job %1 could not be read (%2).</source>
        <translation>Rien n'a été écrit : le Job %1 n'a pas pu être lu (%2).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="430" />
        <source>Predicted waits</source>
        <translation>Attentes prévues</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="444" />
        <source>Stagger</source>
        <translation>Étaler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="450" />
        <source>Close the tabs below the timeline</source>
        <translation>Fermer les onglets sous la chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2288" />
        <source>Reasons shared by several rows</source>
        <translation>Raisons communes à plusieurs lignes</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Scheduled</source>
        <translation>Planifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292" />
        <source>Predicted start</source>
        <translation>Début prévu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Measured</source>
        <translation>Mesuré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Certain</source>
        <translation>Certain</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293" />
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2295" />
        <source>What this job waited at this level in the job history (StartTime minus SchedTime): median / maximum. The catalog's start is set after the connections and Client Run Before Job, so that time is part of it.</source>
        <translation>Ce que ce Job a attendu à ce niveau dans l'historique des Jobs (StartTime moins SchedTime) : médiane / maximum. Le début dans le catalogue est fixé après les connexions et le Client Run Before Job ; ce temps en fait donc partie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2299" />
        <source>Predicted start minus scheduled time: seconds ("27 s"), minutes:seconds ("1:35 min") or, from ten minutes on, hours:minutes ("0:53 h")</source>
        <translation>Début prévu moins heure planifiée : secondes ("27 s"), minutes:secondes ("1:35 min") ou, à partir de dix minutes, heures:minutes ("0:53 h")</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2302" />
        <source>No: the prediction depends on a limit the Director did not report or on a duration that was not measured</source>
        <translation>Non : la prévision dépend d'une limite que le Director n'a pas signalée ou d'une durée qui n'a pas été mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2304" />
        <source>Predicted waits of the dates on display</source>
        <translation>Attentes prévues pour les dates affichées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2306" />
        <source>One row per run that is predicted to wait or whose prediction is uncertain. The reason of the current row is below the table.</source>
        <translation>Une ligne par exécution dont l'attente est prévue ou dont la prévision est incertaine. La raison de la ligne courante figure sous le tableau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2328" />
        <source>Reason of the selected wait</source>
        <translation>Raison de l'attente sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2351" />
        <source>Parallel writers of the devices</source>
        <translation>Écritures parallèles des périphériques</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2353" />
        <source>Notes</source>
        <translation>Remarques</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2355" />
        <source>Basis of the prediction</source>
        <translation>Base de la prévision</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2357" />
        <source>Back-test</source>
        <translation>Test rétrospectif</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2367" />
        <source>Back-test of the prediction</source>
        <translation>Test rétrospectif de la prévision</translation>
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
        <translation>Priorité %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2393" />
        <source>Queue: priority %1 first</source>
        <translation>File : priorité %1 d'abord</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2401" />
        <source>The prediction has changed: propose again.</source>
        <translation>La prédiction a changé : proposez à nouveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2572" />
        <source>Waiting not predicted</source>
        <translation>Aucune attente prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2576" />
        <source>The jobs of the Director are not loaded: it is not known which job runs which schedule.</source>
        <translation>Les Jobs du Director ne sont pas chargés : on ne sait pas quel Job utilise quel Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2579" />
        <source>Predicted for the dates on display from the limits of the Director (Maximum Concurrent Jobs, priorities) and the measured durations.</source>
        <translation>Prévu pour les dates affichées à partir des limites du Director (Maximum Concurrent Jobs, priorités) et des durées mesurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2582" />
        <source>Uncertain: the prediction depends on a limit the Director did not report or on a duration that was not measured.</source>
        <translation>Incertain : la prévision dépend d'une limite que le Director n'a pas signalée ou d'une durée qui n'a pas été mesurée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2586" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="2798" />
        <source>Opens or closes the list of predicted waits below the timeline.</source>
        <translation>Ouvre ou ferme la liste des attentes prévues sous la chronologie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2602" />
        <source>Not predicted: the jobs of the Director are not loaded.</source>
        <translation>Non prévu : les Jobs du Director ne sont pas chargés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2604" />
        <source>%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules from the day before on.</source>
        <translation>%1 exécution(s) affichée(s), rejouée(s) avec les %2 exécution(s) de tous les Schedules activés à partir de la veille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2613" />
        <source>the storage of job %1 (the job names none)</source>
        <translation>le Storage du Job %1 (le Job n'en indique aucun)</translation>
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
        <translation>le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2643" />
        <source>storages: %1</source>
        <translation>Storages : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2645" />
        <source>clients: %1</source>
        <translation>Clients : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2647" />
        <source>Maximum Concurrent Jobs as reported: %1.</source>
        <translation>Maximum Concurrent Jobs signalé : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2649" />
        <source>Maximum Concurrent Jobs not reported by this Director for: %1. Runs it matters for are uncertain.</source>
        <translation>Maximum Concurrent Jobs non signalé par ce Director pour : %1. Les exécutions concernées sont incertaines.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2673" />
        <source>[%1] %2 rows: %3.</source>
        <translation>[%1] %2 lignes : %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2717" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3129" />
        <location filename="../src/schedules/bschedulewidget.cpp" line="3174" />
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2724" />
        <source>not measured</source>
        <translation>non mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726" />
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726" />
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2731" />
        <source>Prediction too low</source>
        <translation>Prévision trop basse</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2732" />
        <source>Prediction too low; %1</source>
        <translation>Prévision trop basse ; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2735" />
        <source>Prediction too high</source>
        <translation>Prévision trop haute</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2736" />
        <source>Prediction too high; %1</source>
        <translation>Prévision trop haute ; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2740" />
        <source>Waits %1 for the device inside the run</source>
        <translation>Attend %1 le périphérique au sein de l'exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2754" />
        <source>No run on display is predicted to wait.</source>
        <translation>Aucune exécution affichée ne devrait attendre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2755" />
        <source>Not predicted.</source>
        <translation>Non prévu.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2795" />
        <source>Predicted waits: %1. The list is open.</source>
        <translation>Attentes prévues : %1. La liste est ouverte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2796" />
        <source>Predicted waits: %1. The list is closed.</source>
        <translation>Attentes prévues : %1. La liste est fermée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2808" />
        <source>Select a row to read its reason in full.</source>
        <translation>Sélectionnez une ligne pour lire sa raison en entier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2816" />
        <source>%1, scheduled %2:</source>
        <translation>%1, planifié %2 :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2897" />
        <source>Storage %1, parallel writers of its device:</source>
        <translation>Storage %1, écritures parallèles de son périphérique :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2904" />
        <source>Parallel writers of the device of storage %1</source>
        <translation>Écritures parallèles du périphérique du Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2905" />
        <source>How many jobs the device(s) of %1 write at a time: Maximum Concurrent Jobs of the Device resource in the storage daemon (several devices: their sum). The Director cannot read it. "unknown": runs that depend on it are uncertain.</source>
        <translation>Combien de Jobs le(s) périphérique(s) de %1 écrivent simultanément : Maximum Concurrent Jobs de la ressource Device dans le Storage Daemon (plusieurs périphériques : leur somme). Le Director ne peut pas le lire. "inconnu" : les exécutions qui en dépendent sont incertaines.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2948" />
        <source>No job history: no suggestion.</source>
        <translation>Aucun historique des Jobs : aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2950" />
        <source>History suggests %1.</source>
        <translation>L'historique suggère %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2952" />
        <source>History gives no suggestion.</source>
        <translation>L'historique ne donne aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958" />
        <source>Use %1</source>
        <translation>Utiliser %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958" />
        <source>Use</source>
        <translation>Utiliser</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2979" />
        <source>Back-test: no job history to replay.</source>
        <translation>Test rétrospectif : aucun historique des Jobs à rejouer.</translation>
    </message>
</context>
<context>
    <name>BScheduleWizard</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="950" />
        <source>Add Schedule</source>
        <translation>Ajouter un Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="1016" />
        <source># This schedule cannot be written:</source>
        <translation># Ce Schedule ne peut pas être écrit :</translation>
    </message>
</context>
<context>
    <name>BSettings</name>
    <message>
        <location filename="../src/config/bsettings.cpp" line="221" />
        <source>Default</source>
        <translation>Par défaut</translation>
    </message>
</context>
<context>
    <name>BSettingsDialog</name>
    <message>
        <location filename="../ui/bsettingsdialog.ui" line="14" />
        <location filename="../src/config/bsettingsdialog.cpp" line="64" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1387" />
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="103" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1400" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1408" />
        <source>Reset</source>
        <translation>Réinitialiser</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="105" />
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="107" />
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="140" />
        <source>🔌 Connection</source>
        <translation>🔌 Connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="144" />
        <source>🎨 Appearance</source>
        <translation>🎨 Apparence</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="148" />
        <source>⚙️ Behavior</source>
        <translation>⚙️ Comportement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="152" />
        <source>🔧 Advanced</source>
        <translation>🔧 Avancé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="181" />
        <source>Connection Manager</source>
        <translation>Gestionnaire de connexions</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="189" />
        <source>Saved Connections</source>
        <translation>Connexions enregistrées</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="203" />
        <source>+ Add</source>
        <translation>+ Ajouter</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="204" />
        <source>Add a new connection profile</source>
        <translation>Ajouter un nouveau profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="205" />
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="207" />
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="209" />
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="239" />
        <source>Profile</source>
        <translation>Profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="245" />
        <source>e.g. Production Server, Test Environment</source>
        <translation>p. ex. Serveur de production, Environnement de test</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="246" />
        <source>Profile Name:</source>
        <translation>Nom du profil :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="251" />
        <source>Director Connection</source>
        <translation>Connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="257" />
        <source>e.g. 192.168.1.100 or bareos-dir.local</source>
        <translation>p. ex. 192.168.1.100 ou bareos-dir.local</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="258" />
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="263" />
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="267" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole de console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="268" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="274" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d'Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="276" />
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="281" />
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Port gRPC du Director (par défaut : port BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="282" />
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="289" />
        <source>Forget certificate</source>
        <translation>Oublier le certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="290" />
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Supprimer le certificat épinglé. La prochaine connexion demandera à nouveau si le certificat du serveur est digne de confiance.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="293" />
        <source>Server certificate:</source>
        <translation>Certificat du serveur :</translation>
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
        <translation>Système d'exploitation du serveur (influe sur les chemins TLS dans les configurations exportées)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="306" />
        <source>Server OS:</source>
        <translation>OS du serveur :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="310" />
        <source>Director Name:</source>
        <translation>Nom du Director :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="314" />
        <source>Console name for authentication (e.g., 'admin' or 'onesimus')</source>
        <translation>Nom de console pour l'authentification (p. ex. 'admin' ou 'onesimus')</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="315" />
        <source>Console Name:</source>
        <translation>Nom de la console :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="320" />
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="327" />
        <location filename="../src/config/bsettingsdialog.cpp" line="333" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1668" />
        <source>(password MD5 hash will appear here)</source>
        <translation>(le hachage MD5 du mot de passe apparaîtra ici)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="328" />
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="368" />
        <source>⚠️ TLS encrypts communication with the Director.
TLS is strongly recommended for production environments!</source>
        <translation>⚠️ TLS chiffre la communication avec le Director.
TLS est fortement recommandé pour les environnements de production !</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="376" />
        <source>Authentication &amp; Encryption</source>
        <translation>Authentification et chiffrement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="382" />
        <source>Authentication Method:</source>
        <translation>Méthode d'authentification :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="388" />
        <source>Use x509 certificates (instead of PSK)</source>
        <translation>Utiliser des certificats x509 (au lieu de PSK)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="390" />
        <source>By default, TLS uses Pre-Shared Key (PSK) authentication.
Enable this to use X.509 certificate-based authentication instead.
Certificate mode requires CA certificate and client certificate/key files.</source>
        <translation>Par défaut, TLS utilise l'authentification par clé pré-partagée (PSK).
Activez cette option pour utiliser plutôt l'authentification par certificat X.509.
Le mode certificat nécessite un certificat CA ainsi que des fichiers de certificat/clé client.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="408" />
        <source>Path to CA certificate (.pem)</source>
        <translation>Chemin du certificat CA (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="409" />
        <location filename="../src/config/bsettingsdialog.cpp" line="419" />
        <location filename="../src/config/bsettingsdialog.cpp" line="429" />
        <location filename="../src/config/bsettingsdialog.cpp" line="440" />
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="414" />
        <source>CA Certificate:</source>
        <translation>Certificat CA :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="418" />
        <source>Path to client certificate (.pem)</source>
        <translation>Chemin du certificat client (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="424" />
        <source>Client Certificate:</source>
        <translation>Certificat client :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="428" />
        <source>Path to private key (.pem, .key)</source>
        <translation>Chemin de la clé privée (.pem, .key)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="434" />
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="439" />
        <source>Path to client certificate (.pfx)</source>
        <translation>Chemin du certificat client (.pfx)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="445" />
        <source>PFX Certificate:</source>
        <translation>Certificat PFX :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="448" />
        <source>Convert PEM/DER to PFX...</source>
        <translation>Convertir PEM/DER en PFX...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="449" />
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="454" />
        <source>Verify server certificate (recommended)</source>
        <translation>Vérifier le certificat du serveur (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="463" />
        <source>TLS Cipher List (optional):</source>
        <translation>Liste de chiffrements TLS (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="469" />
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>p. ex. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="470" />
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>Liste de chiffrements TLS-PSK séparés par des deux-points.
Laisser vide pour la détection automatique.
Exemple : PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="475" />
        <source>Preset</source>
        <translation>Préréglage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="476" />
        <source>Fill with recommended PSK ciphers</source>
        <translation>Remplir avec les chiffrements PSK recommandés</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="487" />
        <source>gRPC is always encrypted with TLS. The server certificate is confirmed on first use and pinned; the Director authenticates with console name and password. TLS-PSK and certificate settings do not apply.</source>
        <translation>gRPC est toujours chiffré avec TLS. Le certificat du serveur est confirmé lors de la première utilisation puis épinglé ; le Director s'authentifie avec le nom de console et le mot de passe. Les paramètres TLS-PSK et de certificat ne s'appliquent pas.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="498" />
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="503" />
        <source>Save password in profile</source>
        <translation>Enregistrer le mot de passe dans le profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="507" />
        <source>Auto-connect on startup</source>
        <translation>Connexion automatique au démarrage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="511" />
        <source>Connection timeout:</source>
        <translation>Délai de connexion :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="515" />
        <location filename="../src/config/bsettingsdialog.cpp" line="769" />
        <source> seconds</source>
        <translation> secondes</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="526" />
        <source>Save Profile</source>
        <translation>Enregistrer le profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="528" />
        <source>Save changes to this connection profile</source>
        <translation>Enregistrer les modifications de ce profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="540" />
        <source>Export All Profiles</source>
        <translation>Exporter tous les profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="541" />
        <source>Import Profiles</source>
        <translation>Importer des profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="580" />
        <source>Appearance</source>
        <translation>Apparence</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="586" />
        <source>Text Size</source>
        <translation>Taille du texte</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="595" />
        <source>The text follows the font of your desktop. This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0).</source>
        <translation>Le texte suit la police de votre bureau. Ce facteur l'agrandit ou le réduit (Ctrl++, Ctrl+-, Ctrl+0).</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="597" />
        <source>Interface zoom:</source>
        <translation>Zoom de l'interface :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="602" />
        <source>User Interface</source>
        <translation>Interface utilisateur</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="606" />
        <source>Enable animations</source>
        <translation>Activer les animations</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="610" />
        <source>Compact mode</source>
        <translation>Mode compact</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="616" />
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="624" />
        <source>System (%1)</source>
        <translation>Système (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="636" />
        <source>Application Language:</source>
        <translation>Langue de l'application :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="639" />
        <source>Language changes require an application restart. "System" follows the language of the operating system.</source>
        <translation>Les changements de langue nécessitent un redémarrage de l'application. « Système » suit la langue du système d'exploitation.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="648" />
        <source>Status Bar Colors</source>
        <translation>Couleurs de la barre d'état</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="657" />
        <source>Color shown when connected to Director</source>
        <translation>Couleur affichée lorsque la connexion au Director est établie</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="659" />
        <source>Connected:</source>
        <translation>Connecté :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="665" />
        <source>Color shown when not connected</source>
        <translation>Couleur affichée en l'absence de connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="667" />
        <source>Disconnected:</source>
        <translation>Déconnecté :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="672" />
        <source>Backup Level Colors</source>
        <translation>Couleurs des niveaux de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="736" />
        <source>Behavior</source>
        <translation>Comportement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="742" />
        <source>Confirmations</source>
        <translation>Confirmations</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="746" />
        <source>Confirm job cancellation</source>
        <translation>Confirmer l'annulation d'un Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="750" />
        <source>Confirm job start</source>
        <translation>Confirmer le démarrage d'un Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="757" />
        <source>Auto-Refresh</source>
        <translation>Actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="761" />
        <source>Auto-refresh</source>
        <translation>Actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="765" />
        <source>Interval:</source>
        <translation>Intervalle :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="778" />
        <source>Display</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="786" />
        <source>Maximum Jobs:</source>
        <translation>Nombre maximal de Jobs :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="788" />
        <source>Show newest jobs first</source>
        <translation>Afficher les Jobs les plus récents en premier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="791" />
        <source>When enabled, the job list shows the most recent jobs first.
When disabled, the oldest jobs are shown first.</source>
        <translation>Si activé, la liste des Jobs affiche les Jobs les plus récents en premier.
Si désactivé, les Jobs les plus anciens sont affichés en premier.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="798" />
        <source> hours</source>
        <translation> heures</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="800" />
        <source>The clients table marks a client as stale when its last backup or contact
is at least this old. The value is taken from the job history; it is not
a live connection check.</source>
        <translation>Le tableau des Clients marque un Client comme obsolète lorsque sa dernière sauvegarde ou son dernier contact
est au moins aussi ancien. La valeur est tirée de l'historique des Jobs ; il ne s'agit pas
d'une vérification de connexion en direct.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="803" />
        <source>Client backup is stale after:</source>
        <translation>Sauvegarde du Client obsolète après :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="808" />
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="815" />
        <source> days</source>
        <translation> jours</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="817" />
        <source>The schedule view measures how long jobs run and how long they wait
from the jobs of the last days in the catalog. A longer time gives more
runs per job and a larger request to the Director. The history is only
requested while the schedule view is open.</source>
        <translation>La vue des Schedules mesure la durée d'exécution et d'attente des Jobs
à partir des Jobs des derniers jours dans le catalogue. Une période plus longue donne plus
d'exécutions par Job et une requête plus volumineuse au Director. L'historique n'est
demandé que lorsque la vue des Schedules est ouverte.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="821" />
        <source>Measure job history over the last:</source>
        <translation>Mesurer l'historique des Jobs sur les derniers :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="826" />
        <source>Visible Backup Levels</source>
        <translation>Niveaux de sauvegarde visibles</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="831" />
        <source>Select which backup levels appear in the Run Backup dialog:</source>
        <translation>Sélectionnez les niveaux de sauvegarde qui apparaissent dans la boîte de dialogue Lancer une sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="837" />
        <source>Complete backup of all files</source>
        <translation>Sauvegarde complète de tous les fichiers</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="838" />
        <source>Backup files changed since last backup</source>
        <translation>Sauvegarder les fichiers modifiés depuis la dernière sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="839" />
        <source>Backup files changed since last Full backup</source>
        <translation>Sauvegarder les fichiers modifiés depuis la dernière sauvegarde Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="840" />
        <source>Consolidate incremental backups into a synthetic full</source>
        <translation>Consolider les sauvegardes Incremental en une Full synthétique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="841" />
        <source>Data-level backup (rarely used)</source>
        <translation>Sauvegarde au niveau des données (rarement utilisée)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="889" />
        <source>Advanced Settings</source>
        <translation>Paramètres avancés</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="896" />
        <source>File Logging</source>
        <translation>Journalisation dans un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="900" />
        <source>Enable file logging</source>
        <translation>Activer la journalisation dans un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="901" />
        <source>Write debug information to a log file</source>
        <translation>Écrire les informations de débogage dans un fichier journal</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="905" />
        <source>Log File:</source>
        <translation>Fichier journal :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="915" />
        <source>Max. Log Size:</source>
        <translation>Taille max. du journal :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="939" />
        <source>Miscellaneous</source>
        <translation>Divers</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="943" />
        <source>Show tooltips</source>
        <translation>Afficher les infobulles</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="951" />
        <source>⚠️ These settings are for advanced users.
Changes may affect application stability.</source>
        <translation>⚠️ Ces paramètres sont destinés aux utilisateurs avancés.
Les modifications peuvent affecter la stabilité de l'application.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1388" />
        <source>Settings have been saved.
Some changes require an application restart.</source>
        <translation>Les paramètres ont été enregistrés.
Certaines modifications nécessitent un redémarrage de l'application.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1401" />
        <source>Do you really want to reset all settings to default values?
This action cannot be undone.</source>
        <translation>Voulez-vous vraiment réinitialiser tous les paramètres à leurs valeurs par défaut ?
Cette action est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1409" />
        <source>All settings have been reset to default values.</source>
        <translation>Tous les paramètres ont été réinitialisés à leurs valeurs par défaut.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1415" />
        <source>Select CA Certificate</source>
        <translation>Sélectionner le certificat CA</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1416" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1428" />
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1424" />
        <source>Select Client Certificate</source>
        <translation>Sélectionner le certificat client</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1426" />
        <source>Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)</source>
        <translation>Certificats (*.pfx *.pem *.crt *.cert *.der);;Fichiers PFX (*.pfx);;Fichiers PEM (*.pem);;Fichiers DER (*.der);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1437" />
        <source>Select Private Key</source>
        <translation>Sélectionner la clé privée</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1438" />
        <source>Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1446" />
        <source>Export Settings</source>
        <translation>Exporter les paramètres</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1462" />
        <source>Export</source>
        <translation>Exporter</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1462" />
        <source>Settings exported successfully.</source>
        <translation>Paramètres exportés avec succès.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1469" />
        <source>Import Settings</source>
        <translation>Importer les paramètres</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1484" />
        <source>Import</source>
        <translation>Importer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1484" />
        <source>Settings imported successfully.</source>
        <translation>Paramètres importés avec succès.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1493" />
        <source>No Profiles</source>
        <translation>Aucun profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1494" />
        <source>There are no connection profiles to delete.</source>
        <translation>Il n'y a aucun profil de connexion à supprimer.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1498" />
        <source>Clear All Profiles</source>
        <translation>Effacer tous les profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1499" />
        <source>Do you really want to delete all %1 connection profile(s)?

This includes all passwords and certificate paths.
This action cannot be undone.</source>
        <translation>Voulez-vous vraiment supprimer les %1 profil(s) de connexion ?

Cela inclut tous les mots de passe et chemins de certificats.
Cette action est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1518" />
        <source>Deleted</source>
        <translation>Supprimé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1519" />
        <source>All connection profiles have been deleted.</source>
        <translation>Tous les profils de connexion ont été supprimés.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1526" />
        <source>Choose Color for Full Backup</source>
        <translation>Choisir la couleur des sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1540" />
        <source>Choose Color for Incremental Backup</source>
        <translation>Choisir la couleur des sauvegardes Incremental</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1554" />
        <source>Choose Color for Differential Backup</source>
        <translation>Choisir la couleur des sauvegardes Differential</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1568" />
        <source>Choose Color for Virtual Full Backup</source>
        <translation>Choisir la couleur des sauvegardes VirtualFull</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1582" />
        <source>Choose Color for Connected Status</source>
        <translation>Choisir la couleur de l'état connecté</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1596" />
        <source>Choose Color for Disconnected Status</source>
        <translation>Choisir la couleur de l'état déconnecté</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1660" />
        <source>••••••••  (password saved)</source>
        <translation>••••••••  (mot de passe enregistré)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1666" />
        <source>Enter password</source>
        <translation>Saisir le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1708" />
        <source>New Connection</source>
        <translation>Nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1746" />
        <source>Delete Profile</source>
        <translation>Supprimer le profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1747" />
        <source>Do you really want to delete the connection profile "%1"?

This action cannot be undone.</source>
        <translation>Voulez-vous vraiment supprimer le profil de connexion "%1" ?

Cette action est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1782" />
        <source>%1 (Copy)</source>
        <translation>%1 (copie)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1806" />
        <location filename="../src/config/bsettingsdialog.cpp" line="1896" />
        <source>Unnamed Connection</source>
        <translation>Connexion sans nom</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1880" />
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(non épinglé - confirmé à la prochaine connexion)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1885" />
        <source>SHA-256: %1</source>
        <translation>SHA-256 : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1897" />
        <source>Profile Saved</source>
        <translation>Profil enregistré</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1898" />
        <source>Connection profile "%1" has been saved.</source>
        <translation>Le profil de connexion "%1" a été enregistré.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1912" />
        <source>PFX File Created</source>
        <translation>Fichier PFX créé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1913" />
        <source>PFX file has been created successfully.

The certificate path has been updated in the current profile.</source>
        <translation>Le fichier PFX a été créé avec succès.

Le chemin du certificat a été mis à jour dans le profil actuel.</translation>
    </message>
</context>
<context>
    <name>BStaggerPlan</name>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="32" />
        <source>Schedule %1 has no Run lines.</source>
        <translation>Le Schedule %1 n'a pas de lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="36" />
        <source>Run line %1 of schedule %2 is not understood.</source>
        <translation>La ligne Run %1 du Schedule %2 n'est pas comprise.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="39" />
        <source>Run line %1 of schedule %2 runs at several hours.</source>
        <translation>La ligne Run %1 du Schedule %2 s'exécute à plusieurs heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="42" />
        <source>Run line %1 of schedule %2 would move into the next day.</source>
        <translation>La ligne Run %1 du Schedule %2 passerait au jour suivant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="52" />
        <source>Run line %1 of schedule %2 did not read back as written (%3).</source>
        <translation>La ligne Run %1 du Schedule %2 ne s'est pas relue comme écrite (%3).</translation>
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
        <translation>Terminé normalement</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26" />
        <source>Warnings</source>
        <translation>Avertissements</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26" />
        <source>Terminated with warnings</source>
        <translation>Terminé avec des avertissements</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27" />
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27" />
        <source>Terminated in error</source>
        <translation>Terminé en erreur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28" />
        <source>Errors</source>
        <translation>Erreurs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28" />
        <source>Non-fatal error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29" />
        <source>Fatal</source>
        <translation>Fatal</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29" />
        <source>Fatal error</source>
        <translation>Erreur fatale</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30" />
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30" />
        <source>Canceled by user</source>
        <translation>Annulé par l'utilisateur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="31" />
        <source>Running</source>
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32" />
        <source>Committing</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32" />
        <source>Committing data (last despool)</source>
        <translation>Validation des données (dernier despool)</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33" />
        <source>Despooling</source>
        <translation>Despool</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33" />
        <source>Despooling data</source>
        <translation>Despool des données</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34" />
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34" />
        <source>Created, not yet running</source>
        <translation>Créé, pas encore démarré</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35" />
        <source>Wait FD</source>
        <translation>Attente FD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35" />
        <source>Waiting on the File daemon</source>
        <translation>En attente du File Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36" />
        <source>Wait SD</source>
        <translation>Attente SD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36" />
        <source>Waiting on the Storage daemon</source>
        <translation>En attente du Storage Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37" />
        <source>Wait media</source>
        <translation>Attente média</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37" />
        <source>Waiting for new media</source>
        <translation>En attente d'un nouveau média</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38" />
        <source>Wait mount</source>
        <translation>Attente montage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38" />
        <source>Waiting for mount</source>
        <translation>En attente de montage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39" />
        <source>Wait storage</source>
        <translation>Attente Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39" />
        <source>Waiting for a storage resource</source>
        <translation>En attente d'une ressource Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40" />
        <source>Wait job</source>
        <translation>Attente Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40" />
        <source>Waiting for a job resource</source>
        <translation>En attente d'une ressource Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41" />
        <source>Wait client</source>
        <translation>Attente Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41" />
        <source>Waiting for a client resource</source>
        <translation>En attente d'une ressource Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42" />
        <source>Wait max jobs</source>
        <translation>Attente max. Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42" />
        <source>Waiting for maximum jobs</source>
        <translation>En attente du nombre maximal de Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43" />
        <source>Wait start</source>
        <translation>Attente début</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43" />
        <source>Waiting for the start time</source>
        <translation>En attente de l'heure de début</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44" />
        <source>Wait priority</source>
        <translation>Attente priorité</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44" />
        <source>Waiting for higher priority jobs to finish</source>
        <translation>En attente de la fin des Jobs de priorité supérieure</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45" />
        <source>Wait device</source>
        <translation>Attente périphérique</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45" />
        <source>Queued, waiting for a device</source>
        <translation>En file d'attente, en attente d'un périphérique</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46" />
        <source>Despool attr.</source>
        <translation>Despool attr.</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46" />
        <source>Storage daemon despooling attributes</source>
        <translation>Storage Daemon en cours de despool des attributs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47" />
        <source>Inserting</source>
        <translation>Insertion</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47" />
        <source>Doing batch insert of file records</source>
        <translation>Insertion par lots des enregistrements de fichiers</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48" />
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49" />
        <source>Differences</source>
        <translation>Différences</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49" />
        <source>Verify found differences</source>
        <translation>Verify a trouvé des différences</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50" />
        <source>Incomplete</source>
        <translation>Incomplet</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50" />
        <source>Incomplete job</source>
        <translation>Job incomplet</translation>
    </message>
</context>
<context>
    <name>BStorageResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319" />
        <source>Name</source>
        <translation>Nom</translation>
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
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="215" />
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BWaitCalibration</name>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="190" />
        <source>this job was not measured: median of all %1 runs on this Director</source>
        <translation>ce Job n'a pas été mesuré : médiane de toutes les %1 exécutions sur ce Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="195" />
        <source>nothing measured: %1 assumed</source>
        <translation>rien de mesuré : %1 supposé</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="203" />
        <source>Duration %1: median of %2 measured run(s). It includes any wait for a device inside the storage daemon (the parallel writers of %3 are not known, so it is not taken out).</source>
        <translation>Durée %1 : médiane de %2 exécution(s) mesurée(s). Elle inclut toute attente d'un périphérique dans le Storage Daemon (les écritures parallèles de %3 ne sont pas connues, elle n'est donc pas retranchée).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="207" />
        <source>its storage</source>
        <translation>son Storage</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="186" />
        <source>median of %n measured run(s) at this level</source>
        <translation>
            <numerusform>médiane de %n exécution mesurée à ce niveau</numerusform>
            <numerusform>médiane de %n exécutions mesurées à ce niveau</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="188" />
        <source>median of %n measured run(s) of this job at other levels</source>
        <translation>
            <numerusform>médiane de %n exécution mesurée de ce Job à d'autres niveaux</numerusform>
            <numerusform>médiane de %n exécutions mesurées de ce Job à d'autres niveaux</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="210" />
        <source>Duration %1: median of %n measured run(s); they did not wait for the device.</source>
        <translation>
            <numerusform>Durée %1 : médiane de %n exécution mesurée ; elle n'a pas attendu le périphérique.</numerusform>
            <numerusform>Durée %1 : médiane de %n exécutions mesurées ; elles n'ont pas attendu le périphérique.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="214" />
        <source>Duration %1 without the wait for the device (measured %2, median of %3 run(s)): counted from the end of the run that held the device before it (%4 parallel writer(s)). It may still hold up to 30 s of the storage daemon's tries.</source>
        <translation>Durée %1 sans l'attente du périphérique (mesurée %2, médiane de %3 exécution(s)) : comptée à partir de la fin de l'exécution qui occupait le périphérique avant elle (%4 écriture(s) parallèle(s)). Elle peut encore contenir jusqu'à 30 s de tentatives du Storage Daemon.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="310" />
        <source>%1 ended %2 after %3 on %4; alone it takes at least %5</source>
        <translation>%1 s'est terminé %2 après %3 le %4 ; seul, il dure au moins %5</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="327" />
        <source>%1 night(s) with runs on storage %2; on %3 more than one of them was queued or running at the same time; on %4 the runs' recorded start to end overlapped.</source>
        <translation>%1 nuit(s) avec des exécutions sur le Storage %2 ; pendant %3, plus d'une d'entre elles était en file d'attente ou en cours en même temps ; pendant %4, le début et la fin enregistrés des exécutions se chevauchaient.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="334" />
        <source>No two runs ever overlapped although several were queued: the storage wrote one job at a time.</source>
        <translation>Aucune paire d'exécutions ne s'est jamais chevauchée bien que plusieurs aient été en file d'attente : le Storage a écrit un Job à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="340" />
        <source>Overlapping runs do not prove parallel writing: with Just In Time Reservation a job waits for the device inside its run.</source>
        <translation>Des exécutions qui se chevauchent ne prouvent pas une écriture parallèle : avec Just In Time Reservation, un Job attend le périphérique au sein de son exécution.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="344" />
        <source>%1 of %2 telling pair(s) ended sooner after each other than one writer allows (e.g. %3): it writes more than one job at a time.</source>
        <translation>%1 sur %2 paire(s) significative(s) se sont terminées plus rapprochées qu'une seule écriture ne le permet (p. ex. %3) : il écrit plus d'un Job à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="351" />
        <source>In all %1 telling pair(s) on %2 night(s) the later run went on after the other ended for at least as long as it takes alone (e.g. %3): consistent with one writer at a time.</source>
        <translation>Dans toutes les %1 paire(s) significative(s) sur %2 nuit(s), l'exécution ultérieure a continué après la fin de l'autre au moins aussi longtemps qu'elle dure seule (p. ex. %3) : compatible avec une écriture à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="358" />
        <source>Not enough telling pairs (%1; a pair tells when the later run started well before the other ended and was also seen running alone): no suggestion.</source>
        <translation>Pas assez de paires significatives (%1 ; une paire est significative lorsque l'exécution ultérieure a démarré bien avant la fin de l'autre et a aussi été vue s'exécutant seule) : aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="446" />
        <source>Back-test: no measured night to replay.</source>
        <translation>Test rétrospectif : aucune nuit mesurée à rejouer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="447" />
        <source>Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 (%5, %6; %7).</source>
        <translation>Test rétrospectif, %1 dernière(s) nuit(s) (%2 exécutions) : erreur médiane %3, pire %4 (%5, %6 ; %7).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453" />
        <source>predicted too late</source>
        <translation>prévu trop tard</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453" />
        <source>predicted too early</source>
        <translation>prévu trop tôt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="455" />
        <source>Replayed without a device limit for %1 (not set).</source>
        <translation>Rejoué sans limite de périphérique pour %1 (non définie).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="458" />
        <source>Durations and overheads come from the same history.</source>
        <translation>Les durées et les surcoûts proviennent du même historique.</translation>
    </message>
</context>
<context>
    <name>BareosAuth</name>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="142" />
        <source>Starting Bareos authentication...</source>
        <translation>Démarrage de l'authentification Bareos...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="149" />
        <source>Setting up TLS-PSK...</source>
        <translation>Configuration de TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="168" />
        <source>Starting TLS-PSK handshake...</source>
        <translation>Démarrage de la négociation TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="175" />
        <source>Setting up certificate-based TLS...</source>
        <translation>Configuration de TLS par certificats...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="191" />
        <source>Starting TLS handshake...</source>
        <translation>Démarrage de la négociation TLS...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="199" />
        <source>Starting legacy authentication (no TLS)...</source>
        <translation>Démarrage de l'authentification héritée (sans TLS)...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="628" />
        <source>TLS established, sending Hello packet...</source>
        <translation>TLS établi, envoi du paquet Hello...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="652" />
        <source>Waiting for Director challenge...</source>
        <translation>En attente du challenge du Director...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="749" />
        <source>Director rejected connection: %1</source>
        <translation>Le Director a rejeté la connexion : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="756" />
        <source>

Hint: The Director requires TLS encryption. Please use PSK or Certificate authentication instead of Legacy mode.</source>
        <translation>

Conseil : le Director exige le chiffrement TLS. Veuillez utiliser l'authentification PSK ou par certificat au lieu du mode Legacy.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="762" />
        <source>Director sent empty response.

This usually means the Director requires TLS encryption but Legacy (unencrypted) mode was used.
Please try PSK or Certificate authentication.</source>
        <translation>Le Director a envoyé une réponse vide.

Cela signifie généralement que le Director exige le chiffrement TLS mais que le mode Legacy (non chiffré) a été utilisé.
Veuillez essayer l'authentification PSK ou par certificat.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="770" />
        <source>Unexpected response from Director:
%1</source>
        <translation>Réponse inattendue du Director :
%1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="832" />
        <source>CRAM-MD5 response sent successfully</source>
        <translation>Réponse CRAM-MD5 envoyée avec succès</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1228" />
        <source>The Director announced a message of %1 bytes during authentication (limit: %2 bytes). Connection closed.</source>
        <translation>Le Director a annoncé un message de %1 octets pendant l'authentification (limite : %2 octets). Connexion fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1374" />
        <location filename="../src/director/bareosauth.cpp" line="1488" />
        <source>Connected</source>
        <translation>Connecté</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1669" />
        <source>TLS-PSK authentication failed: %1

This usually means:
• Wrong password
• Wrong console name
• Console not configured on Director
• TLS-PSK not enabled on Director</source>
        <translation>Échec de l'authentification TLS-PSK : %1

Cela signifie généralement :
• Mot de passe incorrect
• Nom de console incorrect
• Console non configurée sur le Director
• TLS-PSK non activé sur le Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1678" />
        <source>TLS handshake failed: %1

Possible causes:
• No compatible cipher suites
• TLS version mismatch
• Director doesn't support PSK</source>
        <translation>Échec de la négociation TLS : %1

Causes possibles :
• Aucune suite de chiffrement compatible
• Versions TLS incompatibles
• Le Director ne prend pas en charge PSK</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1686" />
        <source>Socket error: %1</source>
        <translation>Erreur de socket : %1</translation>
    </message>
</context>
<context>
    <name>BareosDirector</name>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="183" />
        <source>Changing the configuration needs the gRPC interface of the Onesimus Director</source>
        <translation>La modification de la configuration nécessite l'interface gRPC de l'Onesimus Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="235" />
        <source>Connection to the Director was closed before the reply arrived</source>
        <translation>La connexion au Director a été fermée avant l'arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="294" />
        <source>Disconnected from the Director before the reply arrived</source>
        <translation>Déconnecté du Director avant l'arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="461" />
        <source>No response from Director within %1 seconds</source>
        <translation>Aucune réponse du Director en %1 secondes</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="471" />
        <source>Timeout while loading resources: %1</source>
        <translation>Délai dépassé lors du chargement des ressources : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="910" />
        <source>Connection to the Director was lost before the reply arrived</source>
        <translation>La connexion au Director a été perdue avant l'arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1006" />
        <source>The Director did not confirm the API mode within %1 seconds. The connection was closed.</source>
        <translation>Le Director n'a pas confirmé le mode API en %1 secondes. La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1127" />
        <source>The Director announced a message of %1 bytes (limit: %2 MiB). The connection was closed.</source>
        <translation>Le Director a annoncé un message de %1 octets (limite : %2 MiB). La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1559" />
        <source>TLS is enabled for this connection, but the session with the Director is not encrypted. The connection was closed.</source>
        <translation>TLS est activé pour cette connexion, mais la session avec le Director n'est pas chiffrée. La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1826" />
        <source>Unknown error</source>
        <translation>Erreur inconnue</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1881" />
        <location filename="../src/director/bareosdirector.cpp" line="2328" />
        <source>Not ready</source>
        <translation>Pas prêt</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1933" />
        <source>Error sending command</source>
        <translation>Erreur lors de l'envoi de la commande</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1980" />
        <source>The command was not sent: no ready connection to the Director</source>
        <translation>La commande n'a pas été envoyée : aucune connexion prête au Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2317" />
        <source>This build of Onesimus has no gRPC support</source>
        <translation>Cette version d'Onesimus ne prend pas en charge gRPC</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2408" />
        <source>Server certificate not trusted</source>
        <translation>Certificat du serveur non approuvé</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2424" />
        <source>Command failed</source>
        <translation>Échec de la commande</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2452" />
        <source>Connection to Director lost: %1</source>
        <translation>Connexion au Director perdue : %1</translation>
    </message>
</context>
<context>
    <name>CommandLine</name>
    <message>
        <location filename="../src/main.cpp" line="391" />
        <source>--tls needs --host. Without --host the TLS settings of the saved connection profile are used.</source>
        <translation>--tls nécessite --host. Sans --host, les paramètres TLS du profil de connexion enregistré sont utilisés.</translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="397" />
        <source>--tls needs the console password: use --password-file, the environment variable ONESIMUS_PASSWORD or --password.</source>
        <translation>--tls nécessite le mot de passe de la console : utilisez --password-file, la variable d'environnement ONESIMUS_PASSWORD ou --password.</translation>
    </message>
</context>
<context>
    <name>ConfigPreviewPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2020" />
        <source>Server Configuration Preview</source>
        <translation>Aperçu de la configuration du serveur</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2022" />
        <source>Copy this configuration to your Bareos Director</source>
        <translation>Copiez cette configuration sur votre Director Bareos</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2032" />
        <source>Console resource for the Director:</source>
        <translation>Ressource Console pour le Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2036" />
        <source>Path: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</source>
        <translation>Chemin : /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</translation>
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
        <translation>Copier dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2053" />
        <location filename="../src/bconnectionwizard.cpp" line="2094" />
        <location filename="../src/bconnectionwizard.cpp" line="2184" />
        <location filename="../src/bconnectionwizard.cpp" line="2495" />
        <location filename="../src/bconnectionwizard.cpp" line="2555" />
        <location filename="../src/bconnectionwizard.cpp" line="2632" />
        <source>Save .conf</source>
        <translation>Enregistrer .conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2058" />
        <location filename="../src/bconnectionwizard.cpp" line="2099" />
        <location filename="../src/bconnectionwizard.cpp" line="2189" />
        <location filename="../src/bconnectionwizard.cpp" line="2606" />
        <location filename="../src/bconnectionwizard.cpp" line="2671" />
        <source>Save ZIP</source>
        <translation>Enregistrer ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2066" />
        <source>Console Config</source>
        <translation>Config. Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2073" />
        <source>Director resource TLS settings (if needed):</source>
        <translation>Paramètres TLS de la ressource Director (si nécessaire) :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2077" />
        <location filename="../src/bconnectionwizard.cpp" line="2118" />
        <source>Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Chemin : /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2107" />
        <source>Director TLS</source>
        <translation>TLS du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2114" />
        <source>Enable gRPC in the Director resource:</source>
        <translation>Activer gRPC dans la ressource Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2137" />
        <source>Director (gRPC)</source>
        <translation>Director (gRPC)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2145" />
        <source>Configuration for command-line consoles on the client:</source>
        <translation>Configuration des consoles en ligne de commande sur le client :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2149" />
        <source>Path: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf or oconsole -c &amp;lt;file&amp;gt; (oconsole)</source>
        <translation>Chemin : /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf ou oconsole -c &amp;lt;file&amp;gt; (oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2157" />
        <source>Contains the console password: restrict the file to mode 0640 root:bareos (or 0600 owned by the user running the console).</source>
        <translation>Contient le mot de passe de la console : restreignez le fichier au mode 0640 root:bareos (ou 0600 appartenant à l'utilisateur qui exécute la console).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2197" />
        <source>Console client (bconsole / oconsole)</source>
        <translation>Client console (bconsole / oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2201" />
        <location filename="../src/bconnectionwizard.cpp" line="2229" />
        <source>After copying, paste this into your Bareos Director config and restart bareos-dir.</source>
        <translation>Après la copie, collez ceci dans la configuration de votre Director Bareos et redémarrez bareos-dir.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2227" />
        <source>Add the Console resource and enable gRPC in the Director resource, then reload the Director.</source>
        <translation>Ajoutez la ressource Console et activez gRPC dans la ressource Director, puis rechargez le Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2324" />
        <source>Default gRPC port: BNET port + 1000 (%1 + 1000 = %2).</source>
        <translation>Port gRPC par défaut : port BNET + 1000 (%1 + 1000 = %2).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2326" />
        <source>The Console resource provides name and password for the login.</source>
        <translation>La ressource Console fournit le nom et le mot de passe pour la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2327" />
        <source>After adding the Console, reload the Director:</source>
        <translation>Après avoir ajouté la Console, rechargez le Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2329" />
        <source>(enabling gRPC for the first time requires a Director restart)</source>
        <translation>(la première activation de gRPC nécessite un redémarrage du Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2416" />
        <source>Password in "[md5]&lt;hex&gt;" form: accepted by oconsole and by C++ bconsole.</source>
        <translation>Mot de passe sous la forme "[md5]&lt;hex&gt;" : accepté par oconsole et par le bconsole C++.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2426" />
        <source>not used when the Console resource below matches</source>
        <translation>non utilisé lorsque la ressource Console ci-dessous correspond</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2434" />
        <source>Grpc Fingerprint = &lt;SHA-256 of the Director's gRPC certificate&gt;</source>
        <translation>Grpc Fingerprint = &lt;SHA-256 du certificat gRPC du Director&gt;</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2465" />
        <location filename="../src/bconnectionwizard.cpp" line="2504" />
        <location filename="../src/bconnectionwizard.cpp" line="2515" />
        <location filename="../src/bconnectionwizard.cpp" line="2526" />
        <source>Copied!</source>
        <translation>Copié !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2478" />
        <source>Save Console Client Configuration</source>
        <translation>Enregistrer la configuration du client console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2479" />
        <location filename="../src/bconnectionwizard.cpp" line="2539" />
        <location filename="../src/bconnectionwizard.cpp" line="2616" />
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2485" />
        <location filename="../src/bconnectionwizard.cpp" line="2545" />
        <location filename="../src/bconnectionwizard.cpp" line="2575" />
        <location filename="../src/bconnectionwizard.cpp" line="2622" />
        <location filename="../src/bconnectionwizard.cpp" line="2651" />
        <source>Save Failed</source>
        <translation>Échec de l'enregistrement</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2486" />
        <location filename="../src/bconnectionwizard.cpp" line="2546" />
        <location filename="../src/bconnectionwizard.cpp" line="2623" />
        <source>Could not write to %1</source>
        <translation>Impossible d'écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2493" />
        <location filename="../src/bconnectionwizard.cpp" line="2553" />
        <location filename="../src/bconnectionwizard.cpp" line="2604" />
        <location filename="../src/bconnectionwizard.cpp" line="2630" />
        <location filename="../src/bconnectionwizard.cpp" line="2669" />
        <source>Saved!</source>
        <translation>Enregistré !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2538" />
        <source>Save Console Configuration</source>
        <translation>Enregistrer la configuration de la Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2565" />
        <source>Save Console Configuration as ZIP</source>
        <translation>Enregistrer la configuration de la Console en ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2566" />
        <location filename="../src/bconnectionwizard.cpp" line="2642" />
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2576" />
        <location filename="../src/bconnectionwizard.cpp" line="2652" />
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2615" />
        <source>Save Director TLS Configuration</source>
        <translation>Enregistrer la configuration TLS du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2641" />
        <source>Save Director TLS Configuration as ZIP</source>
        <translation>Enregistrer la configuration TLS du Director en ZIP</translation>
    </message>
</context>
<context>
    <name>ConsoleSetupPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2948" />
        <source>Console configuration</source>
        <translation>Configuration de la console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2956" />
        <source>Modify existing console</source>
        <translation>Modifier une console existante</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2969" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2979" />
        <source>Select a console to view its configuration...</source>
        <translation>Sélectionnez une console pour afficher sa configuration...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2987" />
        <source>Create new console</source>
        <translation>Créer une nouvelle console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2998" />
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3005" />
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3009" />
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3017" />
        <source>Security notice: Consoles with overly wide ACLs (e.g. *all*) grant full access to all backup operations, including deletion. Ask your backup administrator to create a Console resource with restricted ACLs tailored to your needs.</source>
        <translation>Avertissement de sécurité : les consoles dotées d'ACL trop larges (p. ex. *all*) donnent un accès complet à toutes les opérations de sauvegarde, y compris la suppression. Demandez à votre administrateur de sauvegarde de créer une ressource Console avec des ACL restreintes adaptées à vos besoins.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3033" />
        <location filename="../src/bconnectionwizard.cpp" line="3058" />
        <source>Modify an existing console or create a new one on the director.</source>
        <translation>Modifiez une console existante ou créez-en une nouvelle sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3056" />
        <source>gRPC requires 'Enable Grpc = yes' in the Director resource (default port: BNET port + 1000). After adding a console, reload the Director.</source>
        <translation>gRPC nécessite 'Enable Grpc = yes' dans la ressource Director (port par défaut : port BNET + 1000). Après avoir ajouté une console, rechargez le Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068" />
        <source>No Console</source>
        <translation>Aucune console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068" />
        <source>Please select a console.</source>
        <translation>Veuillez sélectionner une console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076" />
        <source>Missing</source>
        <translation>Manquant</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076" />
        <source>Please enter a name and password for the new console.</source>
        <translation>Veuillez saisir un nom et un mot de passe pour la nouvelle console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3179" />
        <location filename="../src/bconnectionwizard.cpp" line="3275" />
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3197" />
        <source>Loading details for %1...</source>
        <translation>Chargement des détails de %1...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3206" />
        <source>Waiting for connection...</source>
        <translation>En attente de connexion...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3216" />
        <location filename="../src/bconnectionwizard.cpp" line="3257" />
        <source>Loading consoles...</source>
        <translation>Chargement des consoles...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3222" />
        <source>Connecting to Director...</source>
        <translation>Connexion au Director...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3246" />
        <source>Loading resources...</source>
        <translation>Chargement des ressources...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3248" />
        <source>Authentication failed: %1</source>
        <translation>Échec de l'authentification : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3266" />
        <source>Connection lost</source>
        <translation>Connexion perdue</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3299" />
        <source>No named consoles found. Ask your backup administrator to create a personalized Console resource on the Director.</source>
        <translation>Aucune console nommée trouvée. Demandez à votre administrateur de sauvegarde de créer une ressource Console personnalisée sur le Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="3303" />
        <source>Found %n console(s)</source>
        <translation>
            <numerusform>%n Console trouvée</numerusform>
            <numerusform>%n Consoles trouvées</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3320" />
        <source>No details found for %1</source>
        <translation>Aucun détail trouvé pour %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3325" />
        <source>Name: %1
</source>
        <translation>Nom : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3327" />
        <source>Description: %1
</source>
        <translation>Description : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3329" />
        <source>Profile: %1
</source>
        <translation>Profile : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3334" />
        <source>TLS Enabled: %1
</source>
        <translation>TLS activé : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3336" />
        <source>TLS Required: %1
</source>
        <translation>TLS requis : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3338" />
        <source>TLS Verify Peer: %1
</source>
        <translation>TLS Verify Peer : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3340" />
        <source>Job ACL: %1
</source>
        <translation>ACL Job : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3342" />
        <source>Client ACL: %1
</source>
        <translation>ACL Client : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3344" />
        <source>Storage ACL: %1
</source>
        <translation>ACL Storage : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3346" />
        <source>Command ACL: %1
</source>
        <translation>ACL Command : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3348" />
        <source>Catalog ACL: %1
</source>
        <translation>ACL Catalog : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3362" />
        <source>
--- WARNING ---
This console has full access (*all*). Consider restricting ACLs to prevent accidental deletion of backups or other critical operations.
</source>
        <translation>
--- AVERTISSEMENT ---
Cette console dispose d'un accès complet (*all*). Envisagez de restreindre les ACL pour éviter la suppression accidentelle de sauvegardes ou d'autres opérations critiques.
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3397" />
        <source>Creating console '%1'...</source>
        <translation>Création de la console '%1'...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3408" />
        <source>Console created successfully!</source>
        <translation>Console créée avec succès !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3411" />
        <source>Failed to create console</source>
        <translation>Échec de la création de la console</translation>
    </message>
</context>
<context>
    <name>CredentialsPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1418" />
        <source>Enter your credentials</source>
        <translation>Saisissez vos identifiants</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1424" />
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1428" />
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1436" />
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1437" />
        <source>Generate a secure random password</source>
        <translation>Générer un mot de passe aléatoire sécurisé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1442" />
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1449" />
        <location filename="../src/bconnectionwizard.cpp" line="1455" />
        <source>(password MD5 hash will appear here)</source>
        <translation>(le hachage MD5 du mot de passe apparaîtra ici)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1450" />
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1488" />
        <source>Remember password</source>
        <translation>Mémoriser le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1497" />
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console config: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</source>
        <translation>Configuration du Director : /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Configuration de la console : /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1503" />
        <source>These must match your Bareos Director configuration.</source>
        <translation>Ces valeurs doivent correspondre à la configuration de votre Director Bareos.</translation>
    </message>
</context>
<context>
    <name>ImportConsoleSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="723" />
        <source>Select Director and Console from imported configuration</source>
        <translation>Sélectionner le Director et la Console de la configuration importée</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="728" />
        <source>The configuration will be parsed from your ZIP file. Select which Director and Console to use for the connection.</source>
        <translation>La configuration sera analysée à partir de votre fichier ZIP. Sélectionnez le Director et la Console à utiliser pour la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="734" />
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="737" />
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="758" />
        <source>Select a console to view its details...</source>
        <translation>Sélectionnez une console pour afficher ses détails...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="800" />
        <source>No ZIP file specified.</source>
        <translation>Aucun fichier ZIP indiqué.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="806" />
        <source>Extracting and parsing ZIP archive...</source>
        <translation>Extraction et analyse de l'archive ZIP...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="814" />
        <source>Failed to create temporary directory.</source>
        <translation>Impossible de créer le répertoire temporaire.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="823" />
        <source>Cannot read ZIP file: %1</source>
        <translation>Impossible de lire le fichier ZIP : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="830" />
        <source>Failed to extract ZIP archive.</source>
        <translation>Impossible d'extraire l'archive ZIP.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="867" />
        <source>No configuration resources found in the archive.</source>
        <translation>Aucune ressource de configuration trouvée dans l'archive.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="892" />
        <source>Parsed %1 resources: %2 Director(s), %3 Console(s)</source>
        <translation>%1 ressources analysées : %2 Director(s), %3 Console(s)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="899" />
        <source>Warning: Need at least one Director and one Console resource.</source>
        <translation>Avertissement : il faut au moins une ressource Director et une ressource Console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="920" />
        <source>=== Director: %1 ===
</source>
        <translation>=== Director : %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="925" />
        <source>(multiple addresses)</source>
        <translation>(plusieurs adresses)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928" />
        <source>  Address: %1
</source>
        <translation>  Adresse : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928" />
        <source>(not set)</source>
        <translation>(non défini)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="929" />
        <source>  Port: %1
</source>
        <translation>  Port : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="930" />
        <location filename="../src/bconnectionwizard.cpp" line="952" />
        <source>  TLS Enable: %1
</source>
        <translation>  TLS Enable : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="937" />
        <source>=== Console: %1 ===
</source>
        <translation>=== Console : %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="943" />
        <source>  Password: [md5]***
</source>
        <translation>  Mot de passe : [md5]***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="945" />
        <source>  Password: ***
</source>
        <translation>  Mot de passe : ***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="948" />
        <source>  Password: (not set)
</source>
        <translation>  Mot de passe : (non défini)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="953" />
        <source>  TLS Require: %1
</source>
        <translation>  TLS Require : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="958" />
        <source>  Profile: %1
</source>
        <translation>  Profile : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="974" />
        <source>
  --- ACLs ---
</source>
        <translation>
  --- ACL ---
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="977" />
        <source>  %1: %2
</source>
        <translation>  %1 : %2
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="982" />
        <source>
  (No ACLs defined on this console)
</source>
        <translation>
  (Aucune ACL définie sur cette console)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1119" />
        <source>Host/Address</source>
        <translation>Hôte/Adresse</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1120" />
        <source>Director Name</source>
        <translation>Nom du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1121" />
        <source>Console Name</source>
        <translation>Nom de la console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1122" />
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1125" />
        <source>Incomplete Configuration</source>
        <translation>Configuration incomplète</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1126" />
        <source>The imported configuration is missing the following required fields:

  %1

You can fill them in on the following pages.</source>
        <translation>Il manque les champs obligatoires suivants dans la configuration importée :

  %1

Vous pouvez les renseigner dans les pages suivantes.</translation>
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
        <translation>Nommer cette connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3425" />
        <source>e.g., Production Server</source>
        <translation>p. ex. Serveur de production</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3428" />
        <source>Set as default</source>
        <translation>Définir par défaut</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3432" />
        <source>Connect now</source>
        <translation>Se connecter maintenant</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3442" />
        <source>Advanced Settings...</source>
        <translation>Paramètres avancés...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3443" />
        <source>Configure TLS options, ACLs, and other advanced settings</source>
        <translation>Configurer les options TLS, les ACL et d'autres paramètres avancés</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3448" />
        <source>Export Config...</source>
        <translation>Exporter la configuration...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3449" />
        <source>Export Bareos configuration files as zip archive</source>
        <translation>Exporter les fichiers de configuration Bareos sous forme d'archive zip</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3456" />
        <source>This name helps identify the connection.</source>
        <translation>Ce nom permet d'identifier la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3536" />
        <source>Console only (for existing Director)</source>
        <translation>Console uniquement (pour un Director existant)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3537" />
        <source>Console + Profile (with ACLs)</source>
        <translation>Console + Profile (avec ACL)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3538" />
        <source>Full configuration (new installation)</source>
        <translation>Configuration complète (nouvelle installation)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3542" />
        <location filename="../src/bconnectionwizard.cpp" line="3557" />
        <source>Export Configuration</source>
        <translation>Exporter la configuration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3543" />
        <source>Select export mode:</source>
        <translation>Sélectionnez le mode d'exportation :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3559" />
        <source>Zip Archives (*.zip)</source>
        <translation>Archives zip (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3564" />
        <source>Export Successful</source>
        <translation>Exportation réussie</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3565" />
        <source>Configuration exported to:
%1

Extract to your Bareos server's root directory and restart the Director.</source>
        <translation>Configuration exportée vers :
%1

Extrayez-la dans le répertoire racine de votre serveur Bareos et redémarrez le Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3569" />
        <source>Export Failed</source>
        <translation>Échec de l'exportation</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3570" />
        <source>Failed to export configuration:
%1</source>
        <translation>Impossible d'exporter la configuration :
%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="30" />
        <source>No files to export</source>
        <translation>Aucun fichier à exporter</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="203" />
        <source>Cannot create zip file: %1</source>
        <translation>Impossible de créer le fichier zip : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="234" />
        <source>Error writing zip file</source>
        <translation>Erreur lors de l'écriture du fichier zip</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="295" />
        <source>Not set</source>
        <translation>Non défini</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="299" />
        <source>%n year(s)</source>
        <translation>
            <numerusform>%n an</numerusform>
            <numerusform>%n ans</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="301" />
        <source>%n day(s)</source>
        <translation>
            <numerusform>%n jour</numerusform>
            <numerusform>%n jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="305" />
        <source>%n hour(s)</source>
        <translation>
            <numerusform>%n heure</numerusform>
            <numerusform>%n heures</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="307" />
        <source>%n second(s)</source>
        <translation>
            <numerusform>%n seconde</numerusform>
            <numerusform>%n secondes</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="27" />
        <source>Add "%1"</source>
        <translation>Ajouter "%1"</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="61" />
        <source>Remove "%1"</source>
        <translation>Supprimer "%1"</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="63" />
        <source>Remove %1 items</source>
        <translation>Supprimer %1 éléments</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="96" />
        <source>Edit item</source>
        <translation>Modifier l'élément</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="137" />
        <source>Move item</source>
        <translation>Déplacer l'élément</translation>
    </message>
</context>
<context>
    <name>ServerPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1154" />
        <source>Where is your Bareos Director?</source>
        <translation>Où se trouve votre Director Bareos ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1159" />
        <source>Found in the network:</source>
        <translation>Trouvés sur le réseau :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1161" />
        <source>Search again</source>
        <translation>Rechercher à nouveau</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1171" />
        <source>Onesimus Directors announce themselves in the local network. Select one to fill in the connection data.</source>
        <translation>Les Onesimus Directors s'annoncent sur le réseau local. Sélectionnez-en un pour renseigner les données de connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1186" />
        <source>Searching...</source>
        <translation>Recherche...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1197" />
        <source>e.g., bareos.example.com or 192.168.1.100</source>
        <translation>p. ex. bareos.example.com ou 192.168.1.100</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1198" />
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1204" />
        <source>Port:</source>
        <translation>Port :</translation>
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
        <translation>Sélectionnez le système d'exploitation du serveur du Director Bareos.
Cela influe sur les chemins des certificats dans les configurations exportées.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1213" />
        <source>Server OS:</source>
        <translation>OS du serveur :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1217" />
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole de console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1218" />
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1224" />
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d'Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1226" />
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1232" />
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1246" />
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Configuration du Director : /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1249" />
        <location filename="../src/bconnectionwizard.cpp" line="1308" />
        <source>The default port is 9101.</source>
        <translation>Le port par défaut est 9101.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1286" />
        <source>Searching for Onesimus Directors in the network...</source>
        <translation>Recherche d'Onesimus Directors sur le réseau...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1292" />
        <source>No Onesimus Director found - enter the server manually.</source>
        <translation>Aucun Onesimus Director trouvé - saisissez le serveur manuellement.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1307" />
        <source>gRPC is encrypted with TLS; the server certificate is confirmed on first use.</source>
        <translation>gRPC est chiffré avec TLS ; le certificat du serveur est confirmé lors de la première utilisation.</translation>
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
            <numerusform>%n Director trouvé. Sélectionnez-le pour l'utiliser.</numerusform>
            <numerusform>%n Directors trouvés. Sélectionnez-en un pour l'utiliser.</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>StorageWidget</name>
    <message>
        <location filename="../src/storagewidget.cpp" line="27" />
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="53" />
        <source>(due)</source>
        <translation>(échu)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="85" />
        <source>&amp;Pool:</source>
        <translation>&amp;Pool :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="88" />
        <source>Pool filter</source>
        <translation>Filtre de pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="94" />
        <source>&amp;Status:</source>
        <translation>É&amp;tat :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="97" />
        <source>Status filter</source>
        <translation>Filtre d'état</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="103" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="114" />
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="117" />
        <source>Volumes:</source>
        <translation>Volumes :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="125" />
        <source>Used:</source>
        <translation>Utilisé :</translation>
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
        <translation>État</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="455" />
        <source>Used</source>
        <translation>Utilisé</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145" />
        <location filename="../src/storagewidget.cpp" line="456" />
        <source>Limit</source>
        <translation>Limite</translation>
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
        <translation>Dernière écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Retention</source>
        <translation>Rétention</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Reusable From</source>
        <translation>Réutilisable à partir du</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146" />
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149" />
        <source>Maximum Volume Bytes of the volume; "none" = no limit</source>
        <translation>Maximum Volume Bytes du Volume ; "aucune" = pas de limite</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="151" />
        <source>From when Bareos may reuse the volume. It does so only when its pool needs a volume, so this is the earliest date, not the day of recycling.</source>
        <translation>Date à partir de laquelle Bareos peut réutiliser le Volume. Il ne le fait que lorsque son Pool a besoin d'un Volume ; il s'agit donc de la date la plus proche, pas du jour du recyclage.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="153" />
        <source>Volumes</source>
        <translation>Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="155" />
        <source>One row per volume. Enter opens the details of the selected volume.</source>
        <translation>Une ligne par Volume. Entrée ouvre les détails du Volume sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="212" />
        <source>All pools (%1)</source>
        <translation>Tous les pools (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="215" />
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="221" />
        <source>All statuses (%1)</source>
        <translation>Tous les états (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="224" />
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="247" />
        <source>Error parsing volume data</source>
        <translation>Erreur lors de l'analyse des données des Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="254" />
        <source>The Director sent the short volume list: limits, job counts and use durations are unknown, and so is when an Append volume will be closed.</source>
        <translation>Le Director a envoyé la liste courte des Volumes : les limites, le nombre de Jobs et les durées d'utilisation sont inconnus, de même que le moment où un Volume Append sera fermé.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/storagewidget.cpp" line="268" />
        <source>%n volume(s) loaded</source>
        <translation>
            <numerusform>%n Volume chargé</numerusform>
            <numerusform>%n Volumes chargés</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="315" />
        <source>%1, %2, %3, last written %4, reusable from %5</source>
        <translation>%1, %2, %3, dernière écriture %4, réutilisable à partir du %5</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="343" />
        <location filename="../src/storagewidget.cpp" line="456" />
        <location filename="../src/storagewidget.cpp" line="458" />
        <location filename="../src/storagewidget.cpp" line="464" />
        <location filename="../src/storagewidget.cpp" line="466" />
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="351" />
        <source>%1 of %2</source>
        <translation>%1 sur %2</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="408" />
        <source>Refreshing the volume list...</source>
        <translation>Actualisation de la liste des Volumes...</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="436" />
        <source>unknown (not in this list)</source>
        <translation>inconnu (absent de cette liste)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="453" />
        <source>Media type</source>
        <translation>Type de média</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="458" />
        <source>Maximum jobs</source>
        <translation>Nombre maximal de Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="459" />
        <source>Errors</source>
        <translation>Erreurs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="460" />
        <source>First written</source>
        <translation>Première écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="461" />
        <source>Last written</source>
        <translation>Dernière écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="462" />
        <source>Labeled</source>
        <translation>Étiqueté</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="463" />
        <source>Volume retention</source>
        <translation>Rétention du Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="464" />
        <source>Use duration</source>
        <translation>Durée d'utilisation</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>Recycle</source>
        <translation>Recyclage</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465" />
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="466" />
        <source>Recycle pool</source>
        <translation>Pool de recyclage</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="467" />
        <source>Times recycled</source>
        <translation>Nombre de recyclages</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468" />
        <source>Reusable from</source>
        <translation>Réutilisable à partir du</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="478" />
        <source>Volume Details</source>
        <translation>Détails du Volume</translation>
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
        <translation>Où se trouvent vos certificats TLS ?</translation>
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
        <translation>Cert. CA :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1785" />
        <source>Path to PFX certificate file</source>
        <translation>Chemin du fichier de certificat PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1790" />
        <source>PFX File:</source>
        <translation>Fichier PFX :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1794" />
        <source>PFX file password (if encrypted)</source>
        <translation>Mot de passe du fichier PFX (s'il est chiffré)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1795" />
        <source>PFX Password:</source>
        <translation>Mot de passe PFX :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1806" />
        <source>/etc/bareos/ssl/client.pem</source>
        <translation>/etc/bareos/ssl/client.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1811" />
        <source>Client Cert:</source>
        <translation>Cert. client :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1815" />
        <source>/etc/bareos/ssl/client.key</source>
        <translation>/etc/bareos/ssl/client.key</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1820" />
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1830" />
        <source>Convert PEM/DER to PFX...</source>
        <translation>Convertir PEM/DER en PFX...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1831" />
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1836" />
        <source>Generate Certificates...</source>
        <translation>Générer des certificats...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1839" />
        <source>No certificates? Click Generate to create them.</source>
        <translation>Pas de certificats ? Cliquez sur Générer pour les créer.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875" />
        <location filename="../src/bconnectionwizard.cpp" line="1886" />
        <location filename="../src/bconnectionwizard.cpp" line="1898" />
        <source>Missing</source>
        <translation>Manquant</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875" />
        <source>Please provide a CA certificate file.</source>
        <translation>Veuillez indiquer un fichier de certificat CA.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879" />
        <location filename="../src/bconnectionwizard.cpp" line="1890" />
        <location filename="../src/bconnectionwizard.cpp" line="1902" />
        <source>Not Found</source>
        <translation>Introuvable</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879" />
        <source>CA certificate file not found.</source>
        <translation>Fichier de certificat CA introuvable.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1886" />
        <source>Please provide a PFX certificate file.</source>
        <translation>Veuillez indiquer un fichier de certificat PFX.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1890" />
        <source>PFX certificate file not found.</source>
        <translation>Fichier de certificat PFX introuvable.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1898" />
        <source>Please provide all certificate files.</source>
        <translation>Veuillez indiquer tous les fichiers de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1902" />
        <source>One or more certificate files not found.</source>
        <translation>Un ou plusieurs fichiers de certificat sont introuvables.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1936" />
        <source>CA Certificate</source>
        <translation>Certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1937" />
        <location filename="../src/bconnectionwizard.cpp" line="1958" />
        <source>Certificates (*.pem *.crt *.cer);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cer);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1954" />
        <source>Select PFX Certificate</source>
        <translation>Sélectionner le certificat PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1955" />
        <source>PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)</source>
        <translation>Fichiers PFX (*.pfx);;Certificats (*.pem *.crt);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1957" />
        <source>Client Certificate</source>
        <translation>Certificat client</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1975" />
        <source>Private Key</source>
        <translation>Clé privée</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1976" />
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés privées (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2008" />
        <source>PFX File Created</source>
        <translation>Fichier PFX créé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2009" />
        <source>PFX file has been created successfully.

The certificate path has been updated.</source>
        <translation>Le fichier PFX a été créé avec succès.

Le chemin du certificat a été mis à jour.</translation>
    </message>
</context>
<context>
    <name>TemplateSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="375" />
        <source>How would you like to create the connection?</source>
        <translation>Comment souhaitez-vous créer la connexion ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="377" />
        <source>You can start from scratch, use a template from an existing connection, or import from a ZIP file.</source>
        <translation>Vous pouvez partir de zéro, utiliser comme modèle une connexion existante ou importer depuis un fichier ZIP.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="381" />
        <source>Create new connection from scratch</source>
        <translation>Créer une nouvelle connexion à partir de zéro</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="382" />
        <source>Use template from existing connection</source>
        <translation>Utiliser une connexion existante comme modèle</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="383" />
        <source>Import from ZIP file</source>
        <translation>Importer depuis un fichier ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="405" />
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="428" />
        <source>Select ZIP file...</source>
        <translation>Sélectionner un fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="431" />
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="542" />
        <source>Director: %1</source>
        <translation>Director : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="543" />
        <source>Console: %1</source>
        <translation>Console : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>TLS: %1</source>
        <translation>TLS : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <source>PSK</source>
        <translation>PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <source>Certificate</source>
        <translation>Certificat</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544" />
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="600" />
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="601" />
        <source>System: %1</source>
        <translation>Système : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="707" />
        <source>Select Director Configuration ZIP</source>
        <translation>Sélectionner le ZIP de configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="709" />
        <source>ZIP Files (*.zip);;All Files (*)</source>
        <translation>Fichiers ZIP (*.zip);;Tous les fichiers (*)</translation>
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
        <translation>Tester la connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2686" />
        <source>Let's test your connection.</source>
        <translation>Testons votre connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2690" />
        <location filename="../src/bconnectionwizard.cpp" line="2726" />
        <source>Click the button to test.</source>
        <translation>Cliquez sur le bouton pour tester.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2749" />
        <source>Connecting...</source>
        <translation>Connexion...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2752" />
        <source>Starting test...</source>
        <translation>Démarrage du test...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2791" />
        <source>Mode: TLS-Cert</source>
        <translation>Mode : TLS-Cert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2803" />
        <source>Mode: TLS-PSK with cipher list: %1</source>
        <translation>Mode : TLS-PSK avec la liste de chiffrements : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2805" />
        <source>Mode: TLS-PSK</source>
        <translation>Mode : TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2818" />
        <source>Mode: gRPC (TLS) on port %1</source>
        <translation>Mode : gRPC (TLS) sur le port %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2819" />
        <location filename="../src/bconnectionwizard.cpp" line="2821" />
        <source>Connecting to %1:%2...</source>
        <translation>Connexion à %1:%2...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2857" />
        <source>Cancelled.</source>
        <translation>Annulé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2865" />
        <source>Connecting</source>
        <translation>Connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2866" />
        <source>Authenticating</source>
        <translation>Authentification</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2867" />
        <source>Setting API</source>
        <translation>Réglage de l'API</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2868" />
        <source>Loading</source>
        <translation>Chargement</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2869" />
        <source>Ready</source>
        <translation>Prêt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2891" />
        <source>Server certificate trusted.</source>
        <translation>Certificat du serveur approuvé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2895" />
        <source>Server certificate not trusted - connection cancelled.</source>
        <translation>Certificat du serveur non approuvé - connexion annulée.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2896" />
        <location filename="../src/bconnectionwizard.cpp" line="2909" />
        <location filename="../src/bconnectionwizard.cpp" line="2915" />
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2908" />
        <source>Auth OK</source>
        <translation>Authentification OK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2909" />
        <source>Auth failed: %1</source>
        <translation>Échec de l'authentification : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2914" />
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2925" />
        <source>Success!</source>
        <translation>Succès !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2926" />
        <source>Connection successful!</source>
        <translation>Connexion réussie !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2929" />
        <source>Test Again</source>
        <translation>Tester à nouveau</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2937" />
        <source>Timeout</source>
        <translation>Délai dépassé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2938" />
        <source>Timed out</source>
        <translation>Délai dépassé</translation>
    </message>
</context>
<context>
    <name>WelcomePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="353" />
        <source>Welcome</source>
        <translation>Bienvenue</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="359" />
        <source>Let's set up a connection to your Bareos Director.

You'll need:
  - The hostname or IP of your Director
  - Your console name and password

Click Next to begin.</source>
        <translation>Configurons une connexion à votre Director Bareos.

Vous aurez besoin :
  - du nom d'hôte ou de l'adresse IP de votre Director
  - de votre nom de console et de votre mot de passe

Cliquez sur Suivant pour commencer.</translation>
    </message>
</context>
</TS>