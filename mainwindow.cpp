/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include "statsview.h"
#include "mainwindow.h"
#define para "</p><p>"
#define bold(X) " <b>" + X + "</b> "

//===================================================
// HELP
//===================================================
void MainWindow::aboutQt() {
    qApp->aboutQt();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About QtAp"),
        bold(tr("QtAp")) +
        tr("for text document control and "
         "generation of information views.") +
        para +
        tr("Wrote in C++ using Qt5 kit and Creator. "
         "It requires GIT for sync to work, which in turn depends on SSH.") +
        para +
        tr("(C) K Ring Technologies Ltd, BSD Licence terms extending a "
         "copyrighted work of The Qt Company Ltd.") +
        para +
        tr("The main work of the application has been to build a framework "
         "to run a library of code, which in turn could be using some "
         "calls to some other libraries of code released under different "
         "terms. Nothing in the code base is currently prohibited from "
         "extension under the BSD Licence.") +
        para +
        tr("Version: ") + QCoreApplication::applicationVersion()
        );
}

//===================================================
// MAIN WINDOW MANAGEMENT
//===================================================
void MainWindow::setMain(QWidget *widget) {
    if(center->indexOf(widget) < 0) {
        center->addWidget(widget);
    }
    if(getMain() != widget) {
        center->setCurrentWidget(widget);
    }
    StatsView *k = (widget != textEdit) ?
                nullptr :
                (StatsView *)widget;
    QMap<StatsView *, QList<QAction *>>::iterator i;
    for (i = inViewActions.begin(); i != inViewActions.end(); ++i) {
        QList<QAction *>::iterator j;
        for (j = (*i).begin(); j != (*i).end(); ++j) {
            (*j)->setVisible(false);
            if(k == i.key()) (*j)->setVisible(true);//and for view
        }
    }
    checkSave(textEdit->document()->isModified());//test save avail?
    checkClipboard();
    checkSelected(lastSelected);
    checkUndo(lastUndo);
    checkRedo(lastRedo);
}

QWidget *MainWindow::getMain() {
    return center->currentWidget();
}

bool MainWindow::isTextMain(bool writable) {
    return (getMain() == textEdit) && (writable || textEdit->canPaste());
}

MainWindow::MainWindow()
    : textEdit(new ATextEdit(this)) {
    QLOAD;
    handle = QRESOLVE("handle");
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QString style = styleSheet();
    if(style == nullptr) style = QString();
    setStyleSheet(style + "\n" + loadStyle());

    exitCheck = false;
    //QIcon::setThemeName("gnome");//TODO maybe become a setting
    QIcon ico = getIconRC("view-text");
    setWindowIcon(ico);
    tray = new QSystemTrayIcon(this);
    tray->setIcon(ico);
    tray->setToolTip(QCoreApplication::applicationName());
    connect(tray, &QSystemTrayIcon::activated, this, &MainWindow::checkTray);
    inViewActions.clear();
    //add in the extra views
    listOfViews.append(new StatsView(this));

    //create menus
    createActions();
    createStatusBar();
    fillCommands();
    readSettings();
    hasRepo();

    center = new QStackedWidget(this);
    setCentralWidget(center);

    settings = new Settings();
    holdWhileSettings = settings;

    setMain(textEdit);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    connect(textEdit, &QPlainTextEdit::copyAvailable,
            this, &MainWindow::checkSelected);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            this, &MainWindow::checkUndo);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            this, &MainWindow::checkRedo);
    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &MainWindow::checkSave);
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::checkClipboard);
}

QString MainWindow::loadStyle() {
    QFile file(":/style.css");
    if(!file.open(QFile::ReadOnly)) return "";
    QTextStream in(&file);
    return in.readAll();
}

QWidget *MainWindow::getQWebEngineView() {
    return QWIDGETPTR(handle->getJSHost());
}

void MainWindow::setCommand(QAction *action, StatsView *view) {
    inViewActions[view].append(action);
    commands->addAction(action);
    commandToolBar->addAction(action);
}

