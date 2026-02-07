#ifndef BRESOURCEWIDGETS_H
#define BRESOURCEWIDGETS_H

#include "director/bresourcewidget.h"

/**
 * @file bresourcewidgets.h
 * @brief Specialized resource widgets for each Bareos/Bacula resource type
 *
 * Each widget provides type-specific columns and details display.
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2025
 */

// ============================================================================
// Director Resource Widget
// ============================================================================
class BDirectorResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BDirectorResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    void updateDetails(const BConfigResource &resource) override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Console Resource Widget
// ============================================================================
class BConsoleResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BConsoleResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    void updateDetails(const BConfigResource &resource) override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Client Resource Widget
// ============================================================================
class BClientResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BClientResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    void updateDetails(const BConfigResource &resource) override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Job Resource Widget
// ============================================================================
class BJobResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BJobResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    void updateDetails(const BConfigResource &resource) override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Storage Resource Widget
// ============================================================================
class BStorageResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BStorageResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// FileSet Resource Widget
// ============================================================================
class BFileSetResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BFileSetResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Pool Resource Widget
// ============================================================================
class BPoolResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BPoolResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Schedule Resource Widget
// ============================================================================
class BScheduleResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BScheduleResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Messages Resource Widget
// ============================================================================
class BMessagesResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BMessagesResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

// ============================================================================
// Catalog Resource Widget
// ============================================================================
class BCatalogResourceWidget : public BResourceWidget
{
    Q_OBJECT
public:
    explicit BCatalogResourceWidget(BDirector *director = nullptr, QWidget *parent = nullptr);

protected:
    void populateTree() override;
    QIcon resourceIcon() const override;
};

#endif // BRESOURCEWIDGETS_H
