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
// $hash=fbc0913a02d637665ac62358d99d1d505c86333b$
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_RESOLVE_CALLBACK_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_RESOLVE_CALLBACK_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_request_context_capi.h"
#include "include/capi/cef_scheme_capi.h"
#include "include/cef_request_context.h"
#include "include/cef_scheme.h"
#include "libcef_dll/cpptoc/cpptoc_ref_counted.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefResolveCallbackCppToC
    : public CefCppToCRefCounted<CefResolveCallbackCppToC,
                                 CefResolveCallback,
                                 cef_resolve_callback_t> {
 public:
  CefResolveCallbackCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_RESOLVE_CALLBACK_CPPTOC_H_
