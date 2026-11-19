#include "bheaderstate.h"

#include "buiscale.h"
#include "config/bcolumnconfiguration.h"

#include <QHeaderView>
#include <QSignalBlocker>

namespace BHeaderState {

int currentScale()
{
    return BUiScale::px(1000);
}

void makeInteractive(QHeaderView *header)
{
    if (!header) return;
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setStretchLastSection(true);
}

void scaleSections(QHeaderView *header, int fromScale, int toScale)
{
    if (!header || fromScale <= 0 || toScale <= 0 || fromScale == toScale) return;
    const QSignalBlocker blocker(header);
    for (int i = 0; i < header->count(); ++i) {
        const int scaled = int((qint64(header->sectionSize(i)) * toScale + fromScale / 2) / fromScale);
        header->resizeSection(i, qMax(header->minimumSectionSize(), scaled));
    }
}

bool restore(QHeaderView *header, const QByteArray &state, int savedScale)
{
    if (!header || state.isEmpty() || !header->restoreState(state)) return false;
    // restoreState() also brings back the resize modes of the time it was
    // saved; every column is resizable now
    makeInteractive(header);
    scaleSections(header, savedScale, currentScale());
    return true;
}

Source restoreOrMigrate(QHeaderView *header, const QByteArray &state, int savedScale,
                        const QString &legacyKey)
{
    if (restore(header, state, savedScale)) return Source::Saved;
    if (!header || legacyKey.isEmpty()) return Source::Nothing;

    BColumnConfiguration legacy;
    if (!legacy.hasHeaderState(legacyKey)) return Source::Nothing;
    legacy.restoreHeaderState(header, legacyKey);
    legacy.removeHeaderState(legacyKey);
    makeInteractive(header);
    return Source::Legacy;
}

} // namespace BHeaderState
