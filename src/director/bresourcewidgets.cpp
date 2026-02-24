/**
 * @file bresourcewidgets.cpp
 * @brief Implementation of specialized resource widgets
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2025
 */

#include "director/bresourcewidgets.h"
#include <QFileInfo>

// ============================================================================
// Director Resource Widget
// ============================================================================

BDirectorResourceWidget::BDirectorResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Director", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Address"), tr("Port"), tr("TLS")});
}

void BDirectorResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());

        // Try to get address from DirAddresses or simple Address
        QString address = res.simpleValue("address", "-");
        if (address == "-") {
            BConfigValue dirAddrs = res.value("diraddresses");
            if (dirAddrs.type() == BConfigValue::Block) {
                address = tr("(multiple)");
            }
        }
        item->setText(1, address);

        QString port = res.simpleValue("dirport", "9101");
        item->setText(2, port);

        QString tls = res.simpleValue("tls enable", "no");
        item->setText(3, tls.toLower() == "yes" ? tr("Yes") : tr("No"));

        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

void BDirectorResourceWidget::updateDetails(const BConfigResource &resource)
{
    QString text;
    text += QString("<h3>Director: %1</h3>").arg(resource.name());
    text += QString("<p><i>Source: %1</i></p>").arg(resource.sourceFile());
    text += "<hr>";

    // Important fields first
    text += "<table>";
    text += QString("<tr><td><b>Name:</b></td><td>%1</td></tr>").arg(resource.simpleValue("name"));
    text += QString("<tr><td><b>Port:</b></td><td>%1</td></tr>").arg(resource.simpleValue("dirport", "9101"));
    text += QString("<tr><td><b>Max Jobs:</b></td><td>%1</td></tr>").arg(resource.simpleValue("maximum concurrent jobs", "-"));
    text += QString("<tr><td><b>Messages:</b></td><td>%1</td></tr>").arg(resource.simpleValue("messages", "-"));
    text += "</table>";

    // TLS Section
    text += "<h4>TLS Configuration</h4><table>";
    text += QString("<tr><td><b>TLS Enable:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls enable", "no"));
    text += QString("<tr><td><b>TLS Require:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls require", "no"));
    text += QString("<tr><td><b>TLS Verify Peer:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls verify peer", "no"));
    text += QString("<tr><td><b>TLS CA Cert:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls ca certificate file", "-"));
    text += "</table>";

    // Password (masked)
    text += "<h4>Authentication</h4><table>";
    QString password = resource.simpleValue("password");
    text += QString("<tr><td><b>Password:</b></td><td>%1</td></tr>")
                .arg(password.isEmpty() ? "-" : "********");
    text += "</table>";

    // All other fields
    text += "<h4>All Directives</h4><pre>";
    for (const QString &key : resource.keys()) {
        BConfigValue value = resource.value(key);
        text += QString("<b>%1</b> = %2\n").arg(key, formatValue(value));
    }
    text += "</pre>";

    m_detailsEdit->setHtml(text);
}

QIcon BDirectorResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("network-server");
}

// ============================================================================
// Console Resource Widget
// ============================================================================

BConsoleResourceWidget::BConsoleResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Console", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("TLS"), tr("ACLs")});
}

void BConsoleResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());

        QString tls = res.simpleValue("tls enable", "no");
        item->setText(1, tls.toLower() == "yes" ? tr("Yes") : tr("No"));

        // Check for ACLs
        QStringList acls;
        if (res.hasKey("commandacl")) acls << "Cmd";
        if (res.hasKey("clientacl")) acls << "Client";
        if (res.hasKey("jobacl")) acls << "Job";
        if (res.hasKey("storageacl")) acls << "Storage";
        item->setText(2, acls.isEmpty() ? tr("None") : acls.join(", "));

        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

