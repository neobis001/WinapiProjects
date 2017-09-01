//for the CSWindow, will not implement normal WNDCLASS constructor because it's superseded
//by WNDCLASSEX (also, it's a lot of work)

//something to note: returning default values may sound like bypassing a subclass chain
//but with this class design, will only want one custom wnd proc. and/or subclass proc.
//so the only thing that could be bypassed is the default procedure
//which is okay

//wnd. proc and subclass proc. design is made to minimize usage of wnd. proc
//will only be used to process messages that can't be handled by a subclass proc.
//like pre-creation messages

//only WNDEXOPS has the class name option, CSWNDCLASSEX doesn't have it.
//only meant to be used in a constructor w/ WNDEXOPS (since if they're both used together,
//that implies that both structs are meant for the same class name
//and I'm seeing WNDEXOPS being used more frequently

#pragma once
#include "custom_window_types.h"
#include "general_window_tools.h"

#include <Windows.h>

#define CUSTOM_SUBCLASS_SETUP 5000 //a message sent to a subclass procedure to link it to
//a given CSWindow, lParam and wParam are 0, and should return 0
#define CONNECTED_SUBCLASS_WND TEXT("CONNECTED_SUBCLASS") //the name of the property that should
//hold a pointer to a CSWindow that should be linked to a given window/subclass proc.

#define STARTING_SUBCLASS_ID 6000 //the id of the subclass that'll be registered with the first
//CSWindow (arbitrary number), each subsequent id is one more than the previous one
#define NULL_SUBCLASS_ID 0 //an id to represent that CSWindow doesn't have a subclass
//though every CSWindow should have a subclass until its destruction

#define DEFAULT_PROC_OPTION 7000 //an option indicating that a message handler proc. should pass
//message processing to the next proc. in the proc. chain (w/ CSWindow, that's either going to be
//a custom subclass proc. going to a custom window proc. 
//or a custom window proc. going to a default proc.
#define DEFAULT_RETURN_OPTION 7001 //an option indicating that a message handler proc. should return
//a value (whether one suggested by docs or by own choice) instead of passing it to next proc.

#define NULL_PLACING_OPTION 8000 //setting for both self placing_option and child placing_option
#define GRID_PLACING_OPTION 8001
#define PACK_PLACING_OPTION 8002
#define CUSTOM_PLACING_OPTION 8003

class CSWindow;
typedef CSWindow* PCSWindow;

typedef struct Return_Template {
	int return_option;
	LRESULT option_value;
} RETURNMSG;

typedef struct CSWNDCLASSEX_Template { //removed wnd. proc and class name
	UINT        style;
	int         cbClsExtra;
	int         cbWndExtra;
	HINSTANCE   hInstance;
	HICON       hIcon;
	HCURSOR     hCursor;
	HBRUSH      hbrBackground;
	DYNCSTR      lpszMenuName;
	HICON       hIconSm;
} CSWNDCLASSEX;

typedef struct CSWNDEXOPS_Template { //removed hWndParent
	DWORD dwExStyle;
	DYNCSTR lpClassName;
	DYNCSTR lpWindowName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeight;
	HMENU hMenu;
	HINSTANCE hInstance;
	LPVOID lpParam;
} CSWNDEXOPS;

enum SIDE {
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	NULL_SIDE //used for default value
};

typedef struct CSRECT_Template {
	int width;
	int height;
} CSRECT;

typedef struct CSPOINT_Template {
	int x;
	int y;
} CSPOINT;

typedef struct CSLOCRECT_Template {
	CSPOINT loc;
	CSRECT dim;
} CSLOCRECT;

//given a custom wndclassex, return the more complete wndclassex with
//a given class name and wndow proc. attached
//used mainly to attach procedures to windows in CSWindow
WNDCLASSEX convert_custom_wndclassex(CSWNDCLASSEX cstm_wndclass, DYNCSTR lpszClassName, WNDPROC lpfnWndProc);

