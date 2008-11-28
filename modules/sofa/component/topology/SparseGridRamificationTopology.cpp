/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/

#include <sofa/core/ObjectFactory.h>

#include "SparseGridRamificationTopology.h"

#include <sofa/component/collision/proximity.h>

namespace sofa
{

namespace component
{

namespace topology
{

using std::cerr; using std::cout; using std::endl;

SOFA_DECL_CLASS(SparseGridRamificationTopology)

int SparseGridRamificationTopologyClass = core::RegisterObject("Sparse grid in 3D (modified)")
        .addAlias("SparseGridRamification")
        .add< SparseGridRamificationTopology >()
        ;

SparseGridRamificationTopology::SparseGridRamificationTopology(bool isVirtual)
    : SparseGridTopology(isVirtual)
    , _finestConnectivity( initData(&_finestConnectivity,true,"finestConnectivity","Test for connectivity at the finest level? (more precise but slower by testing all intersections between the model mesh and the faces between boundary cubes)") )
{
}

SparseGridRamificationTopology::~SparseGridRamificationTopology()
{}


void SparseGridRamificationTopology::init()
{
    SparseGridTopology::init();

    if( this->isVirtual || _nbVirtualFinerLevels.getValue() > 0)
        findCoarsestParents(); // in order to compute findCube by beginning by the finnest, by going up and give the coarsest parent
}

void SparseGridRamificationTopology::buildAsFinest()
{

// 				cerr<<"SparseGridRamificationTopology::buildAsFinest()\n";

    SparseGridTopology::buildAsFinest();



    if( _finestConnectivity.getValue() || this->isVirtual || _nbVirtualFinerLevels.getValue() > 0 )
    {
        // find the connexion graph between the finest hexas
        findConnexionsAtFinestLevel();
    }

    if( _finestConnectivity.getValue() )
    {

        buildRamifiedFinestLevel();
    }
}


void SparseGridRamificationTopology::findConnexionsAtFinestLevel()
{

    _connexions.resize( getNbHexas() );
    for( unsigned i=0; i<_connexions.size(); ++i)
        _connexions[i].push_back( new Connexion() ); // at the finest level, each hexa corresponds exatly to one connexion

    helper::io::Mesh* mesh = helper::io::Mesh::Create(this->fileTopology.getValue().c_str());


    // loop on every cubes
    for(int z=0; z<getNz()-1; ++z)
        for(int y=0; y<getNy()-1; ++y)
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if(cubeIdx != -1) // if existing in SG (ie not outside)
                {

                    Connexion* actualConnexion = _connexions[cubeIdx][0];

                    // find all neighbors in 3 directions

                    if(x>0)
                    {
                        // left neighbor
                        const int neighborIdxRG = _regularGrid.cube(x-1,y,z);
                        const int neighborIdx = _indicesOfRegularCubeInSparseGrid[neighborIdxRG];

                        if(_indicesOfRegularCubeInSparseGrid[neighborIdxRG] != -1 && sharingTriangle( mesh, cubeIdx, neighborIdx, LEFT ))
                        {
                            Connexion* neighbor = _connexions[neighborIdx][0];
                            actualConnexion->_neighbors[LEFT].insert(neighbor);
                            neighbor->_neighbors[RIGHT].insert(actualConnexion);
                        }
                    }
                    if(y>0)
                    {
                        // lower neighbor
                        const int neighborIdxRG = _regularGrid.cube(x,y-1,z);
                        const int neighborIdx = _indicesOfRegularCubeInSparseGrid[neighborIdxRG];

                        if(_indicesOfRegularCubeInSparseGrid[neighborIdxRG] != -1 && sharingTriangle( mesh, cubeIdx, neighborIdx, DOWN ))
                        {
                            Connexion* neighbor = _connexions[neighborIdx][0];
                            actualConnexion->_neighbors[DOWN].insert(neighbor);
                            neighbor->_neighbors[UP].insert(actualConnexion);
                        }

                    }
                    if(z>0)
                    {
                        // back neighbor
                        const int neighborIdxRG = _regularGrid.cube(x,y,z-1);
                        const int neighborIdx = _indicesOfRegularCubeInSparseGrid[neighborIdxRG];

                        if(_indicesOfRegularCubeInSparseGrid[neighborIdxRG] != -1 && sharingTriangle( mesh, cubeIdx, neighborIdx, BEFORE ))
                        {
                            Connexion* neighbor = _connexions[neighborIdx][0];
                            actualConnexion->_neighbors[BEFORE].insert(neighbor);
                            neighbor->_neighbors[BEHIND].insert(actualConnexion);
                        }
                    }
                }
            }

    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        _connexions[i][0]->_hexaIdx = i;
        _connexions[i][0]->_nonRamifiedHexaIdx = i;
    }


