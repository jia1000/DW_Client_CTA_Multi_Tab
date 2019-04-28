#include "tools/convert_vtk_image_data_to_rgba.h"

#include "vtkType.h"
#include "vtkImageData.h"
#include "vtkTemplateAliasMacro.h"
#include "vtkScalarsToColors.h"
#include "vtkImageBlend.h"


///对数据的区间进行限制
template<class F>
inline F TP_LimitNum(F x, F xmin, F xmax)
{
  x = (x > xmin ? x : xmin);
  x = (x < xmax ? x : xmax);
  return x;
}

///将二维数据转成RGBA格式，输出类型为unsigned char, component为4
/// @param inPtr 源数据指针
/// @param outPtr 输出数据指针
/// @param ncols 列数（宽度）
/// @param nrows 行数（高度）
/// @param numComp 源数据component数
/// @param shift 源数据基准值
/// @param scale 源数据缩放系数
/// @note 参考vtkImageMapper3D实现
///
template<class F, class T>
void TP_VtkImageShiftScale(
        const T *inPtr,
        unsigned char *outPtr,
        int ncols,
        int nrows,
        int numComp,
        F shift,
        F scale)
{
  const F vmin = static_cast<F>(0);
  const F vmax = static_cast<F>(255);
  unsigned char alpha = 255;

  int inIncX = numComp;

  // loop through the data and copy it for the texture
  if (numComp == 1)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        // Pixel operation
        F val = (inPtr[0] + shift)*scale;
        val = TP_LimitNum(val, vmin, vmax);
        unsigned char cval = static_cast<unsigned char>(val + 0.5);
        outPtr[0] = cval;
        outPtr[1] = cval;
        outPtr[2] = cval;
        outPtr[3] = alpha;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else if (numComp == 2)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        // Pixel operation
        F val = (inPtr[0] + shift)*scale;
        val = TP_LimitNum(val, vmin, vmax);
        unsigned char cval = static_cast<unsigned char>(val + 0.5);
        val = (inPtr[1] + shift)*scale;
        val = TP_LimitNum(val, vmin, vmax);
        unsigned char aval = static_cast<unsigned char>(val + 0.5);
        outPtr[0] = cval;
        outPtr[1] = cval;
        outPtr[2] = cval;
        outPtr[3] = aval;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else if (numComp == 3)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        // Pixel operation
        F r = (inPtr[0] + shift)*scale;
        F g = (inPtr[1] + shift)*scale;
        F b = (inPtr[2] + shift)*scale;
        r = TP_LimitNum(r, vmin, vmax);
        g = TP_LimitNum(g, vmin, vmax);
        b = TP_LimitNum(b, vmin, vmax);
        outPtr[0] = static_cast<unsigned char>(r + 0.5);
        outPtr[1] = static_cast<unsigned char>(g + 0.5);
        outPtr[2] = static_cast<unsigned char>(b + 0.5);
        outPtr[3] = alpha;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else // if (numComp == 4)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        // Pixel operation
        F r = (inPtr[0] + shift)*scale;
        F g = (inPtr[1] + shift)*scale;
        F b = (inPtr[2] + shift)*scale;
        F a = (inPtr[3] + shift)*scale;
        r = TP_LimitNum(r, vmin, vmax);
        g = TP_LimitNum(g, vmin, vmax);
        b = TP_LimitNum(b, vmin, vmax);
        a = TP_LimitNum(a, vmin, vmax);
        outPtr[0] = static_cast<unsigned char>(r + 0.5);
        outPtr[1] = static_cast<unsigned char>(g + 0.5);
        outPtr[2] = static_cast<unsigned char>(b + 0.5);
        outPtr[3] = static_cast<unsigned char>(a + 0.5);
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
}


