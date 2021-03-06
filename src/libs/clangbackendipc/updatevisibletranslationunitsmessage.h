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
#ifndef CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H
#define CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H

#include <clangbackendipc_global.h>

#include <utf8stringvector.h>

#include <QMetaType>

namespace ClangBackEnd {

class CMBIPC_EXPORT UpdateVisibleTranslationUnitsMessage
{
    friend CMBIPC_EXPORT QDataStream &operator<<(QDataStream &out, const UpdateVisibleTranslationUnitsMessage &message);
    friend CMBIPC_EXPORT QDataStream &operator>>(QDataStream &in, UpdateVisibleTranslationUnitsMessage &message);
    friend CMBIPC_EXPORT bool operator==(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);
    friend CMBIPC_EXPORT bool operator<(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);

public:
    UpdateVisibleTranslationUnitsMessage() = default;
    UpdateVisibleTranslationUnitsMessage(const Utf8String &currentEditorFilePath,
                                         const Utf8StringVector &visibleEditorFilePaths);

    const Utf8String &currentEditorFilePath() const;
    const Utf8StringVector &visibleEditorFilePaths() const;

private:
    Utf8String currentEditorFilePath_;
    Utf8StringVector visibleEditorFilePaths_;
};

CMBIPC_EXPORT QDataStream &operator<<(QDataStream &out, const UpdateVisibleTranslationUnitsMessage &message);
CMBIPC_EXPORT QDataStream &operator>>(QDataStream &in, UpdateVisibleTranslationUnitsMessage &message);
CMBIPC_EXPORT bool operator==(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);
CMBIPC_EXPORT bool operator<(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);

CMBIPC_EXPORT QDebug operator<<(QDebug debug, const UpdateVisibleTranslationUnitsMessage &message);
void PrintTo(const UpdateVisibleTranslationUnitsMessage &message, ::std::ostream* os);
} // namespace ClangBackEnd

Q_DECLARE_METATYPE(ClangBackEnd::UpdateVisibleTranslationUnitsMessage)

#endif // CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H
