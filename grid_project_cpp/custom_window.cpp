#include "custom_window.h"
PCSWindow CSWINDOW_HOLDER = nullptr;

WNDCLASSEX convert_custom_wndclassex(CSWNDCLASSEX cstm_wndclass, DYNCSTR lpszClassName, WNDPROC lpfnWndProc) {
	WNDCLASSEX res_class;
	res_class.cbSize = sizeof(WNDCLASSEX);
	res_class.style = cstm_wndclass.style;
	res_class.lpfnWndProc = lpfnWndProc;
	res_class.cbClsExtra = cstm_wndclass.cbClsExtra;
	res_class.cbWndExtra = cstm_wndclass.cbWndExtra;
	res_class.hInstance = cstm_wndclass.hInstance;
	res_class.hIcon = cstm_wndclass.hIcon;
	res_class.hCursor = cstm_wndclass.hCursor;
	res_class.hbrBackground = cstm_wndclass.hbrBackground;
	res_class.lpszMenuName = cstm_wndclass.lpszMenuName;
	res_class.lpszClassName = lpszClassName;
	res_class.hIconSm = cstm_wndclass.hIconSm;
	return res_class;
}

RETURNMSG create_default_option(LRESULT option_value) {
	RETURNMSG return_msg;
	return_msg.return_option = DEFAULT_RETURN_OPTION;
	return_msg.option_value = option_value;
	return return_msg;
}

RETURNMSG create_defproc_option() {
	RETURNMSG return_msg;
	return_msg.return_option = DEFAULT_PROC_OPTION;
	return_msg.option_value = 0;
	return return_msg;
}

void set_cswindow_holder(PCSWindow cswindow) {
	CSWINDOW_HOLDER = cswindow;
}

void reset_cswindow_holder() {
	CSWINDOW_HOLDER = nullptr;
}

void create_prop_from_cswindow_holder(HWND hwnd) {
	if (CSWINDOW_HOLDER == NULL) {
		MessageBox(NULL, TEXT("Warning (in create_prop_from_cswindow_holder): CSWINDOW_HOLDER\n"\
			"is currently NULL. won't set any prop then"), TEXT("Warning"), MB_OK);
	}
	else if (GetProp(hwnd, CONNECTED_SUBCLASS_WND) != NULL) {
		MessageBox(NULL, TEXT("Warning (in create_prop_from_cswindow_holder): hwnd has\n"\
			"a CONNECTED_SUBCLASS_WND prop already. will not overwrite"), TEXT("Warning"), MB_OK);
	}
	else {
		SetProp(hwnd, CONNECTED_SUBCLASS_WND, (HANDLE)CSWINDOW_HOLDER);
	}
}

