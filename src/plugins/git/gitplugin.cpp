#ifdef WITH_TESTS
#include "clonewizardpage.h"
#include <QTest>
#endif

Q_DECLARE_METATYPE(Git::Internal::FileStates)

using namespace Core;
using namespace Utils;
using namespace VcsBase;
namespace Git {
namespace Internal {

const unsigned minimumRequiredVersion = 0x010702;
const char RC_GIT_MIME_XML[] = ":/git/Git.mimetypes.xml";

const VcsBaseEditorParameters editorParameters[] = {
static GitPlugin *m_instance = 0;
const VcsBaseSubmitEditorParameters submitParameters = {
    VcsBaseSubmitEditorParameters::DiffRows
ParameterAction *GitPlugin::createParameterAction(ActionContainer *ac,
                                                  const QString &defaultText, const QString &parameterText,
                                                  Id id, const Context &context,
                                                  bool addToLocator, const QKeySequence &keys)
{
    auto action = new ParameterAction(defaultText, parameterText, ParameterAction::EnabledWithParameter, this);
    Core::Command *command = ActionManager::registerAction(action, id, context);
    if (!keys.isEmpty())
        command->setDefaultKeySequence(keys);
    return action;
QAction *GitPlugin::createFileAction(ActionContainer *ac,
                                     const QString &defaultText, const QString &parameterText,
                                     Id id, const Context &context, bool addToLocator,
                                     const char *pluginSlot, const QKeySequence &keys)
    ParameterAction *action = createParameterAction(ac, defaultText, parameterText, id, context, addToLocator, keys);
    m_fileActions.push_back(action);
    connect(action, SIGNAL(triggered()), this, pluginSlot);
    return action;
QAction *GitPlugin::createProjectAction(ActionContainer *ac,
                                        const QString &defaultText, const QString &parameterText,
                                        Id id, const Context &context, bool addToLocator,
                                        const char *pluginSlot, const QKeySequence &keys)
    ParameterAction *action = createParameterAction(ac, defaultText, parameterText, id, context, addToLocator, keys);
    m_projectActions.push_back(action);
    connect(action, SIGNAL(triggered()), this, pluginSlot);
    return action;
QAction *GitPlugin::createRepositoryAction(ActionContainer *ac,
                                           const QString &text, Id id,
                                           const Context &context, bool addToLocator,
                                           const QKeySequence &keys)
{
    QAction *action = new QAction(text, this);
    Core::Command *command = ActionManager::registerAction(action, id, context);
    if (!keys.isEmpty())
        command->setDefaultKeySequence(keys);
    return action;
QAction *GitPlugin::createRepositoryAction(ActionContainer *ac,
                                           const QString &text, Id id,
                                           const Context &context, bool addToLocator,
                                           const char *pluginSlot, const QKeySequence &keys)
    QAction *action = createRepositoryAction(ac, text, id, context, addToLocator, keys);
    connect(action, SIGNAL(triggered()), this, pluginSlot);
    action->setData(id.uniqueIdentifier());
    return action;
// taking the directory.
QAction *GitPlugin::createRepositoryAction(ActionContainer *ac,
                                           const QString &text, Id id,
                                           const Context &context, bool addToLocator,
                                           GitClientMemberFunc func, const QKeySequence &keys)
    QAction *action = createRepositoryAction(ac, text, id, context, addToLocator, keys);
    connect(action, &QAction::triggered, [this, func]() -> void {
        QTC_ASSERT(currentState().hasTopLevel(), return);
        (m_gitClient->*func)(currentState().topLevel());
    });
    return action;
    m_settings.readSettings(ICore::settings());
    typedef VcsEditorFactory<GitEditorWidget> GitEditorFactory;
    typedef VcsSubmitEditorFactory<GitSubmitEditor> GitSubmitEditorFactory;
    Context globalcontext(Core::Constants::C_GLOBAL);
    const int editorCount = sizeof(editorParameters) / sizeof(editorParameters[0]);
    m_commandLocator = new CommandLocator("Git", prefix, prefix);
    ActionContainer *toolsContainer = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    ActionContainer *gitContainer = ActionManager::createMenu("Git");

    ActionContainer *currentFileMenu = ActionManager::createMenu("Git.CurrentFileMenu");
    createFileAction(currentFileMenu, tr("Diff Current File"), tr("Diff of \"%1\""),
                     "Git.Diff", globalcontext, true, SLOT(diffCurrentFile()),
                      QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+D") : tr("Alt+G,Alt+D")));

    createFileAction(currentFileMenu, tr("Log Current File"), tr("Log of \"%1\""),
                     "Git.Log", globalcontext, true, SLOT(logFile()),
                     QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+L") : tr("Alt+G,Alt+L")));
    createFileAction(currentFileMenu, tr("Blame Current File"), tr("Blame for \"%1\""),
                     "Git.Blame", globalcontext, true, SLOT(blameFile()),
                     QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+B") : tr("Alt+G,Alt+B")));
    currentFileMenu->addSeparator(globalcontext);
    createFileAction(currentFileMenu, tr("Stage File for Commit"), tr("Stage \"%1\" for Commit"),
                     "Git.Stage", globalcontext, true, SLOT(stageFile()),
                     QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+A") : tr("Alt+G,Alt+A")));
    createFileAction(currentFileMenu, tr("Unstage File from Commit"), tr("Unstage \"%1\" from Commit"),
                     "Git.Unstage", globalcontext, true, SLOT(unstageFile()));
    createFileAction(currentFileMenu, tr("Undo Unstaged Changes"), tr("Undo Unstaged Changes for \"%1\""),
                     "Git.UndoUnstaged", globalcontext,
                     true, SLOT(undoUnstagedFileChanges()));

    createFileAction(currentFileMenu, tr("Undo Uncommitted Changes"), tr("Undo Uncommitted Changes for \"%1\""),
                     "Git.Undo", globalcontext,
                     true, SLOT(undoFileChanges()),
                     QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+U") : tr("Alt+G,Alt+U")));
    ActionContainer *currentProjectMenu = ActionManager::createMenu("Git.CurrentProjectMenu");
    createProjectAction(currentProjectMenu, tr("Diff Current Project"), tr("Diff Project \"%1\""),
                        "Git.DiffProject", globalcontext, true, SLOT(diffCurrentProject()),
                        QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+Shift+D") : tr("Alt+G,Alt+Shift+D")));
    createProjectAction(currentProjectMenu, tr("Log Project"), tr("Log Project \"%1\""),
                        "Git.LogProject", globalcontext, true, SLOT(logProject()),
                        QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+K") : tr("Alt+G,Alt+K")));
    createProjectAction(currentProjectMenu, tr("Clean Project..."), tr("Clean Project \"%1\"..."),
                        "Git.CleanProject", globalcontext, true, SLOT(cleanProject()));
    ActionContainer *localRepositoryMenu = ActionManager::createMenu("Git.LocalRepositoryMenu");
    createRepositoryAction(localRepositoryMenu, tr("Diff"), "Git.DiffRepository",
    createRepositoryAction(localRepositoryMenu, tr("Log"), "Git.LogRepository",
    createRepositoryAction(localRepositoryMenu, tr("Reflog"), "Git.ReflogRepository",
    createRepositoryAction(localRepositoryMenu, tr("Clean..."), "Git.CleanRepository",
    createRepositoryAction(localRepositoryMenu, tr("Status"), "Git.StatusRepository",
    createRepositoryAction(localRepositoryMenu, tr("Commit..."), "Git.Commit",
                           globalcontext, true, SLOT(startCommit()));
                           QKeySequence(UseMacShortcuts ? tr("Meta+G,Meta+C") : tr("Alt+G,Alt+C"));
                                   globalcontext, true, SLOT(startFixupCommit()));

                                   globalcontext, true, SLOT(startRebase()));
                                   globalcontext, true, SLOT(updateSubmodules()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
                                   globalcontext, true, SLOT(continueOrAbortCommand()));
    ActionContainer *patchMenu = ActionManager::createMenu("Git.PatchMenu");
    m_applyCurrentFilePatchAction =

    ActionContainer *stashMenu = ActionManager::createMenu("Git.StashMenu");
    QAction *action = createRepositoryAction(stashMenu, tr("Stash"), "Git.Stash",
                                             globalcontext, true, SLOT(stash()));
    action->setToolTip(tr("Saves the current state of your work and resets the repository."));
    action = createRepositoryAction(stashMenu, tr("Take Snapshot..."), "Git.StashSnapshot",
                                    globalcontext, true, SLOT(stashSnapshot()));
    action->setToolTip(tr("Saves the current state of your work."));
    action = createRepositoryAction(stashMenu, tr("Stash Pop"), "Git.StashPop",
                                    globalcontext, true, &GitClient::stashPop);
    action->setToolTip(tr("Restores changes saved to the stash list using \"Stash\"."));
    ActionContainer *remoteRepositoryMenu = ActionManager::createMenu("Git.RemoteRepositoryMenu");
    createRepositoryAction(remoteRepositoryMenu, tr("Fetch"), "Git.Fetch",
    createRepositoryAction(remoteRepositoryMenu, tr("Pull"), "Git.Pull",
    createRepositoryAction(remoteRepositoryMenu, tr("Push"), "Git.Push",
                           globalcontext, true, SLOT(push()));
    ActionContainer *subversionMenu = ActionManager::createMenu("Git.Subversion");
    ActionContainer *gitToolsMenu = ActionManager::createMenu("Git.GitToolsMenu");
    createFileAction(gitToolsMenu, tr("Gitk Current File"), tr("Gitk of \"%1\""),
                     "Git.GitkFile", globalcontext, true, SLOT(gitkForCurrentFile()));
    createFileAction(gitToolsMenu, tr("Gitk for folder of Current File"), tr("Gitk for folder of \"%1\""),
                     "Git.GitkFolder", globalcontext, true, SLOT(gitkForCurrentFolder()));
    m_repositoryBrowserAction =
            createRepositoryAction(gitToolsMenu,
                                   tr("Repository Browser"), "Git.LaunchRepositoryBrowser",
                                   globalcontext, true, &GitClient::launchRepositoryBrowser);
                                   globalcontext, true, SLOT(startMergeTool()));
    Core::Command *createRepositoryCommand = ActionManager::registerAction(repositoryAction, "Git.CreateRepository", globalcontext);
    Context submitContext(Constants::C_GITSUBMITEDITOR);
    m_submitCurrentAction = new QAction(VcsBaseSubmitEditor::submitIcon(), tr("Commit"), this);
    Core::Command *command = ActionManager::registerAction(m_submitCurrentAction, Constants::SUBMIT_CURRENT, submitContext);
    m_diffSelectedFilesAction = new QAction(VcsBaseSubmitEditor::diffIcon(), tr("Diff &Selected Files"), this);
    command = ActionManager::registerAction(m_diffSelectedFilesAction, Constants::DIFF_SELECTED, submitContext);
    command = ActionManager::registerAction(m_undoAction, Core::Constants::UNDO, submitContext);
    command = ActionManager::registerAction(m_redoAction, Core::Constants::REDO, submitContext);
    connect(VcsManager::instance(), SIGNAL(repositoryChanged(QString)),
    connect(VcsManager::instance(), SIGNAL(repositoryChanged(QString)),
    if (!MimeDatabase::addMimeTypes(QLatin1String(RC_GIT_MIME_XML), errorMessage))
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const int lineNumber = VcsBaseEditorWidget::lineNumberOfCurrentEditor(state.currentFile());
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    FileChangeBlocker fcb(state.currentFile());
    if (!DocumentManager::saveAllModifiedDocuments())
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    LogChangeDialog dialog(true, ICore::mainWindow());
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    LogChangeDialog dialog(false, ICore::mainWindow());
    const VcsBasePluginState state = currentState();
    Id id = action ? Id::fromUniqueIdentifier(action->data().toInt()) : Id();
    ChangeSelectionDialog dialog(state.topLevel(), id, ICore::mainWindow());
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
        VcsBaseOutputWindow::instance()->appendWarning(tr("Another submit is currently being executed."));
    const VcsBasePluginState state = currentState();
        VcsBaseOutputWindow::instance()->appendError(errorMessage);
    TempFileSaver saver;
        VcsBaseOutputWindow::instance()->appendError(saver.errorString());
    IDocument *curDocument = EditorManager::currentDocument();
    InfoBar *infoBar = curDocument->infoBar();
    Id gitVersionWarning("GitVersionWarning");
    infoBar->addInfo(InfoBarEntry(gitVersionWarning,
                        InfoBarEntry::GlobalSuppressionEnabled));
IEditor *GitPlugin::openSubmitEditor(const QString &fileName, const CommitData &cd)
    IEditor *editor = EditorManager::openEditor(fileName, Constants::GITSUBMITEDITOR_ID);
    IDocument *document = submitEditor->document();
    EditorManager::closeEditor();
    IDocument *editorDocument = editor->document();
    const VcsBaseSubmitEditor::PromptSubmitResult answer
    case VcsBaseSubmitEditor::SubmitCanceled:
    case VcsBaseSubmitEditor::SubmitDiscarded:
    SubmitFileModel *model = qobject_cast<SubmitFileModel *>(editor->fileModel());
        if (!DocumentManager::saveDocument(editorDocument))
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
    QWidget *parent = ICore::mainWindow();
    CleanDialog dialog(parent);
    const VcsBasePluginState state = currentState();
    return DocumentManager::saveModifiedDocument(DocumentModel::documentForFilePath(fileName));
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
        file = QFileDialog::getOpenFileName(ICore::mainWindow(), tr("Choose Patch"), QString(), filter);
    VcsBaseOutputWindow *outwin = VcsBaseOutputWindow::instance();
    if (!DocumentManager::saveAllModifiedDocuments())
    const VcsBasePluginState state = currentState();
    const VcsBasePluginState state = currentState();
        dialog = new NonModalDialog(ICore::mainWindow());
void GitPlugin::updateActions(VcsBasePlugin::ActionState as)
    foreach (ParameterAction *fileAction, m_fileActions)
    foreach (ParameterAction *projectAction, m_projectActions)
    GitEditorWidget editor(editorParameters + 3, 0);
    GitEditorWidget editor(editorParameters + 1, 0);
} // namespace Internal
} // namespace Git
