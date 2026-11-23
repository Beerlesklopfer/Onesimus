// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjsonreassembler.h
 * @brief Reassembles a JSON reply from the BNET telegrams it arrives in
 *
 * A Director splits a long reply into telegrams at arbitrary byte positions
 * (Bareos and odir cut at their packet size, not at characters). The bytes
 * are therefore collected as they came and decoded once, when the reply is
 * complete: a UTF-8 character split across two telegrams stays whole, and
 * whitespace at a telegram boundary (inside a file name, a log line) is kept.
 *
 * Completeness is found by one pass over the bytes that tracks the nesting
 * of objects and arrays outside strings. Every byte is looked at once, so a
 * reply of n bytes costs O(n) however many telegrams it comes in. The bytes
 * of a multi-byte UTF-8 character are all >= 0x80 and can never be taken for
 * a brace, a bracket, a quote or a backslash.
 *
 * The scanner only finds where a document ends; whether it is valid JSON is
 * decided by whoever parses it.
 */
#ifndef BJSONREASSEMBLER_H
#define BJSONREASSEMBLER_H

#include <QByteArray>

class BJsonReassembler
{
public:
    /// True when the first byte of @p data that is not whitespace opens a
    /// JSON object or array
    static bool startsJson(const QByteArray &data);

    /// Adds the payload of the next telegram
    void append(const QByteArray &data);

    /// True when nothing is pending. Whitespace between documents is dropped
    /// by takeDocument(), so a buffer of whitespace only counts as empty
    /// after the next call of takeDocument().
    bool isEmpty() const { return m_buffer.isEmpty(); }

    /// Bytes buffered (for limits and diagnostics)
    qsizetype size() const { return m_buffer.size(); }

    /**
     * @brief Takes the next complete top-level document
     *
     * Whitespace around the document is dropped, nothing inside it.
     * @return false while the document is still incomplete, or when the
     *         buffer does not start with a document (see takeAll())
     */
    bool takeDocument(QByteArray *document);

    /**
     * @brief Takes everything buffered, whitespace at both ends dropped
     *
     * For the end of a reply announced by a BNET signal: what was collected
     * belongs to that reply, complete or not.
     */
    QByteArray takeAll();

    /// Forgets everything (new session, connection lost)
    void clear();

    /// Bytes looked at by takeDocument() since construction. Each byte is
    /// looked at once, so this never exceeds the bytes appended (the test
    /// checks the linear cost with it).
    qint64 bytesScanned() const { return m_bytesScanned; }

private:
    void resetScan();

    QByteArray m_buffer;
    qsizetype m_scanned = 0;     ///< bytes of m_buffer already looked at
    qsizetype m_start = -1;      ///< offset of the open document, -1 if none
    int m_depth = 0;             ///< nesting depth inside the open document
    bool m_inString = false;
    bool m_escaped = false;
    bool m_notJson = false;      ///< the buffer starts with something else
    qint64 m_bytesScanned = 0;
};

#endif // BJSONREASSEMBLER_H