void MainWindow::fillCommands() {
    QList<StatsView *>::iterator i;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->setCommands();
    }
}

void MainWindow::setInBackground(QString view, QString command) {
    backgrounded = true;//sets for auto save and other options
    QMap<StatsView *, QList<QAction *>>::iterator i;
    for (i = inViewActions.begin(); i != inViewActions.end(); ++i) {
        if(i.key()->getViewName().toLower() == view) {
            QList<QAction *>::iterator j;
            for (j = (*i).begin(); j != (*i).end(); ++j) {
                if((*j)->text().toLower().replace("&", "") == command)
                    (*j)->trigger();//and for view
            }
        }
    }
}

//===================================================
// PROXY ENABLE ACTION CHECKS
//===================================================
void MainWindow::checkClipboard() {
    bool hasText = false;
    if(QGuiApplication::clipboard()->mimeData()->hasText() &&
            QGuiApplication::clipboard()->text().length() > 0) {//check paste sensible
        hasText = true;
    }
    setPaste(hasText & isTextMain(false));//false forces check to see if paste
}

void MainWindow::checkSelected(bool active) {
    //intecept for selection auto processing
    lastSelected = active;
    setCopy(active & isTextMain());
}

void MainWindow::checkUndo(bool active) {
    //intercept to change allowed undo
    lastUndo = active;
    setUndo(active & isTextMain());
}

void MainWindow::checkRedo(bool active) {
    //intercept to change allowed redo
    lastRedo = active;
    setRedo(active & isTextMain());
}

void MainWindow::checkSave(bool active) {
    checkAvailable(active);//only views of saved
    QList<StatsView *>::iterator i;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        active |= (*i)->needsSave();
    }
    setSave(active);
    setWindowModified(active);
}

void MainWindow::checkTray(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
    }
}

void MainWindow::checkAvailable(bool notSaved) {
    QList<StatsView *>::iterator i;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->defaultAvailable();
    }
    if(!notSaved) for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->checkAvailable();//restore all possible not safe (saved) bets
    }
}

void MainWindow::checkEvents() {
    QCoreApplication::processEvents();//botch loop
}

//===================================================
// GIT MANAGEMENT
//===================================================
int MainWindow::quietBash(QString proc) {
    //NB. NO PROTECTION FROM directory CHANGE
    return QProcess::execute("bash", QStringList()
                                     << "-c"
                                     << "cd \"" + directory + "\" && " + proc);
}

int MainWindow::bash(QString proc) {
    setDirectory(false);
    setClone(false);
    setSync(false);
    QStringList sl = proc.split("&&&");//split notation
    QProgressDialog mb(tr("Please wait."),
                       tr("Cancel"), 0, sl.length(), this);
    mb.setModal(true);
    mb.setMaximum(sl.length());
    mb.setValue(0);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkEvents);
    timer->start(500);
    int j = 0;
    for(int i = 0; i < sl.length(); ++i) {
        j = quietBash(sl[i]);
        mb.setValue(i + 1);//update
        if(j != 0 || mb.wasCanceled()) break;//exit early
    }
    timer->stop();
    mb.hide();
    hasRepo();//re-entrant catch recursive close
    return j;
}

void MainWindow::publish() {
    if(maybeSave()) {
        QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss");
        if(bash(//"git stash push &&&"
                //"git pull &&&"
                //"git stash pop &&&"
                "git commit -a -m \"" + now + "\" &&&"
                "git push"
                ) != 0) {
            QMessageBox::critical(this, tr("Publication Error"),
                     tr("The repository could not be published. "
                        "There maybe a complex data merge issue if many users "
                        "update the same documents. If you did not use SSH to "
                        "clone and used HTTPS instead, you may not have write "
                        "and update privilages. Are you a member of the repository "
                        "commit group? Is there anything to merge?"));
            return;
        }
    statusBar()->showMessage(tr("Published all saved edits"), 2000);
    //reload();
    }
}

