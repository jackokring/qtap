#ifndef SETTINGS_H
#define SETTINGS_H

#include "aviewwidget.h"

namespace Ui {
class Settings;
}

class Settings : public AViewWidget {//inherit
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    //NOT ALL GENERALITY OF AVIEWWIDGET NEEDED FOR SETTINGS
    void readSettings(QSettings *settings) override;
    void writeSettings(QSettings *settings) override;

    //===================================================
    // ACTION
    //===================================================
    bool canCut() override;//can currently cut
    bool canCopy() override;//can currently copy
    bool canPaste() override;//can currently paste
    bool canUndo() override;//can currently undo
    bool canRedo() override;//can currently redo
    void cut() override;
    void copy() override;
    void paste() override;
    void undo() override;
    void redo() override;

    //===================================================
    // USEFUL
    //===================================================
    void readSettings(QSettings *settings, AViewWidget *view);
    void writeSettings(QSettings *settings, AViewWidget *view);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
