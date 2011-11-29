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
#ifndef SOFA_IMAGE_IMAGEPLANEWIDGET_H
#define SOFA_IMAGE_IMAGEPLANEWIDGET_H

#include "initImage.h"
#include <sofa/gui/qt/DataWidget.h>
#include <sofa/gui/qt/SimpleDataWidget.h>

#ifdef SOFA_QT4
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QImage>
#include <QSlider>
#include <QString>
#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtGui>
#else
#include <qlabel.h>
#include <qvboxlayout.h>
#include <qgridlayout.h>
#include <qimage.h>
#include <qlayout.h>
#include <qslider.h>
#include <qstring.h>
#include <qdoublespinbox.h>
#include <qgraphicsView.h>
#include <qgraphicsscene.h>
#include <qtgui.h>
#endif

#include "ImageTypes.h"
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/helper/rmath.h>
#include <sofa/helper/vector.h>


#if !defined(INFINITY)
#define INFINITY 9.0e10
#endif


namespace sofa
{
namespace gui
{
namespace qt
{

using namespace helper;

//-----------------------------------------------------------------------------------------------//
//	image widget -> draw image and handle mouse events
//-----------------------------------------------------------------------------------------------//

class ImagePlaneGraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    ImagePlaneGraphScene(QImage* im,QObject *parent=0) : QGraphicsScene(parent)	, drawrectangle(false),image(im) 	{ this->setSceneRect(0,0,image->width(),image->height()); }
    QPointF P1;
    QPointF P2;
    bool drawrectangle;

private:
    const QImage *image;
    void drawBackground(QPainter *painter, const QRectF &rect)
    {
        QGraphicsScene::drawBackground(painter,rect);
        if(image) painter->drawImage(this->sceneRect(),*image);
    }

    void drawForeground(QPainter *painter, const QRectF &rect)
    {
        QGraphicsScene::drawForeground(painter,rect);

        if(drawrectangle)
        {
            QRectF r(P1,P2);
            painter->setPen ( Qt::NoPen);
            painter->setBrush(Qt::SolidPattern);
            painter->setBrush(Qt::cyan);
            painter->setOpacity (0.1);
            painter->drawRect(r);

            painter->setPen ( QPen ( Qt::darkCyan, 0.5, Qt::DotLine ) );
            painter->setBrush(Qt::NoBrush);
            painter->setOpacity (1);
            painter->drawRect(r);
        }
        else
        {
            QRectF r(round(P1.x()),round(P1.y()),1,1);
            painter->setPen ( Qt::NoPen);
            painter->setBrush(Qt::SolidPattern);
            painter->setBrush(Qt::green);
            painter->setOpacity (0.5);
            painter->drawRect(r);
        }
    }

};


class ImagePlaneGraphWidget : public QGraphicsView
{
    Q_OBJECT

public slots:
    void Render ()  { if(this->scene) this->scene->update(); }
    void setRoiHorizontal(const qreal left, const qreal width)	{roi.setLeft(left); roi.setWidth(width);	this->fitInView(); 	}
    void setRoiVertical(const qreal top, const qreal height)	{roi.setTop(top);	roi.setHeight(height);	this->fitInView(); 	}
    void setCursorX(const qreal v)	{scene->P1.setX(v); Render(); 	}
    void setCursorY(const qreal v)	{scene->P1.setY(v); Render(); 	}
    void togglemodels(int i) { this->visumodels=i?true:false; draw(); }
    void setScrollBarPolicies(bool on)  { if(on) { this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); 	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); }	else   { this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);} }

signals:
    void wheelevent (int delta); // -> change index on slider
    void roiHorizontalChanged(const qreal left, const qreal width); // to synchronize different views (roi)
    void roiVerticalChanged(const qreal top,const qreal height);	// to synchronize different views (roi)
    void cursorChangedX(const qreal);		// to synchronize different views (cursor)
    void cursorChangedY(const qreal);		// to synchronize different views (cursor)
    void cursorChangedZ(const qreal);		// to synchronize different views (cursor)
    void roiResized();		// to synchronize different views (scrollBars on/off)

    void sliceModified();	// when the slice index is modified using slider or wheel -> set widget dirty
    void onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&);  // when the mouse is on image -> update info

