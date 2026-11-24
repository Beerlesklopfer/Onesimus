<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU" sourcelanguage="en">
<context>
    <name>AuthMethodPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1504"/>
        <source>How should we authenticate?</source>
        <translation>Какой способ аутентификации использовать?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1508"/>
        <source>TLS-PSK (recommended)</source>
        <translation>TLS-PSK (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1513"/>
        <source>TLS with certificates</source>
        <translation>TLS с сертификатами</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1531"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Список шифров TLS (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1538"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>напр., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1539"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Click Preset to use recommended ciphers.</source>
        <translation>Список шифров TLS-PSK через двоеточие.
Оставьте пустым для автоопределения.
Нажмите «Предустановка», чтобы использовать рекомендуемые шифры.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1544"/>
        <source>Preset</source>
        <translation>Предустановка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1545"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Заполнить рекомендуемыми шифрами PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1554"/>
        <source>TLS-PSK is the default for Bareos 18.2+.</source>
        <translation>TLS-PSK используется по умолчанию начиная с Bareos 18.2.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1612"/>
        <source>Server supports TLS encryption.</source>
        <translation>Сервер поддерживает шифрование TLS.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1621"/>
        <source>TLS-PSK - not available on this server</source>
        <translation>TLS-PSK — недоступно на этом сервере</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1622"/>
        <source>This server doesn&apos;t appear to support TLS-PSK. Consider upgrading to Bareos 18.2 or later.</source>
        <translation>Похоже, этот сервер не поддерживает TLS-PSK. Рекомендуется обновиться до Bareos 18.2 или новее.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1631"/>
        <source>Server check failed: %1</source>
        <translation>Ошибка проверки сервера: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1649"/>
        <source>TLS-PSK uses your password to encrypt the connection.</source>
        <translation>TLS-PSK использует ваш пароль для шифрования соединения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1654"/>
        <source>You&apos;ll need CA, client certificate, and private key.</source>
        <translation>Потребуются сертификат CA, сертификат клиента и закрытый ключ.</translation>
    </message>
</context>
<context>
    <name>B321Dialog</name>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="91"/>
        <location filename="../src/tools/b321dialog.cpp" line="578"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="112"/>
        <location filename="../src/tools/b321dialog.cpp" line="635"/>
        <source>%1 (not in the volume list)</source>
        <translation>%1 (нет в списке Volumes)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="114"/>
        <location filename="../src/tools/b321dialog.cpp" line="652"/>
        <source>media type unknown</source>
        <translation>тип носителя неизвестен</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="115"/>
        <source>storage unknown</source>
        <translation>Storage неизвестен</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="118"/>
        <source>off site (marked)</source>
        <translation>вне площадки (отмечено)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="120"/>
        <source>off site (location %1)</source>
        <translation>вне площадки (местоположение %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="122"/>
        <source>on site</source>
        <translation>на площадке</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="123"/>
        <source>off site unknown</source>
        <translation>размещение неизвестно</translation>
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
        <translation>Проверка 3-2-1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="136"/>
        <source>Checks for every backup job whether there are 3 copies of the data, on 2 media types, with 1 copy off site. Read only.</source>
        <translation>Проверяет для каждого Job резервного копирования, есть ли 3 копии данных на 2 типах носителей, из них 1 копия вне площадки. Только чтение.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="157"/>
        <source>Data state</source>
        <translation>Состояние данных</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="159"/>
        <source>&amp;Refresh</source>
        <translation>&amp;Обновить</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="161"/>
        <source>Reload the jobs, the volumes of the jobs and the volume list (read only)</source>
        <translation>Перезагрузить Jobs, Volumes этих Jobs и список Volumes (только чтение)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="173"/>
        <source>&amp;Backup jobs:</source>
        <translation>&amp;Jobs резервного копирования:</translation>
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
        <translation>Копии</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Media Types</source>
        <translation>Типы носителей</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Off Site</source>
        <translation>Вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Verdict</source>
        <translation>Вердикт</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="178"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="180"/>
        <source>Copies of the data a restore would use: the chain, its complete copies and, if set, the original data. 3 are needed.</source>
        <translation>Копии данных, которые использовало бы восстановление: цепочка, её полные копии и, если включено, исходные данные. Нужно 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="183"/>
        <source>Distinct media types of the volumes of the chain and its complete copies. 2 are needed.</source>
        <translation>Различные типы носителей у Volumes цепочки и её полных копий. Нужно 2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="185"/>
        <source>Copies off site, by your marking of the storages or a location of the volume. 1 is needed.</source>
        <translation>Копии вне площадки — по вашей отметке Storages или по местоположению Volume. Нужна 1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="188"/>
        <source>3-2-1 check per backup job</source>
        <translation>Проверка 3-2-1 для каждого Job резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="189"/>
        <source>One row per backup job: copies, media types, off site, verdict and reason. The details of the selected job follow below the table.</source>
        <translation>Одна строка на каждый Job резервного копирования: копии, типы носителей, вне площадки, вердикт и причина. Подробности о выбранном Job приведены под таблицей.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="224"/>
        <source>C&amp;hain and copies:</source>
        <translation>&amp;Цепочка и копии:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="229"/>
        <source>Chain and copies of the selected job</source>
        <translation>Цепочка и копии выбранного Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="230"/>
        <source>The jobs a restore would use and the copies of them, with the volumes they lie on</source>
        <translation>Jobs, которые использовало бы восстановление, и их копии вместе с Volumes, на которых они находятся</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="240"/>
        <source>&amp;Why, and what to do:</source>
        <translation>&amp;Почему и что делать:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="244"/>
        <source>Reasons, suggestions and hints for the selected job</source>
        <translation>Причины, предложения и подсказки для выбранного Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="253"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="255"/>
        <source>Count the &amp;original data as a copy</source>
        <translation>Считать &amp;исходные данные копией</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="256"/>
        <source>The 3-2-1 rule is usually stated as the production data plus 2 backups. Off: 3 backups are needed.</source>
        <translation>Правило 3-2-1 обычно формулируется как рабочие данные плюс 2 резервные копии. Если выключено, нужно 3 резервные копии.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="261"/>
        <source>O&amp;ff-site storages:</source>
        <translation>Storages &amp;вне площадки:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="263"/>
        <source>Bareos does not record where a storage stands. Tick the storages that are at another site.</source>
        <translation>Bareos не хранит сведений о том, где находится Storage. Отметьте Storages, расположенные на другой площадке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="270"/>
        <source>Off-site storages</source>
        <translation>Storages вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="271"/>
        <source>A checked storage is off site. Space toggles.</source>
        <translation>Отмеченный Storage находится вне площадки. Пробел переключает отметку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="274"/>
        <source>&amp;Unmarked storages are on site</source>
        <translation>&amp;Неотмеченные Storages находятся на площадке</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="275"/>
        <source>Without this, a storage that is not ticked is &quot;unknown&quot;, not &quot;on site&quot;.</source>
        <translation>Без этой опции неотмеченный Storage считается «неизвестно», а не «на площадке».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="285"/>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="322"/>
        <location filename="../src/tools/b321dialog.cpp" line="438"/>
        <source>%1, off site</source>
        <translation>%1, вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="323"/>
        <location filename="../src/tools/b321dialog.cpp" line="439"/>
        <source>%1, not marked off site</source>
        <translation>%1, не отмечен как вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="444"/>
        <source>No storage known yet</source>
        <translation>Storages пока неизвестны</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="471"/>
        <location filename="../src/tools/b321dialog.cpp" line="520"/>
        <source>Jobs: %1</source>
        <translation>Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="490"/>
        <location filename="../src/tools/b321dialog.cpp" line="523"/>
        <source>Volumes of the jobs: %1</source>
        <translation>Volumes этих Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="505"/>
        <location filename="../src/tools/b321dialog.cpp" line="526"/>
        <source>Volume list: %1</source>
        <translation>Список Volumes: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="557"/>
        <source>%1: %2. %3</source>
        <translation>%1: %2. %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="561"/>
        <source>%1/3</source>
        <translation>%1/3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="564"/>
        <source>Copies: %1 of 3, %2</source>
        <translation>Копии: %1 из 3, %2</translation>
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
        <translation>Типы носителей: %1 из 2%2, %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="579"/>
        <source>%1/1</source>
        <translation>%1/1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="583"/>
        <source>Off site: unknown</source>
        <translation>Вне площадки: неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="584"/>
        <source>Off site: %1 of 1, %2</source>
        <translation>Вне площадки: %1 из 1, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="590"/>
        <source>Verdict: %1</source>
        <translation>Вердикт: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="614"/>
        <source>No job selected.</source>
        <translation>Job не выбран.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="626"/>
        <source>volumes not loaded</source>
        <translation>Volumes не загружены</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="628"/>
        <source>no volume recorded</source>
        <translation>Volume не записан</translation>
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
            <numerusform>Цепочка (сама резервная копия): %n Job</numerusform>
            <numerusform>Цепочка (сама резервная копия): %n Jobs</numerusform>
            <numerusform>Цепочка (сама резервная копия): %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="645"/>
        <source>Copy in pool %1: complete</source>
        <translation>Копия в Pool %1: полная</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="645"/>
        <location filename="../src/tools/b321dialog.cpp" line="648"/>
        <source>(not reported)</source>
        <translation>(не сообщается)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="647"/>
        <source>Copy in pool %1: partial, %2 of %3 jobs</source>
        <translation>Копия в Pool %1: частичная, %2 из %3 Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="656"/>
        <source>last job ended %1</source>
        <translation>последний Job завершён %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="657"/>
        <source>copied %1</source>
        <translation>скопировано %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="658"/>
        <source>%1 — %2</source>
        <translation>%1 — %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="665"/>
        <source>%1 %2, started %3: %4</source>
        <translation>%1 %2, запущен %3: %4</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="668"/>
        <source> (migrated from job %1)</source>
        <translation> (перенесён из Job %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="672"/>
        <source>Copy %1 of %2 %3, copied %4: %5</source>
        <translation>Копия %1 из %2 %3, скопирована %4: %5</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="681"/>
        <source>No copy of %1 %2</source>
        <translation>Нет копии %1 %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="687"/>
        <source>No restorable chain in the job history</source>
        <translation>В истории Jobs нет цепочки, пригодной для восстановления</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="692"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="699"/>
        <source>Why</source>
        <translation>Почему</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="700"/>
        <source>What to do</source>
        <translation>Что делать</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="701"/>
        <source>Hints (not part of the verdict)</source>
        <translation>Подсказки (не влияют на вердикт)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="710"/>
        <source>Not everything could be loaded: %1</source>
        <translation>Не всё удалось загрузить: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="714"/>
        <source>jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="715"/>
        <source>volumes of the jobs</source>
        <translation>Volumes этих Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="716"/>
        <source>volume list</source>
        <translation>список Volumes</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="717"/>
        <source>Loading: %1 …</source>
        <translation>Загрузка: %1 …</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="720"/>
        <source>No job data yet. Refresh loads it (read only).</source>
        <translation>Данных о Jobs пока нет. «Обновить» загружает их (только чтение).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="721"/>
        <source>Not connected to a Director.</source>
        <translation>Нет подключения к Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="729"/>
        <source>%n job(s) checked: %1 pass, %2 fail, %3 unknown. </source>
        <translation>
            <numerusform>Проверен %n Job: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
            <numerusform>Проверено %n Jobs: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
            <numerusform>Проверено %n Jobs: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="731"/>
        <source>From %n job record(s) of the last %1 days</source>
        <translation>
            <numerusform>По %n записи о Jobs за последние %1 дн.</numerusform>
            <numerusform>По %n записям о Jobs за последние %1 дн.</numerusform>
            <numerusform>По %n записям о Jobs за последние %1 дн.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="732"/>
        <source>, %n volume(s)</source>
        <translation>
            <numerusform>, %n Volume</numerusform>
            <numerusform>, %n Volumes</numerusform>
            <numerusform>, %n Volumes</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="733"/>
        <source>, the volumes of %n job(s)</source>
        <translation>
            <numerusform>, Volumes %n Job</numerusform>
            <numerusform>, Volumes %n Jobs</numerusform>
            <numerusform>, Volumes %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="735"/>
        <source>. Read only.</source>
        <translation>. Только чтение.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="16"/>
        <source>3-2-1 &amp;Verifier...</source>
        <translation>Проверка 3-2-&amp;1...</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="17"/>
        <source>Check for every backup job: 3 copies, 2 media types, 1 copy off site (read only)</source>
        <translation>Проверить для каждого Job резервного копирования: 3 копии, 2 типа носителей, 1 копия вне площадки (только чтение)</translation>
    </message>
</context>
<context>
    <name>B321Verifier</name>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="97"/>
        <source>unknown time</source>
        <translation>время неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="103"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="116"/>
        <source>(pool not reported)</source>
        <translation>(Pool не сообщается)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="418"/>
        <source>No successful Full backup of %1 in the job history of the last %n day(s): there is no chain to restore.</source>
        <translation>
            <numerusform>В истории Jobs за последний %n день нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
            <numerusform>В истории Jobs за последние %n дня нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
            <numerusform>В истории Jobs за последние %n дней нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="420"/>
        <source>No successful Full backup of %1 in the job history: there is no chain to restore.</source>
        <translation>В истории Jobs нет успешной резервной копии Full для %1: нет цепочки для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="422"/>
        <source>Run a Full backup of %1 and check that it terminates normally.</source>
        <translation>Выполните резервное копирование Full для %1 и убедитесь, что оно завершилось нормально.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="424"/>
        <source>No successful Full backup in the job history</source>
        <translation>В истории Jobs нет успешной резервной копии Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="426"/>
        <source>Only the original data on the client counts: 1 of 3 copies.</source>
        <translation>Учитываются только исходные данные на клиенте: 1 из 3 копий.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="439"/>
        <source>The chain a restore would use today is %1</source>
        <translation>Цепочка, которую сегодня использовало бы восстановление: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="440"/>
        <source>, a Differential</source>
        <translation>, Differential</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="441"/>
        <source> and %n Incremental(s) after it</source>
        <translation>
            <numerusform> и после неё %n Incremental</numerusform>
            <numerusform> и после неё %n Incremental</numerusform>
            <numerusform> и после неё %n Incremental</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="442"/>
        <source>; it counts as 1 copy.</source>
        <translation>; она считается 1 копией.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="445"/>
        <source>The original data on the client counts as 1 copy (setting &quot;Count the original data as a copy&quot;).</source>
        <translation>Исходные данные на клиенте считаются 1 копией (настройка «Считать исходные данные копией»).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="448"/>
        <source>The original data on the client is not counted (setting &quot;Count the original data as a copy&quot; is off).</source>
        <translation>Исходные данные на клиенте не учитываются (настройка «Считать исходные данные копией» выключена).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="454"/>
        <source>job %1 from job %2</source>
        <translation>Job %1 из Job %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="456"/>
        <source>Migrated chain jobs (%1): a Migrate moves the data and adds no copy; the old job is not restored any more.</source>
        <translation>Перенесённые Jobs цепочки (%1): Migrate перемещает данные и не добавляет копию; старый Job больше не восстанавливается.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="462"/>
        <source>Pool %1 holds a copy of every job of the chain; it counts as 1 copy.</source>
        <translation>Pool %1 содержит копию каждого Job цепочки; это считается 1 копией.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="466"/>
        <source>Pool %1 holds copies of %2 of the %3 jobs of the chain (missing: %4); a partial copy does not count.</source>
        <translation>Pool %1 содержит копии %2 из %3 Jobs цепочки (отсутствуют: %4); частичная копия не учитывается.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="472"/>
        <source>No Copy job has copied the chain.</source>
        <translation>Ни один Job типа Copy не скопировал цепочку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="474"/>
        <source>Copies: %1 of 3.</source>
        <translation>Копии: %1 из 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="480"/>
        <source>The volumes of the chain are not known, so neither are their media types.</source>
        <translation>Volumes цепочки неизвестны, поэтому неизвестны и их типы носителей.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="481"/>
        <source>The volumes of the jobs (JobMedia) were not loaded: media types unknown.</source>
        <translation>Volumes этих Jobs (JobMedia) не загружены: типы носителей неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="483"/>
        <source>Media types of the chain and its complete copies: %1.</source>
        <translation>Типы носителей цепочки и её полных копий: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="486"/>
        <source>A copy that spans several media types counts once; copies on media types of their own: %1.</source>
        <translation>Копия, занимающая несколько типов носителей, считается один раз; копии на собственных типах носителей: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="490"/>
        <source>They lie on the storages %1; the same media type on several storages counts as one type.</source>
        <translation>Они находятся на Storages %1; один и тот же тип носителя на нескольких Storages считается одним типом.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="495"/>
        <source>Some volumes of these jobs are not in the volume list or have no media type.</source>
        <translation>Некоторых Volumes этих Jobs нет в списке Volumes, или у них не указан тип носителя.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="517"/>
        <source>storage %1 marked off site</source>
        <translation>Storage %1 отмечен как вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="518"/>
        <source>volume %1 has location %2 in the catalog</source>
        <translation>у Volume %1 в каталоге указано местоположение %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="522"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="523"/>
        <source>the chain</source>
        <translation>цепочка</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="524"/>
        <source>the copy in pool %1</source>
        <translation>копия в Pool %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="527"/>
        <source>Off site: %1.</source>
        <translation>Вне площадки: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="531"/>
        <source>Off site is unknown: no storage of the chain or its copies is marked off site and no volume has a location in the catalog. Bareos itself does not record whether a storage is off site.</source>
        <translation>Размещение вне площадки неизвестно: ни один Storage цепочки или её копий не отмечен как вне площадки, и ни у одного Volume нет местоположения в каталоге. Сам Bareos не хранит сведений о том, находится ли Storage вне площадки.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="536"/>
        <source>Nothing is off site: every copy has a volume on a storage that is on site.</source>
        <translation>Вне площадки ничего нет: у каждой копии есть Volume на Storage, расположенном на площадке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="542"/>
        <source>the present one</source>
        <translation>текущая</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="545"/>
        <source>Pool %1 lacks copies of the jobs %2: run its Copy job again (Selection Type = PoolUncopiedJobs copies what is missing) so that the whole chain is copied.</source>
        <translation>В Pool %1 нет копий Jobs %2: запустите его Job типа Copy ещё раз (Selection Type = PoolUncopiedJobs копирует недостающее), чтобы скопировать всю цепочку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="553"/>
        <source>Add a Copy job to a second storage with a different media type than %1 (for example tape, a removable disk or cloud storage with a Media Type of its own) (needed: %2 more).</source>
        <translation>Добавьте Job типа Copy на второй Storage с типом носителя, отличным от %1 (например, лента, съёмный диск или облачное хранилище с собственным Media Type) (нужно ещё: %2).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="558"/>
        <source>Add a Copy job to a second storage (needed: %1 more).</source>
        <translation>Добавьте Job типа Copy на второй Storage (нужно ещё: %1).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="562"/>
        <source>Write one of the copies to a storage with another media type than %1.</source>
        <translation>Запишите одну из копий на Storage с типом носителя, отличным от %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="566"/>
        <source>Refresh: the volumes of the jobs and the volume list are needed to tell the media types.</source>
        <translation>Обновите: чтобы определить типы носителей, нужны Volumes этих Jobs и список Volumes.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="571"/>
        <source>Mark storage %1 as off site if it is at another site; if it is not, tick &quot;Unmarked storages are on site&quot;.</source>
        <translation>Отметьте Storage %1 как вне площадки, если он находится на другой площадке; если нет, включите «Неотмеченные Storages находятся на площадке».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="576"/>
        <source>Refresh: the storages of the volumes are needed to tell where the copies are.</source>
        <translation>Обновите: чтобы определить, где находятся копии, нужны Storages этих Volumes.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="580"/>
        <source>Copy the chain to a storage at another site and mark that storage as off site (or take a volume off site and record its location).</source>
        <translation>Скопируйте цепочку на Storage на другой площадке и отметьте этот Storage как вне площадки (или вывезите Volume за пределы площадки и укажите его местоположение).</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="600"/>
        <source>The newest Full (job %1) is %n day(s) old</source>
        <translation>
            <numerusform>Последней копии Full (Job %1) %n день</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дня</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дней</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="602"/>
        <source>; this job&apos;s Fulls ran every %n day(s) (median of the loaded history).</source>
        <translation>
            <numerusform>; копии Full этого Job выполнялись каждый %n день (медиана загруженной истории).</numerusform>
            <numerusform>; копии Full этого Job выполнялись каждые %n дня (медиана загруженной истории).</numerusform>
            <numerusform>; копии Full этого Job выполнялись каждые %n дней (медиана загруженной истории).</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="605"/>
        <source> It is overdue.</source>
        <translation> Она просрочена.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="608"/>
        <source>The newest Full (job %1) is %n day(s) old; it is the only successful Full in the loaded history.</source>
        <translation>
            <numerusform>Последней копии Full (Job %1) %n день; это единственная успешная копия Full в загруженной истории.</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дня; это единственная успешная копия Full в загруженной истории.</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дней; это единственная успешная копия Full в загруженной истории.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="635"/>
        <source>The newest copy of this job (job %1 in pool %2, a copy of job %3) was made %n day(s) ago.</source>
        <translation>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n день назад.</numerusform>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n дня назад.</numerusform>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n дней назад.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="645"/>
        <source>That copy belongs to an earlier chain: the current chain (from job %1) has no complete copy in pool %2.</source>
        <translation>Эта копия относится к более ранней цепочке: у текущей цепочки (начиная с Job %1) нет полной копии в Pool %2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="674"/>
        <source>The chain&apos;s %1 (job %2)</source>
        <translation>%1 цепочки (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="676"/>
        <source>The copy (job %1, pool %2) of the chain&apos;s %3</source>
        <translation>Копия (Job %1, Pool %2) %3 цепочки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="678"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="680"/>
        <source>%1 is on volume %2, which is in status Error.</source>
        <translation>%1 находится на Volume %2 в состоянии Error.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="683"/>
        <source>The chain&apos;s Full is on volume %1, which is in Error: run a new Full backup and check the volume.</source>
        <translation>Full цепочки находится на Volume %1 в состоянии Error: выполните новое резервное копирование Full и проверьте Volume.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="688"/>
        <source>%1 is on volume %2, which is %3: Bareos may overwrite it.</source>
        <translation>%1 находится на Volume %2 в состоянии %3: Bareos может его перезаписать.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="697"/>
        <source>The job list hit its row limit: older jobs and copies may be missing from this check.</source>
        <translation>Список Jobs достиг ограничения по числу строк: более старые Jobs и копии могут отсутствовать в этой проверке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="703"/>
        <source>%1 of 3 copies</source>
        <translation>%1 из 3 копий</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="704"/>
        <source>partial copies: %1</source>
        <translation>частичные копии: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="706"/>
        <source>media types unknown</source>
        <translation>типы носителей неизвестны</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="708"/>
        <source>media: %1 of 2 (%2)</source>
        <translation>носители: %1 из 2 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="711"/>
        <source>off site: %1 of 1</source>
        <translation>вне площадки: %1 из 1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="712"/>
        <source>off site unknown</source>
        <translation>размещение неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="713"/>
        <source>nothing off site</source>
        <translation>вне площадки ничего нет</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="866"/>
        <source>met</source>
        <translation>выполнено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="867"/>
        <source>not met</source>
        <translation>не выполнено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="870"/>
        <location filename="../src/tools/b321verifier.cpp" line="902"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="876"/>
        <source>Passes</source>
        <translation>Пройдено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="877"/>
        <source>Fails</source>
        <translation>Не пройдено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="880"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
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
        <translation>вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="899"/>
        <source>on site</source>
        <translation>на площадке</translation>
    </message>
</context>
<context>
    <name>BAclMatrix</name>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="62"/>
        <source>Jobs</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="63"/>
        <source>Clients</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="64"/>
        <source>Storages</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="65"/>
        <source>Schedules</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="66"/>
        <source>Pools</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="67"/>
        <source>FileSets</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="68"/>
        <source>Catalogs</source>
        <translation>Catalog</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="69"/>
        <source>Commands</source>
        <translation>Команды</translation>
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
        <translation>Director не ответил.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="92"/>
        <source>The Director did not answer with ACLs: %1</source>
        <translation>Director ответил не списком ACL: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="101"/>
        <source>The Director refused &quot;.acl list&quot;: %1</source>
        <translation>Director отклонил «.acl list»: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="106"/>
        <source>The reply holds no list of consoles (&quot;acl&quot;).</source>
        <translation>В ответе нет списка консолей («acl»).</translation>
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
        <translation>Заблокирована: не удалось прочитать ACL каталога этой консоли; разрешены только status, .status, version и .api.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="351"/>
        <source>Allowed (decided by the Director).</source>
        <translation>Разрешено (решение Director).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="351"/>
        <location filename="../src/director/baclmatrix.cpp" line="364"/>
        <source>Allowed by %1.</source>
        <translation>Разрешено записью %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="354"/>
        <source>The file ACL allows it (%1), the catalog ACL does not.</source>
        <translation>ACL файла разрешает (%1), ACL каталога — нет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="356"/>
        <location filename="../src/director/baclmatrix.cpp" line="365"/>
        <source>Denied by %1.</source>
        <translation>Запрещено записью %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="359"/>
        <location filename="../src/director/baclmatrix.cpp" line="367"/>
        <source>The console has no ACL of this type: nothing is allowed.</source>
        <translation>У консоли нет ACL этого типа: ничего не разрешено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="360"/>
        <location filename="../src/director/baclmatrix.cpp" line="368"/>
        <source>No entry of the ACL allows it.</source>
        <translation>Ни одна запись ACL этого не разрешает.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="386"/>
        <source>Console %1</source>
        <translation>Консоль %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="387"/>
        <source>Profiles: none</source>
        <translation>Профили: нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="388"/>
        <source>Profiles: %1</source>
        <translation>Профили: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="390"/>
        <source>Locked: its catalog ACL could not be read.</source>
        <translation>Заблокирована: не удалось прочитать её ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="393"/>
        <source>loaded</source>
        <translation>загружена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="394"/>
        <source>off</source>
        <translation>отключена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="395"/>
        <source>table missing</source>
        <translation>нет таблицы</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="396"/>
        <source>failed</source>
        <translation>ошибка</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="397"/>
        <source>Catalog ACL: %1</source>
        <translation>ACL каталога: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="398"/>
        <source>Catalog ACL: %1 (%2)</source>
        <translation>ACL каталога: %1 (%2)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrix.cpp" line="403"/>
        <source>%n catalog ACL entry(s) narrow the file ACL.</source>
        <translation>
            <numerusform>%n запись ACL каталога сужает ACL файла.</numerusform>
            <numerusform>%n записи ACL каталога сужают ACL файла.</numerusform>
            <numerusform>%n записей ACL каталога сужают ACL файла.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="408"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="408"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="418"/>
        <source>The Director did not answer with profiles: %1</source>
        <translation>Director ответил не списком профилей: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="425"/>
        <source>The reply holds no profiles.</source>
        <translation>В ответе нет профилей.</translation>
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
        <translation>Изменить ACL каталога</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="68"/>
        <source>These changes of the catalog ACL are sent to the Director, one after the other. The file ACL is not changed. The first change the Director refuses stops the rest.</source>
        <translation>Эти изменения ACL каталога отправляются в Director одно за другим. ACL файла не изменяется. Первое изменение, отклонённое Director, останавливает остальные.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="80"/>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="102"/>
        <source>New console</source>
        <translation>Новая консоль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="104"/>
        <source>The Director creates a Console resource with this name, password and profile (configure add console). The profile&apos;s ACLs are the frame of the console; the matrix can narrow them further through the catalog ACL afterwards.</source>
        <translation>Director создаёт ресурс Console с этим именем, паролем и профилем (configure add console). ACL профиля — рамка консоли; затем матрица может дополнительно сузить их через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="116"/>
        <source>e.g. backup-viewer</source>
        <translation>напр. backup-viewer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="117"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="216"/>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="123"/>
        <source>New</source>
        <translation>Новый</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="124"/>
        <source>Generates another password</source>
        <translation>Создаёт другой пароль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="127"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="193"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="128"/>
        <source>Copies the password to the clipboard</source>
        <translation>Копирует пароль в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="133"/>
        <source>&amp;Password:</source>
        <translation>&amp;Пароль:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="139"/>
        <source>The Profile resource whose ACLs the console gets</source>
        <translation>Ресурс Profile, ACL которого получает консоль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="140"/>
        <source>P&amp;rofile:</source>
        <translation>П&amp;рофиль:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="142"/>
        <source>Copy the password now: whoever uses the console needs it, and it cannot be read back here.</source>
        <translation>Скопируйте пароль сейчас: он нужен тому, кто будет пользоваться консолью, и здесь его нельзя прочитать снова.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="150"/>
        <source>Create</source>
        <translation>Создать</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="154"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="233"/>
        <source>Enter a name.</source>
        <translation>Введите имя.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="155"/>
        <source>A console of this name exists.</source>
        <translation>Консоль с таким именем уже существует.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="156"/>
        <source>Choose a profile.</source>
        <translation>Выберите профиль.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="177"/>
        <source>Proposed configuration text</source>
        <translation>Предлагаемый текст конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="179"/>
        <source>Profiles are changed in the configuration files of the Director only. Put this text in place of the Profile resource in its file (the path is a suggestion), check it with the Director&apos;s test (-t) and reload; then Refresh shows the new state. Comments of the old resource are not in this text.</source>
        <translation>Профили изменяются только в файлах конфигурации Director. Вставьте этот текст вместо ресурса Profile в его файле (путь — лишь предложение), проверьте тестом Director (-t) и перезагрузите; затем «Обновить» покажет новое состояние. Комментариев старого ресурса в этом тексте нет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="204"/>
        <source>New profile</source>
        <translation>Новый профиль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="206"/>
        <source>The new profile is a proposal in the matrix: change its cells, then &quot;Config text...&quot; shows its Profile resource to put into the Director&apos;s configuration. Nothing is sent.</source>
        <translation>Новый профиль — это предложение в матрице: измените его ячейки, затем «Текст конфигурации...» покажет его ресурс Profile для вставки в конфигурацию Director. Ничего не отправляется.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="215"/>
        <source>e.g. restore-only</source>
        <translation>напр. restore-only</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="218"/>
        <source>&amp;Description:</source>
        <translation>&amp;Описание:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="220"/>
        <source>(empty: nothing allowed)</source>
        <translation>(пусто: ничего не разрешено)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="222"/>
        <source>The ACL entries the new profile starts with</source>
        <translation>Записи ACL, с которых начинается новый профиль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="223"/>
        <source>&amp;Start as a copy of:</source>
        <translation>&amp;Начать как копию:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="229"/>
        <source>Add to the matrix</source>
        <translation>Добавить в матрицу</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="234"/>
        <source>A profile of this name exists.</source>
        <translation>Профиль с таким именем уже существует.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="253"/>
        <source>(no entries: no restriction)</source>
        <translation>(нет записей: без ограничений)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="281"/>
        <source>ACL matrix of the consoles</source>
        <translation>Матрица ACL консолей</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="287"/>
        <source>Consoles</source>
        <translation>Консоли</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="288"/>
        <source>Profiles</source>
        <translation>Профили</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="289"/>
        <source>Consoles: what each console may use, the catalog ACL can be changed. Profiles: the ACLs of the Profile resources, changed as proposed configuration text.</source>
        <translation>Консоли: что может использовать каждая консоль, ACL каталога можно изменить. Профили: ACL ресурсов Profile, изменяемые в виде предлагаемого текста конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="297"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="299"/>
        <source>Filter resources</source>
        <translation>Фильтр ресурсов</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="302"/>
        <source>Only rows where the consoles differ</source>
        <translation>Только строки, где консоли различаются</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="305"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="306"/>
        <source>Reads the ACLs of every console again (.acl list)</source>
        <translation>Заново читает ACL всех консолей (.acl list)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="308"/>
        <source>Reading the ACLs of the consoles...</source>
        <translation>Чтение ACL консолей...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="312"/>
        <source>New console...</source>
        <translation>Новая консоль...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="313"/>
        <source>Creates a console on the Director with a password and a profile (configure add console); it appears as a new column</source>
        <translation>Создаёт в Director консоль с паролем и профилем (configure add console); она появляется как новый столбец</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="317"/>
        <source>New profile...</source>
        <translation>Новый профиль...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="318"/>
        <source>Adds a proposed profile as a new column; &quot;Config text...&quot; shows its Profile resource to put into the configuration</source>
        <translation>Добавляет предлагаемый профиль как новый столбец; «Текст конфигурации...» показывает его ресурс Profile для вставки в конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="325"/>
        <source>ACL matrix: resources by console</source>
        <translation>Матрица ACL: ресурсы по консолям</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="341"/>
        <source>✓ allowed   ✗ denied by an entry with &quot;!&quot;   · not allowed   ⊘ allowed by the file ACL, narrowed by the catalog ACL   — console locked. A cell&apos;s tool tip says why; a console&apos;s tool tip has its profiles, its catalog ACL and its Where and Plugin Options ACLs.</source>
        <translation>✓ разрешено   ✗ запрещено записью с «!»   · не разрешено   ⊘ разрешено ACL файла, сужено ACL каталога   — консоль заблокирована. Подсказка ячейки объясняет причину; подсказка консоли показывает её профили, ACL каталога и ACL Where и Plugin Options.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="421"/>
        <source>The profiles could not be read: %1</source>
        <translation>Не удалось прочитать профили: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="446"/>
        <source>%n profile(s). Profiles are changed in the configuration files: &quot;Config text...&quot; proposes the new text.</source>
        <translation>
            <numerusform>%n профиль. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
            <numerusform>%n профиля. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
            <numerusform>%n профилей. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="461"/>
        <source>Config text...</source>
        <translation>Текст конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="463"/>
        <source>Shows the new text of every changed profile, to put into the Director&apos;s configuration</source>
        <translation>Показывает новый текст каждого изменённого профиля для вставки в конфигурацию Director</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="467"/>
        <source>A double click (or Space) on a cell turns it in the proposal: ✓ becomes denied, · or ✗ allowed. Changed cells are underlined; &quot;Config text...&quot; shows the new Profile resources. Nothing is sent: a person puts the text in.</source>
        <translation>Двойной щелчок (или пробел) по ячейке меняет её в предложении: ✓ становится запрещённым, · или ✗ — разрешённым. Изменённые ячейки подчёркнуты; «Текст конфигурации...» показывает новые ресурсы Profile. Ничего не отправляется: текст вставляет человек.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="470"/>
        <source>A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows it again. Changed cells are underlined until they are applied. What the file ACL denies is changed in the editor.</source>
        <translation>Двойной щелчок (или пробел) по ✓ запрещает через ACL каталога, по ⊘ снова разрешает. Изменённые ячейки подчёркнуты, пока изменения не применены. То, что запрещает ACL файла, меняется в редакторе.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="363"/>
        <source>Discard</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="364"/>
        <source>Forgets the changes that were not applied</source>
        <translation>Забывает непримененные изменения</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="367"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="461"/>
        <source>Apply...</source>
        <translation>Применить...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="368"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="464"/>
        <source>Shows every change of the catalog ACL once and sends them after your confirmation (.acl set / .acl delete)</source>
        <translation>Показывает каждое изменение ACL каталога один раз и отправляет их после вашего подтверждения (.acl set / .acl delete)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="358"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="359"/>
        <source>Closes the matrix; changes that were not applied are discarded</source>
        <translation>Закрывает матрицу; непримененные изменения отбрасываются</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="408"/>
        <source>%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).</source>
        <translation>
            <numerusform>%n консоль. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
            <numerusform>%n консоли. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
            <numerusform>%n консолей. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="410"/>
        <source>%n console(s). This Director reports the file ACL only; the GUI decided the cells from its entries.</source>
        <translation>
            <numerusform>%n консоль. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
            <numerusform>%n консоли. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
            <numerusform>%n консолей. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="475"/>
        <source>No profiles read yet: Refresh reads them.</source>
        <translation>Профили ещё не прочитаны: «Обновить» их прочитает.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="505"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="531"/>
        <source>Profile %1</source>
        <translation>Профиль %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="511"/>
        <source>Profile %1 is proposed: change its cells, then &quot;Config text...&quot; shows it.</source>
        <translation>Профиль %1 предложен: измените его ячейки, затем «Текст конфигурации...» покажет его.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="532"/>
        <source>Proposed: not on the Director yet.</source>
        <translation>Предложен: ещё нет в Director.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="534"/>
        <source>Used by no console</source>
        <translation>Не используется ни одной консолью</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="534"/>
        <source>Used by: %1</source>
        <translation>Используется: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="537"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="537"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="559"/>
        <source>The proposal stays until the Director has it: after the text is in place and the Director reloaded, Refresh shows it.</source>
        <translation>Предложение сохраняется, пока Director его не получит: когда текст вставлен и Director перезагружен, «Обновить» его покажет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="565"/>
        <source>The ACLs could not be read: %1</source>
        <translation>Не удалось прочитать ACL: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="602"/>
        <source>In the proposal: allowed.</source>
        <translation>В предложении: разрешено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="603"/>
        <source>In the proposal: denied.</source>
        <translation>В предложении: запрещено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="604"/>
        <source>Double click: change it in the proposed configuration text.</source>
        <translation>Двойной щелчок: изменить в предлагаемом тексте конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="626"/>
        <source>Not applied yet: allowed again through the catalog ACL.</source>
        <translation>Ещё не применено: снова разрешено через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="627"/>
        <source>Not applied yet: denied through the catalog ACL.</source>
        <translation>Ещё не применено: запрещено через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="630"/>
        <source>Double click: deny it through the catalog ACL.</source>
        <translation>Двойной щелчок: запретить через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="631"/>
        <source>Double click: allow it again.</source>
        <translation>Двойной щелчок: снова разрешить.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="663"/>
        <source>Console %1 is locked: its catalog ACL cannot be changed until it can be read.</source>
        <translation>Консоль %1 заблокирована: её ACL каталога нельзя изменить, пока её не удаётся прочитать.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="664"/>
        <source>%1 is not allowed by the file ACL of console %2: that is changed in the editor, not here.</source>
        <translation>%1 не разрешено ACL файла консоли %2: это меняется в редакторе, а не здесь.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="697"/>
        <source>Discard the changes?</source>
        <translation>Отбросить изменения?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="698"/>
        <source>%n change(s) of the catalog ACL were not applied. Discard them and close?</source>
        <translation>
            <numerusform>%n изменение ACL каталога не применено. Отбросить и закрыть?</numerusform>
            <numerusform>%n изменения ACL каталога не применены. Отбросить и закрыть?</numerusform>
            <numerusform>%n изменений ACL каталога не применено. Отбросить и закрыть?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="710"/>
        <source>Discard the proposals?</source>
        <translation>Отбросить предложения?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="711"/>
        <source>%n profile proposal(s) are only in the matrix. Discard them and close?</source>
        <translation>
            <numerusform>%n предложение профиля есть только в матрице. Отбросить и закрыть?</numerusform>
            <numerusform>%n предложения профилей есть только в матрице. Отбросить и закрыть?</numerusform>
            <numerusform>%n предложений профилей есть только в матрице. Отбросить и закрыть?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="751"/>
        <source>Creating console %1...</source>
        <translation>Создание консоли %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="762"/>
        <source>Console %1 was not created: %2</source>
        <translation>Консоль %1 не создана: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="765"/>
        <source>Console %1 was created.</source>
        <translation>Консоль %1 создана.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="780"/>
        <source>The changes were discarded.</source>
        <translation>Изменения отменены.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="807"/>
        <source>Console %1, %2:</source>
        <translation>Консоль %1, %2:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="808"/>
        <source>  now: %1</source>
        <translation>  сейчас: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="809"/>
        <source>  new: %1</source>
        <translation>  новое: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="811"/>
        <source>%1 of %2</source>
        <translation>%1 консоли %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="816"/>
        <source>Nothing was sent: the changes were not confirmed.</source>
        <translation>Ничего не отправлено: изменения не подтверждены.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="828"/>
        <source>Changed: %1.</source>
        <translation>Изменено: %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="831"/>
        <source>Changing %1...</source>
        <translation>Изменение %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="762"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="840"/>
        <source>refused</source>
        <translation>отклонено</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="798"/>
        <source>Nothing was sent: the only entry &quot;%1&quot; has a comma outside brackets, which the Director would split into two entries.</source>
        <translation>Ничего не отправлено: единственная запись «%1» содержит запятую вне скобок, и Director разделит её на две записи.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="851"/>
        <source>nothing</source>
        <translation>ничего</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="852"/>
        <source>Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.</source>
        <translation>Не удалось изменить %1: %2. Изменено ранее: %3. Дальше ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="877"/>
        <source>%1 (new)</source>
        <translation>%1 (новый)</translation>
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
        <translation>Каталог</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="295"/>
        <source>Symlink</source>
        <translation>Символическая ссылка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="296"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="359"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="360"/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="361"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="362"/>
        <source>Modified</source>
        <translation>Изменён</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="572"/>
        <source>JSON parse error: %1</source>
        <translation>Ошибка разбора JSON: %1</translation>
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
        <translation>Изменить существующий Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="40"/>
        <source>%1 cannot change an existing schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий Schedule через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="44"/>
        <source>Nothing was changed. A schedule can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. Schedule также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="53"/>
        <source>Prune and purge all volumes</source>
        <translation>Prune и purge для всех Volumes</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="56"/>
        <source>%1 has no command that purges all volumes at once; purge works on named volumes only and ignores retention periods.</source>
        <translation>У %1 нет команды, выполняющей purge для всех Volumes сразу; purge работает только с указанными по имени Volumes и игнорирует сроки хранения.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="59"/>
        <source>&quot;Prune only&quot; applies the retention period of every volume; a single volume can be purged by its name with the purge command.</source>
        <translation>«Только prune» применяет срок хранения каждого Volume; для отдельного Volume можно выполнить purge по его имени командой purge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="66"/>
        <source>Send a FileSet to the Director</source>
        <translation>Отправить FileSet на Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="69"/>
        <source>%1 cannot take a FileSet through the console, because its configure command does not accept nested blocks (Include, Options, Exclude).</source>
        <translation>%1 не может принять FileSet через консоль, так как его команда configure не принимает вложенные блоки (Include, Options, Exclude).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="72"/>
        <source>You can copy the configuration text and place it on the Director yourself.</source>
        <translation>Вы можете скопировать текст конфигурации и разместить его на Director самостоятельно.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="79"/>
        <source>Delete a schedule</source>
        <translation>Удалить Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="82"/>
        <source>%1 cannot delete a schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить Schedule через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="86"/>
        <source>Nothing was deleted. A schedule can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. Schedule также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="95"/>
        <source>Create a schedule</source>
        <translation>Создать Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="98"/>
        <source>%1 cannot create a schedule.</source>
        <translation>%1 не может создать Schedule.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="110"/>
        <source>Change an existing job</source>
        <translation>Изменить существующий Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="113"/>
        <source>%1 cannot change an existing job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий Job через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="117"/>
        <source>Nothing was changed. A job can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. Job также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="126"/>
        <source>Delete a job</source>
        <translation>Удалить Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="129"/>
        <source>%1 cannot delete a job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить Job через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="132"/>
        <source>Nothing was deleted. A job can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. Job также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="141"/>
        <source>Create a job</source>
        <translation>Создать Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="144"/>
        <source>%1 cannot create a job.</source>
        <translation>%1 не может создать Job.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="152"/>
        <source>Change an existing JobDefs</source>
        <translation>Изменить существующий JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="155"/>
        <source>%1 cannot change an existing JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий JobDefs через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="159"/>
        <source>Nothing was changed. A JobDefs can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. JobDefs также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="168"/>
        <source>Delete a JobDefs</source>
        <translation>Удалить JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="171"/>
        <source>%1 cannot delete a JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить JobDefs через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="175"/>
        <source>Nothing was deleted. A JobDefs can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. JobDefs также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="184"/>
        <source>Create a JobDefs</source>
        <translation>Создать JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="187"/>
        <source>%1 cannot create a JobDefs.</source>
        <translation>%1 не может создать JobDefs.</translation>
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
        <translation>Этот Director (Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="338"/>
        <source>This Director (Bareos %1)</source>
        <translation>Этот Director (Bareos %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="342"/>
        <source>This Director (Onesimus Director)</source>
        <translation>Этот Director (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="344"/>
        <source>This Director (Onesimus Director %1)</source>
        <translation>Этот Director (Onesimus Director %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="350"/>
        <source>This Director</source>
        <translation>Этот Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="362"/>
        <source>%1 does not offer this to this session: its configuration capabilities do not list it. The console needs the command ACL &quot;configure&quot; and the ACL of the resource type, or the Director is older than its configuration methods.</source>
        <translation>%1 не предоставляет эту функцию данному сеансу: её нет в списке его возможностей конфигурации. Консоли нужен командный ACL «configure» и ACL типа ресурса, либо Director старше, чем его методы конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="371"/>
        <source>The capabilities query failed: %1.</source>
        <translation>Ошибка запроса возможностей: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="375"/>
        <source>The capabilities were not received for this session.</source>
        <translation>Возможности для этого сеанса не получены.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="384"/>
        <source>The Director lists for this session: %1.</source>
        <translation>Director сообщает для этого сеанса: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="401"/>
        <source>The Onesimus Director supports this over its gRPC interface.</source>
        <translation>Onesimus Director поддерживает это через свой интерфейс gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="404"/>
        <source>Connect to it over gRPC to use this.</source>
        <translation>Чтобы использовать это, подключитесь к нему через gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="411"/>
        <source>The Onesimus Director supports this.</source>
        <translation>Onesimus Director поддерживает это.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="416"/>
        <source>Support for this is planned for a later version of the Onesimus Director.</source>
        <translation>Поддержка этого запланирована в одной из следующих версий Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="419"/>
        <source>The Onesimus Director is planned to support this.</source>
        <translation>Поддержка этого в Onesimus Director запланирована.</translation>
    </message>
</context>
<context>
    <name>BCapabilityNotice</name>
    <message>
        <location filename="../src/bcapabilitynotice.cpp" line="68"/>
        <source>Do not show again</source>
        <translation>Больше не показывать</translation>
    </message>
</context>
<context>
    <name>BCatalogResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>DB Driver</source>
        <translation>Драйвер БД</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="451"/>
        <source>DB Name</source>
        <translation>Имя БД</translation>
    </message>
</context>
<context>
    <name>BCertificateGenerator</name>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="33"/>
        <source>Generate TLS Certificates</source>
        <translation>Создание сертификатов TLS</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="45"/>
        <source>Generate TLS certificates for secure Bareos communication.
This will create a CA certificate, client certificate, and private key.</source>
        <translation>Создание сертификатов TLS для защищённого обмена данными Bareos.
Будут созданы сертификат CA, сертификат клиента и закрытый ключ.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="53"/>
        <source>Certificate Settings</source>
        <translation>Параметры сертификата</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="63"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="66"/>
        <source>Output Directory:</source>
        <translation>Выходной каталог:</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="71"/>
        <source>e.g., onesimus-client</source>
        <translation>напр., onesimus-client</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="72"/>
        <source>Common Name (CN):</source>
        <translation>Общее имя (CN):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="77"/>
        <source>e.g., My Company</source>
        <translation>напр., Моя компания</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="78"/>
        <source>Organization (O):</source>
        <translation>Организация (O):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="83"/>
        <source>Days (e.g., 3650 = 10 years)</source>
        <translation>Дни (напр., 3650 = 10 лет)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="84"/>
        <source>Valid for (days):</source>
        <translation>Срок действия (дней):</translation>
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
        <translation>Перезаписывать существующие файлы</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="134"/>
        <source>Generate Certificates</source>
        <translation>Создать сертификаты</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="136"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="150"/>
        <source>Select Output Directory</source>
        <translation>Выбор выходного каталога</translation>
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
        <translation>Создание...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="199"/>
        <source>Please fill in all required fields</source>
        <translation>Заполните все обязательные поля</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="201"/>
        <location filename="../src/bcertificategenerator.cpp" line="208"/>
        <location filename="../src/bcertificategenerator.cpp" line="217"/>
        <location filename="../src/bcertificategenerator.cpp" line="232"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="206"/>
        <source>The PFX passwords do not match.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="215"/>
        <source>Please enter a password to protect the PFX file.</source>
        <translation type="unfinished">Введите пароль для защиты файла PFX.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="230"/>
        <source>Failed to create output directory</source>
        <translation>Не удалось создать выходной каталог</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="252"/>
        <source>Certificate files already exist. Enable &apos;Overwrite&apos; to replace.</source>
        <translation>Файлы сертификатов уже существуют. Включите «Перезаписывать», чтобы заменить их.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="254"/>
        <source>Files exist</source>
        <translation>Файлы существуют</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="264"/>
        <source>Generating CA private key (4096 bit RSA)...</source>
        <translation>Генерация закрытого ключа CA (RSA 4096 бит)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="174"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="272"/>
        <source>Generating CA certificate...</source>
        <translation>Создание сертификата CA...</translation>
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
        <translation>Генерация закрытого ключа клиента (RSA 4096 бит)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="306"/>
        <source>Saving client private key...</source>
        <translation>Сохранение закрытого ключа клиента...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="313"/>
        <source>Generating client certificate...</source>
        <translation>Создание сертификата клиента...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="346"/>
        <source>Certificates generated successfully!</source>
        <translation>Сертификаты успешно созданы!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="347"/>
        <source>CA Certificate: %1</source>
        <translation>Сертификат CA: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="349"/>
        <source>CA Key (encrypted): %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="351"/>
        <source>Client Certificate: %1</source>
        <translation>Сертификат клиента: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="352"/>
        <source>Private Key: %1</source>
        <translation>Закрытый ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="354"/>
        <source>PFX File: %1</source>
        <translation>Файл PFX: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="357"/>
        <source>Success!</source>
        <translation>Готово!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="360"/>
        <source>Use Certificates</source>
        <translation>Использовать сертификаты</translation>
    </message>
</context>
<context>
    <name>BCheckableHeaderView</name>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="121"/>
        <source>Show All Columns</source>
        <translation>Показать все столбцы</translation>
    </message>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="131"/>
        <source>Reset Columns</source>
        <translation>Сбросить столбцы</translation>
    </message>
</context>
<context>
    <name>BCleanupDialog</name>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="45"/>
        <source>Database Cleanup</source>
        <translation>Очистка базы данных</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="87"/>
        <source>The connection to the Director was lost</source>
        <translation>Соединение с Director потеряно</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="124"/>
        <source>This tool helps maintain your Bareos database by removing old or unnecessary data.
⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding.</source>
        <translation>Этот инструмент помогает обслуживать базу данных Bareos, удаляя старые или ненужные данные.
⚠️ Внимание: операции очистки нельзя отменить. Внимательно проверьте параметры перед продолжением.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="134"/>
        <source>Cleanup Options</source>
        <translation>Параметры очистки</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="139"/>
        <source>Remove old full backups</source>
        <translation>Удалить старые полные резервные копии</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="142"/>
        <source>Delete old successful full backups of a job if newer full backups of the same client, job and FileSet exist. A full backup that incremental or differential backups still depend on is kept.</source>
        <translation>Удалять старые успешные полные резервные копии Job, если существуют более новые полные резервные копии того же клиента, Job и FileSet. Полная резервная копия, от которой ещё зависят инкрементные или дифференциальные копии, сохраняется.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="147"/>
        <source>Keep last:</source>
        <translation>Хранить последние:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="154"/>
        <source> full backups</source>
        <translation> полных копий</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="155"/>
        <source>Number of recent full backups to keep per client, job and FileSet</source>
        <translation>Количество последних полных резервных копий, сохраняемых для каждого клиента, Job и FileSet</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="162"/>
        <source>Remove successful jobs with no files</source>
        <translation>Удалить успешные Jobs без файлов</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="164"/>
        <source>Delete jobs that completed successfully but backed up 0 files</source>
        <translation>Удалять Jobs, завершившиеся успешно, но сохранившие 0 файлов</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="169"/>
        <source>Prune volumes</source>
        <translation>Prune для Volumes</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="171"/>
        <source>Apply retention policy to volumes (removes expired job records)</source>
        <translation>Применить политику хранения к Volumes (удаляет просроченные записи о Jobs)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="174"/>
        <source>Action:</source>
        <translation>Действие:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="178"/>
        <source>Prune only</source>
        <translation>Только prune</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="179"/>
        <source>Prune and purge</source>
        <translation>Prune и purge</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="180"/>
        <source>Prune removes the expired job records of all volumes according to their retention period</source>
        <translation>Prune удаляет просроченные записи о Jobs всех Volumes согласно их сроку хранения</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="198"/>
        <source>Not available: Bareos has no command that purges all volumes at once. Purge works on named volumes only and ignores retention periods, so it is not sent from this dialog.</source>
        <translation>Недоступно: в Bareos нет команды, выполняющей purge для всех Volumes сразу. Purge работает только с указанными по имени Volumes и игнорирует сроки хранения, поэтому из этого диалога не отправляется.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="214"/>
        <source>Remove failed jobs older than</source>
        <translation>Удалить неудачные Jobs старше</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="216"/>
        <source>Delete jobs that failed or were canceled</source>
        <translation>Удалять Jobs, завершившиеся с ошибкой или отменённые</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="223"/>
        <source> days</source>
        <translation> дн.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="259"/>
        <source>Select cleanup options and click &apos;Analyze&apos; to preview</source>
        <translation>Выберите параметры очистки и нажмите «Анализ» для предварительного просмотра</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="264"/>
        <source>Analysis / Log</source>
        <translation>Анализ / Журнал</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="287"/>
        <source>Analyze</source>
        <translation>Анализ</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="288"/>
        <source>Preview what would be cleaned up</source>
        <translation>Предварительный просмотр того, что будет очищено</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="292"/>
        <source>Cleanup</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="294"/>
        <source>Perform cleanup operations (requires analysis first)</source>
        <translation>Выполнить операции очистки (сначала требуется анализ)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="282"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="88"/>
        <source>the connection to the Director was lost</source>
        <translation>соединение с Director потеряно</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="191"/>
        <source>Prune all volumes with &apos;Cleanup&apos;, and purge the volumes of a pool with a chosen status after a preview. Purge ignores retention periods.</source>
        <translation>Prune для всех Volumes кнопкой «Очистить» и purge для Volumes одного Pool с выбранным статусом после предпросмотра. Purge игнорирует сроки хранения.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="240"/>
        <source>Volumes were purged. Refresh the job list to see the remaining jobs.</source>
        <translation>Для Volumes выполнен purge. Обновите список Jobs, чтобы увидеть оставшиеся Jobs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="400"/>
        <source>The options were changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>Параметры изменены. Приведённый выше анализ больше не действителен; нажмите «Анализ» ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="409"/>
        <source>The job list has changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>Список Jobs изменился. Приведённый выше анализ больше не действителен; нажмите «Анализ» ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="420"/>
        <location filename="../src/bcleanupdialog.cpp" line="778"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="421"/>
        <location filename="../src/bcleanupdialog.cpp" line="779"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Сначала подключитесь к Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="427"/>
        <source>No Options Selected</source>
        <translation>Параметры не выбраны</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="428"/>
        <source>Please select at least one cleanup option.</source>
        <translation>Выберите хотя бы один параметр очистки.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="436"/>
        <source>=== Analysis ===</source>
        <translation>=== Анализ ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="449"/>
        <location filename="../src/bcleanupdialog.cpp" line="498"/>
        <source>Analysis Required</source>
        <translation>Требуется анализ</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="450"/>
        <source>Please run analysis before cleanup.</source>
        <translation>Перед очисткой выполните анализ.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="457"/>
        <source>⚠️ This will permanently delete data from the Bareos catalog.

</source>
        <translation>⚠️ Данные будут безвозвратно удалены из каталога Bareos.

</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="458"/>
        <source>Operations to perform:
</source>
        <translation>Будут выполнены операции:
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="461"/>
        <source>- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)
</source>
        <translation>- Удалить %1 старых Jobs полного резервного копирования (сохраняя %2 на каждого клиента, Job и FileSet)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="467"/>
        <source>- Delete %1 empty jobs (successful with 0 files)
</source>
        <translation>- Удалить %1 пустых Jobs (успешных, с 0 файлов)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="472"/>
        <source>- Delete %1 failed jobs older than %2 days
</source>
        <translation>- Удалить %1 неудачных Jobs старше %2 дн.
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="479"/>
        <source>  Job records to delete in total: %1
</source>
        <translation>  Всего записей о Jobs к удалению: %1
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="483"/>
        <source>- Prune all volumes (command: %1)
</source>
        <translation>- Prune для всех Volumes (команда: %1)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="486"/>
        <source>
Do you want to continue?</source>
        <translation>
Продолжить?</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="489"/>
        <source>Confirm Cleanup</source>
        <translation>Подтверждение очистки</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="499"/>
        <source>The job list changed in the meantime. Nothing was deleted; please run the analysis again.</source>
        <translation>Тем временем список Jobs изменился. Ничего не удалено; выполните анализ ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="512"/>
        <source>Stopping: no further commands are sent after the answer that is still awaited.</source>
        <translation>Остановка: после ожидаемого ещё ответа команды больше не отправляются.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="537"/>
        <source>Analyzing cleanup options...</source>
        <translation>Анализ параметров очистки...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="542"/>
        <source>⚠ No job data available!</source>
        <translation>⚠ Нет данных о Jobs!</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="543"/>
        <source>Please load the job list first (click &apos;Refresh&apos; in the main window).</source>
        <translation>Сначала загрузите список Jobs (нажмите «Обновить» в главном окне).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="545"/>
        <source>No job data. Please refresh job list first.</source>
        <translation>Нет данных о Jobs. Сначала обновите список Jobs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="552"/>
        <source>Looking at the %1 jobs currently loaded in the job list. Jobs that are not loaded there are not considered.</source>
        <translation>Рассматриваются %1 Jobs, загруженных сейчас в список Jobs. Не загруженные туда Jobs не учитываются.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="569"/>
        <source>✓ Will prune all volumes (apply each volume&apos;s retention period)</source>
        <translation>✓ Будет выполнен prune для всех Volumes (применяется срок хранения каждого Volume)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="570"/>
        <source>  Command: %1</source>
        <translation>  Команда: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="572"/>
        <source>  The purge is not part of &apos;Cleanup&apos;: use &apos;Preview&apos; and &apos;Purge&apos; in &apos;Purge volumes&apos;.</source>
        <translation>  Purge не входит в «Очистить»: используйте «Предпросмотр» и «Purge» в разделе «Purge для Volumes».</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="576"/>
        <source>  &apos;Prune and purge&apos; is not available and will not be sent.</source>
        <translation>  «Prune и purge» недоступно и отправлено не будет.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="581"/>
        <source>=== Analysis Summary ===</source>
        <translation>=== Итоги анализа ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="585"/>
        <source>Total jobs to delete: %1</source>
        <translation>Всего Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="587"/>
        <source>  - Old full backups: %1 (%2)</source>
        <translation>  - Старые полные резервные копии: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="592"/>
        <source>  - Empty jobs: %1</source>
        <translation>  - Пустые Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="595"/>
        <source>  - Failed jobs: %1</source>
        <translation>  - Неудачные Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="598"/>
        <source>No jobs found matching cleanup criteria.</source>
        <translation>Jobs, соответствующие критериям очистки, не найдены.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="605"/>
        <source>Analysis complete. Click &apos;Cleanup&apos; to proceed.</source>
        <translation>Анализ завершён. Нажмите «Очистить», чтобы продолжить.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="607"/>
        <source>Analysis complete. Nothing to clean up.</source>
        <translation>Анализ завершён. Очищать нечего.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="613"/>
        <source>Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...</source>
        <translation>Анализ старых полных резервных копий (сохраняются %1 последних на каждого клиента, Job и FileSet)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="621"/>
        <source>at least one full backup must be kept</source>
        <translation>необходимо сохранить хотя бы одну полную резервную копию</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="624"/>
        <source>%1 job rows have no job ID</source>
        <translation>у %1 строк Jobs нет ID Job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="627"/>
        <source>%1 job rows have no job type, so backups cannot be told from other jobs</source>
        <translation>у %1 строк Jobs нет типа Job, поэтому резервные копии нельзя отличить от других Jobs</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="631"/>
        <source>%1 backup jobs have no client or job name</source>
        <translation>у %1 Jobs резервного копирования нет клиента или имени Job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="636"/>
        <source>  ⚠ No old full backups are selected: %1.</source>
        <translation>  ⚠ Старые полные резервные копии не выбраны: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="640"/>
        <source>  Note: the FileSet of a job is taken from its current job configuration, not from the catalog record of each run.</source>
        <translation>  Примечание: FileSet Job берётся из его текущей конфигурации, а не из записи каталога о каждом запуске.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="647"/>
        <source>%1 runs have no known FileSet</source>
        <translation>у %1 запусков FileSet неизвестен</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="650"/>
        <source>%1 runs have no status</source>
        <translation>у %1 запусков нет статуса</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="653"/>
        <source>%1 runs have a level other than Full, Differential or Incremental</source>
        <translation>у %1 запусков уровень отличается от Full, Differential или Incremental</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="656"/>
        <source>%1 runs have no readable start time</source>
        <translation>у %1 запусков время начала не читается</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="659"/>
        <source>  ⚠ Client &apos;%1&apos;, job &apos;%2&apos;: skipped, nothing selected (%3)</source>
        <translation>  ⚠ Client «%1», Job «%2»: пропущено, ничего не выбрано (%3)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="665"/>
        <source>  Client &apos;%1&apos;, job &apos;%2&apos;, FileSet &apos;%3&apos;: %4 full backups, will delete %5</source>
        <translation>  Client «%1», Job «%2», FileSet «%3»: полных резервных копий — %4, будет удалено %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="672"/>
        <source>    ... and %1 more</source>
        <translation>    ... и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="676"/>
        <source>    → Job %1: %2 (%3, %4)</source>
        <translation>    → Job %1: %2 (%3, %4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="679"/>
        <source>      its dependent jobs %1 are deleted by this cleanup as well</source>
        <translation>      зависящие от него Jobs %1 также удаляются этой очисткой</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="685"/>
        <source>    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it</source>
        <translation>    ⚠ Job %1 (%2) сохраняется: от него зависят инкрементные/дифференциальные Jobs %3</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="692"/>
        <source>✓ Found %1 old full backups to delete (%2)</source>
        <translation>✓ Найдено старых полных резервных копий к удалению: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="696"/>
        <source>  No old full backups found to delete</source>
        <translation>  Старые полные резервные копии к удалению не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702"/>
        <source>Analyzing empty jobs (successful with 0 files)...</source>
        <translation>Анализ пустых Jobs (успешных, с 0 файлов)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="707"/>
        <source>  → Job %1: %2 [%3] (%4)</source>
        <translation>  → Job %1: %2 [%3] (%4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="712"/>
        <location filename="../src/bcleanupdialog.cpp" line="750"/>
        <source>  ... and %1 more</source>
        <translation>  ... и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="716"/>
        <source>  %1 successful jobs have no file count and are not selected</source>
        <translation>  у %1 успешных Jobs нет количества файлов, они не выбраны</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="721"/>
        <source>✓ Found %1 empty jobs to delete</source>
        <translation>✓ Найдено пустых Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="723"/>
        <source>  No empty jobs found</source>
        <translation>  Пустые Jobs не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="731"/>
        <source>Analyzing failed jobs older than %1 days (before %2)...</source>
        <translation>Анализ неудачных Jobs старше %1 дн. (до %2)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="740"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="741"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="742"/>
        <source>Canceled</source>
        <translation>Отменено</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="743"/>
        <source>Non-fatal error</source>
        <translation>Нефатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="745"/>
        <source>  → Job %1: %2 [%3] (%4) - %5</source>
        <translation>  → Job %1: %2 [%3] (%4) - %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="754"/>
        <source>✓ Found %1 failed jobs to delete</source>
        <translation>✓ Найдено неудачных Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="756"/>
        <source>  No failed jobs found matching criteria</source>
        <translation>  Неудачные Jobs, соответствующие критериям, не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="766"/>
        <source>Stop</source>
        <translation>Остановить</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="766"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="784"/>
        <source>=== Starting Cleanup ===</source>
        <translation>=== Начало очистки ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="791"/>
        <source>Cleanup running: commands are sent one at a time...</source>
        <translation>Выполняется очистка: команды отправляются по одной...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="801"/>
        <source>Sent: %1</source>
        <translation>Отправлено: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="813"/>
        <source>(empty answer)</source>
        <translation>(пустой ответ)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="818"/>
        <source>✓ Job %1: deletion confirmed by the Director</source>
        <translation>✓ Job %1: удаление подтверждено Director</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="821"/>
        <source>✗ Job %1: the Director reported an error: %2</source>
        <translation>✗ Job %1: Director сообщил об ошибке: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="824"/>
        <source>? Job %1: the Director did not confirm the deletion. Answer: %2</source>
        <translation>? Job %1: Director не подтвердил удаление. Ответ: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="828"/>
        <source>? Job %1: result unknown (%2)</source>
        <translation>? Job %1: результат неизвестен (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="836"/>
        <source>Prune volumes: the Director answered without an error: %1</source>
        <translation>Prune для Volumes: Director ответил без ошибки: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="839"/>
        <source>✗ Prune volumes: the Director reported an error: %1</source>
        <translation>✗ Prune для Volumes: Director сообщил об ошибке: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="842"/>
        <source>? Prune volumes: result unknown (%1)</source>
        <translation>? Prune для Volumes: результат неизвестен (%1)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="845"/>
        <source>? Prune volumes: no usable answer: %1</source>
        <translation>? Prune для Volumes: нет пригодного ответа: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="855"/>
        <source>no answer from the Director within the time limit</source>
        <translation>нет ответа от Director в отведённое время</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="870"/>
        <source>=== Cleanup Result ===</source>
        <translation>=== Результат очистки ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="871"/>
        <source>Job deletions confirmed by the Director: %1</source>
        <translation>Удаления Jobs, подтверждённые Director: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="874"/>
        <source>Prune volumes: answered without an error (see the answer above)</source>
        <translation>Prune для Volumes: ответ без ошибки (см. ответ выше)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="877"/>
        <source>Commands the Director answered with an error: %1</source>
        <translation>Команды, на которые Director ответил ошибкой: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="880"/>
        <source>Commands with an unknown result: %1. Refresh the job list to see the actual state.</source>
        <translation>Команды с неизвестным результатом: %1. Обновите список Jobs, чтобы увидеть фактическое состояние.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="884"/>
        <source>Commands not sent because the run was stopped: %1</source>
        <translation>Команды, не отправленные из-за остановки: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="888"/>
        <source>Cleanup finished: the Director confirmed all %1 commands.</source>
        <translation>Очистка завершена: Director подтвердил все команды (%1).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="891"/>
        <source>Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. See the log.</source>
        <translation>Очистка не завершена: подтверждено %1, с ошибкой %2, неизвестно %3, не отправлено %4. См. журнал.</translation>
    </message>
</context>
<context>
    <name>BClientDetailsDialog</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="181"/>
        <source>Client Details - %1</source>
        <translation>Сведения о Client — %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="304"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="208"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="290"/>
        <source>General Information</source>
        <translation>Общие сведения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="297"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="299"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="397"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="305"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="306"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="307"/>
        <source>Version:</source>
        <translation>Версия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="308"/>
        <source>Operating System:</source>
        <translation>Операционная система:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="313"/>
        <source>Connection Information</source>
        <translation>Сведения о соединении</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="327"/>
        <source>Recent (%1)</source>
        <translation>Недавно (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="332"/>
        <source>Stale (%1)</source>
        <translation>Устарело (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="336"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="344"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="339"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="357"/>
        <source>Last backup:</source>
        <translation>Последняя резервная копия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="358"/>
        <source>Last Connection:</source>
        <translation>Последнее соединение:</translation>
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
        <translation>Политики хранения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="396"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="396"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="401"/>
        <source>Auto Prune:</source>
        <translation>Auto Prune:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="402"/>
        <source>File Retention:</source>
        <translation>File Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="403"/>
        <source>Job Retention:</source>
        <translation>Job Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="409"/>
        <source>Information</source>
        <translation>Сведения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="420"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="441"/>
        <source>Checking...</source>
        <translation type="unfinished">Проверка...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="424"/>
        <source>Not known: no client-initiated connection, client event or check holds now. The last backup above is an estimate, not a check.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="460"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="493"/>
        <source>Recent jobs for this client:</source>
        <translation>Последние Jobs этого клиента:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="497"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="536"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="559"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="545"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="546"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="548"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="550"/>
        <source>Total Jobs:</source>
        <translation>Всего Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="551"/>
        <source>Successful Jobs:</source>
        <translation>Успешных Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="552"/>
        <source>Failed Jobs:</source>
        <translation>Неудачных Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="553"/>
        <source>Total Data:</source>
        <translation>Всего данных:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="554"/>
        <source>Total Files:</source>
        <translation>Всего файлов:</translation>
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
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="101"/>
        <source>Blocked</source>
        <translation>Заблокирован</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="102"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="103"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="104"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="105"/>
        <source>Canceled</source>
        <translation>Отменён</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="106"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="136"/>
        <source>Job ID</source>
        <translation>ID Job</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="137"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="138"/>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="139"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="140"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="141"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="142"/>
        <source>Bytes</source>
        <translation>Байты</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="143"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
</context>
<context>
    <name>BClientResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="199"/>
        <source>Port</source>
        <translation>Порт</translation>
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
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="262"/>
        <source>Client: %1</source>
        <translation>Client: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="266"/>
        <source>Address: %1:%2</source>
        <translation>Адрес: %1:%2</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="276"/>
        <source>%1 bytes</source>
        <translation>%1 байт</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="295"/>
        <source>Connected while the client holds a connection to the Director (client-initiated connections from &quot;status director&quot;, or client events of the Onesimus Director); Online or Offline after a client event or &quot;Check now&quot;; otherwise the age of the last backup or contact, an estimate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="304"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="306"/>
        <source>Address</source>
        <translation>Адрес</translation>
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
        <translation>Нет соединения с Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="47"/>
        <source>Loading clients...</source>
        <translation>Загрузка клиентов...</translation>
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
        <translation type="unfinished">Подключено</translation>
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
        <translation>Порт FD: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="292"/>
        <source>From &quot;list backups client=&quot;: all backups of the client in the catalog, not only the jobs on the current page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="308"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="310"/>
        <source>Status</source>
        <translation type="unfinished">Статус</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="318"/>
        <source>Backups</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="312"/>
        <source>Operating System</source>
        <translation>Операционная система</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="112"/>
        <source>Checking the client now...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="314"/>
        <source>Version</source>
        <translation>Версия</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="316"/>
        <source>Last Connection</source>
        <translation>Последнее соединение</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="320"/>
        <source>Total Data</source>
        <translation>Всего данных</translation>
    </message>
</context>
<context>
    <name>BClientsWidget</name>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="40"/>
        <source>Auto</source>
        <translation>Авто</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="41"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="62"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="65"/>
        <source>Filter by client...</source>
        <translation>Фильтр по клиенту...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="72"/>
        <source>Last backup:</source>
        <translation>Последняя резервная копия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="73"/>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="74"/>
        <source>Recent</source>
        <translation>Недавно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="75"/>
        <source>Stale</source>
        <translation>Устарело</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="76"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="77"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="84"/>
        <source>Enable automatic refresh</source>
        <translation>Включить автообновление</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="97"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="100"/>
        <source>Total:</source>
        <translation>Всего:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="121"/>
        <source>Recent:</source>
        <translation>Недавно:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="122"/>
        <source>Clients whose last backup or contact is younger than the limit in the settings</source>
        <translation>Клиенты, у которых последняя резервная копия или контакт новее порога, заданного в настройках</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="124"/>
        <source>Stale:</source>
        <translation>Устарело:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="125"/>
        <source>Clients whose last backup or contact is older than the limit in the settings</source>
        <translation>Клиенты, у которых последняя резервная копия или контакт старше порога, заданного в настройках</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="127"/>
        <source>Never:</source>
        <translation>Никогда:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="128"/>
        <source>Clients without any known backup or contact</source>
        <translation>Клиенты без известных резервных копий или контактов</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="130"/>
        <source>Unknown:</source>
        <translation>Неизвестно:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="131"/>
        <source>Clients whose last backup time cannot be read</source>
        <translation>Клиенты, время последней резервной копии которых не удаётся прочитать</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="392"/>
        <source>Error parsing client data</source>
        <translation>Ошибка разбора данных клиентов</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientswidget.cpp" line="433"/>
        <source>%n client(s) loaded</source>
        <translation>
            <numerusform>Загружен %n клиент</numerusform>
            <numerusform>Загружено %n клиента</numerusform>
            <numerusform>Загружено %n клиентов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="543"/>
        <source>Details / Export...</source>
        <translation>Подробности / Экспорт...</translation>
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
        <translation>Импорт конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="62"/>
        <source>Configuration Source</source>
        <translation>Источник конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="66"/>
        <source>Select directory or ZIP file...</source>
        <translation>Выберите каталог или ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="70"/>
        <source>Directory...</source>
        <translation>Каталог...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="75"/>
        <source>ZIP File...</source>
        <translation>ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="80"/>
        <source>Parse</source>
        <translation>Разобрать</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="104"/>
        <source>Connection Preview</source>
        <translation>Предпросмотр соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="108"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="175"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="113"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="204"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="117"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="211"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="121"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="189"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="125"/>
        <source>TLS:</source>
        <translation>TLS:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="141"/>
        <source>Import</source>
        <translation>Импорт</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="148"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="172"/>
        <source>Connection Settings</source>
        <translation>Параметры соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="206"/>
        <source>hostname or IP address</source>
        <translation>имя хоста или IP-адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="207"/>
        <source>Enter the hostname or IP address of the Director to connect to</source>
        <translation>Введите имя хоста или IP-адрес Director, к которому нужно подключиться</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="215"/>
        <source>Port number (default: 9101)</source>
        <translation>Номер порта (по умолчанию: 9101)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="218"/>
        <source>Create connection profile</source>
        <translation>Создать профиль соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="318"/>
        <source>Reading ZIP archive...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="322"/>
        <source>No configuration resources found in the archive.</source>
        <translation type="unfinished">В архиве не найдено ресурсов конфигурации.</translation>
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
        <translation>Выбор каталога конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="271"/>
        <source>Ready to parse directory</source>
        <translation>Каталог готов к разбору</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="285"/>
        <source>Select Configuration ZIP Archive</source>
        <translation>Выбор ZIP-архива конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="287"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="292"/>
        <source>Ready to parse ZIP archive</source>
        <translation>ZIP-архив готов к разбору</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="307"/>
        <source>Parsing...</source>
        <translation>Разбор...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="350"/>
        <source>Parsed %1: %2</source>
        <translation>Разобрано %1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="357"/>
        <source>Parsing complete: %1 resources found</source>
        <translation>Разбор завершён: найдено ресурсов: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="387"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="390"/>
        <source>Parsing Error</source>
        <translation>Ошибка разбора</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="449"/>
        <source>(multiple addresses)</source>
        <translation>(несколько адресов)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="456"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="456"/>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="515"/>
        <source>Invalid Selection</source>
        <translation>Недопустимый выбор</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="516"/>
        <source>Please select both a Director and a Console.</source>
        <translation>Выберите и Director, и Console.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="521"/>
        <source>Invalid Address</source>
        <translation>Недопустимый адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="522"/>
        <source>Please enter the hostname or IP address of the Director.</source>
        <translation>Введите имя хоста или IP-адрес Director.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="560"/>
        <source>%1 &apos;%2&apos;: Missing required field &apos;%3&apos;</source>
        <translation>%1 «%2»: отсутствует обязательное поле «%3»</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="581"/>
        <source>&lt;b&gt;Validation Warnings (%1 total, showing first 10):&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Предупреждения проверки (всего %1, показаны первые 10):&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="583"/>
        <source>&lt;b&gt;Validation Warnings:&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Предупреждения проверки:&lt;/b&gt;&lt;br&gt;</translation>
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
        <translation>Настройка соединения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="88"/>
        <source>New Connection</source>
        <translation>Новое соединение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="198"/>
        <source>SHA-256 fingerprint of the server certificate:</source>
        <translation>Отпечаток SHA-256 сертификата сервера:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="205"/>
        <source>Server certificate changed</source>
        <translation>Сертификат сервера изменился</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="206"/>
        <source>The certificate of the Director at %1:%2 has changed since the last connection.</source>
        <translation>Сертификат Director по адресу %1:%2 изменился с момента последнего подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="208"/>
        <source>Previously trusted:</source>
        <translation>Ранее доверенный:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="210"/>
        <source>This is expected if the certificate was regenerated on the Director. Otherwise someone may be intercepting the connection.</source>
        <translation>Это ожидаемо, если сертификат был заново создан на Director. В противном случае кто-то может перехватывать соединение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="213"/>
        <source>Trust server certificate?</source>
        <translation>Доверять сертификату сервера?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="214"/>
        <source>The Director at %1:%2 uses a self-signed certificate that has not been trusted yet.</source>
        <translation>Director по адресу %1:%2 использует самоподписанный сертификат, которому ещё не выражено доверие.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="220"/>
        <source>The same fingerprint is announced in the local network (mDNS). This is not proof of the Director&apos;s identity: any device in the network can send such an announcement.</source>
        <translation>Тот же отпечаток объявлен в локальной сети (mDNS). Это не доказывает подлинность Director: такое объявление может отправить любое устройство в сети.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="225"/>
        <source>The fingerprint does NOT match the one announced in the network (mDNS)!</source>
        <translation>Отпечаток НЕ совпадает с объявленным в сети (mDNS)!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="228"/>
        <source>Before trusting it, compare the fingerprint with the one of the Director&apos;s gRPC certificate, obtained from the Director&apos;s administrator or read on the Director itself.</source>
        <translation>Прежде чем доверять, сравните отпечаток с отпечатком сертификата gRPC Director, полученным от администратора Director или прочитанным на самом Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="235"/>
        <source>Trust certificate</source>
        <translation>Доверять сертификату</translation>
    </message>
</context>
<context>
    <name>BConsoleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="112"/>
        <source>Name</source>
        <translation>Имя</translation>
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
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="125"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="133"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BDatabase</name>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="40"/>
        <source>Failed to create database directory: %1</source>
        <translation>Не удалось создать каталог базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="50"/>
        <source>Failed to open database: %1</source>
        <translation>Не удалось открыть базу данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="71"/>
        <source>Failed to create database schema: %1</source>
        <translation>Не удалось создать схему базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="91"/>
        <source>Failed to apply database migrations: %1</source>
        <translation>Не удалось применить миграции базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="103"/>
        <source>Failed to repair database schema: %1</source>
        <translation>Не удалось восстановить схему базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="209"/>
        <source>No migration script for database version %1</source>
        <translation>Нет сценария миграции для версии базы данных %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="224"/>
        <source>Failed to apply migration to version %1: %2</source>
        <translation>Не удалось применить миграцию до версии %1: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="229"/>
        <source>Migration to version %1 did not record its version</source>
        <translation>Миграция до версии %1 не записала свою версию</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="260"/>
        <source>Table %1 is still missing after the repair</source>
        <translation>Таблица %1 по-прежнему отсутствует после восстановления</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="273"/>
        <source>Failed to open SQL resource: %1</source>
        <translation>Не удалось открыть ресурс SQL: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="289"/>
        <source>Failed to start transaction: %1</source>
        <translation>Не удалось начать транзакцию: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="320"/>
        <source>SQL execution failed: %1
Statement: %2</source>
        <translation>Ошибка выполнения SQL: %1
Оператор: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="334"/>
        <source>Failed to commit transaction: %1</source>
        <translation>Не удалось зафиксировать транзакцию: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="485"/>
        <source>Database not ready</source>
        <translation>База данных не готова</translation>
    </message>
</context>
<context>
    <name>BDirectiveSchema</name>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="283"/>
        <source>Required directive &apos;%1&apos; is missing</source>
        <translation>Отсутствует обязательная директива «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="295"/>
        <source>&apos;%1&apos; must be an integer</source>
        <translation>«%1» должно быть целым числом</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="298"/>
        <source>&apos;%1&apos; must be &gt;= %2</source>
        <translation>«%1» должно быть &gt;= %2</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="301"/>
        <source>&apos;%1&apos; must be &lt;= %2</source>
        <translation>«%1» должно быть &lt;= %2</translation>
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
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="28"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="29"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="30"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="31"/>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="32"/>
        <source>DIR Port</source>
        <translation>Порт DIR</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="33"/>
        <source>Query File</source>
        <translation>Query File</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="34"/>
        <source>Working Dir</source>
        <translation>Working Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="35"/>
        <source>PID Dir</source>
        <translation>PID Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="36"/>
        <source>Scripts Dir</source>
        <translation>Scripts Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="37"/>
        <source>Plugin Dir</source>
        <translation>Plugin Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="38"/>
        <source>SubSys Dir</source>
        <translation>SubSys Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="39"/>
        <source>Max Jobs</source>
        <translation>Max Jobs</translation>
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
        <translation>Система</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="48"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="49"/>
        <source>Updated</source>
        <translation>Обновлён</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="50"/>
        <source>Last Connected</source>
        <translation>Последнее подключение</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="51"/>
        <source>Active</source>
        <translation>Активен</translation>
    </message>
</context>
<context>
    <name>BDirectorResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="22"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="39"/>
        <source>(multiple)</source>
        <translation>(несколько)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="48"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="48"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BEditableListModel</name>
    <message>
        <location filename="../src/config/beditablelistmodel.cpp" line="123"/>
        <source>Add %1 items</source>
        <translation>Добавить элементы (%1)</translation>
    </message>
</context>
<context>
    <name>BEditableListWidget</name>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="80"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="113"/>
        <source>Add...</source>
        <translation>Добавить...</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="81"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="126"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="118"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="249"/>
        <source>Add item</source>
        <translation>Добавить элемент</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="254"/>
        <source>Remove selected</source>
        <translation>Удалить выбранное</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="393"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="450"/>
        <source>Select Directory</source>
        <translation>Выбор каталога</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="395"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="452"/>
        <source>Select File</source>
        <translation>Выбор файла</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="400"/>
        <source>Add Item</source>
        <translation>Добавление элемента</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="401"/>
        <source>Enter value:</source>
        <translation>Введите значение:</translation>
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
        <translation>Требуется имя FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1049"/>
        <source>The name may contain only letters, digits, blanks and the characters : . - _ / (no blank at the start or end, at most 127 characters)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1055"/>
        <source>At least one Include block is required</source>
        <translation>Требуется хотя бы один блок Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1063"/>
        <source>Include block %1 has no file paths</source>
        <translation>В блоке Include %1 нет путей к файлам</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="1067"/>
        <source>At least one Include block must have file paths</source>
        <translation>Хотя бы один блок Include должен содержать пути к файлам</translation>
    </message>
</context>
<context>
    <name>BFileSetExcludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="982"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="997"/>
        <source>Global Exclude</source>
        <translation>Глобальное исключение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="983"/>
        <source>Configure paths and patterns to exclude from all Include blocks.</source>
        <translation>Настройте пути и шаблоны, исключаемые из всех блоков Include.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="993"/>
        <source>Exclude Paths</source>
        <translation>Исключаемые пути</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="998"/>
        <source>Enter path to exclude...</source>
        <translation>Введите исключаемый путь...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="999"/>
        <source>+ Add Path</source>
        <translation>+ Добавить путь</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1005"/>
        <source>These paths will be excluded from all backup jobs using this FileSet.
Use absolute paths (e.g., /tmp, /var/cache) or patterns.</source>
        <translation>Эти пути будут исключены из всех Jobs резервного копирования, использующих этот FileSet.
Используйте абсолютные пути (напр., /tmp, /var/cache) или шаблоны.</translation>
    </message>
</context>
<context>
    <name>BFileSetIncludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="875"/>
        <source>Include Blocks</source>
        <translation>Блоки Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="876"/>
        <source>Configure which files and directories to include in the backup.</source>
        <translation>Настройте, какие файлы и каталоги включать в резервную копию.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="889"/>
        <source>+ Add Include Block</source>
        <translation>+ Добавить блок Include</translation>
    </message>
</context>
<context>
    <name>BFileSetPreviewPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1038"/>
        <source>Preview &amp; Deploy</source>
        <translation>Предпросмотр и развёртывание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1039"/>
        <source>Review the generated FileSet configuration. Finish sends it to the Director where the Director accepts FileSets; otherwise copy it for manual deployment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1052"/>
        <source>Configuration:</source>
        <translation>Конфигурация:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1058"/>
        <source>Copy configuration</source>
        <translation>Копировать конфигурацию</translation>
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
        <translation># ПРИМЕЧАНИЕ: Bareos Director не поддерживает &apos;configure add fileset&apos; с
# вложенными блоками Include/Exclude/Options (подтверждённое ограничение Bareos).
# Разверните эту конфигурацию вручную:
#   1. Скопируйте в /etc/bareos/bareos-dir.d/fileset/%1.conf
#   2. Выполните: systemctl reload bareos-dir
#
# Используйте «Копировать конфигурацию», чтобы скопировать текст ниже.

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1154"/>
        <source>Configuration is valid</source>
        <translation>Конфигурация корректна</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1177"/>
        <source>FileSet not complete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1189"/>
        <source>Close Wizard</source>
        <translation>Закрыть мастер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1190"/>
        <source>The configuration has not been sent to the Director.
Use &apos;Copy configuration&apos; to save it before closing.

Do you want to close the wizard?</source>
        <translation>Конфигурация не была отправлена на Director.
Используйте «Копировать конфигурацию», чтобы сохранить её перед закрытием.

Закрыть мастер?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1208"/>
        <source>Another change is still being written. Try again when it is done.</source>
        <translation type="unfinished">Другое изменение ещё записывается. Повторите попытку, когда оно завершится.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1232"/>
        <source>Nothing was written.</source>
        <translation type="unfinished">Ничего не записано.</translation>
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
        <translation>Конфигурация скопирована в буфер обмена</translation>
    </message>
</context>
<context>
    <name>BFileSetResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="357"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="357"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BFileSetSettingsPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="556"/>
        <source>FileSet Settings</source>
        <translation>Настройки FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="557"/>
        <source>Configure the FileSet name and basic options.</source>
        <translation>Задайте имя FileSet и основные параметры.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="571"/>
        <source>Select FileSet:</source>
        <translation>Выберите FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="574"/>
        <source>Loading FileSets...</source>
        <translation>Загрузка FileSets...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="592"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="596"/>
        <source>e.g., LinuxFullSystem</source>
        <translation>напр., LinuxFullSystem</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="597"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="600"/>
        <source>Optional description</source>
        <translation>Необязательное описание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="601"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="606"/>
        <source>Global Options</source>
        <translation>Глобальные параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="609"/>
        <source>Enable VSS</source>
        <translation>Включить VSS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="610"/>
        <source>Enable Volume Shadow Copy Service (Windows)</source>
        <translation>Включить службу теневого копирования томов (Windows)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="613"/>
        <source>Ignore FileSet Changes</source>
        <translation>Игнорировать изменения FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="614"/>
        <source>Don&apos;t trigger Full backup when FileSet changes</source>
        <translation>Не запускать резервное копирование Full при изменении FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="636"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="692"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="804"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="712"/>
        <source>No Director connection</source>
        <translation>Нет соединения с Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="766"/>
        <source>-- Select FileSet --</source>
        <translation>-- Выберите FileSet --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="772"/>
        <source>(%1 FileSets)</source>
        <translation>(FileSets: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="814"/>
        <source>Loaded</source>
        <translation>Загружено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="850"/>
        <source>Loaded from &quot;show filesets&quot;: Enable VSS and Ignore FileSet Changes are not reported there; check them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="865"/>
        <source>Config not cached</source>
        <translation>Конфигурация не кэширована</translation>
    </message>
</context>
<context>
    <name>BFileSetTemplatePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="189"/>
        <source>Select FileSet Template</source>
        <translation>Выбор шаблона FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="190"/>
        <source>Choose a preset template to start with, or mix multiple templates together.</source>
        <translation>Выберите готовый шаблон для начала или объедините несколько шаблонов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="199"/>
        <source>Template:</source>
        <translation>Шаблон:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="202"/>
        <source>Mix Templates...</source>
        <translation>Объединить шаблоны...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="204"/>
        <source>Combine paths from multiple templates</source>
        <translation>Объединить пути из нескольких шаблонов</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="217"/>
        <source>Template Description</source>
        <translation>Описание шаблона</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="232"/>
        <source>Paths Preview</source>
        <translation>Предпросмотр путей</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="237"/>
        <source>&lt;b&gt;Include:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Включить:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="250"/>
        <source>&lt;b&gt;Exclude:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Исключить:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="333"/>
        <source>Platform: Windows</source>
        <translation>Платформа: Windows</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="335"/>
        <source>Platform: Linux/Unix</source>
        <translation>Платформа: Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="337"/>
        <source>Platform: macOS</source>
        <translation>Платформа: macOS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="339"/>
        <source>Platform: Any</source>
        <translation>Платформа: любая</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="352"/>
        <source>Mix Templates</source>
        <translation>Объединение шаблонов</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="357"/>
        <source>Select templates to combine:</source>
        <translation>Выберите шаблоны для объединения:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="375"/>
        <source>%1
Platform: %2</source>
        <translation>%1
Платформа: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="382"/>
        <source>Combined Paths Preview</source>
        <translation>Предпросмотр объединённых путей</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="431"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="433"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="541"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="545"/>
        <source>(none)</source>
        <translation>(нет)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="510"/>
        <source>Mixed_%1</source>
        <translation>Mixed_%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="511"/>
        <source>Combined FileSet from %1 templates</source>
        <translation>Объединённый FileSet из шаблонов: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="513"/>
        <source>Mixed: %1 templates selected</source>
        <translation>Объединение: выбрано шаблонов: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="517"/>
        <source>Templates: %1</source>
        <translation>Шаблоны: %1</translation>
    </message>
</context>
<context>
    <name>BFileSetWizard</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="104"/>
        <source>New FileSet Wizard</source>
        <translation>Мастер создания FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="104"/>
        <source>Edit FileSet</source>
        <translation>Изменить FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="127"/>
        <source>Finish</source>
        <translation>Готово</translation>
    </message>
</context>
<context>
    <name>BGrpcAddressProbe</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="176"/>
        <source>no TLS connection within %1 s</source>
        <translation>нет соединения TLS в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="213"/>
        <source>Cannot resolve the host name %1: %2</source>
        <translation>Не удалось разрешить имя хоста %1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="214"/>
        <source>no address</source>
        <translation>нет адреса</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="238"/>
        <source>Cannot resolve the host name %1: no address</source>
        <translation>Не удалось разрешить имя хоста %1: нет адреса</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="251"/>
        <source>Cannot reach the gRPC endpoint %1:%2 (%3)</source>
        <translation>Конечная точка gRPC %1:%2 недоступна (%3)</translation>
    </message>
</context>
<context>
    <name>BGrpcConnect</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="92"/>
        <source>%1 - the Director no longer knows this session (it ends after 8 hours without a request or when the Director restarts). Connect again.</source>
        <translation>%1 — Director больше не знает этот сеанс (он завершается через 8 часов без запросов или при перезапуске Director). Подключитесь снова.</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="99"/>
        <source>the call was cancelled</source>
        <translation>вызов был отменён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="103"/>
        <source>no connection or no reply within %1 s</source>
        <translation>нет соединения или ответа в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="104"/>
        <source>no connection or no reply in time</source>
        <translation>нет соединения или ответа вовремя</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="107"/>
        <source>this console is not allowed to do that</source>
        <translation>этой консоли это не разрешено</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="110"/>
        <source>the Director does not offer this method (an older version?)</source>
        <translation>Director не предоставляет этот метод (старая версия?)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="113"/>
        <source>the gRPC service of the Director cannot be reached</source>
        <translation>служба gRPC Director недоступна</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="116"/>
        <source>not logged in, or the session has expired</source>
        <translation>вход не выполнен или сеанс истёк</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="120"/>
        <source>the Director reported an error without a message</source>
        <translation>Director сообщил об ошибке без текста</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="123"/>
        <source>no reason given</source>
        <translation>причина не указана</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="135"/>
        <source>The Director does not offer the expected gRPC service (protocol version mismatch: this build speaks %1; the Director may run an older or newer build). Use BNET or update the Director or this application.</source>
        <translation>Director не предоставляет ожидаемую службу gRPC (несовпадение версии протокола: эта сборка использует %1; на Director может работать более старая или новая сборка). Используйте BNET или обновите Director либо это приложение.</translation>
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
        <translation>Ошибка входа gRPC: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="234"/>
        <source>gRPC login failed: no challenge received</source>
        <translation>Ошибка входа gRPC: запрос (challenge) не получен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="267"/>
        <source>Authentication failed: %1</source>
        <translation>Ошибка аутентификации: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="268"/>
        <source>invalid response</source>
        <translation>недопустимый ответ</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="329"/>
        <source>Client %1 connected</source>
        <translation>Client %1 подключён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="332"/>
        <source>Client %1 disconnected</source>
        <translation>Client %1 отключён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="338"/>
        <source>Client %1 is alive</source>
        <translation>Client %1 доступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="339"/>
        <source>Client %1 not reachable</source>
        <translation>Client %1 недоступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="341"/>
        <source>Storage %1 connected (job %2)</source>
        <translation>Storage %1 подключён (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="344"/>
        <source>Storage %1 disconnected (job %2)</source>
        <translation>Storage %1 отключён (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="348"/>
        <source>Storage %1 is alive</source>
        <translation>Storage %1 доступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="349"/>
        <source>Storage %1 not reachable</source>
        <translation>Storage %1 недоступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="482"/>
        <source>Invalid response</source>
        <translation>Недопустимый ответ</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="627"/>
        <location filename="../src/director/bgrpctransport.cpp" line="684"/>
        <source>Not connected to the gRPC service of the Director</source>
        <translation>Нет подключения к службе gRPC Director</translation>
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
        <translation>Не удалось прочитать ответ Director</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="183"/>
        <source>The Director&apos;s certificate on the gRPC connection (SHA-256 %1) does not match the pinned certificate (SHA-256 %2). The connection was refused before the login; someone may be intercepting it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="186"/>
        <source>none</source>
        <translation type="unfinished">нет</translation>
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
        <translation>Блок Include №%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="71"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="98"/>
        <source>Add Path...</source>
        <translation>Добавить путь...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="99"/>
        <source>Enter file or directory path</source>
        <translation>Введите путь к файлу или каталогу</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="102"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="109"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="123"/>
        <source>Exclude Files</source>
        <translation>Исключаемые файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="129"/>
        <source>Add File...</source>
        <translation>Добавить файл...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="130"/>
        <source>Enter file path to exclude</source>
        <translation>Введите путь к исключаемому файлу</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="136"/>
        <source>Directory Patterns (WildDir)</source>
        <translation>Шаблоны каталогов (WildDir)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="142"/>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="155"/>
        <source>Add Pattern...</source>
        <translation>Добавить шаблон...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="143"/>
        <source>e.g., */__pycache__</source>
        <translation>напр., */__pycache__</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="149"/>
        <source>File Patterns (WildFile)</source>
        <translation>Шаблоны файлов (WildFile)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="156"/>
        <source>e.g., *.pyc, *.log</source>
        <translation>напр., *.pyc, *.log</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="161"/>
        <source>Exclude</source>
        <translation>Исключить</translation>
    </message>
</context>
<context>
    <name>BIncludeOptionsForm</name>
    <message>
        <location filename="../src/config/bincludeoptionsform.cpp" line="91"/>
        <source>Failed to load include options schema</source>
        <translation>Не удалось загрузить схему параметров Include</translation>
    </message>
</context>
<context>
    <name>BJobAdvancedPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="639"/>
        <source>Advanced Directives</source>
        <translation>Дополнительные директивы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="640"/>
        <source>Optional settings for fine-tuning (can be skipped).</source>
        <translation>Необязательные параметры для тонкой настройки (можно пропустить).</translation>
    </message>
</context>
<context>
    <name>BJobBasicsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="249"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="254"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="250"/>
        <source>Configure the identity of the resource.</source>
        <translation>Задайте идентификацию ресурса.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="258"/>
        <source>e.g., BackupServer1</source>
        <translation>напр., BackupServer1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="259"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="263"/>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="268"/>
        <source>Inheritance</source>
        <translation>Наследование</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="275"/>
        <source>JobDefs:</source>
        <translation>JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="288"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="300"/>
        <source>Inherits default settings from JobDefs &quot;%1&quot;. Resource fields on the next page become optional.</source>
        <translation>Наследует настройки по умолчанию из JobDefs «%1». Поля ресурсов на следующей странице становятся необязательными.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="314"/>
        <source>Basic Settings (JobDefs)</source>
        <translation>Основные настройки (JobDefs)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitions</name>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="115"/>
        <source>(block)</source>
        <translation>(блок)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitions.cpp" line="128"/>
        <source>(%n block(s))</source>
        <translation>
            <numerusform>(%n блок)</numerusform>
            <numerusform>(%n блока)</numerusform>
            <numerusform>(%n блоков)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="302"/>
        <source>(see the text)</source>
        <translation>(см. текст)</translation>
    </message>
</context>
<context>
    <name>BJobDetailsDialog</name>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="35"/>
        <source>Job Details: %1 (ID: %2) - Client: %3</source>
        <translation>Сведения о Job: %1 (ID: %2) - Client: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="74"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="83"/>
        <source>Job Information</source>
        <translation>Сведения о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="86"/>
        <source>Job ID:</source>
        <translation>ID Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="87"/>
        <source>Job Name:</source>
        <translation>Имя Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="88"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="92"/>
        <source>Backup</source>
        <translation>Backup</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="93"/>
        <source>Type:</source>
        <translation>Тип:</translation>
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
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="105"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="106"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="112"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="240"/>
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="120"/>
        <source>Timing</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="127"/>
        <source>Start Time:</source>
        <translation>Время начала:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="131"/>
        <source>Duration:</source>
        <translation>Длительность:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="136"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="140"/>
        <source>Files Backed Up:</source>
        <translation>Сохранено файлов:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="147"/>
        <source>Data Size:</source>
        <translation>Объём данных:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="152"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="169"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="163"/>
        <source>Job Status:</source>
        <translation>Статус Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="183"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="187"/>
        <source>Logs</source>
        <translation>Журналы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="200"/>
        <source>No connection to the Director.</source>
        <translation>Нет соединения с Director.</translation>
    </message>
</context>
<context>
    <name>BJobLogDialog</name>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="30"/>
        <source>Job Log: %1 (ID: %2) - %3</source>
        <translation>Журнал Job: %1 (ID: %2) - %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="45"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="46"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="47"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="48"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="51"/>
        <source>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</source>
        <translation>&lt;b&gt;Статус:&lt;/b&gt; %1 | &lt;b&gt;Начало:&lt;/b&gt; %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="68"/>
        <source>Copy Selection</source>
        <translation>Копировать выделенное</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="74"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
</context>
<context>
    <name>BJobLogView</name>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="70"/>
        <source>No log entries for this job.</source>
        <translation>Нет записей журнала для этого Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="74"/>
        <source>Could not load the job log: %1</source>
        <translation>Не удалось загрузить журнал Job: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="104"/>
        <source>No connection to the Director.</source>
        <translation>Нет соединения с Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="108"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
</context>
<context>
    <name>BJobPreviewPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="703"/>
        <source>Preview &amp; Deploy</source>
        <translation>Предпросмотр и развёртывание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="704"/>
        <source>Review the configuration and deploy to the Director.</source>
        <translation>Проверьте конфигурацию и разверните её на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="709"/>
        <source>Configuration:</source>
        <translation>Конфигурация:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="721"/>
        <source>Copy Configuration</source>
        <translation>Копировать конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="728"/>
        <source>Director Command:</source>
        <translation>Команда Director:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="740"/>
        <source>Copy Command</source>
        <translation>Копировать команду</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="798"/>
        <source>Note: RunScript blocks cannot be sent via &apos;configure add&apos;. The command above creates the job without RunScript blocks. Deploy the full configuration manually using &apos;Copy Configuration&apos; to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.</source>
        <translation>Примечание: блоки RunScript нельзя отправить через &apos;configure add&apos;. Команда выше создаёт Job без блоков RunScript. Разверните полную конфигурацию вручную: с помощью «Копировать конфигурацию» поместите её в /etc/bareos/bareos-dir.d/job/%1.conf и перезагрузите Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="821"/>
        <source>Deploy to Director</source>
        <translation>Развернуть на Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="822"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Отправить &apos;configure add&apos; на Director?

Нажмите «Да», чтобы выполнить, или «Нет», чтобы закрыть без развёртывания.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="842"/>
        <source>Not sent: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="851"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="886"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="895"/>
        <source>Resource created successfully.</source>
        <translation>Ресурс успешно создан.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="903"/>
        <source>Resource created. Reloading Director...</source>
        <translation>Ресурс создан. Перезагрузка Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="926"/>
        <source>Resource created and Director reloaded successfully.</source>
        <translation>Ресурс создан, Director успешно перезагружен.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="929"/>
        <source>Resource created, but reloading the Director failed. Run &apos;reload&apos; on the Director.</source>
        <translation>Ресурс создан, но перезагрузить Director не удалось. Выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="942"/>
        <source>Resource created, but the connection to the Director was lost before the reload was confirmed.</source>
        <translation>Ресурс создан, но соединение с Director было потеряно до подтверждения перезагрузки.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="946"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check whether the resource exists.</source>
        <translation>Соединение с Director было потеряно до получения ответа. Подключитесь снова и проверьте, существует ли ресурс.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="958"/>
        <source>Resource created, but the Director did not confirm the reload in time. Run &apos;reload&apos; on the Director if the job does not appear.</source>
        <translation>Ресурс создан, но Director не подтвердил перезагрузку вовремя. Если Job не появится, выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="961"/>
        <source>Timeout waiting for Director response. The command may still be processing.</source>
        <translation>Истекло время ожидания ответа Director. Команда, возможно, ещё обрабатывается.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="970"/>
        <source>Configuration copied to clipboard.</source>
        <translation>Конфигурация скопирована в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="977"/>
        <source>Command copied to clipboard.</source>
        <translation>Команда скопирована в буфер обмена.</translation>
    </message>
</context>
<context>
    <name>BJobResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="259"/>
        <source>Level</source>
        <translation>Уровень</translation>
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
        <translation>Ссылки на ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="346"/>
        <source>Select the resources this job uses.</source>
        <translation>Выберите ресурсы, которые использует этот Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="362"/>
        <source>Required Resources</source>
        <translation>Обязательные ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="367"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="371"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="375"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="379"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="383"/>
        <source>Messages:</source>
        <translation>Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="388"/>
        <source>Job Settings</source>
        <translation>Настройки Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="394"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="398"/>
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="402"/>
        <source>Catalog:</source>
        <translation>Catalog:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="407"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="412"/>
        <source>Max Concurrent Jobs:</source>
        <translation>Max Concurrent Jobs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="417"/>
        <source>Restore Settings</source>
        <translation>Настройки Restore</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="421"/>
        <source>/tmp/bareos-restores</source>
        <translation>/tmp/bareos-restores</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="422"/>
        <source>Where:</source>
        <translation>Where:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="426"/>
        <source>Replace:</source>
        <translation>Replace:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="431"/>
        <source>Pool Overrides</source>
        <translation>Переопределение Pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="438"/>
        <source>Full Backup Pool:</source>
        <translation>Full Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="442"/>
        <source>Diff Backup Pool:</source>
        <translation>Diff Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="446"/>
        <source>Inc Backup Pool:</source>
        <translation>Inc Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="451"/>
        <source>Simple Scripts</source>
        <translation>Простые сценарии</translation>
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
        <translation>Run Before Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="460"/>
        <source>Run After Job:</source>
        <translation>Run After Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="464"/>
        <source>Run After Failed Job:</source>
        <translation>Run After Failed Job:</translation>
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
        <translation>Client Run Before:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="472"/>
        <source>Client Run After:</source>
        <translation>Client Run After:</translation>
    </message>
</context>
<context>
    <name>BJobScriptsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="613"/>
        <source>RunScript Blocks</source>
        <translation>Блоки RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="614"/>
        <source>Define complex script execution blocks (optional). For simple scripts, use the fields on the previous page.</source>
        <translation>Определите сложные блоки выполнения сценариев (необязательно). Для простых сценариев используйте поля на предыдущей странице.</translation>
    </message>
</context>
<context>
    <name>BJobWidget</name>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="52"/>
        <source>Successful (T)</source>
        <translation>Успешно (T)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="53"/>
        <source>Warning (W)</source>
        <translation>Предупреждение (W)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="55"/>
        <source>Error (E)</source>
        <translation>Ошибка (E)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="60"/>
        <source>Enable Date Filter</source>
        <translation>Включить фильтр по дате</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="426"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1702"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1705"/>
        <source>Filter</source>
        <translation>Фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="485"/>
        <source>Job Name:</source>
        <translation>Имя Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="486"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="539"/>
        <source>Backup Level</source>
        <translation>Уровень резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="571"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="572"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="573"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="559"/>
        <source>Date Range</source>
        <translation>Диапазон дат</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="43"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="44"/>
        <source>Clear History</source>
        <translation>Очистить историю</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="54"/>
        <source>Failed (F)</source>
        <translation>Неудачно (F)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="56"/>
        <source>Running (R)</source>
        <translation>Выполняется (R)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="57"/>
        <source>Canceled (A)</source>
        <translation>Отменено (A)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="58"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="382"/>
        <source>Zero Bytes</source>
        <translation>Ноль байт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="85"/>
        <source>When enabled, only shows jobs with 0 bytes transferred.
Warning: This filter persists across restarts and may
cause an empty table if no zero-byte jobs exist.</source>
        <translation>Если включено, показываются только Jobs, передавшие 0 байт.
Внимание: этот фильтр сохраняется после перезапуска и может
привести к пустой таблице, если Jobs с нулём байт нет.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="93"/>
        <source>When enabled, only shows jobs within the selected date range.
Warning: This filter persists across restarts and may
cause an empty or limited table if the range is too narrow.</source>
        <translation>Если включено, показываются только Jobs в выбранном диапазоне дат.
Внимание: этот фильтр сохраняется после перезапуска и может
привести к пустой или неполной таблице, если диапазон слишком узкий.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="110"/>
        <source>Filter by job name...</source>
        <translation>Фильтр по имени Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="115"/>
        <source>Filter by client...</source>
        <translation>Фильтр по Client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="120"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="123"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="126"/>
        <source>(no job selected)</source>
        <translation>(Job не выбран)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="158"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Load error</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - Ошибка загрузки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="182"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1752"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - No job selected</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job не выбран</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="186"/>
        <source>-- Select Job Log --</source>
        <translation>-- Выберите журнал Job --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="188"/>
        <source>Select from previously loaded job logs</source>
        <translation>Выбрать из ранее загруженных журналов Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="190"/>
        <source>Copy selected lines to clipboard</source>
        <translation>Копировать выделенные строки в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="193"/>
        <source>Clear job log history</source>
        <translation>Очистить историю журналов Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="196"/>
        <source>Search in log...</source>
        <translation>Поиск в журнале...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="198"/>
        <source>Filter log lines by search text</source>
        <translation>Фильтровать строки журнала по тексту поиска</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="231"/>
        <source>Cancelling job...</source>
        <translation>Отмена Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="234"/>
        <source>Deleting job...</source>
        <translation>Удаление Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="237"/>
        <source>Running job again...</source>
        <translation>Повторный запуск Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="240"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="244"/>
        <source>Pruning expired jobs...</source>
        <translation>Prune просроченных Jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="285"/>
        <source>⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. This filter persists across restarts!</source>
        <translation>⚠ Активен фильтр «Ноль байт» — показываются только Jobs с 0 байт. Этот фильтр сохраняется после перезапуска!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="295"/>
        <source>⚠ Date filter active — only jobs within the selected range are shown. This filter persists across restarts!</source>
        <translation>⚠ Активен фильтр по дате — показываются только Jobs в выбранном диапазоне. Этот фильтр сохраняется после перезапуска!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="385"/>
        <source>Date Range (%1 – %2)</source>
        <translation>Диапазон дат (%1 – %2)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="398"/>
        <source>Status (hidden: %1)</source>
        <translation>Статус (скрыто: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="403"/>
        <source>⚠ Active filters from last session: %1 — results may be limited</source>
        <translation>⚠ Активные фильтры из прошлого сеанса: %1 — результаты могут быть неполными</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="429"/>
        <source>Show/hide filter</source>
        <translation>Показать/скрыть фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="435"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="439"/>
        <source>Auto-Refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="441"/>
        <source>5 s</source>
        <translation>5 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="442"/>
        <source>10 s</source>
        <translation>10 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="443"/>
        <source>30 s</source>
        <translation>30 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="444"/>
        <source>1 min</source>
        <translation>1 мин</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="445"/>
        <source>5 min</source>
        <translation>5 мин</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="490"/>
        <source>&lt;b&gt;Status:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Статус:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="506"/>
        <source>Basic Filters</source>
        <translation>Основные фильтры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="528"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="553"/>
        <source>From:</source>
        <translation>С:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="554"/>
        <source>To:</source>
        <translation>По:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="578"/>
        <source>Job Details</source>
        <translation>Сведения о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="583"/>
        <source>Reset filter</source>
        <translation>Сбросить фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="704"/>
        <source>Updated: %1 - %2 jobs loaded</source>
        <translation>Обновлено: %1 - загружено Jobs: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="750"/>
        <source>No selection</source>
        <translation>Ничего не выбрано</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="751"/>
        <source>Please select a job to cancel.</source>
        <translation>Выберите Job для отмены.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="768"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="805"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="765"/>
        <source>Do you really want to cancel job ID %1?</source>
        <translation>Вы действительно хотите отменить Job с ID %1?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1222"/>
        <source>Loading page %1...</source>
        <translation>Загрузка страницы %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1235"/>
        <source>Refreshing job list...</source>
        <translation>Обновление списка Jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1342"/>
        <source>%1 jobs loaded</source>
        <translation>Загружено Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1577"/>
        <source>Auto-refresh on (%1)</source>
        <translation>Автообновление включено (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1582"/>
        <source>Auto-refresh off</source>
        <translation>Автообновление выключено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1657"/>
        <source>Filtered: %1 of %2 jobs</source>
        <translation>Отфильтровано: %1 из %2 Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1703"/>
        <source>Hide filter</source>
        <translation>Скрыть фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1706"/>
        <source>Show filter</source>
        <translation>Показать фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1734"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Loading...</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1772"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1909"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - строк: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="615"/>
        <source>History:</source>
        <translation>История:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="635"/>
        <source>Job Log</source>
        <translation>Журнал Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="638"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1274"/>
        <source>Error parsing JSON data</source>
        <translation>Ошибка разбора данных JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1588"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1589"/>
        <source>Running</source>
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1590"/>
        <source>Blocked</source>
        <translation>Заблокирован</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1591"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1592"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1593"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1594"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1595"/>
        <source>Non-fatal Error</source>
        <translation>Нефатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1596"/>
        <source>Canceled</source>
        <translation>Отменён</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1650"/>
        <source>⚠ No jobs visible — &apos;Zero Bytes&apos; filter is active!</source>
        <translation>⚠ Нет видимых Jobs — активен фильтр «Ноль байт»!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1652"/>
        <source>⚠ No jobs visible — check your filter settings (%1 jobs loaded)</source>
        <translation>⚠ Нет видимых Jobs — проверьте настройки фильтров (загружено Jobs: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1661"/>
        <source>%1 Jobs</source>
        <translation>Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1767"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines, job is running</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - строк: %3, Job выполняется</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1933"/>
        <source>Copied %1 log lines to clipboard</source>
        <translation>Скопировано строк журнала в буфер обмена: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1948"/>
        <source>Copied %1 selected lines to clipboard</source>
        <translation>Скопировано выделенных строк в буфер обмена: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1962"/>
        <source>Job log history cleared</source>
        <translation>История журналов Jobs очищена</translation>
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
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="777"/>
        <source>Cancelling %n job(s)...</source>
        <translation type="unfinished">
            <numerusform></numerusform>
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
        <translation type="unfinished">причина не указана</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobwidget.cpp" line="806"/>
        <source>The Director did not cancel %n job(s):</source>
        <translation type="unfinished">
            <numerusform></numerusform>
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
        <translation>Мастер создания Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1009"/>
        <source>New JobDefs Wizard</source>
        <translation>Мастер создания JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1019"/>
        <source>Finish</source>
        <translation>Готово</translation>
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
        <translation>ID Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="277"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="278"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="279"/>
        <source>Start Time</source>
        <translation>Время начала</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="280"/>
        <source>End Time</source>
        <translation>Время окончания</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="281"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="282"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="283"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="284"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="285"/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="286"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
</context>
<context>
    <name>BJobsStatisticsWidget</name>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="23"/>
        <source>Job Summary</source>
        <translation>Сводка по Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="29"/>
        <source>Total Jobs:</source>
        <translation>Всего Jobs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="32"/>
        <source>Selected:</source>
        <translation>Выбрано:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="37"/>
        <source>Status Breakdown</source>
        <translation>Разбивка по статусу</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="44"/>
        <source>Successful:</source>
        <translation>Успешно:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="59"/>
        <source>Warnings:</source>
        <translation>Предупреждения:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="74"/>
        <source>Failed:</source>
        <translation>Неудачно:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="88"/>
        <source>Data Summary</source>
        <translation>Сводка по данным</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="92"/>
        <source>Total Files:</source>
        <translation>Всего файлов:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="95"/>
        <source>Total Size:</source>
        <translation>Общий размер:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="97"/>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="185"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="98"/>
        <source>Date Range:</source>
        <translation>Диапазон дат:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="180"/>
        <source>%1 to %2</source>
        <translation>с %1 по %2</translation>
    </message>
</context>
<context>
    <name>BJsonJobView</name>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="531"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="618"/>
        <source>Delete %1 Jobs</source>
        <translation>Удалить Jobs (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="162"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="531"/>
        <source>Delete Job</source>
        <translation>Удалить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="151"/>
        <source>Show Details...</source>
        <translation>Показать подробности...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="156"/>
        <source>Retry Job</source>
        <translation>Повторить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="159"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="727"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="167"/>
        <source>View Log...</source>
        <translation>Просмотреть журнал...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="170"/>
        <source>Restore Files...</source>
        <translation>Восстановить файлы...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="177"/>
        <source>Export to JSON...</source>
        <translation>Экспорт в JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="181"/>
        <source>Export to CSV...</source>
        <translation>Экспорт в CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="188"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="192"/>
        <source>Invert Selection</source>
        <translation>Инвертировать выделение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="196"/>
        <source>Clear Selection</source>
        <translation>Снять выделение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="277"/>
        <source>Export Selected Jobs to JSON</source>
        <translation>Экспорт выбранных Jobs в JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="277"/>
        <source>Export All Jobs to JSON</source>
        <translation>Экспорт всех Jobs в JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="290"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="323"/>
        <source>Export Error</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="291"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="324"/>
        <source>Could not write to file: %1</source>
        <translation>Не удалось записать в файл: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="298"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="331"/>
        <source>Export Successful</source>
        <translation>Экспорт выполнен</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjsonjobview.cpp" line="299"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="332"/>
        <source>Exported %n job(s) to %1</source>
        <translation>
            <numerusform>Экспортирован %n Job в %1</numerusform>
            <numerusform>Экспортировано %n Jobs в %1</numerusform>
            <numerusform>Экспортировано %n Jobs в %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="310"/>
        <source>Export Selected Jobs to CSV</source>
        <translation>Экспорт выбранных Jobs в CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="310"/>
        <source>Export All Jobs to CSV</source>
        <translation>Экспорт всех Jobs в CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="344"/>
        <source>Total Jobs: %1
</source>
        <translation>Всего Jobs: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="345"/>
        <source>Successful: %1
</source>
        <translation>Успешно: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="346"/>
        <source>Warnings: %1
</source>
        <translation>Предупреждения: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="347"/>
        <source>Failed: %1

</source>
        <translation>Неудачно: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="349"/>
        <source>Total Files: %1
</source>
        <translation>Всего файлов: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="350"/>
        <source>Total Size: %1 bytes

</source>
        <translation>Общий размер: %1 байт

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="353"/>
        <source>Date Range:
</source>
        <translation>Диапазон дат:
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="354"/>
        <source>  From: %1
</source>
        <translation>  С: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="355"/>
        <source>  To: %1

</source>
        <translation>  По: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="358"/>
        <source>Selected: %1</source>
        <translation>Выбрано: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="360"/>
        <source>Job Statistics</source>
        <translation>Статистика Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="425"/>
        <source>Export All to JSON...</source>
        <translation>Экспортировать всё в JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="426"/>
        <source>Export All to CSV...</source>
        <translation>Экспортировать всё в CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="541"/>
        <source>ID %1: %2 (%3)</source>
        <translation>ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="547"/>
        <source>&lt;b&gt;%1 jobs selected for deletion:&lt;/b&gt;&lt;br&gt;%2</source>
        <translation>&lt;b&gt;Jobs, выбранные для удаления (%1):&lt;/b&gt;&lt;br&gt;%2</translation>
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
        <translation>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Уровень:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Начало:&lt;/b&gt; %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="574"/>
        <source>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Attention:&lt;/b&gt; %1 dependent Incremental/Differential job(s) found!&lt;br&gt;&lt;br&gt;Deleting Full backups may make dependent backups unusable for restore.&lt;/div&gt;</source>
        <translation>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Внимание:&lt;/b&gt; найдены зависимые Jobs Incremental/Differential: %1!&lt;br&gt;&lt;br&gt;Удаление резервных копий Full может сделать зависимые резервные копии непригодными для восстановления.&lt;/div&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="585"/>
        <source>Delete Mode</source>
        <translation>Режим удаления</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="588"/>
        <source>Delete job record only</source>
        <translation>Удалить только запись о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="589"/>
        <source>Removes the job entry from the catalog database.
The backup data on the storage media remains intact.</source>
        <translation>Удаляет запись о Job из базы данных каталога.
Данные резервной копии на носителях остаются нетронутыми.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="598"/>
        <source>Prune (remove expired jobs by retention policy)</source>
        <translation>Prune (удалить просроченные Jobs по политике хранения)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="599"/>
        <source>Removes job records that have exceeded their configured retention period.
Only expired jobs for the affected client(s) will be removed.</source>
        <translation>Удаляет записи о Jobs, превысившие настроенный срок хранения.
Удаляются только просроченные Jobs затронутых клиентов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="608"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; это действие нельзя отменить!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="618"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="632"/>
        <source>  ID %1: %2 (%3)</source>
        <translation>  ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="637"/>
        <source>Confirm Delete</source>
        <translation>Подтверждение удаления</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="638"/>
        <source>Are you sure you want to delete %1 job(s)?

%2

%3 dependent Incremental/Differential backup(s) may become unusable for restore.</source>
        <translation>Вы действительно хотите удалить Jobs (%1)?

%2

Зависимые резервные копии Incremental/Differential (%3) могут стать непригодными для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="695"/>
        <source>Rerun Job</source>
        <translation>Запустить Job повторно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="696"/>
        <source>Do you want to run job %1 (%2) again?</source>
        <translation>Запустить Job %1 (%2) ещё раз?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="728"/>
        <source>Do you really want to cancel the running job %1 (%2)?</source>
        <translation>Вы действительно хотите отменить выполняющийся Job %1 (%2)?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="757"/>
        <source>Not connected</source>
        <translation>Нет соединения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="758"/>
        <source>No connection to the Director.</source>
        <translation>Соединение с Director недоступно.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="779"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="780"/>
        <source>No director connection available.</source>
        <translation>Соединение с Director недоступно.</translation>
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
        <translation>Масштаб интерфейса: %1 %</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="234"/>
        <source>Connecting...</source>
        <translation>Подключение...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="237"/>
        <source>Authenticating...</source>
        <translation>Аутентификация...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="240"/>
        <source>Setting API mode...</source>
        <translation>Установка режима API...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="243"/>
        <source>Loading resources...</source>
        <translation>Загрузка ресурсов...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="246"/>
        <source>Connected and ready</source>
        <translation>Подключено и готово</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="249"/>
        <source>Connection error</source>
        <translation>Ошибка соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="252"/>
        <location filename="../src/bmainwindow.cpp" line="1831"/>
        <source>Disconnected</source>
        <translation>Отключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="259"/>
        <source>Loading resources... %1/%2</source>
        <translation>Загрузка ресурсов... %1/%2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="284"/>
        <location filename="../src/bmainwindow.cpp" line="789"/>
        <location filename="../src/bmainwindow.cpp" line="2696"/>
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="453"/>
        <location filename="../src/bmainwindow.cpp" line="3097"/>
        <source>Auto-refresh enabled (%1s)</source>
        <translation>Автообновление включено (%1 с)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="457"/>
        <location filename="../src/bmainwindow.cpp" line="3102"/>
        <source>Auto-refresh disabled</source>
        <translation>Автообновление выключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="531"/>
        <source>Job: %1</source>
        <translation>Job: %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="592"/>
        <location filename="../src/bmainwindow.cpp" line="795"/>
        <location filename="../src/bmainwindow.cpp" line="1833"/>
        <location filename="../src/bmainwindow.cpp" line="2471"/>
        <location filename="../src/bmainwindow.cpp" line="2612"/>
        <location filename="../src/bmainwindow.cpp" line="2952"/>
        <source>Not connected</source>
        <translation>Не подключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="712"/>
        <source>Storage/Volumes</source>
        <translation>Storage/Volumes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="770"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="866"/>
        <location filename="../src/bmainwindow.cpp" line="883"/>
        <location filename="../src/bmainwindow.cpp" line="1797"/>
        <location filename="../src/bmainwindow.cpp" line="2035"/>
        <source>Connect</source>
        <translation>Подключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="871"/>
        <location filename="../src/bmainwindow.cpp" line="889"/>
        <location filename="../src/bmainwindow.cpp" line="2036"/>
        <location filename="../src/bmainwindow.cpp" line="2711"/>
        <source>Reconnect</source>
        <translation>Переподключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="877"/>
        <source>Disconnect</source>
        <translation>Отключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="885"/>
        <location filename="../src/bmainwindow.cpp" line="1718"/>
        <location filename="../src/bmainwindow.cpp" line="2496"/>
        <source>Connect to Director</source>
        <translation>Подключиться к Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="891"/>
        <source>Reconnect to last used connection</source>
        <translation>Переподключиться к последнему использованному соединению</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="895"/>
        <location filename="../src/bmainwindow.cpp" line="2037"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="901"/>
        <location filename="../src/bmainwindow.cpp" line="2038"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="906"/>
        <location filename="../src/bmainwindow.cpp" line="2039"/>
        <source>Exit</source>
        <translation>Выход</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="911"/>
        <location filename="../src/bmainwindow.cpp" line="1859"/>
        <source>About Onesimus</source>
        <translation>О программе Onesimus</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="915"/>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="919"/>
        <source>Online Documentation</source>
        <translation>Документация в сети</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="924"/>
        <source>Report a Bug...</source>
        <translation>Сообщить об ошибке...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="928"/>
        <location filename="../src/bmainwindow.cpp" line="2015"/>
        <location filename="../src/bmainwindow.cpp" line="2021"/>
        <location filename="../src/bmainwindow.cpp" line="2103"/>
        <source>Keyboard Shortcuts</source>
        <translation>Сочетания клавиш</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="933"/>
        <location filename="../src/bmainwindow.cpp" line="2433"/>
        <source>My Permissions</source>
        <translation>Мои права</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="939"/>
        <location filename="../src/bmainwindow.cpp" line="2052"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="945"/>
        <location filename="../src/bmainwindow.cpp" line="2053"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="951"/>
        <source>Clear Selection</source>
        <translation>Снять выделение</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="956"/>
        <source>Find...</source>
        <translation>Найти...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="963"/>
        <location filename="../src/bmainwindow.cpp" line="2067"/>
        <source>Show Statistics</source>
        <translation>Показать статистику</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="977"/>
        <location filename="../src/bmainwindow.cpp" line="2068"/>
        <source>Show Job Log</source>
        <translation>Показать журнал Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="992"/>
        <location filename="../src/bmainwindow.cpp" line="2070"/>
        <source>Interface Zoom In</source>
        <translation>Увеличить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="998"/>
        <location filename="../src/bmainwindow.cpp" line="2071"/>
        <source>Interface Zoom Out</source>
        <translation>Уменьшить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1010"/>
        <source>Toggle Dark/Light Theme</source>
        <translation>Переключить тёмную/светлую тему</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1025"/>
        <source>Run Job</source>
        <translation>Запустить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1030"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1035"/>
        <source>Show Details</source>
        <translation>Показать подробности</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1040"/>
        <location filename="../src/bmainwindow.cpp" line="2085"/>
        <source>Refresh Jobs</source>
        <translation>Обновить Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1046"/>
        <source>Export Jobs as JSON...</source>
        <translation>Экспорт Jobs в JSON...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1051"/>
        <source>Export Jobs as CSV...</source>
        <translation>Экспорт Jobs в CSV...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1057"/>
        <source>Add Job...</source>
        <translation>Добавить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1076"/>
        <source>Edit Job...</source>
        <translation>Изменить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1082"/>
        <source>Edit Job</source>
        <translation>Изменить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1083"/>
        <source>No Jobs available.</source>
        <translation>Нет доступных Jobs.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1089"/>
        <source>Job Resources</source>
        <translation>Ресурсы Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1142"/>
        <source>Delete Job...</source>
        <translation>Удалить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1150"/>
        <source>Add JobDefs...</source>
        <translation>Добавить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1169"/>
        <source>Edit JobDefs...</source>
        <translation>Изменить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1175"/>
        <source>JobDefs Resources</source>
        <translation>Ресурсы JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1230"/>
        <source>Delete JobDefs...</source>
        <translation>Удалить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1241"/>
        <location filename="../src/bmainwindow.cpp" line="1245"/>
        <source>Delete JobDefs</source>
        <translation>Удалить JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1241"/>
        <source>No JobDefs available.</source>
        <translation>Нет доступных JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1245"/>
        <source>JobDefs to delete:</source>
        <translation>Удаляемый JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1259"/>
        <source>Jobs that use this JobDefs</source>
        <translation>Jobs, использующие этот JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1265"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1275"/>
        <source>Add FileSet...</source>
        <translation>Добавить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1283"/>
        <source>Edit FileSet...</source>
        <translation>Изменить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1291"/>
        <source>Delete FileSet...</source>
        <translation>Удалить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1300"/>
        <location filename="../src/bmainwindow.cpp" line="1306"/>
        <location filename="../src/bmainwindow.cpp" line="1352"/>
        <location filename="../src/bmainwindow.cpp" line="1357"/>
        <source>Delete FileSet</source>
        <translation>Удалить FileSet</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1301"/>
        <source>No FileSets available.</source>
        <translation>Нет доступных FileSets.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1311"/>
        <source>Select the FileSet to delete:</source>
        <translation>Выберите FileSet для удаления:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1322"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This will remove the FileSet resource from the Director configuration. This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; ресурс FileSet будет удалён из конфигурации Director. Это действие нельзя отменить!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1333"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1345"/>
        <source>Confirm Delete</source>
        <translation>Подтверждение удаления</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1346"/>
        <source>Are you sure you want to delete FileSet &quot;%1&quot;?</source>
        <translation>Вы действительно хотите удалить FileSet «%1»?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1353"/>
        <source>Not connected to a Director. FileSet &quot;%1&quot; was not deleted.</source>
        <translation>Нет подключения к Director. FileSet «%1» не удалён.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1358"/>
        <source>FileSet &quot;%1&quot; no longer exists on the Director. Nothing was deleted.</source>
        <translation>FileSet «%1» больше не существует на Director. Ничего не удалено.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1366"/>
        <source>Add New Client...</source>
        <translation>Добавить новый Client...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1374"/>
        <location filename="../src/bmainwindow.cpp" line="2086"/>
        <source>Refresh Clients</source>
        <translation>Обновить Clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1380"/>
        <source>Show Client Details</source>
        <translation>Показать сведения о Client</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1390"/>
        <location filename="../src/bmainwindow.cpp" line="2087"/>
        <source>Refresh Storage</source>
        <translation>Обновить Storage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1397"/>
        <source>Add Schedule...</source>
        <translation>Добавить Schedule...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1417"/>
        <location filename="../src/bmainwindow.cpp" line="2088"/>
        <source>Refresh Schedules</source>
        <translation>Обновить Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1424"/>
        <source>Database Cleanup...</source>
        <translation>Очистка базы данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1426"/>
        <source>Clean up old backups and free disk space</source>
        <translation>Удалить старые резервные копии и освободить место на диске</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1430"/>
        <source>Connection Wizard...</source>
        <translation>Мастер подключения...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1432"/>
        <source>Set up a new director connection</source>
        <translation>Настроить новое соединение с Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1435"/>
        <source>Import Config...</source>
        <translation>Импорт конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1437"/>
        <source>Import Director configuration from files or ZIP archive</source>
        <translation>Импортировать конфигурацию Director из файлов или ZIP-архива</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1446"/>
        <location filename="../src/bmainwindow.cpp" line="1486"/>
        <location filename="../src/bmainwindow.cpp" line="1502"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1452"/>
        <source>System (%1)</source>
        <translation>Системный (%1)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1487"/>
        <source>Onesimus will speak %1 after a restart.</source>
        <translation>После перезапуска Onesimus будет работать на языке: %1.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1490"/>
        <source>Restart now? The connection to the Director is closed and opened again if automatic connection is on.</source>
        <translation>Перезапустить сейчас? Соединение с Director будет закрыто и, если включено автоматическое подключение, установлено заново.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1492"/>
        <source>Restart now?</source>
        <translation>Перезапустить сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1493"/>
        <source>Restart now</source>
        <translation>Перезапустить сейчас</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1494"/>
        <source>Later</source>
        <translation>Позже</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1503"/>
        <source>Onesimus could not be started again. The new language takes effect at the next start.</source>
        <translation>Не удалось перезапустить Onesimus. Новый язык вступит в силу при следующем запуске.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1513"/>
        <location filename="../src/bmainwindow.cpp" line="2043"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1527"/>
        <location filename="../src/bmainwindow.cpp" line="2058"/>
        <source>Edit</source>
        <translation>Правка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1536"/>
        <location filename="../src/bmainwindow.cpp" line="2076"/>
        <source>View</source>
        <translation>Вид</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1605"/>
        <source>ACL matrix of the consoles...</source>
        <translation>Матрица ACL консолей...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1606"/>
        <source>Which console may use which job, client, storage, schedule, pool, fileset, catalog and command (Onesimus Director)</source>
        <translation>Какая консоль может использовать какие Job, Client, Storage, Schedule, Pool, FileSet, Catalog и команды (Onesimus Director)</translation>
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
            <numerusform>Скопирована %n строка</numerusform>
            <numerusform>Скопировано %n строки</numerusform>
            <numerusform>Скопировано %n строк</numerusform>
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
        <translation>Изменение Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1561"/>
        <source>Modify JobDefs</source>
        <translation>Изменение JobDefs</translation>
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
        <translation>Onesimus - управление резервным копированием Bacula</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="127"/>
        <source>Onesimus - Bareos Backup Management</source>
        <translation>Onesimus - управление резервным копированием Bareos</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1594"/>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1610"/>
        <location filename="../src/bmainwindow.cpp" line="2107"/>
        <source>Help</source>
        <translation>Справка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1639"/>
        <source>Main Toolbar</source>
        <translation>Главная панель инструментов</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1668"/>
        <location filename="../src/bmainwindow.cpp" line="1682"/>
        <location filename="../src/bmainwindow.cpp" line="2585"/>
        <source>Statistics ▼</source>
        <translation>Статистика ▼</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1671"/>
        <source>Show/Hide Statistics</source>
        <translation>Показать/скрыть статистику</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1682"/>
        <location filename="../src/bmainwindow.cpp" line="2585"/>
        <location filename="../src/bmainwindow.cpp" line="2621"/>
        <source>Statistics ▶</source>
        <translation>Статистика ▶</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1710"/>
        <source>No Connection Profiles</source>
        <translation>Нет профилей соединений</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1711"/>
        <source>No connection profiles found.

Please create a connection profile in Settings first.</source>
        <translation>Профили соединений не найдены.

Сначала создайте профиль соединения в настройках.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1725"/>
        <source>Select Connection Profile</source>
        <translation>Выбор профиля соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1731"/>
        <source>Connection Profile</source>
        <translation>Профиль соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1781"/>
        <source>Manage Profiles...</source>
        <translation>Управление профилями...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1809"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1810"/>
        <source>Selected profile is invalid.</source>
        <translation>Выбранный профиль недействителен.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2908"/>
        <location filename="../src/bmainwindow.cpp" line="2971"/>
        <source>Connecting to %1...</source>
        <translation>Подключение к %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1881"/>
        <source>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Version %1 (Build %2)&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Версия %1 (сборка %2)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1888"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bacula Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Современный графический интерфейс на Qt6 для резервного копирования Bacula&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1890"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bareos Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Современный графический интерфейс на Qt6 для резервного копирования Bareos&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1980"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1996"/>
        <source>Documentation</source>
        <translation>Документация</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1997"/>
        <location filename="../src/bmainwindow.cpp" line="2007"/>
        <source>Could not open browser. Please visit:
%1</source>
        <translation>Не удалось открыть браузер. Перейдите по адресу:
%1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2006"/>
        <source>Report Bug</source>
        <translation>Сообщить об ошибке</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2054"/>
        <source>Find</source>
        <translation>Найти</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2069"/>
        <source>Toggle Theme</source>
        <translation>Переключить тему</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2072"/>
        <source>Reset Interface Zoom</source>
        <translation>Сбросить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2093"/>
        <source>Data</source>
        <translation>Данные</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2102"/>
        <source>Help Contents</source>
        <translation>Содержание справки</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2434"/>
        <source>Not connected to a Director.</source>
        <translation>Нет подключения к Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2456"/>
        <source>Settings saved</source>
        <translation>Настройки сохранены</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2493"/>
        <source>Disconnect from Director</source>
        <translation>Отключиться от Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2571"/>
        <source>Connected to %1 version %2</source>
        <translation>Подключено к %1, версия %2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2573"/>
        <source>Connected: version %1</source>
        <translation>Подключено: версия %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2579"/>
        <source>Connected - Loading data...</source>
        <translation>Подключено — загрузка данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2644"/>
        <source>Error: </source>
        <translation>Ошибка: </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2651"/>
        <source>Connection Error</source>
        <translation>Ошибка соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2666"/>
        <source>Refreshing data...</source>
        <translation>Обновление данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2706"/>
        <source>No Saved Connection</source>
        <translation>Нет сохранённого соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2707"/>
        <source>No previous connection found. Please use &apos;Connect&apos; to establish a new connection.</source>
        <translation>Предыдущее соединение не найдено. Используйте «Подключить», чтобы установить новое соединение.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2907"/>
        <source>Auto-connecting to %1...</source>
        <translation>Автоподключение к %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2951"/>
        <source>Connection cancelled: server certificate not trusted</source>
        <translation>Подключение отменено: сертификат сервера не является доверенным</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3050"/>
        <source>All jobs selected</source>
        <translation>Выбраны все Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3065"/>
        <location filename="../src/bmainwindow.cpp" line="3068"/>
        <source>Selection cleared</source>
        <translation>Выделение снято</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3112"/>
        <source>Reset Interface Zoom (now %1 %)</source>
        <translation>Сбросить масштаб интерфейса (сейчас %1 %)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3123"/>
        <source>Switched to Light Theme</source>
        <translation>Включена светлая тема</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3124"/>
        <source>Switched to Dark Theme</source>
        <translation>Включена тёмная тема</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3133"/>
        <location filename="../src/bmainwindow.cpp" line="3145"/>
        <location filename="../src/bmainwindow.cpp" line="3157"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3133"/>
        <location filename="../src/bmainwindow.cpp" line="3145"/>
        <source>Please connect to the Director first.</source>
        <translation>Сначала подключитесь к Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3139"/>
        <source>Jobs exported as JSON</source>
        <translation>Jobs экспортированы в JSON</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3151"/>
        <source>Jobs exported as CSV</source>
        <translation>Jobs экспортированы в CSV</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3158"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Сначала подключитесь к Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3167"/>
        <source>Loading job data...</source>
        <translation>Загрузка данных о Jobs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3203"/>
        <source>Connection profile &apos;%1&apos; saved</source>
        <translation>Профиль соединения «%1» сохранён</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3253"/>
        <source>Password Missing</source>
        <translation>Нет пароля</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3254"/>
        <source>The imported Console &apos;%1&apos; does not contain a password.

You will need to enter the password manually in the connection settings.</source>
        <translation>Импортированная Console «%1» не содержит пароля.

Пароль потребуется ввести вручную в параметрах соединения.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3275"/>
        <source>Created connection profile &apos;%1&apos; for Director &apos;%2&apos;</source>
        <translation>Создан профиль соединения «%1» для Director «%2»</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3280"/>
        <source>Connect Now?</source>
        <translation>Подключиться сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3281"/>
        <source>Connection profile &apos;%1&apos; has been created.

Do you want to connect now?</source>
        <translation>Профиль соединения «%1» создан.

Подключиться сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3297"/>
        <source>Imported Director &apos;%1&apos; with Console &apos;%2&apos;</source>
        <translation>Импортирован Director «%1» с Console «%2»</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2089"/>
        <source>Add Schedule</source>
        <translation>Добавить Schedule</translation>
    </message>
</context>
<context>
    <name>BMdnsBrowser</name>
    <message>
        <location filename="../src/bmdnsbrowser.cpp" line="200"/>
        <source>mDNS: cannot open UDP socket: %1</source>
        <translation>mDNS: не удалось открыть сокет UDP: %1</translation>
    </message>
</context>
<context>
    <name>BMessagesResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="431"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="431"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BMessagesWidget</name>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="237"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="243"/>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="250"/>
        <source>0 messages</source>
        <translation>0 сообщений</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="286"/>
        <source>Polling every %1s</source>
        <translation>Опрос каждые %1 с</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="313"/>
        <source>Messages cleared</source>
        <translation>Сообщения очищены</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="328"/>
        <source>Not connected</source>
        <translation>Не подключено</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="369"/>
        <source>Refreshing messages...</source>
        <translation>Обновление сообщений...</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/messages/bmessageswidget.cpp" line="375"/>
        <source>%n message(s)</source>
        <translation>
            <numerusform>%n сообщение</numerusform>
            <numerusform>%n сообщения</numerusform>
            <numerusform>%n сообщений</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BNewClientDirectorPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="91"/>
        <source>Director Information</source>
        <translation>Сведения о Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="92"/>
        <source>The new client will connect to this Director. These settings are used to generate the File Daemon configuration.</source>
        <translation>Новый клиент будет подключаться к этому Director. Эти параметры используются для создания конфигурации File Daemon.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="99"/>
        <source>Director Name:</source>
        <translation>Имя Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="103"/>
        <source>Director Address:</source>
        <translation>Адрес Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="107"/>
        <source>Director Port:</source>
        <translation>Порт Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="111"/>
        <source>TLS Mode:</source>
        <translation>Режим TLS:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="119"/>
        <source>The Director name above will be used in the File Daemon&apos;s Director resource. The client authenticates against this Director using a shared password.</source>
        <translation>Указанное выше имя Director будет использовано в ресурсе Director конфигурации File Daemon. Клиент проходит аутентификацию на этом Director с помощью общего пароля.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="139"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="146"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="153"/>
        <source>&lt;i&gt;(not available)&lt;/i&gt;</source>
        <translation>&lt;i&gt;(недоступно)&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="140"/>
        <source>Director name could not be retrieved from the connection.</source>
        <translation>Не удалось получить имя Director из соединения.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="147"/>
        <source>Director address could not be retrieved.</source>
        <translation>Не удалось получить адрес Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="154"/>
        <source>Director port could not be retrieved.</source>
        <translation>Не удалось получить порт Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="159"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="164"/>
        <source>&lt;b style=&apos;color:red;&apos;&gt;Cannot proceed:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Make sure you are connected to a Director.</source>
        <translation>&lt;b style=&apos;color:red;&apos;&gt;Невозможно продолжить:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Убедитесь, что вы подключены к Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="182"/>
        <source>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; The current console may not have permission to run &apos;configure add client&apos;. Check the Command ACL in the Console resource on the Director.&lt;/span&gt;&lt;br&gt;You can still generate and export the FD configuration files.</source>
        <translation>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; у текущей консоли может не быть прав на выполнение &apos;configure add client&apos;. Проверьте Command ACL в ресурсе Console на Director.&lt;/span&gt;&lt;br&gt;Вы всё равно можете создать и экспортировать файлы конфигурации FD.</translation>
    </message>
</context>
<context>
    <name>BNewClientPreviewPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="484"/>
        <source>Preview &amp; Export</source>
        <translation>Предпросмотр и экспорт</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="485"/>
        <source>Review generated configuration files, validate, and export.</source>
        <translation>Проверьте созданные файлы конфигурации, выполните проверку и экспортируйте.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="496"/>
        <source>FD: Director</source>
        <translation>FD: Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="499"/>
        <source>FD: Client</source>
        <translation>FD: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="502"/>
        <source>FD: Messages</source>
        <translation>FD: Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="505"/>
        <source>Dir: Client</source>
        <translation>Dir: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="524"/>
        <source>Director Command</source>
        <translation>Команда Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="536"/>
        <source>Copy Current Tab</source>
        <translation>Копировать текущую вкладку</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="540"/>
        <source>Copy All</source>
        <translation>Копировать всё</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="544"/>
        <source>Export ZIP...</source>
        <translation>Экспорт ZIP...</translation>
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
        <translation>%1: не удалось разобрать синтаксис конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="799"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="808"/>
        <source>Client name &apos;%1&apos; does not end with &apos;-fd&apos;. Convention: use &apos;&lt;hostname&gt;-fd&apos;.</source>
        <translation>Имя клиента «%1» не оканчивается на «-fd». Соглашение: используйте &apos;&lt;hostname&gt;-fd&apos;.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="813"/>
        <source>Password MD5 hash has invalid length (%1, expected 32).</source>
        <translation>Хеш MD5 пароля имеет недопустимую длину (%1, ожидается 32).</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="819"/>
        <source>Validation passed.</source>
        <translation>Проверка пройдена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="912"/>
        <source>Copied &apos;%1&apos; to clipboard.</source>
        <translation>«%1» скопировано в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="937"/>
        <source>All configuration files copied to clipboard.</source>
        <translation>Все файлы конфигурации скопированы в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="949"/>
        <source>Export Client Configuration</source>
        <translation>Экспорт конфигурации клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="951"/>
        <source>ZIP Archives (*.zip)</source>
        <translation>ZIP-архивы (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="957"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1030"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1036"/>
        <source>Export Error</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="958"/>
        <source>Failed to create temporary directory.</source>
        <translation>Не удалось создать временный каталог.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1031"/>
        <source>ZIP creation timed out. Is &apos;zip&apos; installed?</source>
        <translation>Истекло время создания ZIP. Установлен ли &apos;zip&apos;?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1037"/>
        <source>ZIP creation failed:
%1</source>
        <translation>Не удалось создать ZIP:
%1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1042"/>
        <source>Exported to: %1</source>
        <translation>Экспортировано в: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1051"/>
        <source>ERROR: No director connection available.</source>
        <translation>ОШИБКА: соединение с Director недоступно.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1060"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1101"/>
        <source>Failed to add client to Director: %1
Go back to change the settings, or click Finish to send the command again.
You can still export the FD config files manually.</source>
        <translation>Не удалось добавить клиента на Director: %1
Вернитесь назад, чтобы изменить параметры, или нажмите «Готово», чтобы отправить команду ещё раз.
Вы всё равно можете экспортировать файлы конфигурации FD вручную.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1114"/>
        <source>Client &apos;%1&apos; added to Director.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен на Director.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1124"/>
        <source>Client &apos;%1&apos; added to Director. Reloading Director...</source>
        <translation>Client «%1» добавлен на Director. Перезагрузка Director...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1151"/>
        <source>Client &apos;%1&apos; added and Director reloaded successfully.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен, Director успешно перезагружен.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1158"/>
        <source>Client &apos;%1&apos; added to Director, but reload failed.
Run &apos;reload&apos; manually on the Director or restart bareos-dir.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен на Director, но перезагрузка не удалась.
Выполните &apos;reload&apos; вручную на Director или перезапустите bareos-dir.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1177"/>
        <source>Client &apos;%1&apos; added to Director, but the connection was lost before the reload was confirmed.
Reconnect and run &apos;reload&apos; on the Director if the client does not appear.</source>
        <translation>Client «%1» добавлен на Director, но соединение было потеряно до подтверждения перезагрузки.
Если клиент не появится, подключитесь снова и выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1186"/>
        <source>The connection to the Director was lost before it answered.
Reconnect and check whether client &apos;%1&apos; exists before sending the command again.</source>
        <translation>Соединение с Director было потеряно до получения ответа.
Подключитесь снова и проверьте, существует ли Client «%1», прежде чем отправлять команду повторно.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1204"/>
        <source>Client &apos;%1&apos; added to Director, but the Director did not confirm the reload within 30 seconds.
Run &apos;reload&apos; manually on the Director if the client does not appear.</source>
        <translation>Client «%1» добавлен на Director, но Director не подтвердил перезагрузку в течение 30 секунд.
Если клиент не появится, выполните &apos;reload&apos; вручную на Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1214"/>
        <source>Command Timeout</source>
        <translation>Тайм-аут команды</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1215"/>
        <source>The &apos;configure add client&apos; command did not respond within 30 seconds.

The Director may be busy or the connection may have been lost.

Retry the command?</source>
        <translation>Команда &apos;configure add client&apos; не ответила в течение 30 секунд.

Director может быть занят, или соединение могло быть потеряно.

Повторить команду?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1227"/>
        <source>Configure command timed out. The client may or may not have been created.
You can export the FD config files and configure the Director manually.</source>
        <translation>Истекло время ожидания команды configure. Клиент мог быть создан, а мог и нет.
Вы можете экспортировать файлы конфигурации FD и настроить Director вручную.</translation>
    </message>
</context>
<context>
    <name>BNewClientSettingsPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="207"/>
        <source>Client Settings</source>
        <translation>Параметры клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="208"/>
        <source>Configure the new client&apos;s identity, password, TLS, and network settings.</source>
        <translation>Задайте идентификацию, пароль, TLS и сетевые параметры нового клиента.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="213"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="217"/>
        <source>e.g. server1-fd</source>
        <translation>напр., server1-fd</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="218"/>
        <source>Client Name:</source>
        <translation>Имя клиента:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="221"/>
        <source>e.g. 192.168.1.100 or server1.example.com</source>
        <translation>напр., 192.168.1.100 или server1.example.com</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="222"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="227"/>
        <source>FD Port:</source>
        <translation>Порт FD:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="232"/>
        <source>Client password</source>
        <translation>Пароль клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="234"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="236"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="242"/>
        <source>MD5 Hash:</source>
        <translation>Хеш MD5:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="252"/>
        <source>Passive mode (client behind NAT/firewall)</source>
        <translation>Пассивный режим (клиент за NAT/межсетевым экраном)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="254"/>
        <source>Enable if the client is behind a firewall/NAT.
The Director and Storage Daemon will initiate connections to the client instead.

Firewall ports to open on the client side:
  - Inbound to File Daemon: TCP 9102</source>
        <translation>Включите, если клиент находится за межсетевым экраном/NAT.
Тогда Director и Storage Daemon сами будут устанавливать соединения с клиентом.

Порты, которые нужно открыть в межсетевом экране на стороне клиента:
  - Входящие к File Daemon: TCP 9102</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="263"/>
        <source>Connection wait time:</source>
        <translation>Время ожидания соединения:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="267"/>
        <source> sec</source>
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="268"/>
        <source>Time in seconds to wait for client-initiated connection</source>
        <translation>Время ожидания соединения, инициированного клиентом, в секундах</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="277"/>
        <source>Execute &quot;configure add client&quot; on Director</source>
        <translation>Выполнить «configure add client» на Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="280"/>
        <source>If checked, the wizard will send the configure command to the Director
on the Preview page to add this client automatically.</source>
        <translation>Если отмечено, мастер отправит команду configure на Director
на странице предпросмотра, чтобы автоматически добавить этого клиента.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="420"/>
        <source>(enter password)</source>
        <translation>(введите пароль)</translation>
    </message>
</context>
<context>
    <name>BNewClientWizard</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="56"/>
        <source>Add New Client</source>
        <translation>Добавление нового клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="70"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="72"/>
        <source>TLS Certificate</source>
        <translation>Сертификат TLS</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="75"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
</context>
<context>
    <name>BNewJobDialog</name>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="30"/>
        <source>Run New Job</source>
        <translation>Запуск нового Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="56"/>
        <source>Run Backup Job</source>
        <translation>Запуск Job резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="66"/>
        <source>Command Preview</source>
        <translation>Предпросмотр команды</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="85"/>
        <source>Estimate</source>
        <translation>Оценка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="87"/>
        <source>Calculate estimated files and size</source>
        <translation>Рассчитать ожидаемое количество файлов и объём</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="97"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="654"/>
        <source>Run Job</source>
        <translation>Запустить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="119"/>
        <source>Job Configuration</source>
        <translation>Конфигурация Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="124"/>
        <source>Select job...</source>
        <translation>Выберите Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="127"/>
        <source>Job:</source>
        <translation>Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="130"/>
        <source>Select client...</source>
        <translation>Выберите Client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="133"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="139"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="144"/>
        <source>Resources</source>
        <translation>Ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="149"/>
        <source>Select FileSet...</source>
        <translation>Выберите FileSet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="152"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="155"/>
        <source>Select Pool...</source>
        <translation>Выберите Pool...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="158"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="161"/>
        <source>Select Storage...</source>
        <translation>Выберите Storage...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="164"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="169"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="175"/>
        <source>Lower values = higher priority. Only sent if changed here; otherwise the job&apos;s own priority applies.</source>
        <translation>Меньшее значение = более высокий приоритет. Отправляется, только если изменено здесь; иначе действует собственный приоритет Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="182"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="188"/>
        <source>Basic</source>
        <translation>Основные</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="198"/>
        <source>Scheduling</source>
        <translation>Планирование</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="202"/>
        <source>e.g. &quot;2024-01-15 14:30:00&quot; or empty for immediate</source>
        <translation>напр., «2024-01-15 14:30:00» или пусто для немедленного запуска</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="203"/>
        <source>Start time in the form YYYY-MM-DD HH:MM:SS</source>
        <translation>Время начала в формате YYYY-MM-DD HH:MM:SS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="205"/>
        <source>When:</source>
        <translation>Когда:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="210"/>
        <source>Bootstrap Options</source>
        <translation>Параметры Bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="213"/>
        <source>Use Bootstrap file</source>
        <translation>Использовать файл Bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="218"/>
        <source>Path to Bootstrap file</source>
        <translation>Путь к файлу Bootstrap</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bnewjobdialog.cpp" line="708"/>
        <source>Estimate: %n file(s), %1</source>
        <translation type="unfinished">
            <numerusform></numerusform>
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
        <translation>Дополнительно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="235"/>
        <source>⚠ No JobWidget connection available</source>
        <translation>⚠ Нет соединения с JobWidget</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="240"/>
        <source>Loading configuration data from JobWidget...</source>
        <translation>Загрузка данных конфигурации из JobWidget...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="278"/>
        <source>✓ %1 jobs, %2 clients loaded</source>
        <translation>✓ Загружено: Jobs — %1, клиентов — %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="284"/>
        <source>⚠ No jobs available - check connection</source>
        <translation>⚠ Нет доступных Jobs — проверьте соединение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="372"/>
        <source>✓ Configuration loaded</source>
        <translation>✓ Конфигурация загружена</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="411"/>
        <source>Loading job defaults...</source>
        <translation>Загрузка параметров Job по умолчанию...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="439"/>
        <source>Failed to load job defaults</source>
        <translation>Не удалось загрузить параметры Job по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="550"/>
        <source>✓ Job defaults loaded</source>
        <translation>✓ Параметры Job по умолчанию загружены</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="581"/>
        <source>No job selected.</source>
        <translation>Job не выбран.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="583"/>
        <source>The value for &apos;%1&apos; contains a line break or control character.</source>
        <translation>Значение для «%1» содержит разрыв строки или управляющий символ.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="585"/>
        <source>&apos;When&apos; must be a valid time in the form YYYY-MM-DD HH:MM:SS.</source>
        <translation>«Когда» должно быть допустимым временем в формате YYYY-MM-DD HH:MM:SS.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="587"/>
        <source>The priority must be a positive number.</source>
        <translation>Приоритет должен быть положительным числом.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="589"/>
        <source>The command could not be built.</source>
        <translation>Не удалось сформировать команду.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="607"/>
        <source>Nothing will be sent: %1</source>
        <translation>Ничего не будет отправлено: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="634"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="675"/>
        <source>Missing Input</source>
        <translation>Недостаточно данных</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="635"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="676"/>
        <source>Please select a job.</source>
        <translation>Выберите Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="643"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="683"/>
        <source>Invalid Input</source>
        <translation>Недопустимые данные</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="655"/>
        <source>Do you want to run the following job?

%1</source>
        <translation>Запустить следующий Job?

%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="698"/>
        <source>Calculating estimate...</source>
        <translation>Расчёт оценки...</translation>
    </message>
</context>
<context>
    <name>BPFXConverter</name>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="25"/>
        <source>Convert to PFX Format</source>
        <translation>Преобразование в формат PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="33"/>
        <source>Convert separate PEM/DER certificate and key files into a single PFX file for Windows.
PFX files are password-protected and contain both certificate and private key.</source>
        <translation>Преобразование отдельных файлов сертификата и ключа PEM/DER в единый файл PFX для Windows.
Файлы PFX защищены паролем и содержат как сертификат, так и закрытый ключ.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="42"/>
        <source>Input Files</source>
        <translation>Входные файлы</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="48"/>
        <source>Select certificate file...</source>
        <translation>Выберите файл сертификата...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="49"/>
        <location filename="../src/bpfxconverter.cpp" line="59"/>
        <location filename="../src/bpfxconverter.cpp" line="69"/>
        <location filename="../src/bpfxconverter.cpp" line="100"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="53"/>
        <source>Certificate:</source>
        <translation>Сертификат:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="58"/>
        <source>Select private key file...</source>
        <translation>Выберите файл закрытого ключа...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="63"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="68"/>
        <source>Optional: CA certificate</source>
        <translation>Необязательно: сертификат CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="73"/>
        <source>CA Cert (Optional):</source>
        <translation>Сертификат CA (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="78"/>
        <source>PFX Password Protection</source>
        <translation>Защита PFX паролем</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="83"/>
        <source>Enter password for PFX file...</source>
        <translation>Введите пароль для файла PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="84"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="88"/>
        <source>Confirm password...</source>
        <translation>Подтвердите пароль...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="89"/>
        <source>Confirm:</source>
        <translation>Подтверждение:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="94"/>
        <source>Output</source>
        <translation>Вывод</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="99"/>
        <source>Select where to save PFX file...</source>
        <translation>Выберите, куда сохранить файл PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="103"/>
        <source>Save PFX File</source>
        <translation>Сохранение файла PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="105"/>
        <source>PFX Files (*.pfx);;All Files (*)</source>
        <translation>Файлы PFX (*.pfx);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="115"/>
        <source>Save As:</source>
        <translation>Сохранить как:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="120"/>
        <source>Conversion Log:</source>
        <translation>Журнал преобразования:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="131"/>
        <source>Convert to PFX</source>
        <translation>Преобразовать в PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="135"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="146"/>
        <source>Select Certificate File</source>
        <translation>Выбор файла сертификата</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="148"/>
        <source>Certificates (*.pem *.crt *.cert *.der);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert *.der);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="151"/>
        <source>✓ Certificate selected: %1</source>
        <translation>✓ Выбран сертификат: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="158"/>
        <source>Select Private Key File</source>
        <translation>Выбор файла закрытого ключа</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="160"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Закрытые ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="163"/>
        <source>✓ Private key selected: %1</source>
        <translation>✓ Выбран закрытый ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="170"/>
        <source>Select CA Certificate</source>
        <translation>Выбор сертификата CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="172"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="175"/>
        <source>✓ CA certificate selected: %1</source>
        <translation>✓ Выбран сертификат CA: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="183"/>
        <location filename="../src/bpfxconverter.cpp" line="190"/>
        <location filename="../src/bpfxconverter.cpp" line="197"/>
        <source>Missing Input</source>
        <translation>Недостаточно данных</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="184"/>
        <source>Please select a certificate file.</source>
        <translation>Выберите файл сертификата.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="191"/>
        <source>Please select a private key file.</source>
        <translation>Выберите файл закрытого ключа.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="198"/>
        <source>Please enter a password to protect the PFX file.</source>
        <translation>Введите пароль для защиты файла PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="204"/>
        <source>Password Mismatch</source>
        <translation>Пароли не совпадают</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="205"/>
        <source>The passwords do not match. Please try again.</source>
        <translation>Пароли не совпадают. Повторите попытку.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="211"/>
        <source>Missing Output</source>
        <translation>Не указан выходной файл</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="212"/>
        <source>Please specify where to save the PFX file.</source>
        <translation>Укажите, куда сохранить файл PFX.</translation>
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
        <translation>Начало преобразования в PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="232"/>
        <source>✓ Conversion successful!</source>
        <translation>✓ Преобразование выполнено успешно!</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="233"/>
        <source>PFX file created: %1</source>
        <translation>Файл PFX создан: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="238"/>
        <source>Success</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="239"/>
        <source>PFX file created successfully!

%1

You can now use this file in the certificate settings.</source>
        <translation>Файл PFX успешно создан!

%1

Теперь этот файл можно использовать в параметрах сертификата.</translation>
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
        <translation>Загрузка сертификата из: %1</translation>
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
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="266"/>
        <source>Loading private key from: %1</source>
        <translation>Загрузка закрытого ключа из: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="279"/>
        <source>Creating PFX (PKCS#12) file...</source>
        <translation>Создание файла PFX (PKCS#12)...</translation>
    </message>
</context>
<context>
    <name>BPaginationWidget</name>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="29"/>
        <source>Enable Pagination</source>
        <translation>Включить постраничный вывод</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="35"/>
        <source>Items per page:</source>
        <translation>Элементов на странице:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="49"/>
        <source>First Page</source>
        <translation>Первая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="55"/>
        <source>Previous Page</source>
        <translation>Предыдущая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="61"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="314"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="337"/>
        <source>Page 0 of 0</source>
        <translation>Страница 0 из 0</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="67"/>
        <source>Next Page</source>
        <translation>Следующая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="73"/>
        <source>Last Page</source>
        <translation>Последняя страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="81"/>
        <source>Go to page:</source>
        <translation>Перейти к странице:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="88"/>
        <source>Go</source>
        <translation>Перейти</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="348"/>
        <source>Page %1 of %2</source>
        <translation>Страница %1 из %2</translation>
    </message>
</context>
<context>
    <name>BPermissionsDialog</name>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2135"/>
        <source>My Permissions</source>
        <translation>Мои права</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2148"/>
        <source>Connection</source>
        <translation>Соединение</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2150"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2151"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2158"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2160"/>
        <source>TLS Certificate</source>
        <translation>Сертификат TLS</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2162"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2164"/>
        <source>Encryption:</source>
        <translation>Шифрование:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2170"/>
        <source>Backup &amp; Restore</source>
        <translation>Резервное копирование и восстановление</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2171"/>
        <source>Job Control</source>
        <translation>Управление Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2172"/>
        <source>Administration</source>
        <translation>Администрирование</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2174"/>
        <source>Media</source>
        <translation>Носители</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2185"/>
        <source>Command Permissions</source>
        <translation>Права на команды</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2192"/>
        <source>Checking...</source>
        <translation>Проверка...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2201"/>
        <source>Restore Details</source>
        <translation>Сведения о Restore</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2203"/>
        <location filename="../src/bmainwindow.cpp" line="2212"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2210"/>
        <source>Other Available Commands</source>
        <translation>Другие доступные команды</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2219"/>
        <source>Raw Director Response</source>
        <translation>Необработанный ответ Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2237"/>
        <source>Querying Director...</source>
        <translation>Запрос к Director...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2326"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2329"/>
        <source>Authorized</source>
        <translation>Разрешено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2333"/>
        <source>Not authorized</source>
        <translation>Не разрешено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2361"/>
        <source>&lt;b&gt;Permission:&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;Право:&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2363"/>
        <source>&lt;b&gt;where= (restore path):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;where= (путь восстановления):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2365"/>
        <source>&lt;b&gt;client= (target client):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;client= (целевой клиент):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2367"/>
        <source>&lt;b&gt;replace= (replace policy):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;replace= (политика замены):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2371"/>
        <source>&lt;br&gt;&lt;b&gt;Available parameters:&lt;/b&gt; %1</source>
        <translation>&lt;br&gt;&lt;b&gt;Доступные параметры:&lt;/b&gt; %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2392"/>
        <source>Total: %1 commands (%2 authorized, %3 denied)</source>
        <translation>Всего команд: %1 (разрешено %2, запрещено %3)</translation>
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
        <translation>Имя</translation>
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
        <translation>Дополнительные параметры профиля — %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="44"/>
        <source>General</source>
        <translation>Общие</translation>
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
        <translation>Необязательное описание этого соединения</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="70"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="75"/>
        <source> seconds</source>
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="76"/>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="77"/>
        <source>Keepalive interval (0 = disabled)</source>
        <translation>Интервал keepalive (0 = выключено)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="78"/>
        <source>Heartbeat Interval:</source>
        <translation>Heartbeat Interval:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="82"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="83"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="89"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="91"/>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="182"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="96"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Порт gRPC Director (по умолчанию: порт BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="97"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="106"/>
        <source>Forget certificate</source>
        <translation>Забыть сертификат</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="107"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Удалить закреплённый сертификат. При следующем подключении снова будет задан вопрос, доверять ли сертификату сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="110"/>
        <source>Server certificate:</source>
        <translation>Сертификат сервера:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="122"/>
        <source>e.g., operator or readonly</source>
        <translation>напр., operator или readonly</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="123"/>
        <source>Reference to a Profile resource on the Director.
The Profile defines ACLs for this console.</source>
        <translation>Ссылка на ресурс Profile на Director.
Profile определяет ACL для этой консоли.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="126"/>
        <source>Director Profile:</source>
        <translation>Profile Director:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="130"/>
        <source>&lt;b&gt;Note:&lt;/b&gt; The Director Profile is a server-side resource that defines permissions. If set, the ACLs in this profile are ignored in favor of the server-side Profile.</source>
        <translation>&lt;b&gt;Примечание:&lt;/b&gt; Profile Director — это ресурс на стороне сервера, определяющий права. Если он задан, ACL этого профиля игнорируются в пользу Profile на стороне сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="150"/>
        <source>TLS Options</source>
        <translation>Параметры TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="153"/>
        <source>TLS Required</source>
        <translation>TLS обязателен</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="154"/>
        <source>Require TLS for all connections (recommended)</source>
        <translation>Требовать TLS для всех соединений (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="157"/>
        <source>TLS Authenticate Only</source>
        <translation>TLS только для аутентификации</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="158"/>
        <source>Use TLS only for authentication, not for data encryption</source>
        <translation>Использовать TLS только для аутентификации, без шифрования данных</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="164"/>
        <source>Cipher Configuration</source>
        <translation>Настройка шифров</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="168"/>
        <source>TLSv1.2 ciphers (colon-separated)</source>
        <translation>Шифры TLSv1.2 (через двоеточие)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="169"/>
        <source>OpenSSL cipher list for TLSv1.2 connections
Example: HIGH:!aNULL:!MD5</source>
        <translation>Список шифров OpenSSL для соединений TLSv1.2
Пример: HIGH:!aNULL:!MD5</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="171"/>
        <source>Cipher List (TLSv1.2):</source>
        <translation>Список шифров (TLSv1.2):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="174"/>
        <source>TLSv1.3 cipher suites (colon-separated)</source>
        <translation>Наборы шифров TLSv1.3 (через двоеточие)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="175"/>
        <source>OpenSSL cipher suites for TLSv1.3 connections
Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</source>
        <translation>Наборы шифров OpenSSL для соединений TLSv1.3
Пример: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="177"/>
        <source>Cipher Suites (TLSv1.3):</source>
        <translation>Наборы шифров (TLSv1.3):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="180"/>
        <source>e.g., -ALL,TLSv1.2,TLSv1.3</source>
        <translation>напр., -ALL,TLSv1.2,TLSv1.3</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="181"/>
        <source>OpenSSL protocol string</source>
        <translation>Строка протоколов OpenSSL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="187"/>
        <source>Certificate Files</source>
        <translation>Файлы сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="191"/>
        <source>DH Parameters:</source>
        <translation>Параметры DH:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="193"/>
        <source>Path to Diffie-Hellman parameters file</source>
        <translation>Путь к файлу параметров Диффи — Хеллмана</translation>
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
        <translation>Выбор файла параметров DH</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="199"/>
        <source>PEM Files (*.pem);;All Files (*)</source>
        <translation>Файлы PEM (*.pem);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="207"/>
        <source>CRL File:</source>
        <translation>Файл CRL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="209"/>
        <source>Path to Certificate Revocation List file</source>
        <translation>Путь к файлу списка отзыва сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="214"/>
        <source>Select CRL File</source>
        <translation>Выбор файла CRL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="215"/>
        <source>CRL Files (*.crl *.pem);;All Files (*)</source>
        <translation>Файлы CRL (*.crl *.pem);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="225"/>
        <source>Allowed Certificate Common Names</source>
        <translation>Разрешённые общие имена сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="229"/>
        <source>One Common Name per line</source>
        <translation>Одно общее имя на строку</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="230"/>
        <source>List of allowed certificate Common Names.
Leave empty to allow all valid certificates.</source>
        <translation>Список разрешённых общих имён (Common Name) сертификатов.
Оставьте пустым, чтобы разрешить все действительные сертификаты.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="253"/>
        <source>&lt;b&gt;Access Control Lists (ACLs)&lt;/b&gt;&lt;br&gt;Define which resources this console can access. One entry per line. Use &lt;code&gt;*all*&lt;/code&gt; to allow all resources of that type. Regex patterns are supported for Job ACL.</source>
        <translation>&lt;b&gt;Списки управления доступом (ACL)&lt;/b&gt;&lt;br&gt;Определяют, к каким ресурсам имеет доступ эта консоль. Одна запись на строку. Используйте &lt;code&gt;*all*&lt;/code&gt;, чтобы разрешить все ресурсы этого типа. Для Job ACL поддерживаются регулярные выражения.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="275"/>
        <source>Job ACL:</source>
        <translation>Job ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>Allowed jobs</source>
        <translation>Разрешённые Jobs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>List of allowed job names (regex supported)</source>
        <translation>Список разрешённых имён Jobs (поддерживаются регулярные выражения)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="279"/>
        <source>Client ACL:</source>
        <translation>Client ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280"/>
        <source>Allowed clients</source>
        <translation>Разрешённые Clients</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280"/>
        <source>List of allowed client names</source>
        <translation>Список разрешённых имён Clients</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="284"/>
        <source>Storage ACL:</source>
        <translation>Storage ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>Allowed storages</source>
        <translation>Разрешённые Storages</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>List of allowed storage names</source>
        <translation>Список разрешённых имён Storages</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="288"/>
        <source>Pool ACL:</source>
        <translation>Pool ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289"/>
        <source>Allowed pools</source>
        <translation>Разрешённые Pools</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289"/>
        <source>List of allowed pool names</source>
        <translation>Список разрешённых имён Pools</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="293"/>
        <source>Schedule ACL:</source>
        <translation>Schedule ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>Allowed schedules</source>
        <translation>Разрешённые Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>List of allowed schedule names</source>
        <translation>Список разрешённых имён Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="297"/>
        <source>FileSet ACL:</source>
        <translation>FileSet ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298"/>
        <source>Allowed filesets</source>
        <translation>Разрешённые FileSets</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298"/>
        <source>List of allowed fileset names</source>
        <translation>Список разрешённых имён FileSets</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="302"/>
        <source>Catalog ACL:</source>
        <translation>Catalog ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>Allowed catalogs</source>
        <translation>Разрешённые Catalogs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>List of allowed catalog names</source>
        <translation>Список разрешённых имён Catalogs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="306"/>
        <source>Command ACL:</source>
        <translation>Command ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307"/>
        <source>Allowed commands</source>
        <translation>Разрешённые команды</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307"/>
        <source>List of allowed commands</source>
        <translation>Список разрешённых команд</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="311"/>
        <source>Where ACL:</source>
        <translation>Where ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>Allowed restore locations</source>
        <translation>Разрешённые места восстановления</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>List of allowed restore locations</source>
        <translation>Список разрешённых мест восстановления</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="315"/>
        <source>Plugin Options ACL:</source>
        <translation>Plugin Options ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="316"/>
        <source>Allowed plugin options</source>
        <translation>Разрешённые параметры плагинов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="316"/>
        <source>List of allowed plugin options</source>
        <translation>Список разрешённых параметров плагинов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="416"/>
        <source>Not used with gRPC: the connection is always encrypted with TLS, the server certificate is confirmed on first use.</source>
        <translation>Не используется с gRPC: соединение всегда шифруется TLS, сертификат сервера подтверждается при первом использовании.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="431"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(не закреплён — будет подтверждён при следующем подключении)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="437"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="449"/>
        <source>Reset to Defaults</source>
        <translation>Сбросить по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="450"/>
        <source>Are you sure you want to reset all advanced settings to their default values?</source>
        <translation>Вы действительно хотите сбросить все дополнительные параметры к значениям по умолчанию?</translation>
    </message>
</context>
<context>
    <name>BRecycleForecast</name>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="69"/>
        <source>now</source>
        <translation>сейчас</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="70"/>
        <source>never</source>
        <translation>никогда</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="73"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="81"/>
        <source>The Director did not report the status of this volume.</source>
        <translation>Director не сообщил статус этого Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="86"/>
        <source>Archived: Bareos never prunes or recycles an archived volume.</source>
        <translation>Archived: Bareos никогда не выполняет prune и recycle для архивного Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="89"/>
        <source>Disabled: Bareos neither writes to nor recycles a disabled volume.</source>
        <translation>Disabled: Bareos не записывает на выключенный Volume и не выполняет для него recycle.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="93"/>
        <source>Error: Bareos never prunes a volume in Error, so it keeps its space until its status is changed. The job records on it still expire.</source>
        <translation>Error: Bareos никогда не выполняет prune для Volume в состоянии Error, поэтому он занимает место, пока его статус не будет изменён. Записи о Jobs на нём всё равно истекают.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="97"/>
        <source>Read-Only: Bareos does not prune or recycle a read-only volume.</source>
        <translation>Read-Only: Bareos не выполняет prune и recycle для Volume только для чтения.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="100"/>
        <source>Cleaning tape: never used for data.</source>
        <translation>Чистящая лента: никогда не используется для данных.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="105"/>
        <source>Recycle = no: the volume is never reused; its data stays until it is relabeled or deleted.</source>
        <translation>Recycle = no: Volume никогда не используется повторно; его данные остаются, пока он не будет переразмечен или удалён.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="109"/>
        <source>Purged: no job needs it any more, reusable now.</source>
        <translation>Purged: ни одному Job он больше не нужен, может быть использован повторно сейчас.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="113"/>
        <source>It still holds %1 on disk until it is recycled or truncated.</source>
        <translation>Он по-прежнему занимает %1 на диске, пока не будет выполнен recycle или truncate.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="119"/>
        <source>Marked for recycling: reused at the next need of its pool.</source>
        <translation>Отмечен для recycle: будет использован повторно при следующей потребности его Pool.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="122"/>
        <source>Busy: in use by a job right now.</source>
        <translation>Busy: сейчас используется Job.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="125"/>
        <source>Unlabeled: not yet written.</source>
        <translation>Без метки: ещё не записан.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="131"/>
        <source>Append: the short volume list does not report the Volume Use Duration, so it is not known whether this volume stays open until it is full.</source>
        <translation>Append: краткий список Volumes не сообщает Volume Use Duration, поэтому неизвестно, останется ли этот Volume открытым до заполнения.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="135"/>
        <source>Append without Volume Use Duration: it stays open until it is full</source>
        <translation>Append без Volume Use Duration: остаётся открытым, пока не заполнится</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="137"/>
        <source>(%1)</source>
        <translation>(%1)</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="140"/>
        <source>or holds %1 jobs</source>
        <translation>или не вместит %1 Jobs</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="142"/>
        <source>; nothing on it is freed before that.</source>
        <translation>; до этого на нём ничего не освобождается.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="148"/>
        <source>Append with a use duration of %1, but the first write or the retention is not reported.</source>
        <translation>Append с длительностью использования %1, но первая запись или срок хранения не сообщаются.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="157"/>
        <source>Append until %1 (use duration %2), then %3 of volume retention.</source>
        <translation>Append до %1 (длительность использования %2), затем %3 срока хранения Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="165"/>
        <source>%1: the last write or the volume retention is not reported.</source>
        <translation>%1: последняя запись или срок хранения Volume не сообщаются.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="170"/>
        <source>%1: last written %2, volume retention %3.</source>
        <translation>%1: последняя запись %2, срок хранения Volume %3.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="181"/>
        <source>%1: the job retention of %2 ends before the volume retention of %3; the jobs are pruned first and the volume becomes Purged.</source>
        <translation>%1: срок хранения Jobs (%2) истекает раньше срока хранения Volume (%3); сначала выполняется prune для Jobs, и Volume получает статус Purged.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="189"/>
        <source>The date has passed: it is pruned when its pool next needs a volume.</source>
        <translation>Дата прошла: prune будет выполнен, когда его Pool в следующий раз понадобится Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="194"/>
        <source>Status &quot;%1&quot; is not known to this version.</source>
        <translation>Статус «%1» неизвестен этой версии.</translation>
    </message>
</context>
<context>
    <name>BResourceDialog</name>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="53"/>
        <source>Select %1:</source>
        <translation>Выберите %1:</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="71"/>
        <source>Edit %1 Resource</source>
        <translation>Изменение ресурса %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="81"/>
        <source>Show advanced directives</source>
        <translation>Показать дополнительные директивы</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="90"/>
        <location filename="../src/director/bresourcedialog.cpp" line="253"/>
        <source>Save .conf</source>
        <translation>Сохранить .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="95"/>
        <location filename="../src/director/bresourcedialog.cpp" line="330"/>
        <source>Save ZIP</source>
        <translation>Сохранить ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="228"/>
        <source>Save %1 Configuration</source>
        <translation>Сохранение конфигурации %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="229"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="235"/>
        <location filename="../src/director/bresourcedialog.cpp" line="243"/>
        <location filename="../src/director/bresourcedialog.cpp" line="278"/>
        <location filename="../src/director/bresourcedialog.cpp" line="286"/>
        <source>Save Failed</source>
        <translation>Ошибка сохранения</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="236"/>
        <location filename="../src/director/bresourcedialog.cpp" line="279"/>
        <source>%1 &quot;%2&quot; cannot be written. %3</source>
        <translation>%1 «%2» не удаётся записать. %3</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="244"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="251"/>
        <location filename="../src/director/bresourcedialog.cpp" line="328"/>
        <source>Saved!</source>
        <translation>Сохранено!</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="268"/>
        <source>Save %1 Configuration as ZIP</source>
        <translation>Сохранение конфигурации %1 в ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="269"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="287"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
</context>
<context>
    <name>BResourceForm</name>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="157"/>
        <source>No schema available for resource type &apos;%1&apos;</source>
        <translation>Нет схемы для типа ресурса «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="186"/>
        <location filename="../src/config/bresourceform.cpp" line="198"/>
        <location filename="../src/config/bresourceform.cpp" line="199"/>
        <location filename="../src/config/bresourceform.cpp" line="202"/>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="383"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="391"/>
        <source>Show/Hide password</source>
        <translation>Показать/скрыть пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="417"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="419"/>
        <source>Browse for file</source>
        <translation>Выбрать файл</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="419"/>
        <source>Browse for directory</source>
        <translation>Выбрать каталог</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="423"/>
        <source>Select File</source>
        <translation>Выбор файла</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="425"/>
        <source>Select Directory</source>
        <translation>Выбор каталога</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="454"/>
        <source>&lt;%1 name&gt;</source>
        <translation>&lt;имя %1&gt;</translation>
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
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="38"/>
        <source>JobDefs resource</source>
        <translation>ресурс JobDefs</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="39"/>
        <source>client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="40"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="41"/>
        <source>pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="42"/>
        <source>storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="45"/>
        <source>resource</source>
        <translation>ресурс</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="52"/>
        <source>%1 %2 was not created</source>
        <translation>%1 %2 не создан</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="53"/>
        <source>%1 %2 was not deleted</source>
        <translation>%1 %2 не удалён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="56"/>
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 не изменён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="108"/>
        <source>There is no connection to the configuration methods of the Director. Nothing was sent.</source>
        <translation>Нет подключения к методам конфигурации Director. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="121"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="142"/>
        <source>Nothing to write</source>
        <translation>Нечего записывать</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="143"/>
        <source>The text is the one the Director has.</source>
        <translation>Текст совпадает с текстом в Director.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="154"/>
        <source>The text the change was made in does not match its SHA-256. Reload the %1 and make the change again.</source>
        <translation>Текст, в котором было сделано изменение, не соответствует своему SHA-256. Перезагрузите %1 и внесите изменение заново.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="289"/>
        <source>%1 %2 created</source>
        <translation>%1 %2 создан</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="290"/>
        <source>%1 %2 deleted</source>
        <translation>%1 %2 удалён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="291"/>
        <source>%1 %2 changed</source>
        <translation>%1 %2 изменён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="293"/>
        <source>The Director wrote the configuration and reloaded it.</source>
        <translation>Director записал конфигурацию и перезагрузил её.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="294"/>
        <source>The Director wrote the configuration but did not report a reload; the change takes effect with its next reload.</source>
        <translation>Director записал конфигурацию, но не сообщил о перезагрузке; изменение вступит в силу при следующей перезагрузке.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="297"/>
        <source>The reload does not adjust everything; the Director names these follow-up commands:</source>
        <translation>Перезагрузка применяет не всё; Director указывает следующие дополнительные команды:</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="302"/>
        <source>%1 %2 was changed on the Director</source>
        <translation>%1 %2 был изменён в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="303"/>
        <source>The Director has another state of the %1 than the one this change started from. Nothing was written and nothing is overwritten. Compare the two texts, reload the %1 and make the change again.</source>
        <translation>В Director находится другое состояние %1, чем то, с которого началось это изменение. Ничего не записано и ничего не перезаписано. Сравните оба текста, перезагрузите %1 и внесите изменение заново.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="310"/>
        <source>The Director checked the new configuration and rejected it. Nothing was written.</source>
        <translation>Director проверил новую конфигурацию и отклонил её. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="314"/>
        <source>The %1 is still used. The Director does not delete a %1 that other resources name; change them first. Nothing was deleted.</source>
        <translation>%1 всё ещё используется. Director не удаляет %1, на который ссылаются другие ресурсы; сначала измените их. Ничего не удалено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="320"/>
        <source>The console of this session may not change this %1 (the ACL of its resource type). Nothing was written.</source>
        <translation>Консоли этого сеанса не разрешено изменять этот %1 (ACL его типа ресурса). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="322"/>
        <source>The console of this session may not change this %1 (its %2 ACL). Nothing was written.</source>
        <translation>Консоли этого сеанса не разрешено изменять этот %1 (его ACL %2). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="327"/>
        <source>The Director has no %1 of this name (any more). Nothing was written.</source>
        <translation>В Director (больше) нет %1 с таким именем. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="331"/>
        <source>The Director already has a resource of this name. Nothing was written.</source>
        <translation>В Director уже есть ресурс с таким именем. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="335"/>
        <source>The Director wrote the file, but reloading the configuration failed, so it put the old file back. The %1 is as it was.</source>
        <translation>Director записал файл, но перезагрузка конфигурации не удалась, поэтому он вернул старый файл. %1 остался прежним.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="341"/>
        <source>This connection has no configuration methods. Nothing was sent.</source>
        <translation>У этого подключения нет методов конфигурации. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="347"/>
        <source>The Director refused the call (%1). The console needs the command ACL &quot;configure&quot;.</source>
        <translation>Director отклонил вызов (%1). Консоли нужен ACL команд «configure».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="349"/>
        <source>The call failed (%1). Nothing was written.</source>
        <translation>Вызов не удался (%1). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="353"/>
        <source>The Director sent no status. Nothing is known to have been written.</source>
        <translation>Director не прислал статус. Неизвестно, было ли что-либо записано.</translation>
    </message>
</context>
<context>
    <name>BResourceReportDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="232"/>
        <source>file %1</source>
        <translation>файл %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="234"/>
        <source>line %1</source>
        <translation>строка %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="236"/>
        <source>column %1</source>
        <translation>столбец %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="240"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="272"/>
        <source>This was found when the Director checked the change; nothing was sent to be written.</source>
        <translation>Это обнаружено, когда Director проверял изменение; ничего не отправлено на запись.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="278"/>
        <source>The Director says: %1</source>
        <translation>Director сообщает: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="282"/>
        <source>Where the configuration is invalid</source>
        <translation>Где конфигурация недействительна</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="291"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="294"/>
        <source>Still referenced by</source>
        <translation>Всё ещё используется в</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="302"/>
        <source>Follow-up commands the Director names</source>
        <translation>Дополнительные команды, указанные Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="309"/>
        <source>They are not run by this application.</source>
        <translation>Это приложение их не выполняет.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="323"/>
        <source>On the Director now</source>
        <translation>Сейчас в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="325"/>
        <source>What was to be written (not written)</source>
        <translation>Что должно было быть записано (не записано)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="331"/>
        <source>&amp;Reload the %1</source>
        <translation>&amp;Перезагрузить %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="333"/>
        <source>Reads the %1 again from the Director; your changes are offered to be kept or discarded</source>
        <translation>Повторно считывает %1 из Director; ваши изменения можно будет сохранить или отменить</translation>
    </message>
</context>
<context>
    <name>BResourceReviewDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="83"/>
        <source>Change %1 %2</source>
        <translation>Изменить %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="84"/>
        <source>Change %1 %2 on the Director?</source>
        <translation>Изменить %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="85"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новую конфигурацию и принимает её. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="89"/>
        <source>Create %1 %2</source>
        <translation>Создать %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="90"/>
        <source>Create %1 %2 on the Director?</source>
        <translation>Создать %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="91"/>
        <source>The Director checked the new %1 and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новый %1 и принимает его. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="95"/>
        <source>Delete %1 %2</source>
        <translation>Удалить %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="96"/>
        <source>Delete %1 %2 on the Director?</source>
        <translation>Удалить %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="97"/>
        <source>The Director checked that nothing refers to the %1. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Director проверил, что на %1 ничто не ссылается. Ничего не будет удалено, пока вы не выберете «Удалить в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="110"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="113"/>
        <source>Changed directives</source>
        <translation>Изменённые директивы</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="124"/>
        <source>Text before and after (- removed, + added)</source>
        <translation>Текст до и после (- удалено, + добавлено)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="126"/>
        <source>Text of the new resource</source>
        <translation>Текст нового ресурса</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="127"/>
        <source>Text that is deleted</source>
        <translation>Удаляемый текст</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="142"/>
        <source>Affected</source>
        <translation>Затрагивает</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bresourcereviewdialog.cpp" line="146"/>
        <source>The change applies to every one of these %n resource(s).</source>
        <translation>
            <numerusform>Изменение применяется к этому %n ресурсу.</numerusform>
            <numerusform>Изменение применяется ко всем этим %n ресурсам.</numerusform>
            <numerusform>Изменение применяется ко всем этим %n ресурсам.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="155"/>
        <source>(nothing else, as far as this application knows)</source>
        <translation>(больше ничего, насколько известно этому приложению)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="159"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Director указывает как ссылающиеся на него: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="169"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Файл в Director: %1 (в нём есть и другие ресурсы; заменяется только этот ресурс).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="171"/>
        <source>File on the Director: %1.</source>
        <translation>Файл в Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="173"/>
        <source>The Director reloads its configuration after writing. Jobs that are running or already queued keep the configuration they started with.</source>
        <translation>После записи Director перезагружает свою конфигурацию. Выполняющиеся и уже поставленные в очередь Jobs сохраняют конфигурацию, с которой они были запущены.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="178"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Удалить в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="179"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Записать в Director</translation>
    </message>
</context>
<context>
    <name>BResourceText</name>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="213"/>
        <source>The text does not start with a resource (line %1).</source>
        <translation>Текст не начинается с ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="215"/>
        <source>Directives on the line of &quot;%1 {&quot; cannot be edited safely (line %2).</source>
        <translation>Директивы в строке «%1 {» нельзя безопасно редактировать (строка %2).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="225"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Текст после закрывающей скобки ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="236"/>
        <source>The closing brace of the resource shares its line with a nested block (line %1).</source>
        <translation>Закрывающая скобка ресурса находится в одной строке с вложенным блоком (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="250"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Закрывающая скобка находится в одной строке с директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="323"/>
        <source>The text contains no resource.</source>
        <translation>Текст не содержит ресурса.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="325"/>
        <source>The resource has no closing brace.</source>
        <translation>У ресурса нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="375"/>
        <source>A change without a directive name.</source>
        <translation>Изменение без имени директивы.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="377"/>
        <source>Directive %1 is changed twice.</source>
        <translation>Директива %1 изменена дважды.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="381"/>
        <source>The value of %1 contains a line break.</source>
        <translation>Значение %1 содержит перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="383"/>
        <source>An empty value for %1; remove the directive instead.</source>
        <translation>Пустое значение для %1; вместо этого удалите директиву.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="386"/>
        <source>Directive %1 is on a line that cannot be edited safely (several directives on one line, a list or a nested block). Change it in the text.</source>
        <translation>Директива %1 находится в строке, которую нельзя безопасно редактировать (несколько директив в одной строке, список или вложенный блок). Измените её в тексте.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="390"/>
        <source>Directive %1 appears more than once. Change it in the text.</source>
        <translation>Директива %1 встречается более одного раза. Измените её в тексте.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="443"/>
        <source>The new text could not be read back: %1</source>
        <translation>Новый текст не удалось прочитать обратно: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="448"/>
        <source>The new text still has directive %1.</source>
        <translation>В новом тексте всё ещё есть директива %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="450"/>
        <source>The new text does not have the intended value of %1.</source>
        <translation>В новом тексте нет нужного значения %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="454"/>
        <source>The new text would change more than the edited directives.</source>
        <translation>Новый текст изменил бы больше, чем отредактированные директивы.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="456"/>
        <source>The new text would change the type of the resource.</source>
        <translation>Новый текст изменил бы тип ресурса.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="458"/>
        <source>The new text would change the name of the resource.</source>
        <translation>Новый текст изменил бы имя ресурса.</translation>
    </message>
</context>
<context>
    <name>BResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="50"/>
        <source>Preview</source>
        <translation>Предпросмотр</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="52"/>
        <source>Toggle config file preview</source>
        <translation>Показать/скрыть предпросмотр файла конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="64"/>
        <source>Edit...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="70"/>
        <source>Only Job and JobDefs resources can be changed here; they are written to the Director. Change this resource in its configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="77"/>
        <source>Export .conf</source>
        <translation>Экспорт .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="83"/>
        <source>Export ZIP</source>
        <translation>Экспорт ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="96"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="96"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="147"/>
        <source>%1: %2 resource(s)</source>
        <translation>%1: ресурсов: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="206"/>
        <location filename="../src/director/bresourcewidget.cpp" line="230"/>
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 не изменён</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="207"/>
        <source>There is no Director connection to write the change to. Nothing was changed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="248"/>
        <source>Not written, because the form cannot write lists or nested blocks: %1. Change them in the configuration file.</source>
        <translation>Не записано, так как форма не может записывать списки и вложенные блоки: %1. Измените их в файле конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="253"/>
        <source>Nothing was changed.</source>
        <translation>Ничего не изменено.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="254"/>
        <source>Nothing that can be written was changed. %1</source>
        <translation>Ничего из того, что можно записать, не изменено. %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="271"/>
        <source>Another change is still being written. Try again when it is done.</source>
        <translation>Другое изменение ещё записывается. Повторите попытку, когда оно завершится.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="281"/>
        <source>The text the Director sent does not match its SHA-256.</source>
        <translation>Текст, присланный Director, не соответствует своему SHA-256.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="283"/>
        <source>%1 Nothing was written.</source>
        <translation>%1 Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="290"/>
        <source>The change cannot be written into the text of the resource safely: %1 Nothing was written.</source>
        <translation>Изменение нельзя безопасно внести в текст ресурса: %1 Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="509"/>
        <source>Schedule &quot;%1&quot; is not written here.</source>
        <translation>Schedule «%1» здесь не записывается.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="609"/>
        <source>Export %1 Configuration</source>
        <translation>Экспорт конфигурации %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="610"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="615"/>
        <location filename="../src/director/bresourcewidget.cpp" line="670"/>
        <source>Export Failed</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="616"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="661"/>
        <source>Export %1 Configuration as ZIP</source>
        <translation>Экспорт конфигурации %1 в ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="662"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="671"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
</context>
<context>
    <name>BRestoreBrowsePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="521"/>
        <source>Select Files</source>
        <translation>Выбор файлов</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="522"/>
        <source>Browse and select files/directories to restore.</source>
        <translation>Просмотрите и выберите файлы/каталоги для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="534"/>
        <source>Loading files...</source>
        <translation>Загрузка файлов...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="560"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="761"/>
        <source>No files selected</source>
        <translation>Файлы не выбраны</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="580"/>
        <source>Step %1 of %2 — Browse and select files/directories to restore.</source>
        <translation>Шаг %1 из %2 — Просмотрите и выберите файлы/каталоги для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="583"/>
        <source>Browse files — Client: %1, FileSet: %2</source>
        <translation>Просмотр файлов — Client: %1, FileSet: %2</translation>
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
            <numerusform>%n каталог</numerusform>
            <numerusform>%n каталога</numerusform>
            <numerusform>%n каталогов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="772"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="773"/>
        <source>Selected: %1</source>
        <translation>Выбрано: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="775"/>
        <source>%n item(s) selected</source>
        <translation>
            <numerusform>Выбран %n элемент</numerusform>
            <numerusform>Выбрано %n элемента</numerusform>
            <numerusform>Выбрано %n элементов</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BRestoreExecutePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1314"/>
        <source>Execute Restore</source>
        <translation>Выполнение восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1315"/>
        <source>Sending commands to the Director...</source>
        <translation>Отправка команд в Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1319"/>
        <source>Preparing...</source>
        <translation>Подготовка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1349"/>
        <source>Step %1 of %2 — Sending commands to the Director...</source>
        <translation>Шаг %1 из %2 — Отправка команд в Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1360"/>
        <source>Step %v of %m</source>
        <translation>Шаг %v из %m</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1387"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1406"/>
        <source>No director connection</source>
        <translation>Нет подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1394"/>
        <source>Creating restore table...</source>
        <translation>Создание таблицы восстановления...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1395"/>
        <source>&gt;&gt;&gt; .bvfs_restore %1</source>
        <translation>&gt;&gt;&gt; .bvfs_restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1413"/>
        <source>Executing restore command...</source>
        <translation>Выполнение команды restore...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1414"/>
        <source>&gt;&gt;&gt; restore %1</source>
        <translation>&gt;&gt;&gt; restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1428"/>
        <source>Cleaning up restore table...</source>
        <translation>Очистка таблицы восстановления...</translation>
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
        <translation>Ошибка .bvfs_restore: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1482"/>
        <source>Restore table created successfully.</source>
        <translation>Таблица восстановления успешно создана.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1498"/>
        <source>Restore command failed: %1</source>
        <translation>Ошибка команды restore: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1502"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1556"/>
        <source>Hint: The Bareos Director restricts the &quot;where&quot; path via WhereACL. Ask your administrator to add the path &quot;%1&quot; to the WhereACL of your console configuration, or use a permitted restore path.</source>
        <translation>Подсказка: Bareos Director ограничивает путь «where» с помощью WhereACL. Попросите администратора добавить путь «%1» в WhereACL конфигурации вашей консоли или используйте разрешённый путь восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1525"/>
        <source>Restore job submitted successfully (Job ID: %1).</source>
        <translation>Job восстановления успешно отправлен (Job ID: %1).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1527"/>
        <source>Restore command accepted.</source>
        <translation>Команда restore принята.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1551"/>
        <source>Restore command failed.</source>
        <translation>Ошибка команды restore.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1570"/>
        <source>Restore job submitted successfully.</source>
        <translation>Job восстановления успешно отправлен.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1580"/>
        <source>WARNING: Unexpected restore response.</source>
        <translation>ПРЕДУПРЕЖДЕНИЕ: неожиданный ответ на восстановление.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1598"/>
        <source>Restore table created (text response).</source>
        <translation>Таблица восстановления создана (текстовый ответ).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1613"/>
        <source>Cleanup complete.</source>
        <translation>Очистка завершена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1623"/>
        <source>Complete</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1626"/>
        <source>Restore job %1 submitted successfully!</source>
        <translation>Job восстановления %1 успешно отправлен!</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1627"/>
        <source>The restore job (ID: %1) is now running on the Director.
You can monitor its progress in the Jobs view.
Refresh the job list to see the restore job status.</source>
        <translation>Job восстановления (ID: %1) теперь выполняется в Director.
Следить за ходом можно в представлении Jobs.
Обновите список Jobs, чтобы увидеть статус восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1632"/>
        <source>Restore completed.</source>
        <translation>Восстановление завершено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1633"/>
        <source>The restore commands have been executed.
Check the log above for details.</source>
        <translation>Команды восстановления выполнены.
Подробности см. в журнале выше.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1645"/>
        <source>ERROR: %1</source>
        <translation>ОШИБКА: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1648"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1649"/>
        <source>Restore failed</source>
        <translation>Восстановление не удалось</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1677"/>
        <source>command</source>
        <translation>команды</translation>
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
        <translation>Параметры восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="789"/>
        <source>Configure where and how to restore the selected files.</source>
        <translation>Укажите, куда и как восстанавливать выбранные файлы.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="795"/>
        <source>Source:</source>
        <translation>Источник:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="798"/>
        <source>Target Client:</source>
        <translation>Целевой Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="803"/>
        <source>Restore Job:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="806"/>
        <source>Restore Where:</source>
        <translation>Восстановить в:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="809"/>
        <source>Always</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="810"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="811"/>
        <source>If Newer</source>
        <translation>Если новее</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="812"/>
        <source>If Older</source>
        <translation>Если старше</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="813"/>
        <source>Replace Policy:</source>
        <translation>Политика замены:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="838"/>
        <source>Step %1 of %2 — Configure where and how to restore the selected files.</source>
        <translation>Шаг %1 из %2 — Укажите, куда и как восстанавливать выбранные файлы.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="843"/>
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n каталог</numerusform>
            <numerusform>%n каталога</numerusform>
            <numerusform>%n каталогов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="845"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="847"/>
        <source>nothing</source>
        <translation>ничего</translation>
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
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="858"/>
        <source>Job %1: %2 — %3 selected (estimated size: %4)</source>
        <translation>Job %1: %2 — выбрано: %3 (оценочный размер: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="933"/>
        <source>Chosen by the Director</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="947"/>
        <source>Always overwrite existing files on the target, regardless of timestamps.</source>
        <translation>Всегда перезаписывать существующие файлы в месте назначения, независимо от отметок времени.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="951"/>
        <source>Never overwrite. Skip files that already exist on the target.</source>
        <translation>Никогда не перезаписывать. Пропускать файлы, которые уже существуют в месте назначения.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="955"/>
        <source>Only overwrite if the backed-up file is newer than the existing file.</source>
        <translation>Перезаписывать, только если файл из резервной копии новее существующего.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="959"/>
        <source>Only overwrite if the backed-up file is older than the existing file.</source>
        <translation>Перезаписывать, только если файл из резервной копии старше существующего.</translation>
    </message>
</context>
<context>
    <name>BRestorePreviewPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="998"/>
        <source>Command Preview</source>
        <translation>Предпросмотр команд</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="999"/>
        <source>Review the commands that will be sent to the Director.</source>
        <translation>Проверьте команды, которые будут отправлены в Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1009"/>
        <source>Permission Checks</source>
        <translation>Проверка прав</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1017"/>
        <source>Restore command:</source>
        <translation>Команда restore:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1018"/>
        <source>Target client:</source>
        <translation>Целевой Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1019"/>
        <source>Where path:</source>
        <translation>Путь where:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1041"/>
        <source>Click &quot;Commit&quot; to execute these commands on the Director.</source>
        <translation>Нажмите «Commit», чтобы выполнить эти команды в Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1054"/>
        <source>Step %1 of %2 — Review the commands that will be sent to the Director.</source>
        <translation>Шаг %1 из %2 — Проверьте команды, которые будут отправлены в Director.</translation>
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
            <numerusform>%n каталог (со всем содержимым)</numerusform>
            <numerusform>%n каталога (со всем содержимым)</numerusform>
            <numerusform>%n каталогов (со всем содержимым)</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="1101"/>
        <source>%n individual file(s)</source>
        <translation>
            <numerusform>%n отдельный файл</numerusform>
            <numerusform>%n отдельных файла</numerusform>
            <numerusform>%n отдельных файлов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="1105"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1107"/>
        <source>No files or directories selected</source>
        <translation>Файлы и каталоги не выбраны</translation>
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
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1118"/>
        <source>Restore summary:
• Selection: %1
• Estimated size: %2
• Target client: %3
• Restore to: %4
• Replace policy: %5</source>
        <translation>Сводка восстановления:
• Выбор: %1
• Оценочный размер: %2
• Целевой Client: %3
• Восстановить в: %4
• Политика замены: %5</translation>
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
        <translation>Проверка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1155"/>
        <source>Querying Director for permissions...</source>
        <translation>Запрос прав у Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1167"/>
        <source>No Director connection</source>
        <translation>Нет подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1226"/>
        <source>restore command lacks client= parameter</source>
        <translation>в команде restore отсутствует параметр client=</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1231"/>
        <source>Available (subject to WhereACL)</source>
        <translation>Доступно (с учётом WhereACL)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1235"/>
        <source>Not available</source>
        <translation>Недоступно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1239"/>
        <source>N/A (restore not authorized)</source>
        <translation>Н/Д (восстановление не разрешено)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1244"/>
        <source>Permission check completed.</source>
        <translation>Проверка прав завершена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1261"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1263"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1265"/>
        <source>Unknown (timeout)</source>
        <translation>Неизвестно (тайм-аут)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1267"/>
        <source>Permission check timed out — you can still proceed.</source>
        <translation>Истекло время проверки прав — вы всё равно можете продолжить.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1295"/>
        <source>Authorized</source>
        <translation>Разрешено</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1299"/>
        <source>Not authorized</source>
        <translation>Не разрешено</translation>
    </message>
</context>
<context>
    <name>BRestoreSelectJobPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="170"/>
        <source>Restore Conditions</source>
        <translation>Условия восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="171"/>
        <source>Configure the restore scope before browsing files.</source>
        <translation>Задайте область восстановления перед просмотром файлов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="178"/>
        <source>Source Job:</source>
        <translation>Исходный Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="182"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="186"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="190"/>
        <source>All related backups (full restore chain)</source>
        <translation>Все связанные резервные копии (полная цепочка восстановления)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="199"/>
        <source>Restore Scope:</source>
        <translation>Область восстановления:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="205"/>
        <source>Before Date:</source>
        <translation>До даты:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="242"/>
        <source>Step %1 of %2 — Configure the restore scope before browsing files.</source>
        <translation>Шаг %1 из %2 — Задайте область восстановления перед просмотром файлов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="245"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="453"/>
        <source>Job %1: %2 (Client: %3, FileSet: %4)</source>
        <translation>Job %1: %2 (Client: %3, FileSet: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="251"/>
        <source>Only this specific job (ID: %1)</source>
        <translation>Только этот Job (ID: %1)</translation>
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
        <translation>Предупреждение: не удалось загрузить %1 из Director за отведённое время. Возможно, Director занят или подключение медленное.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="306"/>
        <source> and </source>
        <translation> и </translation>
    </message>
</context>
<context>
    <name>BRestoreWizard</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="131"/>
        <source>Restore Wizard — Job %1: %2</source>
        <translation>Мастер восстановления — Job %1: %2</translation>
    </message>
</context>
<context>
    <name>BRunOverridesEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="42"/>
        <source>not set</source>
        <translation>не задано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="43"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="44"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="86"/>
        <source>(job&apos;s setting)</source>
        <translation>(настройка Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="90"/>
        <source>The Director has not reported any %1 yet; refresh the view after connecting. Not set: the setting of the job applies.</source>
        <translation>Director ещё не сообщил ни одного %1; обновите представление после подключения. Не задано: действует настройка Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="92"/>
        <source>The %1 the Director reported. Not set: the setting of the job applies.</source>
        <translation>%1, сообщённые Director. Не задано: действует настройка Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="112"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="113"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="114"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="115"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="116"/>
        <source>pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="117"/>
        <source>storages</source>
        <translation>Storages</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="120"/>
        <source>set to</source>
        <translation>задано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="126"/>
        <source>Level of the job</source>
        <translation>Level Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="135"/>
        <source>The level the Run line names. &quot;Level of the job&quot;: the line names none and the level of the job applies.</source>
        <translation>Level, указанный в строке Run. «Level Job»: строка не указывает его, и действует Level Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="141"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="142"/>
        <source>Override the priority</source>
        <translation>Переопределить приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="143"/>
        <source>a time period, e.g. 8 hours</source>
        <translation>период времени, например 8 hours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="144"/>
        <source>Empty: the setting of the job applies</source>
        <translation>Пусто: действует настройка Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="170"/>
        <source>&amp;Level:</source>
        <translation>&amp;Level:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="171"/>
        <source>&amp;Storage:</source>
        <translation>&amp;Storage:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="173"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="174"/>
        <source>&amp;Messages:</source>
        <translation>&amp;Messages:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="176"/>
        <source>&amp;Full pool:</source>
        <translation>&amp;Full pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="177"/>
        <source>P&amp;riority:</source>
        <translation>П&amp;риоритет:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="179"/>
        <source>&amp;Incremental pool:</source>
        <translation>&amp;Incremental pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="180"/>
        <source>Sp&amp;ool data:</source>
        <translation>Sp&amp;ool data:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="182"/>
        <source>&amp;Differential pool:</source>
        <translation>&amp;Differential pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="183"/>
        <source>&amp;Accurate:</source>
        <translation>&amp;Accurate:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="185"/>
        <source>&amp;Next pool:</source>
        <translation>&amp;Next pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="186"/>
        <source>Max run sched &amp;time:</source>
        <translation>Max run sched &amp;time:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="274"/>
        <source>%1 (not reported)</source>
        <translation>%1 (не сообщено)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="276"/>
        <source>%1 is named by the Run line, but the Director did not report it. It is kept as it is.</source>
        <translation>%1 указан в строке Run, но Director о нём не сообщил. Он оставлен без изменений.</translation>
    </message>
</context>
<context>
    <name>BRunParser</name>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="120"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="152"/>
        <source>A Run line cannot contain a line break.</source>
        <translation>Строка Run не может содержать перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="141"/>
        <source>Unexpected character &quot;%1&quot;.</source>
        <translation>Неожиданный символ «%1».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="170"/>
        <source>A quoted value is not closed.</source>
        <translation>Значение в кавычках не закрыто.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="400"/>
        <source>The Run line is empty.</source>
        <translation>Строка Run пуста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="415"/>
        <source>Expected an equals, got: %1</source>
        <translation>Ожидался знак равенства, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="419"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="464"/>
        <source>Expected a value after &quot;%1=&quot;.</source>
        <translation>Ожидалось значение после «%1=».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="430"/>
        <source>Expect a YES or NO, got: %1</source>
        <translation>Ожидалось YES или NO, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="440"/>
        <source>Job level field: %1 not found in run record</source>
        <translation>Поле уровня Job: %1 не найдено в записи run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="447"/>
        <source>expected a positive integer, got: %1</source>
        <translation>ожидалось положительное целое число, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="453"/>
        <source>expected a time period, got: %1</source>
        <translation>ожидался период времени, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="504"/>
        <source>Unexpected token: %1</source>
        <translation>Неожиданный токен: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="509"/>
        <source>Day number out of range (1-31)</source>
        <translation>Номер дня вне диапазона (1-31)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="516"/>
        <source>Unexpected token: &quot;%1&quot;</source>
        <translation>Неожиданный токен: «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="526"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="660"/>
        <source>Week number out of range (0-53)</source>
        <translation>Номер недели вне диапазона (0-53)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="532"/>
        <source>Job type field: %1 in run record not found</source>
        <translation>Поле типа Job: %1 в записи run не найдено</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="562"/>
        <source>Time must be preceded by keyword AT.</source>
        <translation>Перед временем должно стоять ключевое слово AT.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="578"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="583"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="592"/>
        <source>Bad time specification.</source>
        <translation>Неверное указание времени.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="617"/>
        <source>Bad day specification in modulo.</source>
        <translation>Неверное указание дня в modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="619"/>
        <source>Bad day specification, offset must always be &lt;= than modulo.</source>
        <translation>Неверное указание дня: смещение всегда должно быть &lt;= modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="629"/>
        <source>Week number out of range (0-53) in modulo</source>
        <translation>Номер недели вне диапазона (0-53) в modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="631"/>
        <source>Week interval out of range (1-53) in modulo</source>
        <translation>Интервал недель вне диапазона (1-53) в modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="633"/>
        <source>Bad week number specification in modulo, offset must always be &lt;= than modulo.</source>
        <translation>Неверное указание номера недели в modulo: смещение всегда должно быть &lt;= modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="640"/>
        <source>Bad modulo time specification. Format for weekdays is &apos;01/02&apos;, for yearweeks is &apos;w01/w02&apos;.</source>
        <translation>Неверное указание modulo. Формат для дней недели — &apos;01/02&apos;, для недель года — &apos;w01/w02&apos;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="653"/>
        <source>Bad day range specification.</source>
        <translation>Неверное указание диапазона дней.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="668"/>
        <source>Invalid month, week or position day range</source>
        <translation>Недопустимый диапазон месяцев, недель или позиций дня</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="672"/>
        <source>Invalid month, weekday or position range</source>
        <translation>Недопустимый диапазон месяцев, дней недели или позиций</translation>
    </message>
</context>
<context>
    <name>BRunRuleDialog</name>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="97"/>
        <source>Only on these da&amp;ys of the month</source>
        <translation>Только в эти д&amp;ни месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="98"/>
        <source>Only in these &amp;weeks of the year</source>
        <translation>Только в эти &amp;недели года</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="99"/>
        <source>last</source>
        <translation>последняя</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="100"/>
        <source>&amp;Every hour</source>
        <translation>&amp;Каждый час</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="107"/>
        <source>Calendar rule</source>
        <translation>Календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="110"/>
        <source>A calendar rule runs by day of the month or by week of the year. It does not belong to a weekday and is not painted in the grid.</source>
        <translation>Календарное правило запускается по дню месяца или по неделе года. Оно не относится к дню недели и не отображается в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="117"/>
        <source>What runs</source>
        <translation>Что запускается</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="125"/>
        <source>Not checked: every day of the month</source>
        <translation>Не отмечено: каждый день месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="130"/>
        <source>Day %1</source>
        <translation>День %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="135"/>
        <source>Not checked: every week of the year. The weeks are numbered w00 to w53 as in a Run line.</source>
        <translation>Не отмечено: каждая неделя года. Недели нумеруются от w00 до w53, как в строке Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="141"/>
        <source>Week %1</source>
        <translation>Неделя %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="147"/>
        <source>And only</source>
        <translation>И только</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="149"/>
        <source>on these weekdays:</source>
        <translation>в эти дни недели:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="165"/>
        <source>in these weeks of the month:</source>
        <translation>в эти недели месяца:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="180"/>
        <source>The last seven days of a month</source>
        <translation>Последние семь дней месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="186"/>
        <source>in these months:</source>
        <translation>в эти месяцы:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="203"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="208"/>
        <source>%1 hours</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="211"/>
        <source>The run starts in every hour of the day (&quot;hourly&quot;)</source>
        <translation>Запуск в каждый час суток («hourly»)</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="213"/>
        <source>&amp;Minute of the hour:</source>
        <translation>&amp;Минута часа:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="216"/>
        <source>Minute of the hour</source>
        <translation>Минута часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="229"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="294"/>
        <source>Odd</source>
        <translation>Нечётные</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="295"/>
        <source>Even</source>
        <translation>Чётные</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="296"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="390"/>
        <source>Choose days of the month or weeks of the year. A run without them belongs to the grid and is painted there.</source>
        <translation>Выберите дни месяца или недели года. Запуск без них относится к сетке и отображается в ней.</translation>
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
        <translation>Изменить RunScript</translation>
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
        <translation>Выбрать скрипт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="70"/>
        <source>Command:</source>
        <translation>Команда:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="75"/>
        <source>Runs When:</source>
        <translation>Runs When:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="80"/>
        <source>Runs On Client</source>
        <translation>Runs On Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="84"/>
        <source>Runs On Failure</source>
        <translation>Runs On Failure</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="87"/>
        <source>Abort Job On Error</source>
        <translation>Abort Job On Error</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="91"/>
        <source>Fail Job On Error</source>
        <translation>Fail Job On Error</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="102"/>
        <source>Validation</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="102"/>
        <source>Command is required.</source>
        <translation>Требуется команда.</translation>
    </message>
</context>
<context>
    <name>BRunScriptEditor</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>Command</source>
        <translation>Команда</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>Runs When</source>
        <translation>Runs When</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="145"/>
        <source>On Client</source>
        <translation>На Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="159"/>
        <source>+ Add Script</source>
        <translation>+ Добавить скрипт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="160"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="161"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="236"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="236"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BRunWriter</name>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="181"/>
        <source>The run has an empty hour, day, month or week set and cannot be written as a Run line.</source>
        <translation>В запуске задан пустой набор часов, дней, месяцев или недель, поэтому его нельзя записать как строку Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="186"/>
        <source>The minute %1 is out of range.</source>
        <translation>Минута %1 вне диапазона.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="250"/>
        <source>Run line &quot;%1&quot; is not understood (%2) and is never rewritten.</source>
        <translation>Строка Run «%1» не распознана (%2) и никогда не перезаписывается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="269"/>
        <source>The generated Run line &quot;%1&quot; does not read back: %2</source>
        <translation>Сгенерированная строка Run «%1» не читается обратно: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="274"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>Сгенерированная строка Run «%1» означает не то, что предполагалось.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="290"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
</context>
<context>
    <name>BScheduleBrushDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="296"/>
        <source>New brush</source>
        <translation>Новая кисть</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="299"/>
        <source>A brush is what painting puts into a cell: a level, the minute of the hour and, if wanted, settings that replace those of the job.</source>
        <translation>Кисть — это то, что рисование помещает в ячейку: Level, минута часа и, при необходимости, настройки, заменяющие настройки Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="308"/>
        <source>Minute of the hour</source>
        <translation>Минута часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="309"/>
        <source>The run starts at this minute of every painted hour</source>
        <translation>Запуск начинается в эту минуту каждого закрашенного часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="310"/>
        <source>Minute of the &amp;hour:</source>
        <translation>Минута &amp;часа:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="313"/>
        <source>Level and overrides</source>
        <translation>Level и переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="369"/>
        <source>Brush: %1</source>
        <translation>Кисть: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleEntry</name>
    <message>
        <location filename="../src/models/bresourcemodels.cpp" line="682"/>
        <source>The entry does not describe a run.</source>
        <translation>Запись не описывает запуск.</translation>
    </message>
</context>
<context>
    <name>BScheduleGanttWidget</name>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="414"/>
        <source>(no client)</source>
        <translation>(нет Client)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="417"/>
        <source>(no storage)</source>
        <translation>(нет Storage)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="444"/>
        <source>Not used by any job</source>
        <translation>Не используется ни одним Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="560"/>
        <source>No runs in this week</source>
        <translation>На этой неделе запусков нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="561"/>
        <source>No runs on this date</source>
        <translation>В эту дату запусков нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Monday</source>
        <translation>Понедельник</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Tuesday</source>
        <translation>Вторник</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Wednesday</source>
        <translation>Среда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="597"/>
        <source>Thursday</source>
        <translation>Четверг</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Friday</source>
        <translation>Пятница</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Saturday</source>
        <translation>Суббота</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="598"/>
        <source>Sunday</source>
        <translation>Воскресенье</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="601"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="602"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1091"/>
        <source>%1, if it turns out against it</source>
        <translation>%1, если выйдет иначе</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1161"/>
        <source>Utilization</source>
        <translation>Загрузка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1412"/>
        <source>Not used by any enabled job: nothing starts at this time.</source>
        <translation>Не используется ни одним включённым Job: в это время ничего не запускается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1413"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1430"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1416"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1428"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2176"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1418"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1437"/>
        <source>Run</source>
        <translation>Запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1429"/>
        <source>(level of the job)</source>
        <translation>(Level Job)</translation>
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
        <translation>Прогноз ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2176"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1453"/>
        <source>Predicted wait: none</source>
        <translation>Прогноз ожидания: нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1464"/>
        <source>the jobs of the Director are not loaded: it is not known which job runs this schedule</source>
        <translation>Jobs Director не загружены: неизвестно, какой Job использует этот Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1469"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>нет успешных запусков этого Job за последние %1 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1473"/>
        <source>No history</source>
        <translation>Нет истории</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1474"/>
        <source>Duration unknown: the outline marks the start only.</source>
        <translation>Длительность неизвестна: контур отмечает только начало.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1477"/>
        <source>Median duration</source>
        <translation>Медианная длительность</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1479"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1486"/>
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1481"/>
        <source>runs</source>
        <translation>запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1484"/>
        <source>Median wait</source>
        <translation>Медианное ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1489"/>
        <source>Wait</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1489"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1821"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1493"/>
        <source>Estimate: no run at this level in the last %1 days, figures from all levels of the job.</source>
        <translation>Оценка: нет запусков с этим Level за последние %1 дн., данные по всем Level этого Job.</translation>
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
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1513"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1517"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1544"/>
        <source>job(s) running</source>
        <translation>выполняющихся Jobs</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1548"/>
        <source>%n run(s) predicted to wait</source>
        <translation>
            <numerusform>Ожидается, что %n запуск будет ждать</numerusform>
            <numerusform>Ожидается, что %n запуска будут ждать</numerusform>
            <numerusform>Ожидается, что %n запусков будут ждать</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1553"/>
        <source>%1 run(s) without history start here (not counted: duration unknown)</source>
        <translation>Запусков без истории, начинающихся здесь: %1 (не учтены: длительность неизвестна)</translation>
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
        <translation>Выполняется сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1809"/>
        <source>Waiting now</source>
        <translation>Ожидает сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1811"/>
        <source>Queued at</source>
        <translation>В очереди с</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1814"/>
        <source>Started at</source>
        <translation>Запущен в</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1816"/>
        <source>Running for</source>
        <translation>Выполняется уже</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1820"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2172"/>
        <source>Waited</source>
        <translation>Ожидал</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1820"/>
        <source>Waiting for</source>
        <translation>Ожидает уже</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1823"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2183"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1826"/>
        <source>Known from the job history; the Director reports when it ends.</source>
        <translation>Известно из истории Jobs; Director сообщит о завершении.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1827"/>
        <source>Reported by the Director as it happens.</source>
        <translation>Сообщается Director в момент события.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2146"/>
        <source>not recorded</source>
        <translation>не записано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2151"/>
        <source>terminated normally</source>
        <translation>завершено нормально</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2152"/>
        <source>terminated with warnings</source>
        <translation>завершено с предупреждениями</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2153"/>
        <source>terminated in error</source>
        <translation>завершено с ошибкой</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2154"/>
        <source>non-fatal error</source>
        <translation>нефатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2155"/>
        <source>fatal error</source>
        <translation>фатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2156"/>
        <source>canceled</source>
        <translation>отменено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2157"/>
        <source>running</source>
        <translation>выполняется</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2158"/>
        <source>created, not yet running</source>
        <translation>создано, ещё не выполняется</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2159"/>
        <source>status %1</source>
        <translation>статус %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2162"/>
        <source>Measured run of %1</source>
        <translation>Измеренный запуск %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2165"/>
        <source>Scheduled</source>
        <translation>Запланировано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2166"/>
        <source>Started</source>
        <translation>Запущено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2169"/>
        <source>still running</source>
        <translation>ещё выполняется</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2170"/>
        <source>Ended</source>
        <translation>Завершено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2174"/>
        <source>Ran</source>
        <translation>Длился</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="2181"/>
        <source>planned: %1</source>
        <translation>запланировано: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="32"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="79"/>
        <source>Undone: %1</source>
        <translation>Отменено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="43"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="88"/>
        <source>Redone: %1</source>
        <translation>Повторено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="206"/>
        <source>Eraser</source>
        <translation>Ластик</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="259"/>
        <source>This schedule contains a Run line that is not understood. It is locked: nothing in it can be changed here.</source>
        <translation>Этот Schedule содержит нераспознанную строку Run. Он заблокирован: здесь ничего в нём изменить нельзя.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="286"/>
        <source>%n cell(s)</source>
        <translation>
            <numerusform>%n ячейка</numerusform>
            <numerusform>%n ячейки</numerusform>
            <numerusform>%n ячеек</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="288"/>
        <source>%1 of the scope %2</source>
        <translation>%1 области %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="300"/>
        <source>Paint %1 on %2</source>
        <translation>Нарисовать %1 на %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="309"/>
        <source>Add %1 to %2</source>
        <translation>Добавить %1 к %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="317"/>
        <source>Erase %1</source>
        <translation>Стереть %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="325"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="351"/>
        <source>No cell is selected.</source>
        <translation>Ни одна ячейка не выбрана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="333"/>
        <source>No brush is chosen.</source>
        <translation>Кисть не выбрана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="377"/>
        <source>A scope needs at least one month, and a week of the month or &quot;last&quot;.</source>
        <translation>Области нужен хотя бы один месяц и неделя месяца или «последняя».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="387"/>
        <source>Add the scope %1</source>
        <translation>Добавить область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="402"/>
        <source>The scope &quot;%1&quot; is always there.</source>
        <translation>Область «%1» есть всегда.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="407"/>
        <source>The scope %1 still has Run lines. Erase its cells first.</source>
        <translation>В области %1 ещё есть строки Run. Сначала сотрите её ячейки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="413"/>
        <source>Remove the scope %1</source>
        <translation>Удалить область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="423"/>
        <source>Add a calendar rule</source>
        <translation>Добавить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="434"/>
        <source>Change the calendar rule %1</source>
        <translation>Изменить календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="446"/>
        <source>Remove the calendar rule %1</source>
        <translation>Удалить календарное правило %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="718"/>
        <source>Run line %1 (&quot;%2&quot;) is not understood: %3</source>
        <translation>Строка Run %1 («%2») не распознана: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="727"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1180"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1229"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1254"/>
        <source>There is no Run line %1.</source>
        <translation>Строки Run %1 нет.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1182"/>
        <source>Run line %1 is not a line of the grid and is not changed here.</source>
        <translation>Строка Run %1 не является строкой сетки и здесь не изменяется.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1210"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1233"/>
        <source>A run without days of the month and without weeks of the year is not a calendar rule; it is painted in the grid.</source>
        <translation>Запуск без дней месяца и без недель года не является календарным правилом; он отображается в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1231"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1256"/>
        <source>Run line %1 is not a calendar rule.</source>
        <translation>Строка Run %1 не является календарным правилом.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1291"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>Сгенерированная строка Run «%1» означает не то, что предполагалось.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="136"/>
        <source>Runs</source>
        <translation>Запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="137"/>
        <source>Paint the runs on the grid: choose a level in the palette and drag over the hours. Calendar rules (days of the month, weeks of the year) are added below the grid.</source>
        <translation>Нарисуйте запуски на сетке: выберите Level в палитре и проведите по часам. Календарные правила (дни месяца, недели года) добавляются под сеткой.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridText</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="77"/>
        <source>Monday</source>
        <translation>Понедельник</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="78"/>
        <source>Tuesday</source>
        <translation>Вторник</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="79"/>
        <source>Wednesday</source>
        <translation>Среда</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="80"/>
        <source>Thursday</source>
        <translation>Четверг</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="81"/>
        <source>Friday</source>
        <translation>Пятница</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="82"/>
        <source>Saturday</source>
        <translation>Суббота</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="83"/>
        <source>Sunday</source>
        <translation>Воскресенье</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="91"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="92"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="93"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="94"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="95"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="96"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="97"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="105"/>
        <source>January</source>
        <translation>Январь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="106"/>
        <source>February</source>
        <translation>Февраль</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="107"/>
        <source>March</source>
        <translation>Март</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="108"/>
        <source>April</source>
        <translation>Апрель</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="109"/>
        <source>May</source>
        <translation>Май</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="110"/>
        <source>June</source>
        <translation>Июнь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="111"/>
        <source>July</source>
        <translation>Июль</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="112"/>
        <source>August</source>
        <translation>Август</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="113"/>
        <source>September</source>
        <translation>Сентябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="114"/>
        <source>October</source>
        <translation>Октябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="115"/>
        <source>November</source>
        <translation>Ноябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="116"/>
        <source>December</source>
        <translation>Декабрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="125"/>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="131"/>
        <source>%1th</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="127"/>
        <source>%1st</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="128"/>
        <source>%1nd</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="129"/>
        <source>%1rd</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="145"/>
        <source>Every week</source>
        <translation>Каждая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="160"/>
        <source>all</source>
        <translation>все</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="190"/>
        <source>last week only</source>
        <translation>только последняя неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="193"/>
        <source>first week only</source>
        <translation>только первая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="194"/>
        <source>second week only</source>
        <translation>только вторая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="195"/>
        <source>third week only</source>
        <translation>только третья неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="196"/>
        <source>fourth week only</source>
        <translation>только четвёртая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="197"/>
        <source>fifth week only</source>
        <translation>только пятая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="202"/>
        <source>only %1</source>
        <translation>только %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="210"/>
        <source>level of the job</source>
        <translation>Level Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="253"/>
        <source>pool %1</source>
        <translation>pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="254"/>
        <source>full pool %1</source>
        <translation>full pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="255"/>
        <source>incremental pool %1</source>
        <translation>incremental pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="256"/>
        <source>differential pool %1</source>
        <translation>differential pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="257"/>
        <source>next pool %1</source>
        <translation>next pool %1</translation>
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
        <translation>приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="261"/>
        <source>spool data yes</source>
        <translation>spool data yes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="261"/>
        <source>spool data no</source>
        <translation>spool data no</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="262"/>
        <source>accurate yes</source>
        <translation>accurate yes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="262"/>
        <source>accurate no</source>
        <translation>accurate no</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="263"/>
        <source>max run sched time %1</source>
        <translation>max run sched time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="269"/>
        <source>%1 at :%2</source>
        <translation>%1 в :%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="279"/>
        <source>%1 at %2</source>
        <translation>%1 в %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="311"/>
        <source>%1 %2 hours</source>
        <translation>%1, %2 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="319"/>
        <source>%1: no run</source>
        <translation>%1: нет запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="328"/>
        <source>the Run line names no time</source>
        <translation>в строке Run не указано время</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="341"/>
        <source>no run in this scope</source>
        <translation>в этой области нет запусков</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="345"/>
        <source>in other scopes: %1</source>
        <translation>в других областях: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="372"/>
        <source>and</source>
        <translation>и</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="373"/>
        <source>or</source>
        <translation>или</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="381"/>
        <source>%1 to %2</source>
        <translation>%1–%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="403"/>
        <source>on every %1 day (starting with the %2)</source>
        <translation>каждый %1-й день (начиная с %2-го числа)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="405"/>
        <source>on the %1</source>
        <translation>%1 числа</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="408"/>
        <source>%1 of every month</source>
        <translation>%1 каждого месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="409"/>
        <source>%1 of %2</source>
        <translation>%1 (месяцы: %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="411"/>
        <source>in %1</source>
        <translation>в месяцы: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="420"/>
        <source>in every %1 week of the year (starting with week %2)</source>
        <translation>каждую %1-ю неделю года (начиная с недели %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="428"/>
        <source>in week %1 of the year</source>
        <translation>на неделе %1 года</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="429"/>
        <source>in weeks %1 of the year</source>
        <translation>на неделях %1 года</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="436"/>
        <source>in the last seven days of the month</source>
        <translation>в последние семь дней месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="438"/>
        <source>in the %1 week of the month</source>
        <translation>на %1-й неделе месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="441"/>
        <source>or in its last seven days</source>
        <translation>или в его последние семь дней</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="455"/>
        <source>only if it is a %1</source>
        <translation>только если это %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="457"/>
        <source>on %1</source>
        <translation>в дни: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="465"/>
        <source>every hour at :00 (the line names no time)</source>
        <translation>каждый час в :00 (в строке не указано время)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="467"/>
        <source>every hour at :%1</source>
        <translation>каждый час в :%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="475"/>
        <source>at %1</source>
        <translation>в %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridView</name>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="258"/>
        <source>Undo: %1 (Ctrl+Z)</source>
        <translation>Отменить: %1 (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="259"/>
        <source>Nothing to undo (Ctrl+Z)</source>
        <translation>Нечего отменять (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="260"/>
        <source>Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Повторить: %1 (Ctrl+Shift+Z или Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="261"/>
        <source>Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Нечего повторять (Ctrl+Shift+Z или Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="296"/>
        <source>No schedule chosen. Select a schedule in the list on the left to see it as a grid of weekdays and hours.</source>
        <translation>Schedule не выбран. Выберите Schedule в списке слева, чтобы увидеть его в виде сетки дней недели и часов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="337"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="338"/>
        <source>Undo</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="340"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="341"/>
        <source>Redo</source>
        <translation>Повторить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="343"/>
        <source>Revert all</source>
        <translation>Отменить все</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="344"/>
        <source>Undoes every change of this schedule. Redo brings them back.</source>
        <translation>Отменяет все изменения этого Schedule. «Повторить» возвращает их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="346"/>
        <source>&amp;Save...</source>
        <translation>&amp;Сохранить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="348"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="843"/>
        <source>Writes the changed Run lines to the Director. The Director checks them first, and you see old and new lines and the jobs that use the schedule before anything is written.</source>
        <translation>Записывает изменённые строки Run в Director. Сначала Director проверяет их, и до записи вы видите старые и новые строки, а также Jobs, использующие этот Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="353"/>
        <source>Delete...</source>
        <translation>Удалить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="354"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="847"/>
        <source>Deletes this schedule on the Director, after a check that no job uses it and your confirmation</source>
        <translation>Удаляет этот Schedule в Director после проверки, что ни один Job его не использует, и вашего подтверждения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="363"/>
        <source>Cop&amp;y...</source>
        <translation>&amp;Копировать...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="367"/>
        <source>Re&amp;name...</source>
        <translation>Переи&amp;меновать...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="381"/>
        <source>No description</source>
        <translation>Нет описания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="382"/>
        <source>The Description of the schedule. Saved with the Run lines; empty removes it from the file.</source>
        <translation>Description этого Schedule. Сохраняется вместе со строками Run; пустое значение удаляет её из файла.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="420"/>
        <source>&amp;Keep my changes</source>
        <translation>&amp;Сохранить мои изменения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="421"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="927"/>
        <source>&amp;Discard and reload</source>
        <translation>&amp;Отменить и перезагрузить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="431"/>
        <source>Scopes</source>
        <translation>Области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="435"/>
        <source>Scope:</source>
        <translation>Область:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="436"/>
        <source>The weeks of the month and the months a Run line is restricted to. The grid shows the runs of the chosen scope; runs of the other scopes are hatched.</source>
        <translation>Недели месяца и месяцы, которыми ограничена строка Run. Сетка показывает запуски выбранной области; запуски других областей заштрихованы.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="443"/>
        <source>Add scope</source>
        <translation>Добавить область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="444"/>
        <source>Add scope: weeks of the month, and months, to paint a grid of their own for</source>
        <translation>Добавить область: недели месяца и месяцы, для которых рисуется отдельная сетка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="469"/>
        <source>Legend</source>
        <translation>Легенда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="490"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="494"/>
        <source>Run lines</source>
        <translation>Строки Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <source>No.</source>
        <translation>№</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <source>Shown in</source>
        <translation>Показано в</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="500"/>
        <source>The Run lines of the schedule in their order. Choosing a line outlines its cells in the grid and switches to its scope.</source>
        <translation>Строки Run этого Schedule по порядку. Выбор строки обводит её ячейки в сетке и переключает на её область.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="516"/>
        <source>&amp;Add rule...</source>
        <translation>&amp;Добавить правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="517"/>
        <source>Add calendar rule</source>
        <translation>Добавить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="518"/>
        <source>Adds a calendar rule: a Run line by day of the month or by week of the year, such as &quot;on 1 at 21:00&quot;</source>
        <translation>Добавляет календарное правило: строку Run по дню месяца или по неделе года, например «on 1 at 21:00»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="520"/>
        <source>&amp;Edit rule...</source>
        <translation>&amp;Изменить правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="521"/>
        <source>Edit calendar rule</source>
        <translation>Изменить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="522"/>
        <source>Opens the form of the calendar rule that is chosen in the list</source>
        <translation>Открывает форму календарного правила, выбранного в списке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523"/>
        <source>&amp;Remove rule</source>
        <translation>&amp;Удалить правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="524"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1426"/>
        <source>Remove calendar rule</source>
        <translation>Удалить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="525"/>
        <source>Removes the calendar rule that is chosen in the list</source>
        <translation>Удаляет календарное правило, выбранное в списке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="535"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="539"/>
        <source>Calendar rules</source>
        <translation>Календарные правила</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Mark</source>
        <translation>Метка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="544"/>
        <source>Runs</source>
        <translation>Запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="545"/>
        <source>Run lines by day of the month or week of the year. They do not belong to a weekday and are marked in the band below the grid. Enter or a double click opens the form of a rule, Del removes it.</source>
        <translation>Строки Run по дню месяца или неделе года. Они не относятся к дню недели и отмечаются в полосе под сеткой. Enter или двойной щелчок открывает форму правила, Del удаляет его.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="563"/>
        <source>Next five weeks</source>
        <translation>Следующие пять недель</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="569"/>
        <source>The levels that run on each date</source>
        <translation>Level, выполняемые в каждую дату</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="574"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="575"/>
        <source>Previous month</source>
        <translation>Предыдущий месяц</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="578"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="579"/>
        <source>Next month</source>
        <translation>Следующий месяц</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="581"/>
        <source>Today</source>
        <translation>Сегодня</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="582"/>
        <source>Back to the current week</source>
        <translation>Вернуться к текущей неделе</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="744"/>
        <source>Modified: the description, not written to the Director.</source>
        <translation>Изменено: описание, не записано в Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="759"/>
        <source>%n changed</source>
        <translation>
            <numerusform>%n изменена</numerusform>
            <numerusform>%n изменены</numerusform>
            <numerusform>%n изменено</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="761"/>
        <source>%n new</source>
        <translation>
            <numerusform>%n новая</numerusform>
            <numerusform>%n новые</numerusform>
            <numerusform>%n новых</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="763"/>
        <source>%n removed</source>
        <translation>
            <numerusform>%n удалена</numerusform>
            <numerusform>%n удалены</numerusform>
            <numerusform>%n удалено</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="765"/>
        <source>Modified: %1 Run line(s) and the description, not written to the Director.</source>
        <translation>Изменено: строк Run — %1 и описание, не записано в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="767"/>
        <source>Modified: %1 Run line(s), not written to the Director.</source>
        <translation>Изменено строк Run: %1, не записано в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="746"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="770"/>
        <source>The Director reports other Run lines since.</source>
        <translation>С тех пор Director сообщает другие строки Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="376"/>
        <source>Descr&amp;iption:</source>
        <translation>&amp;Описание:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="819"/>
        <source>Creates a new schedule with the Run lines and the Description of this one, as the Director reported them (changes not written are not copied); you choose the name and see the new resource before anything is written</source>
        <translation>Создаёт новый Schedule со строками Run и Description этого, как их сообщил Director (незаписанные изменения не копируются); вы выбираете имя и видите новый ресурс до того, как что-либо будет записано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="827"/>
        <source>Save or revert the changes of this schedule first: renaming replaces it</source>
        <translation>Сначала сохраните или отмените изменения этого Schedule: переименование заменяет его</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="828"/>
        <source>Gives this schedule another name: a copy under the new name, every job that names it changed to the copy, then the old one deleted; all shown once for confirmation first</source>
        <translation>Даёт этому Schedule другое имя: копия под новым именем, каждый Job, который его указывает, переводится на копию, затем старый удаляется; всё сначала показывается один раз для подтверждения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="845"/>
        <source>This connection cannot change a schedule; the click says why.</source>
        <translation>Через это подключение нельзя изменить Schedule; щелчок покажет причину.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="849"/>
        <source>This connection cannot delete a schedule; the click says why.</source>
        <translation>Через это подключение нельзя удалить Schedule; щелчок покажет причину.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="924"/>
        <source>The Director now reports other Run lines for this schedule than the ones your changes started from. Your changes are kept and nothing was replaced.</source>
        <translation>Director теперь сообщает для этого Schedule другие строки Run, чем те, с которых начались ваши изменения. Ваши изменения сохранены, ничего не заменено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="929"/>
        <source>The schedule %1 has changes that are not written. Showing %2 discards them.</source>
        <translation>В Schedule %1 есть незаписанные изменения. Показ %2 отменит их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="931"/>
        <source>&amp;Discard and show %1</source>
        <translation>&amp;Отменить и показать %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="958"/>
        <source>Schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="963"/>
        <source>(jobs not loaded yet)</source>
        <translation>(Jobs ещё не загружены)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="965"/>
        <source>Not used by any job</source>
        <translation>Не используется ни одним Job</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="967"/>
        <source>Used by %n job(s)</source>
        <translation>
            <numerusform>Используется в %n Job</numerusform>
            <numerusform>Используется в %n Jobs</numerusform>
            <numerusform>Используется в %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="973"/>
        <source>Disabled: the Director does not run it.</source>
        <translation>Отключено: Director его не запускает.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="978"/>
        <source>Contains %n Run line(s) that are not understood. The schedule is locked: it cannot be edited or written here. Change it in its configuration file on the Director.</source>
        <translation>
            <numerusform>Содержит %n нераспознанную строку Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
            <numerusform>Содержит %n нераспознанные строки Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
            <numerusform>Содержит %n нераспознанных строк Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
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
        <translation>%1: в этой области нет строк Run. Удалить её можно клавишей Del или через контекстное меню.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1011"/>
        <source>%1: no Run line in this scope</source>
        <translation>%1: в этой области нет строк Run</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1012"/>
        <source>%1: %n Run line(s) in this scope</source>
        <translation>
            <numerusform>%1: %n строка Run в этой области</numerusform>
            <numerusform>%1: %n строки Run в этой области</numerusform>
            <numerusform>%1: %n строк Run в этой области</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1018"/>
        <source>Scope %1, %n Run line(s)</source>
        <translation>
            <numerusform>Область %1, %n строка Run</numerusform>
            <numerusform>Область %1, %n строки Run</numerusform>
            <numerusform>Область %1, %n строк Run</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1055"/>
        <source>other scope, may run on the same day</source>
        <translation>другая область, может выполняться в тот же день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1059"/>
        <source>other scope, never on the same day</source>
        <translation>другая область, никогда не в тот же день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1073"/>
        <source>no time in the Run line: runs every hour at :00</source>
        <translation>в строке Run нет времени: запуск каждый час в :00</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1081"/>
        <source>calendar rule, not a cell</source>
        <translation>календарное правило, не ячейка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1111"/>
        <source>changed</source>
        <translation>изменено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1114"/>
        <source>new</source>
        <translation>новое</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1117"/>
        <source>removed</source>
        <translation>удалено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1129"/>
        <source>Reported by the Director as &quot;%1&quot;, a spelling it does not read back.</source>
        <translation>Director сообщает это как «%1» — в написании, которое он не читает обратно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1136"/>
        <source>not understood: %1</source>
        <translation>не распознано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1137"/>
        <source>Not understood and not shown in the grid: %1</source>
        <translation>Не распознано и не показано в сетке: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1138"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Приложение не может записать этот Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1147"/>
        <source>calendar rule %1</source>
        <translation>календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1152"/>
        <source>scope %1</source>
        <translation>область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1174"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1203"/>
        <source>Nothing selected. Click a cell or drag over cells; click a day or an hour to select all of it.</source>
        <translation>Ничего не выбрано. Щёлкните ячейку или протяните по ячейкам; щёлкните день или час, чтобы выбрать его целиком.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1238"/>
        <source>%1 in %n cell(s)</source>
        <translation>
            <numerusform>%1 в %n ячейке</numerusform>
            <numerusform>%1 в %n ячейках</numerusform>
            <numerusform>%1 в %n ячейках</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1242"/>
        <source>%n cell(s) selected:</source>
        <translation>
            <numerusform>Выбрана %n ячейка:</numerusform>
            <numerusform>Выбрано %n ячейки:</numerusform>
            <numerusform>Выбрано %n ячеек:</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1243"/>
        <source>no run in this scope</source>
        <translation>в этой области нет запусков</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1246"/>
        <source>Other scopes: %1</source>
        <translation>Другие области: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1247"/>
        <source>They stay when this scope is painted or erased</source>
        <translation>Они сохраняются при рисовании или стирании в этой области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1352"/>
        <source>Remove the scope &quot;%1&quot;</source>
        <translation>Удалить область «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1355"/>
        <source>Add scope...</source>
        <translation>Добавить область...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1394"/>
        <source>Calendar rule %1</source>
        <translation>Календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1397"/>
        <source>New calendar rule</source>
        <translation>Новое календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1425"/>
        <source>Edit calendar rule...</source>
        <translation>Изменить календарное правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1430"/>
        <source>Add calendar rule...</source>
        <translation>Добавить календарное правило...</translation>
    </message>
</context>
<context>
    <name>BScheduleGridWidget</name>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="71"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="415"/>
        <source>%1 hours</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="72"/>
        <source>Select all</source>
        <translation>Выбрать все</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="76"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="202"/>
        <source>Selects the whole day</source>
        <translation>Выбирает весь день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="78"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="204"/>
        <source>Selects this hour on every day</source>
        <translation>Выбирает этот час во все дни</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="80"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="205"/>
        <source>Selects all days and hours</source>
        <translation>Выбирает все дни и часы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="190"/>
        <source>Apply the tool</source>
        <translation>Применить инструмент</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="192"/>
        <source>Erase</source>
        <translation>Стереть</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="200"/>
        <source>Selects this cell</source>
        <translation>Выбирает эту ячейку</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="208"/>
        <source>Lists the runs of this cell</source>
        <translation>Показывает запуски этой ячейки</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="210"/>
        <source>Applies the tool in hand (%1) to this cell</source>
        <translation>Применяет текущий инструмент (%1) к этой ячейке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="214"/>
        <source>Removes the runs of the scope on display from this cell</source>
        <translation>Удаляет из этой ячейки запуски отображаемой области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="313"/>
        <source>Schedule grid of %1, scope %2</source>
        <translation>Сетка Schedule %1, область %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="317"/>
        <source>Weekdays by hours. Arrow keys move, Shift extends the selection, Enter lists the runs of a cell.</source>
        <translation>Дни недели по часам. Стрелки перемещают, Shift расширяет выделение, Enter показывает запуски ячейки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="320"/>
        <source>Space applies the tool to the selected cells, Delete erases them, F, D, I and V paint a level.</source>
        <translation>Пробел применяет инструмент к выбранным ячейкам, Delete стирает их, F, D, I и V рисуют Level.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="322"/>
        <source>Tool: %1.</source>
        <translation>Инструмент: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="324"/>
        <source>Paint mode is on.</source>
        <translation>Режим рисования включён.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="328"/>
        <source>Calendar rule %1: %2, %3</source>
        <translation>Календарное правило %1: %2, %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="62"/>
        <source>Schedule grid</source>
        <translation>Сетка Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="284"/>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="634"/>
        <source>Calendar rules</source>
        <translation>Календарные правила</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1242"/>
        <source>%1, %2 to %3</source>
        <translation>%1, %2–%3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1252"/>
        <source>the Run line names no time</source>
        <translation>в строке Run не указано время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1255"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1262"/>
        <source>No run in the scope &quot;%1&quot;</source>
        <translation>Нет запусков в области «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1269"/>
        <source>Other scopes:</source>
        <translation>Другие области:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1273"/>
        <source>%1: may run on the same day as this scope</source>
        <translation>%1: может выполняться в тот же день, что и эта область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1274"/>
        <source>%1: never on the same day as this scope</source>
        <translation>%1: никогда не в тот же день, что и эта область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1280"/>
        <source>Painting or erasing here changes only the scope &quot;%1&quot;; the runs of other scopes stay.</source>
        <translation>Рисование или стирание здесь изменяет только область «%1»; запуски других областей сохраняются.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1291"/>
        <source>Load: no run of an enabled schedule starts in this hour</source>
        <translation>Нагрузка: в этот час не начинается ни один запуск включённого Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1292"/>
        <source>Load: %1 run(s) a week of all enabled schedules start in this hour (average of the next weeks)</source>
        <translation>Нагрузка: запусков в неделю всех включённых Schedule, начинающихся в этот час: %1 (среднее за следующие недели)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1309"/>
        <source>%1: click to select the whole day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1: щёлкните, чтобы выбрать весь день. Ctrl добавляет или убирает его, Shift расширяет выделение от опорной точки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1313"/>
        <source>%1 to %2: click to select this hour on every day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1–%2: щёлкните, чтобы выбрать этот час во все дни. Ctrl добавляет или убирает его, Shift расширяет выделение от опорной точки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1318"/>
        <source>Click to select all days and hours (Ctrl+A)</source>
        <translation>Щёлкните, чтобы выбрать все дни и часы (Ctrl+A)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1320"/>
        <source>Run lines by day of the month or week of the year do not belong to a weekday. They are marked here at their hours and listed under &quot;Calendar rules&quot;; this band cannot be selected or painted. A double click on a mark opens the form of its rule.</source>
        <translation>Строки Run по дню месяца или неделе года не относятся к дню недели. Они отмечены здесь в своё время и перечислены в разделе «Календарные правила»; эту полосу нельзя выделить или закрасить. Двойной щелчок по метке открывает форму её правила.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1401"/>
        <source>%1, %2 to %3: %n run(s)</source>
        <translation>
            <numerusform>%1, %2–%3: %n запуск</numerusform>
            <numerusform>%1, %2–%3: %n запуска</numerusform>
            <numerusform>%1, %2–%3: %n запусков</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1409"/>
        <source>Runs of the cell</source>
        <translation>Запуски ячейки</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Overrides</source>
        <translation>Переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Scope</source>
        <translation>Область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1414"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1427"/>
        <source>no time in the Run line</source>
        <translation>в строке Run нет времени</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1433"/>
        <source>%1 (other scope, may run on the same day)</source>
        <translation>%1 (другая область, может выполняться в тот же день)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1434"/>
        <source>%1 (other scope, never on the same day)</source>
        <translation>%1 (другая область, никогда не в тот же день)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1441"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1453"/>
        <source>Enter or a double click shows the Run line in the list. Esc closes.</source>
        <translation>Enter или двойной щелчок показывает строку Run в списке. Esc закрывает.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1554"/>
        <source>Tool: %1</source>
        <translation>Инструмент: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleLegend</name>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="43"/>
        <source>Full backup. The length of the bar is the median duration of the job&apos;s measured runs at this level.</source>
        <translation>Full backup. Длина полосы — медианная длительность измеренных запусков Job на этом уровне.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="45"/>
        <source>Differential backup.</source>
        <translation>Differential backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="47"/>
        <source>Incremental backup.</source>
        <translation>Incremental backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="49"/>
        <source>Virtual full backup.</source>
        <translation>VirtualFull backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="51"/>
        <source>Pale and dashed, &quot;~&quot; in front of the letter: no run at this level was measured; the length is estimated from the job&apos;s runs at other levels.</source>
        <translation>Бледная и пунктирная, «~» перед буквой: ни один запуск на этом уровне не измерен; длина оценена по запускам Job на других уровнях.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="54"/>
        <source>Dashed outline with &quot;?&quot;: the job never ran, so there is no duration. Only the start is shown.</source>
        <translation>Пунктирный контур с «?»: Job ещё ни разу не выполнялся, поэтому длительность неизвестна. Показано только начало.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="56"/>
        <source>Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).</source>
        <translation>Более светлое продолжение с чертой на конце: к этому моменту завершились девять из десяти запусков (90-й процентиль).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="58"/>
        <source>Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.</source>
        <translation>Заполненные песочные часы: запуск будет ждать. Цветная линия за ними — запланированное время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="60"/>
        <source>Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).</source>
        <translation>Контур песочных часов: запуск может ждать, прогноз не точен (см. список прогнозируемых ожиданий).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="62"/>
        <source>Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.</source>
        <translation>Заштрихованная полоса: ожидание от запланированного времени до прогнозируемого начала. Пунктирный контур: не точно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="64"/>
        <source>&quot;?&quot; in front of a bar: the prediction for this run is not certain, it may have to wait.</source>
        <translation>«?» перед полосой: прогноз для этого запуска не точен, возможно, ему придётся ждать.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="66"/>
        <source>&quot;»&quot; at the right edge: the run starts after the last day on display.</source>
        <translation>«»» у правого края: запуск начинается после последнего отображаемого дня.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="68"/>
        <source>Open diamond: no job uses this schedule. It marks where a run would start.</source>
        <translation>Пустой ромб: этот Schedule не используется ни одним Job. Он отмечает, где начался бы запуск.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="70"/>
        <source>Arrow (day view only): the run at the head waits for the run at the tail, from the moment that one lets it go. The label names the limit; dotted: priority; dashed: only if something unknown turns out against the run. Point at a run to see its arrows.</source>
        <translation>Стрелка (только в представлении дня): запуск у острия ждёт запуск у хвоста — с того момента, как тот его отпустит. Подпись называет ограничение; точечная: приоритет; пунктирная: только если что-то неизвестное обернётся против запуска. Наведите указатель на запуск, чтобы увидеть его стрелки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="74"/>
        <source>Thin track below a bar (past dates and today): the run as the job history measured it, from its start to its end. The dotted line in front of it is the time it waited after its scheduled time. Point at it for the times, level and status.</source>
        <translation>Тонкая дорожка под полосой (прошедшие дни и сегодня): запуск, как его измерила история Jobs, от начала до конца. Пунктирная линия перед ним — время ожидания после запланированного времени. Наведите на неё, чтобы увидеть время, Level и статус.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="78"/>
        <source>Hatched with a dashed outline in the track: the measured run failed or was canceled.</source>
        <translation>Заштриховано с пунктирным контуром на дорожке: измеренный запуск завершился ошибкой или был отменён.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="80"/>
        <source>Red line: now.</source>
        <translation>Красная линия: сейчас.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="301"/>
        <source>&lt;b&gt;What the marks mean&lt;/b&gt;</source>
        <translation>&lt;b&gt;Что означают метки&lt;/b&gt;</translation>
    </message>
</context>
<context>
    <name>BScheduleMiniCalendar</name>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="88"/>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="113"/>
        <source>Calendar of runs, %1</source>
        <translation>Календарь запусков, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="119"/>
        <source>%1 to %2</source>
        <translation>%1 – %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="128"/>
        <source>%1: no run</source>
        <translation>%1: нет запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="131"/>
        <source>%1 %2 (Run line %3)</source>
        <translation>%1 %2 (строка Run %3)</translation>
    </message>
</context>
<context>
    <name>BScheduleNamePage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="70"/>
        <source>New Schedule</source>
        <translation>Новый Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="71"/>
        <source>Name the schedule. On the next page you paint its runs on a grid of weekdays and hours.</source>
        <translation>Дайте Schedule имя. На следующей странице вы нарисуете его запуски на сетке дней недели и часов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78"/>
        <source>e.g., WeeklyCycle</source>
        <translation>например, WeeklyCycle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="80"/>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="84"/>
        <source>e.g., Full Sunday, incremental daily</source>
        <translation>например, Full по воскресеньям, Incremental ежедневно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="85"/>
        <source>&amp;Description:</source>
        <translation>&amp;Описание:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="87"/>
        <source>&amp;Enabled</source>
        <translation>&amp;Включено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="88"/>
        <source>A schedule that is not enabled starts no job.</source>
        <translation>Невключённый Schedule не запускает ни одного Job.</translation>
    </message>
</context>
<context>
    <name>BSchedulePalette</name>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="127"/>
        <source>Brushes</source>
        <translation>Кисти</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="139"/>
        <location filename="../src/schedules/bschedulepalette.cpp" line="140"/>
        <source>Eraser</source>
        <translation>Ластик</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="141"/>
        <source>Removes the runs of the scope on display from cells. Del erases the selected cells with any tool in hand.</source>
        <translation>Удаляет из ячеек запуски отображаемой области. Del стирает выбранные ячейки при любом инструменте.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="144"/>
        <source>New brush...</source>
        <translation>Новая кисть...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="146"/>
        <source>Defines a brush: level, minute of the hour and overrides</source>
        <translation>Определяет кисть: Level, минуту часа и переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="148"/>
        <source>Paint mode</source>
        <translation>Режим рисования</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="150"/>
        <source>On: pressing a cell and dragging over cells applies the tool at once. Off: the mouse selects, and Space applies the tool to the selection. The key P switches it in the grid.</source>
        <translation>Вкл.: нажатие на ячейку и протягивание по ячейкам сразу применяет инструмент. Выкл.: мышь выделяет, а пробел применяет инструмент к выделению. Клавиша P переключает режим в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="154"/>
        <source>Keep other runs</source>
        <translation>Сохранять другие запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="156"/>
        <source>On: painting adds the brush to the runs a cell already has, so a cell can have several runs in one hour. Off: the cell ends up with the brush only. Shift+Space adds once without this switch.</source>
        <translation>Вкл.: рисование добавляет кисть к уже имеющимся в ячейке запускам, так что в ячейке может быть несколько запусков в один час. Выкл.: в ячейке остаётся только кисть. Shift+Space добавляет один раз без этого переключателя.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="201"/>
        <source>%1 = %2</source>
        <translation>%1 = %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="249"/>
        <source>Brush %1</source>
        <translation>Кисть %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="250"/>
        <source>Takes this brush in hand: %1. Space applies it to the selected cells.</source>
        <translation>Взять эту кисть: %1. Пробел применяет её к выбранным ячейкам.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="311"/>
        <source>In hand: the eraser.</source>
        <translation>Текущий инструмент: ластик.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="313"/>
        <source>In hand: the brush %1.</source>
        <translation>Текущий инструмент: кисть %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="317"/>
        <source>Paint mode: press a cell or drag over cells to apply it.</source>
        <translation>Режим рисования: нажмите на ячейку или протяните по ячейкам, чтобы применить.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="319"/>
        <source>Space applies it to the selected cells, Del erases them.</source>
        <translation>Пробел применяет к выбранным ячейкам, Del стирает их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="322"/>
        <source>Other runs of a painted cell stay.</source>
        <translation>Другие запуски закрашенной ячейки сохраняются.</translation>
    </message>
</context>
<context>
    <name>BSchedulePersistence</name>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="106"/>
        <source>The text of schedule %1 arrived changed</source>
        <translation>Текст Schedule %1 пришёл изменённым</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="107"/>
        <source>The SHA-256 of the text the Director sent does not match the text. Nothing was written.</source>
        <translation>SHA-256 текста, присланного Director, не соответствует тексту. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="120"/>
        <source>Schedule %1 has changed on the Director</source>
        <translation>Schedule %1 изменён в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="121"/>
        <source>The configuration file of the schedule does not say what was loaded here, so the change was not sent. Reload the schedule and make the change again. %1</source>
        <translation>Файл конфигурации Schedule не соответствует тому, что было загружено здесь, поэтому изменение не отправлено. Перезагрузите Schedule и внесите изменение заново. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="149"/>
        <source>Schedule %1 cannot be written</source>
        <translation>Schedule %1 нельзя записать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="150"/>
        <source>Run line %1 is not understood by this application (%2). A schedule with such a line is not changed here; change it in its configuration file.</source>
        <translation>Строка Run %1 не распознана этим приложением (%2). Schedule с такой строкой здесь не изменяется; измените его в файле конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="159"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="192"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="279"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="320"/>
        <source>Schedule %1 was not changed</source>
        <translation>Schedule %1 не изменён</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="160"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="379"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="450"/>
        <source>There is no connection to the configuration methods of the Director.</source>
        <translation>Нет подключения к методам конфигурации Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="193"/>
        <source>The change could not be written into the configuration text safely: %1</source>
        <translation>Изменение не удалось безопасно внести в текст конфигурации: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="201"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="267"/>
        <source>Nothing to write</source>
        <translation>Нечего записывать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="202"/>
        <source>The Run lines and the description are the ones the Director has.</source>
        <translation>Строки Run и описание совпадают с теми, что есть у Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="268"/>
        <source>The edited text is the one the Director has.</source>
        <translation>Отредактированный текст совпадает с текстом в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="280"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="420"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="496"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="319"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="449"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="495"/>
        <source>Schedule %1 was not created</source>
        <translation>Schedule %1 не создан</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="321"/>
        <source>The edited text was not sent to the Director: %1</source>
        <translation>Отредактированный текст не отправлен в Director: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="329"/>
        <source>the name of the schedule has to stay %1.</source>
        <translation>имя Schedule должно остаться %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="341"/>
        <source>The text was edited in the review; the Director checked the edited text.</source>
        <translation>Текст был отредактирован при проверке; Director проверил отредактированный текст.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="378"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="419"/>
        <source>Schedule %1 was not deleted</source>
        <translation>Schedule %1 не удалён</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="576"/>
        <source>Run line %1 cannot be written: %2</source>
        <translation>Строку Run %1 нельзя записать: %2</translation>
    </message>
</context>
<context>
    <name>BScheduleResource</name>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="46"/>
        <source>The name is empty.</source>
        <translation>Имя пустое.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="48"/>
        <source>The name is longer than 127 characters.</source>
        <translation>Имя длиннее 127 символов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="51"/>
        <source>A name may contain only letters, digits and the characters _ - . : (no blanks).</source>
        <translation>Имя может содержать только буквы, цифры и символы _ - . : (без пробелов).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="54"/>
        <source>There is already a schedule named %1.</source>
        <translation>Schedule с именем %1 уже существует.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="136"/>
        <source>The text does not start with a Schedule resource (line %1).</source>
        <translation>Текст не начинается с ресурса Schedule (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="140"/>
        <source>Directives on the line of &quot;Schedule {&quot; cannot be edited safely (line %1).</source>
        <translation>Директивы в строке «Schedule {» нельзя безопасно редактировать (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="146"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Текст после закрывающей скобки ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="150"/>
        <source>A nested block in a Schedule resource (line %1).</source>
        <translation>Вложенный блок в ресурсе Schedule (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="153"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Закрывающая скобка находится в одной строке с директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="184"/>
        <source>A quoted Run value does not end on its line (line %1).</source>
        <translation>Значение Run в кавычках не заканчивается в своей строке (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="187"/>
        <source>Something follows the Run value on its line (line %1).</source>
        <translation>После значения Run в его строке что-то следует (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="195"/>
        <source>A Run directive shares its line with another directive (line %1).</source>
        <translation>Директива Run находится в одной строке с другой директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="202"/>
        <source>A Run directive without a value (line %1).</source>
        <translation>Директива Run без значения (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="212"/>
        <source>A Run directive that does not start its line (line %1).</source>
        <translation>Директива Run не в начале строки (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="223"/>
        <source>The text contains no Schedule resource.</source>
        <translation>Текст не содержит ресурса Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="225"/>
        <source>The Schedule resource has no closing brace.</source>
        <translation>У ресурса Schedule нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="256"/>
        <source>The configuration file has %1 Run line(s), the schedule as it was loaded %2.</source>
        <translation>В файле конфигурации строк Run: %1, в загруженном Schedule: %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="266"/>
        <source>Run line %1 differs: the file says &quot;%2&quot;, the schedule as it was loaded &quot;%3&quot;.</source>
        <translation>Строка Run %1 отличается: в файле — «%2», в загруженном Schedule — «%3».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="288"/>
        <source>A new Run line without text.</source>
        <translation>Новая строка Run без текста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="292"/>
        <source>A Run line refers to line %1, which the file does not have.</source>
        <translation>Строка Run ссылается на строку %1, которой нет в файле.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="294"/>
        <source>The Run lines would change their order; that is not written.</source>
        <translation>Строки Run изменили бы свой порядок; это не записывается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="297"/>
        <source>A changed Run line without text.</source>
        <translation>Изменённая строка Run без текста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="301"/>
        <source>A Run line contains a line break.</source>
        <translation>Строка Run содержит перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="403"/>
        <source>The new text could not be read back: %1</source>
        <translation>Новый текст не удалось прочитать обратно: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="408"/>
        <source>The new text does not have the intended Run lines.</source>
        <translation>В новом тексте нет нужных строк Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="410"/>
        <source>The new text would change more than the Run lines.</source>
        <translation>Новый текст изменил бы больше, чем строки Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="412"/>
        <source>The new text would change the name of the schedule.</source>
        <translation>Новый текст изменил бы имя Schedule.</translation>
    </message>
</context>
<context>
    <name>BScheduleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="411"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="411"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewDialog</name>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="57"/>
        <source>(no Run line)</source>
        <translation>(нет строки Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="94"/>
        <source>changed: </source>
        <translation>изменено: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="95"/>
        <source>new: </source>
        <translation>новое: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="96"/>
        <source>removed: </source>
        <translation>удалено: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="111"/>
        <source>Change schedule %1</source>
        <translation>Изменить Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="112"/>
        <source>Change schedule %1 on the Director?</source>
        <translation>Изменить Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="113"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новую конфигурацию и принимает её. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="117"/>
        <source>Create schedule %1</source>
        <translation>Создать Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="118"/>
        <source>Create schedule %1 on the Director?</source>
        <translation>Создать Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="119"/>
        <source>The Director checked the new schedule and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новый Schedule и принимает его. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="123"/>
        <source>Delete schedule %1</source>
        <translation>Удалить Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="124"/>
        <source>Delete schedule %1 on the Director?</source>
        <translation>Удалить Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="125"/>
        <source>The Director checked that nothing refers to the schedule. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Director проверил, что на Schedule ничто не ссылается. Ничего не будет удалено, пока вы не выберете «Удалить в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="143"/>
        <source>Run lines now (in the file)</source>
        <translation>Строки Run сейчас (в файле)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="146"/>
        <source>Run lines now</source>
        <translation>Строки Run сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="151"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="154"/>
        <source>Run lines after writing</source>
        <translation>Строки Run после записи</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="162"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="171"/>
        <source>Jobs that use this schedule</source>
        <translation>Jobs, использующие этот Schedule</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="165"/>
        <source>The change applies to every one of these %n job(s), not only to the one on display. To move a single job, copy the schedule and assign the copy to that job instead.</source>
        <translation>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="176"/>
        <source>(no job of the Director uses it, as far as this application knows)</source>
        <translation>(ни один Job Director его не использует, насколько известно этому приложению)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="180"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Director указывает как ссылающиеся на него: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="192"/>
        <source>Text after writing, editable</source>
        <translation>Текст после записи, редактируемый</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="193"/>
        <source>This text can be edited. An edited text is checked by the Director and shown again before anything is written.</source>
        <translation>Этот текст можно редактировать. Отредактированный текст проверяется Director и показывается снова до какой-либо записи.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="196"/>
        <source>Text after writing</source>
        <translation>Текст после записи</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="199"/>
        <source>Text now</source>
        <translation>Текст сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="203"/>
        <source>The text was edited. &quot;Check with the Director&quot; sends it for a check only; the review then shows it again, and only &quot;Write to the Director&quot; there writes it.</source>
        <translation>Текст был отредактирован. «Проверить в Director» отправляет его только на проверку; затем проверка показывает его снова, и записывает его только «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="213"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Файл в Director: %1 (в нём есть и другие ресурсы; заменяется только этот ресурс).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="215"/>
        <source>File on the Director: %1.</source>
        <translation>Файл в Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="218"/>
        <source>The Director reloads its configuration after writing. Runs it has already queued may still start at the old time.</source>
        <translation>После записи Director перезагружает свою конфигурацию. Запуски, уже поставленные им в очередь, ещё могут начаться в прежнее время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="224"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Удалить в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="225"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Записать в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="256"/>
        <source>&amp;Check with the Director</source>
        <translation>&amp;Проверить в Director</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="202"/>
        <source>Review</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="203"/>
        <source>These are the Run lines and the resource that will be written. Finish creates the schedule.</source>
        <translation>Это строки Run и ресурс, которые будут записаны. «Готово» создаёт Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="208"/>
        <source>Run lines:</source>
        <translation>Строки Run:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="215"/>
        <source>Resource:</source>
        <translation>Ресурс:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="223"/>
        <source>Director command:</source>
        <translation>Команда Director:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="233"/>
        <source>Copy Resource</source>
        <translation>Копировать ресурс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="234"/>
        <source>Copies the resource text to the clipboard</source>
        <translation>Копирует текст ресурса в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="237"/>
        <source>Resource copied to clipboard.</source>
        <translation>Ресурс скопирован в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="286"/>
        <source>This schedule cannot be written. %1</source>
        <translation>Этот Schedule нельзя записать. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Schedule cannot be written</source>
        <translation>Schedule нельзя записать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="309"/>
        <source>%1

Go back and paint at least one run, or cancel the wizard.</source>
        <translation>%1

Вернитесь назад и нарисуйте хотя бы один запуск или отмените мастер.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="322"/>
        <source>Deploy to Director</source>
        <translation>Развернуть на Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="323"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Отправить &apos;configure add&apos; на Director?

Нажмите «Да», чтобы выполнить, или «Нет», чтобы закрыть без развёртывания.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="347"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="369"/>
        <source>Another change of a schedule is being written. Try again when it is done.</source>
        <translation>Записывается другое изменение Schedule. Повторите попытку, когда оно завершится.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="375"/>
        <source>The Director checks the schedule...</source>
        <translation>Director проверяет Schedule...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="383"/>
        <source>Schedule created.</source>
        <translation>Schedule создан.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="387"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="389"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="416"/>
        <source>Schedule created successfully! Click Finish to close.</source>
        <translation>Schedule успешно создан! Нажмите «Готово», чтобы закрыть.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="419"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="427"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check with &apos;show schedules&apos; whether the schedule exists.</source>
        <translation>Подключение к Director было потеряно до его ответа. Подключитесь заново и проверьте командой &apos;show schedules&apos;, существует ли Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="435"/>
        <source>Timeout waiting for Director response. The schedule may have been created — check with &apos;show schedules&apos;.</source>
        <translation>Истекло время ожидания ответа Director. Возможно, Schedule был создан — проверьте командой &apos;show schedules&apos;.</translation>
    </message>
</context>
<context>
    <name>BScheduleScopeDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="382"/>
        <source>&amp;last (the last seven days of a month)</source>
        <translation>&amp;last (последние семь дней месяца)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="386"/>
        <source>Add scope</source>
        <translation>Добавить область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="389"/>
        <source>A scope is the part of the calendar a grid is painted for: weeks of the month, and months if not all. What is painted in it becomes Run lines such as &quot;1st sat at 21:00&quot;.</source>
        <translation>Область — это часть календаря, для которой рисуется сетка: недели месяца и, если не все, месяцы. Нарисованное в ней становится строками Run, например «1st sat at 21:00».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="395"/>
        <source>Weeks of the month</source>
        <translation>Недели месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="401"/>
        <source>Days %1 to %2 of a month</source>
        <translation>Дни с %1 по %2 месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="411"/>
        <source>Months</source>
        <translation>Месяцы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="420"/>
        <source>&amp;All months</source>
        <translation>&amp;Все месяцы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="477"/>
        <source>Choose at least one month.</source>
        <translation>Выберите хотя бы один месяц.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="479"/>
        <source>Choose at least one week of the month, or &quot;last&quot;.</source>
        <translation>Выберите хотя бы одну неделю месяца или «last».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="485"/>
        <source>The scope &quot;%1&quot; exists already; it will be shown.</source>
        <translation>Область «%1» уже существует; она будет показана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="486"/>
        <source>New scope: %1</source>
        <translation>Новая область: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleWaitModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1259"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1283"/>
        <source>%1 s</source>
        <translation>%1 с</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1261"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1282"/>
        <source>%1 min</source>
        <translation>%1 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1266"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1281"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1304"/>
        <source>%1 h</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1280"/>
        <source>%1 d</source>
        <translation>%1 д</translation>
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
        <translation>Max Run Time не задано, принято %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1310"/>
        <source>%n run(s) wait, longest %1</source>
        <translation>
            <numerusform>%n запуск ожидает, самое долгое ожидание: %1</numerusform>
            <numerusform>%n запуска ожидают, самое долгое ожидание: %1</numerusform>
            <numerusform>%n запусков ожидают, самое долгое ожидание: %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1311"/>
        <source>No waiting predicted</source>
        <translation>Ожидание не прогнозируется</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1313"/>
        <source>%1 uncertain</source>
        <translation>%1 неточно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1322"/>
        <source>Director limit: the Director runs %1 job(s) at a time (Maximum Concurrent Jobs = %1) and all of them are taken.</source>
        <translation>Ограничение Director: Director выполняет одновременно Jobs: %1 (Maximum Concurrent Jobs = %1), и все слоты заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1324"/>
        <source>Director limit: Maximum Concurrent Jobs of the Director is not known. With the Bareos default of 1 every job slot would be taken.</source>
        <translation>Ограничение Director: Maximum Concurrent Jobs для Director неизвестно. При значении Bareos по умолчанию 1 все слоты для Jobs были бы заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1328"/>
        <source>Storage limit: storage %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Ограничение Storage: Storage %1 принимает одновременно Jobs: %2 (Maximum Concurrent Jobs = %2), и все места заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1332"/>
        <source>Storage limit: the storage of this job is not known, and so is its Maximum Concurrent Jobs. With one shared storage and the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Storage: Storage этого Job неизвестен, как и его Maximum Concurrent Jobs. При одном общем Storage и значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1335"/>
        <source>Storage limit: Maximum Concurrent Jobs of storage %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Storage: Maximum Concurrent Jobs для Storage %1 неизвестно. При значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1339"/>
        <source>Client limit: client %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Ограничение Client: Client %1 принимает одновременно Jobs: %2 (Maximum Concurrent Jobs = %2), и все места заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1342"/>
        <source>Client limit: Maximum Concurrent Jobs of client %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Client: Maximum Concurrent Jobs для Client %1 неизвестно. При значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1346"/>
        <source>Job limit: job %1 may run %2 time(s) at once (Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.</source>
        <translation>Ограничение Job: Job %1 может выполняться одновременно %2 раз(а) (Maximum Concurrent Jobs = %2), а его более ранний запуск ещё не завершён.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1349"/>
        <source>Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos default of 1 an earlier start of it would be in the way.</source>
        <translation>Ограничение Job: Maximum Concurrent Jobs для Job %1 неизвестно. При значении Bareos по умолчанию 1 помешал бы его более ранний запуск.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1353"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. The Director starts a run of a lower priority (a higher number) only when no run of another priority is running.</source>
        <translation>Приоритет: у этого запуска приоритет %1, а выполняются запуски с приоритетом %2. Director запускает задание с более низким приоритетом (большим числом), только когда не выполняется ни один запуск с другим приоритетом.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1358"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. It starts beside them only if it and all of them have Allow Mixed Priority.</source>
        <translation>Приоритет: у этого запуска приоритет %1, а выполняются запуски с приоритетом %2. Он запустится параллельно с ними, только если у него и у всех них задано Allow Mixed Priority.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1362"/>
        <source>Queue order: a run of priority %1 is waiting ahead in the queue and cannot start yet. The Director starts no run that is behind it in the queue.</source>
        <translation>Порядок очереди: запуск с приоритетом %1 ожидает впереди в очереди и пока не может начаться. Director не запускает ничего, что стоит за ним в очереди.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1386"/>
        <source>from %1, duration unknown</source>
        <translation>с %1, длительность неизвестна</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1396"/>
        <source>%1 to about %2, estimated</source>
        <translation>с %1 примерно до %2, оценка</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1398"/>
        <source>%1 to %2</source>
        <translation>%1–%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1407"/>
        <source>and %1 more</source>
        <translation>и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1420"/>
        <source>%1: it may run until %2 (%3)</source>
        <translation>%1: может выполняться до %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1421"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1426"/>
        <source>Maximum Concurrent Jobs of the Director is not known</source>
        <translation>Maximum Concurrent Jobs для Director неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1429"/>
        <source>the storage of the job is not known (the Job and its JobDefs name none)</source>
        <translation>Storage этого Job неизвестен (ни Job, ни его JobDefs его не указывают)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1430"/>
        <source>Maximum Concurrent Jobs of storage %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Storage %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1432"/>
        <source>Maximum Concurrent Jobs of client %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Client %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1434"/>
        <source>Maximum Concurrent Jobs of job %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Job %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1436"/>
        <source>Allow Mixed Priority of job %1 is not known</source>
        <translation>Allow Mixed Priority для Job %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1438"/>
        <source>the duration of %1 was not measured at this level</source>
        <translation>длительность %1 на этом уровне не измерялась</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1440"/>
        <source>the duration of %1 was never measured</source>
        <translation>длительность %1 никогда не измерялась</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1443"/>
        <source>the number of parallel writers of the storage device is not known (the job names no storage; the Director cannot read it)</source>
        <translation>число параллельных записей устройства Storage неизвестно (Job не указывает Storage; Director не может его прочитать)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1445"/>
        <source>the number of parallel writers of the device of storage %1 is not known (the Director cannot read it; set it in the list of predicted waits)</source>
        <translation>число параллельных записей устройства Storage %1 неизвестно (Director не может его прочитать; задайте его в списке прогнозируемых ожиданий)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1509"/>
        <source>Director limit (Maximum Concurrent Jobs = %1)</source>
        <translation>Ограничение Director (Maximum Concurrent Jobs = %1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1512"/>
        <source>Storage limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Storage (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1516"/>
        <source>Client limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Client (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1520"/>
        <source>Job limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Job (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1524"/>
        <source>Priority %1, runs of priority %2 are running</source>
        <translation>Приоритет %1, выполняются запуски с приоритетом %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1528"/>
        <source>Queue order: a run of priority %1 waits ahead</source>
        <translation>Порядок очереди: впереди ожидает запуск с приоритетом %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1535"/>
        <source>depends on values that are not known</source>
        <translation>зависит от неизвестных значений</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1538"/>
        <source>May wait: %1</source>
        <translation>Может ожидать: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1540"/>
        <source>may be longer: %1</source>
        <translation>может быть дольше: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1562"/>
        <source>%1 to %2: %3</source>
        <translation>%1–%2: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1568"/>
        <source>Waits for: %1</source>
        <translation>Ожидает: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1573"/>
        <source>Predicted start: %1, %2 after the scheduled time (%3)</source>
        <translation>Прогнозируемое начало: %1, через %2 после запланированного времени (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1582"/>
        <source>it depends on values that are not known</source>
        <translation>это зависит от неизвестных значений</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1585"/>
        <source>Uncertain, the wait may be longer: %1.</source>
        <translation>Неточно, ожидание может быть дольше: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1587"/>
        <source>Uncertain, this run may have to wait: %1.</source>
        <translation>Неточно, этому запуску, возможно, придётся ждать: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1591"/>
        <source>If so: %1</source>
        <translation>Если так: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1614"/>
        <source>Inside the run: waits %1 in the storage daemon for a device of storage %2 (%3 parallel writer(s); after two waits for a release it tries again every %4 s)%5.</source>
        <translation>Внутри запуска: ожидает %1 в демоне Storage устройство Storage %2 (параллельных записей: %3; после двух ожиданий освобождения повторяет попытку каждые %4 с)%5.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1621"/>
        <source>, held by %1</source>
        <translation>, занято %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1624"/>
        <source>Predicted end: %1</source>
        <translation>Прогнозируемое окончание: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1627"/>
        <source>Start overhead %1 (from the Director starting the job to its recorded start: connections, Client Run Before Job)%2.</source>
        <translation>Накладные расходы на запуск %1 (от запуска Job в Director до зафиксированного начала: подключения, Client Run Before Job)%2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1636"/>
        <source>Measured wait at this level: %1; predicted %2.</source>
        <translation>Измеренное ожидание на этом уровне: %1; прогноз: %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1663"/>
        <source>not measured</source>
        <translation>не измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1664"/>
        <source>median %1, max %2 (%3 run(s))</source>
        <translation>медиана %1, максимум %2 (запусков: %3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1675"/>
        <source>Prediction too low: measured median %1, predicted %2 - a limit the GUI does not know, e.g. the storage device (parallel writers), or a Client Run Before Job that takes long.</source>
        <translation>Прогноз занижен: измеренная медиана %1, прогноз %2 — ограничение, неизвестное GUI, например устройство Storage (параллельные записи) или долгий Client Run Before Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1680"/>
        <source>Prediction too high: measured median %1, predicted %2 - a limit is set lower here than it is, or the durations used are too long.</source>
        <translation>Прогноз завышен: измеренная медиана %1, прогноз %2 — здесь ограничение задано ниже, чем на самом деле, или используемые длительности слишком велики.</translation>
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
        <translation>Jobs:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="144"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="237"/>
        <source>Timeline</source>
        <translation>Временная шкала</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="238"/>
        <source>Grid</source>
        <translation>Сетка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="239"/>
        <source>Measured waits</source>
        <translation>Измеренные ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="240"/>
        <source>View:</source>
        <translation>Вид:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="246"/>
        <source>Day</source>
        <translation>День</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="247"/>
        <source>Week</source>
        <translation>Неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="256"/>
        <source>Today</source>
        <translation>Сегодня</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="258"/>
        <source>Show today or the current week</source>
        <translation>Показать сегодня или текущую неделю</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="268"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1109"/>
        <source>The date on display; the calendar opens from the arrow</source>
        <translation>Отображаемая дата; календарь открывается по стрелке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="281"/>
        <source>Snap:</source>
        <translation>Привязка:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="288"/>
        <source>Zoom:</source>
        <translation>Масштаб:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="293"/>
        <source>Zoom level (pixels per hour)</source>
        <translation>Масштаб (пикселей в час)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="348"/>
        <source>Legend</source>
        <translation>Легенда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="350"/>
        <source>What the bars, hourglasses, question marks and diamonds of the timeline mean</source>
        <translation>Что означают полосы, песочные часы, вопросительные знаки и ромбы на временной шкале</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="371"/>
        <source>Schedules:</source>
        <translation>Schedules:</translation>
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
        <translation>Тип</translation>
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
        <translation>Медианная длительность</translation>
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
        <translation>Загружено Schedules: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="810"/>
        <source>%1 schedules loaded (%2 Run lines)</source>
        <translation>Загружено Schedules: %1 (строк Run: %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="852"/>
        <source>Refreshing schedule list...</source>
        <translation>Обновление списка Schedules...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="932"/>
        <source>Jobs (%1):</source>
        <translation>Jobs (%1):</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="934"/>
        <source>Jobs (%1 Schedules):</source>
        <translation>Jobs (Schedules: %1):</translation>
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
        <translation>Медиана %1 измеренных запусков на всех уровнях за последние %2 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="975"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1438"/>
        <source>No history</source>
        <translation>Нет истории</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1108"/>
        <source>The week on display: pick any day of it; the calendar opens from the arrow</source>
        <translation>Отображаемая неделя: выберите любой её день; календарь открывается стрелкой</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1118"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1119"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="486"/>
        <source>Measured history</source>
        <translation>Измеренная история</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="333"/>
        <source>Measured runs</source>
        <translation>Измеренные запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="335"/>
        <source>Show the runs of past dates and today as the job history measured them, in a thin track below the planned bars</source>
        <translation>Показывает запуски прошедших дней и сегодняшнего дня, как их измерила история Jobs, тонкой дорожкой под запланированными полосами</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="497"/>
        <source>Close the area below the timeline</source>
        <translation>Закрыть область под временной шкалой</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1141"/>
        <source>Click a run in the timeline: its measured history appears here.</source>
        <translation>Щёлкните по запуску на временной шкале: здесь появится его измеренная история.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1173"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1211"/>
        <source>25% - 75%</source>
        <translation>25% - 75%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1212"/>
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1219"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1988"/>
        <source>Runs</source>
        <translation>Запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1225"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>Median wait</source>
        <translation>Медианное ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1226"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>90% wait</source>
        <translation>Ожидание 90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1988"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1233"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Wait</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1378"/>
        <source>no job uses this schedule</source>
        <translation>этот Schedule не используется ни одним Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1382"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>нет успешных запусков этого Job за последние %1 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1385"/>
        <source>No history: %1.</source>
        <translation>Нет истории: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1397"/>
        <source>Estimate: no run at this level in the last %1 days. The figures are from all levels of the job.</source>
        <translation>Оценка: нет запусков с этим Level за последние %1 дн. Данные по всем Level этого Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1413"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1414"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1433"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2386"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3205"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1467"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1473"/>
        <source>1 hour</source>
        <translation>1 час</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1468"/>
        <source>2 hours</source>
        <translation>2 часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1469"/>
        <source>4 hours</source>
        <translation>4 часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1471"/>
        <source>15 min</source>
        <translation>15 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1472"/>
        <source>30 min</source>
        <translation>30 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1540"/>
        <source>Run line %1 &quot;%2&quot;: %3</source>
        <translation>Строка Run %1 «%2»: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1555"/>
        <source>Not understood and not shown in the views:</source>
        <translation>Не распознано и не показано в представлениях:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1557"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Приложение не может записать этот Schedule.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="1563"/>
        <source>%n Run line(s) not understood</source>
        <translation>
            <numerusform>%n строка Run не распознана</numerusform>
            <numerusform>%n строки Run не распознаны</numerusform>
            <numerusform>%n строк Run не распознано</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1565"/>
        <source>These Run lines are not shown in the views, and their schedules cannot be written by the application:</source>
        <translation>Эти строки Run не показаны в представлениях, и приложение не может записать их Schedules:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1705"/>
        <source>Schedule %1 was not changed: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird), which are not available on this connection. %2 stays at %3 (dropped at %4).</source>
        <translation>Schedule %1 не изменён: для изменения Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird), которые недоступны в этом подключении. %2 остаётся на %3 (перетащено на %4).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1738"/>
        <source>Schedule %1 was not changed: another change is being written.</source>
        <translation>Schedule %1 не изменён: записывается другое изменение.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1752"/>
        <source>Run line %1 is not understood, so the schedule is locked.</source>
        <translation>Строка Run %1 не распознана, поэтому Schedule заблокирован.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1755"/>
        <source>the dragged Run line runs at several hours of the day; change it in the grid.</source>
        <translation>перетащенная строка Run выполняется в несколько часов суток; измените её в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1759"/>
        <source>the dragged run could not be matched to exactly one Run line; refresh and try again, or change it in the grid.</source>
        <translation>перетащенный запуск не удалось сопоставить ровно одной строке Run; обновите и повторите попытку или измените его в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1763"/>
        <source>its Run lines are not loaded yet.</source>
        <translation>его строки Run ещё не загружены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1766"/>
        <source>%1 is not a time of day.</source>
        <translation>%1 не является временем суток.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1770"/>
        <source>the new Run line did not read back as intended (%1).</source>
        <translation>новая строка Run прочиталась обратно не так, как предполагалось (%1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1773"/>
        <source>Schedule %1 was not changed: %2</source>
        <translation>Schedule %1 не изменён: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1789"/>
        <source>Schedule %1 was not changed: the move was cancelled.</source>
        <translation>Schedule %1 не изменён: перемещение отменено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1799"/>
        <source>Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). Only this line is written anew; the other lines stay as they are in the file.</source>
        <translation>Строка Run %1 Schedule %2 переносится с %3 на %4 (перетащено на временной шкале: %5). Заново записывается только эта строка; остальные строки остаются в файле без изменений.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1804"/>
        <source>The Director checks the change of schedule %1...</source>
        <translation>Director проверяет изменение Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1841"/>
        <source>Copying a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules), which are not available on this connection</source>
        <translation>Для копирования Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director (создание Schedule), которые недоступны через это соединение</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857"/>
        <source>Schedule %1 is locked: it contains a Run line that is not understood.</source>
        <translation>Schedule %1 заблокирован: он содержит нераспознанную строку Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1863"/>
        <source>Schedule %1 was not written: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director. Your changes are kept in the grid.</source>
        <translation>Schedule %1 не записан: для изменения Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director. Ваши изменения сохранены в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1869"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1899"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3970"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4009"/>
        <source>Another change is being written.</source>
        <translation>Записывается другое изменение.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1875"/>
        <source>Schedule %1 cannot be written: %2</source>
        <translation>Schedule %1 нельзя записать: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1878"/>
        <source>The Director checks the changes of schedule %1...</source>
        <translation>Director проверяет изменения Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1881"/>
        <source>The changes made in the grid. Lines that were not touched stay as they are in the file.</source>
        <translation>Изменения, внесённые в сетке. Незатронутые строки остаются в файле без изменений.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1893"/>
        <source>Schedule %1 was not deleted: deleting a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Schedule %1 не удалён: для удаления Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1902"/>
        <source>The Director checks whether schedule %1 can be deleted...</source>
        <translation>Director проверяет, можно ли удалить Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1917"/>
        <source>The Director reloaded its configuration.</source>
        <translation>Director перезагрузил свою конфигурацию.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1919"/>
        <source>Follow-up commands: %1</source>
        <translation>Дополнительные команды: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1978"/>
        <source>Refresh history</source>
        <translation>Обновить историю</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1981"/>
        <source>Ask the Director again for the jobs of the last days (llist jobs)</source>
        <translation>Повторно запросить у Director Jobs за последние дни (llist jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1989"/>
        <source>90% duration</source>
        <translation>Длительность 90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1990"/>
        <source>Last run</source>
        <translation>Последний запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1992"/>
        <source>Runs in the window that terminated normally or with warnings</source>
        <translation>Запуски в окне, завершившиеся нормально или с предупреждениями</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1994"/>
        <source>Wait: start of the job minus its scheduled time</source>
        <translation>Ожидание: начало Job минус запланированное время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1996"/>
        <source>Nine of ten runs waited no longer than this</source>
        <translation>Девять из десяти запусков ждали не дольше этого</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2119"/>
        <source>the request got no reply (%1)</source>
        <translation>запрос остался без ответа (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2200"/>
        <source>the Director refused the request (%1)</source>
        <translation>Director отклонил запрос (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2201"/>
        <source>no reason given</source>
        <translation>причина не указана</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2204"/>
        <source>the Director&apos;s reply could not be read (%1)</source>
        <translation>не удалось прочитать ответ Director (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2228"/>
        <source>Schedules: no job history, %1</source>
        <translation>Schedules: нет истории Jobs, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2262"/>
        <source>the job history is being loaded</source>
        <translation>история Jobs загружается</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2266"/>
        <source>the job history has not been requested yet</source>
        <translation>история Jobs ещё не запрошена</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2299"/>
        <source>Not connected.</source>
        <translation>Нет подключения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2301"/>
        <source>The job history has not been requested yet.</source>
        <translation>История Jobs ещё не запрошена.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2303"/>
        <source>No job history: %1.</source>
        <translation>Нет истории Jobs: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2304"/>
        <source>The schedule views work without it: runs have no duration and waits are unknown.</source>
        <translation>Представления Schedules работают и без неё: у запусков нет длительности, ожидания неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2308"/>
        <source>Requesting the jobs of the last %1 days from the Director...</source>
        <translation>Запрос у Director Jobs за последние %1 дн....</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2314"/>
        <source>Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs terminated normally or with warnings and were measured.</source>
        <translation>Jobs, запланированные за последние %1 дн., получено %2: строк: %3, из них %4 запусков завершились нормально или с предупреждениями и были измерены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2323"/>
        <source>The row limit (%1) was reached and this Director returned the oldest rows: the newest runs of the window are missing.</source>
        <translation>Достигнут лимит строк (%1), и этот Director вернул самые старые строки: самые новые запуски окна отсутствуют.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2325"/>
        <source>The row limit (%1) was reached: the oldest runs of the window are missing.</source>
        <translation>Достигнут лимит строк (%1): самые старые запуски окна отсутствуют.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2329"/>
        <source>This Director did not deliver the scheduled time: all waits are unknown.</source>
        <translation>Этот Director не передал запланированное время: все ожидания неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2331"/>
        <source>%1 runs came without a scheduled time: their wait is unknown.</source>
        <translation>Запусков без запланированного времени: %1; их ожидание неизвестно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2335"/>
        <source>%1 rows were left out because they start before their scheduled time: copied, migrated and virtual full jobs carry the times of another job.</source>
        <translation>Пропущено строк: %1, так как они начинаются раньше запланированного времени: Jobs Copy, Migrate и VirtualFull несут время другого Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2340"/>
        <source>%1 rows were left out because their start or end time is missing or the end is before the start.</source>
        <translation>Пропущено строк: %1, так как у них отсутствует время начала или окончания либо окончание раньше начала.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2366"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3006"/>
        <source>(none)</source>
        <translation>(нет)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2375"/>
        <source>From %1 of %2 runs; the others came without a scheduled time.</source>
        <translation>По %1 из %2 запусков; остальные пришли без запланированного времени.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2384"/>
        <source>The Director delivered no scheduled time for these runs.</source>
        <translation>Director не передал запланированное время для этих запусков.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2868"/>
        <source>Waiting not predicted yet</source>
        <translation>Ожидание ещё не спрогнозировано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2873"/>
        <source>The job history is being loaded. Without the measured durations every run would count as lasting as long as its job allows, and the waits would be a guess: they are predicted once the history is in.</source>
        <translation>Загружается история заданий. Без измеренных длительностей каждый запуск длился бы столько, сколько позволяет его Job, и ожидания были бы догадкой: они прогнозируются, как только история загружена.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2903"/>
        <source>Not predicted yet: the job history is being loaded.</source>
        <translation>Прогноза пока нет: загружается история заданий.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3824"/>
        <source>Write to the Director</source>
        <translation>Записать на Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3868"/>
        <source>Move a run of schedule %1</source>
        <translation>Переместить запуск Schedule %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3869"/>
        <source>Schedule %1 is used by %n job(s): %2. What should change?</source>
        <translation>
            <numerusform>Schedule %1 используется %n Job: %2. Что изменить?</numerusform>
            <numerusform>Schedule %1 используется %n Jobs: %2. Что изменить?</numerusform>
            <numerusform>Schedule %1 используется %n Jobs: %2. Что изменить?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3871"/>
        <source>Changing the schedule moves the run for every job that uses it. Moving only %1 creates a copy of the schedule named %2 with the run at %3 and sets the Schedule of %1 to it.</source>
        <translation>Изменение Schedule перемещает запуск для каждого Job, который его использует. Перемещение только %1 создаёт копию Schedule с именем %2 с запуском в %3 и назначает её в Schedule у %1.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3876"/>
        <source>Change schedule %1 for all %n jobs</source>
        <translation>
            <numerusform>Изменить Schedule %1 для %n Job</numerusform>
            <numerusform>Изменить Schedule %1 для всех %n Jobs</numerusform>
            <numerusform>Изменить Schedule %1 для всех %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3878"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4150"/>
        <source>Move only job %1</source>
        <translation>Переместить только Job %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3940"/>
        <source>Renaming a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (read, create and delete schedules, read and change jobs), which are not available on this connection</source>
        <translation>Для переименования Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director (чтение, создание и удаление Schedule, чтение и изменение Jobs), которые недоступны через это соединение</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3965"/>
        <source>Schedule %1 was not copied: creating a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Schedule %1 не скопирован: для создания Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3974"/>
        <source>Schedule %1 was not copied: its Run lines are not loaded.</source>
        <translation>Schedule %1 не скопирован: его строки Run не загружены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3981"/>
        <source>Copy schedule %1</source>
        <translation>Копировать Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3982"/>
        <source>Name of the new schedule:</source>
        <translation>Имя нового Schedule:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3989"/>
        <source>The Director checks the copy %1 of schedule %2...</source>
        <translation>Director проверяет копию %1 Schedule %2...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4001"/>
        <source>Schedule %1 was not renamed: %2.</source>
        <translation>Schedule %1 не переименован: %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4005"/>
        <source>Schedule %1 was not renamed: save or revert its changes first.</source>
        <translation>Schedule %1 не переименован: сначала сохраните или отмените его изменения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4012"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4084"/>
        <source>Rename schedule %1</source>
        <translation>Переименовать Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4012"/>
        <source>New name:</source>
        <translation>Новое имя:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4027"/>
        <source>Reading schedule %1 and the jobs that use it...</source>
        <translation>Чтение Schedule %1 и Jobs, которые его используют...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4036"/>
        <source>Nothing was written: the file of schedule %1 does not say what was loaded (%2). Refresh and try again.</source>
        <translation>Ничего не записано: файл Schedule %1 не соответствует загруженному (%2). Обновите и повторите попытку.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4042"/>
        <source>Nothing was written: the Name of schedule %1 could not be found in its file.</source>
        <translation>Ничего не записано: Name Schedule %1 не найден в его файле.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4051"/>
        <source>New schedule %1, the text of %2 under the new name:</source>
        <translation>Новый Schedule %1, текст %2 под новым именем:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4062"/>
        <source>Nothing was written: job %1 takes its Schedule from its JobDefs. Change the JobDefs first, or copy the schedule instead.</source>
        <translation>Ничего не записано: Job %1 берёт свой Schedule из своего JobDefs. Сначала измените JobDefs или вместо этого скопируйте Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4067"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4128"/>
        <source>Nothing was written: on the Director job %1 names schedule %2, not %3. Refresh and try again.</source>
        <translation>Ничего не записано: на Director Job %1 указывает Schedule %2, а не %3. Обновите и повторите попытку.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4082"/>
        <source>deletion of schedule %1</source>
        <translation>удаление Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4083"/>
        <source>Then schedule %1 is deleted.</source>
        <translation>Затем Schedule %1 удаляется.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4085"/>
        <source>Nothing was written: the rename of schedule %1 was not confirmed.</source>
        <translation>Ничего не записано: переименование Schedule %1 не подтверждено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4090"/>
        <source>Schedule %1 is now named %2. Written: %3.</source>
        <translation>Schedule %1 теперь называется %2. Записано: %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4120"/>
        <source>Reading job %1...</source>
        <translation>Чтение Job %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4144"/>
        <source>New schedule %1, a copy of %2 with Run line %3 at %4, for job %5 only:</source>
        <translation>Новый Schedule %1, копия %2 со строкой Run %3 в %4, только для Job %5:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4146"/>
        <source>Job:</source>
        <translation>Job:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4149"/>
        <source>Schedule %1 and its other jobs stay as they are.</source>
        <translation>Schedule %1 и его остальные Jobs остаются без изменений.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4151"/>
        <source>Nothing was written: the move of job %1 was not confirmed.</source>
        <translation>Ничего не записано: перемещение Job %1 не подтверждено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4154"/>
        <source>Job %1 now runs at %2 on its own schedule %3. Written: %4.</source>
        <translation>Job %1 теперь запускается в %2 по собственному Schedule %3. Записано: %4.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3312"/>
        <source>Replays the queue with the jobs moved quarter hour by quarter hour and keeps the smallest moves that wait least</source>
        <translation>Проигрывает очередь со сдвигом Job по четвертям часа и оставляет наименьшие сдвиги с наименьшим ожиданием</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3514"/>
        <source>Stays: a Run line of its schedule runs at several hours, is not understood or would move into the next day</source>
        <translation>Остаётся: строка Run его Schedule выполняется в несколько часов, не распознана или перешла бы на следующий день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3518"/>
        <source>Moves along to run after %1, which has a higher priority</source>
        <translation>Сдвигается, чтобы выполняться после %1 с более высоким приоритетом</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3311"/>
        <source>Propose later start times</source>
        <translation>Предложить более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3300"/>
        <source>Jobs that are scheduled for the same moment and share a limit (a storage, a client, the job slots of the Director, the parallel writers of a device) wait for each other. The proposal starts some of them later, on a quarter hour (23:15, 23:30 ...), so that they wait less on the dates on display and have room to spare. A wait for a higher priority is meant to be and is not counted; a job of a lower priority that follows a moved job moves along, so that it still runs after it. With &quot;Constraints...&quot; a job can be kept where it is or kept from starting after a given time. Nothing is changed on the Director.</source>
        <translation>Jobs, запланированные на один и тот же момент и разделяющие лимит (Storage, Client, слоты Jobs Director, параллельные записи устройства), ждут друг друга. Предложение запускает некоторые из них позже, на четверть часа (23:15, 23:30 ...), чтобы в показанные дни они ждали меньше и имели запас. Ожидание из-за более высокого приоритета задумано и не учитывается; Job с более низким приоритетом, следующий за перемещённым Job, перемещается вместе с ним, чтобы по-прежнему выполняться после него. Кнопкой «Ограничения...» можно оставить Job на месте или запретить ему начинаться после заданного времени. На Director ничего не меняется.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3316"/>
        <source>Constraints...</source>
        <translation>Ограничения...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3317"/>
        <source>Jobs that must not move, or must not start after a given time; kept on this computer for this Director</source>
        <translation>Jobs, которые нельзя перемещать или нельзя начинать после заданного времени; хранится на этом компьютере для этого Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3326"/>
        <source>Apply...</source>
        <translation>Применить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Start now</source>
        <translation>Запуск сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Proposed start</source>
        <translation>Предлагаемый запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3333"/>
        <source>Later by</source>
        <translation>Позже на</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Wait now</source>
        <translation>Ожидание сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Wait then</source>
        <translation>Ожидание потом</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3334"/>
        <source>Note</source>
        <translation>Примечание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3336"/>
        <source>Time held by a limit or waiting for a device, summed over the job&apos;s runs on the dates on display</source>
        <translation>Время, которое Job удерживается лимитом или ждёт устройство, суммарно по его запускам в показанные дни</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3338"/>
        <source>Proposed later start times</source>
        <translation>Предлагаемое более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3392"/>
        <source>No proposal yet.</source>
        <translation>Предложения пока нет.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3407"/>
        <source>Creates a schedule of its own for every job that starts later and sets the job&apos;s Schedule to it; everything is shown once for confirmation first</source>
        <translation>Создаёт отдельный Schedule для каждого Job, который запускается позже, и переключает Schedule этого Job на него; сначала всё показывается один раз для подтверждения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3409"/>
        <source>Needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules, read and change jobs), which are not available on this connection</source>
        <translation>Требуются методы конфигурации gRPC-интерфейса Onesimus Director (создание Schedule, чтение и изменение Job), недоступные в этом подключении</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3445"/>
        <source>Nothing to stagger: the waits are not predicted (the jobs or the history are not loaded yet).</source>
        <translation>Нечего распределять: ожидания не предсказаны (Job или история ещё не загружены).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3463"/>
        <source>Nothing to stagger: no run on the dates on display waits for a limit or a device.</source>
        <translation>Нечего распределять: ни один запуск в показанные дни не ждёт лимит или устройство.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3466"/>
        <source>No later start helps: avoidable wait %1. See the notes of the jobs.</source>
        <translation>Более поздний запуск не помогает: устранимое ожидание %1. См. примечания к Job.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3469"/>
        <source>Avoidable wait on the dates on display: now %1, with the proposal %2. %n job(s) start later.</source>
        <translation>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускается позже.</numerusform>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускаются позже.</numerusform>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускаются позже.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3510"/>
        <source>Fixed by constraint</source>
        <translation>Закреплён ограничением</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3512"/>
        <source>Stays: latest start %1 by constraint</source>
        <translation>Остаётся: самое позднее начало %1 по ограничению</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3520"/>
        <source>A later start does not help</source>
        <translation>Более поздний запуск не помогает</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3523"/>
        <source>latest start %1 by constraint</source>
        <translation>самое позднее начало %1 по ограничению</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3530"/>
        <source>%n min</source>
        <translation>
            <numerusform>%n мин</numerusform>
            <numerusform>%n мин</numerusform>
            <numerusform>%n мин</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3576"/>
        <source>Apply the later start times</source>
        <translation>Применить более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3578"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3812"/>
        <source>These resources are written to the Director, in this order. The Director checks each one; the first that it refuses stops the rest.</source>
        <translation>Эти ресурсы записываются в Director в указанном порядке. Director проверяет каждый; первый отклонённый останавливает остальные.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3590"/>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3636"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3922"/>
        <source>Nothing was written: %1</source>
        <translation>Ничего не записано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3655"/>
        <source>Reading the jobs that start later...</source>
        <translation>Чтение Job, которые запускаются позже...</translation>
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
        <translation>Новый Schedule %1 для %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3686"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4073"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4134"/>
        <source>Nothing was written: the text of job %1 has no closing brace.</source>
        <translation>Ничего не записано: в тексте Job %1 нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3693"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="4147"/>
        <source>(Schedule from its JobDefs)</source>
        <translation>(Schedule из его JobDefs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3699"/>
        <source>Nothing was written: the later start times were not confirmed.</source>
        <translation>Ничего не записано: более позднее время запуска не подтверждено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3702"/>
        <source>Written: %1. The jobs start at their new times from now on.</source>
        <translation>Записано: %1. Теперь Job запускаются в новое время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3719"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3752"/>
        <source>Nothing was written: there is no connection to the configuration methods of the Director.</source>
        <translation>Ничего не записано: нет соединения с методами конфигурации Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3736"/>
        <source>Nothing was written: %1 %2 could not be read (%3).</source>
        <translation>Ничего не записано: не удалось прочитать %1 %2 (%3).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3768"/>
        <source>Writing %1 failed: the connection to the configuration methods is gone. Written before: %2. Nothing after it was sent.</source>
        <translation>Не удалось записать %1: соединение с методами конфигурации потеряно. Записано ранее: %2. Ничего после этого не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3774"/>
        <source>Writing %1...</source>
        <translation>Запись %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3781"/>
        <source>it was changed on the Director in the meantime</source>
        <translation>он был изменён в Director за это время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3783"/>
        <source>the name is taken</source>
        <translation>имя уже занято</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3770"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3792"/>
        <source>nothing</source>
        <translation>ничего</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3788"/>
        <source>it is still used by %1</source>
        <translation>он всё ещё используется %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3794"/>
        <source>Writing %1 failed: %2. Written before: %3. Nothing after it was sent.</source>
        <translation>Не удалось записать %1: %2. Записано ранее: %3. Дальше ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="476"/>
        <source>Predicted waits</source>
        <translation>Прогнозируемые ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="490"/>
        <source>Stagger</source>
        <translation>Распределить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="496"/>
        <source>Close the tabs below the timeline</source>
        <translation>Закрыть вкладки под шкалой времени</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2479"/>
        <source>Reasons shared by several rows</source>
        <translation>Причины, общие для нескольких строк</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Scheduled</source>
        <translation>Запланировано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2483"/>
        <source>Predicted start</source>
        <translation>Прогнозируемое начало</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Measured</source>
        <translation>Измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Certain</source>
        <translation>Точно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2484"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2486"/>
        <source>What this job waited at this level in the job history (StartTime minus SchedTime): median / maximum. The catalog&apos;s start is set after the connections and Client Run Before Job, so that time is part of it.</source>
        <translation>Сколько этот Job ждал на этом уровне по истории Jobs (StartTime минус SchedTime): медиана / максимум. Начало в каталоге фиксируется после подключений и Client Run Before Job, поэтому это время входит в него.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2490"/>
        <source>Predicted start minus scheduled time: seconds (&quot;27 s&quot;), minutes:seconds (&quot;1:35 min&quot;) or, from ten minutes on, hours:minutes (&quot;0:53 h&quot;)</source>
        <translation>Прогнозируемое начало минус запланированное время: секунды («27 s»), минуты:секунды («1:35 min») или, начиная с десяти минут, часы:минуты («0:53 h»)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2493"/>
        <source>No: the prediction depends on a limit the Director did not report or on a duration that was not measured</source>
        <translation>Нет: прогноз зависит от ограничения, о котором Director не сообщил, или от неизмеренной длительности</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2495"/>
        <source>Predicted waits of the dates on display</source>
        <translation>Прогнозируемые ожидания для отображаемых дат</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2497"/>
        <source>One row per run that is predicted to wait or whose prediction is uncertain. The reason of the current row is below the table.</source>
        <translation>По одной строке на каждый запуск, которому прогнозируется ожидание или прогноз для которого неточен. Причина для текущей строки показана под таблицей.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2520"/>
        <source>Reason of the selected wait</source>
        <translation>Причина выбранного ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2543"/>
        <source>Parallel writers of the devices</source>
        <translation>Параллельные записи устройств</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2545"/>
        <source>Notes</source>
        <translation>Примечания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2547"/>
        <source>Basis of the prediction</source>
        <translation>Основа прогноза</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2549"/>
        <source>Back-test</source>
        <translation>Ретроспективная проверка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2559"/>
        <source>Back-test of the prediction</source>
        <translation>Ретроспективная проверка прогноза</translation>
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
        <translation>Приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2585"/>
        <source>Queue: priority %1 first</source>
        <translation>Очередь: сначала приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2593"/>
        <source>The prediction has changed: propose again.</source>
        <translation>Предсказание изменилось: предложите заново.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2869"/>
        <source>Waiting not predicted</source>
        <translation>Ожидание не прогнозируется</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2877"/>
        <source>The jobs of the Director are not loaded: it is not known which job runs which schedule.</source>
        <translation>Jobs Director не загружены: неизвестно, какой Job использует какой Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2880"/>
        <source>Predicted for the dates on display from the limits of the Director (Maximum Concurrent Jobs, priorities) and the measured durations.</source>
        <translation>Прогноз для отображаемых дат на основе ограничений Director (Maximum Concurrent Jobs, приоритеты) и измеренных длительностей.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2883"/>
        <source>Uncertain: the prediction depends on a limit the Director did not report or on a duration that was not measured.</source>
        <translation>Неточно: прогноз зависит от ограничения, о котором Director не сообщил, или от неизмеренной длительности.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2887"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3101"/>
        <source>Opens or closes the list of predicted waits below the timeline.</source>
        <translation>Открывает или закрывает список прогнозируемых ожиданий под временной шкалой.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2905"/>
        <source>Not predicted: the jobs of the Director are not loaded.</source>
        <translation>Нет прогноза: Jobs Director не загружены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2907"/>
        <source>%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules from the day before on.</source>
        <translation>Отображается запусков: %1, смоделировано вместе с %2 запусками всех включённых Schedules начиная с предыдущего дня.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2916"/>
        <source>the storage of job %1 (the job names none)</source>
        <translation>Storage Job %1 (Job его не указывает)</translation>
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
        <translation>Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2946"/>
        <source>storages: %1</source>
        <translation>Storages: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2948"/>
        <source>clients: %1</source>
        <translation>Clients: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2950"/>
        <source>Maximum Concurrent Jobs as reported: %1.</source>
        <translation>Maximum Concurrent Jobs по данным Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2952"/>
        <source>Maximum Concurrent Jobs not reported by this Director for: %1. Runs it matters for are uncertain.</source>
        <translation>Этот Director не сообщил Maximum Concurrent Jobs для: %1. Запуски, для которых это важно, неточны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2976"/>
        <source>[%1] %2 rows: %3.</source>
        <translation>[%1] строк %2: %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3020"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3474"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3532"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3027"/>
        <source>not measured</source>
        <translation>не измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3029"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3029"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3034"/>
        <source>Prediction too low</source>
        <translation>Прогноз занижен</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3035"/>
        <source>Prediction too low; %1</source>
        <translation>Прогноз занижен; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3038"/>
        <source>Prediction too high</source>
        <translation>Прогноз завышен</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3039"/>
        <source>Prediction too high; %1</source>
        <translation>Прогноз завышен; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3043"/>
        <source>Waits %1 for the device inside the run</source>
        <translation>Ожидает устройство %1 внутри запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3057"/>
        <source>No run on display is predicted to wait.</source>
        <translation>Ни одному отображаемому запуску не прогнозируется ожидание.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3058"/>
        <source>Not predicted.</source>
        <translation>Нет прогноза.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3098"/>
        <source>Predicted waits: %1. The list is open.</source>
        <translation>Прогнозируемые ожидания: %1. Список открыт.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3099"/>
        <source>Predicted waits: %1. The list is closed.</source>
        <translation>Прогнозируемые ожидания: %1. Список закрыт.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3111"/>
        <source>Select a row to read its reason in full.</source>
        <translation>Выберите строку, чтобы прочитать причину полностью.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3119"/>
        <source>%1, scheduled %2:</source>
        <translation>%1, запланировано %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3200"/>
        <source>Storage %1, parallel writers of its device:</source>
        <translation>Storage %1, параллельные записи его устройства:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3207"/>
        <source>Parallel writers of the device of storage %1</source>
        <translation>Параллельные записи устройства Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3208"/>
        <source>How many jobs the device(s) of %1 write at a time: Maximum Concurrent Jobs of the Device resource in the storage daemon (several devices: their sum). The Director cannot read it. &quot;unknown&quot;: runs that depend on it are uncertain.</source>
        <translation>Сколько Jobs одновременно записывает устройство (устройства) %1: Maximum Concurrent Jobs ресурса Device в демоне Storage (несколько устройств: их сумма). Director не может это прочитать. «неизвестно»: зависящие от этого запуски неточны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3251"/>
        <source>No job history: no suggestion.</source>
        <translation>Нет истории Jobs: нет предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3253"/>
        <source>History suggests %1.</source>
        <translation>По истории предлагается %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3255"/>
        <source>History gives no suggestion.</source>
        <translation>История не даёт предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3261"/>
        <source>Use %1</source>
        <translation>Использовать %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3261"/>
        <source>Use</source>
        <translation>Использовать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3282"/>
        <source>Back-test: no job history to replay.</source>
        <translation>Ретроспективная проверка: нет истории Jobs для воспроизведения.</translation>
    </message>
</context>
<context>
    <name>BScheduleWizard</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="465"/>
        <source>Add Schedule</source>
        <translation>Добавить Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="517"/>
        <source>The schedule needs a name.</source>
        <translation>Schedule нужно имя.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="519"/>
        <source>The name is too long: at most 127 characters.</source>
        <translation>Имя слишком длинное: не более 127 символов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="523"/>
        <source>The name may contain letters, digits, blanks and _ . : - only.</source>
        <translation>Имя может содержать только буквы, цифры, пробелы и _ . : -.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="525"/>
        <source>A schedule named %1 exists already.</source>
        <translation>Schedule с именем %1 уже существует.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="536"/>
        <source>The schedule has no runs yet.</source>
        <translation>У Schedule ещё нет запусков.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="546"/>
        <source>The schedule has no runs yet. Paint at least one run on the grid.</source>
        <translation>У Schedule ещё нет запусков. Нарисуйте на сетке хотя бы один запуск.</translation>
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
        <translation>По умолчанию</translation>
    </message>
</context>
<context>
    <name>BSettingsDialog</name>
    <message>
        <location filename="../ui/bsettingsdialog.ui" line="14"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="67"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1149"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1180"/>
        <source>Reset</source>
        <translation>Сбросить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="109"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="146"/>
        <source>🔌 Connection</source>
        <translation>🔌 Подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="150"/>
        <source>🎨 Appearance</source>
        <translation>🎨 Внешний вид</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="154"/>
        <source>⚙️ Behavior</source>
        <translation>⚙️ Поведение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="158"/>
        <source>🔧 Advanced</source>
        <translation>🔧 Дополнительно</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="187"/>
        <source>Connection Manager</source>
        <translation>Диспетчер подключений</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="195"/>
        <source>Saved Connections</source>
        <translation>Сохранённые подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="210"/>
        <source>+ Add</source>
        <translation>+ Добавить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="212"/>
        <source>Add a new connection profile</source>
        <translation>Добавить новый профиль подключения</translation>
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
        <translation>Дублировать</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="215"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="243"/>
        <source>Profile</source>
        <translation>Профиль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="250"/>
        <source>e.g. Production Server, Test Environment</source>
        <translation>например, Рабочий сервер, Тестовая среда</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="251"/>
        <source>Profile Name:</source>
        <translation>Имя профиля:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="256"/>
        <source>Director Connection</source>
        <translation>Подключение к Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="263"/>
        <source>e.g. 192.168.1.100 or bareos-dir.local</source>
        <translation>например, 192.168.1.100 или bareos-dir.local</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="264"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="269"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="273"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="274"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="280"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="282"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="287"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Порт gRPC Director (по умолчанию: порт BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="288"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="295"/>
        <source>Forget certificate</source>
        <translation>Забыть сертификат</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="296"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Удалить закреплённый сертификат. При следующем подключении снова будет задан вопрос, доверять ли сертификату сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="299"/>
        <source>Server certificate:</source>
        <translation>Сертификат сервера:</translation>
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
        <translation>Операционная система сервера (влияет на пути TLS в экспортируемых конфигурациях)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="312"/>
        <source>Server OS:</source>
        <translation>ОС сервера:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="316"/>
        <source>Director Name:</source>
        <translation>Имя Director:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="320"/>
        <source>Console name for authentication (e.g., &apos;admin&apos; or &apos;onesimus&apos;)</source>
        <translation>Имя консоли для аутентификации (например, &apos;admin&apos; или &apos;onesimus&apos;)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="321"/>
        <source>Console Name:</source>
        <translation>Имя консоли:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="326"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="333"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="339"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1485"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(здесь появится MD5-хеш пароля)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="334"/>
        <source>MD5 Hash:</source>
        <translation>MD5-хеш:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="374"/>
        <source>⚠️ TLS encrypts communication with the Director.
TLS is strongly recommended for production environments!</source>
        <translation>⚠️ TLS шифрует обмен данными с Director.
В рабочих средах настоятельно рекомендуется использовать TLS!</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="382"/>
        <source>Authentication &amp; Encryption</source>
        <translation>Аутентификация и шифрование</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="388"/>
        <source>Authentication Method:</source>
        <translation>Метод аутентификации:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="394"/>
        <source>Use x509 certificates (instead of PSK)</source>
        <translation>Использовать сертификаты x509 (вместо PSK)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="396"/>
        <source>By default, TLS uses Pre-Shared Key (PSK) authentication.
Enable this to use X.509 certificate-based authentication instead.
Certificate mode requires CA certificate and client certificate/key files.</source>
        <translation>По умолчанию TLS использует аутентификацию по общему ключу (PSK).
Включите этот параметр, чтобы использовать аутентификацию на основе сертификатов X.509.
Для режима сертификатов нужны файлы сертификата CA и сертификата/ключа клиента.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="414"/>
        <source>Path to CA certificate (.pem)</source>
        <translation>Путь к сертификату CA (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="415"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="425"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="435"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="446"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="420"/>
        <source>CA Certificate:</source>
        <translation>Сертификат CA:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="424"/>
        <source>Path to client certificate (.pem)</source>
        <translation>Путь к сертификату клиента (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="430"/>
        <source>Client Certificate:</source>
        <translation>Сертификат клиента:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="434"/>
        <source>Path to private key (.pem, .key)</source>
        <translation>Путь к закрытому ключу (.pem, .key)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="440"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="445"/>
        <source>Path to client certificate (.pfx)</source>
        <translation>Путь к сертификату клиента (.pfx)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="451"/>
        <source>PFX Certificate:</source>
        <translation>Сертификат PFX:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="454"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Преобразовать PEM/DER в PFX...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="455"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Преобразовать отдельные файлы сертификата и ключа PEM/DER в файл PFX</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="460"/>
        <source>Verify server certificate (recommended)</source>
        <translation>Проверять сертификат сервера (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="469"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Список шифров TLS (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="475"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>например, PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="476"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>Список шифров TLS-PSK через двоеточие.
Оставьте пустым для автоопределения.
Пример: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="481"/>
        <source>Preset</source>
        <translation>Предустановка</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="482"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Заполнить рекомендуемыми шифрами PSK</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="493"/>
        <source>gRPC is always encrypted with TLS. The server certificate is confirmed on first use and pinned; the Director authenticates with console name and password. TLS-PSK and certificate settings do not apply.</source>
        <translation>gRPC всегда шифруется с помощью TLS. Сертификат сервера подтверждается при первом использовании и закрепляется; Director выполняет аутентификацию по имени консоли и паролю. Настройки TLS-PSK и сертификатов не применяются.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="504"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="509"/>
        <source>Save password in profile</source>
        <translation>Сохранять пароль в профиле</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="513"/>
        <source>Auto-connect on startup</source>
        <translation>Подключаться автоматически при запуске</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="591"/>
        <source>Theme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="596"/>
        <source>System</source>
        <translation type="unfinished">Система</translation>
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
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="529"/>
        <source>Export All Profiles</source>
        <translation>Экспортировать все профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="530"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1286"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1294"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1297"/>
        <source>Import Profiles</source>
        <translation>Импортировать профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="569"/>
        <source>Appearance</source>
        <translation>Внешний вид</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="575"/>
        <source>Text Size</source>
        <translation>Размер текста</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="584"/>
        <source>The text follows the font of your desktop. This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0).</source>
        <translation>Текст использует шрифт вашего рабочего стола. Этот коэффициент увеличивает или уменьшает его (Ctrl++, Ctrl+-, Ctrl+0).</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="586"/>
        <source>Interface zoom:</source>
        <translation>Масштаб интерфейса:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="604"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="612"/>
        <source>System (%1)</source>
        <translation>Системный (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="624"/>
        <source>Application Language:</source>
        <translation>Язык приложения:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="627"/>
        <source>Language changes require an application restart. &quot;System&quot; follows the language of the operating system.</source>
        <translation>Для смены языка требуется перезапуск приложения. «Системный» следует языку операционной системы.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="637"/>
        <source>Status Bar Colors</source>
        <translation>Цвета строки состояния</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="646"/>
        <source>Color shown when connected to Director</source>
        <translation>Цвет при подключении к Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="648"/>
        <source>Connected:</source>
        <translation>Подключено:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="654"/>
        <source>Color shown when not connected</source>
        <translation>Цвет при отсутствии подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="656"/>
        <source>Disconnected:</source>
        <translation>Отключено:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="661"/>
        <source>Backup Level Colors</source>
        <translation>Цвета уровней резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="725"/>
        <source>Behavior</source>
        <translation>Поведение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="731"/>
        <source>Confirmations</source>
        <translation>Подтверждения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="735"/>
        <source>Confirm job cancellation</source>
        <translation>Подтверждать отмену Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="740"/>
        <source>Confirm job start</source>
        <translation>Подтверждать запуск Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="747"/>
        <source>Auto-Refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="751"/>
        <source>Auto-refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="755"/>
        <source>Interval:</source>
        <translation>Интервал:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="768"/>
        <source>Display</source>
        <translation>Отображение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="776"/>
        <source>Maximum Jobs:</source>
        <translation>Максимум Jobs:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="778"/>
        <source>Show newest jobs first</source>
        <translation>Сначала новые Jobs</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="781"/>
        <source>When enabled, the job list shows the most recent jobs first.
When disabled, the oldest jobs are shown first.</source>
        <translation>Если включено, список Jobs показывает сначала самые новые Jobs.
Если выключено, сначала показываются самые старые Jobs.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="788"/>
        <source> hours</source>
        <translation> ч</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="790"/>
        <source>The clients table marks a client as stale when its last backup or contact
is at least this old. The value is taken from the job history; it is not
a live connection check.</source>
        <translation>Таблица Clients помечает Client как устаревший, если его последнее резервное копирование
или контакт был не менее этого срока назад. Значение берётся из истории Jobs; это не
проверка подключения в реальном времени.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="793"/>
        <source>Client backup is stale after:</source>
        <translation>Резервная копия Client устаревает через:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="798"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="805"/>
        <source> days</source>
        <translation> дн.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="807"/>
        <source>The schedule view measures how long jobs run and how long they wait
from the jobs of the last days in the catalog. A longer time gives more
runs per job and a larger request to the Director. The history is only
requested while the schedule view is open.</source>
        <translation>Представление Schedules измеряет, сколько Jobs выполняются и сколько ждут,
по Jobs за последние дни в каталоге. Более длинный период даёт больше
запусков на Job и более объёмный запрос к Director. История запрашивается
только пока открыто представление Schedules.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="811"/>
        <source>Measure job history over the last:</source>
        <translation>Измерять историю Jobs за последние:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="816"/>
        <source>Visible Backup Levels</source>
        <translation>Видимые уровни резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="821"/>
        <source>Select which backup levels appear in the Run Backup dialog:</source>
        <translation>Выберите, какие уровни резервного копирования отображаются в диалоге запуска Backup:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="827"/>
        <source>Complete backup of all files</source>
        <translation>Полное резервное копирование всех файлов</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="828"/>
        <source>Backup files changed since last backup</source>
        <translation>Резервное копирование файлов, изменённых с последнего резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="829"/>
        <source>Backup files changed since last Full backup</source>
        <translation>Резервное копирование файлов, изменённых с последнего Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="830"/>
        <source>Consolidate incremental backups into a synthetic full</source>
        <translation>Объединить инкрементальные резервные копии в синтетическую полную</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="831"/>
        <source>Data-level backup (rarely used)</source>
        <translation>Резервное копирование на уровне данных (используется редко)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="879"/>
        <source>Advanced Settings</source>
        <translation>Дополнительные настройки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="886"/>
        <source>File Logging</source>
        <translation>Журналирование в файл</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="890"/>
        <source>Enable file logging</source>
        <translation>Включить журналирование в файл</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="891"/>
        <source>Write debug information to a log file</source>
        <translation>Записывать отладочную информацию в файл журнала</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="895"/>
        <source>Log File:</source>
        <translation>Файл журнала:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="905"/>
        <source>Max. Log Size:</source>
        <translation>Макс. размер журнала:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="929"/>
        <source>Miscellaneous</source>
        <translation>Разное</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="934"/>
        <source>Show tooltips</source>
        <translation>Показывать подсказки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="942"/>
        <source>⚠️ These settings are for advanced users.
Changes may affect application stability.</source>
        <translation>⚠️ Эти настройки предназначены для опытных пользователей.
Изменения могут повлиять на стабильность приложения.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1150"/>
        <source>Settings have been saved.
Some changes require an application restart.</source>
        <translation>Настройки сохранены.
Для некоторых изменений требуется перезапуск приложения.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1203"/>
        <source>Select CA Certificate</source>
        <translation>Выбор сертификата CA</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1204"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1216"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1212"/>
        <source>Select Client Certificate</source>
        <translation>Выбор сертификата клиента</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1214"/>
        <source>Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)</source>
        <translation>Сертификаты (*.pfx *.pem *.crt *.cert *.der);;Файлы PFX (*.pfx);;Файлы PEM (*.pem);;Файлы DER (*.der);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1225"/>
        <source>Select Private Key</source>
        <translation>Выбор закрытого ключа</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1226"/>
        <source>Keys (*.pem *.key);;All Files (*)</source>
        <translation>Ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1336"/>
        <source>No Profiles</source>
        <translation>Нет профилей</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1337"/>
        <source>There are no connection profiles to delete.</source>
        <translation>Нет профилей подключения для удаления.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1341"/>
        <source>Clear All Profiles</source>
        <translation>Удалить все профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1342"/>
        <source>Do you really want to delete all %1 connection profile(s)?

This includes all passwords and certificate paths.
This action cannot be undone.</source>
        <translation>Вы действительно хотите удалить все профили подключения (%1)?

Это включает все пароли и пути к сертификатам.
Это действие нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1361"/>
        <source>Deleted</source>
        <translation>Удалено</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1386"/>
        <source>Choose Color for Full Backup</source>
        <translation>Выбор цвета для Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1391"/>
        <source>Choose Color for Incremental Backup</source>
        <translation>Выбор цвета для Incremental</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1396"/>
        <source>Choose Color for Differential Backup</source>
        <translation>Выбор цвета для Differential</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1401"/>
        <source>Choose Color for Virtual Full Backup</source>
        <translation>Выбор цвета для VirtualFull</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1407"/>
        <source>Choose Color for Connected Status</source>
        <translation>Выбор цвета для состояния «Подключено»</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1417"/>
        <source>Choose Color for Disconnected Status</source>
        <translation>Выбор цвета для состояния «Отключено»</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1477"/>
        <source>••••••••  (password saved)</source>
        <translation>••••••••  (пароль сохранён)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1483"/>
        <source>Enter password</source>
        <translation>Введите пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1552"/>
        <source>New Connection</source>
        <translation>Новое подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1581"/>
        <source>Delete Profile</source>
        <translation>Удалить профиль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1612"/>
        <source>%1 (Copy)</source>
        <translation>%1 (копия)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1627"/>
        <source>Unnamed Connection</source>
        <translation>Безымянное подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1699"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(не закреплён — будет подтверждён при следующем подключении)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1704"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1719"/>
        <source>PFX File Created</source>
        <translation>Файл PFX создан</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1720"/>
        <source>PFX file has been created successfully.

The certificate path has been updated in the current profile.</source>
        <translation>Файл PFX успешно создан.

Путь к сертификату обновлён в текущем профиле.</translation>
    </message>
</context>
<context>
    <name>BStaggerConstraintsDialog</name>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="21"/>
        <source>Constraints for the proposal</source>
        <translation>Ограничения для предложения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="24"/>
        <source>The proposal only starts jobs later. A fixed job is never moved, and a job of a higher priority is then not moved past it either. A job with a latest start does not start after that time. The constraints are kept on this computer, for this Director.</source>
        <translation>Предложение только запускает Jobs позже. Закреплённый Job никогда не перемещается, и тогда Job с более высоким приоритетом тоже не перемещается за него. Job с самым поздним началом не начинается после этого времени. Ограничения хранятся на этом компьютере для этого Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="36"/>
        <source>&lt;b&gt;Job&lt;/b&gt;</source>
        <translation>&lt;b&gt;Job&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="37"/>
        <source>&lt;b&gt;Fixed&lt;/b&gt;</source>
        <translation>&lt;b&gt;Закреплён&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="38"/>
        <source>&lt;b&gt;Latest start&lt;/b&gt;</source>
        <translation>&lt;b&gt;Самое позднее начало&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="41"/>
        <source>No job waits on the dates on display.</source>
        <translation>В показанные дни ни один Job не ждёт.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="51"/>
        <source>%1 is fixed</source>
        <translation>%1 закреплён</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="53"/>
        <source>not after</source>
        <translation>не позже</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="55"/>
        <source>%1 has a latest start</source>
        <translation>У %1 есть самое позднее начало</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerconstraintsdialog.cpp" line="60"/>
        <source>Latest start of %1</source>
        <translation>Самое позднее начало %1</translation>
    </message>
</context>
<context>
    <name>BStaggerPlan</name>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="37"/>
        <source>Schedule %1 has no Run lines.</source>
        <translation>У Schedule %1 нет строк Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="41"/>
        <source>Run line %1 of schedule %2 is not understood.</source>
        <translation>Строка Run %1 в Schedule %2 не распознана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="44"/>
        <source>Run line %1 of schedule %2 runs at several hours.</source>
        <translation>Строка Run %1 в Schedule %2 выполняется в несколько часов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="47"/>
        <source>Run line %1 of schedule %2 would move into the next day.</source>
        <translation>Строка Run %1 в Schedule %2 перешла бы на следующий день.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="57"/>
        <source>Run line %1 of schedule %2 did not read back as written (%3).</source>
        <translation>Строка Run %1 в Schedule %2 не прочиталась так, как была записана (%3).</translation>
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
        <translation>Завершено нормально</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Warnings</source>
        <translation>Предупреждения</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Terminated with warnings</source>
        <translation>Завершено с предупреждениями</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Terminated in error</source>
        <translation>Завершено с ошибкой</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Errors</source>
        <translation>Ошибки</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Non-fatal error</source>
        <translation>Некритическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Fatal</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Fatal error</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Canceled</source>
        <translation>Отменено</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Canceled by user</source>
        <translation>Отменено пользователем</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38"/>
        <source>Running</source>
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Committing</source>
        <translation>Фиксация</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Committing data (last despool)</source>
        <translation>Фиксация данных (последний despool)</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Despooling</source>
        <translation>Despooling</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Despooling data</source>
        <translation>Выгрузка данных из спула</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Created</source>
        <translation>Создано</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Created, not yet running</source>
        <translation>Создано, ещё не выполняется</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Wait FD</source>
        <translation>Ожидание FD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Waiting on the File daemon</source>
        <translation>Ожидание File daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Wait SD</source>
        <translation>Ожидание SD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Waiting on the Storage daemon</source>
        <translation>Ожидание Storage daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Wait media</source>
        <translation>Ожидание носителя</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Waiting for new media</source>
        <translation>Ожидание нового носителя</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Wait mount</source>
        <translation>Ожидание монтирования</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Waiting for mount</source>
        <translation>Ожидание монтирования</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Wait storage</source>
        <translation>Ожидание Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Waiting for a storage resource</source>
        <translation>Ожидание ресурса Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Wait job</source>
        <translation>Ожидание Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Waiting for a job resource</source>
        <translation>Ожидание ресурса Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48"/>
        <source>Wait client</source>
        <translation>Ожидание Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48"/>
        <source>Waiting for a client resource</source>
        <translation>Ожидание ресурса Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Wait max jobs</source>
        <translation>Ожидание max jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Waiting for maximum jobs</source>
        <translation>Ожидание из-за максимума Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Wait start</source>
        <translation>Ожидание старта</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Waiting for the start time</source>
        <translation>Ожидание времени запуска</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="51"/>
        <source>Wait priority</source>
        <translation>Ожидание приоритета</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="51"/>
        <source>Waiting for higher priority jobs to finish</source>
        <translation>Ожидание завершения Jobs с более высоким приоритетом</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="52"/>
        <source>Wait device</source>
        <translation>Ожидание устройства</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="52"/>
        <source>Queued, waiting for a device</source>
        <translation>В очереди, ожидание устройства</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="53"/>
        <source>Despool attr.</source>
        <translation>Despool атриб.</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="53"/>
        <source>Storage daemon despooling attributes</source>
        <translation>Storage daemon выгружает атрибуты из спула</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="54"/>
        <source>Inserting</source>
        <translation>Вставка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="54"/>
        <source>Doing batch insert of file records</source>
        <translation>Пакетная вставка записей файлов</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="55"/>
        <source>Blocked</source>
        <translation>Заблокировано</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="56"/>
        <source>Differences</source>
        <translation>Различия</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="56"/>
        <source>Verify found differences</source>
        <translation>Verify обнаружил различия</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="57"/>
        <source>Incomplete</source>
        <translation>Не завершено</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="57"/>
        <source>Incomplete job</source>
        <translation>Незавершённый Job</translation>
    </message>
</context>
<context>
    <name>BStorageResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="322"/>
        <source>Port</source>
        <translation>Порт</translation>
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
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="218"/>
        <source>No</source>
        <translation>Нет</translation>
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
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BVolumePurge</name>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="77"/>
        <source>The Director refused the command without a message.</source>
        <translation>Director отклонил команду без сообщения.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="108"/>
        <source>Choose either one pool or all pools, not both.</source>
        <translation>Выберите либо один Pool, либо все Pools, но не оба варианта.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="112"/>
        <source>Choose a pool or all pools.</source>
        <translation>Выберите Pool или все Pools.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="118"/>
        <source>The pool name contains characters that cannot be sent on a command line.</source>
        <translation>Имя Pool содержит символы, которые нельзя передать в командной строке.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="124"/>
        <source>Choose at least one volume status.</source>
        <translation>Выберите хотя бы один статус Volume.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="129"/>
        <source>Unknown volume status.</source>
        <translation>Неизвестный статус Volume.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="165"/>
        <source>The Director sent an empty answer.</source>
        <translation>Director прислал пустой ответ.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="167"/>
        <source>The Director did not answer with a purge result: %1</source>
        <translation>Director не ответил результатом purge: %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="182"/>
        <source>The Director&apos;s answer has no purge result: %1</source>
        <translation>Ответ Director не содержит результата purge: %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurge.cpp" line="231"/>
        <source>The Director&apos;s purge result names no mode.</source>
        <translation>В результате purge от Director не указан режим.</translation>
    </message>
</context>
<context>
    <name>BVolumePurgePanel</name>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="30"/>
        <source>Purge volumes</source>
        <translation>Purge для Volumes</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="36"/>
        <source>Purge removes all job and file records of the chosen volumes from the catalog, whatever their retention periods say. Click &apos;Preview&apos; to see what the Director would purge.</source>
        <translation>Purge удаляет из каталога все записи о Jobs и файлах выбранных Volumes, независимо от их сроков хранения. Нажмите «Предпросмотр», чтобы увидеть, для чего Director выполнил бы purge.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="43"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="46"/>
        <source>All pools</source>
        <translation>Все Pools</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="52"/>
        <source>Volume status:</source>
        <translation>Статус Volume:</translation>
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
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="69"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="70"/>
        <source>Result</source>
        <translation>Результат</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="84"/>
        <source>Preview</source>
        <translation>Предпросмотр</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="86"/>
        <source>Ask the Director which volumes it would purge; nothing is changed</source>
        <translation>Спрашивает Director, для каких Volumes он выполнил бы purge; ничего не изменяется</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="89"/>
        <source>Purge</source>
        <translation>Purge</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="91"/>
        <source>Purge the volumes of the preview (requires a preview first)</source>
        <translation>Выполняет purge для Volumes из предпросмотра (сначала нужен предпросмотр)</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="172"/>
        <source>The selection was changed. Click &apos;Preview&apos; again.</source>
        <translation>Выбор изменён. Нажмите «Предпросмотр» ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="208"/>
        <source>all pools</source>
        <translation>все Pools</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="209"/>
        <source>pool &apos;%1&apos;</source>
        <translation>Pool «%1»</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bvolumepurgepanel.cpp" line="210"/>
        <source>Purge %n volume(s) in %1 with the status %2?</source>
        <translation>
            <numerusform>Выполнить purge для %n Volume в %1 со статусом %2?</numerusform>
            <numerusform>Выполнить purge для %n Volumes в %1 со статусом %2?</numerusform>
            <numerusform>Выполнить purge для %n Volumes в %1 со статусом %2?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="213"/>
        <source>Purge ignores the retention periods. All job and file records on these volumes are removed from the catalog, and their backups can no longer be restored through the catalog. This cannot be undone.</source>
        <translation>Purge игнорирует сроки хранения. Все записи о Jobs и файлах на этих Volumes удаляются из каталога, и их резервные копии больше нельзя восстановить через каталог. Это нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="217"/>
        <source>The Director selects the volumes again when it purges; volumes that changed since the preview are purged or skipped according to their state then.</source>
        <translation>При выполнении purge Director выбирает Volumes заново; Volumes, изменившиеся после предпросмотра, обрабатываются или пропускаются в зависимости от их состояния на тот момент.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="220"/>
        <source>Command: %1</source>
        <translation>Команда: %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="222"/>
        <source>Confirm Purge</source>
        <translation>Подтвердить purge</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="238"/>
        <source>Not connected to a Director.</source>
        <translation>Нет подключения к Director.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="244"/>
        <source>Asking the Director for a preview...</source>
        <translation>Запрос предпросмотра у Director...</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="245"/>
        <source>The Director is purging...</source>
        <translation>Director выполняет purge...</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="274"/>
        <source>The Director answered in another mode than the one requested.</source>
        <translation>Director ответил в другом режиме, чем был запрошен.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="282"/>
        <location filename="../src/bvolumepurgepanel.cpp" line="320"/>
        <source>Preview failed: %1</source>
        <translation>Предпросмотр не удался: %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="283"/>
        <source>Purge failed: %1</source>
        <translation>Purge не удался: %1</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="294"/>
        <source>Preview: %1 volumes selected, %2 would be purged, %3 skipped. Click &apos;Purge&apos; to purge them.</source>
        <translation>Предпросмотр: выбрано Volumes: %1, purge для %2, пропущено %3. Нажмите «Purge», чтобы выполнить purge.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="299"/>
        <source>Preview: %1 volumes selected, none would be purged, %2 skipped.</source>
        <translation>Предпросмотр: выбрано Volumes: %1, purge ни для одного, пропущено %2.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="305"/>
        <source>Purge finished: %1 volumes selected, %2 purged, %3 skipped.</source>
        <translation>Purge завершён: выбрано Volumes: %1, purge выполнен для %2, пропущено %3.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="321"/>
        <source>The result of the purge is unknown: %1. Run a preview to see the current state.</source>
        <translation>Результат purge неизвестен: %1. Выполните предпросмотр, чтобы увидеть текущее состояние.</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="329"/>
        <source>no answer from the Director within the time limit</source>
        <translation>нет ответа от Director в отведённое время</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="347"/>
        <source>Would purge</source>
        <translation>Будет purge</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="350"/>
        <source>Purged</source>
        <translation>Purge выполнен</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="353"/>
        <source>Skipped</source>
        <translation>Пропущен</translation>
    </message>
    <message>
        <location filename="../src/bvolumepurgepanel.cpp" line="354"/>
        <source>Skipped: %1</source>
        <translation>Пропущен: %1</translation>
    </message>
</context>
<context>
    <name>BWaitCalibration</name>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="197"/>
        <source>this job was not measured: median of all %1 runs on this Director</source>
        <translation>этот Job не измерялся: медиана всех %1 запусков в этом Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="202"/>
        <source>nothing measured: %1 assumed</source>
        <translation>ничего не измерено: принято %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="210"/>
        <source>Duration %1: median of %2 measured run(s). It includes any wait for a device inside the storage daemon (the parallel writers of %3 are not known, so it is not taken out).</source>
        <translation>Длительность %1: медиана %2 измеренных запусков. Она включает любое ожидание устройства внутри демона Storage (параллельные записи %3 неизвестны, поэтому оно не вычитается).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="214"/>
        <source>its storage</source>
        <translation>его Storage</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="193"/>
        <source>median of %n measured run(s) at this level</source>
        <translation>
            <numerusform>медиана %n измеренного запуска на этом уровне</numerusform>
            <numerusform>медиана %n измеренных запусков на этом уровне</numerusform>
            <numerusform>медиана %n измеренных запусков на этом уровне</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="195"/>
        <source>median of %n measured run(s) of this job at other levels</source>
        <translation>
            <numerusform>медиана %n измеренного запуска этого Job на других уровнях</numerusform>
            <numerusform>медиана %n измеренных запусков этого Job на других уровнях</numerusform>
            <numerusform>медиана %n измеренных запусков этого Job на других уровнях</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="217"/>
        <source>Duration %1: median of %n measured run(s); they did not wait for the device.</source>
        <translation>
            <numerusform>Длительность %1: медиана %n измеренного запуска; он не ждал устройство.</numerusform>
            <numerusform>Длительность %1: медиана %n измеренных запусков; они не ждали устройство.</numerusform>
            <numerusform>Длительность %1: медиана %n измеренных запусков; они не ждали устройство.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="221"/>
        <source>Duration %1 without the wait for the device (measured %2, median of %3 run(s)): counted from the end of the run that held the device before it (%4 parallel writer(s)). It may still hold up to 30 s of the storage daemon&apos;s tries.</source>
        <translation>Длительность %1 без ожидания устройства (измерено %2, медиана %3 запусков): отсчитывается от окончания запуска, который занимал устройство до него (параллельных записей: %4). Она всё ещё может содержать до 30 с попыток демона Storage.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="317"/>
        <source>%1 ended %2 after %3 on %4; alone it takes at least %5</source>
        <translation>%1 завершился через %2 после %3 %4; в одиночку он занимает не менее %5</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="334"/>
        <source>%1 night(s) with runs on storage %2; on %3 more than one of them was queued or running at the same time; on %4 the runs&apos; recorded start to end overlapped.</source>
        <translation>Ночей с запусками на Storage %2: %1; в %3 из них одновременно в очереди или в работе было более одного запуска; в %4 зафиксированные интервалы от начала до конца перекрывались.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="341"/>
        <source>No two runs ever overlapped although several were queued: the storage wrote one job at a time.</source>
        <translation>Никакие два запуска ни разу не перекрывались, хотя в очереди было несколько: Storage записывал по одному Job за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="347"/>
        <source>Overlapping runs do not prove parallel writing: with Just In Time Reservation a job waits for the device inside its run.</source>
        <translation>Перекрывающиеся запуски не доказывают параллельную запись: при Just In Time Reservation Job ожидает устройство внутри своего запуска.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="351"/>
        <source>%1 of %2 telling pair(s) ended sooner after each other than one writer allows (e.g. %3): it writes more than one job at a time.</source>
        <translation>%1 из %2 показательных пар завершились друг за другом быстрее, чем позволяет одна запись (например, %3): он записывает более одного Job за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="358"/>
        <source>In all %1 telling pair(s) on %2 night(s) the later run went on after the other ended for at least as long as it takes alone (e.g. %3): consistent with one writer at a time.</source>
        <translation>Во всех %1 показательных парах за %2 ноч. более поздний запуск продолжался после окончания другого не меньше, чем длится в одиночку (например, %3): это согласуется с одной записью за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="365"/>
        <source>Not enough telling pairs (%1; a pair tells when the later run started well before the other ended and was also seen running alone): no suggestion.</source>
        <translation>Недостаточно показательных пар (%1; пара показательна, если более поздний запуск начался задолго до окончания другого и также наблюдался в одиночку): нет предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453"/>
        <source>Back-test: no measured night to replay.</source>
        <translation>Ретроспективная проверка: нет измеренной ночи для воспроизведения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="454"/>
        <source>Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 (%5, %6; %7).</source>
        <translation>Ретроспективная проверка, последние ночи: %1 (запусков: %2): медианная ошибка %3, наихудшая %4 (%5, %6; %7).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="460"/>
        <source>predicted too late</source>
        <translation>прогноз слишком поздний</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="460"/>
        <source>predicted too early</source>
        <translation>прогноз слишком ранний</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="462"/>
        <source>Replayed without a device limit for %1 (not set).</source>
        <translation>Воспроизведено без ограничения устройства для %1 (не задано).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="465"/>
        <source>Durations and overheads come from the same history.</source>
        <translation>Длительности и накладные расходы взяты из той же истории.</translation>
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
        <translation>Запуск аутентификации Bareos...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="168"/>
        <source>Setting up TLS-PSK...</source>
        <translation>Настройка TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="187"/>
        <source>Starting TLS-PSK handshake...</source>
        <translation>Запуск рукопожатия TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="194"/>
        <source>Setting up certificate-based TLS...</source>
        <translation>Настройка TLS на основе сертификатов...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="210"/>
        <source>Starting TLS handshake...</source>
        <translation>Запуск рукопожатия TLS...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="218"/>
        <source>Starting legacy authentication (no TLS)...</source>
        <translation>Запуск устаревшей аутентификации (без TLS)...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="654"/>
        <source>TLS established, sending Hello packet...</source>
        <translation>TLS установлен, отправка пакета Hello...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="678"/>
        <source>Waiting for Director challenge...</source>
        <translation>Ожидание challenge от Director...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="844"/>
        <source>Director rejected connection: %1</source>
        <translation>Director отклонил подключение: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="851"/>
        <source>

Hint: The Director requires TLS encryption. Please use PSK or Certificate authentication instead of Legacy mode.</source>
        <translation>

Подсказка: Director требует шифрования TLS. Используйте аутентификацию PSK или по сертификату вместо режима Legacy.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="857"/>
        <source>Director sent empty response.

This usually means the Director requires TLS encryption but Legacy (unencrypted) mode was used.
Please try PSK or Certificate authentication.</source>
        <translation>Director прислал пустой ответ.

Обычно это означает, что Director требует шифрования TLS, а использовался режим Legacy (без шифрования).
Попробуйте аутентификацию PSK или по сертификату.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="865"/>
        <source>Unexpected response from Director:
%1</source>
        <translation>Неожиданный ответ от Director:
%1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="952"/>
        <source>CRAM-MD5 response sent successfully</source>
        <translation>Ответ CRAM-MD5 успешно отправлен</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1351"/>
        <source>The Director announced a message of %1 bytes during authentication (limit: %2 bytes). Connection closed.</source>
        <translation>Во время аутентификации Director объявил сообщение размером %1 байт (лимит: %2 байт). Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1567"/>
        <source>Connected</source>
        <translation>Подключено</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1748"/>
        <source>TLS-PSK authentication failed: %1

This usually means:
• Wrong password
• Wrong console name
• Console not configured on Director
• TLS-PSK not enabled on Director</source>
        <translation>Ошибка аутентификации TLS-PSK: %1

Обычно это означает:
• Неверный пароль
• Неверное имя консоли
• Консоль не настроена в Director
• TLS-PSK не включён в Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1757"/>
        <source>TLS handshake failed: %1

Possible causes:
• No compatible cipher suites
• TLS version mismatch
• Director doesn&apos;t support PSK</source>
        <translation>Ошибка согласования TLS: %1

Возможные причины:
• Нет совместимых наборов шифров
• Несовпадение версий TLS
• Director не поддерживает PSK</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1765"/>
        <source>Socket error: %1</source>
        <translation>Ошибка сокета: %1</translation>
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
        <translation type="unfinished">нет</translation>
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
        <translation>Для изменения конфигурации нужен интерфейс gRPC Onesimus Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="219"/>
        <source>Checking a client needs the gRPC interface of the Onesimus Director</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="273"/>
        <source>Connection to the Director was closed before the reply arrived</source>
        <translation>Подключение к Director было закрыто до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="330"/>
        <source>Disconnected from the Director before the reply arrived</source>
        <translation>Отключено от Director до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="525"/>
        <source>No response from Director within %1 seconds</source>
        <translation>Нет ответа от Director в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="535"/>
        <source>Timeout while loading resources: %1</source>
        <translation>Тайм-аут при загрузке ресурсов: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="979"/>
        <source>Connection to the Director was lost before the reply arrived</source>
        <translation>Подключение к Director было потеряно до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1130"/>
        <source>The Director did not confirm the API mode within %1 seconds. The connection was closed.</source>
        <translation>Director не подтвердил режим API в течение %1 с. Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1192"/>
        <source>The Director announced a message of %1 bytes (limit: %2 MiB). The connection was closed.</source>
        <translation>Director объявил сообщение размером %1 байт (лимит: %2 MiB). Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1601"/>
        <source>TLS is enabled for this connection, but the session with the Director is not encrypted. The connection was closed.</source>
        <translation>Для этого подключения включён TLS, но сеанс с Director не зашифрован. Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1862"/>
        <source>Unknown error</source>
        <translation>Неизвестная ошибка</translation>
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
        <translation>Не готово</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1975"/>
        <source>Error sending command</source>
        <translation>Ошибка отправки команды</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2029"/>
        <source>The command was not sent: no ready connection to the Director</source>
        <translation>Команда не отправлена: нет готового подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2442"/>
        <source>This build of Onesimus has no gRPC support</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2537"/>
        <source>Server certificate not trusted</source>
        <translation>Сертификат сервера не является доверенным</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2553"/>
        <source>Command failed</source>
        <translation>Ошибка команды</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2586"/>
        <source>Connection to Director lost: %1</source>
        <translation>Подключение к Director потеряно: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/bareosdirector.cpp" line="1058"/>
        <source>The Director did not answer within %n second(s)</source>
        <translation type="unfinished">
            <numerusform></numerusform>
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
        <translation>--tls требует --host. Без --host используются настройки TLS сохранённого профиля подключения.</translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="423"/>
        <source>--tls needs the console password: use --password-file, the environment variable ONESIMUS_PASSWORD or --password.</source>
        <translation>--tls требует пароль консоли: используйте --password-file, переменную окружения ONESIMUS_PASSWORD или --password.</translation>
    </message>
</context>
<context>
    <name>ConfigPreviewPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1944"/>
        <source>Server Configuration Preview</source>
        <translation>Предпросмотр конфигурации сервера</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1946"/>
        <source>Copy this configuration to your Bareos Director</source>
        <translation>Скопируйте эту конфигурацию в ваш Bareos Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1956"/>
        <source>Console resource for the Director:</source>
        <translation>Ресурс Console для Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1960"/>
        <source>Path: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</source>
        <translation>Путь: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1972"/>
        <location filename="../src/bconnectionwizard.cpp" line="2013"/>
        <location filename="../src/bconnectionwizard.cpp" line="2076"/>
        <location filename="../src/bconnectionwizard.cpp" line="2353"/>
        <location filename="../src/bconnectionwizard.cpp" line="2392"/>
        <location filename="../src/bconnectionwizard.cpp" line="2403"/>
        <source>Copy to Clipboard</source>
        <translation>Копировать в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1977"/>
        <location filename="../src/bconnectionwizard.cpp" line="2018"/>
        <location filename="../src/bconnectionwizard.cpp" line="2081"/>
        <location filename="../src/bconnectionwizard.cpp" line="2381"/>
        <location filename="../src/bconnectionwizard.cpp" line="2430"/>
        <location filename="../src/bconnectionwizard.cpp" line="2507"/>
        <source>Save .conf</source>
        <translation>Сохранить .conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1982"/>
        <location filename="../src/bconnectionwizard.cpp" line="2023"/>
        <location filename="../src/bconnectionwizard.cpp" line="2086"/>
        <location filename="../src/bconnectionwizard.cpp" line="2481"/>
        <location filename="../src/bconnectionwizard.cpp" line="2546"/>
        <source>Save ZIP</source>
        <translation>Сохранить ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1990"/>
        <source>Console Config</source>
        <translation>Конфигурация Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1997"/>
        <source>Director resource TLS settings (if needed):</source>
        <translation>Настройки TLS ресурса Director (при необходимости):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2001"/>
        <source>Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Путь: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2031"/>
        <source>Director TLS</source>
        <translation>Director TLS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2042"/>
        <source>Configuration for command-line consoles on the client:</source>
        <translation>Конфигурация для консолей командной строки на клиенте:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2046"/>
        <source>Path: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf or oconsole -c &amp;lt;file&amp;gt; (oconsole)</source>
        <translation>Путь: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf или oconsole -c &amp;lt;file&amp;gt; (oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2054"/>
        <source>Contains the console password: restrict the file to mode 0640 root:bareos (or 0600 owned by the user running the console).</source>
        <translation>Содержит пароль консоли: ограничьте права файла режимом 0640 root:bareos (или 0600 с владельцем — пользователем, запускающим консоль).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2094"/>
        <source>Console client (bconsole / oconsole)</source>
        <translation>Клиент консоли (bconsole / oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2098"/>
        <location filename="../src/bconnectionwizard.cpp" line="2135"/>
        <source>After copying, paste this into your Bareos Director config and restart bareos-dir.</source>
        <translation>После копирования вставьте это в конфигурацию Bareos Director и перезапустите bareos-dir.</translation>
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
        <translation>Пароль в форме «[md5]&lt;hex&gt;»: принимается oconsole и bconsole на C++.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2312"/>
        <source>not used when the Console resource below matches</source>
        <translation>не используется, если подходит ресурс Console ниже</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2320"/>
        <source>Grpc Fingerprint = &lt;SHA-256 of the Director&apos;s gRPC certificate&gt;</source>
        <translation>Grpc Fingerprint = &lt;SHA-256 сертификата gRPC Director&gt;</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2351"/>
        <location filename="../src/bconnectionwizard.cpp" line="2390"/>
        <location filename="../src/bconnectionwizard.cpp" line="2401"/>
        <source>Copied!</source>
        <translation>Скопировано!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2364"/>
        <source>Save Console Client Configuration</source>
        <translation>Сохранить конфигурацию клиента консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2365"/>
        <location filename="../src/bconnectionwizard.cpp" line="2414"/>
        <location filename="../src/bconnectionwizard.cpp" line="2491"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2371"/>
        <location filename="../src/bconnectionwizard.cpp" line="2420"/>
        <location filename="../src/bconnectionwizard.cpp" line="2450"/>
        <location filename="../src/bconnectionwizard.cpp" line="2497"/>
        <location filename="../src/bconnectionwizard.cpp" line="2526"/>
        <source>Save Failed</source>
        <translation>Ошибка сохранения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2372"/>
        <location filename="../src/bconnectionwizard.cpp" line="2421"/>
        <location filename="../src/bconnectionwizard.cpp" line="2498"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2379"/>
        <location filename="../src/bconnectionwizard.cpp" line="2428"/>
        <location filename="../src/bconnectionwizard.cpp" line="2479"/>
        <location filename="../src/bconnectionwizard.cpp" line="2505"/>
        <location filename="../src/bconnectionwizard.cpp" line="2544"/>
        <source>Saved!</source>
        <translation>Сохранено!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2413"/>
        <source>Save Console Configuration</source>
        <translation>Сохранить конфигурацию Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2440"/>
        <source>Save Console Configuration as ZIP</source>
        <translation>Сохранить конфигурацию Console в ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2441"/>
        <location filename="../src/bconnectionwizard.cpp" line="2517"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2451"/>
        <location filename="../src/bconnectionwizard.cpp" line="2527"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2490"/>
        <source>Save Director TLS Configuration</source>
        <translation>Сохранить конфигурацию TLS Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2516"/>
        <source>Save Director TLS Configuration as ZIP</source>
        <translation>Сохранить конфигурацию TLS Director в ZIP</translation>
    </message>
</context>
<context>
    <name>CredentialsPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1333"/>
        <source>Enter your credentials</source>
        <translation>Введите учётные данные</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1339"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1343"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1351"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1352"/>
        <source>Generate a secure random password</source>
        <translation>Сгенерировать надёжный случайный пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1357"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1364"/>
        <location filename="../src/bconnectionwizard.cpp" line="1370"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(здесь появится MD5-хеш пароля)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1365"/>
        <source>MD5 Hash:</source>
        <translation>MD5-хеш:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1403"/>
        <source>Remember password</source>
        <translation>Запомнить пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1412"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console config: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</source>
        <translation>Конфигурация Director: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Конфигурация Console: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1418"/>
        <source>These must match your Bareos Director configuration.</source>
        <translation>Они должны совпадать с конфигурацией вашего Bareos Director.</translation>
    </message>
</context>
<context>
    <name>ImportConsoleSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="668"/>
        <source>Select Director and Console from imported configuration</source>
        <translation>Выберите Director и Console из импортированной конфигурации</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="673"/>
        <source>The configuration will be parsed from your ZIP file. Select which Director and Console to use for the connection.</source>
        <translation>Конфигурация будет считана из вашего ZIP-файла. Выберите, какие Director и Console использовать для подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="679"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="682"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="704"/>
        <source>Select a console to view its details...</source>
        <translation>Выберите консоль, чтобы просмотреть сведения о ней...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="746"/>
        <source>No ZIP file specified.</source>
        <translation>ZIP-файл не указан.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="753"/>
        <source>Extracting and parsing ZIP archive...</source>
        <translation>Распаковка и разбор ZIP-архива...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="764"/>
        <source>Cannot read the ZIP archive: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="774"/>
        <source>No configuration resources found in the archive.</source>
        <translation>В архиве не найдено ресурсов конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="800"/>
        <source>Parsed %1 resources: %2 Director(s), %3 Console(s)</source>
        <translation>Разобрано ресурсов: %1 (Director: %2, Console: %3)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="808"/>
        <source>Warning: Need at least one Director and one Console resource.</source>
        <translation>Предупреждение: нужен как минимум один ресурс Director и один ресурс Console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="830"/>
        <source>=== Director: %1 ===
</source>
        <translation>=== Director: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="835"/>
        <source>(multiple addresses)</source>
        <translation>(несколько адресов)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="838"/>
        <source>  Address: %1
</source>
        <translation>  Адрес: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="838"/>
        <source>(not set)</source>
        <translation>(не задано)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="839"/>
        <source>  Port: %1
</source>
        <translation>  Порт: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="840"/>
        <location filename="../src/bconnectionwizard.cpp" line="862"/>
        <source>  TLS Enable: %1
</source>
        <translation>  TLS Enable: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="847"/>
        <source>=== Console: %1 ===
</source>
        <translation>=== Console: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="853"/>
        <source>  Password: [md5]***
</source>
        <translation>  Пароль: [md5]***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="855"/>
        <source>  Password: ***
</source>
        <translation>  Пароль: ***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="858"/>
        <source>  Password: (not set)
</source>
        <translation>  Пароль: (не задан)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="863"/>
        <source>  TLS Require: %1
</source>
        <translation>  TLS Require: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="868"/>
        <source>  Profile: %1
</source>
        <translation>  Profile: %1
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
        <translation>  %1: %2
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="892"/>
        <source>
  (No ACLs defined on this console)
</source>
        <translation>
  (В этой консоли не определены ACL)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1034"/>
        <source>Host/Address</source>
        <translation>Хост/адрес</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1035"/>
        <source>Director Name</source>
        <translation>Имя Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1036"/>
        <source>Console Name</source>
        <translation>Имя консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1037"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1040"/>
        <source>Incomplete Configuration</source>
        <translation>Неполная конфигурация</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1041"/>
        <source>The imported configuration is missing the following required fields:

  %1

You can fill them in on the following pages.</source>
        <translation>В импортированной конфигурации отсутствуют следующие обязательные поля:

  %1

Их можно заполнить на следующих страницах.</translation>
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
        <translation>Назовите это подключение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2826"/>
        <source>e.g., Production Server</source>
        <translation>например, Рабочий сервер</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2829"/>
        <source>Set as default</source>
        <translation>Использовать по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2833"/>
        <source>Connect now</source>
        <translation>Подключиться сейчас</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2843"/>
        <source>Advanced Settings...</source>
        <translation>Дополнительные настройки...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2844"/>
        <source>Configure TLS options, ACLs, and other advanced settings</source>
        <translation>Настроить параметры TLS, ACL и другие дополнительные параметры</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2849"/>
        <source>Export Config...</source>
        <translation>Экспорт конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2850"/>
        <source>Export Bareos configuration files as zip archive</source>
        <translation>Экспортировать файлы конфигурации Bareos в ZIP-архив</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2857"/>
        <source>This name helps identify the connection.</source>
        <translation>Это имя помогает распознать подключение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2937"/>
        <source>Console only (for existing Director)</source>
        <translation>Только Console (для существующего Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2938"/>
        <source>Console + Profile (with ACLs)</source>
        <translation>Console + Profile (с ACL)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2939"/>
        <source>Full configuration (new installation)</source>
        <translation>Полная конфигурация (новая установка)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2943"/>
        <location filename="../src/bconnectionwizard.cpp" line="2958"/>
        <source>Export Configuration</source>
        <translation>Экспорт конфигурации</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2944"/>
        <source>Select export mode:</source>
        <translation>Выберите режим экспорта:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2960"/>
        <source>Zip Archives (*.zip)</source>
        <translation>ZIP-архивы (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2965"/>
        <source>Export Successful</source>
        <translation>Экспорт выполнен</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2966"/>
        <source>Configuration exported to:
%1

Extract to your Bareos server&apos;s root directory and restart the Director.</source>
        <translation>Конфигурация экспортирована в:
%1

Распакуйте её в корневой каталог сервера Bareos и перезапустите Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2970"/>
        <source>Export Failed</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2971"/>
        <source>Failed to export configuration:
%1</source>
        <translation>Не удалось экспортировать конфигурацию:
%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="33"/>
        <source>No files to export</source>
        <translation>Нет файлов для экспорта</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="206"/>
        <source>Cannot create zip file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="237"/>
        <source>Error writing zip file</source>
        <translation>Ошибка записи ZIP-файла</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="269"/>
        <source>Not set</source>
        <translation>Не задано</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="273"/>
        <source>%n year(s)</source>
        <translation>
            <numerusform>%n год</numerusform>
            <numerusform>%n года</numerusform>
            <numerusform>%n лет</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="275"/>
        <source>%n day(s)</source>
        <translation>
            <numerusform>%n день</numerusform>
            <numerusform>%n дня</numerusform>
            <numerusform>%n дней</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="279"/>
        <source>%n hour(s)</source>
        <translation>
            <numerusform>%n час</numerusform>
            <numerusform>%n часа</numerusform>
            <numerusform>%n часов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="281"/>
        <source>%n second(s)</source>
        <translation>
            <numerusform>%n секунда</numerusform>
            <numerusform>%n секунды</numerusform>
            <numerusform>%n секунд</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="30"/>
        <source>Add &quot;%1&quot;</source>
        <translation>Добавить «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="64"/>
        <source>Remove &quot;%1&quot;</source>
        <translation>Удалить «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="66"/>
        <source>Remove %1 items</source>
        <translation>Удалить элементы (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="99"/>
        <source>Edit item</source>
        <translation>Изменить элемент</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="140"/>
        <source>Move item</source>
        <translation>Переместить элемент</translation>
    </message>
    <message>
        <location filename="../include/bconnectionprofile.h" line="129"/>
        <source>New Connection</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ServerPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1069"/>
        <source>Where is your Bareos Director?</source>
        <translation>Где находится ваш Bareos Director?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1074"/>
        <source>Found in the network:</source>
        <translation>Найдено в сети:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1076"/>
        <source>Search again</source>
        <translation>Искать снова</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1086"/>
        <source>Onesimus Directors announce themselves in the local network. Select one to fill in the connection data.</source>
        <translation>Onesimus Directors сообщают о себе в локальной сети. Выберите один, чтобы заполнить данные подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1101"/>
        <source>Searching...</source>
        <translation>Поиск...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1112"/>
        <source>e.g., bareos.example.com or 192.168.1.100</source>
        <translation>например, bareos.example.com или 192.168.1.100</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1113"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1119"/>
        <source>Port:</source>
        <translation>Порт:</translation>
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
        <translation>Выберите операционную систему сервера Bareos Director.
Это влияет на пути к сертификатам в экспортируемых конфигурациях.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1128"/>
        <source>Server OS:</source>
        <translation>ОС сервера:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1132"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1133"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1139"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1141"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1147"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1161"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Конфигурация Director: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1164"/>
        <location filename="../src/bconnectionwizard.cpp" line="1223"/>
        <source>The default port is 9101.</source>
        <translation>Порт по умолчанию — 9101.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1201"/>
        <source>Searching for Onesimus Directors in the network...</source>
        <translation>Поиск Onesimus Directors в сети...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1207"/>
        <source>No Onesimus Director found - enter the server manually.</source>
        <translation>Onesimus Director не найден — введите сервер вручную.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1222"/>
        <source>gRPC is encrypted with TLS; the server certificate is confirmed on first use.</source>
        <translation>gRPC шифруется с помощью TLS; сертификат сервера подтверждается при первом использовании.</translation>
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
            <numerusform>Найден %n Director. Выберите его, чтобы использовать.</numerusform>
            <numerusform>Найдено %n Directors. Выберите один, чтобы использовать его.</numerusform>
            <numerusform>Найдено %n Directors. Выберите один, чтобы использовать его.</numerusform>
        </translation>
    </message>
</context>
<context>
    <name>StorageWidget</name>
    <message>
        <location filename="../src/storagewidget.cpp" line="30"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="56"/>
        <source>(due)</source>
        <translation>(срок наступил)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="88"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="91"/>
        <source>Pool filter</source>
        <translation>Фильтр Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="97"/>
        <source>&amp;Status:</source>
        <translation>&amp;Статус:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="100"/>
        <source>Status filter</source>
        <translation>Фильтр статуса</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="106"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="117"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="120"/>
        <source>Volumes:</source>
        <translation>Volumes:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="128"/>
        <source>Used:</source>
        <translation>Использовано:</translation>
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
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="458"/>
        <source>Used</source>
        <translation>Использовано</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="148"/>
        <location filename="../src/storagewidget.cpp" line="459"/>
        <source>Limit</source>
        <translation>Лимит</translation>
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
        <translation>Последняя запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Retention</source>
        <translation>Хранение</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Reusable From</source>
        <translation>Повторно используемо с</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="152"/>
        <source>Maximum Volume Bytes of the volume; &quot;none&quot; = no limit</source>
        <translation>Maximum Volume Bytes этого Volume; «нет» = без ограничения</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="154"/>
        <source>From when Bareos may reuse the volume. It does so only when its pool needs a volume, so this is the earliest date, not the day of recycling.</source>
        <translation>С какого момента Bareos может повторно использовать Volume. Это происходит только тогда, когда его Pool нужен Volume, поэтому это самая ранняя дата, а не день повторного использования.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="156"/>
        <source>Volumes</source>
        <translation>Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="158"/>
        <source>One row per volume. Enter opens the details of the selected volume.</source>
        <translation>По одной строке на Volume. Enter открывает сведения о выбранном Volume.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="215"/>
        <source>All pools (%1)</source>
        <translation>Все Pools (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="218"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="224"/>
        <source>All statuses (%1)</source>
        <translation>Все статусы (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="227"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="250"/>
        <source>Error parsing volume data</source>
        <translation>Ошибка разбора данных Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="257"/>
        <source>The Director sent the short volume list: limits, job counts and use durations are unknown, and so is when an Append volume will be closed.</source>
        <translation>Director прислал краткий список Volumes: лимиты, количество Jobs и сроки использования неизвестны, как и то, когда Volume в состоянии Append будет закрыт.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/storagewidget.cpp" line="271"/>
        <source>%n volume(s) loaded</source>
        <translation>
            <numerusform>Загружен %n Volume</numerusform>
            <numerusform>Загружено %n Volumes</numerusform>
            <numerusform>Загружено %n Volumes</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="318"/>
        <source>%1, %2, %3, last written %4, reusable from %5</source>
        <translation>%1, %2, %3, последняя запись %4, повторно используемо с %5</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="346"/>
        <location filename="../src/storagewidget.cpp" line="459"/>
        <location filename="../src/storagewidget.cpp" line="461"/>
        <location filename="../src/storagewidget.cpp" line="467"/>
        <location filename="../src/storagewidget.cpp" line="469"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="354"/>
        <source>%1 of %2</source>
        <translation>%1 из %2</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="411"/>
        <source>Refreshing the volume list...</source>
        <translation>Обновление списка Volumes...</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="439"/>
        <source>unknown (not in this list)</source>
        <translation>неизвестно (нет в этом списке)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="456"/>
        <source>Media type</source>
        <translation>Тип носителя</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="461"/>
        <source>Maximum jobs</source>
        <translation>Максимум Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="462"/>
        <source>Errors</source>
        <translation>Ошибки</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="463"/>
        <source>First written</source>
        <translation>Первая запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="464"/>
        <source>Last written</source>
        <translation>Последняя запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465"/>
        <source>Labeled</source>
        <translation>Размечено</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="466"/>
        <source>Volume retention</source>
        <translation>Volume Retention</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="467"/>
        <source>Use duration</source>
        <translation>Срок использования</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>Recycle</source>
        <translation>Recycle</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="469"/>
        <source>Recycle pool</source>
        <translation>Recycle Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="470"/>
        <source>Times recycled</source>
        <translation>Число повторных использований</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="471"/>
        <source>Reusable from</source>
        <translation>Повторно используемо с</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="481"/>
        <source>Volume Details</source>
        <translation>Сведения о Volume</translation>
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
        <translation>Где находятся ваши сертификаты TLS?</translation>
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
        <translation>Сертификат CA:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1706"/>
        <source>Path to PFX certificate file</source>
        <translation>Путь к файлу сертификата PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1711"/>
        <source>PFX File:</source>
        <translation>Файл PFX:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1715"/>
        <source>PFX file password (if encrypted)</source>
        <translation>Пароль файла PFX (если зашифрован)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1716"/>
        <source>PFX Password:</source>
        <translation>Пароль PFX:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1727"/>
        <source>/etc/bareos/ssl/client.pem</source>
        <translation>/etc/bareos/ssl/client.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1732"/>
        <source>Client Cert:</source>
        <translation>Сертификат клиента:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1736"/>
        <source>/etc/bareos/ssl/client.key</source>
        <translation>/etc/bareos/ssl/client.key</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1741"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1751"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Преобразовать PEM/DER в PFX...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1752"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Преобразовать отдельные файлы сертификата и ключа PEM/DER в файл PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1757"/>
        <source>Generate Certificates...</source>
        <translation>Сгенерировать сертификаты...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1760"/>
        <source>No certificates? Click Generate to create them.</source>
        <translation>Нет сертификатов? Нажмите «Сгенерировать», чтобы создать их.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1796"/>
        <location filename="../src/bconnectionwizard.cpp" line="1807"/>
        <location filename="../src/bconnectionwizard.cpp" line="1819"/>
        <source>Missing</source>
        <translation>Отсутствует</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1796"/>
        <source>Please provide a CA certificate file.</source>
        <translation>Укажите файл сертификата CA.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1800"/>
        <location filename="../src/bconnectionwizard.cpp" line="1811"/>
        <location filename="../src/bconnectionwizard.cpp" line="1823"/>
        <source>Not Found</source>
        <translation>Не найдено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1800"/>
        <source>CA certificate file not found.</source>
        <translation>Файл сертификата CA не найден.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1807"/>
        <source>Please provide a PFX certificate file.</source>
        <translation>Укажите файл сертификата PFX.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1811"/>
        <source>PFX certificate file not found.</source>
        <translation>Файл сертификата PFX не найден.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1819"/>
        <source>Please provide all certificate files.</source>
        <translation>Укажите все файлы сертификатов.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1823"/>
        <source>One or more certificate files not found.</source>
        <translation>Один или несколько файлов сертификатов не найдены.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1857"/>
        <source>CA Certificate</source>
        <translation>Сертификат CA</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1858"/>
        <location filename="../src/bconnectionwizard.cpp" line="1879"/>
        <source>Certificates (*.pem *.crt *.cer);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cer);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875"/>
        <source>Select PFX Certificate</source>
        <translation>Выбор сертификата PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1876"/>
        <source>PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)</source>
        <translation>Файлы PFX (*.pfx);;Сертификаты (*.pem *.crt);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1878"/>
        <source>Client Certificate</source>
        <translation>Сертификат клиента</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1896"/>
        <source>Private Key</source>
        <translation>Закрытый ключ</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1897"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Закрытые ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1932"/>
        <source>PFX File Created</source>
        <translation>Файл PFX создан</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1933"/>
        <source>PFX file has been created successfully.

The certificate path has been updated.</source>
        <translation>Файл PFX успешно создан.

Путь к сертификату обновлён.</translation>
    </message>
</context>
<context>
    <name>TemplateSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="319"/>
        <source>How would you like to create the connection?</source>
        <translation>Как вы хотите создать подключение?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="321"/>
        <source>You can start from scratch, use a template from an existing connection, or import from a ZIP file.</source>
        <translation>Можно начать с нуля, использовать шаблон существующего подключения или импортировать из ZIP-файла.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="325"/>
        <source>Create new connection from scratch</source>
        <translation>Создать новое подключение с нуля</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="326"/>
        <source>Use template from existing connection</source>
        <translation>Использовать шаблон существующего подключения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="327"/>
        <source>Import from ZIP file</source>
        <translation>Импорт из ZIP-файла</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="349"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="373"/>
        <source>Select ZIP file...</source>
        <translation>Выберите ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="376"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="487"/>
        <source>Director: %1</source>
        <translation>Director: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="488"/>
        <source>Console: %1</source>
        <translation>Console: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>TLS: %1</source>
        <translation>TLS: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <source>PSK</source>
        <translation>PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <source>Certificate</source>
        <translation>Сертификат</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="489"/>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>Disabled</source>
        <translation>Отключено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="545"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="546"/>
        <source>System: %1</source>
        <translation>Система: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="652"/>
        <source>Select Director Configuration ZIP</source>
        <translation>Выбор ZIP-файла конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="654"/>
        <source>ZIP Files (*.zip);;All Files (*)</source>
        <translation>ZIP-файлы (*.zip);;Все файлы (*)</translation>
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
        <translation>Проверка подключения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2561"/>
        <source>Let&apos;s test your connection.</source>
        <translation>Давайте проверим ваше подключение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2565"/>
        <location filename="../src/bconnectionwizard.cpp" line="2601"/>
        <source>Click the button to test.</source>
        <translation>Нажмите кнопку, чтобы начать проверку.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2624"/>
        <source>Connecting...</source>
        <translation>Подключение...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2627"/>
        <source>Starting test...</source>
        <translation>Запуск проверки...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2666"/>
        <source>Mode: TLS-Cert</source>
        <translation>Режим: TLS-Cert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2678"/>
        <source>Mode: TLS-PSK with cipher list: %1</source>
        <translation>Режим: TLS-PSK со списком шифров: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2680"/>
        <source>Mode: TLS-PSK</source>
        <translation>Режим: TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2693"/>
        <source>Mode: gRPC (TLS) on port %1</source>
        <translation>Режим: gRPC (TLS) на порту %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2694"/>
        <location filename="../src/bconnectionwizard.cpp" line="2696"/>
        <source>Connecting to %1:%2...</source>
        <translation>Подключение к %1:%2...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2732"/>
        <source>Cancelled.</source>
        <translation>Отменено.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2740"/>
        <source>Connecting</source>
        <translation>Подключение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2741"/>
        <source>Authenticating</source>
        <translation>Аутентификация</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2742"/>
        <source>Setting API</source>
        <translation>Установка API</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2743"/>
        <source>Loading</source>
        <translation>Загрузка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2744"/>
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2766"/>
        <source>Server certificate trusted.</source>
        <translation>Сертификат сервера является доверенным.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2770"/>
        <source>Server certificate not trusted - connection cancelled.</source>
        <translation>Сертификат сервера не является доверенным — подключение отменено.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2771"/>
        <location filename="../src/bconnectionwizard.cpp" line="2784"/>
        <location filename="../src/bconnectionwizard.cpp" line="2790"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2783"/>
        <source>Auth OK</source>
        <translation>Аутентификация успешна</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2784"/>
        <source>Auth failed: %1</source>
        <translation>Ошибка аутентификации: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2789"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2800"/>
        <source>Success!</source>
        <translation>Успешно!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2801"/>
        <source>Connection successful!</source>
        <translation>Подключение выполнено успешно!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2804"/>
        <source>Test Again</source>
        <translation>Проверить снова</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2812"/>
        <source>Timeout</source>
        <translation>Тайм-аут</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2813"/>
        <source>Timed out</source>
        <translation>Время ожидания истекло</translation>
    </message>
</context>
<context>
    <name>WelcomePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="297"/>
        <source>Welcome</source>
        <translation>Добро пожаловать</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="303"/>
        <source>Let&apos;s set up a connection to your Bareos Director.

You&apos;ll need:
  - The hostname or IP of your Director
  - Your console name and password

Click Next to begin.</source>
        <translation>Давайте настроим подключение к вашему Bareos Director.

Вам понадобятся:
  - Имя хоста или IP-адрес вашего Director
  - Имя консоли и пароль

Нажмите «Далее», чтобы начать.</translation>
    </message>
</context>
</TS>
