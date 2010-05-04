/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
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
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GRAPHHISTORYMANAGER_H
#define SOFA_GRAPHHISTORYMANAGER_H


#include <sofa/core/objectmodel/Base.h>
#include <sofa/simulation/tree/GNode.h>

#include <qobject.h>
#include <vector>

namespace sofa
{

  namespace gui
  {

    namespace qt
    {

        using sofa::core::objectmodel::Base;
        using sofa::simulation::tree::GNode;

        class GraphModeler;

        class GraphHistoryManager: public QObject
        {
            Q_OBJECT            
        public:
            //-----------------------------------------------------------------------------//
            //Historic of actions: management of the undo/redo actions
            ///Basic class storing information about the operation done
            class Operation
            {
            public:
                Operation(){};
                enum op{DELETE_OBJECT,DELETE_GNODE, ADD_OBJECT,ADD_GNODE, NODE_MODIFICATION, COMPONENT_MODIFICATION};
                Operation(Base* sofaComponent_,  op ID_): sofaComponent(sofaComponent_), above(NULL), ID(ID_)
                {}

                Base* sofaComponent;
                GNode* parent;
                Base* above;
                op ID;
                std::string info;
            };

            GraphHistoryManager(GraphModeler *);
            ~GraphHistoryManager();

            bool isUndoEnabled() const {return !historyOperation.empty();}
            bool isRedoEnabled() const {return !historyUndoOperation.empty();}

        public slots:
            void operationPerformed(GraphHistoryManager::Operation&);
            void undo();
            void redo();
            void graphClean();
            void beginModification(sofa::core::objectmodel::Base* object);
            void endModification(sofa::core::objectmodel::Base* object);
        signals:
            void graphModified(bool);
            void undoEnabled(bool);
            void redoEnabled(bool);
            void historyMessage(const std::string&);
        protected:
            void clearHistoryUndo();
            void clearHistory();

            void undoOperation(Operation &);
            std::string componentState(Base *base) const;
            std::string setComponentState(Base *base, const std::string &datasStr);

            std::vector< Operation > historyOperation;
            std::vector< Operation > historyUndoOperation;

            std::map<Base*, std::string> componentPriorModificationState;

            GraphModeler *graph;
      };

    }
  }
}

#endif
