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
#include "aviewwidget.h"
#include "mainwindow.h"

//VIEW INCLUDES
#include "statsview.h"
#include "finddialog.h"
#include "utfdialog.h"

//FORMATTING
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
void MainWindow::setMain(QWidget *widget, bool input) {
    if(input) {
        textEdit = (ATextEdit *)widget;
    }
    if(center->indexOf(widget) < 0) {
        center->addWidget(widget);
    }
    if(getMain() != widget) {
        if(widget != settings) {
            holdWhileSettings = settings;
            settings->writeSettings(settingsStore);
            QList<AViewWidget *>::iterator i;//restore
            for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
                settingsStore->beginGroup((*i)->getExtension());
                (*i)->readSettings(settingsStore);
                settingsStore->endGroup();
            }
            QList<ATextEdit *>::iterator j;//restore
            for(j = listOfInputs.begin(); j != listOfInputs.end(); ++j) {
                settingsStore->beginGroup((*j)->getBaseExtension());
                (*j)->readSettings(settingsStore);
                settingsStore->endGroup();
            }
        } else {
            settings->readSettings(settingsStore);
        }
        center->setCurrentWidget(widget);
    }
    AViewWidget *k = (AViewWidget *)widget;
    QMap<AViewWidget *, QList<QAction *>>::iterator i;
    for(i = inViewActions.begin(); i != inViewActions.end(); ++i) {
        QList<QAction *>::iterator j;
        for(j = (*i).begin(); j != (*i).end(); ++j) {
            (*j)->setVisible(false);
            if(k == i.key()) (*j)->setVisible(true);//and for view
        }
    }
    QList<QAction *>::iterator j;
    for(j = inInputActions.begin(); j != inInputActions.end(); ++j) {
        (*j)->setVisible(false);
        if(textEdit == widget) (*j)->setVisible(true);//and for input
    }
    checkSave(textEdit->document()->isModified());//test save avail?
    checkPaste();
    checkCopy(lastCopy);
    checkCut(lastCut);
    checkUndo(lastUndo);
    checkRedo(lastRedo);
    setNeedsText(isTextMain());
}

QWidget *MainWindow::getMain() {
    return center->currentWidget();
}

bool MainWindow::isTextMain() {
    return (getMain() == textEdit);
}

bool MainWindow::isSettingsMain() {
    return (getMain() == settings);
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
    inInputActions.clear();

    //add in the extra output views
    listOfViews.append(new StatsView(this));

    //create menus
    createActions();
    createStatusBar();

    //input views
    setInputCommand(textEdit);

    //all actions
    fillCommands();

    settingsStore = new QSettings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings = new Settings();
    holdWhileSettings = settings;
    settings->setMainWindow(this);//for link back
    center = new QStackedWidget(this);
    setCentralWidget(center);
    readSettings();
    hasRepo();

    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->clear();//first new
    }

    setMain(textEdit);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    connect(textEdit, &QPlainTextEdit::copyAvailable,
            this, &MainWindow::checkCopy);
    connect(textEdit, &QPlainTextEdit::copyAvailable,
            this, &MainWindow::checkCut);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            this, &MainWindow::checkUndo);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            this, &MainWindow::checkRedo);
    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &MainWindow::checkSave);
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::checkPaste);
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

void MainWindow::setCommand(QAction *action, AViewWidget *view) {
    inViewActions[view].append(action);
    commands->addAction(action);
    commandToolBar->addAction(action);
}

void setNewInput(ATextEdit *input, MainWindow *main) {
    if(main->maybeSave()) {
        if(main->getFilename().isEmpty()) {
            main->setMain(input, true);//allow alteration of input widgets
            main->setCurrentFile(QString(), true);
        } else {
            QStringList old = main->editOldBase().split(".");
            QString newBase = input->getBaseExtension();
            main->setMain(input, true);//allow alteration of input widgets
            QStringList name = main->getFilename().split(".");
            QStringList::iterator i;
            for(i = old.begin(); i != old.end(); ++i) {
                name.removeLast();//remove one for each
            }
            main->setCurrentFile(name.join(".") + newBase, true);//with no clear!!
        }
    }
}

