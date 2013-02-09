/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

#include "pair_cell.h"
#include "WaterPolicy.h"
#include "sign/sign_assert.h"

using namespace sign::scheme;

WaterPolicy::WaterPolicy(int a_max_water) : max_water(a_max_water)
{
}

//----------------------------------------------------------------

void
WaterPolicy::init_water()
{
	inc_low_water = max_water / 16;
	high_water = inc_low_water * 13;
	default_low_water = inc_low_water * 6;
	current_low_water = default_low_water;
	current_low_low_water = default_low_water / 2;
}

//----------------------------------------------------------------

int
WaterPolicy::eval_factor(int water)
{
	if ( high_water <= water ) {
		return 2;
	}
	if ( current_low_water <= water ) {
		return 1;
	}
	return 0;
}

void
WaterPolicy::update_low_water_after_gc(int factor, int water)
{
	SIGN_ASSERT( factor != 0 );
	if ( factor == 2 ) {
		max_water *= 2;
		init_water();
	} else if ( current_low_low_water <= water) {
		current_low_water += inc_low_water;
		current_low_low_water += inc_low_water;
	}
}
