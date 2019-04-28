// Copyright (c) 2018 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=6030f7b5ed22d7a6ed8dbdbc781d0d80a835ad68$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_REQUEST_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_REQUEST_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_request_handler_capi.h"
#include "include/cef_request_handler.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefRequestCallbackCToCpp
    : public CefCToCppRefCounted<CefRequestCallbackCToCpp,
                                 CefRequestCallback,
                                 cef_request_callback_t> {
 public:
  CefRequestCallbackCToCpp();

  // CefRequestCallback methods.
  void Continue(bool allow) OVERRIDE;
  void Cancel() OVERRIDE;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_REQUEST_CALLBACK_CTOCPP_H_
