<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU" sourcelanguage="en">
<context>
    <name>AuthMethodPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1589"/>
        <source>How should we authenticate?</source>
        <translation>Какой способ аутентификации использовать?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1593"/>
        <source>TLS-PSK (recommended)</source>
        <translation>TLS-PSK (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1598"/>
        <source>TLS with certificates</source>
        <translation>TLS с сертификатами</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1615"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Список шифров TLS (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1622"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>напр., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1623"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Click Preset to use recommended ciphers.</source>
        <translation>Список шифров TLS-PSK через двоеточие.
Оставьте пустым для автоопределения.
Нажмите «Предустановка», чтобы использовать рекомендуемые шифры.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1628"/>
        <source>Preset</source>
        <translation>Предустановка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1629"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Заполнить рекомендуемыми шифрами PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1638"/>
        <source>TLS-PSK is the default for Bareos 18.2+.</source>
        <translation>TLS-PSK используется по умолчанию начиная с Bareos 18.2.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1696"/>
        <source>Server supports TLS encryption.</source>
        <translation>Сервер поддерживает шифрование TLS.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1704"/>
        <source>TLS-PSK - not available on this server</source>
        <translation>TLS-PSK — недоступно на этом сервере</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1705"/>
        <source>This server doesn&apos;t appear to support TLS-PSK. Consider upgrading to Bareos 18.2 or later.</source>
        <translation>Похоже, этот сервер не поддерживает TLS-PSK. Рекомендуется обновиться до Bareos 18.2 или новее.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1713"/>
        <source>Server check failed: %1</source>
        <translation>Ошибка проверки сервера: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1730"/>
        <source>TLS-PSK uses your password to encrypt the connection.</source>
        <translation>TLS-PSK использует ваш пароль для шифрования соединения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1734"/>
        <source>You&apos;ll need CA, client certificate, and private key.</source>
        <translation>Потребуются сертификат CA, сертификат клиента и закрытый ключ.</translation>
    </message>
</context>
<context>
    <name>B321Dialog</name>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="88"/>
        <location filename="../src/tools/b321dialog.cpp" line="575"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="109"/>
        <location filename="../src/tools/b321dialog.cpp" line="632"/>
        <source>%1 (not in the volume list)</source>
        <translation>%1 (нет в списке Volumes)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="111"/>
        <location filename="../src/tools/b321dialog.cpp" line="649"/>
        <source>media type unknown</source>
        <translation>тип носителя неизвестен</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="112"/>
        <source>storage unknown</source>
        <translation>Storage неизвестен</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="115"/>
        <source>off site (marked)</source>
        <translation>вне площадки (отмечено)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="117"/>
        <source>off site (location %1)</source>
        <translation>вне площадки (местоположение %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="119"/>
        <source>on site</source>
        <translation>на площадке</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="120"/>
        <source>off site unknown</source>
        <translation>размещение неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="122"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="131"/>
        <location filename="../src/tools/b321dialog.cpp" line="132"/>
        <source>3-2-1 Verifier</source>
        <translation>Проверка 3-2-1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="133"/>
        <source>Checks for every backup job whether there are 3 copies of the data, on 2 media types, with 1 copy off site. Read only.</source>
        <translation>Проверяет для каждого Job резервного копирования, есть ли 3 копии данных на 2 типах носителей, из них 1 копия вне площадки. Только чтение.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="154"/>
        <source>Data state</source>
        <translation>Состояние данных</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="156"/>
        <source>&amp;Refresh</source>
        <translation>&amp;Обновить</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="158"/>
        <source>Reload the jobs, the volumes of the jobs and the volume list (read only)</source>
        <translation>Перезагрузить Jobs, Volumes этих Jobs и список Volumes (только чтение)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="170"/>
        <source>&amp;Backup jobs:</source>
        <translation>&amp;Jobs резервного копирования:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174"/>
        <source>Copies</source>
        <translation>Копии</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="174"/>
        <source>Media Types</source>
        <translation>Типы носителей</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175"/>
        <source>Off Site</source>
        <translation>Вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175"/>
        <source>Verdict</source>
        <translation>Вердикт</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="175"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="177"/>
        <source>Copies of the data a restore would use: the chain, its complete copies and, if set, the original data. 3 are needed.</source>
        <translation>Копии данных, которые использовало бы восстановление: цепочка, её полные копии и, если включено, исходные данные. Нужно 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="180"/>
        <source>Distinct media types of the volumes of the chain and its complete copies. 2 are needed.</source>
        <translation>Различные типы носителей у Volumes цепочки и её полных копий. Нужно 2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="182"/>
        <source>Copies off site, by your marking of the storages or a location of the volume. 1 is needed.</source>
        <translation>Копии вне площадки — по вашей отметке Storages или по местоположению Volume. Нужна 1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="185"/>
        <source>3-2-1 check per backup job</source>
        <translation>Проверка 3-2-1 для каждого Job резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="186"/>
        <source>One row per backup job: copies, media types, off site, verdict and reason. The details of the selected job follow below the table.</source>
        <translation>Одна строка на каждый Job резервного копирования: копии, типы носителей, вне площадки, вердикт и причина. Подробности о выбранном Job приведены под таблицей.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="221"/>
        <source>C&amp;hain and copies:</source>
        <translation>&amp;Цепочка и копии:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="226"/>
        <source>Chain and copies of the selected job</source>
        <translation>Цепочка и копии выбранного Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="227"/>
        <source>The jobs a restore would use and the copies of them, with the volumes they lie on</source>
        <translation>Jobs, которые использовало бы восстановление, и их копии вместе с Volumes, на которых они находятся</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="237"/>
        <source>&amp;Why, and what to do:</source>
        <translation>&amp;Почему и что делать:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="241"/>
        <source>Reasons, suggestions and hints for the selected job</source>
        <translation>Причины, предложения и подсказки для выбранного Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="250"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="252"/>
        <source>Count the &amp;original data as a copy</source>
        <translation>Считать &amp;исходные данные копией</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="253"/>
        <source>The 3-2-1 rule is usually stated as the production data plus 2 backups. Off: 3 backups are needed.</source>
        <translation>Правило 3-2-1 обычно формулируется как рабочие данные плюс 2 резервные копии. Если выключено, нужно 3 резервные копии.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="258"/>
        <source>O&amp;ff-site storages:</source>
        <translation>Storages &amp;вне площадки:</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="260"/>
        <source>Bareos does not record where a storage stands. Tick the storages that are at another site.</source>
        <translation>Bareos не хранит сведений о том, где находится Storage. Отметьте Storages, расположенные на другой площадке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="267"/>
        <source>Off-site storages</source>
        <translation>Storages вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="268"/>
        <source>A checked storage is off site. Space toggles.</source>
        <translation>Отмеченный Storage находится вне площадки. Пробел переключает отметку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="271"/>
        <source>&amp;Unmarked storages are on site</source>
        <translation>&amp;Неотмеченные Storages находятся на площадке</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="272"/>
        <source>Without this, a storage that is not ticked is &quot;unknown&quot;, not &quot;on site&quot;.</source>
        <translation>Без этой опции неотмеченный Storage считается «неизвестно», а не «на площадке».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="282"/>
        <source>&amp;Close</source>
        <translation>&amp;Закрыть</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="319"/>
        <location filename="../src/tools/b321dialog.cpp" line="435"/>
        <source>%1, off site</source>
        <translation>%1, вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="320"/>
        <location filename="../src/tools/b321dialog.cpp" line="436"/>
        <source>%1, not marked off site</source>
        <translation>%1, не отмечен как вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="441"/>
        <source>No storage known yet</source>
        <translation>Storages пока неизвестны</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="468"/>
        <location filename="../src/tools/b321dialog.cpp" line="517"/>
        <source>Jobs: %1</source>
        <translation>Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="487"/>
        <location filename="../src/tools/b321dialog.cpp" line="520"/>
        <source>Volumes of the jobs: %1</source>
        <translation>Volumes этих Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="502"/>
        <location filename="../src/tools/b321dialog.cpp" line="523"/>
        <source>Volume list: %1</source>
        <translation>Список Volumes: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="554"/>
        <source>%1: %2. %3</source>
        <translation>%1: %2. %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="558"/>
        <source>%1/3</source>
        <translation>%1/3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="561"/>
        <source>Copies: %1 of 3, %2</source>
        <translation>Копии: %1 из 3, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="565"/>
        <source>?/2</source>
        <translation>?/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="565"/>
        <source>%1/2</source>
        <translation>%1/2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="568"/>
        <source>Media types: %1 of 2%2, %3</source>
        <translation>Типы носителей: %1 из 2%2, %3</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="576"/>
        <source>%1/1</source>
        <translation>%1/1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="580"/>
        <source>Off site: unknown</source>
        <translation>Вне площадки: неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="581"/>
        <source>Off site: %1 of 1, %2</source>
        <translation>Вне площадки: %1 из 1, %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="587"/>
        <source>Verdict: %1</source>
        <translation>Вердикт: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="611"/>
        <source>No job selected.</source>
        <translation>Job не выбран.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="623"/>
        <source>volumes not loaded</source>
        <translation>Volumes не загружены</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="625"/>
        <source>no volume recorded</source>
        <translation>Volume не записан</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="630"/>
        <source>%1 (%2, %3, %4)</source>
        <translation>%1 (%2, %3, %4)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="640"/>
        <source>Chain (the backup itself): %n job(s)</source>
        <translation>
            <numerusform>Цепочка (сама резервная копия): %n Job</numerusform>
            <numerusform>Цепочка (сама резервная копия): %n Jobs</numerusform>
            <numerusform>Цепочка (сама резервная копия): %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642"/>
        <source>Copy in pool %1: complete</source>
        <translation>Копия в Pool %1: полная</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="642"/>
        <location filename="../src/tools/b321dialog.cpp" line="645"/>
        <source>(not reported)</source>
        <translation>(не сообщается)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="644"/>
        <source>Copy in pool %1: partial, %2 of %3 jobs</source>
        <translation>Копия в Pool %1: частичная, %2 из %3 Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="653"/>
        <source>last job ended %1</source>
        <translation>последний Job завершён %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="654"/>
        <source>copied %1</source>
        <translation>скопировано %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="655"/>
        <source>%1 — %2</source>
        <translation>%1 — %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="662"/>
        <source>%1 %2, started %3: %4</source>
        <translation>%1 %2, запущен %3: %4</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="665"/>
        <source> (migrated from job %1)</source>
        <translation> (перенесён из Job %1)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="669"/>
        <source>Copy %1 of %2 %3, copied %4: %5</source>
        <translation>Копия %1 из %2 %3, скопирована %4: %5</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="678"/>
        <source>No copy of %1 %2</source>
        <translation>Нет копии %1 %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="684"/>
        <source>No restorable chain in the job history</source>
        <translation>В истории Jobs нет цепочки, пригодной для восстановления</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="689"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="696"/>
        <source>Why</source>
        <translation>Почему</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="697"/>
        <source>What to do</source>
        <translation>Что делать</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="698"/>
        <source>Hints (not part of the verdict)</source>
        <translation>Подсказки (не влияют на вердикт)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="707"/>
        <source>Not everything could be loaded: %1</source>
        <translation>Не всё удалось загрузить: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="711"/>
        <source>jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="712"/>
        <source>volumes of the jobs</source>
        <translation>Volumes этих Jobs</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="713"/>
        <source>volume list</source>
        <translation>список Volumes</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="714"/>
        <source>Loading: %1 …</source>
        <translation>Загрузка: %1 …</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="717"/>
        <source>No job data yet. Refresh loads it (read only).</source>
        <translation>Данных о Jobs пока нет. «Обновить» загружает их (только чтение).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="718"/>
        <source>Not connected to a Director.</source>
        <translation>Нет подключения к Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="726"/>
        <source>%n job(s) checked: %1 pass, %2 fail, %3 unknown. </source>
        <translation>
            <numerusform>Проверен %n Job: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
            <numerusform>Проверено %n Jobs: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
            <numerusform>Проверено %n Jobs: %1 пройдено, %2 не пройдено, %3 неизвестно. </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="728"/>
        <source>From %n job record(s) of the last %1 days</source>
        <translation>
            <numerusform>По %n записи о Jobs за последние %1 дн.</numerusform>
            <numerusform>По %n записям о Jobs за последние %1 дн.</numerusform>
            <numerusform>По %n записям о Jobs за последние %1 дн.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="729"/>
        <source>, %n volume(s)</source>
        <translation>
            <numerusform>, %n Volume</numerusform>
            <numerusform>, %n Volumes</numerusform>
            <numerusform>, %n Volumes</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321dialog.cpp" line="730"/>
        <source>, the volumes of %n job(s)</source>
        <translation>
            <numerusform>, Volumes %n Job</numerusform>
            <numerusform>, Volumes %n Jobs</numerusform>
            <numerusform>, Volumes %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321dialog.cpp" line="732"/>
        <source>. Read only.</source>
        <translation>. Только чтение.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="13"/>
        <source>3-2-1 &amp;Verifier...</source>
        <translation>Проверка 3-2-&amp;1...</translation>
    </message>
    <message>
        <location filename="../src/tools/b321action.cpp" line="14"/>
        <source>Check for every backup job: 3 copies, 2 media types, 1 copy off site (read only)</source>
        <translation>Проверить для каждого Job резервного копирования: 3 копии, 2 типа носителей, 1 копия вне площадки (только чтение)</translation>
    </message>
</context>
<context>
    <name>B321Verifier</name>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="90"/>
        <source>unknown time</source>
        <translation>время неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="96"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="109"/>
        <source>(pool not reported)</source>
        <translation>(Pool не сообщается)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="411"/>
        <source>No successful Full backup of %1 in the job history of the last %n day(s): there is no chain to restore.</source>
        <translation>
            <numerusform>В истории Jobs за последний %n день нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
            <numerusform>В истории Jobs за последние %n дня нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
            <numerusform>В истории Jobs за последние %n дней нет успешной резервной копии Full для %1: нет цепочки для восстановления.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="413"/>
        <source>No successful Full backup of %1 in the job history: there is no chain to restore.</source>
        <translation>В истории Jobs нет успешной резервной копии Full для %1: нет цепочки для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="415"/>
        <source>Run a Full backup of %1 and check that it terminates normally.</source>
        <translation>Выполните резервное копирование Full для %1 и убедитесь, что оно завершилось нормально.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="417"/>
        <source>No successful Full backup in the job history</source>
        <translation>В истории Jobs нет успешной резервной копии Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="419"/>
        <source>Only the original data on the client counts: 1 of 3 copies.</source>
        <translation>Учитываются только исходные данные на клиенте: 1 из 3 копий.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="432"/>
        <source>The chain a restore would use today is %1</source>
        <translation>Цепочка, которую сегодня использовало бы восстановление: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="433"/>
        <source>, a Differential</source>
        <translation>, Differential</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="434"/>
        <source> and %n Incremental(s) after it</source>
        <translation>
            <numerusform> и после неё %n Incremental</numerusform>
            <numerusform> и после неё %n Incremental</numerusform>
            <numerusform> и после неё %n Incremental</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="435"/>
        <source>; it counts as 1 copy.</source>
        <translation>; она считается 1 копией.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="438"/>
        <source>The original data on the client counts as 1 copy (setting &quot;Count the original data as a copy&quot;).</source>
        <translation>Исходные данные на клиенте считаются 1 копией (настройка «Считать исходные данные копией»).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="441"/>
        <source>The original data on the client is not counted (setting &quot;Count the original data as a copy&quot; is off).</source>
        <translation>Исходные данные на клиенте не учитываются (настройка «Считать исходные данные копией» выключена).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="447"/>
        <source>job %1 from job %2</source>
        <translation>Job %1 из Job %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="449"/>
        <source>Migrated chain jobs (%1): a Migrate moves the data and adds no copy; the old job is not restored any more.</source>
        <translation>Перенесённые Jobs цепочки (%1): Migrate перемещает данные и не добавляет копию; старый Job больше не восстанавливается.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="455"/>
        <source>Pool %1 holds a copy of every job of the chain; it counts as 1 copy.</source>
        <translation>Pool %1 содержит копию каждого Job цепочки; это считается 1 копией.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="459"/>
        <source>Pool %1 holds copies of %2 of the %3 jobs of the chain (missing: %4); a partial copy does not count.</source>
        <translation>Pool %1 содержит копии %2 из %3 Jobs цепочки (отсутствуют: %4); частичная копия не учитывается.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="465"/>
        <source>No Copy job has copied the chain.</source>
        <translation>Ни один Job типа Copy не скопировал цепочку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="467"/>
        <source>Copies: %1 of 3.</source>
        <translation>Копии: %1 из 3.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="473"/>
        <source>The volumes of the chain are not known, so neither are their media types.</source>
        <translation>Volumes цепочки неизвестны, поэтому неизвестны и их типы носителей.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="474"/>
        <source>The volumes of the jobs (JobMedia) were not loaded: media types unknown.</source>
        <translation>Volumes этих Jobs (JobMedia) не загружены: типы носителей неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="476"/>
        <source>Media types of the chain and its complete copies: %1.</source>
        <translation>Типы носителей цепочки и её полных копий: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="479"/>
        <source>A copy that spans several media types counts once; copies on media types of their own: %1.</source>
        <translation>Копия, занимающая несколько типов носителей, считается один раз; копии на собственных типах носителей: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="483"/>
        <source>They lie on the storages %1; the same media type on several storages counts as one type.</source>
        <translation>Они находятся на Storages %1; один и тот же тип носителя на нескольких Storages считается одним типом.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="488"/>
        <source>Some volumes of these jobs are not in the volume list or have no media type.</source>
        <translation>Некоторых Volumes этих Jobs нет в списке Volumes, или у них не указан тип носителя.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="510"/>
        <source>storage %1 marked off site</source>
        <translation>Storage %1 отмечен как вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="511"/>
        <source>volume %1 has location %2 in the catalog</source>
        <translation>у Volume %1 в каталоге указано местоположение %2</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="515"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="516"/>
        <source>the chain</source>
        <translation>цепочка</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="517"/>
        <source>the copy in pool %1</source>
        <translation>копия в Pool %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="520"/>
        <source>Off site: %1.</source>
        <translation>Вне площадки: %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="524"/>
        <source>Off site is unknown: no storage of the chain or its copies is marked off site and no volume has a location in the catalog. Bareos itself does not record whether a storage is off site.</source>
        <translation>Размещение вне площадки неизвестно: ни один Storage цепочки или её копий не отмечен как вне площадки, и ни у одного Volume нет местоположения в каталоге. Сам Bareos не хранит сведений о том, находится ли Storage вне площадки.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="529"/>
        <source>Nothing is off site: every copy has a volume on a storage that is on site.</source>
        <translation>Вне площадки ничего нет: у каждой копии есть Volume на Storage, расположенном на площадке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="535"/>
        <source>the present one</source>
        <translation>текущая</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="538"/>
        <source>Pool %1 lacks copies of the jobs %2: run its Copy job again (Selection Type = PoolUncopiedJobs copies what is missing) so that the whole chain is copied.</source>
        <translation>В Pool %1 нет копий Jobs %2: запустите его Job типа Copy ещё раз (Selection Type = PoolUncopiedJobs копирует недостающее), чтобы скопировать всю цепочку.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="546"/>
        <source>Add a Copy job to a second storage with a different media type than %1 (for example tape, a removable disk or cloud storage with a Media Type of its own) (needed: %2 more).</source>
        <translation>Добавьте Job типа Copy на второй Storage с типом носителя, отличным от %1 (например, лента, съёмный диск или облачное хранилище с собственным Media Type) (нужно ещё: %2).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="551"/>
        <source>Add a Copy job to a second storage (needed: %1 more).</source>
        <translation>Добавьте Job типа Copy на второй Storage (нужно ещё: %1).</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="555"/>
        <source>Write one of the copies to a storage with another media type than %1.</source>
        <translation>Запишите одну из копий на Storage с типом носителя, отличным от %1.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="559"/>
        <source>Refresh: the volumes of the jobs and the volume list are needed to tell the media types.</source>
        <translation>Обновите: чтобы определить типы носителей, нужны Volumes этих Jobs и список Volumes.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="564"/>
        <source>Mark storage %1 as off site if it is at another site; if it is not, tick &quot;Unmarked storages are on site&quot;.</source>
        <translation>Отметьте Storage %1 как вне площадки, если он находится на другой площадке; если нет, включите «Неотмеченные Storages находятся на площадке».</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="569"/>
        <source>Refresh: the storages of the volumes are needed to tell where the copies are.</source>
        <translation>Обновите: чтобы определить, где находятся копии, нужны Storages этих Volumes.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="573"/>
        <source>Copy the chain to a storage at another site and mark that storage as off site (or take a volume off site and record its location).</source>
        <translation>Скопируйте цепочку на Storage на другой площадке и отметьте этот Storage как вне площадки (или вывезите Volume за пределы площадки и укажите его местоположение).</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="593"/>
        <source>The newest Full (job %1) is %n day(s) old</source>
        <translation>
            <numerusform>Последней копии Full (Job %1) %n день</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дня</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дней</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="595"/>
        <source>; this job&apos;s Fulls ran every %n day(s) (median of the loaded history).</source>
        <translation>
            <numerusform>; копии Full этого Job выполнялись каждый %n день (медиана загруженной истории).</numerusform>
            <numerusform>; копии Full этого Job выполнялись каждые %n дня (медиана загруженной истории).</numerusform>
            <numerusform>; копии Full этого Job выполнялись каждые %n дней (медиана загруженной истории).</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="598"/>
        <source> It is overdue.</source>
        <translation> Она просрочена.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="601"/>
        <source>The newest Full (job %1) is %n day(s) old; it is the only successful Full in the loaded history.</source>
        <translation>
            <numerusform>Последней копии Full (Job %1) %n день; это единственная успешная копия Full в загруженной истории.</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дня; это единственная успешная копия Full в загруженной истории.</numerusform>
            <numerusform>Последней копии Full (Job %1) %n дней; это единственная успешная копия Full в загруженной истории.</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/b321verifier.cpp" line="628"/>
        <source>The newest copy of this job (job %1 in pool %2, a copy of job %3) was made %n day(s) ago.</source>
        <translation>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n день назад.</numerusform>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n дня назад.</numerusform>
            <numerusform>Последняя копия этого Job (Job %1 в Pool %2, копия Job %3) создана %n дней назад.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="638"/>
        <source>That copy belongs to an earlier chain: the current chain (from job %1) has no complete copy in pool %2.</source>
        <translation>Эта копия относится к более ранней цепочке: у текущей цепочки (начиная с Job %1) нет полной копии в Pool %2.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="667"/>
        <source>The chain&apos;s %1 (job %2)</source>
        <translation>%1 цепочки (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="669"/>
        <source>The copy (job %1, pool %2) of the chain&apos;s %3</source>
        <translation>Копия (Job %1, Pool %2) %3 цепочки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="671"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="673"/>
        <source>%1 is on volume %2, which is in status Error.</source>
        <translation>%1 находится на Volume %2 в состоянии Error.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="676"/>
        <source>The chain&apos;s Full is on volume %1, which is in Error: run a new Full backup and check the volume.</source>
        <translation>Full цепочки находится на Volume %1 в состоянии Error: выполните новое резервное копирование Full и проверьте Volume.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="681"/>
        <source>%1 is on volume %2, which is %3: Bareos may overwrite it.</source>
        <translation>%1 находится на Volume %2 в состоянии %3: Bareos может его перезаписать.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="690"/>
        <source>The job list hit its row limit: older jobs and copies may be missing from this check.</source>
        <translation>Список Jobs достиг ограничения по числу строк: более старые Jobs и копии могут отсутствовать в этой проверке.</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="696"/>
        <source>%1 of 3 copies</source>
        <translation>%1 из 3 копий</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="697"/>
        <source>partial copies: %1</source>
        <translation>частичные копии: %1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="699"/>
        <source>media types unknown</source>
        <translation>типы носителей неизвестны</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="701"/>
        <source>media: %1 of 2 (%2)</source>
        <translation>носители: %1 из 2 (%2)</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="704"/>
        <source>off site: %1 of 1</source>
        <translation>вне площадки: %1 из 1</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="705"/>
        <source>off site unknown</source>
        <translation>размещение неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="706"/>
        <source>nothing off site</source>
        <translation>вне площадки ничего нет</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="859"/>
        <source>met</source>
        <translation>выполнено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="860"/>
        <source>not met</source>
        <translation>не выполнено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="863"/>
        <location filename="../src/tools/b321verifier.cpp" line="895"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="869"/>
        <source>Passes</source>
        <translation>Пройдено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="870"/>
        <source>Fails</source>
        <translation>Не пройдено</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="873"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="879"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="880"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="881"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="882"/>
        <source>Virtual Full</source>
        <translation>Virtual Full</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="885"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="891"/>
        <source>off site</source>
        <translation>вне площадки</translation>
    </message>
    <message>
        <location filename="../src/tools/b321verifier.cpp" line="892"/>
        <source>on site</source>
        <translation>на площадке</translation>
    </message>
</context>
<context>
    <name>BAclMatrix</name>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="54"/>
        <source>Jobs</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="55"/>
        <source>Clients</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="56"/>
        <source>Storages</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="57"/>
        <source>Schedules</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="58"/>
        <source>Pools</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="59"/>
        <source>FileSets</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="60"/>
        <source>Catalogs</source>
        <translation>Catalog</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="61"/>
        <source>Commands</source>
        <translation>Команды</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="68"/>
        <source>Where</source>
        <translation>Where</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="69"/>
        <source>Plugin Options</source>
        <translation>Plugin Options</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="83"/>
        <source>The Director sent no reply.</source>
        <translation>Director не ответил.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="84"/>
        <source>The Director did not answer with ACLs: %1</source>
        <translation>Director ответил не списком ACL: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="93"/>
        <source>The Director refused &quot;.acl list&quot;: %1</source>
        <translation>Director отклонил «.acl list»: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="98"/>
        <source>The reply holds no list of consoles (&quot;acl&quot;).</source>
        <translation>В ответе нет списка консолей («acl»).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="309"/>
        <location filename="../src/director/baclmatrix.cpp" line="381"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="316"/>
        <source>Locked: the catalog ACL of the console could not be read; it may run status, .status, version and .api only.</source>
        <translation>Заблокирована: не удалось прочитать ACL каталога этой консоли; разрешены только status, .status, version и .api.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325"/>
        <source>Allowed (decided by the Director).</source>
        <translation>Разрешено (решение Director).</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="325"/>
        <location filename="../src/director/baclmatrix.cpp" line="338"/>
        <source>Allowed by %1.</source>
        <translation>Разрешено записью %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="328"/>
        <source>The file ACL allows it (%1), the catalog ACL does not.</source>
        <translation>ACL файла разрешает (%1), ACL каталога — нет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="330"/>
        <location filename="../src/director/baclmatrix.cpp" line="339"/>
        <source>Denied by %1.</source>
        <translation>Запрещено записью %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="333"/>
        <location filename="../src/director/baclmatrix.cpp" line="341"/>
        <source>The console has no ACL of this type: nothing is allowed.</source>
        <translation>У консоли нет ACL этого типа: ничего не разрешено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="334"/>
        <location filename="../src/director/baclmatrix.cpp" line="342"/>
        <source>No entry of the ACL allows it.</source>
        <translation>Ни одна запись ACL этого не разрешает.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="360"/>
        <source>Console %1</source>
        <translation>Консоль %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="361"/>
        <source>Profiles: none</source>
        <translation>Профили: нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="362"/>
        <source>Profiles: %1</source>
        <translation>Профили: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="364"/>
        <source>Locked: its catalog ACL could not be read.</source>
        <translation>Заблокирована: не удалось прочитать её ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="367"/>
        <source>loaded</source>
        <translation>загружена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="368"/>
        <source>off</source>
        <translation>отключена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="369"/>
        <source>table missing</source>
        <translation>нет таблицы</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="370"/>
        <source>failed</source>
        <translation>ошибка</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="371"/>
        <source>Catalog ACL: %1</source>
        <translation>ACL каталога: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="372"/>
        <source>Catalog ACL: %1 (%2)</source>
        <translation>ACL каталога: %1 (%2)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrix.cpp" line="377"/>
        <source>%n catalog ACL entry(s) narrow the file ACL.</source>
        <translation>
            <numerusform>%n запись ACL каталога сужает ACL файла.</numerusform>
            <numerusform>%n записи ACL каталога сужают ACL файла.</numerusform>
            <numerusform>%n записей ACL каталога сужают ACL файла.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="382"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="392"/>
        <source>The Director did not answer with profiles: %1</source>
        <translation>Director ответил не списком профилей: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="399"/>
        <source>The reply holds no profiles.</source>
        <translation>В ответе нет профилей.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrix.cpp" line="415"/>
        <source>Profile %1</source>
        <translation>Profile %1</translation>
    </message>
</context>
<context>
    <name>BAclMatrixDialog</name>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="63"/>
        <source>Change the catalog ACL</source>
        <translation>Изменить ACL каталога</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="65"/>
        <source>These changes of the catalog ACL are sent to the Director, one after the other. The file ACL is not changed. The first change the Director refuses stops the rest.</source>
        <translation>Эти изменения ACL каталога отправляются в Director одно за другим. ACL файла не изменяется. Первое изменение, отклонённое Director, останавливает остальные.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="77"/>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="99"/>
        <source>New console</source>
        <translation>Новая консоль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="101"/>
        <source>The Director creates a Console resource with this name, password and profile (configure add console). The profile&apos;s ACLs are the frame of the console; the matrix can narrow them further through the catalog ACL afterwards.</source>
        <translation>Director создаёт ресурс Console с этим именем, паролем и профилем (configure add console). ACL профиля — рамка консоли; затем матрица может дополнительно сузить их через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="113"/>
        <source>e.g. backup-viewer</source>
        <translation>напр. backup-viewer</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="114"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="213"/>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="120"/>
        <source>New</source>
        <translation>Новый</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="121"/>
        <source>Generates another password</source>
        <translation>Создаёт другой пароль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="124"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="190"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="125"/>
        <source>Copies the password to the clipboard</source>
        <translation>Копирует пароль в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="130"/>
        <source>&amp;Password:</source>
        <translation>&amp;Пароль:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="136"/>
        <source>The Profile resource whose ACLs the console gets</source>
        <translation>Ресурс Profile, ACL которого получает консоль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="137"/>
        <source>P&amp;rofile:</source>
        <translation>П&amp;рофиль:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="139"/>
        <source>Copy the password now: whoever uses the console needs it, and it cannot be read back here.</source>
        <translation>Скопируйте пароль сейчас: он нужен тому, кто будет пользоваться консолью, и здесь его нельзя прочитать снова.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="147"/>
        <source>Create</source>
        <translation>Создать</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="151"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="230"/>
        <source>Enter a name.</source>
        <translation>Введите имя.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="152"/>
        <source>A console of this name exists.</source>
        <translation>Консоль с таким именем уже существует.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="153"/>
        <source>Choose a profile.</source>
        <translation>Выберите профиль.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="174"/>
        <source>Proposed configuration text</source>
        <translation>Предлагаемый текст конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="176"/>
        <source>Profiles are changed in the configuration files of the Director only. Put this text in place of the Profile resource in its file (the path is a suggestion), check it with the Director&apos;s test (-t) and reload; then Refresh shows the new state. Comments of the old resource are not in this text.</source>
        <translation>Профили изменяются только в файлах конфигурации Director. Вставьте этот текст вместо ресурса Profile в его файле (путь — лишь предложение), проверьте тестом Director (-t) и перезагрузите; затем «Обновить» покажет новое состояние. Комментариев старого ресурса в этом тексте нет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="201"/>
        <source>New profile</source>
        <translation>Новый профиль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="203"/>
        <source>The new profile is a proposal in the matrix: change its cells, then &quot;Config text...&quot; shows its Profile resource to put into the Director&apos;s configuration. Nothing is sent.</source>
        <translation>Новый профиль — это предложение в матрице: измените его ячейки, затем «Текст конфигурации...» покажет его ресурс Profile для вставки в конфигурацию Director. Ничего не отправляется.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="212"/>
        <source>e.g. restore-only</source>
        <translation>напр. restore-only</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="215"/>
        <source>&amp;Description:</source>
        <translation>&amp;Описание:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="217"/>
        <source>(empty: nothing allowed)</source>
        <translation>(пусто: ничего не разрешено)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="219"/>
        <source>The ACL entries the new profile starts with</source>
        <translation>Записи ACL, с которых начинается новый профиль</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="220"/>
        <source>&amp;Start as a copy of:</source>
        <translation>&amp;Начать как копию:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="226"/>
        <source>Add to the matrix</source>
        <translation>Добавить в матрицу</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="231"/>
        <source>A profile of this name exists.</source>
        <translation>Профиль с таким именем уже существует.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="250"/>
        <source>(no entries: no restriction)</source>
        <translation>(нет записей: без ограничений)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="278"/>
        <source>ACL matrix of the consoles</source>
        <translation>Матрица ACL консолей</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="284"/>
        <source>Consoles</source>
        <translation>Консоли</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="285"/>
        <source>Profiles</source>
        <translation>Профили</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="286"/>
        <source>Consoles: what each console may use, the catalog ACL can be changed. Profiles: the ACLs of the Profile resources, changed as proposed configuration text.</source>
        <translation>Консоли: что может использовать каждая консоль, ACL каталога можно изменить. Профили: ACL ресурсов Profile, изменяемые в виде предлагаемого текста конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="294"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="296"/>
        <source>Filter resources</source>
        <translation>Фильтр ресурсов</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="299"/>
        <source>Only rows where the consoles differ</source>
        <translation>Только строки, где консоли различаются</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="302"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="303"/>
        <source>Reads the ACLs of every console again (.acl list)</source>
        <translation>Заново читает ACL всех консолей (.acl list)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="305"/>
        <source>Reading the ACLs of the consoles...</source>
        <translation>Чтение ACL консолей...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="309"/>
        <source>New console...</source>
        <translation>Новая консоль...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="310"/>
        <source>Creates a console on the Director with a password and a profile (configure add console); it appears as a new column</source>
        <translation>Создаёт в Director консоль с паролем и профилем (configure add console); она появляется как новый столбец</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="314"/>
        <source>New profile...</source>
        <translation>Новый профиль...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="315"/>
        <source>Adds a proposed profile as a new column; &quot;Config text...&quot; shows its Profile resource to put into the configuration</source>
        <translation>Добавляет предлагаемый профиль как новый столбец; «Текст конфигурации...» показывает его ресурс Profile для вставки в конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="322"/>
        <source>ACL matrix: resources by console</source>
        <translation>Матрица ACL: ресурсы по консолям</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="338"/>
        <source>✓ allowed   ✗ denied by an entry with &quot;!&quot;   · not allowed   ⊘ allowed by the file ACL, narrowed by the catalog ACL   — console locked. A cell&apos;s tool tip says why; a console&apos;s tool tip has its profiles, its catalog ACL and its Where and Plugin Options ACLs.</source>
        <translation>✓ разрешено   ✗ запрещено записью с «!»   · не разрешено   ⊘ разрешено ACL файла, сужено ACL каталога   — консоль заблокирована. Подсказка ячейки объясняет причину; подсказка консоли показывает её профили, ACL каталога и ACL Where и Plugin Options.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="415"/>
        <source>The profiles could not be read: %1</source>
        <translation>Не удалось прочитать профили: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="440"/>
        <source>%n profile(s). Profiles are changed in the configuration files: &quot;Config text...&quot; proposes the new text.</source>
        <translation>
            <numerusform>%n профиль. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
            <numerusform>%n профиля. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
            <numerusform>%n профилей. Профили изменяются в файлах конфигурации: «Текст конфигурации...» предлагает новый текст.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="455"/>
        <source>Config text...</source>
        <translation>Текст конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="457"/>
        <source>Shows the new text of every changed profile, to put into the Director&apos;s configuration</source>
        <translation>Показывает новый текст каждого изменённого профиля для вставки в конфигурацию Director</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="461"/>
        <source>A double click (or Space) on a cell turns it in the proposal: ✓ becomes denied, · or ✗ allowed. Changed cells are underlined; &quot;Config text...&quot; shows the new Profile resources. Nothing is sent: a person puts the text in.</source>
        <translation>Двойной щелчок (или пробел) по ячейке меняет её в предложении: ✓ становится запрещённым, · или ✗ — разрешённым. Изменённые ячейки подчёркнуты; «Текст конфигурации...» показывает новые ресурсы Profile. Ничего не отправляется: текст вставляет человек.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="464"/>
        <source>A double click (or Space) on ✓ denies it through the catalog ACL, on ⊘ allows it again. Changed cells are underlined until they are applied. What the file ACL denies is changed in the editor.</source>
        <translation>Двойной щелчок (или пробел) по ✓ запрещает через ACL каталога, по ⊘ снова разрешает. Изменённые ячейки подчёркнуты, пока изменения не применены. То, что запрещает ACL файла, меняется в редакторе.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="357"/>
        <source>Discard</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="358"/>
        <source>Forgets the changes that were not applied</source>
        <translation>Забывает непримененные изменения</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="361"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="455"/>
        <source>Apply...</source>
        <translation>Применить...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="362"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="458"/>
        <source>Shows every change of the catalog ACL once and sends them after your confirmation (.acl set / .acl delete)</source>
        <translation>Показывает каждое изменение ACL каталога один раз и отправляет их после вашего подтверждения (.acl set / .acl delete)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="349"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="350"/>
        <source>Closes the matrix; changes that were not applied are discarded</source>
        <translation>Закрывает матрицу; непримененные изменения отбрасываются</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="402"/>
        <source>%n console(s). Allowed is what the Director decided (file ACL and catalog ACL).</source>
        <translation>
            <numerusform>%n консоль. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
            <numerusform>%n консоли. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
            <numerusform>%n консолей. Разрешено то, что решил Director (ACL файла и каталога).</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="404"/>
        <source>%n console(s). This Director reports the file ACL only; the GUI decided the cells from its entries.</source>
        <translation>
            <numerusform>%n консоль. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
            <numerusform>%n консоли. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
            <numerusform>%n консолей. Этот Director сообщает только ACL файла; GUI определил ячейки по её записям.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="469"/>
        <source>No profiles read yet: Refresh reads them.</source>
        <translation>Профили ещё не прочитаны: «Обновить» их прочитает.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="499"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="525"/>
        <source>Profile %1</source>
        <translation>Профиль %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="505"/>
        <source>Profile %1 is proposed: change its cells, then &quot;Config text...&quot; shows it.</source>
        <translation>Профиль %1 предложен: измените его ячейки, затем «Текст конфигурации...» покажет его.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="526"/>
        <source>Proposed: not on the Director yet.</source>
        <translation>Предложен: ещё нет в Director.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="528"/>
        <source>Used by no console</source>
        <translation>Не используется ни одной консолью</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="528"/>
        <source>Used by: %1</source>
        <translation>Используется: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="531"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="531"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="553"/>
        <source>The proposal stays until the Director has it: after the text is in place and the Director reloaded, Refresh shows it.</source>
        <translation>Предложение сохраняется, пока Director его не получит: когда текст вставлен и Director перезагружен, «Обновить» его покажет.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="559"/>
        <source>The ACLs could not be read: %1</source>
        <translation>Не удалось прочитать ACL: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="596"/>
        <source>In the proposal: allowed.</source>
        <translation>В предложении: разрешено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="597"/>
        <source>In the proposal: denied.</source>
        <translation>В предложении: запрещено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="598"/>
        <source>Double click: change it in the proposed configuration text.</source>
        <translation>Двойной щелчок: изменить в предлагаемом тексте конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="620"/>
        <source>Not applied yet: allowed again through the catalog ACL.</source>
        <translation>Ещё не применено: снова разрешено через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="621"/>
        <source>Not applied yet: denied through the catalog ACL.</source>
        <translation>Ещё не применено: запрещено через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="624"/>
        <source>Double click: deny it through the catalog ACL.</source>
        <translation>Двойной щелчок: запретить через ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="625"/>
        <source>Double click: allow it again.</source>
        <translation>Двойной щелчок: снова разрешить.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="657"/>
        <source>Console %1 is locked: its catalog ACL cannot be changed until it can be read.</source>
        <translation>Консоль %1 заблокирована: её ACL каталога нельзя изменить, пока её не удаётся прочитать.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="658"/>
        <source>%1 is not allowed by the file ACL of console %2: that is changed in the editor, not here.</source>
        <translation>%1 не разрешено ACL файла консоли %2: это меняется в редакторе, а не здесь.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="691"/>
        <source>Discard the changes?</source>
        <translation>Отбросить изменения?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="692"/>
        <source>%n change(s) of the catalog ACL were not applied. Discard them and close?</source>
        <translation>
            <numerusform>%n изменение ACL каталога не применено. Отбросить и закрыть?</numerusform>
            <numerusform>%n изменения ACL каталога не применены. Отбросить и закрыть?</numerusform>
            <numerusform>%n изменений ACL каталога не применено. Отбросить и закрыть?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="704"/>
        <source>Discard the proposals?</source>
        <translation>Отбросить предложения?</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/director/baclmatrixdialog.cpp" line="705"/>
        <source>%n profile proposal(s) are only in the matrix. Discard them and close?</source>
        <translation>
            <numerusform>%n предложение профиля есть только в матрице. Отбросить и закрыть?</numerusform>
            <numerusform>%n предложения профилей есть только в матрице. Отбросить и закрыть?</numerusform>
            <numerusform>%n предложений профилей есть только в матрице. Отбросить и закрыть?</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="745"/>
        <source>Creating console %1...</source>
        <translation>Создание консоли %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="756"/>
        <source>Console %1 was not created: %2</source>
        <translation>Консоль %1 не создана: %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="759"/>
        <source>Console %1 was created.</source>
        <translation>Консоль %1 создана.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="774"/>
        <source>The changes were discarded.</source>
        <translation>Изменения отменены.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="792"/>
        <source>Nothing was sent: the entry &quot;%1&quot; contains a comma, which the Director cannot store in a catalog ACL.</source>
        <translation>Ничего не отправлено: запись «%1» содержит запятую, которую Director не может сохранить в ACL каталога.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="802"/>
        <source>Console %1, %2:</source>
        <translation>Консоль %1, %2:</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="803"/>
        <source>  now: %1</source>
        <translation>  сейчас: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="804"/>
        <source>  new: %1</source>
        <translation>  новое: %1</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="806"/>
        <source>%1 of %2</source>
        <translation>%1 консоли %2</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="811"/>
        <source>Nothing was sent: the changes were not confirmed.</source>
        <translation>Ничего не отправлено: изменения не подтверждены.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="823"/>
        <source>Changed: %1.</source>
        <translation>Изменено: %1.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="826"/>
        <source>Changing %1...</source>
        <translation>Изменение %1...</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="756"/>
        <location filename="../src/director/baclmatrixdialog.cpp" line="835"/>
        <source>refused</source>
        <translation>отклонено</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="846"/>
        <source>nothing</source>
        <translation>ничего</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="847"/>
        <source>Changing %1 failed: %2. Changed before: %3. Nothing after it was sent.</source>
        <translation>Не удалось изменить %1: %2. Изменено ранее: %3. Дальше ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="872"/>
        <source>%1 (new)</source>
        <translation>%1 (новый)</translation>
    </message>
    <message>
        <location filename="../src/director/baclmatrixdialog.cpp" line="898"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>BBvfsModel</name>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="234"/>
        <source>Directory</source>
        <translation>Каталог</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="235"/>
        <source>Symlink</source>
        <translation>Символическая ссылка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="236"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="299"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="300"/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="301"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="302"/>
        <source>Modified</source>
        <translation>Изменён</translation>
    </message>
    <message>
        <location filename="../src/jobs/bbvfsmodel.cpp" line="468"/>
        <source>JSON parse error: %1</source>
        <translation>Ошибка разбора JSON: %1</translation>
    </message>
</context>
<context>
    <name>BCapabilities</name>
    <message>
        <location filename="../src/bcapabilities.cpp" line="33"/>
        <source>Change an existing schedule</source>
        <translation>Изменить существующий Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="36"/>
        <source>%1 cannot change an existing schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий Schedule через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="40"/>
        <source>Nothing was changed. A schedule can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. Schedule также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="49"/>
        <source>Prune and purge all volumes</source>
        <translation>Prune и purge для всех Volumes</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="52"/>
        <source>%1 has no command that purges all volumes at once; purge works on named volumes only and ignores retention periods.</source>
        <translation>У %1 нет команды, выполняющей purge для всех Volumes сразу; purge работает только с указанными по имени Volumes и игнорирует сроки хранения.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="55"/>
        <source>&quot;Prune only&quot; applies the retention period of every volume; a single volume can be purged by its name with the purge command.</source>
        <translation>«Только prune» применяет срок хранения каждого Volume; для отдельного Volume можно выполнить purge по его имени командой purge.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="62"/>
        <source>Send a FileSet to the Director</source>
        <translation>Отправить FileSet на Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="65"/>
        <source>%1 cannot take a FileSet through the console, because its configure command does not accept nested blocks (Include, Options, Exclude).</source>
        <translation>%1 не может принять FileSet через консоль, так как его команда configure не принимает вложенные блоки (Include, Options, Exclude).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="68"/>
        <source>You can copy the configuration text and place it on the Director yourself.</source>
        <translation>Вы можете скопировать текст конфигурации и разместить его на Director самостоятельно.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="75"/>
        <source>Delete a schedule</source>
        <translation>Удалить Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="78"/>
        <source>%1 cannot delete a schedule on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить Schedule через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="82"/>
        <source>Nothing was deleted. A schedule can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. Schedule также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="91"/>
        <source>Create a schedule</source>
        <translation>Создать Schedule</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="94"/>
        <source>%1 cannot create a schedule.</source>
        <translation>%1 не может создать Schedule.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="106"/>
        <source>Change an existing job</source>
        <translation>Изменить существующий Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="109"/>
        <source>%1 cannot change an existing job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий Job через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="113"/>
        <source>Nothing was changed. A job can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. Job также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="122"/>
        <source>Delete a job</source>
        <translation>Удалить Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="125"/>
        <source>%1 cannot delete a job on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить Job через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="128"/>
        <source>Nothing was deleted. A job can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. Job также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="137"/>
        <source>Create a job</source>
        <translation>Создать Job</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="140"/>
        <source>%1 cannot create a job.</source>
        <translation>%1 не может создать Job.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="148"/>
        <source>Change an existing JobDefs</source>
        <translation>Изменить существующий JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="151"/>
        <source>%1 cannot change an existing JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может изменить существующий JobDefs через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="155"/>
        <source>Nothing was changed. A JobDefs can also be changed in its configuration file on the Director.</source>
        <translation>Ничего не изменено. JobDefs также можно изменить в его файле конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="164"/>
        <source>Delete a JobDefs</source>
        <translation>Удалить JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="167"/>
        <source>%1 cannot delete a JobDefs on this connection: that needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird).</source>
        <translation>%1 не может удалить JobDefs через это соединение: для этого нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird).</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="171"/>
        <source>Nothing was deleted. A JobDefs can also be removed from the configuration files on the Director.</source>
        <translation>Ничего не удалено. JobDefs также можно удалить из файлов конфигурации на Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="180"/>
        <source>Create a JobDefs</source>
        <translation>Создать JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="183"/>
        <source>%1 cannot create a JobDefs.</source>
        <translation>%1 не может создать JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="309"/>
        <source>This Director (Bareos)</source>
        <translation>Этот Director (Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="310"/>
        <source>This Director (Bareos %1)</source>
        <translation>Этот Director (Bareos %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="314"/>
        <source>This Director (Onesimus Director)</source>
        <translation>Этот Director (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="316"/>
        <source>This Director (Onesimus Director %1)</source>
        <translation>Этот Director (Onesimus Director %1)</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="322"/>
        <source>This Director</source>
        <translation>Этот Director</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="334"/>
        <source>%1 does not offer this to this session: its configuration capabilities do not list it. The console needs the command ACL &quot;configure&quot; and the ACL of the resource type, or the Director is older than its configuration methods.</source>
        <translation>%1 не предоставляет эту функцию данному сеансу: её нет в списке его возможностей конфигурации. Консоли нужен командный ACL «configure» и ACL типа ресурса, либо Director старше, чем его методы конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="343"/>
        <source>The capabilities query failed: %1.</source>
        <translation>Ошибка запроса возможностей: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="347"/>
        <source>The capabilities were not received for this session.</source>
        <translation>Возможности для этого сеанса не получены.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="356"/>
        <source>The Director lists for this session: %1.</source>
        <translation>Director сообщает для этого сеанса: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="372"/>
        <source>The Onesimus Director supports this over its gRPC interface.</source>
        <translation>Onesimus Director поддерживает это через свой интерфейс gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="375"/>
        <source>Connect to it over gRPC to use this.</source>
        <translation>Чтобы использовать это, подключитесь к нему через gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="382"/>
        <source>The Onesimus Director supports this.</source>
        <translation>Onesimus Director поддерживает это.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="387"/>
        <source>Support for this is planned for a later version of the Onesimus Director.</source>
        <translation>Поддержка этого запланирована в одной из следующих версий Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/bcapabilities.cpp" line="390"/>
        <source>The Onesimus Director is planned to support this.</source>
        <translation>Поддержка этого в Onesimus Director запланирована.</translation>
    </message>
</context>
<context>
    <name>BCapabilityNotice</name>
    <message>
        <location filename="../src/bcapabilitynotice.cpp" line="65"/>
        <source>Do not show again</source>
        <translation>Больше не показывать</translation>
    </message>
</context>
<context>
    <name>BCatalogResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448"/>
        <source>DB Driver</source>
        <translation>Драйвер БД</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="448"/>
        <source>DB Name</source>
        <translation>Имя БД</translation>
    </message>
</context>
<context>
    <name>BCertificateGenerator</name>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="36"/>
        <source>Generate TLS Certificates</source>
        <translation>Создание сертификатов TLS</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="48"/>
        <source>Generate TLS certificates for secure Bareos communication.
This will create a CA certificate, client certificate, and private key.</source>
        <translation>Создание сертификатов TLS для защищённого обмена данными Bareos.
Будут созданы сертификат CA, сертификат клиента и закрытый ключ.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="56"/>
        <source>Certificate Settings</source>
        <translation>Параметры сертификата</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="64"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="67"/>
        <source>Output Directory:</source>
        <translation>Выходной каталог:</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="72"/>
        <source>e.g., onesimus-client</source>
        <translation>напр., onesimus-client</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="73"/>
        <source>Common Name (CN):</source>
        <translation>Общее имя (CN):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="78"/>
        <source>e.g., My Company</source>
        <translation>напр., Моя компания</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="79"/>
        <source>Organization (O):</source>
        <translation>Организация (O):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="84"/>
        <source>Days (e.g., 3650 = 10 years)</source>
        <translation>Дни (напр., 3650 = 10 лет)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="85"/>
        <source>Valid for (days):</source>
        <translation>Срок действия (дней):</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="88"/>
        <source>Overwrite existing files</source>
        <translation>Перезаписывать существующие файлы</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="113"/>
        <source>Generate Certificates</source>
        <translation>Создать сертификаты</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="115"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="129"/>
        <source>Select Output Directory</source>
        <translation>Выбор выходного каталога</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="164"/>
        <source>Failed to create key context: %1</source>
        <translation>Не удалось создать контекст ключа: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="169"/>
        <source>Failed to init keygen: %1</source>
        <translation>Не удалось инициализировать генерацию ключа: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="175"/>
        <source>Failed to set key bits: %1</source>
        <translation>Не удалось задать длину ключа: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="181"/>
        <source>Failed to generate key: %1</source>
        <translation>Не удалось сгенерировать ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="197"/>
        <source>Failed to create X509: %1</source>
        <translation>Не удалось создать X509: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="203"/>
        <source>Failed to set X509 version</source>
        <translation>Не удалось задать версию X509</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="211"/>
        <source>Failed to create serial number</source>
        <translation>Не удалось создать серийный номер</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="225"/>
        <source>Failed to set public key</source>
        <translation>Не удалось задать открытый ключ</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="233"/>
        <source>Failed to create X509_NAME</source>
        <translation>Не удалось создать X509_NAME</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="244"/>
        <source>Failed to add Organization to subject: %1</source>
        <translation>Не удалось добавить организацию в субъект: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="252"/>
        <source>Failed to add Common Name to subject: %1</source>
        <translation>Не удалось добавить общее имя в субъект: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="260"/>
        <source>Failed to set subject name: %1</source>
        <translation>Не удалось задать имя субъекта: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="270"/>
        <location filename="../src/bcertificategenerator.cpp" line="278"/>
        <source>Failed to set issuer name: %1</source>
        <translation>Не удалось задать имя издателя: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="311"/>
        <source>Failed to sign certificate: %1</source>
        <translation>Не удалось подписать сертификат: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="325"/>
        <location filename="../src/bcertificategenerator.cpp" line="346"/>
        <location filename="../src/bcertificategenerator.cpp" line="384"/>
        <source>Cannot open file for writing: %1</source>
        <translation>Не удалось открыть файл для записи: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="333"/>
        <source>Failed to write private key: %1</source>
        <translation>Не удалось записать закрытый ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="354"/>
        <source>Failed to write certificate: %1</source>
        <translation>Не удалось записать сертификат: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="376"/>
        <source>Failed to create PKCS12: %1</source>
        <translation>Не удалось создать PKCS12: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="394"/>
        <source>Failed to write PFX file: %1</source>
        <translation>Не удалось записать файл PFX: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="409"/>
        <source>Generating...</source>
        <translation>Создание...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="418"/>
        <source>Please fill in all required fields</source>
        <translation>Заполните все обязательные поля</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="420"/>
        <location filename="../src/bcertificategenerator.cpp" line="434"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="432"/>
        <source>Failed to create output directory</source>
        <translation>Не удалось создать выходной каталог</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="450"/>
        <source>Certificate files already exist. Enable &apos;Overwrite&apos; to replace.</source>
        <translation>Файлы сертификатов уже существуют. Включите «Перезаписывать», чтобы заменить их.</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="452"/>
        <source>Files exist</source>
        <translation>Файлы существуют</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="461"/>
        <source>Generating CA private key (4096 bit RSA)...</source>
        <translation>Генерация закрытого ключа CA (RSA 4096 бит)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="465"/>
        <location filename="../src/bcertificategenerator.cpp" line="476"/>
        <location filename="../src/bcertificategenerator.cpp" line="487"/>
        <location filename="../src/bcertificategenerator.cpp" line="496"/>
        <location filename="../src/bcertificategenerator.cpp" line="509"/>
        <location filename="../src/bcertificategenerator.cpp" line="522"/>
        <location filename="../src/bcertificategenerator.cpp" line="535"/>
        <location filename="../src/bcertificategenerator.cpp" line="546"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="472"/>
        <source>Saving CA private key...</source>
        <translation>Сохранение закрытого ключа CA...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="482"/>
        <source>Generating CA certificate...</source>
        <translation>Создание сертификата CA...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="503"/>
        <source>Generating client private key (4096 bit RSA)...</source>
        <translation>Генерация закрытого ключа клиента (RSA 4096 бит)...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="516"/>
        <source>Saving client private key...</source>
        <translation>Сохранение закрытого ключа клиента...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="528"/>
        <source>Generating client certificate...</source>
        <translation>Создание сертификата клиента...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="554"/>
        <source>Creating PFX file for Windows...</source>
        <translation>Создание файла PFX для Windows...</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="556"/>
        <source>PFX creation failed (non-critical)</source>
        <translation>Не удалось создать PFX (некритично)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="569"/>
        <source>Certificates generated successfully!</source>
        <translation>Сертификаты успешно созданы!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="570"/>
        <source>CA Certificate: %1</source>
        <translation>Сертификат CA: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="571"/>
        <source>Client Certificate: %1</source>
        <translation>Сертификат клиента: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="572"/>
        <source>Private Key: %1</source>
        <translation>Закрытый ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="574"/>
        <source>PFX File: %1</source>
        <translation>Файл PFX: %1</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="575"/>
        <source>PFX Password: (empty - no password)</source>
        <translation>Пароль PFX: (пусто — без пароля)</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="578"/>
        <source>Success!</source>
        <translation>Готово!</translation>
    </message>
    <message>
        <location filename="../src/bcertificategenerator.cpp" line="581"/>
        <source>Use Certificates</source>
        <translation>Использовать сертификаты</translation>
    </message>
</context>
<context>
    <name>BCheckableHeaderView</name>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="118"/>
        <source>Show All Columns</source>
        <translation>Показать все столбцы</translation>
    </message>
    <message>
        <location filename="../src/bcheckableheaderview.cpp" line="128"/>
        <source>Reset Columns</source>
        <translation>Сбросить столбцы</translation>
    </message>
</context>
<context>
    <name>BCleanupDialog</name>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="39"/>
        <source>Database Cleanup</source>
        <translation>Очистка базы данных</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="76"/>
        <source>The connection to the Director was lost</source>
        <translation>Соединение с Director потеряно</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="98"/>
        <source>This tool helps maintain your Bareos database by removing old or unnecessary data.
⚠️ Warning: Cleanup operations cannot be undone. Review options carefully before proceeding.</source>
        <translation>Этот инструмент помогает обслуживать базу данных Bareos, удаляя старые или ненужные данные.
⚠️ Внимание: операции очистки нельзя отменить. Внимательно проверьте параметры перед продолжением.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="108"/>
        <source>Cleanup Options</source>
        <translation>Параметры очистки</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="113"/>
        <source>Remove old full backups</source>
        <translation>Удалить старые полные резервные копии</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="116"/>
        <source>Delete old successful full backups of a job if newer full backups of the same client, job and FileSet exist. A full backup that incremental or differential backups still depend on is kept.</source>
        <translation>Удалять старые успешные полные резервные копии Job, если существуют более новые полные резервные копии того же клиента, Job и FileSet. Полная резервная копия, от которой ещё зависят инкрементные или дифференциальные копии, сохраняется.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="121"/>
        <source>Keep last:</source>
        <translation>Хранить последние:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="128"/>
        <source> full backups</source>
        <translation> полных копий</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="129"/>
        <source>Number of recent full backups to keep per client, job and FileSet</source>
        <translation>Количество последних полных резервных копий, сохраняемых для каждого клиента, Job и FileSet</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="136"/>
        <source>Remove successful jobs with no files</source>
        <translation>Удалить успешные Jobs без файлов</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="138"/>
        <source>Delete jobs that completed successfully but backed up 0 files</source>
        <translation>Удалять Jobs, завершившиеся успешно, но сохранившие 0 файлов</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="143"/>
        <source>Prune volumes</source>
        <translation>Prune для Volumes</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="145"/>
        <source>Apply retention policy to volumes (removes expired job records)</source>
        <translation>Применить политику хранения к Volumes (удаляет просроченные записи о Jobs)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="148"/>
        <source>Action:</source>
        <translation>Действие:</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="152"/>
        <source>Prune only</source>
        <translation>Только prune</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="153"/>
        <source>Prune and purge</source>
        <translation>Prune и purge</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="154"/>
        <source>Prune removes the expired job records of all volumes according to their retention period</source>
        <translation>Prune удаляет просроченные записи о Jobs всех Volumes согласно их сроку хранения</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="164"/>
        <source>Not available: Bareos has no command that purges all volumes at once. Purge works on named volumes only and ignores retention periods, so it is not sent from this dialog.</source>
        <translation>Недоступно: в Bareos нет команды, выполняющей purge для всех Volumes сразу. Purge работает только с указанными по имени Volumes и игнорирует сроки хранения, поэтому из этого диалога не отправляется.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="180"/>
        <source>Remove failed jobs older than</source>
        <translation>Удалить неудачные Jobs старше</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="182"/>
        <source>Delete jobs that failed or were canceled</source>
        <translation>Удалять Jobs, завершившиеся с ошибкой или отменённые</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="189"/>
        <source> days</source>
        <translation> дн.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="211"/>
        <source>Select cleanup options and click &apos;Analyze&apos; to preview</source>
        <translation>Выберите параметры очистки и нажмите «Анализ» для предварительного просмотра</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="216"/>
        <source>Analysis / Log</source>
        <translation>Анализ / Журнал</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="239"/>
        <source>Analyze</source>
        <translation>Анализ</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="240"/>
        <source>Preview what would be cleaned up</source>
        <translation>Предварительный просмотр того, что будет очищено</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="244"/>
        <source>Cleanup</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="246"/>
        <source>Perform cleanup operations (requires analysis first)</source>
        <translation>Выполнить операции очистки (сначала требуется анализ)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="234"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="333"/>
        <source>The options were changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>Параметры изменены. Приведённый выше анализ больше не действителен; нажмите «Анализ» ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="342"/>
        <source>The job list has changed. The analysis above is no longer valid; click &apos;Analyze&apos; again.</source>
        <translation>Список Jobs изменился. Приведённый выше анализ больше не действителен; нажмите «Анализ» ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="353"/>
        <location filename="../src/bcleanupdialog.cpp" line="714"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="354"/>
        <location filename="../src/bcleanupdialog.cpp" line="715"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Сначала подключитесь к Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="360"/>
        <source>No Options Selected</source>
        <translation>Параметры не выбраны</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="361"/>
        <source>Please select at least one cleanup option.</source>
        <translation>Выберите хотя бы один параметр очистки.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="369"/>
        <source>=== Analysis ===</source>
        <translation>=== Анализ ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="382"/>
        <location filename="../src/bcleanupdialog.cpp" line="431"/>
        <source>Analysis Required</source>
        <translation>Требуется анализ</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="383"/>
        <source>Please run analysis before cleanup.</source>
        <translation>Перед очисткой выполните анализ.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="390"/>
        <source>⚠️ This will permanently delete data from the Bareos catalog.

</source>
        <translation>⚠️ Данные будут безвозвратно удалены из каталога Bareos.

</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="391"/>
        <source>Operations to perform:
</source>
        <translation>Будут выполнены операции:
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="394"/>
        <source>- Delete %1 old full backup jobs (keeping %2 per client, job and FileSet)
</source>
        <translation>- Удалить %1 старых Jobs полного резервного копирования (сохраняя %2 на каждого клиента, Job и FileSet)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="400"/>
        <source>- Delete %1 empty jobs (successful with 0 files)
</source>
        <translation>- Удалить %1 пустых Jobs (успешных, с 0 файлов)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="405"/>
        <source>- Delete %1 failed jobs older than %2 days
</source>
        <translation>- Удалить %1 неудачных Jobs старше %2 дн.
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="412"/>
        <source>  Job records to delete in total: %1
</source>
        <translation>  Всего записей о Jobs к удалению: %1
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="416"/>
        <source>- Prune all volumes (command: %1)
</source>
        <translation>- Prune для всех Volumes (команда: %1)
</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="419"/>
        <source>
Do you want to continue?</source>
        <translation>
Продолжить?</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="422"/>
        <source>Confirm Cleanup</source>
        <translation>Подтверждение очистки</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="432"/>
        <source>The job list changed in the meantime. Nothing was deleted; please run the analysis again.</source>
        <translation>Тем временем список Jobs изменился. Ничего не удалено; выполните анализ ещё раз.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="445"/>
        <source>Stopping: no further commands are sent after the answer that is still awaited.</source>
        <translation>Остановка: после ожидаемого ещё ответа команды больше не отправляются.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="470"/>
        <source>Analyzing cleanup options...</source>
        <translation>Анализ параметров очистки...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="475"/>
        <source>⚠ No job data available!</source>
        <translation>⚠ Нет данных о Jobs!</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="476"/>
        <source>Please load the job list first (click &apos;Refresh&apos; in the main window).</source>
        <translation>Сначала загрузите список Jobs (нажмите «Обновить» в главном окне).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="478"/>
        <source>No job data. Please refresh job list first.</source>
        <translation>Нет данных о Jobs. Сначала обновите список Jobs.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="485"/>
        <source>Looking at the %1 jobs currently loaded in the job list. Jobs that are not loaded there are not considered.</source>
        <translation>Рассматриваются %1 Jobs, загруженных сейчас в список Jobs. Не загруженные туда Jobs не учитываются.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="502"/>
        <source>✓ Will prune all volumes (apply each volume&apos;s retention period)</source>
        <translation>✓ Будет выполнен prune для всех Volumes (применяется срок хранения каждого Volume)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="503"/>
        <source>  Command: %1</source>
        <translation>  Команда: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="505"/>
        <source>  &apos;Prune and purge&apos; is not available and will not be sent.</source>
        <translation>  «Prune и purge» недоступно и отправлено не будет.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="510"/>
        <source>=== Analysis Summary ===</source>
        <translation>=== Итоги анализа ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="514"/>
        <source>Total jobs to delete: %1</source>
        <translation>Всего Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="516"/>
        <source>  - Old full backups: %1 (%2)</source>
        <translation>  - Старые полные резервные копии: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="521"/>
        <source>  - Empty jobs: %1</source>
        <translation>  - Пустые Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="524"/>
        <source>  - Failed jobs: %1</source>
        <translation>  - Неудачные Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="527"/>
        <source>No jobs found matching cleanup criteria.</source>
        <translation>Jobs, соответствующие критериям очистки, не найдены.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="534"/>
        <source>Analysis complete. Click &apos;Cleanup&apos; to proceed.</source>
        <translation>Анализ завершён. Нажмите «Очистить», чтобы продолжить.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="536"/>
        <source>Analysis complete. Nothing to clean up.</source>
        <translation>Анализ завершён. Очищать нечего.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="550"/>
        <source>Analyzing old full backups (keeping the %1 most recent per client, job and FileSet)...</source>
        <translation>Анализ старых полных резервных копий (сохраняются %1 последних на каждого клиента, Job и FileSet)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="558"/>
        <source>at least one full backup must be kept</source>
        <translation>необходимо сохранить хотя бы одну полную резервную копию</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="561"/>
        <source>%1 job rows have no job ID</source>
        <translation>у %1 строк Jobs нет ID Job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="564"/>
        <source>%1 job rows have no job type, so backups cannot be told from other jobs</source>
        <translation>у %1 строк Jobs нет типа Job, поэтому резервные копии нельзя отличить от других Jobs</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="568"/>
        <source>%1 backup jobs have no client or job name</source>
        <translation>у %1 Jobs резервного копирования нет клиента или имени Job</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="573"/>
        <source>  ⚠ No old full backups are selected: %1.</source>
        <translation>  ⚠ Старые полные резервные копии не выбраны: %1.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="577"/>
        <source>  Note: the FileSet of a job is taken from its current job configuration, not from the catalog record of each run.</source>
        <translation>  Примечание: FileSet Job берётся из его текущей конфигурации, а не из записи каталога о каждом запуске.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="584"/>
        <source>%1 runs have no known FileSet</source>
        <translation>у %1 запусков FileSet неизвестен</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="587"/>
        <source>%1 runs have no status</source>
        <translation>у %1 запусков нет статуса</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="590"/>
        <source>%1 runs have a level other than Full, Differential or Incremental</source>
        <translation>у %1 запусков уровень отличается от Full, Differential или Incremental</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="593"/>
        <source>%1 runs have no readable start time</source>
        <translation>у %1 запусков время начала не читается</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="596"/>
        <source>  ⚠ Client &apos;%1&apos;, job &apos;%2&apos;: skipped, nothing selected (%3)</source>
        <translation>  ⚠ Client «%1», Job «%2»: пропущено, ничего не выбрано (%3)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="602"/>
        <source>  Client &apos;%1&apos;, job &apos;%2&apos;, FileSet &apos;%3&apos;: %4 full backups, will delete %5</source>
        <translation>  Client «%1», Job «%2», FileSet «%3»: полных резервных копий — %4, будет удалено %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="609"/>
        <source>    ... and %1 more</source>
        <translation>    ... и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="613"/>
        <source>    → Job %1: %2 (%3, %4)</source>
        <translation>    → Job %1: %2 (%3, %4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="616"/>
        <source>      its dependent jobs %1 are deleted by this cleanup as well</source>
        <translation>      зависящие от него Jobs %1 также удаляются этой очисткой</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="622"/>
        <source>    ⚠ Job %1 (%2) is kept: incremental/differential jobs %3 depend on it</source>
        <translation>    ⚠ Job %1 (%2) сохраняется: от него зависят инкрементные/дифференциальные Jobs %3</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="629"/>
        <source>✓ Found %1 old full backups to delete (%2)</source>
        <translation>✓ Найдено старых полных резервных копий к удалению: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="633"/>
        <source>  No old full backups found to delete</source>
        <translation>  Старые полные резервные копии к удалению не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="639"/>
        <source>Analyzing empty jobs (successful with 0 files)...</source>
        <translation>Анализ пустых Jobs (успешных, с 0 файлов)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="644"/>
        <source>  → Job %1: %2 [%3] (%4)</source>
        <translation>  → Job %1: %2 [%3] (%4)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="649"/>
        <location filename="../src/bcleanupdialog.cpp" line="687"/>
        <source>  ... and %1 more</source>
        <translation>  ... и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="653"/>
        <source>  %1 successful jobs have no file count and are not selected</source>
        <translation>  у %1 успешных Jobs нет количества файлов, они не выбраны</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="658"/>
        <source>✓ Found %1 empty jobs to delete</source>
        <translation>✓ Найдено пустых Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="660"/>
        <source>  No empty jobs found</source>
        <translation>  Пустые Jobs не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="668"/>
        <source>Analyzing failed jobs older than %1 days (before %2)...</source>
        <translation>Анализ неудачных Jobs старше %1 дн. (до %2)...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="677"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="678"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="679"/>
        <source>Canceled</source>
        <translation>Отменено</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="680"/>
        <source>Non-fatal error</source>
        <translation>Нефатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="682"/>
        <source>  → Job %1: %2 [%3] (%4) - %5</source>
        <translation>  → Job %1: %2 [%3] (%4) - %5</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="691"/>
        <source>✓ Found %1 failed jobs to delete</source>
        <translation>✓ Найдено неудачных Jobs к удалению: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="693"/>
        <source>  No failed jobs found matching criteria</source>
        <translation>  Неудачные Jobs, соответствующие критериям, не найдены</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702"/>
        <source>Stop</source>
        <translation>Остановить</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="702"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="720"/>
        <source>=== Starting Cleanup ===</source>
        <translation>=== Начало очистки ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="727"/>
        <source>Cleanup running: commands are sent one at a time...</source>
        <translation>Выполняется очистка: команды отправляются по одной...</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="737"/>
        <source>Sent: %1</source>
        <translation>Отправлено: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="749"/>
        <source>(empty answer)</source>
        <translation>(пустой ответ)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="754"/>
        <source>✓ Job %1: deletion confirmed by the Director</source>
        <translation>✓ Job %1: удаление подтверждено Director</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="757"/>
        <source>✗ Job %1: the Director reported an error: %2</source>
        <translation>✗ Job %1: Director сообщил об ошибке: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="760"/>
        <source>? Job %1: the Director did not confirm the deletion. Answer: %2</source>
        <translation>? Job %1: Director не подтвердил удаление. Ответ: %2</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="764"/>
        <source>? Job %1: result unknown (%2)</source>
        <translation>? Job %1: результат неизвестен (%2)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="772"/>
        <source>Prune volumes: the Director answered without an error: %1</source>
        <translation>Prune для Volumes: Director ответил без ошибки: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="775"/>
        <source>✗ Prune volumes: the Director reported an error: %1</source>
        <translation>✗ Prune для Volumes: Director сообщил об ошибке: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="778"/>
        <source>? Prune volumes: result unknown (%1)</source>
        <translation>? Prune для Volumes: результат неизвестен (%1)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="781"/>
        <source>? Prune volumes: no usable answer: %1</source>
        <translation>? Prune для Volumes: нет пригодного ответа: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="791"/>
        <source>no answer from the Director within the time limit</source>
        <translation>нет ответа от Director в отведённое время</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="806"/>
        <source>=== Cleanup Result ===</source>
        <translation>=== Результат очистки ===</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="807"/>
        <source>Job deletions confirmed by the Director: %1</source>
        <translation>Удаления Jobs, подтверждённые Director: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="810"/>
        <source>Prune volumes: answered without an error (see the answer above)</source>
        <translation>Prune для Volumes: ответ без ошибки (см. ответ выше)</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="813"/>
        <source>Commands the Director answered with an error: %1</source>
        <translation>Команды, на которые Director ответил ошибкой: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="816"/>
        <source>Commands with an unknown result: %1. Refresh the job list to see the actual state.</source>
        <translation>Команды с неизвестным результатом: %1. Обновите список Jobs, чтобы увидеть фактическое состояние.</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="820"/>
        <source>Commands not sent because the run was stopped: %1</source>
        <translation>Команды, не отправленные из-за остановки: %1</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="824"/>
        <source>Cleanup finished: the Director confirmed all %1 commands.</source>
        <translation>Очистка завершена: Director подтвердил все команды (%1).</translation>
    </message>
    <message>
        <location filename="../src/bcleanupdialog.cpp" line="827"/>
        <source>Cleanup did not complete: %1 confirmed, %2 failed, %3 unknown, %4 not sent. See the log.</source>
        <translation>Очистка не завершена: подтверждено %1, с ошибкой %2, неизвестно %3, не отправлено %4. См. журнал.</translation>
    </message>
</context>
<context>
    <name>BClientDetailsDialog</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="189"/>
        <source>Client Details - %1</source>
        <translation>Сведения о Client — %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="330"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="216"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="316"/>
        <source>General Information</source>
        <translation>Общие сведения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="323"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="325"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="395"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="331"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="332"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="333"/>
        <source>Version:</source>
        <translation>Версия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="334"/>
        <source>Operating System:</source>
        <translation>Операционная система:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="339"/>
        <source>Connection Information</source>
        <translation>Сведения о соединении</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="353"/>
        <source>Recent (%1)</source>
        <translation>Недавно (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="358"/>
        <source>Stale (%1)</source>
        <translation>Устарело (%1)</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="362"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="370"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="365"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="383"/>
        <source>Last backup:</source>
        <translation>Последняя резервная копия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="384"/>
        <source>Last Connection:</source>
        <translation>Последнее соединение:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="389"/>
        <source>Retention Policies</source>
        <translation>Политики хранения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="394"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="399"/>
        <source>Auto Prune:</source>
        <translation>Auto Prune:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="400"/>
        <source>File Retention:</source>
        <translation>File Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="401"/>
        <source>Job Retention:</source>
        <translation>Job Retention:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="407"/>
        <source>Information</source>
        <translation>Сведения</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="426"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="459"/>
        <source>Recent jobs for this client:</source>
        <translation>Последние Jobs этого клиента:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="463"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="502"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="525"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="511"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="512"/>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="514"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="516"/>
        <source>Total Jobs:</source>
        <translation>Всего Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="517"/>
        <source>Successful Jobs:</source>
        <translation>Успешных Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="518"/>
        <source>Failed Jobs:</source>
        <translation>Неудачных Jobs:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="519"/>
        <source>Total Data:</source>
        <translation>Всего данных:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="520"/>
        <source>Total Files:</source>
        <translation>Всего файлов:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="494"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
</context>
<context>
    <name>BClientJobsModel</name>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="79"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="80"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="81"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="90"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="91"/>
        <source>Running</source>
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="92"/>
        <source>Blocked</source>
        <translation>Заблокирован</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="93"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="94"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="95"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="96"/>
        <source>Canceled</source>
        <translation>Отменён</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="97"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="126"/>
        <source>Job ID</source>
        <translation>ID Job</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="127"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="128"/>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="129"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="130"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="131"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="132"/>
        <source>Bytes</source>
        <translation>Байты</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="133"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
</context>
<context>
    <name>BClientResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="196"/>
        <source>Catalog</source>
        <translation>Catalog</translation>
    </message>
</context>
<context>
    <name>BClientWidget</name>
    <message>
        <location filename="../ui/bclientwidget.ui" line="14"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="29"/>
        <source>Show state</source>
        <translation>Показать состояние</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="30"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientwidget.cpp" line="46"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
</context>
<context>
    <name>BClientsModel</name>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="132"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="193"/>
        <source>Client: %1</source>
        <translation>Client: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="197"/>
        <source>Address: %1:%2</source>
        <translation>Адрес: %1:%2</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="207"/>
        <source>%1 bytes</source>
        <translation>%1 байт</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="226"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="228"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="19"/>
        <source>No connection to Director</source>
        <translation>Нет соединения с Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="24"/>
        <source>Loading clients...</source>
        <translation>Загрузка клиентов...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="201"/>
        <source>FD Port: %1</source>
        <translation>Порт FD: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="230"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="232"/>
        <source>Last Backup</source>
        <translation>Последняя резервная копия</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="234"/>
        <source>Operating System</source>
        <translation>Операционная система</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="236"/>
        <source>Version</source>
        <translation>Версия</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="238"/>
        <source>Last Connection</source>
        <translation>Последнее соединение</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="240"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientsmodel.cpp" line="242"/>
        <source>Total Data</source>
        <translation>Всего данных</translation>
    </message>
</context>
<context>
    <name>BClientsWidget</name>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="35"/>
        <source>Auto</source>
        <translation>Авто</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="36"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="57"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="60"/>
        <source>Filter by client...</source>
        <translation>Фильтр по клиенту...</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="67"/>
        <source>Last backup:</source>
        <translation>Последняя резервная копия:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="68"/>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="69"/>
        <source>Recent</source>
        <translation>Недавно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="70"/>
        <source>Stale</source>
        <translation>Устарело</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="71"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="72"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="79"/>
        <source>Enable automatic refresh</source>
        <translation>Включить автообновление</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="92"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="95"/>
        <source>Total:</source>
        <translation>Всего:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="116"/>
        <source>Recent:</source>
        <translation>Недавно:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="117"/>
        <source>Clients whose last backup or contact is younger than the limit in the settings</source>
        <translation>Клиенты, у которых последняя резервная копия или контакт новее порога, заданного в настройках</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="119"/>
        <source>Stale:</source>
        <translation>Устарело:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="120"/>
        <source>Clients whose last backup or contact is older than the limit in the settings</source>
        <translation>Клиенты, у которых последняя резервная копия или контакт старше порога, заданного в настройках</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="122"/>
        <source>Never:</source>
        <translation>Никогда:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="123"/>
        <source>Clients without any known backup or contact</source>
        <translation>Клиенты без известных резервных копий или контактов</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="125"/>
        <source>Unknown:</source>
        <translation>Неизвестно:</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="126"/>
        <source>Clients whose last backup time cannot be read</source>
        <translation>Клиенты, время последней резервной копии которых не удаётся прочитать</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="387"/>
        <source>Error parsing client data</source>
        <translation>Ошибка разбора данных клиентов</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientswidget.cpp" line="428"/>
        <source>%n client(s) loaded</source>
        <translation>
            <numerusform>Загружен %n клиент</numerusform>
            <numerusform>Загружено %n клиента</numerusform>
            <numerusform>Загружено %n клиентов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/clients/bclientswidget.cpp" line="538"/>
        <source>Details / Export...</source>
        <translation>Подробности / Экспорт...</translation>
    </message>
</context>
<context>
    <name>BConfigImportDialog</name>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="34"/>
        <source>Import Director Configuration</source>
        <translation>Импорт конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="60"/>
        <source>Configuration Source</source>
        <translation>Источник конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="64"/>
        <source>Select directory or ZIP file...</source>
        <translation>Выберите каталог или ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="68"/>
        <source>Directory...</source>
        <translation>Каталог...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="73"/>
        <source>ZIP File...</source>
        <translation>ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="78"/>
        <source>Parse</source>
        <translation>Разобрать</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="101"/>
        <source>Connection Preview</source>
        <translation>Предпросмотр соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="105"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="172"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="110"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="201"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="114"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="208"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="118"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="186"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="122"/>
        <source>TLS:</source>
        <translation>TLS:</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="138"/>
        <source>Import</source>
        <translation>Импорт</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="145"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="169"/>
        <source>Connection Settings</source>
        <translation>Параметры соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="203"/>
        <source>hostname or IP address</source>
        <translation>имя хоста или IP-адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="204"/>
        <source>Enter the hostname or IP address of the Director to connect to</source>
        <translation>Введите имя хоста или IP-адрес Director, к которому нужно подключиться</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="212"/>
        <source>Port number (default: 9101)</source>
        <translation>Номер порта (по умолчанию: 9101)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="215"/>
        <source>Create connection profile</source>
        <translation>Создать профиль соединения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="220"/>
        <source>Copy Debug Info</source>
        <translation>Копировать отладочные сведения</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="221"/>
        <source>Copy parsed resource details to clipboard for debugging</source>
        <translation>Скопировать сведения о разобранных ресурсах в буфер обмена для отладки</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="254"/>
        <source>Debug Info Copied</source>
        <translation>Отладочные сведения скопированы</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="255"/>
        <source>Debug information has been copied to clipboard.

Paste it somewhere to view.</source>
        <translation>Отладочные сведения скопированы в буфер обмена.

Вставьте их куда-нибудь для просмотра.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="260"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="465"/>
        <source>Director</source>
        <translation>Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="265"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="466"/>
        <source>Console</source>
        <translation>Console</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="268"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="467"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="271"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="468"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="274"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="469"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="277"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="470"/>
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="280"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="471"/>
        <source>Pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="283"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="472"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="286"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="473"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="289"/>
        <location filename="../src/director/bconfigimportdialog.cpp" line="474"/>
        <source>Catalogs</source>
        <translation>Catalogs</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="300"/>
        <source>Select Director Configuration Directory</source>
        <translation>Выбор каталога конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="307"/>
        <source>Ready to parse directory</source>
        <translation>Каталог готов к разбору</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="321"/>
        <source>Select Configuration ZIP Archive</source>
        <translation>Выбор ZIP-архива конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="323"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="328"/>
        <source>Ready to parse ZIP archive</source>
        <translation>ZIP-архив готов к разбору</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="343"/>
        <source>Parsing...</source>
        <translation>Разбор...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="357"/>
        <source>Failed to create temporary directory</source>
        <translation>Не удалось создать временный каталог</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="361"/>
        <source>Extracting ZIP archive...</source>
        <translation>Распаковка ZIP-архива...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="369"/>
        <source>Failed to extract ZIP archive (timeout)</source>
        <translation>Не удалось распаковать ZIP-архив (тайм-аут)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="374"/>
        <source>Failed to extract ZIP archive: %1</source>
        <translation>Не удалось распаковать ZIP-архив: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="394"/>
        <source>Parsing extracted files...</source>
        <translation>Разбор распакованных файлов...</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="404"/>
        <source>Parsed %1: %2</source>
        <translation>Разобрано %1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="411"/>
        <source>Parsing complete: %1 resources found</source>
        <translation>Разбор завершён: найдено ресурсов: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="441"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="444"/>
        <source>Parsing Error</source>
        <translation>Ошибка разбора</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="503"/>
        <source>(multiple addresses)</source>
        <translation>(несколько адресов)</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="510"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="510"/>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="569"/>
        <source>Invalid Selection</source>
        <translation>Недопустимый выбор</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="570"/>
        <source>Please select both a Director and a Console.</source>
        <translation>Выберите и Director, и Console.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="575"/>
        <source>Invalid Address</source>
        <translation>Недопустимый адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="576"/>
        <source>Please enter the hostname or IP address of the Director.</source>
        <translation>Введите имя хоста или IP-адрес Director.</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="614"/>
        <source>%1 &apos;%2&apos;: Missing required field &apos;%3&apos;</source>
        <translation>%1 «%2»: отсутствует обязательное поле «%3»</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="635"/>
        <source>&lt;b&gt;Validation Warnings (%1 total, showing first 10):&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Предупреждения проверки (всего %1, показаны первые 10):&lt;/b&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/director/bconfigimportdialog.cpp" line="637"/>
        <source>&lt;b&gt;Validation Warnings:&lt;/b&gt;&lt;br&gt;</source>
        <translation>&lt;b&gt;Предупреждения проверки:&lt;/b&gt;&lt;br&gt;</translation>
    </message>
</context>
<context>
    <name>BConnectionWizard</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="57"/>
        <source>Connection Setup</source>
        <translation>Настройка соединения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="84"/>
        <source>New Connection</source>
        <translation>Новое соединение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="194"/>
        <source>SHA-256 fingerprint of the server certificate:</source>
        <translation>Отпечаток SHA-256 сертификата сервера:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="201"/>
        <source>Server certificate changed</source>
        <translation>Сертификат сервера изменился</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="202"/>
        <source>The certificate of the Director at %1:%2 has changed since the last connection.</source>
        <translation>Сертификат Director по адресу %1:%2 изменился с момента последнего подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="204"/>
        <source>Previously trusted:</source>
        <translation>Ранее доверенный:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="206"/>
        <source>This is expected if the certificate was regenerated on the Director. Otherwise someone may be intercepting the connection.</source>
        <translation>Это ожидаемо, если сертификат был заново создан на Director. В противном случае кто-то может перехватывать соединение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="209"/>
        <source>Trust server certificate?</source>
        <translation>Доверять сертификату сервера?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="210"/>
        <source>The Director at %1:%2 uses a self-signed certificate that has not been trusted yet.</source>
        <translation>Director по адресу %1:%2 использует самоподписанный сертификат, которому ещё не выражено доверие.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="216"/>
        <source>The same fingerprint is announced in the local network (mDNS). This is not proof of the Director&apos;s identity: any device in the network can send such an announcement.</source>
        <translation>Тот же отпечаток объявлен в локальной сети (mDNS). Это не доказывает подлинность Director: такое объявление может отправить любое устройство в сети.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="221"/>
        <source>The fingerprint does NOT match the one announced in the network (mDNS)!</source>
        <translation>Отпечаток НЕ совпадает с объявленным в сети (mDNS)!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="224"/>
        <source>Before trusting it, compare the fingerprint with the one of the Director&apos;s gRPC certificate, obtained from the Director&apos;s administrator or read on the Director itself.</source>
        <translation>Прежде чем доверять, сравните отпечаток с отпечатком сертификата gRPC Director, полученным от администратора Director или прочитанным на самом Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="231"/>
        <source>Trust certificate</source>
        <translation>Доверять сертификату</translation>
    </message>
</context>
<context>
    <name>BConsoleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="109"/>
        <source>ACLs</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="122"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="130"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BDatabase</name>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="37"/>
        <source>Failed to create database directory: %1</source>
        <translation>Не удалось создать каталог базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="47"/>
        <source>Failed to open database: %1</source>
        <translation>Не удалось открыть базу данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="68"/>
        <source>Failed to create database schema: %1</source>
        <translation>Не удалось создать схему базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="88"/>
        <source>Failed to apply database migrations: %1</source>
        <translation>Не удалось применить миграции базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="100"/>
        <source>Failed to repair database schema: %1</source>
        <translation>Не удалось восстановить схему базы данных: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="206"/>
        <source>No migration script for database version %1</source>
        <translation>Нет сценария миграции для версии базы данных %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="221"/>
        <source>Failed to apply migration to version %1: %2</source>
        <translation>Не удалось применить миграцию до версии %1: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="226"/>
        <source>Migration to version %1 did not record its version</source>
        <translation>Миграция до версии %1 не записала свою версию</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="257"/>
        <source>Table %1 is still missing after the repair</source>
        <translation>Таблица %1 по-прежнему отсутствует после восстановления</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="270"/>
        <source>Failed to open SQL resource: %1</source>
        <translation>Не удалось открыть ресурс SQL: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="286"/>
        <source>Failed to start transaction: %1</source>
        <translation>Не удалось начать транзакцию: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="317"/>
        <source>SQL execution failed: %1
Statement: %2</source>
        <translation>Ошибка выполнения SQL: %1
Оператор: %2</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="331"/>
        <source>Failed to commit transaction: %1</source>
        <translation>Не удалось зафиксировать транзакцию: %1</translation>
    </message>
    <message>
        <location filename="../src/db/bdatabase.cpp" line="482"/>
        <source>Database not ready</source>
        <translation>База данных не готова</translation>
    </message>
</context>
<context>
    <name>BDirectiveSchema</name>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="259"/>
        <source>Required directive &apos;%1&apos; is missing</source>
        <translation>Отсутствует обязательная директива «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="271"/>
        <source>&apos;%1&apos; must be an integer</source>
        <translation>«%1» должно быть целым числом</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="274"/>
        <source>&apos;%1&apos; must be &gt;= %2</source>
        <translation>«%1» должно быть &gt;= %2</translation>
    </message>
    <message>
        <location filename="../src/config/bdirectiveschema.cpp" line="277"/>
        <source>&apos;%1&apos; must be &lt;= %2</source>
        <translation>«%1» должно быть &lt;= %2</translation>
    </message>
</context>
<context>
    <name>BDirectorModel</name>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="23"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="24"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="25"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="26"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="27"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="28"/>
        <source>Description</source>
        <translation>Описание</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="29"/>
        <source>DIR Port</source>
        <translation>Порт DIR</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="30"/>
        <source>Query File</source>
        <translation>Query File</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="31"/>
        <source>Working Dir</source>
        <translation>Working Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="32"/>
        <source>PID Dir</source>
        <translation>PID Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="33"/>
        <source>Scripts Dir</source>
        <translation>Scripts Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="34"/>
        <source>Plugin Dir</source>
        <translation>Plugin Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="35"/>
        <source>SubSys Dir</source>
        <translation>SubSys Dir</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="36"/>
        <source>Max Jobs</source>
        <translation>Max Jobs</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="37"/>
        <source>TLS Enable</source>
        <translation>TLS Enable</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="38"/>
        <source>TLS Require</source>
        <translation>TLS Require</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="39"/>
        <source>TLS Verify</source>
        <translation>TLS Verify</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="40"/>
        <source>TLS CA Cert</source>
        <translation>TLS CA Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="41"/>
        <source>TLS Cert</source>
        <translation>TLS Cert</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="42"/>
        <source>TLS Key</source>
        <translation>TLS Key</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="43"/>
        <source>TLS Allowed CN</source>
        <translation>TLS Allowed CN</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="44"/>
        <source>System</source>
        <translation>Система</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="45"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="46"/>
        <source>Updated</source>
        <translation>Обновлён</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="47"/>
        <source>Last Connected</source>
        <translation>Последнее подключение</translation>
    </message>
    <message>
        <location filename="../src/db/bdirectormodel.cpp" line="48"/>
        <source>Active</source>
        <translation>Активен</translation>
    </message>
</context>
<context>
    <name>BDirectorResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="19"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="36"/>
        <source>(multiple)</source>
        <translation>(несколько)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="45"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BEditableListModel</name>
    <message>
        <location filename="../src/config/beditablelistmodel.cpp" line="120"/>
        <source>Add %1 items</source>
        <translation>Добавить элементы (%1)</translation>
    </message>
</context>
<context>
    <name>BEditableListWidget</name>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="77"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="110"/>
        <source>Add...</source>
        <translation>Добавить...</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="78"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="123"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="115"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="246"/>
        <source>Add item</source>
        <translation>Добавить элемент</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="251"/>
        <source>Remove selected</source>
        <translation>Удалить выбранное</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="390"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="447"/>
        <source>Select Directory</source>
        <translation>Выбор каталога</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="392"/>
        <location filename="../src/config/beditablelistwidget.cpp" line="449"/>
        <source>Select File</source>
        <translation>Выбор файла</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="397"/>
        <source>Add Item</source>
        <translation>Добавление элемента</translation>
    </message>
    <message>
        <location filename="../src/config/beditablelistwidget.cpp" line="398"/>
        <source>Enter value:</source>
        <translation>Введите значение:</translation>
    </message>
</context>
<context>
    <name>BFileSetDocument</name>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="755"/>
        <source>FileSet name is required</source>
        <translation>Требуется имя FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="761"/>
        <source>At least one Include block is required</source>
        <translation>Требуется хотя бы один блок Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="769"/>
        <source>Include block %1 has no file paths</source>
        <translation>В блоке Include %1 нет путей к файлам</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bfilesetdocument.cpp" line="774"/>
        <source>At least one Include block must have file paths</source>
        <translation>Хотя бы один блок Include должен содержать пути к файлам</translation>
    </message>
</context>
<context>
    <name>BFileSetExcludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1008"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1023"/>
        <source>Global Exclude</source>
        <translation>Глобальное исключение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1009"/>
        <source>Configure paths and patterns to exclude from all Include blocks.</source>
        <translation>Настройте пути и шаблоны, исключаемые из всех блоков Include.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1019"/>
        <source>Exclude Paths</source>
        <translation>Исключаемые пути</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1024"/>
        <source>Enter path to exclude...</source>
        <translation>Введите исключаемый путь...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1025"/>
        <source>+ Add Path</source>
        <translation>+ Добавить путь</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1031"/>
        <source>These paths will be excluded from all backup jobs using this FileSet.
Use absolute paths (e.g., /tmp, /var/cache) or patterns.</source>
        <translation>Эти пути будут исключены из всех Jobs резервного копирования, использующих этот FileSet.
Используйте абсолютные пути (напр., /tmp, /var/cache) или шаблоны.</translation>
    </message>
</context>
<context>
    <name>BFileSetIncludePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="901"/>
        <source>Include Blocks</source>
        <translation>Блоки Include</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="902"/>
        <source>Configure which files and directories to include in the backup.</source>
        <translation>Настройте, какие файлы и каталоги включать в резервную копию.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="915"/>
        <source>+ Add Include Block</source>
        <translation>+ Добавить блок Include</translation>
    </message>
</context>
<context>
    <name>BFileSetPreviewPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1064"/>
        <source>Preview &amp; Deploy</source>
        <translation>Предпросмотр и развёртывание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1065"/>
        <source>Review the generated FileSet configuration. Copy or export it for manual deployment.</source>
        <translation>Проверьте созданную конфигурацию FileSet. Скопируйте или экспортируйте её для ручного развёртывания.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1077"/>
        <source>Configuration:</source>
        <translation>Конфигурация:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1083"/>
        <source>Copy configuration</source>
        <translation>Копировать конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1141"/>
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
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1176"/>
        <source>Configuration is valid</source>
        <translation>Конфигурация корректна</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1190"/>
        <source>Close Wizard</source>
        <translation>Закрыть мастер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1191"/>
        <source>The configuration has not been sent to the Director.
Use &apos;Copy configuration&apos; to save it before closing.

Do you want to close the wizard?</source>
        <translation>Конфигурация не была отправлена на Director.
Используйте «Копировать конфигурацию», чтобы сохранить её перед закрытием.

Закрыть мастер?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1204"/>
        <source>Executing configure command...</source>
        <translation>Выполнение команды configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1242"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1278"/>
        <source>FileSet created successfully!</source>
        <translation>FileSet успешно создан!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1254"/>
        <source>Unknown error</source>
        <translation>Неизвестная ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1256"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1288"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1296"/>
        <source>Timeout</source>
        <translation>Тайм-аут</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1297"/>
        <source>The configure command timed out.
Do you want to retry?</source>
        <translation>Истекло время ожидания команды configure.
Повторить?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1303"/>
        <source>Command timed out</source>
        <translation>Истекло время ожидания команды</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1313"/>
        <source>Configuration copied to clipboard</source>
        <translation>Конфигурация скопирована в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1325"/>
        <source>ZIP Archives (*.zip);;Configuration Files (*.conf);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1328"/>
        <source>Export FileSet Configuration</source>
        <translation>Экспорт конфигурации FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1348"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1380"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1394"/>
        <source>Export Error</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1349"/>
        <source>Cannot create zip file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1381"/>
        <source>Error writing zip file</source>
        <translation>Ошибка записи ZIP-файла</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1385"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1402"/>
        <source>Exported to: %1</source>
        <translation>Экспортировано в: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="1395"/>
        <source>Cannot write file: %1</source>
        <translation>Не удалось записать файл: %1</translation>
    </message>
</context>
<context>
    <name>BFileSetResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="354"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BFileSetSettingsPage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="616"/>
        <source>FileSet Settings</source>
        <translation>Настройки FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="617"/>
        <source>Configure the FileSet name and basic options.</source>
        <translation>Задайте имя FileSet и основные параметры.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="631"/>
        <source>Select FileSet:</source>
        <translation>Выберите FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="634"/>
        <source>Loading FileSets...</source>
        <translation>Загрузка FileSets...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="651"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="655"/>
        <source>e.g., LinuxFullSystem</source>
        <translation>напр., LinuxFullSystem</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="656"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="659"/>
        <source>Optional description</source>
        <translation>Необязательное описание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="660"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="665"/>
        <source>Global Options</source>
        <translation>Глобальные параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="668"/>
        <source>Enable VSS</source>
        <translation>Включить VSS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="669"/>
        <source>Enable Volume Shadow Copy Service (Windows)</source>
        <translation>Включить службу теневого копирования томов (Windows)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="672"/>
        <source>Ignore FileSet Changes</source>
        <translation>Игнорировать изменения FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="673"/>
        <source>Don&apos;t trigger Full backup when FileSet changes</source>
        <translation>Не запускать резервное копирование Full при изменении FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="676"/>
        <source>Enable Snapshot</source>
        <translation>Включить снимки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="677"/>
        <source>Enable snapshot support (Linux LVM/btrfs)</source>
        <translation>Включить поддержку снимков (Linux LVM/btrfs)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="699"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="755"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="775"/>
        <source>No Director connection</source>
        <translation>Нет соединения с Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="829"/>
        <source>-- Select FileSet --</source>
        <translation>-- Выберите FileSet --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="835"/>
        <source>(%1 FileSets)</source>
        <translation>(FileSets: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="877"/>
        <source>Loaded</source>
        <translation>Загружено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="891"/>
        <source>Config not cached</source>
        <translation>Конфигурация не кэширована</translation>
    </message>
</context>
<context>
    <name>BFileSetTemplatePage</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="165"/>
        <source>Select FileSet Template</source>
        <translation>Выбор шаблона FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="166"/>
        <source>Choose a preset template to start with, or mix multiple templates together.</source>
        <translation>Выберите готовый шаблон для начала или объедините несколько шаблонов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="175"/>
        <source>Template:</source>
        <translation>Шаблон:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="178"/>
        <source>Mix Templates...</source>
        <translation>Объединить шаблоны...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="180"/>
        <source>Combine paths from multiple templates</source>
        <translation>Объединить пути из нескольких шаблонов</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="192"/>
        <source>Template Description</source>
        <translation>Описание шаблона</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="206"/>
        <source>Paths Preview</source>
        <translation>Предпросмотр путей</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="211"/>
        <source>&lt;b&gt;Include:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Включить:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="224"/>
        <source>&lt;b&gt;Exclude:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Исключить:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="393"/>
        <source>Platform: Windows</source>
        <translation>Платформа: Windows</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="395"/>
        <source>Platform: Linux/Unix</source>
        <translation>Платформа: Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="397"/>
        <source>Platform: macOS</source>
        <translation>Платформа: macOS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="399"/>
        <source>Platform: Any</source>
        <translation>Платформа: любая</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="412"/>
        <source>Mix Templates</source>
        <translation>Объединение шаблонов</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="417"/>
        <source>Select templates to combine:</source>
        <translation>Выберите шаблоны для объединения:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="435"/>
        <source>%1
Platform: %2</source>
        <translation>%1
Платформа: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="442"/>
        <source>Combined Paths Preview</source>
        <translation>Предпросмотр объединённых путей</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="491"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="493"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="601"/>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="605"/>
        <source>(none)</source>
        <translation>(нет)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="570"/>
        <source>Mixed_%1</source>
        <translation>Mixed_%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="571"/>
        <source>Combined FileSet from %1 templates</source>
        <translation>Объединённый FileSet из шаблонов: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="573"/>
        <source>Mixed: %1 templates selected</source>
        <translation>Объединение: выбрано шаблонов: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="577"/>
        <source>Templates: %1</source>
        <translation>Шаблоны: %1</translation>
    </message>
</context>
<context>
    <name>BFileSetWizard</name>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80"/>
        <source>New FileSet Wizard</source>
        <translation>Мастер создания FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="80"/>
        <source>Edit FileSet</source>
        <translation>Изменить FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bfilesetwizard.cpp" line="103"/>
        <source>Finish</source>
        <translation>Готово</translation>
    </message>
</context>
<context>
    <name>BGrpcAddressProbe</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="167"/>
        <source>no TLS connection within %1 s</source>
        <translation>нет соединения TLS в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="204"/>
        <source>Cannot resolve the host name %1: %2</source>
        <translation>Не удалось разрешить имя хоста %1: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="205"/>
        <source>no address</source>
        <translation>нет адреса</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="229"/>
        <source>Cannot resolve the host name %1: no address</source>
        <translation>Не удалось разрешить имя хоста %1: нет адреса</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="242"/>
        <source>Cannot reach the gRPC endpoint %1:%2 (%3)</source>
        <translation>Конечная точка gRPC %1:%2 недоступна (%3)</translation>
    </message>
</context>
<context>
    <name>BGrpcConnect</name>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="83"/>
        <source>%1 - the Director no longer knows this session (it ends after 8 hours without a request or when the Director restarts). Connect again.</source>
        <translation>%1 — Director больше не знает этот сеанс (он завершается через 8 часов без запросов или при перезапуске Director). Подключитесь снова.</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="90"/>
        <source>the call was cancelled</source>
        <translation>вызов был отменён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="94"/>
        <source>no connection or no reply within %1 s</source>
        <translation>нет соединения или ответа в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="95"/>
        <source>no connection or no reply in time</source>
        <translation>нет соединения или ответа вовремя</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="98"/>
        <source>this console is not allowed to do that</source>
        <translation>этой консоли это не разрешено</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="101"/>
        <source>the Director does not offer this method (an older version?)</source>
        <translation>Director не предоставляет этот метод (старая версия?)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="104"/>
        <source>the gRPC service of the Director cannot be reached</source>
        <translation>служба gRPC Director недоступна</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="107"/>
        <source>not logged in, or the session has expired</source>
        <translation>вход не выполнен или сеанс истёк</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="111"/>
        <source>the Director reported an error without a message</source>
        <translation>Director сообщил об ошибке без текста</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="114"/>
        <source>no reason given</source>
        <translation>причина не указана</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpcconnect.cpp" line="126"/>
        <source>The Director does not offer the expected gRPC service (protocol version mismatch: this build speaks %1; the Director may run an older or newer build). Use BNET or update the Director or this application.</source>
        <translation>Director не предоставляет ожидаемую службу gRPC (несовпадение версии протокола: эта сборка использует %1; на Director может работать более старая или новая сборка). Используйте BNET или обновите Director либо это приложение.</translation>
    </message>
</context>
<context>
    <name>BGrpcTransport</name>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="195"/>
        <location filename="../src/director/bgrpctransport.cpp" line="228"/>
        <source>gRPC login failed: %1</source>
        <translation>Ошибка входа gRPC: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="203"/>
        <source>gRPC login failed: no challenge received</source>
        <translation>Ошибка входа gRPC: запрос (challenge) не получен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="236"/>
        <source>Authentication failed: %1</source>
        <translation>Ошибка аутентификации: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="237"/>
        <source>invalid response</source>
        <translation>недопустимый ответ</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="295"/>
        <source>Client %1 connected</source>
        <translation>Client %1 подключён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="297"/>
        <source>Client %1 disconnected</source>
        <translation>Client %1 отключён</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="300"/>
        <source>Client %1 is alive</source>
        <translation>Client %1 доступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="301"/>
        <source>Client %1 not reachable</source>
        <translation>Client %1 недоступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="303"/>
        <source>Storage %1 connected (job %2)</source>
        <translation>Storage %1 подключён (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="306"/>
        <source>Storage %1 disconnected (job %2)</source>
        <translation>Storage %1 отключён (Job %2)</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="310"/>
        <source>Storage %1 is alive</source>
        <translation>Storage %1 доступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="311"/>
        <source>Storage %1 not reachable</source>
        <translation>Storage %1 недоступен</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="421"/>
        <source>Invalid response</source>
        <translation>Недопустимый ответ</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="481"/>
        <source>Not connected to the gRPC service of the Director</source>
        <translation>Нет подключения к службе gRPC Director</translation>
    </message>
    <message>
        <location filename="../src/director/bgrpctransport.cpp" line="542"/>
        <source>The reply of the Director could not be read</source>
        <translation>Не удалось прочитать ответ Director</translation>
    </message>
</context>
<context>
    <name>BIncludeBlockWidget</name>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="60"/>
        <source>Include Block #%1</source>
        <translation>Блок Include №%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="68"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="95"/>
        <source>Add Path...</source>
        <translation>Добавить путь...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="96"/>
        <source>Enter file or directory path</source>
        <translation>Введите путь к файлу или каталогу</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="99"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="106"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="120"/>
        <source>Exclude Files</source>
        <translation>Исключаемые файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="126"/>
        <source>Add File...</source>
        <translation>Добавить файл...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="127"/>
        <source>Enter file path to exclude</source>
        <translation>Введите путь к исключаемому файлу</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="133"/>
        <source>Directory Patterns (WildDir)</source>
        <translation>Шаблоны каталогов (WildDir)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="139"/>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="152"/>
        <source>Add Pattern...</source>
        <translation>Добавить шаблон...</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="140"/>
        <source>e.g., */__pycache__</source>
        <translation>напр., */__pycache__</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="146"/>
        <source>File Patterns (WildFile)</source>
        <translation>Шаблоны файлов (WildFile)</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="153"/>
        <source>e.g., *.pyc, *.log</source>
        <translation>напр., *.pyc, *.log</translation>
    </message>
    <message>
        <location filename="../src/jobs/fileset/bincludeblockwidget.cpp" line="158"/>
        <source>Exclude</source>
        <translation>Исключить</translation>
    </message>
</context>
<context>
    <name>BIncludeOptionsForm</name>
    <message>
        <location filename="../src/config/bincludeoptionsform.cpp" line="87"/>
        <source>Failed to load include options schema</source>
        <translation>Не удалось загрузить схему параметров Include</translation>
    </message>
</context>
<context>
    <name>BJobAdvancedPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="633"/>
        <source>Advanced Directives</source>
        <translation>Дополнительные директивы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="634"/>
        <source>Optional settings for fine-tuning (can be skipped).</source>
        <translation>Необязательные параметры для тонкой настройки (можно пропустить).</translation>
    </message>
</context>
<context>
    <name>BJobBasicsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="244"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="249"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="245"/>
        <source>Configure the identity of the resource.</source>
        <translation>Задайте идентификацию ресурса.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="253"/>
        <source>e.g., BackupServer1</source>
        <translation>напр., BackupServer1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="254"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="258"/>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="263"/>
        <source>Inheritance</source>
        <translation>Наследование</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="270"/>
        <source>JobDefs:</source>
        <translation>JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="282"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="294"/>
        <source>Inherits default settings from JobDefs &quot;%1&quot;. Resource fields on the next page become optional.</source>
        <translation>Наследует настройки по умолчанию из JobDefs «%1». Поля ресурсов на следующей странице становятся необязательными.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="308"/>
        <source>Basic Settings (JobDefs)</source>
        <translation>Основные настройки (JobDefs)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitions</name>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="112"/>
        <source>(block)</source>
        <translation>(блок)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitions.cpp" line="125"/>
        <source>(%n block(s))</source>
        <translation>
            <numerusform>(%n блок)</numerusform>
            <numerusform>(%n блока)</numerusform>
            <numerusform>(%n блоков)</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitions.cpp" line="299"/>
        <source>(see the text)</source>
        <translation>(см. текст)</translation>
    </message>
</context>
<context>
    <name>BJobDefinitionsWidget</name>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="157"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="158"/>
        <source>Resource</source>
        <translation>Ресурс</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="159"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="435"/>
        <source>JobDefs</source>
        <translation>JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="160"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="161"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="162"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="163"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="164"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="165"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="166"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="167"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="200"/>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="202"/>
        <source>Disabled, the scheduler does not start it</source>
        <translation>Выключено, планировщик его не запускает</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="235"/>
        <source>Set in this %1</source>
        <translation>Задано в этом %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="238"/>
        <source>Inherited from JobDefs %1</source>
        <translation>Унаследовано из JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="240"/>
        <source>Not known: a JobDefs of the chain was not reported</source>
        <translation>Неизвестно: один из JobDefs цепочки не был передан</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="241"/>
        <source>Not set: the default of the Director applies</source>
        <translation>Не задано: действует значение Director по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="265"/>
        <source>Copy %1 %2</source>
        <translation>Копировать %1 %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="266"/>
        <source>New %1</source>
        <translation>Создать %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="271"/>
        <source>&amp;Name:</source>
        <translation>&amp;Имя:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="286"/>
        <source>&amp;JobDefs:</source>
        <translation>&amp;JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="288"/>
        <source>&amp;Type:</source>
        <translation>&amp;Тип:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="292"/>
        <source>&amp;Level:</source>
        <translation>&amp;Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="295"/>
        <source>&amp;Client:</source>
        <translation>&amp;Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="297"/>
        <source>&amp;FileSet:</source>
        <translation>&amp;FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="299"/>
        <source>&amp;Schedule:</source>
        <translation>&amp;Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="301"/>
        <source>St&amp;orage:</source>
        <translation>St&amp;orage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="303"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="305"/>
        <source>&amp;Messages:</source>
        <translation>&amp;Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="311"/>
        <source>Create it &amp;disabled (Enabled = no)</source>
        <translation>Создать &amp;выключенным (Enabled = no)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="313"/>
        <source>A new job with a schedule is started by the scheduler of the Director. Created disabled, it does not run until you enable it.</source>
        <translation>Новый Job с Schedule запускается планировщиком Director. Если создать его выключенным, он не будет выполняться, пока вы его не включите.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="320"/>
        <source>The copy is the text of %1 with the new name, in a file of its own. Everything else stays as it is.</source>
        <translation>Копия — это текст %1 с новым именем в отдельном файле. Всё остальное остаётся без изменений.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="324"/>
        <source>Fields left empty are not written; a job takes them from its JobDefs. The Director checks the new resource before anything is written.</source>
        <translation>Пустые поля не записываются; Job берёт их из своего JobDefs. Director проверяет новый ресурс до того, как что-либо будет записано.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="334"/>
        <source>&amp;Check and review...</source>
        <translation>&amp;Проверить и просмотреть...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="351"/>
        <source>A name is needed.</source>
        <translation>Требуется имя.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="353"/>
        <source>The name must not contain quotes, slashes, backslashes or &quot;..&quot;.</source>
        <translation>Имя не должно содержать кавычки, косые черты, обратные косые черты или «..».</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="355"/>
        <source>A resource of this name exists already.</source>
        <translation>Ресурс с таким именем уже существует.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="422"/>
        <source>&amp;Refresh</source>
        <translation>&amp;Обновить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="423"/>
        <source>Reads the Job and JobDefs resources from the Director again</source>
        <translation>Повторно считывает ресурсы Job и JobDefs с Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="424"/>
        <source>New &amp;Job...</source>
        <translation>Новый &amp;Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="425"/>
        <source>New Job&amp;Defs...</source>
        <translation>Новый Job&amp;Defs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="426"/>
        <source>&amp;Copy...</source>
        <translation>&amp;Копировать...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="427"/>
        <source>De&amp;lete...</source>
        <translation>&amp;Удалить...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="429"/>
        <source>Filter</source>
        <translation>Фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="431"/>
        <source>Filter the resources</source>
        <translation>Фильтровать ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="433"/>
        <source>Jobs and JobDefs</source>
        <translation>Jobs и JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="434"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="436"/>
        <source>Show</source>
        <translation>Показать</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="457"/>
        <source>Job and JobDefs resources</source>
        <translation>Ресурсы Job и JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504"/>
        <source>Directive</source>
        <translation>Директива</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504"/>
        <source>Value</source>
        <translation>Значение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="504"/>
        <source>Set in</source>
        <translation>Где задано</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="505"/>
        <source>Directives and where they are set</source>
        <translation>Директивы и места, где они заданы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="512"/>
        <source>Directives</source>
        <translation>Директивы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="519"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="526"/>
        <source>Resource text</source>
        <translation>Текст ресурса</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="528"/>
        <source>Edit the &amp;text directly (for directives the form does not have)</source>
        <translation>Редактировать &amp;текст напрямую (для директив, которых нет в форме)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="529"/>
        <source>The text is sent as you write it, after the Director has checked it and you have seen the difference. The fields of the form are not used then.</source>
        <translation>Текст отправляется в том виде, как вы его написали, после того как Director проверит его и вы увидите разницу. Поля формы в этом случае не используются.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="533"/>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="537"/>
        <source>Re&amp;vert</source>
        <translation>&amp;Вернуть</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="538"/>
        <source>Drops the changes made here</source>
        <translation>Отменяет сделанные здесь изменения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="539"/>
        <source>&amp;Save...</source>
        <translation>&amp;Сохранить...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="540"/>
        <source>Lets the Director check the change, shows it, and writes it after you confirm</source>
        <translation>Даёт Director проверить изменение, показывает его и записывает после вашего подтверждения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="626"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1032"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1201"/>
        <source>(not set here)</source>
        <translation>(здесь не задано)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="627"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="628"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="645"/>
        <source>JobDefs:</source>
        <translation>JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="646"/>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="649"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="653"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="654"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="655"/>
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="656"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="657"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="658"/>
        <source>Messages:</source>
        <translation>Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="659"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="660"/>
        <source>Enabled:</source>
        <translation>Включено:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="661"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="776"/>
        <source>Unsaved changes</source>
        <translation>Несохранённые изменения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="777"/>
        <source>%1 has changes that were not written. Drop them?</source>
        <translation>В %1 есть незаписанные изменения. Отменить их?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="805"/>
        <source>No resource selected</source>
        <translation>Ресурс не выбран</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="806"/>
        <source>Choose a Job or JobDefs in the table to see its directives and where they are set.</source>
        <translation>Выберите Job или JobDefs в таблице, чтобы увидеть его директивы и места, где они заданы.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="840"/>
        <source>Uses no JobDefs: every directive is set here or has its default.</source>
        <translation>Не использует JobDefs: каждая директива задана здесь или имеет значение по умолчанию.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="841"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="849"/>
        <source>Inherits from JobDefs %1.</source>
        <translation>Наследует от JobDefs %1.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="844"/>
        <source>No job uses this JobDefs.</source>
        <translation>Ни один Job не использует этот JobDefs.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="845"/>
        <source>Used by %n job(s): %1.</source>
        <translation>
            <numerusform>Используется %n Job: %1.</numerusform>
            <numerusform>Используется %n Jobs: %1.</numerusform>
            <numerusform>Используется %n Jobs: %1.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="890"/>
        <source>the text the Director sent does not match its SHA-256</source>
        <translation>текст, отправленный Director, не соответствует своему SHA-256</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="923"/>
        <source>this %1</source>
        <translation>этом %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="932"/>
        <source>JobDefs %1</source>
        <translation>JobDefs %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="934"/>
        <source>not known (a JobDefs was not reported)</source>
        <translation>неизвестно (один из JobDefs не был передан)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="936"/>
        <source>not set (default)</source>
        <translation>не задано (по умолчанию)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1009"/>
        <source>Set here (as the Director reports it)</source>
        <translation>Задано здесь (как сообщает Director)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1011"/>
        <source>Inherited: %1 (JobDefs %2). Leave empty to keep it.</source>
        <translation>Унаследовано: %1 (JobDefs %2). Оставьте пустым, чтобы сохранить.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1013"/>
        <source>Not known: a JobDefs of the chain was not reported.</source>
        <translation>Неизвестно: один из JobDefs цепочки не был передан.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1014"/>
        <source>Not set: the default of the Director applies.</source>
        <translation>Не задано: действует значение Director по умолчанию.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1016"/>
        <source>Written in a way the form cannot change (several times, a list, or with other directives on its line). Use the text.</source>
        <translation>Записано так, что форма не может это изменить (несколько раз, списком или вместе с другими директивами в строке). Используйте текст.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1033"/>
        <source>(not set here: %1)</source>
        <translation>(здесь не задано: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1057"/>
        <source>Read-only on this connection: changing Job and JobDefs resources needs the configuration methods of the gRPC interface of the Onesimus Director. New Job and New JobDefs use the job wizard (&quot;configure add&quot;).</source>
        <translation>Только чтение через это соединение: для изменения ресурсов Job и JobDefs нужны методы конфигурации интерфейса gRPC Onesimus Director. «Новый Job» и «Новый JobDefs» используют мастер Job («configure add»).</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1061"/>
        <source>Read-only: this session is not offered changes of %1 resources (the console needs the command ACL &quot;configure&quot; and the ACL of the resource type).</source>
        <translation>Только чтение: этому сеансу не предоставлено изменение ресурсов %1 (консоли нужен командный ACL «configure» и ACL типа ресурса).</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1065"/>
        <source>Reading the text of the resource from the Director...</source>
        <translation>Чтение текста ресурса с Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1067"/>
        <source>Read-only: the text of the resource could not be read: %1</source>
        <translation>Только чтение: не удалось прочитать текст ресурса: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1069"/>
        <source>The form is locked: the text of the resource cannot be changed field by field safely (%1). Edit the text directly instead.</source>
        <translation>Форма заблокирована: текст ресурса нельзя безопасно изменить по полям (%1). Вместо этого редактируйте текст напрямую.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1100"/>
        <source>Creates a resource with the text of this one and a new name</source>
        <translation>Создаёт ресурс с текстом этого ресурса и новым именем</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1101"/>
        <source>Copying needs the configuration methods of the gRPC interface of the Onesimus Director (read and create) for this resource type</source>
        <translation>Для копирования нужны методы конфигурации интерфейса gRPC Onesimus Director (чтение и создание) для этого типа ресурса</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1199"/>
        <source>%1: %2 → %3</source>
        <translation>%1: %2 → %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1202"/>
        <source>(not set here, inherited)</source>
        <translation>(здесь не задано, унаследовано)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1262"/>
        <source>%1 was not written: changing it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 не записан: для его изменения нужны методы конфигурации интерфейса gRPC Onesimus Director. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1267"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1317"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1361"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1394"/>
        <source>Another change is being written.</source>
        <translation>Записывается другое изменение.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1275"/>
        <source>The text as edited</source>
        <translation>Текст после редактирования</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1280"/>
        <source>%1 cannot be written: %2</source>
        <translation>Не удаётся записать %1: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1281"/>
        <source>%1 was not written</source>
        <translation>%1 не записан</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1282"/>
        <source>The change could not be written into the text of the resource safely: %1

Nothing was sent.</source>
        <translation>Не удалось безопасно внести изменение в текст ресурса: %1

Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1292"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1323"/>
        <source>Jobs and JobDefs that use this JobDefs</source>
        <translation>Jobs и JobDefs, использующие этот JobDefs</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1293"/>
        <source>A JobDefs gives its values to every job that does not set them itself; the change applies to those of the %n resource(s) below.</source>
        <translation>
            <numerusform>JobDefs передаёт свои значения каждому Job, который не задаёт их сам; изменение затрагивает %n ресурс ниже.</numerusform>
            <numerusform>JobDefs передаёт свои значения каждому Job, который не задаёт их сам; изменение затрагивает %n ресурса ниже.</numerusform>
            <numerusform>JobDefs передаёт свои значения каждому Job, который не задаёт их сам; изменение затрагивает %n ресурсов ниже.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1297"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1326"/>
        <source>Jobs affected</source>
        <translation>Затрагиваемые Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1298"/>
        <source>Lines that were not edited stay as they are in the file.</source>
        <translation>Неотредактированные строки остаются в файле без изменений.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1300"/>
        <source>The Director checks the change of %1...</source>
        <translation>Director проверяет изменение %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1312"/>
        <source>%1 was not deleted: deleting it needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 не удалён: для удаления нужны методы конфигурации интерфейса gRPC Onesimus Director. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1327"/>
        <source>The job&apos;s records in the catalog stay; only its definition is removed. A running job is not touched.</source>
        <translation>Записи Job в каталоге сохраняются; удаляется только его определение. Выполняющийся Job не затрагивается.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1330"/>
        <source>The Director checks whether %1 can be deleted...</source>
        <translation>Director проверяет, можно ли удалить %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1372"/>
        <source>A new job. The Director checks that everything it needs is set, here or in its JobDefs.</source>
        <translation>Новый Job. Director проверяет, что всё необходимое задано — здесь или в его JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1374"/>
        <source>A new JobDefs. No job uses it until a job names it.</source>
        <translation>Новый JobDefs. Ни один Job не использует его, пока не сошлётся на него.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1376"/>
        <source>The Director checks the new %1 %2...</source>
        <translation>Director проверяет новый %1 %2...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1388"/>
        <source>%1 %2 was not copied: copying needs the configuration methods of the gRPC interface of the Onesimus Director. Nothing was sent.</source>
        <translation>%1 %2 не скопирован: для копирования нужны методы конфигурации интерфейса gRPC Onesimus Director. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1408"/>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1417"/>
        <source>%1 %2 was not copied: %3</source>
        <translation>%1 %2 не скопирован: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1421"/>
        <source>A copy of %1 %2 with the new name.</source>
        <translation>Копия %1 %2 с новым именем.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1427"/>
        <source>The Director checks the copy %1...</source>
        <translation>Director проверяет копию %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1442"/>
        <source>The Director reloaded its configuration.</source>
        <translation>Director перезагрузил свою конфигурацию.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdefinitionswidget.cpp" line="1444"/>
        <source>Follow-up commands: %1</source>
        <translation>Последующие команды: %1</translation>
    </message>
</context>
<context>
    <name>BJobDetailsDialog</name>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="31"/>
        <source>Job Details: %1 (ID: %2) - Client: %3</source>
        <translation>Сведения о Job: %1 (ID: %2) - Client: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="70"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="79"/>
        <source>Job Information</source>
        <translation>Сведения о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="82"/>
        <source>Job ID:</source>
        <translation>ID Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="83"/>
        <source>Job Name:</source>
        <translation>Имя Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="84"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="88"/>
        <source>Backup</source>
        <translation>Backup</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="89"/>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="93"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="94"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="95"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="97"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="101"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="102"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="108"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="236"/>
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="116"/>
        <source>Timing</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="123"/>
        <source>Start Time:</source>
        <translation>Время начала:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="127"/>
        <source>Duration:</source>
        <translation>Длительность:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="132"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="136"/>
        <source>Files Backed Up:</source>
        <translation>Сохранено файлов:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="143"/>
        <source>Data Size:</source>
        <translation>Объём данных:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="148"/>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="165"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="159"/>
        <source>Job Status:</source>
        <translation>Статус Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="179"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="183"/>
        <source>Logs</source>
        <translation>Журналы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobdetailsdialog.cpp" line="196"/>
        <source>No connection to the Director.</source>
        <translation>Нет соединения с Director.</translation>
    </message>
</context>
<context>
    <name>BJobLogDialog</name>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="27"/>
        <source>Job Log: %1 (ID: %2) - %3</source>
        <translation>Журнал Job: %1 (ID: %2) - %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="42"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="43"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="44"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="45"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="48"/>
        <source>&lt;b&gt;Status:&lt;/b&gt; %1 | &lt;b&gt;Start:&lt;/b&gt; %2</source>
        <translation>&lt;b&gt;Статус:&lt;/b&gt; %1 | &lt;b&gt;Начало:&lt;/b&gt; %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="65"/>
        <source>Copy Selection</source>
        <translation>Копировать выделенное</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogdialog.cpp" line="71"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
</context>
<context>
    <name>BJobLogView</name>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="67"/>
        <source>No log entries for this job.</source>
        <translation>Нет записей журнала для этого Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="71"/>
        <source>Could not load the job log: %1</source>
        <translation>Не удалось загрузить журнал Job: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="101"/>
        <source>No connection to the Director.</source>
        <translation>Нет соединения с Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjoblogview.cpp" line="105"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
</context>
<context>
    <name>BJobPreviewPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="697"/>
        <source>Preview &amp; Deploy</source>
        <translation>Предпросмотр и развёртывание</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="698"/>
        <source>Review the configuration and deploy to the Director.</source>
        <translation>Проверьте конфигурацию и разверните её на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="703"/>
        <source>Configuration:</source>
        <translation>Конфигурация:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="715"/>
        <source>Copy Configuration</source>
        <translation>Копировать конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="722"/>
        <source>Director Command:</source>
        <translation>Команда Director:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="734"/>
        <source>Copy Command</source>
        <translation>Копировать команду</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="787"/>
        <source>Note: RunScript blocks cannot be sent via &apos;configure add&apos;. The command above creates the job without RunScript blocks. Deploy the full configuration manually using &apos;Copy Configuration&apos; to /etc/bareos/bareos-dir.d/job/%1.conf and reload the Director.</source>
        <translation>Примечание: блоки RunScript нельзя отправить через &apos;configure add&apos;. Команда выше создаёт Job без блоков RunScript. Разверните полную конфигурацию вручную: с помощью «Копировать конфигурацию» поместите её в /etc/bareos/bareos-dir.d/job/%1.conf и перезагрузите Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="810"/>
        <source>Deploy to Director</source>
        <translation>Развернуть на Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="811"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Отправить &apos;configure add&apos; на Director?

Нажмите «Да», чтобы выполнить, или «Нет», чтобы закрыть без развёртывания.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="833"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="868"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="877"/>
        <source>Resource created successfully.</source>
        <translation>Ресурс успешно создан.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="885"/>
        <source>Resource created. Reloading Director...</source>
        <translation>Ресурс создан. Перезагрузка Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="908"/>
        <source>Resource created and Director reloaded successfully.</source>
        <translation>Ресурс создан, Director успешно перезагружен.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="911"/>
        <source>Resource created, but reloading the Director failed. Run &apos;reload&apos; on the Director.</source>
        <translation>Ресурс создан, но перезагрузить Director не удалось. Выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="924"/>
        <source>Resource created, but the connection to the Director was lost before the reload was confirmed.</source>
        <translation>Ресурс создан, но соединение с Director было потеряно до подтверждения перезагрузки.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="928"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check whether the resource exists.</source>
        <translation>Соединение с Director было потеряно до получения ответа. Подключитесь снова и проверьте, существует ли ресурс.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="940"/>
        <source>Resource created, but the Director did not confirm the reload in time. Run &apos;reload&apos; on the Director if the job does not appear.</source>
        <translation>Ресурс создан, но Director не подтвердил перезагрузку вовремя. Если Job не появится, выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="943"/>
        <source>Timeout waiting for Director response. The command may still be processing.</source>
        <translation>Истекло время ожидания ответа Director. Команда, возможно, ещё обрабатывается.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="952"/>
        <source>Configuration copied to clipboard.</source>
        <translation>Конфигурация скопирована в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="959"/>
        <source>Command copied to clipboard.</source>
        <translation>Команда скопирована в буфер обмена.</translation>
    </message>
</context>
<context>
    <name>BJobResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="256"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
</context>
<context>
    <name>BJobResourcesPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="339"/>
        <source>Resource References</source>
        <translation>Ссылки на ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="340"/>
        <source>Select the resources this job uses.</source>
        <translation>Выберите ресурсы, которые использует этот Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="356"/>
        <source>Required Resources</source>
        <translation>Обязательные ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="361"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="365"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="369"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="373"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="377"/>
        <source>Messages:</source>
        <translation>Messages:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="382"/>
        <source>Job Settings</source>
        <translation>Настройки Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="388"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="392"/>
        <source>Schedule:</source>
        <translation>Schedule:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="396"/>
        <source>Catalog:</source>
        <translation>Catalog:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="401"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="406"/>
        <source>Max Concurrent Jobs:</source>
        <translation>Max Concurrent Jobs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="411"/>
        <source>Restore Settings</source>
        <translation>Настройки Restore</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="415"/>
        <source>/tmp/bareos-restores</source>
        <translation>/tmp/bareos-restores</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="416"/>
        <source>Where:</source>
        <translation>Where:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="420"/>
        <source>Replace:</source>
        <translation>Replace:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="425"/>
        <source>Pool Overrides</source>
        <translation>Переопределение Pool</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="432"/>
        <source>Full Backup Pool:</source>
        <translation>Full Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="436"/>
        <source>Diff Backup Pool:</source>
        <translation>Diff Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="440"/>
        <source>Inc Backup Pool:</source>
        <translation>Inc Backup Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="445"/>
        <source>Simple Scripts</source>
        <translation>Простые сценарии</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="449"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="453"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="457"/>
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="450"/>
        <source>Run Before Job:</source>
        <translation>Run Before Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="454"/>
        <source>Run After Job:</source>
        <translation>Run After Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="458"/>
        <source>Run After Failed Job:</source>
        <translation>Run After Failed Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="461"/>
        <location filename="../src/jobs/bjobwizard.cpp" line="465"/>
        <source>/path/to/client-script.sh</source>
        <translation>/path/to/client-script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="462"/>
        <source>Client Run Before:</source>
        <translation>Client Run Before:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="466"/>
        <source>Client Run After:</source>
        <translation>Client Run After:</translation>
    </message>
</context>
<context>
    <name>BJobScriptsPage</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="607"/>
        <source>RunScript Blocks</source>
        <translation>Блоки RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="608"/>
        <source>Define complex script execution blocks (optional). For simple scripts, use the fields on the previous page.</source>
        <translation>Определите сложные блоки выполнения сценариев (необязательно). Для простых сценариев используйте поля на предыдущей странице.</translation>
    </message>
</context>
<context>
    <name>BJobWidget</name>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="48"/>
        <source>Successful (T)</source>
        <translation>Успешно (T)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="49"/>
        <source>Warning (W)</source>
        <translation>Предупреждение (W)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="51"/>
        <source>Error (E)</source>
        <translation>Ошибка (E)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="56"/>
        <source>Enable Date Filter</source>
        <translation>Включить фильтр по дате</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="400"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1660"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1663"/>
        <source>Filter</source>
        <translation>Фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="459"/>
        <source>Job Name:</source>
        <translation>Имя Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="460"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="513"/>
        <source>Backup Level</source>
        <translation>Уровень резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="545"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="546"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="547"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="533"/>
        <source>Date Range</source>
        <translation>Диапазон дат</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="39"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="40"/>
        <source>Clear History</source>
        <translation>Очистить историю</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="50"/>
        <source>Failed (F)</source>
        <translation>Неудачно (F)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="52"/>
        <source>Running (R)</source>
        <translation>Выполняется (R)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="53"/>
        <source>Canceled (A)</source>
        <translation>Отменено (A)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="54"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="356"/>
        <source>Zero Bytes</source>
        <translation>Ноль байт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="81"/>
        <source>When enabled, only shows jobs with 0 bytes transferred.
Warning: This filter persists across restarts and may
cause an empty table if no zero-byte jobs exist.</source>
        <translation>Если включено, показываются только Jobs, передавшие 0 байт.
Внимание: этот фильтр сохраняется после перезапуска и может
привести к пустой таблице, если Jobs с нулём байт нет.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="89"/>
        <source>When enabled, only shows jobs within the selected date range.
Warning: This filter persists across restarts and may
cause an empty or limited table if the range is too narrow.</source>
        <translation>Если включено, показываются только Jobs в выбранном диапазоне дат.
Внимание: этот фильтр сохраняется после перезапуска и может
привести к пустой или неполной таблице, если диапазон слишком узкий.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="105"/>
        <source>Filter by job name...</source>
        <translation>Фильтр по имени Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="109"/>
        <source>Filter by client...</source>
        <translation>Фильтр по Client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="114"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="117"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="120"/>
        <source>(no job selected)</source>
        <translation>(Job не выбран)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="133"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Load error</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - Ошибка загрузки</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="157"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1710"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - No job selected</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job не выбран</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="161"/>
        <source>-- Select Job Log --</source>
        <translation>-- Выберите журнал Job --</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="163"/>
        <source>Select from previously loaded job logs</source>
        <translation>Выбрать из ранее загруженных журналов Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="165"/>
        <source>Copy selected lines to clipboard</source>
        <translation>Копировать выделенные строки в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="168"/>
        <source>Clear job log history</source>
        <translation>Очистить историю журналов Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="171"/>
        <source>Search in log...</source>
        <translation>Поиск в журнале...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="173"/>
        <source>Filter log lines by search text</source>
        <translation>Фильтровать строки журнала по тексту поиска</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="206"/>
        <source>Cancelling job...</source>
        <translation>Отмена Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="209"/>
        <source>Deleting job...</source>
        <translation>Удаление Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="212"/>
        <source>Running job again...</source>
        <translation>Повторный запуск Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="215"/>
        <source>Loading job log...</source>
        <translation>Загрузка журнала Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="219"/>
        <source>Pruning expired jobs...</source>
        <translation>Prune просроченных Jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="260"/>
        <source>⚠ Zero Bytes filter active — only jobs with 0 bytes are shown. This filter persists across restarts!</source>
        <translation>⚠ Активен фильтр «Ноль байт» — показываются только Jobs с 0 байт. Этот фильтр сохраняется после перезапуска!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="270"/>
        <source>⚠ Date filter active — only jobs within the selected range are shown. This filter persists across restarts!</source>
        <translation>⚠ Активен фильтр по дате — показываются только Jobs в выбранном диапазоне. Этот фильтр сохраняется после перезапуска!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="359"/>
        <source>Date Range (%1 – %2)</source>
        <translation>Диапазон дат (%1 – %2)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="372"/>
        <source>Status (hidden: %1)</source>
        <translation>Статус (скрыто: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="377"/>
        <source>⚠ Active filters from last session: %1 — results may be limited</source>
        <translation>⚠ Активные фильтры из прошлого сеанса: %1 — результаты могут быть неполными</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="403"/>
        <source>Show/hide filter</source>
        <translation>Показать/скрыть фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="409"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="413"/>
        <source>Auto-Refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="415"/>
        <source>5 s</source>
        <translation>5 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="416"/>
        <source>10 s</source>
        <translation>10 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="417"/>
        <source>30 s</source>
        <translation>30 с</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="418"/>
        <source>1 min</source>
        <translation>1 мин</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="419"/>
        <source>5 min</source>
        <translation>5 мин</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="464"/>
        <source>&lt;b&gt;Status:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Статус:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="480"/>
        <source>Basic Filters</source>
        <translation>Основные фильтры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="502"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="527"/>
        <source>From:</source>
        <translation>С:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="528"/>
        <source>To:</source>
        <translation>По:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="552"/>
        <source>Job Details</source>
        <translation>Сведения о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="557"/>
        <source>Reset filter</source>
        <translation>Сбросить фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="678"/>
        <source>Updated: %1 - %2 jobs loaded</source>
        <translation>Обновлено: %1 - загружено Jobs: %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="723"/>
        <source>No selection</source>
        <translation>Ничего не выбрано</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="724"/>
        <source>Please select a job to cancel.</source>
        <translation>Выберите Job для отмены.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="735"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="736"/>
        <source>Do you really want to cancel job ID %1?</source>
        <translation>Вы действительно хотите отменить Job с ID %1?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="744"/>
        <source>Job Cancelled</source>
        <translation>Job отменён</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="745"/>
        <source>Job %1 was cancelled.</source>
        <translation>Job %1 отменён.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1142"/>
        <source>Loading page %1...</source>
        <translation>Загрузка страницы %1...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1155"/>
        <source>Refreshing job list...</source>
        <translation>Обновление списка Jobs...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1280"/>
        <source>%1 jobs loaded</source>
        <translation>Загружено Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1515"/>
        <source>Auto-refresh on (%1)</source>
        <translation>Автообновление включено (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1520"/>
        <source>Auto-refresh off</source>
        <translation>Автообновление выключено</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1615"/>
        <source>Filtered: %1 of %2 jobs</source>
        <translation>Отфильтровано: %1 из %2 Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1661"/>
        <source>Hide filter</source>
        <translation>Скрыть фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1664"/>
        <source>Show filter</source>
        <translation>Показать фильтр</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1692"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - Loading...</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1730"/>
        <location filename="../src/jobs/bjobwidget.cpp" line="1867"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - строк: %3</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="589"/>
        <source>History:</source>
        <translation>История:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="609"/>
        <source>Job Log</source>
        <translation>Журнал Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="612"/>
        <source>Messages</source>
        <translation>Messages</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1194"/>
        <source>Error parsing JSON data</source>
        <translation>Ошибка разбора данных JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1546"/>
        <source>Created</source>
        <translation>Создан</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1547"/>
        <source>Running</source>
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1548"/>
        <source>Blocked</source>
        <translation>Заблокирован</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1549"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1550"/>
        <source>Warning</source>
        <translation>Предупреждение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1551"/>
        <source>Failed</source>
        <translation>Неудачно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1552"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1553"/>
        <source>Non-fatal Error</source>
        <translation>Нефатальная ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1554"/>
        <source>Canceled</source>
        <translation>Отменён</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1608"/>
        <source>⚠ No jobs visible — &apos;Zero Bytes&apos; filter is active!</source>
        <translation>⚠ Нет видимых Jobs — активен фильтр «Ноль байт»!</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1610"/>
        <source>⚠ No jobs visible — check your filter settings (%1 jobs loaded)</source>
        <translation>⚠ Нет видимых Jobs — проверьте настройки фильтров (загружено Jobs: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1619"/>
        <source>%1 Jobs</source>
        <translation>Jobs: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1725"/>
        <source>&lt;b&gt;Job Log&lt;/b&gt; - Job: %1 (ID: %2) - %3 lines, job is running</source>
        <translation>&lt;b&gt;Журнал Job&lt;/b&gt; - Job: %1 (ID: %2) - строк: %3, Job выполняется</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1891"/>
        <source>Copied %1 log lines to clipboard</source>
        <translation>Скопировано строк журнала в буфер обмена: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1906"/>
        <source>Copied %1 selected lines to clipboard</source>
        <translation>Скопировано выделенных строк в буфер обмена: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwidget.cpp" line="1920"/>
        <source>Job log history cleared</source>
        <translation>История журналов Jobs очищена</translation>
    </message>
</context>
<context>
    <name>BJobWizard</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="990"/>
        <source>New Job Wizard</source>
        <translation>Мастер создания Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="991"/>
        <source>New JobDefs Wizard</source>
        <translation>Мастер создания JobDefs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="1001"/>
        <source>Finish</source>
        <translation>Готово</translation>
    </message>
</context>
<context>
    <name>BJobsModel</name>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="253"/>
        <source>Job ID</source>
        <translation>ID Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="254"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="255"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="256"/>
        <source>Start Time</source>
        <translation>Время начала</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="257"/>
        <source>End Time</source>
        <translation>Время окончания</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="258"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="259"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="260"/>
        <source>Level</source>
        <translation>Уровень</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="261"/>
        <source>Files</source>
        <translation>Файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="262"/>
        <source>Size</source>
        <translation>Размер</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobmodels.cpp" line="263"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
</context>
<context>
    <name>BJobsStatisticsWidget</name>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="19"/>
        <source>Job Summary</source>
        <translation>Сводка по Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="25"/>
        <source>Total Jobs:</source>
        <translation>Всего Jobs:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="28"/>
        <source>Selected:</source>
        <translation>Выбрано:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="33"/>
        <source>Status Breakdown</source>
        <translation>Разбивка по статусу</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="40"/>
        <source>Successful:</source>
        <translation>Успешно:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="55"/>
        <source>Warnings:</source>
        <translation>Предупреждения:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="70"/>
        <source>Failed:</source>
        <translation>Неудачно:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="84"/>
        <source>Data Summary</source>
        <translation>Сводка по данным</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="88"/>
        <source>Total Files:</source>
        <translation>Всего файлов:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="91"/>
        <source>Total Size:</source>
        <translation>Общий размер:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="93"/>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="181"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="94"/>
        <source>Date Range:</source>
        <translation>Диапазон дат:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobsstatisticswidget.cpp" line="176"/>
        <source>%1 to %2</source>
        <translation>с %1 по %2</translation>
    </message>
</context>
<context>
    <name>BJsonJobView</name>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="526"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="612"/>
        <source>Delete %1 Jobs</source>
        <translation>Удалить Jobs (%1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="157"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="526"/>
        <source>Delete Job</source>
        <translation>Удалить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="146"/>
        <source>Show Details...</source>
        <translation>Показать подробности...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="151"/>
        <source>Retry Job</source>
        <translation>Повторить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="154"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="720"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="162"/>
        <source>View Log...</source>
        <translation>Просмотреть журнал...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="165"/>
        <source>Restore Files...</source>
        <translation>Восстановить файлы...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="172"/>
        <source>Export to JSON...</source>
        <translation>Экспорт в JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="176"/>
        <source>Export to CSV...</source>
        <translation>Экспорт в CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="183"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="187"/>
        <source>Invert Selection</source>
        <translation>Инвертировать выделение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="191"/>
        <source>Clear Selection</source>
        <translation>Снять выделение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272"/>
        <source>Export Selected Jobs to JSON</source>
        <translation>Экспорт выбранных Jobs в JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="272"/>
        <source>Export All Jobs to JSON</source>
        <translation>Экспорт всех Jobs в JSON</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="285"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="318"/>
        <source>Export Error</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="286"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="319"/>
        <source>Could not write to file: %1</source>
        <translation>Не удалось записать в файл: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="293"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="326"/>
        <source>Export Successful</source>
        <translation>Экспорт выполнен</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/bjsonjobview.cpp" line="294"/>
        <location filename="../src/jobs/bjsonjobview.cpp" line="327"/>
        <source>Exported %n job(s) to %1</source>
        <translation>
            <numerusform>Экспортирован %n Job в %1</numerusform>
            <numerusform>Экспортировано %n Jobs в %1</numerusform>
            <numerusform>Экспортировано %n Jobs в %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305"/>
        <source>Export Selected Jobs to CSV</source>
        <translation>Экспорт выбранных Jobs в CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="305"/>
        <source>Export All Jobs to CSV</source>
        <translation>Экспорт всех Jobs в CSV</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="339"/>
        <source>Total Jobs: %1
</source>
        <translation>Всего Jobs: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="340"/>
        <source>Successful: %1
</source>
        <translation>Успешно: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="341"/>
        <source>Warnings: %1
</source>
        <translation>Предупреждения: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="342"/>
        <source>Failed: %1

</source>
        <translation>Неудачно: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="344"/>
        <source>Total Files: %1
</source>
        <translation>Всего файлов: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="345"/>
        <source>Total Size: %1 bytes

</source>
        <translation>Общий размер: %1 байт

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="348"/>
        <source>Date Range:
</source>
        <translation>Диапазон дат:
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="349"/>
        <source>  From: %1
</source>
        <translation>  С: %1
</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="350"/>
        <source>  To: %1

</source>
        <translation>  По: %1

</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="353"/>
        <source>Selected: %1</source>
        <translation>Выбрано: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="355"/>
        <source>Job Statistics</source>
        <translation>Статистика Jobs</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="420"/>
        <source>Export All to JSON...</source>
        <translation>Экспортировать всё в JSON...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="421"/>
        <source>Export All to CSV...</source>
        <translation>Экспортировать всё в CSV...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="536"/>
        <source>ID %1: %2 (%3)</source>
        <translation>ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="542"/>
        <source>&lt;b&gt;%1 jobs selected for deletion:&lt;/b&gt;&lt;br&gt;%2</source>
        <translation>&lt;b&gt;Jobs, выбранные для удаления (%1):&lt;/b&gt;&lt;br&gt;%2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="551"/>
        <source>Full</source>
        <translation>Full</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="552"/>
        <source>Incremental</source>
        <translation>Incremental</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="553"/>
        <source>Differential</source>
        <translation>Differential</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="555"/>
        <source>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Level:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Start:&lt;/b&gt; %5</source>
        <translation>&lt;b&gt;Job:&lt;/b&gt; %1 (ID: %2)&lt;br&gt;&lt;b&gt;Client:&lt;/b&gt; %3&lt;br&gt;&lt;b&gt;Уровень:&lt;/b&gt; %4&lt;br&gt;&lt;b&gt;Начало:&lt;/b&gt; %5</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="569"/>
        <source>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Attention:&lt;/b&gt; %1 dependent Incremental/Differential job(s) found!&lt;br&gt;&lt;br&gt;Deleting Full backups may make dependent backups unusable for restore.&lt;/div&gt;</source>
        <translation>&lt;div style=&apos;background-color: #fff3cd; color: #664d03; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;&apos;&gt;&lt;b&gt;⚠ Внимание:&lt;/b&gt; найдены зависимые Jobs Incremental/Differential: %1!&lt;br&gt;&lt;br&gt;Удаление резервных копий Full может сделать зависимые резервные копии непригодными для восстановления.&lt;/div&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="580"/>
        <source>Delete Mode</source>
        <translation>Режим удаления</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="583"/>
        <source>Delete job record only</source>
        <translation>Удалить только запись о Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="584"/>
        <source>Removes the job entry from the catalog database.
The backup data on the storage media remains intact.</source>
        <translation>Удаляет запись о Job из базы данных каталога.
Данные резервной копии на носителях остаются нетронутыми.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="593"/>
        <source>Prune (remove expired jobs by retention policy)</source>
        <translation>Prune (удалить просроченные Jobs по политике хранения)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="594"/>
        <source>Removes job records that have exceeded their configured retention period.
Only expired jobs for the affected client(s) will be removed.</source>
        <translation>Удаляет записи о Jobs, превысившие настроенный срок хранения.
Удаляются только просроченные Jobs затронутых клиентов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="603"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; это действие нельзя отменить!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="612"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="626"/>
        <source>  ID %1: %2 (%3)</source>
        <translation>  ID %1: %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="631"/>
        <source>Confirm Delete</source>
        <translation>Подтверждение удаления</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="632"/>
        <source>Are you sure you want to delete %1 job(s)?

%2

%3 dependent Incremental/Differential backup(s) may become unusable for restore.</source>
        <translation>Вы действительно хотите удалить Jobs (%1)?

%2

Зависимые резервные копии Incremental/Differential (%3) могут стать непригодными для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="688"/>
        <source>Rerun Job</source>
        <translation>Запустить Job повторно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="689"/>
        <source>Do you want to run job %1 (%2) again?</source>
        <translation>Запустить Job %1 (%2) ещё раз?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="721"/>
        <source>Do you really want to cancel the running job %1 (%2)?</source>
        <translation>Вы действительно хотите отменить выполняющийся Job %1 (%2)?</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="750"/>
        <source>Not connected</source>
        <translation>Нет соединения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="751"/>
        <source>No connection to the Director.</source>
        <translation>Соединение с Director недоступно.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="772"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjsonjobview.cpp" line="773"/>
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
        <location filename="../src/bmainwindow.cpp" line="142"/>
        <source>Interface zoom: %1 %</source>
        <translation>Масштаб интерфейса: %1 %</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="202"/>
        <source>Connecting...</source>
        <translation>Подключение...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="205"/>
        <source>Authenticating...</source>
        <translation>Аутентификация...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="208"/>
        <source>Setting API mode...</source>
        <translation>Установка режима API...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="211"/>
        <source>Loading resources...</source>
        <translation>Загрузка ресурсов...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="214"/>
        <source>Connected and ready</source>
        <translation>Подключено и готово</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="217"/>
        <source>Connection error</source>
        <translation>Ошибка соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="220"/>
        <location filename="../src/bmainwindow.cpp" line="1694"/>
        <source>Disconnected</source>
        <translation>Отключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="227"/>
        <source>Loading resources... %1/%2</source>
        <translation>Загрузка ресурсов... %1/%2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="249"/>
        <location filename="../src/bmainwindow.cpp" line="686"/>
        <location filename="../src/bmainwindow.cpp" line="2525"/>
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="400"/>
        <location filename="../src/bmainwindow.cpp" line="2885"/>
        <source>Auto-refresh enabled (%1s)</source>
        <translation>Автообновление включено (%1 с)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="404"/>
        <location filename="../src/bmainwindow.cpp" line="2890"/>
        <source>Auto-refresh disabled</source>
        <translation>Автообновление выключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="472"/>
        <source>Job: %1</source>
        <translation>Job: %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="533"/>
        <location filename="../src/bmainwindow.cpp" line="690"/>
        <location filename="../src/bmainwindow.cpp" line="2441"/>
        <source>Not connected</source>
        <translation>Не подключено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="609"/>
        <source>Storage/Volumes</source>
        <translation>Storage/Volumes</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="667"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="700"/>
        <location filename="../src/bmainwindow.cpp" line="717"/>
        <location filename="../src/bmainwindow.cpp" line="1629"/>
        <location filename="../src/bmainwindow.cpp" line="1886"/>
        <source>Connect</source>
        <translation>Подключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="705"/>
        <location filename="../src/bmainwindow.cpp" line="723"/>
        <location filename="../src/bmainwindow.cpp" line="1887"/>
        <source>Reconnect</source>
        <translation>Переподключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="711"/>
        <source>Disconnect</source>
        <translation>Отключить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="719"/>
        <location filename="../src/bmainwindow.cpp" line="1551"/>
        <location filename="../src/bmainwindow.cpp" line="2340"/>
        <source>Connect to Director</source>
        <translation>Подключиться к Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="725"/>
        <source>Reconnect to last used connection</source>
        <translation>Переподключиться к последнему использованному соединению</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="729"/>
        <location filename="../src/bmainwindow.cpp" line="1888"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="735"/>
        <location filename="../src/bmainwindow.cpp" line="1889"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="740"/>
        <location filename="../src/bmainwindow.cpp" line="1890"/>
        <source>Exit</source>
        <translation>Выход</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="745"/>
        <location filename="../src/bmainwindow.cpp" line="1719"/>
        <source>About Onesimus</source>
        <translation>О программе Onesimus</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="749"/>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="753"/>
        <source>Online Documentation</source>
        <translation>Документация в сети</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="758"/>
        <source>Report a Bug...</source>
        <translation>Сообщить об ошибке...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="762"/>
        <location filename="../src/bmainwindow.cpp" line="1866"/>
        <location filename="../src/bmainwindow.cpp" line="1872"/>
        <location filename="../src/bmainwindow.cpp" line="1953"/>
        <source>Keyboard Shortcuts</source>
        <translation>Сочетания клавиш</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="767"/>
        <location filename="../src/bmainwindow.cpp" line="2287"/>
        <source>My Permissions</source>
        <translation>Мои права</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="773"/>
        <location filename="../src/bmainwindow.cpp" line="1903"/>
        <source>Copy</source>
        <translation>Копировать</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="779"/>
        <location filename="../src/bmainwindow.cpp" line="1904"/>
        <source>Select All</source>
        <translation>Выделить всё</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="785"/>
        <source>Clear Selection</source>
        <translation>Снять выделение</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="790"/>
        <source>Find...</source>
        <translation>Найти...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="797"/>
        <location filename="../src/bmainwindow.cpp" line="1918"/>
        <source>Show Statistics</source>
        <translation>Показать статистику</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="811"/>
        <location filename="../src/bmainwindow.cpp" line="1919"/>
        <source>Show Job Log</source>
        <translation>Показать журнал Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="826"/>
        <location filename="../src/bmainwindow.cpp" line="1921"/>
        <source>Interface Zoom In</source>
        <translation>Увеличить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="832"/>
        <location filename="../src/bmainwindow.cpp" line="1922"/>
        <source>Interface Zoom Out</source>
        <translation>Уменьшить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="844"/>
        <source>Toggle Dark/Light Theme</source>
        <translation>Переключить тёмную/светлую тему</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="858"/>
        <source>Run Job</source>
        <translation>Запустить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="863"/>
        <source>Cancel Job</source>
        <translation>Отменить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="868"/>
        <source>Show Details</source>
        <translation>Показать подробности</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="873"/>
        <location filename="../src/bmainwindow.cpp" line="1936"/>
        <source>Refresh Jobs</source>
        <translation>Обновить Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="879"/>
        <source>Export Jobs as JSON...</source>
        <translation>Экспорт Jobs в JSON...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="884"/>
        <source>Export Jobs as CSV...</source>
        <translation>Экспорт Jobs в CSV...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="890"/>
        <source>Add Job...</source>
        <translation>Добавить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="909"/>
        <source>Edit Job...</source>
        <translation>Изменить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="915"/>
        <source>Edit Job</source>
        <translation>Изменить Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="916"/>
        <source>No Jobs available.</source>
        <translation>Нет доступных Jobs.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="922"/>
        <source>Job Resources</source>
        <translation>Ресурсы Job</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="975"/>
        <source>Delete Job...</source>
        <translation>Удалить Job...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="983"/>
        <source>Add JobDefs...</source>
        <translation>Добавить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1002"/>
        <source>Edit JobDefs...</source>
        <translation>Изменить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1008"/>
        <source>JobDefs Resources</source>
        <translation>Ресурсы JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1063"/>
        <source>Delete JobDefs...</source>
        <translation>Удалить JobDefs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074"/>
        <location filename="../src/bmainwindow.cpp" line="1078"/>
        <source>Delete JobDefs</source>
        <translation>Удалить JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1074"/>
        <source>No JobDefs available.</source>
        <translation>Нет доступных JobDefs.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1078"/>
        <source>JobDefs to delete:</source>
        <translation>Удаляемый JobDefs:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1092"/>
        <source>Jobs that use this JobDefs</source>
        <translation>Jobs, использующие этот JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1098"/>
        <source>Job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1108"/>
        <source>Add FileSet...</source>
        <translation>Добавить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1116"/>
        <source>Edit FileSet...</source>
        <translation>Изменить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1124"/>
        <source>Delete FileSet...</source>
        <translation>Удалить FileSet...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1133"/>
        <location filename="../src/bmainwindow.cpp" line="1139"/>
        <location filename="../src/bmainwindow.cpp" line="1183"/>
        <location filename="../src/bmainwindow.cpp" line="1188"/>
        <source>Delete FileSet</source>
        <translation>Удалить FileSet</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1134"/>
        <source>No FileSets available.</source>
        <translation>Нет доступных FileSets.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1144"/>
        <source>Select the FileSet to delete:</source>
        <translation>Выберите FileSet для удаления:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1154"/>
        <source>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; This will remove the FileSet resource from the Director configuration. This action cannot be undone!&lt;/span&gt;</source>
        <translation>&lt;span style=&apos;color: #cc0000;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; ресурс FileSet будет удалён из конфигурации Director. Это действие нельзя отменить!&lt;/span&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1164"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1176"/>
        <source>Confirm Delete</source>
        <translation>Подтверждение удаления</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1177"/>
        <source>Are you sure you want to delete FileSet &quot;%1&quot;?</source>
        <translation>Вы действительно хотите удалить FileSet «%1»?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1184"/>
        <source>Not connected to a Director. FileSet &quot;%1&quot; was not deleted.</source>
        <translation>Нет подключения к Director. FileSet «%1» не удалён.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1189"/>
        <source>FileSet &quot;%1&quot; no longer exists on the Director. Nothing was deleted.</source>
        <translation>FileSet «%1» больше не существует на Director. Ничего не удалено.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1197"/>
        <source>Add New Client...</source>
        <translation>Добавить новый Client...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1205"/>
        <location filename="../src/bmainwindow.cpp" line="1937"/>
        <source>Refresh Clients</source>
        <translation>Обновить Clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1211"/>
        <source>Show Client Details</source>
        <translation>Показать сведения о Client</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1221"/>
        <location filename="../src/bmainwindow.cpp" line="1938"/>
        <source>Refresh Storage</source>
        <translation>Обновить Storage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1228"/>
        <source>Add Schedule...</source>
        <translation>Добавить Schedule...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1250"/>
        <location filename="../src/bmainwindow.cpp" line="1939"/>
        <source>Refresh Schedules</source>
        <translation>Обновить Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1257"/>
        <source>Database Cleanup...</source>
        <translation>Очистка базы данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1259"/>
        <source>Clean up old backups and free disk space</source>
        <translation>Удалить старые резервные копии и освободить место на диске</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1263"/>
        <source>Connection Wizard...</source>
        <translation>Мастер подключения...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1265"/>
        <source>Set up a new director connection</source>
        <translation>Настроить новое соединение с Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1268"/>
        <source>Import Config...</source>
        <translation>Импорт конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1270"/>
        <source>Import Director configuration from files or ZIP archive</source>
        <translation>Импортировать конфигурацию Director из файлов или ZIP-архива</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1279"/>
        <location filename="../src/bmainwindow.cpp" line="1319"/>
        <location filename="../src/bmainwindow.cpp" line="1335"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1285"/>
        <source>System (%1)</source>
        <translation>Системный (%1)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1320"/>
        <source>Onesimus will speak %1 after a restart.</source>
        <translation>После перезапуска Onesimus будет работать на языке: %1.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1323"/>
        <source>Restart now? The connection to the Director is closed and opened again if automatic connection is on.</source>
        <translation>Перезапустить сейчас? Соединение с Director будет закрыто и, если включено автоматическое подключение, установлено заново.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1325"/>
        <source>Restart now?</source>
        <translation>Перезапустить сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1326"/>
        <source>Restart now</source>
        <translation>Перезапустить сейчас</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1327"/>
        <source>Later</source>
        <translation>Позже</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1336"/>
        <source>Onesimus could not be started again. The new language takes effect at the next start.</source>
        <translation>Не удалось перезапустить Onesimus. Новый язык вступит в силу при следующем запуске.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1346"/>
        <location filename="../src/bmainwindow.cpp" line="1894"/>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1360"/>
        <location filename="../src/bmainwindow.cpp" line="1909"/>
        <source>Edit</source>
        <translation>Правка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1369"/>
        <location filename="../src/bmainwindow.cpp" line="1927"/>
        <source>View</source>
        <translation>Вид</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1438"/>
        <source>ACL matrix of the consoles...</source>
        <translation>Матрица ACL консолей...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1439"/>
        <source>Which console may use which job, client, storage, schedule, pool, fileset, catalog and command (Onesimus Director)</source>
        <translation>Какая консоль может использовать какие Job, Client, Storage, Schedule, Pool, FileSet, Catalog и команды (Onesimus Director)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bmainwindow.cpp" line="2809"/>
        <source>%n row(s) copied</source>
        <translation>
            <numerusform>Скопирована %n строка</numerusform>
            <numerusform>Скопировано %n строки</numerusform>
            <numerusform>Скопировано %n строк</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="601"/>
        <location filename="../src/bmainwindow.cpp" line="1380"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1388"/>
        <source>Modify Jobs</source>
        <translation>Изменение Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1394"/>
        <source>Modify JobDefs</source>
        <translation>Изменение JobDefs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1400"/>
        <source>Filesets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="605"/>
        <location filename="../src/bmainwindow.cpp" line="1410"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1417"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="638"/>
        <location filename="../src/bmainwindow.cpp" line="1421"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="97"/>
        <source>Onesimus - Bacula Backup Management</source>
        <translation>Onesimus - управление резервным копированием Bacula</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="99"/>
        <source>Onesimus - Bareos Backup Management</source>
        <translation>Onesimus - управление резервным копированием Bareos</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1427"/>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1443"/>
        <location filename="../src/bmainwindow.cpp" line="1957"/>
        <source>Help</source>
        <translation>Справка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1472"/>
        <source>Main Toolbar</source>
        <translation>Главная панель инструментов</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1501"/>
        <location filename="../src/bmainwindow.cpp" line="1515"/>
        <location filename="../src/bmainwindow.cpp" line="2414"/>
        <source>Statistics ▼</source>
        <translation>Статистика ▼</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1504"/>
        <source>Show/Hide Statistics</source>
        <translation>Показать/скрыть статистику</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1515"/>
        <location filename="../src/bmainwindow.cpp" line="2414"/>
        <location filename="../src/bmainwindow.cpp" line="2450"/>
        <source>Statistics ▶</source>
        <translation>Статистика ▶</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1543"/>
        <source>No Connection Profiles</source>
        <translation>Нет профилей соединений</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1544"/>
        <source>No connection profiles found.

Please create a connection profile in Settings first.</source>
        <translation>Профили соединений не найдены.

Сначала создайте профиль соединения в настройках.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1558"/>
        <source>Select Connection Profile</source>
        <translation>Выбор профиля соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1564"/>
        <source>Connection Profile</source>
        <translation>Профиль соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1613"/>
        <source>Manage Profiles...</source>
        <translation>Управление профилями...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1641"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1642"/>
        <source>Selected profile is invalid.</source>
        <translation>Выбранный профиль недействителен.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1680"/>
        <location filename="../src/bmainwindow.cpp" line="2539"/>
        <location filename="../src/bmainwindow.cpp" line="2771"/>
        <source>Connecting to %1...</source>
        <translation>Подключение к %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1739"/>
        <source>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Version %1 (Build %2)&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;color:#666; margin:0;&apos;&gt;Версия %1 (сборка %2)&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1745"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bacula Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Современный графический интерфейс на Qt6 для резервного копирования Bacula&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1747"/>
        <source>&lt;p style=&apos;margin:0;&apos;&gt;Modern Qt6 GUI for Bareos Backup&lt;/p&gt;</source>
        <translation>&lt;p style=&apos;margin:0;&apos;&gt;Современный графический интерфейс на Qt6 для резервного копирования Bareos&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1831"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1847"/>
        <source>Documentation</source>
        <translation>Документация</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1848"/>
        <location filename="../src/bmainwindow.cpp" line="1858"/>
        <source>Could not open browser. Please visit:
%1</source>
        <translation>Не удалось открыть браузер. Перейдите по адресу:
%1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1857"/>
        <source>Report Bug</source>
        <translation>Сообщить об ошибке</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1905"/>
        <source>Find</source>
        <translation>Найти</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1920"/>
        <source>Toggle Theme</source>
        <translation>Переключить тему</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1923"/>
        <source>Reset Interface Zoom</source>
        <translation>Сбросить масштаб интерфейса</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1943"/>
        <source>Data</source>
        <translation>Данные</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1952"/>
        <source>Help Contents</source>
        <translation>Содержание справки</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2288"/>
        <source>Not connected to a Director.</source>
        <translation>Нет подключения к Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2310"/>
        <source>Settings saved</source>
        <translation>Настройки сохранены</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2337"/>
        <source>Disconnect from Director</source>
        <translation>Отключиться от Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2400"/>
        <source>Connected to %1 version %2</source>
        <translation>Подключено к %1, версия %2</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2402"/>
        <source>Connected: version %1</source>
        <translation>Подключено: версия %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2408"/>
        <source>Connected - Loading data...</source>
        <translation>Подключено — загрузка данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2473"/>
        <source>Error: </source>
        <translation>Ошибка: </translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2480"/>
        <source>Connection Error</source>
        <translation>Ошибка соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2495"/>
        <source>Refreshing data...</source>
        <translation>Обновление данных...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2534"/>
        <source>No Saved Connection</source>
        <translation>Нет сохранённого соединения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2535"/>
        <source>No previous connection found. Please use &apos;Connect&apos; to establish a new connection.</source>
        <translation>Предыдущее соединение не найдено. Используйте «Подключить», чтобы установить новое соединение.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2725"/>
        <source>Auto-connecting to %1...</source>
        <translation>Автоподключение к %1...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2752"/>
        <source>Connection cancelled: server certificate not trusted</source>
        <translation>Подключение отменено: сертификат сервера не является доверенным</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2815"/>
        <source>Copying client data is not implemented yet</source>
        <translation>Копирование данных клиентов пока не реализовано</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2828"/>
        <source>All jobs selected</source>
        <translation>Выбраны все Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2831"/>
        <source>Selecting all clients is not implemented yet</source>
        <translation>Выбор всех клиентов пока не реализован</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2843"/>
        <source>Selection cleared</source>
        <translation>Выделение снято</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2846"/>
        <source>Clearing the client selection is not implemented yet</source>
        <translation>Снятие выделения для клиентов пока не реализовано</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2857"/>
        <location filename="../src/bmainwindow.cpp" line="2867"/>
        <source>Search</source>
        <translation>Поиск</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2858"/>
        <source>Use the filter options in the Job view
to search for specific jobs.

Available filters:
• Job Name
• Client Name
• Status (Success, Warning, Failed)
• Level (Full, Incremental, Differential)
• Date Range</source>
        <translation>Используйте параметры фильтра в представлении Jobs,
чтобы найти нужные Jobs.

Доступные фильтры:
• Имя Job
• Имя клиента
• Статус (успешно, предупреждение, неудачно)
• Уровень (Full, Incremental, Differential)
• Диапазон дат</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2868"/>
        <source>Use the filter combo box in the Client view
to search for specific clients.</source>
        <translation>Используйте поле фильтра в представлении Clients,
чтобы найти нужных клиентов.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2900"/>
        <source>Reset Interface Zoom (now %1 %)</source>
        <translation>Сбросить масштаб интерфейса (сейчас %1 %)</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2914"/>
        <source>Switched to Light Theme</source>
        <translation>Включена светлая тема</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2918"/>
        <source>Switched to Dark Theme</source>
        <translation>Включена тёмная тема</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2931"/>
        <location filename="../src/bmainwindow.cpp" line="2943"/>
        <location filename="../src/bmainwindow.cpp" line="2955"/>
        <source>Not Connected</source>
        <translation>Нет подключения</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2931"/>
        <location filename="../src/bmainwindow.cpp" line="2943"/>
        <source>Please connect to the Director first.</source>
        <translation>Сначала подключитесь к Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2937"/>
        <source>Jobs exported as JSON</source>
        <translation>Jobs экспортированы в JSON</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2949"/>
        <source>Jobs exported as CSV</source>
        <translation>Jobs экспортированы в CSV</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2956"/>
        <source>Please connect to Bareos Director first.</source>
        <translation>Сначала подключитесь к Bareos Director.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2965"/>
        <source>Loading job data...</source>
        <translation>Загрузка данных о Jobs...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3000"/>
        <source>Connection profile &apos;%1&apos; saved</source>
        <translation>Профиль соединения «%1» сохранён</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3085"/>
        <source>Password Missing</source>
        <translation>Нет пароля</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3086"/>
        <source>The imported Console &apos;%1&apos; does not contain a password.

You will need to enter the password manually in the connection settings.</source>
        <translation>Импортированная Console «%1» не содержит пароля.

Пароль потребуется ввести вручную в параметрах соединения.</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3107"/>
        <source>Created connection profile &apos;%1&apos; for Director &apos;%2&apos;</source>
        <translation>Создан профиль соединения «%1» для Director «%2»</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3112"/>
        <source>Connect Now?</source>
        <translation>Подключиться сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3113"/>
        <source>Connection profile &apos;%1&apos; has been created.

Do you want to connect now?</source>
        <translation>Профиль соединения «%1» создан.

Подключиться сейчас?</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="3129"/>
        <source>Imported Director &apos;%1&apos; with Console &apos;%2&apos;</source>
        <translation>Импортирован Director «%1» с Console «%2»</translation>
    </message>
</context>
<context>
    <name>BMdnsBrowser</name>
    <message>
        <location filename="../src/bmdnsbrowser.cpp" line="197"/>
        <source>mDNS: cannot open UDP socket: %1</source>
        <translation>mDNS: не удалось открыть сокет UDP: %1</translation>
    </message>
</context>
<context>
    <name>BMessagesResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="428"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BMessagesWidget</name>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="232"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="238"/>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="245"/>
        <source>0 messages</source>
        <translation>0 сообщений</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="280"/>
        <source>Polling every %1s</source>
        <translation>Опрос каждые %1 с</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="307"/>
        <source>Messages cleared</source>
        <translation>Сообщения очищены</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="322"/>
        <source>Not connected</source>
        <translation>Не подключено</translation>
    </message>
    <message>
        <location filename="../src/messages/bmessageswidget.cpp" line="363"/>
        <source>Refreshing messages...</source>
        <translation>Обновление сообщений...</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/messages/bmessageswidget.cpp" line="369"/>
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
        <location filename="../src/clients/bnewclientdialog.cpp" line="86"/>
        <source>Director Information</source>
        <translation>Сведения о Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="87"/>
        <source>The new client will connect to this Director. These settings are used to generate the File Daemon configuration.</source>
        <translation>Новый клиент будет подключаться к этому Director. Эти параметры используются для создания конфигурации File Daemon.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="94"/>
        <source>Director Name:</source>
        <translation>Имя Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="98"/>
        <source>Director Address:</source>
        <translation>Адрес Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="102"/>
        <source>Director Port:</source>
        <translation>Порт Director:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="106"/>
        <source>TLS Mode:</source>
        <translation>Режим TLS:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="114"/>
        <source>The Director name above will be used in the File Daemon&apos;s Director resource. The client authenticates against this Director using a shared password.</source>
        <translation>Указанное выше имя Director будет использовано в ресурсе Director конфигурации File Daemon. Клиент проходит аутентификацию на этом Director с помощью общего пароля.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="134"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="141"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="148"/>
        <source>&lt;i&gt;(not available)&lt;/i&gt;</source>
        <translation>&lt;i&gt;(недоступно)&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="135"/>
        <source>Director name could not be retrieved from the connection.</source>
        <translation>Не удалось получить имя Director из соединения.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="142"/>
        <source>Director address could not be retrieved.</source>
        <translation>Не удалось получить адрес Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="149"/>
        <source>Director port could not be retrieved.</source>
        <translation>Не удалось получить порт Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="154"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="159"/>
        <source>&lt;b style=&apos;color:red;&apos;&gt;Cannot proceed:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Make sure you are connected to a Director.</source>
        <translation>&lt;b style=&apos;color:red;&apos;&gt;Невозможно продолжить:&lt;/b&gt;&lt;br&gt;%1&lt;br&gt;&lt;br&gt;Убедитесь, что вы подключены к Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="176"/>
        <source>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Warning:&lt;/b&gt; The current console may not have permission to run &apos;configure add client&apos;. Check the Command ACL in the Console resource on the Director.&lt;/span&gt;&lt;br&gt;You can still generate and export the FD configuration files.</source>
        <translation>&lt;span style=&apos;color:orange;&apos;&gt;&lt;b&gt;Внимание:&lt;/b&gt; у текущей консоли может не быть прав на выполнение &apos;configure add client&apos;. Проверьте Command ACL в ресурсе Console на Director.&lt;/span&gt;&lt;br&gt;Вы всё равно можете создать и экспортировать файлы конфигурации FD.</translation>
    </message>
</context>
<context>
    <name>BNewClientPreviewPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="473"/>
        <source>Preview &amp; Export</source>
        <translation>Предпросмотр и экспорт</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="474"/>
        <source>Review generated configuration files, validate, and export.</source>
        <translation>Проверьте созданные файлы конфигурации, выполните проверку и экспортируйте.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="485"/>
        <source>FD: Director</source>
        <translation>FD: Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="488"/>
        <source>FD: Client</source>
        <translation>FD: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="491"/>
        <source>FD: Messages</source>
        <translation>FD: Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="494"/>
        <source>Dir: Client</source>
        <translation>Dir: Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="513"/>
        <source>Director Command</source>
        <translation>Команда Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="525"/>
        <source>Copy Current Tab</source>
        <translation>Копировать текущую вкладку</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="529"/>
        <source>Copy All</source>
        <translation>Копировать всё</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="533"/>
        <source>Export ZIP...</source>
        <translation>Экспорт ZIP...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="766"/>
        <source>FD Director</source>
        <translation>FD Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="767"/>
        <source>FD Client</source>
        <translation>FD Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="768"/>
        <source>FD Messages</source>
        <translation>FD Messages</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="769"/>
        <source>Dir Client</source>
        <translation>Dir Client</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="775"/>
        <source>%1: Config syntax could not be parsed.</source>
        <translation>%1: не удалось разобрать синтаксис конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="786"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="795"/>
        <source>Client name &apos;%1&apos; does not end with &apos;-fd&apos;. Convention: use &apos;&lt;hostname&gt;-fd&apos;.</source>
        <translation>Имя клиента «%1» не оканчивается на «-fd». Соглашение: используйте &apos;&lt;hostname&gt;-fd&apos;.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="800"/>
        <source>Password MD5 hash has invalid length (%1, expected 32).</source>
        <translation>Хеш MD5 пароля имеет недопустимую длину (%1, ожидается 32).</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="806"/>
        <source>Validation passed.</source>
        <translation>Проверка пройдена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="899"/>
        <source>Copied &apos;%1&apos; to clipboard.</source>
        <translation>«%1» скопировано в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="924"/>
        <source>All configuration files copied to clipboard.</source>
        <translation>Все файлы конфигурации скопированы в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="936"/>
        <source>Export Client Configuration</source>
        <translation>Экспорт конфигурации клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="938"/>
        <source>ZIP Archives (*.zip)</source>
        <translation>ZIP-архивы (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="944"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1017"/>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1023"/>
        <source>Export Error</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="945"/>
        <source>Failed to create temporary directory.</source>
        <translation>Не удалось создать временный каталог.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1018"/>
        <source>ZIP creation timed out. Is &apos;zip&apos; installed?</source>
        <translation>Истекло время создания ZIP. Установлен ли &apos;zip&apos;?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1024"/>
        <source>ZIP creation failed:
%1</source>
        <translation>Не удалось создать ZIP:
%1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1029"/>
        <source>Exported to: %1</source>
        <translation>Экспортировано в: %1</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1038"/>
        <source>ERROR: No director connection available.</source>
        <translation>ОШИБКА: соединение с Director недоступно.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1047"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1088"/>
        <source>Failed to add client to Director: %1
Go back to change the settings, or click Finish to send the command again.
You can still export the FD config files manually.</source>
        <translation>Не удалось добавить клиента на Director: %1
Вернитесь назад, чтобы изменить параметры, или нажмите «Готово», чтобы отправить команду ещё раз.
Вы всё равно можете экспортировать файлы конфигурации FD вручную.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1101"/>
        <source>Client &apos;%1&apos; added to Director.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен на Director.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1111"/>
        <source>Client &apos;%1&apos; added to Director. Reloading Director...</source>
        <translation>Client «%1» добавлен на Director. Перезагрузка Director...</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1138"/>
        <source>Client &apos;%1&apos; added and Director reloaded successfully.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен, Director успешно перезагружен.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1145"/>
        <source>Client &apos;%1&apos; added to Director, but reload failed.
Run &apos;reload&apos; manually on the Director or restart bareos-dir.
Deploy the FD config files to the client machine and restart bareos-fd.</source>
        <translation>Client «%1» добавлен на Director, но перезагрузка не удалась.
Выполните &apos;reload&apos; вручную на Director или перезапустите bareos-dir.
Разверните файлы конфигурации FD на клиентской машине и перезапустите bareos-fd.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1164"/>
        <source>Client &apos;%1&apos; added to Director, but the connection was lost before the reload was confirmed.
Reconnect and run &apos;reload&apos; on the Director if the client does not appear.</source>
        <translation>Client «%1» добавлен на Director, но соединение было потеряно до подтверждения перезагрузки.
Если клиент не появится, подключитесь снова и выполните &apos;reload&apos; на Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1173"/>
        <source>The connection to the Director was lost before it answered.
Reconnect and check whether client &apos;%1&apos; exists before sending the command again.</source>
        <translation>Соединение с Director было потеряно до получения ответа.
Подключитесь снова и проверьте, существует ли Client «%1», прежде чем отправлять команду повторно.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1191"/>
        <source>Client &apos;%1&apos; added to Director, but the Director did not confirm the reload within 30 seconds.
Run &apos;reload&apos; manually on the Director if the client does not appear.</source>
        <translation>Client «%1» добавлен на Director, но Director не подтвердил перезагрузку в течение 30 секунд.
Если клиент не появится, выполните &apos;reload&apos; вручную на Director.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1201"/>
        <source>Command Timeout</source>
        <translation>Тайм-аут команды</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1202"/>
        <source>The &apos;configure add client&apos; command did not respond within 30 seconds.

The Director may be busy or the connection may have been lost.

Retry the command?</source>
        <translation>Команда &apos;configure add client&apos; не ответила в течение 30 секунд.

Director может быть занят, или соединение могло быть потеряно.

Повторить команду?</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="1214"/>
        <source>Configure command timed out. The client may or may not have been created.
You can export the FD config files and configure the Director manually.</source>
        <translation>Истекло время ожидания команды configure. Клиент мог быть создан, а мог и нет.
Вы можете экспортировать файлы конфигурации FD и настроить Director вручную.</translation>
    </message>
</context>
<context>
    <name>BNewClientSettingsPage</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="200"/>
        <source>Client Settings</source>
        <translation>Параметры клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="201"/>
        <source>Configure the new client&apos;s identity, password, TLS, and network settings.</source>
        <translation>Задайте идентификацию, пароль, TLS и сетевые параметры нового клиента.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="206"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="210"/>
        <source>e.g. server1-fd</source>
        <translation>напр., server1-fd</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="211"/>
        <source>Client Name:</source>
        <translation>Имя клиента:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="214"/>
        <source>e.g. 192.168.1.100 or server1.example.com</source>
        <translation>напр., 192.168.1.100 или server1.example.com</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="215"/>
        <source>Address:</source>
        <translation>Адрес:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="220"/>
        <source>FD Port:</source>
        <translation>Порт FD:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="225"/>
        <source>Client password</source>
        <translation>Пароль клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="227"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="229"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="235"/>
        <source>MD5 Hash:</source>
        <translation>Хеш MD5:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="245"/>
        <source>Passive mode (client behind NAT/firewall)</source>
        <translation>Пассивный режим (клиент за NAT/межсетевым экраном)</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="247"/>
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
        <location filename="../src/clients/bnewclientdialog.cpp" line="256"/>
        <source>Connection wait time:</source>
        <translation>Время ожидания соединения:</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="260"/>
        <source> sec</source>
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="261"/>
        <source>Time in seconds to wait for client-initiated connection</source>
        <translation>Время ожидания соединения, инициированного клиентом, в секундах</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="270"/>
        <source>Execute &quot;configure add client&quot; on Director</source>
        <translation>Выполнить «configure add client» на Director</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="273"/>
        <source>If checked, the wizard will send the configure command to the Director
on the Preview page to add this client automatically.</source>
        <translation>Если отмечено, мастер отправит команду configure на Director
на странице предпросмотра, чтобы автоматически добавить этого клиента.</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="409"/>
        <source>(enter password)</source>
        <translation>(введите пароль)</translation>
    </message>
</context>
<context>
    <name>BNewClientWizard</name>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="51"/>
        <source>Add New Client</source>
        <translation>Добавление нового клиента</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="65"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="67"/>
        <source>TLS Certificate</source>
        <translation>Сертификат TLS</translation>
    </message>
    <message>
        <location filename="../src/clients/bnewclientdialog.cpp" line="70"/>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
</context>
<context>
    <name>BNewJobDialog</name>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="24"/>
        <source>Run New Job</source>
        <translation>Запуск нового Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="50"/>
        <source>Run Backup Job</source>
        <translation>Запуск Job резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="60"/>
        <source>Command Preview</source>
        <translation>Предпросмотр команды</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="79"/>
        <source>Estimate</source>
        <translation>Оценка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="81"/>
        <source>Calculate estimated files and size</source>
        <translation>Рассчитать ожидаемое количество файлов и объём</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="91"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="665"/>
        <source>Run Job</source>
        <translation>Запустить Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="113"/>
        <source>Job Configuration</source>
        <translation>Конфигурация Job</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="118"/>
        <source>Select job...</source>
        <translation>Выберите Job...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="121"/>
        <source>Job:</source>
        <translation>Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="124"/>
        <source>Select client...</source>
        <translation>Выберите Client...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="127"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="133"/>
        <source>Level:</source>
        <translation>Уровень:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="138"/>
        <source>Resources</source>
        <translation>Ресурсы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="143"/>
        <source>Select FileSet...</source>
        <translation>Выберите FileSet...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="146"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="149"/>
        <source>Select Pool...</source>
        <translation>Выберите Pool...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="152"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="155"/>
        <source>Select Storage...</source>
        <translation>Выберите Storage...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="158"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="163"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="169"/>
        <source>Lower values = higher priority. Only sent if changed here; otherwise the job&apos;s own priority applies.</source>
        <translation>Меньшее значение = более высокий приоритет. Отправляется, только если изменено здесь; иначе действует собственный приоритет Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="176"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="182"/>
        <source>Basic</source>
        <translation>Основные</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="192"/>
        <source>Scheduling</source>
        <translation>Планирование</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="196"/>
        <source>e.g. &quot;2024-01-15 14:30:00&quot; or empty for immediate</source>
        <translation>напр., «2024-01-15 14:30:00» или пусто для немедленного запуска</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="197"/>
        <source>Start time in the form YYYY-MM-DD HH:MM:SS</source>
        <translation>Время начала в формате YYYY-MM-DD HH:MM:SS</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="199"/>
        <source>When:</source>
        <translation>Когда:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="204"/>
        <source>Bootstrap Options</source>
        <translation>Параметры Bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="207"/>
        <source>Use Bootstrap file</source>
        <translation>Использовать файл Bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="212"/>
        <source>Path to Bootstrap file</source>
        <translation>Путь к файлу Bootstrap</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="222"/>
        <source>Replace</source>
        <translation>Заменять</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="225"/>
        <source>Restore mode: Replace existing files</source>
        <translation>Режим восстановления: заменять существующие файлы</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="229"/>
        <source>Always</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="230"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="231"/>
        <source>If Newer</source>
        <translation>Если новее</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="232"/>
        <source>If Older</source>
        <translation>Если старее</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="242"/>
        <source>Advanced</source>
        <translation>Дополнительно</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="248"/>
        <source>⚠ No JobWidget connection available</source>
        <translation>⚠ Нет соединения с JobWidget</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="253"/>
        <source>Loading configuration data from JobWidget...</source>
        <translation>Загрузка данных конфигурации из JobWidget...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="291"/>
        <source>✓ %1 jobs, %2 clients loaded</source>
        <translation>✓ Загружено: Jobs — %1, клиентов — %2</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="297"/>
        <source>⚠ No jobs available - check connection</source>
        <translation>⚠ Нет доступных Jobs — проверьте соединение</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="383"/>
        <source>✓ Configuration loaded</source>
        <translation>✓ Конфигурация загружена</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="422"/>
        <source>Loading job defaults...</source>
        <translation>Загрузка параметров Job по умолчанию...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="450"/>
        <source>Failed to load job defaults</source>
        <translation>Не удалось загрузить параметры Job по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="561"/>
        <source>✓ Job defaults loaded</source>
        <translation>✓ Параметры Job по умолчанию загружены</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="592"/>
        <source>No job selected.</source>
        <translation>Job не выбран.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="594"/>
        <source>The value for &apos;%1&apos; contains a line break or control character.</source>
        <translation>Значение для «%1» содержит разрыв строки или управляющий символ.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="596"/>
        <source>&apos;When&apos; must be a valid time in the form YYYY-MM-DD HH:MM:SS.</source>
        <translation>«Когда» должно быть допустимым временем в формате YYYY-MM-DD HH:MM:SS.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="598"/>
        <source>The priority must be a positive number.</source>
        <translation>Приоритет должен быть положительным числом.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="600"/>
        <source>The command could not be built.</source>
        <translation>Не удалось сформировать команду.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="618"/>
        <source>Nothing will be sent: %1</source>
        <translation>Ничего не будет отправлено: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="645"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="686"/>
        <source>Missing Input</source>
        <translation>Недостаточно данных</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="646"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="687"/>
        <source>Please select a job.</source>
        <translation>Выберите Job.</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="654"/>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="694"/>
        <source>Invalid Input</source>
        <translation>Недопустимые данные</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="666"/>
        <source>Do you want to run the following job?

%1</source>
        <translation>Запустить следующий Job?

%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/bnewjobdialog.cpp" line="705"/>
        <source>Calculating estimate...</source>
        <translation>Расчёт оценки...</translation>
    </message>
</context>
<context>
    <name>BPFXConverter</name>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="27"/>
        <source>Convert to PFX Format</source>
        <translation>Преобразование в формат PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="35"/>
        <source>Convert separate PEM/DER certificate and key files into a single PFX file for Windows.
PFX files are password-protected and contain both certificate and private key.</source>
        <translation>Преобразование отдельных файлов сертификата и ключа PEM/DER в единый файл PFX для Windows.
Файлы PFX защищены паролем и содержат как сертификат, так и закрытый ключ.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="44"/>
        <source>Input Files</source>
        <translation>Входные файлы</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="50"/>
        <source>Select certificate file...</source>
        <translation>Выберите файл сертификата...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="51"/>
        <location filename="../src/bpfxconverter.cpp" line="61"/>
        <location filename="../src/bpfxconverter.cpp" line="71"/>
        <location filename="../src/bpfxconverter.cpp" line="102"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="55"/>
        <source>Certificate:</source>
        <translation>Сертификат:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="60"/>
        <source>Select private key file...</source>
        <translation>Выберите файл закрытого ключа...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="65"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="70"/>
        <source>Optional: CA certificate</source>
        <translation>Необязательно: сертификат CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="75"/>
        <source>CA Cert (Optional):</source>
        <translation>Сертификат CA (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="80"/>
        <source>PFX Password Protection</source>
        <translation>Защита PFX паролем</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="85"/>
        <source>Enter password for PFX file...</source>
        <translation>Введите пароль для файла PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="86"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="90"/>
        <source>Confirm password...</source>
        <translation>Подтвердите пароль...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="91"/>
        <source>Confirm:</source>
        <translation>Подтверждение:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="96"/>
        <source>Output</source>
        <translation>Вывод</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="101"/>
        <source>Select where to save PFX file...</source>
        <translation>Выберите, куда сохранить файл PFX...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="105"/>
        <source>Save PFX File</source>
        <translation>Сохранение файла PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="107"/>
        <source>PFX Files (*.pfx);;All Files (*)</source>
        <translation>Файлы PFX (*.pfx);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="117"/>
        <source>Save As:</source>
        <translation>Сохранить как:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="122"/>
        <source>Conversion Log:</source>
        <translation>Журнал преобразования:</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="133"/>
        <source>Convert to PFX</source>
        <translation>Преобразовать в PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="137"/>
        <source>Close</source>
        <translation>Закрыть</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="148"/>
        <source>Select Certificate File</source>
        <translation>Выбор файла сертификата</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="150"/>
        <source>Certificates (*.pem *.crt *.cert *.der);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert *.der);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="153"/>
        <source>✓ Certificate selected: %1</source>
        <translation>✓ Выбран сертификат: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="160"/>
        <source>Select Private Key File</source>
        <translation>Выбор файла закрытого ключа</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="162"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Закрытые ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="165"/>
        <source>✓ Private key selected: %1</source>
        <translation>✓ Выбран закрытый ключ: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="172"/>
        <source>Select CA Certificate</source>
        <translation>Выбор сертификата CA</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="174"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="177"/>
        <source>✓ CA certificate selected: %1</source>
        <translation>✓ Выбран сертификат CA: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="185"/>
        <location filename="../src/bpfxconverter.cpp" line="192"/>
        <location filename="../src/bpfxconverter.cpp" line="199"/>
        <source>Missing Input</source>
        <translation>Недостаточно данных</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="186"/>
        <source>Please select a certificate file.</source>
        <translation>Выберите файл сертификата.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="193"/>
        <source>Please select a private key file.</source>
        <translation>Выберите файл закрытого ключа.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="200"/>
        <source>Please enter a password to protect the PFX file.</source>
        <translation>Введите пароль для защиты файла PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="206"/>
        <source>Password Mismatch</source>
        <translation>Пароли не совпадают</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="207"/>
        <source>The passwords do not match. Please try again.</source>
        <translation>Пароли не совпадают. Повторите попытку.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="213"/>
        <source>Missing Output</source>
        <translation>Не указан выходной файл</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="214"/>
        <source>Please specify where to save the PFX file.</source>
        <translation>Укажите, куда сохранить файл PFX.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="224"/>
        <location filename="../src/bpfxconverter.cpp" line="226"/>
        <location filename="../src/bpfxconverter.cpp" line="233"/>
        <location filename="../src/bpfxconverter.cpp" line="236"/>
        <source>========================================</source>
        <translation>========================================</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="225"/>
        <source>Starting PFX Conversion</source>
        <translation>Начало преобразования в PFX</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="234"/>
        <source>✓ Conversion successful!</source>
        <translation>✓ Преобразование выполнено успешно!</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="235"/>
        <source>PFX file created: %1</source>
        <translation>Файл PFX создан: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="240"/>
        <source>Success</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="241"/>
        <source>PFX file created successfully!

%1

You can now use this file in the certificate settings.</source>
        <translation>Файл PFX успешно создан!

%1

Теперь этот файл можно использовать в параметрах сертификата.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="251"/>
        <source>Loading certificate from: %1</source>
        <translation>Загрузка сертификата из: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="256"/>
        <source>✗ ERROR: Cannot open certificate file</source>
        <translation>✗ ОШИБКА: не удалось открыть файл сертификата</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257"/>
        <location filename="../src/bpfxconverter.cpp" line="271"/>
        <location filename="../src/bpfxconverter.cpp" line="284"/>
        <location filename="../src/bpfxconverter.cpp" line="301"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="257"/>
        <source>Cannot open certificate file.</source>
        <translation>Не удалось открыть файл сертификата.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="270"/>
        <source>✗ ERROR: Cannot parse certificate (not valid PEM or DER)</source>
        <translation>✗ ОШИБКА: не удалось разобрать сертификат (недопустимый PEM или DER)</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="272"/>
        <source>Cannot parse certificate file. Make sure it&apos;s a valid PEM or DER certificate.</source>
        <translation>Не удалось разобрать файл сертификата. Убедитесь, что это допустимый сертификат PEM или DER.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="276"/>
        <source>✓ Certificate loaded: %1</source>
        <translation>✓ Сертификат загружен: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="279"/>
        <source>Loading private key from: %1</source>
        <translation>Загрузка закрытого ключа из: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="283"/>
        <source>✗ ERROR: Cannot open private key file</source>
        <translation>✗ ОШИБКА: не удалось открыть файл закрытого ключа</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="284"/>
        <source>Cannot open private key file.</source>
        <translation>Не удалось открыть файл закрытого ключа.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="300"/>
        <source>✗ ERROR: Cannot parse private key</source>
        <translation>✗ ОШИБКА: не удалось разобрать закрытый ключ</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="302"/>
        <source>Cannot parse private key file. Make sure it&apos;s a valid PEM or DER key.</source>
        <translation>Не удалось разобрать файл закрытого ключа. Убедитесь, что это допустимый ключ PEM или DER.</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="306"/>
        <source>✓ Private key loaded</source>
        <translation>✓ Закрытый ключ загружен</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="309"/>
        <source>Creating PFX (PKCS#12) file...</source>
        <translation>Создание файла PFX (PKCS#12)...</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="313"/>
        <source>⚠ Note: Full PFX conversion requires OpenSSL PKCS#12 API</source>
        <translation>⚠ Примечание: для полного преобразования в PFX требуется API OpenSSL PKCS#12</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="314"/>
        <source>  The certificate and key files are valid and can be loaded</source>
        <translation>  Файлы сертификата и ключа корректны и могут быть загружены</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="321"/>
        <source>✓ Files validated successfully</source>
        <translation>✓ Файлы успешно проверены</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="322"/>
        <source>  Certificate: %1</source>
        <translation>  Сертификат: %1</translation>
    </message>
    <message>
        <location filename="../src/bpfxconverter.cpp" line="323"/>
        <source>  Private Key: %1</source>
        <translation>  Закрытый ключ: %1</translation>
    </message>
</context>
<context>
    <name>BPaginationWidget</name>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="26"/>
        <source>Enable Pagination</source>
        <translation>Включить постраничный вывод</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="32"/>
        <source>Items per page:</source>
        <translation>Элементов на странице:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="46"/>
        <source>First Page</source>
        <translation>Первая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="52"/>
        <source>Previous Page</source>
        <translation>Предыдущая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="58"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="311"/>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="334"/>
        <source>Page 0 of 0</source>
        <translation>Страница 0 из 0</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="64"/>
        <source>Next Page</source>
        <translation>Следующая страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="70"/>
        <source>Last Page</source>
        <translation>Последняя страница</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="78"/>
        <source>Go to page:</source>
        <translation>Перейти к странице:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="85"/>
        <source>Go</source>
        <translation>Перейти</translation>
    </message>
    <message>
        <location filename="../src/jobs/bpaginationwidget.cpp" line="345"/>
        <source>Page %1 of %2</source>
        <translation>Страница %1 из %2</translation>
    </message>
</context>
<context>
    <name>BPermissionsDialog</name>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1985"/>
        <source>My Permissions</source>
        <translation>Мои права</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="1998"/>
        <source>Connection</source>
        <translation>Соединение</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2000"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2001"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2008"/>
        <source>TLS-PSK</source>
        <translation>TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2010"/>
        <source>TLS Certificate</source>
        <translation>Сертификат TLS</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2012"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2014"/>
        <source>Encryption:</source>
        <translation>Шифрование:</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2020"/>
        <source>Backup &amp; Restore</source>
        <translation>Резервное копирование и восстановление</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2021"/>
        <source>Job Control</source>
        <translation>Управление Jobs</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2022"/>
        <source>Administration</source>
        <translation>Администрирование</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2024"/>
        <source>Media</source>
        <translation>Носители</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2035"/>
        <source>Command Permissions</source>
        <translation>Права на команды</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2042"/>
        <source>Checking...</source>
        <translation>Проверка...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2051"/>
        <source>Restore Details</source>
        <translation>Сведения о Restore</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2053"/>
        <location filename="../src/bmainwindow.cpp" line="2062"/>
        <source>Loading...</source>
        <translation>Загрузка...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2060"/>
        <source>Other Available Commands</source>
        <translation>Другие доступные команды</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2069"/>
        <source>Raw Director Response</source>
        <translation>Необработанный ответ Director</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2087"/>
        <source>Querying Director...</source>
        <translation>Запрос к Director...</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2176"/>
        <source>N/A</source>
        <translation>Н/Д</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2179"/>
        <source>Authorized</source>
        <translation>Разрешено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2182"/>
        <source>Not authorized</source>
        <translation>Не разрешено</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2207"/>
        <source>&lt;b&gt;Permission:&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;Право:&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2211"/>
        <source>&lt;b&gt;where= (restore path):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;where= (путь восстановления):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2215"/>
        <source>&lt;b&gt;client= (target client):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;client= (целевой клиент):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2219"/>
        <source>&lt;b&gt;replace= (replace policy):&lt;/b&gt; %1&lt;br&gt;</source>
        <translation>&lt;b&gt;replace= (политика замены):&lt;/b&gt; %1&lt;br&gt;</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2225"/>
        <source>&lt;br&gt;&lt;b&gt;Available parameters:&lt;/b&gt; %1</source>
        <translation>&lt;br&gt;&lt;b&gt;Доступные параметры:&lt;/b&gt; %1</translation>
    </message>
    <message>
        <location filename="../src/bmainwindow.cpp" line="2246"/>
        <source>Total: %1 commands (%2 authorized, %3 denied)</source>
        <translation>Всего команд: %1 (разрешено %2, запрещено %3)</translation>
    </message>
</context>
<context>
    <name>BPoolResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374"/>
        <source>Pool Type</source>
        <translation>Pool Type</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="374"/>
        <source>Recycle</source>
        <translation>Recycle</translation>
    </message>
</context>
<context>
    <name>BProfileSettingsDialog</name>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="23"/>
        <source>Advanced Profile Settings - %1</source>
        <translation>Дополнительные параметры профиля — %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="40"/>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="41"/>
        <source>TLS</source>
        <translation>TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="42"/>
        <source>ACL</source>
        <translation>ACL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="65"/>
        <source>Optional description for this connection</source>
        <translation>Необязательное описание этого соединения</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="66"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="71"/>
        <source> seconds</source>
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="72"/>
        <source>Disabled</source>
        <translation>Выключено</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="73"/>
        <source>Keepalive interval (0 = disabled)</source>
        <translation>Интервал keepalive (0 = выключено)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="74"/>
        <source>Heartbeat Interval:</source>
        <translation>Heartbeat Interval:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="78"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="79"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="85"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="87"/>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="178"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="92"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Порт gRPC Director (по умолчанию: порт BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="93"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="102"/>
        <source>Forget certificate</source>
        <translation>Забыть сертификат</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="103"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Удалить закреплённый сертификат. При следующем подключении снова будет задан вопрос, доверять ли сертификату сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="106"/>
        <source>Server certificate:</source>
        <translation>Сертификат сервера:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="118"/>
        <source>e.g., operator or readonly</source>
        <translation>напр., operator или readonly</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="119"/>
        <source>Reference to a Profile resource on the Director.
The Profile defines ACLs for this console.</source>
        <translation>Ссылка на ресурс Profile на Director.
Profile определяет ACL для этой консоли.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="122"/>
        <source>Director Profile:</source>
        <translation>Profile Director:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="126"/>
        <source>&lt;b&gt;Note:&lt;/b&gt; The Director Profile is a server-side resource that defines permissions. If set, the ACLs in this profile are ignored in favor of the server-side Profile.</source>
        <translation>&lt;b&gt;Примечание:&lt;/b&gt; Profile Director — это ресурс на стороне сервера, определяющий права. Если он задан, ACL этого профиля игнорируются в пользу Profile на стороне сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="146"/>
        <source>TLS Options</source>
        <translation>Параметры TLS</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="149"/>
        <source>TLS Required</source>
        <translation>TLS обязателен</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="150"/>
        <source>Require TLS for all connections (recommended)</source>
        <translation>Требовать TLS для всех соединений (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="153"/>
        <source>TLS Authenticate Only</source>
        <translation>TLS только для аутентификации</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="154"/>
        <source>Use TLS only for authentication, not for data encryption</source>
        <translation>Использовать TLS только для аутентификации, без шифрования данных</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="160"/>
        <source>Cipher Configuration</source>
        <translation>Настройка шифров</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="164"/>
        <source>TLSv1.2 ciphers (colon-separated)</source>
        <translation>Шифры TLSv1.2 (через двоеточие)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="165"/>
        <source>OpenSSL cipher list for TLSv1.2 connections
Example: HIGH:!aNULL:!MD5</source>
        <translation>Список шифров OpenSSL для соединений TLSv1.2
Пример: HIGH:!aNULL:!MD5</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="167"/>
        <source>Cipher List (TLSv1.2):</source>
        <translation>Список шифров (TLSv1.2):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="170"/>
        <source>TLSv1.3 cipher suites (colon-separated)</source>
        <translation>Наборы шифров TLSv1.3 (через двоеточие)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="171"/>
        <source>OpenSSL cipher suites for TLSv1.3 connections
Example: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</source>
        <translation>Наборы шифров OpenSSL для соединений TLSv1.3
Пример: TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="173"/>
        <source>Cipher Suites (TLSv1.3):</source>
        <translation>Наборы шифров (TLSv1.3):</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="176"/>
        <source>e.g., -ALL,TLSv1.2,TLSv1.3</source>
        <translation>напр., -ALL,TLSv1.2,TLSv1.3</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="177"/>
        <source>OpenSSL protocol string</source>
        <translation>Строка протоколов OpenSSL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="183"/>
        <source>Certificate Files</source>
        <translation>Файлы сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="187"/>
        <source>DH Parameters:</source>
        <translation>Параметры DH:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="189"/>
        <source>Path to Diffie-Hellman parameters file</source>
        <translation>Путь к файлу параметров Диффи — Хеллмана</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="191"/>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="207"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="194"/>
        <source>Select DH Parameters File</source>
        <translation>Выбор файла параметров DH</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="195"/>
        <source>PEM Files (*.pem);;All Files (*)</source>
        <translation>Файлы PEM (*.pem);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="203"/>
        <source>CRL File:</source>
        <translation>Файл CRL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="205"/>
        <source>Path to Certificate Revocation List file</source>
        <translation>Путь к файлу списка отзыва сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="210"/>
        <source>Select CRL File</source>
        <translation>Выбор файла CRL</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="211"/>
        <source>CRL Files (*.crl *.pem);;All Files (*)</source>
        <translation>Файлы CRL (*.crl *.pem);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="221"/>
        <source>Allowed Certificate Common Names</source>
        <translation>Разрешённые общие имена сертификатов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="225"/>
        <source>One Common Name per line</source>
        <translation>Одно общее имя на строку</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="226"/>
        <source>List of allowed certificate Common Names.
Leave empty to allow all valid certificates.</source>
        <translation>Список разрешённых общих имён (Common Name) сертификатов.
Оставьте пустым, чтобы разрешить все действительные сертификаты.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="249"/>
        <source>&lt;b&gt;Access Control Lists (ACLs)&lt;/b&gt;&lt;br&gt;Define which resources this console can access. One entry per line. Use &lt;code&gt;*all*&lt;/code&gt; to allow all resources of that type. Regex patterns are supported for Job ACL.</source>
        <translation>&lt;b&gt;Списки управления доступом (ACL)&lt;/b&gt;&lt;br&gt;Определяют, к каким ресурсам имеет доступ эта консоль. Одна запись на строку. Используйте &lt;code&gt;*all*&lt;/code&gt;, чтобы разрешить все ресурсы этого типа. Для Job ACL поддерживаются регулярные выражения.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="271"/>
        <source>Job ACL:</source>
        <translation>Job ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272"/>
        <source>Allowed jobs</source>
        <translation>Разрешённые Jobs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="272"/>
        <source>List of allowed job names (regex supported)</source>
        <translation>Список разрешённых имён Jobs (поддерживаются регулярные выражения)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="275"/>
        <source>Client ACL:</source>
        <translation>Client ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>Allowed clients</source>
        <translation>Разрешённые Clients</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="276"/>
        <source>List of allowed client names</source>
        <translation>Список разрешённых имён Clients</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="280"/>
        <source>Storage ACL:</source>
        <translation>Storage ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281"/>
        <source>Allowed storages</source>
        <translation>Разрешённые Storages</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="281"/>
        <source>List of allowed storage names</source>
        <translation>Список разрешённых имён Storages</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="284"/>
        <source>Pool ACL:</source>
        <translation>Pool ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>Allowed pools</source>
        <translation>Разрешённые Pools</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="285"/>
        <source>List of allowed pool names</source>
        <translation>Список разрешённых имён Pools</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="289"/>
        <source>Schedule ACL:</source>
        <translation>Schedule ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290"/>
        <source>Allowed schedules</source>
        <translation>Разрешённые Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="290"/>
        <source>List of allowed schedule names</source>
        <translation>Список разрешённых имён Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="293"/>
        <source>FileSet ACL:</source>
        <translation>FileSet ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>Allowed filesets</source>
        <translation>Разрешённые FileSets</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="294"/>
        <source>List of allowed fileset names</source>
        <translation>Список разрешённых имён FileSets</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="298"/>
        <source>Catalog ACL:</source>
        <translation>Catalog ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299"/>
        <source>Allowed catalogs</source>
        <translation>Разрешённые Catalogs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="299"/>
        <source>List of allowed catalog names</source>
        <translation>Список разрешённых имён Catalogs</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="302"/>
        <source>Command ACL:</source>
        <translation>Command ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>Allowed commands</source>
        <translation>Разрешённые команды</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="303"/>
        <source>List of allowed commands</source>
        <translation>Список разрешённых команд</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="307"/>
        <source>Where ACL:</source>
        <translation>Where ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308"/>
        <source>Allowed restore locations</source>
        <translation>Разрешённые места восстановления</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="308"/>
        <source>List of allowed restore locations</source>
        <translation>Список разрешённых мест восстановления</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="311"/>
        <source>Plugin Options ACL:</source>
        <translation>Plugin Options ACL:</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>Allowed plugin options</source>
        <translation>Разрешённые параметры плагинов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="312"/>
        <source>List of allowed plugin options</source>
        <translation>Список разрешённых параметров плагинов</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="412"/>
        <source>Not used with gRPC: the connection is always encrypted with TLS, the server certificate is confirmed on first use.</source>
        <translation>Не используется с gRPC: соединение всегда шифруется TLS, сертификат сервера подтверждается при первом использовании.</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="427"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(не закреплён — будет подтверждён при следующем подключении)</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="433"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="445"/>
        <source>Reset to Defaults</source>
        <translation>Сбросить по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/config/bprofilesettingsdialog.cpp" line="446"/>
        <source>Are you sure you want to reset all advanced settings to their default values?</source>
        <translation>Вы действительно хотите сбросить все дополнительные параметры к значениям по умолчанию?</translation>
    </message>
</context>
<context>
    <name>BRecycleForecast</name>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="62"/>
        <source>now</source>
        <translation>сейчас</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="63"/>
        <source>never</source>
        <translation>никогда</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="66"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="74"/>
        <source>The Director did not report the status of this volume.</source>
        <translation>Director не сообщил статус этого Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="79"/>
        <source>Archived: Bareos never prunes or recycles an archived volume.</source>
        <translation>Archived: Bareos никогда не выполняет prune и recycle для архивного Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="82"/>
        <source>Disabled: Bareos neither writes to nor recycles a disabled volume.</source>
        <translation>Disabled: Bareos не записывает на выключенный Volume и не выполняет для него recycle.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="86"/>
        <source>Error: Bareos never prunes a volume in Error, so it keeps its space until its status is changed. The job records on it still expire.</source>
        <translation>Error: Bareos никогда не выполняет prune для Volume в состоянии Error, поэтому он занимает место, пока его статус не будет изменён. Записи о Jobs на нём всё равно истекают.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="90"/>
        <source>Read-Only: Bareos does not prune or recycle a read-only volume.</source>
        <translation>Read-Only: Bareos не выполняет prune и recycle для Volume только для чтения.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="93"/>
        <source>Cleaning tape: never used for data.</source>
        <translation>Чистящая лента: никогда не используется для данных.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="98"/>
        <source>Recycle = no: the volume is never reused; its data stays until it is relabeled or deleted.</source>
        <translation>Recycle = no: Volume никогда не используется повторно; его данные остаются, пока он не будет переразмечен или удалён.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="102"/>
        <source>Purged: no job needs it any more, reusable now.</source>
        <translation>Purged: ни одному Job он больше не нужен, может быть использован повторно сейчас.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="106"/>
        <source>It still holds %1 on disk until it is recycled or truncated.</source>
        <translation>Он по-прежнему занимает %1 на диске, пока не будет выполнен recycle или truncate.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="112"/>
        <source>Marked for recycling: reused at the next need of its pool.</source>
        <translation>Отмечен для recycle: будет использован повторно при следующей потребности его Pool.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="115"/>
        <source>Busy: in use by a job right now.</source>
        <translation>Busy: сейчас используется Job.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="118"/>
        <source>Unlabeled: not yet written.</source>
        <translation>Без метки: ещё не записан.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="124"/>
        <source>Append: the short volume list does not report the Volume Use Duration, so it is not known whether this volume stays open until it is full.</source>
        <translation>Append: краткий список Volumes не сообщает Volume Use Duration, поэтому неизвестно, останется ли этот Volume открытым до заполнения.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="128"/>
        <source>Append without Volume Use Duration: it stays open until it is full</source>
        <translation>Append без Volume Use Duration: остаётся открытым, пока не заполнится</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="130"/>
        <source>(%1)</source>
        <translation>(%1)</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="133"/>
        <source>or holds %1 jobs</source>
        <translation>или не вместит %1 Jobs</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="135"/>
        <source>; nothing on it is freed before that.</source>
        <translation>; до этого на нём ничего не освобождается.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="141"/>
        <source>Append with a use duration of %1, but the first write or the retention is not reported.</source>
        <translation>Append с длительностью использования %1, но первая запись или срок хранения не сообщаются.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="150"/>
        <source>Append until %1 (use duration %2), then %3 of volume retention.</source>
        <translation>Append до %1 (длительность использования %2), затем %3 срока хранения Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="158"/>
        <source>%1: the last write or the volume retention is not reported.</source>
        <translation>%1: последняя запись или срок хранения Volume не сообщаются.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="163"/>
        <source>%1: last written %2, volume retention %3.</source>
        <translation>%1: последняя запись %2, срок хранения Volume %3.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="174"/>
        <source>%1: the job retention of %2 ends before the volume retention of %3; the jobs are pruned first and the volume becomes Purged.</source>
        <translation>%1: срок хранения Jobs (%2) истекает раньше срока хранения Volume (%3); сначала выполняется prune для Jobs, и Volume получает статус Purged.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="182"/>
        <source>The date has passed: it is pruned when its pool next needs a volume.</source>
        <translation>Дата прошла: prune будет выполнен, когда его Pool в следующий раз понадобится Volume.</translation>
    </message>
    <message>
        <location filename="../src/storage/model/brecycleforecast.cpp" line="187"/>
        <source>Status &quot;%1&quot; is not known to this version.</source>
        <translation>Статус «%1» неизвестен этой версии.</translation>
    </message>
</context>
<context>
    <name>BResourceDialog</name>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="49"/>
        <source>Select %1:</source>
        <translation>Выберите %1:</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="67"/>
        <source>Edit %1 Resource</source>
        <translation>Изменение ресурса %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="77"/>
        <source>Show advanced directives</source>
        <translation>Показать дополнительные директивы</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="86"/>
        <location filename="../src/director/bresourcedialog.cpp" line="248"/>
        <source>Save .conf</source>
        <translation>Сохранить .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="91"/>
        <location filename="../src/director/bresourcedialog.cpp" line="325"/>
        <source>Save ZIP</source>
        <translation>Сохранить ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="223"/>
        <source>Save %1 Configuration</source>
        <translation>Сохранение конфигурации %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="224"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="230"/>
        <location filename="../src/director/bresourcedialog.cpp" line="238"/>
        <location filename="../src/director/bresourcedialog.cpp" line="273"/>
        <location filename="../src/director/bresourcedialog.cpp" line="281"/>
        <source>Save Failed</source>
        <translation>Ошибка сохранения</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="231"/>
        <location filename="../src/director/bresourcedialog.cpp" line="274"/>
        <source>%1 &quot;%2&quot; cannot be written. %3</source>
        <translation>%1 «%2» не удаётся записать. %3</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="239"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="246"/>
        <location filename="../src/director/bresourcedialog.cpp" line="323"/>
        <source>Saved!</source>
        <translation>Сохранено!</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="263"/>
        <source>Save %1 Configuration as ZIP</source>
        <translation>Сохранение конфигурации %1 в ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="264"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcedialog.cpp" line="282"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
</context>
<context>
    <name>BResourceForm</name>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="153"/>
        <source>No schema available for resource type &apos;%1&apos;</source>
        <translation>Нет схемы для типа ресурса «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="181"/>
        <location filename="../src/config/bresourceform.cpp" line="193"/>
        <location filename="../src/config/bresourceform.cpp" line="194"/>
        <location filename="../src/config/bresourceform.cpp" line="197"/>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="378"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="386"/>
        <source>Show/Hide password</source>
        <translation>Показать/скрыть пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="412"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414"/>
        <source>Browse for file</source>
        <translation>Выбрать файл</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="414"/>
        <source>Browse for directory</source>
        <translation>Выбрать каталог</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="418"/>
        <source>Select File</source>
        <translation>Выбор файла</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="420"/>
        <source>Select Directory</source>
        <translation>Выбор каталога</translation>
    </message>
    <message>
        <location filename="../src/config/bresourceform.cpp" line="449"/>
        <source>&lt;%1 name&gt;</source>
        <translation>&lt;имя %1&gt;</translation>
    </message>
</context>
<context>
    <name>BResourcePersistence</name>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="33"/>
        <source>schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="34"/>
        <source>job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="35"/>
        <source>JobDefs resource</source>
        <translation>ресурс JobDefs</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="36"/>
        <source>client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="37"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="38"/>
        <source>pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="39"/>
        <source>storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="42"/>
        <source>resource</source>
        <translation>ресурс</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="49"/>
        <source>%1 %2 was not created</source>
        <translation>%1 %2 не создан</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="50"/>
        <source>%1 %2 was not deleted</source>
        <translation>%1 %2 не удалён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="53"/>
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 не изменён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="105"/>
        <source>There is no connection to the configuration methods of the Director. Nothing was sent.</source>
        <translation>Нет подключения к методам конфигурации Director. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="118"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="139"/>
        <source>Nothing to write</source>
        <translation>Нечего записывать</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="140"/>
        <source>The text is the one the Director has.</source>
        <translation>Текст совпадает с текстом в Director.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="151"/>
        <source>The text the change was made in does not match its SHA-256. Reload the %1 and make the change again.</source>
        <translation>Текст, в котором было сделано изменение, не соответствует своему SHA-256. Перезагрузите %1 и внесите изменение заново.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="286"/>
        <source>%1 %2 created</source>
        <translation>%1 %2 создан</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="287"/>
        <source>%1 %2 deleted</source>
        <translation>%1 %2 удалён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="288"/>
        <source>%1 %2 changed</source>
        <translation>%1 %2 изменён</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="290"/>
        <source>The Director wrote the configuration and reloaded it.</source>
        <translation>Director записал конфигурацию и перезагрузил её.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="291"/>
        <source>The Director wrote the configuration but did not report a reload; the change takes effect with its next reload.</source>
        <translation>Director записал конфигурацию, но не сообщил о перезагрузке; изменение вступит в силу при следующей перезагрузке.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="294"/>
        <source>The reload does not adjust everything; the Director names these follow-up commands:</source>
        <translation>Перезагрузка применяет не всё; Director указывает следующие дополнительные команды:</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="299"/>
        <source>%1 %2 was changed on the Director</source>
        <translation>%1 %2 был изменён в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="300"/>
        <source>The Director has another state of the %1 than the one this change started from. Nothing was written and nothing is overwritten. Compare the two texts, reload the %1 and make the change again.</source>
        <translation>В Director находится другое состояние %1, чем то, с которого началось это изменение. Ничего не записано и ничего не перезаписано. Сравните оба текста, перезагрузите %1 и внесите изменение заново.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="307"/>
        <source>The Director checked the new configuration and rejected it. Nothing was written.</source>
        <translation>Director проверил новую конфигурацию и отклонил её. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="311"/>
        <source>The %1 is still used. The Director does not delete a %1 that other resources name; change them first. Nothing was deleted.</source>
        <translation>%1 всё ещё используется. Director не удаляет %1, на который ссылаются другие ресурсы; сначала измените их. Ничего не удалено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="317"/>
        <source>The console of this session may not change this %1 (the ACL of its resource type). Nothing was written.</source>
        <translation>Консоли этого сеанса не разрешено изменять этот %1 (ACL его типа ресурса). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="319"/>
        <source>The console of this session may not change this %1 (its %2 ACL). Nothing was written.</source>
        <translation>Консоли этого сеанса не разрешено изменять этот %1 (его ACL %2). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="324"/>
        <source>The Director has no %1 of this name (any more). Nothing was written.</source>
        <translation>В Director (больше) нет %1 с таким именем. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="328"/>
        <source>The Director already has a resource of this name. Nothing was written.</source>
        <translation>В Director уже есть ресурс с таким именем. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="332"/>
        <source>The Director wrote the file, but reloading the configuration failed, so it put the old file back. The %1 is as it was.</source>
        <translation>Director записал файл, но перезагрузка конфигурации не удалась, поэтому он вернул старый файл. %1 остался прежним.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="338"/>
        <source>This connection has no configuration methods. Nothing was sent.</source>
        <translation>У этого подключения нет методов конфигурации. Ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="344"/>
        <source>The Director refused the call (%1). The console needs the command ACL &quot;configure&quot;.</source>
        <translation>Director отклонил вызов (%1). Консоли нужен ACL команд «configure».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="346"/>
        <source>The call failed (%1). Nothing was written.</source>
        <translation>Вызов не удался (%1). Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcepersistence.cpp" line="350"/>
        <source>The Director sent no status. Nothing is known to have been written.</source>
        <translation>Director не прислал статус. Неизвестно, было ли что-либо записано.</translation>
    </message>
</context>
<context>
    <name>BResourceReportDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="229"/>
        <source>file %1</source>
        <translation>файл %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="231"/>
        <source>line %1</source>
        <translation>строка %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="233"/>
        <source>column %1</source>
        <translation>столбец %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="237"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="269"/>
        <source>This was found when the Director checked the change; nothing was sent to be written.</source>
        <translation>Это обнаружено, когда Director проверял изменение; ничего не отправлено на запись.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="275"/>
        <source>The Director says: %1</source>
        <translation>Director сообщает: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="279"/>
        <source>Where the configuration is invalid</source>
        <translation>Где конфигурация недействительна</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="288"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="291"/>
        <source>Still referenced by</source>
        <translation>Всё ещё используется в</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="299"/>
        <source>Follow-up commands the Director names</source>
        <translation>Дополнительные команды, указанные Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="306"/>
        <source>They are not run by this application.</source>
        <translation>Это приложение их не выполняет.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="320"/>
        <source>On the Director now</source>
        <translation>Сейчас в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="322"/>
        <source>What was to be written (not written)</source>
        <translation>Что должно было быть записано (не записано)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="328"/>
        <source>&amp;Reload the %1</source>
        <translation>&amp;Перезагрузить %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="330"/>
        <source>Reads the %1 again from the Director; your changes are offered to be kept or discarded</source>
        <translation>Повторно считывает %1 из Director; ваши изменения можно будет сохранить или отменить</translation>
    </message>
</context>
<context>
    <name>BResourceReviewDialog</name>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="80"/>
        <source>Change %1 %2</source>
        <translation>Изменить %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="81"/>
        <source>Change %1 %2 on the Director?</source>
        <translation>Изменить %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="82"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новую конфигурацию и принимает её. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="86"/>
        <source>Create %1 %2</source>
        <translation>Создать %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="87"/>
        <source>Create %1 %2 on the Director?</source>
        <translation>Создать %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="88"/>
        <source>The Director checked the new %1 and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новый %1 и принимает его. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="92"/>
        <source>Delete %1 %2</source>
        <translation>Удалить %1 %2</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="93"/>
        <source>Delete %1 %2 on the Director?</source>
        <translation>Удалить %1 %2 в Director?</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="94"/>
        <source>The Director checked that nothing refers to the %1. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Director проверил, что на %1 ничто не ссылается. Ничего не будет удалено, пока вы не выберете «Удалить в Director».</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="107"/>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="110"/>
        <source>Changed directives</source>
        <translation>Изменённые директивы</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="121"/>
        <source>Text before and after (- removed, + added)</source>
        <translation>Текст до и после (- удалено, + добавлено)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="123"/>
        <source>Text of the new resource</source>
        <translation>Текст нового ресурса</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="124"/>
        <source>Text that is deleted</source>
        <translation>Удаляемый текст</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="139"/>
        <source>Affected</source>
        <translation>Затрагивает</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/config/bresourcereviewdialog.cpp" line="143"/>
        <source>The change applies to every one of these %n resource(s).</source>
        <translation>
            <numerusform>Изменение применяется к этому %n ресурсу.</numerusform>
            <numerusform>Изменение применяется ко всем этим %n ресурсам.</numerusform>
            <numerusform>Изменение применяется ко всем этим %n ресурсам.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="152"/>
        <source>(nothing else, as far as this application knows)</source>
        <translation>(больше ничего, насколько известно этому приложению)</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="156"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Director указывает как ссылающиеся на него: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="166"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Файл в Director: %1 (в нём есть и другие ресурсы; заменяется только этот ресурс).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="168"/>
        <source>File on the Director: %1.</source>
        <translation>Файл в Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="170"/>
        <source>The Director reloads its configuration after writing. Jobs that are running or already queued keep the configuration they started with.</source>
        <translation>После записи Director перезагружает свою конфигурацию. Выполняющиеся и уже поставленные в очередь Jobs сохраняют конфигурацию, с которой они были запущены.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="175"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Удалить в Director</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcereviewdialog.cpp" line="176"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Записать в Director</translation>
    </message>
</context>
<context>
    <name>BResourceText</name>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="223"/>
        <source>The text does not start with a resource (line %1).</source>
        <translation>Текст не начинается с ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="225"/>
        <source>Directives on the line of &quot;%1 {&quot; cannot be edited safely (line %2).</source>
        <translation>Директивы в строке «%1 {» нельзя безопасно редактировать (строка %2).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="235"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Текст после закрывающей скобки ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="246"/>
        <source>The closing brace of the resource shares its line with a nested block (line %1).</source>
        <translation>Закрывающая скобка ресурса находится в одной строке с вложенным блоком (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="260"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Закрывающая скобка находится в одной строке с директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="333"/>
        <source>The text contains no resource.</source>
        <translation>Текст не содержит ресурса.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="335"/>
        <source>The resource has no closing brace.</source>
        <translation>У ресурса нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="385"/>
        <source>A change without a directive name.</source>
        <translation>Изменение без имени директивы.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="387"/>
        <source>Directive %1 is changed twice.</source>
        <translation>Директива %1 изменена дважды.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="391"/>
        <source>The value of %1 contains a line break.</source>
        <translation>Значение %1 содержит перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="393"/>
        <source>An empty value for %1; remove the directive instead.</source>
        <translation>Пустое значение для %1; вместо этого удалите директиву.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="396"/>
        <source>Directive %1 is on a line that cannot be edited safely (several directives on one line, a list or a nested block). Change it in the text.</source>
        <translation>Директива %1 находится в строке, которую нельзя безопасно редактировать (несколько директив в одной строке, список или вложенный блок). Измените её в тексте.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="400"/>
        <source>Directive %1 appears more than once. Change it in the text.</source>
        <translation>Директива %1 встречается более одного раза. Измените её в тексте.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="453"/>
        <source>The new text could not be read back: %1</source>
        <translation>Новый текст не удалось прочитать обратно: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="458"/>
        <source>The new text still has directive %1.</source>
        <translation>В новом тексте всё ещё есть директива %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="460"/>
        <source>The new text does not have the intended value of %1.</source>
        <translation>В новом тексте нет нужного значения %1.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="464"/>
        <source>The new text would change more than the edited directives.</source>
        <translation>Новый текст изменил бы больше, чем отредактированные директивы.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="466"/>
        <source>The new text would change the type of the resource.</source>
        <translation>Новый текст изменил бы тип ресурса.</translation>
    </message>
    <message>
        <location filename="../src/config/bresourcetext.cpp" line="468"/>
        <source>The new text would change the name of the resource.</source>
        <translation>Новый текст изменил бы имя ресурса.</translation>
    </message>
</context>
<context>
    <name>BResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="46"/>
        <source>Preview</source>
        <translation>Предпросмотр</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="48"/>
        <source>Toggle config file preview</source>
        <translation>Показать/скрыть предпросмотр файла конфигурации</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="60"/>
        <source>Edit...</source>
        <translation>Изменить...</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="66"/>
        <source>Export .conf</source>
        <translation>Экспорт .conf</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="72"/>
        <source>Export ZIP</source>
        <translation>Экспорт ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="85"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="136"/>
        <source>%1: %2 resource(s)</source>
        <translation>%1: ресурсов: %2</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="225"/>
        <source>%1 %2 was not changed</source>
        <translation>%1 %2 не изменён</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="243"/>
        <source>Not written, because the form cannot write lists or nested blocks: %1. Change them in the configuration file.</source>
        <translation>Не записано, так как форма не может записывать списки и вложенные блоки: %1. Измените их в файле конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="248"/>
        <source>Nothing was changed.</source>
        <translation>Ничего не изменено.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="249"/>
        <source>Nothing that can be written was changed. %1</source>
        <translation>Ничего из того, что можно записать, не изменено. %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="266"/>
        <source>Another change is still being written. Try again when it is done.</source>
        <translation>Другое изменение ещё записывается. Повторите попытку, когда оно завершится.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="276"/>
        <source>The text the Director sent does not match its SHA-256.</source>
        <translation>Текст, присланный Director, не соответствует своему SHA-256.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="278"/>
        <source>%1 Nothing was written.</source>
        <translation>%1 Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="285"/>
        <source>The change cannot be written into the text of the resource safely: %1 Nothing was written.</source>
        <translation>Изменение нельзя безопасно внести в текст ресурса: %1 Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="504"/>
        <source>Schedule &quot;%1&quot; is not written here.</source>
        <translation>Schedule «%1» здесь не записывается.</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="604"/>
        <source>Export %1 Configuration</source>
        <translation>Экспорт конфигурации %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="605"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="610"/>
        <location filename="../src/director/bresourcewidget.cpp" line="665"/>
        <source>Export Failed</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="611"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="656"/>
        <source>Export %1 Configuration as ZIP</source>
        <translation>Экспорт конфигурации %1 в ZIP</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="657"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidget.cpp" line="666"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
</context>
<context>
    <name>BRestoreBrowsePage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="479"/>
        <source>Select Files</source>
        <translation>Выбор файлов</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="480"/>
        <source>Browse and select files/directories to restore.</source>
        <translation>Просмотрите и выберите файлы/каталоги для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="492"/>
        <source>Loading files...</source>
        <translation>Загрузка файлов...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="518"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="699"/>
        <source>No files selected</source>
        <translation>Файлы не выбраны</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="531"/>
        <source>Step %1 of %2 — Browse and select files/directories to restore.</source>
        <translation>Шаг %1 из %2 — Просмотрите и выберите файлы/каталоги для восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="534"/>
        <source>Browse files — Client: %1, FileSet: %2</source>
        <translation>Просмотр файлов — Client: %1, FileSet: %2</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="708"/>
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n каталог</numerusform>
            <numerusform>%n каталога</numerusform>
            <numerusform>%n каталогов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="710"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="711"/>
        <source>Selected: %1</source>
        <translation>Выбрано: %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="713"/>
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
        <location filename="../src/jobs/brestorewizard.cpp" line="1170"/>
        <source>Execute Restore</source>
        <translation>Выполнение восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1171"/>
        <source>Sending commands to the Director...</source>
        <translation>Отправка команд в Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1175"/>
        <source>Preparing...</source>
        <translation>Подготовка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1205"/>
        <source>Step %1 of %2 — Sending commands to the Director...</source>
        <translation>Шаг %1 из %2 — Отправка команд в Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1214"/>
        <source>Step %v of %m</source>
        <translation>Шаг %v из %m</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1240"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1259"/>
        <source>No director connection</source>
        <translation>Нет подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1247"/>
        <source>Creating restore table...</source>
        <translation>Создание таблицы восстановления...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1248"/>
        <source>&gt;&gt;&gt; .bvfs_restore %1</source>
        <translation>&gt;&gt;&gt; .bvfs_restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1266"/>
        <source>Executing restore command...</source>
        <translation>Выполнение команды restore...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1267"/>
        <source>&gt;&gt;&gt; restore %1</source>
        <translation>&gt;&gt;&gt; restore %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1281"/>
        <source>Cleaning up restore table...</source>
        <translation>Очистка таблицы восстановления...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1284"/>
        <source>&gt;&gt;&gt; .bvfs_cleanup %1</source>
        <translation>&gt;&gt;&gt; .bvfs_cleanup %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1300"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1416"/>
        <source>.bvfs_restore failed: %1</source>
        <translation>Ошибка .bvfs_restore: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1304"/>
        <source>Restore table created successfully.</source>
        <translation>Таблица восстановления успешно создана.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1320"/>
        <source>Restore command failed: %1</source>
        <translation>Ошибка команды restore: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1324"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1378"/>
        <source>Hint: The Bareos Director restricts the &quot;where&quot; path via WhereACL. Ask your administrator to add the path &quot;%1&quot; to the WhereACL of your console configuration, or use a permitted restore path.</source>
        <translation>Подсказка: Bareos Director ограничивает путь «where» с помощью WhereACL. Попросите администратора добавить путь «%1» в WhereACL конфигурации вашей консоли или используйте разрешённый путь восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1347"/>
        <source>Restore job submitted successfully (Job ID: %1).</source>
        <translation>Job восстановления успешно отправлен (Job ID: %1).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1349"/>
        <source>Restore command accepted.</source>
        <translation>Команда restore принята.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1373"/>
        <source>Restore command failed.</source>
        <translation>Ошибка команды restore.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1392"/>
        <source>Restore job submitted successfully.</source>
        <translation>Job восстановления успешно отправлен.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1402"/>
        <source>WARNING: Unexpected restore response.</source>
        <translation>ПРЕДУПРЕЖДЕНИЕ: неожиданный ответ на восстановление.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1420"/>
        <source>Restore table created (text response).</source>
        <translation>Таблица восстановления создана (текстовый ответ).</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1435"/>
        <source>Cleanup complete.</source>
        <translation>Очистка завершена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1445"/>
        <source>Complete</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1448"/>
        <source>Restore job %1 submitted successfully!</source>
        <translation>Job восстановления %1 успешно отправлен!</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1449"/>
        <source>The restore job (ID: %1) is now running on the Director.
You can monitor its progress in the Jobs view.
Refresh the job list to see the restore job status.</source>
        <translation>Job восстановления (ID: %1) теперь выполняется в Director.
Следить за ходом можно в представлении Jobs.
Обновите список Jobs, чтобы увидеть статус восстановления.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1454"/>
        <source>Restore completed.</source>
        <translation>Восстановление завершено.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1455"/>
        <source>The restore commands have been executed.
Check the log above for details.</source>
        <translation>Команды восстановления выполнены.
Подробности см. в журнале выше.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1467"/>
        <source>ERROR: %1</source>
        <translation>ОШИБКА: %1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1470"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1471"/>
        <source>Restore failed</source>
        <translation>Восстановление не удалось</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1499"/>
        <source>command</source>
        <translation>команды</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1500"/>
        <source>Timeout waiting for %1 response (no response within %2 seconds)</source>
        <translation>Истекло время ожидания ответа %1 (нет ответа в течение %2 с)</translation>
    </message>
</context>
<context>
    <name>BRestoreOptionsPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="726"/>
        <source>Restore Options</source>
        <translation>Параметры восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="727"/>
        <source>Configure where and how to restore the selected files.</source>
        <translation>Укажите, куда и как восстанавливать выбранные файлы.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="733"/>
        <source>Source:</source>
        <translation>Источник:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="736"/>
        <source>Target Client:</source>
        <translation>Целевой Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="739"/>
        <source>Restore Where:</source>
        <translation>Восстановить в:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="742"/>
        <source>Always</source>
        <translation>Всегда</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="743"/>
        <source>Never</source>
        <translation>Никогда</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="744"/>
        <source>If Newer</source>
        <translation>Если новее</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="745"/>
        <source>If Older</source>
        <translation>Если старше</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="746"/>
        <source>Replace Policy:</source>
        <translation>Политика замены:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="770"/>
        <source>Step %1 of %2 — Configure where and how to restore the selected files.</source>
        <translation>Шаг %1 из %2 — Укажите, куда и как восстанавливать выбранные файлы.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="775"/>
        <source>%n directory(ies)</source>
        <translation>
            <numerusform>%n каталог</numerusform>
            <numerusform>%n каталога</numerusform>
            <numerusform>%n каталогов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="777"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="779"/>
        <source>nothing</source>
        <translation>ничего</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="785"/>
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="786"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="788"/>
        <source>Job %1: %2 — %3 selected (estimated size: %4)</source>
        <translation>Job %1: %2 — выбрано: %3 (оценочный размер: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="822"/>
        <source>Always overwrite existing files on the target, regardless of timestamps.</source>
        <translation>Всегда перезаписывать существующие файлы в месте назначения, независимо от отметок времени.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="826"/>
        <source>Never overwrite. Skip files that already exist on the target.</source>
        <translation>Никогда не перезаписывать. Пропускать файлы, которые уже существуют в месте назначения.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="830"/>
        <source>Only overwrite if the backed-up file is newer than the existing file.</source>
        <translation>Перезаписывать, только если файл из резервной копии новее существующего.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="834"/>
        <source>Only overwrite if the backed-up file is older than the existing file.</source>
        <translation>Перезаписывать, только если файл из резервной копии старше существующего.</translation>
    </message>
</context>
<context>
    <name>BRestorePreviewPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="872"/>
        <source>Command Preview</source>
        <translation>Предпросмотр команд</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="873"/>
        <source>Review the commands that will be sent to the Director.</source>
        <translation>Проверьте команды, которые будут отправлены в Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="883"/>
        <source>Permission Checks</source>
        <translation>Проверка прав</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="890"/>
        <source>Restore command:</source>
        <translation>Команда restore:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="891"/>
        <source>Target client:</source>
        <translation>Целевой Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="892"/>
        <source>Where path:</source>
        <translation>Путь where:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="914"/>
        <source>Click &quot;Commit&quot; to execute these commands on the Director.</source>
        <translation>Нажмите «Commit», чтобы выполнить эти команды в Director.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="927"/>
        <source>Step %1 of %2 — Review the commands that will be sent to the Director.</source>
        <translation>Шаг %1 из %2 — Проверьте команды, которые будут отправлены в Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="970"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="973"/>
        <source>%n directory(ies) (including all contents)</source>
        <translation>
            <numerusform>%n каталог (со всем содержимым)</numerusform>
            <numerusform>%n каталога (со всем содержимым)</numerusform>
            <numerusform>%n каталогов (со всем содержимым)</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="971"/>
        <source>%n individual file(s)</source>
        <translation>
            <numerusform>%n отдельный файл</numerusform>
            <numerusform>%n отдельных файла</numerusform>
            <numerusform>%n отдельных файлов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/jobs/brestorewizard.cpp" line="975"/>
        <source>%n file(s)</source>
        <translation>
            <numerusform>%n файл</numerusform>
            <numerusform>%n файла</numerusform>
            <numerusform>%n файлов</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="977"/>
        <source>No files or directories selected</source>
        <translation>Файлы и каталоги не выбраны</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="984"/>
        <source>~%1</source>
        <translation>~%1</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="985"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="987"/>
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
        <location filename="../src/jobs/brestorewizard.cpp" line="1014"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1015"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1016"/>
        <source>Checking...</source>
        <translation>Проверка...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1017"/>
        <source>Querying Director for permissions...</source>
        <translation>Запрос прав у Director...</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1029"/>
        <source>No Director connection</source>
        <translation>Нет подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1087"/>
        <source>restore command lacks client= parameter</source>
        <translation>в команде restore отсутствует параметр client=</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1092"/>
        <source>Available (subject to WhereACL)</source>
        <translation>Доступно (с учётом WhereACL)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1095"/>
        <source>Not available</source>
        <translation>Недоступно</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1098"/>
        <source>N/A (restore not authorized)</source>
        <translation>Н/Д (восстановление не разрешено)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1102"/>
        <source>Permission check completed.</source>
        <translation>Проверка прав завершена.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1119"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1121"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="1123"/>
        <source>Unknown (timeout)</source>
        <translation>Неизвестно (тайм-аут)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1125"/>
        <source>Permission check timed out — you can still proceed.</source>
        <translation>Истекло время проверки прав — вы всё равно можете продолжить.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1153"/>
        <source>Authorized</source>
        <translation>Разрешено</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="1156"/>
        <source>Not authorized</source>
        <translation>Не разрешено</translation>
    </message>
</context>
<context>
    <name>BRestoreSelectJobPage</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="146"/>
        <source>Restore Conditions</source>
        <translation>Условия восстановления</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="147"/>
        <source>Configure the restore scope before browsing files.</source>
        <translation>Задайте область восстановления перед просмотром файлов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="154"/>
        <source>Source Job:</source>
        <translation>Исходный Job:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="158"/>
        <source>Client:</source>
        <translation>Client:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="162"/>
        <source>FileSet:</source>
        <translation>FileSet:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="166"/>
        <source>All related backups (full restore chain)</source>
        <translation>Все связанные резервные копии (полная цепочка восстановления)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="175"/>
        <source>Restore Scope:</source>
        <translation>Область восстановления:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="181"/>
        <source>Before Date:</source>
        <translation>До даты:</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="208"/>
        <source>Step %1 of %2 — Configure the restore scope before browsing files.</source>
        <translation>Шаг %1 из %2 — Задайте область восстановления перед просмотром файлов.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="211"/>
        <location filename="../src/jobs/brestorewizard.cpp" line="416"/>
        <source>Job %1: %2 (Client: %3, FileSet: %4)</source>
        <translation>Job %1: %2 (Client: %3, FileSet: %4)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="217"/>
        <source>Only this specific job (ID: %1)</source>
        <translation>Только этот Job (ID: %1)</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="266"/>
        <source>Clients</source>
        <translation>Clients</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="267"/>
        <source>FileSets</source>
        <translation>FileSets</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="270"/>
        <source>Warning: Failed to load %1 from Director within timeout. The Director may be busy or the connection may be slow.</source>
        <translation>Предупреждение: не удалось загрузить %1 из Director за отведённое время. Возможно, Director занят или подключение медленное.</translation>
    </message>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="272"/>
        <source> and </source>
        <translation> и </translation>
    </message>
</context>
<context>
    <name>BRestoreWizard</name>
    <message>
        <location filename="../src/jobs/brestorewizard.cpp" line="114"/>
        <source>Restore Wizard — Job %1: %2</source>
        <translation>Мастер восстановления — Job %1: %2</translation>
    </message>
</context>
<context>
    <name>BRunOverridesEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="39"/>
        <source>not set</source>
        <translation>не задано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="40"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="41"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="83"/>
        <source>(job&apos;s setting)</source>
        <translation>(настройка Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="87"/>
        <source>The Director has not reported any %1 yet; refresh the view after connecting. Not set: the setting of the job applies.</source>
        <translation>Director ещё не сообщил ни одного %1; обновите представление после подключения. Не задано: действует настройка Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="89"/>
        <source>The %1 the Director reported. Not set: the setting of the job applies.</source>
        <translation>%1, сообщённые Director. Не задано: действует настройка Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="109"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="110"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="111"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="112"/>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="113"/>
        <source>pools</source>
        <translation>Pools</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="114"/>
        <source>storages</source>
        <translation>Storages</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="117"/>
        <source>set to</source>
        <translation>задано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="123"/>
        <source>Level of the job</source>
        <translation>Level Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="132"/>
        <source>The level the Run line names. &quot;Level of the job&quot;: the line names none and the level of the job applies.</source>
        <translation>Level, указанный в строке Run. «Level Job»: строка не указывает его, и действует Level Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="138"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="139"/>
        <source>Override the priority</source>
        <translation>Переопределить приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="140"/>
        <source>a time period, e.g. 8 hours</source>
        <translation>период времени, например 8 hours</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="141"/>
        <source>Empty: the setting of the job applies</source>
        <translation>Пусто: действует настройка Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="167"/>
        <source>&amp;Level:</source>
        <translation>&amp;Level:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="168"/>
        <source>&amp;Storage:</source>
        <translation>&amp;Storage:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="170"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="171"/>
        <source>&amp;Messages:</source>
        <translation>&amp;Messages:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="173"/>
        <source>&amp;Full pool:</source>
        <translation>&amp;Full pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="174"/>
        <source>P&amp;riority:</source>
        <translation>П&amp;риоритет:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="176"/>
        <source>&amp;Incremental pool:</source>
        <translation>&amp;Incremental pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="177"/>
        <source>Sp&amp;ool data:</source>
        <translation>Sp&amp;ool data:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="179"/>
        <source>&amp;Differential pool:</source>
        <translation>&amp;Differential pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="180"/>
        <source>&amp;Accurate:</source>
        <translation>&amp;Accurate:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="182"/>
        <source>&amp;Next pool:</source>
        <translation>&amp;Next pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="183"/>
        <source>Max run sched &amp;time:</source>
        <translation>Max run sched &amp;time:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="271"/>
        <source>%1 (not reported)</source>
        <translation>%1 (не сообщено)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="273"/>
        <source>%1 is named by the Run line, but the Director did not report it. It is kept as it is.</source>
        <translation>%1 указан в строке Run, но Director о нём не сообщил. Он оставлен без изменений.</translation>
    </message>
</context>
<context>
    <name>BRunParser</name>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="113"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="145"/>
        <source>A Run line cannot contain a line break.</source>
        <translation>Строка Run не может содержать перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="134"/>
        <source>Unexpected character &quot;%1&quot;.</source>
        <translation>Неожиданный символ «%1».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="163"/>
        <source>A quoted value is not closed.</source>
        <translation>Значение в кавычках не закрыто.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="393"/>
        <source>The Run line is empty.</source>
        <translation>Строка Run пуста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="408"/>
        <source>Expected an equals, got: %1</source>
        <translation>Ожидался знак равенства, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="412"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="457"/>
        <source>Expected a value after &quot;%1=&quot;.</source>
        <translation>Ожидалось значение после «%1=».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="423"/>
        <source>Expect a YES or NO, got: %1</source>
        <translation>Ожидалось YES или NO, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="433"/>
        <source>Job level field: %1 not found in run record</source>
        <translation>Поле уровня Job: %1 не найдено в записи run</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="440"/>
        <source>expected a positive integer, got: %1</source>
        <translation>ожидалось положительное целое число, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="446"/>
        <source>expected a time period, got: %1</source>
        <translation>ожидался период времени, получено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="497"/>
        <source>Unexpected token: %1</source>
        <translation>Неожиданный токен: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="502"/>
        <source>Day number out of range (1-31)</source>
        <translation>Номер дня вне диапазона (1-31)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="509"/>
        <source>Unexpected token: &quot;%1&quot;</source>
        <translation>Неожиданный токен: «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="519"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="653"/>
        <source>Week number out of range (0-53)</source>
        <translation>Номер недели вне диапазона (0-53)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="525"/>
        <source>Job type field: %1 in run record not found</source>
        <translation>Поле типа Job: %1 в записи run не найдено</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="555"/>
        <source>Time must be preceded by keyword AT.</source>
        <translation>Перед временем должно стоять ключевое слово AT.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="571"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="576"/>
        <location filename="../src/schedules/model/brunparser.cpp" line="585"/>
        <source>Bad time specification.</source>
        <translation>Неверное указание времени.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="610"/>
        <source>Bad day specification in modulo.</source>
        <translation>Неверное указание дня в modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="612"/>
        <source>Bad day specification, offset must always be &lt;= than modulo.</source>
        <translation>Неверное указание дня: смещение всегда должно быть &lt;= modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="622"/>
        <source>Week number out of range (0-53) in modulo</source>
        <translation>Номер недели вне диапазона (0-53) в modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="624"/>
        <source>Week interval out of range (1-53) in modulo</source>
        <translation>Интервал недель вне диапазона (1-53) в modulo</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="626"/>
        <source>Bad week number specification in modulo, offset must always be &lt;= than modulo.</source>
        <translation>Неверное указание номера недели в modulo: смещение всегда должно быть &lt;= modulo.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="633"/>
        <source>Bad modulo time specification. Format for weekdays is &apos;01/02&apos;, for yearweeks is &apos;w01/w02&apos;.</source>
        <translation>Неверное указание modulo. Формат для дней недели — &apos;01/02&apos;, для недель года — &apos;w01/w02&apos;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="646"/>
        <source>Bad day range specification.</source>
        <translation>Неверное указание диапазона дней.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="661"/>
        <source>Invalid month, week or position day range</source>
        <translation>Недопустимый диапазон месяцев, недель или позиций дня</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunparser.cpp" line="665"/>
        <source>Invalid month, weekday or position range</source>
        <translation>Недопустимый диапазон месяцев, дней недели или позиций</translation>
    </message>
</context>
<context>
    <name>BRunRuleDialog</name>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="94"/>
        <source>Only on these da&amp;ys of the month</source>
        <translation>Только в эти д&amp;ни месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="95"/>
        <source>Only in these &amp;weeks of the year</source>
        <translation>Только в эти &amp;недели года</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="96"/>
        <source>last</source>
        <translation>последняя</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="97"/>
        <source>&amp;Every hour</source>
        <translation>&amp;Каждый час</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="104"/>
        <source>Calendar rule</source>
        <translation>Календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="107"/>
        <source>A calendar rule runs by day of the month or by week of the year. It does not belong to a weekday and is not painted in the grid.</source>
        <translation>Календарное правило запускается по дню месяца или по неделе года. Оно не относится к дню недели и не отображается в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="114"/>
        <source>What runs</source>
        <translation>Что запускается</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="122"/>
        <source>Not checked: every day of the month</source>
        <translation>Не отмечено: каждый день месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="127"/>
        <source>Day %1</source>
        <translation>День %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="132"/>
        <source>Not checked: every week of the year. The weeks are numbered w00 to w53 as in a Run line.</source>
        <translation>Не отмечено: каждая неделя года. Недели нумеруются от w00 до w53, как в строке Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="138"/>
        <source>Week %1</source>
        <translation>Неделя %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="144"/>
        <source>And only</source>
        <translation>И только</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="146"/>
        <source>on these weekdays:</source>
        <translation>в эти дни недели:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="162"/>
        <source>in these weeks of the month:</source>
        <translation>в эти недели месяца:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="177"/>
        <source>The last seven days of a month</source>
        <translation>Последние семь дней месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="183"/>
        <source>in these months:</source>
        <translation>в эти месяцы:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="200"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="205"/>
        <source>%1 hours</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="208"/>
        <source>The run starts in every hour of the day (&quot;hourly&quot;)</source>
        <translation>Запуск в каждый час суток («hourly»)</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="210"/>
        <source>&amp;Minute of the hour:</source>
        <translation>&amp;Минута часа:</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="213"/>
        <source>Minute of the hour</source>
        <translation>Минута часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="226"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="291"/>
        <source>Odd</source>
        <translation>Нечётные</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="292"/>
        <source>Even</source>
        <translation>Чётные</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="293"/>
        <source>None</source>
        <translation>Нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="387"/>
        <source>Choose days of the month or weeks of the year. A run without them belongs to the grid and is painted there.</source>
        <translation>Выберите дни месяца или недели года. Запуск без них относится к сетке и отображается в ней.</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="420"/>
        <source>Run = %1</source>
        <translation>Run = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/brunruledialog.cpp" line="434"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
</context>
<context>
    <name>BRunScriptDialog</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="45"/>
        <source>Edit RunScript</source>
        <translation>Изменить RunScript</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="54"/>
        <source>/path/to/script.sh</source>
        <translation>/path/to/script.sh</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="55"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="58"/>
        <source>Select Script</source>
        <translation>Выбрать скрипт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="65"/>
        <source>Command:</source>
        <translation>Команда:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="70"/>
        <source>Runs When:</source>
        <translation>Runs When:</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="75"/>
        <source>Runs On Client</source>
        <translation>Runs On Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="79"/>
        <source>Runs On Failure</source>
        <translation>Runs On Failure</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="82"/>
        <source>Abort Job On Error</source>
        <translation>Abort Job On Error</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="86"/>
        <source>Fail Job On Error</source>
        <translation>Fail Job On Error</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97"/>
        <source>Validation</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="97"/>
        <source>Command is required.</source>
        <translation>Требуется команда.</translation>
    </message>
</context>
<context>
    <name>BRunScriptEditor</name>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140"/>
        <source>Command</source>
        <translation>Команда</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140"/>
        <source>Runs When</source>
        <translation>Runs When</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="140"/>
        <source>On Client</source>
        <translation>На Client</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="154"/>
        <source>+ Add Script</source>
        <translation>+ Добавить скрипт</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="155"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="156"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/jobs/bjobwizard.cpp" line="231"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BRunWriter</name>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="177"/>
        <source>The run has an empty hour, day, month or week set and cannot be written as a Run line.</source>
        <translation>В запуске задан пустой набор часов, дней, месяцев или недель, поэтому его нельзя записать как строку Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="182"/>
        <source>The minute %1 is out of range.</source>
        <translation>Минута %1 вне диапазона.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="246"/>
        <source>Run line &quot;%1&quot; is not understood (%2) and is never rewritten.</source>
        <translation>Строка Run «%1» не распознана (%2) и никогда не перезаписывается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="265"/>
        <source>The generated Run line &quot;%1&quot; does not read back: %2</source>
        <translation>Сгенерированная строка Run «%1» не читается обратно: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="270"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>Сгенерированная строка Run «%1» означает не то, что предполагалось.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/brunwriter.cpp" line="286"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
</context>
<context>
    <name>BScheduleBasicsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="516"/>
        <source>Schedule Basics</source>
        <translation>Основные параметры Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="517"/>
        <source>Configure the schedule name and select an optional template.</source>
        <translation>Задайте имя Schedule и при желании выберите шаблон.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="522"/>
        <source>Basic Settings</source>
        <translation>Основные настройки</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="526"/>
        <source>e.g., WeeklyCycle</source>
        <translation>например, WeeklyCycle</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="527"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="530"/>
        <source>e.g., Full Sunday, incremental daily</source>
        <translation>например, Full по воскресеньям, Incremental ежедневно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="531"/>
        <source>Description:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="533"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="540"/>
        <source>Template</source>
        <translation>Шаблон</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="544"/>
        <source>(Empty — build from scratch)</source>
        <translation>(Пусто — создать с нуля)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="550"/>
        <location filename="../src/schedules/bschedulewizard.cpp" line="596"/>
        <source>Select a template to pre-fill the schedule with common run directives.</source>
        <translation>Выберите шаблон, чтобы предварительно заполнить Schedule типичными директивами Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="609"/>
        <source>&lt;b&gt;%1&lt;/b&gt;: %2&lt;br&gt;%3 run directive(s)</source>
        <translation>&lt;b&gt;%1&lt;/b&gt;: %2&lt;br&gt;директив Run: %3</translation>
    </message>
</context>
<context>
    <name>BScheduleBrushDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="293"/>
        <source>New brush</source>
        <translation>Новая кисть</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="296"/>
        <source>A brush is what painting puts into a cell: a level, the minute of the hour and, if wanted, settings that replace those of the job.</source>
        <translation>Кисть — это то, что рисование помещает в ячейку: Level, минута часа и, при необходимости, настройки, заменяющие настройки Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="305"/>
        <source>Minute of the hour</source>
        <translation>Минута часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="306"/>
        <source>The run starts at this minute of every painted hour</source>
        <translation>Запуск начинается в эту минуту каждого закрашенного часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="307"/>
        <source>Minute of the &amp;hour:</source>
        <translation>Минута &amp;часа:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="310"/>
        <source>Level and overrides</source>
        <translation>Level и переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="366"/>
        <source>Brush: %1</source>
        <translation>Кисть: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleEntry</name>
    <message>
        <location filename="../src/models/bresourcemodels.cpp" line="679"/>
        <source>The entry does not describe a run.</source>
        <translation>Запись не описывает запуск.</translation>
    </message>
</context>
<context>
    <name>BScheduleGanttWidget</name>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="405"/>
        <source>(no client)</source>
        <translation>(нет Client)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="408"/>
        <source>(no storage)</source>
        <translation>(нет Storage)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="435"/>
        <source>Not used by any job</source>
        <translation>Не используется ни одним Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="551"/>
        <source>No runs in this week</source>
        <translation>На этой неделе запусков нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="552"/>
        <source>No runs on this date</source>
        <translation>В эту дату запусков нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582"/>
        <source>Monday</source>
        <translation>Понедельник</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582"/>
        <source>Tuesday</source>
        <translation>Вторник</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582"/>
        <source>Wednesday</source>
        <translation>Среда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="582"/>
        <source>Thursday</source>
        <translation>Четверг</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583"/>
        <source>Friday</source>
        <translation>Пятница</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583"/>
        <source>Saturday</source>
        <translation>Суббота</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="583"/>
        <source>Sunday</source>
        <translation>Воскресенье</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="586"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="587"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1076"/>
        <source>%1, if it turns out against it</source>
        <translation>%1, если выйдет иначе</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1146"/>
        <source>Utilization</source>
        <translation>Загрузка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1373"/>
        <source>Not used by any enabled job: nothing starts at this time.</source>
        <translation>Не используется ни одним включённым Job: в это время ничего не запускается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1374"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1391"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1377"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1389"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1379"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1398"/>
        <source>Run</source>
        <translation>Запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1390"/>
        <source>(level of the job)</source>
        <translation>(Level Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1402"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1405"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1411"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1414"/>
        <source>Predicted wait</source>
        <translation>Прогноз ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1411"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1414"/>
        <source>Predicted wait: none</source>
        <translation>Прогноз ожидания: нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1425"/>
        <source>the jobs of the Director are not loaded: it is not known which job runs this schedule</source>
        <translation>Jobs Director не загружены: неизвестно, какой Job использует этот Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1430"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>нет успешных запусков этого Job за последние %1 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1434"/>
        <source>No history</source>
        <translation>Нет истории</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1435"/>
        <source>Duration unknown: the outline marks the start only.</source>
        <translation>Длительность неизвестна: контур отмечает только начало.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1438"/>
        <source>Median duration</source>
        <translation>Медианная длительность</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1440"/>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1447"/>
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1442"/>
        <source>runs</source>
        <translation>запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1445"/>
        <source>Median wait</source>
        <translation>Медианное ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450"/>
        <source>Wait</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1450"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1454"/>
        <source>Estimate: no run at this level in the last %1 days, figures from all levels of the job.</source>
        <translation>Оценка: нет запусков с этим Level за последние %1 дн., данные по всем Level этого Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1460"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1463"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1472"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1474"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1478"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1508"/>
        <source>job(s) running</source>
        <translation>выполняющихся Jobs</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1512"/>
        <source>%n run(s) predicted to wait</source>
        <translation>
            <numerusform>Ожидается, что %n запуск будет ждать</numerusform>
            <numerusform>Ожидается, что %n запуска будут ждать</numerusform>
            <numerusform>Ожидается, что %n запусков будут ждать</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleganttwidget.cpp" line="1517"/>
        <source>%1 run(s) without history start here (not counted: duration unknown)</source>
        <translation>Запусков без истории, начинающихся здесь: %1 (не учтены: длительность неизвестна)</translation>
    </message>
</context>
<context>
    <name>BScheduleGridEditor</name>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="29"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="76"/>
        <source>Undone: %1</source>
        <translation>Отменено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="40"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="85"/>
        <source>Redone: %1</source>
        <translation>Повторено: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="203"/>
        <source>Eraser</source>
        <translation>Ластик</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="256"/>
        <source>This schedule contains a Run line that is not understood. It is locked: nothing in it can be changed here.</source>
        <translation>Этот Schedule содержит нераспознанную строку Run. Он заблокирован: здесь ничего в нём изменить нельзя.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="283"/>
        <source>%n cell(s)</source>
        <translation>
            <numerusform>%n ячейка</numerusform>
            <numerusform>%n ячейки</numerusform>
            <numerusform>%n ячеек</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="285"/>
        <source>%1 of the scope %2</source>
        <translation>%1 области %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="297"/>
        <source>Paint %1 on %2</source>
        <translation>Нарисовать %1 на %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="306"/>
        <source>Add %1 to %2</source>
        <translation>Добавить %1 к %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="314"/>
        <source>Erase %1</source>
        <translation>Стереть %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="322"/>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="348"/>
        <source>No cell is selected.</source>
        <translation>Ни одна ячейка не выбрана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="330"/>
        <source>No brush is chosen.</source>
        <translation>Кисть не выбрана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="374"/>
        <source>A scope needs at least one month, and a week of the month or &quot;last&quot;.</source>
        <translation>Области нужен хотя бы один месяц и неделя месяца или «последняя».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="384"/>
        <source>Add the scope %1</source>
        <translation>Добавить область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="399"/>
        <source>The scope &quot;%1&quot; is always there.</source>
        <translation>Область «%1» есть всегда.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="404"/>
        <source>The scope %1 still has Run lines. Erase its cells first.</source>
        <translation>В области %1 ещё есть строки Run. Сначала сотрите её ячейки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="410"/>
        <source>Remove the scope %1</source>
        <translation>Удалить область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="420"/>
        <source>Add a calendar rule</source>
        <translation>Добавить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="431"/>
        <source>Change the calendar rule %1</source>
        <translation>Изменить календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegrideditor.cpp" line="443"/>
        <source>Remove the calendar rule %1</source>
        <translation>Удалить календарное правило %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="715"/>
        <source>Run line %1 (&quot;%2&quot;) is not understood: %3</source>
        <translation>Строка Run %1 («%2») не распознана: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="724"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1177"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1226"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1251"/>
        <source>There is no Run line %1.</source>
        <translation>Строки Run %1 нет.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1179"/>
        <source>Run line %1 is not a line of the grid and is not changed here.</source>
        <translation>Строка Run %1 не является строкой сетки и здесь не изменяется.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1207"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1230"/>
        <source>A run without days of the month and without weeks of the year is not a calendar rule; it is painted in the grid.</source>
        <translation>Запуск без дней месяца и без недель года не является календарным правилом; он отображается в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1228"/>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1253"/>
        <source>Run line %1 is not a calendar rule.</source>
        <translation>Строка Run %1 не является календарным правилом.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridmodel.cpp" line="1288"/>
        <source>The generated Run line &quot;%1&quot; does not say what was intended.</source>
        <translation>Сгенерированная строка Run «%1» означает не то, что предполагалось.</translation>
    </message>
</context>
<context>
    <name>BScheduleGridText</name>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="74"/>
        <source>Monday</source>
        <translation>Понедельник</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="75"/>
        <source>Tuesday</source>
        <translation>Вторник</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="76"/>
        <source>Wednesday</source>
        <translation>Среда</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="77"/>
        <source>Thursday</source>
        <translation>Четверг</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="78"/>
        <source>Friday</source>
        <translation>Пятница</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="79"/>
        <source>Saturday</source>
        <translation>Суббота</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="80"/>
        <source>Sunday</source>
        <translation>Воскресенье</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="88"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="89"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="90"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="91"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="92"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="93"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="94"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="102"/>
        <source>January</source>
        <translation>Январь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="103"/>
        <source>February</source>
        <translation>Февраль</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="104"/>
        <source>March</source>
        <translation>Март</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="105"/>
        <source>April</source>
        <translation>Апрель</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="106"/>
        <source>May</source>
        <translation>Май</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="107"/>
        <source>June</source>
        <translation>Июнь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="108"/>
        <source>July</source>
        <translation>Июль</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="109"/>
        <source>August</source>
        <translation>Август</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="110"/>
        <source>September</source>
        <translation>Сентябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="111"/>
        <source>October</source>
        <translation>Октябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="112"/>
        <source>November</source>
        <translation>Ноябрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="113"/>
        <source>December</source>
        <translation>Декабрь</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="122"/>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="128"/>
        <source>%1th</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="124"/>
        <source>%1st</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="125"/>
        <source>%1nd</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="126"/>
        <source>%1rd</source>
        <translation>%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="142"/>
        <source>Every week</source>
        <translation>Каждая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="157"/>
        <source>all</source>
        <translation>все</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="187"/>
        <source>last week only</source>
        <translation>только последняя неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="190"/>
        <source>first week only</source>
        <translation>только первая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="191"/>
        <source>second week only</source>
        <translation>только вторая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="192"/>
        <source>third week only</source>
        <translation>только третья неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="193"/>
        <source>fourth week only</source>
        <translation>только четвёртая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="194"/>
        <source>fifth week only</source>
        <translation>только пятая неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="199"/>
        <source>only %1</source>
        <translation>только %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="207"/>
        <source>level of the job</source>
        <translation>Level Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="250"/>
        <source>pool %1</source>
        <translation>pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="251"/>
        <source>full pool %1</source>
        <translation>full pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="252"/>
        <source>incremental pool %1</source>
        <translation>incremental pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="253"/>
        <source>differential pool %1</source>
        <translation>differential pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="254"/>
        <source>next pool %1</source>
        <translation>next pool %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="255"/>
        <source>storage %1</source>
        <translation>storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="256"/>
        <source>messages %1</source>
        <translation>messages %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="257"/>
        <source>priority %1</source>
        <translation>приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258"/>
        <source>spool data yes</source>
        <translation>spool data yes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="258"/>
        <source>spool data no</source>
        <translation>spool data no</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259"/>
        <source>accurate yes</source>
        <translation>accurate yes</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="259"/>
        <source>accurate no</source>
        <translation>accurate no</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="260"/>
        <source>max run sched time %1</source>
        <translation>max run sched time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="266"/>
        <source>%1 at :%2</source>
        <translation>%1 в :%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="276"/>
        <source>%1 at %2</source>
        <translation>%1 в %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="308"/>
        <source>%1 %2 hours</source>
        <translation>%1, %2 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="316"/>
        <source>%1: no run</source>
        <translation>%1: нет запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="325"/>
        <source>the Run line names no time</source>
        <translation>в строке Run не указано время</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="338"/>
        <source>no run in this scope</source>
        <translation>в этой области нет запусков</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="342"/>
        <source>in other scopes: %1</source>
        <translation>в других областях: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="369"/>
        <source>and</source>
        <translation>и</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="370"/>
        <source>or</source>
        <translation>или</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="378"/>
        <source>%1 to %2</source>
        <translation>%1–%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="400"/>
        <source>on every %1 day (starting with the %2)</source>
        <translation>каждый %1-й день (начиная с %2-го числа)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="402"/>
        <source>on the %1</source>
        <translation>%1 числа</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="405"/>
        <source>%1 of every month</source>
        <translation>%1 каждого месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="406"/>
        <source>%1 of %2</source>
        <translation>%1 (месяцы: %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="408"/>
        <source>in %1</source>
        <translation>в месяцы: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="417"/>
        <source>in every %1 week of the year (starting with week %2)</source>
        <translation>каждую %1-ю неделю года (начиная с недели %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="425"/>
        <source>in week %1 of the year</source>
        <translation>на неделе %1 года</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="426"/>
        <source>in weeks %1 of the year</source>
        <translation>на неделях %1 года</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="433"/>
        <source>in the last seven days of the month</source>
        <translation>в последние семь дней месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="435"/>
        <source>in the %1 week of the month</source>
        <translation>на %1-й неделе месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="438"/>
        <source>or in its last seven days</source>
        <translation>или в его последние семь дней</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="452"/>
        <source>only if it is a %1</source>
        <translation>только если это %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="454"/>
        <source>on %1</source>
        <translation>в дни: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="462"/>
        <source>every hour at :00 (the line names no time)</source>
        <translation>каждый час в :00 (в строке не указано время)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="464"/>
        <source>every hour at :%1</source>
        <translation>каждый час в :%1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulegridtext.cpp" line="472"/>
        <source>at %1</source>
        <translation>в %1</translation>
    </message>
</context>
<context>
    <name>BScheduleGridView</name>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="255"/>
        <source>Undo: %1 (Ctrl+Z)</source>
        <translation>Отменить: %1 (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="256"/>
        <source>Nothing to undo (Ctrl+Z)</source>
        <translation>Нечего отменять (Ctrl+Z)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="257"/>
        <source>Redo: %1 (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Повторить: %1 (Ctrl+Shift+Z или Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="258"/>
        <source>Nothing to redo (Ctrl+Shift+Z or Ctrl+Y)</source>
        <translation>Нечего повторять (Ctrl+Shift+Z или Ctrl+Y)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="293"/>
        <source>No schedule chosen. Select a schedule in the list on the left to see it as a grid of weekdays and hours.</source>
        <translation>Schedule не выбран. Выберите Schedule в списке слева, чтобы увидеть его в виде сетки дней недели и часов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="334"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="335"/>
        <source>Undo</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="337"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="338"/>
        <source>Redo</source>
        <translation>Повторить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="340"/>
        <source>Revert all</source>
        <translation>Отменить все</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="341"/>
        <source>Undoes every change of this schedule. Redo brings them back.</source>
        <translation>Отменяет все изменения этого Schedule. «Повторить» возвращает их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="343"/>
        <source>&amp;Save...</source>
        <translation>&amp;Сохранить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="345"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="788"/>
        <source>Writes the changed Run lines to the Director. The Director checks them first, and you see old and new lines and the jobs that use the schedule before anything is written.</source>
        <translation>Записывает изменённые строки Run в Director. Сначала Director проверяет их, и до записи вы видите старые и новые строки, а также Jobs, использующие этот Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="350"/>
        <source>Delete...</source>
        <translation>Удалить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="351"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="792"/>
        <source>Deletes this schedule on the Director, after a check that no job uses it and your confirmation</source>
        <translation>Удаляет этот Schedule в Director после проверки, что ни один Job его не использует, и вашего подтверждения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="366"/>
        <source>No description</source>
        <translation>Нет описания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="367"/>
        <source>The Description of the schedule. Saved with the Run lines; empty removes it from the file.</source>
        <translation>Description этого Schedule. Сохраняется вместе со строками Run; пустое значение удаляет её из файла.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="399"/>
        <source>&amp;Keep my changes</source>
        <translation>&amp;Сохранить мои изменения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="400"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="861"/>
        <source>&amp;Discard and reload</source>
        <translation>&amp;Отменить и перезагрузить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="410"/>
        <source>Scopes</source>
        <translation>Области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="414"/>
        <source>Scope:</source>
        <translation>Область:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="415"/>
        <source>The weeks of the month and the months a Run line is restricted to. The grid shows the runs of the chosen scope; runs of the other scopes are hatched.</source>
        <translation>Недели месяца и месяцы, которыми ограничена строка Run. Сетка показывает запуски выбранной области; запуски других областей заштрихованы.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="422"/>
        <source>Add scope</source>
        <translation>Добавить область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="423"/>
        <source>Add scope: weeks of the month, and months, to paint a grid of their own for</source>
        <translation>Добавить область: недели месяца и месяцы, для которых рисуется отдельная сетка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="448"/>
        <source>Legend</source>
        <translation>Легенда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="469"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="473"/>
        <source>Run lines</source>
        <translation>Строки Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478"/>
        <source>No.</source>
        <translation>№</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="478"/>
        <source>Shown in</source>
        <translation>Показано в</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="479"/>
        <source>The Run lines of the schedule in their order. Choosing a line outlines its cells in the grid and switches to its scope.</source>
        <translation>Строки Run этого Schedule по порядку. Выбор строки обводит её ячейки в сетке и переключает на её область.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="495"/>
        <source>&amp;Add rule...</source>
        <translation>&amp;Добавить правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="496"/>
        <source>Add calendar rule</source>
        <translation>Добавить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="497"/>
        <source>Adds a calendar rule: a Run line by day of the month or by week of the year, such as &quot;on 1 at 21:00&quot;</source>
        <translation>Добавляет календарное правило: строку Run по дню месяца или по неделе года, например «on 1 at 21:00»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="499"/>
        <source>&amp;Edit rule...</source>
        <translation>&amp;Изменить правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="500"/>
        <source>Edit calendar rule</source>
        <translation>Изменить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="501"/>
        <source>Opens the form of the calendar rule that is chosen in the list</source>
        <translation>Открывает форму календарного правила, выбранного в списке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="502"/>
        <source>&amp;Remove rule</source>
        <translation>&amp;Удалить правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="503"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1360"/>
        <source>Remove calendar rule</source>
        <translation>Удалить календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="504"/>
        <source>Removes the calendar rule that is chosen in the list</source>
        <translation>Удаляет календарное правило, выбранное в списке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="514"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="518"/>
        <source>Calendar rules</source>
        <translation>Календарные правила</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523"/>
        <source>Mark</source>
        <translation>Метка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="523"/>
        <source>Runs</source>
        <translation>Запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="524"/>
        <source>Run lines by day of the month or week of the year. They do not belong to a weekday and are marked in the band below the grid. Enter or a double click opens the form of a rule, Del removes it.</source>
        <translation>Строки Run по дню месяца или неделе года. Они не относятся к дню недели и отмечаются в полосе под сеткой. Enter или двойной щелчок открывает форму правила, Del удаляет его.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="542"/>
        <source>Next five weeks</source>
        <translation>Следующие пять недель</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="548"/>
        <source>The levels that run on each date</source>
        <translation>Level, выполняемые в каждую дату</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="553"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="554"/>
        <source>Previous month</source>
        <translation>Предыдущий месяц</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="557"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="558"/>
        <source>Next month</source>
        <translation>Следующий месяц</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="560"/>
        <source>Today</source>
        <translation>Сегодня</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="561"/>
        <source>Back to the current week</source>
        <translation>Вернуться к текущей неделе</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="721"/>
        <source>Modified: the description, not written to the Director.</source>
        <translation>Изменено: описание, не записано в Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="736"/>
        <source>%n changed</source>
        <translation>
            <numerusform>%n изменена</numerusform>
            <numerusform>%n изменены</numerusform>
            <numerusform>%n изменено</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="738"/>
        <source>%n new</source>
        <translation>
            <numerusform>%n новая</numerusform>
            <numerusform>%n новые</numerusform>
            <numerusform>%n новых</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="740"/>
        <source>%n removed</source>
        <translation>
            <numerusform>%n удалена</numerusform>
            <numerusform>%n удалены</numerusform>
            <numerusform>%n удалено</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="742"/>
        <source>Modified: %1 Run line(s) and the description, not written to the Director.</source>
        <translation>Изменено: строк Run — %1 и описание, не записано в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="744"/>
        <source>Modified: %1 Run line(s), not written to the Director.</source>
        <translation>Изменено строк Run: %1, не записано в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="723"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="747"/>
        <source>The Director reports other Run lines since.</source>
        <translation>С тех пор Director сообщает другие строки Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="362"/>
        <source>Descr&amp;iption:</source>
        <translation>&amp;Описание:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="790"/>
        <source>This connection cannot change a schedule; the click says why.</source>
        <translation>Через это подключение нельзя изменить Schedule; щелчок покажет причину.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="794"/>
        <source>This connection cannot delete a schedule; the click says why.</source>
        <translation>Через это подключение нельзя удалить Schedule; щелчок покажет причину.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="858"/>
        <source>The Director now reports other Run lines for this schedule than the ones your changes started from. Your changes are kept and nothing was replaced.</source>
        <translation>Director теперь сообщает для этого Schedule другие строки Run, чем те, с которых начались ваши изменения. Ваши изменения сохранены, ничего не заменено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="863"/>
        <source>The schedule %1 has changes that are not written. Showing %2 discards them.</source>
        <translation>В Schedule %1 есть незаписанные изменения. Показ %2 отменит их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="865"/>
        <source>&amp;Discard and show %1</source>
        <translation>&amp;Отменить и показать %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="892"/>
        <source>Schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="897"/>
        <source>(jobs not loaded yet)</source>
        <translation>(Jobs ещё не загружены)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="899"/>
        <source>Not used by any job</source>
        <translation>Не используется ни одним Job</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="901"/>
        <source>Used by %n job(s)</source>
        <translation>
            <numerusform>Используется в %n Job</numerusform>
            <numerusform>Используется в %n Jobs</numerusform>
            <numerusform>Используется в %n Jobs</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="907"/>
        <source>Disabled: the Director does not run it.</source>
        <translation>Отключено: Director его не запускает.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="912"/>
        <source>Contains %n Run line(s) that are not understood. The schedule is locked: it cannot be edited or written here. Change it in its configuration file on the Director.</source>
        <translation>
            <numerusform>Содержит %n нераспознанную строку Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
            <numerusform>Содержит %n нераспознанные строки Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
            <numerusform>Содержит %n нераспознанных строк Run. Schedule заблокирован: здесь его нельзя изменить или записать. Измените его в файле конфигурации в Director.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="941"/>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1164"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="944"/>
        <source>%1: no Run line in this scope. Del or the context menu removes it.</source>
        <translation>%1: в этой области нет строк Run. Удалить её можно клавишей Del или через контекстное меню.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="945"/>
        <source>%1: no Run line in this scope</source>
        <translation>%1: в этой области нет строк Run</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="946"/>
        <source>%1: %n Run line(s) in this scope</source>
        <translation>
            <numerusform>%1: %n строка Run в этой области</numerusform>
            <numerusform>%1: %n строки Run в этой области</numerusform>
            <numerusform>%1: %n строк Run в этой области</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="952"/>
        <source>Scope %1, %n Run line(s)</source>
        <translation>
            <numerusform>Область %1, %n строка Run</numerusform>
            <numerusform>Область %1, %n строки Run</numerusform>
            <numerusform>Область %1, %n строк Run</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="989"/>
        <source>other scope, may run on the same day</source>
        <translation>другая область, может выполняться в тот же день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="993"/>
        <source>other scope, never on the same day</source>
        <translation>другая область, никогда не в тот же день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1007"/>
        <source>no time in the Run line: runs every hour at :00</source>
        <translation>в строке Run нет времени: запуск каждый час в :00</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1015"/>
        <source>calendar rule, not a cell</source>
        <translation>календарное правило, не ячейка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1045"/>
        <source>changed</source>
        <translation>изменено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1048"/>
        <source>new</source>
        <translation>новое</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1051"/>
        <source>removed</source>
        <translation>удалено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1063"/>
        <source>Reported by the Director as &quot;%1&quot;, a spelling it does not read back.</source>
        <translation>Director сообщает это как «%1» — в написании, которое он не читает обратно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1070"/>
        <source>not understood: %1</source>
        <translation>не распознано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1071"/>
        <source>Not understood and not shown in the grid: %1</source>
        <translation>Не распознано и не показано в сетке: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1072"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Приложение не может записать этот Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1081"/>
        <source>calendar rule %1</source>
        <translation>календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1086"/>
        <source>scope %1</source>
        <translation>область %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1108"/>
        <source>%1 %2</source>
        <translation>%1 %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1137"/>
        <source>Nothing selected. Click a cell or drag over cells; click a day or an hour to select all of it.</source>
        <translation>Ничего не выбрано. Щёлкните ячейку или протяните по ячейкам; щёлкните день или час, чтобы выбрать его целиком.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1172"/>
        <source>%1 in %n cell(s)</source>
        <translation>
            <numerusform>%1 в %n ячейке</numerusform>
            <numerusform>%1 в %n ячейках</numerusform>
            <numerusform>%1 в %n ячейках</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridview.cpp" line="1176"/>
        <source>%n cell(s) selected:</source>
        <translation>
            <numerusform>Выбрана %n ячейка:</numerusform>
            <numerusform>Выбрано %n ячейки:</numerusform>
            <numerusform>Выбрано %n ячеек:</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1177"/>
        <source>no run in this scope</source>
        <translation>в этой области нет запусков</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1180"/>
        <source>Other scopes: %1</source>
        <translation>Другие области: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1181"/>
        <source>They stay when this scope is painted or erased</source>
        <translation>Они сохраняются при рисовании или стирании в этой области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1286"/>
        <source>Remove the scope &quot;%1&quot;</source>
        <translation>Удалить область «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1289"/>
        <source>Add scope...</source>
        <translation>Добавить область...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1328"/>
        <source>Calendar rule %1</source>
        <translation>Календарное правило %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1331"/>
        <source>New calendar rule</source>
        <translation>Новое календарное правило</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1359"/>
        <source>Edit calendar rule...</source>
        <translation>Изменить календарное правило...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridview.cpp" line="1364"/>
        <source>Add calendar rule...</source>
        <translation>Добавить календарное правило...</translation>
    </message>
</context>
<context>
    <name>BScheduleGridWidget</name>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="68"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="412"/>
        <source>%1 hours</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="69"/>
        <source>Select all</source>
        <translation>Выбрать все</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="73"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="199"/>
        <source>Selects the whole day</source>
        <translation>Выбирает весь день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="75"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="201"/>
        <source>Selects this hour on every day</source>
        <translation>Выбирает этот час во все дни</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="77"/>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="202"/>
        <source>Selects all days and hours</source>
        <translation>Выбирает все дни и часы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="187"/>
        <source>Apply the tool</source>
        <translation>Применить инструмент</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="189"/>
        <source>Erase</source>
        <translation>Стереть</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="197"/>
        <source>Selects this cell</source>
        <translation>Выбирает эту ячейку</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="205"/>
        <source>Lists the runs of this cell</source>
        <translation>Показывает запуски этой ячейки</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="207"/>
        <source>Applies the tool in hand (%1) to this cell</source>
        <translation>Применяет текущий инструмент (%1) к этой ячейке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="211"/>
        <source>Removes the runs of the scope on display from this cell</source>
        <translation>Удаляет из этой ячейки запуски отображаемой области</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="310"/>
        <source>Schedule grid of %1, scope %2</source>
        <translation>Сетка Schedule %1, область %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="314"/>
        <source>Weekdays by hours. Arrow keys move, Shift extends the selection, Enter lists the runs of a cell.</source>
        <translation>Дни недели по часам. Стрелки перемещают, Shift расширяет выделение, Enter показывает запуски ячейки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="317"/>
        <source>Space applies the tool to the selected cells, Delete erases them, F, D, I and V paint a level.</source>
        <translation>Пробел применяет инструмент к выбранным ячейкам, Delete стирает их, F, D, I и V рисуют Level.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="319"/>
        <source>Tool: %1.</source>
        <translation>Инструмент: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="321"/>
        <source>Paint mode is on.</source>
        <translation>Режим рисования включён.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridaccessible.cpp" line="325"/>
        <source>Calendar rule %1: %2, %3</source>
        <translation>Календарное правило %1: %2, %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="59"/>
        <source>Schedule grid</source>
        <translation>Сетка Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="281"/>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="631"/>
        <source>Calendar rules</source>
        <translation>Календарные правила</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1239"/>
        <source>%1, %2 to %3</source>
        <translation>%1, %2–%3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1249"/>
        <source>the Run line names no time</source>
        <translation>в строке Run не указано время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1252"/>
        <source>Run line %1: %2</source>
        <translation>Строка Run %1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1259"/>
        <source>No run in the scope &quot;%1&quot;</source>
        <translation>Нет запусков в области «%1»</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1266"/>
        <source>Other scopes:</source>
        <translation>Другие области:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1270"/>
        <source>%1: may run on the same day as this scope</source>
        <translation>%1: может выполняться в тот же день, что и эта область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1271"/>
        <source>%1: never on the same day as this scope</source>
        <translation>%1: никогда не в тот же день, что и эта область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1277"/>
        <source>Painting or erasing here changes only the scope &quot;%1&quot;; the runs of other scopes stay.</source>
        <translation>Рисование или стирание здесь изменяет только область «%1»; запуски других областей сохраняются.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1284"/>
        <source>Expected load: %1 runs at the same time</source>
        <translation>Ожидаемая нагрузка: одновременных запусков: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1299"/>
        <source>%1: click to select the whole day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1: щёлкните, чтобы выбрать весь день. Ctrl добавляет или убирает его, Shift расширяет выделение от опорной точки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1303"/>
        <source>%1 to %2: click to select this hour on every day. Ctrl adds or removes it, Shift extends the selection from its anchor.</source>
        <translation>%1–%2: щёлкните, чтобы выбрать этот час во все дни. Ctrl добавляет или убирает его, Shift расширяет выделение от опорной точки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1308"/>
        <source>Click to select all days and hours (Ctrl+A)</source>
        <translation>Щёлкните, чтобы выбрать все дни и часы (Ctrl+A)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1310"/>
        <source>Run lines by day of the month or week of the year do not belong to a weekday. They are marked here at their hours and listed under &quot;Calendar rules&quot;; this band cannot be selected or painted. A double click on a mark opens the form of its rule.</source>
        <translation>Строки Run по дню месяца или неделе года не относятся к дню недели. Они отмечены здесь в своё время и перечислены в разделе «Календарные правила»; эту полосу нельзя выделить или закрасить. Двойной щелчок по метке открывает форму её правила.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1391"/>
        <source>%1, %2 to %3: %n run(s)</source>
        <translation>
            <numerusform>%1, %2–%3: %n запуск</numerusform>
            <numerusform>%1, %2–%3: %n запуска</numerusform>
            <numerusform>%1, %2–%3: %n запусков</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1399"/>
        <source>Runs of the cell</source>
        <translation>Запуски ячейки</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404"/>
        <source>Overrides</source>
        <translation>Переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404"/>
        <source>Scope</source>
        <translation>Область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1404"/>
        <source>Run line</source>
        <translation>Строка Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1417"/>
        <source>no time in the Run line</source>
        <translation>в строке Run нет времени</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1423"/>
        <source>%1 (other scope, may run on the same day)</source>
        <translation>%1 (другая область, может выполняться в тот же день)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1424"/>
        <source>%1 (other scope, never on the same day)</source>
        <translation>%1 (другая область, никогда не в тот же день)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1431"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1443"/>
        <source>Enter or a double click shows the Run line in the list. Esc closes.</source>
        <translation>Enter или двойной щелчок показывает строку Run в списке. Esc закрывает.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegridwidget.cpp" line="1544"/>
        <source>Tool: %1</source>
        <translation>Инструмент: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleLegend</name>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="40"/>
        <source>Full backup. The length of the bar is the median duration of the job&apos;s measured runs at this level.</source>
        <translation>Full backup. Длина полосы — медианная длительность измеренных запусков Job на этом уровне.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="42"/>
        <source>Differential backup.</source>
        <translation>Differential backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="44"/>
        <source>Incremental backup.</source>
        <translation>Incremental backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="46"/>
        <source>Virtual full backup.</source>
        <translation>VirtualFull backup.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="48"/>
        <source>Pale and dashed, &quot;~&quot; in front of the letter: no run at this level was measured; the length is estimated from the job&apos;s runs at other levels.</source>
        <translation>Бледная и пунктирная, «~» перед буквой: ни один запуск на этом уровне не измерен; длина оценена по запускам Job на других уровнях.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="51"/>
        <source>Dashed outline with &quot;?&quot;: the job never ran, so there is no duration. Only the start is shown.</source>
        <translation>Пунктирный контур с «?»: Job ещё ни разу не выполнялся, поэтому длительность неизвестна. Показано только начало.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="53"/>
        <source>Lighter extension with a line at its end: nine of ten runs ended by then (90th percentile).</source>
        <translation>Более светлое продолжение с чертой на конце: к этому моменту завершились девять из десяти запусков (90-й процентиль).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="55"/>
        <source>Filled hourglass: the run will wait. The coloured line behind it is the scheduled time.</source>
        <translation>Заполненные песочные часы: запуск будет ждать. Цветная линия за ними — запланированное время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="57"/>
        <source>Hourglass outline: the run may wait, the prediction is not certain (see the list of predicted waits).</source>
        <translation>Контур песочных часов: запуск может ждать, прогноз не точен (см. список прогнозируемых ожиданий).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="59"/>
        <source>Hatched strip: the wait, from the scheduled time to the predicted start. A dashed outline: not certain.</source>
        <translation>Заштрихованная полоса: ожидание от запланированного времени до прогнозируемого начала. Пунктирный контур: не точно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="61"/>
        <source>&quot;?&quot; in front of a bar: the prediction for this run is not certain, it may have to wait.</source>
        <translation>«?» перед полосой: прогноз для этого запуска не точен, возможно, ему придётся ждать.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="63"/>
        <source>&quot;»&quot; at the right edge: the run starts after the last day on display.</source>
        <translation>«»» у правого края: запуск начинается после последнего отображаемого дня.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="65"/>
        <source>Open diamond: no job uses this schedule. It marks where a run would start.</source>
        <translation>Пустой ромб: этот Schedule не используется ни одним Job. Он отмечает, где начался бы запуск.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="67"/>
        <source>Arrow (day view only): the run at the head waits for the run at the tail, from the moment that one lets it go. The label names the limit; dotted: priority; dashed: only if something unknown turns out against the run. Point at a run to see its arrows.</source>
        <translation>Стрелка (только в представлении дня): запуск у острия ждёт запуск у хвоста — с того момента, как тот его отпустит. Подпись называет ограничение; точечная: приоритет; пунктирная: только если что-то неизвестное обернётся против запуска. Наведите указатель на запуск, чтобы увидеть его стрелки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="71"/>
        <source>Red line: now.</source>
        <translation>Красная линия: сейчас.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulelegend.cpp" line="259"/>
        <source>&lt;b&gt;What the marks mean&lt;/b&gt;</source>
        <translation>&lt;b&gt;Что означают метки&lt;/b&gt;</translation>
    </message>
</context>
<context>
    <name>BScheduleMiniCalendar</name>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="85"/>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="110"/>
        <source>Calendar of runs, %1</source>
        <translation>Календарь запусков, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="116"/>
        <source>%1 to %2</source>
        <translation>%1 – %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="125"/>
        <source>%1: no run</source>
        <translation>%1: нет запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bscheduleminicalendar.cpp" line="128"/>
        <source>%1 %2 (Run line %3)</source>
        <translation>%1 %2 (строка Run %3)</translation>
    </message>
</context>
<context>
    <name>BSchedulePalette</name>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="124"/>
        <source>Brushes</source>
        <translation>Кисти</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="136"/>
        <location filename="../src/schedules/bschedulepalette.cpp" line="137"/>
        <source>Eraser</source>
        <translation>Ластик</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="138"/>
        <source>Removes the runs of the scope on display from cells. Del erases the selected cells with any tool in hand.</source>
        <translation>Удаляет из ячеек запуски отображаемой области. Del стирает выбранные ячейки при любом инструменте.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="141"/>
        <source>New brush...</source>
        <translation>Новая кисть...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="143"/>
        <source>Defines a brush: level, minute of the hour and overrides</source>
        <translation>Определяет кисть: Level, минуту часа и переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="145"/>
        <source>Paint mode</source>
        <translation>Режим рисования</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="147"/>
        <source>On: pressing a cell and dragging over cells applies the tool at once. Off: the mouse selects, and Space applies the tool to the selection. The key P switches it in the grid.</source>
        <translation>Вкл.: нажатие на ячейку и протягивание по ячейкам сразу применяет инструмент. Выкл.: мышь выделяет, а пробел применяет инструмент к выделению. Клавиша P переключает режим в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="151"/>
        <source>Keep other runs</source>
        <translation>Сохранять другие запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="153"/>
        <source>On: painting adds the brush to the runs a cell already has, so a cell can have several runs in one hour. Off: the cell ends up with the brush only. Shift+Space adds once without this switch.</source>
        <translation>Вкл.: рисование добавляет кисть к уже имеющимся в ячейке запускам, так что в ячейке может быть несколько запусков в один час. Выкл.: в ячейке остаётся только кисть. Shift+Space добавляет один раз без этого переключателя.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="198"/>
        <source>%1 = %2</source>
        <translation>%1 = %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="246"/>
        <source>Brush %1</source>
        <translation>Кисть %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="247"/>
        <source>Takes this brush in hand: %1. Space applies it to the selected cells.</source>
        <translation>Взять эту кисть: %1. Пробел применяет её к выбранным ячейкам.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="308"/>
        <source>In hand: the eraser.</source>
        <translation>Текущий инструмент: ластик.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="310"/>
        <source>In hand: the brush %1.</source>
        <translation>Текущий инструмент: кисть %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="314"/>
        <source>Paint mode: press a cell or drag over cells to apply it.</source>
        <translation>Режим рисования: нажмите на ячейку или протяните по ячейкам, чтобы применить.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="316"/>
        <source>Space applies it to the selected cells, Del erases them.</source>
        <translation>Пробел применяет к выбранным ячейкам, Del стирает их.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepalette.cpp" line="319"/>
        <source>Other runs of a painted cell stay.</source>
        <translation>Другие запуски закрашенной ячейки сохраняются.</translation>
    </message>
</context>
<context>
    <name>BSchedulePersistence</name>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="103"/>
        <source>The text of schedule %1 arrived changed</source>
        <translation>Текст Schedule %1 пришёл изменённым</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="104"/>
        <source>The SHA-256 of the text the Director sent does not match the text. Nothing was written.</source>
        <translation>SHA-256 текста, присланного Director, не соответствует тексту. Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="117"/>
        <source>Schedule %1 has changed on the Director</source>
        <translation>Schedule %1 изменён в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="118"/>
        <source>The configuration file of the schedule does not say what was loaded here, so the change was not sent. Reload the schedule and make the change again. %1</source>
        <translation>Файл конфигурации Schedule не соответствует тому, что было загружено здесь, поэтому изменение не отправлено. Перезагрузите Schedule и внесите изменение заново. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="146"/>
        <source>Schedule %1 cannot be written</source>
        <translation>Schedule %1 нельзя записать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="147"/>
        <source>Run line %1 is not understood by this application (%2). A schedule with such a line is not changed here; change it in its configuration file.</source>
        <translation>Строка Run %1 не распознана этим приложением (%2). Schedule с такой строкой здесь не изменяется; измените его в файле конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="156"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="189"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="276"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="317"/>
        <source>Schedule %1 was not changed</source>
        <translation>Schedule %1 не изменён</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="157"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="376"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="447"/>
        <source>There is no connection to the configuration methods of the Director.</source>
        <translation>Нет подключения к методам конфигурации Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="190"/>
        <source>The change could not be written into the configuration text safely: %1</source>
        <translation>Изменение не удалось безопасно внести в текст конфигурации: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="198"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="264"/>
        <source>Nothing to write</source>
        <translation>Нечего записывать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="199"/>
        <source>The Run lines and the description are the ones the Director has.</source>
        <translation>Строки Run и описание совпадают с теми, что есть у Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="265"/>
        <source>The edited text is the one the Director has.</source>
        <translation>Отредактированный текст совпадает с текстом в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="277"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="417"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="493"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="316"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="446"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="492"/>
        <source>Schedule %1 was not created</source>
        <translation>Schedule %1 не создан</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="318"/>
        <source>The edited text was not sent to the Director: %1</source>
        <translation>Отредактированный текст не отправлен в Director: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="326"/>
        <source>the name of the schedule has to stay %1.</source>
        <translation>имя Schedule должно остаться %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="338"/>
        <source>The text was edited in the review; the Director checked the edited text.</source>
        <translation>Текст был отредактирован при проверке; Director проверил отредактированный текст.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="375"/>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="416"/>
        <source>Schedule %1 was not deleted</source>
        <translation>Schedule %1 не удалён</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulepersistence.cpp" line="573"/>
        <source>Run line %1 cannot be written: %2</source>
        <translation>Строку Run %1 нельзя записать: %2</translation>
    </message>
</context>
<context>
    <name>BSchedulePreviewPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="674"/>
        <source>Preview &amp; Deploy</source>
        <translation>Предпросмотр и развёртывание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="675"/>
        <source>Review the schedule configuration and deploy to the Director.</source>
        <translation>Проверьте конфигурацию Schedule и разверните её в Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="680"/>
        <source>Configuration:</source>
        <translation>Конфигурация:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="691"/>
        <source>Copy Configuration</source>
        <translation>Копировать конфигурацию</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="698"/>
        <source>Director Command:</source>
        <translation>Команда Director:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="709"/>
        <source>Copy Command</source>
        <translation>Копировать команду</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="756"/>
        <source>CreateResource (gRPC) with the configuration text above</source>
        <translation>CreateResource (gRPC) с приведённым выше текстом конфигурации</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="767"/>
        <source>This schedule cannot be written. %1</source>
        <translation>Этот Schedule нельзя записать. %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="780"/>
        <source>Schedule cannot be written</source>
        <translation>Schedule нельзя записать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="781"/>
        <source>%1

Go back and remove or replace this run, or cancel the wizard.</source>
        <translation>%1

Вернитесь назад и удалите или замените этот запуск либо отмените мастер.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="796"/>
        <source>Deploy to Director</source>
        <translation>Развернуть в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="797"/>
        <source>Send &apos;configure add&apos; to the Director?

Click Yes to execute, No to close without deploying.</source>
        <translation>Отправить &apos;configure add&apos; в Director?

Нажмите «Да», чтобы выполнить, или «Нет», чтобы закрыть без развёртывания.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="819"/>
        <source>Sending configure command...</source>
        <translation>Отправка команды configure...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="846"/>
        <source>The Director checks the schedule...</source>
        <translation>Director проверяет Schedule...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="853"/>
        <location filename="../src/schedules/bschedulewizard.cpp" line="884"/>
        <source>Schedule created successfully! Click Finish to close.</source>
        <translation>Schedule успешно создан! Нажмите «Готово», чтобы закрыть.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="856"/>
        <source>Nothing was written.</source>
        <translation>Ничего не записано.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="859"/>
        <source>%1: %2</source>
        <translation>%1: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="887"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="897"/>
        <source>The connection to the Director was lost before it answered. Reconnect and check with &apos;show schedules&apos; whether the schedule exists.</source>
        <translation>Подключение к Director было потеряно до его ответа. Подключитесь заново и проверьте командой &apos;show schedules&apos;, существует ли Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="905"/>
        <source>Timeout waiting for Director response. The schedule may have been created — check with &apos;show schedules&apos;.</source>
        <translation>Истекло время ожидания ответа Director. Возможно, Schedule был создан — проверьте командой &apos;show schedules&apos;.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="922"/>
        <source>Configuration copied to clipboard.</source>
        <translation>Конфигурация скопирована в буфер обмена.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="929"/>
        <source>Command copied to clipboard.</source>
        <translation>Команда скопирована в буфер обмена.</translation>
    </message>
</context>
<context>
    <name>BScheduleResource</name>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="97"/>
        <source>The text does not start with a Schedule resource (line %1).</source>
        <translation>Текст не начинается с ресурса Schedule (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="101"/>
        <source>Directives on the line of &quot;Schedule {&quot; cannot be edited safely (line %1).</source>
        <translation>Директивы в строке «Schedule {» нельзя безопасно редактировать (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="107"/>
        <source>Text after the closing brace of the resource (line %1).</source>
        <translation>Текст после закрывающей скобки ресурса (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="111"/>
        <source>A nested block in a Schedule resource (line %1).</source>
        <translation>Вложенный блок в ресурсе Schedule (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="114"/>
        <source>The closing brace shares its line with a directive (line %1).</source>
        <translation>Закрывающая скобка находится в одной строке с директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="145"/>
        <source>A quoted Run value does not end on its line (line %1).</source>
        <translation>Значение Run в кавычках не заканчивается в своей строке (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="148"/>
        <source>Something follows the Run value on its line (line %1).</source>
        <translation>После значения Run в его строке что-то следует (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="156"/>
        <source>A Run directive shares its line with another directive (line %1).</source>
        <translation>Директива Run находится в одной строке с другой директивой (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="163"/>
        <source>A Run directive without a value (line %1).</source>
        <translation>Директива Run без значения (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="173"/>
        <source>A Run directive that does not start its line (line %1).</source>
        <translation>Директива Run не в начале строки (строка %1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="184"/>
        <source>The text contains no Schedule resource.</source>
        <translation>Текст не содержит ресурса Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="186"/>
        <source>The Schedule resource has no closing brace.</source>
        <translation>У ресурса Schedule нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="217"/>
        <source>The configuration file has %1 Run line(s), the schedule as it was loaded %2.</source>
        <translation>В файле конфигурации строк Run: %1, в загруженном Schedule: %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="227"/>
        <source>Run line %1 differs: the file says &quot;%2&quot;, the schedule as it was loaded &quot;%3&quot;.</source>
        <translation>Строка Run %1 отличается: в файле — «%2», в загруженном Schedule — «%3».</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="249"/>
        <source>A new Run line without text.</source>
        <translation>Новая строка Run без текста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="253"/>
        <source>A Run line refers to line %1, which the file does not have.</source>
        <translation>Строка Run ссылается на строку %1, которой нет в файле.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="255"/>
        <source>The Run lines would change their order; that is not written.</source>
        <translation>Строки Run изменили бы свой порядок; это не записывается.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="258"/>
        <source>A changed Run line without text.</source>
        <translation>Изменённая строка Run без текста.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="262"/>
        <source>A Run line contains a line break.</source>
        <translation>Строка Run содержит перевод строки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="364"/>
        <source>The new text could not be read back: %1</source>
        <translation>Новый текст не удалось прочитать обратно: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="369"/>
        <source>The new text does not have the intended Run lines.</source>
        <translation>В новом тексте нет нужных строк Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="371"/>
        <source>The new text would change more than the Run lines.</source>
        <translation>Новый текст изменил бы больше, чем строки Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bscheduleresource.cpp" line="373"/>
        <source>The new text would change the name of the schedule.</source>
        <translation>Новый текст изменил бы имя Schedule.</translation>
    </message>
</context>
<context>
    <name>BScheduleResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="408"/>
        <source>Source File</source>
        <translation>Исходный файл</translation>
    </message>
</context>
<context>
    <name>BScheduleReviewDialog</name>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="54"/>
        <source>(no Run line)</source>
        <translation>(нет строки Run)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="91"/>
        <source>changed: </source>
        <translation>изменено: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="92"/>
        <source>new: </source>
        <translation>новое: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="93"/>
        <source>removed: </source>
        <translation>удалено: </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="108"/>
        <source>Change schedule %1</source>
        <translation>Изменить Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="109"/>
        <source>Change schedule %1 on the Director?</source>
        <translation>Изменить Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="110"/>
        <source>The Director checked the new configuration and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новую конфигурацию и принимает её. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="114"/>
        <source>Create schedule %1</source>
        <translation>Создать Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="115"/>
        <source>Create schedule %1 on the Director?</source>
        <translation>Создать Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="116"/>
        <source>The Director checked the new schedule and accepts it. Nothing is written until you choose &quot;Write to the Director&quot;.</source>
        <translation>Director проверил новый Schedule и принимает его. Ничего не будет записано, пока вы не выберете «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="120"/>
        <source>Delete schedule %1</source>
        <translation>Удалить Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="121"/>
        <source>Delete schedule %1 on the Director?</source>
        <translation>Удалить Schedule %1 в Director?</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="122"/>
        <source>The Director checked that nothing refers to the schedule. Nothing is deleted until you choose &quot;Delete on the Director&quot;.</source>
        <translation>Director проверил, что на Schedule ничто не ссылается. Ничего не будет удалено, пока вы не выберете «Удалить в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="140"/>
        <source>Run lines now (in the file)</source>
        <translation>Строки Run сейчас (в файле)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="143"/>
        <source>Run lines now</source>
        <translation>Строки Run сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="148"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="151"/>
        <source>Run lines after writing</source>
        <translation>Строки Run после записи</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="159"/>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="168"/>
        <source>Jobs that use this schedule</source>
        <translation>Jobs, использующие этот Schedule</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="162"/>
        <source>The change applies to every one of these %n job(s), not only to the one on display. To move a single job, copy the schedule and assign the copy to that job instead.</source>
        <translation>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
            <numerusform>Изменение применяется ко всем этим Jobs (%n), а не только к отображаемому. Чтобы перенести отдельный Job, скопируйте Schedule и назначьте копию этому Job.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="173"/>
        <source>(no job of the Director uses it, as far as this application knows)</source>
        <translation>(ни один Job Director его не использует, насколько известно этому приложению)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="177"/>
        <source>The Director names as referencing it: %1</source>
        <translation>Director указывает как ссылающиеся на него: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="189"/>
        <source>Text after writing, editable</source>
        <translation>Текст после записи, редактируемый</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="190"/>
        <source>This text can be edited. An edited text is checked by the Director and shown again before anything is written.</source>
        <translation>Этот текст можно редактировать. Отредактированный текст проверяется Director и показывается снова до какой-либо записи.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="193"/>
        <source>Text after writing</source>
        <translation>Текст после записи</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="196"/>
        <source>Text now</source>
        <translation>Текст сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="200"/>
        <source>The text was edited. &quot;Check with the Director&quot; sends it for a check only; the review then shows it again, and only &quot;Write to the Director&quot; there writes it.</source>
        <translation>Текст был отредактирован. «Проверить в Director» отправляет его только на проверку; затем проверка показывает его снова, и записывает его только «Записать в Director».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="210"/>
        <source>File on the Director: %1 (it holds other resources as well; only this resource is replaced).</source>
        <translation>Файл в Director: %1 (в нём есть и другие ресурсы; заменяется только этот ресурс).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="212"/>
        <source>File on the Director: %1.</source>
        <translation>Файл в Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="215"/>
        <source>The Director reloads its configuration after writing. Runs it has already queued may still start at the old time.</source>
        <translation>После записи Director перезагружает свою конфигурацию. Запуски, уже поставленные им в очередь, ещё могут начаться в прежнее время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="221"/>
        <source>&amp;Delete on the Director</source>
        <translation>&amp;Удалить в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="222"/>
        <source>&amp;Write to the Director</source>
        <translation>&amp;Записать в Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulereviewdialog.cpp" line="253"/>
        <source>&amp;Check with the Director</source>
        <translation>&amp;Проверить в Director</translation>
    </message>
</context>
<context>
    <name>BScheduleRunDialog</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="48"/>
        <source>Edit Run Directive</source>
        <translation>Изменить директиву Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="56"/>
        <source>Level:</source>
        <translation>Level:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="61"/>
        <source>Time:</source>
        <translation>Время:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="65"/>
        <source>(every week)</source>
        <translation>(каждую неделю)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="66"/>
        <source>Week of Month:</source>
        <translation>Неделя месяца:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="71"/>
        <source>Days of Week</source>
        <translation>Дни недели</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="77"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="78"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="89"/>
        <source>Daily</source>
        <translation>Ежедневно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="93"/>
        <source>Mon-Fri</source>
        <translation>Пн–Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="98"/>
        <source>Clear</source>
        <translation>Очистить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="111"/>
        <source>Optional Overrides</source>
        <translation>Необязательные переопределения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="117"/>
        <source>Pool:</source>
        <translation>Pool:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="122"/>
        <source>Storage:</source>
        <translation>Storage:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="127"/>
        <source>(default)</source>
        <translation>(по умолчанию)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="128"/>
        <source>Priority:</source>
        <translation>Приоритет:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="138"/>
        <source>Preview:</source>
        <translation>Предпросмотр:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="159"/>
        <source>Validation</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="160"/>
        <source>Please select at least one day of the week.</source>
        <translation>Выберите хотя бы один день недели.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="179"/>
        <source>(level of the job)</source>
        <translation>(Level Job)</translation>
    </message>
</context>
<context>
    <name>BScheduleRunEditor</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Days</source>
        <translation>Дни</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Time</source>
        <translation>Время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="308"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="323"/>
        <source>+ Add Run</source>
        <translation>+ Добавить Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="324"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="325"/>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="326"/>
        <source>Duplicate</source>
        <translation>Дублировать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="381"/>
        <location filename="../src/schedules/bschedulewizard.cpp" line="396"/>
        <source>Edit Run Directive</source>
        <translation>Изменить директиву Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="382"/>
        <source>This Run line is not understood and cannot be edited:
%1

%2

Remove it to continue.</source>
        <translation>Эта строка Run не распознана и не может быть изменена:
%1

%2

Удалите её, чтобы продолжить.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="397"/>
        <source>This Run line uses forms that this dialog cannot show, for example months, days of the month, several times or other overrides:
%1

It is kept exactly as it is. To replace it, remove it and add a new run.</source>
        <translation>В этой строке Run используются формы, которые этот диалог не может показать, например месяцы, дни месяца, несколько значений времени или другие переопределения:
%1

Она сохраняется в точности как есть. Чтобы заменить её, удалите её и добавьте новый запуск.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="453"/>
        <source>Not understood: %1</source>
        <translation>Не распознано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="454"/>
        <source>Not understood</source>
        <translation>Не распознано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="469"/>
        <source>Kept exactly as written: %1</source>
        <translation>Сохранено в точности как написано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="479"/>
        <source>(level of the job)</source>
        <translation>(Level Job)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="484"/>
        <source>daily</source>
        <translation>ежедневно</translation>
    </message>
</context>
<context>
    <name>BScheduleRunsPage</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="636"/>
        <source>Run Directives</source>
        <translation>Директивы Run</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="637"/>
        <source>Define when this schedule runs backups. Add one or more Run entries.</source>
        <translation>Укажите, когда этот Schedule запускает резервное копирование. Добавьте одну или несколько записей Run.</translation>
    </message>
</context>
<context>
    <name>BScheduleScopeDialog</name>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="379"/>
        <source>&amp;last (the last seven days of a month)</source>
        <translation>&amp;last (последние семь дней месяца)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="383"/>
        <source>Add scope</source>
        <translation>Добавить область</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="386"/>
        <source>A scope is the part of the calendar a grid is painted for: weeks of the month, and months if not all. What is painted in it becomes Run lines such as &quot;1st sat at 21:00&quot;.</source>
        <translation>Область — это часть календаря, для которой рисуется сетка: недели месяца и, если не все, месяцы. Нарисованное в ней становится строками Run, например «1st sat at 21:00».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="392"/>
        <source>Weeks of the month</source>
        <translation>Недели месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="398"/>
        <source>Days %1 to %2 of a month</source>
        <translation>Дни с %1 по %2 месяца</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="408"/>
        <source>Months</source>
        <translation>Месяцы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="417"/>
        <source>&amp;All months</source>
        <translation>&amp;Все месяцы</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="474"/>
        <source>Choose at least one month.</source>
        <translation>Выберите хотя бы один месяц.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="476"/>
        <source>Choose at least one week of the month, or &quot;last&quot;.</source>
        <translation>Выберите хотя бы одну неделю месяца или «last».</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="482"/>
        <source>The scope &quot;%1&quot; exists already; it will be shown.</source>
        <translation>Область «%1» уже существует; она будет показана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulegriddialogs.cpp" line="483"/>
        <source>New scope: %1</source>
        <translation>Новая область: %1</translation>
    </message>
</context>
<context>
    <name>BScheduleWaitModel</name>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1068"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1092"/>
        <source>%1 s</source>
        <translation>%1 с</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1070"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1091"/>
        <source>%1 min</source>
        <translation>%1 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1075"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1090"/>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1113"/>
        <source>%1 h</source>
        <translation>%1 ч</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1089"/>
        <source>%1 d</source>
        <translation>%1 д</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1100"/>
        <source>Max Run Time %1</source>
        <translation>Max Run Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1102"/>
        <source>Full Max Runtime %1</source>
        <translation>Full Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1104"/>
        <source>Incremental Max Runtime %1</source>
        <translation>Incremental Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1106"/>
        <source>Differential Max Runtime %1</source>
        <translation>Differential Max Runtime %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1108"/>
        <source>Max Run Sched Time %1</source>
        <translation>Max Run Sched Time %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1113"/>
        <source>no Max Run Time set, %1 assumed</source>
        <translation>Max Run Time не задано, принято %1</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1119"/>
        <source>%n run(s) wait, longest %1</source>
        <translation>
            <numerusform>%n запуск ожидает, самое долгое ожидание: %1</numerusform>
            <numerusform>%n запуска ожидают, самое долгое ожидание: %1</numerusform>
            <numerusform>%n запусков ожидают, самое долгое ожидание: %1</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1120"/>
        <source>No waiting predicted</source>
        <translation>Ожидание не прогнозируется</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1122"/>
        <source>%1 uncertain</source>
        <translation>%1 неточно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1131"/>
        <source>Director limit: the Director runs %1 job(s) at a time (Maximum Concurrent Jobs = %1) and all of them are taken.</source>
        <translation>Ограничение Director: Director выполняет одновременно Jobs: %1 (Maximum Concurrent Jobs = %1), и все слоты заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1133"/>
        <source>Director limit: Maximum Concurrent Jobs of the Director is not known. With the Bareos default of 1 every job slot would be taken.</source>
        <translation>Ограничение Director: Maximum Concurrent Jobs для Director неизвестно. При значении Bareos по умолчанию 1 все слоты для Jobs были бы заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1137"/>
        <source>Storage limit: storage %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Ограничение Storage: Storage %1 принимает одновременно Jobs: %2 (Maximum Concurrent Jobs = %2), и все места заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1141"/>
        <source>Storage limit: the storage of this job is not known, and so is its Maximum Concurrent Jobs. With one shared storage and the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Storage: Storage этого Job неизвестен, как и его Maximum Concurrent Jobs. При одном общем Storage и значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1144"/>
        <source>Storage limit: Maximum Concurrent Jobs of storage %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Storage: Maximum Concurrent Jobs для Storage %1 неизвестно. При значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1148"/>
        <source>Client limit: client %1 takes %2 job(s) at a time (Maximum Concurrent Jobs = %2) and all places are taken.</source>
        <translation>Ограничение Client: Client %1 принимает одновременно Jobs: %2 (Maximum Concurrent Jobs = %2), и все места заняты.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1151"/>
        <source>Client limit: Maximum Concurrent Jobs of client %1 is not known. With the Bareos default of 1 it would be full.</source>
        <translation>Ограничение Client: Maximum Concurrent Jobs для Client %1 неизвестно. При значении Bareos по умолчанию 1 он был бы заполнен.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1155"/>
        <source>Job limit: job %1 may run %2 time(s) at once (Maximum Concurrent Jobs = %2) and an earlier start of it has not finished.</source>
        <translation>Ограничение Job: Job %1 может выполняться одновременно %2 раз(а) (Maximum Concurrent Jobs = %2), а его более ранний запуск ещё не завершён.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1158"/>
        <source>Job limit: Maximum Concurrent Jobs of job %1 is not known. With the Bareos default of 1 an earlier start of it would be in the way.</source>
        <translation>Ограничение Job: Maximum Concurrent Jobs для Job %1 неизвестно. При значении Bareos по умолчанию 1 помешал бы его более ранний запуск.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1162"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. The Director starts a run of a lower priority (a higher number) only when no run of another priority is running.</source>
        <translation>Приоритет: у этого запуска приоритет %1, а выполняются запуски с приоритетом %2. Director запускает задание с более низким приоритетом (большим числом), только когда не выполняется ни один запуск с другим приоритетом.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1167"/>
        <source>Priority: this run has priority %1 and runs of priority %2 are running. It starts beside them only if it and all of them have Allow Mixed Priority.</source>
        <translation>Приоритет: у этого запуска приоритет %1, а выполняются запуски с приоритетом %2. Он запустится параллельно с ними, только если у него и у всех них задано Allow Mixed Priority.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1171"/>
        <source>Queue order: a run of priority %1 is waiting ahead in the queue and cannot start yet. The Director starts no run that is behind it in the queue.</source>
        <translation>Порядок очереди: запуск с приоритетом %1 ожидает впереди в очереди и пока не может начаться. Director не запускает ничего, что стоит за ним в очереди.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1195"/>
        <source>from %1, duration unknown</source>
        <translation>с %1, длительность неизвестна</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1205"/>
        <source>%1 to about %2, estimated</source>
        <translation>с %1 примерно до %2, оценка</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1207"/>
        <source>%1 to %2</source>
        <translation>%1–%2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1216"/>
        <source>and %1 more</source>
        <translation>и ещё %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1229"/>
        <source>%1: it may run until %2 (%3)</source>
        <translation>%1: может выполняться до %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1230"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1235"/>
        <source>Maximum Concurrent Jobs of the Director is not known</source>
        <translation>Maximum Concurrent Jobs для Director неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1238"/>
        <source>the storage of the job is not known (the Job and its JobDefs name none)</source>
        <translation>Storage этого Job неизвестен (ни Job, ни его JobDefs его не указывают)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1239"/>
        <source>Maximum Concurrent Jobs of storage %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Storage %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1241"/>
        <source>Maximum Concurrent Jobs of client %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Client %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1243"/>
        <source>Maximum Concurrent Jobs of job %1 is not known</source>
        <translation>Maximum Concurrent Jobs для Job %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1245"/>
        <source>Allow Mixed Priority of job %1 is not known</source>
        <translation>Allow Mixed Priority для Job %1 неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1247"/>
        <source>the duration of %1 was not measured at this level</source>
        <translation>длительность %1 на этом уровне не измерялась</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1249"/>
        <source>the duration of %1 was never measured</source>
        <translation>длительность %1 никогда не измерялась</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1252"/>
        <source>the number of parallel writers of the storage device is not known (the job names no storage; the Director cannot read it)</source>
        <translation>число параллельных записей устройства Storage неизвестно (Job не указывает Storage; Director не может его прочитать)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1254"/>
        <source>the number of parallel writers of the device of storage %1 is not known (the Director cannot read it; set it in the list of predicted waits)</source>
        <translation>число параллельных записей устройства Storage %1 неизвестно (Director не может его прочитать; задайте его в списке прогнозируемых ожиданий)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1318"/>
        <source>Director limit (Maximum Concurrent Jobs = %1)</source>
        <translation>Ограничение Director (Maximum Concurrent Jobs = %1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1321"/>
        <source>Storage limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Storage (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1325"/>
        <source>Client limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Client (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1329"/>
        <source>Job limit (%1: Maximum Concurrent Jobs = %2)</source>
        <translation>Ограничение Job (%1: Maximum Concurrent Jobs = %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1333"/>
        <source>Priority %1, runs of priority %2 are running</source>
        <translation>Приоритет %1, выполняются запуски с приоритетом %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1337"/>
        <source>Queue order: a run of priority %1 waits ahead</source>
        <translation>Порядок очереди: впереди ожидает запуск с приоритетом %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1344"/>
        <source>depends on values that are not known</source>
        <translation>зависит от неизвестных значений</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1347"/>
        <source>May wait: %1</source>
        <translation>Может ожидать: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1349"/>
        <source>may be longer: %1</source>
        <translation>может быть дольше: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1371"/>
        <source>%1 to %2: %3</source>
        <translation>%1–%2: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1377"/>
        <source>Waits for: %1</source>
        <translation>Ожидает: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1382"/>
        <source>Predicted start: %1, %2 after the scheduled time (%3)</source>
        <translation>Прогнозируемое начало: %1, через %2 после запланированного времени (%3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1391"/>
        <source>it depends on values that are not known</source>
        <translation>это зависит от неизвестных значений</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1394"/>
        <source>Uncertain, the wait may be longer: %1.</source>
        <translation>Неточно, ожидание может быть дольше: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1396"/>
        <source>Uncertain, this run may have to wait: %1.</source>
        <translation>Неточно, этому запуску, возможно, придётся ждать: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1400"/>
        <source>If so: %1</source>
        <translation>Если так: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1423"/>
        <source>Inside the run: waits %1 in the storage daemon for a device of storage %2 (%3 parallel writer(s); after two waits for a release it tries again every %4 s)%5.</source>
        <translation>Внутри запуска: ожидает %1 в демоне Storage устройство Storage %2 (параллельных записей: %3; после двух ожиданий освобождения повторяет попытку каждые %4 с)%5.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1430"/>
        <source>, held by %1</source>
        <translation>, занято %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1433"/>
        <source>Predicted end: %1</source>
        <translation>Прогнозируемое окончание: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1436"/>
        <source>Start overhead %1 (from the Director starting the job to its recorded start: connections, Client Run Before Job)%2.</source>
        <translation>Накладные расходы на запуск %1 (от запуска Job в Director до зафиксированного начала: подключения, Client Run Before Job)%2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1445"/>
        <source>Measured wait at this level: %1; predicted %2.</source>
        <translation>Измеренное ожидание на этом уровне: %1; прогноз: %2.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1472"/>
        <source>not measured</source>
        <translation>не измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1473"/>
        <source>median %1, max %2 (%3 run(s))</source>
        <translation>медиана %1, максимум %2 (запусков: %3)</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1484"/>
        <source>Prediction too low: measured median %1, predicted %2 - a limit the GUI does not know, e.g. the storage device (parallel writers), or a Client Run Before Job that takes long.</source>
        <translation>Прогноз занижен: измеренная медиана %1, прогноз %2 — ограничение, неизвестное GUI, например устройство Storage (параллельные записи) или долгий Client Run Before Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bschedulewaitmodel.cpp" line="1489"/>
        <source>Prediction too high: measured median %1, predicted %2 - a limit is set lower here than it is, or the durations used are too long.</source>
        <translation>Прогноз завышен: измеренная медиана %1, прогноз %2 — здесь ограничение задано ниже, чем на самом деле, или используемые длительности слишком велики.</translation>
    </message>
</context>
<context>
    <name>BScheduleWidget</name>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="125"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="860"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3301"/>
        <source>Jobs:</source>
        <translation>Jobs:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="134"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="208"/>
        <source>Timeline</source>
        <translation>Временная шкала</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="209"/>
        <source>Grid</source>
        <translation>Сетка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="210"/>
        <source>Measured waits</source>
        <translation>Измеренные ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="211"/>
        <source>View:</source>
        <translation>Вид:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="217"/>
        <source>Day</source>
        <translation>День</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="218"/>
        <source>Week</source>
        <translation>Неделя</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="227"/>
        <source>Today</source>
        <translation>Сегодня</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="229"/>
        <source>Show today or the current week</source>
        <translation>Показать сегодня или текущую неделю</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="239"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1042"/>
        <source>The date on display; the calendar opens from the arrow</source>
        <translation>Отображаемая дата; календарь открывается по стрелке</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="252"/>
        <source>Snap:</source>
        <translation>Привязка:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="259"/>
        <source>Zoom:</source>
        <translation>Масштаб:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="264"/>
        <source>Zoom level (pixels per hour)</source>
        <translation>Масштаб (пикселей в час)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="302"/>
        <source>Legend</source>
        <translation>Легенда</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="304"/>
        <source>What the bars, hourglasses, question marks and diamonds of the timeline mean</source>
        <translation>Что означают полосы, песочные часы, вопросительные знаки и ромбы на временной шкале</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="325"/>
        <source>Schedules:</source>
        <translation>Schedules:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023"/>
        <source>Job</source>
        <translation>Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1102"/>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1103"/>
        <source>Client</source>
        <translation>Client</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1105"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1104"/>
        <source>FileSet</source>
        <translation>FileSet</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="344"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1143"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1858"/>
        <source>Median duration</source>
        <translation>Медианная длительность</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="373"/>
        <source>Clients (FD)</source>
        <translation>Clients (FD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="374"/>
        <source>Storage (SD)</source>
        <translation>Storage (SD)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="668"/>
        <source>%1 schedules loaded</source>
        <translation>Загружено Schedules: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="758"/>
        <source>%1 schedules loaded (%2 Run lines)</source>
        <translation>Загружено Schedules: %1 (строк Run: %2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="791"/>
        <source>Refreshing schedule list...</source>
        <translation>Обновление списка Schedules...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="865"/>
        <source>Jobs (%1):</source>
        <translation>Jobs (%1):</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="867"/>
        <source>Jobs (%1 Schedules):</source>
        <translation>Jobs (Schedules: %1):</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="888"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="892"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="904"/>
        <source>-</source>
        <translation>-</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="906"/>
        <source>Median of %1 measured runs at all levels in the last %2 days</source>
        <translation>Медиана %1 измеренных запусков на всех уровнях за последние %2 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="908"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1371"/>
        <source>No history</source>
        <translation>Нет истории</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1041"/>
        <source>The week on display: pick any day of it; the calendar opens from the arrow</source>
        <translation>Отображаемая неделя: выберите любой её день; календарь открывается стрелкой</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051"/>
        <source>Mon</source>
        <translation>Пн</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051"/>
        <source>Tue</source>
        <translation>Вт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051"/>
        <source>Wed</source>
        <translation>Ср</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1051"/>
        <source>Thu</source>
        <translation>Чт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052"/>
        <source>Fri</source>
        <translation>Пт</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052"/>
        <source>Sat</source>
        <translation>Сб</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1052"/>
        <source>Sun</source>
        <translation>Вс</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="440"/>
        <source>Measured history</source>
        <translation>Измеренная история</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="451"/>
        <source>Close the area below the timeline</source>
        <translation>Закрыть область под временной шкалой</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1074"/>
        <source>Click a run in the timeline: its measured history appears here.</source>
        <translation>Щёлкните по запуску на временной шкале: здесь появится его измеренная история.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1106"/>
        <source>Priority</source>
        <translation>Приоритет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1144"/>
        <source>25% - 75%</source>
        <translation>25% - 75%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1145"/>
        <source>90%</source>
        <translation>90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1152"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857"/>
        <source>Runs</source>
        <translation>Запуски</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1158"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859"/>
        <source>Median wait</source>
        <translation>Медианное ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1159"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859"/>
        <source>90% wait</source>
        <translation>Ожидание 90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166"/>
        <source>Start</source>
        <translation>Начало</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1857"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292"/>
        <source>Level</source>
        <translation>Level</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166"/>
        <source>Duration</source>
        <translation>Длительность</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1166"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293"/>
        <source>Wait</source>
        <translation>Ожидание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1311"/>
        <source>no job uses this schedule</source>
        <translation>этот Schedule не используется ни одним Job</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1315"/>
        <source>no successful run of this job in the last %1 days</source>
        <translation>нет успешных запусков этого Job за последние %1 дн.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1318"/>
        <source>No history: %1.</source>
        <translation>Нет истории: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1330"/>
        <source>Estimate: no run at this level in the last %1 days. The figures are from all levels of the job.</source>
        <translation>Оценка: нет запусков с этим Level за последние %1 дн. Данные по всем Level этого Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1346"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1347"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1366"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2195"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2902"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1400"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1406"/>
        <source>1 hour</source>
        <translation>1 час</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1401"/>
        <source>2 hours</source>
        <translation>2 часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1402"/>
        <source>4 hours</source>
        <translation>4 часа</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1404"/>
        <source>15 min</source>
        <translation>15 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1405"/>
        <source>30 min</source>
        <translation>30 мин</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1473"/>
        <source>Run line %1 &quot;%2&quot;: %3</source>
        <translation>Строка Run %1 «%2»: %3</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1488"/>
        <source>Not understood and not shown in the views:</source>
        <translation>Не распознано и не показано в представлениях:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1490"/>
        <source>This schedule cannot be written by the application.</source>
        <translation>Приложение не может записать этот Schedule.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="1496"/>
        <source>%n Run line(s) not understood</source>
        <translation>
            <numerusform>%n строка Run не распознана</numerusform>
            <numerusform>%n строки Run не распознаны</numerusform>
            <numerusform>%n строк Run не распознано</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1498"/>
        <source>These Run lines are not shown in the views, and their schedules cannot be written by the application:</source>
        <translation>Эти строки Run не показаны в представлениях, и приложение не может записать их Schedules:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1608"/>
        <source>Schedule %1 was not changed: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director (onesimus-dird), which are not available on this connection. %2 stays at %3 (dropped at %4).</source>
        <translation>Schedule %1 не изменён: для изменения Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director (onesimus-dird), которые недоступны в этом подключении. %2 остаётся на %3 (перетащено на %4).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1635"/>
        <source>Schedule %1 was not changed: another change is being written.</source>
        <translation>Schedule %1 не изменён: записывается другое изменение.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1649"/>
        <source>Run line %1 is not understood, so the schedule is locked.</source>
        <translation>Строка Run %1 не распознана, поэтому Schedule заблокирован.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1652"/>
        <source>the dragged Run line runs at several hours of the day; change it in the grid.</source>
        <translation>перетащенная строка Run выполняется в несколько часов суток; измените её в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1656"/>
        <source>the dragged run could not be matched to exactly one Run line; refresh and try again, or change it in the grid.</source>
        <translation>перетащенный запуск не удалось сопоставить ровно одной строке Run; обновите и повторите попытку или измените его в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1660"/>
        <source>its Run lines are not loaded yet.</source>
        <translation>его строки Run ещё не загружены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1663"/>
        <source>%1 is not a time of day.</source>
        <translation>%1 не является временем суток.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1667"/>
        <source>the new Run line did not read back as intended (%1).</source>
        <translation>новая строка Run прочиталась обратно не так, как предполагалось (%1).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1670"/>
        <source>Schedule %1 was not changed: %2</source>
        <translation>Schedule %1 не изменён: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1675"/>
        <source>Run line %1 of schedule %2 moves from %3 to %4 (dragged in the timeline: %5). Only this line is written anew; the other lines stay as they are in the file.</source>
        <translation>Строка Run %1 Schedule %2 переносится с %3 на %4 (перетащено на временной шкале: %5). Заново записывается только эта строка; остальные строки остаются в файле без изменений.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1680"/>
        <source>The Director checks the change of schedule %1...</source>
        <translation>Director проверяет изменение Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1726"/>
        <source>Schedule %1 is locked: it contains a Run line that is not understood.</source>
        <translation>Schedule %1 заблокирован: он содержит нераспознанную строку Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1732"/>
        <source>Schedule %1 was not written: changing a schedule needs the configuration methods of the gRPC interface of the Onesimus Director. Your changes are kept in the grid.</source>
        <translation>Schedule %1 не записан: для изменения Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director. Ваши изменения сохранены в сетке.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1738"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1768"/>
        <source>Another change is being written.</source>
        <translation>Записывается другое изменение.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1744"/>
        <source>Schedule %1 cannot be written: %2</source>
        <translation>Schedule %1 нельзя записать: %2</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1747"/>
        <source>The Director checks the changes of schedule %1...</source>
        <translation>Director проверяет изменения Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1750"/>
        <source>The changes made in the grid. Lines that were not touched stay as they are in the file.</source>
        <translation>Изменения, внесённые в сетке. Незатронутые строки остаются в файле без изменений.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1762"/>
        <source>Schedule %1 was not deleted: deleting a schedule needs the configuration methods of the gRPC interface of the Onesimus Director.</source>
        <translation>Schedule %1 не удалён: для удаления Schedule нужны методы конфигурации интерфейса gRPC Onesimus Director.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1771"/>
        <source>The Director checks whether schedule %1 can be deleted...</source>
        <translation>Director проверяет, можно ли удалить Schedule %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1786"/>
        <source>The Director reloaded its configuration.</source>
        <translation>Director перезагрузил свою конфигурацию.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1788"/>
        <source>Follow-up commands: %1</source>
        <translation>Дополнительные команды: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1847"/>
        <source>Refresh history</source>
        <translation>Обновить историю</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1850"/>
        <source>Ask the Director again for the jobs of the last days (llist jobs)</source>
        <translation>Повторно запросить у Director Jobs за последние дни (llist jobs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1858"/>
        <source>90% duration</source>
        <translation>Длительность 90%</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1859"/>
        <source>Last run</source>
        <translation>Последний запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1861"/>
        <source>Runs in the window that terminated normally or with warnings</source>
        <translation>Запуски в окне, завершившиеся нормально или с предупреждениями</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1863"/>
        <source>Wait: start of the job minus its scheduled time</source>
        <translation>Ожидание: начало Job минус запланированное время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1865"/>
        <source>Nine of ten runs waited no longer than this</source>
        <translation>Девять из десяти запусков ждали не дольше этого</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="1981"/>
        <source>the request got no reply (%1)</source>
        <translation>запрос остался без ответа (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2011"/>
        <source>the Director refused the request (%1)</source>
        <translation>Director отклонил запрос (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2012"/>
        <source>no reason given</source>
        <translation>причина не указана</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2015"/>
        <source>the Director&apos;s reply could not be read (%1)</source>
        <translation>не удалось прочитать ответ Director (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2039"/>
        <source>Schedules: no job history, %1</source>
        <translation>Schedules: нет истории Jobs, %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2073"/>
        <source>the job history is being loaded</source>
        <translation>история Jobs загружается</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2077"/>
        <source>the job history has not been requested yet</source>
        <translation>история Jobs ещё не запрошена</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2108"/>
        <source>Not connected.</source>
        <translation>Нет подключения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2110"/>
        <source>The job history has not been requested yet.</source>
        <translation>История Jobs ещё не запрошена.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2112"/>
        <source>No job history: %1.</source>
        <translation>Нет истории Jobs: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2113"/>
        <source>The schedule views work without it: runs have no duration and waits are unknown.</source>
        <translation>Представления Schedules работают и без неё: у запусков нет длительности, ожидания неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2117"/>
        <source>Requesting the jobs of the last %1 days from the Director...</source>
        <translation>Запрос у Director Jobs за последние %1 дн....</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2123"/>
        <source>Jobs scheduled in the last %1 days, fetched %2: %3 rows, of which %4 runs terminated normally or with warnings and were measured.</source>
        <translation>Jobs, запланированные за последние %1 дн., получено %2: строк: %3, из них %4 запусков завершились нормально или с предупреждениями и были измерены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2132"/>
        <source>The row limit (%1) was reached and this Director returned the oldest rows: the newest runs of the window are missing.</source>
        <translation>Достигнут лимит строк (%1), и этот Director вернул самые старые строки: самые новые запуски окна отсутствуют.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2134"/>
        <source>The row limit (%1) was reached: the oldest runs of the window are missing.</source>
        <translation>Достигнут лимит строк (%1): самые старые запуски окна отсутствуют.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2138"/>
        <source>This Director did not deliver the scheduled time: all waits are unknown.</source>
        <translation>Этот Director не передал запланированное время: все ожидания неизвестны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2140"/>
        <source>%1 runs came without a scheduled time: their wait is unknown.</source>
        <translation>Запусков без запланированного времени: %1; их ожидание неизвестно.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2144"/>
        <source>%1 rows were left out because they start before their scheduled time: copied, migrated and virtual full jobs carry the times of another job.</source>
        <translation>Пропущено строк: %1, так как они начинаются раньше запланированного времени: Jobs Copy, Migrate и VirtualFull несут время другого Job.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2149"/>
        <source>%1 rows were left out because their start or end time is missing or the end is before the start.</source>
        <translation>Пропущено строк: %1, так как у них отсутствует время начала или окончания либо окончание раньше начала.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2175"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2703"/>
        <source>(none)</source>
        <translation>(нет)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2184"/>
        <source>From %1 of %2 runs; the others came without a scheduled time.</source>
        <translation>По %1 из %2 запусков; остальные пришли без запланированного времени.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2193"/>
        <source>The Director delivered no scheduled time for these runs.</source>
        <translation>Director не передал запланированное время для этих запусков.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2997"/>
        <source>Jobs that are scheduled for the same moment and share a limit (a storage, a client, the job slots of the Director, the parallel writers of a device) wait for each other. The proposal starts some of them later, on a quarter hour (23:15, 23:30 ...), so that they wait less on the dates on display and have room to spare. A wait for a higher priority is meant to be and is not counted; a job of a lower priority that follows a moved job moves along, so that it still runs after it. Nothing is changed on the Director.</source>
        <translation>Job, запланированные на один и тот же момент и делящие общий лимит (Storage, Client, слоты Job у Director, параллельные записи устройства), ждут друг друга. Предложение запускает некоторые из них позже, в начале четверти часа (23:15, 23:30 ...), чтобы в показанные дни они ждали меньше и имели запас. Ожидание из-за более высокого приоритета предусмотрено и не учитывается; Job с более низким приоритетом, следующий за сдвинутым Job, сдвигается вместе с ним, чтобы по-прежнему выполняться после него. На Director ничего не изменяется.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3008"/>
        <source>Replays the queue with the jobs moved quarter hour by quarter hour and keeps the smallest moves that wait least</source>
        <translation>Проигрывает очередь со сдвигом Job по четвертям часа и оставляет наименьшие сдвиги с наименьшим ожиданием</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3162"/>
        <source>Stays: a Run line of its schedule runs at several hours, is not understood or would move into the next day</source>
        <translation>Остаётся: строка Run его Schedule выполняется в несколько часов, не распознана или перешла бы на следующий день</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3165"/>
        <source>Moves along to run after %1, which has a higher priority</source>
        <translation>Сдвигается, чтобы выполняться после %1 с более высоким приоритетом</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3007"/>
        <source>Propose later start times</source>
        <translation>Предложить более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3016"/>
        <source>Apply...</source>
        <translation>Применить...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023"/>
        <source>Schedule</source>
        <translation>Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023"/>
        <source>Start now</source>
        <translation>Запуск сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023"/>
        <source>Proposed start</source>
        <translation>Предлагаемый запуск</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3023"/>
        <source>Later by</source>
        <translation>Позже на</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024"/>
        <source>Wait now</source>
        <translation>Ожидание сейчас</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024"/>
        <source>Wait then</source>
        <translation>Ожидание потом</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3024"/>
        <source>Note</source>
        <translation>Примечание</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3026"/>
        <source>Time held by a limit or waiting for a device, summed over the job&apos;s runs on the dates on display</source>
        <translation>Время, которое Job удерживается лимитом или ждёт устройство, суммарно по его запускам в показанные дни</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3028"/>
        <source>Proposed later start times</source>
        <translation>Предлагаемое более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3048"/>
        <source>No proposal yet.</source>
        <translation>Предложения пока нет.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3063"/>
        <source>Creates a schedule of its own for every job that starts later and sets the job&apos;s Schedule to it; everything is shown once for confirmation first</source>
        <translation>Создаёт отдельный Schedule для каждого Job, который запускается позже, и переключает Schedule этого Job на него; сначала всё показывается один раз для подтверждения</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3065"/>
        <source>Needs the configuration methods of the gRPC interface of the Onesimus Director (create schedules, read and change jobs), which are not available on this connection</source>
        <translation>Требуются методы конфигурации gRPC-интерфейса Onesimus Director (создание Schedule, чтение и изменение Job), недоступные в этом подключении</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3101"/>
        <source>Nothing to stagger: the waits are not predicted (the jobs or the history are not loaded yet).</source>
        <translation>Нечего распределять: ожидания не предсказаны (Job или история ещё не загружены).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3118"/>
        <source>Nothing to stagger: no run on the dates on display waits for a limit or a device.</source>
        <translation>Нечего распределять: ни один запуск в показанные дни не ждёт лимит или устройство.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3121"/>
        <source>No later start helps: avoidable wait %1. See the notes of the jobs.</source>
        <translation>Более поздний запуск не помогает: устранимое ожидание %1. См. примечания к Job.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3124"/>
        <source>Avoidable wait on the dates on display: now %1, with the proposal %2. %n job(s) start later.</source>
        <translation>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускается позже.</numerusform>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускаются позже.</numerusform>
            <numerusform>Устранимое ожидание в показанные дни: сейчас %1, с предложением %2. %n Job запускаются позже.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3167"/>
        <source>A later start does not help</source>
        <translation>Более поздний запуск не помогает</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/bschedulewidget.cpp" line="3172"/>
        <source>%n min</source>
        <translation>
            <numerusform>%n мин</numerusform>
            <numerusform>%n мин</numerusform>
            <numerusform>%n мин</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3188"/>
        <source>Apply the later start times</source>
        <translation>Применить более позднее время запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3190"/>
        <source>These resources are written to the Director, in this order. The Director checks each one; the first that it refuses stops the rest.</source>
        <translation>Эти ресурсы записываются в Director в указанном порядке. Director проверяет каждый; первый отклонённый останавливает остальные.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3202"/>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3248"/>
        <source>Nothing was written: %1</source>
        <translation>Ничего не записано: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3276"/>
        <source>Reading the jobs that start later...</source>
        <translation>Чтение Job, которые запускаются позже...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3297"/>
        <source>schedule %1</source>
        <translation>Schedule %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3298"/>
        <source>New schedule %1 for %2:</source>
        <translation>Новый Schedule %1 для %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3307"/>
        <source>Nothing was written: the text of job %1 has no closing brace.</source>
        <translation>Ничего не записано: в тексте Job %1 нет закрывающей скобки.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3314"/>
        <source>(Schedule from its JobDefs)</source>
        <translation>(Schedule из его JobDefs)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3320"/>
        <source>Nothing was written: the later start times were not confirmed.</source>
        <translation>Ничего не записано: более позднее время запуска не подтверждено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3332"/>
        <source>Written: %1. The jobs start at their new times from now on.</source>
        <translation>Записано: %1. Теперь Job запускаются в новое время.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3336"/>
        <source>Writing %1...</source>
        <translation>Запись %1...</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3342"/>
        <source>it was changed on the Director in the meantime</source>
        <translation>он был изменён в Director за это время</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3344"/>
        <source>the name is taken</source>
        <translation>имя уже занято</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3345"/>
        <source>nothing</source>
        <translation>ничего</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3347"/>
        <source>Writing %1 failed: %2. Written before: %3. Nothing after it was sent.</source>
        <translation>Не удалось записать %1: %2. Записано ранее: %3. Дальше ничего не отправлено.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3363"/>
        <source>Nothing was written: job %1 could not be read (%2).</source>
        <translation>Ничего не записано: не удалось прочитать Job %1 (%2).</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="430"/>
        <source>Predicted waits</source>
        <translation>Прогнозируемые ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="444"/>
        <source>Stagger</source>
        <translation>Распределить</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="450"/>
        <source>Close the tabs below the timeline</source>
        <translation>Закрыть вкладки под шкалой времени</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2288"/>
        <source>Reasons shared by several rows</source>
        <translation>Причины, общие для нескольких строк</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292"/>
        <source>Scheduled</source>
        <translation>Запланировано</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2292"/>
        <source>Predicted start</source>
        <translation>Прогнозируемое начало</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293"/>
        <source>Measured</source>
        <translation>Измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293"/>
        <source>Certain</source>
        <translation>Точно</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2293"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2295"/>
        <source>What this job waited at this level in the job history (StartTime minus SchedTime): median / maximum. The catalog&apos;s start is set after the connections and Client Run Before Job, so that time is part of it.</source>
        <translation>Сколько этот Job ждал на этом уровне по истории Jobs (StartTime минус SchedTime): медиана / максимум. Начало в каталоге фиксируется после подключений и Client Run Before Job, поэтому это время входит в него.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2299"/>
        <source>Predicted start minus scheduled time: seconds (&quot;27 s&quot;), minutes:seconds (&quot;1:35 min&quot;) or, from ten minutes on, hours:minutes (&quot;0:53 h&quot;)</source>
        <translation>Прогнозируемое начало минус запланированное время: секунды («27 s»), минуты:секунды («1:35 min») или, начиная с десяти минут, часы:минуты («0:53 h»)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2302"/>
        <source>No: the prediction depends on a limit the Director did not report or on a duration that was not measured</source>
        <translation>Нет: прогноз зависит от ограничения, о котором Director не сообщил, или от неизмеренной длительности</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2304"/>
        <source>Predicted waits of the dates on display</source>
        <translation>Прогнозируемые ожидания для отображаемых дат</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2306"/>
        <source>One row per run that is predicted to wait or whose prediction is uncertain. The reason of the current row is below the table.</source>
        <translation>По одной строке на каждый запуск, которому прогнозируется ожидание или прогноз для которого неточен. Причина для текущей строки показана под таблицей.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2328"/>
        <source>Reason of the selected wait</source>
        <translation>Причина выбранного ожидания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2351"/>
        <source>Parallel writers of the devices</source>
        <translation>Параллельные записи устройств</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2353"/>
        <source>Notes</source>
        <translation>Примечания</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2355"/>
        <source>Basis of the prediction</source>
        <translation>Основа прогноза</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2357"/>
        <source>Back-test</source>
        <translation>Ретроспективная проверка</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2367"/>
        <source>Back-test of the prediction</source>
        <translation>Ретроспективная проверка прогноза</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2383"/>
        <source>Director (%1)</source>
        <translation>Director (%1)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2385"/>
        <source>Storage %1 (%2)</source>
        <translation>Storage %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2387"/>
        <source>Client %1 (%2)</source>
        <translation>Client %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2389"/>
        <source>Job %1 (%2)</source>
        <translation>Job %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2391"/>
        <source>Priority %1</source>
        <translation>Приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2393"/>
        <source>Queue: priority %1 first</source>
        <translation>Очередь: сначала приоритет %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2401"/>
        <source>The prediction has changed: propose again.</source>
        <translation>Предсказание изменилось: предложите заново.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2572"/>
        <source>Waiting not predicted</source>
        <translation>Ожидание не прогнозируется</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2576"/>
        <source>The jobs of the Director are not loaded: it is not known which job runs which schedule.</source>
        <translation>Jobs Director не загружены: неизвестно, какой Job использует какой Schedule.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2579"/>
        <source>Predicted for the dates on display from the limits of the Director (Maximum Concurrent Jobs, priorities) and the measured durations.</source>
        <translation>Прогноз для отображаемых дат на основе ограничений Director (Maximum Concurrent Jobs, приоритеты) и измеренных длительностей.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2582"/>
        <source>Uncertain: the prediction depends on a limit the Director did not report or on a duration that was not measured.</source>
        <translation>Неточно: прогноз зависит от ограничения, о котором Director не сообщил, или от неизмеренной длительности.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2586"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2798"/>
        <source>Opens or closes the list of predicted waits below the timeline.</source>
        <translation>Открывает или закрывает список прогнозируемых ожиданий под временной шкалой.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2602"/>
        <source>Not predicted: the jobs of the Director are not loaded.</source>
        <translation>Нет прогноза: Jobs Director не загружены.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2604"/>
        <source>%1 run(s) on display, replayed with the %2 run(s) of all enabled schedules from the day before on.</source>
        <translation>Отображается запусков: %1, смоделировано вместе с %2 запусками всех включённых Schedules начиная с предыдущего дня.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2613"/>
        <source>the storage of job %1 (the job names none)</source>
        <translation>Storage Job %1 (Job его не указывает)</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2620"/>
        <source>storage %1</source>
        <translation>Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2627"/>
        <source>client %1</source>
        <translation>Client %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2632"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3311"/>
        <source>job %1</source>
        <translation>Job %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2639"/>
        <source>Director = %1</source>
        <translation>Director = %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2641"/>
        <source>the Director</source>
        <translation>Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2643"/>
        <source>storages: %1</source>
        <translation>Storages: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2645"/>
        <source>clients: %1</source>
        <translation>Clients: %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2647"/>
        <source>Maximum Concurrent Jobs as reported: %1.</source>
        <translation>Maximum Concurrent Jobs по данным Director: %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2649"/>
        <source>Maximum Concurrent Jobs not reported by this Director for: %1. Runs it matters for are uncertain.</source>
        <translation>Этот Director не сообщил Maximum Concurrent Jobs для: %1. Запуски, для которых это важно, неточны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2673"/>
        <source>[%1] %2 rows: %3.</source>
        <translation>[%1] строк %2: %3.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2717"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3129"/>
        <location filename="../src/schedules/bschedulewidget.cpp" line="3174"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2724"/>
        <source>not measured</source>
        <translation>не измерено</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2726"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2731"/>
        <source>Prediction too low</source>
        <translation>Прогноз занижен</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2732"/>
        <source>Prediction too low; %1</source>
        <translation>Прогноз занижен; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2735"/>
        <source>Prediction too high</source>
        <translation>Прогноз завышен</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2736"/>
        <source>Prediction too high; %1</source>
        <translation>Прогноз завышен; %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2740"/>
        <source>Waits %1 for the device inside the run</source>
        <translation>Ожидает устройство %1 внутри запуска</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2754"/>
        <source>No run on display is predicted to wait.</source>
        <translation>Ни одному отображаемому запуску не прогнозируется ожидание.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2755"/>
        <source>Not predicted.</source>
        <translation>Нет прогноза.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2795"/>
        <source>Predicted waits: %1. The list is open.</source>
        <translation>Прогнозируемые ожидания: %1. Список открыт.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2796"/>
        <source>Predicted waits: %1. The list is closed.</source>
        <translation>Прогнозируемые ожидания: %1. Список закрыт.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2808"/>
        <source>Select a row to read its reason in full.</source>
        <translation>Выберите строку, чтобы прочитать причину полностью.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2816"/>
        <source>%1, scheduled %2:</source>
        <translation>%1, запланировано %2:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2897"/>
        <source>Storage %1, parallel writers of its device:</source>
        <translation>Storage %1, параллельные записи его устройства:</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2904"/>
        <source>Parallel writers of the device of storage %1</source>
        <translation>Параллельные записи устройства Storage %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2905"/>
        <source>How many jobs the device(s) of %1 write at a time: Maximum Concurrent Jobs of the Device resource in the storage daemon (several devices: their sum). The Director cannot read it. &quot;unknown&quot;: runs that depend on it are uncertain.</source>
        <translation>Сколько Jobs одновременно записывает устройство (устройства) %1: Maximum Concurrent Jobs ресурса Device в демоне Storage (несколько устройств: их сумма). Director не может это прочитать. «неизвестно»: зависящие от этого запуски неточны.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2948"/>
        <source>No job history: no suggestion.</source>
        <translation>Нет истории Jobs: нет предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2950"/>
        <source>History suggests %1.</source>
        <translation>По истории предлагается %1.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2952"/>
        <source>History gives no suggestion.</source>
        <translation>История не даёт предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958"/>
        <source>Use %1</source>
        <translation>Использовать %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2958"/>
        <source>Use</source>
        <translation>Использовать</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewidget.cpp" line="2979"/>
        <source>Back-test: no job history to replay.</source>
        <translation>Ретроспективная проверка: нет истории Jobs для воспроизведения.</translation>
    </message>
</context>
<context>
    <name>BScheduleWizard</name>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="950"/>
        <source>Add Schedule</source>
        <translation>Добавить Schedule</translation>
    </message>
    <message>
        <location filename="../src/schedules/bschedulewizard.cpp" line="1016"/>
        <source># This schedule cannot be written:</source>
        <translation># Этот Schedule нельзя записать:</translation>
    </message>
</context>
<context>
    <name>BSettings</name>
    <message>
        <location filename="../src/config/bsettings.cpp" line="221"/>
        <source>Default</source>
        <translation>По умолчанию</translation>
    </message>
</context>
<context>
    <name>BSettingsDialog</name>
    <message>
        <location filename="../ui/bsettingsdialog.ui" line="14"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="64"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1388"/>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="103"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1401"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1409"/>
        <source>Reset</source>
        <translation>Сбросить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="105"/>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="107"/>
        <source>Apply</source>
        <translation>Применить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="141"/>
        <source>🔌 Connection</source>
        <translation>🔌 Подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="145"/>
        <source>🎨 Appearance</source>
        <translation>🎨 Внешний вид</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="149"/>
        <source>⚙️ Behavior</source>
        <translation>⚙️ Поведение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="153"/>
        <source>🔧 Advanced</source>
        <translation>🔧 Дополнительно</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="182"/>
        <source>Connection Manager</source>
        <translation>Диспетчер подключений</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="190"/>
        <source>Saved Connections</source>
        <translation>Сохранённые подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="204"/>
        <source>+ Add</source>
        <translation>+ Добавить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="205"/>
        <source>Add a new connection profile</source>
        <translation>Добавить новый профиль подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="206"/>
        <source>Edit</source>
        <translation>Изменить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="208"/>
        <source>Duplicate</source>
        <translation>Дублировать</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="210"/>
        <source>Delete</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="240"/>
        <source>Profile</source>
        <translation>Профиль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="246"/>
        <source>e.g. Production Server, Test Environment</source>
        <translation>например, Рабочий сервер, Тестовая среда</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="247"/>
        <source>Profile Name:</source>
        <translation>Имя профиля:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="252"/>
        <source>Director Connection</source>
        <translation>Подключение к Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="258"/>
        <source>e.g. 192.168.1.100 or bareos-dir.local</source>
        <translation>например, 192.168.1.100 или bareos-dir.local</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="259"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="264"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="268"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="269"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="275"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="277"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="282"/>
        <source>gRPC port of the Director (default: BNET port + 1000)</source>
        <translation>Порт gRPC Director (по умолчанию: порт BNET + 1000)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="283"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="290"/>
        <source>Forget certificate</source>
        <translation>Забыть сертификат</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="291"/>
        <source>Remove the pinned certificate. The next connection asks again whether the server certificate is trusted.</source>
        <translation>Удалить закреплённый сертификат. При следующем подключении снова будет задан вопрос, доверять ли сертификату сервера.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="294"/>
        <source>Server certificate:</source>
        <translation>Сертификат сервера:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="302"/>
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="303"/>
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="304"/>
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="305"/>
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="306"/>
        <source>Server operating system (affects TLS paths in exported configs)</source>
        <translation>Операционная система сервера (влияет на пути TLS в экспортируемых конфигурациях)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="307"/>
        <source>Server OS:</source>
        <translation>ОС сервера:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="311"/>
        <source>Director Name:</source>
        <translation>Имя Director:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="315"/>
        <source>Console name for authentication (e.g., &apos;admin&apos; or &apos;onesimus&apos;)</source>
        <translation>Имя консоли для аутентификации (например, &apos;admin&apos; или &apos;onesimus&apos;)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="316"/>
        <source>Console Name:</source>
        <translation>Имя консоли:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="321"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="328"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="334"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1669"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(здесь появится MD5-хеш пароля)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="329"/>
        <source>MD5 Hash:</source>
        <translation>MD5-хеш:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="369"/>
        <source>⚠️ TLS encrypts communication with the Director.
TLS is strongly recommended for production environments!</source>
        <translation>⚠️ TLS шифрует обмен данными с Director.
В рабочих средах настоятельно рекомендуется использовать TLS!</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="377"/>
        <source>Authentication &amp; Encryption</source>
        <translation>Аутентификация и шифрование</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="383"/>
        <source>Authentication Method:</source>
        <translation>Метод аутентификации:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="389"/>
        <source>Use x509 certificates (instead of PSK)</source>
        <translation>Использовать сертификаты x509 (вместо PSK)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="391"/>
        <source>By default, TLS uses Pre-Shared Key (PSK) authentication.
Enable this to use X.509 certificate-based authentication instead.
Certificate mode requires CA certificate and client certificate/key files.</source>
        <translation>По умолчанию TLS использует аутентификацию по общему ключу (PSK).
Включите этот параметр, чтобы использовать аутентификацию на основе сертификатов X.509.
Для режима сертификатов нужны файлы сертификата CA и сертификата/ключа клиента.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="409"/>
        <source>Path to CA certificate (.pem)</source>
        <translation>Путь к сертификату CA (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="410"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="420"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="430"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="441"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="415"/>
        <source>CA Certificate:</source>
        <translation>Сертификат CA:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="419"/>
        <source>Path to client certificate (.pem)</source>
        <translation>Путь к сертификату клиента (.pem)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="425"/>
        <source>Client Certificate:</source>
        <translation>Сертификат клиента:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="429"/>
        <source>Path to private key (.pem, .key)</source>
        <translation>Путь к закрытому ключу (.pem, .key)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="435"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="440"/>
        <source>Path to client certificate (.pfx)</source>
        <translation>Путь к сертификату клиента (.pfx)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="446"/>
        <source>PFX Certificate:</source>
        <translation>Сертификат PFX:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="449"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Преобразовать PEM/DER в PFX...</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="450"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Преобразовать отдельные файлы сертификата и ключа PEM/DER в файл PFX</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="455"/>
        <source>Verify server certificate (recommended)</source>
        <translation>Проверять сертификат сервера (рекомендуется)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="464"/>
        <source>TLS Cipher List (optional):</source>
        <translation>Список шифров TLS (необязательно):</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="470"/>
        <source>e.g., PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>например, PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="471"/>
        <source>Colon-separated list of TLS-PSK ciphers.
Leave empty for auto-detection.
Example: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</source>
        <translation>Список шифров TLS-PSK через двоеточие.
Оставьте пустым для автоопределения.
Пример: PSK-AES256-GCM-SHA384:PSK-AES128-GCM-SHA256</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="476"/>
        <source>Preset</source>
        <translation>Предустановка</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="477"/>
        <source>Fill with recommended PSK ciphers</source>
        <translation>Заполнить рекомендуемыми шифрами PSK</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="488"/>
        <source>gRPC is always encrypted with TLS. The server certificate is confirmed on first use and pinned; the Director authenticates with console name and password. TLS-PSK and certificate settings do not apply.</source>
        <translation>gRPC всегда шифруется с помощью TLS. Сертификат сервера подтверждается при первом использовании и закрепляется; Director выполняет аутентификацию по имени консоли и паролю. Настройки TLS-PSK и сертификатов не применяются.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="499"/>
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="504"/>
        <source>Save password in profile</source>
        <translation>Сохранять пароль в профиле</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="508"/>
        <source>Auto-connect on startup</source>
        <translation>Подключаться автоматически при запуске</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="512"/>
        <source>Connection timeout:</source>
        <translation>Тайм-аут подключения:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="516"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="770"/>
        <source> seconds</source>
        <translation> с</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="527"/>
        <source>Save Profile</source>
        <translation>Сохранить профиль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="529"/>
        <source>Save changes to this connection profile</source>
        <translation>Сохранить изменения этого профиля подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="541"/>
        <source>Export All Profiles</source>
        <translation>Экспортировать все профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="542"/>
        <source>Import Profiles</source>
        <translation>Импортировать профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="581"/>
        <source>Appearance</source>
        <translation>Внешний вид</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="587"/>
        <source>Text Size</source>
        <translation>Размер текста</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="596"/>
        <source>The text follows the font of your desktop. This factor enlarges or reduces it (Ctrl++, Ctrl+-, Ctrl+0).</source>
        <translation>Текст использует шрифт вашего рабочего стола. Этот коэффициент увеличивает или уменьшает его (Ctrl++, Ctrl+-, Ctrl+0).</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="598"/>
        <source>Interface zoom:</source>
        <translation>Масштаб интерфейса:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="603"/>
        <source>User Interface</source>
        <translation>Пользовательский интерфейс</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="607"/>
        <source>Enable animations</source>
        <translation>Включить анимацию</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="611"/>
        <source>Compact mode</source>
        <translation>Компактный режим</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="617"/>
        <source>Language</source>
        <translation>Язык</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="625"/>
        <source>System (%1)</source>
        <translation>Системный (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="637"/>
        <source>Application Language:</source>
        <translation>Язык приложения:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="640"/>
        <source>Language changes require an application restart. &quot;System&quot; follows the language of the operating system.</source>
        <translation>Для смены языка требуется перезапуск приложения. «Системный» следует языку операционной системы.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="649"/>
        <source>Status Bar Colors</source>
        <translation>Цвета строки состояния</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="658"/>
        <source>Color shown when connected to Director</source>
        <translation>Цвет при подключении к Director</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="660"/>
        <source>Connected:</source>
        <translation>Подключено:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="666"/>
        <source>Color shown when not connected</source>
        <translation>Цвет при отсутствии подключения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="668"/>
        <source>Disconnected:</source>
        <translation>Отключено:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="673"/>
        <source>Backup Level Colors</source>
        <translation>Цвета уровней резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="737"/>
        <source>Behavior</source>
        <translation>Поведение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="743"/>
        <source>Confirmations</source>
        <translation>Подтверждения</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="747"/>
        <source>Confirm job cancellation</source>
        <translation>Подтверждать отмену Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="751"/>
        <source>Confirm job start</source>
        <translation>Подтверждать запуск Job</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="758"/>
        <source>Auto-Refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="762"/>
        <source>Auto-refresh</source>
        <translation>Автообновление</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="766"/>
        <source>Interval:</source>
        <translation>Интервал:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="779"/>
        <source>Display</source>
        <translation>Отображение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="787"/>
        <source>Maximum Jobs:</source>
        <translation>Максимум Jobs:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="789"/>
        <source>Show newest jobs first</source>
        <translation>Сначала новые Jobs</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="792"/>
        <source>When enabled, the job list shows the most recent jobs first.
When disabled, the oldest jobs are shown first.</source>
        <translation>Если включено, список Jobs показывает сначала самые новые Jobs.
Если выключено, сначала показываются самые старые Jobs.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="799"/>
        <source> hours</source>
        <translation> ч</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="801"/>
        <source>The clients table marks a client as stale when its last backup or contact
is at least this old. The value is taken from the job history; it is not
a live connection check.</source>
        <translation>Таблица Clients помечает Client как устаревший, если его последнее резервное копирование
или контакт был не менее этого срока назад. Значение берётся из истории Jobs; это не
проверка подключения в реальном времени.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="804"/>
        <source>Client backup is stale after:</source>
        <translation>Резервная копия Client устаревает через:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="809"/>
        <source>Schedules</source>
        <translation>Schedules</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="816"/>
        <source> days</source>
        <translation> дн.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="818"/>
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
        <location filename="../src/config/bsettingsdialog.cpp" line="822"/>
        <source>Measure job history over the last:</source>
        <translation>Измерять историю Jobs за последние:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="827"/>
        <source>Visible Backup Levels</source>
        <translation>Видимые уровни резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="832"/>
        <source>Select which backup levels appear in the Run Backup dialog:</source>
        <translation>Выберите, какие уровни резервного копирования отображаются в диалоге запуска Backup:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="838"/>
        <source>Complete backup of all files</source>
        <translation>Полное резервное копирование всех файлов</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="839"/>
        <source>Backup files changed since last backup</source>
        <translation>Резервное копирование файлов, изменённых с последнего резервного копирования</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="840"/>
        <source>Backup files changed since last Full backup</source>
        <translation>Резервное копирование файлов, изменённых с последнего Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="841"/>
        <source>Consolidate incremental backups into a synthetic full</source>
        <translation>Объединить инкрементальные резервные копии в синтетическую полную</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="842"/>
        <source>Data-level backup (rarely used)</source>
        <translation>Резервное копирование на уровне данных (используется редко)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="890"/>
        <source>Advanced Settings</source>
        <translation>Дополнительные настройки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="897"/>
        <source>File Logging</source>
        <translation>Журналирование в файл</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="901"/>
        <source>Enable file logging</source>
        <translation>Включить журналирование в файл</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="902"/>
        <source>Write debug information to a log file</source>
        <translation>Записывать отладочную информацию в файл журнала</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="906"/>
        <source>Log File:</source>
        <translation>Файл журнала:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="916"/>
        <source>Max. Log Size:</source>
        <translation>Макс. размер журнала:</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="940"/>
        <source>Miscellaneous</source>
        <translation>Разное</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="944"/>
        <source>Show tooltips</source>
        <translation>Показывать подсказки</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="952"/>
        <source>⚠️ These settings are for advanced users.
Changes may affect application stability.</source>
        <translation>⚠️ Эти настройки предназначены для опытных пользователей.
Изменения могут повлиять на стабильность приложения.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1389"/>
        <source>Settings have been saved.
Some changes require an application restart.</source>
        <translation>Настройки сохранены.
Для некоторых изменений требуется перезапуск приложения.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1402"/>
        <source>Do you really want to reset all settings to default values?
This action cannot be undone.</source>
        <translation>Вы действительно хотите сбросить все настройки к значениям по умолчанию?
Это действие нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1410"/>
        <source>All settings have been reset to default values.</source>
        <translation>Все настройки сброшены к значениям по умолчанию.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1416"/>
        <source>Select CA Certificate</source>
        <translation>Выбор сертификата CA</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1417"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1429"/>
        <source>Certificates (*.pem *.crt *.cert);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cert);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1425"/>
        <source>Select Client Certificate</source>
        <translation>Выбор сертификата клиента</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1427"/>
        <source>Certificates (*.pfx *.pem *.crt *.cert *.der);;PFX Files (*.pfx);;PEM Files (*.pem);;DER Files (*.der);;All Files (*)</source>
        <translation>Сертификаты (*.pfx *.pem *.crt *.cert *.der);;Файлы PFX (*.pfx);;Файлы PEM (*.pem);;Файлы DER (*.der);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1438"/>
        <source>Select Private Key</source>
        <translation>Выбор закрытого ключа</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1439"/>
        <source>Keys (*.pem *.key);;All Files (*)</source>
        <translation>Ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1447"/>
        <source>Export Settings</source>
        <translation>Экспорт настроек</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1463"/>
        <source>Export</source>
        <translation>Экспорт</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1463"/>
        <source>Settings exported successfully.</source>
        <translation>Настройки успешно экспортированы.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1470"/>
        <source>Import Settings</source>
        <translation>Импорт настроек</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1485"/>
        <source>Import</source>
        <translation>Импорт</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1485"/>
        <source>Settings imported successfully.</source>
        <translation>Настройки успешно импортированы.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1494"/>
        <source>No Profiles</source>
        <translation>Нет профилей</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1495"/>
        <source>There are no connection profiles to delete.</source>
        <translation>Нет профилей подключения для удаления.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1499"/>
        <source>Clear All Profiles</source>
        <translation>Удалить все профили</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1500"/>
        <source>Do you really want to delete all %1 connection profile(s)?

This includes all passwords and certificate paths.
This action cannot be undone.</source>
        <translation>Вы действительно хотите удалить все профили подключения (%1)?

Это включает все пароли и пути к сертификатам.
Это действие нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1519"/>
        <source>Deleted</source>
        <translation>Удалено</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1520"/>
        <source>All connection profiles have been deleted.</source>
        <translation>Все профили подключения удалены.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1527"/>
        <source>Choose Color for Full Backup</source>
        <translation>Выбор цвета для Full</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1541"/>
        <source>Choose Color for Incremental Backup</source>
        <translation>Выбор цвета для Incremental</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1555"/>
        <source>Choose Color for Differential Backup</source>
        <translation>Выбор цвета для Differential</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1569"/>
        <source>Choose Color for Virtual Full Backup</source>
        <translation>Выбор цвета для VirtualFull</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1583"/>
        <source>Choose Color for Connected Status</source>
        <translation>Выбор цвета для состояния «Подключено»</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1597"/>
        <source>Choose Color for Disconnected Status</source>
        <translation>Выбор цвета для состояния «Отключено»</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1661"/>
        <source>••••••••  (password saved)</source>
        <translation>••••••••  (пароль сохранён)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1667"/>
        <source>Enter password</source>
        <translation>Введите пароль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1709"/>
        <source>New Connection</source>
        <translation>Новое подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1747"/>
        <source>Delete Profile</source>
        <translation>Удалить профиль</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1748"/>
        <source>Do you really want to delete the connection profile &quot;%1&quot;?

This action cannot be undone.</source>
        <translation>Вы действительно хотите удалить профиль подключения «%1»?

Это действие нельзя отменить.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1783"/>
        <source>%1 (Copy)</source>
        <translation>%1 (копия)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1807"/>
        <location filename="../src/config/bsettingsdialog.cpp" line="1897"/>
        <source>Unnamed Connection</source>
        <translation>Безымянное подключение</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1881"/>
        <source>(not pinned - confirmed on next connection)</source>
        <translation>(не закреплён — будет подтверждён при следующем подключении)</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1886"/>
        <source>SHA-256: %1</source>
        <translation>SHA-256: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1898"/>
        <source>Profile Saved</source>
        <translation>Профиль сохранён</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1899"/>
        <source>Connection profile &quot;%1&quot; has been saved.</source>
        <translation>Профиль подключения «%1» сохранён.</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1913"/>
        <source>PFX File Created</source>
        <translation>Файл PFX создан</translation>
    </message>
    <message>
        <location filename="../src/config/bsettingsdialog.cpp" line="1914"/>
        <source>PFX file has been created successfully.

The certificate path has been updated in the current profile.</source>
        <translation>Файл PFX успешно создан.

Путь к сертификату обновлён в текущем профиле.</translation>
    </message>
</context>
<context>
    <name>BStaggerPlan</name>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="32"/>
        <source>Schedule %1 has no Run lines.</source>
        <translation>У Schedule %1 нет строк Run.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="36"/>
        <source>Run line %1 of schedule %2 is not understood.</source>
        <translation>Строка Run %1 в Schedule %2 не распознана.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="39"/>
        <source>Run line %1 of schedule %2 runs at several hours.</source>
        <translation>Строка Run %1 в Schedule %2 выполняется в несколько часов.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="42"/>
        <source>Run line %1 of schedule %2 would move into the next day.</source>
        <translation>Строка Run %1 в Schedule %2 перешла бы на следующий день.</translation>
    </message>
    <message>
        <location filename="../src/schedules/bstaggerplan.cpp" line="52"/>
        <source>Run line %1 of schedule %2 did not read back as written (%3).</source>
        <translation>Строка Run %1 в Schedule %2 не прочиталась так, как была записана (%3).</translation>
    </message>
</context>
<context>
    <name>BStatus</name>
    <message>
        <location filename="../src/bstatuschip.cpp" line="25"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="25"/>
        <source>Terminated normally</source>
        <translation>Завершено нормально</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26"/>
        <source>Warnings</source>
        <translation>Предупреждения</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="26"/>
        <source>Terminated with warnings</source>
        <translation>Завершено с предупреждениями</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="27"/>
        <source>Terminated in error</source>
        <translation>Завершено с ошибкой</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28"/>
        <source>Errors</source>
        <translation>Ошибки</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="28"/>
        <source>Non-fatal error</source>
        <translation>Некритическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29"/>
        <source>Fatal</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="29"/>
        <source>Fatal error</source>
        <translation>Критическая ошибка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30"/>
        <source>Canceled</source>
        <translation>Отменено</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="30"/>
        <source>Canceled by user</source>
        <translation>Отменено пользователем</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="31"/>
        <source>Running</source>
        <translation>Выполняется</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32"/>
        <source>Committing</source>
        <translation>Фиксация</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="32"/>
        <source>Committing data (last despool)</source>
        <translation>Фиксация данных (последний despool)</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Despooling</source>
        <translation>Despooling</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="33"/>
        <source>Despooling data</source>
        <translation>Выгрузка данных из спула</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Created</source>
        <translation>Создано</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="34"/>
        <source>Created, not yet running</source>
        <translation>Создано, ещё не выполняется</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Wait FD</source>
        <translation>Ожидание FD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="35"/>
        <source>Waiting on the File daemon</source>
        <translation>Ожидание File daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Wait SD</source>
        <translation>Ожидание SD</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="36"/>
        <source>Waiting on the Storage daemon</source>
        <translation>Ожидание Storage daemon</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Wait media</source>
        <translation>Ожидание носителя</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="37"/>
        <source>Waiting for new media</source>
        <translation>Ожидание нового носителя</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38"/>
        <source>Wait mount</source>
        <translation>Ожидание монтирования</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="38"/>
        <source>Waiting for mount</source>
        <translation>Ожидание монтирования</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Wait storage</source>
        <translation>Ожидание Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="39"/>
        <source>Waiting for a storage resource</source>
        <translation>Ожидание ресурса Storage</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Wait job</source>
        <translation>Ожидание Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="40"/>
        <source>Waiting for a job resource</source>
        <translation>Ожидание ресурса Job</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Wait client</source>
        <translation>Ожидание Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="41"/>
        <source>Waiting for a client resource</source>
        <translation>Ожидание ресурса Client</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Wait max jobs</source>
        <translation>Ожидание max jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="42"/>
        <source>Waiting for maximum jobs</source>
        <translation>Ожидание из-за максимума Jobs</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Wait start</source>
        <translation>Ожидание старта</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="43"/>
        <source>Waiting for the start time</source>
        <translation>Ожидание времени запуска</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Wait priority</source>
        <translation>Ожидание приоритета</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="44"/>
        <source>Waiting for higher priority jobs to finish</source>
        <translation>Ожидание завершения Jobs с более высоким приоритетом</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Wait device</source>
        <translation>Ожидание устройства</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="45"/>
        <source>Queued, waiting for a device</source>
        <translation>В очереди, ожидание устройства</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Despool attr.</source>
        <translation>Despool атриб.</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="46"/>
        <source>Storage daemon despooling attributes</source>
        <translation>Storage daemon выгружает атрибуты из спула</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Inserting</source>
        <translation>Вставка</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="47"/>
        <source>Doing batch insert of file records</source>
        <translation>Пакетная вставка записей файлов</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="48"/>
        <source>Blocked</source>
        <translation>Заблокировано</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Differences</source>
        <translation>Различия</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="49"/>
        <source>Verify found differences</source>
        <translation>Verify обнаружил различия</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Incomplete</source>
        <translation>Не завершено</translation>
    </message>
    <message>
        <location filename="../src/bstatuschip.cpp" line="50"/>
        <source>Incomplete job</source>
        <translation>Незавершённый Job</translation>
    </message>
</context>
<context>
    <name>BStorageResourceWidget</name>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319"/>
        <source>Name</source>
        <translation>Имя</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319"/>
        <source>Address</source>
        <translation>Адрес</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319"/>
        <source>Port</source>
        <translation>Порт</translation>
    </message>
    <message>
        <location filename="../src/director/bresourcewidgets.cpp" line="319"/>
        <source>Device</source>
        <translation>Device</translation>
    </message>
</context>
<context>
    <name>BTableModel</name>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="215"/>
        <source>Yes</source>
        <translation>Да</translation>
    </message>
    <message>
        <location filename="../src/models/bbasemodels.cpp" line="215"/>
        <source>No</source>
        <translation>Нет</translation>
    </message>
</context>
<context>
    <name>BWaitCalibration</name>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="190"/>
        <source>this job was not measured: median of all %1 runs on this Director</source>
        <translation>этот Job не измерялся: медиана всех %1 запусков в этом Director</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="195"/>
        <source>nothing measured: %1 assumed</source>
        <translation>ничего не измерено: принято %1</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="203"/>
        <source>Duration %1: median of %2 measured run(s). It includes any wait for a device inside the storage daemon (the parallel writers of %3 are not known, so it is not taken out).</source>
        <translation>Длительность %1: медиана %2 измеренных запусков. Она включает любое ожидание устройства внутри демона Storage (параллельные записи %3 неизвестны, поэтому оно не вычитается).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="207"/>
        <source>its storage</source>
        <translation>его Storage</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="186"/>
        <source>median of %n measured run(s) at this level</source>
        <translation>
            <numerusform>медиана %n измеренного запуска на этом уровне</numerusform>
            <numerusform>медиана %n измеренных запусков на этом уровне</numerusform>
            <numerusform>медиана %n измеренных запусков на этом уровне</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="188"/>
        <source>median of %n measured run(s) of this job at other levels</source>
        <translation>
            <numerusform>медиана %n измеренного запуска этого Job на других уровнях</numerusform>
            <numerusform>медиана %n измеренных запусков этого Job на других уровнях</numerusform>
            <numerusform>медиана %n измеренных запусков этого Job на других уровнях</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="210"/>
        <source>Duration %1: median of %n measured run(s); they did not wait for the device.</source>
        <translation>
            <numerusform>Длительность %1: медиана %n измеренного запуска; он не ждал устройство.</numerusform>
            <numerusform>Длительность %1: медиана %n измеренных запусков; они не ждали устройство.</numerusform>
            <numerusform>Длительность %1: медиана %n измеренных запусков; они не ждали устройство.</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="214"/>
        <source>Duration %1 without the wait for the device (measured %2, median of %3 run(s)): counted from the end of the run that held the device before it (%4 parallel writer(s)). It may still hold up to 30 s of the storage daemon&apos;s tries.</source>
        <translation>Длительность %1 без ожидания устройства (измерено %2, медиана %3 запусков): отсчитывается от окончания запуска, который занимал устройство до него (параллельных записей: %4). Она всё ещё может содержать до 30 с попыток демона Storage.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="310"/>
        <source>%1 ended %2 after %3 on %4; alone it takes at least %5</source>
        <translation>%1 завершился через %2 после %3 %4; в одиночку он занимает не менее %5</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="327"/>
        <source>%1 night(s) with runs on storage %2; on %3 more than one of them was queued or running at the same time; on %4 the runs&apos; recorded start to end overlapped.</source>
        <translation>Ночей с запусками на Storage %2: %1; в %3 из них одновременно в очереди или в работе было более одного запуска; в %4 зафиксированные интервалы от начала до конца перекрывались.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="334"/>
        <source>No two runs ever overlapped although several were queued: the storage wrote one job at a time.</source>
        <translation>Никакие два запуска ни разу не перекрывались, хотя в очереди было несколько: Storage записывал по одному Job за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="340"/>
        <source>Overlapping runs do not prove parallel writing: with Just In Time Reservation a job waits for the device inside its run.</source>
        <translation>Перекрывающиеся запуски не доказывают параллельную запись: при Just In Time Reservation Job ожидает устройство внутри своего запуска.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="344"/>
        <source>%1 of %2 telling pair(s) ended sooner after each other than one writer allows (e.g. %3): it writes more than one job at a time.</source>
        <translation>%1 из %2 показательных пар завершились друг за другом быстрее, чем позволяет одна запись (например, %3): он записывает более одного Job за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="351"/>
        <source>In all %1 telling pair(s) on %2 night(s) the later run went on after the other ended for at least as long as it takes alone (e.g. %3): consistent with one writer at a time.</source>
        <translation>Во всех %1 показательных парах за %2 ноч. более поздний запуск продолжался после окончания другого не меньше, чем длится в одиночку (например, %3): это согласуется с одной записью за раз.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="358"/>
        <source>Not enough telling pairs (%1; a pair tells when the later run started well before the other ended and was also seen running alone): no suggestion.</source>
        <translation>Недостаточно показательных пар (%1; пара показательна, если более поздний запуск начался задолго до окончания другого и также наблюдался в одиночку): нет предложения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="446"/>
        <source>Back-test: no measured night to replay.</source>
        <translation>Ретроспективная проверка: нет измеренной ночи для воспроизведения.</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="447"/>
        <source>Back-test, last %1 night(s) (%2 runs): median error %3, worst %4 (%5, %6; %7).</source>
        <translation>Ретроспективная проверка, последние ночи: %1 (запусков: %2): медианная ошибка %3, наихудшая %4 (%5, %6; %7).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453"/>
        <source>predicted too late</source>
        <translation>прогноз слишком поздний</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="453"/>
        <source>predicted too early</source>
        <translation>прогноз слишком ранний</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="455"/>
        <source>Replayed without a device limit for %1 (not set).</source>
        <translation>Воспроизведено без ограничения устройства для %1 (не задано).</translation>
    </message>
    <message>
        <location filename="../src/schedules/model/bwaitcalibration.cpp" line="458"/>
        <source>Durations and overheads come from the same history.</source>
        <translation>Длительности и накладные расходы взяты из той же истории.</translation>
    </message>
</context>
<context>
    <name>BareosAuth</name>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="142"/>
        <source>Starting Bareos authentication...</source>
        <translation>Запуск аутентификации Bareos...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="149"/>
        <source>Setting up TLS-PSK...</source>
        <translation>Настройка TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="168"/>
        <source>Starting TLS-PSK handshake...</source>
        <translation>Запуск рукопожатия TLS-PSK...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="175"/>
        <source>Setting up certificate-based TLS...</source>
        <translation>Настройка TLS на основе сертификатов...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="191"/>
        <source>Starting TLS handshake...</source>
        <translation>Запуск рукопожатия TLS...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="199"/>
        <source>Starting legacy authentication (no TLS)...</source>
        <translation>Запуск устаревшей аутентификации (без TLS)...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="628"/>
        <source>TLS established, sending Hello packet...</source>
        <translation>TLS установлен, отправка пакета Hello...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="652"/>
        <source>Waiting for Director challenge...</source>
        <translation>Ожидание challenge от Director...</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="749"/>
        <source>Director rejected connection: %1</source>
        <translation>Director отклонил подключение: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="756"/>
        <source>

Hint: The Director requires TLS encryption. Please use PSK or Certificate authentication instead of Legacy mode.</source>
        <translation>

Подсказка: Director требует шифрования TLS. Используйте аутентификацию PSK или по сертификату вместо режима Legacy.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="762"/>
        <source>Director sent empty response.

This usually means the Director requires TLS encryption but Legacy (unencrypted) mode was used.
Please try PSK or Certificate authentication.</source>
        <translation>Director прислал пустой ответ.

Обычно это означает, что Director требует шифрования TLS, а использовался режим Legacy (без шифрования).
Попробуйте аутентификацию PSK или по сертификату.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="770"/>
        <source>Unexpected response from Director:
%1</source>
        <translation>Неожиданный ответ от Director:
%1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="832"/>
        <source>CRAM-MD5 response sent successfully</source>
        <translation>Ответ CRAM-MD5 успешно отправлен</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1228"/>
        <source>The Director announced a message of %1 bytes during authentication (limit: %2 bytes). Connection closed.</source>
        <translation>Во время аутентификации Director объявил сообщение размером %1 байт (лимит: %2 байт). Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1374"/>
        <location filename="../src/director/bareosauth.cpp" line="1488"/>
        <source>Connected</source>
        <translation>Подключено</translation>
    </message>
    <message>
        <location filename="../src/director/bareosauth.cpp" line="1669"/>
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
        <location filename="../src/director/bareosauth.cpp" line="1678"/>
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
        <location filename="../src/director/bareosauth.cpp" line="1686"/>
        <source>Socket error: %1</source>
        <translation>Ошибка сокета: %1</translation>
    </message>
</context>
<context>
    <name>BareosDirector</name>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="183"/>
        <source>Changing the configuration needs the gRPC interface of the Onesimus Director</source>
        <translation>Для изменения конфигурации нужен интерфейс gRPC Onesimus Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="235"/>
        <source>Connection to the Director was closed before the reply arrived</source>
        <translation>Подключение к Director было закрыто до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="294"/>
        <source>Disconnected from the Director before the reply arrived</source>
        <translation>Отключено от Director до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="461"/>
        <source>No response from Director within %1 seconds</source>
        <translation>Нет ответа от Director в течение %1 с</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="471"/>
        <source>Timeout while loading resources: %1</source>
        <translation>Тайм-аут при загрузке ресурсов: %1</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="910"/>
        <source>Connection to the Director was lost before the reply arrived</source>
        <translation>Подключение к Director было потеряно до получения ответа</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1006"/>
        <source>The Director did not confirm the API mode within %1 seconds. The connection was closed.</source>
        <translation>Director не подтвердил режим API в течение %1 с. Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1127"/>
        <source>The Director announced a message of %1 bytes (limit: %2 MiB). The connection was closed.</source>
        <translation>Director объявил сообщение размером %1 байт (лимит: %2 MiB). Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1559"/>
        <source>TLS is enabled for this connection, but the session with the Director is not encrypted. The connection was closed.</source>
        <translation>Для этого подключения включён TLS, но сеанс с Director не зашифрован. Подключение закрыто.</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1826"/>
        <source>Unknown error</source>
        <translation>Неизвестная ошибка</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1881"/>
        <location filename="../src/director/bareosdirector.cpp" line="2328"/>
        <source>Not ready</source>
        <translation>Не готово</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1933"/>
        <source>Error sending command</source>
        <translation>Ошибка отправки команды</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="1980"/>
        <source>The command was not sent: no ready connection to the Director</source>
        <translation>Команда не отправлена: нет готового подключения к Director</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2317"/>
        <source>This build of Onesimus has no gRPC support</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2408"/>
        <source>Server certificate not trusted</source>
        <translation>Сертификат сервера не является доверенным</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2424"/>
        <source>Command failed</source>
        <translation>Ошибка команды</translation>
    </message>
    <message>
        <location filename="../src/director/bareosdirector.cpp" line="2452"/>
        <source>Connection to Director lost: %1</source>
        <translation>Подключение к Director потеряно: %1</translation>
    </message>
</context>
<context>
    <name>CommandLine</name>
    <message>
        <location filename="../src/main.cpp" line="391"/>
        <source>--tls needs --host. Without --host the TLS settings of the saved connection profile are used.</source>
        <translation>--tls требует --host. Без --host используются настройки TLS сохранённого профиля подключения.</translation>
    </message>
    <message>
        <location filename="../src/main.cpp" line="397"/>
        <source>--tls needs the console password: use --password-file, the environment variable ONESIMUS_PASSWORD or --password.</source>
        <translation>--tls требует пароль консоли: используйте --password-file, переменную окружения ONESIMUS_PASSWORD или --password.</translation>
    </message>
</context>
<context>
    <name>ConfigPreviewPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2020"/>
        <source>Server Configuration Preview</source>
        <translation>Предпросмотр конфигурации сервера</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2022"/>
        <source>Copy this configuration to your Bareos Director</source>
        <translation>Скопируйте эту конфигурацию в ваш Bareos Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2032"/>
        <source>Console resource for the Director:</source>
        <translation>Ресурс Console для Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2036"/>
        <source>Path: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</source>
        <translation>Путь: /etc/bareos/bareos-dir.d/console/&amp;lt;name&amp;gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2048"/>
        <location filename="../src/bconnectionwizard.cpp" line="2089"/>
        <location filename="../src/bconnectionwizard.cpp" line="2130"/>
        <location filename="../src/bconnectionwizard.cpp" line="2179"/>
        <location filename="../src/bconnectionwizard.cpp" line="2467"/>
        <location filename="../src/bconnectionwizard.cpp" line="2506"/>
        <location filename="../src/bconnectionwizard.cpp" line="2517"/>
        <location filename="../src/bconnectionwizard.cpp" line="2528"/>
        <source>Copy to Clipboard</source>
        <translation>Копировать в буфер обмена</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2053"/>
        <location filename="../src/bconnectionwizard.cpp" line="2094"/>
        <location filename="../src/bconnectionwizard.cpp" line="2184"/>
        <location filename="../src/bconnectionwizard.cpp" line="2495"/>
        <location filename="../src/bconnectionwizard.cpp" line="2555"/>
        <location filename="../src/bconnectionwizard.cpp" line="2632"/>
        <source>Save .conf</source>
        <translation>Сохранить .conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2058"/>
        <location filename="../src/bconnectionwizard.cpp" line="2099"/>
        <location filename="../src/bconnectionwizard.cpp" line="2189"/>
        <location filename="../src/bconnectionwizard.cpp" line="2606"/>
        <location filename="../src/bconnectionwizard.cpp" line="2671"/>
        <source>Save ZIP</source>
        <translation>Сохранить ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2066"/>
        <source>Console Config</source>
        <translation>Конфигурация Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2073"/>
        <source>Director resource TLS settings (if needed):</source>
        <translation>Настройки TLS ресурса Director (при необходимости):</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2077"/>
        <location filename="../src/bconnectionwizard.cpp" line="2118"/>
        <source>Path: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Путь: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2107"/>
        <source>Director TLS</source>
        <translation>Director TLS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2114"/>
        <source>Enable gRPC in the Director resource:</source>
        <translation>Включите gRPC в ресурсе Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2137"/>
        <source>Director (gRPC)</source>
        <translation>Director (gRPC)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2145"/>
        <source>Configuration for command-line consoles on the client:</source>
        <translation>Конфигурация для консолей командной строки на клиенте:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2149"/>
        <source>Path: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf or oconsole -c &amp;lt;file&amp;gt; (oconsole)</source>
        <translation>Путь: /etc/bareos/bconsole.conf (bconsole), /etc/bareos/oconsole.conf или oconsole -c &amp;lt;file&amp;gt; (oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2157"/>
        <source>Contains the console password: restrict the file to mode 0640 root:bareos (or 0600 owned by the user running the console).</source>
        <translation>Содержит пароль консоли: ограничьте права файла режимом 0640 root:bareos (или 0600 с владельцем — пользователем, запускающим консоль).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2197"/>
        <source>Console client (bconsole / oconsole)</source>
        <translation>Клиент консоли (bconsole / oconsole)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2201"/>
        <location filename="../src/bconnectionwizard.cpp" line="2229"/>
        <source>After copying, paste this into your Bareos Director config and restart bareos-dir.</source>
        <translation>После копирования вставьте это в конфигурацию Bareos Director и перезапустите bareos-dir.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2227"/>
        <source>Add the Console resource and enable gRPC in the Director resource, then reload the Director.</source>
        <translation>Добавьте ресурс Console и включите gRPC в ресурсе Director, затем перезагрузите Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2324"/>
        <source>Default gRPC port: BNET port + 1000 (%1 + 1000 = %2).</source>
        <translation>Порт gRPC по умолчанию: порт BNET + 1000 (%1 + 1000 = %2).</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2326"/>
        <source>The Console resource provides name and password for the login.</source>
        <translation>Ресурс Console задаёт имя и пароль для входа.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2327"/>
        <source>After adding the Console, reload the Director:</source>
        <translation>После добавления Console перезагрузите Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2329"/>
        <source>(enabling gRPC for the first time requires a Director restart)</source>
        <translation>(при первом включении gRPC требуется перезапуск Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2416"/>
        <source>Password in &quot;[md5]&lt;hex&gt;&quot; form: accepted by oconsole and by C++ bconsole.</source>
        <translation>Пароль в форме «[md5]&lt;hex&gt;»: принимается oconsole и bconsole на C++.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2426"/>
        <source>not used when the Console resource below matches</source>
        <translation>не используется, если подходит ресурс Console ниже</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2434"/>
        <source>Grpc Fingerprint = &lt;SHA-256 of the Director&apos;s gRPC certificate&gt;</source>
        <translation>Grpc Fingerprint = &lt;SHA-256 сертификата gRPC Director&gt;</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2465"/>
        <location filename="../src/bconnectionwizard.cpp" line="2504"/>
        <location filename="../src/bconnectionwizard.cpp" line="2515"/>
        <location filename="../src/bconnectionwizard.cpp" line="2526"/>
        <source>Copied!</source>
        <translation>Скопировано!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2478"/>
        <source>Save Console Client Configuration</source>
        <translation>Сохранить конфигурацию клиента консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2479"/>
        <location filename="../src/bconnectionwizard.cpp" line="2539"/>
        <location filename="../src/bconnectionwizard.cpp" line="2616"/>
        <source>Configuration Files (*.conf);;All Files (*)</source>
        <translation>Файлы конфигурации (*.conf);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2485"/>
        <location filename="../src/bconnectionwizard.cpp" line="2545"/>
        <location filename="../src/bconnectionwizard.cpp" line="2575"/>
        <location filename="../src/bconnectionwizard.cpp" line="2622"/>
        <location filename="../src/bconnectionwizard.cpp" line="2651"/>
        <source>Save Failed</source>
        <translation>Ошибка сохранения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2486"/>
        <location filename="../src/bconnectionwizard.cpp" line="2546"/>
        <location filename="../src/bconnectionwizard.cpp" line="2623"/>
        <source>Could not write to %1</source>
        <translation>Не удалось записать в %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2493"/>
        <location filename="../src/bconnectionwizard.cpp" line="2553"/>
        <location filename="../src/bconnectionwizard.cpp" line="2604"/>
        <location filename="../src/bconnectionwizard.cpp" line="2630"/>
        <location filename="../src/bconnectionwizard.cpp" line="2669"/>
        <source>Saved!</source>
        <translation>Сохранено!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2538"/>
        <source>Save Console Configuration</source>
        <translation>Сохранить конфигурацию Console</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2565"/>
        <source>Save Console Configuration as ZIP</source>
        <translation>Сохранить конфигурацию Console в ZIP</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2566"/>
        <location filename="../src/bconnectionwizard.cpp" line="2642"/>
        <source>ZIP Archives (*.zip);;All Files (*)</source>
        <translation>ZIP-архивы (*.zip);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2576"/>
        <location filename="../src/bconnectionwizard.cpp" line="2652"/>
        <source>Could not create ZIP file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2615"/>
        <source>Save Director TLS Configuration</source>
        <translation>Сохранить конфигурацию TLS Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2641"/>
        <source>Save Director TLS Configuration as ZIP</source>
        <translation>Сохранить конфигурацию TLS Director в ZIP</translation>
    </message>
</context>
<context>
    <name>ConsoleSetupPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2948"/>
        <source>Console configuration</source>
        <translation>Конфигурация консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2956"/>
        <source>Modify existing console</source>
        <translation>Изменить существующую консоль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2969"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2979"/>
        <source>Select a console to view its configuration...</source>
        <translation>Выберите консоль, чтобы просмотреть её конфигурацию...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2987"/>
        <source>Create new console</source>
        <translation>Создать новую консоль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2998"/>
        <source>Name:</source>
        <translation>Имя:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3005"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3009"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3017"/>
        <source>Security notice: Consoles with overly wide ACLs (e.g. *all*) grant full access to all backup operations, including deletion. Ask your backup administrator to create a Console resource with restricted ACLs tailored to your needs.</source>
        <translation>Предупреждение о безопасности: консоли со слишком широкими ACL (например, *all*) дают полный доступ ко всем операциям резервного копирования, включая удаление. Попросите администратора резервного копирования создать ресурс Console с ограниченными ACL, соответствующими вашим задачам.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3033"/>
        <location filename="../src/bconnectionwizard.cpp" line="3058"/>
        <source>Modify an existing console or create a new one on the director.</source>
        <translation>Измените существующую консоль или создайте новую в Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3056"/>
        <source>gRPC requires &apos;Enable Grpc = yes&apos; in the Director resource (default port: BNET port + 1000). After adding a console, reload the Director.</source>
        <translation>Для gRPC требуется &apos;Enable Grpc = yes&apos; в ресурсе Director (порт по умолчанию: порт BNET + 1000). После добавления консоли перезагрузите Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068"/>
        <source>No Console</source>
        <translation>Нет консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3068"/>
        <source>Please select a console.</source>
        <translation>Выберите консоль.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076"/>
        <source>Missing</source>
        <translation>Отсутствует</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3076"/>
        <source>Please enter a name and password for the new console.</source>
        <translation>Введите имя и пароль для новой консоли.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3179"/>
        <location filename="../src/bconnectionwizard.cpp" line="3275"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3197"/>
        <source>Loading details for %1...</source>
        <translation>Загрузка сведений о %1...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3206"/>
        <source>Waiting for connection...</source>
        <translation>Ожидание подключения...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3216"/>
        <location filename="../src/bconnectionwizard.cpp" line="3257"/>
        <source>Loading consoles...</source>
        <translation>Загрузка консолей...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3222"/>
        <source>Connecting to Director...</source>
        <translation>Подключение к Director...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3246"/>
        <source>Loading resources...</source>
        <translation>Загрузка ресурсов...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3248"/>
        <source>Authentication failed: %1</source>
        <translation>Ошибка аутентификации: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3266"/>
        <source>Connection lost</source>
        <translation>Подключение потеряно</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3299"/>
        <source>No named consoles found. Ask your backup administrator to create a personalized Console resource on the Director.</source>
        <translation>Именованные консоли не найдены. Попросите администратора резервного копирования создать персональный ресурс Console в Director.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="3303"/>
        <source>Found %n console(s)</source>
        <translation>
            <numerusform>Найдена %n Console</numerusform>
            <numerusform>Найдено %n Console</numerusform>
            <numerusform>Найдено %n Console</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3320"/>
        <source>No details found for %1</source>
        <translation>Сведения о %1 не найдены</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3325"/>
        <source>Name: %1
</source>
        <translation>Имя: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3327"/>
        <source>Description: %1
</source>
        <translation>Описание: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3329"/>
        <source>Profile: %1
</source>
        <translation>Profile: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3334"/>
        <source>TLS Enabled: %1
</source>
        <translation>TLS Enabled: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3336"/>
        <source>TLS Required: %1
</source>
        <translation>TLS Required: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3338"/>
        <source>TLS Verify Peer: %1
</source>
        <translation>TLS Verify Peer: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3340"/>
        <source>Job ACL: %1
</source>
        <translation>Job ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3342"/>
        <source>Client ACL: %1
</source>
        <translation>Client ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3344"/>
        <source>Storage ACL: %1
</source>
        <translation>Storage ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3346"/>
        <source>Command ACL: %1
</source>
        <translation>Command ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3348"/>
        <source>Catalog ACL: %1
</source>
        <translation>Catalog ACL: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3362"/>
        <source>
--- WARNING ---
This console has full access (*all*). Consider restricting ACLs to prevent accidental deletion of backups or other critical operations.
</source>
        <translation>
--- ПРЕДУПРЕЖДЕНИЕ ---
У этой консоли полный доступ (*all*). Рекомендуется ограничить ACL, чтобы предотвратить случайное удаление резервных копий или другие критические операции.
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3397"/>
        <source>Creating console &apos;%1&apos;...</source>
        <translation>Создание консоли &apos;%1&apos;...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3408"/>
        <source>Console created successfully!</source>
        <translation>Консоль успешно создана!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3411"/>
        <source>Failed to create console</source>
        <translation>Не удалось создать консоль</translation>
    </message>
</context>
<context>
    <name>CredentialsPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1418"/>
        <source>Enter your credentials</source>
        <translation>Введите учётные данные</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1424"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1428"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1436"/>
        <source>Generate</source>
        <translation>Сгенерировать</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1437"/>
        <source>Generate a secure random password</source>
        <translation>Сгенерировать надёжный случайный пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1442"/>
        <source>Password:</source>
        <translation>Пароль:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1449"/>
        <location filename="../src/bconnectionwizard.cpp" line="1455"/>
        <source>(password MD5 hash will appear here)</source>
        <translation>(здесь появится MD5-хеш пароля)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1450"/>
        <source>MD5 Hash:</source>
        <translation>MD5-хеш:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1488"/>
        <source>Remember password</source>
        <translation>Запомнить пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1497"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Console config: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</source>
        <translation>Конфигурация Director: /etc/bareos/bareos-dir.d/director/bareos-dir.conf
Конфигурация Console: /etc/bareos/bareos-dir.d/console/&lt;name&gt;.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1503"/>
        <source>These must match your Bareos Director configuration.</source>
        <translation>Они должны совпадать с конфигурацией вашего Bareos Director.</translation>
    </message>
</context>
<context>
    <name>ImportConsoleSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="723"/>
        <source>Select Director and Console from imported configuration</source>
        <translation>Выберите Director и Console из импортированной конфигурации</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="728"/>
        <source>The configuration will be parsed from your ZIP file. Select which Director and Console to use for the connection.</source>
        <translation>Конфигурация будет считана из вашего ZIP-файла. Выберите, какие Director и Console использовать для подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="734"/>
        <source>Director:</source>
        <translation>Director:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="737"/>
        <source>Console:</source>
        <translation>Console:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="758"/>
        <source>Select a console to view its details...</source>
        <translation>Выберите консоль, чтобы просмотреть сведения о ней...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="800"/>
        <source>No ZIP file specified.</source>
        <translation>ZIP-файл не указан.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="806"/>
        <source>Extracting and parsing ZIP archive...</source>
        <translation>Распаковка и разбор ZIP-архива...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="814"/>
        <source>Failed to create temporary directory.</source>
        <translation>Не удалось создать временный каталог.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="823"/>
        <source>Cannot read ZIP file: %1</source>
        <translation>Не удалось прочитать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="830"/>
        <source>Failed to extract ZIP archive.</source>
        <translation>Не удалось распаковать ZIP-архив.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="867"/>
        <source>No configuration resources found in the archive.</source>
        <translation>В архиве не найдено ресурсов конфигурации.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="892"/>
        <source>Parsed %1 resources: %2 Director(s), %3 Console(s)</source>
        <translation>Разобрано ресурсов: %1 (Director: %2, Console: %3)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="899"/>
        <source>Warning: Need at least one Director and one Console resource.</source>
        <translation>Предупреждение: нужен как минимум один ресурс Director и один ресурс Console.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="920"/>
        <source>=== Director: %1 ===
</source>
        <translation>=== Director: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="925"/>
        <source>(multiple addresses)</source>
        <translation>(несколько адресов)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928"/>
        <source>  Address: %1
</source>
        <translation>  Адрес: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="928"/>
        <source>(not set)</source>
        <translation>(не задано)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="929"/>
        <source>  Port: %1
</source>
        <translation>  Порт: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="930"/>
        <location filename="../src/bconnectionwizard.cpp" line="952"/>
        <source>  TLS Enable: %1
</source>
        <translation>  TLS Enable: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="937"/>
        <source>=== Console: %1 ===
</source>
        <translation>=== Console: %1 ===
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="943"/>
        <source>  Password: [md5]***
</source>
        <translation>  Пароль: [md5]***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="945"/>
        <source>  Password: ***
</source>
        <translation>  Пароль: ***
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="948"/>
        <source>  Password: (not set)
</source>
        <translation>  Пароль: (не задан)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="953"/>
        <source>  TLS Require: %1
</source>
        <translation>  TLS Require: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="958"/>
        <source>  Profile: %1
</source>
        <translation>  Profile: %1
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="974"/>
        <source>
  --- ACLs ---
</source>
        <translation>
  --- ACL ---
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="977"/>
        <source>  %1: %2
</source>
        <translation>  %1: %2
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="982"/>
        <source>
  (No ACLs defined on this console)
</source>
        <translation>
  (В этой консоли не определены ACL)
</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1119"/>
        <source>Host/Address</source>
        <translation>Хост/адрес</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1120"/>
        <source>Director Name</source>
        <translation>Имя Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1121"/>
        <source>Console Name</source>
        <translation>Имя консоли</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1122"/>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1125"/>
        <source>Incomplete Configuration</source>
        <translation>Неполная конфигурация</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1126"/>
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
        <location filename="../src/bconnectionwizard.cpp" line="3422"/>
        <source>Name this connection</source>
        <translation>Назовите это подключение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3425"/>
        <source>e.g., Production Server</source>
        <translation>например, Рабочий сервер</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3428"/>
        <source>Set as default</source>
        <translation>Использовать по умолчанию</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3432"/>
        <source>Connect now</source>
        <translation>Подключиться сейчас</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3442"/>
        <source>Advanced Settings...</source>
        <translation>Дополнительные настройки...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3443"/>
        <source>Configure TLS options, ACLs, and other advanced settings</source>
        <translation>Настроить параметры TLS, ACL и другие дополнительные параметры</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3448"/>
        <source>Export Config...</source>
        <translation>Экспорт конфигурации...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3449"/>
        <source>Export Bareos configuration files as zip archive</source>
        <translation>Экспортировать файлы конфигурации Bareos в ZIP-архив</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3456"/>
        <source>This name helps identify the connection.</source>
        <translation>Это имя помогает распознать подключение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3536"/>
        <source>Console only (for existing Director)</source>
        <translation>Только Console (для существующего Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3537"/>
        <source>Console + Profile (with ACLs)</source>
        <translation>Console + Profile (с ACL)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3538"/>
        <source>Full configuration (new installation)</source>
        <translation>Полная конфигурация (новая установка)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3542"/>
        <location filename="../src/bconnectionwizard.cpp" line="3557"/>
        <source>Export Configuration</source>
        <translation>Экспорт конфигурации</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3543"/>
        <source>Select export mode:</source>
        <translation>Выберите режим экспорта:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3559"/>
        <source>Zip Archives (*.zip)</source>
        <translation>ZIP-архивы (*.zip)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3564"/>
        <source>Export Successful</source>
        <translation>Экспорт выполнен</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3565"/>
        <source>Configuration exported to:
%1

Extract to your Bareos server&apos;s root directory and restart the Director.</source>
        <translation>Конфигурация экспортирована в:
%1

Распакуйте её в корневой каталог сервера Bareos и перезапустите Director.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3569"/>
        <source>Export Failed</source>
        <translation>Ошибка экспорта</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="3570"/>
        <source>Failed to export configuration:
%1</source>
        <translation>Не удалось экспортировать конфигурацию:
%1</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="30"/>
        <source>No files to export</source>
        <translation>Нет файлов для экспорта</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="203"/>
        <source>Cannot create zip file: %1</source>
        <translation>Не удалось создать ZIP-файл: %1</translation>
    </message>
    <message>
        <location filename="../src/config/bconfigexporter.cpp" line="234"/>
        <source>Error writing zip file</source>
        <translation>Ошибка записи ZIP-файла</translation>
    </message>
    <message>
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="295"/>
        <source>Not set</source>
        <translation>Не задано</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="299"/>
        <source>%n year(s)</source>
        <translation>
            <numerusform>%n год</numerusform>
            <numerusform>%n года</numerusform>
            <numerusform>%n лет</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="301"/>
        <source>%n day(s)</source>
        <translation>
            <numerusform>%n день</numerusform>
            <numerusform>%n дня</numerusform>
            <numerusform>%n дней</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="305"/>
        <source>%n hour(s)</source>
        <translation>
            <numerusform>%n час</numerusform>
            <numerusform>%n часа</numerusform>
            <numerusform>%n часов</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../src/clients/bclientdetailsdialog.cpp" line="307"/>
        <source>%n second(s)</source>
        <translation>
            <numerusform>%n секунда</numerusform>
            <numerusform>%n секунды</numerusform>
            <numerusform>%n секунд</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="27"/>
        <source>Add &quot;%1&quot;</source>
        <translation>Добавить «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="61"/>
        <source>Remove &quot;%1&quot;</source>
        <translation>Удалить «%1»</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="63"/>
        <source>Remove %1 items</source>
        <translation>Удалить элементы (%1)</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="96"/>
        <source>Edit item</source>
        <translation>Изменить элемент</translation>
    </message>
    <message>
        <location filename="../src/config/bundocommands.cpp" line="137"/>
        <source>Move item</source>
        <translation>Переместить элемент</translation>
    </message>
</context>
<context>
    <name>ServerPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1154"/>
        <source>Where is your Bareos Director?</source>
        <translation>Где находится ваш Bareos Director?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1159"/>
        <source>Found in the network:</source>
        <translation>Найдено в сети:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1161"/>
        <source>Search again</source>
        <translation>Искать снова</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1171"/>
        <source>Onesimus Directors announce themselves in the local network. Select one to fill in the connection data.</source>
        <translation>Onesimus Directors сообщают о себе в локальной сети. Выберите один, чтобы заполнить данные подключения.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1186"/>
        <source>Searching...</source>
        <translation>Поиск...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1197"/>
        <source>e.g., bareos.example.com or 192.168.1.100</source>
        <translation>например, bareos.example.com или 192.168.1.100</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1198"/>
        <source>Host:</source>
        <translation>Хост:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1204"/>
        <source>Port:</source>
        <translation>Порт:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1207"/>
        <source>Linux/Unix</source>
        <translation>Linux/Unix</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1208"/>
        <source>Windows</source>
        <translation>Windows</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1209"/>
        <source>FreeBSD</source>
        <translation>FreeBSD</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1210"/>
        <source>macOS</source>
        <translation>macOS</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1211"/>
        <source>Select the operating system of the Bareos Director server.
This affects certificate paths in exported configurations.</source>
        <translation>Выберите операционную систему сервера Bareos Director.
Это влияет на пути к сертификатам в экспортируемых конфигурациях.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1213"/>
        <source>Server OS:</source>
        <translation>ОС сервера:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1217"/>
        <source>BNET (Bareos console protocol)</source>
        <translation>BNET (протокол консоли Bareos)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1218"/>
        <source>gRPC (Onesimus Director)</source>
        <translation>gRPC (Onesimus Director)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1224"/>
        <source>This build of Onesimus has no gRPC support.</source>
        <translation>Эта сборка Onesimus не поддерживает gRPC.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1226"/>
        <source>Protocol:</source>
        <translation>Протокол:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1232"/>
        <source>gRPC port:</source>
        <translation>Порт gRPC:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1246"/>
        <source>Director config: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</source>
        <translation>Конфигурация Director: /etc/bareos/bareos-dir.d/director/bareos-dir.conf</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1249"/>
        <location filename="../src/bconnectionwizard.cpp" line="1308"/>
        <source>The default port is 9101.</source>
        <translation>Порт по умолчанию — 9101.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1286"/>
        <source>Searching for Onesimus Directors in the network...</source>
        <translation>Поиск Onesimus Directors в сети...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1292"/>
        <source>No Onesimus Director found - enter the server manually.</source>
        <translation>Onesimus Director не найден — введите сервер вручную.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1307"/>
        <source>gRPC is encrypted with TLS; the server certificate is confirmed on first use.</source>
        <translation>gRPC шифруется с помощью TLS; сертификат сервера подтверждается при первом использовании.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1327"/>
        <source>(gRPC %1)</source>
        <translation>(gRPC %1)</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/bconnectionwizard.cpp" line="1340"/>
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
        <location filename="../src/storagewidget.cpp" line="27"/>
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="53"/>
        <source>(due)</source>
        <translation>(срок наступил)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="85"/>
        <source>&amp;Pool:</source>
        <translation>&amp;Pool:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="88"/>
        <source>Pool filter</source>
        <translation>Фильтр Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="94"/>
        <source>&amp;Status:</source>
        <translation>&amp;Статус:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="97"/>
        <source>Status filter</source>
        <translation>Фильтр статуса</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="103"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="114"/>
        <source>Statistics</source>
        <translation>Статистика</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="117"/>
        <source>Volumes:</source>
        <translation>Volumes:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="125"/>
        <source>Used:</source>
        <translation>Использовано:</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <source>Volume</source>
        <translation>Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <location filename="../src/storagewidget.cpp" line="451"/>
        <source>Pool</source>
        <translation>Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <location filename="../src/storagewidget.cpp" line="452"/>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <location filename="../src/storagewidget.cpp" line="455"/>
        <source>Used</source>
        <translation>Использовано</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <location filename="../src/storagewidget.cpp" line="456"/>
        <source>Limit</source>
        <translation>Лимит</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="145"/>
        <location filename="../src/storagewidget.cpp" line="457"/>
        <source>Jobs</source>
        <translation>Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146"/>
        <source>Last Written</source>
        <translation>Последняя запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146"/>
        <source>Retention</source>
        <translation>Хранение</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146"/>
        <source>Reusable From</source>
        <translation>Повторно используемо с</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="146"/>
        <source>Reason</source>
        <translation>Причина</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="149"/>
        <source>Maximum Volume Bytes of the volume; &quot;none&quot; = no limit</source>
        <translation>Maximum Volume Bytes этого Volume; «нет» = без ограничения</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="151"/>
        <source>From when Bareos may reuse the volume. It does so only when its pool needs a volume, so this is the earliest date, not the day of recycling.</source>
        <translation>С какого момента Bareos может повторно использовать Volume. Это происходит только тогда, когда его Pool нужен Volume, поэтому это самая ранняя дата, а не день повторного использования.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="153"/>
        <source>Volumes</source>
        <translation>Volumes</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="155"/>
        <source>One row per volume. Enter opens the details of the selected volume.</source>
        <translation>По одной строке на Volume. Enter открывает сведения о выбранном Volume.</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="212"/>
        <source>All pools (%1)</source>
        <translation>Все Pools (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="215"/>
        <source>%1 (%2)</source>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="221"/>
        <source>All statuses (%1)</source>
        <translation>Все статусы (%1)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="224"/>
        <source>%1 %2 (%3)</source>
        <translation>%1 %2 (%3)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="247"/>
        <source>Error parsing volume data</source>
        <translation>Ошибка разбора данных Volume</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="254"/>
        <source>The Director sent the short volume list: limits, job counts and use durations are unknown, and so is when an Append volume will be closed.</source>
        <translation>Director прислал краткий список Volumes: лимиты, количество Jobs и сроки использования неизвестны, как и то, когда Volume в состоянии Append будет закрыт.</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/storagewidget.cpp" line="268"/>
        <source>%n volume(s) loaded</source>
        <translation>
            <numerusform>Загружен %n Volume</numerusform>
            <numerusform>Загружено %n Volumes</numerusform>
            <numerusform>Загружено %n Volumes</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="315"/>
        <source>%1, %2, %3, last written %4, reusable from %5</source>
        <translation>%1, %2, %3, последняя запись %4, повторно используемо с %5</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="343"/>
        <location filename="../src/storagewidget.cpp" line="456"/>
        <location filename="../src/storagewidget.cpp" line="458"/>
        <location filename="../src/storagewidget.cpp" line="464"/>
        <location filename="../src/storagewidget.cpp" line="466"/>
        <source>none</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="351"/>
        <source>%1 of %2</source>
        <translation>%1 из %2</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="408"/>
        <source>Refreshing the volume list...</source>
        <translation>Обновление списка Volumes...</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="436"/>
        <source>unknown (not in this list)</source>
        <translation>неизвестно (нет в этом списке)</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="453"/>
        <source>Media type</source>
        <translation>Тип носителя</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="458"/>
        <source>Maximum jobs</source>
        <translation>Максимум Jobs</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="459"/>
        <source>Errors</source>
        <translation>Ошибки</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="460"/>
        <source>First written</source>
        <translation>Первая запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="461"/>
        <source>Last written</source>
        <translation>Последняя запись</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="462"/>
        <source>Labeled</source>
        <translation>Размечено</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="463"/>
        <source>Volume retention</source>
        <translation>Volume Retention</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="464"/>
        <source>Use duration</source>
        <translation>Срок использования</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465"/>
        <source>Recycle</source>
        <translation>Recycle</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465"/>
        <source>yes</source>
        <translation>да</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="465"/>
        <source>no</source>
        <translation>нет</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="466"/>
        <source>Recycle pool</source>
        <translation>Recycle Pool</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="467"/>
        <source>Times recycled</source>
        <translation>Число повторных использований</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="468"/>
        <source>Reusable from</source>
        <translation>Повторно используемо с</translation>
    </message>
    <message>
        <location filename="../src/storagewidget.cpp" line="478"/>
        <source>Volume Details</source>
        <translation>Сведения о Volume</translation>
    </message>
    <message>
        <location filename="../ui/storagewidget.ui" line="14"/>
        <location filename="../src/storagewidget.cpp" line="454"/>
        <source>Storage</source>
        <translation>Storage</translation>
    </message>
</context>
<context>
    <name>TLSPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1768"/>
        <source>Where are your TLS certificates?</source>
        <translation>Где находятся ваши сертификаты TLS?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1774"/>
        <source>/etc/bareos/ssl/ca.pem</source>
        <translation>/etc/bareos/ssl/ca.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1775"/>
        <location filename="../src/bconnectionwizard.cpp" line="1786"/>
        <location filename="../src/bconnectionwizard.cpp" line="1807"/>
        <location filename="../src/bconnectionwizard.cpp" line="1816"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1779"/>
        <source>CA Cert:</source>
        <translation>Сертификат CA:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1785"/>
        <source>Path to PFX certificate file</source>
        <translation>Путь к файлу сертификата PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1790"/>
        <source>PFX File:</source>
        <translation>Файл PFX:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1794"/>
        <source>PFX file password (if encrypted)</source>
        <translation>Пароль файла PFX (если зашифрован)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1795"/>
        <source>PFX Password:</source>
        <translation>Пароль PFX:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1806"/>
        <source>/etc/bareos/ssl/client.pem</source>
        <translation>/etc/bareos/ssl/client.pem</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1811"/>
        <source>Client Cert:</source>
        <translation>Сертификат клиента:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1815"/>
        <source>/etc/bareos/ssl/client.key</source>
        <translation>/etc/bareos/ssl/client.key</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1820"/>
        <source>Private Key:</source>
        <translation>Закрытый ключ:</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1830"/>
        <source>Convert PEM/DER to PFX...</source>
        <translation>Преобразовать PEM/DER в PFX...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1831"/>
        <source>Convert separate PEM/DER certificate and key files into a PFX file</source>
        <translation>Преобразовать отдельные файлы сертификата и ключа PEM/DER в файл PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1836"/>
        <source>Generate Certificates...</source>
        <translation>Сгенерировать сертификаты...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1839"/>
        <source>No certificates? Click Generate to create them.</source>
        <translation>Нет сертификатов? Нажмите «Сгенерировать», чтобы создать их.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875"/>
        <location filename="../src/bconnectionwizard.cpp" line="1886"/>
        <location filename="../src/bconnectionwizard.cpp" line="1898"/>
        <source>Missing</source>
        <translation>Отсутствует</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1875"/>
        <source>Please provide a CA certificate file.</source>
        <translation>Укажите файл сертификата CA.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879"/>
        <location filename="../src/bconnectionwizard.cpp" line="1890"/>
        <location filename="../src/bconnectionwizard.cpp" line="1902"/>
        <source>Not Found</source>
        <translation>Не найдено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1879"/>
        <source>CA certificate file not found.</source>
        <translation>Файл сертификата CA не найден.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1886"/>
        <source>Please provide a PFX certificate file.</source>
        <translation>Укажите файл сертификата PFX.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1890"/>
        <source>PFX certificate file not found.</source>
        <translation>Файл сертификата PFX не найден.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1898"/>
        <source>Please provide all certificate files.</source>
        <translation>Укажите все файлы сертификатов.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1902"/>
        <source>One or more certificate files not found.</source>
        <translation>Один или несколько файлов сертификатов не найдены.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1936"/>
        <source>CA Certificate</source>
        <translation>Сертификат CA</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1937"/>
        <location filename="../src/bconnectionwizard.cpp" line="1958"/>
        <source>Certificates (*.pem *.crt *.cer);;All Files (*)</source>
        <translation>Сертификаты (*.pem *.crt *.cer);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1954"/>
        <source>Select PFX Certificate</source>
        <translation>Выбор сертификата PFX</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1955"/>
        <source>PFX Files (*.pfx);;Certificates (*.pem *.crt);;All Files (*)</source>
        <translation>Файлы PFX (*.pfx);;Сертификаты (*.pem *.crt);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1957"/>
        <source>Client Certificate</source>
        <translation>Сертификат клиента</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1975"/>
        <source>Private Key</source>
        <translation>Закрытый ключ</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="1976"/>
        <source>Private Keys (*.pem *.key);;All Files (*)</source>
        <translation>Закрытые ключи (*.pem *.key);;Все файлы (*)</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2008"/>
        <source>PFX File Created</source>
        <translation>Файл PFX создан</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2009"/>
        <source>PFX file has been created successfully.

The certificate path has been updated.</source>
        <translation>Файл PFX успешно создан.

Путь к сертификату обновлён.</translation>
    </message>
</context>
<context>
    <name>TemplateSelectionPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="375"/>
        <source>How would you like to create the connection?</source>
        <translation>Как вы хотите создать подключение?</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="377"/>
        <source>You can start from scratch, use a template from an existing connection, or import from a ZIP file.</source>
        <translation>Можно начать с нуля, использовать шаблон существующего подключения или импортировать из ZIP-файла.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="381"/>
        <source>Create new connection from scratch</source>
        <translation>Создать новое подключение с нуля</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="382"/>
        <source>Use template from existing connection</source>
        <translation>Использовать шаблон существующего подключения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="383"/>
        <source>Import from ZIP file</source>
        <translation>Импорт из ZIP-файла</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="405"/>
        <source>Refresh</source>
        <translation>Обновить</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="428"/>
        <source>Select ZIP file...</source>
        <translation>Выберите ZIP-файл...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="431"/>
        <source>Browse...</source>
        <translation>Обзор...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="542"/>
        <source>Director: %1</source>
        <translation>Director: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="543"/>
        <source>Console: %1</source>
        <translation>Console: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544"/>
        <location filename="../src/bconnectionwizard.cpp" line="600"/>
        <source>TLS: %1</source>
        <translation>TLS: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544"/>
        <source>PSK</source>
        <translation>PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544"/>
        <source>Certificate</source>
        <translation>Сертификат</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="544"/>
        <location filename="../src/bconnectionwizard.cpp" line="600"/>
        <source>Disabled</source>
        <translation>Отключено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="600"/>
        <source>Enabled</source>
        <translation>Включено</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="601"/>
        <source>System: %1</source>
        <translation>Система: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="707"/>
        <source>Select Director Configuration ZIP</source>
        <translation>Выбор ZIP-файла конфигурации Director</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="709"/>
        <source>ZIP Files (*.zip);;All Files (*)</source>
        <translation>ZIP-файлы (*.zip);;Все файлы (*)</translation>
    </message>
</context>
<context>
    <name>TestPage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2682"/>
        <location filename="../src/bconnectionwizard.cpp" line="2699"/>
        <location filename="../src/bconnectionwizard.cpp" line="2729"/>
        <location filename="../src/bconnectionwizard.cpp" line="2838"/>
        <source>Test Connection</source>
        <translation>Проверка подключения</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2686"/>
        <source>Let&apos;s test your connection.</source>
        <translation>Давайте проверим ваше подключение.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2690"/>
        <location filename="../src/bconnectionwizard.cpp" line="2726"/>
        <source>Click the button to test.</source>
        <translation>Нажмите кнопку, чтобы начать проверку.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2749"/>
        <source>Connecting...</source>
        <translation>Подключение...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2752"/>
        <source>Starting test...</source>
        <translation>Запуск проверки...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2791"/>
        <source>Mode: TLS-Cert</source>
        <translation>Режим: TLS-Cert</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2803"/>
        <source>Mode: TLS-PSK with cipher list: %1</source>
        <translation>Режим: TLS-PSK со списком шифров: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2805"/>
        <source>Mode: TLS-PSK</source>
        <translation>Режим: TLS-PSK</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2818"/>
        <source>Mode: gRPC (TLS) on port %1</source>
        <translation>Режим: gRPC (TLS) на порту %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2819"/>
        <location filename="../src/bconnectionwizard.cpp" line="2821"/>
        <source>Connecting to %1:%2...</source>
        <translation>Подключение к %1:%2...</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2857"/>
        <source>Cancelled.</source>
        <translation>Отменено.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2865"/>
        <source>Connecting</source>
        <translation>Подключение</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2866"/>
        <source>Authenticating</source>
        <translation>Аутентификация</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2867"/>
        <source>Setting API</source>
        <translation>Установка API</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2868"/>
        <source>Loading</source>
        <translation>Загрузка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2869"/>
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2891"/>
        <source>Server certificate trusted.</source>
        <translation>Сертификат сервера является доверенным.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2895"/>
        <source>Server certificate not trusted - connection cancelled.</source>
        <translation>Сертификат сервера не является доверенным — подключение отменено.</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2896"/>
        <location filename="../src/bconnectionwizard.cpp" line="2909"/>
        <location filename="../src/bconnectionwizard.cpp" line="2915"/>
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2908"/>
        <source>Auth OK</source>
        <translation>Аутентификация успешна</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2909"/>
        <source>Auth failed: %1</source>
        <translation>Ошибка аутентификации: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2914"/>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2925"/>
        <source>Success!</source>
        <translation>Успешно!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2926"/>
        <source>Connection successful!</source>
        <translation>Подключение выполнено успешно!</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2929"/>
        <source>Test Again</source>
        <translation>Проверить снова</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2937"/>
        <source>Timeout</source>
        <translation>Тайм-аут</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="2938"/>
        <source>Timed out</source>
        <translation>Время ожидания истекло</translation>
    </message>
</context>
<context>
    <name>WelcomePage</name>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="353"/>
        <source>Welcome</source>
        <translation>Добро пожаловать</translation>
    </message>
    <message>
        <location filename="../src/bconnectionwizard.cpp" line="359"/>
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
