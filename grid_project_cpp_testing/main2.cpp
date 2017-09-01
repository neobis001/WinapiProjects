#include "line_bouncing_logic.h"
#include "point_tools.h"

#include <vector>

int main()
{
	/*
	std::vector<POINT> v;
	v.push_back({ -1.0f, -1.0f });
	v.push_back({ -1.0f, -.5f });
	v.push_back({ -1.0f, 0.0f });
	v.push_back({ -1.0f, 0.5f });
	v.push_back({ -1.0f, 1.0f });

	v.push_back({ -.5f, 1.0f });
	v.push_back({ 0.0f, 1.0f });
	v.push_back({ 0.5f, 1.0f });
	v.push_back({ 1.0f, 1.0f });

	v.push_back({ 1.0f, 0.5f });
	v.push_back({ 1.0f, 0.0f });
	v.push_back({ 1.0f, -0.5f });
	v.push_back({ 1.0f, -1.0f });

	v.push_back({ 0.5f, -1.0f });
	v.push_back({ 0.0f, -1.0f });
	v.push_back({ -0.5f, -1.0f });

	std::vector<POINT>::const_iterator iterator;

	for (iterator = v.begin(); iterator != v.end(); iterator++) {
		POINT poi = *iterator;
		cout << "\n\nPoint to use " << poi.x << " " << poi.y << endl;
		LocVector a(0.5f, 0.5f, poi.x, poi.y);
		BOUNCEPOINT p = find_target_bpoint(a);
		cout << "result target point: " << p.p.x << " " << p.p.y << endl;
		cout << "result bounce vector: " << p.bounce_vec.x << " " << p.bounce_vec.y << endl;
	}

	LocVector a(0.5f, 0.5f, 1.0f, 0.0f);
	BOUNCEPOINT p = find_target_bpoint(a);
	
	cout << "\n\nend result of processing" << endl;
	*/

	//LocVector start = select_contained_locvector();
	//start.report_values();

	
	InterpolatedLV a(0.4f, 0.4f, 0.3f, -0.1f);
	a.set_interpolate_target({ 0.9f, 0.1f });

	std::vector<float> v;
	v.push_back(0.0f);
	v.push_back(1.0f);
	v.push_back(2.5f);
	v.push_back(4.9f);
	v.push_back(5.0f);

	std::vector<float>::const_iterator iter;

	for (int i = 0; i < 4; i++) {
		cout << "\n\n Round " << i << endl;
		BOUNCEPOINT bp = find_target_bpoint(a);
		a.set_interpolate_target(bp.p);

		cout << "CURRENT_TAIL" << point_tools::get_point_str(a.get_inter_tail_point()) << endl;
		for (iter = v.begin(); iter != v.end(); iter++) {
			float current_iter = *iter;
			a.interpolate_tail(0.0f, 5.0f, current_iter);

			cout << "CURRENT_INTER" <<  point_tools::get_point_str(a.get_current_inter_point()) << endl;
		}
		cout << "CURRENT_TIP" << point_tools::get_point_str(a.get_inter_tip_point()) << endl;

		a.reflect_locvector(bp);
		a.report_values();
	}


	/*
	for (int i = 0; i < 4; i++) {
		cout << "\n\nRound " << i << endl;
		BOUNCEPOINT bp = find_target_bpoint(a);
		a.reflect_locvector(bp);
		a.report_values();
	}*/

	return 0;
}