public:
    ImagePlaneGraphWidget(QWidget *parent )	: QGraphicsView(parent),scene(NULL)   {	 this->setMouseTracking(true); }

    virtual QSize 	sizeHint () const { return QSize(10000,10000); }  // for vertical expanding

    virtual void fromOption(const unsigned int i)=0;					// slice index change -> redraw
    virtual void fromGraph(const QPointF &pt,const bool clicked)=0;		// mouse move -> get image info
    virtual void draw()=0;

    bool isRoiResized() {return RoiResized;}

protected:
    void resizeEvent ( QResizeEvent* /*resizeevent*/)  { this->fitInView();	}

    void fitInView() // modified version of original fitinview (no margin)
    {
        QRectF unity = this->matrix().mapRect(QRectF(0, 0, 1, 1));
        scale(1 / unity.width(), 1 / unity.height());
        QRectF sceneRect = this->matrix().mapRect(roi);
        scale(viewport()->rect().width() / sceneRect.width(), viewport()->rect().height() / sceneRect.height());
        centerOn(roi.center());
    }


    void setRoi ( const QRectF &rect )
    {
        if(!rect.width() || !rect.height()) return;
        roi=rect;
        RoiResized=(roi.width()==image.width() && roi.height()==image.height())?false:true;
        this->setScrollBarPolicies(RoiResized);
        this->fitInView();
        emit roiHorizontalChanged(roi.left(),roi.width());
        emit roiVerticalChanged(roi.top(),roi.height());
    }

    //void scrollContentsBy ( int dx, int dy )
    //{
    //	QGraphicsView::scrollContentsBy(dx,dy);
    //	roi.translate  ( this->mapToScene(dx, dy ));
    //}

    void mousePressEvent(QMouseEvent *mouseEvent)
    {
        QGraphicsView::mousePressEvent(mouseEvent);
        if (mouseEvent->modifiers()==Qt::ControlModifier)	{	scene->P1=scene->P2=this->mapToScene(mouseEvent->pos());		scene->drawrectangle=true; }
    }

    void mouseReleaseEvent(QMouseEvent *mouseEvent)
    {
        QGraphicsView::mouseReleaseEvent(mouseEvent);
        if (scene->drawrectangle) {  setRoi(QRectF(scene->P1,scene->P2)); emit roiResized(); scene->drawrectangle=false; }
    }

    void mouseDoubleClickEvent ( QMouseEvent *mouseEvent )
    {
        QGraphicsView::mouseDoubleClickEvent(mouseEvent);
        if (mouseEvent->modifiers()==Qt::ControlModifier)  {  setRoi(QRectF(0,0,image.width(),image.height()));  emit roiResized(); }
    }

    void mouseMoveEvent(QMouseEvent *mouseEvent)
    {
        QGraphicsView::mouseMoveEvent(mouseEvent);
        QPointF pt(this->mapToScene(mouseEvent->pos()));
        pt.setX(pt.x()-0.5); pt.setY(pt.y()-0.5);
        this->fromGraph(pt,false); // update info

        if (scene->drawrectangle)	{scene->P2=this->mapToScene(mouseEvent->pos()); Render ();}
        else { scene->P1=pt; emit cursorChangedX(pt.x()); emit cursorChangedY(pt.y()); Render ();}
    }

    void wheelEvent (QWheelEvent *wheelev) {		emit wheelevent((wheelev->delta()>0)?1:-1);  }

    ImagePlaneGraphScene * scene;
    QRectF roi;
    QImage image;
    bool visumodels;
    unsigned int axis;	// slice axis
    unsigned int index;	// slice index
    bool RoiResized;
};


//-----------------------------------------------------------------------------------------------------------------//
//  template<ImagePlanetype> version of ImagePlaneGraphWidget -> generate images from Data<ImagePlanetype>
//-----------------------------------------------------------------------------------------------------------------//