    delete mesh;


// 				printNeighborhood();




// 				cerr<<"END SparseGridRamificationTopology::buildAsFinest()\n";

}


bool SparseGridRamificationTopology::sharingTriangle(helper::io::Mesh* mesh, int cubeIdx, int neighborIdx, unsigned where )
{
// 				return true;


    if(!_finestConnectivity.getValue() )
        return true;
    if( mesh==NULL )
    {
        cerr<<"Warning: SparseGridRamificationTopology::sharingTriangle -- fileTopology (so mesh) is NULL\n";
        return true;
    }


    // it is not necessary to analyse connectivity between non-boundary cells
    if( getType(cubeIdx)!=BOUNDARY || getType(neighborIdx)!=BOUNDARY)
        return true;


// 				cerr<<"SparseGridRamificationTopology::sharingTriangle "<<this->fileTopology.getValue()<<"\n";

    const Hexa& hexa=getHexa( cubeIdx );

    Vector3 a,b,c,d,P,Q;

    //trouver la face commune
    switch(where)
    {
    case LEFT:
        a = getPointPos( hexa[0] );
        b = getPointPos( hexa[4] );
        c = getPointPos( hexa[7] );
        d = getPointPos( hexa[3] );
        break;
    case DOWN:
        a = getPointPos( hexa[0] );
        b = getPointPos( hexa[1] );
        c = getPointPos( hexa[5] );
        d = getPointPos( hexa[4] );
        break;
    case BEFORE:
        a = getPointPos( hexa[0] );
        b = getPointPos( hexa[1] );
        c = getPointPos( hexa[2] );
        d = getPointPos( hexa[3] );
        break;
    }


    const helper::vector< helper::vector < helper::vector <int> > >& facets = mesh->getFacets();
    const helper::vector<Vector3>& vertices = mesh->getVertices();
    for (unsigned int f=0; f<facets.size(); f++)
    {
        const helper::vector<int>& facet = facets[f][0];

        for (unsigned int j=1; j<facet.size(); j++) // Triangularize
        {
            const Vector3& A = vertices[facet[j-1]];
            const Vector3& B = vertices[facet[j]];

            //tester si la face commune intersecte facet
            // pour les 3 aretes de facet avec le carre
            static collision::DistanceSegTri proximitySolver;

            proximitySolver.NewComputation( a,b,c, A, B,P,Q);
            if( (Q-P).norm2() < 1.0e-3 ) return true;
            proximitySolver.NewComputation( a,c,d, A, B,P,Q);
            if( (Q-P).norm2() < 1.0e-3 ) return true;
        }
    }

    return false;
}


void SparseGridRamificationTopology::buildRamifiedFinestLevel()
{

    SeqHexas& hexas = *seqHexas.beginEdit();


    vector< CubeCorners > cubeCorners; // saving temporary positions of all cube corners
    for(unsigned i=0; i<hexas.size(); ++i)
    {
        CubeCorners c;
        for(int w=0; w<8; ++w)
            c[w]=getPointPos( hexas[i][w] );
        cubeCorners.push_back( c );
    }



    hexas.clear();

    nbPoints = 0;

    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {
            hexas.push_back( Hexa(nbPoints, nbPoints+1, nbPoints+2, nbPoints+3, nbPoints+4, nbPoints+5, nbPoints+6, nbPoints+7) );
            (*it)->_hexaIdx = hexas.size()-1;
            (*it)->_nonRamifiedHexaIdx = i;
            nbPoints += 8;
        }
    }




