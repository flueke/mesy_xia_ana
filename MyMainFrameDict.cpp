// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MyMainFrameDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
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

// Header files passed as explicit arguments
#include "MyMainFrame.h"
#include "CanvasFrame.h"
#include "PictureFrame.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *PictureFrame_Dictionary();
   static void PictureFrame_TClassManip(TClass*);
   static void delete_PictureFrame(void *p);
   static void deleteArray_PictureFrame(void *p);
   static void destruct_PictureFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PictureFrame*)
   {
      ::PictureFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PictureFrame));
      static ::ROOT::TGenericClassInfo 
         instance("PictureFrame", "PictureFrame.h", 31,
                  typeid(::PictureFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PictureFrame_Dictionary, isa_proxy, 0,
                  sizeof(::PictureFrame) );
      instance.SetDelete(&delete_PictureFrame);
      instance.SetDeleteArray(&deleteArray_PictureFrame);
      instance.SetDestructor(&destruct_PictureFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PictureFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::PictureFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::PictureFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PictureFrame_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::PictureFrame*>(nullptr))->GetClass();
      PictureFrame_TClassManip(theClass);
   return theClass;
   }

   static void PictureFrame_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CanvasFrame_Dictionary();
   static void CanvasFrame_TClassManip(TClass*);
   static void delete_CanvasFrame(void *p);
   static void deleteArray_CanvasFrame(void *p);
   static void destruct_CanvasFrame(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CanvasFrame*)
   {
      ::CanvasFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CanvasFrame));
      static ::ROOT::TGenericClassInfo 
         instance("CanvasFrame", "CanvasFrame.h", 35,
                  typeid(::CanvasFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CanvasFrame_Dictionary, isa_proxy, 0,
                  sizeof(::CanvasFrame) );
      instance.SetDelete(&delete_CanvasFrame);
      instance.SetDeleteArray(&deleteArray_CanvasFrame);
      instance.SetDestructor(&destruct_CanvasFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CanvasFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::CanvasFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::CanvasFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CanvasFrame_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::CanvasFrame*>(nullptr))->GetClass();
      CanvasFrame_TClassManip(theClass);
   return theClass;
   }

   static void CanvasFrame_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_MyMainFrame(void *p);
   static void deleteArray_MyMainFrame(void *p);
   static void destruct_MyMainFrame(void *p);
   static void streamer_MyMainFrame(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MyMainFrame*)
   {
      ::MyMainFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MyMainFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MyMainFrame", ::MyMainFrame::Class_Version(), "MyMainFrame.h", 25,
                  typeid(::MyMainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MyMainFrame::Dictionary, isa_proxy, 16,
                  sizeof(::MyMainFrame) );
      instance.SetDelete(&delete_MyMainFrame);
      instance.SetDeleteArray(&deleteArray_MyMainFrame);
      instance.SetDestructor(&destruct_MyMainFrame);
      instance.SetStreamerFunc(&streamer_MyMainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MyMainFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::MyMainFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MyMainFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MyMainFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MyMainFrame::Class_Name()
{
   return "MyMainFrame";
}

//______________________________________________________________________________
const char *MyMainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MyMainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MyMainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MyMainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PictureFrame(void *p) {
      delete (static_cast<::PictureFrame*>(p));
   }
   static void deleteArray_PictureFrame(void *p) {
      delete [] (static_cast<::PictureFrame*>(p));
   }
   static void destruct_PictureFrame(void *p) {
      typedef ::PictureFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::PictureFrame

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CanvasFrame(void *p) {
      delete (static_cast<::CanvasFrame*>(p));
   }
   static void deleteArray_CanvasFrame(void *p) {
      delete [] (static_cast<::CanvasFrame*>(p));
   }
   static void destruct_CanvasFrame(void *p) {
      typedef ::CanvasFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::CanvasFrame

//______________________________________________________________________________
void MyMainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MyMainFrame.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TGMainFrame::Streamer(R__b);
      R__b >> fCanvasFrame;
      R__b >> fFilename1;
      R__b >> fFilename2;
      R__b >> fFilename3;
      R__b >> fExportButton;
      R__b >> fDrawButton;
      R__b >> fClearButton;
      R__b >> fExitButton;
      file.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, MyMainFrame::IsA());
   } else {
      R__c = R__b.WriteVersion(MyMainFrame::IsA(), kTRUE);
      TGMainFrame::Streamer(R__b);
      R__b << fCanvasFrame;
      R__b << fFilename1;
      R__b << fFilename2;
      R__b << fFilename3;
      R__b << fExportButton;
      R__b << fDrawButton;
      R__b << fClearButton;
      R__b << fExitButton;
      file.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MyMainFrame(void *p) {
      delete (static_cast<::MyMainFrame*>(p));
   }
   static void deleteArray_MyMainFrame(void *p) {
      delete [] (static_cast<::MyMainFrame*>(p));
   }
   static void destruct_MyMainFrame(void *p) {
      typedef ::MyMainFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MyMainFrame(TBuffer &buf, void *obj) {
      ((::MyMainFrame*)obj)->::MyMainFrame::Streamer(buf);
   }
} // end of namespace ROOT for class ::MyMainFrame

namespace {
  void TriggerDictionaryInitialization_MyMainFrameDict_Impl() {
    static const char* headers[] = {
"MyMainFrame.h",
"CanvasFrame.h",
"PictureFrame.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/florian/local/root-6.28.04/include/",
"/home/florian/src/xiakang.xia-offline-ana/OfflineAna/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "MyMainFrameDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$PictureFrame.h")))  __attribute__((annotate("$clingAutoload$MyMainFrame.h")))  PictureFrame;
class __attribute__((annotate("$clingAutoload$CanvasFrame.h")))  __attribute__((annotate("$clingAutoload$MyMainFrame.h")))  CanvasFrame;
class __attribute__((annotate("$clingAutoload$MyMainFrame.h")))  MyMainFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "MyMainFrameDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "MyMainFrame.h"
#include "CanvasFrame.h"
#include "PictureFrame.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CanvasFrame", payloadCode, "@",
"MyMainFrame", payloadCode, "@",
"PictureFrame", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MyMainFrameDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MyMainFrameDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MyMainFrameDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MyMainFrameDict() {
  TriggerDictionaryInitialization_MyMainFrameDict_Impl();
}