void MainWindow::setInputCommand(ATextEdit *input) {
    if(!hasInitiated) {
        hasInitiated = true;
        document = input->document();
    } else {
        QTextDocument *old = input->document();
        input->setDocument(document);
        delete old;//ensure consistent document
    }
    const QIcon newIcon = getIconRC(input->getIconName());
    QAction *newAct = new QAction(newIcon, input->getInputName(), this);
    if(input->getShortcut() > 0) newAct->setShortcut(input->getShortcut());
    if(input->getHelpText() != nullptr) newAct->setStatusTip(input->getHelpText());
    connect(newAct, &QAction::triggered, this,
            [this, input]{ setNewInput(input, this); });
    inInputActions.append(newAct);
    listOfInputs.append(input);
    commands->addAction(newAct);
    commandToolBar->addAction(newAct);
}

QString MainWindow::getFilename() {
    return curFile;
}

QString MainWindow::editOldBase() {
    return textEdit->getBaseExtension();
}

bool MainWindow::fileExtensionOK(QString file) {
    bool ok = false;
    QList<ATextEdit *>::iterator i;
    for(i = listOfInputs.begin(); i != listOfInputs.end(); ++i) {
        int j = file.lastIndexOf((*i)->getBaseExtension());
        if(j + (*i)->getBaseExtension().length() == file.length()) {
            ok = true;
        }
    }
    return ok;
}

void MainWindow::fillCommands() {
    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->setCommands();
    }
    commandToolBar->addSeparator();
}

void MainWindow::setInBackground(QString view, QString command) {
    backgrounded = true;//sets for auto save and other options
    QMap<AViewWidget *, QList<QAction *>>::iterator i;
    for(i = inViewActions.begin(); i != inViewActions.end(); ++i) {
        if(i.key()->getViewName().toLower() == view) {
            setMain(i.key());//set view
            if(!command.isEmpty()) {
                QList<QAction *>::iterator j;
                for(j = (*i).begin(); j != (*i).end(); ++j) {
                    if((*j)->text().toLower().remove("&") == command)
                        (*j)->trigger();//and for view
                }
            }
            save();
            close();
        }
    }
}

QString MainWindow::getText() {
    return textEdit->document()->toPlainText();
}

QWidget *MainWindow::focused(QWidget *top) {
    QWidget *f = QApplication::focusWidget();
    QList<QWidget *> wl = top->findChildren<QWidget *>();
    QList<QWidget *>::iterator i;
    for(i = wl.begin(); i != wl.end(); ++i) {
        if((*i) == f) {
            return f;
        }
    }
    return top;//saves quite a lot of checking
}

void MainWindow::status(QString display) {
    statusBar()->showMessage(display, 2000);
}

void MainWindow::setModified() {
    textEdit->document()->setModified();
}

QList<AViewWidget *>::iterator MainWindow::begin() {
    return listOfViews.begin();
}

QList<AViewWidget *>::iterator MainWindow::end() {
    return listOfViews.end();
}

//===================================================
// PROXY ENABLE ACTION CHECKS
//===================================================
void MainWindow::checkPaste() {
    bool hasText = false;
    if(QGuiApplication::clipboard()->mimeData()->hasText() &&
            QGuiApplication::clipboard()->text().length() > 0) {//check paste sensible
        hasText = true;
    }
    if(isTextMain()) {
        setPaste(hasText & textEdit->canPaste());//check to see if paste
        return;
    }
    setPaste(hasText & ((AViewWidget *)getMain())->canPaste());
}

void MainWindow::checkCopy(bool active) {
    //intecept for selection auto processing
    lastCopy = active;
    if(isTextMain()) {
        setCopy(active);
        return;
    }
    setCopy(((AViewWidget *)getMain())->canCopy());
}