    // which cube is neigbor of which cube? in order to link similar vertices (link entiere faces)
    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {

            Hexa& hexa = hexas[ (*it)->_hexaIdx ]; // the hexa corresponding to the connexion


            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[BEFORE].begin();
                neig != (*it)->_neighbors[BEFORE].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[4] );
                changeIndices( hexa[1], neighbor[5] );
                changeIndices( hexa[2], neighbor[6] );
                changeIndices( hexa[3], neighbor[7] );
            }

            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[DOWN].begin();
                neig != (*it)->_neighbors[DOWN].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[3] );
                changeIndices( hexa[4], neighbor[7] );
                changeIndices( hexa[5], neighbor[6] );
                changeIndices( hexa[1], neighbor[2] );
            }

            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[LEFT].begin();
                neig != (*it)->_neighbors[LEFT].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[1] );
                changeIndices( hexa[4], neighbor[5] );
                changeIndices( hexa[7], neighbor[6] );
                changeIndices( hexa[3], neighbor[2] );
            }

        }
    }

    // saving incident hexas for each points in order to be able to link or not vertices
    helper::vector< helper::vector< helper::fixed_array<unsigned,3> > > hexasConnectedToThePoint(nbPoints);
    unsigned c=0;
    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {
            for(unsigned p=0; p<8; ++p)
            {
                hexasConnectedToThePoint[hexas[c][p]].push_back( helper::fixed_array<unsigned,3>  (c, p, i) );
            }
            c++;
        }
    }

    seqPoints.clear();
    nbPoints=0;
    for(unsigned i=0; i<hexasConnectedToThePoint.size(); ++i)
    {
        if( !hexasConnectedToThePoint[i].empty() )
        {
            for(unsigned j=0; j<hexasConnectedToThePoint[i].size(); ++j)
            {
                hexas[ hexasConnectedToThePoint[i][j][0] ][ hexasConnectedToThePoint[i][j][1] ] = nbPoints;
            }


            // find corresponding 3D coordinate
            seqPoints.push_back( cubeCorners[ hexasConnectedToThePoint[i][0][2] ][ hexasConnectedToThePoint[i][0][1] ] );

            ++nbPoints;
        }
    }

    seqHexas.endEdit();

}



