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
#include "mainwindow.h"

//===================================================
// HELP
//===================================================
void MainWindow::aboutQt() {
    qApp->aboutQt();
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QtAp"),
            tr("<b>QtAp</b> for document control. "
               "Writen in C++ using Qt. "
               "It requires Git for sync to work. "));
}

//===================================================
// MAIN WINDOW MANAGEMENT
//===================================================
void MainWindow::setMain(QWidget *widget) {
    if(center->indexOf(widget) == -1) {
        center->addWidget(widget);
    }
    center->setCurrentWidget(widget);

    //plus some repair?
    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);
    connect(textEdit, &QPlainTextEdit::copyAvailable,
            this, &MainWindow::checkSelected);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            this, &MainWindow::checkUndo);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            this, &MainWindow::checkRedo);
    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &MainWindow::checkSave);
}

MainWindow::MainWindow()
    : textEdit(new ATextEdit(this)) {
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    setStyleSheet(loadStyle());

    QIcon::setThemeName("gnome");//TODO maybe become a setting
    QIcon ico = getIconRC("view-text");
    setWindowIcon(ico);
    tray = new QSystemTrayIcon(this);
    tray->setIcon(ico);
    tray->setToolTip(QCoreApplication::applicationName());
    connect(tray, &QSystemTrayIcon::activated, this, &MainWindow::checkTray);
    createActions();
    createStatusBar();
    readSettings();

    center = new QStackedWidget(this);
    setCentralWidget(center);

    holdWhileSettings = false;
    settings = new Settings();

    setMain(textEdit);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

QString MainWindow::loadStyle() {
    QFile file(":/style.css");
    if(!file.open(QFile::ReadOnly)) return "";
    QTextStream in(&file);
    return in.readAll();
}

//===================================================
// PROXY ENABLE ACTION CHECKS
//===================================================
void MainWindow::checkClipboard() {
    bool hasText = false;
    if(!holdWhileSettings) //text view
        if(QGuiApplication::clipboard()->mimeData()->hasText() &&
                QGuiApplication::clipboard()->text().length() > 0) {//check paste sensible
            hasText = true;
        }
    setPaste(hasText);
}

void MainWindow::checkSelected(bool active) {
    if(holdWhileSettings) {//inhibit
        active = false;
    }
    setCopy(active);
}

void MainWindow::checkUndo(bool active) {
    if(holdWhileSettings) {//inhibit
        active = false;
    }
    setUndo(active);
}

void MainWindow::checkRedo(bool active) {
    if(holdWhileSettings) {//inhibit
        active = false;
    }
    setRedo(active);
}

void MainWindow::checkSave(bool active) {
    setSave(active);
}

void MainWindow::checkTray(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
    }
}

//===================================================
// GIT MANAGEMENT
//===================================================
void MainWindow::publish() {
    maybeSave();
    QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss");
    if(QProcess::execute("cd " + directory + "&&"
        "git add .&&git stash&&git pull&&git stash pop&&" //incorporate
        "git commit -m \"" + now + "\"&&git push") != 0) {
        QMessageBox::critical(this, tr("Publication Error"),
                 tr("The repository could not be published."));
        return;
    }
    statusBar()->showMessage(tr("Published"), 2000);
}

void MainWindow::subscribe() {
    if(QProcess::execute("cd " + directory + "&&"
        "git add .&&git stash&&git pull&&git stash pop") != 0) { //incorporate
        QMessageBox::critical(this, tr("Reading Error"),
                 tr("The repository could not be read."));
        return;
    }
    statusBar()->showMessage(tr("Read"), 2000);
}