void MainWindow::checkCut(bool active) {
    //intecept for selection auto processing
    lastCut = active;
    if(isTextMain()) {
        setCut(active & textEdit->canPaste());//edit check!!
        return;
    }
    setCut(((AViewWidget *)getMain())->canCut());
}

void MainWindow::checkUndo(bool active) {
    //intercept to change allowed undo
    lastUndo = active;
    if(isTextMain()) {
        setUndo(active);
        return;
    }
    setUndo(((AViewWidget *)getMain())->canUndo());
}

void MainWindow::checkRedo(bool active) {
    //intercept to change allowed redo
    lastRedo = active;
    if(isTextMain()) {
        setRedo(active);
        return;
    }
    setRedo(((AViewWidget *)getMain())->canRedo());
}

void MainWindow::checkSave(bool active) {
    checkAvailable(active);//only views of saved
    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
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
    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->checkAvailable(false);
    }
    if(!notSaved) for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        (*i)->checkAvailable(true);//restore all possible not safe (saved) bets
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

int MainWindow::bash(QString proc, QString undo) {
    prohibits();
    QStringList sl = proc.split("&&&");//split notation
    QProgressDialog mb(tr("Please wait."),
                       (undo == nullptr) ? tr("Abort") :
                                           tr("Cancel"), 0, sl.length(), this);
    mb.setModal(true);
    mb.setValue(0);
    int j = 0;
    int baulk = 0;
    QStringList::iterator i;
    for(i = sl.begin(); i != sl.end(); ++i) {
        j = quietBash(*i);
        mb.setValue(mb.value() + 1);//update
        baulk++;
        if(j != 0 || mb.wasCanceled()) break;//exit early
    }
    mb.hide();
    if(mb.wasCanceled()) {
        if(undo == nullptr) {
            QMessageBox::critical(this, tr("Abort Error"),
                                 tr("No undo technique is supplied."));
            hasRepo();
            return j;
        }
        //undo processing
        QStringList sl2 = undo.split("&&&");//split notation
        if(sl.length() != sl2.length()) {//sanity debug steps!!!
            QMessageBox::critical(this, tr("Cancel Undo Error"),
                                 tr("No undo is correctly programmed."));
            hasRepo();
            return j;
        }
        //1 <= baulk < sl.length()
        QProgressDialog mb(tr("Please wait undoing actions."),
                           tr("Abort Undo"), 0, sl2.length(), this);
        mb.setModal(true);
        mb.setValue(0);
        int j = 0;
        int toskip = sl.length() - baulk;
        int skipped = 0;
        QStringList::iterator k;
        for(k = sl2.begin(); k != sl2.end(); ++k) {
            if(toskip <= skipped) {//count down number required
                j = quietBash(*k);//so order is based on first undo does last
            } else {
                skipped++;
            }
            mb.setValue(mb.value() + 1);//update
            if(j != 0 || mb.wasCanceled()) break;//exit early
        }
        mb.hide();
    }
    hasRepo();
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
            QMessageBox::warning(this, tr("Cloud Publication Error"),
                     tr("The repository could not be published. "
                        "There maybe a complex data merge issue if many users "
                        "update the same documents. If you did not use SSH to "
                        "clone and used HTTPS instead, you may not have write "
                        "and update privilages. Are you a member of the repository "
                        "commit group? Is there anything to merge?"));
            return;
        }
    status(tr("Published all saved edits"));
    //reload();
    }
}

void MainWindow::read() {
    if(bash("git stash push -a || exit 0 &&&"
            "git pull &&&"
            "git stash pop || exit 0"
            ) != 0) { //incorporate
        QMessageBox::warning(this, tr("Cloud Reading Error"),
                 tr("The repository could not be read. "
                    "There maybe a complex data merge issue if many users "
                    "update the same documents."));
        return;
    }
    status(tr("Read all updates and restored saved edits"));
    reload();
}

