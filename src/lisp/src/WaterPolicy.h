#ifndef __SIGN_SCHEME_WATER_POLICY_H__
#define __SIGN_SCHEME_WATER_POLICY_H__

/* Copyright (C) 2012 Sinby Corporation. All Rights Reserved. */

namespace sign {
namespace scheme {

class WaterPolicy {
public:
	WaterPolicy(int a_max_water);
	void init_water();
	int eval_factor(int water);
	void update_low_water_after_gc(int factor, int water);

	int max_water;
	int current_low_low_water;
	int default_low_water;
	int current_low_water;
	int inc_low_water;
	int high_water;
};

} // namespace scheme
} // namespace sign

#endif /* __SIGN_SCHEME_WATER_POLICY_H__ */
