#ifndef BRESOURCEDIALOG_H
#define BRESOURCEDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include "config/bconfigparser.h"

// Forward declaration
class BResourceForm;

/**
 * @file bresourcedialog.h
 * @brief Dynamic schema-driven dialog for editing Bareos/Bacula resources
 *
 * Uses BResourceForm internally to generate form fields dynamically
 * from JSON directive schemas.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
class BResourceDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Construct a resource editing dialog
     * @param resourceType The resource type (e.g., "Client", "Job", "Director")
     * @param existing Existing resource to pre-populate fields (optional)
     * @param parent Parent widget
     */
    explicit BResourceDialog(const QString &resourceType,
                             const BConfigResource &existing = BConfigResource(),
                             QWidget *parent = nullptr);

    /**
     * @brief Construct with resource selector combo at the top
     * @param resourceType The resource type (e.g., "Job", "Client")
     * @param names List of resource names for the selector combo
     * @param parent Parent widget
     */
    explicit BResourceDialog(const QString &resourceType,
                             const QStringList &names,
                             QWidget *parent = nullptr);

    /**
     * @brief Get the edited resource with collected values
     * @return BConfigResource with values from the form
     */
    BConfigResource resource() const;

    /**
     * @brief Populate resource_reference combo boxes with available names
     * @param referenceData Map of reference type to list of names
     */
    void setReferenceData(const QMap<QString, QStringList> &referenceData);

private slots:
    void onShowAdvanced(bool checked);
    void onAccepted();
    void onSaveConf();
    void onSaveZip();
    void onResourceSelected(int index);

private:
    void buildUi(const BConfigResource &existing);

    QString m_resourceType;
    BResourceForm *m_form;
    QComboBox *m_selectorCombo = nullptr;
    QCheckBox *m_advancedToggle;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_saveConfButton;
    QPushButton *m_saveZipButton;
};

#endif // BRESOURCEDIALOG_H