void MainWindow::root() {
    prohibits();
    QFileDialog dialog(this, tr("Set Sync Working Directory"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setNameFilterDetailsVisible(false);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setLabelText(QFileDialog::Accept, tr("Set"));
    //dialog.setOption(QFileDialog::DontUseNativeDialog);

    if (dialog.exec() != QDialog::Accepted) {
        hasRepo();
        return;
    }
    if (!dialog.selectedFiles().first().isEmpty()) {
        directory = dialog.selectedFiles().first();
        status(tr("Working directory set"));
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
        QMessageBox::warning(this, tr("Subscription Error"),
                 tr("The repository could not be subscribed. "
                    "The working directory may have things in "
                    "which prevent a clone subscription as that "
                    "requires the working directory to be empty."));
        return;
    }
    status(tr("Subscription created by cloning repository"));
    reload();
}

bool MainWindow::hasRepo() {//and restore prohibits
    setDirectory(true);
    checkSave(textEdit->document()->isModified());//test save
    if(quietBash("git status") != 0) {
        if(quietBash("git --version") != 0) {
            //no git
            if(!hasGitTestShown) {
                QMessageBox::warning(this, tr("Git Availability Error"),
                         tr("Git is not on your system. Some features will not work."));
                hasGitTestShown = true;//only once
                return false;
            }
        }
        setClone(true);
        setSync(false);
        return false;
    }
    setSync(true);
    setClone(false);
    return true;
}

void MainWindow::prohibits() {//for bash IO locking
    setDirectory(false);
    setClone(false);
    setSync(false);
    setSave(false);//prohibit write
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
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
        QList<AViewWidget *>::iterator i;
        for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
            (*i)->recycle();
            (*i)->clear();
        }
    }
}

void MainWindow::open() {
    openBoth(false);
}

void MainWindow::openFix() {
    openBoth(true);
}

void MainWindow::openBoth(bool fix) {
    //asNewShow();
    if (maybeSave()) {
        QFileDialog dialog(this, tr("Open File"), directory);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        //dialog.setOption(QFileDialog::DontUseNativeDialog);

        QStringList kinds;
        QString base = textEdit->getBaseExtension();
        kinds << textEdit->getBaseTypeDescription() + " (*." +
                 base + ")";
        if(!fix) {
            QList<AViewWidget *>::iterator i;
            for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
                if((*i)->hasRegenerate()) {
                    kinds << (*i)->getViewName().remove("&") +
                             " (*." + base + "." + (*i)->getExtension() + ")";
                }
            }
        }
        dialog.setNameFilters(kinds);
        if (dialog.exec() != QDialog::Accepted)
            return;
        //TODO: maybe need to do view transform on type
        if (!dialog.selectedFiles().first().isEmpty()) {
            QString filter = dialog.selectedNameFilter();
            if(filter == kinds.first()) {
                loadFile(dialog.selectedFiles().first(), false, fix);
            } else {
                loadFile(dialog.selectedFiles().first(), true);//regenarate
            }
        }
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
    kinds << textEdit->getBaseTypeDescription() + " (*." +
             textEdit->getBaseExtension() + ")";
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
                         QString help, Spec option, AViewWidget *view) {
    static QMenu *aMenu;
    static QMenu *trayMenu = new QMenu(this);
    static QToolBar *aToolBar;
    static int count = 0;
    if(menu != nullptr) {
        aMenu = menuBar()->addMenu(menu);
        if(!(option & noAddBarThisMenu)) {
            if(aToolBar != nullptr && !(option & doOwnSpacerPrevious)) {
                //add separator at end unless following is not added
                //in this case the help menu surpresses the final
                //separator as it is a noAddBarThisMenu
                aToolBar->addSeparator();
            }
            aToolBar = addToolBar(menu.remove("&"));
            aToolBar->setMovable(false);
        }
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
        connect(newAct, &QAction::triggered, view, &AViewWidget::selectView);
        //newAct->setEnabled(false);
        connect(view, &AViewWidget::setAvailable, newAct, &QAction::setEnabled);
    }
    if(option & canCopy) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setCopy, newAct, &QAction::setEnabled);
    }
    if(option & canCut) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setCut, newAct, &QAction::setEnabled);
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
    if(option & auxNeedsText) {
        //newAct->setEnabled(false);
        connect(this, &MainWindow::setNeedsText, newAct, &QAction::setEnabled);
    }
    //more options
    if(option & noBar) return aMenu;
    if(option & afterBarSpace) {
        aToolBar->addSeparator();
    }
    aToolBar->addAction(newAct);
    return aMenu;
}

