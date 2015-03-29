#ifndef IRUI_SKETCH_HPP
#define IRUI_SKETCH_HPP

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
#include <QTabWidget>

//------------------------------------------------------------------------------
//

class Graph;

//------------------------------------------------------------------------------
//

class Sketch
    : public QTabWidget
{
    Q_OBJECT

public:

    Sketch( QWidget* parent = 0x0 );

    ~Sketch();

    void clear();

    Graph* setupGraph();

    Graph* loopGraph();

private:

    Graph* m_setupGraph;
    Graph* m_loopGraph;

};

//------------------------------------------------------------------------------
//

bool write( Sketch* sketch,
            std::string path );

//------------------------------------------------------------------------------
//

bool read( const std::string& path,
           Sketch* sketch );

#endif // IRUI_SKETCH_HPP
