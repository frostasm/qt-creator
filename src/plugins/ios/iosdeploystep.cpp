/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "iosdeploystepwidget.h"
#include "iosdeploystep.h"
#include "iosbuildstep.h"
#include "iosconstants.h"
#include "iosrunconfiguration.h"
#include "iosmanager.h"
#include "iostoolhandler.h"

#include <coreplugin/messagemanager.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <qt4projectmanager/qt4buildconfiguration.h>
#include <qt4projectmanager/qt4project.h>
#include <qt4projectmanager/qt4nodes.h>

#include <qtsupport/qtkitinformation.h>

#include <QDir>

#define ASSERT_STATE(state) ASSERT_STATE_GENERIC(State, state, m_state)

using namespace ProjectExplorer;
using namespace Qt4ProjectManager;

namespace Ios {
namespace Internal {

static const char USE_LOCAL_QT_KEY[] = "Qt4ProjectManager.IosDeployStep.UseLocalQtLibs";

const Core::Id IosDeployStep::Id("Qt4ProjectManager.IosDeployStep");

IosDeployStep::IosDeployStep(ProjectExplorer::BuildStepList *parent)
    : BuildStep(parent, Id)
{
    ctor();
}

IosDeployStep::IosDeployStep(ProjectExplorer::BuildStepList *parent,
    IosDeployStep *other)
    : BuildStep(parent, other)
{
    ctor();
}

IosDeployStep::~IosDeployStep() { }

void IosDeployStep::ctor()
{
    m_transferStatus = NoTransfer;
    m_device = ProjectExplorer::DeviceKitInformation::device(target()->kit());
    QString devName;
    if (!m_device.isNull())
        devName = m_device->displayName();
    if (devName.isEmpty())
        devName = tr("iOS device");
    setDefaultDisplayName(tr("Deploy to %1").arg(devName));
}

bool IosDeployStep::init()
{
    QTC_CHECK(m_transferStatus == NoTransfer);
    m_device = ProjectExplorer::DeviceKitInformation::device(target()->kit());
    if (m_device.isNull()) {
        emit addOutput(tr("Error: no device available, deploy failed."),
                       BuildStep::ErrorMessageOutput);
        return false;
    }
    return true;
}

void IosDeployStep::run(QFutureInterface<bool> &fi)
{
    m_futureInterface = fi;
    QTC_CHECK(m_transferStatus == NoTransfer);
    if (iosdevice().isNull()) {
        m_futureInterface.reportResult(!iossimulator().isNull());
        cleanup();
        m_futureInterface.reportFinished();
        return;
    }
    m_transferStatus = TransferInProgress;
    IosToolHandler *toolHandler = new IosToolHandler(IosToolHandler::IosDeviceType, this);
    m_futureInterface.setProgressRange(0, 200);
    m_futureInterface.setProgressValueAndText(0, QLatin1String("Transferring application"));
    m_futureInterface.reportStarted();
    connect(toolHandler, SIGNAL(isTransferringApp(Ios::IosToolHandler*,QString,QString,int,int,QString)),
            SLOT(handleIsTransferringApp(Ios::IosToolHandler*,QString,QString,int,int,QString)));
    connect(toolHandler, SIGNAL(didTransferApp(Ios::IosToolHandler*,QString,QString,Ios::IosToolHandler::OpStatus)),
            SLOT(handleDidTransferApp(Ios::IosToolHandler*,QString,QString,Ios::IosToolHandler::OpStatus)));
    connect(toolHandler, SIGNAL(finished(Ios::IosToolHandler*)),
            SLOT(handleFinished(Ios::IosToolHandler*)));
    connect(toolHandler, SIGNAL(errorMsg(Ios::IosToolHandler*,QString)),
            SLOT(handleErrorMsg(Ios::IosToolHandler*,QString)));
    toolHandler->requestTransferApp(appBundle(), deviceId());
}

void IosDeployStep::cancel()
{
    if (m_toolHandler)
        m_toolHandler->stop();
}

void IosDeployStep::cleanup()
{
    QTC_CHECK(m_transferStatus != TransferInProgress);
    m_transferStatus = NoTransfer;
    m_device.clear();
    m_toolHandler = 0;
}

void IosDeployStep::handleIsTransferringApp(IosToolHandler *handler, const QString &bundlePath,
                                            const QString &deviceId, int progress, int maxProgress,
                                            const QString &info)
{
    Q_UNUSED(handler); Q_UNUSED(bundlePath); Q_UNUSED(deviceId);
    QTC_CHECK(m_transferStatus == TransferInProgress);
    m_futureInterface.setProgressRange(0, maxProgress);
    m_futureInterface.setProgressValueAndText(progress, info);
}

void IosDeployStep::handleDidTransferApp(IosToolHandler *handler, const QString &bundlePath,
                                         const QString &deviceId, IosToolHandler::OpStatus status)
{
    Q_UNUSED(handler); Q_UNUSED(bundlePath); Q_UNUSED(deviceId);
    QTC_CHECK(m_transferStatus == TransferInProgress);
    if (status == IosToolHandler::Success)
        m_transferStatus = TransferOk;
    else
        m_transferStatus = TransferFailed;
    m_futureInterface.reportResult(status == IosToolHandler::Success);
}

void IosDeployStep::handleFinished(IosToolHandler *handler)
{
    switch (m_transferStatus) {
    case TransferInProgress:
        m_transferStatus = TransferFailed;
        m_futureInterface.reportResult(false);
        break;
    case NoTransfer:
    case TransferOk:
    case TransferFailed:
        break;
    }
    cleanup();
    handler->deleteLater();
    // move it when result is reported? (would need care to avoid problems with concurrent runs)
    m_futureInterface.reportFinished();
}

void IosDeployStep::handleErrorMsg(IosToolHandler *handler, const QString &msg)
{
    Q_UNUSED(handler);
    emit addOutput(msg, BuildStep::ErrorMessageOutput);
}

BuildStepConfigWidget *IosDeployStep::createConfigWidget()
{
    return new IosDeployStepWidget(this);
}

bool IosDeployStep::fromMap(const QVariantMap &map)
{
    return ProjectExplorer::BuildStep::fromMap(map);
}

QVariantMap IosDeployStep::toMap() const
{
    QVariantMap map = ProjectExplorer::BuildStep::toMap();
    return map;
}

QString IosDeployStep::deviceId() const
{
    if (iosdevice().isNull())
        return QString();
    return iosdevice()->uniqueDeviceID();
}

QString IosDeployStep::appBundle() const
{
    IosRunConfiguration * runConfig = qobject_cast<IosRunConfiguration *>(
                this->target()->activeRunConfiguration());
    return runConfig->bundleDir().toString();
}

void IosDeployStep::raiseError(const QString &errorString)
{
    emit addTask(Task(Task::Error, errorString, Utils::FileName::fromString(QString()), -1,
        ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM));
}

void IosDeployStep::writeOutput(const QString &text, OutputFormat format)
{
    emit addOutput(text, format);
}

IDevice::ConstPtr IosDeployStep::device() const
{
    return m_device;
}

IosDevice::ConstPtr IosDeployStep::iosdevice() const
{
    return m_device.dynamicCast<const IosDevice>();
}

IosSimulator::ConstPtr IosDeployStep::iossimulator() const
{
    return m_device.dynamicCast<const IosSimulator>();
}

} // namespace Internal
} // namespace Qt4ProjectManager
