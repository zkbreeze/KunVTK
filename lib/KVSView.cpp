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
	camera->SetPosition( dims[0] / 2.0, dims[1] / 2.0, dims[0] * 2.0 * m_scale );
	camera->SetFocalPoint( dims[0] / 2.0, dims[1] / 2.0, dims[2] / 2.0 );
	camera->SetViewUp( 0, 1, 0 );
	camera->SetViewAngle( 45 );

	return camera;
}

} // end of namespace kun