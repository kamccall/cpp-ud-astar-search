#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;
using std::cin;
using std::cout;
using std::string;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

// kmccall: helper function to ensure user input are valid floats
bool checkFloatInput(string userInput)
{
    // will fail below test (and return false) if anything other than integer or decimal is found (fragment found on //stackoverflow)
    return (userInput.find_first_not_of("1234567890.") == string::npos);
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    //Complete this TODO to satisfy Project Rubric Criterias of User Input
  
    // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get user input for these values using std::cin
    // Pass the user input to the RoutePlanner object below in place of 10, 10, 90, 90.
    string x1_input, y1_input, x2_input, y2_input;
    float start_x, start_y, end_x, end_y; 
  
    // get value of start_x
    cout << "enter value for start_x: ";
    cin >> x1_input;

    // error check start_x input
    while (!checkFloatInput(x1_input))
    {
        cout << "re-enter valid value for start_x: ";
        cin >> x1_input;
    }

    start_x = atof(x1_input.c_str());    // (fragment found on stackoverflow)

    // get value of start_y
    cout << "enter value for start_y: ";
    cin >> y1_input;

    // error check start_y input
    while (!checkFloatInput(y1_input))
    {
        cout << "re-enter valid value for start_y: ";
        cin >> y1_input;
    }

    start_y = atof(y1_input.c_str());

    // get value of end_x
    cout << "enter value for end_x: ";
    cin >> x2_input;

    // error check end_x input
    while (!checkFloatInput(x2_input))
    {
        cout << "re-enter valid value for end_x: ";
        cin >> x2_input;
    }

    end_x = atof(x2_input.c_str());

    // get value of end_y
    cout << "enter value for end_y: ";
    cin >> y2_input;

    // error check end_y input
    while (!checkFloatInput(y2_input))
    {
        cout << "re-enter valid value for end_y: ";
        cin >> y2_input;
    }

    end_y = atof(y2_input.c_str());

    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    // RoutePlanner route_planner{model, 10, 10, 90, 90};
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
