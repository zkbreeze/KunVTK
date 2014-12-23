//
//  KVSView.h
//
//
//  Created by Kun Zhao on 2014-12-23 21:56:46.
//
//
//  Still only can be used for the kvs::Camera's xform 2014-12-23 22:00:59

#ifndef KUN__KVSVIEW_H_INCLUDE
#define KUN__KVSVIEW_H_INCLUDE
 
#include <vtkMatrix4x4.h>
#include <vtkImageData.h>
#include <vtkCamera.h>

namespace kun
{

class KVSView
{
	vtkImageData* m_volume;
	float m_scale;

public:
	KVSView(){ m_scale = 1.0; }
	KVSView( float scale ){ this->SetScale( scale ); }

	void SetScale( float scale ){ m_scale = scale; }
	void SetVolume( vtkImageData* volume ){ m_volume = volume; }
	vtkCamera* ToVTKCamera();
	// vtkMatrix4x4* ToVTKRotation(); and so on
};

} // end of namespace kun 

 
#endif // KUN__KVSView_H_INCLUDE