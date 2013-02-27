%module(directors="1") wiimote
%{
#define SWIGWRAPPER
#include "../wiimote.h"
%}

// Enable automatic documentation ("1" means with type information)
%feature("autodoc", "1");


// Define a few win32 types for the benefit of SWIG's type system
#define BYTE unsigned char
#define WORD short
#define DWORD long
#define BOOLEAN bool

#ifdef _UNICODE
#define TCHAR wchar_t
#else
#define TCHAR char
#endif


// Bounds-check these functions, and fail appropriately
%define OUT_OF_RANGE_EXCEPTION(name)
%exception name {
   try {
      $action
   } catch (std::out_of_range &e) {
      SWIG_exception_fail(SWIG_IndexError, e.what());
   }
}
%enddef

%ignore wiimote::ButtonNameFromBit;
%ignore wiimote::ClassicButtonNameFromBit;
%ignore wiimote::FreqLookup;
OUT_OF_RANGE_EXCEPTION(wiimote::GetButtonNameFromBit)
OUT_OF_RANGE_EXCEPTION(wiimote::GetClassicButtonNameFromBit)
OUT_OF_RANGE_EXCEPTION(wiimote::GetFreqLookup)

// Rename a promoted-to-global struct to allow the Python module to fix the name
%rename(wiimote__state_event) wiimote_state_event;

// Use directors to handle the callback
%ignore ChangedCallback;
%feature("director");

%ignore ReportTypeName;

%include "wiimote_state.i"
%include "../wiimote.h"


// Python module footer code
%pythoncode %{
__all__ = []
def __cleanup_namespace():
  ''' Cleanup exported variables, to reduce bloat in the module.
      Removes _swigregister functions and undoes the promote-to-global
      modification which was applied earlier. '''
  class NullClass:
    pass
  for i in globals().copy():
    if i.endswith("_swigregister"):
      del globals()[i]
    elif '__' in i and not i.startswith('__') and not i.endswith('__'):
      # This name was promoted to global scope to help SWIG, as SWIG
      # cannot yet handle nested structs. The following code undoes the
      # promotion and puts the struct back to the proper place.
      path = i.split('__')
      # "super-mkdir" for objects
      for k in range(1,len(path)):
        par = '.'.join(path[:k])
        parobj = eval(par)
        if not hasattr(parobj,path[k]):
          setattr(parobj,path[k],NullClass)
      obj = globals()[i]
      setattr(parobj,path[k],obj)
      obj.__name__ = '.'.join(path)
    elif '__' not in i and not i.startswith('_') and i not in ['weakref', 'weakref_proxy']:
      __all__.append(i)
__cleanup_namespace()
del __cleanup_namespace

# export arrays from C
wiimote.ButtonNameFromBit = tuple([wiimote.GetButtonNameFromBit(i) for i in range(TOTAL_BUTTON_BITS)])
wiimote.ClassicButtonNameFromBit = tuple([wiimote.GetClassicButtonNameFromBit(i) for i in range(TOTAL_BUTTON_BITS)])
wiimote.FreqLookup = tuple([wiimote.GetFreqLookup(i) for i in range(TOTAL_FREQUENCIES)])
del i
%}
