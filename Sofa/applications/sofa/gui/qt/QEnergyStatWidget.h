/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GUI_QT_QENERGYSTATWIDGET_H
#define SOFA_GUI_QT_QENERGYSTATWIDGET_H

#include "QGraphStatWidget.h"

#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/behavior/BaseMass.h>


namespace sofa
{
namespace gui
{
namespace qt
{

class QEnergyStatWidget : public QGraphStatWidget
{

    Q_OBJECT

public:

    QEnergyStatWidget( QWidget* parent, simulation::Node* node ) : QGraphStatWidget( parent, node, "Energy", 3 )
    {
        setCurve( 0, "Kinetic", Qt::red );
        setCurve( 1, "Potential", Qt::green );
        setCurve( 2, "Mechanical", Qt::blue );
    }

    void step()
    {
        //Add Time
        QGraphStatWidget::step();

        //Add Kinetic Energy
        if( _node->mass )
            _YHistory[0].push_back( _node->mass->getKineticEnergy() );
        else
            _YHistory[0].push_back(0);

        //Add Potential Energy
        double potentialEnergy=0;
        typedef sofa::simulation::Node::Sequence<core::behavior::BaseForceField> SeqFF;
        for( SeqFF::iterator it = _node->forceField.begin() ; it != _node->forceField.end() ; ++it )
        {
            potentialEnergy += (*it)->getPotentialEnergy();
        }
        _YHistory[1].push_back( potentialEnergy );

        //Add Mechanical Energy
        _YHistory[2].push_back( _YHistory[0].back() + _YHistory[1].back() );
    }

};


} // qt
} // gui
} //sofa

#endif // SOFA_GUI_QT_QDATADESCRIPTIONWIDGET_H
