/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/core/ObjectFactory.h>
#include <sofa/component/loader/MeshTrianLoader.h>

//#include <sofa/helper/system/FileRepository.h>
//#include <stdlib.h>
#include <iostream>
//#include <string>

//#include <cstdio>

namespace sofa
{

namespace component
{

namespace loader
{

using namespace sofa::defaulttype;

SOFA_DECL_CLASS(MeshTrianLoader)

  int MeshTrianLoaderClass = core::RegisterObject("Specific mesh loader for trian (only triangulations) file format.")
.add< MeshTrianLoader >()
;
  
  MeshTrianLoader::MeshTrianLoader() : MeshLoader()
				     , neighborTable(initData(&neighborTable,"neighborTable","Table of neighborhood triangle indices for each triangle."))
				     , edgesOnBorder(initData(&edgesOnBorder,"edgesOnBorder","List of edges which are on the border of the mesh loaded."))
				     , trianglesOnBorderList(initData(&trianglesOnBorderList,"trianglesOnBorderList","List of triangle indices which are on the border of the mesh loaded."))
  {
    neighborTable.setPersistent(false);
    edgesOnBorder.setPersistent(false);
    trianglesOnBorderList.setPersistent(false);
  }
  
  

bool MeshTrianLoader::load()
{
  std::cout << "Loading Trian file: " << m_filename << std::endl;

  FILE* file;
  bool fileRead = false;
  
  // -- Loading file
  const char* filename = m_filename.getFullPath().c_str();
  if ((file = fopen(filename, "r")) == NULL)
  {
    std::cerr << "Error: MeshTrianLoader: Cannot read file '" << m_filename << "'." << std::endl;
    return false;
  }

  // -- Reading file
  fileRead = this->readTrian (filename);

  fclose (file);
  return fileRead;
}
  


  bool MeshTrianLoader::readTrian (const char* filename)
{
  std::ifstream dataFile (filename);
   


    // --- data used in trian files ---
    unsigned int nbVertices = 0;
    unsigned int nbTriangles = 0;
    sofa::helper::vector <unsigned int> the_edge;
    the_edge.resize (2);
          

    // --- Loading Vertices positions ---
    dataFile >> nbVertices; //Loading number of Vertex

    helper::vector<sofa::defaulttype::Vector3>& my_positions = *(positions.beginEdit());
    for (unsigned int i=0; i<nbVertices; ++i)
    {
      double x,y,z;

      dataFile >> x >> y >> z;
      
      my_positions.push_back (Vector3(x, y, z));
    }
    positions.endEdit();

    // --- Loading Triangles array ---
    dataFile >> nbTriangles; //Loading number of Triangle

    helper::vector<helper::fixed_array <unsigned int,3> >& my_triangles = *(triangles.beginEdit());
    helper::vector<helper::fixed_array <int,3> >& my_neighborTable = *(neighborTable.beginEdit());
    helper::vector<helper::vector <unsigned int> >& my_edgesOnBorder = *(edgesOnBorder.beginEdit());
    helper::vector<unsigned int>& my_trianglesOnBorderList = *(trianglesOnBorderList.beginEdit());
    
    for (unsigned int i=0; i<nbTriangles; ++i)
    {
      helper::fixed_array <unsigned int,3> nodes;
      helper::fixed_array <int,3> ngh;
      
      dataFile >>  nodes[0] >> nodes[1] >> nodes[2] >> ngh[0] >> ngh[1] >> ngh[2];

      // set 3 triangle vertices  ==>> Dans le MeshLoader?

      my_triangles.push_back (nodes);
      my_neighborTable.push_back (ngh);

      // if we have a boundary edge store it in the m_edgeOnBorder array:
      if (ngh[0] == -1) // TODO : verifier la correspondance edgeArray et vectexIndices
      {
	the_edge[0] = nodes[1];
	the_edge[1] = nodes[2];

	my_edgesOnBorder.push_back (the_edge);
      }
      
      if (ngh[1] == -1) // TODO : verifier la correspondance edgeArray et vectexIndices
      {
	the_edge[0] = nodes[2];
	the_edge[1] = nodes[0];

	my_edgesOnBorder.push_back (the_edge);
      }

      if (ngh[2] == -1) // TODO : verifier la correspondance edgeArray et vectexIndices
      {
	the_edge[0] = nodes[0];
	the_edge[1] = nodes[1];

	my_edgesOnBorder.push_back (the_edge);
      }


      // if we have a boundary triangle store it (ONLY ONCE) in the m_triangleOnBorderList array:
      for (unsigned int j=0; j<3; ++j)
	if (ngh[j] == -1)
	{
	  my_trianglesOnBorderList.push_back (i);
	  break;
	}


      
      /*
      if ((v0<v1)|| (ngh2== -1))
      {
	std::cout << " ((v0<v1)|| (ngh2== -1)) " << std::endl;
	
	e=new E(trian,vertexTable[v0],vertexTable[v1],t,0);
	t->setEdge(2,e);
	// if we have a boundary edge store it in the vertex
	if (ngh2== -1)
	{
	  t->getVertex(1)->setEdge(e);
	  t->getVertex(1)->setOrientation(1);
	}
      }
      if ((v1<v2) | (ngh0== -1))
      {
	std::cout << " ((v1<v2) | (ngh0== -1)) " << std::endl;
	e=new E(trian,vertexTable[v1],vertexTable[v2],
		t,0);
	t->setEdge(0,e);
	// if we have a boundary edge store it in the vertex
	if (ngh0== -1)
	{
	  t->getVertex(2)->setEdge(e);
	  t->getVertex(2)->setOrientation(1);
	}
	
      }
      if ((v2<v0)| (ngh1== -1))
      {
	std::cout << " ((v2<v0)| (ngh1== -1)) " << std::endl;
	e=new E(trian,vertexTable[v2],vertexTable[v0],
		t,0);
	t->setEdge(1,e);
	// if we have a boundary edge store it in the vertex
	if (ngh1== -1)
	{
	  t->getVertex(0)->setEdge(e);
	  t->getVertex(0)->setOrientation(1);
	}

      }
      */
      
    }

    triangles.endEdit();
    neighborTable.endEdit();
    trianglesOnBorderList.endEdit();
    edgesOnBorder.endEdit();
    
    //MeshLoader job now
    //trian->computeCenter();
    //createVirtualVertices(vv,ee,tt,zz,trian);

    return true;
  }



} // namespace loader

} // namespace component

} // namespace sofa