void MainWindow::read() {
    if(bash("git stash push -a || exit 0 &&&"
            "git pull &&&"
            "git stash pop || exit 0"
            ) != 0) { //incorporate
        QMessageBox::critical(this, tr("Reading Error"),
                 tr("The repository could not be read. "
                    "There maybe a complex data merge issue if many users "
                    "update the same documents."));
        return;
    }
    statusBar()->showMessage(tr("Read all updates and restored saved edits"), 2000);
    reload();
}

void MainWindow::root() {
    QFileDialog dialog(this, tr("Set Sync Working Directory"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setNameFilterDetailsVisible(false);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setLabelText(QFileDialog::Accept, tr("Set"));
    //dialog.setOption(QFileDialog::DontUseNativeDialog);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (!dialog.selectedFiles().first().isEmpty()) {
        directory = dialog.selectedFiles().first();
        statusBar()->showMessage(tr("Working directory set"), 2000);
        hasRepo();
    }
}

void MainWindow::subscribe() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Clone (with SSH to write)"),
        tr("GIT repository identifier"), QLineEdit::Normal,
        "git@github.com:jackokring/qtap-tests-and-progress.git", &ok);

    if (!ok) return;
    if(bash("git clone \"" + text + "\" \"" + directory + "\"") != 0) {
        QMessageBox::critical(this, tr("Subscription Error"),
                 tr("The repository could not be subscribed. "
                    "The working directory may have things in "
                    "which prevent a clone subscription as that "
                    "requires the working directory to be empty."));
        return;
    }
    statusBar()->showMessage(tr("Subscription created by cloning repository"), 2000);
    reload();
    hasRepo();
}

bool MainWindow::hasRepo() {
    setDirectory(true);
    if(quietBash("git status") != 0) {
        setClone(true);
        setSync(false);
        return false;
    }
    setSync(true);
    setClone(false);
    return true;
}

//===================================================
// NEW, OPEN AND SAVE ACTIONS
//===================================================
void MainWindow::asNewShow() {
    show();
    //(this->*fp)();
}

void MainWindow::newFile() {
    //asNewShow();
    if (maybeSave()) {//TODO: maybe make new doc
        textEdit->clear();
        //TODO: select base type
        setCurrentFile(QString());
    }
}

void MainWindow::open() {
    //asNewShow();
    if (maybeSave()) {
        QFileDialog dialog(this, tr("Open File"), directory);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        //dialog.setOption(QFileDialog::DontUseNativeDialog);

        QStringList kinds;
        kinds << "Text files (*.txt)";
        dialog.setNameFilters(kinds);
        if (dialog.exec() != QDialog::Accepted)
            return;
        //TODO: maybe need to do view transform on type
        if (!dialog.selectedFiles().first().isEmpty())
            loadFile(dialog.selectedFiles().first());
    }
}

void MainWindow::save() {
    saved = true;
    if (curFile.isEmpty()) {
        saveAs();
    } else {
        saveFile(curFile);
    }
}

