/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSurfaceSpatialObject.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkSurfaceSpatialObject_txx
#define __itkSurfaceSpatialObject_txx

#include "itkSurfaceSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
template< unsigned int TDimension , unsigned int SpaceDimension >
SurfaceSpatialObject< TDimension, SpaceDimension > 
::SurfaceSpatialObject()  
{ 
  m_Dimension = TDimension;
  strcpy(m_TypeName,"SurfaceSpatialObject");
  m_Property->SetRed(1); 
  m_Property->SetGreen(0); 
  m_Property->SetBlue(0); 
  m_Property->SetAlpha(1); 
  ComputeBoundingBox();
} 

/** Destructor */ 
template< unsigned int TDimension , unsigned int SpaceDimension >
SurfaceSpatialObject< TDimension, SpaceDimension >  
::~SurfaceSpatialObject()
{ 
} 
 
/** Get the list of points composing the surface */
template< unsigned int TDimension , unsigned int SpaceDimension >
typename SurfaceSpatialObject< TDimension, SpaceDimension > ::PointListType &  
SurfaceSpatialObject< TDimension, SpaceDimension > 
::GetPoints() 
{ 
  itkDebugMacro( "Getting SurfacePoint list" );
  return m_Points;
} 
 
/** Set the list of points composing the surface */
template< unsigned int TDimension , unsigned int SpaceDimension >
void  
SurfaceSpatialObject< TDimension, SpaceDimension >  
::SetPoints( PointListType & points )  
{
  // in this function, passing a null pointer as argument will
  // just clear the list...
  m_Points.clear();
   
  typename PointListType::iterator it,end;
  it = points.begin();    
  end = points.end();
  while(it != end)
  {
    m_Points.push_back(*it);
    it++;
  } 
   
  this->Modified();
}
 
/** Print the surface object */
template< unsigned int TDimension , unsigned int SpaceDimension >
void  
SurfaceSpatialObject< TDimension, SpaceDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "SurfaceSpatialObject(" << this << ")" << std::endl; 
  os << indent << "ID: " << m_Id << std::endl; 
  os << indent << "nb of points: "<< m_Points.size() << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 

/** Compute the bounds of the surface */
template< unsigned int TDimension , unsigned int SpaceDimension >
bool 
SurfaceSpatialObject< TDimension, SpaceDimension >  
::ComputeBoundingBox( unsigned int depth, char * name ) 
{ 
  itkDebugMacro( "Computing surface bounding box" );
  bool ret = false;

  if( this->GetMTime() > m_BoundsMTime )
    {
    ret = Superclass::ComputeBoundingBox(depth, name);

    if(name == NULL || strstr(typeid(Self).name(), name) )
      {
      typename PointListType::iterator it  = m_Points.begin();
      typename PointListType::iterator end = m_Points.end();
  
      if(it == end)
        {
        return ret;
        }
      else
        {
        if(!ret)
          {
          m_Bounds->SetMinimum((*it).GetPosition());
          m_Bounds->SetMaximum((*it).GetPosition());
          it++;
          }
        while(it!= end) 
          {     
          m_Bounds->ConsiderPoint((*it).GetPosition());
          it++;
          }
        ret = true;
        }
      }

    m_BoundsMTime = this->GetMTime();
    }

  return ret;
} 

/** Return true is the given point is on the surface */
template< unsigned int TDimension , unsigned int SpaceDimension >
bool 
SurfaceSpatialObject< TDimension, SpaceDimension >  
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "is on the surface" );

  if(name == NULL || strstr(typeid(Self).name(), name) )
    {
    typename PointListType::const_iterator it = m_Points.begin();
    typename PointListType::const_iterator itEnd = m_Points.end();
    
    PointType transformedPoint = point;
    TransformPointToLocalCoordinate(transformedPoint);
  
    if( m_Bounds->IsInside(transformedPoint) )
      {
      while(it != itEnd)
        {
        if((*it).GetPosition() == transformedPoint)
          {
          return true;
          }
        it++;
        }
      }
    }

  return Superclass::IsInside(point, depth, name);
} 

/** Return true if the surface is evaluable at a specified point */
template< unsigned int TDimension , unsigned int SpaceDimension >
bool
SurfaceSpatialObject< TDimension, SpaceDimension > 
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name )
{
  itkDebugMacro( "Checking if the surface is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Return 1 if the point is on the surface */
template< unsigned int TDimension , unsigned int SpaceDimension >
void
SurfaceSpatialObject< TDimension, SpaceDimension > 
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name )
{
  itkDebugMacro( "Getting the value of the surface at " << point );
  if( IsInside(point, 0, name) )
    {
    value = 1;
    return;
    }
  else
    {
    if( Superclass::IsEvaluableAt(point, depth, name) )
      {
      Superclass::ValueAt(point, value, depth, name);
      return;
      }
    else
      {
      value = 0;
      itk::ExceptionObject e("SurfaceSpatialObject.txx");
      e.SetLocation("SurfaceSpatialObject::ValueAt( const PointType & )");
      e.SetDescription("this object cannot provide a value at the point");
      throw e;
      }
    }
}

} // end namespace itk 

#endif
