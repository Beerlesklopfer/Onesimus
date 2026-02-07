#include "btranslations.h"
#include "blogging.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>

BTranslations* BTranslations::s_instance = nullptr;

BTranslations::BTranslations(QObject *parent)
    : QObject(parent)
    , m_translator(new QTranslator(this))
    , m_currentLanguage(English)  // Default to English
{
    // Load system locale by default
    setLanguage(English);
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

    QString languageFile = QString("onesimus_%1").arg(languageCode(language));

    // Try to load translation file from resources or translations directory
    if (m_translator->load(languageFile, ":/translations") ||
        m_translator->load(languageFile, QCoreApplication::applicationDirPath() + "/translations")) {
        QCoreApplication::installTranslator(m_translator);
        BLOG_DEBUG() << "Loaded translation:" << languageFile;
    } else {
        BLOG_DEBUG() << "Translation file not found:" << languageFile;
    }

    // Set system locale based on language
    QLocale locale;
    switch (language) {
        case English:
            locale = QLocale(QLocale::English, QLocale::UnitedStates);
            break;
        case German:
            locale = QLocale(QLocale::German, QLocale::Germany);
            break;
        case Spanish:
            locale = QLocale(QLocale::Spanish, QLocale::Spain);
            break;
        case French:
            locale = QLocale(QLocale::French, QLocale::France);
            break;
        case Italian:
            locale = QLocale(QLocale::Italian, QLocale::Italy);
            break;
        case Russian:
            locale = QLocale(QLocale::Russian, QLocale::Russia);
            break;
        default:
            locale = QLocale::system();
    }
    QLocale::setDefault(locale);
    BLOG_DEBUG() << "Set default locale to:" << locale.name();

    m_currentLanguage = language;
    emit languageChanged(language);
}

QString BTranslations::languageName(Language language)
{
    switch (language) {
        case English: return "English";
        case German: return "Deutsch";
        case Spanish: return "Español";
        case French: return "Français";
        case Italian: return "Italiano";
        case Russian: return "Русский";
        default: return "Unknown";
    }
}

QString BTranslations::languageCode(Language language)
{
    switch (language) {
        case English: return "en";
        case German: return "de";
        case Spanish: return "es";
        case French: return "fr";
        case Italian: return "it";
        case Russian: return "ru";
        default: return "en";
    }
}

QList<BTranslations::Language> BTranslations::availableLanguages()
{
    return {
        English,
        German,
        Spanish,
        French,
        Italian,
        Russian
    };
}

BTranslations::Language BTranslations::languageFromCode(const QString& code)
{
    QString lowerCode = code.toLower();
    if (lowerCode == "en") return English;
    if (lowerCode == "de") return German;
    if (lowerCode == "es") return Spanish;
    if (lowerCode == "fr") return French;
    if (lowerCode == "it") return Italian;
    if (lowerCode == "ru") return Russian;
    return English;  // Default fallback
}

QString BTranslations::languageFlag(Language language)
{
    switch (language) {
        case English: return "🇬🇧";  // UK flag
        case German: return "🇩🇪";   // Germany flag
        case Spanish: return "🇪🇸";  // Spain flag
        case French: return "🇫🇷";   // France flag
        case Italian: return "🇮🇹";  // Italy flag
        case Russian: return "🇷🇺";  // Russia flag
        default: return "🌐";        // Globe icon as fallback
    }
}

BTranslations::Language BTranslations::detectSystemLanguage()
{
    QLocale systemLocale = QLocale::system();
    QLocale::Language sysLang = systemLocale.language();

    BLOG_DEBUG() << "Detecting system language:" << systemLocale.name()
             << "Language:" << QLocale::languageToString(sysLang);

    // Map system language to our supported languages
    switch (sysLang) {
        case QLocale::German:
            return German;
        case QLocale::Spanish:
            return Spanish;
        case QLocale::French:
            return French;
        case QLocale::Italian:
            return Italian;
        case QLocale::Russian:
            return Russian;
        case QLocale::English:
            return English;
        default:
            // For unsupported languages, default to English
            BLOG_DEBUG() << "System language not directly supported, defaulting to English";
            return English;
    }
}