///
/// \brief 将VTK_UNSIGNED_CHAR类型数据转为RGBA格式
/// \param inPtr   源数据指针
/// \param outPtr  结果数据指针
/// \param ncols   源数据列数（宽度）
/// \param nrows   源数据行数（高度）
/// \param numComp 元素的数据个数
/// @note 参考vtkImageMapper3D中的vtkImageMapperConvertToRGBA
static void VtkUCImageConvertToRGBA(
        const unsigned char *inPtr,
        unsigned char *outPtr,
        int ncols,
        int nrows,
        int numComp)
{
  unsigned char alpha = 255;
  int inIncX = numComp;

  if (numComp == 1)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        unsigned char val = inPtr[0];
        outPtr[0] = val;
        outPtr[1] = val;
        outPtr[2] = val;
        outPtr[3] = alpha;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else if (numComp == 2)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        unsigned char val = inPtr[0];
        unsigned char a = inPtr[1];
        outPtr[0] = val;
        outPtr[1] = val;
        outPtr[2] = val;
        outPtr[3] = a;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else if (numComp == 3)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        outPtr[0] = inPtr[0];
        outPtr[1] = inPtr[1];
        outPtr[2] = inPtr[2];
        outPtr[3] = alpha;
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
  else // if (numComp == 4)
  {
    for (int idy = 0; idy < nrows; idy++)
    {
      for (int idx = 0; idx < ncols; idx++)
      {
        outPtr[0] = inPtr[0];
        outPtr[1] = inPtr[1];
        outPtr[2] = inPtr[2];
        outPtr[3] = inPtr[3];
        outPtr += 4;
        inPtr += inIncX;
      }
      //outPtr += outIncY;
      //inPtr += inIncY;
    }
  }
}



bool ConvertVtkImagedataToRGBA::ConvertImageScalarsToRGBA(vtkImageData *pSrcData, vtkImageData **pResult, int slice, float win, float level)
{
    bool bre = false;
    do
    {
        if(nullptr == pSrcData)
        {
            break;
        }

        if(nullptr != *pResult)
        {
            (*pResult)->Delete();
        }

        int dim[3];
        pSrcData->GetDimensions(dim);
        if(((slice < 0) || (slice >= dim[2])) && (-1 != slice))
        {
            break;
        }

        //获取默认窗宽窗位
        if((0 == win) && (0 == level))
        {
            double scalarrange[2];
            pSrcData->GetScalarRange(scalarrange);
            win = (scalarrange[1] - scalarrange[0])/2;
            level = (scalarrange[0]+scalarrange[1])/4;
        }
        win = (win >= 2) ? win : 2;

        float range[2];
        range[0] = level - 0.5*win;
        range[1] = level + 0.5*win;

        float shift = -range[0];
        float scale = 255.0;

        if (range[0] < range[1])
        {
            scale /= (range[1] - range[0]);
        }
        else
        {
            scale = 1e+32;
        }

        int extent[6];
        pSrcData->GetExtent(extent);

        if(-1 != slice)
        {
            dim[2] = 1;
            extent[4] = 0;
            extent[5] = 1;
        }

        *pResult = vtkImageData::New();
        (*pResult)->SetDimensions(dim);
        (*pResult)->SetSpacing(pSrcData->GetSpacing());
        (*pResult)->SetOrigin(pSrcData->GetOrigin());
        (*pResult)->SetExtent(extent);
#if VTK_MAJOR_VERSION > 5
        (*pResult)->AllocateScalars(VTK_UNSIGNED_CHAR, 4);
		// by zhangjian not know how to do here...
		//(*pResult)->SetScalarType(VTK_UNSIGNED_CHAR);
		//(*pResult)->SetNumberOfScalarComponents(4);
#else
		(*pResult)->AllocateScalars();		//VTK_UNSIGNED_CHAR, 4
		(*pResult)->SetScalarType(VTK_UNSIGNED_CHAR);
		(*pResult)->SetNumberOfScalarComponents(4);
#endif

        int scalarType = pSrcData->GetScalarType();
        int numComp = pSrcData->GetNumberOfScalarComponents();
        int ncols = dim[0];
        int nrows = dim[1] * dim[2];
        unsigned char* outPtr = (unsigned char*)(*pResult)->GetScalarPointer();
        void* inPtr = (void*)pSrcData->GetScalarPointer();

        if(-1 != slice)
        {
            inPtr = (void*)pSrcData->GetScalarPointer(0, 0, slice);
        }



        if ((VTK_UNSIGNED_CHAR == scalarType) &&
            (static_cast<int>(shift*scale) == 0) &&
            (static_cast<int>((255 + shift)*scale) == 255))
        {

          VtkUCImageConvertToRGBA(static_cast<unsigned char *>(inPtr),
                                      outPtr, ncols, nrows, numComp);
          bre = true;
          break;
        }

      switch (scalarType)
      {
        vtkTemplateAliasMacro(
          TP_VtkImageShiftScale(static_cast<VTK_TT*>(inPtr), outPtr, ncols, nrows, numComp, shift, scale));
        default:
          vtkGenericWarningMacro("ConvertImageScalarsToRGBA: Unknown input ScalarType");
      }
      bre = true;

    }while(false);

    return bre;
}

