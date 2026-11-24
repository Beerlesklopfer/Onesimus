<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR" sourcelanguage="en">
<context>
    <name>AuthMethodPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1504"/>
        <source>How should we authenticate?</source>
        <translation>Comment voulez-vous vous authentifier ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1508"/>
        <source>TLS-PSK (recommended)</source>
        <translation>TLS-PSK (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1513"/>
        <source>TLS with certificates</source>
        <translation>TLS avec certificats</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1531"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Liste de chiffrements TLS (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1538"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>p. ex. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1539"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Click Preset to use recommended ciphers.</source>
        <translation>Liste de chiffrements TLS-PSK séparés par des deux-points.
Laisser vide pour la détection automatique.
Cliquez sur Préréglage pour utiliser les chiffrements recommandés.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1544"/>
        <source>Preset</source>
        <translation>Préréglage</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1545"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Remplir avec les chiffrements PSK recommandés</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1554"/>
        <source>TLS-PSK is the default for Bareos 18.2+.</source>
        <translation>TLS-PSK est la valeur par défaut depuis Bareos 18.2.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1612"/>
        <source>Server supports TLS encryption.</source>
        <translation>Le serveur prend en charge le chiffrement TLS.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1621"/>
        <source>TLS-PSK - not available on this server</source>
        <translation>TLS-PSK - non disponible sur ce serveur</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1622"/>
        <source>This server doesn&apos;t appear to support TLS-PSK. Consider upgrading to Bareos 18.2 or later.</source>
        <translation>Ce serveur ne semble pas prendre en charge TLS-PSK. Envisagez une mise à niveau vers Bareos 18.2 ou ultérieur.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1631"/>
        <source>Server check failed: %1</source>
        <translation>Échec de la vérification du serveur : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1649"/>
        <source>TLS-PSK uses your password to encrypt the connection.</source>
        <translation>TLS-PSK utilise votre mot de passe pour chiffrer la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1654"/>
        <source>You&apos;ll need CA, client certificate, and private key.</source>
        <translation>Vous aurez besoin d&apos;une CA, d&apos;un certificat client et d&apos;une clé privée.</translation>
    </message>
</context>
<context>
    <name>B321Dialog</name>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="91"/>
        <location filename="../src/tools/b321dialog.cpp" line="578"/>
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="112"/>
        <location filename="../src/tools/b321dialog.cpp" line="635"/>
        <source>%1 (not in the volume list)</source>
        <translation>%1 (absent de la liste des volumes)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="114"/>
        <location filename="../src/tools/b321dialog.cpp" line="652"/>
        <source>media type unknown</source>
        <translation>type de support inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="115"/>
        <source>storage unknown</source>
        <translation>Storage inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="118"/>
        <source>off site (marked)</source>
        <translation>hors site (marqué)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="120"/>
        <source>off site (location %1)</source>
        <translation>hors site (emplacement %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="122"/>
        <source>on site</source>
        <translation>sur site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="123"/>
        <source>off site unknown</source>
        <translation>hors site inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="125"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="134"/>
        <location filename="../src/tools/b321dialog.cpp" line="135"/>
        <source>3-2-1 Verifier</source>
        <translation>Vérificateur 3-2-1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="136"/>
        <source>Checks for every backup job whether there are 3 copies of the data, on 2 media types, with 1 copy off site. Read only.</source>
        <translation>Vérifie pour chaque job de sauvegarde s&apos;il existe 3 copies des données, sur 2 types de support, dont 1 copie hors site. Lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="157"/>
        <source>Data state</source>
        <translation>État des données</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="159"/>
        <source>&amp;Refresh</source>
        <translation>&amp;Actualiser</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="161"/>
        <source>Reload the jobs, the volumes of the jobs and the volume list (read only)</source>
        <translation>Recharger les jobs, les volumes des jobs et la liste des volumes (lecture seule)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="173"/>
        <source>&amp;Backup jobs:</source>
        <translation>Jobs de &amp;sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Copies</source>
        <translation>Copies</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Media Types</source>
        <translation>Types de support</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Off Site</source>
        <translation>Hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Verdict</source>
        <translation>Verdict</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="180"/>
        <source>Copies of the data a restore would use: the chain, its complete copies and, if set, the original data. 3 are needed.</source>
        <translation>Copies des données qu&apos;une restauration utiliserait : la chaîne, ses copies complètes et, si activé, les données d&apos;origine. 3 sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="183"/>
        <source>Distinct media types of the volumes of the chain and its complete copies. 2 are needed.</source>
        <translation>Types de support distincts des volumes de la chaîne et de ses copies complètes. 2 sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="185"/>
        <source>Copies off site, by your marking of the storages or a location of the volume. 1 is needed.</source>
        <translation>Copies hors site, d&apos;après votre marquage des Storages ou un emplacement du volume. 1 est nécessaire.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="188"/>
        <source>3-2-1 check per backup job</source>
        <translation>Vérification 3-2-1 par job de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="189"/>
        <source>One row per backup job: copies, media types, off site, verdict and reason. The details of the selected job follow below the table.</source>
        <translation>Une ligne par job de sauvegarde : copies, types de support, hors site, verdict et raison. Les détails du job sélectionné suivent sous le tableau.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="224"/>
        <source>C&amp;hain and copies:</source>
        <translation>C&amp;haîne et copies :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="229"/>
        <source>Chain and copies of the selected job</source>
        <translation>Chaîne et copies du job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="230"/>
        <source>The jobs a restore would use and the copies of them, with the volumes they lie on</source>
        <translation>Les jobs qu&apos;une restauration utiliserait et leurs copies, avec les volumes sur lesquels ils se trouvent</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="240"/>
        <source>&amp;Why, and what to do:</source>
        <translation>&amp;Pourquoi, et que faire :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="244"/>
        <source>Reasons, suggestions and hints for the selected job</source>
        <translation>Raisons, suggestions et conseils pour le job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="253"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="255"/>
        <source>Count the &amp;original data as a copy</source>
        <translation>Compter les données d&apos;&amp;origine comme une copie</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="256"/>
        <source>The 3-2-1 rule is usually stated as the production data plus 2 backups. Off: 3 backups are needed.</source>
        <translation>La règle 3-2-1 s&apos;énonce généralement comme les données de production plus 2 sauvegardes. Désactivé : 3 sauvegardes sont nécessaires.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="261"/>
        <source>O&amp;ff-site storages:</source>
        <translation>Storages &amp;hors site :</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="263"/>
        <source>Bareos does not record where a storage stands. Tick the storages that are at another site.</source>
        <translation>Bareos n&apos;enregistre pas où se trouve un Storage. Cochez les Storages situés sur un autre site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="270"/>
        <source>Off-site storages</source>
        <translation>Storages hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="271"/>
        <source>A checked storage is off site. Space toggles.</source>
        <translation>Un Storage coché est hors site. Espace pour basculer.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="274"/>
        <source>&amp;Unmarked storages are on site</source>
        <translation>Les Storages non &amp;marqués sont sur site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="275"/>
        <source>Without this, a storage that is not ticked is &quot;unknown&quot;, not &quot;on site&quot;.</source>
        <translation>Sans cette option, un Storage non coché est « inconnu », et non « sur site ».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="285"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="322"/>
        <location filename="../src/tools/b321dialog.cpp" line="438"/>
        <source>%1, off site</source>
        <translation>%1, hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="323"/>
        <location filename="../src/tools/b321dialog.cpp" line="439"/>
        <source>%1, not marked off site</source>
        <translation>%1, non marqué hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="444"/>
        <source>No storage known yet</source>
        <translation>Aucun Storage connu pour l&apos;instant</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="471"/>
        <location filename="../src/tools/b321dialog.cpp" line="520"/>
        <source>Jobs: %1</source>
        <translation>Jobs : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="490"/>
        <location filename="../src/tools/b321dialog.cpp" line="523"/>
        <source>Volumes of the jobs: %1</source>
        <translation>Volumes des jobs : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="505"/>
        <location filename="../src/tools/b321dialog.cpp" line="526"/>
        <source>Volume list: %1</source>
        <translation>Liste des volumes : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="557"/>
        <source>%1: %2. %3</source>
        <translation>%1 : %2. %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="561"/>
        <source>%1/3</source>
        <translation>%1/3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="564"/>
        <source>Copies: %1 of 3, %2</source>
        <translation>Copies : %1 sur 3, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="568"/>
        <source>?/2</source>
        <translation>?/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="568"/>
        <source>%1/2</source>
        <translation>%1/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="571"/>
        <source>Media types: %1 of 2%2, %3</source>
        <translation>Types de support : %1 sur 2%2, %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="579"/>
        <source>%1/1</source>
        <translation>%1/1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="583"/>
        <source>Off site: unknown</source>
        <translation>Hors site : inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="584"/>
        <source>Off site: %1 of 1, %2</source>
        <translation>Hors site : %1 sur 1, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="590"/>
        <source>Verdict: %1</source>
        <translation>Verdict : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="614"/>
        <source>No job selected.</source>
        <translation>Aucun job sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="626"/>
        <source>volumes not loaded</source>
        <translation>volumes non chargés</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="628"/>
        <source>no volume recorded</source>
        <translation>aucun volume enregistré</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="633"/>
        <source>%1 (%2, %3, %4)</source>
        <translation>%1 (%2, %3, %4)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="643"/>
        <source>Chain (the backup itself): %n job(s)</source>
        <translation>
            <numerusform>Chaîne (la sauvegarde elle-même) : %n job</numerusform>
            <numerusform>Chaîne (la sauvegarde elle-même) : %n jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="645"/>
        <source>Copy in pool %1: complete</source>
        <translation>Copie dans le pool %1 : complète</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="645"/>
        <location filename="../src/tools/b321dialog.cpp" line="648"/>
        <source>(not reported)</source>
        <translation>(non indiqué)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="647"/>
        <source>Copy in pool %1: partial, %2 of %3 jobs</source>
        <translation>Copie dans le pool %1 : partielle, %2 jobs sur %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="656"/>
        <source>last job ended %1</source>
        <translation>dernier job terminé %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="657"/>
        <source>copied %1</source>
        <translation>copié %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="658"/>
        <source>%1 — %2</source>
        <translation>%1 — %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="665"/>
        <source>%1 %2, started %3: %4</source>
        <translation>%1 %2, démarré %3 : %4</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="668"/>
        <source> (migrated from job %1)</source>
        <translation> (migré depuis le job %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="672"/>
        <source>Copy %1 of %2 %3, copied %4: %5</source>
        <translation>Copie %1 de %2 %3, copiée %4 : %5</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="681"/>
        <source>No copy of %1 %2</source>
        <translation>Aucune copie de %1 %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="687"/>
        <source>No restorable chain in the job history</source>
        <translation>Aucune chaîne restaurable dans l&apos;historique des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="692"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="699"/>
        <source>Why</source>
        <translation>Pourquoi</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="700"/>
        <source>What to do</source>
        <translation>Que faire</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="701"/>
        <source>Hints (not part of the verdict)</source>
        <translation>Conseils (ne font pas partie du verdict)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="710"/>
        <source>Not everything could be loaded: %1</source>
        <translation>Tout n&apos;a pas pu être chargé : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="714"/>
        <source>jobs</source>
        <translation>jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="715"/>
        <source>volumes of the jobs</source>
        <translation>volumes des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="716"/>
        <source>volume list</source>
        <translation>liste des volumes</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="717"/>
        <source>Loading: %1 …</source>
        <translation>Chargement : %1 …</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="720"/>
        <source>No job data yet. Refresh loads it (read only).</source>
        <translation>Aucune donnée de job pour l&apos;instant. Actualiser les charge (lecture seule).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="721"/>
        <source>Not connected to a Director.</source>
        <translation>Non connecté à un Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="729"/>
        <source>%n job(s) checked: %1 pass, %2 fail, %3 unknown. </source>
        <translation>
            <numerusform>%n job vérifié : %1 réussi(s), %2 échoué(s), %3 inconnu(s). </numerusform>
            <numerusform>%n jobs vérifiés : %1 réussi(s), %2 échoué(s), %3 inconnu(s). </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="731"/>
        <source>From %n job record(s) of the last %1 days</source>
        <translation>
            <numerusform>À partir de %n enregistrement de job des %1 derniers jours</numerusform>
            <numerusform>À partir de %n enregistrements de jobs des %1 derniers jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="732"/>
        <source>, %n volume(s)</source>
        <translation>
            <numerusform>, %n volume</numerusform>
            <numerusform>, %n volumes</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="733"/>
        <source>, the volumes of %n job(s)</source>
        <translation>
            <numerusform>, les volumes de %n job</numerusform>
            <numerusform>, les volumes de %n jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="735"/>
        <source>. Read only.</source>
        <translation>. Lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="16"/>
        <source>3-2-1 &amp;Verifier...</source>
        <translation>&amp;Vérificateur 3-2-1...</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="17"/>
        <source>Check for every backup job: 3 copies, 2 media types, 1 copy off site (read only)</source>
        <translation>Vérifier pour chaque job de sauvegarde : 3 copies, 2 types de support, 1 copie hors site (lecture seule)</translation>
    </message>
</context>
<context>
    <name>B321Verifier</name>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="97"/>
        <source>unknown time</source>
        <translation>heure inconnue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="103"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="116"/>
        <source>(pool not reported)</source>
        <translation>(pool non indiqué)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="418"/>
        <source>No successful Full backup of %1 in the job history of the last %n day(s): there is no chain to restore.</source>
        <translation>
            <numerusform>Aucune sauvegarde Full réussie de %1 dans l&apos;historique des jobs du dernier %n jour : il n&apos;y a aucune chaîne à restaurer.</numerusform>
            <numerusform>Aucune sauvegarde Full réussie de %1 dans l&apos;historique des jobs des %n derniers jours : il n&apos;y a aucune chaîne à restaurer.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="420"/>
        <source>No successful Full backup of %1 in the job history: there is no chain to restore.</source>
        <translation>Aucune sauvegarde Full réussie de %1 dans l&apos;historique des jobs : il n&apos;y a aucune chaîne à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="422"/>
        <source>Run a Full backup of %1 and check that it terminates normally.</source>
        <translation>Lancez une sauvegarde Full de %1 et vérifiez qu&apos;elle se termine normalement.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="424"/>
        <source>No successful Full backup in the job history</source>
        <translation>Aucune sauvegarde Full réussie dans l&apos;historique des jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="426"/>
        <source>Only the original data on the client counts: 1 of 3 copies.</source>
        <translation>Seules les données d&apos;origine sur le client comptent : 1 copie sur 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="439"/>
        <source>The chain a restore would use today is %1</source>
        <translation>La chaîne qu&apos;une restauration utiliserait aujourd&apos;hui est %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="440"/>
        <source>, a Differential</source>
        <translation>, une Differential</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="441"/>
        <source> and %n Incremental(s) after it</source>
        <translation>
            <numerusform> et %n Incremental après celle-ci</numerusform>
            <numerusform> et %n Incremental après celle-ci</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="442"/>
        <source>; it counts as 1 copy.</source>
        <translation>; elle compte comme 1 copie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="445"/>
        <source>The original data on the client counts as 1 copy (setting &quot;Count the original data as a copy&quot;).</source>
        <translation>Les données d&apos;origine sur le client comptent comme 1 copie (paramètre « Compter les données d&apos;origine comme une copie »).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="448"/>
        <source>The original data on the client is not counted (setting &quot;Count the original data as a copy&quot; is off).</source>
        <translation>Les données d&apos;origine sur le client ne sont pas comptées (le paramètre « Compter les données d&apos;origine comme une copie » est désactivé).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="454"/>
        <source>job %1 from job %2</source>
        <translation>job %1 issu du job %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="456"/>
        <source>Migrated chain jobs (%1): a Migrate moves the data and adds no copy; the old job is not restored any more.</source>
        <translation>Jobs de la chaîne migrés (%1) : un Migrate déplace les données et n&apos;ajoute aucune copie ; l&apos;ancien job n&apos;est plus restauré.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="462"/>
        <source>Pool %1 holds a copy of every job of the chain; it counts as 1 copy.</source>
        <translation>Le pool %1 contient une copie de chaque job de la chaîne ; il compte comme 1 copie.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="466"/>
        <source>Pool %1 holds copies of %2 of the %3 jobs of the chain (missing: %4); a partial copy does not count.</source>
        <translation>Le pool %1 contient des copies de %2 des %3 jobs de la chaîne (manquants : %4) ; une copie partielle ne compte pas.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="472"/>
        <source>No Copy job has copied the chain.</source>
        <translation>Aucun job Copy n&apos;a copié la chaîne.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="474"/>
        <source>Copies: %1 of 3.</source>
        <translation>Copies : %1 sur 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="480"/>
        <source>The volumes of the chain are not known, so neither are their media types.</source>
        <translation>Les volumes de la chaîne ne sont pas connus, leurs types de support non plus.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="481"/>
        <source>The volumes of the jobs (JobMedia) were not loaded: media types unknown.</source>
        <translation>Les volumes des jobs (JobMedia) n&apos;ont pas été chargés : types de support inconnus.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="483"/>
        <source>Media types of the chain and its complete copies: %1.</source>
        <translation>Types de support de la chaîne et de ses copies complètes : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="486"/>
        <source>A copy that spans several media types counts once; copies on media types of their own: %1.</source>
        <translation>Une copie répartie sur plusieurs types de support compte une seule fois ; copies sur des types de support propres : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="490"/>
        <source>They lie on the storages %1; the same media type on several storages counts as one type.</source>
        <translation>Elles se trouvent sur les Storages %1 ; le même type de support sur plusieurs Storages compte comme un seul type.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="495"/>
        <source>Some volumes of these jobs are not in the volume list or have no media type.</source>
        <translation>Certains volumes de ces jobs ne figurent pas dans la liste des volumes ou n&apos;ont pas de type de support.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="517"/>
        <source>storage %1 marked off site</source>
        <translation>Storage %1 marqué hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="518"/>
        <source>volume %1 has location %2 in the catalog</source>
        <translation>le volume %1 a l&apos;emplacement %2 dans le catalogue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="522"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="523"/>
        <source>the chain</source>
        <translation>la chaîne</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="524"/>
        <source>the copy in pool %1</source>
        <translation>la copie dans le pool %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="527"/>
        <source>Off site: %1.</source>
        <translation>Hors site : %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="531"/>
        <source>Off site is unknown: no storage of the chain or its copies is marked off site and no volume has a location in the catalog. Bareos itself does not record whether a storage is off site.</source>
        <translation>Hors site inconnu : aucun Storage de la chaîne ou de ses copies n&apos;est marqué hors site et aucun volume n&apos;a d&apos;emplacement dans le catalogue. Bareos lui-même n&apos;enregistre pas si un Storage est hors site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="536"/>
        <source>Nothing is off site: every copy has a volume on a storage that is on site.</source>
        <translation>Rien n&apos;est hors site : chaque copie a un volume sur un Storage situé sur site.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="542"/>
        <source>the present one</source>
        <translation>l&apos;actuelle</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="545"/>
        <source>Pool %1 lacks copies of the jobs %2: run its Copy job again (Selection Type = PoolUncopiedJobs copies what is missing) so that the whole chain is copied.</source>
        <translation>Il manque au pool %1 des copies des jobs %2 : relancez son job Copy (Selection Type = PoolUncopiedJobs copie ce qui manque) afin que toute la chaîne soit copiée.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="553"/>
        <source>Add a Copy job to a second storage with a different media type than %1 (for example tape, a removable disk or cloud storage with a Media Type of its own) (needed: %2 more).</source>
        <translation>Ajoutez un job Copy vers un second Storage ayant un type de support différent de %1 (par exemple une bande, un disque amovible ou un stockage cloud avec son propre Media Type) (encore %2 nécessaire(s)).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="558"/>
        <source>Add a Copy job to a second storage (needed: %1 more).</source>
        <translation>Ajoutez un job Copy vers un second Storage (encore %1 nécessaire(s)).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="562"/>
        <source>Write one of the copies to a storage with another media type than %1.</source>
        <translation>Écrivez l&apos;une des copies sur un Storage ayant un autre type de support que %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="566"/>
        <source>Refresh: the volumes of the jobs and the volume list are needed to tell the media types.</source>
        <translation>Actualiser : les volumes des jobs et la liste des volumes sont nécessaires pour déterminer les types de support.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="571"/>
        <source>Mark storage %1 as off site if it is at another site; if it is not, tick &quot;Unmarked storages are on site&quot;.</source>
        <translation>Marquez le Storage %1 comme hors site s&apos;il se trouve sur un autre site ; sinon, cochez « Les Storages non marqués sont sur site ».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="576"/>
        <source>Refresh: the storages of the volumes are needed to tell where the copies are.</source>
        <translation>Actualiser : les Storages des volumes sont nécessaires pour savoir où se trouvent les copies.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="580"/>
        <source>Copy the chain to a storage at another site and mark that storage as off site (or take a volume off site and record its location).</source>
        <translation>Copiez la chaîne vers un Storage situé sur un autre site et marquez ce Storage comme hors site (ou emportez un volume hors site et enregistrez son emplacement).</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="600"/>
        <source>The newest Full (job %1) is %n day(s) old</source>
        <translation>
            <numerusform>La Full la plus récente (job %1) date de %n jour</numerusform>
            <numerusform>La Full la plus récente (job %1) date de %n jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="602"/>
        <source>; this job&apos;s Fulls ran every %n day(s) (median of the loaded history).</source>
        <translation>
            <numerusform>; les Full de ce job ont été exécutées tous les %n jour (médiane de l&apos;historique chargé).</numerusform>
            <numerusform>; les Full de ce job ont été exécutées tous les %n jours (médiane de l&apos;historique chargé).</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="605"/>
        <source> It is overdue.</source>
        <translation> Elle est en retard.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="608"/>
        <source>The newest Full (job %1) is %n day(s) old; it is the only successful Full in the loaded history.</source>
        <translation>
            <numerusform>La Full la plus récente (job %1) date de %n jour ; c&apos;est la seule Full réussie de l&apos;historique chargé.</numerusform>
            <numerusform>La Full la plus récente (job %1) date de %n jours ; c&apos;est la seule Full réussie de l&apos;historique chargé.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="635"/>
        <source>The newest copy of this job (job %1 in pool %2, a copy of job %3) was made %n day(s) ago.</source>
        <translation>
            <numerusform>La copie la plus récente de ce job (job %1 dans le pool %2, une copie du job %3) a été faite il y a %n jour.</numerusform>
            <numerusform>La copie la plus récente de ce job (job %1 dans le pool %2, une copie du job %3) a été faite il y a %n jours.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="645"/>
        <source>That copy belongs to an earlier chain: the current chain (from job %1) has no complete copy in pool %2.</source>
        <translation>Cette copie appartient à une chaîne antérieure : la chaîne actuelle (à partir du job %1) n&apos;a pas de copie complète dans le pool %2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="674"/>
        <source>The chain&apos;s %1 (job %2)</source>
        <translation>%1 de la chaîne (job %2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="676"/>
        <source>The copy (job %1, pool %2) of the chain&apos;s %3</source>
        <translation>La copie (job %1, pool %2) de %3 de la chaîne</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="678"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="680"/>
        <source>%1 is on volume %2, which is in status Error.</source>
        <translation>%1 se trouve sur le volume %2, qui est à l&apos;état Error.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="683"/>
        <source>The chain&apos;s Full is on volume %1, which is in Error: run a new Full backup and check the volume.</source>
        <translation>La Full de la chaîne se trouve sur le volume %1, qui est en Error : lancez une nouvelle sauvegarde Full et vérifiez le volume.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="688"/>
        <source>%1 is on volume %2, which is %3: Bareos may overwrite it.</source>
        <translation>%1 se trouve sur le volume %2, qui est %3 : Bareos peut l&apos;écraser.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="697"/>
        <source>The job list hit its row limit: older jobs and copies may be missing from this check.</source>
        <translation>La liste des jobs a atteint sa limite de lignes : des jobs et copies plus anciens peuvent manquer dans cette vérification.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="703"/>
        <source>%1 of 3 copies</source>
        <translation>%1 copies sur 3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="704"/>
        <source>partial copies: %1</source>
        <translation>copies partielles : %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="706"/>
        <source>media types unknown</source>
        <translation>types de support inconnus</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="708"/>
        <source>media: %1 of 2 (%2)</source>
        <translation>supports : %1 sur 2 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="711"/>
        <source>off site: %1 of 1</source>
        <translation>hors site : %1 sur 1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="712"/>
        <source>off site unknown</source>
        <translation>hors site inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="713"/>
        <source>nothing off site</source>
        <translation>rien hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="866"/>
        <source>met</source>
        <translation>respectée</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="867"/>
        <source>not met</source>
        <translation>non respectée</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="870"/>
        <location filename="../src/tools/b321verifier.cpp" line="902"/>
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="876"/>
        <source>Passes</source>
        <translation>Réussit</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="877"/>
        <source>Fails</source>
        <translation>Échoue</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="880"/>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="886"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="887"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="888"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="889"/>
        <source>Virtual Full</source>
        <translation>Virtual Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="892"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="898"/>
        <source>off site</source>
        <translation>hors site</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="899"/>
        <source>on site</source>
        <translation>sur site</translation>
    </message>
</context>
<context>
    <name>BAclMatrix</name>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="62"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="63"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="64"/>
        <source>Storages</source>
        <translation>Storages</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="65"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="66"/>
        <source>Pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="67"/>
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="68"/>
        <source>Catalogs</source>
        <translation>Catalogs</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="69"/>
        <source>Commands</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="76"/>
        <source>Where</source>
        <translation>Where</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="77"/>
        <source>Plugin Options</source>
        <translation>Plugin Options</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="91"/>
        <source>The Director sent no reply.</source>
        <translation>Le Director n&apos;a pas répondu.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="92"/>
        <source>The Director did not answer with ACLs: %1</source>
        <translation>Le Director n&apos;a pas répondu avec des ACL : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="101"/>
        <source>The Director refused &quot;.acl list&quot;: %1</source>
        <translation>Le Director a refusé &quot;.acl list&quot; : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="106"/>
        <source>The reply holds no list of consoles (&quot;acl&quot;).</source>
        <translation>La réponse ne contient pas de liste de consoles (&quot;acl&quot;).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="335"/>
        <location filename="../src/director/baclmatrix.cpp" line="407"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="342"/>
        <source>Locked: the catalog ACL of the console could not be read; it may run status, .status, version and .api only.</source>
        <translation>Verrouillée : l&apos;ACL du catalogue de la console n&apos;a pas pu être lue ; elle ne peut exécuter que status, .status, version et .api.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="351"/>
        <source>Allowed (decided by the Director).</source>
        <translation>Autorisé (décidé par le Director).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="351"/>
        <location filename="../src/director/baclmatrix.cpp" line="364"/>
        <source>Allowed by %1.</source>
        <translation>Autorisé par %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="354"/>
        <source>The file ACL allows it (%1), the catalog ACL does not.</source>
        <translation>L&apos;ACL du fichier l&apos;autorise (%1), l&apos;ACL du catalogue non.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="356"/>
        <location filename="../src/director/baclmatrix.cpp" line="365"/>
        <source>Denied by %1.</source>
        <translation>Refusé par %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="359"/>
        <location filename="../src/director/baclmatrix.cpp" line="367"/>
        <source>The console has no ACL of this type: nothing is allowed.</source>
        <translation>La console n&apos;a pas d&apos;ACL de ce type : rien n&apos;est autorisé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="360"/>
        <location filename="../src/director/baclmatrix.cpp" line="368"/>
        <source>No entry of the ACL allows it.</source>
        <translation>Aucune entrée de l&apos;ACL ne l&apos;autorise.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="386"/>
        <source>Console %1</source>
        <translation>Console %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="387"/>
        <source>Profiles: none</source>
        <translation>Profils : aucun</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="388"/>
        <source>Profiles: %1</source>
        <translation>Profils : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="390"/>
        <source>Locked: its catalog ACL could not be read.</source>
        <translation>Verrouillée : son ACL du catalogue n&apos;a pas pu être lue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="393"/>
        <source>loaded</source>
        <translation>chargée</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="394"/>
        <source>off</source>
        <translation>désactivée</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="395"/>
        <source>table missing</source>
        <translation>table manquante</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="396"/>
        <source>failed</source>
        <translation>échec</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="397"/>
        <source>Catalog ACL: %1</source>
        <translation>ACL du catalogue : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="398"/>
        <source>Catalog ACL: %1 (%2)</source>
        <translation>ACL du catalogue : %1 (%2)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrix.cpp" line="403"/>
        <source>%n catalog ACL entry(s) narrow the file ACL.</source>
        <translation>
            <numerusform>%n entrée de l&apos;ACL du catalogue restreint l&apos;ACL du fichier.</numerusform>
            <numerusform>%n entrées de l&apos;ACL du catalogue restreignent l&apos;ACL du fichier.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="408"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="408"/>
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="418"/>
        <source>The Director did not answer with profiles: %1</source>
        <translation>Le Director n&apos;a pas répondu avec des profils : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="425"/>
        <source>The reply holds no profiles.</source>
        <translation>La réponse ne contient pas de profils.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="441"/>
        <source>Profile %1</source>
        <translation>Profile %1</translation>
    </message>
</context>
<context>
    <name>BAclMatrixDialog</name>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="66"/>
        <source>Change the catalog ACL</source>
        <translation>Modifier l&apos;ACL du catalogue</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="68"/>
        <source>These changes of the catalog ACL are sent to the Director, one after the other. The file ACL is not changed. The first change the Director refuses stops the rest.</source>
        <translation>Ces modifications de l&apos;ACL du catalogue sont envoyées au Director l&apos;une après l&apos;autre. L&apos;ACL du fichier n&apos;est pas modifiée. La première modification refusée par le Director arrête le reste.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="80"/>
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="102"/>
        <source>New console</source>
        <translation>Nouvelle console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="104"/>
        <source>The Director creates a Console resource with this name, password and profile (configure add console). The profile&apos;s ACLs are the frame of the console; the matrix can narrow them further through the catalog ACL afterwards.</source>
        <translation>Le Director crée une ressource Console avec ce nom, ce mot de passe et ce profil (configure add console). Les ACL du profil sont le cadre de la console ; la matrice peut ensuite les restreindre davantage par l&apos;ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="116"/>
        <source>e.g. backup-viewer</source>
        <translation>p. ex. backup-viewer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="117"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="216"/>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="123"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="124"/>
        <source>Generates another password</source>
        <translation>Génère un autre mot de passe</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="127"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="193"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="128"/>
        <source>Copies the password to the clipboard</source>
        <translation>Copie le mot de passe dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="133"/>
        <source>&amp;Password:</source>
        <translation>&amp;Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="139"/>
        <source>The Profile resource whose ACLs the console gets</source>
        <translation>La ressource Profile dont la console reçoit les ACL</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="140"/>
        <source>P&amp;rofile:</source>
        <translation>P&amp;rofil :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="142"/>
        <source>Copy the password now: whoever uses the console needs it, and it cannot be read back here.</source>
        <translation>Copiez le mot de passe maintenant : la personne qui utilise la console en a besoin, et il ne peut pas être relu ici.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="150"/>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="154"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="233"/>
        <source>Enter a name.</source>
        <translation>Saisissez un nom.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="155"/>
        <source>A console of this name exists.</source>
        <translation>Une console de ce nom existe déjà.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="156"/>
        <source>Choose a profile.</source>
        <translation>Choisissez un profil.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="177"/>
        <source>Proposed configuration text</source>
        <translation>Texte de configuration proposé</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="179"/>
        <source>Profiles are changed in the configuration files of the Director only. Put this text in place of the Profile resource in its file (the path is a suggestion), check it with the Director&apos;s test (-t) and reload; then Refresh shows the new state. Comments of the old resource are not in this text.</source>
        <translation>Les profils ne se modifient que dans les fichiers de configuration du Director. Mettez ce texte à la place de la ressource Profile dans son fichier (le chemin est une suggestion), vérifiez-le avec le test du Director (-t) et rechargez ; Actualiser montre alors le nouvel état. Les commentaires de l&apos;ancienne ressource ne figurent pas dans ce texte.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="204"/>
        <source>New profile</source>
        <translation>Nouveau profil</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="206"/>
        <source>The new profile is a proposal in the matrix: change its cells, then &quot;Config text...&quot; shows its Profile resource to put into the Director&apos;s configuration. Nothing is sent.</source>
        <translation>Le nouveau profil est une proposition dans la matrice : modifiez ses cellules, puis « Texte de configuration... » montre sa ressource Profile à mettre dans la configuration du Director. Rien n&apos;est envoyé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="215"/>
        <source>e.g. restore-only</source>
        <translation>p. ex. restore-only</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="218"/>
        <source>&amp;Description:</source>
        <translation>&amp;Description :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="220"/>
        <source>(empty: nothing allowed)</source>
        <translation>(vide : rien d&apos;autorisé)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="222"/>
        <source>The ACL entries the new profile starts with</source>
        <translation>Les entrées ACL avec lesquelles le nouveau profil commence</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="223"/>
        <source>&amp;Start as a copy of:</source>
        <translation>&amp;Commencer comme copie de :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="229"/>
        <source>Add to the matrix</source>
        <translation>Ajouter à la matrice</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="234"/>
        <source>A profile of this name exists.</source>
        <translation>Un profil de ce nom existe déjà.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="253"/>
        <source>(no entries: no restriction)</source>
        <translation>(aucune entrée : aucune restriction)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="281"/>
        <source>ACL matrix of the consoles</source>
        <translation>Matrice des ACL des consoles</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="287"/>
        <source>Consoles</source>
        <translation>Consoles</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="288"/>
        <source>Profiles</source>
        <translation>Profils</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="289"/>
        <source>Consoles: what each console may use, the catalog ACL can be changed. Profiles: the ACLs of the Profile resources, changed as proposed configuration text.</source>
        <translation>Consoles : ce que chaque console peut utiliser, l&apos;ACL du catalogue est modifiable. Profils : les ACL des ressources Profile, modifiées sous forme de texte de configuration proposé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="297"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="299"/>
        <source>Filter resources</source>
        <translation>Filtrer les ressources</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="302"/>
        <source>Only rows where the consoles differ</source>
        <translation>Seulement les lignes où les consoles diffèrent</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="305"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="306"/>
        <source>Reads the ACLs of every console again (.acl list)</source>
        <translation>Relit les ACL de toutes les consoles (.acl list)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="308"/>
        <source>Reading the ACLs of the consoles...</source>
        <translation>Lecture des ACL des consoles...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="312"/>
        <source>New console...</source>
        <translation>Nouvelle console...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="313"/>
        <source>Creates a console on the Director with a password and a profile (configure add console); it appears as a new column</source>
        <translation>Crée sur le Director une console avec mot de passe et profil (configure add console) ; elle apparaît comme nouvelle colonne</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="317"/>
        <source>New profile...</source>
        <translation>Nouveau profil...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="318"/>
        <source>Adds a proposed profile as a new column; &quot;Config text...&quot; shows its Profile resource to put into the configuration</source>
        <translation>Ajoute un profil proposé comme nouvelle colonne ; « Texte de configuration... » montre sa ressource Profile à mettre dans la configuration</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="325"/>
        <source>ACL matrix: resources by console</source>
        <translation>Matrice des ACL : ressources par console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="341"/>
        <source>✓ allowed   ✗ denied by an entry with &quot;!&quot;   · not allowed   ⊘ allowed by the file ACL, narrowed by the catalog ACL   — console locked. A cell&apos;s tool tip says why; a console&apos;s tool tip has its profiles, its catalog ACL and its Where and Plugin Options ACLs.</source>
        <translation>✓ autorisé   ✗ refusé par une entrée avec &quot;!&quot;   · non autorisé   ⊘ autorisé par l&apos;ACL du fichier, restreint par l&apos;ACL du catalogue   — console verrouillée. L&apos;info-bulle d&apos;une cellule dit pourquoi ; celle d&apos;une console montre ses profils, son ACL du catalogue et ses ACL Where et Plugin Options.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="421"/>
        <source>The profiles could not be read: %1</source>
        <translation>Les profils n&apos;ont pas pu être lus : %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="446"/>
        <source>%n profile(s). Profiles are changed in the configuration files: &quot;Config text...&quot; proposes the new text.</source>
        <translation>
            <numerusform>%n profil. Les profils se modifient dans les fichiers de configuration : « Texte de configuration... » propose le nouveau texte.</numerusform>
            <numerusform>%n profils. Les profils se modifient dans les fichiers de configuration : « Texte de configuration... » propose le nouveau texte.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="461"/>
        <source>Config text...</source>
        <translation>Texte de configuration...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="463"/>
        <source>Shows the new text of every changed profile, to put into the Director&apos;s configuration</source>
        <translation>Montre le nouveau texte de chaque profil modifié, à mettre dans la configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="467"/>
        <source>A double click (or Space) on a cell turns it in the proposal: ✓ becomes denied, · or ✗ allowed. Changed cells are underlined; &quot;Config text...&quot; shows the new Profile resources. Nothing is sent: a person puts the text in.</source>
        <translation>Un double clic (ou Espace) sur une cellule la modifie dans la proposition : ✓ devient refusé, · ou ✗ autorisé. Les cellules modifiées sont soulignées ; « Texte de configuration... » montre les nouvelles ressources Profile. Rien n&apos;est envoyé : une personne met le texte en place.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="470"/>
        <source>A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows it again. Changed cells are underlined until they are applied. What the file ACL denies is changed in the editor.</source>
        <translation>Un double clic (ou Espace) sur ✓ le refuse par l&apos;ACL du catalogue, sur ⊘ l&apos;autorise à nouveau. Les cellules modifiées sont soulignées jusqu&apos;à leur application. Ce que l&apos;ACL du fichier refuse se modifie dans l&apos;éditeur.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="363"/>
        <source>Discard</source>
        <translation>Annuler les modifications</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="364"/>
        <source>Forgets the changes that were not applied</source>
        <translation>Oublie les modifications non appliquées</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="367"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="461"/>
        <source>Apply...</source>
        <translation>Appliquer...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="368"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="464"/>
        <source>Shows every change of the catalog ACL once and sends them after your confirmation (.acl set / .acl delete)</source>
        <translation>Montre chaque modification de l&apos;ACL du catalogue une fois et les envoie après votre confirmation (.acl set / .acl delete)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="358"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="359"/>
        <source>Closes the matrix; changes that were not applied are discarded</source>
        <translation>Ferme la matrice ; les modifications non appliquées sont abandonnées</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="408"/>
        <source>%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).</source>
        <translation>
            <numerusform>%n console. Autorisé est ce que le Director a décidé (ACL du fichier et du catalogue).</numerusform>
            <numerusform>%n consoles. Autorisé est ce que le Director a décidé (ACL du fichier et du catalogue).</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="410"/>
        <source>%n console(s). This Director reports the file ACL only; the GUI decided the cells from its entries.</source>
        <translation>
            <numerusform>%n console. Ce Director ne signale que l&apos;ACL du fichier ; la GUI a déterminé les cellules à partir de ses entrées.</numerusform>
            <numerusform>%n consoles. Ce Director ne signale que l&apos;ACL du fichier ; la GUI a déterminé les cellules à partir de ses entrées.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="475"/>
        <source>No profiles read yet: Refresh reads them.</source>
        <translation>Aucun profil lu : Actualiser les lit.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="505"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="531"/>
        <source>Profile %1</source>
        <translation>Profil %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="511"/>
        <source>Profile %1 is proposed: change its cells, then &quot;Config text...&quot; shows it.</source>
        <translation>Le profil %1 est proposé : modifiez ses cellules, puis « Texte de configuration... » le montre.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="532"/>
        <source>Proposed: not on the Director yet.</source>
        <translation>Proposé : pas encore sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="534"/>
        <source>Used by no console</source>
        <translation>Utilisé par aucune console</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="534"/>
        <source>Used by: %1</source>
        <translation>Utilisé par : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="537"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="537"/>
        <source>none</source>
        <translation>aucun</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="559"/>
        <source>The proposal stays until the Director has it: after the text is in place and the Director reloaded, Refresh shows it.</source>
        <translation>La proposition reste jusqu&apos;à ce que le Director l&apos;ait : une fois le texte en place et le Director rechargé, Actualiser la montre.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="565"/>
        <source>The ACLs could not be read: %1</source>
        <translation>Les ACL n&apos;ont pas pu être lues : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="602"/>
        <source>In the proposal: allowed.</source>
        <translation>Dans la proposition : autorisé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="603"/>
        <source>In the proposal: denied.</source>
        <translation>Dans la proposition : refusé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="604"/>
        <source>Double click: change it in the proposed configuration text.</source>
        <translation>Double clic : le modifier dans le texte de configuration proposé.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="626"/>
        <source>Not applied yet: allowed again through the catalog ACL.</source>
        <translation>Pas encore appliqué : de nouveau autorisé par l&apos;ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="627"/>
        <source>Not applied yet: denied through the catalog ACL.</source>
        <translation>Pas encore appliqué : refusé par l&apos;ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="630"/>
        <source>Double click: deny it through the catalog ACL.</source>
        <translation>Double clic : le refuser par l&apos;ACL du catalogue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="631"/>
        <source>Double click: allow it again.</source>
        <translation>Double clic : l&apos;autoriser à nouveau.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="663"/>
        <source>Console %1 is locked: its catalog ACL cannot be changed until it can be read.</source>
        <translation>La console %1 est verrouillée : son ACL du catalogue ne peut pas être modifiée tant qu&apos;elle ne peut pas être lue.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="664"/>
        <source>%1 is not allowed by the file ACL of console %2: that is changed in the editor, not here.</source>
        <translation>%1 n&apos;est pas autorisé par l&apos;ACL du fichier de la console %2 : cela se modifie dans l&apos;éditeur, pas ici.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="697"/>
        <source>Discard the changes?</source>
        <translation>Abandonner les modifications ?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="698"/>
        <source>%n change(s) of the catalog ACL were not applied. Discard them and close?</source>
        <translation>
            <numerusform>%n modification de l&apos;ACL du catalogue n&apos;a pas été appliquée. L&apos;abandonner et fermer ?</numerusform>
            <numerusform>%n modifications de l&apos;ACL du catalogue n&apos;ont pas été appliquées. Les abandonner et fermer ?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="710"/>
        <source>Discard the proposals?</source>
        <translation>Abandonner les propositions ?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="711"/>
        <source>%n profile proposal(s) are only in the matrix. Discard them and close?</source>
        <translation>
            <numerusform>%n proposition de profil n&apos;existe que dans la matrice. L&apos;abandonner et fermer ?</numerusform>
            <numerusform>%n propositions de profil n&apos;existent que dans la matrice. Les abandonner et fermer ?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="751"/>
        <source>Creating console %1...</source>
        <translation>Création de la console %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="762"/>
        <source>Console %1 was not created: %2</source>
        <translation>La console %1 n&apos;a pas été créée : %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="765"/>
        <source>Console %1 was created.</source>
        <translation>La console %1 a été créée.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="780"/>
        <source>The changes were discarded.</source>
        <translation>Les modifications ont été annulées.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="807"/>
        <source>Console %1, %2:</source>
        <translation>Console %1, %2 :</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="808"/>
        <source>  now: %1</source>
        <translation>  actuel : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="809"/>
        <source>  new: %1</source>
        <translation>  nouveau : %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="811"/>
        <source>%1 of %2</source>
        <translation>%1 de %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="816"/>
        <source>Nothing was sent: the changes were not confirmed.</source>
        <translation>Rien n&apos;a été envoyé : les modifications n&apos;ont pas été confirmées.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="828"/>
        <source>Changed: %1.</source>
        <translation>Modifié : %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="831"/>
        <source>Changing %1...</source>
        <translation>Modification de %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="762"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="840"/>
        <source>refused</source>
        <translation>refusé</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="798"/>
        <source>Nothing was sent: the only entry &quot;%1&quot; has a comma outside brackets, which the Director would split into two entries.</source>
        <translation>Rien n&apos;a été envoyé : l&apos;unique entrée &quot;%1&quot; contient une virgule hors des crochets, que le Director découperait en deux entrées.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="851"/>
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="852"/>
        <source>Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.</source>
        <translation>La modification de %1 a échoué : %2. Modifié avant : %3. Rien n&apos;a été envoyé ensuite.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="877"/>
        <source>%1 (new)</source>
        <translation>%1 (nouveau)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="903"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>BBvfsModel</name>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="294"/>
        <source>Directory</source>
        <translation>Répertoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="295"/>
        <source>Symlink</source>
        <translation>Lien symbolique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="296"/>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="359"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="360"/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="361"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="362"/>
        <source>Modified</source>
        <translation>Modifié</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="572"/>
        <source>JSON parse error: %1</source>
        <translation>Erreur d&apos;analyse JSON : %1</translation>
    </message>
</context>
<context>
    <name>BBvfsReplyRouter</name>
    <message>
        <location filename="../src/jobs/bbvfsreplyrouter.cpp" line="36"/>
        <source>The connection to the Director was lost.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsreplyrouter.cpp" line="38"/>
        <source>The connection to the Director was established again.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BCapabilities</name>
    <message>
        <location filename="../src/bcapabilities.cpp" line="37"/>
        <source>Change an existing schedule</source>
        <translation>Modifier un Schedule existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="40"/>
        <source>%1 cannot change an existing schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un Schedule existant sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="44"/>
        <source>Nothing was changed. A schedule can also be changed in its configuration file on the Director.</source>
        <translation>Rien n&apos;a été modifié. Un Schedule peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="53"/>
        <source>Prune and purge all volumes</source>
        <translation>Élaguer et purger tous les volumes</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="56"/>
        <source>%1 has no command that purges all volumes at once; purge works on named volumes only and ignores retention periods.</source>
        <translation>%1 n&apos;a aucune commande qui purge tous les volumes à la fois ; purge ne fonctionne que sur des volumes nommés et ignore les durées de rétention.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="59"/>
        <source>&quot;Prune only&quot; applies the retention period of every volume; a single volume can be purged by its name with the purge command.</source>
        <translation>« Élaguer uniquement » applique la durée de rétention de chaque volume ; un volume isolé peut être purgé par son nom avec la commande purge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="66"/>
        <source>Send a FileSet to the Director</source>
        <translation>Envoyer un FileSet au Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="69"/>
        <source>%1 cannot take a FileSet through the console, because its configure command does not accept nested blocks (Include, Options, Exclude).</source>
        <translation>%1 ne peut pas recevoir de FileSet via la console, car sa commande configure n&apos;accepte pas les blocs imbriqués (Include, Options, Exclude).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="72"/>
        <source>You can copy the configuration text and place it on the Director yourself.</source>
        <translation>Vous pouvez copier le texte de configuration et le placer vous-même sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="79"/>
        <source>Delete a schedule</source>
        <translation>Supprimer un Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="82"/>
        <source>%1 cannot delete a schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un Schedule sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="86"/>
        <source>Nothing was deleted. A schedule can also be removed from the configuration files on the Director.</source>
        <translation>Rien n&apos;a été supprimé. Un Schedule peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="95"/>
        <source>Create a schedule</source>
        <translation>Créer un Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="98"/>
        <source>%1 cannot create a schedule.</source>
        <translation>%1 ne peut pas créer de Schedule.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="110"/>
        <source>Change an existing job</source>
        <translation>Modifier un job existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="113"/>
        <source>%1 cannot change an existing job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un job existant sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="117"/>
        <source>Nothing was changed. A job can also be changed in its configuration file on the Director.</source>
        <translation>Rien n&apos;a été modifié. Un job peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="126"/>
        <source>Delete a job</source>
        <translation>Supprimer un job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="129"/>
        <source>%1 cannot delete a job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un job sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="132"/>
        <source>Nothing was deleted. A job can also be removed from the configuration files on the Director.</source>
        <translation>Rien n&apos;a été supprimé. Un job peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="141"/>
        <source>Create a job</source>
        <translation>Créer un job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="144"/>
        <source>%1 cannot create a job.</source>
        <translation>%1 ne peut pas créer de job.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="152"/>
        <source>Change an existing JobDefs</source>
        <translation>Modifier un JobDefs existant</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="155"/>
        <source>%1 cannot change an existing JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas modifier un JobDefs existant sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="159"/>
        <source>Nothing was changed. A JobDefs can also be changed in its configuration file on the Director.</source>
        <translation>Rien n&apos;a été modifié. Un JobDefs peut aussi être modifié dans son fichier de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="168"/>
        <source>Delete a JobDefs</source>
        <translation>Supprimer un JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="171"/>
        <source>%1 cannot delete a JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 ne peut pas supprimer un JobDefs sur cette connexion : cela nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="175"/>
        <source>Nothing was deleted. A JobDefs can also be removed from the configuration files on the Director.</source>
        <translation>Rien n&apos;a été supprimé. Un JobDefs peut aussi être retiré des fichiers de configuration sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="184"/>
        <source>Create a JobDefs</source>
        <translation>Créer un JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="187"/>
        <source>%1 cannot create a JobDefs.</source>
        <translation>%1 ne peut pas créer de JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="202"/>
        <source>Check whether a client is reachable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="205"/>
        <source>%1 cannot check a client on this connection: in the API mode the GUI uses, the reply of &quot;status client&quot; does not say whether the client answered.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="209"/>
        <source>Nothing was sent. You can run &quot;status client=&lt;name&gt;&quot; in a console (bconsole) and read the reply.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="337"/>
        <source>This Director (Bareos)</source>
        <translation>Ce Director (Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="338"/>
        <source>This Director (Bareos %1)</source>
        <translation>Ce Director (Bareos %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="342"/>
        <source>This Director (Onesimus Director)</source>
        <translation>Ce Director (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="344"/>
        <source>This Director (Onesimus Director %1)</source>
        <translation>Ce Director (Onesimus Director %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="350"/>
        <source>This Director</source>
        <translation>Ce Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="362"/>
        <source>%1 does not offer this to this session: its configuration capabilities do not list it. The console needs the command ACL &quot;configure&quot; and the ACL of the resource type, or the Director is older than its configuration methods.</source>
        <translation>%1 ne propose pas cela à cette session : ses capacités de configuration ne le mentionnent pas. La console a besoin de l&apos;ACL de commande « configure » et de l&apos;ACL du type de ressource, ou le Director est antérieur à ses méthodes de configuration.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="371"/>
        <source>The capabilities query failed: %1.</source>
        <translation>La requête des capacités a échoué : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="375"/>
        <source>The capabilities were not received for this session.</source>
        <translation>Les capacités n&apos;ont pas été reçues pour cette session.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="384"/>
        <source>The Director lists for this session: %1.</source>
        <translation>Le Director indique pour cette session : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="401"/>
        <source>The Onesimus Director supports this over its gRPC interface.</source>
        <translation>L&apos;Onesimus Director prend cela en charge via son interface gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="404"/>
        <source>Connect to it over gRPC to use this.</source>
        <translation>Connectez-vous à lui via gRPC pour utiliser cette fonction.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="411"/>
        <source>The Onesimus Director supports this.</source>
        <translation>L&apos;Onesimus Director prend cela en charge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="416"/>
        <source>Support for this is planned for a later version of the Onesimus Director.</source>
        <translation>La prise en charge est prévue dans une version ultérieure de l&apos;Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="419"/>
        <source>The Onesimus Director is planned to support this.</source>
        <translation>L&apos;Onesimus Director doit prendre cela en charge à l&apos;avenir.</translation>
    </message>
</context>
<context>
    <name>BCapabilityNotice</name>
    <message>
        <location filename="../src/bcapabilitynotice.cpp" line="68"/>
        <source>Do not show again</source>
        <translation>Ne plus afficher</translation>
    </message>
</context>
<context>
    <name>BCatalogResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>DB Driver</source>
        <translation>Pilote de base de données</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>DB Name</source>
        <translation>Nom de la base de données</translation>
    </message>
</context>
<context>
    <name>BCertificateGenerator</name>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="33"/>
        <source>Generate TLS Certificates</source>
        <translation>Générer des certificats TLS</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="45"/>
        <source>Generate TLS certificates for secure Bareos communication.
This will create a CA certificate, client certificate, and private key.</source>
        <translation>Générer des certificats TLS pour une communication Bareos sécurisée.
Cela créera un certificat CA, un certificat client et une clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="53"/>
        <source>Certificate Settings</source>
        <translation>Paramètres du certificat</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="63"/>
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="66"/>
        <source>Output Directory:</source>
        <translation>Répertoire de sortie :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="71"/>
        <source>e.g., onesimus-client</source>
        <translation>p. ex. onesimus-client</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="72"/>
        <source>Common Name (CN):</source>
        <translation>Nom commun (CN) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="77"/>
        <source>e.g., My Company</source>
        <translation>p. ex. Mon entreprise</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="78"/>
        <source>Organization (O):</source>
        <translation>Organisation (O) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="83"/>
        <source>Days (e.g., 3650 = 10 years)</source>
        <translation>Jours (p. ex. 3650 = 10 ans)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="84"/>
        <source>Valid for (days):</source>
        <translation>Valide pendant (jours) :</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="90"/>
        <source>Empty: the CA key is not kept</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="91"/>
        <source>With a passphrase the CA key is saved encrypted (ca.key) to sign more certificates later. Without one it is not saved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="93"/>
        <source>CA key passphrase:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="99"/>
        <source>Required: protects client.pfx</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="101"/>
        <source>Optional: also writes client.pfx</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="103"/>
        <source>PFX password:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="106"/>
        <source>Confirm PFX password:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="109"/>
        <source>Overwrite existing files</source>
        <translation>Écraser les fichiers existants</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="134"/>
        <source>Generate Certificates</source>
        <translation>Générer les certificats</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="136"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="150"/>
        <source>Select Output Directory</source>
        <translation>Sélectionner le répertoire de sortie</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="328"/>
        <source>Creating PFX file...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="335"/>
        <source>PFX creation failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="188"/>
        <source>Generating...</source>
        <translation>Génération...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="199"/>
        <source>Please fill in all required fields</source>
        <translation>Veuillez remplir tous les champs obligatoires</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="201"/>
        <location filename="../src/bcertificategenerator.cpp" line="208"/>
        <location filename="../src/bcertificategenerator.cpp" line="217"/>
        <location filename="../src/bcertificategenerator.cpp" line="232"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="206"/>
        <source>The PFX passwords do not match.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="215"/>
        <source>Please enter a password to protect the PFX file.</source>
        <translation type="unfinished">Veuillez saisir un mot de passe pour protéger le fichier PFX.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="230"/>
        <source>Failed to create output directory</source>
        <translation>Impossible de créer le répertoire de sortie</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="252"/>
        <source>Certificate files already exist. Enable &apos;Overwrite&apos; to replace.</source>
        <translation>Les fichiers de certificat existent déjà. Activez « Écraser » pour les remplacer.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="254"/>
        <source>Files exist</source>
        <translation>Fichiers existants</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="264"/>
        <source>Generating CA private key (4096 bit RSA)...</source>
        <translation>Génération de la clé privée CA (RSA 4096 bits)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="174"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="272"/>
        <source>Generating CA certificate...</source>
        <translation>Génération du certificat CA...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="286"/>
        <source>Saving CA private key (encrypted with the passphrase)...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="292"/>
        <source>No CA key passphrase: the CA private key is not saved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="298"/>
        <source>Generating client private key (4096 bit RSA)...</source>
        <translation>Génération de la clé privée client (RSA 4096 bits)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="306"/>
        <source>Saving client private key...</source>
        <translation>Enregistrement de la clé privée client...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="313"/>
        <source>Generating client certificate...</source>
        <translation>Génération du certificat client...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="346"/>
        <source>Certificates generated successfully!</source>
        <translation>Certificats générés avec succès !</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="347"/>
        <source>CA Certificate: %1</source>
        <translation>Certificat CA : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="349"/>
        <source>CA Key (encrypted): %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="351"/>
        <source>Client Certificate: %1</source>
        <translation>Certificat client : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="352"/>
        <source>Private Key: %1</source>
        <translation>Clé privée : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="354"/>
        <source>PFX File: %1</source>
        <translation>Fichier PFX : %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="357"/>
        <source>Success!</source>
        <translation>Succès !</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="360"/>
        <source>Use Certificates</source>
        <translation>Utiliser les certificats</translation>
    </message>
</context>
<context>
    <name>BCheckableHeaderView</name>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="121"/>
        <source>Show All Columns</source>
        <translation>Afficher toutes les colonnes</translation>
    </message>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="131"/>
        <source>Reset Columns</source>
        <translation>Réinitialiser les colonnes</translation>
    </message>
</context>
<context>
    <name>BCleanupDialog</name>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="45"/>
        <source>Database Cleanup</source>
        <translation>Nettoyage de la base de données</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="87"/>
        <source>The connection to the Director was lost</source>
        <translation>La connexion au Director a été perdue</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="124"/>
        <source>This tool helps maintain your Bareos database by removing old or unnecessary data.
⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding.</source>
        <translation>Cet outil aide à entretenir votre base de données Bareos en supprimant les données anciennes ou inutiles.
⚠️ Attention : les opérations de nettoyage sont irréversibles. Examinez attentivement les options avant de continuer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="134"/>
        <source>Cleanup Options</source>
        <translation>Options de nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="139"/>
        <source>Remove old full backups</source>
        <translation>Supprimer les anciennes sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="142"/>
        <source>Delete old successful full backups of a job if newer full backups of the same client, job and FileSet exist. A full backup that incremental or differential backups still depend on is kept.</source>
        <translation>Supprimer les anciennes sauvegardes Full réussies d&apos;un job s&apos;il existe des sauvegardes Full plus récentes du même client, job et FileSet. Une sauvegarde Full dont dépendent encore des sauvegardes Incremental ou Differential est conservée.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="147"/>
        <source>Keep last:</source>
        <translation>Conserver les dernières :</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="154"/>
        <source> full backups</source>
        <translation> sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="155"/>
        <source>Number of recent full backups to keep per client, job and FileSet</source>
        <translation>Nombre de sauvegardes Full récentes à conserver par client, job et FileSet</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="162"/>
        <source>Remove successful jobs with no files</source>
        <translation>Supprimer les jobs réussis sans fichiers</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="164"/>
        <source>Delete jobs that completed successfully but backed up 0 files</source>
        <translation>Supprimer les jobs terminés avec succès mais ayant sauvegardé 0 fichier</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="169"/>
        <source>Prune volumes</source>
        <translation>Élaguer les volumes</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="171"/>
        <source>Apply retention policy to volumes (removes expired job records)</source>
        <translation>Appliquer la politique de rétention aux volumes (supprime les enregistrements de jobs expirés)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="174"/>
        <source>Action:</source>
        <translation>Action :</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="178"/>
        <source>Prune only</source>
        <translation>Élaguer uniquement</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="179"/>
        <source>Prune and purge</source>
        <translation>Élaguer et purger</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="180"/>
        <source>Prune removes the expired job records of all volumes according to their retention period</source>
        <translation>L&apos;élagage supprime les enregistrements de jobs expirés de tous les volumes selon leur durée de rétention</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="198"/>
        <source>Not available: Bareos has no command that purges all volumes at once. Purge works on named volumes only and ignores retention periods, so it is not sent from this dialog.</source>
        <translation>Non disponible : Bareos n&apos;a aucune commande qui purge tous les volumes à la fois. La purge ne fonctionne que sur des volumes nommés et ignore les durées de rétention ; elle n&apos;est donc pas envoyée depuis cette boîte de dialogue.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="214"/>
        <source>Remove failed jobs older than</source>
        <translation>Supprimer les jobs en échec antérieurs à</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="216"/>
        <source>Delete jobs that failed or were canceled</source>
        <translation>Supprimer les jobs en échec ou annulés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="223"/>
        <source> days</source>
        <translation> jours</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="259"/>
        <source>Select cleanup options and click &apos;Analyze&apos; to preview</source>
        <translation>Sélectionnez les options de nettoyage et cliquez sur « Analyser » pour un aperçu</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="264"/>
        <source>Analysis / Log</source>
        <translation>Analyse / Journal</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="287"/>
        <source>Analyze</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="288"/>
        <source>Preview what would be cleaned up</source>
        <translation>Prévisualiser ce qui serait nettoyé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="292"/>
        <source>Cleanup</source>
        <translation>Nettoyer</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="294"/>
        <source>Perform cleanup operations (requires analysis first)</source>
        <translation>Effectuer les opérations de nettoyage (nécessite une analyse préalable)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="282"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="88"/>
        <source>the connection to the Director was lost</source>
        <translation>la connexion au Director a été perdue</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="191"/>
        <source>Prune all volumes with &apos;Cleanup&apos;, and purge the volumes of a pool with a chosen status after a preview. Purge ignores retention periods.</source>
        <translation>Élaguer tous les volumes avec « Nettoyer », et purger les volumes d&apos;un pool ayant un statut choisi après un aperçu. Purge ignore les durées de rétention.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="240"/>
        <source>Volumes were purged. Refresh the job list to see the remaining jobs.</source>
        <translation>Des volumes ont été purgés. Actualisez la liste des jobs pour voir les jobs restants.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="400"/>
        <source>The options were changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>Les options ont été modifiées. L&apos;analyse ci-dessus n&apos;est plus valide ; cliquez de nouveau sur « Analyser ».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="409"/>
        <source>The job list has changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>La liste des jobs a changé. L&apos;analyse ci-dessus n&apos;est plus valide ; cliquez de nouveau sur « Analyser ».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="420"/>
        <location filename="../src/bcleanupdialog.cpp" line="778"/>
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="421"/>
        <location filename="../src/bcleanupdialog.cpp" line="779"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Veuillez d&apos;abord vous connecter au Director Bareos.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="427"/>
        <source>No Options Selected</source>
        <translation>Aucune option sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="428"/>
        <source>Please select at least one cleanup option.</source>
        <translation>Veuillez sélectionner au moins une option de nettoyage.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="436"/>
        <source>=== Analysis ===</source>
        <translation>=== Analyse ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="449"/>
        <location filename="../src/bcleanupdialog.cpp" line="498"/>
        <source>Analysis Required</source>
        <translation>Analyse requise</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="450"/>
        <source>Please run analysis before cleanup.</source>
        <translation>Veuillez lancer l&apos;analyse avant le nettoyage.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="457"/>
        <source>⚠️ This will permanently delete data from the Bareos catalog.

</source>
        <translation>⚠️ Cette opération supprimera définitivement des données du catalogue Bareos.

</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="458"/>
        <source>Operations to perform:
</source>
        <translation>Opérations à effectuer :
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="461"/>
        <source>- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)
</source>
        <translation>- Supprimer %1 anciens jobs de sauvegarde Full (en conservant %2 par client, job et FileSet)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="467"/>
        <source>- Delete %1 empty jobs (successful with 0 files)
</source>
        <translation>- Supprimer %1 jobs vides (réussis avec 0 fichier)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="472"/>
        <source>- Delete %1 failed jobs older than %2 days
</source>
        <translation>- Supprimer %1 jobs en échec de plus de %2 jours
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="479"/>
        <source>  Job records to delete in total: %1
</source>
        <translation>  Total des enregistrements de jobs à supprimer : %1
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="483"/>
        <source>- Prune all volumes (command: %1)
</source>
        <translation>- Élaguer tous les volumes (commande : %1)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="486"/>
        <source>
Do you want to continue?</source>
        <translation>
Voulez-vous continuer ?</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="489"/>
        <source>Confirm Cleanup</source>
        <translation>Confirmer le nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="499"/>
        <source>The job list changed in the meantime. Nothing was deleted; please run the analysis again.</source>
        <translation>La liste des jobs a changé entre-temps. Rien n&apos;a été supprimé ; veuillez relancer l&apos;analyse.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="512"/>
        <source>Stopping: no further commands are sent after the answer that is still awaited.</source>
        <translation>Arrêt : aucune autre commande n&apos;est envoyée après la réponse encore attendue.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="537"/>
        <source>Analyzing cleanup options...</source>
        <translation>Analyse des options de nettoyage...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="542"/>
        <source>⚠ No job data available!</source>
        <translation>⚠ Aucune donnée de job disponible !</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="543"/>
        <source>Please load the job list first (click &apos;Refresh&apos; in the main window).</source>
        <translation>Veuillez d&apos;abord charger la liste des jobs (cliquez sur « Actualiser » dans la fenêtre principale).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="545"/>
        <source>No job data. Please refresh job list first.</source>
        <translation>Aucune donnée de job. Veuillez d&apos;abord actualiser la liste des jobs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="552"/>
        <source>Looking at the %1 jobs currently loaded in the job list. Jobs that are not loaded there are not considered.</source>
        <translation>Examen des %1 jobs actuellement chargés dans la liste des jobs. Les jobs qui n&apos;y sont pas chargés ne sont pas pris en compte.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="569"/>
        <source>✓ Will prune all volumes (apply each volume&apos;s retention period)</source>
        <translation>✓ Élaguera tous les volumes (applique la durée de rétention de chaque volume)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="570"/>
        <source>  Command: %1</source>
        <translation>  Commande : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="572"/>
        <source>  The purge is not part of &apos;Cleanup&apos;: use &apos;Preview&apos; and &apos;Purge&apos; in &apos;Purge volumes&apos;.</source>
        <translation>  La purge ne fait pas partie de « Nettoyer » : utilisez « Aperçu » et « Purger » dans « Purger les volumes ».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="576"/>
        <source>  &apos;Prune and purge&apos; is not available and will not be sent.</source>
        <translation>  « Élaguer et purger » n&apos;est pas disponible et ne sera pas envoyé.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="581"/>
        <source>=== Analysis Summary ===</source>
        <translation>=== Résumé de l&apos;analyse ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="585"/>
        <source>Total jobs to delete: %1</source>
        <translation>Total des jobs à supprimer : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="587"/>
        <source>  - Old full backups: %1 (%2)</source>
        <translation>  - Anciennes sauvegardes Full : %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="592"/>
        <source>  - Empty jobs: %1</source>
        <translation>  - Jobs vides : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="595"/>
        <source>  - Failed jobs: %1</source>
        <translation>  - Jobs en échec : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="598"/>
        <source>No jobs found matching cleanup criteria.</source>
        <translation>Aucun job ne correspond aux critères de nettoyage.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="605"/>
        <source>Analysis complete. Click &apos;Cleanup&apos; to proceed.</source>
        <translation>Analyse terminée. Cliquez sur « Nettoyer » pour continuer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="607"/>
        <source>Analysis complete. Nothing to clean up.</source>
        <translation>Analyse terminée. Rien à nettoyer.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="613"/>
        <source>Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...</source>
        <translation>Analyse des anciennes sauvegardes Full (en conservant les %1 plus récentes par client, job et FileSet)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="621"/>
        <source>at least one full backup must be kept</source>
        <translation>au moins une sauvegarde Full doit être conservée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="624"/>
        <source>%1 job rows have no job ID</source>
        <translation>%1 lignes de jobs n&apos;ont pas d&apos;ID de job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="627"/>
        <source>%1 job rows have no job type, so backups cannot be told from other jobs</source>
        <translation>%1 lignes de jobs n&apos;ont pas de type de job ; les sauvegardes ne peuvent donc pas être distinguées des autres jobs</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="631"/>
        <source>%1 backup jobs have no client or job name</source>
        <translation>%1 jobs de sauvegarde n&apos;ont ni client ni nom de job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="636"/>
        <source>  ⚠ No old full backups are selected: %1.</source>
        <translation>  ⚠ Aucune ancienne sauvegarde Full n&apos;est sélectionnée : %1.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="640"/>
        <source>  Note: the FileSet of a job is taken from its current job configuration, not from the catalog record of each run.</source>
        <translation>  Remarque : le FileSet d&apos;un job provient de sa configuration de job actuelle, et non de l&apos;enregistrement du catalogue de chaque exécution.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="647"/>
        <source>%1 runs have no known FileSet</source>
        <translation>%1 exécutions n&apos;ont pas de FileSet connu</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="650"/>
        <source>%1 runs have no status</source>
        <translation>%1 exécutions n&apos;ont pas de statut</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="653"/>
        <source>%1 runs have a level other than Full, Differential or Incremental</source>
        <translation>%1 exécutions ont un niveau autre que Full, Differential ou Incremental</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="656"/>
        <source>%1 runs have no readable start time</source>
        <translation>%1 exécutions n&apos;ont pas d&apos;heure de début lisible</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="659"/>
        <source>  ⚠ Client &apos;%1&apos;, job &apos;%2&apos;: skipped, nothing selected (%3)</source>
        <translation>  ⚠ Client &apos;%1&apos;, job &apos;%2&apos; : ignoré, rien de sélectionné (%3)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="665"/>
        <source>  Client &apos;%1&apos;, job &apos;%2&apos;, FileSet &apos;%3&apos;: %4 full backups, will delete %5</source>
        <translation>  Client &apos;%1&apos;, job &apos;%2&apos;, FileSet &apos;%3&apos; : %4 sauvegardes Full, %5 seront supprimées</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="672"/>
        <source>    ... and %1 more</source>
        <translation>    ... et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="676"/>
        <source>    → Job %1: %2 (%3, %4)</source>
        <translation>    → Job %1 : %2 (%3, %4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="679"/>
        <source>      its dependent jobs %1 are deleted by this cleanup as well</source>
        <translation>      ses jobs dépendants %1 sont également supprimés par ce nettoyage</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="685"/>
        <source>    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it</source>
        <translation>    ⚠ Le job %1 (%2) est conservé : les jobs Incremental/Differential %3 en dépendent</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="692"/>
        <source>✓ Found %1 old full backups to delete (%2)</source>
        <translation>✓ %1 anciennes sauvegardes Full à supprimer trouvées (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="696"/>
        <source>  No old full backups found to delete</source>
        <translation>  Aucune ancienne sauvegarde Full à supprimer trouvée</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702"/>
        <source>Analyzing empty jobs (successful with 0 files)...</source>
        <translation>Analyse des jobs vides (réussis avec 0 fichier)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="707"/>
        <source>  → Job %1: %2 [%3] (%4)</source>
        <translation>  → Job %1 : %2 [%3] (%4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="712"/>
        <location filename="../src/bcleanupdialog.cpp" line="750"/>
        <source>  ... and %1 more</source>
        <translation>  ... et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="716"/>
        <source>  %1 successful jobs have no file count and are not selected</source>
        <translation>  %1 jobs réussis n&apos;ont pas de nombre de fichiers et ne sont pas sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="721"/>
        <source>✓ Found %1 empty jobs to delete</source>
        <translation>✓ %1 jobs vides à supprimer trouvés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="723"/>
        <source>  No empty jobs found</source>
        <translation>  Aucun job vide trouvé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="731"/>
        <source>Analyzing failed jobs older than %1 days (before %2)...</source>
        <translation>Analyse des jobs en échec de plus de %1 jours (avant le %2)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="740"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="741"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="742"/>
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="743"/>
        <source>Non-fatal error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="745"/>
        <source>  → Job %1: %2 [%3] (%4) - %5</source>
        <translation>  → Job %1 : %2 [%3] (%4) - %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="754"/>
        <source>✓ Found %1 failed jobs to delete</source>
        <translation>✓ %1 jobs en échec à supprimer trouvés</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="756"/>
        <source>  No failed jobs found matching criteria</source>
        <translation>  Aucun job en échec ne correspond aux critères</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="766"/>
        <source>Stop</source>
        <translation>Arrêter</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="766"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="784"/>
        <source>=== Starting Cleanup ===</source>
        <translation>=== Démarrage du nettoyage ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="791"/>
        <source>Cleanup running: commands are sent one at a time...</source>
        <translation>Nettoyage en cours : les commandes sont envoyées une par une...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="801"/>
        <source>Sent: %1</source>
        <translation>Envoyé : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="813"/>
        <source>(empty answer)</source>
        <translation>(réponse vide)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="818"/>
        <source>✓ Job %1: deletion confirmed by the Director</source>
        <translation>✓ Job %1 : suppression confirmée par le Director</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="821"/>
        <source>✗ Job %1: the Director reported an error: %2</source>
        <translation>✗ Job %1 : le Director a signalé une erreur : %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="824"/>
        <source>? Job %1: the Director did not confirm the deletion. Answer: %2</source>
        <translation>? Job %1 : le Director n&apos;a pas confirmé la suppression. Réponse : %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="828"/>
        <source>? Job %1: result unknown (%2)</source>
        <translation>? Job %1 : résultat inconnu (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="836"/>
        <source>Prune volumes: the Director answered without an error: %1</source>
        <translation>Élagage des volumes : le Director a répondu sans erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="839"/>
        <source>✗ Prune volumes: the Director reported an error: %1</source>
        <translation>✗ Élagage des volumes : le Director a signalé une erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="842"/>
        <source>? Prune volumes: result unknown (%1)</source>
        <translation>? Élagage des volumes : résultat inconnu (%1)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="845"/>
        <source>? Prune volumes: no usable answer: %1</source>
        <translation>? Élagage des volumes : aucune réponse exploitable : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="855"/>
        <source>no answer from the Director within the time limit</source>
        <translation>aucune réponse du Director dans le délai imparti</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="870"/>
        <source>=== Cleanup Result ===</source>
        <translation>=== Résultat du nettoyage ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="871"/>
        <source>Job deletions confirmed by the Director: %1</source>
        <translation>Suppressions de jobs confirmées par le Director : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="874"/>
        <source>Prune volumes: answered without an error (see the answer above)</source>
        <translation>Élagage des volumes : réponse sans erreur (voir la réponse ci-dessus)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="877"/>
        <source>Commands the Director answered with an error: %1</source>
        <translation>Commandes auxquelles le Director a répondu par une erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="880"/>
        <source>Commands with an unknown result: %1. Refresh the job list to see the actual state.</source>
        <translation>Commandes au résultat inconnu : %1. Actualisez la liste des jobs pour voir l&apos;état réel.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="884"/>
        <source>Commands not sent because the run was stopped: %1</source>
        <translation>Commandes non envoyées car l&apos;exécution a été arrêtée : %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="888"/>
        <source>Cleanup finished: the Director confirmed all %1 commands.</source>
        <translation>Nettoyage terminé : le Director a confirmé les %1 commandes.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="891"/>
        <source>Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. See the log.</source>
        <translation>Le nettoyage n&apos;est pas terminé : %1 confirmé(s), %2 en échec, %3 inconnu(s), %4 non envoyé(s). Consultez le journal.</translation>
    </message>
</context>
<context>
    <name>BClientDetailsDialog</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="181"/>
        <source>Client Details - %1</source>
        <translation>Détails du client - %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="304"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="208"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="290"/>
        <source>General Information</source>
        <translation>Informations générales</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="297"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="299"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="397"/>
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="305"/>
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="306"/>
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="307"/>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="308"/>
        <source>Operating System:</source>
        <translation>Système d&apos;exploitation :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="313"/>
        <source>Connection Information</source>
        <translation>Informations de connexion</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="327"/>
        <source>Recent (%1)</source>
        <translation>Récent (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="332"/>
        <source>Stale (%1)</source>
        <translation>Ancien (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="336"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="344"/>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="339"/>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="357"/>
        <source>Last backup:</source>
        <translation>Dernière sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="358"/>
        <source>Last Connection:</source>
        <translation>Dernière connexion :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="364"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="441"/>
        <source>Check Now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="365"/>
        <source>Ask the Director whether it can reach this client now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="371"/>
        <source>Reachable:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="391"/>
        <source>Retention Policies</source>
        <translation>Politiques de rétention</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="396"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="396"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="401"/>
        <source>Auto Prune:</source>
        <translation>Auto Prune :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="402"/>
        <source>File Retention:</source>
        <translation>File Retention :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="403"/>
        <source>Job Retention:</source>
        <translation>Job Retention :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="409"/>
        <source>Information</source>
        <translation>Informations</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="420"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="441"/>
        <source>Checking...</source>
        <translation type="unfinished">Vérification...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="424"/>
        <source>Not known: no client-initiated connection, client event or check holds now. The last backup above is an estimate, not a check.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="460"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="493"/>
        <source>Recent jobs for this client:</source>
        <translation>Jobs récents de ce client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="497"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="536"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="559"/>
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="545"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="546"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="548"/>
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="550"/>
        <source>Total Jobs:</source>
        <translation>Total des jobs :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="551"/>
        <source>Successful Jobs:</source>
        <translation>Jobs réussis :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="552"/>
        <source>Failed Jobs:</source>
        <translation>Jobs en échec :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="553"/>
        <source>Total Data:</source>
        <translation>Total des données :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="554"/>
        <source>Total Files:</source>
        <translation>Total des fichiers :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="528"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
</context>
<context>
    <name>BClientJobsModel</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="88"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="89"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="90"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="99"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="100"/>
        <source>Running</source>
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="101"/>
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="102"/>
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="103"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="104"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="105"/>
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="106"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="136"/>
        <source>Job ID</source>
        <translation>ID du job</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="137"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="138"/>
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="139"/>
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="140"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="141"/>
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="142"/>
        <source>Bytes</source>
        <translation>Octets</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="143"/>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
</context>
<context>
    <name>BClientResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Catalog</source>
        <translation>Catalog</translation>
    </message>
</context>
<context>
    <name>BClientsModel</name>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="199"/>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="262"/>
        <source>Client: %1</source>
        <translation>Client : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="266"/>
        <source>Address: %1:%2</source>
        <translation>Adresse : %1:%2</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="276"/>
        <source>%1 bytes</source>
        <translation>%1 octets</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="295"/>
        <source>Connected while the client holds a connection to the Director (client-initiated connections from &quot;status director&quot;, or client events of the Onesimus Director); Online or Offline after a client event or &quot;Check now&quot;; otherwise the age of the last backup or contact, an estimate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="304"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="306"/>
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1076"/>
        <source>The client answered &quot;Check now&quot; at %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1077"/>
        <source>The Director could not connect to the client at %1 (&quot;Check now&quot;).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1091"/>
        <source>Check at %1: the client answered.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1093"/>
        <source>Check at %1: the Director could not connect to the client.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1095"/>
        <source>Check at %1: the client is disabled on the Director, so it was not contacted.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1097"/>
        <source>Check at %1: the reply does not tell whether the client answered.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1099"/>
        <source>Check at %1: no answer in time; the state is unknown.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1102"/>
        <source>Check at %1 failed; the state is unknown.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1103"/>
        <source>Check at %1 failed: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1106"/>
        <source>Check at %1: not possible on this connection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1107"/>
        <source>Check at %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="42"/>
        <source>No connection to Director</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="47"/>
        <source>Loading clients...</source>
        <translation>Chargement des clients...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="995"/>
        <source>Checking client %1...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1054"/>
        <source>Online</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1053"/>
        <source>Offline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1054"/>
        <source>Connected</source>
        <translation type="unfinished">Connecté</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1064"/>
        <source>The client holds a client-initiated connection to the Director, connected at %1 (&quot;status director&quot;).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1066"/>
        <source>The client held a client-initiated connection to the Director at %1 (&quot;status director&quot;). It is no longer in the list, which also happens while it runs a job.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1071"/>
        <source>The Director has an open connection to the client since %1 (client event of the Director).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1073"/>
        <source>The Director reached the client at %1 (client event of the Director).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="1074"/>
        <source>The Director could not reach the client at %1 (client event of the Director).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="270"/>
        <source>FD Port: %1</source>
        <translation>Port FD : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="292"/>
        <source>From &quot;list backups client=&quot;: all backups of the client in the catalog, not only the jobs on the current page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="308"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="310"/>
        <source>Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="318"/>
        <source>Backups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="312"/>
        <source>Operating System</source>
        <translation>Système d&apos;exploitation</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="112"/>
        <source>Checking the client now...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="314"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="316"/>
        <source>Last Connection</source>
        <translation>Dernière connexion</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="320"/>
        <source>Total Data</source>
        <translation>Total des données</translation>
    </message>
</context>
<context>
    <name>BClientsWidget</name>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="40"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="41"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="62"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="65"/>
        <source>Filter by client...</source>
        <translation>Filtrer par client...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="72"/>
        <source>Last backup:</source>
        <translation>Dernière sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="73"/>
        <source>All</source>
        <translation>Tous</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="74"/>
        <source>Recent</source>
        <translation>Récent</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="75"/>
        <source>Stale</source>
        <translation>Ancien</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="76"/>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="77"/>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="84"/>
        <source>Enable automatic refresh</source>
        <translation>Activer l&apos;actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="97"/>
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="100"/>
        <source>Total:</source>
        <translation>Total :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="121"/>
        <source>Recent:</source>
        <translation>Récents :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="122"/>
        <source>Clients whose last backup or contact is younger than the limit in the settings</source>
        <translation>Clients dont la dernière sauvegarde ou le dernier contact est plus récent que la limite définie dans les paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="124"/>
        <source>Stale:</source>
        <translation>Anciens :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="125"/>
        <source>Clients whose last backup or contact is older than the limit in the settings</source>
        <translation>Clients dont la dernière sauvegarde ou le dernier contact est plus ancien que la limite définie dans les paramètres</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="127"/>
        <source>Never:</source>
        <translation>Jamais :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="128"/>
        <source>Clients without any known backup or contact</source>
        <translation>Clients sans aucune sauvegarde ni aucun contact connu</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="130"/>
        <source>Unknown:</source>
        <translation>Inconnus :</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="131"/>
        <source>Clients whose last backup time cannot be read</source>
        <translation>Clients dont l&apos;heure de la dernière sauvegarde ne peut pas être lue</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="392"/>
        <source>Error parsing client data</source>
        <translation>Erreur lors de l&apos;analyse des données du client</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientswidget.cpp" line="433"/>
        <source>%n client(s) loaded</source>
        <translation>
            <numerusform>%n client chargé</numerusform>
            <numerusform>%n clients chargés</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="543"/>
        <source>Details / Export...</source>
        <translation>Détails / Exporter...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="554"/>
        <source>Check Now</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="555"/>
        <source>Check Now (checking %1...)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="556"/>
        <source>Ask the Director whether it can reach this client now</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BConfigImportDialog</name>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="36"/>
        <source>Import Director Configuration</source>
        <translation>Importer la configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="62"/>
        <source>Configuration Source</source>
        <translation>Source de la configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="66"/>
        <source>Select directory or ZIP file...</source>
        <translation>Sélectionner un répertoire ou un fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="70"/>
        <source>Directory...</source>
        <translation>Répertoire...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="75"/>
        <source>ZIP File...</source>
        <translation>Fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="80"/>
        <source>Parse</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="104"/>
        <source>Connection Preview</source>
        <translation>Aperçu de la connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="108"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="175"/>
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="113"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="204"/>
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="117"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="211"/>
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="121"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="189"/>
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="125"/>
        <source>TLS:</source>
        <translation>TLS :</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="141"/>
        <source>Import</source>
        <translation>Importer</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="148"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="172"/>
        <source>Connection Settings</source>
        <translation>Paramètres de connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="206"/>
        <source>hostname or IP address</source>
        <translation>nom d&apos;hôte ou adresse IP</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="207"/>
        <source>Enter the hostname or IP address of the Director to connect to</source>
        <translation>Saisissez le nom d&apos;hôte ou l&apos;adresse IP du Director auquel se connecter</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="215"/>
        <source>Port number (default: 9101)</source>
        <translation>Numéro de port (par défaut : 9101)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="218"/>
        <source>Create connection profile</source>
        <translation>Créer un profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="318"/>
        <source>Reading ZIP archive...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="322"/>
        <source>No configuration resources found in the archive.</source>
        <translation type="unfinished">Aucune ressource de configuration trouvée dans l&apos;archive.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="327"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="343"/>
        <source>Entries not read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="328"/>
        <source>These entries of the archive were not read:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="224"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="411"/>
        <source>Director</source>
        <translation>Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="229"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="412"/>
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="232"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="413"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="235"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="414"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="238"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="415"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="241"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="416"/>
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="244"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="417"/>
        <source>Pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="247"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="418"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="250"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="419"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="253"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="420"/>
        <source>Catalogs</source>
        <translation>Catalogs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="264"/>
        <source>Select Director Configuration Directory</source>
        <translation>Sélectionner le répertoire de configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="271"/>
        <source>Ready to parse directory</source>
        <translation>Prêt à analyser le répertoire</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="285"/>
        <source>Select Configuration ZIP Archive</source>
        <translation>Sélectionner l&apos;archive ZIP de configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="287"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="292"/>
        <source>Ready to parse ZIP archive</source>
        <translation>Prêt à analyser l&apos;archive ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="307"/>
        <source>Parsing...</source>
        <translation>Analyse...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="350"/>
        <source>Parsed %1: %2</source>
        <translation>%1 analysé : %2</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="357"/>
        <source>Parsing complete: %1 resources found</source>
        <translation>Analyse terminée : %1 ressources trouvées</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="387"/>
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="390"/>
        <source>Parsing Error</source>
        <translation>Erreur d&apos;analyse</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="449"/>
        <source>(multiple addresses)</source>
        <translation>(plusieurs adresses)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="456"/>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="456"/>
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="515"/>
        <source>Invalid Selection</source>
        <translation>Sélection invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="516"/>
        <source>Please select both a Director and a Console.</source>
        <translation>Veuillez sélectionner à la fois un Director et une Console.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="521"/>
        <source>Invalid Address</source>
        <translation>Adresse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="522"/>
        <source>Please enter the hostname or IP address of the Director.</source>
        <translation>Veuillez saisir le nom d&apos;hôte ou l&apos;adresse IP du Director.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="560"/>
        <source>%1 &apos;%2&apos;: Missing required field &apos;%3&apos;</source>
        <translation>%1 &apos;%2&apos; : champ obligatoire &apos;%3&apos; manquant</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="581"/>
        <source>&lt;b&gt;Validation Warnings (%1 total, showing first 10):&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Avertissements de validation (%1 au total, affichage des 10 premiers) :&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="583"/>
        <source>&lt;b&gt;Validation Warnings:&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Avertissements de validation :&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="344"/>
        <source>These parts of the configuration were not read:
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BConfigParser</name>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="381"/>
        <location filename="../src/config/bconfigparser.cpp" line="432"/>
        <source>%1: the quoted string has no closing quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="411"/>
        <source>%1: the file name after @ is missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="120"/>
        <source>%1: more than %2 MiB of configuration text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="604"/>
        <source>%1: UTF-16 files cannot be read; save the file as UTF-8</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="728"/>
        <source>%1: @%2 was not read (includes are only followed when one file is opened)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="734"/>
        <source>%1: @%2 was not run (commands are never run)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="738"/>
        <source>%1: @%2 was not read (relative path)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="742"/>
        <source>%1: includes nested deeper than %2 levels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="749"/>
        <source>%1: cannot open the included file %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="755"/>
        <source>%1: %2 includes itself</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="759"/>
        <source>%1: more than %2 included files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="785"/>
        <source>%1: expected a resource type, got &quot;%2&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="792"/>
        <source>%1: expected &quot;{&quot; after the resource type %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="807"/>
        <source>%1: the %2 resource has no Name and was left out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="821"/>
        <source>%1: blocks nested deeper than %2 levels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="832"/>
        <source>%1: the text ends before the closing &quot;}&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="853"/>
        <source>%1: expected &quot;=&quot; after %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="859"/>
        <source>%1: unexpected &quot;%2&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bconfigparser.cpp" line="877"/>
        <location filename="../src/config/bconfigparser.cpp" line="889"/>
        <source>%1: expected a value for %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BConnectionWizard</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="62"/>
        <source>Connection Setup</source>
        <translation>Configuration de la connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="88"/>
        <source>New Connection</source>
        <translation>Nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="198"/>
        <source>SHA-256 fingerprint of the server certificate:</source>
        <translation>Empreinte SHA-256 du certificat du serveur :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="205"/>
        <source>Server certificate changed</source>
        <translation>Le certificat du serveur a changé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="206"/>
        <source>The certificate of the Director at %1:%2 has changed since the last connection.</source>
        <translation>Le certificat du Director à l&apos;adresse %1:%2 a changé depuis la dernière connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="208"/>
        <source>Previously trusted:</source>
        <translation>Précédemment approuvé :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="210"/>
        <source>This is expected if the certificate was regenerated on the Director. Otherwise someone may be intercepting the connection.</source>
        <translation>C&apos;est normal si le certificat a été régénéré sur le Director. Sinon, quelqu&apos;un intercepte peut-être la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="213"/>
        <source>Trust server certificate?</source>
        <translation>Approuver le certificat du serveur ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="214"/>
        <source>The Director at %1:%2 uses a self-signed certificate that has not been trusted yet.</source>
        <translation>Le Director à l&apos;adresse %1:%2 utilise un certificat auto-signé qui n&apos;a pas encore été approuvé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="220"/>
        <source>The same fingerprint is announced in the local network (mDNS). This is not proof of the Director&apos;s identity: any device in the network can send such an announcement.</source>
        <translation>La même empreinte est annoncée sur le réseau local (mDNS). Ce n&apos;est pas une preuve de l&apos;identité du Director : n&apos;importe quel appareil du réseau peut envoyer une telle annonce.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="225"/>
        <source>The fingerprint does NOT match the one announced in the network (mDNS)!</source>
        <translation>L&apos;empreinte NE correspond PAS à celle annoncée sur le réseau (mDNS) !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="228"/>
        <source>Before trusting it, compare the fingerprint with the one of the Director&apos;s gRPC certificate, obtained from the Director&apos;s administrator or read on the Director itself.</source>
        <translation>Avant de l&apos;approuver, comparez l&apos;empreinte avec celle du certificat gRPC du Director, obtenue auprès de l&apos;administrateur du Director ou lue sur le Director lui-même.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="235"/>
        <source>Trust certificate</source>
        <translation>Approuver le certificat</translation>
    </message>
</context>
<context>
    <name>BConsoleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="112"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="112"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="112"/>
        <source>ACLs</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="125"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="125"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="133"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
</context>
<context>
    <name>BDatabase</name>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="40"/>
        <source>Failed to create database directory: %1</source>
        <translation>Impossible de créer le répertoire de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="50"/>
        <source>Failed to open database: %1</source>
        <translation>Impossible d&apos;ouvrir la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="71"/>
        <source>Failed to create database schema: %1</source>
        <translation>Impossible de créer le schéma de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="91"/>
        <source>Failed to apply database migrations: %1</source>
        <translation>Impossible d&apos;appliquer les migrations de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="103"/>
        <source>Failed to repair database schema: %1</source>
        <translation>Impossible de réparer le schéma de la base de données : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="209"/>
        <source>No migration script for database version %1</source>
        <translation>Aucun script de migration pour la version %1 de la base de données</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="224"/>
        <source>Failed to apply migration to version %1: %2</source>
        <translation>Impossible d&apos;appliquer la migration vers la version %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="229"/>
        <source>Migration to version %1 did not record its version</source>
        <translation>La migration vers la version %1 n&apos;a pas enregistré sa version</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="260"/>
        <source>Table %1 is still missing after the repair</source>
        <translation>La table %1 est toujours absente après la réparation</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="273"/>
        <source>Failed to open SQL resource: %1</source>
        <translation>Impossible d&apos;ouvrir la ressource SQL : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="289"/>
        <source>Failed to start transaction: %1</source>
        <translation>Impossible de démarrer la transaction : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="320"/>
        <source>SQL execution failed: %1
Statement: %2</source>
        <translation>Échec de l&apos;exécution SQL : %1
Instruction : %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="334"/>
        <source>Failed to commit transaction: %1</source>
        <translation>Impossible de valider la transaction : %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="485"/>
        <source>Database not ready</source>
        <translation>Base de données non prête</translation>
    </message>
</context>
<context>
    <name>BDirectiveSchema</name>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="283"/>
        <source>Required directive &apos;%1&apos; is missing</source>
        <translation>La directive obligatoire &apos;%1&apos; est manquante</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="295"/>
        <source>&apos;%1&apos; must be an integer</source>
        <translation>&apos;%1&apos; doit être un entier</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="298"/>
        <source>&apos;%1&apos; must be &gt;= %2</source>
        <translation>&apos;%1&apos; doit être &gt;= %2</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="301"/>
        <source>&apos;%1&apos; must be &lt;= %2</source>
        <translation>&apos;%1&apos; doit être &lt;= %2</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="318"/>
        <source>&apos;%1&apos; must be yes or no</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="324"/>
        <source>&apos;%1&apos; must be a size such as 500 mb or 2 gb</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="330"/>
        <source>&apos;%1&apos; must be a time such as 30 days or 1 hour 30 min</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BDirectorModel</name>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="26"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="27"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="28"/>
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="29"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="30"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="31"/>
        <source>Description</source>
        <translation>Description</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="32"/>
        <source>DIR Port</source>
        <translation>Port DIR</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="33"/>
        <source>Query File</source>
        <translation>Fichier de requêtes</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="34"/>
        <source>Working Dir</source>
        <translation>Répertoire de travail</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="35"/>
        <source>PID Dir</source>
        <translation>Répertoire PID</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="36"/>
        <source>Scripts Dir</source>
        <translation>Répertoire des scripts</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="37"/>
        <source>Plugin Dir</source>
        <translation>Répertoire des plugins</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="38"/>
        <source>SubSys Dir</source>
        <translation>Répertoire SubSys</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="39"/>
        <source>Max Jobs</source>
        <translation>Jobs max.</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="40"/>
        <source>TLS Enable</source>
        <translation>TLS Enable</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="41"/>
        <source>TLS Require</source>
        <translation>TLS Require</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="42"/>
        <source>TLS Verify</source>
        <translation>TLS Verify</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="43"/>
        <source>TLS CA Cert</source>
        <translation>TLS CA Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="44"/>
        <source>TLS Cert</source>
        <translation>TLS Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="45"/>
        <source>TLS Key</source>
        <translation>TLS Key</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="46"/>
        <source>TLS Allowed CN</source>
        <translation>TLS Allowed CN</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="47"/>
        <source>System</source>
        <translation>Système</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="48"/>
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="49"/>
        <source>Updated</source>
        <translation>Mis à jour</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="50"/>
        <source>Last Connected</source>
        <translation>Dernière connexion</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="51"/>
        <source>Active</source>
        <translation>Actif</translation>
    </message>
</context>
<context>
    <name>BDirectorResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="39"/>
        <source>(multiple)</source>
        <translation>(plusieurs)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="48"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="48"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BEditableListModel</name>
    <message>
        <location filename="../src/config/beditablelistmodel.cpp" line="123"/>
        <source>Add %1 items</source>
        <translation>Ajouter %1 éléments</translation>
    </message>
</context>
<context>
    <name>BEditableListWidget</name>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="80"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="113"/>
        <source>Add...</source>
        <translation>Ajouter...</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="81"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="126"/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="118"/>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="249"/>
        <source>Add item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="254"/>
        <source>Remove selected</source>
        <translation>Supprimer la sélection</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="393"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="450"/>
        <source>Select Directory</source>
        <translation>Sélectionner un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="395"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="452"/>
        <source>Select File</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="400"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="401"/>
        <source>Enter value:</source>
        <translation>Saisir une valeur :</translation>
    </message>
</context>
<context>
    <name>BFileSetDocument</name>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="174"/>
        <source>Line %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="223"/>
        <source>a quoted value is not closed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="253"/>
        <source>blocks are nested too deeply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="258"/>
        <source>a closing brace is missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="264"/>
        <source>a closing brace has no opening brace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="269"/>
        <source>@include is not read here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="283"/>
        <source>a directive name is missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="289"/>
        <source>&quot;%1&quot; is followed by neither = nor {</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="761"/>
        <source>The text is not one FileSet resource.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1029"/>
        <source>A value contains a line break or another control character.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1046"/>
        <source>FileSet name is required</source>
        <translation>Le nom du FileSet est obligatoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1049"/>
        <source>The name may contain only letters, digits, blanks and the characters : . - _ / (no blank at the start or end, at most 127 characters)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1055"/>
        <source>At least one Include block is required</source>
        <translation>Au moins un bloc Include est requis</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1063"/>
        <source>Include block %1 has no file paths</source>
        <translation>Le bloc Include %1 n&apos;a aucun chemin de fichier</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1067"/>
        <source>At least one Include block must have file paths</source>
        <translation>Au moins un bloc Include doit avoir des chemins de fichiers</translation>
    </message>
</context>
<context>
    <name>BFileSetExcludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="982"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="997"/>
        <source>Global Exclude</source>
        <translation>Exclusion globale</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="983"/>
        <source>Configure paths and patterns to exclude from all Include blocks.</source>
        <translation>Configurer les chemins et motifs à exclure de tous les blocs Include.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="993"/>
        <source>Exclude Paths</source>
        <translation>Chemins exclus</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="998"/>
        <source>Enter path to exclude...</source>
        <translation>Saisir le chemin à exclure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="999"/>
        <source>+ Add Path</source>
        <translation>+ Ajouter un chemin</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1005"/>
        <source>These paths will be excluded from all backup jobs using this FileSet.
Use absolute paths (e.g., /tmp, /var/cache) or patterns.</source>
        <translation>Ces chemins seront exclus de tous les jobs de sauvegarde utilisant ce FileSet.
Utilisez des chemins absolus (p. ex. /tmp, /var/cache) ou des motifs.</translation>
    </message>
</context>
<context>
    <name>BFileSetIncludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="875"/>
        <source>Include Blocks</source>
        <translation>Blocs Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="876"/>
        <source>Configure which files and directories to include in the backup.</source>
        <translation>Configurer les fichiers et répertoires à inclure dans la sauvegarde.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="889"/>
        <source>+ Add Include Block</source>
        <translation>+ Ajouter un bloc Include</translation>
    </message>
</context>
<context>
    <name>BFileSetPreviewPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1038"/>
        <source>Preview &amp; Deploy</source>
        <translation>Aperçu &amp; déploiement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1039"/>
        <source>Review the generated FileSet configuration. Finish sends it to the Director where the Director accepts FileSets; otherwise copy it for manual deployment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1052"/>
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1058"/>
        <source>Copy configuration</source>
        <translation>Copier la configuration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1116"/>
        <source># Finish writes this text over the FileSet %1 on the Director, after the
# Director has checked it and you have confirmed the change.

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1118"/>
        <source># Finish creates this FileSet on the Director, after the Director has
# checked it and you have confirmed it.

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1121"/>
        <source># NOTE: Bareos Director does not support &apos;configure add fileset&apos; with
# nested Include/Exclude/Options blocks (confirmed Bareos limitation).
# Deploy this configuration manually:
#   1. Copy to /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Run: systemctl reload bareos-dir
#
# Use &apos;Copy configuration&apos; to copy the text below.

</source>
        <translation># REMARQUE : le Director Bareos ne prend pas en charge &apos;configure add fileset&apos; avec
# des blocs Include/Exclude/Options imbriqués (limitation Bareos confirmée).
# Déployez cette configuration manuellement :
#   1. Copiez-la dans /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Exécutez : systemctl reload bareos-dir
#
# Utilisez « Copier la configuration » pour copier le texte ci-dessous.

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1154"/>
        <source>Configuration is valid</source>
        <translation>La configuration est valide</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1177"/>
        <source>FileSet not complete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1189"/>
        <source>Close Wizard</source>
        <translation>Fermer l&apos;assistant</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1190"/>
        <source>The configuration has not been sent to the Director.
Use &apos;Copy configuration&apos; to save it before closing.

Do you want to close the wizard?</source>
        <translation>La configuration n&apos;a pas été envoyée au Director.
Utilisez « Copier la configuration » pour l&apos;enregistrer avant de fermer.

Voulez-vous fermer l&apos;assistant ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1208"/>
        <source>Another change is still being written. Try again when it is done.</source>
        <translation type="unfinished">Une autre modification est encore en cours d&apos;écriture. Réessayez lorsqu&apos;elle sera terminée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1232"/>
        <source>Nothing was written.</source>
        <translation type="unfinished">Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1244"/>
        <source>The Director checks the FileSet...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1247"/>
        <source>The wizard writes the whole FileSet as it generates it: every directive is kept, comments and the layout of the file are not.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1251"/>
        <source>A new FileSet. No job uses it until a job names it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1268"/>
        <source>Configuration copied to clipboard</source>
        <translation>Configuration copiée dans le presse-papiers</translation>
    </message>
</context>
<context>
    <name>BFileSetResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="357"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="357"/>
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BFileSetSettingsPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="556"/>
        <source>FileSet Settings</source>
        <translation>Paramètres du FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="557"/>
        <source>Configure the FileSet name and basic options.</source>
        <translation>Configurer le nom du FileSet et les options de base.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="571"/>
        <source>Select FileSet:</source>
        <translation>Sélectionner un FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="574"/>
        <source>Loading FileSets...</source>
        <translation>Chargement des FileSets...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="592"/>
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="596"/>
        <source>e.g., LinuxFullSystem</source>
        <translation>p. ex. LinuxFullSystem</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="597"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="600"/>
        <source>Optional description</source>
        <translation>Description facultative</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="601"/>
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="606"/>
        <source>Global Options</source>
        <translation>Options globales</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="609"/>
        <source>Enable VSS</source>
        <translation>Activer VSS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="610"/>
        <source>Enable Volume Shadow Copy Service (Windows)</source>
        <translation>Activer le service de cliché instantané de volume (Windows)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="613"/>
        <source>Ignore FileSet Changes</source>
        <translation>Ignorer les modifications du FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="614"/>
        <source>Don&apos;t trigger Full backup when FileSet changes</source>
        <translation>Ne pas déclencher de sauvegarde Full lorsque le FileSet change</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="636"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="692"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="804"/>
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="712"/>
        <source>No Director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="766"/>
        <source>-- Select FileSet --</source>
        <translation>-- Sélectionner un FileSet --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="772"/>
        <source>(%1 FileSets)</source>
        <translation>(%1 FileSets)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="814"/>
        <source>Loaded</source>
        <translation>Chargé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="850"/>
        <source>Loaded from &quot;show filesets&quot;: Enable VSS and Ignore FileSet Changes are not reported there; check them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="865"/>
        <source>Config not cached</source>
        <translation>Configuration non mise en cache</translation>
    </message>
</context>
<context>
    <name>BFileSetTemplatePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="189"/>
        <source>Select FileSet Template</source>
        <translation>Sélectionner un modèle de FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="190"/>
        <source>Choose a preset template to start with, or mix multiple templates together.</source>
        <translation>Choisissez un modèle prédéfini pour commencer, ou combinez plusieurs modèles.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="199"/>
        <source>Template:</source>
        <translation>Modèle :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="202"/>
        <source>Mix Templates...</source>
        <translation>Combiner des modèles...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="204"/>
        <source>Combine paths from multiple templates</source>
        <translation>Combiner les chemins de plusieurs modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="217"/>
        <source>Template Description</source>
        <translation>Description du modèle</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="232"/>
        <source>Paths Preview</source>
        <translation>Aperçu des chemins</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="237"/>
        <source>&lt;b&gt;Include:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Include :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="250"/>
        <source>&lt;b&gt;Exclude:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Exclude :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="333"/>
        <source>Platform: Windows</source>
        <translation>Plateforme : Windows</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="335"/>
        <source>Platform: Linux/Unix</source>
        <translation>Plateforme : Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="337"/>
        <source>Platform: macOS</source>
        <translation>Plateforme : macOS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="339"/>
        <source>Platform: Any</source>
        <translation>Plateforme : toutes</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="352"/>
        <source>Mix Templates</source>
        <translation>Combiner des modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="357"/>
        <source>Select templates to combine:</source>
        <translation>Sélectionnez les modèles à combiner :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="375"/>
        <source>%1
Platform: %2</source>
        <translation>%1
Plateforme : %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="382"/>
        <source>Combined Paths Preview</source>
        <translation>Aperçu des chemins combinés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="431"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="433"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="541"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="545"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="510"/>
        <source>Mixed_%1</source>
        <translation>Mixed_%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="511"/>
        <source>Combined FileSet from %1 templates</source>
        <translation>FileSet combiné à partir de %1 modèles</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="513"/>
        <source>Mixed: %1 templates selected</source>
        <translation>Combiné : %1 modèles sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="517"/>
        <source>Templates: %1</source>
        <translation>Modèles : %1</translation>
    </message>
</context>
<context>
    <name>BFileSetWizard</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="104"/>
        <source>New FileSet Wizard</source>
        <translation>Assistant de nouveau FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="104"/>
        <source>Edit FileSet</source>
        <translation>Modifier le FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="127"/>
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
</context>
<context>
    <name>BGrpcAddressProbe</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="176"/>
        <source>no TLS connection within %1 s</source>
        <translation>aucune connexion TLS en %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="213"/>
        <source>Cannot resolve the host name %1: %2</source>
        <translation>Impossible de résoudre le nom d&apos;hôte %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="214"/>
        <source>no address</source>
        <translation>aucune adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="238"/>
        <source>Cannot resolve the host name %1: no address</source>
        <translation>Impossible de résoudre le nom d&apos;hôte %1 : aucune adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="251"/>
        <source>Cannot reach the gRPC endpoint %1:%2 (%3)</source>
        <translation>Impossible de joindre le point de terminaison gRPC %1:%2 (%3)</translation>
    </message>
</context>
<context>
    <name>BGrpcConnect</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="92"/>
        <source>%1 - the Director no longer knows this session (it ends after 8 hours without a request or when the Director restarts). Connect again.</source>
        <translation>%1 - le Director ne connaît plus cette session (elle se termine après 8 heures sans requête ou au redémarrage du Director). Reconnectez-vous.</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="99"/>
        <source>the call was cancelled</source>
        <translation>l&apos;appel a été annulé</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="103"/>
        <source>no connection or no reply within %1 s</source>
        <translation>aucune connexion ou aucune réponse en %1 s</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="104"/>
        <source>no connection or no reply in time</source>
        <translation>aucune connexion ou aucune réponse à temps</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="107"/>
        <source>this console is not allowed to do that</source>
        <translation>cette console n&apos;est pas autorisée à faire cela</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="110"/>
        <source>the Director does not offer this method (an older version?)</source>
        <translation>le Director ne propose pas cette méthode (version plus ancienne ?)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="113"/>
        <source>the gRPC service of the Director cannot be reached</source>
        <translation>le service gRPC du Director est injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="116"/>
        <source>not logged in, or the session has expired</source>
        <translation>non connecté, ou la session a expiré</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="120"/>
        <source>the Director reported an error without a message</source>
        <translation>le Director a signalé une erreur sans message</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="123"/>
        <source>no reason given</source>
        <translation>aucune raison indiquée</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="135"/>
        <source>The Director does not offer the expected gRPC service (protocol version mismatch: this build speaks %1; the Director may run an older or newer build). Use BNET or update the Director or this application.</source>
        <translation>Le Director ne propose pas le service gRPC attendu (incompatibilité de version du protocole : cette version utilise %1 ; le Director exécute peut-être une version plus ancienne ou plus récente). Utilisez BNET ou mettez à jour le Director ou cette application.</translation>
    </message>
</context>
<context>
    <name>BGrpcPinnedRelay</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="410"/>
        <source>no pinned certificate</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BGrpcTransport</name>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="226"/>
        <location filename="../src/director/bgrpctransport.cpp" line="259"/>
        <source>gRPC login failed: %1</source>
        <translation>Échec de la connexion gRPC : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="234"/>
        <source>gRPC login failed: no challenge received</source>
        <translation>Échec de la connexion gRPC : aucun challenge reçu</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="267"/>
        <source>Authentication failed: %1</source>
        <translation>Échec de l&apos;authentification : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="268"/>
        <source>invalid response</source>
        <translation>réponse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="329"/>
        <source>Client %1 connected</source>
        <translation>Client %1 connecté</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="332"/>
        <source>Client %1 disconnected</source>
        <translation>Client %1 déconnecté</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="338"/>
        <source>Client %1 is alive</source>
        <translation>Client %1 actif</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="339"/>
        <source>Client %1 not reachable</source>
        <translation>Client %1 injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="341"/>
        <source>Storage %1 connected (job %2)</source>
        <translation>Storage %1 connecté (job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="344"/>
        <source>Storage %1 disconnected (job %2)</source>
        <translation>Storage %1 déconnecté (job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="348"/>
        <source>Storage %1 is alive</source>
        <translation>Storage %1 actif</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="349"/>
        <source>Storage %1 not reachable</source>
        <translation>Storage %1 injoignable</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="482"/>
        <source>Invalid response</source>
        <translation>Réponse invalide</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="627"/>
        <location filename="../src/director/bgrpctransport.cpp" line="684"/>
        <source>Not connected to the gRPC service of the Director</source>
        <translation>Non connecté au service gRPC du Director</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="631"/>
        <source>Another client is being checked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="661"/>
        <location filename="../src/director/bgrpctransport.cpp" line="745"/>
        <source>The reply of the Director could not be read</source>
        <translation>La réponse du Director n&apos;a pas pu être lue</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="183"/>
        <source>The Director&apos;s certificate on the gRPC connection (SHA-256 %1) does not match the pinned certificate (SHA-256 %2). The connection was refused before the login; someone may be intercepting it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="186"/>
        <source>none</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="193"/>
        <source>Cannot open the local relay for the gRPC connection: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BIncludeBlockWidget</name>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="63"/>
        <source>Include Block #%1</source>
        <translation>Bloc Include n°%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="71"/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="98"/>
        <source>Add Path...</source>
        <translation>Ajouter un chemin...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="99"/>
        <source>Enter file or directory path</source>
        <translation>Saisir le chemin d&apos;un fichier ou d&apos;un répertoire</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="102"/>
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="109"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="123"/>
        <source>Exclude Files</source>
        <translation>Fichiers exclus</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="129"/>
        <source>Add File...</source>
        <translation>Ajouter un fichier...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="130"/>
        <source>Enter file path to exclude</source>
        <translation>Saisir le chemin du fichier à exclure</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="136"/>
        <source>Directory Patterns (WildDir)</source>
        <translation>Motifs de répertoires (WildDir)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="142"/>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="155"/>
        <source>Add Pattern...</source>
        <translation>Ajouter un motif...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="143"/>
        <source>e.g., */__pycache__</source>
        <translation>p. ex. */__pycache__</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="149"/>
        <source>File Patterns (WildFile)</source>
        <translation>Motifs de fichiers (WildFile)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="156"/>
        <source>e.g., *.pyc, *.log</source>
        <translation>p. ex. *.pyc, *.log</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="161"/>
        <source>Exclude</source>
        <translation>Exclure</translation>
    </message>
</context>
<context>
    <name>BIncludeOptionsForm</name>
    <message>
        <location filename="../src/config/bincludeoptionsform.cpp" line="91"/>
        <source>Failed to load include options schema</source>
        <translation>Impossible de charger le schéma des options Include</translation>
    </message>
</context>
<context>
    <name>BJobAdvancedPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="639"/>
        <source>Advanced Directives</source>
        <translation>Directives avancées</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="640"/>
        <source>Optional settings for fine-tuning (can be skipped).</source>
        <translation>Paramètres facultatifs pour un réglage fin (peut être ignoré).</translation>
    </message>
</context>
<context>
    <name>BJobBasicsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="249"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="254"/>
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="250"/>
        <source>Configure the identity of the resource.</source>
        <translation>Configurer l&apos;identité de la ressource.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="258"/>
        <source>e.g., BackupServer1</source>
        <translation>p. ex. BackupServer1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="259"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="263"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="268"/>
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="275"/>
        <source>JobDefs:</source>
        <translation>JobDefs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="288"/>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="300"/>
        <source>Inherits default settings from JobDefs &quot;%1&quot;. Resource fields on the next page become optional.</source>
        <translation>Hérite des paramètres par défaut du JobDefs « %1 ». Les champs de ressources de la page suivante deviennent facultatifs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="314"/>
        <source>Basic Settings (JobDefs)</source>
        <translation>Paramètres de base (JobDefs)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitions</name>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="115"/>
        <source>(block)</source>
        <translation>(bloc)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitions.cpp" line="128"/>
        <source>(%n block(s))</source>
        <translation>
            <numerusform>(%n bloc)</numerusform>
            <numerusform>(%n blocs)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="302"/>
        <source>(see the text)</source>
        <translation>(voir le texte)</translation>
    </message>
</context>
<context>
    <name>BJobDetailsDialog</name>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="35"/>
        <source>Job Details: %1 (ID: %2) - Client: %3</source>
        <translation>Détails du job : %1 (ID : %2) - Client : %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="74"/>
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="83"/>
        <source>Job Information</source>
        <translation>Informations sur le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="86"/>
        <source>Job ID:</source>
        <translation>ID du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="87"/>
        <source>Job Name:</source>
        <translation>Nom du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="88"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="92"/>
        <source>Backup</source>
        <translation>Backup</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="93"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="97"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="98"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="99"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="101"/>
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="105"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="106"/>
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="112"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="240"/>
        <source>Schedule:</source>
        <translation>Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="120"/>
        <source>Timing</source>
        <translation>Horaires</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="127"/>
        <source>Start Time:</source>
        <translation>Heure de début :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="131"/>
        <source>Duration:</source>
        <translation>Durée :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="136"/>
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="140"/>
        <source>Files Backed Up:</source>
        <translation>Fichiers sauvegardés :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="147"/>
        <source>Data Size:</source>
        <translation>Taille des données :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="152"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="169"/>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="163"/>
        <source>Job Status:</source>
        <translation>Statut du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="183"/>
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="187"/>
        <source>Logs</source>
        <translation>Journaux</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="200"/>
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director.</translation>
    </message>
</context>
<context>
    <name>BJobLogDialog</name>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="30"/>
        <source>Job Log: %1 (ID: %2) - %3</source>
        <translation>Journal du job : %1 (ID : %2) - %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="45"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="46"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="47"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="48"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="51"/>
        <source>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</source>
        <translation>&lt;b&gt;Statut :&lt;/b&gt; %1 | &lt;b&gt;Début :&lt;/b&gt; %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="68"/>
        <source>Copy Selection</source>
        <translation>Copier la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="74"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
</context>
<context>
    <name>BJobLogView</name>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="70"/>
        <source>No log entries for this job.</source>
        <translation>Aucune entrée de journal pour ce job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="74"/>
        <source>Could not load the job log: %1</source>
        <translation>Impossible de charger le journal du job : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="104"/>
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="108"/>
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
</context>
<context>
    <name>BJobPreviewPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="703"/>
        <source>Preview &amp; Deploy</source>
        <translation>Aperçu &amp; déploiement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="704"/>
        <source>Review the configuration and deploy to the Director.</source>
        <translation>Vérifiez la configuration et déployez-la sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="709"/>
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="721"/>
        <source>Copy Configuration</source>
        <translation>Copier la configuration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="728"/>
        <source>Director Command:</source>
        <translation>Commande du Director :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="740"/>
        <source>Copy Command</source>
        <translation>Copier la commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="798"/>
        <source>Note: RunScript blocks cannot be sent via &apos;configure add&apos;. The command above creates the job without RunScript blocks. Deploy the full configuration manually using &apos;Copy Configuration&apos; to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.</source>
        <translation>Remarque : les blocs RunScript ne peuvent pas être envoyés via &apos;configure add&apos;. La commande ci-dessus crée le job sans blocs RunScript. Déployez manuellement la configuration complète à l&apos;aide de « Copier la configuration » dans /etc/bareos/bareos-dir.d/job/%1.conf et rechargez le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="821"/>
        <source>Deploy to Director</source>
        <translation>Déployer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="822"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Envoyer &apos;configure add&apos; au Director ?

Cliquez sur Oui pour exécuter, sur Non pour fermer sans déployer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="842"/>
        <source>Not sent: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="851"/>
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="886"/>
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="895"/>
        <source>Resource created successfully.</source>
        <translation>Ressource créée avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="903"/>
        <source>Resource created. Reloading Director...</source>
        <translation>Ressource créée. Rechargement du Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="926"/>
        <source>Resource created and Director reloaded successfully.</source>
        <translation>Ressource créée et Director rechargé avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="929"/>
        <source>Resource created, but reloading the Director failed. Run &apos;reload&apos; on the Director.</source>
        <translation>Ressource créée, mais le rechargement du Director a échoué. Exécutez &apos;reload&apos; sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="942"/>
        <source>Resource created, but the connection to the Director was lost before the reload was confirmed.</source>
        <translation>Ressource créée, mais la connexion au Director a été perdue avant la confirmation du rechargement.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="946"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check whether the resource exists.</source>
        <translation>La connexion au Director a été perdue avant sa réponse. Reconnectez-vous et vérifiez si la ressource existe.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="958"/>
        <source>Resource created, but the Director did not confirm the reload in time. Run &apos;reload&apos; on the Director if the job does not appear.</source>
        <translation>Ressource créée, mais le Director n&apos;a pas confirmé le rechargement à temps. Exécutez &apos;reload&apos; sur le Director si le job n&apos;apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="961"/>
        <source>Timeout waiting for Director response. The command may still be processing.</source>
        <translation>Délai d&apos;attente de la réponse du Director dépassé. La commande est peut-être encore en cours de traitement.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="970"/>
        <source>Configuration copied to clipboard.</source>
        <translation>Configuration copiée dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="977"/>
        <source>Command copied to clipboard.</source>
        <translation>Commande copiée dans le presse-papiers.</translation>
    </message>
</context>
<context>
    <name>BJobResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
</context>
<context>
    <name>BJobResourcesPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="345"/>
        <source>Resource References</source>
        <translation>Références de ressources</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="346"/>
        <source>Select the resources this job uses.</source>
        <translation>Sélectionnez les ressources utilisées par ce job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="362"/>
        <source>Required Resources</source>
        <translation>Ressources requises</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="367"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="371"/>
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="375"/>
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="379"/>
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="383"/>
        <source>Messages:</source>
        <translation>Messages :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="388"/>
        <source>Job Settings</source>
        <translation>Paramètres du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="394"/>
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="398"/>
        <source>Schedule:</source>
        <translation>Schedule :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="402"/>
        <source>Catalog:</source>
        <translation>Catalog :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="407"/>
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="412"/>
        <source>Max Concurrent Jobs:</source>
        <translation>Max Concurrent Jobs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="417"/>
        <source>Restore Settings</source>
        <translation>Paramètres de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="421"/>
        <source>/tmp/bareos-restores</source>
        <translation>/tmp/bareos-restores</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="422"/>
        <source>Where:</source>
        <translation>Where :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="426"/>
        <source>Replace:</source>
        <translation>Replace :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="431"/>
        <source>Pool Overrides</source>
        <translation>Remplacements de pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="438"/>
        <source>Full Backup Pool:</source>
        <translation>Full Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="442"/>
        <source>Diff Backup Pool:</source>
        <translation>Diff Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="446"/>
        <source>Inc Backup Pool:</source>
        <translation>Inc Backup Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="451"/>
        <source>Simple Scripts</source>
        <translation>Scripts simples</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="455"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="459"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="463"/>
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="456"/>
        <source>Run Before Job:</source>
        <translation>Run Before Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="460"/>
        <source>Run After Job:</source>
        <translation>Run After Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="464"/>
        <source>Run After Failed Job:</source>
        <translation>Run After Failed Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="467"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="471"/>
        <source>/path/to/client-script.sh</source>
        <translation>/path/to/client-script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="468"/>
        <source>Client Run Before:</source>
        <translation>Client Run Before :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="472"/>
        <source>Client Run After:</source>
        <translation>Client Run After :</translation>
    </message>
</context>
<context>
    <name>BJobScriptsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="613"/>
        <source>RunScript Blocks</source>
        <translation>Blocs RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="614"/>
        <source>Define complex script execution blocks (optional). For simple scripts, use the fields on the previous page.</source>
        <translation>Définissez des blocs d&apos;exécution de scripts complexes (facultatif). Pour des scripts simples, utilisez les champs de la page précédente.</translation>
    </message>
</context>
<context>
    <name>BJobWidget</name>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="52"/>
        <source>Successful (T)</source>
        <translation>Réussi (T)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="53"/>
        <source>Warning (W)</source>
        <translation>Avertissement (W)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="55"/>
        <source>Error (E)</source>
        <translation>Erreur (E)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="60"/>
        <source>Enable Date Filter</source>
        <translation>Activer le filtre de date</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="426"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1702"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1705"/>
        <source>Filter</source>
        <translation>Filtre</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="485"/>
        <source>Job Name:</source>
        <translation>Nom du job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="486"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="539"/>
        <source>Backup Level</source>
        <translation>Niveau de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="571"/>
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="572"/>
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="573"/>
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="559"/>
        <source>Date Range</source>
        <translation>Plage de dates</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="43"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="44"/>
        <source>Clear History</source>
        <translation>Effacer l&apos;historique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="54"/>
        <source>Failed (F)</source>
        <translation>Échec (F)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="56"/>
        <source>Running (R)</source>
        <translation>En cours (R)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="57"/>
        <source>Canceled (A)</source>
        <translation>Annulé (A)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="58"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="382"/>
        <source>Zero Bytes</source>
        <translation>Zéro octet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="85"/>
        <source>When enabled, only shows jobs with 0 bytes transferred.
Warning: This filter persists across restarts and may
cause an empty table if no zero-byte jobs exist.</source>
        <translation>Si activé, affiche uniquement les jobs ayant transféré 0 octet.
Attention : ce filtre est conservé après un redémarrage et peut
produire un tableau vide s&apos;il n&apos;existe aucun job à zéro octet.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="93"/>
        <source>When enabled, only shows jobs within the selected date range.
Warning: This filter persists across restarts and may
cause an empty or limited table if the range is too narrow.</source>
        <translation>Si activé, affiche uniquement les jobs compris dans la plage de dates sélectionnée.
Attention : ce filtre est conservé après un redémarrage et peut
produire un tableau vide ou limité si la plage est trop étroite.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="110"/>
        <source>Filter by job name...</source>
        <translation>Filtrer par nom de job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="115"/>
        <source>Filter by client...</source>
        <translation>Filtrer par client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="120"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="123"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="126"/>
        <source>(no job selected)</source>
        <translation>(aucun job sélectionné)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="158"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Load error</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - Erreur de chargement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="182"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1752"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - No job selected</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Aucun job sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="186"/>
        <source>-- Select Job Log --</source>
        <translation>-- Sélectionner un journal de job --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="188"/>
        <source>Select from previously loaded job logs</source>
        <translation>Sélectionner parmi les journaux de jobs chargés précédemment</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="190"/>
        <source>Copy selected lines to clipboard</source>
        <translation>Copier les lignes sélectionnées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="193"/>
        <source>Clear job log history</source>
        <translation>Effacer l&apos;historique des journaux de jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="196"/>
        <source>Search in log...</source>
        <translation>Rechercher dans le journal...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="198"/>
        <source>Filter log lines by search text</source>
        <translation>Filtrer les lignes du journal par texte recherché</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="231"/>
        <source>Cancelling job...</source>
        <translation>Annulation du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="234"/>
        <source>Deleting job...</source>
        <translation>Suppression du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="237"/>
        <source>Running job again...</source>
        <translation>Nouvelle exécution du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="240"/>
        <source>Loading job log...</source>
        <translation>Chargement du journal du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="244"/>
        <source>Pruning expired jobs...</source>
        <translation>Élagage des jobs expirés...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="285"/>
        <source>⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. This filter persists across restarts!</source>
        <translation>⚠ Filtre Zéro octet actif — seuls les jobs avec 0 octet sont affichés. Ce filtre est conservé après un redémarrage !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="295"/>
        <source>⚠ Date filter active — only jobs within the selected range are shown. This filter persists across restarts!</source>
        <translation>⚠ Filtre de date actif — seuls les jobs compris dans la plage sélectionnée sont affichés. Ce filtre est conservé après un redémarrage !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="385"/>
        <source>Date Range (%1 – %2)</source>
        <translation>Plage de dates (%1 – %2)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="398"/>
        <source>Status (hidden: %1)</source>
        <translation>Statut (masqués : %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="403"/>
        <source>⚠ Active filters from last session: %1 — results may be limited</source>
        <translation>⚠ Filtres actifs de la dernière session : %1 — les résultats peuvent être limités</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="429"/>
        <source>Show/hide filter</source>
        <translation>Afficher/masquer les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="435"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="439"/>
        <source>Auto-Refresh</source>
        <translation>Actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="441"/>
        <source>5 s</source>
        <translation>5 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="442"/>
        <source>10 s</source>
        <translation>10 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="443"/>
        <source>30 s</source>
        <translation>30 s</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="444"/>
        <source>1 min</source>
        <translation>1 min</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="445"/>
        <source>5 min</source>
        <translation>5 min</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="490"/>
        <source>&lt;b&gt;Status:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Statut :&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="506"/>
        <source>Basic Filters</source>
        <translation>Filtres de base</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="528"/>
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="553"/>
        <source>From:</source>
        <translation>Du :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="554"/>
        <source>To:</source>
        <translation>Au :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="578"/>
        <source>Job Details</source>
        <translation>Détails du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="583"/>
        <source>Reset filter</source>
        <translation>Réinitialiser les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="704"/>
        <source>Updated: %1 - %2 jobs loaded</source>
        <translation>Mis à jour : %1 - %2 jobs chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="750"/>
        <source>No selection</source>
        <translation>Aucune sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="751"/>
        <source>Please select a job to cancel.</source>
        <translation>Veuillez sélectionner un job à annuler.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="768"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="805"/>
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="765"/>
        <source>Do you really want to cancel job ID %1?</source>
        <translation>Voulez-vous vraiment annuler le job d&apos;ID %1 ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1222"/>
        <source>Loading page %1...</source>
        <translation>Chargement de la page %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1235"/>
        <source>Refreshing job list...</source>
        <translation>Actualisation de la liste des jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1342"/>
        <source>%1 jobs loaded</source>
        <translation>%1 jobs chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1577"/>
        <source>Auto-refresh on (%1)</source>
        <translation>Actualisation automatique activée (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1582"/>
        <source>Auto-refresh off</source>
        <translation>Actualisation automatique désactivée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1657"/>
        <source>Filtered: %1 of %2 jobs</source>
        <translation>Filtrés : %1 jobs sur %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1703"/>
        <source>Hide filter</source>
        <translation>Masquer les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1706"/>
        <source>Show filter</source>
        <translation>Afficher les filtres</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1734"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Loading...</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - Chargement...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1772"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1909"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - %3 lignes</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="615"/>
        <source>History:</source>
        <translation>Historique :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="635"/>
        <source>Job Log</source>
        <translation>Journal du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="638"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1274"/>
        <source>Error parsing JSON data</source>
        <translation>Erreur lors de l&apos;analyse des données JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1588"/>
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1589"/>
        <source>Running</source>
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1590"/>
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1591"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1592"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1593"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1594"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1595"/>
        <source>Non-fatal Error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1596"/>
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1650"/>
        <source>⚠ No jobs visible — &apos;Zero Bytes&apos; filter is active!</source>
        <translation>⚠ Aucun job visible — le filtre « Zéro octet » est actif !</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1652"/>
        <source>⚠ No jobs visible — check your filter settings (%1 jobs loaded)</source>
        <translation>⚠ Aucun job visible — vérifiez vos paramètres de filtre (%1 jobs chargés)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1661"/>
        <source>%1 Jobs</source>
        <translation>%1 jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1767"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines, job is running</source>
        <translation>&lt;b&gt;Journal du job&lt;/b&gt; - Job : %1 (ID : %2) - %3 lignes, job en cours</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1933"/>
        <source>Copied %1 log lines to clipboard</source>
        <translation>%1 lignes de journal copiées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1948"/>
        <source>Copied %1 selected lines to clipboard</source>
        <translation>%1 lignes sélectionnées copiées dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1962"/>
        <source>Job log history cleared</source>
        <translation>Historique des journaux de jobs effacé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="755"/>
        <source>A cancel is still waiting for the Director&apos;s answer</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="766"/>
        <source>Do you really want to cancel these %n jobs: %1?</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="777"/>
        <source>Cancelling %n job(s)...</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="794"/>
        <source>The Director accepted the cancel of %n job(s): %1</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="801"/>
        <source>Job %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="802"/>
        <source>no reason given</source>
        <translation type="unfinished">aucune raison indiquée</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="806"/>
        <source>The Director did not cancel %n job(s):</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BJobWizard</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1008"/>
        <source>New Job Wizard</source>
        <translation>Assistant de nouveau job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1009"/>
        <source>New JobDefs Wizard</source>
        <translation>Assistant de nouveau JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1019"/>
        <source>Finish</source>
        <translation>Terminer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1095"/>
        <source>a control character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1096"/>
        <source>%1 contains %2, which &apos;configure add&apos; cannot write.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BJobsModel</name>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="276"/>
        <source>Job ID</source>
        <translation>ID du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="277"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="278"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="279"/>
        <source>Start Time</source>
        <translation>Heure de début</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="280"/>
        <source>End Time</source>
        <translation>Heure de fin</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="281"/>
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="282"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="283"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="284"/>
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="285"/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="286"/>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
</context>
<context>
    <name>BJobsStatisticsWidget</name>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="23"/>
        <source>Job Summary</source>
        <translation>Résumé des jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="29"/>
        <source>Total Jobs:</source>
        <translation>Total des jobs :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="32"/>
        <source>Selected:</source>
        <translation>Sélectionnés :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="37"/>
        <source>Status Breakdown</source>
        <translation>Répartition par statut</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="44"/>
        <source>Successful:</source>
        <translation>Réussis :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="59"/>
        <source>Warnings:</source>
        <translation>Avertissements :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="74"/>
        <source>Failed:</source>
        <translation>Échoués :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="88"/>
        <source>Data Summary</source>
        <translation>Résumé des données</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="92"/>
        <source>Total Files:</source>
        <translation>Total des fichiers :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="95"/>
        <source>Total Size:</source>
        <translation>Taille totale :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="97"/>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="185"/>
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="98"/>
        <source>Date Range:</source>
        <translation>Plage de dates :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="180"/>
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
</context>
<context>
    <name>BJsonJobView</name>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="531"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="618"/>
        <source>Delete %1 Jobs</source>
        <translation>Supprimer %1 jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="162"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="531"/>
        <source>Delete Job</source>
        <translation>Supprimer le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="151"/>
        <source>Show Details...</source>
        <translation>Afficher les détails...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="156"/>
        <source>Retry Job</source>
        <translation>Réessayer le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="159"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="727"/>
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="167"/>
        <source>View Log...</source>
        <translation>Afficher le journal...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="170"/>
        <source>Restore Files...</source>
        <translation>Restaurer des fichiers...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="177"/>
        <source>Export to JSON...</source>
        <translation>Exporter en JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="181"/>
        <source>Export to CSV...</source>
        <translation>Exporter en CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="188"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="192"/>
        <source>Invert Selection</source>
        <translation>Inverser la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="196"/>
        <source>Clear Selection</source>
        <translation>Effacer la sélection</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="277"/>
        <source>Export Selected Jobs to JSON</source>
        <translation>Exporter les jobs sélectionnés en JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="277"/>
        <source>Export All Jobs to JSON</source>
        <translation>Exporter tous les jobs en JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="290"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="323"/>
        <source>Export Error</source>
        <translation>Erreur d&apos;exportation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="291"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="324"/>
        <source>Could not write to file: %1</source>
        <translation>Impossible d&apos;écrire dans le fichier : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="298"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="331"/>
        <source>Export Successful</source>
        <translation>Exportation réussie</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjsonjobview.cpp" line="299"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="332"/>
        <source>Exported %n job(s) to %1</source>
        <translation>
            <numerusform>%n job exporté vers %1</numerusform>
            <numerusform>%n jobs exportés vers %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="310"/>
        <source>Export Selected Jobs to CSV</source>
        <translation>Exporter les jobs sélectionnés en CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="310"/>
        <source>Export All Jobs to CSV</source>
        <translation>Exporter tous les jobs en CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="344"/>
        <source>Total Jobs: %1
</source>
        <translation>Total des jobs : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="345"/>
        <source>Successful: %1
</source>
        <translation>Réussis : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="346"/>
        <source>Warnings: %1
</source>
        <translation>Avertissements : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="347"/>
        <source>Failed: %1

</source>
        <translation>Échoués : %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="349"/>
        <source>Total Files: %1
</source>
        <translation>Total des fichiers : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="350"/>
        <source>Total Size: %1 bytes

</source>
        <translation>Taille totale : %1 octets

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="353"/>
        <source>Date Range:
</source>
        <translation>Plage de dates :
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="354"/>
        <source>  From: %1
</source>
        <translation>  De : %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="355"/>
        <source>  To: %1

</source>
        <translation>  À : %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="358"/>
        <source>Selected: %1</source>
        <translation>Sélectionnés : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="360"/>
        <source>Job Statistics</source>
        <translation>Statistiques des jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="425"/>
        <source>Export All to JSON...</source>
        <translation>Tout exporter en JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="426"/>
        <source>Export All to CSV...</source>
        <translation>Tout exporter en CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="541"/>
        <source>ID %1: %2 (%3)</source>
        <translation>ID %1 : %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="547"/>
        <source>&lt;b&gt;%1 jobs selected for deletion:&lt;/b&gt;&lt;br&gt;%2</source>
        <translation>&lt;b&gt;%1 jobs sélectionnés pour suppression :&lt;/b&gt;&lt;br&gt;%2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="556"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="557"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="558"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="560"/>
        <source>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Level:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Start:&lt;/b&gt; %5</source>
        <translation>&lt;b&gt;Job :&lt;/b&gt; %1 (ID : %2)&lt;br&gt;&lt;b&gt;Client :&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Niveau :&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Début :&lt;/b&gt; %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="574"/>
        <source>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Attention:&lt;/b&gt; %1 dependent Incremental/Differential job(s) found!&lt;br&gt;&lt;br&gt;Deleting Full backups may make dependent backups unusable for restore.&lt;/div&gt;</source>
        <translation>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Attention :&lt;/b&gt; %1 job(s) Incremental/Differential dépendant(s) trouvé(s) !&lt;br&gt;&lt;br&gt;La suppression de sauvegardes Full peut rendre les sauvegardes dépendantes inutilisables pour la restauration.&lt;/div&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="585"/>
        <source>Delete Mode</source>
        <translation>Mode de suppression</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="588"/>
        <source>Delete job record only</source>
        <translation>Supprimer uniquement l&apos;enregistrement du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="589"/>
        <source>Removes the job entry from the catalog database.
The backup data on the storage media remains intact.</source>
        <translation>Supprime l&apos;entrée du job de la base de données du catalogue.
Les données de sauvegarde sur les supports de stockage restent intactes.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="598"/>
        <source>Prune (remove expired jobs by retention policy)</source>
        <translation>Élaguer (supprimer les jobs expirés selon la politique de rétention)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="599"/>
        <source>Removes job records that have exceeded their configured retention period.
Only expired jobs for the affected client(s) will be removed.</source>
        <translation>Supprime les enregistrements de jobs ayant dépassé leur durée de rétention configurée.
Seuls les jobs expirés du ou des clients concernés seront supprimés.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="608"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Attention :&lt;/b&gt; cette action est irréversible !&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="618"/>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="632"/>
        <source>  ID %1: %2 (%3)</source>
        <translation>  ID %1 : %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="637"/>
        <source>Confirm Delete</source>
        <translation>Confirmer la suppression</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="638"/>
        <source>Are you sure you want to delete %1 job(s)?

%2

%3 dependent Incremental/Differential backup(s) may become unusable for restore.</source>
        <translation>Voulez-vous vraiment supprimer %1 job(s) ?

%2

%3 sauvegarde(s) Incremental/Differential dépendante(s) peuvent devenir inutilisables pour la restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="695"/>
        <source>Rerun Job</source>
        <translation>Réexécuter le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="696"/>
        <source>Do you want to run job %1 (%2) again?</source>
        <translation>Voulez-vous exécuter à nouveau le job %1 (%2) ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="728"/>
        <source>Do you really want to cancel the running job %1 (%2)?</source>
        <translation>Voulez-vous vraiment annuler le job en cours %1 (%2) ?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="757"/>
        <source>Not connected</source>
        <translation>Aucune connexion</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="758"/>
        <source>No connection to the Director.</source>
        <translation>Aucune connexion au Director disponible.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="779"/>
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="780"/>
        <source>No director connection available.</source>
        <translation>Aucune connexion au Director disponible.</translation>
    </message>
</context>
<context>
    <name>BMainWindow</name>
    <message>
        <location filename="../ui/bmainwindow.ui" line="14"/>
        <source>Bacula Qt UI</source>
        <translation>Bacula Qt UI</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="170"/>
        <source>Interface zoom: %1 %</source>
        <translation>Zoom de l&apos;interface : %1 %</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="234"/>
        <source>Connecting...</source>
        <translation>Connexion...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="237"/>
        <source>Authenticating...</source>
        <translation>Authentification...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="240"/>
        <source>Setting API mode...</source>
        <translation>Définition du mode API...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="243"/>
        <source>Loading resources...</source>
        <translation>Chargement des ressources...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="246"/>
        <source>Connected and ready</source>
        <translation>Connecté et prêt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="249"/>
        <source>Connection error</source>
        <translation>Erreur de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="252"/>
        <location filename="../src/bmainwindow.cpp" line="1831"/>
        <source>Disconnected</source>
        <translation>Déconnecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="259"/>
        <source>Loading resources... %1/%2</source>
        <translation>Chargement des ressources... %1/%2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="284"/>
        <location filename="../src/bmainwindow.cpp" line="789"/>
        <location filename="../src/bmainwindow.cpp" line="2696"/>
        <source>Ready</source>
        <translation>Prêt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="453"/>
        <location filename="../src/bmainwindow.cpp" line="3097"/>
        <source>Auto-refresh enabled (%1s)</source>
        <translation>Actualisation automatique activée (%1 s)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="457"/>
        <location filename="../src/bmainwindow.cpp" line="3102"/>
        <source>Auto-refresh disabled</source>
        <translation>Actualisation automatique désactivée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="531"/>
        <source>Job: %1</source>
        <translation>Job : %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="592"/>
        <location filename="../src/bmainwindow.cpp" line="795"/>
        <location filename="../src/bmainwindow.cpp" line="1833"/>
        <location filename="../src/bmainwindow.cpp" line="2471"/>
        <location filename="../src/bmainwindow.cpp" line="2612"/>
        <location filename="../src/bmainwindow.cpp" line="2952"/>
        <source>Not connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="712"/>
        <source>Storage/Volumes</source>
        <translation>Storage/Volumes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="770"/>
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="866"/>
        <location filename="../src/bmainwindow.cpp" line="883"/>
        <location filename="../src/bmainwindow.cpp" line="1797"/>
        <location filename="../src/bmainwindow.cpp" line="2035"/>
        <source>Connect</source>
        <translation>Se connecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="871"/>
        <location filename="../src/bmainwindow.cpp" line="889"/>
        <location filename="../src/bmainwindow.cpp" line="2036"/>
        <location filename="../src/bmainwindow.cpp" line="2711"/>
        <source>Reconnect</source>
        <translation>Se reconnecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="877"/>
        <source>Disconnect</source>
        <translation>Se déconnecter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="885"/>
        <location filename="../src/bmainwindow.cpp" line="1718"/>
        <location filename="../src/bmainwindow.cpp" line="2496"/>
        <source>Connect to Director</source>
        <translation>Se connecter au Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="891"/>
        <source>Reconnect to last used connection</source>
        <translation>Se reconnecter à la dernière connexion utilisée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="895"/>
        <location filename="../src/bmainwindow.cpp" line="2037"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="901"/>
        <location filename="../src/bmainwindow.cpp" line="2038"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="906"/>
        <location filename="../src/bmainwindow.cpp" line="2039"/>
        <source>Exit</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="911"/>
        <location filename="../src/bmainwindow.cpp" line="1859"/>
        <source>About Onesimus</source>
        <translation>À propos d&apos;Onesimus</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="915"/>
        <source>About Qt</source>
        <translation>À propos de Qt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="919"/>
        <source>Online Documentation</source>
        <translation>Documentation en ligne</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="924"/>
        <source>Report a Bug...</source>
        <translation>Signaler un bogue...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="928"/>
        <location filename="../src/bmainwindow.cpp" line="2015"/>
        <location filename="../src/bmainwindow.cpp" line="2021"/>
        <location filename="../src/bmainwindow.cpp" line="2103"/>
        <source>Keyboard Shortcuts</source>
        <translation>Raccourcis clavier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="933"/>
        <location filename="../src/bmainwindow.cpp" line="2433"/>
        <source>My Permissions</source>
        <translation>Mes autorisations</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="939"/>
        <location filename="../src/bmainwindow.cpp" line="2052"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="945"/>
        <location filename="../src/bmainwindow.cpp" line="2053"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="951"/>
        <source>Clear Selection</source>
        <translation>Effacer la sélection</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="956"/>
        <source>Find...</source>
        <translation>Rechercher...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="963"/>
        <location filename="../src/bmainwindow.cpp" line="2067"/>
        <source>Show Statistics</source>
        <translation>Afficher les statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="977"/>
        <location filename="../src/bmainwindow.cpp" line="2068"/>
        <source>Show Job Log</source>
        <translation>Afficher le journal du job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="992"/>
        <location filename="../src/bmainwindow.cpp" line="2070"/>
        <source>Interface Zoom In</source>
        <translation>Agrandir l&apos;interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="998"/>
        <location filename="../src/bmainwindow.cpp" line="2071"/>
        <source>Interface Zoom Out</source>
        <translation>Réduire l&apos;interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1010"/>
        <source>Toggle Dark/Light Theme</source>
        <translation>Basculer thème sombre/clair</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1025"/>
        <source>Run Job</source>
        <translation>Exécuter le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1030"/>
        <source>Cancel Job</source>
        <translation>Annuler le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1035"/>
        <source>Show Details</source>
        <translation>Afficher les détails</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1040"/>
        <location filename="../src/bmainwindow.cpp" line="2085"/>
        <source>Refresh Jobs</source>
        <translation>Actualiser les jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1046"/>
        <source>Export Jobs as JSON...</source>
        <translation>Exporter les jobs en JSON...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1051"/>
        <source>Export Jobs as CSV...</source>
        <translation>Exporter les jobs en CSV...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1057"/>
        <source>Add Job...</source>
        <translation>Ajouter un job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1076"/>
        <source>Edit Job...</source>
        <translation>Modifier le job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1082"/>
        <source>Edit Job</source>
        <translation>Modifier le job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1083"/>
        <source>No Jobs available.</source>
        <translation>Aucun job disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1089"/>
        <source>Job Resources</source>
        <translation>Ressources Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1142"/>
        <source>Delete Job...</source>
        <translation>Supprimer le job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1150"/>
        <source>Add JobDefs...</source>
        <translation>Ajouter un JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1169"/>
        <source>Edit JobDefs...</source>
        <translation>Modifier le JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1175"/>
        <source>JobDefs Resources</source>
        <translation>Ressources JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1230"/>
        <source>Delete JobDefs...</source>
        <translation>Supprimer le JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1241"/>
        <location filename="../src/bmainwindow.cpp" line="1245"/>
        <source>Delete JobDefs</source>
        <translation>Supprimer le JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1241"/>
        <source>No JobDefs available.</source>
        <translation>Aucun JobDefs disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1245"/>
        <source>JobDefs to delete:</source>
        <translation>JobDefs à supprimer :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1259"/>
        <source>Jobs that use this JobDefs</source>
        <translation>Jobs qui utilisent ce JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1265"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1275"/>
        <source>Add FileSet...</source>
        <translation>Ajouter un FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1283"/>
        <source>Edit FileSet...</source>
        <translation>Modifier le FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1291"/>
        <source>Delete FileSet...</source>
        <translation>Supprimer le FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1300"/>
        <location filename="../src/bmainwindow.cpp" line="1306"/>
        <location filename="../src/bmainwindow.cpp" line="1352"/>
        <location filename="../src/bmainwindow.cpp" line="1357"/>
        <source>Delete FileSet</source>
        <translation>Supprimer le FileSet</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1301"/>
        <source>No FileSets available.</source>
        <translation>Aucun FileSet disponible.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1311"/>
        <source>Select the FileSet to delete:</source>
        <translation>Sélectionnez le FileSet à supprimer :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1322"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This will remove the FileSet resource from the Director configuration. This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Attention :&lt;/b&gt; cette opération supprimera la ressource FileSet de la configuration du Director. Cette action est irréversible !&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1333"/>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1345"/>
        <source>Confirm Delete</source>
        <translation>Confirmer la suppression</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1346"/>
        <source>Are you sure you want to delete FileSet &quot;%1&quot;?</source>
        <translation>Voulez-vous vraiment supprimer le FileSet « %1 » ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1353"/>
        <source>Not connected to a Director. FileSet &quot;%1&quot; was not deleted.</source>
        <translation>Non connecté à un Director. Le FileSet « %1 » n&apos;a pas été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1358"/>
        <source>FileSet &quot;%1&quot; no longer exists on the Director. Nothing was deleted.</source>
        <translation>Le FileSet « %1 » n&apos;existe plus sur le Director. Rien n&apos;a été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1366"/>
        <source>Add New Client...</source>
        <translation>Ajouter un nouveau client...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1374"/>
        <location filename="../src/bmainwindow.cpp" line="2086"/>
        <source>Refresh Clients</source>
        <translation>Actualiser les clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1380"/>
        <source>Show Client Details</source>
        <translation>Afficher les détails du client</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1390"/>
        <location filename="../src/bmainwindow.cpp" line="2087"/>
        <source>Refresh Storage</source>
        <translation>Actualiser les Storages</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1397"/>
        <source>Add Schedule...</source>
        <translation>Ajouter un Schedule...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1417"/>
        <location filename="../src/bmainwindow.cpp" line="2088"/>
        <source>Refresh Schedules</source>
        <translation>Actualiser les Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1424"/>
        <source>Database Cleanup...</source>
        <translation>Nettoyage de la base de données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1426"/>
        <source>Clean up old backups and free disk space</source>
        <translation>Nettoyer les anciennes sauvegardes et libérer de l&apos;espace disque</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1430"/>
        <source>Connection Wizard...</source>
        <translation>Assistant de connexion...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1432"/>
        <source>Set up a new director connection</source>
        <translation>Configurer une nouvelle connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1435"/>
        <source>Import Config...</source>
        <translation>Importer une configuration...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1437"/>
        <source>Import Director configuration from files or ZIP archive</source>
        <translation>Importer la configuration du Director depuis des fichiers ou une archive ZIP</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1446"/>
        <location filename="../src/bmainwindow.cpp" line="1486"/>
        <location filename="../src/bmainwindow.cpp" line="1502"/>
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1452"/>
        <source>System (%1)</source>
        <translation>Système (%1)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1487"/>
        <source>Onesimus will speak %1 after a restart.</source>
        <translation>Onesimus parlera %1 après un redémarrage.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1490"/>
        <source>Restart now? The connection to the Director is closed and opened again if automatic connection is on.</source>
        <translation>Redémarrer maintenant ? La connexion au Director est fermée, puis rouverte si la connexion automatique est activée.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1492"/>
        <source>Restart now?</source>
        <translation>Redémarrer maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1493"/>
        <source>Restart now</source>
        <translation>Redémarrer maintenant</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1494"/>
        <source>Later</source>
        <translation>Plus tard</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1503"/>
        <source>Onesimus could not be started again. The new language takes effect at the next start.</source>
        <translation>Onesimus n&apos;a pas pu être relancé. La nouvelle langue s&apos;appliquera au prochain démarrage.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1513"/>
        <location filename="../src/bmainwindow.cpp" line="2043"/>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1527"/>
        <location filename="../src/bmainwindow.cpp" line="2058"/>
        <source>Edit</source>
        <translation>Édition</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1536"/>
        <location filename="../src/bmainwindow.cpp" line="2076"/>
        <source>View</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1605"/>
        <source>ACL matrix of the consoles...</source>
        <translation>Matrice des ACL des consoles...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1606"/>
        <source>Which console may use which job, client, storage, schedule, pool, fileset, catalog and command (Onesimus Director)</source>
        <translation>Quelle console peut utiliser quel Job, Client, Storage, Schedule, Pool, FileSet, Catalog et commande (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1831"/>
        <source>Connection attempt cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1972"/>
        <source>Third-party licences...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2712"/>
        <source>The connection profile &quot;%1&quot; has no saved password or is incomplete. Complete it in Settings, then connect again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2913"/>
        <source>Cancel the connection attempt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2930"/>
        <source>Passwords Not in the Keychain</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bmainwindow.cpp" line="3005"/>
        <location filename="../src/bmainwindow.cpp" line="3038"/>
        <source>%n row(s) copied</source>
        <translation>
            <numerusform>%n ligne copiée</numerusform>
            <numerusform>%n lignes copiées</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3013"/>
        <source>No client selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3053"/>
        <source>All clients selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3081"/>
        <source>Find works in the Jobs and Clients tabs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="704"/>
        <location filename="../src/bmainwindow.cpp" line="1547"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1555"/>
        <source>Modify Jobs</source>
        <translation>Modifier les jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1561"/>
        <source>Modify JobDefs</source>
        <translation>Modifier les JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1567"/>
        <source>Filesets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="708"/>
        <location filename="../src/bmainwindow.cpp" line="1577"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1584"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="741"/>
        <location filename="../src/bmainwindow.cpp" line="1588"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="125"/>
        <source>Onesimus - Bacula Backup Management</source>
        <translation>Onesimus - Gestion des sauvegardes Bacula</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="127"/>
        <source>Onesimus - Bareos Backup Management</source>
        <translation>Onesimus - Gestion des sauvegardes Bareos</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1594"/>
        <source>Tools</source>
        <translation>Outils</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1610"/>
        <location filename="../src/bmainwindow.cpp" line="2107"/>
        <source>Help</source>
        <translation>Aide</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1639"/>
        <source>Main Toolbar</source>
        <translation>Barre d&apos;outils principale</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1668"/>
        <location filename="../src/bmainwindow.cpp" line="1682"/>
        <location filename="../src/bmainwindow.cpp" line="2585"/>
        <source>Statistics ▼</source>
        <translation>Statistiques ▼</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1671"/>
        <source>Show/Hide Statistics</source>
        <translation>Afficher/masquer les statistiques</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1682"/>
        <location filename="../src/bmainwindow.cpp" line="2585"/>
        <location filename="../src/bmainwindow.cpp" line="2621"/>
        <source>Statistics ▶</source>
        <translation>Statistiques ▶</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1710"/>
        <source>No Connection Profiles</source>
        <translation>Aucun profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1711"/>
        <source>No connection profiles found.

Please create a connection profile in Settings first.</source>
        <translation>Aucun profil de connexion trouvé.

Veuillez d&apos;abord créer un profil de connexion dans les Paramètres.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1725"/>
        <source>Select Connection Profile</source>
        <translation>Sélectionner un profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1731"/>
        <source>Connection Profile</source>
        <translation>Profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1781"/>
        <source>Manage Profiles...</source>
        <translation>Gérer les profils...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1809"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1810"/>
        <source>Selected profile is invalid.</source>
        <translation>Le profil sélectionné n&apos;est pas valide.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2908"/>
        <location filename="../src/bmainwindow.cpp" line="2971"/>
        <source>Connecting to %1...</source>
        <translation>Connexion à %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1881"/>
        <source>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Version %1 (Build %2)&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Version %1 (Build %2)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1888"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bacula Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Interface Qt6 moderne pour les sauvegardes Bacula&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1890"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bareos Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Interface Qt6 moderne pour les sauvegardes Bareos&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1980"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1996"/>
        <source>Documentation</source>
        <translation>Documentation</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1997"/>
        <location filename="../src/bmainwindow.cpp" line="2007"/>
        <source>Could not open browser. Please visit:
%1</source>
        <translation>Impossible d&apos;ouvrir le navigateur. Veuillez consulter :
%1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2006"/>
        <source>Report Bug</source>
        <translation>Signaler un bogue</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2054"/>
        <source>Find</source>
        <translation>Rechercher</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2069"/>
        <source>Toggle Theme</source>
        <translation>Changer de thème</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2072"/>
        <source>Reset Interface Zoom</source>
        <translation>Réinitialiser le zoom de l&apos;interface</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2093"/>
        <source>Data</source>
        <translation>Données</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2102"/>
        <source>Help Contents</source>
        <translation>Sommaire de l&apos;aide</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2434"/>
        <source>Not connected to a Director.</source>
        <translation>Non connecté à un Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2456"/>
        <source>Settings saved</source>
        <translation>Paramètres enregistrés</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2493"/>
        <source>Disconnect from Director</source>
        <translation>Se déconnecter du Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2571"/>
        <source>Connected to %1 version %2</source>
        <translation>Connecté à %1 version %2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2573"/>
        <source>Connected: version %1</source>
        <translation>Connecté : version %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2579"/>
        <source>Connected - Loading data...</source>
        <translation>Connecté - Chargement des données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2644"/>
        <source>Error: </source>
        <translation>Erreur : </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2651"/>
        <source>Connection Error</source>
        <translation>Erreur de connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2666"/>
        <source>Refreshing data...</source>
        <translation>Actualisation des données...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2706"/>
        <source>No Saved Connection</source>
        <translation>Aucune connexion enregistrée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2707"/>
        <source>No previous connection found. Please use &apos;Connect&apos; to establish a new connection.</source>
        <translation>Aucune connexion précédente trouvée. Veuillez utiliser « Se connecter » pour établir une nouvelle connexion.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2907"/>
        <source>Auto-connecting to %1...</source>
        <translation>Connexion automatique à %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2951"/>
        <source>Connection cancelled: server certificate not trusted</source>
        <translation>Connexion annulée : certificat du serveur non approuvé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3050"/>
        <source>All jobs selected</source>
        <translation>Tous les jobs sélectionnés</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3065"/>
        <location filename="../src/bmainwindow.cpp" line="3068"/>
        <source>Selection cleared</source>
        <translation>Sélection annulée</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3112"/>
        <source>Reset Interface Zoom (now %1 %)</source>
        <translation>Réinitialiser le zoom de l&apos;interface (actuellement %1 %)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3123"/>
        <source>Switched to Light Theme</source>
        <translation>Thème clair activé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3124"/>
        <source>Switched to Dark Theme</source>
        <translation>Thème sombre activé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3133"/>
        <location filename="../src/bmainwindow.cpp" line="3145"/>
        <location filename="../src/bmainwindow.cpp" line="3157"/>
        <source>Not Connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3133"/>
        <location filename="../src/bmainwindow.cpp" line="3145"/>
        <source>Please connect to the Director first.</source>
        <translation>Veuillez d&apos;abord vous connecter au Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3139"/>
        <source>Jobs exported as JSON</source>
        <translation>Jobs exportés en JSON</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3151"/>
        <source>Jobs exported as CSV</source>
        <translation>Jobs exportés en CSV</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3158"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Veuillez d&apos;abord vous connecter au Director Bareos.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3167"/>
        <source>Loading job data...</source>
        <translation>Chargement des données des jobs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3203"/>
        <source>Connection profile &apos;%1&apos; saved</source>
        <translation>Profil de connexion &apos;%1&apos; enregistré</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3253"/>
        <source>Password Missing</source>
        <translation>Mot de passe manquant</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3254"/>
        <source>The imported Console &apos;%1&apos; does not contain a password.

You will need to enter the password manually in the connection settings.</source>
        <translation>La Console importée &apos;%1&apos; ne contient pas de mot de passe.

Vous devrez saisir le mot de passe manuellement dans les paramètres de connexion.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3275"/>
        <source>Created connection profile &apos;%1&apos; for Director &apos;%2&apos;</source>
        <translation>Profil de connexion &apos;%1&apos; créé pour le Director &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3280"/>
        <source>Connect Now?</source>
        <translation>Se connecter maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3281"/>
        <source>Connection profile &apos;%1&apos; has been created.

Do you want to connect now?</source>
        <translation>Le profil de connexion &apos;%1&apos; a été créé.

Voulez-vous vous connecter maintenant ?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3297"/>
        <source>Imported Director &apos;%1&apos; with Console &apos;%2&apos;</source>
        <translation>Director &apos;%1&apos; importé avec la Console &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2089"/>
        <source>Add Schedule</source>
        <translation>Ajouter un Schedule</translation>
    </message>
</context>
<context>
    <name>BMdnsBrowser</name>
    <message>
        <location filename="../src/bmdnsbrowser.cpp" line="200"/>
        <source>mDNS: cannot open UDP socket: %1</source>
        <translation>mDNS : impossible d&apos;ouvrir le socket UDP : %1</translation>
    </message>
</context>
<context>
    <name>BMessagesResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="431"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="431"/>
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BMessagesWidget</name>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="237"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="243"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="250"/>
        <source>0 messages</source>
        <translation>0 message</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="286"/>
        <source>Polling every %1s</source>
        <translation>Interrogation toutes les %1 s</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="313"/>
        <source>Messages cleared</source>
        <translation>Messages effacés</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="328"/>
        <source>Not connected</source>
        <translation>Non connecté</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="369"/>
        <source>Refreshing messages...</source>
        <translation>Actualisation des messages...</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/messages/bmessageswidget.cpp" line="375"/>
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
        <location filename="../src/clients/bnewclientdialog.cpp" line="91"/>
        <source>Director Information</source>
        <translation>Informations sur le Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="92"/>
        <source>The new client will connect to this Director. These settings are used to generate the File Daemon configuration.</source>
        <translation>Le nouveau client se connectera à ce Director. Ces paramètres servent à générer la configuration du File Daemon.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="99"/>
        <source>Director Name:</source>
        <translation>Nom du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="103"/>
        <source>Director Address:</source>
        <translation>Adresse du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="107"/>
        <source>Director Port:</source>
        <translation>Port du Director :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="111"/>
        <source>TLS Mode:</source>
        <translation>Mode TLS :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="119"/>
        <source>The Director name above will be used in the File Daemon&apos;s Director resource. The client authenticates against this Director using a shared password.</source>
        <translation>Le nom du Director ci-dessus sera utilisé dans la ressource Director du File Daemon. Le client s&apos;authentifie auprès de ce Director à l&apos;aide d&apos;un mot de passe partagé.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="139"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="146"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="153"/>
        <source>&lt;i&gt;(not available)&lt;/i&gt;</source>
        <translation>&lt;i&gt;(non disponible)&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="140"/>
        <source>Director name could not be retrieved from the connection.</source>
        <translation>Le nom du Director n&apos;a pas pu être récupéré depuis la connexion.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="147"/>
        <source>Director address could not be retrieved.</source>
        <translation>L&apos;adresse du Director n&apos;a pas pu être récupérée.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="154"/>
        <source>Director port could not be retrieved.</source>
        <translation>Le port du Director n&apos;a pas pu être récupéré.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="159"/>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="164"/>
        <source>&lt;b style=&apos;color:red;&apos;&gt;Cannot proceed:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Make sure you are connected to a Director.</source>
        <translation>&lt;b style=&apos;color:red;&apos;&gt;Impossible de continuer :&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Assurez-vous d&apos;être connecté à un Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="182"/>
        <source>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; The current console may not have permission to run &apos;configure add client&apos;. Check the Command ACL in the Console resource on the Director.&lt;/span&gt;&lt;br&gt;You can still generate and export the FD configuration files.</source>
        <translation>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Attention :&lt;/b&gt; la console actuelle n&apos;a peut-être pas l&apos;autorisation d&apos;exécuter &apos;configure add client&apos;. Vérifiez le Command ACL dans la ressource Console du Director.&lt;/span&gt;&lt;br&gt;Vous pouvez tout de même générer et exporter les fichiers de configuration FD.</translation>
    </message>
</context>
<context>
    <name>BNewClientPreviewPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="484"/>
        <source>Preview &amp; Export</source>
        <translation>Aperçu &amp; exportation</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="485"/>
        <source>Review generated configuration files, validate, and export.</source>
        <translation>Vérifiez les fichiers de configuration générés, validez-les et exportez-les.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="496"/>
        <source>FD: Director</source>
        <translation>FD : Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="499"/>
        <source>FD: Client</source>
        <translation>FD : Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="502"/>
        <source>FD: Messages</source>
        <translation>FD : Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="505"/>
        <source>Dir: Client</source>
        <translation>Dir : Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="524"/>
        <source>Director Command</source>
        <translation>Commande du Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="536"/>
        <source>Copy Current Tab</source>
        <translation>Copier l&apos;onglet actuel</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="540"/>
        <source>Copy All</source>
        <translation>Tout copier</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="544"/>
        <source>Export ZIP...</source>
        <translation>Exporter en ZIP...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="779"/>
        <source>FD Director</source>
        <translation>FD Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="780"/>
        <source>FD Client</source>
        <translation>FD Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="781"/>
        <source>FD Messages</source>
        <translation>FD Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="782"/>
        <source>Dir Client</source>
        <translation>Dir Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="788"/>
        <source>%1: Config syntax could not be parsed.</source>
        <translation>%1 : la syntaxe de la configuration n&apos;a pas pu être analysée.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="799"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="808"/>
        <source>Client name &apos;%1&apos; does not end with &apos;-fd&apos;. Convention: use &apos;&lt;hostname&gt;-fd&apos;.</source>
        <translation>Le nom du client &apos;%1&apos; ne se termine pas par &apos;-fd&apos;. Convention : utilisez &apos;&lt;hostname&gt;-fd&apos;.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="813"/>
        <source>Password MD5 hash has invalid length (%1, expected 32).</source>
        <translation>Le hachage MD5 du mot de passe a une longueur invalide (%1, 32 attendus).</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="819"/>
        <source>Validation passed.</source>
        <translation>Validation réussie.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="912"/>
        <source>Copied &apos;%1&apos; to clipboard.</source>
        <translation>&apos;%1&apos; copié dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="937"/>
        <source>All configuration files copied to clipboard.</source>
        <translation>Tous les fichiers de configuration ont été copiés dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="949"/>
        <source>Export Client Configuration</source>
        <translation>Exporter la configuration du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="951"/>
        <source>ZIP Archives (*.zip)</source>
        <translation>Archives ZIP (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="957"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1030"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1036"/>
        <source>Export Error</source>
        <translation>Erreur d&apos;exportation</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="958"/>
        <source>Failed to create temporary directory.</source>
        <translation>Impossible de créer le répertoire temporaire.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1031"/>
        <source>ZIP creation timed out. Is &apos;zip&apos; installed?</source>
        <translation>La création du ZIP a dépassé le délai. &apos;zip&apos; est-il installé ?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1037"/>
        <source>ZIP creation failed:
%1</source>
        <translation>Échec de la création du ZIP :
%1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1042"/>
        <source>Exported to: %1</source>
        <translation>Exporté vers : %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1051"/>
        <source>ERROR: No director connection available.</source>
        <translation>ERREUR : aucune connexion au Director disponible.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1060"/>
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1101"/>
        <source>Failed to add client to Director: %1
Go back to change the settings, or click Finish to send the command again.
You can still export the FD config files manually.</source>
        <translation>Impossible d&apos;ajouter le client au Director : %1
Revenez en arrière pour modifier les paramètres, ou cliquez sur Terminer pour renvoyer la commande.
Vous pouvez tout de même exporter manuellement les fichiers de configuration FD.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1114"/>
        <source>Client &apos;%1&apos; added to Director.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client &apos;%1&apos; ajouté au Director.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1124"/>
        <source>Client &apos;%1&apos; added to Director. Reloading Director...</source>
        <translation>Client &apos;%1&apos; ajouté au Director. Rechargement du Director...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1151"/>
        <source>Client &apos;%1&apos; added and Director reloaded successfully.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client &apos;%1&apos; ajouté et Director rechargé avec succès.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1158"/>
        <source>Client &apos;%1&apos; added to Director, but reload failed.
Run &apos;reload&apos; manually on the Director or restart bareos-dir.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client &apos;%1&apos; ajouté au Director, mais le rechargement a échoué.
Exécutez &apos;reload&apos; manuellement sur le Director ou redémarrez bareos-dir.
Déployez les fichiers de configuration FD sur la machine cliente et redémarrez bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1177"/>
        <source>Client &apos;%1&apos; added to Director, but the connection was lost before the reload was confirmed.
Reconnect and run &apos;reload&apos; on the Director if the client does not appear.</source>
        <translation>Client &apos;%1&apos; ajouté au Director, mais la connexion a été perdue avant la confirmation du rechargement.
Reconnectez-vous et exécutez &apos;reload&apos; sur le Director si le client n&apos;apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1186"/>
        <source>The connection to the Director was lost before it answered.
Reconnect and check whether client &apos;%1&apos; exists before sending the command again.</source>
        <translation>La connexion au Director a été perdue avant sa réponse.
Reconnectez-vous et vérifiez si le client &apos;%1&apos; existe avant de renvoyer la commande.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1204"/>
        <source>Client &apos;%1&apos; added to Director, but the Director did not confirm the reload within 30 seconds.
Run &apos;reload&apos; manually on the Director if the client does not appear.</source>
        <translation>Client &apos;%1&apos; ajouté au Director, mais le Director n&apos;a pas confirmé le rechargement dans les 30 secondes.
Exécutez &apos;reload&apos; manuellement sur le Director si le client n&apos;apparaît pas.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1214"/>
        <source>Command Timeout</source>
        <translation>Délai de la commande dépassé</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1215"/>
        <source>The &apos;configure add client&apos; command did not respond within 30 seconds.

The Director may be busy or the connection may have been lost.

Retry the command?</source>
        <translation>La commande &apos;configure add client&apos; n&apos;a pas répondu dans les 30 secondes.

Le Director est peut-être occupé ou la connexion a peut-être été perdue.

Réessayer la commande ?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1227"/>
        <source>Configure command timed out. The client may or may not have been created.
You can export the FD config files and configure the Director manually.</source>
        <translation>La commande configure a dépassé le délai. Le client a peut-être été créé, ou non.
Vous pouvez exporter les fichiers de configuration FD et configurer le Director manuellement.</translation>
    </message>
</context>
<context>
    <name>BNewClientSettingsPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="207"/>
        <source>Client Settings</source>
        <translation>Paramètres du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="208"/>
        <source>Configure the new client&apos;s identity, password, TLS, and network settings.</source>
        <translation>Configurez l&apos;identité, le mot de passe, le TLS et les paramètres réseau du nouveau client.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="213"/>
        <source>Basic Settings</source>
        <translation>Paramètres de base</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="217"/>
        <source>e.g. server1-fd</source>
        <translation>p. ex. server1-fd</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="218"/>
        <source>Client Name:</source>
        <translation>Nom du client :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="221"/>
        <source>e.g. 192.168.1.100 or server1.example.com</source>
        <translation>p. ex. 192.168.1.100 ou server1.example.com</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="222"/>
        <source>Address:</source>
        <translation>Adresse :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="227"/>
        <source>FD Port:</source>
        <translation>Port FD :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="232"/>
        <source>Client password</source>
        <translation>Mot de passe du client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="234"/>
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="236"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="242"/>
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="252"/>
        <source>Passive mode (client behind NAT/firewall)</source>
        <translation>Mode passif (client derrière un NAT/pare-feu)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="254"/>
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
        <location filename="../src/clients/bnewclientdialog.cpp" line="263"/>
        <source>Connection wait time:</source>
        <translation>Temps d&apos;attente de connexion :</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="267"/>
        <source> sec</source>
        <translation> s</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="268"/>
        <source>Time in seconds to wait for client-initiated connection</source>
        <translation>Durée en secondes d&apos;attente d&apos;une connexion initiée par le client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="277"/>
        <source>Execute &quot;configure add client&quot; on Director</source>
        <translation>Exécuter « configure add client » sur le Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="280"/>
        <source>If checked, the wizard will send the configure command to the Director
on the Preview page to add this client automatically.</source>
        <translation>Si cette case est cochée, l&apos;assistant enverra la commande configure au Director
sur la page Aperçu pour ajouter automatiquement ce client.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="420"/>
        <source>(enter password)</source>
        <translation>(saisir le mot de passe)</translation>
    </message>
</context>
<context>
    <name>BNewClientWizard</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="56"/>
        <source>Add New Client</source>
        <translation>Ajouter un nouveau client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="70"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="72"/>
        <source>TLS Certificate</source>
        <translation>Certificat TLS</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="75"/>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
</context>
<context>
    <name>BNewJobDialog</name>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="30"/>
        <source>Run New Job</source>
        <translation>Exécuter un nouveau job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="56"/>
        <source>Run Backup Job</source>
        <translation>Exécuter un job de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="66"/>
        <source>Command Preview</source>
        <translation>Aperçu de la commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="85"/>
        <source>Estimate</source>
        <translation>Estimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="87"/>
        <source>Calculate estimated files and size</source>
        <translation>Calculer le nombre de fichiers et la taille estimés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="97"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="654"/>
        <source>Run Job</source>
        <translation>Exécuter le job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="119"/>
        <source>Job Configuration</source>
        <translation>Configuration du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="124"/>
        <source>Select job...</source>
        <translation>Sélectionner un job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="127"/>
        <source>Job:</source>
        <translation>Job :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="130"/>
        <source>Select client...</source>
        <translation>Sélectionner un client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="133"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="139"/>
        <source>Level:</source>
        <translation>Niveau :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="144"/>
        <source>Resources</source>
        <translation>Ressources</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="149"/>
        <source>Select FileSet...</source>
        <translation>Sélectionner un FileSet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="152"/>
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="155"/>
        <source>Select Pool...</source>
        <translation>Sélectionner un Pool...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="158"/>
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="161"/>
        <source>Select Storage...</source>
        <translation>Sélectionner un Storage...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="164"/>
        <source>Storage:</source>
        <translation>Storage :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="169"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="175"/>
        <source>Lower values = higher priority. Only sent if changed here; otherwise the job&apos;s own priority applies.</source>
        <translation>Valeur plus basse = priorité plus haute. Envoyée uniquement si modifiée ici ; sinon, la priorité propre du job s&apos;applique.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="182"/>
        <source>Priority:</source>
        <translation>Priorité :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="188"/>
        <source>Basic</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="198"/>
        <source>Scheduling</source>
        <translation>Planification</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="202"/>
        <source>e.g. &quot;2024-01-15 14:30:00&quot; or empty for immediate</source>
        <translation>p. ex. &quot;2024-01-15 14:30:00&quot; ou vide pour immédiat</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="203"/>
        <source>Start time in the form YYYY-MM-DD HH:MM:SS</source>
        <translation>Heure de début au format YYYY-MM-DD HH:MM:SS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="205"/>
        <source>When:</source>
        <translation>Quand :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="210"/>
        <source>Bootstrap Options</source>
        <translation>Options de bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="213"/>
        <source>Use Bootstrap file</source>
        <translation>Utiliser un fichier bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="218"/>
        <source>Path to Bootstrap file</source>
        <translation>Chemin du fichier bootstrap</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bnewjobdialog.cpp" line="708"/>
        <source>Estimate: %n file(s), %1</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="713"/>
        <source>The Director sent no estimate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="714"/>
        <source>Estimate failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="229"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="235"/>
        <source>⚠ No JobWidget connection available</source>
        <translation>⚠ Aucune connexion JobWidget disponible</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="240"/>
        <source>Loading configuration data from JobWidget...</source>
        <translation>Chargement des données de configuration depuis JobWidget...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="278"/>
        <source>✓ %1 jobs, %2 clients loaded</source>
        <translation>✓ %1 jobs, %2 clients chargés</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="284"/>
        <source>⚠ No jobs available - check connection</source>
        <translation>⚠ Aucun job disponible - vérifiez la connexion</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="372"/>
        <source>✓ Configuration loaded</source>
        <translation>✓ Configuration chargée</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="411"/>
        <source>Loading job defaults...</source>
        <translation>Chargement des valeurs par défaut du job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="439"/>
        <source>Failed to load job defaults</source>
        <translation>Impossible de charger les valeurs par défaut du job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="550"/>
        <source>✓ Job defaults loaded</source>
        <translation>✓ Valeurs par défaut du job chargées</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="581"/>
        <source>No job selected.</source>
        <translation>Aucun job sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="583"/>
        <source>The value for &apos;%1&apos; contains a line break or control character.</source>
        <translation>La valeur de &apos;%1&apos; contient un saut de ligne ou un caractère de contrôle.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="585"/>
        <source>&apos;When&apos; must be a valid time in the form YYYY-MM-DD HH:MM:SS.</source>
        <translation>« Quand » doit être une heure valide au format YYYY-MM-DD HH:MM:SS.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="587"/>
        <source>The priority must be a positive number.</source>
        <translation>La priorité doit être un nombre positif.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="589"/>
        <source>The command could not be built.</source>
        <translation>La commande n&apos;a pas pu être construite.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="607"/>
        <source>Nothing will be sent: %1</source>
        <translation>Rien ne sera envoyé : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="634"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="675"/>
        <source>Missing Input</source>
        <translation>Saisie manquante</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="635"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="676"/>
        <source>Please select a job.</source>
        <translation>Veuillez sélectionner un job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="643"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="683"/>
        <source>Invalid Input</source>
        <translation>Saisie invalide</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="655"/>
        <source>Do you want to run the following job?

%1</source>
        <translation>Voulez-vous exécuter le job suivant ?

%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="698"/>
        <source>Calculating estimate...</source>
        <translation>Calcul de l&apos;estimation...</translation>
    </message>
</context>
<context>
    <name>BPFXConverter</name>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="25"/>
        <source>Convert to PFX Format</source>
        <translation>Convertir au format PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="33"/>
        <source>Convert separate PEM/DER certificate and key files into a single PFX file for Windows.
PFX files are password-protected and contain both certificate and private key.</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un seul fichier PFX pour Windows.
Les fichiers PFX sont protégés par mot de passe et contiennent à la fois le certificat et la clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="42"/>
        <source>Input Files</source>
        <translation>Fichiers d&apos;entrée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="48"/>
        <source>Select certificate file...</source>
        <translation>Sélectionner le fichier de certificat...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="49"/>
        <location filename="../src/bpfxconverter.cpp" line="59"/>
        <location filename="../src/bpfxconverter.cpp" line="69"/>
        <location filename="../src/bpfxconverter.cpp" line="100"/>
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="53"/>
        <source>Certificate:</source>
        <translation>Certificat :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="58"/>
        <source>Select private key file...</source>
        <translation>Sélectionner le fichier de clé privée...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="63"/>
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="68"/>
        <source>Optional: CA certificate</source>
        <translation>Facultatif : certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="73"/>
        <source>CA Cert (Optional):</source>
        <translation>Certificat CA (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="78"/>
        <source>PFX Password Protection</source>
        <translation>Protection du PFX par mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="83"/>
        <source>Enter password for PFX file...</source>
        <translation>Saisir le mot de passe du fichier PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="84"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="88"/>
        <source>Confirm password...</source>
        <translation>Confirmer le mot de passe...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="89"/>
        <source>Confirm:</source>
        <translation>Confirmation :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="94"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="99"/>
        <source>Select where to save PFX file...</source>
        <translation>Choisir où enregistrer le fichier PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="103"/>
        <source>Save PFX File</source>
        <translation>Enregistrer le fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="105"/>
        <source>PFX Files (*.pfx);;All Files (*)</source>
        <translation>Fichiers PFX (*.pfx);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="115"/>
        <source>Save As:</source>
        <translation>Enregistrer sous :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="120"/>
        <source>Conversion Log:</source>
        <translation>Journal de conversion :</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="131"/>
        <source>Convert to PFX</source>
        <translation>Convertir en PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="135"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="146"/>
        <source>Select Certificate File</source>
        <translation>Sélectionner le fichier de certificat</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="148"/>
        <source>Certificates (*.pem *.crt *.cert *.der);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert *.der);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="151"/>
        <source>✓ Certificate selected: %1</source>
        <translation>✓ Certificat sélectionné : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="158"/>
        <source>Select Private Key File</source>
        <translation>Sélectionner le fichier de clé privée</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="160"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés privées (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="163"/>
        <source>✓ Private key selected: %1</source>
        <translation>✓ Clé privée sélectionnée : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="170"/>
        <source>Select CA Certificate</source>
        <translation>Sélectionner le certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="172"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="175"/>
        <source>✓ CA certificate selected: %1</source>
        <translation>✓ Certificat CA sélectionné : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="183"/>
        <location filename="../src/bpfxconverter.cpp" line="190"/>
        <location filename="../src/bpfxconverter.cpp" line="197"/>
        <source>Missing Input</source>
        <translation>Saisie manquante</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="184"/>
        <source>Please select a certificate file.</source>
        <translation>Veuillez sélectionner un fichier de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="191"/>
        <source>Please select a private key file.</source>
        <translation>Veuillez sélectionner un fichier de clé privée.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="198"/>
        <source>Please enter a password to protect the PFX file.</source>
        <translation>Veuillez saisir un mot de passe pour protéger le fichier PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="204"/>
        <source>Password Mismatch</source>
        <translation>Les mots de passe ne correspondent pas</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="205"/>
        <source>The passwords do not match. Please try again.</source>
        <translation>Les mots de passe ne correspondent pas. Veuillez réessayer.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="211"/>
        <source>Missing Output</source>
        <translation>Sortie manquante</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="212"/>
        <source>Please specify where to save the PFX file.</source>
        <translation>Veuillez indiquer où enregistrer le fichier PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="222"/>
        <location filename="../src/bpfxconverter.cpp" line="224"/>
        <location filename="../src/bpfxconverter.cpp" line="231"/>
        <location filename="../src/bpfxconverter.cpp" line="234"/>
        <source>========================================</source>
        <translation>========================================</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="223"/>
        <source>Starting PFX Conversion</source>
        <translation>Démarrage de la conversion PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="232"/>
        <source>✓ Conversion successful!</source>
        <translation>✓ Conversion réussie !</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="233"/>
        <source>PFX file created: %1</source>
        <translation>Fichier PFX créé : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="238"/>
        <source>Success</source>
        <translation>Succès</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="239"/>
        <source>PFX file created successfully!

%1

You can now use this file in the certificate settings.</source>
        <translation>Fichier PFX créé avec succès !

%1

Vous pouvez maintenant utiliser ce fichier dans les paramètres de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="252"/>
        <source>✗ ERROR: Cannot open %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="254"/>
        <source>Cannot open %1:
%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="262"/>
        <source>Loading certificate from: %1</source>
        <translation>Chargement du certificat depuis : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="263"/>
        <source>the certificate file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="267"/>
        <source>the private key file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="272"/>
        <source>the CA certificate file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="283"/>
        <source>✗ ERROR: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="285"/>
        <source>The PFX file was not created.

%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="253"/>
        <location filename="../src/bpfxconverter.cpp" line="284"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="266"/>
        <source>Loading private key from: %1</source>
        <translation>Chargement de la clé privée depuis : %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="279"/>
        <source>Creating PFX (PKCS#12) file...</source>
        <translation>Création du fichier PFX (PKCS#12)...</translation>
    </message>
</context>
<context>
    <name>BPaginationWidget</name>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="29"/>
        <source>Enable Pagination</source>
        <translation>Activer la pagination</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="35"/>
        <source>Items per page:</source>
        <translation>Éléments par page :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="49"/>
        <source>First Page</source>
        <translation>Première page</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="55"/>
        <source>Previous Page</source>
        <translation>Page précédente</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="61"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="314"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="337"/>
        <source>Page 0 of 0</source>
        <translation>Page 0 sur 0</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="67"/>
        <source>Next Page</source>
        <translation>Page suivante</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="73"/>
        <source>Last Page</source>
        <translation>Dernière page</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="81"/>
        <source>Go to page:</source>
        <translation>Aller à la page :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="88"/>
        <source>Go</source>
        <translation>Aller</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="348"/>
        <source>Page %1 of %2</source>
        <translation>Page %1 sur %2</translation>
    </message>
</context>
<context>
    <name>BPermissionsDialog</name>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2135"/>
        <source>My Permissions</source>
        <translation>Mes autorisations</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2148"/>
        <source>Connection</source>
        <translation>Connexion</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2150"/>
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2151"/>
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2158"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2160"/>
        <source>TLS Certificate</source>
        <translation>Certificat TLS</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2162"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2164"/>
        <source>Encryption:</source>
        <translation>Chiffrement :</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2170"/>
        <source>Backup &amp; Restore</source>
        <translation>Sauvegarde &amp; restauration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2171"/>
        <source>Job Control</source>
        <translation>Contrôle des jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2172"/>
        <source>Administration</source>
        <translation>Administration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2174"/>
        <source>Media</source>
        <translation>Supports</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2185"/>
        <source>Command Permissions</source>
        <translation>Autorisations des commandes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2192"/>
        <source>Checking...</source>
        <translation>Vérification...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2201"/>
        <source>Restore Details</source>
        <translation>Détails de la restauration</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2203"/>
        <location filename="../src/bmainwindow.cpp" line="2212"/>
        <source>Loading...</source>
        <translation>Chargement...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2210"/>
        <source>Other Available Commands</source>
        <translation>Autres commandes disponibles</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2219"/>
        <source>Raw Director Response</source>
        <translation>Réponse brute du Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2237"/>
        <source>Querying Director...</source>
        <translation>Interrogation du Director...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2326"/>
        <source>N/A</source>
        <translation>N/D</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2329"/>
        <source>Authorized</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2333"/>
        <source>Not authorized</source>
        <translation>Non autorisé</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2361"/>
        <source>&lt;b&gt;Permission:&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;Autorisation :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2363"/>
        <source>&lt;b&gt;where= (restore path):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;where= (chemin de restauration) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2365"/>
        <source>&lt;b&gt;client= (target client):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;client= (client cible) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2367"/>
        <source>&lt;b&gt;replace= (replace policy):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;replace= (politique de remplacement) :&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2371"/>
        <source>&lt;br&gt;&lt;b&gt;Available parameters:&lt;/b&gt; %1</source>
        <translation>&lt;br&gt;&lt;b&gt;Paramètres disponibles :&lt;/b&gt; %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2392"/>
        <source>Total: %1 commands (%2 authorized, %3 denied)</source>
        <translation>Total : %1 commandes (%2 autorisées, %3 refusées)</translation>
    </message>
</context>
<context>
    <name>BPki</name>
    <message>
        <location filename="../src/bpki.cpp" line="83"/>
        <source>Cannot open %1 for writing: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="89"/>
        <source>Cannot write %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="103"/>
        <source>Cannot create certificate extension %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="109"/>
        <source>Cannot add certificate extension %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="150"/>
        <source>Cannot create the key context</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="157"/>
        <source>Cannot generate the RSA key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="171"/>
        <source>Invalid certificate request</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="178"/>
        <source>Cannot create the certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="186"/>
        <source>Cannot set the public key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="198"/>
        <source>Cannot set the subject name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="204"/>
        <source>Cannot set the issuer name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="233"/>
        <source>Cannot sign the certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="258"/>
        <source>Cannot prepare the private key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="270"/>
        <source>Cannot encode the private key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="280"/>
        <source>Cannot encode the certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="291"/>
        <source>A PFX file needs a password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="297"/>
        <source>A PFX file needs a certificate and its private key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="303"/>
        <source>The private key does not belong to the certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="313"/>
        <source>Cannot prepare the certificate chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="323"/>
        <source>Cannot create the PKCS#12 structure</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="331"/>
        <source>Cannot encode the PFX file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="347"/>
        <source>Not a PEM or DER certificate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="373"/>
        <source>The private key is encrypted; decrypt it first (openssl pkey -in key.pem -out plain.pem)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="376"/>
        <source>Not a PEM or DER private key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="398"/>
        <source>Cannot open %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="406"/>
        <source>%1 is not a PKCS#12 file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="415"/>
        <source>Cannot open %1 with this password</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="434"/>
        <source>Certificate: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="439"/>
        <source>Private key: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="446"/>
        <source>CA certificate: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bpki.cpp" line="458"/>
        <source>The written PFX file cannot be read back</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BPoolResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="377"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="377"/>
        <source>Pool Type</source>
        <translation>Pool Type</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="377"/>
        <source>Recycle</source>
        <translation>Recycle</translation>
    </message>
</context>
<context>
    <name>BProfileSettingsDialog</name>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="27"/>
        <source>Advanced Profile Settings - %1</source>
        <translation>Paramètres avancés du profil - %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="44"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="45"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="46"/>
        <source>ACL</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="69"/>
        <source>Optional description for this connection</source>
        <translation>Description facultative de cette connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="70"/>
        <source>Description:</source>
        <translation>Description :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="75"/>
        <source> seconds</source>
        <translation> secondes</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="76"/>
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="77"/>
        <source>Keepalive interval (0 = disabled)</source>
        <translation>Intervalle de keepalive (0 = désactivé)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="78"/>
        <source>Heartbeat Interval:</source>
        <translation>Intervalle de heartbeat :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="82"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="83"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="89"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d&apos;Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="91"/>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="182"/>
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="96"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Port gRPC du Director (par défaut : port BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="97"/>
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="106"/>
        <source>Forget certificate</source>
        <translation>Oublier le certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="107"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Supprimer le certificat épinglé. La prochaine connexion demandera de nouveau si le certificat du serveur est approuvé.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="110"/>
        <source>Server certificate:</source>
        <translation>Certificat du serveur :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="122"/>
        <source>e.g., operator or readonly</source>
        <translation>p. ex. operator ou readonly</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="123"/>
        <source>Reference to a Profile resource on the Director.
The Profile defines ACLs for this console.</source>
        <translation>Référence à une ressource Profile sur le Director.
Le Profile définit les ACL de cette console.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="126"/>
        <source>Director Profile:</source>
        <translation>Profile du Director :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="130"/>
        <source>&lt;b&gt;Note:&lt;/b&gt; The Director Profile is a server-side resource that defines permissions. If set, the ACLs in this profile are ignored in favor of the server-side Profile.</source>
        <translation>&lt;b&gt;Remarque :&lt;/b&gt; le Profile du Director est une ressource côté serveur qui définit les autorisations. S&apos;il est défini, les ACL de ce profil sont ignorées au profit du Profile côté serveur.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="150"/>
        <source>TLS Options</source>
        <translation>Options TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="153"/>
        <source>TLS Required</source>
        <translation>TLS requis</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="154"/>
        <source>Require TLS for all connections (recommended)</source>
        <translation>Exiger TLS pour toutes les connexions (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="157"/>
        <source>TLS Authenticate Only</source>
        <translation>TLS pour l&apos;authentification uniquement</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="158"/>
        <source>Use TLS only for authentication, not for data encryption</source>
        <translation>Utiliser TLS uniquement pour l&apos;authentification, pas pour le chiffrement des données</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="164"/>
        <source>Cipher Configuration</source>
        <translation>Configuration des chiffrements</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="168"/>
        <source>TLSv1.2 ciphers (colon-separated)</source>
        <translation>Chiffrements TLSv1.2 (séparés par des deux-points)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="169"/>
        <source>OpenSSL cipher list for TLSv1.2 connections
Example: HIGH:!aNULL:!MD5</source>
        <translation>Liste de chiffrements OpenSSL pour les connexions TLSv1.2
Exemple : HIGH:!aNULL:!MD5</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="171"/>
        <source>Cipher List (TLSv1.2):</source>
        <translation>Liste de chiffrements (TLSv1.2) :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="174"/>
        <source>TLSv1.3 cipher suites (colon-separated)</source>
        <translation>Suites de chiffrement TLSv1.3 (séparées par des deux-points)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="175"/>
        <source>OpenSSL cipher suites for TLSv1.3 connections
Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</source>
        <translation>Suites de chiffrement OpenSSL pour les connexions TLSv1.3
Exemple : TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="177"/>
        <source>Cipher Suites (TLSv1.3):</source>
        <translation>Suites de chiffrement (TLSv1.3) :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="180"/>
        <source>e.g., -ALL,TLSv1.2,TLSv1.3</source>
        <translation>p. ex. -ALL,TLSv1.2,TLSv1.3</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="181"/>
        <source>OpenSSL protocol string</source>
        <translation>Chaîne de protocole OpenSSL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="187"/>
        <source>Certificate Files</source>
        <translation>Fichiers de certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="191"/>
        <source>DH Parameters:</source>
        <translation>Paramètres DH :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="193"/>
        <source>Path to Diffie-Hellman parameters file</source>
        <translation>Chemin du fichier de paramètres Diffie-Hellman</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="195"/>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="211"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="198"/>
        <source>Select DH Parameters File</source>
        <translation>Sélectionner le fichier de paramètres DH</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="199"/>
        <source>PEM Files (*.pem);;All Files (*)</source>
        <translation>Fichiers PEM (*.pem);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="207"/>
        <source>CRL File:</source>
        <translation>Fichier CRL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="209"/>
        <source>Path to Certificate Revocation List file</source>
        <translation>Chemin du fichier de liste de révocation de certificats</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="214"/>
        <source>Select CRL File</source>
        <translation>Sélectionner le fichier CRL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="215"/>
        <source>CRL Files (*.crl *.pem);;All Files (*)</source>
        <translation>Fichiers CRL (*.crl *.pem);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="225"/>
        <source>Allowed Certificate Common Names</source>
        <translation>Noms communs de certificat autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="229"/>
        <source>One Common Name per line</source>
        <translation>Un nom commun par ligne</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="230"/>
        <source>List of allowed certificate Common Names.
Leave empty to allow all valid certificates.</source>
        <translation>Liste des noms communs de certificat autorisés.
Laisser vide pour autoriser tous les certificats valides.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="253"/>
        <source>&lt;b&gt;Access Control Lists (ACLs)&lt;/b&gt;&lt;br&gt;Define which resources this console can access. One entry per line. Use &lt;code&gt;*all*&lt;/code&gt; to allow all resources of that type. Regex patterns are supported for Job ACL.</source>
        <translation>&lt;b&gt;Listes de contrôle d&apos;accès (ACL)&lt;/b&gt;&lt;br&gt;Définissez les ressources auxquelles cette console peut accéder. Une entrée par ligne. Utilisez &lt;code&gt;*all*&lt;/code&gt; pour autoriser toutes les ressources de ce type. Les expressions régulières sont prises en charge pour le Job ACL.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="275"/>
        <source>Job ACL:</source>
        <translation>Job ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>Allowed jobs</source>
        <translation>Jobs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>List of allowed job names (regex supported)</source>
        <translation>Liste des noms de jobs autorisés (expressions régulières prises en charge)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="279"/>
        <source>Client ACL:</source>
        <translation>Client ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280"/>
        <source>Allowed clients</source>
        <translation>Clients autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280"/>
        <source>List of allowed client names</source>
        <translation>Liste des noms de clients autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="284"/>
        <source>Storage ACL:</source>
        <translation>Storage ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>Allowed storages</source>
        <translation>Storages autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>List of allowed storage names</source>
        <translation>Liste des noms de Storages autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="288"/>
        <source>Pool ACL:</source>
        <translation>Pool ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289"/>
        <source>Allowed pools</source>
        <translation>Pools autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289"/>
        <source>List of allowed pool names</source>
        <translation>Liste des noms de pools autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="293"/>
        <source>Schedule ACL:</source>
        <translation>Schedule ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>Allowed schedules</source>
        <translation>Schedules autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>List of allowed schedule names</source>
        <translation>Liste des noms de Schedules autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="297"/>
        <source>FileSet ACL:</source>
        <translation>FileSet ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298"/>
        <source>Allowed filesets</source>
        <translation>FileSets autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298"/>
        <source>List of allowed fileset names</source>
        <translation>Liste des noms de FileSets autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="302"/>
        <source>Catalog ACL:</source>
        <translation>Catalog ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>Allowed catalogs</source>
        <translation>Catalogs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>List of allowed catalog names</source>
        <translation>Liste des noms de Catalogs autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="306"/>
        <source>Command ACL:</source>
        <translation>Command ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307"/>
        <source>Allowed commands</source>
        <translation>Commandes autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307"/>
        <source>List of allowed commands</source>
        <translation>Liste des commandes autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="311"/>
        <source>Where ACL:</source>
        <translation>Where ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>Allowed restore locations</source>
        <translation>Emplacements de restauration autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>List of allowed restore locations</source>
        <translation>Liste des emplacements de restauration autorisés</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="315"/>
        <source>Plugin Options ACL:</source>
        <translation>Plugin Options ACL :</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="316"/>
        <source>Allowed plugin options</source>
        <translation>Options de plugin autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="316"/>
        <source>List of allowed plugin options</source>
        <translation>Liste des options de plugin autorisées</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="416"/>
        <source>Not used with gRPC: the connection is always encrypted with TLS, the server certificate is confirmed on first use.</source>
        <translation>Non utilisé avec gRPC : la connexion est toujours chiffrée avec TLS, le certificat du serveur est confirmé à la première utilisation.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="431"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(non épinglé - confirmé à la prochaine connexion)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="437"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256 : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="449"/>
        <source>Reset to Defaults</source>
        <translation>Rétablir les valeurs par défaut</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="450"/>
        <source>Are you sure you want to reset all advanced settings to their default values?</source>
        <translation>Voulez-vous vraiment rétablir tous les paramètres avancés à leurs valeurs par défaut ?</translation>
    </message>
</context>
<context>
    <name>BRecycleForecast</name>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="69"/>
        <source>now</source>
        <translation>maintenant</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="70"/>
        <source>never</source>
        <translation>jamais</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="73"/>
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="81"/>
        <source>The Director did not report the status of this volume.</source>
        <translation>Le Director n&apos;a pas indiqué le statut de ce volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="86"/>
        <source>Archived: Bareos never prunes or recycles an archived volume.</source>
        <translation>Archived : Bareos n&apos;élague ni ne recycle jamais un volume archivé.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="89"/>
        <source>Disabled: Bareos neither writes to nor recycles a disabled volume.</source>
        <translation>Disabled : Bareos n&apos;écrit pas sur un volume désactivé et ne le recycle pas.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="93"/>
        <source>Error: Bareos never prunes a volume in Error, so it keeps its space until its status is changed. The job records on it still expire.</source>
        <translation>Error : Bareos n&apos;élague jamais un volume en Error ; il conserve donc son espace jusqu&apos;à ce que son statut soit modifié. Les enregistrements de jobs qu&apos;il contient expirent malgré tout.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="97"/>
        <source>Read-Only: Bareos does not prune or recycle a read-only volume.</source>
        <translation>Read-Only : Bareos n&apos;élague ni ne recycle un volume en lecture seule.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="100"/>
        <source>Cleaning tape: never used for data.</source>
        <translation>Bande de nettoyage : jamais utilisée pour des données.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="105"/>
        <source>Recycle = no: the volume is never reused; its data stays until it is relabeled or deleted.</source>
        <translation>Recycle = no : le volume n&apos;est jamais réutilisé ; ses données restent jusqu&apos;à ce qu&apos;il soit réétiqueté ou supprimé.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="109"/>
        <source>Purged: no job needs it any more, reusable now.</source>
        <translation>Purged : plus aucun job n&apos;en a besoin, réutilisable dès maintenant.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="113"/>
        <source>It still holds %1 on disk until it is recycled or truncated.</source>
        <translation>Il occupe encore %1 sur le disque jusqu&apos;à ce qu&apos;il soit recyclé ou tronqué.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="119"/>
        <source>Marked for recycling: reused at the next need of its pool.</source>
        <translation>Marqué pour recyclage : réutilisé au prochain besoin de son pool.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="122"/>
        <source>Busy: in use by a job right now.</source>
        <translation>Busy : utilisé par un job en ce moment.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="125"/>
        <source>Unlabeled: not yet written.</source>
        <translation>Non étiqueté : pas encore écrit.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="131"/>
        <source>Append: the short volume list does not report the Volume Use Duration, so it is not known whether this volume stays open until it is full.</source>
        <translation>Append : la liste courte des volumes n&apos;indique pas le Volume Use Duration ; on ne sait donc pas si ce volume reste ouvert jusqu&apos;à ce qu&apos;il soit plein.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="135"/>
        <source>Append without Volume Use Duration: it stays open until it is full</source>
        <translation>Append sans Volume Use Duration : il reste ouvert jusqu&apos;à ce qu&apos;il soit plein</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="137"/>
        <source>(%1)</source>
        <translation>(%1)</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="140"/>
        <source>or holds %1 jobs</source>
        <translation>ou contient %1 jobs</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="142"/>
        <source>; nothing on it is freed before that.</source>
        <translation>; rien n&apos;y est libéré avant cela.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="148"/>
        <source>Append with a use duration of %1, but the first write or the retention is not reported.</source>
        <translation>Append avec une durée d&apos;utilisation de %1, mais la première écriture ou la rétention n&apos;est pas indiquée.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="157"/>
        <source>Append until %1 (use duration %2), then %3 of volume retention.</source>
        <translation>Append jusqu&apos;au %1 (durée d&apos;utilisation %2), puis %3 de rétention du volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="165"/>
        <source>%1: the last write or the volume retention is not reported.</source>
        <translation>%1 : la dernière écriture ou la rétention du volume n&apos;est pas indiquée.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="170"/>
        <source>%1: last written %2, volume retention %3.</source>
        <translation>%1 : dernière écriture %2, rétention du volume %3.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="181"/>
        <source>%1: the job retention of %2 ends before the volume retention of %3; the jobs are pruned first and the volume becomes Purged.</source>
        <translation>%1 : la rétention des jobs de %2 se termine avant la rétention du volume de %3 ; les jobs sont élagués en premier et le volume passe à Purged.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="189"/>
        <source>The date has passed: it is pruned when its pool next needs a volume.</source>
        <translation>La date est passée : il est élagué la prochaine fois que son pool a besoin d&apos;un volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="194"/>
        <source>Status &quot;%1&quot; is not known to this version.</source>
        <translation>Le statut « %1 » n&apos;est pas connu de cette version.</translation>
    </message>
</context>
<context>
    <name>BResourceDialog</name>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="53"/>
        <source>Select %1:</source>
        <translation>Sélectionner %1 :</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="71"/>
        <source>Edit %1 Resource</source>
        <translation>Modifier la ressource %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="81"/>
        <source>Show advanced directives</source>
        <translation>Afficher les directives avancées</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="90"/>
        <location filename="../src/director/bresourcedialog.cpp" line="253"/>
        <source>Save .conf</source>
        <translation>Enregistrer le .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="95"/>
        <location filename="../src/director/bresourcedialog.cpp" line="330"/>
        <source>Save ZIP</source>
        <translation>Enregistrer le ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="228"/>
        <source>Save %1 Configuration</source>
        <translation>Enregistrer la configuration %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="229"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="235"/>
        <location filename="../src/director/bresourcedialog.cpp" line="243"/>
        <location filename="../src/director/bresourcedialog.cpp" line="278"/>
        <location filename="../src/director/bresourcedialog.cpp" line="286"/>
        <source>Save Failed</source>
        <translation>Échec de l&apos;enregistrement</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="236"/>
        <location filename="../src/director/bresourcedialog.cpp" line="279"/>
        <source>%1 &quot;%2&quot; cannot be written. %3</source>
        <translation>%1 « %2 » ne peut pas être écrit. %3</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="244"/>
        <source>Could not write to %1</source>
        <translation>Impossible d&apos;écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="251"/>
        <location filename="../src/director/bresourcedialog.cpp" line="328"/>
        <source>Saved!</source>
        <translation>Enregistré !</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="268"/>
        <source>Save %1 Configuration as ZIP</source>
        <translation>Enregistrer la configuration %1 en ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="269"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="287"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
</context>
<context>
    <name>BResourceForm</name>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="157"/>
        <source>No schema available for resource type &apos;%1&apos;</source>
        <translation>Aucun schéma disponible pour le type de ressource &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="186"/>
        <location filename="../src/config/bresourceform.cpp" line="198"/>
        <location filename="../src/config/bresourceform.cpp" line="199"/>
        <location filename="../src/config/bresourceform.cpp" line="202"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="383"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="391"/>
        <source>Show/Hide password</source>
        <translation>Afficher/masquer le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="417"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="419"/>
        <source>Browse for file</source>
        <translation>Rechercher un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="419"/>
        <source>Browse for directory</source>
        <translation>Rechercher un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="423"/>
        <source>Select File</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="425"/>
        <source>Select Directory</source>
        <translation>Sélectionner un répertoire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="454"/>
        <source>&lt;%1 name&gt;</source>
        <translation>&lt;nom de %1&gt;</translation>
    </message>
</context>
<context>
    <name>BResourcePersistence</name>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="36"/>
        <source>schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="37"/>
        <source>job</source>
        <translation>job</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="38"/>
        <source>JobDefs resource</source>
        <translation>ressource JobDefs</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="39"/>
        <source>client</source>
        <translation>client</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="40"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="41"/>
        <source>pool</source>
        <translation>pool</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="42"/>
        <source>storage</source>
        <translation>storage</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="45"/>
        <source>resource</source>
        <translation>ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="52"/>
        <source>%1 %2 was not created</source>
        <translation>Création de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="53"/>
        <source>%1 %2 was not deleted</source>
        <translation>Suppression de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="56"/>
        <source>%1 %2 was not changed</source>
        <translation>Modification de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="108"/>
        <source>There is no connection to the configuration methods of the Director. Nothing was sent.</source>
        <translation>Aucune connexion aux méthodes de configuration du Director. Rien n&apos;a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="121"/>
        <source>Nothing was written.</source>
        <translation>Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="142"/>
        <source>Nothing to write</source>
        <translation>Rien à écrire</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="143"/>
        <source>The text is the one the Director has.</source>
        <translation>Le texte est celui que possède le Director.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="154"/>
        <source>The text the change was made in does not match its SHA-256. Reload the %1 and make the change again.</source>
        <translation>Le texte sur lequel la modification a été faite ne correspond pas à son SHA-256. Rechargez la ressource (%1) et refaites la modification.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="289"/>
        <source>%1 %2 created</source>
        <translation>%1 %2 : création effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="290"/>
        <source>%1 %2 deleted</source>
        <translation>%1 %2 : suppression effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="291"/>
        <source>%1 %2 changed</source>
        <translation>%1 %2 : modification effectuée</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="293"/>
        <source>The Director wrote the configuration and reloaded it.</source>
        <translation>Le Director a écrit la configuration et l&apos;a rechargée.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="294"/>
        <source>The Director wrote the configuration but did not report a reload; the change takes effect with its next reload.</source>
        <translation>Le Director a écrit la configuration mais n&apos;a pas signalé de rechargement ; la modification prendra effet au prochain rechargement.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="297"/>
        <source>The reload does not adjust everything; the Director names these follow-up commands:</source>
        <translation>Le rechargement n&apos;ajuste pas tout ; le Director indique ces commandes complémentaires :</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="302"/>
        <source>%1 %2 was changed on the Director</source>
        <translation>%1 %2 a été modifié sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="303"/>
        <source>The Director has another state of the %1 than the one this change started from. Nothing was written and nothing is overwritten. Compare the two texts, reload the %1 and make the change again.</source>
        <translation>Le Director possède un autre état de la ressource (%1) que celui dont cette modification est partie. Rien n&apos;a été écrit et rien n&apos;est écrasé. Comparez les deux textes, rechargez la ressource (%1) et refaites la modification.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="310"/>
        <source>The Director checked the new configuration and rejected it. Nothing was written.</source>
        <translation>Le Director a vérifié la nouvelle configuration et l&apos;a rejetée. Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="314"/>
        <source>The %1 is still used. The Director does not delete a %1 that other resources name; change them first. Nothing was deleted.</source>
        <translation>La ressource (%1) est encore utilisée. Le Director ne supprime pas une ressource (%1) que d&apos;autres ressources désignent ; modifiez-les d&apos;abord. Rien n&apos;a été supprimé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="320"/>
        <source>The console of this session may not change this %1 (the ACL of its resource type). Nothing was written.</source>
        <translation>La console de cette session n&apos;a pas le droit de modifier cette ressource (%1) (ACL de son type de ressource). Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="322"/>
        <source>The console of this session may not change this %1 (its %2 ACL). Nothing was written.</source>
        <translation>La console de cette session n&apos;a pas le droit de modifier cette ressource (%1) (son ACL %2). Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="327"/>
        <source>The Director has no %1 of this name (any more). Nothing was written.</source>
        <translation>Le Director n&apos;a pas (ou plus) de ressource (%1) portant ce nom. Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="331"/>
        <source>The Director already has a resource of this name. Nothing was written.</source>
        <translation>Le Director possède déjà une ressource portant ce nom. Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="335"/>
        <source>The Director wrote the file, but reloading the configuration failed, so it put the old file back. The %1 is as it was.</source>
        <translation>Le Director a écrit le fichier, mais le rechargement de la configuration a échoué ; il a donc remis l&apos;ancien fichier en place. La ressource (%1) est inchangée.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="341"/>
        <source>This connection has no configuration methods. Nothing was sent.</source>
        <translation>Cette connexion n&apos;a pas de méthodes de configuration. Rien n&apos;a été envoyé.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="347"/>
        <source>The Director refused the call (%1). The console needs the command ACL &quot;configure&quot;.</source>
        <translation>Le Director a refusé l&apos;appel (%1). La console a besoin de l&apos;ACL de commande &quot;configure&quot;.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="349"/>
        <source>The call failed (%1). Nothing was written.</source>
        <translation>L&apos;appel a échoué (%1). Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="353"/>
        <source>The Director sent no status. Nothing is known to have been written.</source>
        <translation>Le Director n&apos;a envoyé aucun statut. Rien ne permet de savoir si quelque chose a été écrit.</translation>
    </message>
</context>
<context>
    <name>BResourceReportDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="232"/>
        <source>file %1</source>
        <translation>fichier %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="234"/>
        <source>line %1</source>
        <translation>ligne %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="236"/>
        <source>column %1</source>
        <translation>colonne %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="240"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="272"/>
        <source>This was found when the Director checked the change; nothing was sent to be written.</source>
        <translation>Ceci a été détecté lorsque le Director a vérifié la modification ; rien n&apos;a été envoyé pour écriture.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="278"/>
        <source>The Director says: %1</source>
        <translation>Le Director indique : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="282"/>
        <source>Where the configuration is invalid</source>
        <translation>Où la configuration n&apos;est pas valide</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="291"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="294"/>
        <source>Still referenced by</source>
        <translation>Encore référencé par</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="302"/>
        <source>Follow-up commands the Director names</source>
        <translation>Commandes complémentaires indiquées par le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="309"/>
        <source>They are not run by this application.</source>
        <translation>Elles ne sont pas exécutées par cette application.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="323"/>
        <source>On the Director now</source>
        <translation>Actuellement sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="325"/>
        <source>What was to be written (not written)</source>
        <translation>Ce qui devait être écrit (non écrit)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="331"/>
        <source>&amp;Reload the %1</source>
        <translation>&amp;Recharger la ressource (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="333"/>
        <source>Reads the %1 again from the Director; your changes are offered to be kept or discarded</source>
        <translation>Relit la ressource (%1) depuis le Director ; vous pourrez conserver ou abandonner vos modifications</translation>
    </message>
</context>
<context>
    <name>BResourceReviewDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="83"/>
        <source>Change %1 %2</source>
        <translation>Modifier %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="84"/>
        <source>Change %1 %2 on the Director?</source>
        <translation>Modifier %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="85"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Le Director a vérifié la nouvelle configuration et l&apos;accepte. Rien n&apos;est écrit tant que vous n&apos;avez pas choisi &quot;Écrire sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="89"/>
        <source>Create %1 %2</source>
        <translation>Créer %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="90"/>
        <source>Create %1 %2 on the Director?</source>
        <translation>Créer %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="91"/>
        <source>The Director checked the new %1 and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Le Director a vérifié la nouvelle ressource (%1) et l&apos;accepte. Rien n&apos;est écrit tant que vous n&apos;avez pas choisi &quot;Écrire sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="95"/>
        <source>Delete %1 %2</source>
        <translation>Supprimer %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="96"/>
        <source>Delete %1 %2 on the Director?</source>
        <translation>Supprimer %1 %2 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="97"/>
        <source>The Director checked that nothing refers to the %1. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Le Director a vérifié que rien ne fait référence à la ressource (%1). Rien n&apos;est supprimé tant que vous n&apos;avez pas choisi &quot;Supprimer sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="110"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="113"/>
        <source>Changed directives</source>
        <translation>Directives modifiées</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="124"/>
        <source>Text before and after (- removed, + added)</source>
        <translation>Texte avant et après (- supprimé, + ajouté)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="126"/>
        <source>Text of the new resource</source>
        <translation>Texte de la nouvelle ressource</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="127"/>
        <source>Text that is deleted</source>
        <translation>Texte supprimé</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="142"/>
        <source>Affected</source>
        <translation>Concerné</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bresourcereviewdialog.cpp" line="146"/>
        <source>The change applies to every one of these %n resource(s).</source>
        <translation>
            <numerusform>La modification s&apos;applique à cette ressource (%n).</numerusform>
            <numerusform>La modification s&apos;applique à chacune de ces %n ressources.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="155"/>
        <source>(nothing else, as far as this application knows)</source>
        <translation>(rien d&apos;autre, à la connaissance de cette application)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="159"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Le Director indique comme y faisant référence : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="169"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Fichier sur le Director : %1 (il contient aussi d&apos;autres ressources ; seule cette ressource est remplacée).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="171"/>
        <source>File on the Director: %1.</source>
        <translation>Fichier sur le Director : %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="173"/>
        <source>The Director reloads its configuration after writing. Jobs that are running or already queued keep the configuration they started with.</source>
        <translation>Le Director recharge sa configuration après l&apos;écriture. Les Jobs en cours ou déjà en file d&apos;attente conservent la configuration avec laquelle ils ont démarré.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="178"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Supprimer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="179"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Écrire sur le Director</translation>
    </message>
</context>
<context>
    <name>BResourceText</name>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="213"/>
        <source>The text does not start with a resource (line %1).</source>
        <translation>Le texte ne commence pas par une ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="215"/>
        <source>Directives on the line of &quot;%1 {&quot; cannot be edited safely (line %2).</source>
        <translation>Les directives situées sur la ligne de &quot;%1 {&quot; ne peuvent pas être modifiées de manière sûre (ligne %2).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="225"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Texte après l&apos;accolade fermante de la ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="236"/>
        <source>The closing brace of the resource shares its line with a nested block (line %1).</source>
        <translation>L&apos;accolade fermante de la ressource partage sa ligne avec un bloc imbriqué (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="250"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>L&apos;accolade fermante partage sa ligne avec une directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="323"/>
        <source>The text contains no resource.</source>
        <translation>Le texte ne contient aucune ressource.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="325"/>
        <source>The resource has no closing brace.</source>
        <translation>La ressource n&apos;a pas d&apos;accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="375"/>
        <source>A change without a directive name.</source>
        <translation>Une modification sans nom de directive.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="377"/>
        <source>Directive %1 is changed twice.</source>
        <translation>La directive %1 est modifiée deux fois.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="381"/>
        <source>The value of %1 contains a line break.</source>
        <translation>La valeur de %1 contient un saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="383"/>
        <source>An empty value for %1; remove the directive instead.</source>
        <translation>Valeur vide pour %1 ; supprimez plutôt la directive.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="386"/>
        <source>Directive %1 is on a line that cannot be edited safely (several directives on one line, a list or a nested block). Change it in the text.</source>
        <translation>La directive %1 se trouve sur une ligne qui ne peut pas être modifiée de manière sûre (plusieurs directives sur une ligne, une liste ou un bloc imbriqué). Modifiez-la dans le texte.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="390"/>
        <source>Directive %1 appears more than once. Change it in the text.</source>
        <translation>La directive %1 apparaît plusieurs fois. Modifiez-la dans le texte.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="443"/>
        <source>The new text could not be read back: %1</source>
        <translation>Le nouveau texte n&apos;a pas pu être relu : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="448"/>
        <source>The new text still has directive %1.</source>
        <translation>Le nouveau texte contient encore la directive %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="450"/>
        <source>The new text does not have the intended value of %1.</source>
        <translation>Le nouveau texte n&apos;a pas la valeur prévue pour %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="454"/>
        <source>The new text would change more than the edited directives.</source>
        <translation>Le nouveau texte modifierait plus que les directives éditées.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="456"/>
        <source>The new text would change the type of the resource.</source>
        <translation>Le nouveau texte modifierait le type de la ressource.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="458"/>
        <source>The new text would change the name of the resource.</source>
        <translation>Le nouveau texte modifierait le nom de la ressource.</translation>
    </message>
</context>
<context>
    <name>BResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="50"/>
        <source>Preview</source>
        <translation>Aperçu</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="52"/>
        <source>Toggle config file preview</source>
        <translation>Afficher/masquer l&apos;aperçu du fichier de configuration</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="64"/>
        <source>Edit...</source>
        <translation>Modifier...</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="70"/>
        <source>Only Job and JobDefs resources can be changed here; they are written to the Director. Change this resource in its configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="77"/>
        <source>Export .conf</source>
        <translation>Exporter .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="83"/>
        <source>Export ZIP</source>
        <translation>Exporter ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="96"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="96"/>
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="147"/>
        <source>%1: %2 resource(s)</source>
        <translation>%1 : %2 ressource(s)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="206"/>
        <location filename="../src/director/bresourcewidget.cpp" line="230"/>
        <source>%1 %2 was not changed</source>
        <translation>Modification de %1 %2 non effectuée</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="207"/>
        <source>There is no Director connection to write the change to. Nothing was changed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="248"/>
        <source>Not written, because the form cannot write lists or nested blocks: %1. Change them in the configuration file.</source>
        <translation>Non écrit, car le formulaire ne peut pas écrire de listes ni de blocs imbriqués : %1. Modifiez-les dans le fichier de configuration.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="253"/>
        <source>Nothing was changed.</source>
        <translation>Rien n&apos;a été modifié.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="254"/>
        <source>Nothing that can be written was changed. %1</source>
        <translation>Rien de ce qui peut être écrit n&apos;a été modifié. %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="271"/>
        <source>Another change is still being written. Try again when it is done.</source>
        <translation>Une autre modification est encore en cours d&apos;écriture. Réessayez lorsqu&apos;elle sera terminée.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="281"/>
        <source>The text the Director sent does not match its SHA-256.</source>
        <translation>Le texte envoyé par le Director ne correspond pas à son SHA-256.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="283"/>
        <source>%1 Nothing was written.</source>
        <translation>%1 Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="290"/>
        <source>The change cannot be written into the text of the resource safely: %1 Nothing was written.</source>
        <translation>La modification ne peut pas être écrite de manière sûre dans le texte de la ressource : %1 Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="509"/>
        <source>Schedule &quot;%1&quot; is not written here.</source>
        <translation>Le Schedule &quot;%1&quot; n&apos;est pas écrit ici.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="609"/>
        <source>Export %1 Configuration</source>
        <translation>Exporter la configuration %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="610"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="615"/>
        <location filename="../src/director/bresourcewidget.cpp" line="670"/>
        <source>Export Failed</source>
        <translation>Échec de l&apos;exportation</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="616"/>
        <source>Could not write to %1</source>
        <translation>Impossible d&apos;écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="661"/>
        <source>Export %1 Configuration as ZIP</source>
        <translation>Exporter la configuration %1 en ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="662"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="671"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
</context>
<context>
    <name>BRestoreBrowsePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="521"/>
        <source>Select Files</source>
        <translation>Sélectionner les fichiers</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="522"/>
        <source>Browse and select files/directories to restore.</source>
        <translation>Parcourez et sélectionnez les fichiers/répertoires à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="534"/>
        <source>Loading files...</source>
        <translation>Chargement des fichiers...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="560"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="761"/>
        <source>No files selected</source>
        <translation>Aucun fichier sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="580"/>
        <source>Step %1 of %2 — Browse and select files/directories to restore.</source>
        <translation>Étape %1 sur %2 — Parcourez et sélectionnez les fichiers/répertoires à restaurer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="583"/>
        <source>Browse files — Client: %1, FileSet: %2</source>
        <translation>Parcourir les fichiers — Client : %1, FileSet : %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="754"/>
        <source>The Director reported an error: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="770"/>
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n répertoire</numerusform>
            <numerusform>%n répertoires</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="772"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="773"/>
        <source>Selected: %1</source>
        <translation>Sélection : %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="775"/>
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
        <location filename="../src/jobs/brestorewizard.cpp" line="1314"/>
        <source>Execute Restore</source>
        <translation>Exécuter la restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1315"/>
        <source>Sending commands to the Director...</source>
        <translation>Envoi des commandes au Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1319"/>
        <source>Preparing...</source>
        <translation>Préparation...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1349"/>
        <source>Step %1 of %2 — Sending commands to the Director...</source>
        <translation>Étape %1 sur %2 — Envoi des commandes au Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1360"/>
        <source>Step %v of %m</source>
        <translation>Étape %v sur %m</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1387"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1406"/>
        <source>No director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1394"/>
        <source>Creating restore table...</source>
        <translation>Création de la table de restauration...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1395"/>
        <source>&gt;&gt;&gt; .bvfs_restore %1</source>
        <translation>&gt;&gt;&gt; .bvfs_restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1413"/>
        <source>Executing restore command...</source>
        <translation>Exécution de la commande de restauration...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1414"/>
        <source>&gt;&gt;&gt; restore %1</source>
        <translation>&gt;&gt;&gt; restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1428"/>
        <source>Cleaning up restore table...</source>
        <translation>Nettoyage de la table de restauration...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1431"/>
        <source>&gt;&gt;&gt; .bvfs_cleanup %1</source>
        <translation>&gt;&gt;&gt; .bvfs_cleanup %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1446"/>
        <source>The Director answered.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1461"/>
        <source>Cleanup failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1478"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1594"/>
        <source>.bvfs_restore failed: %1</source>
        <translation>Échec de .bvfs_restore : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1482"/>
        <source>Restore table created successfully.</source>
        <translation>Table de restauration créée avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1498"/>
        <source>Restore command failed: %1</source>
        <translation>Échec de la commande de restauration : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1502"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1556"/>
        <source>Hint: The Bareos Director restricts the &quot;where&quot; path via WhereACL. Ask your administrator to add the path &quot;%1&quot; to the WhereACL of your console configuration, or use a permitted restore path.</source>
        <translation>Conseil : le Director Bareos restreint le chemin &quot;where&quot; via WhereACL. Demandez à votre administrateur d&apos;ajouter le chemin &quot;%1&quot; à la WhereACL de votre configuration de console, ou utilisez un chemin de restauration autorisé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1525"/>
        <source>Restore job submitted successfully (Job ID: %1).</source>
        <translation>Job de restauration soumis avec succès (ID du Job : %1).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1527"/>
        <source>Restore command accepted.</source>
        <translation>Commande de restauration acceptée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1551"/>
        <source>Restore command failed.</source>
        <translation>Échec de la commande de restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1570"/>
        <source>Restore job submitted successfully.</source>
        <translation>Job de restauration soumis avec succès.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1580"/>
        <source>WARNING: Unexpected restore response.</source>
        <translation>AVERTISSEMENT : réponse de restauration inattendue.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1598"/>
        <source>Restore table created (text response).</source>
        <translation>Table de restauration créée (réponse texte).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1613"/>
        <source>Cleanup complete.</source>
        <translation>Nettoyage terminé.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1623"/>
        <source>Complete</source>
        <translation>Terminé</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1626"/>
        <source>Restore job %1 submitted successfully!</source>
        <translation>Job de restauration %1 soumis avec succès !</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1627"/>
        <source>The restore job (ID: %1) is now running on the Director.
You can monitor its progress in the Jobs view.
Refresh the job list to see the restore job status.</source>
        <translation>Le Job de restauration (ID : %1) est maintenant en cours sur le Director.
Vous pouvez suivre sa progression dans la vue Jobs.
Actualisez la liste des Jobs pour voir le statut du Job de restauration.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1632"/>
        <source>Restore completed.</source>
        <translation>Restauration terminée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1633"/>
        <source>The restore commands have been executed.
Check the log above for details.</source>
        <translation>Les commandes de restauration ont été exécutées.
Consultez le journal ci-dessus pour plus de détails.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1645"/>
        <source>ERROR: %1</source>
        <translation>ERREUR : %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1648"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1649"/>
        <source>Restore failed</source>
        <translation>Échec de la restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1677"/>
        <source>command</source>
        <translation>commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1683"/>
        <source>No answer from the Director to %1 within %2 seconds.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1686"/>
        <source>Waiting for the Director...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1687"/>
        <source>The restore may still start. Check the job list before you start it again. The answer is still awaited while this window is open.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BRestoreOptionsPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="788"/>
        <source>Restore Options</source>
        <translation>Options de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="789"/>
        <source>Configure where and how to restore the selected files.</source>
        <translation>Configurez où et comment restaurer les fichiers sélectionnés.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="795"/>
        <source>Source:</source>
        <translation>Source :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="798"/>
        <source>Target Client:</source>
        <translation>Client cible :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="803"/>
        <source>Restore Job:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="806"/>
        <source>Restore Where:</source>
        <translation>Restaurer dans :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="809"/>
        <source>Always</source>
        <translation>Toujours</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="810"/>
        <source>Never</source>
        <translation>Jamais</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="811"/>
        <source>If Newer</source>
        <translation>Si plus récent</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="812"/>
        <source>If Older</source>
        <translation>Si plus ancien</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="813"/>
        <source>Replace Policy:</source>
        <translation>Politique de remplacement :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="838"/>
        <source>Step %1 of %2 — Configure where and how to restore the selected files.</source>
        <translation>Étape %1 sur %2 — Configurez où et comment restaurer les fichiers sélectionnés.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="843"/>
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n répertoire</numerusform>
            <numerusform>%n répertoires</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="845"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="847"/>
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="855"/>
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="855"/>
        <source>at least %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="856"/>
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="858"/>
        <source>Job %1: %2 — %3 selected (estimated size: %4)</source>
        <translation>Job %1 : %2 — %3 sélectionné(s) (taille estimée : %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="933"/>
        <source>Chosen by the Director</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="947"/>
        <source>Always overwrite existing files on the target, regardless of timestamps.</source>
        <translation>Toujours écraser les fichiers existants sur la cible, quels que soient les horodatages.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="951"/>
        <source>Never overwrite. Skip files that already exist on the target.</source>
        <translation>Ne jamais écraser. Ignorer les fichiers qui existent déjà sur la cible.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="955"/>
        <source>Only overwrite if the backed-up file is newer than the existing file.</source>
        <translation>N&apos;écraser que si le fichier sauvegardé est plus récent que le fichier existant.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="959"/>
        <source>Only overwrite if the backed-up file is older than the existing file.</source>
        <translation>N&apos;écraser que si le fichier sauvegardé est plus ancien que le fichier existant.</translation>
    </message>
</context>
<context>
    <name>BRestorePreviewPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="998"/>
        <source>Command Preview</source>
        <translation>Aperçu des commandes</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="999"/>
        <source>Review the commands that will be sent to the Director.</source>
        <translation>Vérifiez les commandes qui seront envoyées au Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1009"/>
        <source>Permission Checks</source>
        <translation>Vérification des autorisations</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1017"/>
        <source>Restore command:</source>
        <translation>Commande de restauration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1018"/>
        <source>Target client:</source>
        <translation>Client cible :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1019"/>
        <source>Where path:</source>
        <translation>Chemin de destination :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1041"/>
        <source>Click &quot;Commit&quot; to execute these commands on the Director.</source>
        <translation>Cliquez sur &quot;Valider&quot; pour exécuter ces commandes sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1054"/>
        <source>Step %1 of %2 — Review the commands that will be sent to the Director.</source>
        <translation>Étape %1 sur %2 — Vérifiez les commandes qui seront envoyées au Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1093"/>
        <source>The restore command cannot be built: the value of %1= is not valid.</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="1100"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1103"/>
        <source>%n directory(ies) (including all contents)</source>
        <translation>
            <numerusform>%n répertoire (avec tout son contenu)</numerusform>
            <numerusform>%n répertoires (avec tout leur contenu)</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="1101"/>
        <source>%n individual file(s)</source>
        <translation>
            <numerusform>%n fichier individuel</numerusform>
            <numerusform>%n fichiers individuels</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="1105"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n fichier</numerusform>
            <numerusform>%n fichiers</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1107"/>
        <source>No files or directories selected</source>
        <translation>Aucun fichier ni répertoire sélectionné</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1115"/>
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1115"/>
        <source>at least %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1116"/>
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1118"/>
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
        <location filename="../src/jobs/brestorewizard.cpp" line="1130"/>
        <source>• Restore job: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1152"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1153"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1154"/>
        <source>Checking...</source>
        <translation>Vérification...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1155"/>
        <source>Querying Director for permissions...</source>
        <translation>Interrogation du Director sur les autorisations...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1167"/>
        <source>No Director connection</source>
        <translation>Aucune connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1226"/>
        <source>restore command lacks client= parameter</source>
        <translation>la commande restore n&apos;a pas de paramètre client=</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1231"/>
        <source>Available (subject to WhereACL)</source>
        <translation>Disponible (sous réserve de WhereACL)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1235"/>
        <source>Not available</source>
        <translation>Non disponible</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1239"/>
        <source>N/A (restore not authorized)</source>
        <translation>N/A (restauration non autorisée)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1244"/>
        <source>Permission check completed.</source>
        <translation>Vérification des autorisations terminée.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1261"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1263"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1265"/>
        <source>Unknown (timeout)</source>
        <translation>Inconnu (délai dépassé)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1267"/>
        <source>Permission check timed out — you can still proceed.</source>
        <translation>La vérification des autorisations a dépassé le délai — vous pouvez tout de même continuer.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1295"/>
        <source>Authorized</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1299"/>
        <source>Not authorized</source>
        <translation>Non autorisé</translation>
    </message>
</context>
<context>
    <name>BRestoreSelectJobPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="170"/>
        <source>Restore Conditions</source>
        <translation>Conditions de restauration</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="171"/>
        <source>Configure the restore scope before browsing files.</source>
        <translation>Configurez l&apos;étendue de la restauration avant de parcourir les fichiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="178"/>
        <source>Source Job:</source>
        <translation>Job source :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="182"/>
        <source>Client:</source>
        <translation>Client :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="186"/>
        <source>FileSet:</source>
        <translation>FileSet :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="190"/>
        <source>All related backups (full restore chain)</source>
        <translation>Toutes les sauvegardes associées (chaîne de restauration complète)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="199"/>
        <source>Restore Scope:</source>
        <translation>Étendue de la restauration :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="205"/>
        <source>Before Date:</source>
        <translation>Avant la date :</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="242"/>
        <source>Step %1 of %2 — Configure the restore scope before browsing files.</source>
        <translation>Étape %1 sur %2 — Configurez l&apos;étendue de la restauration avant de parcourir les fichiers.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="245"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="453"/>
        <source>Job %1: %2 (Client: %3, FileSet: %4)</source>
        <translation>Job %1 : %2 (Client : %3, FileSet : %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="251"/>
        <source>Only this specific job (ID: %1)</source>
        <translation>Uniquement ce Job précis (ID : %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="300"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="301"/>
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="304"/>
        <source>Warning: Failed to load %1 from Director within timeout. The Director may be busy or the connection may be slow.</source>
        <translation>Avertissement : impossible de charger %1 depuis le Director dans le délai imparti. Le Director est peut-être occupé ou la connexion lente.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="306"/>
        <source> and </source>
        <translation> et </translation>
    </message>
</context>
<context>
    <name>BRestoreWizard</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="131"/>
        <source>Restore Wizard — Job %1: %2</source>
        <translation>Assistant de restauration — Job %1 : %2</translation>
    </message>
</context>
<context>
    <name>BRunOverridesEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="42"/>
        <source>not set</source>
        <translation>non défini</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="43"/>
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="44"/>
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="86"/>
        <source>(job&apos;s setting)</source>
        <translation>(réglage du Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="90"/>
        <source>The Director has not reported any %1 yet; refresh the view after connecting. Not set: the setting of the job applies.</source>
        <translation>Le Director n&apos;a encore signalé aucun(e) %1 ; actualisez la vue après la connexion. Non défini : le réglage du Job s&apos;applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="92"/>
        <source>The %1 the Director reported. Not set: the setting of the job applies.</source>
        <translation>Les %1 signalés par le Director. Non défini : le réglage du Job s&apos;applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="112"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="113"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="114"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="115"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="116"/>
        <source>pools</source>
        <translation>pools</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="117"/>
        <source>storages</source>
        <translation>storages</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="120"/>
        <source>set to</source>
        <translation>définir à</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="126"/>
        <source>Level of the job</source>
        <translation>Niveau du Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="135"/>
        <source>The level the Run line names. &quot;Level of the job&quot;: the line names none and the level of the job applies.</source>
        <translation>Le niveau indiqué par la ligne Run. &quot;Niveau du Job&quot; : la ligne n&apos;en indique aucun et le niveau du Job s&apos;applique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="141"/>
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="142"/>
        <source>Override the priority</source>
        <translation>Remplacer la priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="143"/>
        <source>a time period, e.g. 8 hours</source>
        <translation>une durée, p. ex. 8 hours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="144"/>
        <source>Empty: the setting of the job applies</source>
        <translation>Vide : le réglage du Job s&apos;applique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="170"/>
        <source>&amp;Level:</source>
        <translation>&amp;Niveau :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="171"/>
        <source>&amp;Storage:</source>
        <translation>&amp;Storage :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="173"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="174"/>
        <source>&amp;Messages:</source>
        <translation>&amp;Messages :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="176"/>
        <source>&amp;Full pool:</source>
        <translation>Pool &amp;Full :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="177"/>
        <source>P&amp;riority:</source>
        <translation>P&amp;riorité :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="179"/>
        <source>&amp;Incremental pool:</source>
        <translation>Pool &amp;Incremental :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="180"/>
        <source>Sp&amp;ool data:</source>
        <translation>Sp&amp;ool des données :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="182"/>
        <source>&amp;Differential pool:</source>
        <translation>Pool &amp;Differential :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="183"/>
        <source>&amp;Accurate:</source>
        <translation>&amp;Accurate :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="185"/>
        <source>&amp;Next pool:</source>
        <translation>Pool suiva&amp;nt :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="186"/>
        <source>Max run sched &amp;time:</source>
        <translation>Durée max. depuis la &amp;planification :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="274"/>
        <source>%1 (not reported)</source>
        <translation>%1 (non signalé)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="276"/>
        <source>%1 is named by the Run line, but the Director did not report it. It is kept as it is.</source>
        <translation>%1 est indiqué par la ligne Run, mais le Director ne l&apos;a pas signalé. Il est conservé tel quel.</translation>
    </message>
</context>
<context>
    <name>BRunParser</name>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="120"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="152"/>
        <source>A Run line cannot contain a line break.</source>
        <translation>Une ligne Run ne peut pas contenir de saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="141"/>
        <source>Unexpected character &quot;%1&quot;.</source>
        <translation>Caractère inattendu &quot;%1&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="170"/>
        <source>A quoted value is not closed.</source>
        <translation>Une valeur entre guillemets n&apos;est pas fermée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="400"/>
        <source>The Run line is empty.</source>
        <translation>La ligne Run est vide.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="415"/>
        <source>Expected an equals, got: %1</source>
        <translation>Signe égal attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="419"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="464"/>
        <source>Expected a value after &quot;%1=&quot;.</source>
        <translation>Valeur attendue après &quot;%1=&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="430"/>
        <source>Expect a YES or NO, got: %1</source>
        <translation>YES ou NO attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="440"/>
        <source>Job level field: %1 not found in run record</source>
        <translation>Champ de niveau du Job : %1 introuvable dans l&apos;enregistrement Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="447"/>
        <source>expected a positive integer, got: %1</source>
        <translation>entier positif attendu, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="453"/>
        <source>expected a time period, got: %1</source>
        <translation>durée attendue, obtenu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="504"/>
        <source>Unexpected token: %1</source>
        <translation>Jeton inattendu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="509"/>
        <source>Day number out of range (1-31)</source>
        <translation>Numéro de jour hors limites (1-31)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="516"/>
        <source>Unexpected token: &quot;%1&quot;</source>
        <translation>Jeton inattendu : &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="526"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="660"/>
        <source>Week number out of range (0-53)</source>
        <translation>Numéro de semaine hors limites (0-53)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="532"/>
        <source>Job type field: %1 in run record not found</source>
        <translation>Champ de type du Job : %1 introuvable dans l&apos;enregistrement Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="562"/>
        <source>Time must be preceded by keyword AT.</source>
        <translation>L&apos;heure doit être précédée du mot-clé AT.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="578"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="583"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="592"/>
        <source>Bad time specification.</source>
        <translation>Spécification d&apos;heure incorrecte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="617"/>
        <source>Bad day specification in modulo.</source>
        <translation>Spécification de jour incorrecte dans le modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="619"/>
        <source>Bad day specification, offset must always be &lt;= than modulo.</source>
        <translation>Spécification de jour incorrecte, le décalage doit toujours être &lt;= au modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="629"/>
        <source>Week number out of range (0-53) in modulo</source>
        <translation>Numéro de semaine hors limites (0-53) dans le modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="631"/>
        <source>Week interval out of range (1-53) in modulo</source>
        <translation>Intervalle de semaines hors limites (1-53) dans le modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="633"/>
        <source>Bad week number specification in modulo, offset must always be &lt;= than modulo.</source>
        <translation>Spécification de numéro de semaine incorrecte dans le modulo, le décalage doit toujours être &lt;= au modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="640"/>
        <source>Bad modulo time specification. Format for weekdays is &apos;01/02&apos;, for yearweeks is &apos;w01/w02&apos;.</source>
        <translation>Spécification de modulo incorrecte. Le format pour les jours de semaine est &apos;01/02&apos;, pour les semaines de l&apos;année &apos;w01/w02&apos;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="653"/>
        <source>Bad day range specification.</source>
        <translation>Spécification de plage de jours incorrecte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="668"/>
        <source>Invalid month, week or position day range</source>
        <translation>Plage de mois, de semaines ou de jours de position non valide</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="672"/>
        <source>Invalid month, weekday or position range</source>
        <translation>Plage de mois, de jours de semaine ou de positions non valide</translation>
    </message>
</context>
<context>
    <name>BRunRuleDialog</name>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="97"/>
        <source>Only on these da&amp;ys of the month</source>
        <translation>Uniquement ces &amp;jours du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="98"/>
        <source>Only in these &amp;weeks of the year</source>
        <translation>Uniquement ces &amp;semaines de l&apos;année</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="99"/>
        <source>last</source>
        <translation>dernière</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="100"/>
        <source>&amp;Every hour</source>
        <translation>&amp;Toutes les heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="107"/>
        <source>Calendar rule</source>
        <translation>Règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="110"/>
        <source>A calendar rule runs by day of the month or by week of the year. It does not belong to a weekday and is not painted in the grid.</source>
        <translation>Une règle de calendrier s&apos;exécute selon le jour du mois ou la semaine de l&apos;année. Elle n&apos;appartient à aucun jour de semaine et n&apos;est pas peinte dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="117"/>
        <source>What runs</source>
        <translation>Ce qui s&apos;exécute</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="125"/>
        <source>Not checked: every day of the month</source>
        <translation>Non coché : chaque jour du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="130"/>
        <source>Day %1</source>
        <translation>Jour %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="135"/>
        <source>Not checked: every week of the year. The weeks are numbered w00 to w53 as in a Run line.</source>
        <translation>Non coché : chaque semaine de l&apos;année. Les semaines sont numérotées de w00 à w53 comme dans une ligne Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="141"/>
        <source>Week %1</source>
        <translation>Semaine %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="147"/>
        <source>And only</source>
        <translation>Et uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="149"/>
        <source>on these weekdays:</source>
        <translation>ces jours de semaine :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="165"/>
        <source>in these weeks of the month:</source>
        <translation>ces semaines du mois :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="180"/>
        <source>The last seven days of a month</source>
        <translation>Les sept derniers jours d&apos;un mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="186"/>
        <source>in these months:</source>
        <translation>ces mois :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="203"/>
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="208"/>
        <source>%1 hours</source>
        <translation>%1 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="211"/>
        <source>The run starts in every hour of the day (&quot;hourly&quot;)</source>
        <translation>L&apos;exécution démarre à chaque heure de la journée (&quot;hourly&quot;)</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="213"/>
        <source>&amp;Minute of the hour:</source>
        <translation>&amp;Minute de l&apos;heure :</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="216"/>
        <source>Minute of the hour</source>
        <translation>Minute de l&apos;heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="229"/>
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="294"/>
        <source>Odd</source>
        <translation>Impaires</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="295"/>
        <source>Even</source>
        <translation>Paires</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="296"/>
        <source>None</source>
        <translation>Aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="390"/>
        <source>Choose days of the month or weeks of the year. A run without them belongs to the grid and is painted there.</source>
        <translation>Choisissez des jours du mois ou des semaines de l&apos;année. Une exécution sans eux appartient à la grille et y est peinte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="423"/>
        <source>Run = %1</source>
        <translation>Run = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="437"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
</context>
<context>
    <name>BRunScriptDialog</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="50"/>
        <source>Edit RunScript</source>
        <translation>Modifier le RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="59"/>
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="60"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="63"/>
        <source>Select Script</source>
        <translation>Sélectionner un script</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="70"/>
        <source>Command:</source>
        <translation>Commande :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="75"/>
        <source>Runs When:</source>
        <translation>Exécuter quand :</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="80"/>
        <source>Runs On Client</source>
        <translation>Exécuter sur le client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="84"/>
        <source>Runs On Failure</source>
        <translation>Exécuter en cas d&apos;échec</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="87"/>
        <source>Abort Job On Error</source>
        <translation>Abandonner le Job en cas d&apos;erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="91"/>
        <source>Fail Job On Error</source>
        <translation>Faire échouer le Job en cas d&apos;erreur</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="102"/>
        <source>Validation</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="102"/>
        <source>Command is required.</source>
        <translation>La commande est obligatoire.</translation>
    </message>
</context>
<context>
    <name>BRunScriptEditor</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>Runs When</source>
        <translation>Exécuter quand</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>On Client</source>
        <translation>Sur le client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="159"/>
        <source>+ Add Script</source>
        <translation>+ Ajouter un script</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="160"/>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="161"/>
        <source>Remove</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="236"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="236"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BRunWriter</name>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="181"/>
        <source>The run has an empty hour, day, month or week set and cannot be written as a Run line.</source>
        <translation>L&apos;exécution a un ensemble d&apos;heures, de jours, de mois ou de semaines vide et ne peut pas être écrite comme ligne Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="186"/>
        <source>The minute %1 is out of range.</source>
        <translation>La minute %1 est hors limites.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="250"/>
        <source>Run line &quot;%1&quot; is not understood (%2) and is never rewritten.</source>
        <translation>La ligne Run &quot;%1&quot; n&apos;est pas comprise (%2) et n&apos;est jamais réécrite.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="269"/>
        <source>The generated Run line &quot;%1&quot; does not read back: %2</source>
        <translation>La ligne Run générée &quot;%1&quot; ne se relit pas : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="274"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>La ligne Run générée &quot;%1&quot; ne dit pas ce qui était prévu.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="290"/>
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
</context>
<context>
    <name>BScheduleBrushDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="296"/>
        <source>New brush</source>
        <translation>Nouveau pinceau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="299"/>
        <source>A brush is what painting puts into a cell: a level, the minute of the hour and, if wanted, settings that replace those of the job.</source>
        <translation>Un pinceau est ce que la peinture place dans une cellule : un niveau, la minute de l&apos;heure et, si souhaité, des réglages qui remplacent ceux du Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="308"/>
        <source>Minute of the hour</source>
        <translation>Minute de l&apos;heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="309"/>
        <source>The run starts at this minute of every painted hour</source>
        <translation>L&apos;exécution démarre à cette minute de chaque heure peinte</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="310"/>
        <source>Minute of the &amp;hour:</source>
        <translation>Minute de l&apos;&amp;heure :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="313"/>
        <source>Level and overrides</source>
        <translation>Niveau et remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="369"/>
        <source>Brush: %1</source>
        <translation>Pinceau : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleEntry</name>
    <message>
        <location filename="../src/models/bresourcemodels.cpp" line="682"/>
        <source>The entry does not describe a run.</source>
        <translation>L&apos;entrée ne décrit pas une exécution.</translation>
    </message>
</context>
<context>
    <name>BScheduleGanttWidget</name>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="414"/>
        <source>(no client)</source>
        <translation>(aucun client)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="417"/>
        <source>(no storage)</source>
        <translation>(aucun storage)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="444"/>
        <source>Not used by any job</source>
        <translation>Utilisé par aucun Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="560"/>
        <source>No runs in this week</source>
        <translation>Aucune exécution cette semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="561"/>
        <source>No runs on this date</source>
        <translation>Aucune exécution à cette date</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Monday</source>
        <translation>Lundi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Tuesday</source>
        <translation>Mardi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Wednesday</source>
        <translation>Mercredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Thursday</source>
        <translation>Jeudi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Friday</source>
        <translation>Vendredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Saturday</source>
        <translation>Samedi</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Sunday</source>
        <translation>Dimanche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1091"/>
        <source>%1, if it turns out against it</source>
        <translation>%1, si cela tombe mal</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1161"/>
        <source>Utilization</source>
        <translation>Occupation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1412"/>
        <source>Not used by any enabled job: nothing starts at this time.</source>
        <translation>Utilisé par aucun Job activé : rien ne démarre à cette heure.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1413"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1430"/>
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1416"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1428"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2176"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1418"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1437"/>
        <source>Run</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1429"/>
        <source>(level of the job)</source>
        <translation>(niveau du Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1441"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1444"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1453"/>
        <source>Predicted wait</source>
        <translation>Attente prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2176"/>
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1453"/>
        <source>Predicted wait: none</source>
        <translation>Attente prévue : aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1464"/>
        <source>the jobs of the Director are not loaded: it is not known which job runs this schedule</source>
        <translation>les Jobs du Director ne sont pas chargés : on ne sait pas quel Job utilise ce Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1469"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>aucune exécution réussie de ce Job au cours des %1 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1473"/>
        <source>No history</source>
        <translation>Aucun historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1474"/>
        <source>Duration unknown: the outline marks the start only.</source>
        <translation>Durée inconnue : le contour ne marque que le début.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1477"/>
        <source>Median duration</source>
        <translation>Durée médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1479"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1486"/>
        <source>90%</source>
        <translation>90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1481"/>
        <source>runs</source>
        <translation>exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1484"/>
        <source>Median wait</source>
        <translation>Attente médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1489"/>
        <source>Wait</source>
        <translation>Attente</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1489"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1821"/>
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1493"/>
        <source>Estimate: no run at this level in the last %1 days, figures from all levels of the job.</source>
        <translation>Estimation : aucune exécution à ce niveau au cours des %1 derniers jours, chiffres issus de tous les niveaux du Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1499"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1502"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1511"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1513"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1517"/>
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1544"/>
        <source>job(s) running</source>
        <translation>Job(s) en cours</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1548"/>
        <source>%n run(s) predicted to wait</source>
        <translation>
            <numerusform>%n exécution devrait attendre</numerusform>
            <numerusform>%n exécutions devraient attendre</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1553"/>
        <source>%1 run(s) without history start here (not counted: duration unknown)</source>
        <translation>%1 exécution(s) sans historique démarrent ici (non comptées : durée inconnue)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1808"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2164"/>
        <source>JobId</source>
        <translation>JobId</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1809"/>
        <source>Running now</source>
        <translation>En cours maintenant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1809"/>
        <source>Waiting now</source>
        <translation>En attente maintenant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1811"/>
        <source>Queued at</source>
        <translation>En file depuis</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1814"/>
        <source>Started at</source>
        <translation>Démarré à</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1816"/>
        <source>Running for</source>
        <translation>En cours depuis</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1820"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2172"/>
        <source>Waited</source>
        <translation>Attente</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1820"/>
        <source>Waiting for</source>
        <translation>En attente depuis</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1823"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2183"/>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1826"/>
        <source>Known from the job history; the Director reports when it ends.</source>
        <translation>Connu par l&apos;historique des jobs ; le Director signale sa fin.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1827"/>
        <source>Reported by the Director as it happens.</source>
        <translation>Signalé par le Director au moment où cela se produit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2146"/>
        <source>not recorded</source>
        <translation>non enregistré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2151"/>
        <source>terminated normally</source>
        <translation>terminé normalement</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2152"/>
        <source>terminated with warnings</source>
        <translation>terminé avec des avertissements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2153"/>
        <source>terminated in error</source>
        <translation>terminé en erreur</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2154"/>
        <source>non-fatal error</source>
        <translation>erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2155"/>
        <source>fatal error</source>
        <translation>erreur fatale</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2156"/>
        <source>canceled</source>
        <translation>annulé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2157"/>
        <source>running</source>
        <translation>en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2158"/>
        <source>created, not yet running</source>
        <translation>créé, pas encore en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2159"/>
        <source>status %1</source>
        <translation>statut %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2162"/>
        <source>Measured run of %1</source>
        <translation>Exécution mesurée de %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2165"/>
        <source>Scheduled</source>
        <translation>Planifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2166"/>
        <source>Started</source>
        <translation>Démarré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2169"/>
        <source>still running</source>
        <translation>toujours en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2170"/>
        <source>Ended</source>
        <translation>Terminé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2174"/>
        <source>Ran</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2181"/>
        <source>planned: %1</source>
        <translation>planifié : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="32"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="79"/>
        <source>Undone: %1</source>
        <translation>Annulé : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="43"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="88"/>
        <source>Redone: %1</source>
        <translation>Rétabli : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="206"/>
        <source>Eraser</source>
        <translation>Gomme</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="259"/>
        <source>This schedule contains a Run line that is not understood. It is locked: nothing in it can be changed here.</source>
        <translation>Ce Schedule contient une ligne Run qui n&apos;est pas comprise. Il est verrouillé : rien ne peut y être modifié ici.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="286"/>
        <source>%n cell(s)</source>
        <translation>
            <numerusform>%n cellule</numerusform>
            <numerusform>%n cellules</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="288"/>
        <source>%1 of the scope %2</source>
        <translation>%1 de la portée %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="300"/>
        <source>Paint %1 on %2</source>
        <translation>Peindre %1 sur %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="309"/>
        <source>Add %1 to %2</source>
        <translation>Ajouter %1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="317"/>
        <source>Erase %1</source>
        <translation>Effacer %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="325"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="351"/>
        <source>No cell is selected.</source>
        <translation>Aucune cellule n&apos;est sélectionnée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="333"/>
        <source>No brush is chosen.</source>
        <translation>Aucun pinceau n&apos;est choisi.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="377"/>
        <source>A scope needs at least one month, and a week of the month or &quot;last&quot;.</source>
        <translation>Une portée nécessite au moins un mois, et une semaine du mois ou &quot;dernière&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="387"/>
        <source>Add the scope %1</source>
        <translation>Ajouter la portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="402"/>
        <source>The scope &quot;%1&quot; is always there.</source>
        <translation>La portée &quot;%1&quot; est toujours présente.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="407"/>
        <source>The scope %1 still has Run lines. Erase its cells first.</source>
        <translation>La portée %1 contient encore des lignes Run. Effacez d&apos;abord ses cellules.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="413"/>
        <source>Remove the scope %1</source>
        <translation>Supprimer la portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="423"/>
        <source>Add a calendar rule</source>
        <translation>Ajouter une règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="434"/>
        <source>Change the calendar rule %1</source>
        <translation>Modifier la règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="446"/>
        <source>Remove the calendar rule %1</source>
        <translation>Supprimer la règle de calendrier %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="718"/>
        <source>Run line %1 (&quot;%2&quot;) is not understood: %3</source>
        <translation>La ligne Run %1 (&quot;%2&quot;) n&apos;est pas comprise : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="727"/>
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1180"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1229"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1254"/>
        <source>There is no Run line %1.</source>
        <translation>Il n&apos;y a pas de ligne Run %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1182"/>
        <source>Run line %1 is not a line of the grid and is not changed here.</source>
        <translation>La ligne Run %1 n&apos;est pas une ligne de la grille et n&apos;est pas modifiée ici.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1210"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1233"/>
        <source>A run without days of the month and without weeks of the year is not a calendar rule; it is painted in the grid.</source>
        <translation>Une exécution sans jours du mois et sans semaines de l&apos;année n&apos;est pas une règle de calendrier ; elle est peinte dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1231"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1256"/>
        <source>Run line %1 is not a calendar rule.</source>
        <translation>La ligne Run %1 n&apos;est pas une règle de calendrier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1291"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>La ligne Run générée &quot;%1&quot; ne dit pas ce qui était prévu.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="136"/>
        <source>Runs</source>
        <translation>Exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="137"/>
        <source>Paint the runs on the grid: choose a level in the palette and drag over the hours. Calendar rules (days of the month, weeks of the year) are added below the grid.</source>
        <translation>Peignez les exécutions sur la grille : choisissez un niveau dans la palette et faites glisser sur les heures. Les règles de calendrier (jours du mois, semaines de l&apos;année) s&apos;ajoutent sous la grille.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridText</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="77"/>
        <source>Monday</source>
        <translation>Lundi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="78"/>
        <source>Tuesday</source>
        <translation>Mardi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="79"/>
        <source>Wednesday</source>
        <translation>Mercredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="80"/>
        <source>Thursday</source>
        <translation>Jeudi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="81"/>
        <source>Friday</source>
        <translation>Vendredi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="82"/>
        <source>Saturday</source>
        <translation>Samedi</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="83"/>
        <source>Sunday</source>
        <translation>Dimanche</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="91"/>
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="92"/>
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="93"/>
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="94"/>
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="95"/>
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="96"/>
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="97"/>
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="105"/>
        <source>January</source>
        <translation>Janvier</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="106"/>
        <source>February</source>
        <translation>Février</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="107"/>
        <source>March</source>
        <translation>Mars</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="108"/>
        <source>April</source>
        <translation>Avril</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="109"/>
        <source>May</source>
        <translation>Mai</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="110"/>
        <source>June</source>
        <translation>Juin</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="111"/>
        <source>July</source>
        <translation>Juillet</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="112"/>
        <source>August</source>
        <translation>Août</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="113"/>
        <source>September</source>
        <translation>Septembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="114"/>
        <source>October</source>
        <translation>Octobre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="115"/>
        <source>November</source>
        <translation>Novembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="116"/>
        <source>December</source>
        <translation>Décembre</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="125"/>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="131"/>
        <source>%1th</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="127"/>
        <source>%1st</source>
        <translation>%1er</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="128"/>
        <source>%1nd</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="129"/>
        <source>%1rd</source>
        <translation>%1e</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="145"/>
        <source>Every week</source>
        <translation>Chaque semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="160"/>
        <source>all</source>
        <translation>toutes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="190"/>
        <source>last week only</source>
        <translation>dernière semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="193"/>
        <source>first week only</source>
        <translation>première semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="194"/>
        <source>second week only</source>
        <translation>deuxième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="195"/>
        <source>third week only</source>
        <translation>troisième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="196"/>
        <source>fourth week only</source>
        <translation>quatrième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="197"/>
        <source>fifth week only</source>
        <translation>cinquième semaine uniquement</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="202"/>
        <source>only %1</source>
        <translation>uniquement %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="210"/>
        <source>level of the job</source>
        <translation>niveau du Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="253"/>
        <source>pool %1</source>
        <translation>pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="254"/>
        <source>full pool %1</source>
        <translation>pool Full %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="255"/>
        <source>incremental pool %1</source>
        <translation>pool Incremental %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="256"/>
        <source>differential pool %1</source>
        <translation>pool Differential %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="257"/>
        <source>next pool %1</source>
        <translation>pool suivant %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258"/>
        <source>storage %1</source>
        <translation>storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259"/>
        <source>messages %1</source>
        <translation>messages %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="260"/>
        <source>priority %1</source>
        <translation>priorité %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="261"/>
        <source>spool data yes</source>
        <translation>spool des données oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="261"/>
        <source>spool data no</source>
        <translation>spool des données non</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="262"/>
        <source>accurate yes</source>
        <translation>accurate oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="262"/>
        <source>accurate no</source>
        <translation>accurate non</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="263"/>
        <source>max run sched time %1</source>
        <translation>durée max. depuis la planification %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="269"/>
        <source>%1 at :%2</source>
        <translation>%1 à :%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="279"/>
        <source>%1 at %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="311"/>
        <source>%1 %2 hours</source>
        <translation>%1 %2 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="319"/>
        <source>%1: no run</source>
        <translation>%1 : aucune exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="328"/>
        <source>the Run line names no time</source>
        <translation>la ligne Run n&apos;indique aucune heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="341"/>
        <source>no run in this scope</source>
        <translation>aucune exécution dans cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="345"/>
        <source>in other scopes: %1</source>
        <translation>dans d&apos;autres portées : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="372"/>
        <source>and</source>
        <translation>et</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="373"/>
        <source>or</source>
        <translation>ou</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="381"/>
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="403"/>
        <source>on every %1 day (starting with the %2)</source>
        <translation>chaque %1 jour (à partir du %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="405"/>
        <source>on the %1</source>
        <translation>le %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="408"/>
        <source>%1 of every month</source>
        <translation>%1 de chaque mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="409"/>
        <source>%1 of %2</source>
        <translation>%1 en %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="411"/>
        <source>in %1</source>
        <translation>en %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="420"/>
        <source>in every %1 week of the year (starting with week %2)</source>
        <translation>chaque %1 semaine de l&apos;année (à partir de la semaine %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="428"/>
        <source>in week %1 of the year</source>
        <translation>la semaine %1 de l&apos;année</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="429"/>
        <source>in weeks %1 of the year</source>
        <translation>les semaines %1 de l&apos;année</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="436"/>
        <source>in the last seven days of the month</source>
        <translation>les sept derniers jours du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="438"/>
        <source>in the %1 week of the month</source>
        <translation>la %1 semaine du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="441"/>
        <source>or in its last seven days</source>
        <translation>ou ses sept derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="455"/>
        <source>only if it is a %1</source>
        <translation>uniquement si c&apos;est un %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="457"/>
        <source>on %1</source>
        <translation>le %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="465"/>
        <source>every hour at :00 (the line names no time)</source>
        <translation>toutes les heures à :00 (la ligne n&apos;indique aucune heure)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="467"/>
        <source>every hour at :%1</source>
        <translation>toutes les heures à :%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="475"/>
        <source>at %1</source>
        <translation>à %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridView</name>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="258"/>
        <source>Undo: %1 (Ctrl+Z)</source>
        <translation>Annuler : %1 (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="259"/>
        <source>Nothing to undo (Ctrl+Z)</source>
        <translation>Rien à annuler (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="260"/>
        <source>Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Rétablir : %1 (Ctrl+Shift+Z ou Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="261"/>
        <source>Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Rien à rétablir (Ctrl+Shift+Z ou Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="296"/>
        <source>No schedule chosen. Select a schedule in the list on the left to see it as a grid of weekdays and hours.</source>
        <translation>Aucun Schedule choisi. Sélectionnez un Schedule dans la liste de gauche pour l&apos;afficher sous forme de grille de jours de semaine et d&apos;heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="337"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="338"/>
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="340"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="341"/>
        <source>Redo</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="343"/>
        <source>Revert all</source>
        <translation>Tout annuler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="344"/>
        <source>Undoes every change of this schedule. Redo brings them back.</source>
        <translation>Annule toutes les modifications de ce Schedule. Rétablir les ramène.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="346"/>
        <source>&amp;Save...</source>
        <translation>&amp;Enregistrer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="348"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="843"/>
        <source>Writes the changed Run lines to the Director. The Director checks them first, and you see old and new lines and the jobs that use the schedule before anything is written.</source>
        <translation>Écrit les lignes Run modifiées sur le Director. Le Director les vérifie d&apos;abord, et vous voyez les anciennes et nouvelles lignes ainsi que les Jobs qui utilisent ce Schedule avant que quoi que ce soit ne soit écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="353"/>
        <source>Delete...</source>
        <translation>Supprimer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="354"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="847"/>
        <source>Deletes this schedule on the Director, after a check that no job uses it and your confirmation</source>
        <translation>Supprime ce Schedule sur le Director, après avoir vérifié qu&apos;aucun Job ne l&apos;utilise et après votre confirmation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="363"/>
        <source>Cop&amp;y...</source>
        <translation>&amp;Copier...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="367"/>
        <source>Re&amp;name...</source>
        <translation>Re&amp;nommer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="381"/>
        <source>No description</source>
        <translation>Pas de description</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="382"/>
        <source>The Description of the schedule. Saved with the Run lines; empty removes it from the file.</source>
        <translation>La Description du Schedule. Enregistrée avec les lignes Run ; vide, elle est retirée du fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="420"/>
        <source>&amp;Keep my changes</source>
        <translation>&amp;Conserver mes modifications</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="421"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="927"/>
        <source>&amp;Discard and reload</source>
        <translation>&amp;Abandonner et recharger</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="431"/>
        <source>Scopes</source>
        <translation>Portées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="435"/>
        <source>Scope:</source>
        <translation>Portée :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="436"/>
        <source>The weeks of the month and the months a Run line is restricted to. The grid shows the runs of the chosen scope; runs of the other scopes are hatched.</source>
        <translation>Les semaines du mois et les mois auxquels une ligne Run est restreinte. La grille affiche les exécutions de la portée choisie ; celles des autres portées sont hachurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="443"/>
        <source>Add scope</source>
        <translation>Ajouter une portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="444"/>
        <source>Add scope: weeks of the month, and months, to paint a grid of their own for</source>
        <translation>Ajouter une portée : des semaines du mois, et des mois, pour lesquels peindre une grille propre</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="469"/>
        <source>Legend</source>
        <translation>Légende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="490"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="494"/>
        <source>Run lines</source>
        <translation>Lignes Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <source>No.</source>
        <translation>N°</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <source>Shown in</source>
        <translation>Affiché dans</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="500"/>
        <source>The Run lines of the schedule in their order. Choosing a line outlines its cells in the grid and switches to its scope.</source>
        <translation>Les lignes Run du Schedule dans leur ordre. Choisir une ligne entoure ses cellules dans la grille et passe à sa portée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="516"/>
        <source>&amp;Add rule...</source>
        <translation>&amp;Ajouter une règle...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="517"/>
        <source>Add calendar rule</source>
        <translation>Ajouter une règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="518"/>
        <source>Adds a calendar rule: a Run line by day of the month or by week of the year, such as &quot;on 1 at 21:00&quot;</source>
        <translation>Ajoute une règle de calendrier : une ligne Run selon le jour du mois ou la semaine de l&apos;année, par exemple &quot;on 1 at 21:00&quot;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="520"/>
        <source>&amp;Edit rule...</source>
        <translation>&amp;Modifier la règle...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="521"/>
        <source>Edit calendar rule</source>
        <translation>Modifier la règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="522"/>
        <source>Opens the form of the calendar rule that is chosen in the list</source>
        <translation>Ouvre le formulaire de la règle de calendrier choisie dans la liste</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523"/>
        <source>&amp;Remove rule</source>
        <translation>&amp;Supprimer la règle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="524"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1426"/>
        <source>Remove calendar rule</source>
        <translation>Supprimer la règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="525"/>
        <source>Removes the calendar rule that is chosen in the list</source>
        <translation>Supprime la règle de calendrier choisie dans la liste</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="535"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="539"/>
        <source>Calendar rules</source>
        <translation>Règles de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Mark</source>
        <translation>Marque</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Runs</source>
        <translation>Exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="545"/>
        <source>Run lines by day of the month or week of the year. They do not belong to a weekday and are marked in the band below the grid. Enter or a double click opens the form of a rule, Del removes it.</source>
        <translation>Lignes Run selon le jour du mois ou la semaine de l&apos;année. Elles n&apos;appartiennent à aucun jour de semaine et sont marquées dans la bande sous la grille. Entrée ou un double-clic ouvre le formulaire d&apos;une règle, Suppr la supprime.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="563"/>
        <source>Next five weeks</source>
        <translation>Cinq prochaines semaines</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="569"/>
        <source>The levels that run on each date</source>
        <translation>Les niveaux exécutés à chaque date</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="574"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="575"/>
        <source>Previous month</source>
        <translation>Mois précédent</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="578"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="579"/>
        <source>Next month</source>
        <translation>Mois suivant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="581"/>
        <source>Today</source>
        <translation>Aujourd&apos;hui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="582"/>
        <source>Back to the current week</source>
        <translation>Revenir à la semaine en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="744"/>
        <source>Modified: the description, not written to the Director.</source>
        <translation>Modifié : la description, non écrite sur le Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="759"/>
        <source>%n changed</source>
        <translation>
            <numerusform>%n modifiée</numerusform>
            <numerusform>%n modifiées</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="761"/>
        <source>%n new</source>
        <translation>
            <numerusform>%n nouvelle</numerusform>
            <numerusform>%n nouvelles</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="763"/>
        <source>%n removed</source>
        <translation>
            <numerusform>%n supprimée</numerusform>
            <numerusform>%n supprimées</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="765"/>
        <source>Modified: %1 Run line(s) and the description, not written to the Director.</source>
        <translation>Modifié : %1 ligne(s) Run et la description, non écrites sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="767"/>
        <source>Modified: %1 Run line(s), not written to the Director.</source>
        <translation>Modifié : %1 ligne(s) Run, non écrite(s) sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="746"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="770"/>
        <source>The Director reports other Run lines since.</source>
        <translation>Le Director signale depuis d&apos;autres lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="376"/>
        <source>Descr&amp;iption:</source>
        <translation>Descr&amp;iption :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="819"/>
        <source>Creates a new schedule with the Run lines and the Description of this one, as the Director reported them (changes not written are not copied); you choose the name and see the new resource before anything is written</source>
        <translation>Crée un nouveau Schedule avec les lignes Run et la Description de celui-ci, telles que le Director les a signalées (les modifications non écrites ne sont pas copiées) ; vous choisissez le nom et voyez la nouvelle ressource avant que quoi que ce soit ne soit écrit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="827"/>
        <source>Save or revert the changes of this schedule first: renaming replaces it</source>
        <translation>Enregistrez ou annulez d&apos;abord les modifications de ce Schedule : le renommer le remplace</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="828"/>
        <source>Gives this schedule another name: a copy under the new name, every job that names it changed to the copy, then the old one deleted; all shown once for confirmation first</source>
        <translation>Donne un autre nom à ce Schedule : une copie sous le nouveau nom, chaque job qui le nomme modifié vers la copie, puis l&apos;ancien supprimé ; le tout est d&apos;abord montré une fois pour confirmation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="845"/>
        <source>This connection cannot change a schedule; the click says why.</source>
        <translation>Cette connexion ne peut pas modifier un Schedule ; un clic en indique la raison.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="849"/>
        <source>This connection cannot delete a schedule; the click says why.</source>
        <translation>Cette connexion ne peut pas supprimer un Schedule ; un clic en indique la raison.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="924"/>
        <source>The Director now reports other Run lines for this schedule than the ones your changes started from. Your changes are kept and nothing was replaced.</source>
        <translation>Le Director signale maintenant pour ce Schedule d&apos;autres lignes Run que celles dont vos modifications sont parties. Vos modifications sont conservées et rien n&apos;a été remplacé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="929"/>
        <source>The schedule %1 has changes that are not written. Showing %2 discards them.</source>
        <translation>Le Schedule %1 comporte des modifications non écrites. Afficher %2 les abandonne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="931"/>
        <source>&amp;Discard and show %1</source>
        <translation>&amp;Abandonner et afficher %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="958"/>
        <source>Schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="963"/>
        <source>(jobs not loaded yet)</source>
        <translation>(Jobs pas encore chargés)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="965"/>
        <source>Not used by any job</source>
        <translation>Utilisé par aucun Job</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="967"/>
        <source>Used by %n job(s)</source>
        <translation>
            <numerusform>Utilisé par %n Job</numerusform>
            <numerusform>Utilisé par %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="973"/>
        <source>Disabled: the Director does not run it.</source>
        <translation>Désactivé : le Director ne l&apos;exécute pas.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="978"/>
        <source>Contains %n Run line(s) that are not understood. The schedule is locked: it cannot be edited or written here. Change it in its configuration file on the Director.</source>
        <translation>
            <numerusform>Contient %n ligne Run non comprise. Le Schedule est verrouillé : il ne peut être ni modifié ni écrit ici. Modifiez-le dans son fichier de configuration sur le Director.</numerusform>
            <numerusform>Contient %n lignes Run non comprises. Le Schedule est verrouillé : il ne peut être ni modifié ni écrit ici. Modifiez-le dans son fichier de configuration sur le Director.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1007"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1230"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1010"/>
        <source>%1: no Run line in this scope. Del or the context menu removes it.</source>
        <translation>%1 : aucune ligne Run dans cette portée. Suppr ou le menu contextuel la supprime.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1011"/>
        <source>%1: no Run line in this scope</source>
        <translation>%1 : aucune ligne Run dans cette portée</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1012"/>
        <source>%1: %n Run line(s) in this scope</source>
        <translation>
            <numerusform>%1 : %n ligne Run dans cette portée</numerusform>
            <numerusform>%1 : %n lignes Run dans cette portée</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1018"/>
        <source>Scope %1, %n Run line(s)</source>
        <translation>
            <numerusform>Portée %1, %n ligne Run</numerusform>
            <numerusform>Portée %1, %n lignes Run</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1055"/>
        <source>other scope, may run on the same day</source>
        <translation>autre portée, peut s&apos;exécuter le même jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1059"/>
        <source>other scope, never on the same day</source>
        <translation>autre portée, jamais le même jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1073"/>
        <source>no time in the Run line: runs every hour at :00</source>
        <translation>aucune heure dans la ligne Run : s&apos;exécute toutes les heures à :00</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1081"/>
        <source>calendar rule, not a cell</source>
        <translation>règle de calendrier, pas une cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1111"/>
        <source>changed</source>
        <translation>modifiée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1114"/>
        <source>new</source>
        <translation>nouvelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1117"/>
        <source>removed</source>
        <translation>supprimée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1129"/>
        <source>Reported by the Director as &quot;%1&quot;, a spelling it does not read back.</source>
        <translation>Signalée par le Director sous la forme &quot;%1&quot;, une écriture qu&apos;il ne relit pas.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1136"/>
        <source>not understood: %1</source>
        <translation>non comprise : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1137"/>
        <source>Not understood and not shown in the grid: %1</source>
        <translation>Non comprise et non affichée dans la grille : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1138"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Ce Schedule ne peut pas être écrit par l&apos;application.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1147"/>
        <source>calendar rule %1</source>
        <translation>règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1152"/>
        <source>scope %1</source>
        <translation>portée %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1174"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1203"/>
        <source>Nothing selected. Click a cell or drag over cells; click a day or an hour to select all of it.</source>
        <translation>Rien n&apos;est sélectionné. Cliquez sur une cellule ou faites glisser sur des cellules ; cliquez sur un jour ou une heure pour le sélectionner entièrement.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1238"/>
        <source>%1 in %n cell(s)</source>
        <translation>
            <numerusform>%1 dans %n cellule</numerusform>
            <numerusform>%1 dans %n cellules</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1242"/>
        <source>%n cell(s) selected:</source>
        <translation>
            <numerusform>%n cellule sélectionnée :</numerusform>
            <numerusform>%n cellules sélectionnées :</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1243"/>
        <source>no run in this scope</source>
        <translation>aucune exécution dans cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1246"/>
        <source>Other scopes: %1</source>
        <translation>Autres portées : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1247"/>
        <source>They stay when this scope is painted or erased</source>
        <translation>Elles restent lorsque cette portée est peinte ou effacée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1352"/>
        <source>Remove the scope &quot;%1&quot;</source>
        <translation>Supprimer la portée &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1355"/>
        <source>Add scope...</source>
        <translation>Ajouter une portée...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1394"/>
        <source>Calendar rule %1</source>
        <translation>Règle de calendrier %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1397"/>
        <source>New calendar rule</source>
        <translation>Nouvelle règle de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1425"/>
        <source>Edit calendar rule...</source>
        <translation>Modifier la règle de calendrier...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1430"/>
        <source>Add calendar rule...</source>
        <translation>Ajouter une règle de calendrier...</translation>
    </message>
</context>
<context>
    <name>BScheduleGridWidget</name>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="71"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="415"/>
        <source>%1 hours</source>
        <translation>%1 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="72"/>
        <source>Select all</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="76"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="202"/>
        <source>Selects the whole day</source>
        <translation>Sélectionne toute la journée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="78"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="204"/>
        <source>Selects this hour on every day</source>
        <translation>Sélectionne cette heure pour chaque jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="80"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="205"/>
        <source>Selects all days and hours</source>
        <translation>Sélectionne tous les jours et toutes les heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="190"/>
        <source>Apply the tool</source>
        <translation>Appliquer l&apos;outil</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="192"/>
        <source>Erase</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="200"/>
        <source>Selects this cell</source>
        <translation>Sélectionne cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="208"/>
        <source>Lists the runs of this cell</source>
        <translation>Liste les exécutions de cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="210"/>
        <source>Applies the tool in hand (%1) to this cell</source>
        <translation>Applique l&apos;outil en main (%1) à cette cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="214"/>
        <source>Removes the runs of the scope on display from this cell</source>
        <translation>Retire de cette cellule les exécutions de la portée affichée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="313"/>
        <source>Schedule grid of %1, scope %2</source>
        <translation>Grille du Schedule %1, portée %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="317"/>
        <source>Weekdays by hours. Arrow keys move, Shift extends the selection, Enter lists the runs of a cell.</source>
        <translation>Jours de semaine par heures. Les flèches déplacent, Maj étend la sélection, Entrée liste les exécutions d&apos;une cellule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="320"/>
        <source>Space applies the tool to the selected cells, Delete erases them, F, D, I and V paint a level.</source>
        <translation>Espace applique l&apos;outil aux cellules sélectionnées, Suppr les efface, F, D, I et V peignent un niveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="322"/>
        <source>Tool: %1.</source>
        <translation>Outil : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="324"/>
        <source>Paint mode is on.</source>
        <translation>Le mode peinture est activé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="328"/>
        <source>Calendar rule %1: %2, %3</source>
        <translation>Règle de calendrier %1 : %2, %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="62"/>
        <source>Schedule grid</source>
        <translation>Grille du Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="284"/>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="634"/>
        <source>Calendar rules</source>
        <translation>Règles de calendrier</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1242"/>
        <source>%1, %2 to %3</source>
        <translation>%1, %2 à %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1252"/>
        <source>the Run line names no time</source>
        <translation>la ligne Run n&apos;indique aucune heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1255"/>
        <source>Run line %1: %2</source>
        <translation>Ligne Run %1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1262"/>
        <source>No run in the scope &quot;%1&quot;</source>
        <translation>Aucune exécution dans la portée &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1269"/>
        <source>Other scopes:</source>
        <translation>Autres portées :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1273"/>
        <source>%1: may run on the same day as this scope</source>
        <translation>%1 : peut s&apos;exécuter le même jour que cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1274"/>
        <source>%1: never on the same day as this scope</source>
        <translation>%1 : jamais le même jour que cette portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1280"/>
        <source>Painting or erasing here changes only the scope &quot;%1&quot;; the runs of other scopes stay.</source>
        <translation>Peindre ou effacer ici ne modifie que la portée &quot;%1&quot; ; les exécutions des autres portées restent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1291"/>
        <source>Load: no run of an enabled schedule starts in this hour</source>
        <translation>Charge : aucune exécution d&apos;un Schedule activé ne démarre à cette heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1292"/>
        <source>Load: %1 run(s) a week of all enabled schedules start in this hour (average of the next weeks)</source>
        <translation>Charge : %1 exécution(s) par semaine de tous les Schedules activés démarrent à cette heure (moyenne des prochaines semaines)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1309"/>
        <source>%1: click to select the whole day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1 : cliquez pour sélectionner toute la journée. Ctrl l&apos;ajoute ou la retire, Maj étend la sélection depuis son ancre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1313"/>
        <source>%1 to %2: click to select this hour on every day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1 à %2 : cliquez pour sélectionner cette heure pour chaque jour. Ctrl l&apos;ajoute ou la retire, Maj étend la sélection depuis son ancre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1318"/>
        <source>Click to select all days and hours (Ctrl+A)</source>
        <translation>Cliquez pour sélectionner tous les jours et toutes les heures (Ctrl+A)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1320"/>
        <source>Run lines by day of the month or week of the year do not belong to a weekday. They are marked here at their hours and listed under &quot;Calendar rules&quot;; this band cannot be selected or painted. A double click on a mark opens the form of its rule.</source>
        <translation>Les lignes Run selon le jour du mois ou la semaine de l&apos;année n&apos;appartiennent à aucun jour de semaine. Elles sont marquées ici à leurs heures et listées sous &quot;Règles de calendrier&quot; ; cette bande ne peut être ni sélectionnée ni peinte. Un double-clic sur une marque ouvre le formulaire de sa règle.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1401"/>
        <source>%1, %2 to %3: %n run(s)</source>
        <translation>
            <numerusform>%1, %2 à %3 : %n exécution</numerusform>
            <numerusform>%1, %2 à %3 : %n exécutions</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1409"/>
        <source>Runs of the cell</source>
        <translation>Exécutions de la cellule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Time</source>
        <translation>Heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Overrides</source>
        <translation>Remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Scope</source>
        <translation>Portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Run line</source>
        <translation>Ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1427"/>
        <source>no time in the Run line</source>
        <translation>aucune heure dans la ligne Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1433"/>
        <source>%1 (other scope, may run on the same day)</source>
        <translation>%1 (autre portée, peut s&apos;exécuter le même jour)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1434"/>
        <source>%1 (other scope, never on the same day)</source>
        <translation>%1 (autre portée, jamais le même jour)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1441"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1453"/>
        <source>Enter or a double click shows the Run line in the list. Esc closes.</source>
        <translation>Entrée ou un double-clic affiche la ligne Run dans la liste. Échap ferme.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1554"/>
        <source>Tool: %1</source>
        <translation>Outil : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleLegend</name>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="43"/>
        <source>Full backup. The length of the bar is the median duration of the job&apos;s measured runs at this level.</source>
        <translation>Sauvegarde Full. La longueur de la barre est la durée médiane des exécutions mesurées du Job à ce niveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="45"/>
        <source>Differential backup.</source>
        <translation>Sauvegarde Differential.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="47"/>
        <source>Incremental backup.</source>
        <translation>Sauvegarde Incremental.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="49"/>
        <source>Virtual full backup.</source>
        <translation>Sauvegarde VirtualFull.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="51"/>
        <source>Pale and dashed, &quot;~&quot; in front of the letter: no run at this level was measured; the length is estimated from the job&apos;s runs at other levels.</source>
        <translation>Pâle et en pointillés, &quot;~&quot; devant la lettre : aucune exécution n&apos;a été mesurée à ce niveau ; la longueur est estimée à partir des exécutions du Job aux autres niveaux.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="54"/>
        <source>Dashed outline with &quot;?&quot;: the job never ran, so there is no duration. Only the start is shown.</source>
        <translation>Contour en pointillés avec &quot;?&quot; : le Job ne s&apos;est jamais exécuté, il n&apos;y a donc pas de durée. Seul le début est affiché.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="56"/>
        <source>Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).</source>
        <translation>Prolongement plus clair avec un trait à son extrémité : neuf exécutions sur dix étaient terminées à ce moment (90e centile).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="58"/>
        <source>Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.</source>
        <translation>Sablier plein : l&apos;exécution attendra. Le trait coloré derrière lui est l&apos;heure planifiée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="60"/>
        <source>Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).</source>
        <translation>Contour de sablier : l&apos;exécution peut attendre, la prévision n&apos;est pas certaine (voir la liste des attentes prévues).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="62"/>
        <source>Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.</source>
        <translation>Bande hachurée : l&apos;attente, de l&apos;heure planifiée au début prévu. Un contour en pointillés : non certain.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="64"/>
        <source>&quot;?&quot; in front of a bar: the prediction for this run is not certain, it may have to wait.</source>
        <translation>&quot;?&quot; devant une barre : la prévision pour cette exécution n&apos;est pas certaine, elle pourrait devoir attendre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="66"/>
        <source>&quot;»&quot; at the right edge: the run starts after the last day on display.</source>
        <translation>&quot;»&quot; au bord droit : l&apos;exécution démarre après le dernier jour affiché.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="68"/>
        <source>Open diamond: no job uses this schedule. It marks where a run would start.</source>
        <translation>Losange vide : aucun Job n&apos;utilise ce Schedule. Il marque l&apos;endroit où une exécution démarrerait.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="70"/>
        <source>Arrow (day view only): the run at the head waits for the run at the tail, from the moment that one lets it go. The label names the limit; dotted: priority; dashed: only if something unknown turns out against the run. Point at a run to see its arrows.</source>
        <translation>Flèche (vue journalière uniquement) : l&apos;exécution à la pointe attend l&apos;exécution à la base, à partir du moment où celle-ci la libère. L&apos;étiquette nomme la limite ; pointillés fins : priorité ; tirets : uniquement si un élément inconnu tourne en défaveur de l&apos;exécution. Survolez une exécution pour voir ses flèches.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="74"/>
        <source>Thin track below a bar (past dates and today): the run as the job history measured it, from its start to its end. The dotted line in front of it is the time it waited after its scheduled time. Point at it for the times, level and status.</source>
        <translation>Piste fine sous une barre (dates passées et aujourd&apos;hui) : l&apos;exécution telle que l&apos;historique des jobs l&apos;a mesurée, de son début à sa fin. La ligne pointillée devant est le temps d&apos;attente après l&apos;heure planifiée. Pointez-la pour voir les heures, le niveau et le statut.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="78"/>
        <source>Hatched with a dashed outline in the track: the measured run failed or was canceled.</source>
        <translation>Hachuré avec un contour en pointillés dans la piste : l&apos;exécution mesurée a échoué ou a été annulée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="80"/>
        <source>Red line: now.</source>
        <translation>Trait rouge : maintenant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="301"/>
        <source>&lt;b&gt;What the marks mean&lt;/b&gt;</source>
        <translation>&lt;b&gt;Signification des marques&lt;/b&gt;</translation>
    </message>
</context>
<context>
    <name>BScheduleMiniCalendar</name>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="88"/>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="113"/>
        <source>Calendar of runs, %1</source>
        <translation>Calendrier des exécutions, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="119"/>
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="128"/>
        <source>%1: no run</source>
        <translation>%1 : aucune exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="131"/>
        <source>%1 %2 (Run line %3)</source>
        <translation>%1 %2 (ligne Run %3)</translation>
    </message>
</context>
<context>
    <name>BScheduleNamePage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="70"/>
        <source>New Schedule</source>
        <translation>Nouveau Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="71"/>
        <source>Name the schedule. On the next page you paint its runs on a grid of weekdays and hours.</source>
        <translation>Nommez le Schedule. Sur la page suivante, vous peignez ses exécutions sur une grille de jours de la semaine et d&apos;heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78"/>
        <source>e.g., WeeklyCycle</source>
        <translation>p. ex. WeeklyCycle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="80"/>
        <source>&amp;Name:</source>
        <translation>&amp;Nom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="84"/>
        <source>e.g., Full Sunday, incremental daily</source>
        <translation>p. ex. Full le dimanche, Incremental chaque jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="85"/>
        <source>&amp;Description:</source>
        <translation>&amp;Description :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="87"/>
        <source>&amp;Enabled</source>
        <translation>&amp;Activé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="88"/>
        <source>A schedule that is not enabled starts no job.</source>
        <translation>Un Schedule qui n&apos;est pas activé ne démarre aucun job.</translation>
    </message>
</context>
<context>
    <name>BSchedulePalette</name>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="127"/>
        <source>Brushes</source>
        <translation>Pinceaux</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="139"/>
        <location filename="../src/schedules/bschedulepalette.cpp" line="140"/>
        <source>Eraser</source>
        <translation>Gomme</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="141"/>
        <source>Removes the runs of the scope on display from cells. Del erases the selected cells with any tool in hand.</source>
        <translation>Retire des cellules les exécutions de la portée affichée. Suppr efface les cellules sélectionnées quel que soit l&apos;outil en main.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="144"/>
        <source>New brush...</source>
        <translation>Nouveau pinceau...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="146"/>
        <source>Defines a brush: level, minute of the hour and overrides</source>
        <translation>Définit un pinceau : niveau, minute de l&apos;heure et remplacements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="148"/>
        <source>Paint mode</source>
        <translation>Mode peinture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="150"/>
        <source>On: pressing a cell and dragging over cells applies the tool at once. Off: the mouse selects, and Space applies the tool to the selection. The key P switches it in the grid.</source>
        <translation>Activé : appuyer sur une cellule et faire glisser sur des cellules applique immédiatement l&apos;outil. Désactivé : la souris sélectionne et Espace applique l&apos;outil à la sélection. La touche P le bascule dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="154"/>
        <source>Keep other runs</source>
        <translation>Conserver les autres exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="156"/>
        <source>On: painting adds the brush to the runs a cell already has, so a cell can have several runs in one hour. Off: the cell ends up with the brush only. Shift+Space adds once without this switch.</source>
        <translation>Activé : peindre ajoute le pinceau aux exécutions qu&apos;une cellule possède déjà, une cellule peut donc avoir plusieurs exécutions dans une heure. Désactivé : la cellule ne contient plus que le pinceau. Maj+Espace ajoute une fois sans ce commutateur.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="201"/>
        <source>%1 = %2</source>
        <translation>%1 = %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="249"/>
        <source>Brush %1</source>
        <translation>Pinceau %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="250"/>
        <source>Takes this brush in hand: %1. Space applies it to the selected cells.</source>
        <translation>Prend ce pinceau en main : %1. Espace l&apos;applique aux cellules sélectionnées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="311"/>
        <source>In hand: the eraser.</source>
        <translation>En main : la gomme.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="313"/>
        <source>In hand: the brush %1.</source>
        <translation>En main : le pinceau %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="317"/>
        <source>Paint mode: press a cell or drag over cells to apply it.</source>
        <translation>Mode peinture : appuyez sur une cellule ou faites glisser sur des cellules pour l&apos;appliquer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="319"/>
        <source>Space applies it to the selected cells, Del erases them.</source>
        <translation>Espace l&apos;applique aux cellules sélectionnées, Suppr les efface.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="322"/>
        <source>Other runs of a painted cell stay.</source>
        <translation>Les autres exécutions d&apos;une cellule peinte restent.</translation>
    </message>
</context>
<context>
    <name>BSchedulePersistence</name>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="106"/>
        <source>The text of schedule %1 arrived changed</source>
        <translation>Le texte du Schedule %1 est arrivé modifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="107"/>
        <source>The SHA-256 of the text the Director sent does not match the text. Nothing was written.</source>
        <translation>Le SHA-256 du texte envoyé par le Director ne correspond pas au texte. Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="120"/>
        <source>Schedule %1 has changed on the Director</source>
        <translation>Le Schedule %1 a été modifié sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="121"/>
        <source>The configuration file of the schedule does not say what was loaded here, so the change was not sent. Reload the schedule and make the change again. %1</source>
        <translation>Le fichier de configuration du Schedule ne correspond pas à ce qui a été chargé ici ; la modification n&apos;a donc pas été envoyée. Rechargez le Schedule et refaites la modification. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="149"/>
        <source>Schedule %1 cannot be written</source>
        <translation>Le Schedule %1 ne peut pas être écrit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="150"/>
        <source>Run line %1 is not understood by this application (%2). A schedule with such a line is not changed here; change it in its configuration file.</source>
        <translation>La ligne Run %1 n&apos;est pas comprise par cette application (%2). Un Schedule contenant une telle ligne n&apos;est pas modifié ici ; modifiez-le dans son fichier de configuration.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="159"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="192"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="279"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="320"/>
        <source>Schedule %1 was not changed</source>
        <translation>Le Schedule %1 n&apos;a pas été modifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="160"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="379"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="450"/>
        <source>There is no connection to the configuration methods of the Director.</source>
        <translation>Aucune connexion aux méthodes de configuration du Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="193"/>
        <source>The change could not be written into the configuration text safely: %1</source>
        <translation>La modification n&apos;a pas pu être écrite de manière sûre dans le texte de configuration : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="201"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="267"/>
        <source>Nothing to write</source>
        <translation>Rien à écrire</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="202"/>
        <source>The Run lines and the description are the ones the Director has.</source>
        <translation>Les lignes Run et la description sont celles du Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="268"/>
        <source>The edited text is the one the Director has.</source>
        <translation>Le texte modifié est celui que possède le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="280"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="420"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="496"/>
        <source>Nothing was written.</source>
        <translation>Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="319"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="449"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="495"/>
        <source>Schedule %1 was not created</source>
        <translation>Le Schedule %1 n&apos;a pas été créé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="321"/>
        <source>The edited text was not sent to the Director: %1</source>
        <translation>Le texte modifié n&apos;a pas été envoyé au Director : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="329"/>
        <source>the name of the schedule has to stay %1.</source>
        <translation>le nom du Schedule doit rester %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="341"/>
        <source>The text was edited in the review; the Director checked the edited text.</source>
        <translation>Le texte a été modifié lors de la vérification ; le Director a vérifié le texte modifié.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="378"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="419"/>
        <source>Schedule %1 was not deleted</source>
        <translation>Le Schedule %1 n&apos;a pas été supprimé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="576"/>
        <source>Run line %1 cannot be written: %2</source>
        <translation>La ligne Run %1 ne peut pas être écrite : %2</translation>
    </message>
</context>
<context>
    <name>BScheduleResource</name>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="46"/>
        <source>The name is empty.</source>
        <translation>Le nom est vide.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="48"/>
        <source>The name is longer than 127 characters.</source>
        <translation>Le nom dépasse 127 caractères.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="51"/>
        <source>A name may contain only letters, digits and the characters _ - . : (no blanks).</source>
        <translation>Un nom ne peut contenir que des lettres, des chiffres et les caractères _ - . : (pas d&apos;espaces).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="54"/>
        <source>There is already a schedule named %1.</source>
        <translation>Il existe déjà un Schedule nommé %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="136"/>
        <source>The text does not start with a Schedule resource (line %1).</source>
        <translation>Le texte ne commence pas par une ressource Schedule (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="140"/>
        <source>Directives on the line of &quot;Schedule {&quot; cannot be edited safely (line %1).</source>
        <translation>Les directives situées sur la ligne de &quot;Schedule {&quot; ne peuvent pas être modifiées de manière sûre (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="146"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Texte après l&apos;accolade fermante de la ressource (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="150"/>
        <source>A nested block in a Schedule resource (line %1).</source>
        <translation>Un bloc imbriqué dans une ressource Schedule (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="153"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>L&apos;accolade fermante partage sa ligne avec une directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="184"/>
        <source>A quoted Run value does not end on its line (line %1).</source>
        <translation>Une valeur Run entre guillemets ne se termine pas sur sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="187"/>
        <source>Something follows the Run value on its line (line %1).</source>
        <translation>Quelque chose suit la valeur Run sur sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="195"/>
        <source>A Run directive shares its line with another directive (line %1).</source>
        <translation>Une directive Run partage sa ligne avec une autre directive (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="202"/>
        <source>A Run directive without a value (line %1).</source>
        <translation>Une directive Run sans valeur (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="212"/>
        <source>A Run directive that does not start its line (line %1).</source>
        <translation>Une directive Run qui ne commence pas sa ligne (ligne %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="223"/>
        <source>The text contains no Schedule resource.</source>
        <translation>Le texte ne contient aucune ressource Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="225"/>
        <source>The Schedule resource has no closing brace.</source>
        <translation>La ressource Schedule n&apos;a pas d&apos;accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="256"/>
        <source>The configuration file has %1 Run line(s), the schedule as it was loaded %2.</source>
        <translation>Le fichier de configuration a %1 ligne(s) Run, le Schedule tel qu&apos;il a été chargé %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="266"/>
        <source>Run line %1 differs: the file says &quot;%2&quot;, the schedule as it was loaded &quot;%3&quot;.</source>
        <translation>La ligne Run %1 diffère : le fichier indique &quot;%2&quot;, le Schedule tel qu&apos;il a été chargé &quot;%3&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="288"/>
        <source>A new Run line without text.</source>
        <translation>Une nouvelle ligne Run sans texte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="292"/>
        <source>A Run line refers to line %1, which the file does not have.</source>
        <translation>Une ligne Run fait référence à la ligne %1, que le fichier ne contient pas.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="294"/>
        <source>The Run lines would change their order; that is not written.</source>
        <translation>Les lignes Run changeraient d&apos;ordre ; ce n&apos;est pas écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="297"/>
        <source>A changed Run line without text.</source>
        <translation>Une ligne Run modifiée sans texte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="301"/>
        <source>A Run line contains a line break.</source>
        <translation>Une ligne Run contient un saut de ligne.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="403"/>
        <source>The new text could not be read back: %1</source>
        <translation>Le nouveau texte n&apos;a pas pu être relu : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="408"/>
        <source>The new text does not have the intended Run lines.</source>
        <translation>Le nouveau texte n&apos;a pas les lignes Run prévues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="410"/>
        <source>The new text would change more than the Run lines.</source>
        <translation>Le nouveau texte modifierait plus que les lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="412"/>
        <source>The new text would change the name of the schedule.</source>
        <translation>Le nouveau texte modifierait le nom du Schedule.</translation>
    </message>
</context>
<context>
    <name>BScheduleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="411"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="411"/>
        <source>Source File</source>
        <translation>Fichier source</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewDialog</name>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="57"/>
        <source>(no Run line)</source>
        <translation>(aucune ligne Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="94"/>
        <source>changed: </source>
        <translation>modifiée : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="95"/>
        <source>new: </source>
        <translation>nouvelle : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="96"/>
        <source>removed: </source>
        <translation>supprimée : </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="111"/>
        <source>Change schedule %1</source>
        <translation>Modifier le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="112"/>
        <source>Change schedule %1 on the Director?</source>
        <translation>Modifier le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="113"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Le Director a vérifié la nouvelle configuration et l&apos;accepte. Rien n&apos;est écrit tant que vous n&apos;avez pas choisi &quot;Écrire sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="117"/>
        <source>Create schedule %1</source>
        <translation>Créer le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="118"/>
        <source>Create schedule %1 on the Director?</source>
        <translation>Créer le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="119"/>
        <source>The Director checked the new schedule and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Le Director a vérifié le nouveau Schedule et l&apos;accepte. Rien n&apos;est écrit tant que vous n&apos;avez pas choisi &quot;Écrire sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="123"/>
        <source>Delete schedule %1</source>
        <translation>Supprimer le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="124"/>
        <source>Delete schedule %1 on the Director?</source>
        <translation>Supprimer le Schedule %1 sur le Director ?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="125"/>
        <source>The Director checked that nothing refers to the schedule. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Le Director a vérifié que rien ne fait référence au Schedule. Rien n&apos;est supprimé tant que vous n&apos;avez pas choisi &quot;Supprimer sur le Director&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="143"/>
        <source>Run lines now (in the file)</source>
        <translation>Lignes Run actuelles (dans le fichier)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="146"/>
        <source>Run lines now</source>
        <translation>Lignes Run actuelles</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="151"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="154"/>
        <source>Run lines after writing</source>
        <translation>Lignes Run après l&apos;écriture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="162"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="171"/>
        <source>Jobs that use this schedule</source>
        <translation>Jobs qui utilisent ce Schedule</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="165"/>
        <source>The change applies to every one of these %n job(s), not only to the one on display. To move a single job, copy the schedule and assign the copy to that job instead.</source>
        <translation>
            <numerusform>La modification s&apos;applique à ce Job (%n), pas seulement à celui affiché. Pour déplacer un seul Job, copiez le Schedule et attribuez plutôt la copie à ce Job.</numerusform>
            <numerusform>La modification s&apos;applique à chacun de ces %n Jobs, pas seulement à celui affiché. Pour déplacer un seul Job, copiez le Schedule et attribuez plutôt la copie à ce Job.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="176"/>
        <source>(no job of the Director uses it, as far as this application knows)</source>
        <translation>(aucun Job du Director ne l&apos;utilise, à la connaissance de cette application)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="180"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Le Director indique comme y faisant référence : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="192"/>
        <source>Text after writing, editable</source>
        <translation>Texte après l&apos;écriture, modifiable</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="193"/>
        <source>This text can be edited. An edited text is checked by the Director and shown again before anything is written.</source>
        <translation>Ce texte peut être modifié. Un texte modifié est vérifié par le Director et affiché à nouveau avant que quoi que ce soit ne soit écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="196"/>
        <source>Text after writing</source>
        <translation>Texte après l&apos;écriture</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="199"/>
        <source>Text now</source>
        <translation>Texte actuel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="203"/>
        <source>The text was edited. &quot;Check with the Director&quot; sends it for a check only; the review then shows it again, and only &quot;Write to the Director&quot; there writes it.</source>
        <translation>Le texte a été modifié. &quot;Vérifier auprès du Director&quot; l&apos;envoie uniquement pour vérification ; la vérification l&apos;affiche ensuite à nouveau, et seul &quot;Écrire sur le Director&quot; l&apos;y écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="213"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Fichier sur le Director : %1 (il contient aussi d&apos;autres ressources ; seule cette ressource est remplacée).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="215"/>
        <source>File on the Director: %1.</source>
        <translation>Fichier sur le Director : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="218"/>
        <source>The Director reloads its configuration after writing. Runs it has already queued may still start at the old time.</source>
        <translation>Le Director recharge sa configuration après l&apos;écriture. Les exécutions qu&apos;il a déjà mises en file d&apos;attente peuvent encore démarrer à l&apos;ancienne heure.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="224"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Supprimer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="225"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Écrire sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="256"/>
        <source>&amp;Check with the Director</source>
        <translation>&amp;Vérifier auprès du Director</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="202"/>
        <source>Review</source>
        <translation>Vérification</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="203"/>
        <source>These are the Run lines and the resource that will be written. Finish creates the schedule.</source>
        <translation>Voici les lignes Run et la ressource qui seront écrites. « Terminer » crée le Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="208"/>
        <source>Run lines:</source>
        <translation>Lignes Run :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="215"/>
        <source>Resource:</source>
        <translation>Ressource :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="223"/>
        <source>Director command:</source>
        <translation>Commande du Director :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="233"/>
        <source>Copy Resource</source>
        <translation>Copier la ressource</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="234"/>
        <source>Copies the resource text to the clipboard</source>
        <translation>Copie le texte de la ressource dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="237"/>
        <source>Resource copied to clipboard.</source>
        <translation>Ressource copiée dans le presse-papiers.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="286"/>
        <source>This schedule cannot be written. %1</source>
        <translation>Ce Schedule ne peut pas être écrit. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Schedule cannot be written</source>
        <translation>Le Schedule ne peut pas être écrit</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="309"/>
        <source>%1

Go back and paint at least one run, or cancel the wizard.</source>
        <translation>%1

Revenez en arrière et peignez au moins une exécution, ou annulez l&apos;assistant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="322"/>
        <source>Deploy to Director</source>
        <translation>Déployer sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="323"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Envoyer &apos;configure add&apos; au Director ?

Cliquez sur Oui pour exécuter, sur Non pour fermer sans déployer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="347"/>
        <source>Sending configure command...</source>
        <translation>Envoi de la commande configure...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="369"/>
        <source>Another change of a schedule is being written. Try again when it is done.</source>
        <translation>Une autre modification d&apos;un Schedule est en cours d&apos;écriture. Réessayez lorsqu&apos;elle sera terminée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="375"/>
        <source>The Director checks the schedule...</source>
        <translation>Le Director vérifie le Schedule...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="383"/>
        <source>Schedule created.</source>
        <translation>Schedule créé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="387"/>
        <source>Nothing was written.</source>
        <translation>Rien n&apos;a été écrit.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="389"/>
        <source>%1: %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="416"/>
        <source>Schedule created successfully! Click Finish to close.</source>
        <translation>Schedule créé avec succès ! Cliquez sur Terminer pour fermer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="419"/>
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="427"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check with &apos;show schedules&apos; whether the schedule exists.</source>
        <translation>La connexion au Director a été perdue avant sa réponse. Reconnectez-vous et vérifiez avec &apos;show schedules&apos; si le Schedule existe.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="435"/>
        <source>Timeout waiting for Director response. The schedule may have been created — check with &apos;show schedules&apos;.</source>
        <translation>Délai dépassé en attente de la réponse du Director. Le Schedule a peut-être été créé — vérifiez avec &apos;show schedules&apos;.</translation>
    </message>
</context>
<context>
    <name>BScheduleScopeDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="382"/>
        <source>&amp;last (the last seven days of a month)</source>
        <translation>&amp;dernière (les sept derniers jours d&apos;un mois)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="386"/>
        <source>Add scope</source>
        <translation>Ajouter une portée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="389"/>
        <source>A scope is the part of the calendar a grid is painted for: weeks of the month, and months if not all. What is painted in it becomes Run lines such as &quot;1st sat at 21:00&quot;.</source>
        <translation>Une portée est la partie du calendrier pour laquelle une grille est peinte : des semaines du mois, et des mois si ce ne sont pas tous. Ce qui y est peint devient des lignes Run comme &quot;1st sat at 21:00&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="395"/>
        <source>Weeks of the month</source>
        <translation>Semaines du mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="401"/>
        <source>Days %1 to %2 of a month</source>
        <translation>Jours %1 à %2 d&apos;un mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="411"/>
        <source>Months</source>
        <translation>Mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="420"/>
        <source>&amp;All months</source>
        <translation>&amp;Tous les mois</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="477"/>
        <source>Choose at least one month.</source>
        <translation>Choisissez au moins un mois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="479"/>
        <source>Choose at least one week of the month, or &quot;last&quot;.</source>
        <translation>Choisissez au moins une semaine du mois, ou &quot;dernière&quot;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="485"/>
        <source>The scope &quot;%1&quot; exists already; it will be shown.</source>
        <translation>La portée &quot;%1&quot; existe déjà ; elle sera affichée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="486"/>
        <source>New scope: %1</source>
        <translation>Nouvelle portée : %1</translation>
    </message>
</context>
<context>
    <name>BScheduleWaitModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1259"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1283"/>
        <source>%1 s</source>
        <translation>%1 s</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1261"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1282"/>
        <source>%1 min</source>
        <translation>%1 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1266"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1281"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1304"/>
        <source>%1 h</source>
        <translation>%1 h</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1280"/>
        <source>%1 d</source>
        <translation>%1 j</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1291"/>
        <source>Max Run Time %1</source>
        <translation>Max Run Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1293"/>
        <source>Full Max Runtime %1</source>
        <translation>Full Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1295"/>
        <source>Incremental Max Runtime %1</source>
        <translation>Incremental Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1297"/>
        <source>Differential Max Runtime %1</source>
        <translation>Differential Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1299"/>
        <source>Max Run Sched Time %1</source>
        <translation>Max Run Sched Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1304"/>
        <source>no Max Run Time set, %1 assumed</source>
        <translation>aucun Max Run Time défini, %1 supposé</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1310"/>
        <source>%n run(s) wait, longest %1</source>
        <translation>
            <numerusform>%n exécution en attente, la plus longue %1</numerusform>
            <numerusform>%n exécutions en attente, la plus longue %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1311"/>
        <source>No waiting predicted</source>
        <translation>Aucune attente prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1313"/>
        <source>%1 uncertain</source>
        <translation>%1 incertaine(s)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1322"/>
        <source>Director limit: the Director runs %1 job(s) at a time (Maximum Concurrent Jobs = %1) and all of them are taken.</source>
        <translation>Limite du Director : le Director exécute %1 Job(s) à la fois (Maximum Concurrent Jobs = %1) et tous les emplacements sont occupés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1324"/>
        <source>Director limit: Maximum Concurrent Jobs of the Director is not known. With the Bareos default of 1 every job slot would be taken.</source>
        <translation>Limite du Director : le Maximum Concurrent Jobs du Director n&apos;est pas connu. Avec la valeur par défaut de Bareos de 1, tous les emplacements de Job seraient occupés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1328"/>
        <source>Storage limit: storage %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Limite du Storage : le Storage %1 accepte %2 Job(s) à la fois (Maximum Concurrent Jobs = %2) et toutes les places sont occupées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1332"/>
        <source>Storage limit: the storage of this job is not known, and so is its Maximum Concurrent Jobs. With one shared storage and the Bareos default of 1 it would be full.</source>
        <translation>Limite du Storage : le Storage de ce Job n&apos;est pas connu, pas plus que son Maximum Concurrent Jobs. Avec un seul Storage partagé et la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1335"/>
        <source>Storage limit: Maximum Concurrent Jobs of storage %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Limite du Storage : le Maximum Concurrent Jobs du Storage %1 n&apos;est pas connu. Avec la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1339"/>
        <source>Client limit: client %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Limite du Client : le Client %1 accepte %2 Job(s) à la fois (Maximum Concurrent Jobs = %2) et toutes les places sont occupées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1342"/>
        <source>Client limit: Maximum Concurrent Jobs of client %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Limite du Client : le Maximum Concurrent Jobs du Client %1 n&apos;est pas connu. Avec la valeur par défaut de Bareos de 1, il serait plein.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1346"/>
        <source>Job limit: job %1 may run %2 time(s) at once (Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.</source>
        <translation>Limite du Job : le Job %1 peut s&apos;exécuter %2 fois simultanément (Maximum Concurrent Jobs = %2) et un démarrage antérieur de celui-ci n&apos;est pas terminé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1349"/>
        <source>Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos default of 1 an earlier start of it would be in the way.</source>
        <translation>Limite du Job : le Maximum Concurrent Jobs du Job %1 n&apos;est pas connu. Avec la valeur par défaut de Bareos de 1, un démarrage antérieur de celui-ci gênerait.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1353"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. The Director starts a run of a lower priority (a higher number) only when no run of another priority is running.</source>
        <translation>Priorité : cette exécution a la priorité %1 et des exécutions de priorité %2 sont en cours. Le Director ne démarre une exécution de priorité inférieure (un nombre plus élevé) que lorsqu&apos;aucune exécution d&apos;une autre priorité n&apos;est en cours.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1358"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. It starts beside them only if it and all of them have Allow Mixed Priority.</source>
        <translation>Priorité : cette exécution a la priorité %1 et des exécutions de priorité %2 sont en cours. Elle ne démarre à côté d&apos;elles que si elle et toutes celles-ci ont Allow Mixed Priority.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1362"/>
        <source>Queue order: a run of priority %1 is waiting ahead in the queue and cannot start yet. The Director starts no run that is behind it in the queue.</source>
        <translation>Ordre de la file : une exécution de priorité %1 attend devant elle dans la file et ne peut pas encore démarrer. Le Director ne démarre aucune exécution placée derrière elle dans la file.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1386"/>
        <source>from %1, duration unknown</source>
        <translation>à partir de %1, durée inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1396"/>
        <source>%1 to about %2, estimated</source>
        <translation>%1 à environ %2, estimé</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1398"/>
        <source>%1 to %2</source>
        <translation>%1 à %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1407"/>
        <source>and %1 more</source>
        <translation>et %1 de plus</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1420"/>
        <source>%1: it may run until %2 (%3)</source>
        <translation>%1 : peut s&apos;exécuter jusqu&apos;à %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1421"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1426"/>
        <source>Maximum Concurrent Jobs of the Director is not known</source>
        <translation>Le Maximum Concurrent Jobs du Director n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1429"/>
        <source>the storage of the job is not known (the Job and its JobDefs name none)</source>
        <translation>le Storage du Job n&apos;est pas connu (le Job et ses JobDefs n&apos;en indiquent aucun)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1430"/>
        <source>Maximum Concurrent Jobs of storage %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Storage %1 n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1432"/>
        <source>Maximum Concurrent Jobs of client %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Client %1 n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1434"/>
        <source>Maximum Concurrent Jobs of job %1 is not known</source>
        <translation>Le Maximum Concurrent Jobs du Job %1 n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1436"/>
        <source>Allow Mixed Priority of job %1 is not known</source>
        <translation>L&apos;Allow Mixed Priority du Job %1 n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1438"/>
        <source>the duration of %1 was not measured at this level</source>
        <translation>la durée de %1 n&apos;a pas été mesurée à ce niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1440"/>
        <source>the duration of %1 was never measured</source>
        <translation>la durée de %1 n&apos;a jamais été mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1443"/>
        <source>the number of parallel writers of the storage device is not known (the job names no storage; the Director cannot read it)</source>
        <translation>le nombre d&apos;écritures parallèles du périphérique de Storage n&apos;est pas connu (le Job n&apos;indique aucun Storage ; le Director ne peut pas le lire)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1445"/>
        <source>the number of parallel writers of the device of storage %1 is not known (the Director cannot read it; set it in the list of predicted waits)</source>
        <translation>le nombre d&apos;écritures parallèles du périphérique du Storage %1 n&apos;est pas connu (le Director ne peut pas le lire ; définissez-le dans la liste des attentes prévues)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1509"/>
        <source>Director limit (Maximum Concurrent Jobs = %1)</source>
        <translation>Limite du Director (Maximum Concurrent Jobs = %1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1512"/>
        <source>Storage limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Storage (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1516"/>
        <source>Client limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Client (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1520"/>
        <source>Job limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Limite du Job (%1 : Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1524"/>
        <source>Priority %1, runs of priority %2 are running</source>
        <translation>Priorité %1, des exécutions de priorité %2 sont en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1528"/>
        <source>Queue order: a run of priority %1 waits ahead</source>
        <translation>Ordre de la file : une exécution de priorité %1 attend devant</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1535"/>
        <source>depends on values that are not known</source>
        <translation>dépend de valeurs inconnues</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1538"/>
        <source>May wait: %1</source>
        <translation>Peut attendre : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1540"/>
        <source>may be longer: %1</source>
        <translation>peut être plus longue : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1562"/>
        <source>%1 to %2: %3</source>
        <translation>%1 à %2 : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1568"/>
        <source>Waits for: %1</source>
        <translation>Attend : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1573"/>
        <source>Predicted start: %1, %2 after the scheduled time (%3)</source>
        <translation>Début prévu : %1, %2 après l&apos;heure planifiée (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1582"/>
        <source>it depends on values that are not known</source>
        <translation>cela dépend de valeurs inconnues</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1585"/>
        <source>Uncertain, the wait may be longer: %1.</source>
        <translation>Incertain, l&apos;attente peut être plus longue : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1587"/>
        <source>Uncertain, this run may have to wait: %1.</source>
        <translation>Incertain, cette exécution pourrait devoir attendre : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1591"/>
        <source>If so: %1</source>
        <translation>Si c&apos;est le cas : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1614"/>
        <source>Inside the run: waits %1 in the storage daemon for a device of storage %2 (%3 parallel writer(s); after two waits for a release it tries again every %4 s)%5.</source>
        <translation>Au sein de l&apos;exécution : attend %1 dans le Storage Daemon un périphérique du Storage %2 (%3 écriture(s) parallèle(s) ; après deux attentes de libération, il réessaie toutes les %4 s)%5.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1621"/>
        <source>, held by %1</source>
        <translation>, tenu par %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1624"/>
        <source>Predicted end: %1</source>
        <translation>Fin prévue : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1627"/>
        <source>Start overhead %1 (from the Director starting the job to its recorded start: connections, Client Run Before Job)%2.</source>
        <translation>Surcoût de démarrage %1 (du démarrage du Job par le Director à son début enregistré : connexions, Client Run Before Job)%2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1636"/>
        <source>Measured wait at this level: %1; predicted %2.</source>
        <translation>Attente mesurée à ce niveau : %1 ; prévue %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1663"/>
        <source>not measured</source>
        <translation>non mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1664"/>
        <source>median %1, max %2 (%3 run(s))</source>
        <translation>médiane %1, max %2 (%3 exécution(s))</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1675"/>
        <source>Prediction too low: measured median %1, predicted %2 - a limit the GUI does not know, e.g. the storage device (parallel writers), or a Client Run Before Job that takes long.</source>
        <translation>Prévision trop basse : médiane mesurée %1, prévue %2 - une limite que l&apos;interface ne connaît pas, p. ex. le périphérique de Storage (écritures parallèles), ou un Client Run Before Job qui dure longtemps.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1680"/>
        <source>Prediction too high: measured median %1, predicted %2 - a limit is set lower here than it is, or the durations used are too long.</source>
        <translation>Prévision trop haute : médiane mesurée %1, prévue %2 - une limite est définie ici plus basse qu&apos;elle ne l&apos;est, ou les durées utilisées sont trop longues.</translation>
    </message>
</context>
<context>
    <name>BScheduleWidget</name>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="135"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="927"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3680"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4054"/>
        <source>Jobs:</source>
        <translation>Jobs :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="144"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="237"/>
        <source>Timeline</source>
        <translation>Chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="238"/>
        <source>Grid</source>
        <translation>Grille</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="239"/>
        <source>Measured waits</source>
        <translation>Attentes mesurées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="240"/>
        <source>View:</source>
        <translation>Vue :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="246"/>
        <source>Day</source>
        <translation>Jour</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="247"/>
        <source>Week</source>
        <translation>Semaine</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="256"/>
        <source>Today</source>
        <translation>Aujourd&apos;hui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="258"/>
        <source>Show today or the current week</source>
        <translation>Afficher aujourd&apos;hui ou la semaine en cours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="268"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1109"/>
        <source>The date on display; the calendar opens from the arrow</source>
        <translation>La date affichée ; le calendrier s&apos;ouvre depuis la flèche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="281"/>
        <source>Snap:</source>
        <translation>Aimanter :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="288"/>
        <source>Zoom:</source>
        <translation>Zoom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="293"/>
        <source>Zoom level (pixels per hour)</source>
        <translation>Niveau de zoom (pixels par heure)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="348"/>
        <source>Legend</source>
        <translation>Légende</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="350"/>
        <source>What the bars, hourglasses, question marks and diamonds of the timeline mean</source>
        <translation>Signification des barres, sabliers, points d&apos;interrogation et losanges de la chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="371"/>
        <source>Schedules:</source>
        <translation>Schedules :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1988"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1169"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1170"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1172"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1171"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="390"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1210"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1989"/>
        <source>Median duration</source>
        <translation>Durée médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="419"/>
        <source>Clients (FD)</source>
        <translation>Clients (FD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="420"/>
        <source>Storage (SD)</source>
        <translation>Storage (SD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="720"/>
        <source>%1 schedules loaded</source>
        <translation>%1 Schedules chargés</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="810"/>
        <source>%1 schedules loaded (%2 Run lines)</source>
        <translation>%1 Schedules chargés (%2 lignes Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="852"/>
        <source>Refreshing schedule list...</source>
        <translation>Actualisation de la liste des Schedules...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="932"/>
        <source>Jobs (%1):</source>
        <translation>Jobs (%1) :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="934"/>
        <source>Jobs (%1 Schedules):</source>
        <translation>Jobs (%1 Schedules) :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="955"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="959"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="971"/>
        <source>-</source>
        <translation>-</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="973"/>
        <source>Median of %1 measured runs at all levels in the last %2 days</source>
        <translation>Médiane de %1 exécutions mesurées à tous les niveaux au cours des %2 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="975"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1438"/>
        <source>No history</source>
        <translation>Aucun historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1108"/>
        <source>The week on display: pick any day of it; the calendar opens from the arrow</source>
        <translation>La semaine affichée : choisissez n&apos;importe quel jour de celle-ci ; le calendrier s&apos;ouvre par la flèche</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Mon</source>
        <translation>Lun</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Tue</source>
        <translation>Mar</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Wed</source>
        <translation>Mer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Thu</source>
        <translation>Jeu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Fri</source>
        <translation>Ven</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Sat</source>
        <translation>Sam</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Sun</source>
        <translation>Dim</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="486"/>
        <source>Measured history</source>
        <translation>Historique mesuré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="333"/>
        <source>Measured runs</source>
        <translation>Exécutions mesurées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="335"/>
        <source>Show the runs of past dates and today as the job history measured them, in a thin track below the planned bars</source>
        <translation>Affiche les exécutions des dates passées et d&apos;aujourd&apos;hui telles que l&apos;historique des jobs les a mesurées, dans une piste fine sous les barres planifiées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="497"/>
        <source>Close the area below the timeline</source>
        <translation>Fermer la zone sous la frise chronologique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1141"/>
        <source>Click a run in the timeline: its measured history appears here.</source>
        <translation>Cliquez sur une exécution dans la frise chronologique : son historique mesuré s&apos;affiche ici.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1173"/>
        <source>Priority</source>
        <translation>Priorité</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1211"/>
        <source>25% - 75%</source>
        <translation>25 % - 75 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1212"/>
        <source>90%</source>
        <translation>90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1219"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1988"/>
        <source>Runs</source>
        <translation>Exécutions</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1225"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>Median wait</source>
        <translation>Attente médiane</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1226"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>90% wait</source>
        <translation>Attente à 90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <source>Start</source>
        <translation>Début</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1988"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <source>Duration</source>
        <translation>Durée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Wait</source>
        <translation>Attente</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1378"/>
        <source>no job uses this schedule</source>
        <translation>aucun Job n&apos;utilise ce Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1382"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>aucune exécution réussie de ce Job au cours des %1 derniers jours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1385"/>
        <source>No history: %1.</source>
        <translation>Aucun historique : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1397"/>
        <source>Estimate: no run at this level in the last %1 days. The figures are from all levels of the job.</source>
        <translation>Estimation : aucune exécution à ce niveau au cours des %1 derniers jours. Les chiffres proviennent de tous les niveaux du Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1413"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1414"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1433"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2386"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3205"/>
        <source>unknown</source>
        <translation>inconnue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1467"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1473"/>
        <source>1 hour</source>
        <translation>1 heure</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1468"/>
        <source>2 hours</source>
        <translation>2 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1469"/>
        <source>4 hours</source>
        <translation>4 heures</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1471"/>
        <source>15 min</source>
        <translation>15 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1472"/>
        <source>30 min</source>
        <translation>30 min</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1540"/>
        <source>Run line %1 &quot;%2&quot;: %3</source>
        <translation>Ligne Run %1 &quot;%2&quot; : %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1555"/>
        <source>Not understood and not shown in the views:</source>
        <translation>Non comprises et non affichées dans les vues :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1557"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Ce Schedule ne peut pas être écrit par l&apos;application.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="1563"/>
        <source>%n Run line(s) not understood</source>
        <translation>
            <numerusform>%n ligne Run non comprise</numerusform>
            <numerusform>%n lignes Run non comprises</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1565"/>
        <source>These Run lines are not shown in the views, and their schedules cannot be written by the application:</source>
        <translation>Ces lignes Run ne sont pas affichées dans les vues, et leurs Schedules ne peuvent pas être écrits par l&apos;application :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1705"/>
        <source>Schedule %1 was not changed: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird), which are not available on this connection. %2 stays at %3 (dropped at %4).</source>
        <translation>Le Schedule %1 n&apos;a pas été modifié : la modification d&apos;un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (onesimus-dird), qui ne sont pas disponibles sur cette connexion. %2 reste à %3 (déposé à %4).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1738"/>
        <source>Schedule %1 was not changed: another change is being written.</source>
        <translation>Le Schedule %1 n&apos;a pas été modifié : une autre modification est en cours d&apos;écriture.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1752"/>
        <source>Run line %1 is not understood, so the schedule is locked.</source>
        <translation>La ligne Run %1 n&apos;est pas comprise, le Schedule est donc verrouillé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1755"/>
        <source>the dragged Run line runs at several hours of the day; change it in the grid.</source>
        <translation>la ligne Run déplacée s&apos;exécute à plusieurs heures de la journée ; modifiez-la dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1759"/>
        <source>the dragged run could not be matched to exactly one Run line; refresh and try again, or change it in the grid.</source>
        <translation>l&apos;exécution déplacée n&apos;a pas pu être associée à exactement une ligne Run ; actualisez et réessayez, ou modifiez-la dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1763"/>
        <source>its Run lines are not loaded yet.</source>
        <translation>ses lignes Run ne sont pas encore chargées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1766"/>
        <source>%1 is not a time of day.</source>
        <translation>%1 n&apos;est pas une heure de la journée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1770"/>
        <source>the new Run line did not read back as intended (%1).</source>
        <translation>la nouvelle ligne Run ne s&apos;est pas relue comme prévu (%1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1773"/>
        <source>Schedule %1 was not changed: %2</source>
        <translation>Le Schedule %1 n&apos;a pas été modifié : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1789"/>
        <source>Schedule %1 was not changed: the move was cancelled.</source>
        <translation>Le Schedule %1 n&apos;a pas été modifié : le déplacement a été annulé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1799"/>
        <source>Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). Only this line is written anew; the other lines stay as they are in the file.</source>
        <translation>La ligne Run %1 du Schedule %2 passe de %3 à %4 (déplacée dans la chronologie : %5). Seule cette ligne est réécrite ; les autres lignes restent telles qu&apos;elles sont dans le fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1804"/>
        <source>The Director checks the change of schedule %1...</source>
        <translation>Le Director vérifie la modification du Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1841"/>
        <source>Copying a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules), which are not available on this connection</source>
        <translation>Copier un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (créer des Schedules), qui ne sont pas disponibles sur cette connexion</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857"/>
        <source>Schedule %1 is locked: it contains a Run line that is not understood.</source>
        <translation>Le Schedule %1 est verrouillé : il contient une ligne Run qui n&apos;est pas comprise.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1863"/>
        <source>Schedule %1 was not written: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director. Your changes are kept in the grid.</source>
        <translation>Le Schedule %1 n&apos;a pas été écrit : la modification d&apos;un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director. Vos modifications sont conservées dans la grille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1869"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1899"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3970"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4009"/>
        <source>Another change is being written.</source>
        <translation>Une autre modification est en cours d&apos;écriture.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1875"/>
        <source>Schedule %1 cannot be written: %2</source>
        <translation>Le Schedule %1 ne peut pas être écrit : %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1878"/>
        <source>The Director checks the changes of schedule %1...</source>
        <translation>Le Director vérifie les modifications du Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1881"/>
        <source>The changes made in the grid. Lines that were not touched stay as they are in the file.</source>
        <translation>Les modifications faites dans la grille. Les lignes non touchées restent telles qu&apos;elles sont dans le fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1893"/>
        <source>Schedule %1 was not deleted: deleting a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Le Schedule %1 n&apos;a pas été supprimé : la suppression d&apos;un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1902"/>
        <source>The Director checks whether schedule %1 can be deleted...</source>
        <translation>Le Director vérifie si le Schedule %1 peut être supprimé...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1917"/>
        <source>The Director reloaded its configuration.</source>
        <translation>Le Director a rechargé sa configuration.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1919"/>
        <source>Follow-up commands: %1</source>
        <translation>Commandes complémentaires : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1978"/>
        <source>Refresh history</source>
        <translation>Actualiser l&apos;historique</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1981"/>
        <source>Ask the Director again for the jobs of the last days (llist jobs)</source>
        <translation>Redemander au Director les Jobs des derniers jours (llist jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1989"/>
        <source>90% duration</source>
        <translation>Durée à 90 %</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>Last run</source>
        <translation>Dernière exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1992"/>
        <source>Runs in the window that terminated normally or with warnings</source>
        <translation>Exécutions de la période terminées normalement ou avec des avertissements</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1994"/>
        <source>Wait: start of the job minus its scheduled time</source>
        <translation>Attente : début du Job moins son heure planifiée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1996"/>
        <source>Nine of ten runs waited no longer than this</source>
        <translation>Neuf exécutions sur dix n&apos;ont pas attendu plus longtemps que cela</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2119"/>
        <source>the request got no reply (%1)</source>
        <translation>la requête n&apos;a reçu aucune réponse (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2200"/>
        <source>the Director refused the request (%1)</source>
        <translation>le Director a refusé la requête (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2201"/>
        <source>no reason given</source>
        <translation>aucune raison indiquée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2204"/>
        <source>the Director&apos;s reply could not be read (%1)</source>
        <translation>la réponse du Director n&apos;a pas pu être lue (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2228"/>
        <source>Schedules: no job history, %1</source>
        <translation>Schedules : aucun historique des Jobs, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2262"/>
        <source>the job history is being loaded</source>
        <translation>l&apos;historique des Jobs est en cours de chargement</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2266"/>
        <source>the job history has not been requested yet</source>
        <translation>l&apos;historique des Jobs n&apos;a pas encore été demandé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2299"/>
        <source>Not connected.</source>
        <translation>Non connecté.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2301"/>
        <source>The job history has not been requested yet.</source>
        <translation>L&apos;historique des Jobs n&apos;a pas encore été demandé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2303"/>
        <source>No job history: %1.</source>
        <translation>Aucun historique des Jobs : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2304"/>
        <source>The schedule views work without it: runs have no duration and waits are unknown.</source>
        <translation>Les vues des Schedules fonctionnent sans lui : les exécutions n&apos;ont pas de durée et les attentes sont inconnues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2308"/>
        <source>Requesting the jobs of the last %1 days from the Director...</source>
        <translation>Demande au Director des Jobs des %1 derniers jours...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2314"/>
        <source>Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs terminated normally or with warnings and were measured.</source>
        <translation>Jobs planifiés au cours des %1 derniers jours, récupérés %2 : %3 lignes, dont %4 exécutions terminées normalement ou avec des avertissements ont été mesurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2323"/>
        <source>The row limit (%1) was reached and this Director returned the oldest rows: the newest runs of the window are missing.</source>
        <translation>La limite de lignes (%1) a été atteinte et ce Director a renvoyé les lignes les plus anciennes : les exécutions les plus récentes de la période manquent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2325"/>
        <source>The row limit (%1) was reached: the oldest runs of the window are missing.</source>
        <translation>La limite de lignes (%1) a été atteinte : les exécutions les plus anciennes de la période manquent.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2329"/>
        <source>This Director did not deliver the scheduled time: all waits are unknown.</source>
        <translation>Ce Director n&apos;a pas fourni l&apos;heure planifiée : toutes les attentes sont inconnues.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2331"/>
        <source>%1 runs came without a scheduled time: their wait is unknown.</source>
        <translation>%1 exécutions sont arrivées sans heure planifiée : leur attente est inconnue.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2335"/>
        <source>%1 rows were left out because they start before their scheduled time: copied, migrated and virtual full jobs carry the times of another job.</source>
        <translation>%1 lignes ont été ignorées car elles démarrent avant leur heure planifiée : les Jobs Copy, Migrate et VirtualFull portent les heures d&apos;un autre Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2340"/>
        <source>%1 rows were left out because their start or end time is missing or the end is before the start.</source>
        <translation>%1 lignes ont été ignorées car leur heure de début ou de fin manque, ou la fin précède le début.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2366"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3006"/>
        <source>(none)</source>
        <translation>(aucune)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2375"/>
        <source>From %1 of %2 runs; the others came without a scheduled time.</source>
        <translation>Sur la base de %1 exécutions sur %2 ; les autres sont arrivées sans heure planifiée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2384"/>
        <source>The Director delivered no scheduled time for these runs.</source>
        <translation>Le Director n&apos;a fourni aucune heure planifiée pour ces exécutions.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2868"/>
        <source>Waiting not predicted yet</source>
        <translation>Attente pas encore prévue</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2873"/>
        <source>The job history is being loaded. Without the measured durations every run would count as lasting as long as its job allows, and the waits would be a guess: they are predicted once the history is in.</source>
        <translation>L&apos;historique des Jobs est en cours de chargement. Sans les durées mesurées, chaque exécution durerait aussi longtemps que son Job le permet et les attentes seraient devinées : elles sont prévues dès que l&apos;historique est là.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2903"/>
        <source>Not predicted yet: the job history is being loaded.</source>
        <translation>Pas encore prévu : l&apos;historique des Jobs est en cours de chargement.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3824"/>
        <source>Write to the Director</source>
        <translation>Écrire sur le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3868"/>
        <source>Move a run of schedule %1</source>
        <translation>Déplacer une exécution du Schedule %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3869"/>
        <source>Schedule %1 is used by %n job(s): %2. What should change?</source>
        <translation>
            <numerusform>Le Schedule %1 est utilisé par %n job : %2. Que faut-il modifier ?</numerusform>
            <numerusform>Le Schedule %1 est utilisé par %n jobs : %2. Que faut-il modifier ?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3871"/>
        <source>Changing the schedule moves the run for every job that uses it. Moving only %1 creates a copy of the schedule named %2 with the run at %3 and sets the Schedule of %1 to it.</source>
        <translation>Modifier le Schedule déplace l&apos;exécution pour chaque job qui l&apos;utilise. Déplacer seulement %1 crée une copie du Schedule nommée %2 avec l&apos;exécution à %3 et règle le Schedule de %1 sur celle-ci.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3876"/>
        <source>Change schedule %1 for all %n jobs</source>
        <translation>
            <numerusform>Modifier le Schedule %1 pour %n job</numerusform>
            <numerusform>Modifier le Schedule %1 pour les %n jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3878"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4150"/>
        <source>Move only job %1</source>
        <translation>Déplacer seulement le job %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3940"/>
        <source>Renaming a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (read, create and delete schedules, read and change jobs), which are not available on this connection</source>
        <translation>Renommer un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (lire, créer et supprimer des Schedules, lire et modifier des jobs), qui ne sont pas disponibles sur cette connexion</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3965"/>
        <source>Schedule %1 was not copied: creating a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Le Schedule %1 n&apos;a pas été copié : créer un Schedule nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3974"/>
        <source>Schedule %1 was not copied: its Run lines are not loaded.</source>
        <translation>Le Schedule %1 n&apos;a pas été copié : ses lignes Run ne sont pas chargées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3981"/>
        <source>Copy schedule %1</source>
        <translation>Copier le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3982"/>
        <source>Name of the new schedule:</source>
        <translation>Nom du nouveau Schedule :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3989"/>
        <source>The Director checks the copy %1 of schedule %2...</source>
        <translation>Le Director vérifie la copie %1 du Schedule %2...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4001"/>
        <source>Schedule %1 was not renamed: %2.</source>
        <translation>Le Schedule %1 n&apos;a pas été renommé : %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4005"/>
        <source>Schedule %1 was not renamed: save or revert its changes first.</source>
        <translation>Le Schedule %1 n&apos;a pas été renommé : enregistrez ou annulez d&apos;abord ses modifications.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4012"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4084"/>
        <source>Rename schedule %1</source>
        <translation>Renommer le Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4012"/>
        <source>New name:</source>
        <translation>Nouveau nom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4027"/>
        <source>Reading schedule %1 and the jobs that use it...</source>
        <translation>Lecture du Schedule %1 et des jobs qui l&apos;utilisent...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4036"/>
        <source>Nothing was written: the file of schedule %1 does not say what was loaded (%2). Refresh and try again.</source>
        <translation>Rien n&apos;a été écrit : le fichier du Schedule %1 ne dit pas ce qui a été chargé (%2). Actualisez et réessayez.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4042"/>
        <source>Nothing was written: the Name of schedule %1 could not be found in its file.</source>
        <translation>Rien n&apos;a été écrit : le Name du Schedule %1 est introuvable dans son fichier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4051"/>
        <source>New schedule %1, the text of %2 under the new name:</source>
        <translation>Nouveau Schedule %1, le texte de %2 sous le nouveau nom :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4062"/>
        <source>Nothing was written: job %1 takes its Schedule from its JobDefs. Change the JobDefs first, or copy the schedule instead.</source>
        <translation>Rien n&apos;a été écrit : le job %1 reprend son Schedule de son JobDefs. Modifiez d&apos;abord le JobDefs, ou copiez plutôt le Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4067"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4128"/>
        <source>Nothing was written: on the Director job %1 names schedule %2, not %3. Refresh and try again.</source>
        <translation>Rien n&apos;a été écrit : sur le Director, le job %1 nomme le Schedule %2, et non %3. Actualisez et réessayez.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4082"/>
        <source>deletion of schedule %1</source>
        <translation>suppression du Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4083"/>
        <source>Then schedule %1 is deleted.</source>
        <translation>Ensuite, le Schedule %1 est supprimé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4085"/>
        <source>Nothing was written: the rename of schedule %1 was not confirmed.</source>
        <translation>Rien n&apos;a été écrit : le renommage du Schedule %1 n&apos;a pas été confirmé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4090"/>
        <source>Schedule %1 is now named %2. Written: %3.</source>
        <translation>Le Schedule %1 s&apos;appelle maintenant %2. Écrit : %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4120"/>
        <source>Reading job %1...</source>
        <translation>Lecture du job %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4144"/>
        <source>New schedule %1, a copy of %2 with Run line %3 at %4, for job %5 only:</source>
        <translation>Nouveau Schedule %1, une copie de %2 avec la ligne Run %3 à %4, pour le job %5 uniquement :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4146"/>
        <source>Job:</source>
        <translation>Job :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4149"/>
        <source>Schedule %1 and its other jobs stay as they are.</source>
        <translation>Le Schedule %1 et ses autres jobs restent tels quels.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4151"/>
        <source>Nothing was written: the move of job %1 was not confirmed.</source>
        <translation>Rien n&apos;a été écrit : le déplacement du job %1 n&apos;a pas été confirmé.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4154"/>
        <source>Job %1 now runs at %2 on its own schedule %3. Written: %4.</source>
        <translation>Le job %1 s&apos;exécute maintenant à %2 avec son propre Schedule %3. Écrit : %4.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3312"/>
        <source>Replays the queue with the jobs moved quarter hour by quarter hour and keeps the smallest moves that wait least</source>
        <translation>Rejoue la file avec les Jobs décalés de quart d&apos;heure en quart d&apos;heure et garde les plus petits décalages qui attendent le moins</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3514"/>
        <source>Stays: a Run line of its schedule runs at several hours, is not understood or would move into the next day</source>
        <translation>Reste : une ligne Run de son Schedule s&apos;exécute à plusieurs heures, n&apos;est pas comprise ou passerait au jour suivant</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3518"/>
        <source>Moves along to run after %1, which has a higher priority</source>
        <translation>Est déplacé pour s&apos;exécuter après %1, qui a une priorité plus élevée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3311"/>
        <source>Propose later start times</source>
        <translation>Proposer des heures de début plus tardives</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3300"/>
        <source>Jobs that are scheduled for the same moment and share a limit (a storage, a client, the job slots of the Director, the parallel writers of a device) wait for each other. The proposal starts some of them later, on a quarter hour (23:15, 23:30 ...), so that they wait less on the dates on display and have room to spare. A wait for a higher priority is meant to be and is not counted; a job of a lower priority that follows a moved job moves along, so that it still runs after it. With &quot;Constraints...&quot; a job can be kept where it is or kept from starting after a given time. Nothing is changed on the Director.</source>
        <translation>Les Jobs planifiés au même moment qui partagent une limite (un Storage, un Client, les emplacements de jobs du Director, les écritures parallèles d&apos;un périphérique) s&apos;attendent mutuellement. La proposition démarre certains d&apos;entre eux plus tard, au quart d&apos;heure (23:15, 23:30 ...), afin qu&apos;ils attendent moins aux dates affichées et aient de la marge. Une attente due à une priorité plus haute est voulue et n&apos;est pas comptée ; un job de priorité plus basse qui suit un job déplacé est déplacé avec lui, afin qu&apos;il s&apos;exécute toujours après lui. Avec « Contraintes... », un job peut être maintenu à sa place ou empêché de démarrer après une heure donnée. Rien n&apos;est modifié sur le Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3316"/>
        <source>Constraints...</source>
        <translation>Contraintes...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3317"/>
        <source>Jobs that must not move, or must not start after a given time; kept on this computer for this Director</source>
        <translation>Jobs qui ne doivent pas être déplacés ou ne doivent pas démarrer après une heure donnée ; conservé sur cet ordinateur pour ce Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3326"/>
        <source>Apply...</source>
        <translation>Appliquer...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Start now</source>
        <translation>Début actuel</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Proposed start</source>
        <translation>Début proposé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Later by</source>
        <translation>Plus tard de</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Wait now</source>
        <translation>Attente actuelle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Wait then</source>
        <translation>Attente ensuite</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Note</source>
        <translation>Remarque</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3336"/>
        <source>Time held by a limit or waiting for a device, summed over the job&apos;s runs on the dates on display</source>
        <translation>Temps retenu par une limite ou en attente d&apos;un périphérique, additionné sur les exécutions du Job aux dates affichées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3338"/>
        <source>Proposed later start times</source>
        <translation>Heures de début plus tardives proposées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3392"/>
        <source>No proposal yet.</source>
        <translation>Pas encore de proposition.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3407"/>
        <source>Creates a schedule of its own for every job that starts later and sets the job&apos;s Schedule to it; everything is shown once for confirmation first</source>
        <translation>Crée un Schedule propre pour chaque Job qui démarre plus tard et y règle le Schedule du Job ; tout est d&apos;abord montré une fois pour confirmation</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3409"/>
        <source>Needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules, read and change jobs), which are not available on this connection</source>
        <translation>Nécessite les méthodes de configuration de l&apos;interface gRPC de l&apos;Onesimus Director (créer des Schedules, lire et modifier des Jobs), qui ne sont pas disponibles sur cette connexion</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3445"/>
        <source>Nothing to stagger: the waits are not predicted (the jobs or the history are not loaded yet).</source>
        <translation>Rien à étaler : les attentes ne sont pas prédites (les Jobs ou l&apos;historique ne sont pas encore chargés).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3463"/>
        <source>Nothing to stagger: no run on the dates on display waits for a limit or a device.</source>
        <translation>Rien à étaler : aucune exécution aux dates affichées n&apos;attend une limite ou un périphérique.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3466"/>
        <source>No later start helps: avoidable wait %1. See the notes of the jobs.</source>
        <translation>Aucun début plus tardif n&apos;aide : attente évitable %1. Voir les remarques des Jobs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3469"/>
        <source>Avoidable wait on the dates on display: now %1, with the proposal %2. %n job(s) start later.</source>
        <translation>
            <numerusform>Attente évitable aux dates affichées : maintenant %1, avec la proposition %2. %n Job démarre plus tard.</numerusform>
            <numerusform>Attente évitable aux dates affichées : maintenant %1, avec la proposition %2. %n Jobs démarrent plus tard.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3510"/>
        <source>Fixed by constraint</source>
        <translation>Fixé par contrainte</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3512"/>
        <source>Stays: latest start %1 by constraint</source>
        <translation>Reste : début au plus tard %1 par contrainte</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3520"/>
        <source>A later start does not help</source>
        <translation>Un début plus tardif n&apos;aide pas</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3523"/>
        <source>latest start %1 by constraint</source>
        <translation>début au plus tard %1 par contrainte</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3530"/>
        <source>%n min</source>
        <translation>
            <numerusform>%n min</numerusform>
            <numerusform>%n min</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3576"/>
        <source>Apply the later start times</source>
        <translation>Appliquer les heures de début plus tardives</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3578"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3812"/>
        <source>These resources are written to the Director, in this order. The Director checks each one; the first that it refuses stops the rest.</source>
        <translation>Ces ressources sont écrites sur le Director, dans cet ordre. Le Director vérifie chacune ; la première qu&apos;il refuse arrête le reste.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3590"/>
        <source>Apply</source>
        <translation>Appliquer</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3636"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3922"/>
        <source>Nothing was written: %1</source>
        <translation>Rien n&apos;a été écrit : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3655"/>
        <source>Reading the jobs that start later...</source>
        <translation>Lecture des Jobs qui démarrent plus tard...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3676"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4050"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4138"/>
        <source>schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3677"/>
        <source>New schedule %1 for %2:</source>
        <translation>Nouveau Schedule %1 pour %2 :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3686"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4073"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4134"/>
        <source>Nothing was written: the text of job %1 has no closing brace.</source>
        <translation>Rien n&apos;a été écrit : le texte du Job %1 n&apos;a pas d&apos;accolade fermante.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3693"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4147"/>
        <source>(Schedule from its JobDefs)</source>
        <translation>(Schedule de ses JobDefs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3699"/>
        <source>Nothing was written: the later start times were not confirmed.</source>
        <translation>Rien n&apos;a été écrit : les heures de début plus tardives n&apos;ont pas été confirmées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3702"/>
        <source>Written: %1. The jobs start at their new times from now on.</source>
        <translation>Écrit : %1. Les Jobs démarrent désormais à leurs nouvelles heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3719"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3752"/>
        <source>Nothing was written: there is no connection to the configuration methods of the Director.</source>
        <translation>Rien n&apos;a été écrit : il n&apos;y a pas de connexion aux méthodes de configuration du Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3736"/>
        <source>Nothing was written: %1 %2 could not be read (%3).</source>
        <translation>Rien n&apos;a été écrit : %1 %2 n&apos;a pas pu être lu (%3).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3768"/>
        <source>Writing %1 failed: the connection to the configuration methods is gone. Written before: %2. Nothing after it was sent.</source>
        <translation>L&apos;écriture de %1 a échoué : la connexion aux méthodes de configuration est perdue. Écrit auparavant : %2. Rien n&apos;a été envoyé ensuite.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3774"/>
        <source>Writing %1...</source>
        <translation>Écriture de %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3781"/>
        <source>it was changed on the Director in the meantime</source>
        <translation>il a été modifié sur le Director entre-temps</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3783"/>
        <source>the name is taken</source>
        <translation>le nom est déjà pris</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3770"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3792"/>
        <source>nothing</source>
        <translation>rien</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3788"/>
        <source>it is still used by %1</source>
        <translation>il est encore utilisé par %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3794"/>
        <source>Writing %1 failed: %2. Written before: %3. Nothing after it was sent.</source>
        <translation>L&apos;écriture de %1 a échoué : %2. Écrit avant : %3. Rien n&apos;a été envoyé ensuite.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="476"/>
        <source>Predicted waits</source>
        <translation>Attentes prévues</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="490"/>
        <source>Stagger</source>
        <translation>Étaler</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="496"/>
        <source>Close the tabs below the timeline</source>
        <translation>Fermer les onglets sous la chronologie</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2479"/>
        <source>Reasons shared by several rows</source>
        <translation>Raisons communes à plusieurs lignes</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Scheduled</source>
        <translation>Planifié</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Predicted start</source>
        <translation>Début prévu</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Measured</source>
        <translation>Mesuré</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Certain</source>
        <translation>Certain</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2486"/>
        <source>What this job waited at this level in the job history (StartTime minus SchedTime): median / maximum. The catalog&apos;s start is set after the connections and Client Run Before Job, so that time is part of it.</source>
        <translation>Ce que ce Job a attendu à ce niveau dans l&apos;historique des Jobs (StartTime moins SchedTime) : médiane / maximum. Le début dans le catalogue est fixé après les connexions et le Client Run Before Job ; ce temps en fait donc partie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2490"/>
        <source>Predicted start minus scheduled time: seconds (&quot;27 s&quot;), minutes:seconds (&quot;1:35 min&quot;) or, from ten minutes on, hours:minutes (&quot;0:53 h&quot;)</source>
        <translation>Début prévu moins heure planifiée : secondes (&quot;27 s&quot;), minutes:secondes (&quot;1:35 min&quot;) ou, à partir de dix minutes, heures:minutes (&quot;0:53 h&quot;)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2493"/>
        <source>No: the prediction depends on a limit the Director did not report or on a duration that was not measured</source>
        <translation>Non : la prévision dépend d&apos;une limite que le Director n&apos;a pas signalée ou d&apos;une durée qui n&apos;a pas été mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2495"/>
        <source>Predicted waits of the dates on display</source>
        <translation>Attentes prévues pour les dates affichées</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2497"/>
        <source>One row per run that is predicted to wait or whose prediction is uncertain. The reason of the current row is below the table.</source>
        <translation>Une ligne par exécution dont l&apos;attente est prévue ou dont la prévision est incertaine. La raison de la ligne courante figure sous le tableau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2520"/>
        <source>Reason of the selected wait</source>
        <translation>Raison de l&apos;attente sélectionnée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2543"/>
        <source>Parallel writers of the devices</source>
        <translation>Écritures parallèles des périphériques</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2545"/>
        <source>Notes</source>
        <translation>Remarques</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2547"/>
        <source>Basis of the prediction</source>
        <translation>Base de la prévision</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2549"/>
        <source>Back-test</source>
        <translation>Test rétrospectif</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2559"/>
        <source>Back-test of the prediction</source>
        <translation>Test rétrospectif de la prévision</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2575"/>
        <source>Director (%1)</source>
        <translation>Director (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2577"/>
        <source>Storage %1 (%2)</source>
        <translation>Storage %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2579"/>
        <source>Client %1 (%2)</source>
        <translation>Client %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2581"/>
        <source>Job %1 (%2)</source>
        <translation>Job %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2583"/>
        <source>Priority %1</source>
        <translation>Priorité %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2585"/>
        <source>Queue: priority %1 first</source>
        <translation>File : priorité %1 d&apos;abord</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2593"/>
        <source>The prediction has changed: propose again.</source>
        <translation>La prédiction a changé : proposez à nouveau.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2869"/>
        <source>Waiting not predicted</source>
        <translation>Attente non prédite</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2877"/>
        <source>The jobs of the Director are not loaded: it is not known which job runs which schedule.</source>
        <translation>Les Jobs du Director ne sont pas chargés : on ne sait pas quel Job utilise quel Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2880"/>
        <source>Predicted for the dates on display from the limits of the Director (Maximum Concurrent Jobs, priorities) and the measured durations.</source>
        <translation>Prévu pour les dates affichées à partir des limites du Director (Maximum Concurrent Jobs, priorités) et des durées mesurées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2883"/>
        <source>Uncertain: the prediction depends on a limit the Director did not report or on a duration that was not measured.</source>
        <translation>Incertain : la prévision dépend d&apos;une limite que le Director n&apos;a pas signalée ou d&apos;une durée qui n&apos;a pas été mesurée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2887"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3101"/>
        <source>Opens or closes the list of predicted waits below the timeline.</source>
        <translation>Ouvre ou ferme la liste des attentes prévues sous la chronologie.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2905"/>
        <source>Not predicted: the jobs of the Director are not loaded.</source>
        <translation>Non prévu : les Jobs du Director ne sont pas chargés.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2907"/>
        <source>%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules from the day before on.</source>
        <translation>%1 exécution(s) affichée(s), rejouée(s) avec les %2 exécution(s) de tous les Schedules activés à partir de la veille.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2916"/>
        <source>the storage of job %1 (the job names none)</source>
        <translation>le Storage du Job %1 (le Job n&apos;en indique aucun)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2923"/>
        <source>storage %1</source>
        <translation>Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2930"/>
        <source>client %1</source>
        <translation>Client %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2935"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3690"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4077"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4140"/>
        <source>job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2942"/>
        <source>Director = %1</source>
        <translation>Director = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2944"/>
        <source>the Director</source>
        <translation>le Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2946"/>
        <source>storages: %1</source>
        <translation>Storages : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2948"/>
        <source>clients: %1</source>
        <translation>Clients : %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2950"/>
        <source>Maximum Concurrent Jobs as reported: %1.</source>
        <translation>Maximum Concurrent Jobs signalé : %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2952"/>
        <source>Maximum Concurrent Jobs not reported by this Director for: %1. Runs it matters for are uncertain.</source>
        <translation>Maximum Concurrent Jobs non signalé par ce Director pour : %1. Les exécutions concernées sont incertaines.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2976"/>
        <source>[%1] %2 rows: %3.</source>
        <translation>[%1] %2 lignes : %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3020"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3474"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3532"/>
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3027"/>
        <source>not measured</source>
        <translation>non mesurée</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3029"/>
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3029"/>
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3034"/>
        <source>Prediction too low</source>
        <translation>Prévision trop basse</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3035"/>
        <source>Prediction too low; %1</source>
        <translation>Prévision trop basse ; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3038"/>
        <source>Prediction too high</source>
        <translation>Prévision trop haute</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3039"/>
        <source>Prediction too high; %1</source>
        <translation>Prévision trop haute ; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3043"/>
        <source>Waits %1 for the device inside the run</source>
        <translation>Attend %1 le périphérique au sein de l&apos;exécution</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3057"/>
        <source>No run on display is predicted to wait.</source>
        <translation>Aucune exécution affichée ne devrait attendre.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3058"/>
        <source>Not predicted.</source>
        <translation>Non prévu.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3098"/>
        <source>Predicted waits: %1. The list is open.</source>
        <translation>Attentes prévues : %1. La liste est ouverte.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3099"/>
        <source>Predicted waits: %1. The list is closed.</source>
        <translation>Attentes prévues : %1. La liste est fermée.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3111"/>
        <source>Select a row to read its reason in full.</source>
        <translation>Sélectionnez une ligne pour lire sa raison en entier.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3119"/>
        <source>%1, scheduled %2:</source>
        <translation>%1, planifié %2 :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3200"/>
        <source>Storage %1, parallel writers of its device:</source>
        <translation>Storage %1, écritures parallèles de son périphérique :</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3207"/>
        <source>Parallel writers of the device of storage %1</source>
        <translation>Écritures parallèles du périphérique du Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3208"/>
        <source>How many jobs the device(s) of %1 write at a time: Maximum Concurrent Jobs of the Device resource in the storage daemon (several devices: their sum). The Director cannot read it. &quot;unknown&quot;: runs that depend on it are uncertain.</source>
        <translation>Combien de Jobs le(s) périphérique(s) de %1 écrivent simultanément : Maximum Concurrent Jobs de la ressource Device dans le Storage Daemon (plusieurs périphériques : leur somme). Le Director ne peut pas le lire. &quot;inconnu&quot; : les exécutions qui en dépendent sont incertaines.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3251"/>
        <source>No job history: no suggestion.</source>
        <translation>Aucun historique des Jobs : aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3253"/>
        <source>History suggests %1.</source>
        <translation>L&apos;historique suggère %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3255"/>
        <source>History gives no suggestion.</source>
        <translation>L&apos;historique ne donne aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3261"/>
        <source>Use %1</source>
        <translation>Utiliser %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3261"/>
        <source>Use</source>
        <translation>Utiliser</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3282"/>
        <source>Back-test: no job history to replay.</source>
        <translation>Test rétrospectif : aucun historique des Jobs à rejouer.</translation>
    </message>
</context>
<context>
    <name>BScheduleWizard</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="465"/>
        <source>Add Schedule</source>
        <translation>Ajouter un Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="517"/>
        <source>The schedule needs a name.</source>
        <translation>Le Schedule a besoin d&apos;un nom.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="519"/>
        <source>The name is too long: at most 127 characters.</source>
        <translation>Le nom est trop long : 127 caractères au maximum.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="523"/>
        <source>The name may contain letters, digits, blanks and _ . : - only.</source>
        <translation>Le nom ne peut contenir que des lettres, des chiffres, des espaces et _ . : -.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="525"/>
        <source>A schedule named %1 exists already.</source>
        <translation>Un Schedule nommé %1 existe déjà.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="536"/>
        <source>The schedule has no runs yet.</source>
        <translation>Le Schedule n&apos;a pas encore d&apos;exécutions.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="546"/>
        <source>The schedule has no runs yet. Paint at least one run on the grid.</source>
        <translation>Le Schedule n&apos;a pas encore d&apos;exécutions. Peignez au moins une exécution sur la grille.</translation>
    </message>
</context>
<context>
    <name>BSettings</name>
    <message>
        <location filename="../src/config/bsettings.cpp" line="168"/>
        <source>A stored password is missing from the system keychain. Enter the password of the connection profile again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="172"/>
        <source>The passwords of the connection profiles could not be read from the system keychain (%1). Unlock the keychain and restart Onesimus, or enter the passwords again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="175"/>
        <location filename="../src/config/bsettings.cpp" line="286"/>
        <source>no keychain available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="311"/>
        <source>No system keychain could be used (%1). The passwords of the connection profiles are stored in the settings file %2, readable only by you. Install and unlock a keychain (for example GNOME Keyring or KWallet) to keep them out of the file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="453"/>
        <source>The file is not valid JSON: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="485"/>
        <source>The file contains no connection profiles.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettings.cpp" line="530"/>
        <source>Default</source>
        <translation>Par défaut</translation>
    </message>
</context>
<context>
    <name>BSettingsDialog</name>
    <message>
        <location filename="../ui/bsettingsdialog.ui" line="14"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="67"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1149"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1180"/>
        <source>Reset</source>
        <translation>Réinitialiser</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="109"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="146"/>
        <source>🔌 Connection</source>
        <translation>🔌 Connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="150"/>
        <source>🎨 Appearance</source>
        <translation>🎨 Apparence</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="154"/>
        <source>⚙️ Behavior</source>
        <translation>⚙️ Comportement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="158"/>
        <source>🔧 Advanced</source>
        <translation>🔧 Avancé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="187"/>
        <source>Connection Manager</source>
        <translation>Gestionnaire de connexions</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="195"/>
        <source>Saved Connections</source>
        <translation>Connexions enregistrées</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="210"/>
        <source>+ Add</source>
        <translation>+ Ajouter</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="212"/>
        <source>Add a new connection profile</source>
        <translation>Ajouter un nouveau profil de connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="105"/>
        <source>Reset...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="106"/>
        <source>Reset the settings to their defaults at once. The connection profiles are kept unless you choose otherwise.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="112"/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="213"/>
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="215"/>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="243"/>
        <source>Profile</source>
        <translation>Profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="250"/>
        <source>e.g. Production Server, Test Environment</source>
        <translation>p. ex. Serveur de production, Environnement de test</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="251"/>
        <source>Profile Name:</source>
        <translation>Nom du profil :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="256"/>
        <source>Director Connection</source>
        <translation>Connexion au Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="263"/>
        <source>e.g. 192.168.1.100 or bareos-dir.local</source>
        <translation>p. ex. 192.168.1.100 ou bareos-dir.local</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="264"/>
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="269"/>
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="273"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole de console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="274"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="280"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d&apos;Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="282"/>
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="287"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Port gRPC du Director (par défaut : port BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="288"/>
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="295"/>
        <source>Forget certificate</source>
        <translation>Oublier le certificat</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="296"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Supprimer le certificat épinglé. La prochaine connexion demandera à nouveau si le certificat du serveur est digne de confiance.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="299"/>
        <source>Server certificate:</source>
        <translation>Certificat du serveur :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="307"/>
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="308"/>
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="309"/>
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="310"/>
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="311"/>
        <source>Server operating system (affects TLS paths in exported configs)</source>
        <translation>Système d&apos;exploitation du serveur (influe sur les chemins TLS dans les configurations exportées)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="312"/>
        <source>Server OS:</source>
        <translation>OS du serveur :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="316"/>
        <source>Director Name:</source>
        <translation>Nom du Director :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="320"/>
        <source>Console name for authentication (e.g., &apos;admin&apos; or &apos;onesimus&apos;)</source>
        <translation>Nom de console pour l&apos;authentification (p. ex. &apos;admin&apos; ou &apos;onesimus&apos;)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="321"/>
        <source>Console Name:</source>
        <translation>Nom de la console :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="326"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="333"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="339"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1485"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(le hachage MD5 du mot de passe apparaîtra ici)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="334"/>
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="374"/>
        <source>⚠️ TLS encrypts communication with the Director.
TLS is strongly recommended for production environments!</source>
        <translation>⚠️ TLS chiffre la communication avec le Director.
TLS est fortement recommandé pour les environnements de production !</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="382"/>
        <source>Authentication &amp; Encryption</source>
        <translation>Authentification et chiffrement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="388"/>
        <source>Authentication Method:</source>
        <translation>Méthode d&apos;authentification :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="394"/>
        <source>Use x509 certificates (instead of PSK)</source>
        <translation>Utiliser des certificats x509 (au lieu de PSK)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="396"/>
        <source>By default, TLS uses Pre-Shared Key (PSK) authentication.
Enable this to use X.509 certificate-based authentication instead.
Certificate mode requires CA certificate and client certificate/key files.</source>
        <translation>Par défaut, TLS utilise l&apos;authentification par clé pré-partagée (PSK).
Activez cette option pour utiliser plutôt l&apos;authentification par certificat X.509.
Le mode certificat nécessite un certificat CA ainsi que des fichiers de certificat/clé client.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="414"/>
        <source>Path to CA certificate (.pem)</source>
        <translation>Chemin du certificat CA (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="415"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="425"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="435"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="446"/>
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="420"/>
        <source>CA Certificate:</source>
        <translation>Certificat CA :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="424"/>
        <source>Path to client certificate (.pem)</source>
        <translation>Chemin du certificat client (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="430"/>
        <source>Client Certificate:</source>
        <translation>Certificat client :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="434"/>
        <source>Path to private key (.pem, .key)</source>
        <translation>Chemin de la clé privée (.pem, .key)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="440"/>
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="445"/>
        <source>Path to client certificate (.pfx)</source>
        <translation>Chemin du certificat client (.pfx)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="451"/>
        <source>PFX Certificate:</source>
        <translation>Certificat PFX :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="454"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Convertir PEM/DER en PFX...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="455"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="460"/>
        <source>Verify server certificate (recommended)</source>
        <translation>Vérifier le certificat du serveur (recommandé)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="469"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Liste de chiffrements TLS (facultatif) :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="475"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>p. ex. PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="476"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>Liste de chiffrements TLS-PSK séparés par des deux-points.
Laisser vide pour la détection automatique.
Exemple : PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="481"/>
        <source>Preset</source>
        <translation>Préréglage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="482"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Remplir avec les chiffrements PSK recommandés</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="493"/>
        <source>gRPC is always encrypted with TLS. The server certificate is confirmed on first use and pinned; the Director authenticates with console name and password. TLS-PSK and certificate settings do not apply.</source>
        <translation>gRPC est toujours chiffré avec TLS. Le certificat du serveur est confirmé lors de la première utilisation puis épinglé ; le Director s&apos;authentifie avec le nom de console et le mot de passe. Les paramètres TLS-PSK et de certificat ne s&apos;appliquent pas.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="504"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="509"/>
        <source>Save password in profile</source>
        <translation>Enregistrer le mot de passe dans le profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="513"/>
        <source>Auto-connect on startup</source>
        <translation>Connexion automatique au démarrage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="591"/>
        <source>Theme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="596"/>
        <source>System</source>
        <translation type="unfinished">Système</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="597"/>
        <source>Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="598"/>
        <source>Dark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="599"/>
        <source>&quot;System&quot; follows the light or dark colour scheme of the desktop.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="600"/>
        <source>Colour theme:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1173"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1189"/>
        <source>Reset Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1174"/>
        <source>Reset all settings to their default values now?

This takes effect at once, also if you then press Cancel, and cannot be undone.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1178"/>
        <source>Also delete all connection profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1190"/>
        <source>All settings and connection profiles have been reset.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1191"/>
        <source>All settings have been reset to their default values. The connection profiles were kept.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1236"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1241"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1246"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1255"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1258"/>
        <source>Export Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1237"/>
        <source>There are no connection profiles to export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1242"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1287"/>
        <source>JSON (*.json)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1247"/>
        <source>Include the stored passwords in the file?

The file then holds the password hashes, and anyone who has it can log in to the Directors with them. Without them, the passwords must be entered again after the import.</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bsettingsdialog.cpp" line="1259"/>
        <source>%n connection profile(s) exported.</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1268"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1278"/>
        <source>Cannot write %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bsettingsdialog.cpp" line="1298"/>
        <source>%n connection profile(s) imported. They are stored when you save the settings.</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1307"/>
        <source>Cannot read %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1362"/>
        <source>All connection profiles have been removed. They are deleted when you save the settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1582"/>
        <source>Do you really want to delete the connection profile &quot;%1&quot;?

It is deleted when you save the settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="759"/>
        <source> seconds</source>
        <translation> secondes</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="529"/>
        <source>Export All Profiles</source>
        <translation>Exporter tous les profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="530"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1286"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1294"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1297"/>
        <source>Import Profiles</source>
        <translation>Importer des profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="569"/>
        <source>Appearance</source>
        <translation>Apparence</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="575"/>
        <source>Text Size</source>
        <translation>Taille du texte</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="584"/>
        <source>The text follows the font of your desktop. This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0).</source>
        <translation>Le texte suit la police de votre bureau. Ce facteur l&apos;agrandit ou le réduit (Ctrl++, Ctrl+-, Ctrl+0).</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="586"/>
        <source>Interface zoom:</source>
        <translation>Zoom de l&apos;interface :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="604"/>
        <source>Language</source>
        <translation>Langue</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="612"/>
        <source>System (%1)</source>
        <translation>Système (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="624"/>
        <source>Application Language:</source>
        <translation>Langue de l&apos;application :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="627"/>
        <source>Language changes require an application restart. &quot;System&quot; follows the language of the operating system.</source>
        <translation>Les changements de langue nécessitent un redémarrage de l&apos;application. « Système » suit la langue du système d&apos;exploitation.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="637"/>
        <source>Status Bar Colors</source>
        <translation>Couleurs de la barre d&apos;état</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="646"/>
        <source>Color shown when connected to Director</source>
        <translation>Couleur affichée lorsque la connexion au Director est établie</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="648"/>
        <source>Connected:</source>
        <translation>Connecté :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="654"/>
        <source>Color shown when not connected</source>
        <translation>Couleur affichée en l&apos;absence de connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="656"/>
        <source>Disconnected:</source>
        <translation>Déconnecté :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="661"/>
        <source>Backup Level Colors</source>
        <translation>Couleurs des niveaux de sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="725"/>
        <source>Behavior</source>
        <translation>Comportement</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="731"/>
        <source>Confirmations</source>
        <translation>Confirmations</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="735"/>
        <source>Confirm job cancellation</source>
        <translation>Confirmer l&apos;annulation d&apos;un Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="740"/>
        <source>Confirm job start</source>
        <translation>Confirmer le démarrage d&apos;un Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="747"/>
        <source>Auto-Refresh</source>
        <translation>Actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="751"/>
        <source>Auto-refresh</source>
        <translation>Actualisation automatique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="755"/>
        <source>Interval:</source>
        <translation>Intervalle :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="768"/>
        <source>Display</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="776"/>
        <source>Maximum Jobs:</source>
        <translation>Nombre maximal de Jobs :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="778"/>
        <source>Show newest jobs first</source>
        <translation>Afficher les Jobs les plus récents en premier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="781"/>
        <source>When enabled, the job list shows the most recent jobs first.
When disabled, the oldest jobs are shown first.</source>
        <translation>Si activé, la liste des Jobs affiche les Jobs les plus récents en premier.
Si désactivé, les Jobs les plus anciens sont affichés en premier.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="788"/>
        <source> hours</source>
        <translation> heures</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="790"/>
        <source>The clients table marks a client as stale when its last backup or contact
is at least this old. The value is taken from the job history; it is not
a live connection check.</source>
        <translation>Le tableau des Clients marque un Client comme obsolète lorsque sa dernière sauvegarde ou son dernier contact
est au moins aussi ancien. La valeur est tirée de l&apos;historique des Jobs ; il ne s&apos;agit pas
d&apos;une vérification de connexion en direct.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="793"/>
        <source>Client backup is stale after:</source>
        <translation>Sauvegarde du Client obsolète après :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="798"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="805"/>
        <source> days</source>
        <translation> jours</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="807"/>
        <source>The schedule view measures how long jobs run and how long they wait
from the jobs of the last days in the catalog. A longer time gives more
runs per job and a larger request to the Director. The history is only
requested while the schedule view is open.</source>
        <translation>La vue des Schedules mesure la durée d&apos;exécution et d&apos;attente des Jobs
à partir des Jobs des derniers jours dans le catalogue. Une période plus longue donne plus
d&apos;exécutions par Job et une requête plus volumineuse au Director. L&apos;historique n&apos;est
demandé que lorsque la vue des Schedules est ouverte.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="811"/>
        <source>Measure job history over the last:</source>
        <translation>Mesurer l&apos;historique des Jobs sur les derniers :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="816"/>
        <source>Visible Backup Levels</source>
        <translation>Niveaux de sauvegarde visibles</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="821"/>
        <source>Select which backup levels appear in the Run Backup dialog:</source>
        <translation>Sélectionnez les niveaux de sauvegarde qui apparaissent dans la boîte de dialogue Lancer une sauvegarde :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="827"/>
        <source>Complete backup of all files</source>
        <translation>Sauvegarde complète de tous les fichiers</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="828"/>
        <source>Backup files changed since last backup</source>
        <translation>Sauvegarder les fichiers modifiés depuis la dernière sauvegarde</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="829"/>
        <source>Backup files changed since last Full backup</source>
        <translation>Sauvegarder les fichiers modifiés depuis la dernière sauvegarde Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="830"/>
        <source>Consolidate incremental backups into a synthetic full</source>
        <translation>Consolider les sauvegardes Incremental en une Full synthétique</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="831"/>
        <source>Data-level backup (rarely used)</source>
        <translation>Sauvegarde au niveau des données (rarement utilisée)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="879"/>
        <source>Advanced Settings</source>
        <translation>Paramètres avancés</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="886"/>
        <source>File Logging</source>
        <translation>Journalisation dans un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="890"/>
        <source>Enable file logging</source>
        <translation>Activer la journalisation dans un fichier</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="891"/>
        <source>Write debug information to a log file</source>
        <translation>Écrire les informations de débogage dans un fichier journal</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="895"/>
        <source>Log File:</source>
        <translation>Fichier journal :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="905"/>
        <source>Max. Log Size:</source>
        <translation>Taille max. du journal :</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="929"/>
        <source>Miscellaneous</source>
        <translation>Divers</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="934"/>
        <source>Show tooltips</source>
        <translation>Afficher les infobulles</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="942"/>
        <source>⚠️ These settings are for advanced users.
Changes may affect application stability.</source>
        <translation>⚠️ Ces paramètres sont destinés aux utilisateurs avancés.
Les modifications peuvent affecter la stabilité de l&apos;application.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1150"/>
        <source>Settings have been saved.
Some changes require an application restart.</source>
        <translation>Les paramètres ont été enregistrés.
Certaines modifications nécessitent un redémarrage de l&apos;application.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1203"/>
        <source>Select CA Certificate</source>
        <translation>Sélectionner le certificat CA</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1204"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1216"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cert);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1212"/>
        <source>Select Client Certificate</source>
        <translation>Sélectionner le certificat client</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1214"/>
        <source>Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)</source>
        <translation>Certificats (*.pfx *.pem *.crt *.cert *.der);;Fichiers PFX (*.pfx);;Fichiers PEM (*.pem);;Fichiers DER (*.der);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1225"/>
        <source>Select Private Key</source>
        <translation>Sélectionner la clé privée</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1226"/>
        <source>Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1336"/>
        <source>No Profiles</source>
        <translation>Aucun profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1337"/>
        <source>There are no connection profiles to delete.</source>
        <translation>Il n&apos;y a aucun profil de connexion à supprimer.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1341"/>
        <source>Clear All Profiles</source>
        <translation>Effacer tous les profils</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1342"/>
        <source>Do you really want to delete all %1 connection profile(s)?

This includes all passwords and certificate paths.
This action cannot be undone.</source>
        <translation>Voulez-vous vraiment supprimer les %1 profil(s) de connexion ?

Cela inclut tous les mots de passe et chemins de certificats.
Cette action est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1361"/>
        <source>Deleted</source>
        <translation>Supprimé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1386"/>
        <source>Choose Color for Full Backup</source>
        <translation>Choisir la couleur des sauvegardes Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1391"/>
        <source>Choose Color for Incremental Backup</source>
        <translation>Choisir la couleur des sauvegardes Incremental</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1396"/>
        <source>Choose Color for Differential Backup</source>
        <translation>Choisir la couleur des sauvegardes Differential</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1401"/>
        <source>Choose Color for Virtual Full Backup</source>
        <translation>Choisir la couleur des sauvegardes VirtualFull</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1407"/>
        <source>Choose Color for Connected Status</source>
        <translation>Choisir la couleur de l&apos;état connecté</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1417"/>
        <source>Choose Color for Disconnected Status</source>
        <translation>Choisir la couleur de l&apos;état déconnecté</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1477"/>
        <source>••••••••  (password saved)</source>
        <translation>••••••••  (mot de passe enregistré)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1483"/>
        <source>Enter password</source>
        <translation>Saisir le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1552"/>
        <source>New Connection</source>
        <translation>Nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1581"/>
        <source>Delete Profile</source>
        <translation>Supprimer le profil</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1612"/>
        <source>%1 (Copy)</source>
        <translation>%1 (copie)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1627"/>
        <source>Unnamed Connection</source>
        <translation>Connexion sans nom</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1699"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(non épinglé - confirmé à la prochaine connexion)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1704"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256 : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1719"/>
        <source>PFX File Created</source>
        <translation>Fichier PFX créé</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1720"/>
        <source>PFX file has been created successfully.

The certificate path has been updated in the current profile.</source>
        <translation>Le fichier PFX a été créé avec succès.

Le chemin du certificat a été mis à jour dans le profil actuel.</translation>
    </message>
</context>
<context>
    <name>BStaggerConstraintsDialog</name>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="21"/>
        <source>Constraints for the proposal</source>
        <translation>Contraintes pour la proposition</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="24"/>
        <source>The proposal only starts jobs later. A fixed job is never moved, and a job of a higher priority is then not moved past it either. A job with a latest start does not start after that time. The constraints are kept on this computer, for this Director.</source>
        <translation>La proposition ne fait que démarrer des jobs plus tard. Un job fixé n&apos;est jamais déplacé, et un job de priorité plus haute n&apos;est alors pas non plus déplacé au-delà de lui. Un job avec un début au plus tard ne démarre pas après cette heure. Les contraintes sont conservées sur cet ordinateur, pour ce Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="36"/>
        <source>&lt;b&gt;Job&lt;/b&gt;</source>
        <translation>&lt;b&gt;Job&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="37"/>
        <source>&lt;b&gt;Fixed&lt;/b&gt;</source>
        <translation>&lt;b&gt;Fixé&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="38"/>
        <source>&lt;b&gt;Latest start&lt;/b&gt;</source>
        <translation>&lt;b&gt;Début au plus tard&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="41"/>
        <source>No job waits on the dates on display.</source>
        <translation>Aucun job n&apos;attend aux dates affichées.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="51"/>
        <source>%1 is fixed</source>
        <translation>%1 est fixé</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="53"/>
        <source>not after</source>
        <translation>pas après</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="55"/>
        <source>%1 has a latest start</source>
        <translation>%1 a un début au plus tard</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="60"/>
        <source>Latest start of %1</source>
        <translation>Début au plus tard de %1</translation>
    </message>
</context>
<context>
    <name>BStaggerPlan</name>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="37"/>
        <source>Schedule %1 has no Run lines.</source>
        <translation>Le Schedule %1 n&apos;a pas de lignes Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="41"/>
        <source>Run line %1 of schedule %2 is not understood.</source>
        <translation>La ligne Run %1 du Schedule %2 n&apos;est pas comprise.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="44"/>
        <source>Run line %1 of schedule %2 runs at several hours.</source>
        <translation>La ligne Run %1 du Schedule %2 s&apos;exécute à plusieurs heures.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="47"/>
        <source>Run line %1 of schedule %2 would move into the next day.</source>
        <translation>La ligne Run %1 du Schedule %2 passerait au jour suivant.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="57"/>
        <source>Run line %1 of schedule %2 did not read back as written (%3).</source>
        <translation>La ligne Run %1 du Schedule %2 ne s&apos;est pas relue comme écrite (%3).</translation>
    </message>
</context>
<context>
    <name>BStatus</name>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32"/>
        <source>Terminated normally</source>
        <translation>Terminé normalement</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Warnings</source>
        <translation>Avertissements</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Terminated with warnings</source>
        <translation>Terminé avec des avertissements</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Terminated in error</source>
        <translation>Terminé en erreur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Errors</source>
        <translation>Erreurs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Non-fatal error</source>
        <translation>Erreur non fatale</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Fatal</source>
        <translation>Fatal</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Fatal error</source>
        <translation>Erreur fatale</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Canceled</source>
        <translation>Annulé</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Canceled by user</source>
        <translation>Annulé par l&apos;utilisateur</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38"/>
        <source>Running</source>
        <translation>En cours</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Committing</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Committing data (last despool)</source>
        <translation>Validation des données (dernier despool)</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Despooling</source>
        <translation>Despool</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Despooling data</source>
        <translation>Despool des données</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Created</source>
        <translation>Créé</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Created, not yet running</source>
        <translation>Créé, pas encore démarré</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Wait FD</source>
        <translation>Attente FD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Waiting on the File daemon</source>
        <translation>En attente du File Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Wait SD</source>
        <translation>Attente SD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Waiting on the Storage daemon</source>
        <translation>En attente du Storage Daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Wait media</source>
        <translation>Attente média</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Waiting for new media</source>
        <translation>En attente d&apos;un nouveau média</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Wait mount</source>
        <translation>Attente montage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Waiting for mount</source>
        <translation>En attente de montage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Wait storage</source>
        <translation>Attente Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Waiting for a storage resource</source>
        <translation>En attente d&apos;une ressource Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Wait job</source>
        <translation>Attente Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Waiting for a job resource</source>
        <translation>En attente d&apos;une ressource Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48"/>
        <source>Wait client</source>
        <translation>Attente Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48"/>
        <source>Waiting for a client resource</source>
        <translation>En attente d&apos;une ressource Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Wait max jobs</source>
        <translation>Attente max. Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Waiting for maximum jobs</source>
        <translation>En attente du nombre maximal de Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Wait start</source>
        <translation>Attente début</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Waiting for the start time</source>
        <translation>En attente de l&apos;heure de début</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="51"/>
        <source>Wait priority</source>
        <translation>Attente priorité</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="51"/>
        <source>Waiting for higher priority jobs to finish</source>
        <translation>En attente de la fin des Jobs de priorité supérieure</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="52"/>
        <source>Wait device</source>
        <translation>Attente périphérique</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="52"/>
        <source>Queued, waiting for a device</source>
        <translation>En file d&apos;attente, en attente d&apos;un périphérique</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="53"/>
        <source>Despool attr.</source>
        <translation>Despool attr.</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="53"/>
        <source>Storage daemon despooling attributes</source>
        <translation>Storage Daemon en cours de despool des attributs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="54"/>
        <source>Inserting</source>
        <translation>Insertion</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="54"/>
        <source>Doing batch insert of file records</source>
        <translation>Insertion par lots des enregistrements de fichiers</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="55"/>
        <source>Blocked</source>
        <translation>Bloqué</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="56"/>
        <source>Differences</source>
        <translation>Différences</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="56"/>
        <source>Verify found differences</source>
        <translation>Verify a trouvé des différences</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="57"/>
        <source>Incomplete</source>
        <translation>Incomplet</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="57"/>
        <source>Incomplete job</source>
        <translation>Job incomplet</translation>
    </message>
</context>
<context>
    <name>BStorageResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Address</source>
        <translation>Adresse</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Port</source>
        <translation>Port</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Device</source>
        <translation>Device</translation>
    </message>
</context>
<context>
    <name>BTableModel</name>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="218"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="218"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>BThirdPartyNoticesDialog</name>
    <message>
        <location filename="../src/bthirdpartynotices.cpp" line="61"/>
        <source>Third-party licences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bthirdpartynotices.cpp" line="67"/>
        <source>Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bthirdpartynotices.cpp" line="71"/>
        <source>Third-party notices</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bthirdpartynotices.cpp" line="72"/>
        <source>Onesimus licence (AGPL-3.0)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bthirdpartynotices.cpp" line="95"/>
        <source>The third-party notices are missing from this build.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BUsageStatus</name>
    <message>
        <location filename="../src/busagestatus.cpp" line="76"/>
        <source>Usage: %1 of %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="77"/>
        <source>Usage: %1 of %2 (%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="79"/>
        <source>Usage: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="79"/>
        <source>Usage: %1 (%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="116"/>
        <source>Licence: free tier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="119"/>
        <source>Licence: licensed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="120"/>
        <source>Licence: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="123"/>
        <source>Licence: licensed, valid until %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="124"/>
        <source>Licence: %1, valid until %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="130"/>
        <source>Licence: expired, the free tier applies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="133"/>
        <source>Licence: not valid, the free tier applies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="142"/>
        <source>Limit: %1 per month</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="143"/>
        <source>Limit: none</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="146"/>
        <source>Written by finished jobs: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="147"/>
        <source>Written by running jobs so far: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="149"/>
        <source>Used: %1 % of the limit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="153"/>
        <source>Projected by the end of the month: %1, above the limit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="154"/>
        <source>Projected by the end of the month: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="156"/>
        <source>Backup jobs held this month: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="158"/>
        <source>Resets on %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="163"/>
        <source>Limit reached: new backup jobs are held until the reset. Restores are not affected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="166"/>
        <source>Close to the limit: at the limit new backup jobs are held.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="215"/>
        <source>Licence not valid: free tier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/busagestatus.cpp" line="220"/>
        <source>Licence expired: free tier</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/busagestatus.cpp" line="225"/>
        <source>Licence ends in %n day(s), on %1</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BVolumePurge</name>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="77"/>
        <source>The Director refused the command without a message.</source>
        <translation>Le Director a refusé la commande sans message.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="108"/>
        <source>Choose either one pool or all pools, not both.</source>
        <translation>Choisissez soit un pool, soit tous les pools, pas les deux.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="112"/>
        <source>Choose a pool or all pools.</source>
        <translation>Choisissez un pool ou tous les pools.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="118"/>
        <source>The pool name contains characters that cannot be sent on a command line.</source>
        <translation>Le nom du pool contient des caractères qui ne peuvent pas être envoyés sur une ligne de commande.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="124"/>
        <source>Choose at least one volume status.</source>
        <translation>Choisissez au moins un statut de volume.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="129"/>
        <source>Unknown volume status.</source>
        <translation>Statut de volume inconnu.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="165"/>
        <source>The Director sent an empty answer.</source>
        <translation>Le Director a envoyé une réponse vide.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="167"/>
        <source>The Director did not answer with a purge result: %1</source>
        <translation>Le Director n&apos;a pas répondu par un résultat de purge : %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="182"/>
        <source>The Director&apos;s answer has no purge result: %1</source>
        <translation>La réponse du Director ne contient aucun résultat de purge : %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="231"/>
        <source>The Director&apos;s purge result names no mode.</source>
        <translation>Le résultat de purge du Director n&apos;indique aucun mode.</translation>
    </message>
</context>
<context>
    <name>BVolumePurgePanel</name>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="30"/>
        <source>Purge volumes</source>
        <translation>Purger les volumes</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="36"/>
        <source>Purge removes all job and file records of the chosen volumes from the catalog, whatever their retention periods say. Click &apos;Preview&apos; to see what the Director would purge.</source>
        <translation>Purge supprime du catalogue tous les enregistrements de jobs et de fichiers des volumes choisis, quelles que soient leurs durées de rétention. Cliquez sur « Aperçu » pour voir ce que le Director purgerait.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="43"/>
        <source>Pool:</source>
        <translation>Pool :</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="46"/>
        <source>All pools</source>
        <translation>Tous les pools</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="52"/>
        <source>Volume status:</source>
        <translation>Statut du volume :</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="69"/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="69"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="69"/>
        <source>Status</source>
        <translation>Statut</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="69"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Files</source>
        <translation>Fichiers</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Result</source>
        <translation>Résultat</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="84"/>
        <source>Preview</source>
        <translation>Aperçu</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="86"/>
        <source>Ask the Director which volumes it would purge; nothing is changed</source>
        <translation>Demande au Director quels volumes il purgerait ; rien n&apos;est modifié</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="89"/>
        <source>Purge</source>
        <translation>Purger</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="91"/>
        <source>Purge the volumes of the preview (requires a preview first)</source>
        <translation>Purge les volumes de l&apos;aperçu (nécessite d&apos;abord un aperçu)</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="172"/>
        <source>The selection was changed. Click &apos;Preview&apos; again.</source>
        <translation>La sélection a été modifiée. Cliquez à nouveau sur « Aperçu ».</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="208"/>
        <source>all pools</source>
        <translation>tous les pools</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="209"/>
        <source>pool &apos;%1&apos;</source>
        <translation>pool « %1 »</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bvolumepurgepanel.cpp" line="210"/>
        <source>Purge %n volume(s) in %1 with the status %2?</source>
        <translation>
            <numerusform>Purger %n volume dans %1 avec le statut %2 ?</numerusform>
            <numerusform>Purger %n volumes dans %1 avec le statut %2 ?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="213"/>
        <source>Purge ignores the retention periods. All job and file records on these volumes are removed from the catalog, and their backups can no longer be restored through the catalog. This cannot be undone.</source>
        <translation>Purge ignore les durées de rétention. Tous les enregistrements de jobs et de fichiers de ces volumes sont supprimés du catalogue, et leurs sauvegardes ne peuvent plus être restaurées via le catalogue. Cette opération est irréversible.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="217"/>
        <source>The Director selects the volumes again when it purges; volumes that changed since the preview are purged or skipped according to their state then.</source>
        <translation>Le Director sélectionne à nouveau les volumes lors de la purge ; les volumes modifiés depuis l&apos;aperçu sont purgés ou ignorés selon leur état à ce moment-là.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="220"/>
        <source>Command: %1</source>
        <translation>Commande : %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="222"/>
        <source>Confirm Purge</source>
        <translation>Confirmer la purge</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="238"/>
        <source>Not connected to a Director.</source>
        <translation>Non connecté à un Director.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="244"/>
        <source>Asking the Director for a preview...</source>
        <translation>Demande d&apos;un aperçu au Director...</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="245"/>
        <source>The Director is purging...</source>
        <translation>Le Director purge...</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="274"/>
        <source>The Director answered in another mode than the one requested.</source>
        <translation>Le Director a répondu dans un autre mode que celui demandé.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="282"/>
        <location filename="../src/bvolumepurgepanel.cpp" line="320"/>
        <source>Preview failed: %1</source>
        <translation>Échec de l&apos;aperçu : %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="283"/>
        <source>Purge failed: %1</source>
        <translation>Échec de la purge : %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="294"/>
        <source>Preview: %1 volumes selected, %2 would be purged, %3 skipped. Click &apos;Purge&apos; to purge them.</source>
        <translation>Aperçu : %1 volumes sélectionnés, %2 seraient purgés, %3 ignorés. Cliquez sur « Purger » pour les purger.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="299"/>
        <source>Preview: %1 volumes selected, none would be purged, %2 skipped.</source>
        <translation>Aperçu : %1 volumes sélectionnés, aucun ne serait purgé, %2 ignorés.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="305"/>
        <source>Purge finished: %1 volumes selected, %2 purged, %3 skipped.</source>
        <translation>Purge terminée : %1 volumes sélectionnés, %2 purgés, %3 ignorés.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="321"/>
        <source>The result of the purge is unknown: %1. Run a preview to see the current state.</source>
        <translation>Le résultat de la purge est inconnu : %1. Lancez un aperçu pour voir l&apos;état actuel.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="329"/>
        <source>no answer from the Director within the time limit</source>
        <translation>aucune réponse du Director dans le délai imparti</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="347"/>
        <source>Would purge</source>
        <translation>Serait purgé</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="350"/>
        <source>Purged</source>
        <translation>Purgé</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="353"/>
        <source>Skipped</source>
        <translation>Ignoré</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="354"/>
        <source>Skipped: %1</source>
        <translation>Ignoré : %1</translation>
    </message>
</context>
<context>
    <name>BWaitCalibration</name>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="197"/>
        <source>this job was not measured: median of all %1 runs on this Director</source>
        <translation>ce Job n&apos;a pas été mesuré : médiane de toutes les %1 exécutions sur ce Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="202"/>
        <source>nothing measured: %1 assumed</source>
        <translation>rien de mesuré : %1 supposé</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="210"/>
        <source>Duration %1: median of %2 measured run(s). It includes any wait for a device inside the storage daemon (the parallel writers of %3 are not known, so it is not taken out).</source>
        <translation>Durée %1 : médiane de %2 exécution(s) mesurée(s). Elle inclut toute attente d&apos;un périphérique dans le Storage Daemon (les écritures parallèles de %3 ne sont pas connues, elle n&apos;est donc pas retranchée).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="214"/>
        <source>its storage</source>
        <translation>son Storage</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="193"/>
        <source>median of %n measured run(s) at this level</source>
        <translation>
            <numerusform>médiane de %n exécution mesurée à ce niveau</numerusform>
            <numerusform>médiane de %n exécutions mesurées à ce niveau</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="195"/>
        <source>median of %n measured run(s) of this job at other levels</source>
        <translation>
            <numerusform>médiane de %n exécution mesurée de ce Job à d&apos;autres niveaux</numerusform>
            <numerusform>médiane de %n exécutions mesurées de ce Job à d&apos;autres niveaux</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="217"/>
        <source>Duration %1: median of %n measured run(s); they did not wait for the device.</source>
        <translation>
            <numerusform>Durée %1 : médiane de %n exécution mesurée ; elle n&apos;a pas attendu le périphérique.</numerusform>
            <numerusform>Durée %1 : médiane de %n exécutions mesurées ; elles n&apos;ont pas attendu le périphérique.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="221"/>
        <source>Duration %1 without the wait for the device (measured %2, median of %3 run(s)): counted from the end of the run that held the device before it (%4 parallel writer(s)). It may still hold up to 30 s of the storage daemon&apos;s tries.</source>
        <translation>Durée %1 sans l&apos;attente du périphérique (mesurée %2, médiane de %3 exécution(s)) : comptée à partir de la fin de l&apos;exécution qui occupait le périphérique avant elle (%4 écriture(s) parallèle(s)). Elle peut encore contenir jusqu&apos;à 30 s de tentatives du Storage Daemon.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="317"/>
        <source>%1 ended %2 after %3 on %4; alone it takes at least %5</source>
        <translation>%1 s&apos;est terminé %2 après %3 le %4 ; seul, il dure au moins %5</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="334"/>
        <source>%1 night(s) with runs on storage %2; on %3 more than one of them was queued or running at the same time; on %4 the runs&apos; recorded start to end overlapped.</source>
        <translation>%1 nuit(s) avec des exécutions sur le Storage %2 ; pendant %3, plus d&apos;une d&apos;entre elles était en file d&apos;attente ou en cours en même temps ; pendant %4, le début et la fin enregistrés des exécutions se chevauchaient.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="341"/>
        <source>No two runs ever overlapped although several were queued: the storage wrote one job at a time.</source>
        <translation>Aucune paire d&apos;exécutions ne s&apos;est jamais chevauchée bien que plusieurs aient été en file d&apos;attente : le Storage a écrit un Job à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="347"/>
        <source>Overlapping runs do not prove parallel writing: with Just In Time Reservation a job waits for the device inside its run.</source>
        <translation>Des exécutions qui se chevauchent ne prouvent pas une écriture parallèle : avec Just In Time Reservation, un Job attend le périphérique au sein de son exécution.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="351"/>
        <source>%1 of %2 telling pair(s) ended sooner after each other than one writer allows (e.g. %3): it writes more than one job at a time.</source>
        <translation>%1 sur %2 paire(s) significative(s) se sont terminées plus rapprochées qu&apos;une seule écriture ne le permet (p. ex. %3) : il écrit plus d&apos;un Job à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="358"/>
        <source>In all %1 telling pair(s) on %2 night(s) the later run went on after the other ended for at least as long as it takes alone (e.g. %3): consistent with one writer at a time.</source>
        <translation>Dans toutes les %1 paire(s) significative(s) sur %2 nuit(s), l&apos;exécution ultérieure a continué après la fin de l&apos;autre au moins aussi longtemps qu&apos;elle dure seule (p. ex. %3) : compatible avec une écriture à la fois.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="365"/>
        <source>Not enough telling pairs (%1; a pair tells when the later run started well before the other ended and was also seen running alone): no suggestion.</source>
        <translation>Pas assez de paires significatives (%1 ; une paire est significative lorsque l&apos;exécution ultérieure a démarré bien avant la fin de l&apos;autre et a aussi été vue s&apos;exécutant seule) : aucune suggestion.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453"/>
        <source>Back-test: no measured night to replay.</source>
        <translation>Test rétrospectif : aucune nuit mesurée à rejouer.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="454"/>
        <source>Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 (%5, %6; %7).</source>
        <translation>Test rétrospectif, %1 dernière(s) nuit(s) (%2 exécutions) : erreur médiane %3, pire %4 (%5, %6 ; %7).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="460"/>
        <source>predicted too late</source>
        <translation>prévu trop tard</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="460"/>
        <source>predicted too early</source>
        <translation>prévu trop tôt</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="462"/>
        <source>Replayed without a device limit for %1 (not set).</source>
        <translation>Rejoué sans limite de périphérique pour %1 (non définie).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="465"/>
        <source>Durations and overheads come from the same history.</source>
        <translation>Les durées et les surcoûts proviennent du même historique.</translation>
    </message>
</context>
<context>
    <name>BZipImport</name>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="53"/>
        <location filename="../src/config/bzipimport.cpp" line="65"/>
        <source>The file is not a ZIP archive.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="72"/>
        <source>ZIP64 archives are not read.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="76"/>
        <source>The archive has %1 entries; at most %2 are read.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="80"/>
        <location filename="../src/config/bzipimport.cpp" line="86"/>
        <location filename="../src/config/bzipimport.cpp" line="100"/>
        <source>The ZIP archive is damaged.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="141"/>
        <source>Cannot open %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="145"/>
        <source>The archive is larger than %1 MiB.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="157"/>
        <source>Cannot read the ZIP archive %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="172"/>
        <source>the path leaves the archive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="174"/>
        <source>a symbolic link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="176"/>
        <source>encrypted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="178"/>
        <source>compression method %1 is not read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="180"/>
        <location filename="../src/config/bzipimport.cpp" line="214"/>
        <source>the size does not match the archive directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="182"/>
        <source>larger than %1 KiB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="184"/>
        <source>the name cannot be read</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bzipimport.cpp" line="206"/>
        <source>The configuration files are larger than %1 MiB together.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BareosAuth</name>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="161"/>
        <source>Starting Bareos authentication...</source>
        <translation>Démarrage de l&apos;authentification Bareos...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="168"/>
        <source>Setting up TLS-PSK...</source>
        <translation>Configuration de TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="187"/>
        <source>Starting TLS-PSK handshake...</source>
        <translation>Démarrage de la négociation TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="194"/>
        <source>Setting up certificate-based TLS...</source>
        <translation>Configuration de TLS par certificats...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="210"/>
        <source>Starting TLS handshake...</source>
        <translation>Démarrage de la négociation TLS...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="218"/>
        <source>Starting legacy authentication (no TLS)...</source>
        <translation>Démarrage de l&apos;authentification héritée (sans TLS)...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="654"/>
        <source>TLS established, sending Hello packet...</source>
        <translation>TLS établi, envoi du paquet Hello...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="678"/>
        <source>Waiting for Director challenge...</source>
        <translation>En attente du challenge du Director...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="844"/>
        <source>Director rejected connection: %1</source>
        <translation>Le Director a rejeté la connexion : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="851"/>
        <source>

Hint: The Director requires TLS encryption. Please use PSK or Certificate authentication instead of Legacy mode.</source>
        <translation>

Conseil : le Director exige le chiffrement TLS. Veuillez utiliser l&apos;authentification PSK ou par certificat au lieu du mode Legacy.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="857"/>
        <source>Director sent empty response.

This usually means the Director requires TLS encryption but Legacy (unencrypted) mode was used.
Please try PSK or Certificate authentication.</source>
        <translation>Le Director a envoyé une réponse vide.

Cela signifie généralement que le Director exige le chiffrement TLS mais que le mode Legacy (non chiffré) a été utilisé.
Veuillez essayer l&apos;authentification PSK ou par certificat.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="865"/>
        <source>Unexpected response from Director:
%1</source>
        <translation>Réponse inattendue du Director :
%1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="952"/>
        <source>CRAM-MD5 response sent successfully</source>
        <translation>Réponse CRAM-MD5 envoyée avec succès</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1351"/>
        <source>The Director announced a message of %1 bytes during authentication (limit: %2 bytes). Connection closed.</source>
        <translation>Le Director a annoncé un message de %1 octets pendant l&apos;authentification (limite : %2 octets). Connexion fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1567"/>
        <source>Connected</source>
        <translation>Connecté</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1748"/>
        <source>TLS-PSK authentication failed: %1

This usually means:
• Wrong password
• Wrong console name
• Console not configured on Director
• TLS-PSK not enabled on Director</source>
        <translation>Échec de l&apos;authentification TLS-PSK : %1

Cela signifie généralement :
• Mot de passe incorrect
• Nom de console incorrect
• Console non configurée sur le Director
• TLS-PSK non activé sur le Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1757"/>
        <source>TLS handshake failed: %1

Possible causes:
• No compatible cipher suites
• TLS version mismatch
• Director doesn&apos;t support PSK</source>
        <translation>Échec de la négociation TLS : %1

Causes possibles :
• Aucune suite de chiffrement compatible
• Versions TLS incompatibles
• Le Director ne prend pas en charge PSK</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1765"/>
        <source>Socket error: %1</source>
        <translation>Erreur de socket : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="153"/>
        <source>TLS is required for this connection, but neither TLS-PSK nor certificate TLS is enabled in the profile. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="286"/>
        <source>Certificate verification is on, but no CA certificate file is set in the profile. Set the CA certificate file to the CA that signed the Director&apos;s certificate. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="298"/>
        <source>The CA certificate file &quot;%1&quot; cannot be read or contains no certificate. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="329"/>
        <source>The client certificate file &quot;%1&quot; cannot be read (wrong password, or not a PKCS#12 file). Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="343"/>
        <source>The client certificate file &quot;%1&quot; cannot be read or contains no certificate. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="364"/>
        <source>The private key file &quot;%1&quot; cannot be read (an unencrypted RSA or EC key in PEM format is needed). Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="547"/>
        <source>The TLS library of this system offers no TLS-PSK cipher suite, so TLS-PSK cannot be used. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="731"/>
        <source>The Director&apos;s certificate was not accepted: %1. It must be signed by the CA of the profile (&quot;%2&quot;). Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="762"/>
        <source>The common name of the Director&apos;s certificate (&quot;%1&quot;) is not in the TLS Allowed CN list of the profile (%2). Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="784"/>
        <source>The Director&apos;s certificate is not issued for the host name &quot;%1&quot; (the certificate names: %2). Connect with a name the certificate contains, or add its common name to TLS Allowed CN in the profile. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="787"/>
        <source>none</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="896"/>
        <source>The server sent this console&apos;s own challenge back instead of a challenge of the Director (reflection attack). It is not the configured Director. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="909"/>
        <source>TLS is required for this connection, but the session with the Director is not encrypted. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="913"/>
        <source>The Director requires TLS, but this connection is not encrypted. Use TLS-PSK or certificate TLS in the profile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="944"/>
        <source>Failed to send the CRAM-MD5 response to the Director.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1484"/>
        <source>Failed to send the CRAM-MD5 challenge to the Director.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1487"/>
        <source>The Director rejected the password of console &quot;%1&quot; (wrong password or console name). Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1490"/>
        <source>The server did not complete the CRAM-MD5 handshake (it answered &quot;%1&quot; instead of &quot;1000 OK auth&quot;). It may not be a Bareos or Onesimus Director. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1514"/>
        <source>The Director could not prove that it knows the console password: its answer to the CRAM-MD5 challenge is wrong. Either the server is not the configured Director, or the password stored on the Director differs from this profile. Connection refused.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1528"/>
        <source>Failed to send &quot;1000 OK auth&quot; to the Director.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BareosDirector</name>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="206"/>
        <source>Changing the configuration needs the gRPC interface of the Onesimus Director</source>
        <translation>La modification de la configuration nécessite l&apos;interface gRPC de l&apos;Onesimus Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="219"/>
        <source>Checking a client needs the gRPC interface of the Onesimus Director</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="273"/>
        <source>Connection to the Director was closed before the reply arrived</source>
        <translation>La connexion au Director a été fermée avant l&apos;arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="330"/>
        <source>Disconnected from the Director before the reply arrived</source>
        <translation>Déconnecté du Director avant l&apos;arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="525"/>
        <source>No response from Director within %1 seconds</source>
        <translation>Aucune réponse du Director en %1 secondes</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="535"/>
        <source>Timeout while loading resources: %1</source>
        <translation>Délai dépassé lors du chargement des ressources : %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="979"/>
        <source>Connection to the Director was lost before the reply arrived</source>
        <translation>La connexion au Director a été perdue avant l&apos;arrivée de la réponse</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1130"/>
        <source>The Director did not confirm the API mode within %1 seconds. The connection was closed.</source>
        <translation>Le Director n&apos;a pas confirmé le mode API en %1 secondes. La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1192"/>
        <source>The Director announced a message of %1 bytes (limit: %2 MiB). The connection was closed.</source>
        <translation>Le Director a annoncé un message de %1 octets (limite : %2 MiB). La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1601"/>
        <source>TLS is enabled for this connection, but the session with the Director is not encrypted. The connection was closed.</source>
        <translation>TLS est activé pour cette connexion, mais la session avec le Director n&apos;est pas chiffrée. La connexion a été fermée.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1862"/>
        <source>Unknown error</source>
        <translation>Erreur inconnue</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1904"/>
        <source>Not sent: a value contains a control character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1924"/>
        <location filename="../src/director/bareosdirector.cpp" line="2453"/>
        <source>Not ready</source>
        <translation>Pas prêt</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1975"/>
        <source>Error sending command</source>
        <translation>Erreur lors de l&apos;envoi de la commande</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2029"/>
        <source>The command was not sent: no ready connection to the Director</source>
        <translation>La commande n&apos;a pas été envoyée : aucune connexion prête au Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2442"/>
        <source>This build of Onesimus has no gRPC support</source>
        <translation>Cette version d&apos;Onesimus ne prend pas en charge gRPC</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2537"/>
        <source>Server certificate not trusted</source>
        <translation>Certificat du serveur non approuvé</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2553"/>
        <source>Command failed</source>
        <translation>Échec de la commande</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2586"/>
        <source>Connection to Director lost: %1</source>
        <translation>Connexion au Director perdue : %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/bareosdirector.cpp" line="1058"/>
        <source>The Director did not answer within %n second(s)</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1994"/>
        <source>The command was not sent: one of its values cannot be written in a command line</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommandLine</name>
    <message>
        <location filename="../src/main.cpp" line="417"/>
        <source>--tls needs --host. Without --host the TLS settings of the saved connection profile are used.</source>
        <translation>--tls nécessite --host. Sans --host, les paramètres TLS du profil de connexion enregistré sont utilisés.</translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="423"/>
        <source>--tls needs the console password: use --password-file, the environment variable ONESIMUS_PASSWORD or --password.</source>
        <translation>--tls nécessite le mot de passe de la console : utilisez --password-file, la variable d&apos;environnement ONESIMUS_PASSWORD ou --password.</translation>
    </message>
</context>
<context>
    <name>ConfigPreviewPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1944"/>
        <source>Server Configuration Preview</source>
        <translation>Aperçu de la configuration du serveur</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1946"/>
        <source>Copy this configuration to your Bareos Director</source>
        <translation>Copiez cette configuration sur votre Director Bareos</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1956"/>
        <source>Console resource for the Director:</source>
        <translation>Ressource Console pour le Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1960"/>
        <source>Path: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</source>
        <translation>Chemin : /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1972"/>
        <location filename="../src/bconnectionwizard.cpp" line="2013"/>
        <location filename="../src/bconnectionwizard.cpp" line="2076"/>
        <location filename="../src/bconnectionwizard.cpp" line="2353"/>
        <location filename="../src/bconnectionwizard.cpp" line="2392"/>
        <location filename="../src/bconnectionwizard.cpp" line="2403"/>
        <source>Copy to Clipboard</source>
        <translation>Copier dans le presse-papiers</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1977"/>
        <location filename="../src/bconnectionwizard.cpp" line="2018"/>
        <location filename="../src/bconnectionwizard.cpp" line="2081"/>
        <location filename="../src/bconnectionwizard.cpp" line="2381"/>
        <location filename="../src/bconnectionwizard.cpp" line="2430"/>
        <location filename="../src/bconnectionwizard.cpp" line="2507"/>
        <source>Save .conf</source>
        <translation>Enregistrer .conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1982"/>
        <location filename="../src/bconnectionwizard.cpp" line="2023"/>
        <location filename="../src/bconnectionwizard.cpp" line="2086"/>
        <location filename="../src/bconnectionwizard.cpp" line="2481"/>
        <location filename="../src/bconnectionwizard.cpp" line="2546"/>
        <source>Save ZIP</source>
        <translation>Enregistrer ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1990"/>
        <source>Console Config</source>
        <translation>Config. Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1997"/>
        <source>Director resource TLS settings (if needed):</source>
        <translation>Paramètres TLS de la ressource Director (si nécessaire) :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2001"/>
        <source>Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Chemin : /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2031"/>
        <source>Director TLS</source>
        <translation>TLS du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2042"/>
        <source>Configuration for command-line consoles on the client:</source>
        <translation>Configuration des consoles en ligne de commande sur le client :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2046"/>
        <source>Path: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf or oconsole -c &amp;lt;file&amp;gt; (oconsole)</source>
        <translation>Chemin : /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf ou oconsole -c &amp;lt;file&amp;gt; (oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2054"/>
        <source>Contains the console password: restrict the file to mode 0640 root:bareos (or 0600 owned by the user running the console).</source>
        <translation>Contient le mot de passe de la console : restreignez le fichier au mode 0640 root:bareos (ou 0600 appartenant à l&apos;utilisateur qui exécute la console).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2094"/>
        <source>Console client (bconsole / oconsole)</source>
        <translation>Client console (bconsole / oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2098"/>
        <location filename="../src/bconnectionwizard.cpp" line="2135"/>
        <source>After copying, paste this into your Bareos Director config and restart bareos-dir.</source>
        <translation>Après la copie, collez ceci dans la configuration de votre Director Bareos et redémarrez bareos-dir.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2123"/>
        <source>Add the Console resource, then reload the Director. The Onesimus Director offers gRPC on the BNET port + 1000; nothing else to configure.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2129"/>
        <source>Add the Console resource and &quot;Grpc Port = %1&quot; to the Director resource, then restart the Director (the default would be %2).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2302"/>
        <source>Password in &quot;[md5]&lt;hex&gt;&quot; form: accepted by oconsole and by C++ bconsole.</source>
        <translation>Mot de passe sous la forme &quot;[md5]&lt;hex&gt;&quot; : accepté par oconsole et par le bconsole C++.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2312"/>
        <source>not used when the Console resource below matches</source>
        <translation>non utilisé lorsque la ressource Console ci-dessous correspond</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2320"/>
        <source>Grpc Fingerprint = &lt;SHA-256 of the Director&apos;s gRPC certificate&gt;</source>
        <translation>Grpc Fingerprint = &lt;SHA-256 du certificat gRPC du Director&gt;</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2351"/>
        <location filename="../src/bconnectionwizard.cpp" line="2390"/>
        <location filename="../src/bconnectionwizard.cpp" line="2401"/>
        <source>Copied!</source>
        <translation>Copié !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2364"/>
        <source>Save Console Client Configuration</source>
        <translation>Enregistrer la configuration du client console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2365"/>
        <location filename="../src/bconnectionwizard.cpp" line="2414"/>
        <location filename="../src/bconnectionwizard.cpp" line="2491"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Fichiers de configuration (*.conf);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2371"/>
        <location filename="../src/bconnectionwizard.cpp" line="2420"/>
        <location filename="../src/bconnectionwizard.cpp" line="2450"/>
        <location filename="../src/bconnectionwizard.cpp" line="2497"/>
        <location filename="../src/bconnectionwizard.cpp" line="2526"/>
        <source>Save Failed</source>
        <translation>Échec de l&apos;enregistrement</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2372"/>
        <location filename="../src/bconnectionwizard.cpp" line="2421"/>
        <location filename="../src/bconnectionwizard.cpp" line="2498"/>
        <source>Could not write to %1</source>
        <translation>Impossible d&apos;écrire dans %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2379"/>
        <location filename="../src/bconnectionwizard.cpp" line="2428"/>
        <location filename="../src/bconnectionwizard.cpp" line="2479"/>
        <location filename="../src/bconnectionwizard.cpp" line="2505"/>
        <location filename="../src/bconnectionwizard.cpp" line="2544"/>
        <source>Saved!</source>
        <translation>Enregistré !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2413"/>
        <source>Save Console Configuration</source>
        <translation>Enregistrer la configuration de la Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2440"/>
        <source>Save Console Configuration as ZIP</source>
        <translation>Enregistrer la configuration de la Console en ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2441"/>
        <location filename="../src/bconnectionwizard.cpp" line="2517"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>Archives ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2451"/>
        <location filename="../src/bconnectionwizard.cpp" line="2527"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Impossible de créer le fichier ZIP : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2490"/>
        <source>Save Director TLS Configuration</source>
        <translation>Enregistrer la configuration TLS du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2516"/>
        <source>Save Director TLS Configuration as ZIP</source>
        <translation>Enregistrer la configuration TLS du Director en ZIP</translation>
    </message>
</context>
<context>
    <name>CredentialsPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1333"/>
        <source>Enter your credentials</source>
        <translation>Saisissez vos identifiants</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1339"/>
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1343"/>
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1351"/>
        <source>Generate</source>
        <translation>Générer</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1352"/>
        <source>Generate a secure random password</source>
        <translation>Générer un mot de passe aléatoire sécurisé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1357"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1364"/>
        <location filename="../src/bconnectionwizard.cpp" line="1370"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(le hachage MD5 du mot de passe apparaîtra ici)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1365"/>
        <source>MD5 Hash:</source>
        <translation>Hachage MD5 :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1403"/>
        <source>Remember password</source>
        <translation>Mémoriser le mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1412"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console config: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</source>
        <translation>Configuration du Director : /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Configuration de la console : /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1418"/>
        <source>These must match your Bareos Director configuration.</source>
        <translation>Ces valeurs doivent correspondre à la configuration de votre Director Bareos.</translation>
    </message>
</context>
<context>
    <name>ImportConsoleSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="668"/>
        <source>Select Director and Console from imported configuration</source>
        <translation>Sélectionner le Director et la Console de la configuration importée</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="673"/>
        <source>The configuration will be parsed from your ZIP file. Select which Director and Console to use for the connection.</source>
        <translation>La configuration sera analysée à partir de votre fichier ZIP. Sélectionnez le Director et la Console à utiliser pour la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="679"/>
        <source>Director:</source>
        <translation>Director :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="682"/>
        <source>Console:</source>
        <translation>Console :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="704"/>
        <source>Select a console to view its details...</source>
        <translation>Sélectionnez une console pour afficher ses détails...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="746"/>
        <source>No ZIP file specified.</source>
        <translation>Aucun fichier ZIP indiqué.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="753"/>
        <source>Extracting and parsing ZIP archive...</source>
        <translation>Extraction et analyse de l&apos;archive ZIP...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="764"/>
        <source>Cannot read the ZIP archive: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="774"/>
        <source>No configuration resources found in the archive.</source>
        <translation>Aucune ressource de configuration trouvée dans l&apos;archive.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="800"/>
        <source>Parsed %1 resources: %2 Director(s), %3 Console(s)</source>
        <translation>%1 ressources analysées : %2 Director(s), %3 Console(s)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="808"/>
        <source>Warning: Need at least one Director and one Console resource.</source>
        <translation>Avertissement : il faut au moins une ressource Director et une ressource Console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="830"/>
        <source>=== Director: %1 ===
</source>
        <translation>=== Director : %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="835"/>
        <source>(multiple addresses)</source>
        <translation>(plusieurs adresses)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="838"/>
        <source>  Address: %1
</source>
        <translation>  Adresse : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="838"/>
        <source>(not set)</source>
        <translation>(non défini)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="839"/>
        <source>  Port: %1
</source>
        <translation>  Port : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="840"/>
        <location filename="../src/bconnectionwizard.cpp" line="862"/>
        <source>  TLS Enable: %1
</source>
        <translation>  TLS Enable : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="847"/>
        <source>=== Console: %1 ===
</source>
        <translation>=== Console : %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="853"/>
        <source>  Password: [md5]***
</source>
        <translation>  Mot de passe : [md5]***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="855"/>
        <source>  Password: ***
</source>
        <translation>  Mot de passe : ***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="858"/>
        <source>  Password: (not set)
</source>
        <translation>  Mot de passe : (non défini)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="863"/>
        <source>  TLS Require: %1
</source>
        <translation>  TLS Require : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="868"/>
        <source>  Profile: %1
</source>
        <translation>  Profile : %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="884"/>
        <source>
  --- ACLs ---
</source>
        <translation>
  --- ACL ---
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="887"/>
        <source>  %1: %2
</source>
        <translation>  %1 : %2
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="892"/>
        <source>
  (No ACLs defined on this console)
</source>
        <translation>
  (Aucune ACL définie sur cette console)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1034"/>
        <source>Host/Address</source>
        <translation>Hôte/Adresse</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1035"/>
        <source>Director Name</source>
        <translation>Nom du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1036"/>
        <source>Console Name</source>
        <translation>Nom de la console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1037"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1040"/>
        <source>Incomplete Configuration</source>
        <translation>Configuration incomplète</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1041"/>
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
        <location filename="../ui/jobwidget.ui" line="14"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
</context>
<context>
    <name>ProfileNamePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2823"/>
        <source>Name this connection</source>
        <translation>Nommer cette connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2826"/>
        <source>e.g., Production Server</source>
        <translation>p. ex. Serveur de production</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2829"/>
        <source>Set as default</source>
        <translation>Définir par défaut</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2833"/>
        <source>Connect now</source>
        <translation>Se connecter maintenant</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2843"/>
        <source>Advanced Settings...</source>
        <translation>Paramètres avancés...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2844"/>
        <source>Configure TLS options, ACLs, and other advanced settings</source>
        <translation>Configurer les options TLS, les ACL et d&apos;autres paramètres avancés</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2849"/>
        <source>Export Config...</source>
        <translation>Exporter la configuration...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2850"/>
        <source>Export Bareos configuration files as zip archive</source>
        <translation>Exporter les fichiers de configuration Bareos sous forme d&apos;archive zip</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2857"/>
        <source>This name helps identify the connection.</source>
        <translation>Ce nom permet d&apos;identifier la connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2937"/>
        <source>Console only (for existing Director)</source>
        <translation>Console uniquement (pour un Director existant)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2938"/>
        <source>Console + Profile (with ACLs)</source>
        <translation>Console + Profile (avec ACL)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2939"/>
        <source>Full configuration (new installation)</source>
        <translation>Configuration complète (nouvelle installation)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2943"/>
        <location filename="../src/bconnectionwizard.cpp" line="2958"/>
        <source>Export Configuration</source>
        <translation>Exporter la configuration</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2944"/>
        <source>Select export mode:</source>
        <translation>Sélectionnez le mode d&apos;exportation :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2960"/>
        <source>Zip Archives (*.zip)</source>
        <translation>Archives zip (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2965"/>
        <source>Export Successful</source>
        <translation>Exportation réussie</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2966"/>
        <source>Configuration exported to:
%1

Extract to your Bareos server&apos;s root directory and restart the Director.</source>
        <translation>Configuration exportée vers :
%1

Extrayez-la dans le répertoire racine de votre serveur Bareos et redémarrez le Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2970"/>
        <source>Export Failed</source>
        <translation>Échec de l&apos;exportation</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2971"/>
        <source>Failed to export configuration:
%1</source>
        <translation>Impossible d&apos;exporter la configuration :
%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="33"/>
        <source>No files to export</source>
        <translation>Aucun fichier à exporter</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="206"/>
        <source>Cannot create zip file: %1</source>
        <translation>Impossible de créer le fichier zip : %1</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="237"/>
        <source>Error writing zip file</source>
        <translation>Erreur lors de l&apos;écriture du fichier zip</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="269"/>
        <source>Not set</source>
        <translation>Non défini</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="273"/>
        <source>%n year(s)</source>
        <translation>
            <numerusform>%n an</numerusform>
            <numerusform>%n ans</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="275"/>
        <source>%n day(s)</source>
        <translation>
            <numerusform>%n jour</numerusform>
            <numerusform>%n jours</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="279"/>
        <source>%n hour(s)</source>
        <translation>
            <numerusform>%n heure</numerusform>
            <numerusform>%n heures</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="281"/>
        <source>%n second(s)</source>
        <translation>
            <numerusform>%n seconde</numerusform>
            <numerusform>%n secondes</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="30"/>
        <source>Add &quot;%1&quot;</source>
        <translation>Ajouter &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="64"/>
        <source>Remove &quot;%1&quot;</source>
        <translation>Supprimer &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="66"/>
        <source>Remove %1 items</source>
        <translation>Supprimer %1 éléments</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="99"/>
        <source>Edit item</source>
        <translation>Modifier l&apos;élément</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="140"/>
        <source>Move item</source>
        <translation>Déplacer l&apos;élément</translation>
    </message>
    <message>
        <location filename="../include/bconnectionprofile.h" line="129"/>
        <source>New Connection</source>
        <translation type="unfinished">Nouvelle connexion</translation>
    </message>
</context>
<context>
    <name>ServerPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1069"/>
        <source>Where is your Bareos Director?</source>
        <translation>Où se trouve votre Director Bareos ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1074"/>
        <source>Found in the network:</source>
        <translation>Trouvés sur le réseau :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1076"/>
        <source>Search again</source>
        <translation>Rechercher à nouveau</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1086"/>
        <source>Onesimus Directors announce themselves in the local network. Select one to fill in the connection data.</source>
        <translation>Les Onesimus Directors s&apos;annoncent sur le réseau local. Sélectionnez-en un pour renseigner les données de connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1101"/>
        <source>Searching...</source>
        <translation>Recherche...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1112"/>
        <source>e.g., bareos.example.com or 192.168.1.100</source>
        <translation>p. ex. bareos.example.com ou 192.168.1.100</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1113"/>
        <source>Host:</source>
        <translation>Hôte :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1119"/>
        <source>Port:</source>
        <translation>Port :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1122"/>
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1123"/>
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1124"/>
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1125"/>
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1126"/>
        <source>Select the operating system of the Bareos Director server.
This affects certificate paths in exported configurations.</source>
        <translation>Sélectionnez le système d&apos;exploitation du serveur du Director Bareos.
Cela influe sur les chemins des certificats dans les configurations exportées.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1128"/>
        <source>Server OS:</source>
        <translation>OS du serveur :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1132"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (protocole de console Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1133"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1139"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Cette version d&apos;Onesimus ne prend pas en charge gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1141"/>
        <source>Protocol:</source>
        <translation>Protocole :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1147"/>
        <source>gRPC port:</source>
        <translation>Port gRPC :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1161"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Configuration du Director : /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1164"/>
        <location filename="../src/bconnectionwizard.cpp" line="1223"/>
        <source>The default port is 9101.</source>
        <translation>Le port par défaut est 9101.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1201"/>
        <source>Searching for Onesimus Directors in the network...</source>
        <translation>Recherche d&apos;Onesimus Directors sur le réseau...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1207"/>
        <source>No Onesimus Director found - enter the server manually.</source>
        <translation>Aucun Onesimus Director trouvé - saisissez le serveur manuellement.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1222"/>
        <source>gRPC is encrypted with TLS; the server certificate is confirmed on first use.</source>
        <translation>gRPC est chiffré avec TLS ; le certificat du serveur est confirmé lors de la première utilisation.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1242"/>
        <source>(gRPC %1)</source>
        <translation>(gRPC %1)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="1255"/>
        <source>%n Director(s) found. Select one to use it.</source>
        <translation>
            <numerusform>%n Director trouvé. Sélectionnez-le pour l&apos;utiliser.</numerusform>
            <numerusform>%n Directors trouvés. Sélectionnez-en un pour l&apos;utiliser.</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>StorageWidget</name>
    <message>
        <location filename="../src/storagewidget.cpp" line="30"/>
        <source>unknown</source>
        <translation>inconnu</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="56"/>
        <source>(due)</source>
        <translation>(échu)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="88"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="91"/>
        <source>Pool filter</source>
        <translation>Filtre de pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="97"/>
        <source>&amp;Status:</source>
        <translation>É&amp;tat :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="100"/>
        <source>Status filter</source>
        <translation>Filtre d&apos;état</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="106"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="117"/>
        <source>Statistics</source>
        <translation>Statistiques</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="120"/>
        <source>Volumes:</source>
        <translation>Volumes :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="128"/>
        <source>Used:</source>
        <translation>Utilisé :</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="454"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="455"/>
        <source>Status</source>
        <translation>État</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="458"/>
        <source>Used</source>
        <translation>Utilisé</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="459"/>
        <source>Limit</source>
        <translation>Limite</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="460"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Last Written</source>
        <translation>Dernière écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Retention</source>
        <translation>Rétention</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Reusable From</source>
        <translation>Réutilisable à partir du</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Reason</source>
        <translation>Raison</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="152"/>
        <source>Maximum Volume Bytes of the volume; &quot;none&quot; = no limit</source>
        <translation>Maximum Volume Bytes du Volume ; &quot;aucune&quot; = pas de limite</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="154"/>
        <source>From when Bareos may reuse the volume. It does so only when its pool needs a volume, so this is the earliest date, not the day of recycling.</source>
        <translation>Date à partir de laquelle Bareos peut réutiliser le Volume. Il ne le fait que lorsque son Pool a besoin d&apos;un Volume ; il s&apos;agit donc de la date la plus proche, pas du jour du recyclage.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="156"/>
        <source>Volumes</source>
        <translation>Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="158"/>
        <source>One row per volume. Enter opens the details of the selected volume.</source>
        <translation>Une ligne par Volume. Entrée ouvre les détails du Volume sélectionné.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="215"/>
        <source>All pools (%1)</source>
        <translation>Tous les pools (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="218"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="224"/>
        <source>All statuses (%1)</source>
        <translation>Tous les états (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="227"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="250"/>
        <source>Error parsing volume data</source>
        <translation>Erreur lors de l&apos;analyse des données des Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="257"/>
        <source>The Director sent the short volume list: limits, job counts and use durations are unknown, and so is when an Append volume will be closed.</source>
        <translation>Le Director a envoyé la liste courte des Volumes : les limites, le nombre de Jobs et les durées d&apos;utilisation sont inconnus, de même que le moment où un Volume Append sera fermé.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/storagewidget.cpp" line="271"/>
        <source>%n volume(s) loaded</source>
        <translation>
            <numerusform>%n Volume chargé</numerusform>
            <numerusform>%n Volumes chargés</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="318"/>
        <source>%1, %2, %3, last written %4, reusable from %5</source>
        <translation>%1, %2, %3, dernière écriture %4, réutilisable à partir du %5</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="346"/>
        <location filename="../src/storagewidget.cpp" line="459"/>
        <location filename="../src/storagewidget.cpp" line="461"/>
        <location filename="../src/storagewidget.cpp" line="467"/>
        <location filename="../src/storagewidget.cpp" line="469"/>
        <source>none</source>
        <translation>aucune</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="354"/>
        <source>%1 of %2</source>
        <translation>%1 sur %2</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="411"/>
        <source>Refreshing the volume list...</source>
        <translation>Actualisation de la liste des Volumes...</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="439"/>
        <source>unknown (not in this list)</source>
        <translation>inconnu (absent de cette liste)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="456"/>
        <source>Media type</source>
        <translation>Type de média</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="461"/>
        <source>Maximum jobs</source>
        <translation>Nombre maximal de Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="462"/>
        <source>Errors</source>
        <translation>Erreurs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="463"/>
        <source>First written</source>
        <translation>Première écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="464"/>
        <source>Last written</source>
        <translation>Dernière écriture</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465"/>
        <source>Labeled</source>
        <translation>Étiqueté</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="466"/>
        <source>Volume retention</source>
        <translation>Rétention du Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="467"/>
        <source>Use duration</source>
        <translation>Durée d&apos;utilisation</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>Recycle</source>
        <translation>Recyclage</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>yes</source>
        <translation>oui</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>no</source>
        <translation>non</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="469"/>
        <source>Recycle pool</source>
        <translation>Pool de recyclage</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="470"/>
        <source>Times recycled</source>
        <translation>Nombre de recyclages</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="471"/>
        <source>Reusable from</source>
        <translation>Réutilisable à partir du</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="481"/>
        <source>Volume Details</source>
        <translation>Détails du Volume</translation>
    </message>
    <message>
        <location filename="../ui/storagewidget.ui" line="14"/>
        <location filename="../src/storagewidget.cpp" line="457"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
</context>
<context>
    <name>TLSPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1689"/>
        <source>Where are your TLS certificates?</source>
        <translation>Où se trouvent vos certificats TLS ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1695"/>
        <source>/etc/bareos/ssl/ca.pem</source>
        <translation>/etc/bareos/ssl/ca.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1696"/>
        <location filename="../src/bconnectionwizard.cpp" line="1707"/>
        <location filename="../src/bconnectionwizard.cpp" line="1728"/>
        <location filename="../src/bconnectionwizard.cpp" line="1737"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1700"/>
        <source>CA Cert:</source>
        <translation>Cert. CA :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1706"/>
        <source>Path to PFX certificate file</source>
        <translation>Chemin du fichier de certificat PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1711"/>
        <source>PFX File:</source>
        <translation>Fichier PFX :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1715"/>
        <source>PFX file password (if encrypted)</source>
        <translation>Mot de passe du fichier PFX (s&apos;il est chiffré)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1716"/>
        <source>PFX Password:</source>
        <translation>Mot de passe PFX :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1727"/>
        <source>/etc/bareos/ssl/client.pem</source>
        <translation>/etc/bareos/ssl/client.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1732"/>
        <source>Client Cert:</source>
        <translation>Cert. client :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1736"/>
        <source>/etc/bareos/ssl/client.key</source>
        <translation>/etc/bareos/ssl/client.key</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1741"/>
        <source>Private Key:</source>
        <translation>Clé privée :</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1751"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Convertir PEM/DER en PFX...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1752"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Convertir des fichiers de certificat et de clé PEM/DER séparés en un fichier PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1757"/>
        <source>Generate Certificates...</source>
        <translation>Générer des certificats...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1760"/>
        <source>No certificates? Click Generate to create them.</source>
        <translation>Pas de certificats ? Cliquez sur Générer pour les créer.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1796"/>
        <location filename="../src/bconnectionwizard.cpp" line="1807"/>
        <location filename="../src/bconnectionwizard.cpp" line="1819"/>
        <source>Missing</source>
        <translation>Manquant</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1796"/>
        <source>Please provide a CA certificate file.</source>
        <translation>Veuillez indiquer un fichier de certificat CA.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1800"/>
        <location filename="../src/bconnectionwizard.cpp" line="1811"/>
        <location filename="../src/bconnectionwizard.cpp" line="1823"/>
        <source>Not Found</source>
        <translation>Introuvable</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1800"/>
        <source>CA certificate file not found.</source>
        <translation>Fichier de certificat CA introuvable.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1807"/>
        <source>Please provide a PFX certificate file.</source>
        <translation>Veuillez indiquer un fichier de certificat PFX.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1811"/>
        <source>PFX certificate file not found.</source>
        <translation>Fichier de certificat PFX introuvable.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1819"/>
        <source>Please provide all certificate files.</source>
        <translation>Veuillez indiquer tous les fichiers de certificat.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1823"/>
        <source>One or more certificate files not found.</source>
        <translation>Un ou plusieurs fichiers de certificat sont introuvables.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1857"/>
        <source>CA Certificate</source>
        <translation>Certificat CA</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1858"/>
        <location filename="../src/bconnectionwizard.cpp" line="1879"/>
        <source>Certificates (*.pem *.crt *.cer);;All Files (*)</source>
        <translation>Certificats (*.pem *.crt *.cer);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875"/>
        <source>Select PFX Certificate</source>
        <translation>Sélectionner le certificat PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1876"/>
        <source>PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)</source>
        <translation>Fichiers PFX (*.pfx);;Certificats (*.pem *.crt);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1878"/>
        <source>Client Certificate</source>
        <translation>Certificat client</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1896"/>
        <source>Private Key</source>
        <translation>Clé privée</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1897"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Clés privées (*.pem *.key);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1932"/>
        <source>PFX File Created</source>
        <translation>Fichier PFX créé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1933"/>
        <source>PFX file has been created successfully.

The certificate path has been updated.</source>
        <translation>Le fichier PFX a été créé avec succès.

Le chemin du certificat a été mis à jour.</translation>
    </message>
</context>
<context>
    <name>TemplateSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="319"/>
        <source>How would you like to create the connection?</source>
        <translation>Comment souhaitez-vous créer la connexion ?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="321"/>
        <source>You can start from scratch, use a template from an existing connection, or import from a ZIP file.</source>
        <translation>Vous pouvez partir de zéro, utiliser comme modèle une connexion existante ou importer depuis un fichier ZIP.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="325"/>
        <source>Create new connection from scratch</source>
        <translation>Créer une nouvelle connexion à partir de zéro</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="326"/>
        <source>Use template from existing connection</source>
        <translation>Utiliser une connexion existante comme modèle</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="327"/>
        <source>Import from ZIP file</source>
        <translation>Importer depuis un fichier ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="349"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="373"/>
        <source>Select ZIP file...</source>
        <translation>Sélectionner un fichier ZIP...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="376"/>
        <source>Browse...</source>
        <translation>Parcourir...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="487"/>
        <source>Director: %1</source>
        <translation>Director : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="488"/>
        <source>Console: %1</source>
        <translation>Console : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>TLS: %1</source>
        <translation>TLS : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <source>PSK</source>
        <translation>PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <source>Certificate</source>
        <translation>Certificat</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>Disabled</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>Enabled</source>
        <translation>Activé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="546"/>
        <source>System: %1</source>
        <translation>Système : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="652"/>
        <source>Select Director Configuration ZIP</source>
        <translation>Sélectionner le ZIP de configuration du Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="654"/>
        <source>ZIP Files (*.zip);;All Files (*)</source>
        <translation>Fichiers ZIP (*.zip);;Tous les fichiers (*)</translation>
    </message>
</context>
<context>
    <name>TestPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2557"/>
        <location filename="../src/bconnectionwizard.cpp" line="2574"/>
        <location filename="../src/bconnectionwizard.cpp" line="2604"/>
        <location filename="../src/bconnectionwizard.cpp" line="2713"/>
        <source>Test Connection</source>
        <translation>Tester la connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2561"/>
        <source>Let&apos;s test your connection.</source>
        <translation>Testons votre connexion.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2565"/>
        <location filename="../src/bconnectionwizard.cpp" line="2601"/>
        <source>Click the button to test.</source>
        <translation>Cliquez sur le bouton pour tester.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2624"/>
        <source>Connecting...</source>
        <translation>Connexion...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2627"/>
        <source>Starting test...</source>
        <translation>Démarrage du test...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2666"/>
        <source>Mode: TLS-Cert</source>
        <translation>Mode : TLS-Cert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2678"/>
        <source>Mode: TLS-PSK with cipher list: %1</source>
        <translation>Mode : TLS-PSK avec la liste de chiffrements : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2680"/>
        <source>Mode: TLS-PSK</source>
        <translation>Mode : TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2693"/>
        <source>Mode: gRPC (TLS) on port %1</source>
        <translation>Mode : gRPC (TLS) sur le port %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2694"/>
        <location filename="../src/bconnectionwizard.cpp" line="2696"/>
        <source>Connecting to %1:%2...</source>
        <translation>Connexion à %1:%2...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2732"/>
        <source>Cancelled.</source>
        <translation>Annulé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2740"/>
        <source>Connecting</source>
        <translation>Connexion</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2741"/>
        <source>Authenticating</source>
        <translation>Authentification</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2742"/>
        <source>Setting API</source>
        <translation>Réglage de l&apos;API</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2743"/>
        <source>Loading</source>
        <translation>Chargement</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2744"/>
        <source>Ready</source>
        <translation>Prêt</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2766"/>
        <source>Server certificate trusted.</source>
        <translation>Certificat du serveur approuvé.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2770"/>
        <source>Server certificate not trusted - connection cancelled.</source>
        <translation>Certificat du serveur non approuvé - connexion annulée.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2771"/>
        <location filename="../src/bconnectionwizard.cpp" line="2784"/>
        <location filename="../src/bconnectionwizard.cpp" line="2790"/>
        <source>Failed</source>
        <translation>Échec</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2783"/>
        <source>Auth OK</source>
        <translation>Authentification OK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2784"/>
        <source>Auth failed: %1</source>
        <translation>Échec de l&apos;authentification : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2789"/>
        <source>Error: %1</source>
        <translation>Erreur : %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2800"/>
        <source>Success!</source>
        <translation>Succès !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2801"/>
        <source>Connection successful!</source>
        <translation>Connexion réussie !</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2804"/>
        <source>Test Again</source>
        <translation>Tester à nouveau</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2812"/>
        <source>Timeout</source>
        <translation>Délai dépassé</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2813"/>
        <source>Timed out</source>
        <translation>Délai dépassé</translation>
    </message>
</context>
<context>
    <name>WelcomePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="297"/>
        <source>Welcome</source>
        <translation>Bienvenue</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="303"/>
        <source>Let&apos;s set up a connection to your Bareos Director.

You&apos;ll need:
  - The hostname or IP of your Director
  - Your console name and password

Click Next to begin.</source>
        <translation>Configurons une connexion à votre Director Bareos.

Vous aurez besoin :
  - du nom d&apos;hôte ou de l&apos;adresse IP de votre Director
  - de votre nom de console et de votre mot de passe

Cliquez sur Suivant pour commencer.</translation>
    </message>
</context>
</TS>
