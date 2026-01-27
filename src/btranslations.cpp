#include "btranslations.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

BTranslations* BTranslations::s_instance = nullptr;

BTranslations::BTranslations(QObject *parent)
    : QObject(parent)
    , m_translator(new QTranslator(this))
    , m_currentLanguage(German)  // Default to German
{
    // Load system locale by default
    setLanguage(German);
}

BTranslations::~BTranslations()
{
}

BTranslations* BTranslations::instance()
{
    if (!s_instance) {
        s_instance = new BTranslations(qApp);
    }
    return s_instance;
}

void BTranslations::setLanguage(Language language)
{
    // Remove old translator
    QCoreApplication::removeTranslator(m_translator);
    
    QString languageFile = QString("bacula_%1").arg(languageCode(language));
    
    // Try to load translation file from resources or translations directory
    if (m_translator->load(languageFile, ":/translations") ||
        m_translator->load(languageFile, QCoreApplication::applicationDirPath() + "/translations")) {
        QCoreApplication::installTranslator(m_translator);
        qDebug() << "Loaded translation:" << languageFile;
    } else {
        qDebug() << "Translation file not found:" << languageFile;
    }
    
    // Set system locale based on language
    QLocale locale;
    switch (language) {
        case German:
            locale = QLocale(QLocale::German, QLocale::Germany);
            break;
        case English:
            locale = QLocale(QLocale::English, QLocale::UnitedStates);
            break;
        default:
            locale = QLocale::system();
    }
    QLocale::setDefault(locale);
    qDebug() << "Set default locale to:" << locale.name();
    
    m_currentLanguage = language;
    emit languageChanged(language);
}

QString BTranslations::languageName(Language language)
{
    switch (language) {
        case English: return "English";
        case German: return "Deutsch";
        default: return "Unknown";
    }
}

QString BTranslations::languageCode(Language language)
{
    switch (language) {
        case English: return "en";
        case German: return "de";
        default: return "en";
    }
}