template <class DataType>
class TImagePlaneGraphWidget: public ImagePlaneGraphWidget
{
    typedef DataType ImagePlanetype;
    typedef typename ImagePlanetype::T T;
    typedef typename ImagePlanetype::Real Real;
    typedef typename ImagePlanetype::pCoord pCoord;
    typedef typename ImagePlanetype::Coord Coord;

protected:
    const ImagePlanetype* imageplane;
    unsigned int backupindex;
    unsigned int indexmax;
    //CImg<unsigned char> cimage;

public:
    TImagePlaneGraphWidget(QWidget * parent,unsigned int _axis,const ImagePlanetype& d0)
        :ImagePlaneGraphWidget(parent),imageplane(NULL),indexmax(0)
    {
        this->axis=_axis; 		if(this->axis>2) this->axis=2;
        this->index=0;
        this->visumodels=true;
        readFromData(d0);
        this->setScrollBarPolicies(false);
//			this->horizontalScrollBar()->setFixedHeight ( scrollbarsize );
//			this->verticalScrollBar()->setFixedWidth( scrollbarsize );
        //		this->cornerWidget()->setFixedSize(scrollbarsize+2,scrollbarsize+2);
        this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        scene = new ImagePlaneGraphScene(&image,this);
        this->setScene(scene);
        setRoi(QRectF(0,0,image.width(),image.height()));
        Render();
    }

    virtual ~TImagePlaneGraphWidget() {};

    void readFromData(const ImagePlanetype& d0)
    {
        this->imageplane=&d0;
        if(!this->imageplane) return;

        this->indexmax=this->imageplane->getDimensions()[this->axis]-1;
        this->index=this->backupindex=this->imageplane->getPlane()[this->axis];

        CImg<T> temp = this->imageplane->get_slice(0,this->axis);
        this->image=QImage(temp.width(),temp.height(),QImage::Format_RGB32);
        draw();
    }

    void writeToData(ImagePlanetype& d)
    {
        pCoord plane=d.getPlane();
        plane[this->axis]=this->index;
        d.setPlane(plane);
    }

    void fromGraph(const QPointF &pt,const bool /*clicked*/)
    {
        if(!this->imageplane) return;
        Coord P;
        if(this->axis==0) P=Coord(this->index,(pt.y()),(pt.x()));
        else if(this->axis==1) P=Coord((pt.x()),this->index,(pt.y()));
        else P=Coord((pt.x()),(pt.y()),this->index);

        Coord p=this->imageplane->get_pointCoord(P);
        CImg<T> val=this->imageplane->get_point(P);

        QString tval;
        if(!val) tval=QString("-");
        else if(val.spectrum()==1) tval=QString().setNum((double)val(0,0,0,0));
        else
        {
            tval.push_back("[");
            cimg_forC(val,c) tval.push_back(QString().setNum((double)val(0,0,0,c))+",");
            tval.chop(1); tval.push_back("]");
        }

        emit onPlane(this->axis,P,p,tval);
    }

    void fromOption(const unsigned int i)
    {
        if(!this->imageplane) return;
        if(this->index!=i)
        {
            this->index=i;
            emit sliceModified(); // -> set widget dirty
            emit cursorChangedZ((qreal)this->index);   // -> change cursor
        }
        draw();
    }

