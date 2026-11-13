#include "FSLapGameMode.h"

#include "FSLapHUD.h"
#include "FSLapVehiclePawn.h"

AFSLapGameMode::AFSLapGameMode()
{
	DefaultPawnClass = AFSLapVehiclePawn::StaticClass();
	HUDClass = AFSLapHUD::StaticClass();
}