QMenu* MainWindow::addViewMenu(Spec option) {
    QList<AViewWidget *>::iterator i;
    QMenu *menu;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
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
    if(isTextMain()) {
        textEdit->undo();
        return;
    }
    ((AViewWidget *)getMain())->undo();
}

void MainWindow::redo() {
    if(isTextMain()) {
        textEdit->redo();
        return;
    }
    ((AViewWidget *)getMain())->redo();
}

void MainWindow::cut() {
    if(isTextMain()) {
        textEdit->cut();
        return;
    }
    ((AViewWidget *)getMain())->cut();
}

void MainWindow::copy() {
    if(isTextMain()) {
        textEdit->copy();
        return;
    }
    ((AViewWidget *)getMain())->copy();
}

void MainWindow::paste() {
    if(isTextMain()) {
        textEdit->paste();
        return;
    }
    ((AViewWidget *)getMain())->paste();
}

void MainWindow::find() {
    FindDialog find(this);
    find.setModal(true);
    find.setText(textEdit);
    if (find.exec() == QDialog::Accepted) {
        return;
    }
}

//===================================================
// VIEW MANAGEMENT
//===================================================
void MainWindow::viewText() {
    setMain(textEdit);
}

void MainWindow::viewSettings() {
    if(holdWhileSettings == settings) {
        holdWhileSettings = getMain();//for restore
        setMain(settings);
    } else {
        setMain(holdWhileSettings);
    }
}

void MainWindow::font() {
    bool ok;
    QFont f = QFontDialog::getFont(&ok, textEdit->font());
    textEdit->setFont(f);
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
    addMenu(nullptr, &MainWindow::openFix,
            "document-open", tr("UTF &Fix Open..."), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O),//+O
            tr("Open and fix the UTF-8 encoding of an existing file"), noBar);
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
    //menuBar()->addSeparator();

    addMenu(tr("&Edit"), &MainWindow::undo,
            "edit-undo", tr("&Undo"), QKeySequence::Undo,//Z
            tr("Undo the last edit"), noBar | canUndo);
    addMenu(nullptr, &MainWindow::redo,
            "edit-redo", tr("&Redo"), QKeySequence::Redo,//+Z
            tr("Redo the last undo"), noBar | canRedo)->addSeparator();
#ifndef QT_NO_CLIPBOARD
    addMenu(nullptr, &MainWindow::cut,
            "edit-cut",  tr("Cu&t"), QKeySequence::Cut,//X
            tr("Cut the current selection to the clipboard"), canCut);
    addMenu(nullptr, &MainWindow::copy,
            "edit-copy", tr("&Copy"), QKeySequence::Copy,//C
            tr("Copy the current selection to the clipboard"), canCopy);
    addMenu(nullptr, &MainWindow::paste,
            "edit-paste", tr("&Paste"), QKeySequence::Paste,//V
            tr("Paste the clipboard into the current selection"), canPaste)->addSeparator();


