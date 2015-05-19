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

#ifndef DESIGNERCUSTOMOBJECTDATA_H
#define DESIGNERCUSTOMOBJECTDATA_H

#include "nodeinstanceglobal.h"

#include <QHash>
#include <QObject>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QQmlAbstractBinding;
class QQmlContext;
QT_END_NAMESPACE

namespace QmlDesigner {

namespace Internal {

namespace QmlPrivateGate {

class DesignerCustomObjectData
{
public:
    static void registerData(QObject *object, QQmlContext *context);
    static DesignerCustomObjectData *get(QObject *object);
    static QVariant getResetValue(QObject *object, const PropertyName &propertyName);
    static void doResetProperty(QObject *object, const PropertyName &propertyName);
    static bool hasValidResetBinding(QObject *object, const PropertyName &propertyName);
    static bool hasBindingForProperty(QObject *object, const PropertyName &propertyName, bool *hasChanged);

private:
    DesignerCustomObjectData(QObject *object, QQmlContext *context);
    void populateResetHashes();
    QObject *object() const;
    QQmlContext *context() const;
    QVariant getResetValue(const PropertyName &propertyName) const;
    void doResetProperty(const PropertyName &propertyName);
    bool hasValidResetBinding(const PropertyName &propertyName) const;
    QQmlAbstractBinding *getResetBinding(const PropertyName &propertyName) const;
    bool hasBindingForProperty(const PropertyName &propertyName, bool *hasChanged) const;

    QObject *m_object;
    QQmlContext *m_context;
    QHash<PropertyName, QVariant> m_resetValueHash;
    QHash<PropertyName, QWeakPointer<QQmlAbstractBinding> > m_resetBindingHash;
    mutable QHash<PropertyName, bool> m_hasBindingHash;

};

} // namespace QmlPrivateGate
} // namespace Internal
} // namespace QmlDesigner

#endif // DESIGNERCUSTOMOBJECTDATA_H