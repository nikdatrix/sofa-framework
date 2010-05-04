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
#ifndef SOFA_COMPONENT_COLLISION_TRIANGLEMODEL_INL
#define SOFA_COMPONENT_COLLISION_TRIANGLEMODEL_INL

#include <sofa/component/collision/TriangleModel.h>
#include <sofa/component/collision/TriangleLocalMinDistanceFilter.h>
#include <sofa/component/collision/CubeModel.h>
#include <sofa/component/collision/Triangle.h>
#include <sofa/component/topology/TriangleData.inl>
#include <sofa/simulation/common/Node.h>
#include <sofa/component/topology/RegularGridTopology.h>
#include <sofa/core/CollisionElement.h>
#include <vector>
#include <sofa/helper/gl/template.h>
#include <iostream>

#include <sofa/component/topology/PointSetTopologyChange.h>
#include <sofa/component/topology/TriangleSetTopologyChange.h>

#include <sofa/simulation/common/Simulation.h>



namespace sofa
{

namespace component
{

namespace collision
{

template<class DataTypes>
TTriangleModel<DataTypes>::TTriangleModel()
: bothSide(initData(&bothSide, false, "bothSide", "activate collision on both side of the triangle model") )
, mstate(NULL)
, computeNormals(initData(&computeNormals, true, "computeNormals", "set to false to disable computation of triangles normal"))
, meshRevision(-1)
, m_lmdFilter(NULL)
{
    triangles = &mytriangles;
}

template<class DataTypes>
void TTriangleModel<DataTypes>::resize(int size)
{
    this->core::CollisionModel::resize(size);
    //helper::vector<TriangleInfo>& e = *(elems.beginEdit());
    //e.resize(size);
    //elems.endEdit();
    normals.resize(size);
}

template<class DataTypes>
void TTriangleModel<DataTypes>::init()
{
	_topology = this->getContext()->getMeshTopology();

    this->CollisionModel::init();
    mstate = dynamic_cast< core::componentmodel::behavior::MechanicalState<DataTypes>* > (this->getContext()->getMechanicalState());

	this->getContext()->get(mpoints);

    if (mstate==NULL)
    {
        serr << "TriangleModel requires a Vec3 Mechanical Model" << sendl;
        return;
    }

	if (!_topology) 
	{
        serr << "TriangleModel requires a BaseMeshTopology" << sendl;
        return;
    }

	simulation::Node* node = dynamic_cast< simulation::Node* >(this->getContext());
	if (node != 0)
	{
		m_lmdFilter = node->getNodeObject< TriangleLocalMinDistanceFilter >();
	}

	//sout << "INFO_print : Col - init TRIANGLE " << sendl;
	 sout << "TriangleModel: initially "<<_topology->getNbTriangles()<<" triangles." << sendl;
	triangles = &_topology->getTriangles();
	resize(_topology->getNbTriangles());

    updateFromTopology();
    updateNormals();
}

template<class DataTypes>
void TTriangleModel<DataTypes>::updateNormals()
{
    for (int i=0;i<size;i++)
    {
        Element t(this,i);
        const Vector3& pt1 = t.p1();
        const Vector3& pt2 = t.p2();
        const Vector3& pt3 = t.p3();

        t.n() = cross(pt2-pt1,pt3-pt1);
        t.n().normalize();
        //sout << i << " " << t.n() << sendl;
    }
}

template<class DataTypes>
void TTriangleModel<DataTypes>::updateFromTopology()
{
//    needsUpdate = false;
    const unsigned npoints = mstate->getX()->size();
    const unsigned ntris = _topology->getNbTriangles();
    const unsigned nquads = _topology->getNbQuads();
    const unsigned newsize = ntris+2*nquads;

    int revision = _topology->getRevision();
    if (revision == meshRevision && newsize==(unsigned)size) {
        return;
    }
    needsUpdate=true;

    resize(newsize);

    if (newsize == ntris)
    { // no need to copy the triangle indices
        triangles = & _topology->getTriangles();
    }
    else
    {
        triangles = &mytriangles;
        mytriangles.resize(newsize);
        int index = 0;
        for (unsigned i=0; i<ntris; i++)
        {
            topology::BaseMeshTopology::Triangle idx = _topology->getTriangle(i);
            if (idx[0] >= npoints || idx[1] >= npoints || idx[2] >= npoints)
            {
                serr << "ERROR: Out of range index in triangle "<<i<<": "<<idx[0]<<" "<<idx[1]<<" "<<idx[2]<<" ( total points="<<npoints<<")"<<sendl;
                if (idx[0] >= npoints) idx[0] = npoints-1;
                if (idx[1] >= npoints) idx[1] = npoints-1;
                if (idx[2] >= npoints) idx[2] = npoints-1;
            }
            mytriangles[index] = idx;
            ++index;
        }
        for (unsigned i=0; i<nquads; i++)
        {
            topology::BaseMeshTopology::Quad idx = _topology->getQuad(i);
            if (idx[0] >= npoints || idx[1] >= npoints || idx[2] >= npoints || idx[3] >= npoints)
            {
                serr << "ERROR: Out of range index in quad "<<i<<": "<<idx[0]<<" "<<idx[1]<<" "<<idx[2]<<" "<<idx[3]<<" ( total points="<<npoints<<")"<<sendl;
                if (idx[0] >= npoints) idx[0] = npoints-1;
                if (idx[1] >= npoints) idx[1] = npoints-1;
                if (idx[2] >= npoints) idx[2] = npoints-1;
                if (idx[3] >= npoints) idx[3] = npoints-1;
            }
            mytriangles[index][0] = idx[1];
            mytriangles[index][1] = idx[2];
            mytriangles[index][2] = idx[0];
            ++index;
            mytriangles[index][0] = idx[3];
            mytriangles[index][1] = idx[0];
            mytriangles[index][2] = idx[2];
            ++index;
        }
    }
    updateFlags();
    updateNormals();
    meshRevision = revision;
}

template<class DataTypes>
void TTriangleModel<DataTypes>::updateFlags(int /*ntri*/)
{
#if 0
    if (ntri < 0) ntri = triangles->size();
    //VecCoord& x = *mstate->getX();
    //VecDeriv& v = *mstate->getV();
    vector<bool> pflags(mstate->getSize());
    std::set<std::pair<int,int> > eflags;
    for (unsigned i=0; i<triangles->size(); i++)
    {
        int f = 0;
        topology::Triangle t = (*triangles)[i];
        if (!pflags[t[0]])
        {
            f |= FLAG_P1;
            pflags[t[0]] = true;
        }
        if (!pflags[t[1]])
        {
            f |= FLAG_P2;
            pflags[t[1]] = true;
        }
        if (!pflags[t[2]])
        {
            f |= FLAG_P3;
            pflags[t[2]] = true;
        }
        if (eflags.insert( (t[0]<t[1])?std::make_pair(t[0],t[1]):std::make_pair(t[1],t[0]) ).second)
        {
            f |= FLAG_E12;
        }
        if (i < (unsigned)ntri && eflags.insert( (t[1]<t[2])?std::make_pair(t[1],t[2]):std::make_pair(t[2],t[1]) ).second) // don't use the diagonal edge of quads
        {
            f |= FLAG_E23;
        }
        if (eflags.insert( (t[2]<t[0])?std::make_pair(t[2],t[0]):std::make_pair(t[0],t[2]) ).second)
        {
            f |= FLAG_E31;
        }
        elems[i].flags = f;
    }
#endif
}

template<class DataTypes>
int TTriangleModel<DataTypes>::getTriangleFlags(int i)
{
    int f = 0;
    sofa::core::componentmodel::topology::BaseMeshTopology::Triangle t = (*triangles)[i];

    if (i < _topology->getNbTriangles())
    {
	if (_topology->getTrianglesAroundVertex(t[0])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_P1;
	if (_topology->getTrianglesAroundVertex(t[1])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_P2;
	if (_topology->getTrianglesAroundVertex(t[2])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_P3;
	
	const sofa::core::componentmodel::topology::BaseMeshTopology::EdgesInTriangle& e = _topology->getEdgesInTriangle(i);



	if (_topology->getTrianglesAroundEdge(e[0])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_E12;
	if (_topology->getTrianglesAroundEdge(e[1])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_E23;
	if (_topology->getTrianglesAroundEdge(e[2])[0] == (sofa::core::componentmodel::topology::BaseMeshTopology::TriangleID)i)
	    f |= FLAG_E31;
    }
    else
    {
	/// \TODO flags for quads
    }
    return f;
}

template<class DataTypes>
void TTriangleModel<DataTypes>::handleTopologyChange()
{
    //bool debug_mode = false;

    if (triangles != &mytriangles)
    {
        // We use the same triangle array as the topology -> only resize and recompute flags

        std::list<const sofa::core::componentmodel::topology::TopologyChange *>::const_iterator itBegin=_topology->firstChange();
        std::list<const sofa::core::componentmodel::topology::TopologyChange *>::const_iterator itEnd=_topology->lastChange();
        //elems.handleTopologyEvents(itBegin,itEnd);

	while( itBegin != itEnd )
	{
	    core::componentmodel::topology::TopologyChangeType changeType = (*itBegin)->getChangeType();

	    switch( changeType ) {


	    case core::componentmodel::topology::ENDING_EVENT:
	    {
		sout << "TriangleModel: now "<<_topology->getNbTriangles()<<" triangles." << sendl;
		resize(_topology->getNbTriangles());
		needsUpdate=true;
		updateFlags();

//                 updateNormals();
		break;
	    }
			/*
				case core::componentmodel::topology::TRIANGLESADDED:
				{
						//sout << "INFO_print : Vis - TRIANGLESADDED" << sendl;
					const sofa::component::topology::TrianglesAdded *ta=static_cast< const sofa::component::topology::TrianglesAdded * >( *itBegin );
					for (unsigned int i=0;i<ta->getNbAddedTriangles();++i) {
						Triangle t(this, size - ta->getNbAddedTriangles() + i);
						const Vector3& pt1 = t.p1();
						const Vector3& pt2 = t.p2();
						const Vector3& pt3 = t.p3();
						t.n() = cross(pt2-pt1,pt3-pt1);
						t.n().normalize();
					}
					break;
				}*/
				default: break;
	    }
	    ++itBegin;
	}
	return;
    }
#if 0
	sofa::core::componentmodel::topology::TopologyModifier* topoMod;
	this->getContext()->get(topoMod);

    if (topoMod) { // dynamic topology

        std::list<const sofa::core::componentmodel::topology::TopologyChange *>::const_iterator itBegin=_topology->firstChange();
        std::list<const sofa::core::componentmodel::topology::TopologyChange *>::const_iterator itEnd=_topology->lastChange();


		while( itBegin != itEnd )
		{
			core::componentmodel::topology::TopologyChangeType changeType = (*itBegin)->getChangeType();

			switch( changeType ) {


				case core::componentmodel::topology::ENDING_EVENT:
				{
					//sout << "INFO_print : Col - ENDING_EVENT" << sendl;
					needsUpdate=true;
					break;
				}


				case core::componentmodel::topology::TRIANGLESADDED:
					{
						//sout << "INFO_print : Col - TRIANGLESADDED" << sendl;
						TriangleInfo t;
						const sofa::component::topology::TrianglesAdded *ta=static_cast< const sofa::component::topology::TrianglesAdded * >( *itBegin );
						for (unsigned int i=0;i<ta->getNbAddedTriangles();++i) {
							mytriangles.push_back(ta->triangleArray[i]);
						}
						resize( mytriangles.size());
						needsUpdate=true;

						break;
					}

				case core::componentmodel::topology::TRIANGLESREMOVED:
					{
						//sout << "INFO_print : Col - TRIANGLESREMOVED" << sendl;
						unsigned int last;
						unsigned int ind_last;

						last= _topology->getNbPoints() - 1;

						const sofa::helper::vector<unsigned int> &tab = ( static_cast< const sofa::component::topology::TrianglesRemoved *>( *itBegin ) )->getArray();

                        TriangleInfo tmp;
                        topology::Triangle tmp2;

						for (unsigned int i = 0; i <tab.size(); ++i)
						{

							unsigned int ind_k = tab[i];

							tmp = elems[ind_k];
							elems[ind_k] = elems[last];
							elems[last] = tmp;

                            tmp2 = mytriangles[ind_k];
                            mytriangles[ind_k] = mytriangles[last];
                            mytriangles[last] = tmp2;

							ind_last = elems.size() - 1;

							if(last != ind_last){

								tmp = elems[last];
								elems[last] = elems[ind_last];
								elems[ind_last] = tmp;

                                  tmp2 = mytriangles[last];
                                  mytriangles[last] = mytriangles[ind_last];
                                  mytriangles[ind_last] = tmp2;
							}

							mytriangles.resize( elems.size() - 1 );
							resize( elems.size() - 1 );

							--last;
						}

						needsUpdate=true;

						break;
					}


				case core::componentmodel::topology::POINTSREMOVED:
					{
							//sout << "INFO_print : Col - POINTSREMOVED" << sendl;
							if (_topology->getNbTriangles()>0) {

								unsigned int last = _topology->getNbPoints() -1;

								unsigned int i,j;
								const sofa::helper::vector<unsigned int> tab = ( static_cast< const sofa::component::topology::PointsRemoved * >( *itBegin ) )->getArray();

								sofa::helper::vector<unsigned int> lastIndexVec;
								for(unsigned int i_init = 0; i_init < tab.size(); ++i_init){

									lastIndexVec.push_back(last - i_init);
								}

								for ( i = 0; i < tab.size(); ++i)
								{
									unsigned int i_next = i;
									bool is_reached = false;
									while( (!is_reached) && (i_next < lastIndexVec.size() - 1)){

										i_next += 1 ;
										is_reached = is_reached || (lastIndexVec[i_next] == tab[i]);
									}

									if(is_reached){

										lastIndexVec[i_next] = lastIndexVec[i];

									}

									const sofa::helper::vector<unsigned int> &shell=_topology->getTrianglesAroundVertex(lastIndexVec[i]);
									for (j=0;j<shell.size();++j) {

										unsigned int ind_j =shell[j];

										if ((unsigned)mytriangles[ind_j][0]==last)
											mytriangles[ind_j][0]=tab[i];
										else if ((unsigned)mytriangles[ind_j][1]==last)
											mytriangles[ind_j][1]=tab[i];
										else if ((unsigned)mytriangles[ind_j][2]==last)
											mytriangles[ind_j][2]=tab[i];
									}

									if (debug_mode) {

										for (unsigned int j_loc=0;j_loc<mytriangles.size();++j_loc) {

											bool is_forgotten = false;
											if ((unsigned)mytriangles[j_loc][0]==last){
												mytriangles[j_loc][0]=tab[i];
												is_forgotten=true;

											}else{
												if ((unsigned)mytriangles[j_loc][1]==last){
													mytriangles[j_loc][1]=tab[i];
													is_forgotten=true;

												}else{
													if ((unsigned)mytriangles[j_loc][2]==last){
														mytriangles[j_loc][2]=tab[i];
														is_forgotten=true;
													}
												}

											}

											if(is_forgotten){

												unsigned int ind_forgotten = j;

												bool is_in_shell = false;
												for (unsigned int j_glob=0;j_glob<shell.size();++j_glob) {
													is_in_shell = is_in_shell || (shell[j_glob] == ind_forgotten);
												}

												if(!is_in_shell) {
													sout << "INFO_print : Col - triangle is forgotten in SHELL !!! global index = "  << ind_forgotten << sendl;
												}

											}

										}
									}

									--last;
								}

							}

						needsUpdate=true;

						break;
					}

					// Case "POINTSRENUMBERING" added to propagate the treatment to the Visual Model

					case core::componentmodel::topology::POINTSRENUMBERING:
					{
							//sout << "INFO_print : Vis - POINTSRENUMBERING" << sendl;

							if (_topology->getNbTriangles()>0) {

								unsigned int i;

								const sofa::helper::vector<unsigned int> tab = ( static_cast< const sofa::component::topology::PointsRenumbering * >( *itBegin ) )->getinv_IndexArray();

								for ( i = 0; i < mytriangles.size(); ++i)
								{
									mytriangles[i][0]  = tab[mytriangles[i][0]];
									mytriangles[i][1]  = tab[mytriangles[i][1]];
									mytriangles[i][2]  = tab[mytriangles[i][2]];
								}

							}

						//}

						break;

					}

				default:
					// Ignore events that are not Triangle  related.
					break;
			}; // switch( changeType )

            mytriangles.resize( elems.size() ); // not necessary
			resize( elems.size() ); // not necessary

			++itBegin;
		} // while( changeIt != last; )
	}
	if (needsUpdate){
        updateFlags();
	}
#endif
}

template<class DataTypes>
void TTriangleModel<DataTypes>::draw(int index)
{
    Element t(this,index);
    glBegin(GL_TRIANGLES);
    helper::gl::glNormalT(t.n());
    helper::gl::glVertexT(t.p1());
    helper::gl::glVertexT(t.p2());
    helper::gl::glVertexT(t.p3());
    glEnd();
}

template<class DataTypes>
void TTriangleModel<DataTypes>::draw()
{
    if (getContext()->getShowCollisionModels())
    {
		if( size != _topology->getNbTriangles())
			updateFromTopology();

        if (bothSide.getValue() || getContext()->getShowWireFrame())
            simulation::getSimulation()->DrawUtility.setPolygonMode(0,getContext()->getShowWireFrame());
        else
        {
            simulation::getSimulation()->DrawUtility.setPolygonMode(2,true);
            simulation::getSimulation()->DrawUtility.setPolygonMode(1,false);
        }

		std::vector< Vector3 > points;
		std::vector< Vec<3,int> > indices;
		std::vector< Vector3 > normals;
		int index=0;
		for (int i=0;i<size;i++)
		{
		  Element t(this,i);
		  normals.push_back(t.n());
		  points.push_back(t.p1());
		  points.push_back(t.p2());
		  points.push_back(t.p3());
		  indices.push_back(Vec<3,int>(index,index+1,index+2));
		  index+=3;
		}

                sofa::simulation::getSimulation()->DrawUtility.setLightingEnabled(true);
		simulation::getSimulation()->DrawUtility.drawTriangles(points, indices, normals, Vec<4,float>(getColor4f()));
                sofa::simulation::getSimulation()->DrawUtility.setLightingEnabled(false);
		simulation::getSimulation()->DrawUtility.setPolygonMode(0,false);


                if (getContext()->getShowNormals())
                  {
                    std::vector< Vector3 > points;
                    for (int i=0;i<size;i++)
                      {
                        Element t(this,i);
                        points.push_back((t.p1()+t.p2()+t.p3())/3.0);
                        points.push_back(points.back()+t.n());                            
                      }

                    simulation::getSimulation()->DrawUtility.drawLines(points, 1, Vec<4,float>(1,1,1,1));

                  }
        }
	if (getPrevious()!=NULL && getContext()->getShowBoundingCollisionModels())
		getPrevious()->draw();
}


template<class DataTypes>
bool TTriangleModel<DataTypes>::canCollideWithElement(int index, CollisionModel* model2, int index2)
{
    if (!this->bSelfCollision.getValue()) return true; // we need to perform this verification process only for the selfcollision case.
    if (this->getContext() != model2->getContext()) return true;

	//if (model2 == mpoints && index2==4)
	//{
	//	std::cout<<"Triangle model : at index ["<<index<<"] can collide with point 4 ?"<<std::endl;
	//}


	//return true;

	Element t(this,index);
	if (model2 == mpoints)
	{
		// if point belong to the triangle, return false
		if ( index2==t.p1Index() || index2==t.p2Index() || index2==t.p3Index())
			return false;

		//const helper::vector <unsigned int>& EdgesAroundVertex11 =topology->getEdgesAroundVertex(p11);
		//const helper::vector <unsigned int>& EdgesAroundVertex12 =topology->getEdgesAroundVertex(p12);

		//// if the point belong to the the neighborhood of the triangle, return false
		//for (unsigned int i1=0; i1<EdgesAroundVertex11.size(); i1++)
		//{
		//	unsigned int e11 = EdgesAroundVertex11[i1];
		//	p11 = elems[e11].i1;
		//	p12 = elems[e11].i2;
		//	if (index2==p11 || index2==p12)
		//		return false;
		//}
		//for (unsigned int i1=0; i1<EdgesAroundVertex11.size(); i1++)
		//{
		//	unsigned int e12 = EdgesAroundVertex12[i1];
		//	p11 = elems[e12].i1;
		//	p12 = elems[e12].i2;
		//	if (index2==p11 || index2==p12)
		//		return false;
	}

	//// TODO : case with auto-collis with segment and auto-collis with itself

	return true;

}

template<class DataTypes>
void TTriangleModel<DataTypes>::computeBoundingTree(int maxDepth)
{
	CubeModel* cubeModel = createPrevious<CubeModel>();
	updateFromTopology();

	if (needsUpdate && !cubeModel->empty()) cubeModel->resize(0);

	if (!isMoving() && !cubeModel->empty() && !needsUpdate) return; // No need to recompute BBox if immobile

	needsUpdate=false;
	Vector3 minElem, maxElem;
	const VecCoord& x = *this->mstate->getX();

	const bool calcNormals = computeNormals.getValue();

	if (maxDepth == 0)
	{ // no hierarchy
		if (empty())
			cubeModel->resize(0);
		else
		{
			cubeModel->resize(1);
			minElem = x[0];
			maxElem = x[0];
			for (unsigned i=1;i<x.size();i++)
			{
				const Vector3& pt1 = x[i];
				if (pt1[0] > maxElem[0]) maxElem[0] = pt1[0];
				else if (pt1[0] < minElem[0]) minElem[0] = pt1[0];
				if (pt1[1] > maxElem[1]) maxElem[1] = pt1[1];
				else if (pt1[1] < minElem[1]) minElem[1] = pt1[1];
				if (pt1[2] > maxElem[2]) maxElem[2] = pt1[2];
				else if (pt1[2] < minElem[2]) minElem[2] = pt1[2];
			}
			if (calcNormals)
				for (int i=0;i<size;i++)
				{
					Element t(this,i);
					const Vector3& pt1 = x[t.p1Index()];
					const Vector3& pt2 = x[t.p2Index()];
					const Vector3& pt3 = x[t.p3Index()];

					/*for (int c = 0; c < 3; c++)
					{
					if (i==0)
					{
					minElem[c] = pt1[c];
					maxElem[c] = pt1[c];
					}
					else
					{
					if (pt1[c] > maxElem[c]) maxElem[c] = pt1[c];
					else if (pt1[c] < minElem[c]) minElem[c] = pt1[c];
					}
					if (pt2[c] > maxElem[c]) maxElem[c] = pt2[c];
					else if (pt2[c] < minElem[c]) minElem[c] = pt2[c];
					if (pt3[c] > maxElem[c]) maxElem[c] = pt3[c];
					else if (pt3[c] < minElem[c]) minElem[c] = pt3[c];
					}*/

					// Also recompute normal vector
					t.n() = cross(pt2-pt1,pt3-pt1);
					t.n().normalize();
				}

				cubeModel->setLeafCube(0, std::make_pair(this->begin(),this->end()), minElem, maxElem); // define the bounding box of the current triangle
		}
	}
	else
	{

		cubeModel->resize(size);  // size = number of triangles
		if (!empty())
		{
			for (int i=0;i<size;i++)
			{
				Element t(this,i);
				const Vector3& pt1 = x[t.p1Index()];
				const Vector3& pt2 = x[t.p2Index()];
				const Vector3& pt3 = x[t.p3Index()];

				for (int c = 0; c < 3; c++)
				{
					minElem[c] = pt1[c];
					maxElem[c] = pt1[c];
					if (pt2[c] > maxElem[c]) maxElem[c] = pt2[c];
					else if (pt2[c] < minElem[c]) minElem[c] = pt2[c];
					if (pt3[c] > maxElem[c]) maxElem[c] = pt3[c];
					else if (pt3[c] < minElem[c]) minElem[c] = pt3[c];
				}
				if (calcNormals)
				{
					// Also recompute normal vector
					t.n() = cross(pt2-pt1,pt3-pt1);
					t.n().normalize();
				}
				cubeModel->setParentOf(i, minElem, maxElem); // define the bounding box of the current triangle
			}
			cubeModel->computeBoundingTree(maxDepth);
		}
	}

	if (m_lmdFilter != 0)
	{
		m_lmdFilter->invalidate();
	}
}

template<class DataTypes>
void TTriangleModel<DataTypes>::computeContinuousBoundingTree(double dt, int maxDepth)
{	
	CubeModel* cubeModel = createPrevious<CubeModel>();
	updateFromTopology();
	if (needsUpdate) cubeModel->resize(0);
	if (!isMoving() && !cubeModel->empty() && !needsUpdate) return; // No need to recompute BBox if immobile

	needsUpdate=false;
	Vector3 minElem, maxElem;

	cubeModel->resize(size);
	if (!empty())
	{
		for (int i=0;i<size;i++)
		{
			Element t(this,i);
			const Vector3& pt1 = t.p1();
			const Vector3& pt2 = t.p2();
			const Vector3& pt3 = t.p3();
			const Vector3 pt1v = pt1 + t.v1()*dt;
			const Vector3 pt2v = pt2 + t.v2()*dt;
			const Vector3 pt3v = pt3 + t.v3()*dt;

			for (int c = 0; c < 3; c++)
			{
				                              minElem[c] = pt1[c];
				                              maxElem[c] = pt1[c];
				     if (pt2[c] > maxElem[c]) maxElem[c] = pt2[c];
				else if (pt2[c] < minElem[c]) minElem[c] = pt2[c];
				     if (pt3[c] > maxElem[c]) maxElem[c] = pt3[c];
				else if (pt3[c] < minElem[c]) minElem[c] = pt3[c];

				     if (pt1v[c] > maxElem[c]) maxElem[c] = pt1v[c];
				else if (pt1v[c] < minElem[c]) minElem[c] = pt1v[c];
				     if (pt2v[c] > maxElem[c]) maxElem[c] = pt2v[c];
				else if (pt2v[c] < minElem[c]) minElem[c] = pt2v[c];
				     if (pt3v[c] > maxElem[c]) maxElem[c] = pt3v[c];
				else if (pt3v[c] < minElem[c]) minElem[c] = pt3v[c];
			}

			// Also recompute normal vector
			t.n() = cross(pt2-pt1,pt3-pt1);
			t.n().normalize();

			cubeModel->setParentOf(i, minElem, maxElem);
		}
		cubeModel->computeBoundingTree(maxDepth);
	}
}

template<class DataTypes>
TriangleLocalMinDistanceFilter *TTriangleModel<DataTypes>::getFilter() const
{
	return m_lmdFilter;
}


template<class DataTypes>
void TTriangleModel<DataTypes>::setFilter(TriangleLocalMinDistanceFilter *lmdFilter)
{
	m_lmdFilter = lmdFilter;
}


} // namespace collision

} // namespace component

} // namespace sofa

#endif
