#ifndef BTRANSLATIONS_H
#define BTRANSLATIONS_H

#include <QObject>
#include <QTranslator>
#include <QCoreApplication>

/**
 * @brief Manages application translations and localization
 * @version 2.0
 * @since 2026-01-29
 *
 * Supports multiple languages through Qt's translation system.
 * Currently supported: English (en), German (de), Spanish (es),
 * French (fr), Italian (it), Russian (ru)
 */
class BTranslations : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Available languages
     */
    enum Language {
        English,    ///< English (en)
        German,     ///< German (de)
        Spanish,    ///< Spanish (es)
        French,     ///< French (fr)
        Italian,    ///< Italian (it)
        Russian     ///< Russian (ru)
    };
    Q_ENUM(Language)

    /**
     * @brief Gets the singleton instance
     * @return Pointer to BTranslations instance
     * @since 1.0
     */
    static BTranslations* instance();
    
    /**
     * @brief Sets the application language
     * @param language Language to set
     * @since 1.0
     */
    void setLanguage(Language language);
    
    /**
     * @brief Gets the current language
     * @return Current language
     * @since 1.0
     */
    Language currentLanguage() const { return m_currentLanguage; }
    
    /**
     * @brief Gets language name as string
     * @param language Language enum
     * @return Language name (e.g., "Deutsch", "English")
     * @since 1.0
     */
    static QString languageName(Language language);
    
    /**
     * @brief Gets language code
     * @param language Language enum
     * @return Language code (e.g., "de", "en")
     * @since 1.0
     */
    static QString languageCode(Language language);

    /**
     * @brief Gets list of all available languages
     * @return QList of all supported languages
     * @since 2.0
     */
    static QList<Language> availableLanguages();

    /**
     * @brief Converts language code to Language enum
     * @param code Language code (e.g., "de", "en", "es")
     * @return Corresponding Language enum value
     * @since 2.0
     */
    static Language languageFromCode(const QString& code);

    /**
     * @brief Gets flag emoji for a language
     * @param language Language enum
     * @return Unicode flag emoji string
     * @since 2.0
     */
    static QString languageFlag(Language language);

    /**
     * @brief Detects the best matching language from system locale
     * @return Language enum matching system locale, or English as fallback
     * @since 2.0
     */
    static Language detectSystemLanguage();

signals:
    /**
     * @brief Emitted when language changes
     * @param language New language
     * @since 1.0
     */
    void languageChanged(Language language);

private:
    explicit BTranslations(QObject *parent = nullptr);
    ~BTranslations();
    
    static BTranslations *s_instance;   ///< Singleton instance
    QTranslator *m_translator;          ///< Qt translator
    Language m_currentLanguage;         ///< Current language
};

/**
 * @brief Macro for translatable strings (wraps QObject::tr)
 */
#define TR(text) QObject::tr(text)

#endif // BTRANSLATIONS_H