#endif // !QT_NO_CLIPBOARD
    addMenu(nullptr, &MainWindow::find,
            "edit-find", tr("&Find..."), QKeySequence::Find,//F
            tr("Find and maybe replace text"), auxNeedsText);
    addMenu(nullptr, &MainWindow::font,
            "edit-font", tr("Font &Change..."), QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F),//+F
            tr("Change font kind"), auxNeedsText | noBar);
    menuBar()->addSeparator();
    addMenu(tr("&View"), &MainWindow::viewText,
            "view-text", tr("&Text"), QKeySequence::AddTab,//T
            tr("Show editable text view"))->addSeparator();
    addViewMenu()->addSeparator();
    addMenu(nullptr, &MainWindow::viewSettings,
            "view-settings", tr("Settin&gs"), QKeySequence(Qt::CTRL + Qt::Key_G),//G
            tr("Show and hide settings view"), afterBarSpace);
    //menuBar()->addSeparator();
    addMenu(tr("&Command"));
    //menuBar()->addSeparator();

    addMenu(tr("&Sync"), &MainWindow::publish,
            "sync-publish", tr("&Publish"), QKeySequence::Print,//P
            tr("Publish with services"), canSync | doOwnSpacerPrevious);
    addMenu(nullptr, &MainWindow::read,
            "sync-read", tr("&Read"), QKeySequence(Qt::CTRL + Qt::Key_R),//R
            tr("Read from services"), canSync | inTray)->addSeparator();
    addMenu(nullptr, &MainWindow::root,
            "sync-directory", tr("Set &Dir Root..."), QKeySequence(Qt::CTRL + Qt::Key_D),//D
            tr("Set default working directory"), canSetdir | noBar);
    addMenu(nullptr, &MainWindow::subscribe,
            "sync-subscribe", tr("Subscribe C&lone..."), QKeySequence(Qt::CTRL + Qt::Key_L),//L
            tr("Subscribe to a remote git ssh repository and clone it"), canClone | noBar);
    //menuBar()->addSeparator();

    addMenu(tr("&Help"), &MainWindow::about,
            "help-about", tr("&About"), 0,
            tr("Show the application's About box"), noBar | noAddBarThisMenu);
    addMenu(nullptr, &MainWindow::aboutQt,
            "QtIcon", tr("About &Qt"), 0,
            tr("Show the Qt library's About box"), noBar);
}

void MainWindow::createStatusBar() {
    status(tr("Ready"));
}

//===================================================
// SETTINGS IO (DEFAULTS)
//===================================================
void MainWindow::readSettings() {
    const QByteArray geometry = settingsStore->value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    directory = settingsStore->value("directory", "").toString();
    textEdit->setFont(settingsStore->value("font", textEdit->font()).value<QFont>());
    QList<QToolBar *> toolbars = findChildren<QToolBar *>();
    while (!toolbars.isEmpty()) {
        QToolBar *tb = toolbars.takeFirst();
        bool visible = settingsStore->value("tbv" + tb->windowTitle(), true).toBool();
        tb->setVisible(visible);
    }
    setMain(settings);
    //settings->readSettings(settingsStore);
}

void MainWindow::writeSettings() {
    settingsStore->setValue("geometry", saveGeometry());
    settingsStore->setValue("directory", directory);
    settingsStore->setValue("font", textEdit->font());
    QList<QToolBar *> toolbars = findChildren<QToolBar *>();
    while (isVisible() && !toolbars.isEmpty()) {//prevents setting background mangling
        QToolBar *tb = toolbars.takeFirst();
        settingsStore->setValue("tbv" + tb->windowTitle(), tb->isVisible());
    }
    setMain(textEdit);//to restore settings to things
    //just incase settings is last view and not saved
    //this indirectly leads to the settings page to save state
}

