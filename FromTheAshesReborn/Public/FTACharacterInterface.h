#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FTACharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFTACharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class FROMTHEASHESREBORN_API IFTACharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void UpdateWarpTarget(float Distance) = 0;

	UFUNCTION()
	virtual void ResetWarpTarget() = 0;

	UFUNCTION()
	virtual AFTACharacter* GetReferenceToSelf() = 0;
};
