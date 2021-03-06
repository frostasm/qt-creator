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
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#ifndef QMLDESIGNER_IMPORTMANAGERVIEW_H
#define QMLDESIGNER_IMPORTMANAGERVIEW_H

#include <abstractview.h>
#include <QPointer>

namespace QmlDesigner {

class ImportsWidget;

class ImportManagerView : public AbstractView
{
    Q_OBJECT
public:
    explicit ImportManagerView(QObject *parent = 0);
    ~ImportManagerView();

    bool hasWidget() const override;
    WidgetInfo widgetInfo() override;

    void modelAttached(Model *model) override;
    void modelAboutToBeDetached(Model *model) override;

    void nodeCreated(const ModelNode &createdNode) override;
    void nodeAboutToBeRemoved(const ModelNode &removedNode) override;

    void importsChanged(const QList<Import> &addedImports, const QList<Import> &removedImports) override;

private slots:
    void removeImport(const Import &import);
    void addImport(const Import &import);

private:
    QPointer<ImportsWidget> m_importsWidget;
};

} // namespace QmlDesigner

#endif // QMLDESIGNER_IMPORTMANAGERVIEW_H