    void draw()
    {
        if(!this->imageplane) return;

        CImg<unsigned char> plane = convertToUC( this->imageplane->get_slice(this->index,this->axis).cut(imageplane->getClamp()[0],imageplane->getClamp()[1]) );
        CImg<unsigned char> slicedModels; 	if(this->visumodels) slicedModels = this->imageplane->get_slicedModels(this->index,this->axis);

        if(plane)
            for( int y=0; y<this->image.height(); y++)
                for( int x=0; x<this->image.width(); x++)
                    if(plane.spectrum()<3) this->image.setPixel ( x, y,  qRgb(plane(x,y,0,0),plane(x,y,0,0),plane(x,y,0,0)));
                    else this->image.setPixel ( x, y,  qRgb(plane(x,y,0,0),plane(x,y,0,1),plane(x,y,0,2)));

        if(slicedModels)
            for( int y=0; y<this->image.height(); y++)
                for( int x=0; x<this->image.width(); x++)
                    if(slicedModels(x,y,0,0)) if(slicedModels(x,y,0,1)) if(slicedModels(x,y,0,2))
                                this->image.setPixel ( x, y,  qRgb(slicedModels(x,y,0,0),slicedModels(x,y,0,1) ,slicedModels(x,y,0,2)));

        Render();
    }


    const unsigned int& getIndexMax() const {return indexmax;}
    const unsigned int& getIndex() const {return index;}

};




//-----------------------------------------------------------------------------------------------//
//	slider widget -> select plane  + toggle button
//-----------------------------------------------------------------------------------------------//

class ImagePlaneOptionWidget: public QWidget
{
    Q_OBJECT

public:

    ImagePlaneOptionWidget(ImagePlaneGraphWidget *s, QWidget *parent,const int minimum, const int maximum,const int val, const int height)
        : QWidget(parent) , S(s)
    {
        slider=new QSlider(Qt::Horizontal,this);
        slider->setPageStep ( 1 );
        slider->setRange(minimum,maximum);
        slider->setValue(val);
        slider->setFixedHeight ( height );
        connect(slider, SIGNAL( valueChanged(int) ), this, SLOT( change(int) ) );

        label=new QLabel(this);
        label->setNum(val);
        label->setFixedHeight ( height );

        toggle = new QCheckBox(this);
        toggle->setChecked(true);
        toggle->setFixedHeight ( height );
        QObject::connect(toggle, SIGNAL( stateChanged(int) ), this, SLOT( togglestateChanged(int) ) );
        QObject::connect(this,SIGNAL(toggled(bool)),slider,SLOT(setVisible(bool)));
        QObject::connect(this,SIGNAL(toggled(bool)),label,SLOT(setVisible(bool)));

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(10);
        layout->add(toggle);
        layout->add(slider);
        layout->add(label);

        this->setFixedHeight ( height );
    }

    void setChecked(bool val) { toggle->setChecked(val);}

signals:
    void toggled(bool);

public slots:

    void change(int i)
    {
        S->fromOption(i);
        label->setNum(i);
    }

    void togglestateChanged(int i)
    {
        if(i) {emit toggled(true); S->fromOption(slider->value()); }
        else {emit toggled(false); S->fromOption(slider->maximum()+1); }

    }

    void changeSlider ( int delta ) { slider->setValue(slider->value() + delta ); }

protected:
    QCheckBox* toggle;
    QSlider *slider;
    QLabel *label;
    ImagePlaneGraphWidget *S;
};




//-----------------------------------------------------------------------------------------------//
//	show info (mouse position, etc.)
//-----------------------------------------------------------------------------------------------//

class ImagePlaneInfoWidget: public QWidget
{
    Q_OBJECT

public:

    ImagePlaneInfoWidget(QWidget *parent)
        : QWidget(parent)
    {
        label1=new QLabel(this);
        label2=new QLabel(this);
        label3=new QLabel(this);

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->add(label1);
        layout->add(label2);
        layout->add(label3);
    }

public slots:

    void onPlane(const unsigned int /*axis*/,const Vector3& ip,const Vector3& p,const QString& val)
    {
        label1->setText("Pixel value = " + val);
        label2->setText("Image position = [ " + QString().setNum((int)round(ip[0])) + "," + QString().setNum((int)round(ip[1])) + "," + QString().setNum((int)round(ip[2]))+ " ]");
        label3->setText("3D position = [ " + QString().setNum((float)p[0]) + "," + QString().setNum((float)p[1]) + "," + QString().setNum((float)p[2])+ " ]");
    }

protected:
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
};

