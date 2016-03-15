#pragma once
#include "Remotery.h"

class RemoteryDebug
{
public:
	static Remotery *get_remotery()
	{
		static Remotery* _rmt = nullptr;
		if (!_rmt)
			rmt_CreateGlobalInstance(&_rmt);
		return _rmt;
	}
	static void ReleaseRemotery()
	{
		rmt_DestroyGlobalInstance(RemoteryDebug::get_remotery());
	}
};
