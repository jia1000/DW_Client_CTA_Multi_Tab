/*=========================================================================

  Program:   ImagingEngine
  Module:    vtk_image_data_creator.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "tools/vtk_image_data_creator.h"
#include <vtkImageData.h>

template <class T> int getDataType();

template <> int getDataType<char>()
{
	return VTK_CHAR;
}

template <> int getDataType<signed char>()
{
	return VTK_SIGNED_CHAR;
}

template <> int getDataType<unsigned char>()
{
	return VTK_UNSIGNED_CHAR;
}

template <> int getDataType<short>()
{
	return VTK_SHORT;
}

template <> int getDataType<unsigned short>()
{
	return VTK_UNSIGNED_SHORT;
}

template <> int getDataType<int>()
{
	return VTK_INT;
}

template <> int getDataType<unsigned int>()
{
	return VTK_UNSIGNED_INT;
}

template <> int getDataType<long>()
{
	return VTK_LONG;
}

template <> int getDataType<unsigned long>()
{
	return VTK_UNSIGNED_LONG;
}

template <> int getDataType<float>()
{
	return VTK_FLOAT;
}

template <> int getDataType<double>()
{
	return VTK_DOUBLE;
}

VtkImageDataCreator::VtkImageDataCreator()
	: m_numberOfComponents(0)
{
	m_origin[0] = m_origin[1] = m_origin[2] = 0.0;
	m_spacing[0] = m_spacing[1] = m_spacing[2] = 1.0;
	m_dimensions[0] = m_dimensions[1] = m_dimensions[2] = 0.0;
}

VtkImageDataCreator& VtkImageDataCreator::SetOrigin(double origin[3])
{
	m_origin[0] = origin[0];
	m_origin[1] = origin[1];
	m_origin[2] = origin[2];
	return *this;
}

VtkImageDataCreator& VtkImageDataCreator::SetSpacing(double spacing[3])
{
	m_spacing[0] = spacing[0];
	m_spacing[1] = spacing[1];
	m_spacing[2] = spacing[2];
	return *this;
}

VtkImageDataCreator& VtkImageDataCreator::SetDimensions(int dimensions[3])
{
	m_dimensions[0] = dimensions[0];
	m_dimensions[1] = dimensions[1];
	m_dimensions[2] = dimensions[2];
	return *this;
}

VtkImageDataCreator& VtkImageDataCreator::SetNumberOfComponents(int numberOfComponents)
{
	m_numberOfComponents = numberOfComponents;
	return *this;
}

template <class T>
vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const T *data)
{
	vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->SetOrigin(m_origin);
	imageData->SetSpacing(m_spacing);
	imageData->SetDimensions(m_dimensions);
#if VTK_MAJOR_VERSION > 5
	imageData->AllocateScalars(getDataType<T>(), m_numberOfComponents);
#else
	int scalar_type = getDataType<T>();
	if (VTK_UNSIGNED_CHAR == scalar_type){
		imageData->SetScalarTypeToUnsignedChar();
	}
	else if (VTK_CHAR == scalar_type){
		imageData->SetScalarTypeToChar();
	}
	else if (VTK_UNSIGNED_SHORT == scalar_type){
		imageData->SetScalarTypeToUnsignedShort();
	}
	else if (VTK_SHORT == scalar_type){
		imageData->SetScalarTypeToShort();
	}
	else if (VTK_INT == scalar_type){
		imageData->SetScalarTypeToInt();
	}
	imageData->SetNumberOfScalarComponents(m_numberOfComponents);
	imageData->AllocateScalars();
#endif
	
	if (data)
	{
		memcpy(imageData->GetScalarPointer(), data, static_cast<size_t>(m_dimensions[0] * m_dimensions[1] * m_dimensions[2] * m_numberOfComponents) * sizeof(T));
	}

	return imageData;
}

// Instantiate create() for all numeric types.
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const signed char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const unsigned char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const short *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const unsigned short *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const int *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const unsigned int *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const long *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const unsigned long *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const float *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::Create(const double *data);

template <class T> vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const T *data)
{
	vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->SetOrigin(m_origin);
	imageData->SetSpacing(m_spacing);
	imageData->SetExtent(0, width - 1, 0, height - 1, 0, depth - 1);
#if VTK_MAJOR_VERSION > 5
	imageData->AllocateScalars(getDataType<T>(), 1);
#else
	int scalar_type = getDataType<T>();
	if (VTK_UNSIGNED_CHAR == scalar_type){
		imageData->SetScalarTypeToUnsignedChar();
	}
	else if (VTK_CHAR == scalar_type){
		imageData->SetScalarTypeToChar();
	}
	else if (VTK_UNSIGNED_SHORT == scalar_type){
		imageData->SetScalarTypeToUnsignedShort();
	}
	else if (VTK_SHORT == scalar_type){
		imageData->SetScalarTypeToShort();
	}
	else if (VTK_INT == scalar_type){
		imageData->SetScalarTypeToInt();
	}
	imageData->SetNumberOfScalarComponents(1);
	imageData->AllocateScalars();
#endif
	
	if (data)
	{
		memcpy(imageData->GetScalarPointer(), data, static_cast<size_t>(width * height * depth) * sizeof(T));
	}
	return imageData;
}

// We make the method instantiates by all these types. This avoids errors in undefined reference or unresolved external symbol.
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const signed char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const unsigned char *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const short *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const unsigned short *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const int *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const unsigned int *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const long *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const unsigned long *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const float *data);
template vtkSmartPointer<vtkImageData> VtkImageDataCreator::CreateVtkImageData(int width, int height, int depth, const double *data);
