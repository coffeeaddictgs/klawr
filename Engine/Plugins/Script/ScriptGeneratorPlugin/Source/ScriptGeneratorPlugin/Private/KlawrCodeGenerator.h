//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
#pragma once

#include "ScriptCodeGeneratorBase.h"

/**
 * Generates a C# wrapper class for each scriptable UE4 class.
 *
 * The generated C# class encapsulates a bunch of delegates that are bound to pointers to native
 * wrapper functions (that are also generated by this class).
 */
class FKlawrCodeGenerator : public FScriptCodeGeneratorBase
{
public:
	FKlawrCodeGenerator(
		const FString& RootLocalPath, const FString& RootBuildPath, const FString& OutputDirectory,
		const FString& InIncludeBase
	);

public: // FScriptCodeGeneratorBase interface
	virtual void ExportClass(
		UClass* Class, const FString& SourceHeaderFilename, const FString& GeneratedHeaderFilename,
		bool bHasChanged
	) override;

	virtual void FinishExport() override;

protected: // FScriptCodeGeneratorBase interface
	virtual bool CanExportClass(UClass* Class) override;

	virtual bool CanExportFunction(
		const FString& ClassNameCPP, UClass* Class, UFunction* Function
	) override;

	virtual bool CanExportProperty(
		const FString& ClassNameCPP, UClass* Class, UProperty* Property
	) override;

	virtual FString InitializeFunctionDispatchParam(
		UFunction* Function, UProperty* Param, int32 ParamIndex
	) override;

private:
	// for convenience
	typedef FScriptCodeGeneratorBase Super;

	static const FName Name_Vector2D;
	static const FName Name_Vector;
	static const FName Name_Vector4;
	static const FName Name_Quat;
	static const FName Name_Transform;
	static const FName Name_LinearColor;
	static const FName Name_Color;

	FString NativeIndent;
	FString ManagedIndent;

	static const FString NativeTab;
	static const FString ManagedTab;

	struct FExportedProperty
	{
		FString GetterDelegateName;
		FString GetterDelegateTypeName;
		FString SetterDelegateName;
		FString SetterDelegateTypeName;
	};

	struct FExportedFunction
	{
		const UFunction* Function;
		bool bHasReturnValue;
	};

	/** All generated C++ script header filenames. */
	TArray<FString> AllScriptHeaders;
	/** Engine source header filenames for all exported classes. */
	TArray<FString> AllSourceClassHeaders;
	/** Functions that were exported for each class. */
	TMap<const UClass*, TArray<FExportedFunction> > ClassExportedFunctions;
	/** Properties that were exported for each class. */
	TMap<const UClass*, TArray<FExportedProperty> > ClassExportedProperties;

	/** Create a 'glue' file that merges all generated script files */
	void GlueAllGeneratedFiles();

	/** Export native and managed wrappers for the given UFunction. */
	void ExportFunction(
		const FString& ClassNameCPP, const UClass* Class, UFunction* Function, 
		FString& NativeGlueCode, FString& ManagedGlueCode
	);
	/** Export native and managed wrappers for the given UProperty. */
	void ExportProperty(
		const FString& ClassNameCPP, UClass* Class, UProperty* Property,
		FString& NativeGlueCode, FString& ManagedGlueCode
	);
	/** Generate a statement returning the given value. */
	FString GenerateNativeReturnValueHandler(UProperty* ReturnValue, const FString& ReturnValueName);
	/** Check if a property type is supported */
	bool IsPropertyTypeSupported(UProperty* Property) const;
	/** Check if the property type is a pointer. */
	static bool IsPropertyTypePointer(const UProperty* Property);
	/** Check if the property type is a struct that can be used for interop. */
	static bool IsStructPropertyTypeSupported(const UStructProperty* Property);

	FString GetPropertyNativeType(UProperty* Property);
	FString GetPropertyInteropType(UProperty* Property);

	UProperty* GetNativeWrapperArgsAndReturnType(
		const UFunction* Function, FString& OutFormalArgs, FString& OutActualArgs
	);

	void GenerateManagedWrapperArgsAndReturnType(
		const UFunction* Function, FString& OutFormalInteropArgs, FString& OutActualInteropArgs,
		FString& OutFormalManagedArgs, FString& OutActualManagedArgs,
		FString& OutReturnValueType
	);
		
	FString GenerateNativeWrapperFunction(
		const UClass* Class, UFunction* Function, const UClass* FuncSuper
	);

	FString GenerateManagedWrapperFunction(
		const UClass* Class, const UFunction* Function, const UClass* FuncSuper
	);

	FString GenerateNativePropertyGetterWrapper(
		const FString& ClassNameCPP, UClass* Class, UProperty* Property, UClass* PropertySuper
	);

	FString GenerateNativePropertySetterWrapper(
		const FString& ClassNameCPP, UClass* Class, UProperty* Property, UClass* PropertySuper
	);

	FString GenerateManagedPropertyWrapper(
		UClass* Class,  UProperty* Property, UClass* PropertySuper
	);

	FString GenerateManagedStaticConstructor(const UClass* Class);
	FString GenerateNativeGlueCodeHeader(const UClass* Class) const;
	FString GenerateNativeGlueCodeFooter(const UClass* Class) const;
	FString GenerateManagedGlueCodeHeader(const UClass* Class);
	FString GenerateManagedGlueCodeFooter(const UClass* Class);
	FString GenerateDelegateTypeName(const FString& FunctionName, bool bHasReturnValue) const;
	FString GenerateDelegateName(const FString& FunctionName) const;
	/** Indents ManagedIndent by 4 spaces. */
	void IndentManagedCode();
	/** Unindents ManagedIndent by 4 spaces. */
	void UnindentManagedCode();
	/** Indents NativeIndent by one tab. */
	void IndentNativeCode();
	/** Unindents NativeIndent by one tab. */
	void UnindentNativeCode();
};
