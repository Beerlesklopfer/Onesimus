#ifndef BINCLUDEOPTIONSFORM_H
#define BINCLUDEOPTIONSFORM_H

#include <QWidget>
#include <QScrollArea>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <QJsonObject>

class BEditableListWidget;

/**
 * @file bincludeoptionsform.h
 * @brief Schema-driven form for FileSet Include/Exclude Options
 *
 * Reads include_options from fileset.json and dynamically generates
 * widgets (checkboxes for booleans, comboboxes for strings with values).
 *
 * Can be used for both Include Options (full set) or Exclude block
 * (limited set of options).
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
class BIncludeOptionsForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Mode determines which options to show
     */
    enum Mode {
        IncludeMode,    ///< Full Include Options (signature, compression, etc.)
        ExcludeMode     ///< Exclude block options (limited subset)
    };
    Q_ENUM(Mode)

    /**
     * @brief Construct the options form
     * @param mode Include or Exclude mode
     * @param parent Parent widget
     */
    explicit BIncludeOptionsForm(Mode mode = IncludeMode, QWidget *parent = nullptr);

    /**
     * @brief Get current mode
     * @return Mode (Include or Exclude)
     */
    Mode mode() const { return m_mode; }

    /**
     * @brief Get value of a specific option
     * @param name Option name (e.g., "onefs", "compression")
     * @return Current value from the widget
     */
    QVariant optionValue(const QString &name) const;

    /**
     * @brief Set value of a specific option
     * @param name Option name
     * @param value Value to set
     */
    void setOptionValue(const QString &name, const QVariant &value);

    /**
     * @brief Get all option values as a map
     * @return Map of option names to values
     */
    QMap<QString, QVariant> allValues() const;

    /**
     * @brief Set multiple option values at once
     * @param values Map of option names to values
     */
    void setValues(const QMap<QString, QVariant> &values);

    /**
     * @brief Reset all options to their default values
     */
    void resetToDefaults();

    /**
     * @brief Load the include_options schema from fileset.json
     * @return JsonObject with include_options
     */
    static QJsonObject loadSchema();

    /**
     * @brief Generate Bareos configuration text for Options block
     *
     * Exports options in the format:
     *   Signature = SHA1
     *   Compression = LZ4
     *   OneFS = yes
     *   Exclude {
     *     File = /path
     *     WildDir = *.tmp
     *   }
     *
     * @param indent Indentation prefix (e.g., "      " for 6 spaces)
     * @param excludeFiles List of files to exclude (nested in Options)
     * @param excludePatterns List of WildDir patterns to exclude
     * @return Configuration text for Options directives
     */
    QString toConfigText(const QString &indent = "      ",
                         const QStringList &excludeFiles = QStringList(),
                         const QStringList &excludePatterns = QStringList()) const;

    /**
     * @brief Get the Bareos directive name for an option
     *
     * Converts internal option names (lowercase, no spaces) to
     * Bareos directive format (e.g., "onefs" -> "OneFS")
     *
     * @param optionName Internal option name
     * @return Bareos directive name
     */
    static QString directiveName(const QString &optionName);

signals:
    /**
     * @brief Emitted when any option value changes
     */
    void valueChanged();

    /**
     * @brief Emitted when a specific option changes
     * @param name Option name
     * @param value New value
     */
    void optionChanged(const QString &name, const QVariant &value);

private:
    void buildForm();
    QWidget *createWidgetForOption(const QString &name, const QJsonObject &optionDef);
    void connectWidgetSignals(const QString &name, QWidget *widget);

    Mode m_mode = IncludeMode;
    QScrollArea *m_scrollArea = nullptr;
    QMap<QString, QWidget*> m_widgets;
    QMap<QString, QJsonObject> m_optionDefs;
    static QJsonObject s_schema;

    bool shouldShowOption(const QString &name, const QJsonObject &def) const;
};

#endif // BINCLUDEOPTIONSFORM_H
