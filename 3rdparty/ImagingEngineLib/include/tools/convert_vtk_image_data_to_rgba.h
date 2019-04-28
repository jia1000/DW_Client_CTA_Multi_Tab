/*=========================================================================

  Program:   ImagingEngine
  Module:    convert_vtk_image_data_to_rgba.h
  author: 	 zhangjian
  Brief:	 提供将vtkimagedata转为RGBA格式的方法

=========================================================================*/
///  @file ConvertVtkImagedataToRGBA.h
///  @brief 将vtkimagedata转为RGBA格式
///  @author lizhiguo
///  @date 2018
///
///
#ifndef CONVERTVTKIMAGEDATATORGBA_H
#define CONVERTVTKIMAGEDATATORGBA_H

//#include <QImage>

class vtkImageData;
class vtkScalarsToColors;

class ConvertVtkImagedataToRGBA
{
public:
    ConvertVtkImagedataToRGBA();

    /// \brief 将vtkImageData数据转换为RGBA格式
    /// \param pSrcData 源数据
    /// \param pResult  结果数据
    /// \param slice    转换的层，－1表示全部转换
    /// \param win      窗宽
    /// \param level    窗位
    /// \return true:成功 false:失败
    /// @note pResult由函数内部创建，调用者释放。
    /// 调用者只需要定义vtkImageData *pResult = nullptr;即可。
    bool ConvertImageScalarsToRGBA(vtkImageData *pSrcData,
                                   vtkImageData **pResult,
                                   int slice=-1,
                                   float win = 255.0,
                                   float level = 127.5);

    ///
    /// \brief 将vtkImageData数据附带颜色表转换为RGBA格式
    /// \param pSrcData     源数据
    /// \param pColorTable  颜色表
    /// \param slice        转换的切片，－1表示转换所有的
    /// \param pResult      结果数据
    /// \return true:成功 false:失败
    /// @note pResult由函数内部创建，调用者释放。
    /// 调用者只需要定义vtkImageData *pResult = nullptr;即可。
    bool ConvertImageWithColorTableScalarsToRGBA(vtkImageData *pSrcData,
                                                 vtkScalarsToColors *pColorTable,
                                                 int slice,
                                                 vtkImageData **pResult);

    /////
    ///// \brief 获取vtkimagedata的缩略图
    ///// \param pSrcData     源数据
    ///// \param win          窗宽
    ///// \param level        窗位
    ///// \param imageOut     结果
    ///// \param slice        获取的缩略图所在的切片，-1表示取中间层
    ///// \return true:成功 false:失败
    /////
    //bool GetThumbnail(vtkImageData *pSrcData, float win, float level, QImage &imageOut, int slice=-1);


    ///
    /// \brief 合并两个vtkImageData,可以带颜色表
    /// \param pSrcData         源数据
    /// \param pSrcColorTable   源数据颜色表，可以为空
    /// \param pMaskData        模板数据
    /// \param pMaskColorTable  模板数据颜色表，可以为空
    /// \param slice            合并的切片，－1表示合并所有的
    /// \param pResult          合并后的结果
    /// \return true:成功 false:失败
    ///
    bool CombineVtkImageData(vtkImageData *pSrcData,
                             vtkScalarsToColors *pSrcColorTable,
                             vtkImageData *pMaskData,
                             vtkScalarsToColors *pMaskColorTable,
                             int slice,
                             vtkImageData *pResult);


};

#endif // CONVERTVTKIMAGEDATATORGBA_H