//create a RETURNMSG that will signal to a window or subclass procedure to
//return a default value based on a given message
RETURNMSG create_default_option(LRESULT option_value);

//create a RETURNMSG that will signal to a window/subclass proc. to 
//defer the message to a default procedure
RETURNMSG create_defproc_option();

//set the gloal cswindow_holder variable that'll
//be linked with a window/subclass procedure when an hwnd is linked to a
//window/subclass proc.
//for the subclass proc, need to also explicity send CUSTOM_SUBCLASS_SETUP message
void set_cswindow_holder(PCSWindow cswindow);

//reset global cswindow_holder variable to a nullptr
void reset_cswindow_holder();

//creates a property called CONNECTED_SUBCLASS_WND that links a
//hwnd to the CSWindow pointer stored in the global cswindow var
//skips if either the global var is null or the prop already exists for that handle
void create_prop_from_cswindow_holder(HWND hwnd);

class CSWindow {
private:
	//show a message box saying that a window class wasn't registered
	//successfully, preventing an hwnd from being made if one wasn't passed
	void show_register_quit();
	
	//show a message box saying that an hwnd couldn't be made when trying
	//to make it
	void show_window_creation_quit();

	//using a reference to another CSWindow, uniquely copy all the member variables
	void copy_window_data(const CSWindow& other);

	//set a window subclass with a unique id for reference later
	//(mainly for removing window subclass on class destruction)
	void set_window_subclass();

	//send a CUSTOM_SUBCLASS_SETUP message to set a property
	//in a proc. to a pointer of this instance
	void setup_subclass_connection();

	//if has_subclass is true, give either a unique subclass id and set a subclass flag to true
	//else set flag to false and set id to NULL_SUBCLASS_ID
	void validate_subclass_id(bool has_subclass);

	//attach a subclass proc. that's linked to a pointer of this instance
	//and set the has_subclass flag to true
	void attach_subclass();

	HWND get_hwnd_parent();

	//attempt to create an hwnd for this class instance w/ stored window options and parent considered, 
	//force quit on failure (this only connects a window proc., not a subclass proc.)
	void attempt_hwnd_creation();

	//set the parent class instance, attempt to create the main hwnd
	//and attach a subclass proc. afterwards
	void create_linked_hwnd(PCSWindow parent);

	int get_total_weights(int* weight_list, int weight_list_size);

	//given a window dimension, a row/column number, and weight data
	//return corresponding pixel 
	int get_weighted_space(LONG window_dimension, int dim_coord,
		int span, pint weight_list, int weight_list_size);

	int get_padded_space(int allocated_dimension, int padding);

	int get_padded_coord(int allocated_dimension, int target_dim_coord, int padding);

	//check for legitimate coordinate and span inputs in addition to standard window
	//management checks
	void check_okay_gridding(int row_number, int col_number, int rowspan, int colspan);

	//checks if both grid coords and spanning options fit within parent's grid settings
	bool has_okay_grid_coords(int row_number, int col_number, int rowspan, int colspan);

	//check if both row and column number are in range of specified grid coords.
	bool pure_grid_coords_are_valid(int row_number, int col_number);

	//checks if row/colunn requests are at least 1 and that, combined with the rowspan/columnspan
	//would fit into the parent's gridding space (assuming parent exists
	bool grid_spanning_is_valid(int row_number, int col_number, int rowspan, int colspan);

	//initialize a list of 1 int weights to signify that columns/rows are to be
	//given equal spacing
	void initialize_gridding_weights(pint& weight_list, int num_weight_entries);
	
	//focus on a dim_coord that's either a row or column number and with the corr. window data
	//return the corr. pixel
	int get_weighted_grid_coord(LONG window_dim, int dim_coord, pint weight_list, int weight_list_size);

	//tests whether a row_number/col_number is a valid one and whether the weight
	//input is valid too
	bool weight_changes_are_valid(int dim_coord, int dim_size, int weight);

