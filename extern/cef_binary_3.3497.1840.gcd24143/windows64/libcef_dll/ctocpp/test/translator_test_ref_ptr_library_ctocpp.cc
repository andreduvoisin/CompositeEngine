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
// $hash=3180c3ee19a92ad7e6b22e2671cdbe6bdab88c77$
//

#include "libcef_dll/ctocpp/test/translator_test_ref_ptr_library_ctocpp.h"
#include "libcef_dll/ctocpp/test/translator_test_ref_ptr_library_child_child_ctocpp.h"
#include "libcef_dll/ctocpp/test/translator_test_ref_ptr_library_child_ctocpp.h"

// STATIC METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall")
CefRefPtr<CefTranslatorTestRefPtrLibrary>
CefTranslatorTestRefPtrLibrary::Create(int value) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_translator_test_ref_ptr_library_t* _retval =
      cef_translator_test_ref_ptr_library_create(value);

  // Return type: refptr_same
  return CefTranslatorTestRefPtrLibraryCToCpp::Wrap(_retval);
}

// VIRTUAL METHODS - Body may be edited by hand.

NO_SANITIZE("cfi-icall") int CefTranslatorTestRefPtrLibraryCToCpp::GetValue() {
  cef_translator_test_ref_ptr_library_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_value))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_value(_struct);

  // Return type: simple
  return _retval;
}

NO_SANITIZE("cfi-icall")
void CefTranslatorTestRefPtrLibraryCToCpp::SetValue(int value) {
  cef_translator_test_ref_ptr_library_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_value))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_value(_struct, value);
}

// CONSTRUCTOR - Do not edit by hand.

CefTranslatorTestRefPtrLibraryCToCpp::CefTranslatorTestRefPtrLibraryCToCpp() {}

template <>
cef_translator_test_ref_ptr_library_t*
CefCToCppRefCounted<CefTranslatorTestRefPtrLibraryCToCpp,
                    CefTranslatorTestRefPtrLibrary,
                    cef_translator_test_ref_ptr_library_t>::
    UnwrapDerived(CefWrapperType type, CefTranslatorTestRefPtrLibrary* c) {
  if (type == WT_TRANSLATOR_TEST_REF_PTR_LIBRARY_CHILD) {
    return reinterpret_cast<cef_translator_test_ref_ptr_library_t*>(
        CefTranslatorTestRefPtrLibraryChildCToCpp::Unwrap(
            reinterpret_cast<CefTranslatorTestRefPtrLibraryChild*>(c)));
  }
  if (type == WT_TRANSLATOR_TEST_REF_PTR_LIBRARY_CHILD_CHILD) {
    return reinterpret_cast<cef_translator_test_ref_ptr_library_t*>(
        CefTranslatorTestRefPtrLibraryChildChildCToCpp::Unwrap(
            reinterpret_cast<CefTranslatorTestRefPtrLibraryChildChild*>(c)));
  }
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template <>
base::AtomicRefCount CefCToCppRefCounted<
    CefTranslatorTestRefPtrLibraryCToCpp,
    CefTranslatorTestRefPtrLibrary,
    cef_translator_test_ref_ptr_library_t>::DebugObjCt ATOMIC_DECLARATION;
#endif

template <>
CefWrapperType
    CefCToCppRefCounted<CefTranslatorTestRefPtrLibraryCToCpp,
                        CefTranslatorTestRefPtrLibrary,
                        cef_translator_test_ref_ptr_library_t>::kWrapperType =
        WT_TRANSLATOR_TEST_REF_PTR_LIBRARY;