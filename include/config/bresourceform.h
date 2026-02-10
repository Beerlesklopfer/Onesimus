#ifndef BRESOURCEFORM_H
#define BRESOURCEFORM_H

#include <QWidget>
#include <QScrollArea>
#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QList>
#include <QSet>
#include "config/bconfigparser.h"
#include "config/bdirectiveschema.h"

/**
 * @file bresourceform.h
 * @brief Reusable schema-driven form widget for Bareos/Bacula resources
 *
 * Generates form fields dynamically from JSON directive schemas.
 * Can be embedded in dialogs, wizard pages, or any other container.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */
class BResourceForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construct a resource form widget
     * @param resourceType The resource type (e.g., "Client", "Job", "Director")
     * @param parent Parent widget
     */
    explicit BResourceForm(const QString &resourceType, QWidget *parent = nullptr);

    /**
     * @brief Set filter to show only directives from specific groups
     * @param groups List of group names to show (empty = show all)
     */
    void setGroupFilter(const QStringList &groups);

    /**
     * @brief Set existing resource to pre-populate fields
     * @param existing Existing resource with values
     */
    void setExistingResource(const BConfigResource &existing);

    /**
     * @brief Get the edited resource with collected values
     * @return BConfigResource with values from the form
     */
    BConfigResource resource() const;

    /**
     * @brief Collect values from form widgets into the internal resource
     * Call this before resource() to ensure values are up-to-date
     */
    void collectValues();

    /**
     * @brief Check if the form has advanced directives
     * @return true if advanced directives exist
     */
    bool hasAdvancedDirectives() const { return m_hasAdvancedDirectives; }

    /**
     * @brief Set visibility of advanced directives
     * @param visible true to show advanced directives
     */
    void setAdvancedVisible(bool visible);

    /**
     * @brief Get the value of a specific directive
     * @param name Directive name
     * @return Current value from the form widget
     */
    QVariant directiveValue(const QString &name) const;

    /**
     * @brief Set the value of a specific directive
     * @param name Directive name
     * @param value Value to set
     */
    void setDirectiveValue(const QString &name, const QVariant &value);

    /**
     * @brief Set directives to exclude from the form (already shown elsewhere)
     * @param names List of directive names to skip during buildForm()
     */
    void setExcludedDirectives(const QStringList &names);

    /**
     * @brief Populate resource_reference combo boxes with available names
     * @param referenceData Map of reference type (e.g., "Client", "Pool") to list of names
     */
    void setReferenceData(const QMap<QString, QStringList> &referenceData);

signals:
    /**
     * @brief Emitted when any field value changes
     */
    void valueChanged();

    /**
     * @brief Emitted when a specific directive's value changes
     * @param name Directive name
     * @param value New value
     */
    void directiveValueChanged(const QString &name, const QVariant &value);

private:
    void buildForm();
    QWidget *createWidgetForDirective(const BDirective &directive,
                                      const BConfigValue &currentValue);
    void setupConditionalVisibility();
    void updateDependentFields(const QString &controllingDirective);
    void connectFieldSignals(const QString &name, QWidget *widget);
    static QString blockValueToText(const QMap<QString, BConfigValue> &block, int indent = 0);

    QString m_resourceType;
    QStringList m_groupFilter;
    QStringList m_excludedDirectives;
    BConfigResource m_resource;
    BConfigResource m_existing;
    QMap<QString, QWidget*> m_fieldWidgets;
    QMap<QString, QLabel*> m_fieldLabels;
    QMap<QString, BDirective> m_fieldDirectives;
    QList<QWidget*> m_advancedWidgets;
    QList<QLabel*> m_advancedLabels;
    QScrollArea *m_scrollArea = nullptr;
    bool m_hasAdvancedDirectives = false;
};

#endif // BRESOURCEFORM_H
