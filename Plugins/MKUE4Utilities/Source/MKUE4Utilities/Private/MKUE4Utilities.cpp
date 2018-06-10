// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MKUE4Utilities.h"
//#include "MKUE4UtilitiesPrivatePCH.h"
#include "MKUE4UtilitiesLibrary.h"

#define LOCTEXT_NAMESPACE "FMKUE4UtilitiesModule"

DEFINE_LOG_CATEGORY(MKUE4UtilitiesLog)

void FMKUE4UtilitiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(MKUE4UtilitiesLog, Display, TEXT("HELLO, UTILITIES STARTED"));
}

void FMKUE4UtilitiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMKUE4UtilitiesModule, MKUE4Utilities)