	//given a child pointer (made w/ new), append it to the children list
	//Rule of Three doesn't apply (working w/ pointers)
	//set_parent is --not-- called for the child 
	//(this function not meant to be called publicly)
	void add_child(PCSWindow child);

	//extend a CSWNDCLASSEX to a complete WNDCLASSEX and attempt to register class
	//force quit with message on failure
	void attempt_window_registration(CSWNDCLASSEX cswndclass, DYNCSTR lpszClassName, WNDPROC lpfnWndProc);

	//returns false if parent's children_placing_option is the null_option or
	//a different one from one passed in
	bool requests_dif_wnd_manager(int placing_option);

	//sets this and parent's placing modes to same placing_option
	void connect_placing_modes(int PLACING_OPTION);

	//check for existing parent and legal packing request
	void check_okay_packing();

	//recursively search parents until a parent doesn't have its own parent
	//and call for drawing there
	void topmost_draw();

	void init_grid_values(int row_number, int col_number, int padx, int pady, int rowspan, int columnspan);

	void init_pack_values(SIDE side, bool expand, int pad_x, int pad_y);

	void do_standard_manage_checks(int parent_manager_request);

	void resize_via_packing_cavity();

	void store_wops_data(CSWNDEXOPS wops);

	void prepare_stored_wops(CSWNDEXOPS wops, PCSWindow parent);

	//File: custom_window_packer.cpp
	void set_packing_cavity();

protected:
	static unsigned short int available_subclass_id; //initialized separately
	int subclass_id = NULL_SUBCLASS_ID;
	bool has_subclass = false;
	int num_children = 0;
	PCSWindow* children = nullptr;
	PCSWindow parent = nullptr;
	HWND hwnd = (HWND)NULL;
	CSWNDEXOPS stored_wops = create_zeroed_struct<CSWNDEXOPS>();
	int num_hwnds = 1;

	int base_width = 0;
	int base_height = 0;
	int pad_x = 0;
	int pad_y = 0;

	//gridding properties
	int placing_option = NULL_PLACING_OPTION;
	int children_placing_option = NULL_PLACING_OPTION;
	int row_number = 0;
	int col_number = 0;
	int num_rows = 0;
	int num_cols = 0;
	int columnspan = 0;
	int rowspan = 0;
	pint row_weights = nullptr; //a list of ints mapping each row with a weight value
	//divide an entry by the sum of weights to get the relative influence/size of the row
	pint col_weights = nullptr; //like row weights, but for columns

	//packing properties
	int pack_min_cavity_width = 0;
	int pack_min_cavity_height = 0;
	bool resize_on_pack = true;
	SIDE side = TOP; //could do NULL_SIDE, but using a working default for now
	bool expand = false;

	virtual void init(CSWNDCLASSEX cswndclass, CSWNDEXOPS wops, PCSWindow parent);

	virtual void init(CSWNDEXOPS wops, PCSWindow parent);

	void set_base_dimensions(int base_width, int base_height);

	int get_deep_num_children();

	int get_num_child_hwnds();

	bool is_prg = false;

	void prg_draw_as_grid(HDWP* hdwp);

	void prg_draw(HDWP* receiving_hdwp);

	void prg_draw_self(HDWP* hdwp);

	void prg_toggle_isprg(bool turn_on);

	virtual void prg_draw_children();

	void prg_toggle_children_prgs(bool turn_on);

	virtual void prg_draw_as_custom(HDWP* hdwp);

	virtual void add_wops_defaults();

	//File: custom_window_packer.cpp
	void prg_draw_as_pack(HDWP* hdwp);

public:
	CSWindow(); //default constructor does nothing, for list purposes

	CSWindow(CSWNDCLASSEX cswndclass, CSWNDEXOPS wops, PCSWindow parent);

	CSWindow(CSWNDEXOPS wops, PCSWindow parent);

	CSWindow(const CSWindow& other); //when working on copy/assignment constructors
	  //make sure to explicitly transfer all data

	CSWindow& operator= (const CSWindow& other);

	~CSWindow();

