/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLineSpatialObject.txx
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

#ifndef __itkLineSpatialObject_txx
#define __itkLineSpatialObject_txx

#include "itkLineSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
template< unsigned int TDimension , unsigned int SpaceDimension >
LineSpatialObject< TDimension, SpaceDimension > 
::LineSpatialObject()  
{ 
  m_Dimension = TDimension;
  strcpy(m_TypeName,"LineSpatialObject");
  m_Property->SetRed(1); 
  m_Property->SetGreen(0); 
  m_Property->SetBlue(0); 
  m_Property->SetAlpha(1); 
  ComputeBoundingBox();
} 
 
/** Destructor */
template< unsigned int TDimension , unsigned int SpaceDimension >
LineSpatialObject< TDimension, SpaceDimension >  
::~LineSpatialObject()
{ 
} 
 
/** Returns a reference to the list of the Line points.*/ 
template< unsigned int TDimension , unsigned int SpaceDimension >
typename LineSpatialObject< TDimension, SpaceDimension > ::PointListType &  
LineSpatialObject< TDimension, SpaceDimension > 
::GetPoints()
{ 
  itkDebugMacro( "Getting LinePoint list" );
  return m_Points;
} 

/** Set the list of Line points. */
template< unsigned int TDimension , unsigned int SpaceDimension >
void  
LineSpatialObject< TDimension, SpaceDimension >  
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
 
/** Print the object. */
template< unsigned int TDimension , unsigned int SpaceDimension >
void  
LineSpatialObject< TDimension, SpaceDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "LineSpatialObject(" << this << ")" << std::endl; 
  os << indent << "ID: " << m_Id << std::endl; 
  os << indent << "nb of points: "<< m_Points.size() << std::endl;
  Superclass::PrintSelf( os, indent ); 
} 
   
/** Compute the boundaries of the line.*/
template< unsigned int TDimension , unsigned int SpaceDimension >
bool 
LineSpatialObject< TDimension, SpaceDimension >  
::ComputeBoundingBox( unsigned int depth, char * name ) 
{ 
  itkDebugMacro( "Computing tube bounding box" );
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

/** Check if a given point is inside a line
 *  return True only if the point is in the point list */
template< unsigned int TDimension , unsigned int SpaceDimension >
bool 
LineSpatialObject< TDimension, SpaceDimension >  
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "] is on the Line" );

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

/** Returns true if the line is evaluable at the requested point, 
 *  false otherwise. */
template< unsigned int TDimension , unsigned int SpaceDimension >
bool
LineSpatialObject< TDimension, SpaceDimension > 
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name )
{
  itkDebugMacro( "Checking if the tube is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Returns the value of the line at that point.
 * Currently this function returns a binary value,
 * but it might want to return a degree of membership
 * in case of fuzzy Lines. */
template< unsigned int TDimension , unsigned int SpaceDimension >
void
LineSpatialObject< TDimension, SpaceDimension > 
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name )
{
  itkDebugMacro( "Getting the value of the tube at " << point );

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
      itk::ExceptionObject e("LineSpatialObject.txx");
      e.SetLocation("LineSpatialObject::ValueAt( const PointType & )");
      e.SetDescription("this object cannot provide a value at the point");
      throw e;
      }
    }
}

} // end namespace itk 

#endif
