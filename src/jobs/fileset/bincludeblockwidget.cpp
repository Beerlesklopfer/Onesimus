/**
 * @file bincludeblockwidget.cpp
 * @brief Widget for editing a single FileSet Include block
 *
 * @author Joerg Bernau <Joerg@bernau.family>
 * @date 2026
 */

#include "jobs/fileset/bincludeblockwidget.h"
#include "jobs/fileset/bfilesetdocument.h"
#include "config/beditablelistwidget.h"
#include "config/beditablelistmodel.h"
#include "config/bincludeoptionsform.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

BIncludeBlockWidget::BIncludeBlockWidget(BFileSetDocument *document, int blockIndex,
                                         QWidget *parent)
    : QWidget(parent)
    , m_document(document)
    , m_blockIndex(blockIndex)
{
    setupUi();
    connectSignals();
    refresh();
}

void BIncludeBlockWidget::setBlockIndex(int index)
{
    m_blockIndex = index;
}

void BIncludeBlockWidget::refresh()
{
    BFileSetDocument::IncludeBlock *block = m_document->includeBlock(m_blockIndex);
    if (!block) {
        return;
    }

    loadOptionsFromBlock();
}

void BIncludeBlockWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Header with title and remove button
    QFrame *headerFrame = new QFrame;
    headerFrame->setFrameShape(QFrame::StyledPanel);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(8, 4, 8, 4);

    QLabel *titleLabel = new QLabel(tr("Include Block #%1").arg(m_blockIndex + 1));
    QFont font = titleLabel->font();
    font.setBold(true);
    titleLabel->setFont(font);
    headerLayout->addWidget(titleLabel);

    headerLayout->addStretch();

    QPushButton *removeBtn = new QPushButton(tr("Remove"));
    removeBtn->setIcon(QIcon::fromTheme("list-remove"));
    connect(removeBtn, &QPushButton::clicked, this, &BIncludeBlockWidget::onRemoveClicked);
    headerLayout->addWidget(removeBtn);

    mainLayout->addWidget(headerFrame);

    // Tab widget for Files, Options, Exclude
    m_tabWidget = new QTabWidget;

    createFilesTab();
    createOptionsTab();
    createExcludeTab();

    mainLayout->addWidget(m_tabWidget);
}

void BIncludeBlockWidget::createFilesTab()
{
    BFileSetDocument::IncludeBlock *block = m_document->includeBlock(m_blockIndex);
    if (!block) {
        return;
    }

    m_pathsWidget = new BEditableListWidget;
    m_pathsWidget->setModel(block->pathsModel);
    m_pathsWidget->setUndoStack(m_document->undoStack());
    m_pathsWidget->setAddButtonText(tr("Add Path..."));
    m_pathsWidget->setPlaceholderText(tr("Enter file or directory path"));
    m_pathsWidget->setBrowseMode(BEditableListDelegate::BrowseDirectory);

    m_tabWidget->addTab(m_pathsWidget, tr("Files"));
}

void BIncludeBlockWidget::createOptionsTab()
{
    // Use schema-driven form from fileset.json include_options
    m_optionsForm = new BIncludeOptionsForm(BIncludeOptionsForm::IncludeMode);
    m_tabWidget->addTab(m_optionsForm, tr("Options"));
}

void BIncludeBlockWidget::createExcludeTab()
{
    BFileSetDocument::IncludeBlock *block = m_document->includeBlock(m_blockIndex);
    if (!block) {
        return;
    }

    m_excludeWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(m_excludeWidget);

    // Exclude files
    QGroupBox *filesGroup = new QGroupBox(tr("Exclude Files"));
    QVBoxLayout *filesLayout = new QVBoxLayout(filesGroup);

    m_excludeFilesWidget = new BEditableListWidget;
    m_excludeFilesWidget->setModel(block->excludeFilesModel);
    m_excludeFilesWidget->setUndoStack(m_document->undoStack());
    m_excludeFilesWidget->setAddButtonText(tr("Add File..."));
    m_excludeFilesWidget->setPlaceholderText(tr("Enter file path to exclude"));
    filesLayout->addWidget(m_excludeFilesWidget);

    layout->addWidget(filesGroup);

    // Exclude patterns (WildDir) - directory patterns
    QGroupBox *wildDirGroup = new QGroupBox(tr("Directory Patterns (WildDir)"));
    QVBoxLayout *wildDirLayout = new QVBoxLayout(wildDirGroup);

    m_excludePatternsWidget = new BEditableListWidget;
    m_excludePatternsWidget->setModel(block->excludePatternsModel);
    m_excludePatternsWidget->setUndoStack(m_document->undoStack());
    m_excludePatternsWidget->setAddButtonText(tr("Add Pattern..."));
    m_excludePatternsWidget->setPlaceholderText(tr("e.g., */__pycache__"));
    wildDirLayout->addWidget(m_excludePatternsWidget);

    layout->addWidget(wildDirGroup);

    // Exclude patterns (WildFile) - file patterns
    QGroupBox *wildFileGroup = new QGroupBox(tr("File Patterns (WildFile)"));
    QVBoxLayout *wildFileLayout = new QVBoxLayout(wildFileGroup);

    m_excludeWildFileWidget = new BEditableListWidget;
    m_excludeWildFileWidget->setModel(block->excludeWildFileModel);
    m_excludeWildFileWidget->setUndoStack(m_document->undoStack());
    m_excludeWildFileWidget->setAddButtonText(tr("Add Pattern..."));
    m_excludeWildFileWidget->setPlaceholderText(tr("e.g., *.pyc, *.log"));
    wildFileLayout->addWidget(m_excludeWildFileWidget);

    layout->addWidget(wildFileGroup);

    m_tabWidget->addTab(m_excludeWidget, tr("Exclude"));
}

void BIncludeBlockWidget::connectSignals()
{
    // Connect options form value changes to save
    connect(m_optionsForm, &BIncludeOptionsForm::valueChanged,
            this, &BIncludeBlockWidget::onOptionChanged);
}

void BIncludeBlockWidget::loadOptionsFromBlock()
{
    BFileSetDocument::IncludeBlock *block = m_document->includeBlock(m_blockIndex);
    if (!block) {
        return;
    }

    // Load options from document into form
    m_optionsForm->setValues(block->options);
}

void BIncludeBlockWidget::saveOptionsToBlock()
{
    BFileSetDocument::IncludeBlock *block = m_document->includeBlock(m_blockIndex);
    if (!block) {
        return;
    }

    // Save all form values to document
    block->options = m_optionsForm->allValues();
}

void BIncludeBlockWidget::onRemoveClicked()
{
    emit removeRequested(m_blockIndex);
}

void BIncludeBlockWidget::onOptionChanged()
{
    saveOptionsToBlock();
}