	int get_placing_option();

	int get_base_width();

	int get_base_height();

	bool get_packing_expand();

	SIDE get_packing_side();

	LONG get_client_x();

	LONG get_client_y();

	LONG get_window_y();

	LONG get_window_x();

	LONG get_client_width();

	LONG get_client_height();
	
	LONG get_window_width();

	LONG get_window_height();

	HWND get_hwnd() const;

	int get_num_children() const;

	bool get_has_subclass() const;

	int get_subclass_id() const;

	PCSWindow get_parent();

	//return a dynamically allocated copy of only the pointers held
	//by the children var.
	PCSWindow* get_children_copy() const;

	//delete the list of pointers to children and nullptr it
	void clear_children_list();

	void delete_children();

	void grid(int row_number, int col_number, int padx, int pady, int rowspan, int columnspan);

	virtual void pack(SIDE side, bool expand, int padx, int pady);

	void set_gridding_space(int num_rows, int num_cols);

	void weight_grid_row(int row_num, int weight);

	void weight_grid_col(int col_num, int weight);

	void set_parent(PCSWindow parent);

	virtual void show(int nCmdShow);

	virtual void update();

	virtual void set_window_pos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
		UINT uFlags);

	void visual_redraw();

	CSLOCRECT get_padded_rect_info(CSLOCRECT target_lrect);

	virtual void prg_defer_window_pos(HDWP* hdwp, HWND hWndInsertAfter, int X, int Y, int cx, int cy,
		UINT uFlags);

	void request_custom_placing();

	//big note: hwnd_in_creation provides access to the hwnd passed in to the message loop
	//on creation
	//At this time, in the WM_CREATE message, the instance member hwnd variable is still 0
	//It will become the hwnd_in_creation after the CreateWindowEx function in
	//CSWindow::attempt_hwnd_creation finishes
	//So if want to access hwnd now (note a handle is just like a pointer), need hwnd_in_creation
	virtual RETURNMSG ON_WM_CREATE(LPCREATESTRUCT lp_cs, HWND hwnd_in_creation);

	virtual RETURNMSG ON_WM_WINDOWPOSCHANGING(LPWINDOWPOS lp_wndps);

	virtual RETURNMSG ON_WM_SIZE(UINT resize_type, INT new_width, INT new_height); 

	virtual RETURNMSG ON_WM_CLOSE();

	virtual RETURNMSG ON_WM_DESTROY();

	virtual RETURNMSG ON_WM_NCDESTROY();

	virtual RETURNMSG ON_WM_KEYDOWN(UINT char_code, UINT flags);

	virtual RETURNMSG ON_WM_SIZING(UINT nSide, LPRECT lpRect);

	virtual RETURNMSG ON_WM_ERASEBKGND(HDC hdc);

	virtual RETURNMSG ON_WM_PAINT();

	virtual RETURNMSG ON_WM_COMMAND(WPARAM wParam, LPARAM lParam);

	virtual RETURNMSG ON_WM_NOTIFY(WPARAM wParam, LPARAM lParam);

	virtual RETURNMSG ON_PBN_CLICKED(PCSWindow child, UINT ctl_id, UINT note_code, HWND h_button);

	virtual RETURNMSG ON_BN_CLICKED(UINT ctl_id, UINT note_code, HWND h_button);

	virtual RETURNMSG ON_PUDN_DELTAPOS(PCSWindow child, LPNMUPDOWN lp_nmupdown);

	virtual RETURNMSG ON_UDN_DELTAPOS(LPNMUPDOWN lp_nmupdown);

	//NOTE: WM_TIMER also has an lParam that is a pointer to a special function
	//could consider adding that in later, but for now, just focus on ID
	virtual RETURNMSG ON_WM_TIMER(UINT_PTR nIDEvent);

};

LRESULT subproc_return(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, RETURNMSG return_msg);
LRESULT proc_return(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, RETURNMSG return_msg);

LRESULT CALLBACK defaultWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK defaultSubclassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData);