//-----------------------------------------------------------------------------------------------//
//	Widget Container + show info (mouse position, etc.)
//-----------------------------------------------------------------------------------------------//

class imageplane_data_widget_container: public QObject
{
    Q_OBJECT;

public slots:
    virtual void handleSliderPolicies()=0; // needed for synchronization of slider visiblity
};

template<class T>
class Timageplane_data_widget_container: public imageplane_data_widget_container
{
    typedef T ImagePlanetype;
    typedef TImagePlaneGraphWidget<ImagePlanetype> Graph;
    typedef ImagePlaneOptionWidget Options;
    typedef QVBoxLayout Layout;

    static const unsigned int optionheight=15;

public:

    Options *optionsXY;
    Graph* graphXY;

    Options *optionsXZ;
    Graph* graphXZ;

    Options *optionsZY;
    Graph* graphZY;

    Layout* container_layout;

    ImagePlaneInfoWidget* info;

    QCheckBox* togglemodels;

    Timageplane_data_widget_container()
        : optionsXY(NULL), graphXY(NULL),
          optionsXZ(NULL), graphXZ(NULL),
          optionsZY(NULL), graphZY(NULL),
          container_layout(NULL), info(NULL)
    {}

    bool createLayout( DataWidget* parent )
    {
        if( parent->layout() != NULL || container_layout != NULL ) return false;
        container_layout = new Layout(parent);
        return true;
    }

    bool createLayout( QLayout* layout)
    {
        if ( container_layout != NULL ) return false;
        container_layout = new Layout(layout);
        return true;
    }

