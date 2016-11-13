/**
Copyright (c) 2013, Philip Deegan.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.
    * Neither the name of Philip Deegan nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _KUL_SYS_HPP_
#define _KUL_SYS_HPP_

#include "kul/log.hpp"
#include "kul/sys.base.hpp"

#include <dlfcn.h>

#ifndef __KUL_SYS_DLOPEN__
#define __KUL_SYS_DLOPEN__ RTLD_NOW|RTLD_GLOBAL
#endif//__KUL_SYS_DLOPEN__

namespace kul{
namespace sys {

template <class F>
class SharedFunctionp;

class SharedLibrary {
    template <class F> friend class SharedFunction;
    private:
        bool _loaded = 0;
        void* _handle;
        const kul::File _f;
    public:
        SharedLibrary(const kul::File& f) throw(Exception) : _f(f){
            KLOG(INF) << f;
            if(f) KLOG(INF) << f.real();
            if(!_f) KEXCEPSTREAM << "Library attempted to be loaded does not exist: " << _f.full();
            _handle = dlopen(_f.real().c_str(), __KUL_SYS_DLOPEN__);
            if(!_handle) KEXCEPSTREAM << "Cannot load library: " << f << " - Error: " << dlerror();
            _loaded = 1;
            if(f) KLOG(INF);
        }
        ~SharedLibrary(){
            if(_loaded) dlclose(_handle);
            dlerror();
        }
};

template <class F>
class SharedFunction {
    private:
        F* _funcP;
        SharedLibrary& _lib;
    public:
        SharedFunction(SharedLibrary& lib, const std::string& f) throw(Exception) : _lib(lib){
            _funcP = (F*) dlsym(_lib._handle, f.c_str());
            const char* dlsym_error = dlerror();
            if (dlsym_error) KEXCEPSTREAM << "Cannot load symbol create " << dlsym_error;
        }
        ~SharedFunction(){
            dlerror();
        }
        F* pointer(){
            return _funcP;
        }
};

template <class T>
class SharedClass{
    typedef T*   construct_t();
    typedef void destruct_t (T* t);
    private:
        SharedLibrary _lib;
        SharedFunction<construct_t> _c;
        SharedFunction<destruct_t>  _d;
    public: 

        SharedClass(const kul::File& f, const std::string& c, const std::string& d) throw(Exception) 
            : _lib(f), _c(_lib, c), _d(_lib, d) {}
        virtual ~SharedClass(){}

    protected: 
        void construct(T*& t) throw(Exception) {
            t = _c.pointer()();
            if(!t) KEXCEPSTREAM << "Dynamically loaded class was not created";
        }
        void destruct (T*& t){
            _d.pointer()(t);
            t = nullptr;
            if(t)  KEXCEPSTREAM << "Dynamically loaded class was not destroyed";
        }

};


} // END NAMESPACE sys
} // END NAMESPACE kul

#endif /* _KUL_SYS_HPP_ */
