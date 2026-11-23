// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bjsonreassembler.cpp
 * @brief Reassembly of a JSON reply from BNET telegrams (see the header)
 */

#include "director/bjsonreassembler.h"

namespace {

bool isJsonSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

} // namespace

bool BJsonReassembler::startsJson(const QByteArray &data)
{
    for (const char c : data) {
        if (isJsonSpace(c)) {
            continue;
        }
        return c == '{' || c == '[';
    }
    return false;
}

void BJsonReassembler::append(const QByteArray &data)
{
    m_buffer.append(data);
}

bool BJsonReassembler::takeDocument(QByteArray *document)
{
    if (m_notJson) {
        return false;
    }

    const char *data = m_buffer.constData();
    const qsizetype size = m_buffer.size();

    for (qsizetype i = m_scanned; i < size; ++i) {
        const char c = data[i];
        ++m_bytesScanned;

        if (m_start < 0) {
            // Between documents: whitespace, then an opening brace or bracket
            if (isJsonSpace(c)) {
                continue;
            }
            if (c != '{' && c != '[') {
                // Not a document; the caller takes it with takeAll() when
                // the reply ends
                m_notJson = true;
                m_scanned = i;
                return false;
            }
            m_start = i;
            m_depth = 1;
            continue;
        }

        if (m_inString) {
            if (m_escaped) {
                m_escaped = false;
            } else if (c == '\\') {
                m_escaped = true;
            } else if (c == '"') {
                m_inString = false;
            }
            continue;
        }

        switch (c) {
        case '"':
            m_inString = true;
            break;
        case '{':
        case '[':
            ++m_depth;
            break;
        case '}':
        case ']':
            if (--m_depth == 0) {
                if (document) {
                    *document = m_buffer.mid(m_start, i + 1 - m_start);
                }
                // What follows stays for the next call, which also drops
                // it if it is only whitespace
                m_buffer.remove(0, i + 1);
                resetScan();
                return true;
            }
            break;
        default:
            break;
        }
    }

    if (m_start < 0) {
        // Only whitespace so far: nothing is pending
        m_buffer.clear();
        m_scanned = 0;
        return false;
    }
    m_scanned = size;
    return false;
}

QByteArray BJsonReassembler::takeAll()
{
    QByteArray all = m_buffer.trimmed();
    clear();
    return all;
}

void BJsonReassembler::clear()
{
    m_buffer.clear();
    resetScan();
}

void BJsonReassembler::resetScan()
{
    m_scanned = 0;
    m_start = -1;
    m_depth = 0;
    m_inString = false;
    m_escaped = false;
    m_notJson = false;
}