void MainWindow::saveAs() {
    QFileDialog dialog(this, tr("Save File"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    //dialog.setOption(QFileDialog::DontUseNativeDialog);

    QStringList kinds;
    kinds << "Text files (*.txt)";
    dialog.setNameFilters(kinds);
    saved = true;
    if (dialog.exec() != QDialog::Accepted) {
        saved = false;
        return;
    }
    //TODO: maybe need to do view transform on type
    saveFile(dialog.selectedFiles().first());
}

void MainWindow::reload() {
    if(maybeSave(true)) {//reload message and avoid no change exit
        loadFile(curFile);
    }
}

//===================================================
// MENU AND ICON UTILITIES
//===================================================
constexpr Spec operator|(Spec X, Spec Y) {
    return static_cast<Spec>(
        static_cast<unsigned int>(X) | static_cast<unsigned int>(Y));
}

Spec& operator|=(Spec& X, Spec Y) {
    X = X | Y; return X;
}

QIcon MainWindow::getIconRC(QString named) {
    return QIcon::fromTheme(named, QIcon(":/images/" + named + ".png"));
}

QMenu* MainWindow::addMenu(QString menu, void(MainWindow::*fp)(),
                         QString named, QString entry,
                         QKeySequence shorty,
                         QString help, Spec option, StatsView *view) {
    static QMenu *aMenu;
    static QMenu *trayMenu = new QMenu(this);
    static QToolBar *aToolBar;
    static int count = 0;
    if(menu != nullptr) {
        aMenu = menuBar()->addMenu(menu);
        aToolBar = addToolBar(menu.replace('&', ""));
        aToolBar->setMovable(false);
        if(count > 0) trayMenu->addSeparator();
        count = 0;
    }
    if(aMenu == nullptr) {
        aMenu = menuBar()->addMenu("&Menu");
        aToolBar = addToolBar("Menu");
    }

    if(named == nullptr) {
        named = QString("void");//icon name
        option |= noBar;//as sensible
    }
    if(fp == nullptr) {// a blank with not pointer
        commands = aMenu;
        commandToolBar = aToolBar;
        return aMenu;//so don't even add it
    }
    const QIcon newIcon = getIconRC(named);
    if(entry == nullptr) entry = ">>Blank entry<<";
    QAction *newAct = new QAction(newIcon, entry, this);
    if(shorty > 0) newAct->setShortcut(shorty);
    if(help != nullptr) newAct->setStatusTip(help);
    aMenu->addAction(newAct);
    if(option & inTray) {
        tray->setContextMenu(trayMenu);
        tray->setVisible(true);
        trayMenu->addAction(newAct);
        count++;
        //tray show on selection
        connect(newAct, &QAction::triggered, this, &MainWindow::asNewShow);
    }
    //must be done after show. maybe bad but is automatic for menu build.
    if(fp != nullptr) {
        connect(newAct, &QAction::triggered, this, fp);
    }
    if(view != nullptr) {
        connect(newAct, &QAction::triggered, view, &StatsView::selectView);
        newAct->setEnabled(false);
        connect(view, &StatsView::setAvailable, newAct, &QAction::setEnabled);
    }
    if(option & canCopy) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setCopy, newAct, &QAction::setEnabled);
    }
    if(option & canUndo) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setUndo, newAct, &QAction::setEnabled);
    }
    if(option & canRedo) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setRedo, newAct, &QAction::setEnabled);
    }
    if(option & canPaste) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setPaste, newAct, &QAction::setEnabled);
    }
    if(option & canSave) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setSave, newAct, &QAction::setEnabled);
    }
    if(option & canClone) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setClone, newAct, &QAction::setEnabled);
    }
    if(option & canSync) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setSync, newAct, &QAction::setEnabled);
    }
    if(option & canSync) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setDirectory, newAct, &QAction::setEnabled);
    }
    //more options
    if(option & noBar) return aMenu;
    aToolBar->addAction(newAct);
    return aMenu;
}

QMenu* MainWindow::addViewMenu(Spec option) {
    QList<StatsView *>::iterator i;
    QMenu *menu;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        menu = addMenu(nullptr, &MainWindow::viewText, //does default before show
                       (*i)->getIconName(), (*i)->getViewName(),
                       (*i)->getShortCut(), (*i)->getToolTipHelp(), option, *i);
        (*i)->setMainWindow(this);
    }
    return menu;
}

//===================================================
// BASIC PROXY ACTIONS
//===================================================
void MainWindow::closeEvent(QCloseEvent *event) {
    if(event == nullptr) QApplication::exit();//straight out end
    if(!exitCheck && QSystemTrayIcon::isSystemTrayAvailable()) {
        hide();
        event->ignore();//to tray
    } else {
        if (maybeSave()) {
            writeSettings();
            event->accept();
        } else {
            event->ignore();
            exitCheck = false;
        }
    }
}

void MainWindow::close() {
    exitCheck = true;
    QWidget::close();
}