//===================================================
// FILE IO
//===================================================
bool MainWindow::maybeSave(bool reload) {
    if (!textEdit->document()->isModified() && !reload)
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::question(this, tr("Save Changes"),
                               reload ?
                               tr("The document may have been modified by synchronization.\n"
                                  "Do you want to save any possible changes?") :
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               (reload ?
                                    QMessageBox::Save | QMessageBox::No
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

void MainWindow::loadFile(const QString &fileName, bool regen, bool fix) {
    setClone(false);
    setSync(false);
    QString name = fileName;
    QFile file(name);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, tr("File Read Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(file.fileName()),
                                  file.errorString()));
        hasRepo();
        return;
    }
    AViewWidget *me = nullptr;
    QString loaded;
    loadModified = false;
    if(fix & !regen) {//primary source fix, dependants more format based
        loaded = loadAllErrors(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    } else {
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        QTextStream in(&file);
        in.setCodec("UTF-8");
        loaded = in.readAll();
    }
    if(regen) {
        QList<AViewWidget *>::iterator i;
        for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
            if(QFileInfo(fileName).suffix().toLower() == (*i)->getExtension()) {
                me = (*i);
                break;
            }
        }
        me->cacheLoad(loaded);
        textEdit->setPlainText(me->regenerate());
        QStringList count = me->getExtension().split(".");
        QStringList nameBits = name.split(".");
        QStringList::iterator j;
        for(j = count.begin(); j != count.end(); ++i) {
            nameBits.removeLast();
        }
        name = nameBits.join(".");//removed extension xtra
    } else {
        textEdit->setPlainText(loaded);
    }
    setCurrentFile(name);
    if(loadModified) setModified();
    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        if((*i)->canCache() && (*i) != me) {
            QFile file(name + "." + (*i)->getExtension());
            if(!file.open(QFile::ReadOnly)) {
                /* QMessageBox::warning(this, tr("File Error"),
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(file.fileName()),
                                          file.errorString())); */
                //not actually an error, just a slow down
                //hasRepo();
                continue;
            }
            QTextStream in(&file);
            in.setCodec("UTF-8");
            (*i)->cacheLoad(in.readAll());//load file
        }
    }
    hasRepo();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    status(tr("File loaded"));
}

QString MainWindow::loadAllErrors(QFile *name) {
    QTextDecoder td(QTextCodec::codecForName("UTF-8"),
                    QTextCodec::ConvertInvalidToNull |
                    QTextCodec::IgnoreHeader);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    int size = name->size();
    QByteArray ba = name->read(size);
    UTFDialog utf(this);
    utf.setModal(true);
    utf.setBytes(ba);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    if (utf.exec() == QDialog::Accepted) {
        return td.toUnicode(utf.bytes());
    }
    return td.toUnicode(ba);
}

void MainWindow::setLoadModified() {
    loadModified = true;
}

void MainWindow::saveFile(const QString &fileName) {
    setClone(false);
    setSync(false);
    QString name;
    if(QFileInfo(fileName).suffix().toLower() != textEdit->getBaseExtension()) {
        name = QString(fileName + "." + textEdit->getBaseExtension());
    } else {
        name = fileName;
    }
    QFile file(name);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    if(textEdit->document()->isModified()) {
        if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
            QMessageBox::critical(this, tr("File Write Error"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(file.fileName()),
                                      file.errorString()));
        } else {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << textEdit->toPlainText();
            setCurrentFile(name);
        }
    }
    QList<AViewWidget *>::iterator i;
    for(i = listOfViews.begin(); i != listOfViews.end(); ++i) {
        if((*i)->needsSave()) {
            QFile file(name + "." + (*i)->getExtension());
            if (!file.open(QFile::WriteOnly)) {
                QMessageBox::critical(this, tr("File Write Error"),
                                     tr("Cannot write file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(file.fileName()),
                                          file.errorString()));
                //hasRepo();
                continue;
            }
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << (*i)->blockingSave();//save file
        }
    }
    hasRepo();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    status(tr("File saved"));
    return;
}

void MainWindow::setCurrentFile(const QString &fileName, bool noClear) {
    curFile = fileName;
    if(!noClear) {
        textEdit->document()->clearUndoRedoStacks();//makes more sense
        textEdit->document()->setModified(false);
        setWindowModified(false);
    }

    QString shownName = strippedName(curFile);
    if(curFile.isEmpty())
        shownName = "untitled." + textEdit->getBaseExtension();//default to text
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