void SparseGridRamificationTopology::buildFromFiner()
{
// 				cerr<<"SparseGridRamificationTopology::buildFromFiner()\n";

// 				SparseGridTopology::buildFromFiner();	return;


    SparseGridRamificationTopology* finerSparseGridRamification = dynamic_cast<SparseGridRamificationTopology*>(_finerSparseGrid);



    setNx( _finerSparseGrid->getNx()/2+1 );
    setNy( _finerSparseGrid->getNy()/2+1 );
    setNz( _finerSparseGrid->getNz()/2+1 );


    _regularGrid.setSize(getNx(),getNy(),getNz());

    setMin(_finerSparseGrid->getMin());



    // the cube size of the coarser mesh is twice the cube size of the finer mesh
    // if the finer mesh contains an odd number of cubes in any direction,
    // the coarser mesh will be a half cube size larger in that direction
    Vector3 dx = _finerSparseGrid->_regularGrid.getDx();
    Vector3 dy = _finerSparseGrid->_regularGrid.getDy();
    Vector3 dz = _finerSparseGrid->_regularGrid.getDz();
    setXmax(getXmin() + (getNx()-1) * (SReal)2.0 * dx[0]);
    setYmax(getYmin() + (getNy()-1) * (SReal)2.0 * dy[1]);
    setZmax(getZmin() + (getNz()-1) * (SReal)2.0 * dz[2]);




    _regularGrid.setPos(getXmin(), getXmax(), getYmin(), getYmax(), getZmin(), getZmax());


    _indicesOfRegularCubeInSparseGrid.resize( _regularGrid.getNbHexas(), -1 ); // to redirect an indice of a cube in the regular grid to its indice in the sparse grid



    vector< CubeCorners > cubeCorners; // saving temporary positions of all cube corners
    MapBetweenCornerPositionAndIndice cubeCornerPositionIndiceMap; // to compute cube corner indice values


    HierarchicalCubeMap nonRamifiedHierarchicalCubeMap;
    sofa::helper::vector<Type> nonRamifiedTypes;

    // as classical SparseGridTopology, build the hierarchy and deduce who is inside or boundary
    for(int i=0; i<getNx()-1; i++)
        for(int j=0; j<getNy()-1; j++)
            for(int k=0; k<getNz()-1; k++)
            {
                int x = 2*i;
                int y = 2*j;
                int z = 2*k;

                fixed_array<int,8> fineIndices;
                for(int idx=0; idx<8; ++idx)
                {
                    const int idxX = x + (idx & 1);
                    const int idxY = y + (idx & 2)/2;
                    const int idxZ = z + (idx & 4)/4;
                    if(idxX < _finerSparseGrid->getNx()-1 && idxY < _finerSparseGrid->getNy()-1 && idxZ < _finerSparseGrid->getNz()-1)
                        fineIndices[idx] = _finerSparseGrid->_indicesOfRegularCubeInSparseGrid[ _finerSparseGrid->_regularGrid.cube(idxX,idxY,idxZ) ];
                    else
                        fineIndices[idx] = -1;
                }

                bool inside = true;
                bool outside = true;
                for( int w=0; w<8 && (inside || outside); ++w)
                {
                    if( fineIndices[w] == -1 ) inside=false;
                    else
                    {

                        if( _finerSparseGrid->getType( fineIndices[w] ) == BOUNDARY ) { inside=false; outside=false; }
                        else if( _finerSparseGrid->getType( fineIndices[w] ) == INSIDE ) {outside=false;}
                    }
                }

                if(outside) continue;
                if( inside )
                {
                    nonRamifiedTypes.push_back(INSIDE);
                }
                else
                {
                    nonRamifiedTypes.push_back(BOUNDARY);
                }

                int coarseRegularIndice = _regularGrid.cube( i,j,k );
                Hexa c = _regularGrid.getHexaCopy( coarseRegularIndice );

                CubeCorners corners;
                for(int w=0; w<8; ++w)
                {
                    corners[w] = _regularGrid.getPoint( c[w] );
                    cubeCornerPositionIndiceMap[corners[w]] = 0;
                }

                cubeCorners.push_back(corners);

                _indicesOfRegularCubeInSparseGrid[coarseRegularIndice] = cubeCorners.size()-1;
                _indicesOfCubeinRegularGrid.push_back( coarseRegularIndice );

                nonRamifiedHierarchicalCubeMap.push_back( fineIndices );
            }




    // deduce the number of independant connexions inside each coarse hexas depending on the fine connexion graph

    _connexions.resize( cubeCorners.size() );

    // find all fine connexions included in every coarse hexa
    for(unsigned idx=0; idx<_connexions.size(); ++idx) // for all the coarse hexa
    {
        helper::vector< Connexion* > allFineConnexions; // to save all fine connexions included in the coarse hexa

        fixed_array<int,8>& children = nonRamifiedHierarchicalCubeMap[idx]; // the child hexa

        for( int child = 0; child < 8 ; child ++)
        {
            int childIdx = children[child];

            if( childIdx != -1 )
            {
                helper::vector<Connexion*> & childConnexions = finerSparseGridRamification->_connexions[ childIdx ]; // all connexions of the child hexa

                for(helper::vector<Connexion*>::iterator fineConnexion = childConnexions.begin() ; fineConnexion != childConnexions.end() ; ++fineConnexion)
                {
                    allFineConnexions.push_back( *fineConnexion );
                    (*fineConnexion)->_tmp = -1;
                }
            }
        }



        int connexionNumber=0;
        // who is neighbor between allFineConnexions of a coarse hexa
        for( unsigned i=0; i<allFineConnexions.size(); ++i )
        {
            allFineConnexions[i]->propagateConnexionNumberToNeighbors( connexionNumber, allFineConnexions ); // give the same connexion number to all the neighborood
            ++connexionNumber;
        }


        // keep only unique ConnexionNumber
        std::map<int,int> uniqueConnexionNumber;
        for(unsigned i=0; i<allFineConnexions.size(); ++i )
        {
            uniqueConnexionNumber[ allFineConnexions[i]->_tmp]++;
        }

        // for each ConnexionNumber, build a new coarse connexion
        for( std::map<int,int>::iterator it=uniqueConnexionNumber.begin(); it!=uniqueConnexionNumber.end(); ++it)
        {
            Connexion* newConnexion = new Connexion();
            _connexions[idx].push_back(newConnexion);

            for(unsigned i=0; i<allFineConnexions.size(); ++i )
            {
                if( allFineConnexions[i]->_tmp == (*it).first )
                {
                    newConnexion->_children.push_back( allFineConnexions[i] );
                    allFineConnexions[i]->_parent = newConnexion;
                }
            }
        }
    }


    // print debug
// 				for(int z=0; z<getNz()-1; ++z)
// 				{
// 					for(int y=0; y<getNy()-1; ++y)
// 					{
// 						for(int x=0; x<getNx()-1; ++x)
// 						{
// 							const int cubeIdxRG = _regularGrid.cube(x,y,z);
// 							const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];
//
// 							if( cubeIdx==-1)
// 								cout << "  ";
// 							else
// 							{
// 								cout << _connexions[cubeIdx].size() << " ";
// 							}
// 						}
// 						std::cout << std::endl;
// 					}
// 					std::cout << " -- " << std::endl;
// 				}



    // to know in what cube is a connexion
    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {
            (*it)->_tmp = i;
        }
    }


    // build connectivity graph between new Connexions by looking into children neighborood


    for(int z=0; z<_finerSparseGrid->getNz()-1; ++z)
    {
        for(int y=0; y<_finerSparseGrid->getNy()-1; ++y)
        {
            for(int x=0; x<_finerSparseGrid->getNx()-1; ++x)
            {
                int fineIdx = _finerSparseGrid->_indicesOfRegularCubeInSparseGrid[ _finerSparseGrid->_regularGrid.cube(x,y,z) ];
                if( fineIdx != -1 )
                {

                    for( helper::vector<Connexion*>::iterator it = finerSparseGridRamification->_connexions[fineIdx].begin(); it != finerSparseGridRamification->_connexions[fineIdx].end() ; ++it)
                    {
                        Connexion* fineConnexion1 = *it;
                        Connexion* coarseConnexion1 = fineConnexion1->_parent;
                        int coarseHexa1 = coarseConnexion1->_tmp;

                        for( unsigned i=0; i<NUM_CONNECTED_NODES; ++i) // in all directions
                        {
                            for(std::set<Connexion*>::iterator n = fineConnexion1->_neighbors[i].begin(); n!=fineConnexion1->_neighbors[i].end(); ++n)
                            {

                                Connexion* fineConnexion2 = *n;
                                Connexion* coarseConnexion2 = fineConnexion2->_parent;
                                int coarseHexa2 = coarseConnexion2->_tmp;

// 											cerr<<"coarseHexa : "<<coarseHexa1<<" "<<coarseHexa2<<endl;

// 											if( coarseConnexion1 != coarseConnexion2 )
                                if( coarseHexa1 != coarseHexa2 ) // the both fine hexas are not in the same coarse hexa
                                {
                                    // 											cout<<"coarseConnexion1 : "<<coarseConnexion1<<"   "<<(coarseConnexion1==NULL?"NULL":"not")<<endl;
                                    coarseConnexion1->_neighbors[i].insert( coarseConnexion2 );
                                    coarseConnexion2->_neighbors[ !(i%2)?i+1:i-1 ].insert( coarseConnexion1 );
                                }
                            }
                        }
                    }
                }
            }
        }
    }



