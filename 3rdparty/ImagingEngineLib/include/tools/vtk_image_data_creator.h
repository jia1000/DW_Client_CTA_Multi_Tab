/*=========================================================================

  Program:   ImagingEngine
  Module:    vtk_image_data_creator.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <array>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

///
// The VtkImageDataCreator class allows to create a vtkImageData instance using the builder pattern.
///
class VtkImageDataCreator {

public:

    VtkImageDataCreator();

    /// Assigns the vtkImageData source to be created.
    VtkImageDataCreator& SetOrigin(double origin[3]);
    /// Assigns the spacing of the vtkImageData to be created.
    VtkImageDataCreator& SetSpacing(double spacing[3]);
    /// Sets the dimensions of the vtkImageData to be created.
    VtkImageDataCreator& SetDimensions(int dimensions[3]);
    /// Sets the number of components of the vtkImageData to be created.
    VtkImageDataCreator& SetNumberOfComponents(int numberOfComponents);

    /// Creates and returns a vtkImageData instance with the current configuration and the given data.
    template <class T>
    vtkSmartPointer<vtkImageData> Create(const T *data);
    /// Creates and returns a vtkImageData instance with the current origin and spacing, the given dimensions, 1 component, and the given data.
    /// Kept for backward compatibility, you should use create() instead.
	template <class T> vtkSmartPointer<vtkImageData> CreateVtkImageData(int width, int height, int depth, const T *data);

private:

    double m_origin[3];
    double m_spacing[3];
    int m_dimensions[3];
    int m_numberOfComponents;

};
