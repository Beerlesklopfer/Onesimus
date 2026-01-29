#ifndef BJOBLOGMODEL_H
#define BJOBLOGMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QFont>

/**
 * @brief Model for displaying job log entries
 * @version 1.0
 * @since 2026-01-28
 *
 * Parses JSON-RPC responses from Bareos Director and extracts log entries.
 * Supports multiple JSON response structures and falls back to plain text.
 */
class BJobLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BJobLogModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Parses JSON-RPC response and populates log entries
     * @param jsonResponse Raw JSON response from Director
     * @return True if parsing succeeded (or plain text fallback), false on error
     * @since 1.0
     */
    bool parseJsonResponse(const QString &jsonResponse);

    /**
     * @brief Sets log entries from a string list
     * @param logLines List of log lines to display
     * @since 1.0
     */
    void setLogLines(const QStringList &logLines);

    /**
     * @brief Clears all log entries
     * @since 1.0
     */
    void clear();

    /**
     * @brief Returns all log entries as string list
     * @return QStringList of log lines
     * @since 1.0
     */
    QStringList logLines() const { return m_logLines; }

private:
    QStringList m_logLines;     ///< List of log entries
    QFont m_logFont;            ///< Font for log display (Courier, 9pt)
};

#endif // BJOBLOGMODEL_H
