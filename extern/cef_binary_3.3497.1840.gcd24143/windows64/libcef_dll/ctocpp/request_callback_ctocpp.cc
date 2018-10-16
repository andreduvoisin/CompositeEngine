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
// $hash=1d9f467076c3f94bd7b2f7a335467bfeed341849$
//

#include "libcef_dll/ctocpp/request_callback_ctocpp.h"

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") void CefRequestCallbackCToCpp::Continue(bool allow) {
  cef_request_callback_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, cont))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cont(_struct, allow);
}

NO_SANITIZE("cfi-icall") void CefRequestCallbackCToCpp::Cancel() {
  cef_request_callback_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, cancel))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cancel(_struct);
}

// CONSTRUCTOR - Do not edit by hand.

CefRequestCallbackCToCpp::CefRequestCallbackCToCpp() {}

template <>
cef_request_callback_t* CefCToCppRefCounted<
    CefRequestCallbackCToCpp,
    CefRequestCallback,
    cef_request_callback_t>::UnwrapDerived(CefWrapperType type,
                                           CefRequestCallback* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template <>
base::AtomicRefCount CefCToCppRefCounted<CefRequestCallbackCToCpp,
                                         CefRequestCallback,
                                         cef_request_callback_t>::DebugObjCt
    ATOMIC_DECLARATION;
#endif

template <>
CefWrapperType CefCToCppRefCounted<CefRequestCallbackCToCpp,
                                   CefRequestCallback,
                                   cef_request_callback_t>::kWrapperType =
    WT_REQUEST_CALLBACK;