// 				printNeighborhood();


    // build an new coarse hexa for each independnnt connexion

    SeqHexas& hexas = *seqHexas.beginEdit();
    hexas.clear();

    nbPoints = 0;

    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {
            hexas.push_back( Hexa(nbPoints, nbPoints+1, nbPoints+2, nbPoints+3, nbPoints+4, nbPoints+5, nbPoints+6, nbPoints+7) );
            (*it)->_hexaIdx = hexas.size()-1;
            (*it)->_nonRamifiedHexaIdx = i;
            nbPoints += 8;
        }
    }




    // which cube is neigbor of which cube? in order to link similar vertices (link entiere faces)
    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {

            Hexa& hexa = hexas[ (*it)->_hexaIdx ]; // the hexa corresponding to the connexion


            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[BEFORE].begin();
                neig != (*it)->_neighbors[BEFORE].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[4] );
                changeIndices( hexa[1], neighbor[5] );
                changeIndices( hexa[2], neighbor[6] );
                changeIndices( hexa[3], neighbor[7] );
            }

            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[DOWN].begin();
                neig != (*it)->_neighbors[DOWN].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[3] );
                changeIndices( hexa[4], neighbor[7] );
                changeIndices( hexa[5], neighbor[6] );
                changeIndices( hexa[1], neighbor[2] );
            }

            for(std::set<Connexion*>::iterator neig = (*it)->_neighbors[LEFT].begin();
                neig != (*it)->_neighbors[LEFT].end(); ++neig)
            {
                Hexa& neighbor = hexas[ (*neig)->_hexaIdx ]; // the hexa corresponding to the neighbor connexion

                changeIndices( hexa[0], neighbor[1] );
                changeIndices( hexa[4], neighbor[5] );
                changeIndices( hexa[7], neighbor[6] );
                changeIndices( hexa[3], neighbor[2] );
            }

        }
    }




// 				cerr<<hexas<<endl;




    // saving incident hexas for each points in order to be able to link or not vertices
    helper::vector< helper::vector< helper::fixed_array<unsigned,3> > > hexasConnectedToThePoint(nbPoints);
    unsigned c=0;
    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
        {
            for(unsigned p=0; p<8; ++p)
            {
                hexasConnectedToThePoint[hexas[c][p]].push_back( helper::fixed_array<unsigned,3>  (c, p, i) );
            }
            c++;
        }
    }

