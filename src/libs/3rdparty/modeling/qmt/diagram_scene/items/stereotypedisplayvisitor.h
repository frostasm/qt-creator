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

#ifndef QMT_STEREOTYPEDISPLAYVISITOR_H
#define QMT_STEREOTYPEDISPLAYVISITOR_H

#include "qmt/diagram_controller/dvoidvisitor.h"

#include "qmt/diagram/dobject.h"
#include "qmt/stereotype/stereotypeicon.h"

namespace qmt {

class ModelController;
class StereotypeController;

class StereotypeDisplayVisitor : public DConstVoidVisitor
{
public:
    StereotypeDisplayVisitor();
    ~StereotypeDisplayVisitor();

    void setModelController(ModelController *modelController);
    void setStereotypeController(StereotypeController *stereotypeController);
    DObject::StereotypeDisplay stereotypeDisplay() const { return m_stereotypeDisplay; }
    StereotypeIcon::Display stereotypeIconDisplay() const;
    QString stereotypeIconId() const { return m_stereotypeIconId; }
    QString shapeIconId() const { return m_shapeIconId; }

    void visitDObject(const DObject *object) override;
    void visitDPackage(const DPackage *package) override;
    void visitDClass(const DClass *klass) override;
    void visitDComponent(const DComponent *component) override;
    void visitDDiagram(const DDiagram *diagram) override;
    void visitDItem(const DItem *item) override;

private:
    ModelController *m_modelController = 0;
    StereotypeController *m_stereotypeController = 0;
    DObject::StereotypeDisplay m_stereotypeDisplay = DObject::StereotypeNone;
    QString m_stereotypeIconId;
    QString m_shapeIconId;
    StereotypeIcon::Element m_stereotypeIconElement = StereotypeIcon::ElementAny;
    DObject::StereotypeDisplay m_stereotypeSmartDisplay = DObject::StereotypeDecoration;
};

} // namespace qmt

#endif // QMT_STEREOTYPEDISPLAYVISITOR_H
