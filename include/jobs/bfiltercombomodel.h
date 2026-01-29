#ifndef BFILTERCOMBOMODEL_H
#define BFILTERCOMBOMODEL_H

#include <QObject>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>

/**
 * @brief Model class for filter ComboBox data
 *
 * Extracts unique values from job JSON data for use in filter ComboBoxes.
 * Provides lists of unique job names and client names.
 *
 * @since 1.0
 */
class BFilterComboModel : public QObject
{
    Q_OBJECT

public:
    explicit BFilterComboModel(QObject *parent = nullptr);

    /**
     * @brief Updates the model with new job data
     * @param jobsArray JSON array containing job objects
     */
    void updateFromJobsArray(const QJsonArray &jobsArray);

    /**
     * @brief Updates job names from .jobs dot-command response
     * @param dotJobsResponse JSON response from .jobs command
     * @since 2.5
     */
    void updateJobNamesFromDotCommand(const QString &dotJobsResponse);

    /**
     * @brief Updates client names from .clients dot-command response
     * @param dotClientsResponse JSON response from .clients command
     * @since 2.5
     */
    void updateClientNamesFromDotCommand(const QString &dotClientsResponse);

    /**
     * @brief Returns list of unique job names
     * @return QStringList of job names, sorted alphabetically
     */
    QStringList jobNames() const { return m_jobNames; }

    /**
     * @brief Returns list of unique client names
     * @return QStringList of client names, sorted alphabetically
     */
    QStringList clientNames() const { return m_clientNames; }

    /**
     * @brief Clears all data from the model
     */
    void clear();

signals:
    /**
     * @brief Emitted when the model data has been updated
     */
    void dataUpdated();

private:
    QStringList m_jobNames;      ///< Unique job names
    QStringList m_clientNames;   ///< Unique client names
};

#endif // BFILTERCOMBOMODEL_H