// 				cerr<<seqPoints.size()<<" "<<hexas.size()<<endl;


    seqPoints.clear();
    nbPoints=0;
    for(unsigned i=0; i<hexasConnectedToThePoint.size(); ++i)
    {
        if( !hexasConnectedToThePoint[i].empty() )
        {
            for(unsigned j=0; j<hexasConnectedToThePoint[i].size(); ++j)
            {
                hexas[ hexasConnectedToThePoint[i][j][0] ][ hexasConnectedToThePoint[i][j][1] ] = nbPoints;
            }

            // find corresponding 3D coordinate
            seqPoints.push_back( cubeCorners[ hexasConnectedToThePoint[i][0][2] ][ hexasConnectedToThePoint[i][0][1] ] );



            ///DEBUG  FAAALLLSSSSEEEE
// 						if( hexasConnectedToThePoint[i].size() == 1 )
// 						{
// 							seqPoints.back()[2] -= hexasConnectedToThePoint[i][0][0]*3; // decale les particles dedoubl�es du nb de leur connexion pour bien les diff�rencier
// 						}



            ++nbPoints;
        }
    }


    seqHexas.endEdit();


    for(unsigned i=0 ; i<_connexions.size(); ++i)
    {
        fixed_array<int,8> nonRamifiedFineIndices = nonRamifiedHierarchicalCubeMap[ i ];

        if( _connexions[i].size()==1 ) // 1 seule connexion pour l'element ==> element normal non ramifi� ==> meme enfants
        {
            _hierarchicalCubeMap.push_back( nonRamifiedFineIndices );
            _types.push_back( nonRamifiedTypes[i] );
        }
        else // plrs connexion pour un element normal ==> trouver quels fils sont dans quelle connexion
        {

            for( helper::vector<Connexion*>::iterator it = _connexions[i].begin(); it != _connexions[i].end() ; ++it)
            {
                fixed_array<int,8> fineIndices;

                for( std::list<Connexion*>::iterator child=(*it)->_children.begin(); child!=(*it)->_children.end(); ++child)
                {
                    unsigned childIdx=(*child)->_nonRamifiedHexaIdx;
                    for(unsigned p=0; p<8; ++p)
                    {
                        if( childIdx == (unsigned)nonRamifiedFineIndices[p] )
                            fineIndices[p] = (*child)->_hexaIdx;
                        else
                            fineIndices[p] = -1;
                    }
                }
                _hierarchicalCubeMap.push_back( fineIndices );
                _types.push_back( BOUNDARY );
            }
        }
    }



    // compute stiffness coefficient from children
    _stiffnessCoefs.resize( this->getNbHexas() );
    for(int i=0; i<this->getNbHexas(); ++i)
    {
        helper::fixed_array<int,8> finerChildren = this->_hierarchicalCubeMap[i];
        unsigned nbchildren = 0;
        for(int w=0; w<8; ++w)
        {
            if( finerChildren[w] != -1 )
            {
                _stiffnessCoefs[i] += this->_finerSparseGrid->getStiffnessCoef(finerChildren[w]);
                ++nbchildren;
            }
        }
        _stiffnessCoefs[i] /= 8.0;//(float)nbchildren;
    }





// 				cerr<<"nbPoints : "<<seqPoints.size()<<endl;

// 				cerr<<"END SparseGridRamificationTopology::buildFromFiner()\n";
}




void SparseGridRamificationTopology::buildVirtualFinerLevels()
{
    int nb = _nbVirtualFinerLevels.getValue();

    _virtualFinerLevels.resize(nb);

    int newnx=n.getValue()[0],newny=n.getValue()[1],newnz=n.getValue()[2];
    for( int i=0; i<nb; ++i)
    {
        newnx = (newnx-1)*2+1;
        newny = (newny-1)*2+1;
        newnz = (newnz-1)*2+1;
    }

    _virtualFinerLevels[0] = new SparseGridRamificationTopology(true);
    _virtualFinerLevels[0]->setNx( newnx );
    _virtualFinerLevels[0]->setNy( newny );
    _virtualFinerLevels[0]->setNz( newnz );
    _virtualFinerLevels[0]->load(this->fileTopology.getValue().c_str());
    _virtualFinerLevels[0]->init();

    cerr<<"SparseGridTopology "<<getName()<<" buildVirtualFinerLevels : ";
    cerr<<"("<<newnx<<"x"<<newny<<"x"<<newnz<<") -> "<< _virtualFinerLevels[0]->getNbHexas() <<" elements , ";

    for(int i=1; i<nb; ++i)
    {
        _virtualFinerLevels[i] = new SparseGridRamificationTopology(true);


        _virtualFinerLevels[i]->setFinerSparseGrid(_virtualFinerLevels[i-1]);

        _virtualFinerLevels[i]->init();


        cerr<<"("<<_virtualFinerLevels[i]->getNx()<<"x"<<_virtualFinerLevels[i]->getNy()<<"x"<<_virtualFinerLevels[i]->getNz()<<") -> "<< _virtualFinerLevels[i]->getNbHexas() <<" elements , ";
    }

    cerr<<endl;

    this->setFinerSparseGrid(_virtualFinerLevels[nb-1]);


}