    bool createWidgets(DataWidget* parent, const ImagePlanetype& d, bool /*readOnly*/)
    {
        info = new ImagePlaneInfoWidget(parent);

        togglemodels = new QCheckBox(QString("Visual Models"),parent); togglemodels->setChecked(true);

        if(d.getDimensions()[0]>1 && d.getDimensions()[1]>1)
        {
            graphXY = new Graph(parent,2,d);
            QObject::connect(togglemodels, SIGNAL( stateChanged(int) ), graphXY, SLOT( togglemodels(int) ) );
            QObject::connect(graphXY,SIGNAL(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)),info,SLOT(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)));
            if(d.getDimensions()[2]>1)
            {
                optionsXY = new Options(graphXY,parent,0,graphXY->getIndexMax(),graphXY->getIndex(),optionheight);
                QObject::connect(optionsXY,SIGNAL(toggled(bool)),graphXY,SLOT(setVisible(bool)));
                QObject::connect(graphXY,SIGNAL(wheelevent(int)),optionsXY,SLOT(changeSlider(int)));
                if(graphXY->getIndex()>graphXY->getIndexMax()) optionsXY->setChecked(false);
            }
        }

        if(d.getDimensions()[0]>1 && d.getDimensions()[2]>1)
        {
            graphXZ = new Graph(parent,1,d);
            QObject::connect(togglemodels, SIGNAL( stateChanged(int) ), graphXZ, SLOT( togglemodels(int) ) );
            QObject::connect(graphXZ,SIGNAL(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)),info,SLOT(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)));
            if(d.getDimensions()[1]>1)
            {
                optionsXZ = new Options(graphXZ,parent,0,graphXZ->getIndexMax(),graphXZ->getIndex(),optionheight);
                QObject::connect(optionsXZ,SIGNAL(toggled(bool)),graphXZ,SLOT(setVisible(bool)));
                QObject::connect(graphXZ,SIGNAL(wheelevent(int)),optionsXZ,SLOT(changeSlider(int)));
                if(graphXZ->getIndex()>graphXZ->getIndexMax()) optionsXZ->setChecked(false);
            }
        }

        if(d.getDimensions()[1]>1 && d.getDimensions()[2]>1)
        {
            graphZY = new Graph(parent,0,d);
            QObject::connect(togglemodels, SIGNAL( stateChanged(int) ), graphZY, SLOT( togglemodels(int) ) );
            QObject::connect(graphZY,SIGNAL(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)),info,SLOT(onPlane(const unsigned int,const Vector3&,const Vector3&,const QString&)));
            if(d.getDimensions()[0]>1)
            {
                optionsZY = new Options(graphZY,parent,0,graphZY->getIndexMax(),graphZY->getIndex(),optionheight);
                QObject::connect(optionsZY,SIGNAL(toggled(bool)),graphZY,SLOT(setVisible(bool)));
                QObject::connect(graphZY,SIGNAL(wheelevent(int)),optionsZY,SLOT(changeSlider(int)));
                if(graphZY->getIndex()>graphZY->getIndexMax()) optionsZY->setChecked(false);
            }
        }

        if(graphXY && graphXZ && graphZY) // synchronize views
        {
            QObject::connect(graphXY->horizontalScrollBar (),SIGNAL(valueChanged (int)),graphXZ->horizontalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphXY,SIGNAL(roiHorizontalChanged(const qreal,const qreal)),graphXZ,SLOT(setRoiHorizontal(const qreal,const qreal)));
            QObject::connect(graphXY->verticalScrollBar (),SIGNAL(valueChanged (int)),graphZY->verticalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphXY,SIGNAL(roiVerticalChanged(const qreal,const qreal)),graphZY,SLOT(setRoiVertical(const qreal,const qreal)));
            QObject::connect(graphXY,SIGNAL(cursorChangedX(const qreal)),graphXZ,SLOT(setCursorX(const qreal)));
            QObject::connect(graphXY,SIGNAL(cursorChangedZ(const qreal)),graphXZ,SLOT(setCursorY(const qreal)));
            QObject::connect(graphXY,SIGNAL(cursorChangedY(const qreal)),graphZY,SLOT(setCursorY(const qreal)));
            QObject::connect(graphXY,SIGNAL(cursorChangedZ(const qreal)),graphZY,SLOT(setCursorX(const qreal)));

            QObject::connect(graphXZ->horizontalScrollBar (),SIGNAL(valueChanged (int)),graphXY->horizontalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphXZ,SIGNAL(roiHorizontalChanged(const qreal,const qreal)),graphXY,SLOT(setRoiHorizontal(const qreal,const qreal)));
            QObject::connect(graphXZ->verticalScrollBar (),SIGNAL(valueChanged (int)),graphZY->horizontalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphXZ,SIGNAL(roiVerticalChanged(const qreal,const qreal)),graphZY,SLOT(setRoiHorizontal(const qreal,const qreal)));
            QObject::connect(graphXZ,SIGNAL(cursorChangedX(const qreal)),graphXY,SLOT(setCursorX(const qreal)));
            QObject::connect(graphXZ,SIGNAL(cursorChangedZ(const qreal)),graphXY,SLOT(setCursorY(const qreal)));
            QObject::connect(graphXZ,SIGNAL(cursorChangedY(const qreal)),graphZY,SLOT(setCursorX(const qreal)));
            QObject::connect(graphXZ,SIGNAL(cursorChangedZ(const qreal)),graphZY,SLOT(setCursorY(const qreal)));

            QObject::connect(graphZY->horizontalScrollBar (),SIGNAL(valueChanged (int)),graphXZ->verticalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphZY,SIGNAL(roiHorizontalChanged(const qreal,const qreal)),graphXZ,SLOT(setRoiVertical(const qreal,const qreal)));
            QObject::connect(graphZY->verticalScrollBar (),SIGNAL(valueChanged (int)),graphXY->verticalScrollBar (),SLOT(setValue(int)));
            QObject::connect(graphZY,SIGNAL(roiVerticalChanged(const qreal,const qreal)),graphXY,SLOT(setRoiVertical(const qreal,const qreal)));
            QObject::connect(graphZY,SIGNAL(cursorChangedX(const qreal)),graphXZ,SLOT(setCursorY(const qreal)));
            QObject::connect(graphZY,SIGNAL(cursorChangedZ(const qreal)),graphXZ,SLOT(setCursorX(const qreal)));
            QObject::connect(graphZY,SIGNAL(cursorChangedY(const qreal)),graphXY,SLOT(setCursorY(const qreal)));
            QObject::connect(graphZY,SIGNAL(cursorChangedZ(const qreal)),graphXY,SLOT(setCursorX(const qreal)));

            QObject::connect(graphXY,SIGNAL(roiResized()), this, SLOT(handleSliderPolicies()));
            QObject::connect(graphXZ,SIGNAL(roiResized()), this, SLOT(handleSliderPolicies()));
            QObject::connect(graphZY,SIGNAL(roiResized()), this, SLOT(handleSliderPolicies()));

        }

        return true;
    }

    void setReadOnly(bool /*readOnly*/) { }
    void readFromData(const ImagePlanetype& d0) {  if(graphXY) graphXY->readFromData(d0); if(graphXZ) graphXZ->readFromData(d0); if(graphZY) graphZY->readFromData(d0);}
    void writeToData(ImagePlanetype& d) { if(graphXY) graphXY->writeToData(d); if(graphXZ) graphXZ->writeToData(d); if(graphZY) graphZY->writeToData(d);}

    void insertWidgets()
    {
        assert(container_layout);

        QGridLayout* layout = new QGridLayout();
        if(graphXY) layout->addWidget(graphXY,0,0);
        if(optionsXY) layout->addWidget(optionsXY,1,0);

        if(graphXZ) layout->addWidget(graphXZ,2,0);
        if(optionsXZ) layout->addWidget(optionsXZ,3,0);

        if(graphZY) layout->addWidget(graphZY,0,1);
        if(optionsZY) layout->addWidget(optionsZY,1,1);

        container_layout->addLayout(layout);

        container_layout->add(togglemodels);

        //if(graphXY && graphXZ && graphZY) layout->addWidget(info,2,1);
        //else
        container_layout->add(info);

    }

    void handleSliderPolicies()
    {
        if(graphXY && graphXZ && graphZY)
        {
            if(this->graphXY->isRoiResized() || this->graphXZ->isRoiResized() || this->graphZY->isRoiResized())
            {
                this->graphXY->setScrollBarPolicies(true);
                this->graphXZ->setScrollBarPolicies(true);
                this->graphZY->setScrollBarPolicies(true);
            }
            else
            {
                this->graphXY->setScrollBarPolicies(false);
                this->graphXZ->setScrollBarPolicies(false);
                this->graphZY->setScrollBarPolicies(false);
            }
        }
    }
};


