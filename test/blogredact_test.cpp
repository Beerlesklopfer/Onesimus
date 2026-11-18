/**
 * @file blogredact_test.cpp
 * @brief Secrets stay out of the log: password redaction and log file permissions
 *
 * Runs without a Director (registered with CTest). The target is built with
 * ONESIMUS_FILE_LOGGING, so the real file logger is exercised.
 */

#include <QtTest>
#include <QTemporaryDir>

#include "blogging.h"
#include "blogredact.h"
#include "bprivatefile.h"

class BLogRedactTest : public QObject
{
    Q_OBJECT

private slots:
    void redact_data();
    void redact();
    void openOwnerOnlyCreatesPrivateFile();
    void openOwnerOnlyTightensExistingFile();
    void logFileIsPrivate();

private:
    static const QFileDevice::Permissions OwnerOnly;
    static const QFileDevice::Permissions GroupAndOthers;
};

const QFileDevice::Permissions BLogRedactTest::OwnerOnly =
    QFileDevice::ReadOwner | QFileDevice::WriteOwner;
const QFileDevice::Permissions BLogRedactTest::GroupAndOthers =
    QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
    | QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther;

void BLogRedactTest::redact_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<QString>("expected");

    QTest::newRow("quoted")
        << R"(configure add console name=admin password="s3cret" profile=operator tlsenable=false)"
        << R"(configure add console name=admin password=*** profile=operator tlsenable=false)";
    QTest::newRow("quoted with spaces")
        << R"(add console name=a password="two words here" profile=p)"
        << R"(add console name=a password=*** profile=p)";
    QTest::newRow("quoted with escaped quote")
        << R"(add console password="a\"b c" profile=p)"
        << R"(add console password=*** profile=p)";
    QTest::newRow("unquoted")
        << "add client name=fd1 address=10.0.0.1 password=abc123 passive=yes"
        << "add client name=fd1 address=10.0.0.1 password=*** passive=yes";
    QTest::newRow("unquoted at the end")
        << "add client name=fd1 password=abc123"
        << "add client name=fd1 password=***";
    QTest::newRow("upper case")
        << R"(add console PASSWORD="s3cret")"
        << "add console PASSWORD=***";
    QTest::newRow("mixed case")
        << "add console Password=s3cret name=x"
        << "add console Password=*** name=x";
    QTest::newRow("md5 form")
        << "add console password=[md5]0123456789abcdef0123456789abcdef name=x"
        << "add console password=*** name=x";
    QTest::newRow("spaces around equals (resource text)")
        << R"(Password = "s3cret")"
        << "Password = ***";
    QTest::newRow("missing closing quote masks the rest")
        << R"(add console password="s3cret profile=p)"
        << "add console password=***";
    QTest::newRow("quote inside unquoted value")
        << R"(add console password=ab"cd ef" name=x)"
        << "add console password=*** name=x";
    QTest::newRow("two passwords")
        << R"(password=one name=x password="two")"
        << "password=*** name=x password=***";
    QTest::newRow("key ending in password")
        << "set tlspskpassword=abc"
        << "set tlspskpassword=***";
    QTest::newRow("empty value at the end")
        << "add console password="
        << "add console password=***";
    QTest::newRow("empty quoted value")
        << R"(add console password="" name=x)"
        << "add console password=*** name=x";
    QTest::newRow("no password")
        << "list jobs client=fd1 limit=50"
        << "list jobs client=fd1 limit=50";
    QTest::newRow("the word alone is not a value")
        << "show console password"
        << "show console password";
    QTest::newRow("empty line") << "" << "";
}

void BLogRedactTest::redact()
{
    QFETCH(QString, line);
    QFETCH(QString, expected);

    const QString redacted = bRedactCommand(line);
    QCOMPARE(redacted, expected);
    QVERIFY(!redacted.contains("s3cret"));
}

void BLogRedactTest::openOwnerOnlyCreatesPrivateFile()
{
#ifdef Q_OS_UNIX
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    QFile file(dir.filePath("console.conf"));
    QVERIFY(bOpenOwnerOnly(file, QIODevice::WriteOnly | QIODevice::Text));
    file.write("Password = \"s3cret\"\n");
    file.close();

    const QFileDevice::Permissions permissions = QFile::permissions(file.fileName());
    QVERIFY(permissions.testFlag(QFileDevice::ReadOwner));
    QVERIFY(permissions.testFlag(QFileDevice::WriteOwner));
    QCOMPARE(permissions & GroupAndOthers, QFileDevice::Permissions());
#else
    QSKIP("POSIX permission bits only");
#endif
}

void BLogRedactTest::openOwnerOnlyTightensExistingFile()
{
#ifdef Q_OS_UNIX
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath("old.conf");

    {
        QFile old(path);
        QVERIFY(old.open(QIODevice::WriteOnly));
        old.write("old content\n");
    }
    QVERIFY(QFile::setPermissions(path, OwnerOnly | QFileDevice::ReadGroup | QFileDevice::ReadOther));

    QFile file(path);
    QVERIFY(bOpenOwnerOnly(file, QIODevice::WriteOnly | QIODevice::Append));
    file.close();

    QCOMPARE(QFile::permissions(path) & GroupAndOthers, QFileDevice::Permissions());
#else
    QSKIP("POSIX permission bits only");
#endif
}

void BLogRedactTest::logFileIsPrivate()
{
#ifdef Q_OS_UNIX
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath("onesimus.log");

    // A log and a rotated log left behind by an older version, world-readable
    for (const QString &name : {path, path + ".1"}) {
        QFile old(name);
        QVERIFY(old.open(QIODevice::WriteOnly));
        old.write("old log line\n");
        old.close();
        QVERIFY(QFile::setPermissions(name, OwnerOnly | QFileDevice::ReadGroup | QFileDevice::ReadOther));
    }

    BLOG_INIT(path, 10);
    BLOG_SET_ENABLED(true);
    BLOG_INFO() << "Sending:" << bRedactCommand(R"(add console password="s3cret")");
    BLOG_CLOSE();

    QCOMPARE(QFile::permissions(path) & GroupAndOthers, QFileDevice::Permissions());
    QCOMPARE(QFile::permissions(path + ".1") & GroupAndOthers, QFileDevice::Permissions());

    QFile log(path);
    QVERIFY(log.open(QIODevice::ReadOnly));
    const QByteArray content = log.readAll();
    QVERIFY(content.contains("old log line"));          // appended, not truncated
    QVERIFY(content.contains("password=***"));
    QVERIFY(!content.contains("s3cret"));
#else
    QSKIP("POSIX permission bits only");
#endif
}

QTEST_GUILESS_MAIN(BLogRedactTest)
#include "blogredact_test.moc"
