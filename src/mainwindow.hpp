#ifndef IRUI_MAINWINDOW_HPP
#define IRUI_MAINWINDOW_HPP

// Copyright (C) 2015 Engine Development
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Note: Source code incorporated from 'QNodesEditor':
//     http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//
// Copyright (c) 2012, STANISLAW ADASZEWSKI
//

//------------------------------------------------------------------------------
//

// Qt
#include <QMainWindow>

//------------------------------------------------------------------------------
//

class Sketch;
class TypeStore;

//------------------------------------------------------------------------------
//

class MainWindow
    : public QMainWindow
{
	Q_OBJECT

public:

    explicit MainWindow( QWidget* parent = 0 );

    ~MainWindow();

private slots:

    bool newFile();

    bool openFile();

    bool saveFile();

    bool saveAsFile();

    bool closeFile();

private:

    void updateWindowTitle();

    QMenu* m_fileMenu;
    QString m_fileName;
    Sketch* m_sketch;
    TypeStore* m_typeStore;

};

#endif // IRUI_MAINWINDOW_HPP
