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

#include "dclass.h"

#include "dvisitor.h"
#include "dconstvisitor.h"

namespace qmt {

DClass::DClass()
    : m_templateDisplay(TemplateSmart),
      m_showAllMembers(false)
{
}

DClass::~DClass()
{
}

void DClass::setUmlNamespace(const QString &umlNamespace)
{
    m_umlNamespace = umlNamespace;
}

void DClass::setTemplateParameters(const QList<QString> &templateParameters)
{
    m_templateParameters = templateParameters;
}

void DClass::setMembers(const QList<MClassMember> &members)
{
    m_members = members;
}

void DClass::setVisibleMembers(const QSet<Uid> &visibleMembers)
{
    m_visibleMembers = visibleMembers;
}

void DClass::setTemplateDisplay(DClass::TemplateDisplay templateDisplay)
{
    m_templateDisplay = templateDisplay;
}

void DClass::setShowAllMembers(bool showAllMembers)
{
    m_showAllMembers = showAllMembers;
}

void DClass::accept(DVisitor *visitor)
{
    visitor->visitDClass(this);
}

void DClass::accept(DConstVisitor *visitor) const
{
    visitor->visitDClass(this);
}

} // namespace qmt