bool ConvertVtkImagedataToRGBA::ConvertImageWithColorTableScalarsToRGBA(vtkImageData *pSrcData,
                                                                    vtkScalarsToColors *pColorTable,
                                                                    int slice,
                                                                    vtkImageData **pResult)
{
    bool bre = false;

    do
    {
        if(nullptr == pSrcData)
        {
            break;
        }

        if(nullptr == pColorTable)
        {
            break;
        }

        if(nullptr != *pResult)
        {
            (*pResult)->Delete();
        }

        int dim[3];
        pSrcData->GetDimensions(dim);
        if(((slice < 0) || (slice >= dim[2])) && (-1 != slice))
        {
            break;
        }


        int extent[6];
        pSrcData->GetExtent(extent);

        if(-1 != slice)
        {
            dim[2] = 1;
            extent[4] = 0;
            extent[5] = 1;
        }

        *pResult = vtkImageData::New();
        (*pResult)->SetDimensions(dim);
        (*pResult)->SetSpacing(pSrcData->GetSpacing());
        (*pResult)->SetOrigin(pSrcData->GetOrigin());
        (*pResult)->SetExtent(extent);
#if VTK_MAJOR_VERSION > 5
        (*pResult)->AllocateScalars(VTK_UNSIGNED_CHAR, 4);
		// by zhangjian not know how to do here...
		//(*pResult)->SetScalarType(VTK_UNSIGNED_CHAR);
		//(*pResult)->SetNumberOfScalarComponents(4);
#else
        (*pResult)->AllocateScalars();		//VTK_UNSIGNED_CHAR, 4
		(*pResult)->SetScalarType(VTK_UNSIGNED_CHAR);
		(*pResult)->SetNumberOfScalarComponents(4);

#endif

        int scalarType = pSrcData->GetScalarType();
        int numComp = pSrcData->GetNumberOfScalarComponents();
        int ncols = dim[0] * dim[1] * dim[2];
        unsigned char* outPtr = (unsigned char*)(*pResult)->GetScalarPointer();
        void* inPtr = (void*)pSrcData->GetScalarPointer();

        if(-1 != slice)
        {
            inPtr = (void*)pSrcData->GetScalarPointer(0, 0, slice);
        }

        // pass the data through the lookup table
        if (numComp == 1)
        {
            pColorTable->MapScalarsThroughTable(inPtr, outPtr, scalarType, ncols, numComp, VTK_RGBA);
        }
        else
        {
            pColorTable->MapVectorsThroughTable(inPtr, outPtr, scalarType, ncols, numComp, VTK_RGBA);
        }

        bre = true;
    }while(false);

    return bre;
}


