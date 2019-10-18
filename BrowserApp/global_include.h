#pragma once


#include <stdio.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include <vtkJPEGReader.h>
#include <vtkTexture.h>

#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>

#include <vtkImageViewer2.h>
#include <vtkMetaImageReader.h>


#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>

#include <vtkDICOMImageReader.h>



#include <sstream>
#include <vtkTextMapper.h>
#include <vtkObjectFactory.h>
#include <vtkTextProperty.h>
#include <vtkActor2D.h>


#include <vtkImageCanvasSource2D.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkTextMapper.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>


#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>
#include <vtkImageActor.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkLeaderActor2D.h>
#include <vtkProperty2D.h>

#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkDataSetMapper.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkTextActor.h>
#include <vtkTextWidget.h>
#include <vtkTextRepresentation.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkBalloonWidget.h>
#include <vtkBalloonRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionActor2D.h>


#include <vtkBoxWidget.h>
#include <vtkLinearTransform.h>
#include <vtkProp3D.h>
#include <vtkTransform.h>

#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkMath.h>
#include <vtkCellArray.h>

#include <vtkImagePlaneWidget.h>

#include <vtkPolyDataWriter.h>
#include <vtkLine.h>

#include <vtkDoubleArray.h>
#include <vtkPointData.h>

#include <vtkBMPReader.h>
#include <vtkImageShrink3D.h>

#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>

#include <vtkVolume16Reader.h>
#include <vtkStripper.h>
#include <vtkOutlineFilter.h>
#include <vtkImageMapper3D.h>
#include <vtkCamera.h>

#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkPolyLine.h>

#include <vtkRendererCollection.h>

// TODO: 在此处引用程序需要的其他头文件