template<class T>
class SOFA_IMAGE_API ImagePlaneDataWidget : public SimpleDataWidget<T, Timageplane_data_widget_container< T > >
{
    typedef SimpleDataWidget<T, Timageplane_data_widget_container< T > > Inherit;
    typedef sofa::core::objectmodel::Data<T> MyData;

public:
    ImagePlaneDataWidget(QWidget* parent,const char* name, MyData* d) : Inherit(parent,name,d) {}
    virtual unsigned int sizeWidget() {return 3;}
    virtual unsigned int numColumnWidget() {return 1;}
    virtual bool createWidgets()
    {
        bool b = Inherit::createWidgets();

        ImagePlaneGraphWidget* sXY = dynamic_cast<ImagePlaneGraphWidget*>(this->container.graphXY);
        ImagePlaneGraphWidget* sXZ = dynamic_cast<ImagePlaneGraphWidget*>(this->container.graphXZ);
        ImagePlaneGraphWidget* sZY = dynamic_cast<ImagePlaneGraphWidget*>(this->container.graphZY);

        if(sXY) QObject::connect(sXY,SIGNAL(sliceModified()), this, SLOT(setWidgetDirty()));
        if(sXZ) QObject::connect(sXZ,SIGNAL(sliceModified()), this, SLOT(setWidgetDirty()));
        if(sZY) QObject::connect(sZY,SIGNAL(sliceModified()), this, SLOT(setWidgetDirty()));

        return b;
    }
};


}

}

}

#endif // SOFA_IMAGE_IMAGEPLANEWIDGET_H