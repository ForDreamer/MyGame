#ifndef _MapCamera_H_
#define _MapCamera_H_

#include "MapObject.h"

class MapCamera: public MapObject
{
public:
	MapCamera(void);
	~MapCamera(void);

	void SetPos(float x, float y);

protected:
};

#endif