int SparseGridRamificationTopology::findCube(const Vector3 &pos, SReal &fx, SReal &fy, SReal &fz)
{
// 				if(  !this->isVirtual && _nbVirtualFinerLevels.getValue() == 0 )
// 					return SparseGridTopology::findCube(pos, fx, fy, fz);

    if(  _nbVirtualFinerLevels.getValue() == 0 )
        return SparseGridTopology::findCube(pos, fx, fy, fz);


    SparseGridRamificationTopology* finestSparseGridTopology = dynamic_cast<SparseGridRamificationTopology*>(_virtualFinerLevels[0]);

    int finestSparseCube = finestSparseGridTopology->SparseGridTopology::findCube(pos,fx,fy,fz);

    if( finestSparseCube == -1 ) return -1;

    Connexion * finestConnexion = finestSparseGridTopology->_connexions[ finestSparseCube ][0];


    _regularGrid.findCube( pos,fx,fy,fz); // only to compute fx,fy,fz

    return finestConnexion->_coarsestParent;

}

int SparseGridRamificationTopology::findNearestCube(const Vector3 &pos, SReal &fx, SReal &fy, SReal &fz)
{
// 				if( !this->isVirtual && _nbVirtualFinerLevels.getValue() == 0 )
// 					return SparseGridTopology::findNearestCube(pos, fx, fy, fz);


    if( _nbVirtualFinerLevels.getValue() == 0 )
        return SparseGridTopology::findNearestCube(pos, fx, fy, fz);


    SparseGridRamificationTopology* finestSparseGridTopology = dynamic_cast<SparseGridRamificationTopology*>(_virtualFinerLevels[0]);

    int finestSparseCube = finestSparseGridTopology->SparseGridTopology::findNearestCube(pos,fx,fy,fz);

    if( finestSparseCube == -1 ) return -1;

    Connexion * finestConnexion = finestSparseGridTopology->_connexions[ finestSparseCube ][0];

    _regularGrid.findCube( pos,fx,fy,fz); // only to compute fx,fy,fz

    return finestConnexion->_coarsestParent;
}





void SparseGridRamificationTopology::findCoarsestParents()
{
    for( unsigned i=0; i<_virtualFinerLevels.size(); ++i)
    {
        SparseGridRamificationTopology* finestSGRT = dynamic_cast<SparseGridRamificationTopology*>(_virtualFinerLevels[i]);


        for(int z=0; z<finestSGRT->getNz()-1; ++z)
        {
            for(int y=0; y<finestSGRT->getNy()-1; ++y)
            {
                for(int x=0; x<finestSGRT->getNx()-1; ++x)
                {
                    const int cubeIdxRG = finestSGRT->_regularGrid.cube(x,y,z);
                    const int cubeIdx = finestSGRT->_indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                    if( cubeIdx!=-1)
                    {
                        for( helper::vector<Connexion*>::iterator it = finestSGRT->_connexions[cubeIdx].begin(); it != finestSGRT->_connexions[cubeIdx].end() ; ++it)
                        {
                            Connexion * finestConnexion = *it;
                            while( finestConnexion->_parent != NULL )
                                finestConnexion = finestConnexion->_parent;

                            (*it)->_coarsestParent = finestConnexion->_hexaIdx;
                        }
                    }
                }
            }
        }
    }

    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=0; y<getNy()-1; ++y)
        {
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx!=-1)
                {
                    for( helper::vector<Connexion*>::iterator it = _connexions[cubeIdx].begin(); it != _connexions[cubeIdx].end() ; ++it)
                    {
                        (*it)->_coarsestParent = (*it)->_hexaIdx;
                    }
                }
            }
        }
    }


// 				printParents();

}



void SparseGridRamificationTopology::changeIndices(unsigned oldidx, unsigned newidx)
{
    SeqHexas& hexas = *seqHexas.beginEdit();
    for(unsigned i=0; i<hexas.size(); ++i)
        for(int j=0; j<8; ++j)
        {
            if( hexas[i][j] == oldidx )
            {
                hexas[i][j] = newidx;
                break; // very small optimization (if the point is found on a hexa, it is no in the follow points of the same hexa)
            }
        }
}






///////////////// DEBUG PRINTING /////////////////////////////////


