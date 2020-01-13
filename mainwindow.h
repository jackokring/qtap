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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextDocument>
#include <QStackedWidget>
#include <QSystemTrayIcon>
#include <QProgressDialog>
#include "atextedit.h"
#include "./libkqfn/libkqfn.h"
#include "settings.h"
#include "aviewwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

//===================================================
// MENU SPECIAL AUTO BUILDING
//===================================================
enum Spec : unsigned int {
    none = 0,//for completeness
    noBar = 1,//no icon in action bar
    canCopy = 2,//can copy a selection
    canPaste = 4,//can do a paste into somewhere
    canUndo = 8,//needs undo to exist
    canRedo = 16,//needs redo to exist
    canSave = 32,//operates file IO
    //load is controlled sufficiently for read while write new
    inTray = 64,//placed in system tray menu
    canClone = 128,//is a git clone operation
    canSync = 256,//is a git sync operation
    canSetdir = 512,//action can set root directory
    canCut = 1024,//action needs something to cut
    auxNeedsText = 2048,//only active when text viewed
    afterBarSpace = 4096,//adds a space before this action in the bar
    noAddBarThisMenu = 8192,//prevents a new bar section adding
    doOwnSpacerPrevious = 16384,//useful to prevent command malformat
};

class MainWindow : public QMainWindow {
    Q_OBJECT
    //===================================================
    // HELP
    //===================================================
private slots:
    void aboutQt();
    void about();

    //===================================================
    // MAIN WINDOW MANAGEMENT
    //===================================================
public:
    void setMain(QWidget *widget);
    QWidget *getMain();
    bool isTextMain();
    bool isSettingsMain();
    MainWindow();
    QString loadStyle();
    QWidget *getQWebEngineView();
    void setCommand(QAction *action, AViewWidget *view);
    void setInputCommand(ATextEdit *input);
    void fillCommands();
    void setInBackground(QString view, QString command);
    QString getText();
    static QWidget *focused(QWidget *top);
    void status(QString display);
    void setModified();

    //===================================================
    // PROXY ENABLE ACTION CHECKS
    //===================================================
private slots:
    void checkPaste();//for paste
    void checkCopy(bool active);//for copy
    void checkCut(bool active);//for cut
    void checkUndo(bool active);
    void checkRedo(bool active);
    void checkSave(bool active);
    void checkTray(QSystemTrayIcon::ActivationReason reason);
    void checkAvailable(bool notSaved);
    void checkEvents();

    //===================================================
    // GIT MANAGEMENT
    //===================================================
    int quietBash(QString proc);
    int bash(QString proc, QString undo = nullptr);
    void publish();
    void read();
    void root();
    void subscribe();
    bool hasRepo();
    void prohibits();

    //===================================================
    // NEW, OPEN AND SAVE ACTIONS
    //===================================================
    void asNewShow();
    void newFile();
    void open();
    void openFix();
    void openBoth(bool fix);
    void save();
    void saveAs();
    void reload();

    void close();
    //void documentWasModified();

    //===================================================
    // MENU AND ICON UTILITIES
    //===================================================
public:
    static QIcon getIconRC(QString named);
private:
    QMenu* addMenu(QString menu = nullptr, void(MainWindow::*fp)() = nullptr,
         QString named = nullptr, QString entry = nullptr, QKeySequence shorty = 0,
         QString help = nullptr, Spec option = none, AViewWidget *view = nullptr);
    QMenu* addViewMenu(Spec option = none);

    //===================================================
    // BASIC PROXY ACTIONS
    //===================================================
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void find();

    //===================================================
    // VIEW MANAGEMENT
    //===================================================
    void viewText();
    void viewSettings();
    void font();

    //===================================================
    // MENU AND STATUS BAR CREATION
    //===================================================
private:
    void createActions();
    void createStatusBar();

    //===================================================
    // SETTINGS IO (DEFAULTS)
    //===================================================
    void readSettings();
    void writeSettings();

    //===================================================
    // FILE IO
    //===================================================
    bool maybeSave(bool reload = false);
public:
    void loadFile(const QString &fileName, bool regen = false, bool fix = false);
    QString loadAllErrors(QFile *name);
private:
    void saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

    //===================================================
    // SIGNALS FOR SENDING PROXIED CHECKS AND SUCH LIKE
    //===================================================
signals:
    void setPaste(bool active);
    void setCopy(bool active);
    void setCut(bool active);
    void setUndo(bool active);
    void setRedo(bool active);
    void setSave(bool active);
    void setSync(bool active);
    void setClone(bool active);
    void setDirectory(bool active);
    void setNeedsText(bool active);

    //===================================================
    // CONFIGURATION VARIABLES
    //===================================================
private:
    ATextEdit *textEdit;//the main edit area
    QString curFile;//current file name
    bool saved;//used in save cancel logic
    Libkqfn *handle;//library handle
    QString directory;//directory default for open save etc
    QWidget *holdWhileSettings;//prevents edits while edit not displayed
    Settings *settings;//a settings UI
    QStackedWidget *center;//multi view handling
    QSystemTrayIcon *tray;
    bool exitCheck;
    QList<AViewWidget *> listOfViews;
    //lasts
    bool lastCopy = false;
    bool lastCut = false;
    bool lastUndo = false;
    bool lastRedo = false;
    QMap<AViewWidget *, QList<QAction *>> inViewActions;
    QList<QAction *> inInputActions;
    QMenu *commands;
    QToolBar *commandToolBar;
    bool backgrounded = false;
    QSettings *settingsStore;
    bool hasGitTestShown = false;
};
#endif
