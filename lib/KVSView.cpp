//
//  KVSView.cpp
//
//
//  Created by Kun Zhao on 2014-12-23 22:01:50.
//
//

#include "KVSView.h"
#include <kvs/Xform>
#include <vtkCamera.h>

namespace kun
{
	
vtkCamera* KVSView::ToVTKCamera()
{
	vtkCamera* camera = vtkCamera::New();
	int* dims = m_volume->GetDimensions();
	int max_dim = 0;
	if( dims[1] > dims[0] ) max_dim = 1;

	camera->SetPosition( 0.0, 0.0, dims[max_dim] * 2.0 * m_scale );
	camera->SetFocalPoint( 0.0, 0.0, 0.0 );
	camera->SetViewUp( 0, 1, 0 );
	camera->SetViewAngle( 45 );
	camera->SetClippingRange( 0.001, dims[max_dim] * 2.0 * m_scale + 1000);

	return camera;
}

} // end of namespace kun