#include <vcsbase/vcsoutputwindow.h>
    Constants::FILELOG_ID,
    Constants::FILELOG_DISPLAY_NAME,
    Constants::FILELOG,
    Constants::LOGAPP},
    VcsBaseSubmitEditorParameters::DiffFiles
    const auto widgetCreator = []() { return new MercurialEditorWidget; };
    connect(m_client, SIGNAL(parsedStatus(QList<VcsBaseClient::StatusItem>)),
            this, SLOT(showCommitWidget(QList<VcsBaseClient::StatusItem>)));
    disconnect(m_client, SIGNAL(parsedStatus(QList<VcsBaseClient::StatusItem>)),
               this, SLOT(showCommitWidget(QList<VcsBaseClient::StatusItem>)));
        VcsOutputWindow::appendError(tr("There are no changes to commit."));
        VcsOutputWindow::appendError(saver.errorString());
        VcsOutputWindow::appendError(tr("Unable to create an editor for the commit."));
    VcsBaseEditorWidget::testDiffFileResolving(editorParameters[2].id);
    VcsBaseEditorWidget::testLogResolving(editorParameters[0].id, data, "18473:692cbda1eb50", "18472:37100f30590f");