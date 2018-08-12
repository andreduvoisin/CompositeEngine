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
// $hash=1c9915e5ad68a382d2076f30b6e516ac78772d8f$
//

#include "libcef_dll/ctocpp/test/translator_test_scoped_library_ctocpp.h"
#include "libcef_dll/ctocpp/test/translator_test_scoped_library_child_child_ctocpp.h"
#include "libcef_dll/ctocpp/test/translator_test_scoped_library_child_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

CefOwnPtr<CefTranslatorTestScopedLibrary>
CefTranslatorTestScopedLibrary::Create(int value) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_translator_test_scoped_library_t* _retval =
      cef_translator_test_scoped_library_create(value);

  // Return type: ownptr_same
  return CefTranslatorTestScopedLibraryCToCpp::Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

int CefTranslatorTestScopedLibraryCToCpp::GetValue() {
  cef_translator_test_scoped_library_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_value))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_value(_struct);

  // Return type: simple
  return _retval;
}

void CefTranslatorTestScopedLibraryCToCpp::SetValue(int value) {
  cef_translator_test_scoped_library_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_value))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_value(_struct, value);
}

// CONSTRUCTOR - Do not edit by hand.

CefTranslatorTestScopedLibraryCToCpp::CefTranslatorTestScopedLibraryCToCpp() {}

template <>
cef_translator_test_scoped_library_t*
CefCToCppScoped<CefTranslatorTestScopedLibraryCToCpp,
                CefTranslatorTestScopedLibrary,
                cef_translator_test_scoped_library_t>::
    UnwrapDerivedOwn(CefWrapperType type,
                     CefOwnPtr<CefTranslatorTestScopedLibrary> c) {
  if (type == WT_TRANSLATOR_TEST_SCOPED_LIBRARY_CHILD) {
    return reinterpret_cast<cef_translator_test_scoped_library_t*>(
        CefTranslatorTestScopedLibraryChildCToCpp::UnwrapOwn(
            CefOwnPtr<CefTranslatorTestScopedLibraryChild>(
                reinterpret_cast<CefTranslatorTestScopedLibraryChild*>(
                    c.release()))));
  }
  if (type == WT_TRANSLATOR_TEST_SCOPED_LIBRARY_CHILD_CHILD) {
    return reinterpret_cast<cef_translator_test_scoped_library_t*>(
        CefTranslatorTestScopedLibraryChildChildCToCpp::UnwrapOwn(
            CefOwnPtr<CefTranslatorTestScopedLibraryChildChild>(
                reinterpret_cast<CefTranslatorTestScopedLibraryChildChild*>(
                    c.release()))));
  }
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

template <>
cef_translator_test_scoped_library_t*
CefCToCppScoped<CefTranslatorTestScopedLibraryCToCpp,
                CefTranslatorTestScopedLibrary,
                cef_translator_test_scoped_library_t>::
    UnwrapDerivedRaw(CefWrapperType type,
                     CefRawPtr<CefTranslatorTestScopedLibrary> c) {
  if (type == WT_TRANSLATOR_TEST_SCOPED_LIBRARY_CHILD) {
    return reinterpret_cast<cef_translator_test_scoped_library_t*>(
        CefTranslatorTestScopedLibraryChildCToCpp::UnwrapRaw(
            CefRawPtr<CefTranslatorTestScopedLibraryChild>(
                reinterpret_cast<CefTranslatorTestScopedLibraryChild*>(
                    CEF_RAW_PTR_GET(c)))));
  }
  if (type == WT_TRANSLATOR_TEST_SCOPED_LIBRARY_CHILD_CHILD) {
    return reinterpret_cast<cef_translator_test_scoped_library_t*>(
        CefTranslatorTestScopedLibraryChildChildCToCpp::UnwrapRaw(
            CefRawPtr<CefTranslatorTestScopedLibraryChildChild>(
                reinterpret_cast<CefTranslatorTestScopedLibraryChildChild*>(
                    CEF_RAW_PTR_GET(c)))));
  }
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template <>
base::AtomicRefCount CefCToCppScoped<
    CefTranslatorTestScopedLibraryCToCpp,
    CefTranslatorTestScopedLibrary,
    cef_translator_test_scoped_library_t>::DebugObjCt ATOMIC_DECLARATION;
#endif

template <>
CefWrapperType
    CefCToCppScoped<CefTranslatorTestScopedLibraryCToCpp,
                    CefTranslatorTestScopedLibrary,
                    cef_translator_test_scoped_library_t>::kWrapperType =
        WT_TRANSLATOR_TEST_SCOPED_LIBRARY;