LRESULT subproc_return(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, RETURNMSG return_msg) {
	if (return_msg.return_option == DEFAULT_PROC_OPTION) {
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
	else if (return_msg.return_option == DEFAULT_RETURN_OPTION) {
		return return_msg.option_value;
	}
	else {
		DYNCHAR message[100];
		wsprintf(message,
			TEXT("Warning (in subproc_return): non-standard return option (%lu), will return DefSubClassProc"),
			return_msg.return_option);
		MessageBox(NULL, message, TEXT("Warning"), MB_OK);
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT proc_return(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, RETURNMSG return_msg) {
	if (return_msg.return_option == DEFAULT_PROC_OPTION) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else if (return_msg.return_option == DEFAULT_RETURN_OPTION) {
		return return_msg.option_value;
	}
	else {
		DYNCHAR message[100];
		wsprintf(message, 
			TEXT("Warning (in proc_return): non-standard return option (%lu), will return DefWindowProc"),
			return_msg.return_option);
		MessageBox(NULL, message, TEXT("Warning"), MB_OK);
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

unsigned short int CSWindow::available_subclass_id = STARTING_SUBCLASS_ID;

void CSWindow::show_register_quit() {
	msg_force_quit(TEXT("Error (in CSWindow constructor):\n"\
		"Window Registration Failed! Will quit program.\n"\
		"Consider checking errors. GetLastError() may help"));
}

void CSWindow::show_window_creation_quit() {
	msg_force_quit(TEXT("Error (in CSWindow constructor):\n"\
		"Window Creation Failed! Will quit program.\n"\
		"Consider checking errors. GetLastError() may help"));
}

void CSWindow::copy_window_data(const CSWindow& other) {
	subclass_id = other.subclass_id;
	has_subclass = other.has_subclass;
	num_children = other.num_children;
	children = get_list_copy<PCSWindow>(children, num_children);
	parent = other.parent;
	hwnd = other.hwnd;
	stored_wops = other.stored_wops;

	base_width = other.base_width;
	base_height = other.base_height;
	pad_x = other.pad_x;
	pad_y = other.pad_y;

	placing_option = other.placing_option;
	children_placing_option = other.children_placing_option;
	row_number = other.row_number;
	col_number = other.col_number;
	num_rows = other.num_rows;
	num_cols = other.num_cols;
	columnspan = other.columnspan;
	rowspan = other.rowspan;
	row_weights = get_list_copy<int>(other.row_weights, num_rows);
	col_weights = get_list_copy<int>(other.col_weights, num_cols);
	pack_min_cavity_width = other.pack_min_cavity_width;
	pack_min_cavity_height = other.pack_min_cavity_height;
	resize_on_pack = other.resize_on_pack;
	side = other.side;
	expand = other.expand;
	is_prg = other.is_prg;
}
void CSWindow::set_window_subclass() {
	SetWindowSubclass(hwnd, &defaultSubclassWndProc, available_subclass_id, 0);
	validate_subclass_id(true);
}

void CSWindow::setup_subclass_connection() {
	set_cswindow_holder(this);
	SendMessage(hwnd, CUSTOM_SUBCLASS_SETUP, 0, 0);
	reset_cswindow_holder();
}

void CSWindow::validate_subclass_id(bool has_subclass) {
	if (has_subclass) {
		this->has_subclass = true;
		subclass_id = available_subclass_id;
		available_subclass_id++;
	}
	else {
		this->has_subclass = false;
		subclass_id = NULL_SUBCLASS_ID;
	}
}

void CSWindow::attach_subclass() {
	set_window_subclass();
	setup_subclass_connection();
}

HWND CSWindow::get_hwnd_parent() {
	if (parent == nullptr) {
		return (HWND)NULL;
	}
	else {
		return parent->get_hwnd();
	}
}

void CSWindow::attempt_hwnd_creation() {
	set_cswindow_holder(this);

	HWND parent_hwnd = get_hwnd_parent();
	CSWNDEXOPS sw = stored_wops;
	HWND res_hwnd = CreateWindowEx(sw.dwExStyle, sw.lpClassName, sw.lpWindowName, sw.dwStyle, 
		sw.x, sw.y, sw.nWidth, sw.nHeight, parent_hwnd, sw.hMenu, sw.hInstance,
		sw.lpParam);
	if (res_hwnd == NULL) {
		show_window_creation_quit();
	}
	else {
		hwnd = res_hwnd;
	}

	reset_cswindow_holder();
}

void CSWindow::create_linked_hwnd(PCSWindow parent) {
	attempt_hwnd_creation();
	attach_subclass();
}

int CSWindow::get_total_weights(int* weight_list, int weight_list_size) {
	int total_weights = 0;
	for (int i = 0; i < weight_list_size; i++) {
		total_weights += *(weight_list + i);
	}
	return total_weights;
}


int CSWindow::get_weighted_space(LONG window_dimension, int dim_coord, 
	int span, pint weight_list, int weight_list_size) {
	int alloted_weight = 0;
	for (int i = dim_coord; i < dim_coord + span; i++) {
		alloted_weight += *(weight_list + i);
	}
	int total_weight = get_total_weights(weight_list, weight_list_size);

	float weighted_space = (float)window_dimension * ((float)alloted_weight / (float)total_weight);
	return (int)weighted_space;
	//return window_dimension * (alloted_weight / total_weight);
	//return (window_dimension / number_divisions) * span;
}

int CSWindow::get_padded_space(int allocated_dimension, int padding) {
	int padded_space = allocated_dimension - 2 * padding;
	padded_space = get_max_value(0, padded_space);

	return padded_space;
}

int CSWindow::get_padded_coord(int allocated_dimension, int target_dim_coord, int padding) {
	int allocated_half_width = allocated_dimension / 2;
	int half_width_coord = target_dim_coord + allocated_half_width;
	int padded_coord = target_dim_coord + padding;
	
	int res_coord = get_min_value(half_width_coord, padded_coord);
	return res_coord;
}


void CSWindow::check_okay_gridding(int row_number, int col_number, int rowspan, int colspan) {
	do_standard_manage_checks(GRID_PLACING_OPTION);
	if (!has_okay_grid_coords(row_number, col_number, rowspan, colspan)) {
		DYNCHAR buffer[300];
		wsprintf(buffer, "Error (in CSWindow::check_okay_gridding): coords out of range: \n"\
			"row input: %i, column input: %i (should be at least 0)\n"\
			"row span: %i, column span: %i (should be at least 1)\n"\
			"parent number rows: %i, parent number colums: %i",
			row_number, col_number, rowspan,
			colspan, parent->num_rows, parent->num_cols);
		msg_force_quit(buffer);
	}
}

bool CSWindow::has_okay_grid_coords(int row_number, int col_number, int rowspan, int colspan) {
	//assume parent exists
	if (!pure_grid_coords_are_valid(row_number, col_number)) {
		return false;
	}
	else if (!grid_spanning_is_valid(row_number, col_number, rowspan, colspan)) {
		return false;
	}
	else {
		return true;
	}
}

bool CSWindow::pure_grid_coords_are_valid(int row_number, int col_number) {
	if (row_number < 0 || row_number >= parent->num_rows) {
		return false;
	}
	else if (col_number < 0 || col_number >= parent->num_cols) {
		return false;
	}
	else {
		return true;
	}
}

bool CSWindow::grid_spanning_is_valid(int row_number, int col_number, int rowspan, int colspan) {
	//assume row and col numbers are good
	if (rowspan < 1 || row_number + rowspan > parent->num_rows) {
		return false;
	}
	else if (colspan < 1 || col_number + colspan > parent->num_cols) {
		return false;
	}
	else {
		return true;
	}
}

void CSWindow::initialize_gridding_weights(pint& weight_list, int num_weight_entries) {
	weight_list = new int[num_weight_entries];
	for (int i = 0; i < num_weight_entries; i++) {
		*(weight_list + i) = 1;
	}
}

int CSWindow::get_weighted_grid_coord(LONG window_dim,
	int dim_coord, pint weight_list, int weight_list_size) {
	int coord_weight = 0;
	for (int i = 0; i < dim_coord; i++) {
		coord_weight += *(weight_list + i);
	}
	int total_weight = get_total_weights(weight_list, weight_list_size);

	float res_coord = (float)window_dim * ((float)coord_weight / (float)total_weight);
	return (int)res_coord;
}

bool CSWindow::weight_changes_are_valid(int dim_coord, int dim_size, int weight) {
	if (dim_coord < 0 || dim_coord >= dim_size || weight < 1) {
		return false;
	}
	else {
		return true;
	}
}

void CSWindow::add_child(PCSWindow child) { //idea: append previous children pointers first
											//if any, then append the new child
	if (in<PCSWindow>(child, children, num_children)) {
		DYNCHAR buffer[200];
		wsprintf(buffer, "Warning (in CSWindow::add_child): child of address %p already in\n"\
			"parent of address %p. won't add child", child, this);
		msg_string(buffer);
	}
	else if (child == nullptr) {
		msg_string("Warning(in CSWindow::add_child): child is nullptr. won't add child");
	} 
	else {
		int current_num_children = num_children;
		num_children++;
		int new_num_children = num_children;

		PCSWindow* current_children_list = children;
		children = new PCSWindow[new_num_children];

		for (int i = 0; i < current_num_children; i++) {
			*(children + i) = *(current_children_list + i);
		}
		int new_child_index = new_num_children - 1;
		*(children + new_child_index) = child;

		delete[] current_children_list;
		current_children_list = nullptr;
	}
}

void CSWindow::attempt_window_registration(CSWNDCLASSEX cswndclass, DYNCSTR lpszClassName, WNDPROC lpfnWndProc) {
	WNDCLASSEX wndclass = convert_custom_wndclassex(cswndclass, lpszClassName, &defaultWndProc);
	if (RegisterClassEx(&wndclass) == 0) //== 0 means registering failed
	{
		show_register_quit();
	}
}

bool CSWindow::requests_dif_wnd_manager(int requested_placing_mode) { //assumes parent exists
	if (parent->children_placing_option == NULL_PLACING_OPTION) {
		return false;
	} 
	else {
		if (requested_placing_mode != parent->children_placing_option) {
			return true;
		}
		else {
			return false;
		}
	}
}

void CSWindow::connect_placing_modes(int placing_option) {
	parent->children_placing_option = placing_option; //if placing multiples times of the
	  //same option, then this is repetitive, but harmless
	  //seing this as better than doing if statement check (and faster by one instruction haha)
	this->placing_option = placing_option;
}

void CSWindow::check_okay_packing() {
	do_standard_manage_checks(PACK_PLACING_OPTION);
}

void CSWindow::topmost_draw() {
	if (parent == nullptr) {
		prg_draw_children();
	}
	else {
		parent->topmost_draw();
	}
}

void CSWindow::init_grid_values(int row_number, int col_number, int padx, int pady, int rowspan, int columnspan) {
	this->row_number = row_number;
	this->col_number = col_number;
	this->pad_x = padx;
	this->pad_y = pady;
	this->rowspan = rowspan;
	this->columnspan = columnspan;
}

void CSWindow::init_pack_values(SIDE side, bool expand, int pad_x, int pad_y) {
	this->side = side;
	this->expand = expand;
	this->pad_x = pad_x;
	this->pad_y = pad_y;
}

void CSWindow::do_standard_manage_checks(int parent_manager_request) {
	if (parent == nullptr) {
		msg_force_quit("Error (in CSWindow::do_standard_manage_checks): no parent found,\n"\
			"need a parent before gridding.");
	}
	else if (requests_dif_wnd_manager(parent_manager_request)) {
		msg_force_quit("Error (in CSWindow::do_standard_manage_checks): atttempting to grid while \n"\
			"parent has children, but parent's placing mode is not gridding.");
	}
}

void CSWindow::resize_via_packing_cavity() {
	if (parent == nullptr) {
		RECT r;
		r.bottom = pack_min_cavity_height;
		r.left = 0;
		r.top = 0;
		r.right = pack_min_cavity_width;
		AdjustWindowRectEx(&r, stored_wops.dwStyle, false, stored_wops.dwExStyle);
		
		int res_width = r.right - r.left;
		int res_height = r.bottom - r.top;
		set_window_pos(NULL, 0, 0, res_width, res_height, SWP_NOMOVE);
	}
	else {
		set_window_pos(NULL, 0, 0, pack_min_cavity_width, pack_min_cavity_height, SWP_NOMOVE);
	}
}

void CSWindow::store_wops_data(CSWNDEXOPS wops) {
	stored_wops = wops;
	set_base_dimensions(stored_wops.nWidth, stored_wops.nHeight);
}

void CSWindow::prepare_stored_wops(CSWNDEXOPS wops, PCSWindow parent) {
	if (parent != nullptr) {
		set_parent(parent); //parent must be set for later access by attempt_hwnd_creation
	}
	store_wops_data(wops);
	add_wops_defaults();
}

void CSWindow::init(CSWNDCLASSEX cswndclass, CSWNDEXOPS wops, PCSWindow parent) {
	attempt_window_registration(cswndclass, wops.lpClassName, &defaultWndProc);
	prepare_stored_wops(wops, parent);
	create_linked_hwnd(parent);
	//if the wops and cswndclass have same class name, then a wnd. proc and subclass proc will both be made
	//at first, sounds incorrect to have both
	//but wnd. proc will only handle pre-window-creation messages, design-wise
}

void CSWindow::init(CSWNDEXOPS wops, PCSWindow parent) {
	prepare_stored_wops(wops, parent);
	create_linked_hwnd(parent);
}

void CSWindow::set_base_dimensions(int base_width, int base_height) {
	this->base_width = base_width;
	this->base_height = base_height;
}

int CSWindow::get_deep_num_children() {
	if (num_children == 0) {
		return 0;
	}
	else {
		int total_to_add = num_children;
		for (int i = 0; i < num_children; i++) {
			PCSWindow w = *(children + i);
			total_to_add += w->get_deep_num_children();
		}
		return total_to_add;
	}
}

int CSWindow::get_num_child_hwnds() {
	int total = 0;
	for (int i = 0; i < num_children; i++) {
		PCSWindow window = *(children + i);
		total += window->num_hwnds;
	}
	return total;
}

void CSWindow::prg_draw_as_grid(HDWP* hdwp) {
	check_okay_gridding(row_number, col_number, rowspan, columnspan);

	//
	//start by getting grid data involving parent
	LONG parent_client_width = parent->get_client_width();
	LONG parent_client_height = parent->get_client_height();
	int parent_number_cols = parent->num_cols;
	int parent_number_rows = parent->num_rows;
	int* parent_row_weights = parent->row_weights;
	int* parent_col_weights = parent->col_weights;

	//
	//calculate the dimensions needed to normally grid an item, w/o considering padding
	//or edge cases
	int allocated_width = get_weighted_space(parent_client_width, col_number, columnspan,
		parent_col_weights, parent_number_cols);
	int allocated_height = get_weighted_space(parent_client_height, row_number, rowspan,
		parent_row_weights, parent_number_rows);

	int target_x = get_weighted_grid_coord(parent_client_width, col_number,
		parent_col_weights, parent_number_cols);
	int target_y = get_weighted_grid_coord(parent_client_height, row_number,
		parent_row_weights, parent_number_rows);

	//
	//possibly adjust the allocated width/height if it's on an edge row/column
	if (col_number + 1 == parent_number_cols) {
		allocated_width = parent_client_width - target_x;
	}

	if (row_number + 1 == parent_number_rows) {
		allocated_height = parent_client_height - target_y;
	}

	//
	//get padded dimensions as final gridding dimensions
	CSLOCRECT allocated_rect = create_zeroed_struct<CSLOCRECT>();
	allocated_rect.loc.x = target_x;
	allocated_rect.loc.y = target_y;
	allocated_rect.dim.width = allocated_width;
	allocated_rect.dim.height = allocated_height;

	CSLOCRECT res_rect = get_padded_rect_info(allocated_rect);

	prg_defer_window_pos(hdwp, NULL, res_rect.loc.x, res_rect.loc.y,
		res_rect.dim.width, res_rect.dim.height, 0);
}

void CSWindow::prg_draw(HDWP* receiving_hdwp) { 
	//hdwp exists, so assuming optional 0 check made beforehand
	prg_draw_self(receiving_hdwp);
}

void CSWindow::prg_draw_self(HDWP* hdwp) {
	if (placing_option == GRID_PLACING_OPTION) {
		prg_draw_as_grid(hdwp);
	}

}

void CSWindow::prg_toggle_isprg(bool turn_on) {
	this->is_prg = turn_on;
}

void CSWindow::prg_draw_children() {
	if (num_children != 0) {
		int num_child_hwnds = get_num_child_hwnds();
		HDWP hdwp = BeginDeferWindowPos(num_child_hwnds);
		
		prg_toggle_children_prgs(true);
		for (int i = 0; i < num_children; i++) {
			PCSWindow window = *(children + i);
			if (children_placing_option == PACK_PLACING_OPTION) {
				prg_draw_as_pack(&hdwp);
			} else if (children_placing_option == CUSTOM_PLACING_OPTION) {
				prg_draw_as_custom(&hdwp);
			}
			else {
				window->prg_draw(&hdwp);
			}
		}
		EndDeferWindowPos(hdwp);
		prg_toggle_children_prgs(false);

		for (int i = 0; i < num_children; i++) {
			PCSWindow window = *(children + i);
			window->prg_draw_children();
		}
	}
}

void CSWindow::prg_toggle_children_prgs(bool turn_on) {
	for (int i = 0; i < num_children; i++) {
		PCSWindow window = *(children + i);
		window->prg_toggle_isprg(turn_on);
	}
}

void CSWindow::prg_draw_as_custom(HDWP* hdwp) {
	msg_force_quit("Error (in CSWindow::prg_draw_as_custom): this custom draw function should be overwritten"\
		"Will quit for now");
}

void CSWindow::add_wops_defaults() {
	HWND hwnd_parent = get_hwnd_parent();
	if (hwnd_parent != (HWND)NULL) {
		stored_wops.dwStyle |= WS_CHILD;
	}

	stored_wops.dwStyle |= WS_CLIPCHILDREN;
}

CSWindow::CSWindow() {}

CSWindow::CSWindow(CSWNDCLASSEX cswndclass, CSWNDEXOPS wops, PCSWindow parent) {
	init(cswndclass, wops, parent);
}

CSWindow::CSWindow(CSWNDEXOPS wops, PCSWindow parent) {
	init(wops, parent);
}

CSWindow::CSWindow(const CSWindow& other) {
	copy_window_data(other);
}


CSWindow& CSWindow::operator= (const CSWindow& other) {
	if (this == &other) {
		return *this;
	}
	else {
		if (children != nullptr) {
			delete[] children;
		}

		copy_window_data(other);
		return *this;
	}
}


CSWindow::~CSWindow()
{
	delete_children();

	delete[] row_weights;
	delete[] col_weights;
}

int CSWindow::get_placing_option() {
	return placing_option;
}


int CSWindow::get_base_width() {
	return base_width;
}

int CSWindow::get_base_height() {
	return base_height;
}

SIDE CSWindow::get_packing_side() {
	return side;
}

bool CSWindow::get_packing_expand() {
	return expand;
}

LONG CSWindow::get_client_x() {
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	LONG client_x = windowRect.left;
	return client_x;
}

LONG CSWindow::get_client_y() {
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	LONG client_x = windowRect.left;
	return client_x;
}

LONG CSWindow::get_window_y() {
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	LONG window_y = windowRect.top;
	return window_y;
}

LONG CSWindow::get_window_x() {
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	LONG window_x = windowRect.left;
	return window_x;
}

LONG CSWindow::get_client_height() {
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	LONG client_height = windowRect.bottom - windowRect.top;
	return client_height;
}

LONG CSWindow::get_client_width() {
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	LONG client_width = windowRect.right - windowRect.left;
	return client_width;
}

LONG CSWindow::get_window_width() {
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	LONG window_width = windowRect.right - windowRect.left;
	return window_width;
}


LONG CSWindow::get_window_height() {
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	LONG window_height = windowRect.bottom - windowRect.top;
	return window_height;
}

HWND CSWindow::get_hwnd() const {
	return hwnd;
}

int CSWindow::get_num_children() const {
	return num_children;
}

bool CSWindow::get_has_subclass() const {
	return has_subclass;
}

int CSWindow::get_subclass_id() const {
	return subclass_id;
}

PCSWindow* CSWindow::get_children_copy() const {
	if (children == nullptr) {
		return nullptr;
	}
	else {
		PCSWindow* new_list = new PCSWindow[num_children];

		for (int i = 0; i < num_children; i++) {
			*(new_list + i) = *(children + i);
		}
		return new_list;
	}
}

PCSWindow CSWindow::get_parent() {
	return parent;
}

void CSWindow::clear_children_list() {
	delete[] children;
	children = nullptr;
}

void CSWindow::delete_children() {
	if (children != nullptr) {
		for (int i = 0; i < num_children; i++) {
			PCSWindow pcsw = *(children + i);
			delete pcsw;
		}

		clear_children_list();
	}
}

void CSWindow::grid(int row_number, int col_number, int padx, int pady, int rowspan, int columnspan) {
	check_okay_gridding(row_number, col_number, rowspan, columnspan);
	connect_placing_modes(GRID_PLACING_OPTION);
	
	init_grid_values(row_number, col_number, padx, pady, rowspan, columnspan);
}

void CSWindow::pack(SIDE side, bool expand, int padx, int pady) {
	check_okay_packing();
	connect_placing_modes(PACK_PLACING_OPTION);

	init_pack_values(side, expand, padx, pady);
	parent->set_packing_cavity();
}

void CSWindow::set_gridding_space(int num_rows, int num_cols) {
	if (num_rows < 1 || num_cols < 1) {
		DYNCHAR buffer[200];
		wsprintf(buffer, "Error (in CSWindow::set_gridding_space): invalid grid dimensions\n"\
			"number rows: %i, number columns: %i (should both be at least 1)",
			num_rows, num_cols);
		msg_force_quit(buffer);
	}

	this->num_rows = num_rows;
	this->num_cols = num_cols;

	initialize_gridding_weights(row_weights, num_rows);
	initialize_gridding_weights(col_weights, num_cols);
}

void CSWindow::weight_grid_row(int row_num, int weight) {
	if (!weight_changes_are_valid(row_num, num_rows, weight)) {
		DYNCHAR buffer[200];
		wsprintf(buffer, "Error (in CSWindow::weight_grid_row): invalid weight inputs\n"\
			"row number: %i, number rows: %i\n"\
			"input weight: %i (should at least be 1 to be a feasible weight)", 
			row_num, num_rows, weight);
		msg_force_quit(buffer);
	} else {
		*(row_weights + row_num) = weight;
	}
}

void CSWindow::weight_grid_col(int col_num, int weight) {
	if (!weight_changes_are_valid(col_num, num_cols, weight)) {
		DYNCHAR buffer[200];
		wsprintf(buffer, "Error (in CSWindow::weight_grid_col): invalid weight inputs\n"\
			"col number: %i, number cols: %i\n"\
			"input weight: %i (should at least be 1 to be a feasible weight)",
			col_num, num_cols, weight);
		msg_force_quit(buffer);
	}
	else {
		*(col_weights + col_num) = weight;
	}
}

void CSWindow::set_parent(PCSWindow parent_input) {
	if (parent_input == nullptr) {
		msg_string("Warning (in CSWindow::set_parent): parent_input is nullptr. won't set parent");
	}
	else {
		parent = parent_input;
		parent->add_child(this);
	}
}

void CSWindow::show(int nCmdShow) {
	ShowWindow(hwnd, nCmdShow);
}

void CSWindow::update() {
	UpdateWindow(hwnd);
}

void CSWindow::set_window_pos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
	UINT uFlags) {
	SetWindowPos(hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void CSWindow::visual_redraw() {
	InvalidateRect(hwnd, NULL, TRUE); //NULL means request --entire-- client area for redraw
	update();
}

CSLOCRECT CSWindow::get_padded_rect_info(CSLOCRECT target_lrect) {
	CSLOCRECT tr = target_lrect;

	int padded_width = get_padded_space(tr.dim.width, pad_x);
	int padded_height = get_padded_space(tr.dim.height, pad_y);

	int padded_target_x = get_padded_coord(tr.dim.width, tr.loc.x, pad_x);
	int padded_target_y = get_padded_coord(tr.dim.height, tr.loc.y, pad_y);

	CSLOCRECT res_lrect = create_zeroed_struct<CSLOCRECT>();
	res_lrect.dim.width = padded_width;
	res_lrect.dim.height = padded_height;
	res_lrect.loc.x = padded_target_x;
	res_lrect.loc.y = padded_target_y;

	return res_lrect;
}

void CSWindow::prg_defer_window_pos(HDWP* hdwp, HWND hWndInsertAfter, int X, int Y, int cx, int cy,
	UINT uFlags) {
	*hdwp = DeferWindowPos(*hdwp, hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

void CSWindow::request_custom_placing() {
	do_standard_manage_checks(CUSTOM_PLACING_OPTION);
	connect_placing_modes(CUSTOM_PLACING_OPTION);
}

RETURNMSG CSWindow::ON_WM_CREATE(LPCREATESTRUCT lp_cs) {
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_WINDOWPOSCHANGING(LPWINDOWPOS lp_wndps) {
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_CLOSE() {
	DestroyWindow(hwnd);
	return create_default_option(0);
}

RETURNMSG CSWindow::ON_WM_SIZE(UINT resize_type, INT new_width, INT new_height) {
	if (!is_prg) {
		prg_draw_children();
		return create_default_option(0);
	}
	else {
		return create_defproc_option();
	}
}

RETURNMSG CSWindow::ON_WM_DESTROY() {
	PostQuitMessage(0);
	return create_default_option(0);
}

RETURNMSG CSWindow::ON_WM_NCDESTROY() {
	if (has_subclass) {
		RemoveWindowSubclass(hwnd, &defaultSubclassWndProc, subclass_id);
		validate_subclass_id(false); //to make it logical, after NCDESTROY, the whole class should be irrelevant
		  //still do it for completeness
	}
	return create_default_option(0);
}

RETURNMSG CSWindow::ON_WM_KEYDOWN(UINT char_code, UINT flags) {
	if (char_code == VK_ESCAPE) {
		DestroyWindow(hwnd);
		return create_default_option(0);
	}
	else {
		return create_defproc_option();
	}
}

RETURNMSG CSWindow::ON_WM_SIZING(UINT nSide, LPRECT lpRect) {
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_ERASEBKGND(HDC hdc) {
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_PAINT() {
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_COMMAND(WPARAM wParam, LPARAM lParam) {
	if (lParam != (LPARAM)0) { //must be handle to control window
		  //else the 0 would mean HWND_DESKTOP, which makes no sense as a child
		HWND hctrl = (HWND)lParam;
		UINT note_code = HIWORD(wParam);
		UINT ctl_id = LOWORD(wParam);

		switch (note_code) {
		case BN_CLICKED: {
			PCSWindow child = (PCSWindow)GetProp(hctrl, CONNECTED_SUBCLASS_WND);
			if (child != NULL) {
				return ON_PBN_CLICKED(child, ctl_id, note_code, hctrl);
			}
		}
		}

	}
	 
	return create_defproc_option();
}

RETURNMSG CSWindow::ON_WM_NOTIFY(WPARAM wParam, LPARAM lParam) {
	UINT ctl_id = (UINT)wParam;
	
	LPNMHDR lp_nmhdr = (LPNMHDR)lParam;
	HWND hctrl = lp_nmhdr->hwndFrom;
	UINT note_code = lp_nmhdr->code;
	UINT octl_id = lp_nmhdr->idFrom; //ctl_id and 'o'fficial_ctl_id should be same
	  //but docs recommend to grab from pointer

	switch (note_code) {
	case UDN_DELTAPOS: {
		PCSWindow child = (PCSWindow)GetProp(hctrl, CONNECTED_SUBCLASS_WND);
		if (child != NULL) { //can be null in cases where a class is designed to have multiple hwnds
			  //and that only one hwnd gets connected with the class instance
			LPNMUPDOWN lpnmud = (LPNMUPDOWN)lParam;
			return ON_PUDN_DELTAPOS(child, lpnmud);
		}
	}
	}


	return create_defproc_option(); //docs say ignored except if notifications say otherwise
}

RETURNMSG CSWindow::ON_PBN_CLICKED(PCSWindow child, UINT ctl_id, UINT note_code, HWND h_button) {
	return child->ON_BN_CLICKED(ctl_id, note_code, h_button);
}

RETURNMSG CSWindow::ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button) {
	return create_defproc_option(); //return value irrelevant
}

RETURNMSG CSWindow::ON_PUDN_DELTAPOS(PCSWindow child, LPNMUPDOWN lp_nmupdown) {
	return child->ON_UDN_DELTAPOS(lp_nmupdown);
}

RETURNMSG CSWindow::ON_UDN_DELTAPOS(LPNMUPDOWN lp_nmupdown) {
	return create_default_option(0); //return 0 to allow any changes
}


LRESULT CALLBACK defaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE: {
		create_prop_from_cswindow_holder(hwnd); //should not need to check if CONNECTED_SUBCLASS_WND is null
		  //no way for it to exist before creation
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			LPCREATESTRUCT lp_cs = (LPCREATESTRUCT)lParam;
			RETURNMSG return_msg = connected_wnd->ON_WM_CREATE(lp_cs);
			return proc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK defaultSubclassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	
	switch (uMsg) {	
	case CUSTOM_SUBCLASS_SETUP: { //with custom messages, no big reason to worry about what to return
		  //just do it for consistency
		if (GetProp(hwnd, CONNECTED_SUBCLASS_WND) == NULL) {
			create_prop_from_cswindow_holder(hwnd);
		}
		RETURNMSG return_msg = create_default_option(0); //always returning 0 by choice
		return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
	}			
	case WM_SIZE: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			UINT resize_type = (UINT)wParam;
			INT new_client_width = LOWORD(lParam);
			INT new_client_height = HIWORD(lParam);
			RETURNMSG return_msg = connected_wnd->ON_WM_SIZE(resize_type, new_client_width, new_client_height);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_WINDOWPOSCHANGING: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			LPWINDOWPOS lp_wndps = (LPWINDOWPOS)lParam;
			RETURNMSG return_msg = connected_wnd->ON_WM_WINDOWPOSCHANGING(lp_wndps);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_CLOSE: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_CLOSE();
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_DESTROY: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_DESTROY();
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_NCDESTROY: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_NCDESTROY();
			RemoveProp(hwnd, CONNECTED_SUBCLASS_WND);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
					   
	case WM_KEYDOWN: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		UINT char_code = (UINT)wParam;
		UINT flags = (UINT)lParam;
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_KEYDOWN(char_code, flags);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_SIZING: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		UINT nSide = (UINT)wParam;
		LPRECT lpRect = (LPRECT)lParam;
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_SIZING(nSide, lpRect);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_ERASEBKGND: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		HDC hdc = (HDC)wParam;
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_ERASEBKGND(hdc);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_PAINT: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_PAINT();
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
		break;
	}
	case WM_COMMAND: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_COMMAND(wParam, lParam);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		} 
	}
	case WM_NOTIFY: {
		PCSWindow connected_wnd = (PCSWindow)GetProp(hwnd, CONNECTED_SUBCLASS_WND);
		if (connected_wnd != NULL) {
			RETURNMSG return_msg = connected_wnd->ON_WM_NOTIFY(wParam, lParam);
			return subproc_return(hwnd, uMsg, wParam, lParam, return_msg);
		}
	}

	}
	

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);  //return value for unprocesssed messages
}