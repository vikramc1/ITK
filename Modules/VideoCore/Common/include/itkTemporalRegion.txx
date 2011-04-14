/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkTemporalRegion_txx
#define __itkTemporalRegion_txx

#include "itkTemporalRegion.h"

namespace itk
{

//
// Constructor
//
TemporalRegion
::TemporalRegion()
  : m_RealStart(),
    m_RealDuration(0,0),
    m_FrameStart(0),
    m_FrameDuration(0)
{
}

// ---------------------------------------------------------------------------
bool
TemporalRegion
::
operator==(const Self & region) const
{
  bool same = true;
  same &= m_RealStart == region.m_RealStart;
  same &= m_RealDuration == region.m_RealDuration;
  same &= m_FrameStart == region.m_FrameStart;
  same &= m_FrameDuration == region.m_FrameDuration;
  return same;
}



// ---------------------------------------------------------------------------
bool
TemporalRegion
::
operator!=(const Self & region) const
{
  return !(operator==(region));
}


} // end namespace itk

#endif
