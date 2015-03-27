#ifndef IRUI_UTILITY_HPP
#define IRUI_UTILITY_HPP

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

//------------------------------------------------------------------------------
//

// Qt
#include <QGlobal.h>

// Iron
#include <iron.hpp>

//------------------------------------------------------------------------------
//

template< class T >
EN_INLINE T randRange( T low,
                       T high )
{
    // Random number between low and high
    return qrand() % ( ( high + 1 ) - low ) + low;
}

#endif // IRUI_UTILITY_HPP
