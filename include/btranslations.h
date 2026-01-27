#ifndef BTRANSLATIONS_H
#define BTRANSLATIONS_H

#include <QObject>
#include <QTranslator>
#include <QCoreApplication>

/**
 * @brief Manages application translations and localization
 * @version 1.0
 * @since 2026-01-26
 * 
 * Supports multiple languages through Qt's translation system.
 * Currently supported: English (en), German (de)
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
        German      ///< German (de)
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
