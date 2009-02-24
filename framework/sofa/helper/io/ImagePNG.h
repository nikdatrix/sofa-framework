/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_HELPER_IO_IMAGEPNG_H
#define SOFA_HELPER_IO_IMAGEPNG_H

#include <sofa/helper/io/Image.h>
#include <string>
#include <assert.h>

namespace sofa
{

namespace helper
{

namespace io
{

//using namespace sofa::defaulttype;

#ifdef SOFA_HAVE_PNG

class ImagePNG : public Image
{
public:
	ImagePNG ()
	{
	}

	ImagePNG (const std::string &filename)
	{
		load(filename);
	}

	bool load(std::string filename);
	bool save(std::string filename, int compression_level = -1);
};

#else
// #warning not supported by MSVC
//#warning PNG not supported. Define SOFA_HAVE_PNG in sofa.cfg to activate.
#endif

} // namespace io

} // namespace helper

} // namespace sofa

#endif
