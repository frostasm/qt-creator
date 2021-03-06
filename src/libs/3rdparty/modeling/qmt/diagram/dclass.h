/***************************************************************************
**
** Copyright (C) 2015 Jochen Becher
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

#ifndef QMT_DCLASS_H
#define QMT_DCLASS_H

#include "dobject.h"
#include "qmt/model/mclassmember.h"

#include <QSet>

namespace qmt {

class QMT_EXPORT DClass : public DObject
{
public:
    enum TemplateDisplay {
        TemplateSmart,
        TemplateBox,
        TemplateName
    };

    DClass();
    ~DClass() override;

    QString umlNamespace() const { return m_umlNamespace; }
    void setUmlNamespace(const QString &umlNamespace);
    QList<QString> templateParameters() const { return m_templateParameters; }
    void setTemplateParameters(const QList<QString> &templateParameters);
    QList<MClassMember> members() const { return m_members; }
    void setMembers(const QList<MClassMember> &members);
    QSet<Uid> visibleMembers() const { return m_visibleMembers; }
    void setVisibleMembers(const QSet<Uid> &visibleMembers);
    TemplateDisplay templateDisplay() const { return m_templateDisplay; }
    void setTemplateDisplay(TemplateDisplay templateDisplay);
    bool showAllMembers() const { return m_showAllMembers; }
    void setShowAllMembers(bool showAllMembers);

    void accept(DVisitor *visitor) override;
    void accept(DConstVisitor *visitor) const override;

private:
    QString m_umlNamespace;
    QList<QString> m_templateParameters;
    QList<MClassMember> m_members;
    QSet<Uid> m_visibleMembers;
    TemplateDisplay m_templateDisplay;
    bool m_showAllMembers;
};

} // namespace qmt

#endif // QMT_DCLASS_H
