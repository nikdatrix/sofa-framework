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
#ifndef SOFA_IMAGE_IMAGEFILTER_H
#define SOFA_IMAGE_IMAGEFILTER_H

#include "initImage.h"
#include "ImageTypes.h"
#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/rmath.h>
#include <sofa/helper/OptionsGroup.h>

#include <sofa/component/component.h>

#define NONE 0
#define BLURDERICHE 1
#define BLURMEDIAN 2
#define BLURBILATERAL 3
#define BLURANISOTROPIC 4
#define DERICHE 5
#define CROP 6
#define RESIZE 7
#define TRIM 8
#define DILATE 9
#define ERODE 10
#define NOISE 11
#define QUANTIZE 12
#define THRESHOLD 13
#define LAPLACIAN 14
#define STENSOR 15
#define DISTANCE 16
#define GRADIENT 17
#define HESSIAN 18
#define NORMALIZE 19
#define RESAMPLE 20
#define SELECTCHANNEL 21


namespace sofa
{

namespace component
{

namespace engine
{

using helper::vector;
using namespace helper;
using namespace cimg_library;

/**
 * This class computes a filtered image
 */


template <class _InImageTypes,class _OutImageTypes>
class ImageFilter : public core::DataEngine
{
public:
    typedef core::DataEngine Inherited;
    SOFA_CLASS(SOFA_TEMPLATE2(ImageFilter,_InImageTypes,_OutImageTypes),Inherited);

    typedef _InImageTypes InImageTypes;
    typedef typename InImageTypes::T Ti;
    typedef typename InImageTypes::imCoord imCoordi;
    typedef helper::WriteAccessor<Data< InImageTypes > > waImagei;
    typedef helper::ReadAccessor<Data< InImageTypes > > raImagei;

    typedef _OutImageTypes OutImageTypes;
    typedef typename OutImageTypes::T To;
    typedef typename OutImageTypes::imCoord imCoordo;
    typedef helper::WriteAccessor<Data< OutImageTypes > > waImageo;
    typedef helper::ReadAccessor<Data< OutImageTypes > > raImageo;

    typedef SReal Real;
    typedef defaulttype::ImageLPTransform<Real> TransformType;
    typedef typename TransformType::Coord Coord;
    typedef helper::WriteAccessor<Data< TransformType > > waTransform;
    typedef helper::ReadAccessor<Data< TransformType > > raTransform;

    typedef vector<double> ParamTypes;
    typedef helper::ReadAccessor<Data< ParamTypes > > raParam;

    Data<helper::OptionsGroup> filter;
    Data< ParamTypes > param;

    Data< InImageTypes > inputImage;
    Data< TransformType > inputTransform;

    Data< OutImageTypes > outputImage;
    Data< TransformType > outputTransform;

    virtual std::string getTemplateName() const    { return templateName(this);    }
    static std::string templateName(const ImageFilter<InImageTypes,OutImageTypes>* = NULL) { return InImageTypes::Name()+std::string(",")+OutImageTypes::Name(); }