void MainWindow::setRepo() {
    QFileDialog dialog(this, tr("Set Sync"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setNameFilterDetailsVisible(false);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setLabelText(QFileDialog::Accept, tr("Set"));
    dialog.setOption(QFileDialog::DontUseNativeDialog);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (!dialog.selectedFiles().first().isEmpty())
        directory = dialog.selectedFiles().first();
}

//===================================================
// NEW, OPEN AND SAVE ACTIONS
//===================================================
void MainWindow::newFile() {
    if (maybeSave()) {//TODO: maybe make new doc
        textEdit->clear();
        //TODO: select base type
        setCurrentFile(QString());
    }
}

void MainWindow::open() {
    if (maybeSave()) {
        QFileDialog dialog(this, tr("Open File"), directory);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setOption(QFileDialog::DontUseNativeDialog);

        QStringList mimeTypes;
        mimeTypes << "text/plain";
        dialog.setMimeTypeFilters(mimeTypes);
        if (dialog.exec() != QDialog::Accepted)
            return;
        //TODO: maybe need to do view transform on type
        if (!dialog.selectedFiles().first().isEmpty())
            loadFile(dialog.selectedFiles().first());
    }
}

void MainWindow::save() {
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
    dialog.setOption(QFileDialog::DontUseNativeDialog);

    QStringList mimeTypes;
    mimeTypes << "text/plain";
    dialog.setMimeTypeFilters(mimeTypes);
    saved = true;
    if (dialog.exec() != QDialog::Accepted) {
        saved = false;
        return;
    }
    //TODO: maybe need to do view transform on type
    saveFile(dialog.selectedFiles().first());
}

void MainWindow::documentWasModified() {
    setWindowModified(textEdit->document()->isModified());
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
                         QString help, Spec option) {
    static QMenu *aMenu;
    static QMenu *trayMenu = new QMenu(this);
    static QToolBar *aToolBar;
    if(menu != nullptr) {
        aMenu = menuBar()->addMenu(menu);
        aToolBar = addToolBar(menu.replace('&', ""));
    }
    if(aMenu == nullptr) {
        aMenu = menuBar()->addMenu("&Menu");
        aToolBar = addToolBar("Menu");
    }

    if(named == nullptr) {
        named = QString("void");
        option |= noBar;//as sensible
    }
    const QIcon newIcon = getIconRC(named);
    if(entry == nullptr) entry = "Blank";
    QAction *newAct = new QAction(newIcon, entry, this);
    if(shorty > 0) newAct->setShortcut(shorty);
    if(help != nullptr) newAct->setStatusTip(help);
    if(fp != nullptr) connect(newAct, &QAction::triggered, this, fp);
    aMenu->addAction(newAct);
    if(option & inTray) {
        tray->setContextMenu(trayMenu);
        tray->setVisible(true);
        trayMenu->addAction(newAct);
    }
    if(option & canCopy) {
        newAct->setEnabled(false);
        connect(this, &MainWindow::setCopy, newAct, &QAction::setEnabled);
    }
    if(option & canUndo) {
        newAct->setEnabled(false);
        connect(this, &MainWindow::setUndo, newAct, &QAction::setEnabled);
    }
    if(option & canRedo) {
        newAct->setEnabled(false);
        connect(this, &MainWindow::setRedo, newAct, &QAction::setEnabled);
    }
    if(option & canPaste) {
        newAct->setEnabled(false);
        connect(QGuiApplication::clipboard(),
                &QClipboard::dataChanged, this, &MainWindow::checkClipboard);
        connect(this, &MainWindow::setPaste, newAct, &QAction::setEnabled);
    }
    if(option & canSave) {
        newAct->setEnabled(false);
        connect(this, &MainWindow::setSave, newAct, &QAction::setEnabled);
    }
    //more options
    if(option & noBar) return aMenu;
    aToolBar->addAction(newAct);
    return aMenu;
}

//===================================================
// BASIC PROXY ACTIONS
//===================================================
void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::close() {
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
    //TODO
}

void MainWindow::viewSettings() {
    //TODO restore text etc
    if(!holdWhileSettings) {
        setMain(settings);
        holdWhileSettings = true;
    } else {
        setMain(textEdit);
        //textEdit->setFocus();//to front
        holdWhileSettings = false;
    }
}

//===================================================
// MENU AND STATUS BAR CREATION
//===================================================
void MainWindow::createActions() {
    addMenu(tr("&File"), &MainWindow::newFile,
            "document-new", tr("&New"), QKeySequence::New,//N
            tr("Create a new file"));
    addMenu(nullptr, &MainWindow::open,
            "document-open", tr("&Open..."), QKeySequence::Open,//O
            tr("Open an existing file"));
    addMenu(nullptr, &MainWindow::save,
            "document-save", tr("&Save"), QKeySequence::Save,//S
            tr("Save the document to disk"), canSave);
    addMenu(nullptr, &MainWindow::saveAs,
            "document-save-as", tr("Save &As..."), QKeySequence::SaveAs,//+S
            tr("Save the document under a new name"), noBar | canSave)->addSeparator();//no bar entry
    addMenu(nullptr, &MainWindow::close,
            "application-exit", tr("E&xit"), QKeySequence::Quit,//Q
            tr("Exit the application"), noBar | inTray);//no bar entry
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
    addMenu(nullptr, &MainWindow::viewSettings,
            "system-run", tr("Settin&gs"), QKeySequence(Qt::CTRL + Qt::Key_G),
            tr("Show and hide settings view"));
    menuBar()->addSeparator();

    addMenu(tr("&Sync"), &MainWindow::publish,
            "sync-publish", tr("&Publish"), QKeySequence::Print,//P
            tr("Publish with services"));
    addMenu(nullptr, &MainWindow::subscribe,
            "sync-read", tr("&Read"), QKeySequence(Qt::CTRL + Qt::Key_R),
            tr("Read from services"))->addSeparator();
    addMenu(nullptr, &MainWindow::setRepo,
            nullptr, tr("Set &Directory..."), QKeySequence(Qt::CTRL + Qt::Key_D),
            tr("Set work and synchronization directory"));
    addMenu(nullptr, nullptr,
            nullptr, tr("C&lone Remote..."), QKeySequence(Qt::CTRL + Qt::Key_L),
            tr("Clone a remote repository"));
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
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(),
                       QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("directory", directory);
}

//===================================================
// FILE IO
//===================================================
bool MainWindow::maybeSave() {
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Save Changes"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
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
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
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
                             .arg(QDir::toNativeSeparators(name),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(name);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if(curFile.isEmpty())
        shownName = "untitled.txt";//default to text
    //TODO: set default as note taking
    setWindowFilePath(shownName);
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
