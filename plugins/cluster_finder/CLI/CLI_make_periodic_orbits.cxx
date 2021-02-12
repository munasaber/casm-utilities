#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include <CLI/CLI.hpp>
#include <CLI/App.hpp>
#include <casm/casm_io/json/jsonParser.hh>
#include <casm/casm_io/json/jsonFile.hh>
#include <casmutils/definitions.hpp>
#include <casmutils/xtal/structure.hpp>
#include <casmutils/xtal/symmetry.hpp>
#include <casmutils/sym/cartesian.hpp>
#include <casmutils/xtal/site.hpp>
#include <casmutils/xtal/structure_tools.hpp>
#include <casmutils/clusterography/cluster_finder.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <../tests/autotools.hh>

using json= nlohmann::json;

void test_json(casmutils::fs::path& jsonpath)
{
	std::ifstream ifs(jsonpath);
	json j = json::parse(ifs);
	std::cout<<j.at("prim");
	std::cout<<j.at("shift_points");
	std::cout<<j.at("execute");
}
std::vector<int> get_orbit_labels(casmutils::fs::path& jsonpath)
{
	std::ifstream ifs(jsonpath);
	json j = json::parse(ifs);
	return j.at("orbit_labels");
}

std::vector<double> get_max_lengths(casmutils::fs::path& jsonpath)
{
	std::ifstream ifs(jsonpath);
	json j = json::parse(ifs);
	return j.at("max_lengths");
}
/*
std::vector<Eigen::Vector3d> get_filtered_sites(casmutils::fs::path& jsonpath)
{
	std::ifstream ifs(jsonpath);
	json j = json::parse(ifs);
	return j.at("site_filter");
}
*/
/*
void test_json(casmutils::fs::path& jsonpath)
{
	//std::ifstream ifs(jsonpath);
	//json j = json::parse(ifs);
	json j = json::parse(jsonpath);
	std::vector<int> orbit_labels;
	std::vector<double> max_lengths;
	//for (json::iterator it = j.begin(); it != j.end(); ++it) {
 	//	 std::cout << *it << '\n';
	//}
	for (auto &my_orbits: j["orbit_branch_specs"])
	{
		orbit_labels=my_orbits["orbit_labels"];
		max_lengths=my_orbits["max_lengths"];
	}
	//json jstring["My test input"];
	//jstring["

}
*/
/*
void read_json_file(casmutils::fs::path& jsonpath)
{
	//create jsonParser object from json path?
	CASM::jsonParser json(jsonpath);

	
	//get data from json for the first layer which includes getting the orbits that will be examined
	CASM::jsonParser parse_orbits;
	std::vector<std::string> orbit_labels;
	//CASM::jsonParser Orbit_branch_specs=CASM::jsonParser::get_if(orbit_labels, "orbit_branch_specs",  parse_orbits);
	json.get_if(orbit_labels, "orbit_branch_specs",  parse_orbits);

	//for each orbit get the max_length
	std::vector<double>& maxlengths;
	parse_orbits=CASM::jsonParser::get_if(&double, "max_length", maxlengths);

	//check if first argument in parse, orbits is (1?), else return error?
	if(parse_orbits[0]=="1")
	{
		std::cerr<<"error, orbits must start with "1".\n";
		return EXIT_FAILURE;
	}

	//get filter to remove unwanted sites if applicable
	std::vector<xtal::Site>& filtered_sites;
	CASM::jsonParser Site_filter=CASM::jsonParser::get_if(&xtal::Site, "Site_Filter", filtered_sites);

	//get string for whether proper string was called. Not sure if neccessary or correct
	std::string Orbit_branch_specs_string= from_json(Orbit_branch_specs);
	
	//get the maxlengths from the parsed json data
	maxlengths= from_json(parse_orbits);

	//get the vector of filtered sites from the parsed json data
	filtered_sites= from_json(Site_filter);

	//assuming this works I have no idea what to do with it now...
 
}
*/

//Convert json to data and manipulate using the cluster_finder functions

std::vector<casmutils::cluster::Orbit> make_orbits(casmutils::fs::path& jsonpath, casmutils::fs::path& structurepath)
{
	
	//orbit_labels=get_orbit_labels(jsonpath);
	//Get maxlength from json
	const std::vector<double> maxlengths=get_max_lengths(jsonpath);

	//get structure pointer from structure path
	
	casmutils::xtal::Structure structure= casmutils::xtal::Structure::from_poscar(structurepath);
	
        //std::unique_ptr<casmutils::xtal::Structure> struc_ptr;
        //struc_ptr.reset(new casmutils::xtal::Structure(casmutils::xtal::Structure::from_poscar(structurepath)));
	//get left out clusters from user input
	
	//make the orbits from the cluster finder app
	//std::vector<casmutils::cluster::Orbit> total_orbits= casmutils::cluster::make_periodic_orbits(maxlengths, structure);
	std::vector<casmutils::cluster::Orbit> total_orbits;
	return total_orbits;
	
};



int main(int argc, char* argv[]) { 
	
	//Provides app description when the -h tag is applied
	CLI::App app{"This allows one to find cluster coordinates and indicies of a particular structure based on cutoff radius"};
	
	//Tag for the user input for the structures path
        casmutils::fs::path structurepath;
	CLI::Option* structure_path=app.add_option("-s, --structure", structurepath, "Please input the file path of the base structure")-> required();
        
	//Tag for maximum cutoff radius for sites to be included into the cluster
	std::vector<double>  maxlengths;
	app.add_option("-m, --max_length", maxlengths, "Select the maximum cutoff radius for sites to be included in a given cluster. Higher order clusters must either have a smaller or same size radius as lower order clusters.");
	
	//Tag for the user input of .json file path
	casmutils::fs::path jsonpath;
	app.add_option("-j, --json_path", jsonpath, "Please input the path to the input json file");
		
	//Tag for the path to the output file (This is not required so otherwise the file will be output to the screen)
	casmutils::fs::path outputpath;
	app.add_option("-o, --output", outputpath, "Please input the path to the output file. If this option is not used, the cluster data will be printed to the screen"); 

	CLI11_PARSE(app, argc, argv);
	
	
	
	
	test_json(jsonpath);
	return 0; 
}