    ImageFilter()    :   Inherited()
        , filter ( initData ( &filter,"filter","Filter" ) )
        , param ( initData ( &param,"param","Parameters" ) )
        , inputImage(initData(&inputImage,InImageTypes(),"inputImage",""))
        , inputTransform(initData(&inputTransform,TransformType(),"inputTransform",""))
        , outputImage(initData(&outputImage,OutImageTypes(),"outputImage",""))
        , outputTransform(initData(&outputTransform,TransformType(),"outputTransform",""))
    {
        inputImage.setReadOnly(true);
        inputTransform.setReadOnly(true);
        outputImage.setReadOnly(true);
        outputTransform.setReadOnly(true);
        helper::OptionsGroup filterOptions(22	,"0 - None"
                ,"1 - Blur ( sigma )"
                ,"2 - Blur Median ( n )"
                ,"3 - Blur Bilateral ( sigma_s, sigma_r)"
                ,"4 - Blur Anisotropic ( amplitude )"
                ,"5 - Deriche ( sigma , order , axis )"
                ,"6 - Crop ( xmin , ymin , zmin , xmax , ymax , zmax)"
                ,"7 - Resize( dimx , dimy , dimz , no interp.|nearest neighb.|moving av.|linear|grid|bicubic|lanzcos)"
                ,"8 - Trim ( tmin , tmax )"
                ,"9 - Dilate ( size )"
                ,"10 - Erode ( size )"
                ,"11 - Noise ( sigma , gaussian|uniform|Salt&Pepper|Poisson|Rician )"
                ,"12 - Quantize ( nbLevels )"
                ,"13 - Threshold ( value )"
                ,"14 - Laplacian"
                ,"15 - Structure tensors ( scheme )"
                ,"16 - Distance ( value, scale )"
                ,"17 - Gradient ( axis x | y | z | magnitude)"
                ,"18 - Hessian (axis1 , axis2) "
                ,"19 - Normalize ( out_min, out_max , in_min, in_max)"
                ,"20 - Resample ( ox , oy , oz , dimx , dimy , dimz , dx , dy , dz  , nearest neighb.|linear|cubic)"
                ,"21 - SelectChannels ( c0, c1 )"
                                          );
        filterOptions.setSelectedItem(NONE);
        filter.setValue(filterOptions);
    }

    virtual ~ImageFilter() {}

    virtual void init()
    {
        addInput(&inputImage);
        addInput(&inputTransform);
        addOutput(&outputImage);
        addOutput(&outputTransform);
        setDirtyValue();
    }

    virtual void reinit() { update(); }

protected:

