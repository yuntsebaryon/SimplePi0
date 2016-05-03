// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME SimplePi0Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "SimpleNuFinder.h"
#include "SimplePi0Finder.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *larlitecLcLSimpleNuFinder_Dictionary();
   static void larlitecLcLSimpleNuFinder_TClassManip(TClass*);
   static void *new_larlitecLcLSimpleNuFinder(void *p = 0);
   static void *newArray_larlitecLcLSimpleNuFinder(Long_t size, void *p);
   static void delete_larlitecLcLSimpleNuFinder(void *p);
   static void deleteArray_larlitecLcLSimpleNuFinder(void *p);
   static void destruct_larlitecLcLSimpleNuFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::larlite::SimpleNuFinder*)
   {
      ::larlite::SimpleNuFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::larlite::SimpleNuFinder));
      static ::ROOT::TGenericClassInfo 
         instance("larlite::SimpleNuFinder", "SimpleNuFinder.h", 29,
                  typeid(::larlite::SimpleNuFinder), DefineBehavior(ptr, ptr),
                  &larlitecLcLSimpleNuFinder_Dictionary, isa_proxy, 4,
                  sizeof(::larlite::SimpleNuFinder) );
      instance.SetNew(&new_larlitecLcLSimpleNuFinder);
      instance.SetNewArray(&newArray_larlitecLcLSimpleNuFinder);
      instance.SetDelete(&delete_larlitecLcLSimpleNuFinder);
      instance.SetDeleteArray(&deleteArray_larlitecLcLSimpleNuFinder);
      instance.SetDestructor(&destruct_larlitecLcLSimpleNuFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::larlite::SimpleNuFinder*)
   {
      return GenerateInitInstanceLocal((::larlite::SimpleNuFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::larlite::SimpleNuFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *larlitecLcLSimpleNuFinder_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::larlite::SimpleNuFinder*)0x0)->GetClass();
      larlitecLcLSimpleNuFinder_TClassManip(theClass);
   return theClass;
   }

   static void larlitecLcLSimpleNuFinder_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *larlitecLcLSimplePi0Finder_Dictionary();
   static void larlitecLcLSimplePi0Finder_TClassManip(TClass*);
   static void *new_larlitecLcLSimplePi0Finder(void *p = 0);
   static void *newArray_larlitecLcLSimplePi0Finder(Long_t size, void *p);
   static void delete_larlitecLcLSimplePi0Finder(void *p);
   static void deleteArray_larlitecLcLSimplePi0Finder(void *p);
   static void destruct_larlitecLcLSimplePi0Finder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::larlite::SimplePi0Finder*)
   {
      ::larlite::SimplePi0Finder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::larlite::SimplePi0Finder));
      static ::ROOT::TGenericClassInfo 
         instance("larlite::SimplePi0Finder", "SimplePi0Finder.h", 30,
                  typeid(::larlite::SimplePi0Finder), DefineBehavior(ptr, ptr),
                  &larlitecLcLSimplePi0Finder_Dictionary, isa_proxy, 4,
                  sizeof(::larlite::SimplePi0Finder) );
      instance.SetNew(&new_larlitecLcLSimplePi0Finder);
      instance.SetNewArray(&newArray_larlitecLcLSimplePi0Finder);
      instance.SetDelete(&delete_larlitecLcLSimplePi0Finder);
      instance.SetDeleteArray(&deleteArray_larlitecLcLSimplePi0Finder);
      instance.SetDestructor(&destruct_larlitecLcLSimplePi0Finder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::larlite::SimplePi0Finder*)
   {
      return GenerateInitInstanceLocal((::larlite::SimplePi0Finder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::larlite::SimplePi0Finder*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *larlitecLcLSimplePi0Finder_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::larlite::SimplePi0Finder*)0x0)->GetClass();
      larlitecLcLSimplePi0Finder_TClassManip(theClass);
   return theClass;
   }

   static void larlitecLcLSimplePi0Finder_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_larlitecLcLSimpleNuFinder(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::larlite::SimpleNuFinder : new ::larlite::SimpleNuFinder;
   }
   static void *newArray_larlitecLcLSimpleNuFinder(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::larlite::SimpleNuFinder[nElements] : new ::larlite::SimpleNuFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_larlitecLcLSimpleNuFinder(void *p) {
      delete ((::larlite::SimpleNuFinder*)p);
   }
   static void deleteArray_larlitecLcLSimpleNuFinder(void *p) {
      delete [] ((::larlite::SimpleNuFinder*)p);
   }
   static void destruct_larlitecLcLSimpleNuFinder(void *p) {
      typedef ::larlite::SimpleNuFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::larlite::SimpleNuFinder

namespace ROOT {
   // Wrappers around operator new
   static void *new_larlitecLcLSimplePi0Finder(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::larlite::SimplePi0Finder : new ::larlite::SimplePi0Finder;
   }
   static void *newArray_larlitecLcLSimplePi0Finder(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::larlite::SimplePi0Finder[nElements] : new ::larlite::SimplePi0Finder[nElements];
   }
   // Wrapper around operator delete
   static void delete_larlitecLcLSimplePi0Finder(void *p) {
      delete ((::larlite::SimplePi0Finder*)p);
   }
   static void deleteArray_larlitecLcLSimplePi0Finder(void *p) {
      delete [] ((::larlite::SimplePi0Finder*)p);
   }
   static void destruct_larlitecLcLSimplePi0Finder(void *p) {
      typedef ::larlite::SimplePi0Finder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::larlite::SimplePi0Finder

namespace {
  void TriggerDictionaryInitialization_libSimplePi0_Impl() {
    static const char* headers[] = {
"SimpleNuFinder.h",
"SimplePi0Finder.h",
0
    };
    static const char* includePaths[] = {
"/Users/yuntse/Work/uboone/LArLite/core",
"/Users/yuntse/Sources/root-6.04.08/include",
"/Users/yuntse/Work/uboone/LArLite/UserDev/SimplePi0/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace larlite{class __attribute__((annotate("$clingAutoload$SimpleNuFinder.h")))  SimpleNuFinder;}
namespace larlite{class __attribute__((annotate("$clingAutoload$SimplePi0Finder.h")))  SimplePi0Finder;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "SimpleNuFinder.h"
#include "SimplePi0Finder.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"larlite::SimpleNuFinder", payloadCode, "@",
"larlite::SimplePi0Finder", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSimplePi0",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSimplePi0_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSimplePi0_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSimplePi0() {
  TriggerDictionaryInitialization_libSimplePi0_Impl();
}