void MainWindow::undo() {
    textEdit->undo();
}

void MainWindow::redo() {
    textEdit->redo();
}

void MainWindow::cut() {
    textEdit->cut();
}

void MainWindow::copy() {
    textEdit->copy();
}

void MainWindow::paste() {
    textEdit->paste();
}

//===================================================
// VIEW MANAGEMENT
//===================================================
void MainWindow::viewText() {
    setMain(textEdit);
    holdWhileSettings = settings;
}

void MainWindow::viewSettings() {
    if(holdWhileSettings == settings) {
        holdWhileSettings = getMain();//for restore
        setMain(settings);
    } else {
        setMain(holdWhileSettings);
        holdWhileSettings = settings;
    }
}

//===================================================
// MENU AND STATUS BAR CREATION
//===================================================
void MainWindow::createActions() {
    addMenu(tr("&File"), &MainWindow::newFile,
            "document-new", tr("&New"), QKeySequence::New,//N
            tr("Create a new file"), inTray);
    addMenu(nullptr, &MainWindow::open,
            "document-open", tr("&Open..."), QKeySequence::Open,//O
            tr("Open an existing file"), inTray);
    addMenu(nullptr, &MainWindow::save,
            "document-save", tr("&Save"), QKeySequence::Save,//S
            tr("Save the document to disk"), canSave);
    addMenu(nullptr, &MainWindow::saveAs,
            "document-save-as", tr("Save &As..."), QKeySequence::SaveAs,//+S
            tr("Save the document under a new name"), noBar | canSave)->addSeparator();//no bar entry
    addMenu(nullptr, &MainWindow::hide,
            "application-tray", tr("&Tray"), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T),//+T
            tr("Place app in tray"), noBar);//no bar entry
    addMenu(nullptr, &MainWindow::close,
            "application-exit", tr("E&xit"), QKeySequence::Quit,//Q
            tr("Exit app"), noBar | inTray);//no bar entry
    menuBar()->addSeparator();

    addMenu(tr("&Edit"), &MainWindow::undo,
            "edit-undo", tr("&Undo"), QKeySequence::Undo,//Z
            tr("Undo the last edit"), noBar | canUndo);
    addMenu(nullptr, &MainWindow::redo,
            "edit-redo", tr("&Redo"), QKeySequence::Redo,//+Z
            tr("Redo the last undo"), noBar | canRedo)->addSeparator();
#ifndef QT_NO_CLIPBOARD
    addMenu(nullptr, &MainWindow::cut,
            "edit-cut",  tr("Cu&t"), QKeySequence::Cut,//X
            tr("Cut the current selection to the clipboard"), canCopy);
    addMenu(nullptr, &MainWindow::copy,
            "edit-copy", tr("&Copy"), QKeySequence::Copy,//C
            tr("Copy the current selection to the clipboard"), canCopy);
    addMenu(nullptr, &MainWindow::paste,
            "edit-paste", tr("&Paste"), QKeySequence::Paste,//V
            tr("Paste the clipboard into the current selection"), canPaste);
    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    addMenu(tr("&View"), &MainWindow::viewText,
            "view-text", tr("&Text"), QKeySequence::AddTab,//T
            tr("Show editable text view"))->addSeparator();
    addViewMenu()->addSeparator();
    addMenu(nullptr, &MainWindow::viewSettings,
            "view-settings", tr("Settin&gs"), QKeySequence(Qt::CTRL + Qt::Key_G),//G
            tr("Show and hide settings view"));
    menuBar()->addSeparator();
    addMenu(tr("&Command"));
    menuBar()->addSeparator();

    addMenu(tr("&Sync"), &MainWindow::publish,
            "sync-publish", tr("&Publish"), QKeySequence::Print,//P
            tr("Publish with services"), canSync);
    addMenu(nullptr, &MainWindow::read,
            "sync-read", tr("&Read"), QKeySequence(Qt::CTRL + Qt::Key_R),//R
            tr("Read from services"), canSync | inTray)->addSeparator();
    addMenu(nullptr, &MainWindow::root,
            "sync-directory", tr("Set &Dir Root..."), QKeySequence(Qt::CTRL + Qt::Key_D),//D
            tr("Set default working directory"), canSetdir | noBar);
    addMenu(nullptr, &MainWindow::subscribe,
            "sync-subscribe", tr("Subscribe C&lone..."), QKeySequence(Qt::CTRL + Qt::Key_L),//L
            tr("Subscribe to a remote git ssh repository and clone it"), canClone | noBar);
    menuBar()->addSeparator();

    addMenu(tr("&Help"), &MainWindow::about,
            nullptr, tr("&About"), 0,
            tr("Show the application's About box"));
    addMenu(nullptr, &MainWindow::aboutQt,
            nullptr, tr("About &Qt"), 0,
            tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

//===================================================
// SETTINGS IO (DEFAULTS)
//===================================================
void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    directory = settings.value("directory", "").toString();
    QList<QToolBar *> toolbars = findChildren<QToolBar *>();
    while (!toolbars.isEmpty()) {
        QToolBar *tb = toolbars.takeFirst();
        bool visible = settings.value("tbv" + tb->windowTitle(), true).toBool();
        tb->setVisible(visible);
    }
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("directory", directory);
    QList<QToolBar *> toolbars = findChildren<QToolBar *>();
    while (isVisible() && !toolbars.isEmpty()) {//prevents setting background mangling
        QToolBar *tb = toolbars.takeFirst();
        settings.setValue("tbv" + tb->windowTitle(), tb->isVisible());
    }
}

