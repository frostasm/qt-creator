/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef DEBUGGER_WATCHDATA_H
#define DEBUGGER_WATCHDATA_H

#include "debuggerprotocol.h"

#include <utils/treemodel.h>

#include <QCoreApplication>
#include <QMetaType>

#include <vector>

namespace Debugger {
namespace Internal {

class GdbMi;

class WatchItem : public Utils::TreeItem
{
public:
    WatchItem();

    void parse(const GdbMi &input);

    bool isLocal()   const;
    bool isWatcher() const;
    bool isInspect() const;

    QString expression() const;
    QString realName() const;
    quint64 realAddress() const;
    QByteArray internalName() const;
    QString toToolTip() const;

    QVariant editValue() const;
    int editType() const;

    WatchItem *findItem(const QByteArray &iname);
    WatchItem *parentItem() const;

    enum State
    {
        ValueNeeded       = 2,
        ChildrenNeeded    = 8,

        InitialState = ValueNeeded | ChildrenNeeded
    };

    static const qint64 InvalidId = -1;

    void setAllUnneeded()          { state = State(0); }

    bool isValueNeeded() const { return state & ValueNeeded; }
    void setValueNeeded()      { state = State(state | ValueNeeded); }
    void setValueUnneeded()    { state = State(state & ~ValueNeeded); }

    bool isChildrenNeeded() const { return state & ChildrenNeeded; }
    void setChildrenNeeded()   { state = State(state | ChildrenNeeded); }
    void setChildrenUnneeded() { state = State(state & ~ChildrenNeeded); }
    void setHasChildren(bool c)   { wantsChildren = c;  if (!c) setChildrenUnneeded(); }

    bool isValid()   const { return !iname.isEmpty(); }
    bool isVTablePointer() const;

    void setError(const QString &);
    void setValue(const QString &);
    void setType(const QByteArray &, bool guessChildrenFromType = true);

    QString toString()  const;

    static QString msgNotInScope();
    static QString shadowedName(const QString &name, int seen);
    static const QString &shadowedNameFormat();

    QByteArray hexAddress()  const;

public:
    qint64          id;            // Token for the engine for internal mapping
    qint32          state;         // 'needed' flags;
    QByteArray      iname;         // Internal name sth like 'local.baz.public.a'
    QByteArray      exp;           // The expression
    QString         name;          // Displayed name
    QString         value;         // Displayed value
    QByteArray      editvalue;     // Displayed value
    DebuggerDisplay editformat;    // Format of displayed value
    DebuggerEncoding editencoding; // Encoding of displayed value
    QByteArray      type;          // Type for further processing
    quint64         address;       // Displayed address of the actual object
    quint64         origaddr;      // Address of the pointer referencing this item (gdb auto-deref)
    uint            size;          // Size
    uint            bitpos;        // Position within bit fields
    uint            bitsize;       // Size in case of bit fields
    int             elided;        // Full size if value was cut off, -1 if cut on unknown size, 0 otherwise
    int             arrayIndex;    // -1 if not an array member
    bool            wantsChildren;
    bool            valueEnabled;  // Value will be enabled or not
    bool            valueEditable; // Value will be editable
    bool            outdated;      // \internal item is to be removed.

private:
    void parseHelper(const GdbMi &input);
    Q_DECLARE_TR_FUNCTIONS(Debugger::Internal::WatchHandler)
};

} // namespace Internal
} // namespace Debugger

#endif // DEBUGGER_WATCHDATA_H