    virtual void update()
    {
        bool updateImage = this->inputImage.isDirty();	// change of input image -> update output image
        bool updateTransform = this->inputTransform.isDirty();	// change of input transform -> update output transform
        if(!updateImage && !updateTransform) {updateImage=true; updateTransform=true;}  // change of parameters -> update all

        cleanDirty();
        raParam p(this->param);
        raImagei in(this->inputImage);
        raTransform inT(this->inputTransform);
        waImageo out(this->outputImage);
        waTransform outT(this->outputTransform);

        CImgList<To>& img = out->getCImgList();
        if(updateImage) img.assign(in->getCImgList());	// copy
        if(updateTransform) outT->operator=(inT);	// copy

        switch(this->filter.getValue().getSelectedId())
        {
        case BLURDERICHE:
            if(updateImage)
            {
                float sigma=0; if(p.size()) sigma=(float)p[0];
                cimglist_for(img,l) img(l).blur(sigma);
            }
            break;
        case BLURMEDIAN:
            if(updateImage)
            {
                unsigned int n=0; if(p.size()) n=(unsigned int)p[0];
                cimglist_for(img,l) img(l).blur_median (n);
            }
            break;
        case BLURBILATERAL:
            if(updateImage)
            {
                float sigma_s=0;  if(p.size()) sigma_s=(float)p[0];
                float sigma_r=0; if(p.size()>1) sigma_r=(float)p[1];
                cimglist_for(img,l) img(l).blur_bilateral (sigma_s,sigma_r);
            }
            break;
        case BLURANISOTROPIC:
            if(updateImage)
            {
                float amplitude=0; if(p.size()) amplitude=(float)p[0];
                cimglist_for(img,l) img(l).blur_anisotropic (amplitude);
            }
            break;
        case DERICHE:
            if(updateImage)
            {
                float sigma=0;  if(p.size()) sigma=(float)p[0];
                unsigned int order=0; if(p.size()>1) order=(unsigned int)p[1];
                char axis='x';  if(p.size()>2) { if((int)p[2]==1) axis='y'; else if((int)p[2]==2) axis='z'; }
                cimglist_for(img,l) img(l).deriche (sigma,order,axis);
            }
            break;
        case CROP:
            if(updateImage || updateTransform)
            {
                unsigned int xmin=0; if(p.size())   xmin=(unsigned int)p[0];
                unsigned int ymin=0; if(p.size()>1) ymin=(unsigned int)p[1];
                unsigned int zmin=0; if(p.size()>2) zmin=(unsigned int)p[2];
                unsigned int xmax=in->getDimensions()[0]-1; if(p.size()>3) xmax=(unsigned int)p[3];
                unsigned int ymax=in->getDimensions()[1]-1; if(p.size()>4) ymax=(unsigned int)p[4];
                unsigned int zmax=in->getDimensions()[2]-1; if(p.size()>5) zmax=(unsigned int)p[5];
                if(updateImage) cimglist_for(img,l) img(l).crop(xmin,ymin,zmin,0,xmax,ymax,zmax,in->getDimensions()[3]-1);
                if(updateTransform)
                {
                    outT->getTranslation()=outT->fromImage( Coord((Real)xmin,(Real)ymin,(Real)zmin) );
                    outT->setCamPos((Real)(out->getDimensions()[0]-1)/2.0,(Real)(out->getDimensions()[1]-1)/2.0);
                }
            }
            break;
        case RESIZE:
            if(updateImage || updateTransform)
            {
                unsigned int dim[3]= {in->getDimensions()[0],in->getDimensions()[1],in->getDimensions()[2]};
                if(p.size())   dim[0]=(unsigned int)p[0];
                if(p.size()>1) dim[1]=(unsigned int)p[1];
                if(p.size()>2) dim[2]=(unsigned int)p[2];
                unsigned int interpolation=1; if(p.size()>3) interpolation=(unsigned int)p[3];
                if(updateImage) cimglist_for(img,l) img(l).resize(dim[0],dim[1],dim[2],-100,interpolation);
                if(updateTransform)
                    if(interpolation)
                    {
                        for(unsigned int k=0; k<3; k++)
                        {
                            if(dim[k]!=1 && in->getDimensions()[k]!=1) outT->getScale()[k]*=((Real)in->getDimensions()[k]-1.0)/((Real)dim[k]-1.0); // keep same origin
                            else // dim =1 -> keep same thickness and translate origin
                            {
                                outT->getTranslation()[k]+=(((Real)in->getDimensions()[k])/((Real)dim[k])-1.0)*0.5*outT->getScale()[k];
                                outT->getScale()[k]*=((Real)in->getDimensions()[k])/((Real)dim[k]);
                            }
                        }
                        outT->setCamPos((Real)(out->getDimensions()[0]-1)/2.0,(Real)(out->getDimensions()[1]-1)/2.0);
                    }
            }

            break;
        case TRIM:
            if(updateImage || updateTransform)
            {
                unsigned int tmin=0; if(p.size())   tmin=(unsigned int)p[0];
                unsigned int tmax=in->getDimensions()[4]-1; if(p.size()>1) tmax=(unsigned int)p[1];
                if(updateImage) if(tmax<in->getDimensions()[4]-1) img.remove(tmax+1,in->getDimensions()[4]-1);
                if(updateImage) if(tmin>0) img.remove(0,tmin-1);
                if(updateTransform) outT->getOffsetT()=outT->fromImage((Real)tmin);
            }
            break;
        case DILATE:
            if(updateImage)
            {
                unsigned int size=0; if(p.size()) size=(unsigned int)p[0];
                cimglist_for(img,l) img(l).dilate (size);
            }
            break;
        case ERODE:
            if(updateImage)
            {
                unsigned int size=0; if(p.size()) size=(unsigned int)p[0];
                cimglist_for(img,l) img(l).erode (size);
            }
            break;
        case NOISE:
            if(updateImage)
            {
                float sigma=0;  if(p.size()) sigma=(float)p[0];
                unsigned int noisetype=0; if(p.size()>1) noisetype=(unsigned int)p[1];
                cimglist_for(img,l) img(l).noise (sigma,noisetype);
            }
            break;
        case QUANTIZE:
            if(updateImage)
            {
                unsigned int nblevels=0; if(p.size()) nblevels=(unsigned int)p[0];
                cimglist_for(img,l) img(l).quantize (nblevels);
            }
            break;
        case THRESHOLD:
            if(updateImage)
            {
                To value=0; if(p.size()) value=(To)p[0];
                cimglist_for(img,l) img(l).threshold (value);
            }
            break;
        case LAPLACIAN:
            if(updateImage)
            {
                cimglist_for(img,l) img(l).laplacian ();
            }
            break;
        case STENSOR:
            if(updateImage)
            {
                unsigned int scheme=1; if(p.size()) scheme=(unsigned int)p[0];
                cimglist_for(img,l) img(l).structure_tensors (scheme);
            }
            break;
        case DISTANCE:
            if(updateImage || updateTransform)
            {
                To value=0; if(p.size()) value=(To)p[0];
                float scale=1; if(p.size()>1) scale=(float)p[1];
                float sizex=(float)inT->getScale()[0]*scale;
                float sizey=(float)inT->getScale()[1]*scale;
                float sizez=(float)inT->getScale()[2]*scale;
                cimglist_for(img,l) {img(l).distance ( value , sizex , sizey , sizez);  }
            }
            break;
        case GRADIENT:
            if(updateImage || updateTransform)
            {
                char axis='a';  if(p.size()) { if((int)p[0]==0) axis='x'; else if((int)p[0]==1) axis='y'; else if((int)p[0]==2) axis='z'; }

                CImg_3x3x3(I,To);
                cimglist_for(img,l)
                {
                    To *ptrd = img(l)._data;
                    const CImg<Ti>& inputImg=in->getCImg(l);
                    // Central finite differences.
                    if(axis=='x') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)      *(ptrd++) = (Incc - Ipcc)*(To)0.5/(To)inT->getScale()[0];
                    else if(axis=='y') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To) *(ptrd++) = (Icnc - Icpc)*(To)0.5/(To)inT->getScale()[1];
                    else if(axis=='z') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To) *(ptrd++) = (Iccn - Iccp)*(To)0.5/(To)inT->getScale()[2];
                    else  cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)
                    {
                        To ix = (Incc - Ipcc)*(To)0.5/(To)inT->getScale()[0];
                        To iy = (Icnc - Icpc)*(To)0.5/(To)inT->getScale()[1];
                        To iz = (Iccn - Iccp)*(To)0.5/(To)inT->getScale()[2];
                        *(ptrd++) = sqrt( (SReal) ix*ix+iy*iy+iz*iz);
                    }
                }
            }
            break;
        case HESSIAN:
            if(updateImage || updateTransform)
            {
                char axis1='x';  if(p.size()) { if((int)p[0]==1) axis1='y'; else if((int)p[0]==2) axis1='z'; }
                char axis2='x';  if(p.size()>1) { if((int)p[1]==1) axis2='y'; else if((int)p[1]==2) axis2='z'; }
                if (axis1>axis2) cimg::swap(axis1,axis2);
                CImg_3x3x3(I,To);
                cimglist_for(img,l)
                {
                    To *ptrd = img(l)._data;
                    const CImg<Ti>& inputImg=in->getCImg(l);
                    // Central finite differences.
                    if(axis1=='x' && axis2=='x') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Ipcc + Incc - 2*Iccc)              /(To)(inT->getScale()[0]*inT->getScale()[0]);
                    else if(axis1=='x' && axis2=='y') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Ippc + Innc - Ipnc - Inpc)*(To)0.25/(To)(inT->getScale()[0]*inT->getScale()[1]);
                    else if(axis1=='x' && axis2=='z') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Ipcp + Incn - Ipcn - Incp)*(To)0.25/(To)(inT->getScale()[0]*inT->getScale()[2]);
                    else if(axis1=='y' && axis2=='y') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Icpc + Icnc - 2*Iccc)              /(To)(inT->getScale()[1]*inT->getScale()[1]);
                    else if(axis1=='y' && axis2=='z') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Icpp + Icnn - Icpn - Icnp)*(To)0.25/(To)(inT->getScale()[1]*inT->getScale()[2]);
                    else if(axis1=='z' && axis2=='z') cimg_forC(inputImg,c) cimg_for3x3x3(inputImg,x,y,z,c,I,To)  *(ptrd++) = (Iccn + Iccp - 2*Iccc)              /(To)(inT->getScale()[2]*inT->getScale()[2]);
                }
            }
            break;

        case NORMALIZE:
            if(updateImage)
            {
                To o1=cimg::type<To>::min();    if(p.size())   o1=(To)p[0];
                To o2=cimg::type<To>::max();    if(p.size()>1) o2=(To)p[1];
                Ti i1=cimg::type<Ti>::min();    if(p.size()>2) i1=(Ti)p[2];
                Ti i2=cimg::type<Ti>::max();    if(p.size()>3) i2=(Ti)p[3];
                cimglist_for(img,l) {img(l).cut(i1 , i2).normalize( o1   , o2);  }
            }
            break;
        case RESAMPLE:
            if(updateImage || updateTransform)
            {
                Coord origin = inT->getTranslation();
                if(p.size()>0) origin[0]=(Real)p[0];
                if(p.size()>1) origin[1]=(Real)p[1];
                if(p.size()>2) origin[2]=(Real)p[2];
                unsigned int dimx=in->getDimensions()[0]; if(p.size()>3) dimx=(unsigned int)p[3];
                unsigned int dimy=in->getDimensions()[1]; if(p.size()>4) dimy=(unsigned int)p[4];
                unsigned int dimz=in->getDimensions()[2]; if(p.size()>5) dimz=(unsigned int)p[5];
                Coord scale = inT->getScale();
                if(p.size()>6) scale[0]=(Real)p[6];
                if(p.size()>7) scale[1]=(Real)p[7];
                if(p.size()>8) scale[2]=(Real)p[8];
                unsigned int interpolation=1; if(p.size()>9) interpolation=(unsigned int)p[9];

                outT->getTranslation() = origin;
                outT->getScale() = scale;
                outT->setCamPos((Real)(out->getDimensions()[0]-1)/2.0,(Real)(out->getDimensions()[1]-1)/2.0);

                const CImgList<Ti>& imgIn = in->getCImgList();
                unsigned int nbc=in->getDimensions()[3];
                Ti OutValue=(Ti)0.;
                cimglist_for(img,l)
                {
                    img(l).resize(dimx,dimy,dimz,nbc);
                    cimg_forXYZ(img(l),x,y,z)
                    {
                        Coord p=inT->toImage(outT->fromImage(Coord(x,y,z)));
                        if(p[0]<-0.5 || p[1]<-0.5 || p[2]<-0.5 || p[0]>imgIn(l).width()-0.5 || p[1]>imgIn(l).height()-0.5 || p[2]>imgIn(l).depth()-0.5)
                            for(unsigned int k=0; k<nbc; k++) img(l)(x,y,z,k) = OutValue;
                        else
                        {
                            if(interpolation==0) for(unsigned int k=0; k<nbc; k++) img(l)(x,y,z,k) = (To) imgIn(l).atXYZ(round((double)p[0]),round((double)p[1]),round((double)p[2]),k);
                            else if(interpolation==1) for(unsigned int k=0; k<nbc; k++) img(l)(x,y,z,k) = (To) imgIn(l).linear_atXYZ(p[0],p[1],p[2],k,OutValue);
                            else if(interpolation==2) for(unsigned int k=0; k<nbc; k++) img(l)(x,y,z,k) = (To) imgIn(l).cubic_atXYZ(p[0],p[1],p[2],k,OutValue,cimg::type<Ti>::min(),cimg::type<Ti>::max());
                        }
                    }
                }

            }
            break;
        case SELECTCHANNEL:
            if(updateImage)
            {
                unsigned int c0=0;    if(p.size())   c0=(unsigned int)p[0];
                unsigned int c1=c0;    if(p.size()>1)   c1=(unsigned int)p[1];
                cimglist_for(img,l) {img(l)=in->getCImg(l).get_channels(c0,c1);  }
            }
            break;
        default:
            break;
        }

        if(updateTransform) outT->update(); // update internal data
    }

};


} // namespace engine

} // namespace component

} // namespace sofa

#endif // SOFA_IMAGE_IMAGEFILTER_H
