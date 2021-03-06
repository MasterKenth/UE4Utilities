// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core/Public/Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(MKUE4UtilitiesLog, Log, All);

class FMKUE4UtilitiesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};