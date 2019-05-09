/*=========================================================================

  Program:   ImagingEngine
  Module:    blend_mode.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/

/// Render mode: what kind of mapper is used.
enum class RenderMode { SmartRayCasting, CpuRayCasting, GpuRayCasting };
/// Interpolation mode: how pixels are interpolated when rendering in low resolution.
enum class InterpolationMode { NearestNeighbor, Linear, Cubic };
