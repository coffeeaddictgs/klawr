﻿//
// The MIT License (MIT)
//
// Copyright (c) 2014 Vadim Macagon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

using System;
using System.Runtime.InteropServices;

namespace Klawr.ClrHost.Interfaces
{
    /// <summary>
    /// Contains delegates encapsulating native UObject and UClass utility functions.
    /// </summary>
    [ComVisible(true)]
    [Guid("06A91CEC-0B66-4DCC-B4AB-7DFF3F237F48")]
    [StructLayout(LayoutKind.Sequential)]
    public struct ObjectUtilsProxy
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate UObjectHandle GetClassByNameFunc(string nativeClassName);
        
        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate string GetClassNameFunc(UObjectHandle nativeClass);
        
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public delegate bool IsClassChildOfFunc(UObjectHandle derivedClass, UObjectHandle baseClass);
        
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void RemoveObjectRefAction(IntPtr nativeObject);

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public GetClassByNameFunc GetClassByName;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public GetClassNameFunc GetClassName;
        
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public IsClassChildOfFunc IsClassChildOf;
        
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public RemoveObjectRefAction RemoveObjectRef;
    }
}