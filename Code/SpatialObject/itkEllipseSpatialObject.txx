/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkEllipseSpatialObject.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __EllipseSpatialObject_txx
#define __EllipseSpatialObject_txx

#include "itkEllipseSpatialObject.h" 

namespace itk 
{ 

/** Constructor */
template< unsigned int NDimensions , unsigned int SpaceDimension >
EllipseSpatialObject<NDimensions, SpaceDimension >
::EllipseSpatialObject()
{
  strcpy(m_TypeName,"EllipseSpatialObject");
  m_Radius.Fill(1.0);
  m_Dimension = NDimensions;
} 

/** Destructor */
template< unsigned int NDimensions , unsigned int SpaceDimension >
EllipseSpatialObject<NDimensions, SpaceDimension >
::~EllipseSpatialObject()  
{
  
}

/** Set all radii to the same radius value */
template< unsigned int NDimensions , unsigned int SpaceDimension >
void
EllipseSpatialObject<NDimensions, SpaceDimension >
::SetRadius(double radius)
{
  for(unsigned int i=0;i<NumberOfDimension;i++)
  {
    m_Radius[i]=radius;
  }
}

/** Test if the projection of the point is inside the projection
 *  of the ellipse */
template< unsigned int NDimensions , unsigned int SpaceDimension >
bool
EllipseSpatialObject<NDimensions, SpaceDimension >
::IsInsideProjection(double x, double y, unsigned int i) const
{
  double distance=sqrt(x*x+y*y);
  if(distance == 0)
  {
    return true;
  }

  double teta = acos(x/distance);
  double ellipseDistance = sqrt(m_Radius[i]*cos(teta)*m_Radius[i]*cos(teta)+m_Radius[i+1]*sin(teta)*m_Radius[i+1]*sin(teta));

  if(distance <= ellipseDistance)
  {
    return true;
  }
  else
  {
    return false;
  }
}


/** Test if the given point is inside the ellipse */
template< unsigned int NDimensions , unsigned int SpaceDimension >
bool 
EllipseSpatialObject< NDimensions, SpaceDimension > 
::IsInside( const PointType & point, unsigned int depth, char * name ) const 
{
  itkDebugMacro( "Checking the point [" << point << "is inside the tube" );
    
  if(name == NULL || strstr(typeid(Self).name(), name) )
    {
    PointType transformedPoint = point;
    TransformPointToLocalCoordinate(transformedPoint);
  
    double r = 0;
    for(unsigned int i=0;i<NDimensions-1;i++)
      {
      r += (transformedPoint[i]*transformedPoint[i])/(m_Radius[i]*m_Radius[i]);
      }
  
    if(r<1)
      {
      return true;
      }
    }

   return Superclass::IsInside(point, depth, name);
} 

/** Compute the bounds of the ellipse */
template< unsigned int NDimensions , unsigned int SpaceDimension >
bool
EllipseSpatialObject<NDimensions, SpaceDimension >
::ComputeBoundingBox( unsigned int depth, char * name ) 
{ 
  itkDebugMacro( "Computing tube bounding box" );

  if( this->GetMTime() > m_BoundsMTime )
    { 
    bool ret = Superclass::ComputeBoundingBox(depth, name);

    if(name == NULL || strstr(typeid(Self).name(), name) )
      {
      PointType pnt;
      PointType pnt2;
      for(unsigned int i=0; i<NDimensions;i++) 
        {   
        if(m_Radius[i]>0)
          {
          pnt[i]=m_Radius[i];
          pnt2[i]=-m_Radius[i];
          }
        else
          {
          pnt[i]=-m_Radius[i];
          pnt2[i]=m_Radius[i];
          }
        } 
    
      if(!ret)
        {
        m_Bounds->SetMinimum(pnt);
        m_Bounds->SetMaximum(pnt2);
        }
      else
        {
        m_Bounds->ConsiderPoint(pnt);
        m_Bounds->ConsiderPoint(pnt2);
        }
      }

    m_BoundsMTime = this->GetMTime();
    }

  return true;
} 


/** Returns if the ellipse os evaluable at one point */
template< unsigned int NDimensions , unsigned int SpaceDimension >
bool
EllipseSpatialObject<NDimensions, SpaceDimension >
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name )
{
  itkDebugMacro( "Checking if the ellipse is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Returns the value at one point */
template< unsigned int NDimensions , unsigned int SpaceDimension >
void
EllipseSpatialObject<NDimensions, SpaceDimension >
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name )
{
  itkDebugMacro( "Getting the value of the ellipse at " << point );
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
      itk::ExceptionObject e("BlobSpatialObject.txx");
      e.SetLocation("BlobSpatialObject::ValueAt( const PointType & )");
      e.SetDescription("this object cannot provide a value at the point");
      throw e;
      }
    }
}

/** Print Self function */
template< unsigned int NDimensions , unsigned int SpaceDimension >
void 
EllipseSpatialObject< NDimensions, SpaceDimension >
::PrintSelf( std::ostream& os, Indent indent ) const
{

  Superclass::PrintSelf(os, indent);
  os << "Radius: " << m_Radius << std::endl;

}

} // end namespace itk

#endif