void BConsoleResourceWidget::updateDetails(const BConfigResource &resource)
{
    QString text;
    text += QString("<h3>Console: %1</h3>").arg(resource.name());
    text += QString("<p><i>Source: %1</i></p>").arg(resource.sourceFile());
    text += "<hr>";

    // TLS Section
    text += "<h4>TLS Configuration</h4><table>";
    text += QString("<tr><td><b>TLS Enable:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls enable", "no"));
    text += QString("<tr><td><b>TLS Require:</b></td><td>%1</td></tr>").arg(resource.simpleValue("tls require", "no"));
    text += "</table>";

    // Password (masked)
    text += "<h4>Authentication</h4><table>";
    QString password = resource.simpleValue("password");
    text += QString("<tr><td><b>Password:</b></td><td>%1</td></tr>")
                .arg(password.isEmpty() ? "-" : "********");
    text += "</table>";

    // ACLs
    text += "<h4>Access Control Lists</h4><table>";
    text += QString("<tr><td><b>Command ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("commandacl", "*all*"));
    text += QString("<tr><td><b>Client ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("clientacl", "-"));
    text += QString("<tr><td><b>Job ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("jobacl", "-"));
    text += QString("<tr><td><b>Storage ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("storageacl", "-"));
    text += QString("<tr><td><b>Schedule ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("scheduleacl", "-"));
    text += QString("<tr><td><b>Pool ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("poolacl", "-"));
    text += QString("<tr><td><b>FileSet ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("filesetacl", "-"));
    text += QString("<tr><td><b>Catalog ACL:</b></td><td>%1</td></tr>").arg(resource.simpleValue("catalogacl", "-"));
    text += "</table>";

    // All other fields
    text += "<h4>All Directives</h4><pre>";
    for (const QString &key : resource.keys()) {
        BConfigValue value = resource.value(key);
        text += QString("<b>%1</b> = %2\n").arg(key, formatValue(value));
    }
    text += "</pre>";

    m_detailsEdit->setHtml(text);
}

QIcon BConsoleResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("utilities-terminal");
}

// ============================================================================
// Client Resource Widget
// ============================================================================

BClientResourceWidget::BClientResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Client", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Address"), tr("Port"), tr("Catalog")});
}

void BClientResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, res.simpleValue("address", "-"));
        item->setText(2, res.simpleValue("fdport", "9102"));
        item->setText(3, res.simpleValue("catalog", "-"));
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

void BClientResourceWidget::updateDetails(const BConfigResource &resource)
{
    QString text;
    text += QString("<h3>Client: %1</h3>").arg(resource.name());
    text += QString("<p><i>Source: %1</i></p>").arg(resource.sourceFile());
    text += "<hr>";

    text += "<table>";
    text += QString("<tr><td><b>Address:</b></td><td>%1</td></tr>").arg(resource.simpleValue("address", "-"));
    text += QString("<tr><td><b>FD Port:</b></td><td>%1</td></tr>").arg(resource.simpleValue("fdport", "9102"));
    text += QString("<tr><td><b>Catalog:</b></td><td>%1</td></tr>").arg(resource.simpleValue("catalog", "-"));
    text += QString("<tr><td><b>File Retention:</b></td><td>%1</td></tr>").arg(resource.simpleValue("file retention", "-"));
    text += QString("<tr><td><b>Job Retention:</b></td><td>%1</td></tr>").arg(resource.simpleValue("job retention", "-"));
    text += QString("<tr><td><b>AutoPrune:</b></td><td>%1</td></tr>").arg(resource.simpleValue("autoprune", "-"));
    text += "</table>";

    text += "<h4>All Directives</h4><pre>";
    for (const QString &key : resource.keys()) {
        text += QString("<b>%1</b> = %2\n").arg(key, formatValue(resource.value(key)));
    }
    text += "</pre>";

    m_detailsEdit->setHtml(text);
}

QIcon BClientResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("computer");
}

// ============================================================================
// Job Resource Widget
// ============================================================================

BJobResourceWidget::BJobResourceWidget(const QString &resourceType, BDirector *director, QWidget *parent)
    : BResourceWidget(resourceType, director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Type"), tr("Level"), tr("Client"), tr("FileSet")});
}

void BJobResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, res.simpleValue("type", "-"));
        item->setText(2, res.simpleValue("level", "-"));
        item->setText(3, res.simpleValue("client", "-"));
        item->setText(4, res.simpleValue("fileset", "-"));
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