//===================================================
// FILE IO
//===================================================
bool MainWindow::maybeSave(bool reload) {
    if (!textEdit->document()->isModified() && !reload)
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Save Changes"),
                               reload ?
                               tr("The document may have been modified by synchronization.\n"
                                  "Do you want to save any possible changes?") :
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               (reload ?
                                    QMessageBox::Save | QMessageBox::Discard
                                  : QMessageBox::Save | QMessageBox::Discard |
                                    QMessageBox::Cancel));
    switch (ret) {
    case QMessageBox::Save:
        save();
        return saved;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
    QString name;
    if(QString::compare(QFileInfo(fileName).suffix(), "txt", Qt::CaseInsensitive) != 0) {
        name = QString(fileName + ".txt");
    } else {
        name = fileName;
    }
    QFile file(name);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file.fileName()),
                                  file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());

    setCurrentFile(fileName);
    QList<StatsView *>::iterator i;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        if((*i)->canCache()) {
            QFile file(name + "." + (*i)->getExtension());
            if(!file.open(QFile::ReadOnly)) {
                QMessageBox::warning(this, tr("File Error"),
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(file.fileName()),
                                          file.errorString()));
                return;
            }
            QTextStream in(&file);
            (*i)->cacheLoad(in.readAll());//load file
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName) {
    QString name;
    if(QString::compare(QFileInfo(fileName).suffix(), "txt", Qt::CaseInsensitive) != 0) {
        name = QString(fileName + ".txt");
    } else {
        name = fileName;
    }
    QFile file(name);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file.fileName()),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();

    setCurrentFile(name);
    QList<StatsView *>::iterator i;
    for (i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        if((*i)->needsSave()) {
            QFile file(name + "." + (*i)->getExtension());
            if (!file.open(QFile::WriteOnly)) {
                QMessageBox::warning(this, tr("File Error"),
                                     tr("Cannot write file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(file.fileName()),
                                          file.errorString()));
                return false;
            }
            QTextStream out(&file);
            out << (*i)->blockingSave();//save file
        }
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    textEdit->document()->clearUndoRedoStacks();//makes more sense
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = strippedName(curFile);
    if(curFile.isEmpty())
        shownName = "untitled.txt";//default to text
    setWindowFilePath(shownName);
    tray->setToolTip(QCoreApplication::applicationName() +
                     " " + shownName);
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager) {
    if(manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif
