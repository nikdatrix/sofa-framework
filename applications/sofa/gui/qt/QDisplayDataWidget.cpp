#include "QDisplayDataWidget.h"
#include "ModifyObject.h"
#include "QMonitorTableWidget.h"


#ifdef SOFA_QT4
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Q3GroupBox>
#include <QLabel>
#include <QValidator>
#else
#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qvalidator.h>
#endif

#define TEXTSIZE_THRESHOLD 45

namespace sofa
{

using namespace core::objectmodel;
using namespace sofa::component::misc;

namespace gui
{
namespace qt
{
QDisplayDataWidget::QDisplayDataWidget(QWidget* parent,
        BaseData* data,
        const ModifyObjectFlags& flags):Q3GroupBox(parent),
    data_(data),
    datainfowidget_(NULL),
    datawidget_(NULL),
    numWidgets_(0)

{
    if(data_ == NULL)
    {
        return;
    }

    setTitle(data_->getName().c_str());

    const std::string label_text = data_->getHelp();
    if (label_text != "TODO")
    {
        datainfowidget_ = new QDisplayDataInfoWidget(this,label_text,data_,flags.LINKPATH_MODIFIABLE_FLAG);
        numWidgets_ += datainfowidget_->getNumLines()/3;

    }

    DataWidget::CreatorArgument dwarg;
    dwarg.name =  data_->getName();
    dwarg.data = data_;
    dwarg.parent = this;
    dwarg.readOnly = (data_->isReadOnly() && flags.READONLY_FLAG);

    std::string widget = data_->getWidget();

    if (widget.empty())
        datawidget_ = DataWidgetFactory::CreateAnyObject(dwarg);
    else
        datawidget_ = DataWidgetFactory::CreateObject(dwarg.data->getWidget(), dwarg);
    if (datawidget_ == NULL)
    {
        datawidget_ = new QDataSimpleEdit(this,data_->getName().c_str(), data_);
        datawidget_->createWidgets();
        datawidget_->setEnabled( !(data_->isReadOnly() && flags.READONLY_FLAG) );
        assert(datawidget_ != NULL);
    }

    setColumns(datawidget_->numColumnWidget());
    //std::cout << "WIDGET created for data " << dwarg.data << " : " << dwarg.name << " : " << dwarg.data->getValueTypeString() << std::endl;
    numWidgets_+=datawidget_->sizeWidget();
    connect(datawidget_,SIGNAL(WidgetDirty(bool)), this, SIGNAL ( WidgetDirty(bool) ) );
    connect(this, SIGNAL( WidgetUpdate() ), datawidget_, SLOT( updateWidgetValue() ) );
    connect(this, SIGNAL( DataUpdate() ), datawidget_, SLOT(updateDataValue() ) );
    connect(datawidget_,SIGNAL(DataOwnerDirty(bool)),this,SIGNAL(DataOwnerDirty(bool)) );

}

void QDisplayDataWidget::UpdateData()
{
    emit DataUpdate();
}

void QDisplayDataWidget::UpdateWidgets()
{
    emit WidgetUpdate();
}

QDataSimpleEdit::QDataSimpleEdit(QWidget* parent, const char* name, BaseData* data):
    DataWidget(parent,name,data)
{
}
bool QDataSimpleEdit::createWidgets()
{
    QString str  = QString( getBaseData()->getValueString().c_str() );
    if( str.length() > TEXTSIZE_THRESHOLD )
    {
        innerWidget_.type = TEXTEDIT;
        innerWidget_.widget.textEdit = new QTextEdit(this->parentWidget());
        connect(innerWidget_.widget.textEdit , SIGNAL( textChanged() ), this, SLOT ( setWidgetDirty() ) );
        innerWidget_.widget.textEdit->setText(str);
    }
    else
    {
        innerWidget_.type = LINEEDIT;
        innerWidget_.widget.lineEdit  = new QLineEdit(this->parentWidget());
        connect( innerWidget_.widget.lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT( setWidgetDirty() ) );
        innerWidget_.widget.lineEdit->setText(str);
    }

    return true;
}

void QDataSimpleEdit::readFromData()
{
    QString str = QString( getBaseData()->getValueString().c_str() );
    if(innerWidget_.type == TEXTEDIT)
    {
        innerWidget_.widget.textEdit->setText(str);
    }
    else if(innerWidget_.type == LINEEDIT)
    {
        innerWidget_.widget.lineEdit->setText(str);
    }

}

void QDataSimpleEdit::writeToData()
{
    if(getBaseData())
    {
        std::string value;
        if( innerWidget_.type == TEXTEDIT)
        {
            value = innerWidget_.widget.textEdit->text().ascii();
        }
        else if( innerWidget_.type == LINEEDIT)
        {
            value = innerWidget_.widget.lineEdit->text().ascii();
        }
        getBaseData()->read(value);
    }
}

/* QPoissonRatioWidget */

QPoissonRatioWidget::QPoissonRatioWidget(QWidget * parent, const char * name, sofa::core::objectmodel::TData<double> *data)
    :TDataWidget<double>(parent,name,data)
{

}


bool QPoissonRatioWidget::createWidgets()
{
    QGridLayout* layout = new QGridLayout(this,2,3);

    lineEdit = new QLineEdit(this);
    lineEdit->setText(QString("-1.0"));
    lineEdit->setMaximumSize(lineEdit->size());
    lineEdit->setAlignment(Qt::AlignHCenter);

    lineEdit->setValidator(new QDoubleValidator(0.0,0.5,2,this));

    layout->addWidget(lineEdit,0,1,Qt::AlignHCenter);
    QLabel* min = new QLabel(this);
    min->setText(QString("0.0"));
    min->setMaximumSize( min->sizeHint() );
    layout->addWidget(min,1,0,Qt::AlignHCenter);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0,50); //max times 10 at the power 2 (2 digits after ".")
    slider->setTickmarks(QSlider::Below);
    slider->setTickInterval(5);
    layout->addWidget(slider,1,1,Qt::AlignHCenter);

    QLabel* max = new QLabel(this);
    max->setText(QString("0.5"));
    max->setMaximumSize ( max->sizeHint() );

    layout->addWidget(max,1,2,Qt::AlignHCenter);

    // synchronization between qslider and qlineedit
    connect(slider, SIGNAL( valueChanged(int) ), this, SLOT ( changeLineEditValue() ) );
    connect(slider, SIGNAL( sliderReleased()   ), this, SLOT ( changeLineEditValue() ) );
    connect(lineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT (changeSliderValue() ) );

    // synchronization between the widgets and the modify object dialog box
    connect(lineEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( setWidgetDirty() ) );
    connect(slider, SIGNAL( sliderReleased()  ), this, SLOT ( setWidgetDirty() ) );
    connect(slider, SIGNAL( valueChanged(int) ), this, SLOT ( setWidgetDirty() ) );


    return true;
}

void QPoissonRatioWidget::readFromData()
{
    double value = this->getData()->virtualGetValue();
    QString str;
    str.setNum(value);
    lineEdit->setText(str);
    changeSliderValue();
}

void QPoissonRatioWidget::writeToData()
{
    bool ok;
    double d = lineEdit->text().toDouble(&ok);
    if(ok)
    {
        this->getData()->virtualSetValue(d);
    }
}

void QPoissonRatioWidget::changeLineEditValue()
{
    int v = slider->value();
    double db = (double)v / 100.;
    QString str;
    str.setNum(db);
    lineEdit->setText(str);
}

void QPoissonRatioWidget::changeSliderValue()
{
    bool ok;
    double v = lineEdit->text().toDouble(&ok);
    if(ok)
    {
        slider->setValue( (int)(v*100.) );
    }
}

helper::Creator<DataWidgetFactory, QPoissonRatioWidget> DWClass_Poissonratio("poissonRatio",false);

} // qt
} //gui
} //sofa

