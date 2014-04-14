#include "precomp.h"
#include "client_camera.h"
#include "client_zone.h"
#include "Engine/Common/GameWorld/gameobject.h"

using namespace clan;

ClientCamera::ClientCamera(const Point &projection)
	: projection(projection)
{
}