void SparseGridRamificationTopology::printNeighborhood()
{
    // 				// print draw neighbors
    cerr<<endl<<endl;
    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=getNy()-2; y>=0; --y)
        {


            for(int x=0; x<getNx()-1; ++x)
            {

                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx==-1)
                    cout << "     ";
                else
                {
                    for( helper::vector<Connexion*>::iterator it = _connexions[cubeIdx].begin(); it != _connexions[cubeIdx].end() ; ++it)
                    {
                        if( ! (*it)->_neighbors[UP].empty() ) cout<<"  | ";
                        else cout<<"   ";
                    }
                }
            }
            std::cout << std::endl;



            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];




                if( cubeIdx==-1)
                    cout << "     ";
                else
                {
                    cout<<"[";
                    for( helper::vector<Connexion*>::iterator it = _connexions[cubeIdx].begin(); it != _connexions[cubeIdx].end() ; ++it)
                    {
                        if( it!= _connexions[cubeIdx].begin() ) cout<<",";

                        if( ! (*it)->_neighbors[LEFT].empty() ) cout<<"-";
                        else cout<<" ";

                        if( ! (*it)->_neighbors[BEFORE].empty() ) cout<<"X";
                        else cout<<"0";
                    }
                    cout<<"]";
                }



            }
            std::cout << std::endl;




        }
        std::cout << " -- " << std::endl;
    }
}

void SparseGridRamificationTopology::printNeighbors()
{
    // print nb neighbors per cube
    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=getNy()-2; y>=0; --y)
        {
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx==-1)
                    cout << "  ";
                else
                {
                    int sum=0;
                    for(int i=0; i<NUM_CONNECTED_NODES; ++i)
                        sum+=_connexions[cubeIdx][0]->_neighbors[i].size();
                    cout << sum << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << " -- " << std::endl;
    }
}

void SparseGridRamificationTopology::printNbConnexions()
{
    // print nb connexions
    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=getNy()-2; y>=0; --y)
        {
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx==-1)
                    cout << "  ";
                else
                {
                    cout << _connexions[cubeIdx].size() << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << " -- " << std::endl;
    }
}


void SparseGridRamificationTopology::printParents()
{
    cerr<<"\n\nPARENTS\n\n";

    for( unsigned i=0; i<_virtualFinerLevels.size(); ++i)
    {
        cerr<<"level "<<i<<" :\n";

        SparseGridRamificationTopology* finestSGRT = dynamic_cast<SparseGridRamificationTopology*>(_virtualFinerLevels[i]);

        for(int z=0; z<finestSGRT->getNz()-1; ++z)
        {
            for(int y=finestSGRT->getNy()-2; y>=0; --y)
            {
                for(int x=0; x<finestSGRT->getNx()-1; ++x)
                {
                    const int cubeIdxRG = finestSGRT->_regularGrid.cube(x,y,z);
                    const int cubeIdx = finestSGRT->_indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                    if( cubeIdx==-1)
                        cout << " ";
                    else
                    {
                        cout<<"[";
                        for( helper::vector<Connexion*>::iterator it = finestSGRT->_connexions[cubeIdx].begin(); it != finestSGRT->_connexions[cubeIdx].end() ; ++it)
                        {
                            if( it!= finestSGRT->_connexions[cubeIdx].begin() ) cout<<",";

                            cout << cubeIdx << "->"<<(*it)->_coarsestParent;
                        }
                        cout<<"]";
                        cout<<" ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << " -- " << std::endl;
        }
    }

    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=getNy()-2; y>=0; --y)
        {
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx==-1)
                    cout << " ";
                else
                {
                    cout<<"[";
                    for( helper::vector<Connexion*>::iterator it = _connexions[cubeIdx].begin(); it != _connexions[cubeIdx].end() ; ++it)
                    {
                        if( it!= _connexions[cubeIdx].begin() ) cout<<",";
                        cout << (*it)->_coarsestParent;
                    }
                    cout<<"]";
                    cout<<" ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << " -- " << std::endl;
    }
}


void SparseGridRamificationTopology::printHexaIdx()
{
    // print hexa nb
    for(int z=0; z<getNz()-1; ++z)
    {
        for(int y=getNy()-2; y>=0; --y)
        {
            for(int x=0; x<getNx()-1; ++x)
            {
                const int cubeIdxRG = _regularGrid.cube(x,y,z);
                const int cubeIdx = _indicesOfRegularCubeInSparseGrid[cubeIdxRG];

                if( cubeIdx==-1)
                    cout << "  ";
                else
                {
                    cout<<"[";
                    for( helper::vector<Connexion*>::iterator it = _connexions[cubeIdx].begin(); it != _connexions[cubeIdx].end() ; ++it)
                    {
                        if( it!= _connexions[cubeIdx].begin() ) cout<<",";
                        cout << (*it)->_hexaIdx;
                    }
                    cout<<"]";
                    cout<<" ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << " -- " << std::endl;
    }
}

} // namespace topology

} // namespace component

} // namespace sofa
