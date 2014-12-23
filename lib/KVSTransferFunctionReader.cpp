//
//  KVSTransferFunctionReader.cpp
//
//
//  Created by Kun Zhao on 2014-12-17 16:10:28.
//
//

#include "KVSTransferFunctionReader.h"
#include <kvs/ColorMap>
#include <kvs/OpacityMap>

namespace kun
{

KVSTransferFunctionReader::KVSTransferFunctionReader()
{
	m_tfunc = kvs::TransferFunction( 256 );
}
	
KVSTransferFunctionReader::KVSTransferFunctionReader( std::string filename )
{
	this->Read( filename ); 
}

KVSTransferFunctionReader::KVSTransferFunctionReader( kvs::TransferFunction tfunc )
{
	this->SetKVSTransferFunction( tfunc ); 
}

void KVSTransferFunctionReader::SetRange( double* range )
{
	m_min = range[0]; 
	m_max = range[1];
	m_tfunc.setRange( m_min, m_max );
}

void KVSTransferFunctionReader::Read( std::string filename )
{
	m_tfunc = kvs::TransferFunction( filename );
}

void KVSTransferFunctionReader::SetKVSTransferFunction( kvs::TransferFunction tfunc )
{
	m_tfunc = tfunc;
}

vtkColorTransferFunction* KVSTransferFunctionReader::vtkColorMap()
{
	vtkColorTransferFunction* vtk_cmap = vtkColorTransferFunction::New();
	kvs::ColorMap kvs_cmap = m_tfunc.colorMap();

	for( size_t i = 0; i < 256; i++ )
	{
		float scale = ( m_max - m_min ) / 255.0;

		float location = m_min + i * scale;
		float r = (float)kvs_cmap.at(location).r() / 255.0;
		float g = (float)kvs_cmap.at(location).g() / 255.0;
		float b = (float)kvs_cmap.at(location).b() / 255.0;
		vtk_cmap->AddRGBPoint( location, r, g, b );
	}

	return vtk_cmap;
}

vtkPiecewiseFunction* KVSTransferFunctionReader::vtkOpacityMap()
{
	vtkPiecewiseFunction* vtk_omap = vtkPiecewiseFunction::New();
	kvs::OpacityMap kvs_omap = m_tfunc.opacityMap();

	for( size_t i = 0; i < 256; i++ )
	{
		float scale = ( m_max - m_min ) / 255.0;

		float location = m_min + i * scale;
		float opacity = (float)kvs_omap.at(location);
		vtk_omap->AddPoint( location, opacity );
	}

	return vtk_omap;
}

} // end of namespace kun