void BJobResourceWidget::updateDetails(const BConfigResource &resource)
{
    QString text;
    text += QString("<h3>%1: %2</h3>").arg(m_resourceType, resource.name());
    text += QString("<p><i>Source: %1</i></p>").arg(resource.sourceFile());
    text += "<hr>";

    text += "<table>";
    text += QString("<tr><td><b>Type:</b></td><td>%1</td></tr>").arg(resource.simpleValue("type", "-"));
    text += QString("<tr><td><b>Level:</b></td><td>%1</td></tr>").arg(resource.simpleValue("level", "-"));
    text += QString("<tr><td><b>Client:</b></td><td>%1</td></tr>").arg(resource.simpleValue("client", "-"));
    text += QString("<tr><td><b>FileSet:</b></td><td>%1</td></tr>").arg(resource.simpleValue("fileset", "-"));
    text += QString("<tr><td><b>Storage:</b></td><td>%1</td></tr>").arg(resource.simpleValue("storage", "-"));
    text += QString("<tr><td><b>Pool:</b></td><td>%1</td></tr>").arg(resource.simpleValue("pool", "-"));
    text += QString("<tr><td><b>Schedule:</b></td><td>%1</td></tr>").arg(resource.simpleValue("schedule", "-"));
    text += QString("<tr><td><b>Messages:</b></td><td>%1</td></tr>").arg(resource.simpleValue("messages", "-"));
    text += "</table>";

    text += "<h4>All Directives</h4><pre>";
    for (const QString &key : resource.keys()) {
        text += QString("<b>%1</b> = %2\n").arg(key, formatValue(resource.value(key)));
    }
    text += "</pre>";

    m_detailsEdit->setHtml(text);
}

QIcon BJobResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("media-playback-start");
}

// ============================================================================
// Storage Resource Widget
// ============================================================================

BStorageResourceWidget::BStorageResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Storage", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Address"), tr("Port"), tr("Device")});
}

void BStorageResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, res.simpleValue("address", "-"));
        item->setText(2, res.simpleValue("sdport", "9103"));
        item->setText(3, res.simpleValue("device", "-"));
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

QIcon BStorageResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("drive-harddisk");
}

// ============================================================================
// FileSet Resource Widget
// ============================================================================

BFileSetResourceWidget::BFileSetResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("FileSet", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Source File")});
}

void BFileSetResourceWidget::populateTree()
{
    BResourceWidget::populateTree();
}

QIcon BFileSetResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("folder-documents");
}

// ============================================================================
// Pool Resource Widget
// ============================================================================

BPoolResourceWidget::BPoolResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Pool", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Pool Type"), tr("Recycle")});
}

void BPoolResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, res.simpleValue("pool type", "-"));
        item->setText(2, res.simpleValue("recycle", "-"));
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

QIcon BPoolResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("media-optical");
}

// ============================================================================
// Schedule Resource Widget
// ============================================================================

BScheduleResourceWidget::BScheduleResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Schedule", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Source File")});
}

void BScheduleResourceWidget::populateTree()
{
    BResourceWidget::populateTree();
}

QIcon BScheduleResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("x-office-calendar");
}

// ============================================================================
// Messages Resource Widget
// ============================================================================

BMessagesResourceWidget::BMessagesResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Messages", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("Source File")});
}

void BMessagesResourceWidget::populateTree()
{
    BResourceWidget::populateTree();
}

QIcon BMessagesResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("mail-message");
}

// ============================================================================
// Catalog Resource Widget
// ============================================================================

BCatalogResourceWidget::BCatalogResourceWidget(BDirector *director, QWidget *parent)
    : BResourceWidget("Catalog", director, parent)
{
    m_treeWidget->setHeaderLabels({tr("Name"), tr("DB Driver"), tr("DB Name")});
}

void BCatalogResourceWidget::populateTree()
{
    m_treeWidget->clear();

    for (int i = 0; i < m_resources.size(); ++i) {
        const BConfigResource &res = m_resources.at(i);
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, res.name());
        item->setText(1, res.simpleValue("db driver", "-"));
        item->setText(2, res.simpleValue("db name", "-"));
        item->setData(0, Qt::UserRole, i);
        item->setIcon(0, resourceIcon());
    }

    if (m_treeWidget->topLevelItemCount() > 0) {
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));
    }
}

QIcon BCatalogResourceWidget::resourceIcon() const
{
    return QIcon::fromTheme("server-database");
}