//bool ConvertVtkImagedataToRGBA::GetThumbnail(vtkImageData *pSrcData, float win, float level, QImage &imageOut, int slice)
//{
//    bool bre = false;
//    do
//    {
//        if(nullptr == pSrcData)
//        {
//            break;
//        }
//
//        int *dim = pSrcData->GetDimensions();
//        if(slice < 0 || slice >= dim[2])
//        {
//            slice = (dim[2] - 1) / 2;
//        }
//
//        vtkImageData *pTmpImageData = nullptr;
//
//        if(false == ConvertImageScalarsToRGBA(pSrcData, &pTmpImageData, slice, win, level))
//        {
//            break;
//        }
//
//        QImage img(dim[0], dim[1], QImage::Format_ARGB32);
//        unsigned int *pBuf = (unsigned int*)img.bits();
//
//        unsigned char *pData = (unsigned char*)pTmpImageData->GetScalarPointer();
//        int total = dim[0] * dim[1];
//        for(int i = 0; i < total; i++)
//        {
//            int rr = pData[i*4];
//            int gg = pData[i*4 +1];
//            int bb = pData[i*4 +2];
//            int aa = pData[i*4 +3];
//            pBuf[i] = ((aa << 24) & 0xFF000000) + ((rr << 16) & 0xFF0000) + ((gg << 8) & 0xFF00) + bb;
//        }
//
//        pTmpImageData->Delete();
//        imageOut = img;
//        bre = true;
//
//    }while(false);
//
//    return bre;
//}

bool ConvertVtkImagedataToRGBA::CombineVtkImageData(vtkImageData *pSrcData,
                                                vtkScalarsToColors *pSrcColorTable,
                                                vtkImageData *pMaskData,
                                                vtkScalarsToColors *pMaskColorTable,
                                                int slice,
                                                vtkImageData *pResult)
{
    bool bre = false;
    vtkImageData *pSrcRGBA = nullptr;
    vtkImageData *pMskRGBA = nullptr;
    do
    {
        if(nullptr == pSrcData)
        {
            break;
        }

        if(nullptr == pMaskData)
        {
            break;
        }

        bool b = false;

        //转换源数据
        if(nullptr == pSrcColorTable)
        {
            b = ConvertImageScalarsToRGBA(pSrcData, &pSrcRGBA, slice);
        }
        else
        {
            b = ConvertImageWithColorTableScalarsToRGBA(pSrcData, pSrcColorTable, slice, &pSrcRGBA);
        }

        if(false == b)
        {
            break;
        }

        //转换模板数据
        if(nullptr == pMaskColorTable)
        {
            b = ConvertImageScalarsToRGBA(pMaskData, &pMskRGBA, slice);
        }
        else
        {
            b = ConvertImageWithColorTableScalarsToRGBA(pMaskData, pMaskColorTable, slice, &pMskRGBA);
        }

        if(false == b)
        {
            break;
        }

        vtkImageBlend *pMaskImageBlend = vtkImageBlend::New();
        pMaskImageBlend->SetOpacity(1, 1);
        pMaskImageBlend->SetBlendModeToNormal();
#if VTK_MAJOR_VERSION > 5
		pMaskImageBlend->SetInputData(pSrcRGBA);
        pMaskImageBlend->AddInputData(pMskRGBA);
#else
		pMaskImageBlend->SetInput(pSrcRGBA);
        pMaskImageBlend->AddInput(pMskRGBA);
#endif        
        pMaskImageBlend->Update();

        pResult->DeepCopy(pMaskImageBlend->GetOutput());
        pMaskImageBlend->Delete();

        bre = true;

    }while(false);

    if(nullptr != pSrcRGBA)
    {
        pSrcRGBA->Delete();
    }

    if(nullptr != pMskRGBA)
    {
        pMskRGBA->Delete();
    }
    return bre;
}



ConvertVtkImagedataToRGBA::ConvertVtkImagedataToRGBA()
{

}