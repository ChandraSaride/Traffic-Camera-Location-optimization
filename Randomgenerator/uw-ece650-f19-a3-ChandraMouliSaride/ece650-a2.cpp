// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
//libraries used for running this program
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include<set>
#include<queue>
#include <map>
#include <iterator>
#include <algorithm>

//Class for storing graph information
class UndirectedGraph{
private:
    std::map<int,std::set<int>>neighbours_map;
public:
    void add_edge(int src,int dst){
        neighbours_map[src].insert(dst);
        neighbours_map[dst].insert(src);
    }
    std::map<int,std::set<int>> get_edges(){
    	return neighbours_map;
    }
    std::set<int>&  get_neighbours(int src){
        return neighbours_map[src];
    }
    void clear_edges(){
    	neighbours_map.clear();
    }
};

//Implementation of finding shortest path logic is based on resources from internet , modified for our requirement
//function for generating map with node as key and neighbours to  source  as value and then finding shortest path from the map of neighbours
std::vector<int> get_shortpath(UndirectedGraph& ugraph, int src, int dst){
    std::queue<int> q;
    //map variable named tree to store neighbours details
    std::map<int, int*> tree={{src,nullptr}};
    q.push(src);
    while(!q.empty()){
        int current_vertex=q.front();
        q.pop();
        int* parent_vertex=new int(current_vertex);
        for(int child_vertex:ugraph.get_neighbours(current_vertex)){
            if(tree.find(child_vertex)==tree.end()){
                tree[child_vertex]=parent_vertex;
                q.push(child_vertex);
            }
        }
    }
    //block with implementation to find shortest path between source neighbours and destination
    int current_vertex=dst;
    int *next_vertex=tree[dst];
    std::vector<int> total_path;
    while (true){
        total_path.push_back(current_vertex);
        next_vertex=tree[current_vertex];
        if(next_vertex==nullptr){
            break;
        }
        current_vertex=*next_vertex;
    }
    std::reverse(total_path.begin(),total_path.end());
    return total_path;
}



//main method with logic to parse code and call relevant functions
int main(int argc, char** argv) {


    unsigned lower_limit=0;
    unsigned upper_limit=0;
    UndirectedGraph ugraph;
    std::set<int> vertices;
    // read from stdin until EOF
    while (!std::cin.eof()) {


        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);

        // we expect each line to contain a list of numbers
        // this vector will store the numbers.
        // they are assumed to be unsigned (i.e., positive)
        std::vector<unsigned> nums;
        char cmd;
        //to detect eof and exit program gracefully
        if(std::cin.eof()){
        	break;
        }
        //to detect empty inputs
        if(line.size()==0){
        	std::cerr << "Error: Please enter appropiate input\n";
        	continue;
        }
        cmd=line.at(0);
        //to detect inputs other than needed ones
        if(cmd!='V' && cmd!='E' && cmd!='s'){
        	std::cerr << "Error: Command not recognised\n";
        }
        //logic for taking vertices count for setting upper limit of vertices
        if(cmd=='V'){
        	std::cout<<line<<std::endl;
        	vertices.clear();
        	std::istringstream vertex_input(line);
        	 // while there are characters in the input line
        	while (!vertex_input.eof()) {
            	unsigned num;
            	char tmp_cmdchk;
            	//sliding through to stream of input for vertex number
            	vertex_input >> tmp_cmdchk;
            	if (vertex_input.fail()) {
               	 	std::cerr << "Error: Parsing Error\n";
                	break;
            	}

            	// if eof bail out
            	if (vertex_input.eof()){
                	std::cerr << "Error: Vertex count not mentioned\n";
                	break;
            	}


                //taking vertex count and storing it in num
	            vertex_input>>num;
	             if (vertex_input.fail()) {
                	std::cerr << "Error: parsing number\n";
                	break;
            		}
            	upper_limit=num-1;

	        }
	        //clearing any previous info
	        ugraph.clear_edges();
        }
        //logic for taking edge inputs and parsing it to store in ugraph
        if(cmd=='E'){
        	std::cout<<line<<std::endl;
        	std::istringstream edge_input(line);
        	 // while there are characters in the input line
        	while (!edge_input.eof()) {
            	char tmp_brkchk;
            	edge_input>>tmp_brkchk;
            	//checking for bracket to extract integers and store in edges of ugraph
            	if(tmp_brkchk=='<'){
            		unsigned src,dst;
            		edge_input>>src;
            		//if integer is not specified
            		if (edge_input.fail()) {
                	std::cerr << "Error: parsing number\n";
                	ugraph.clear_edges();
                	break;
            		}
            		char comma_chk;
            		edge_input>>comma_chk;
            		//checking comma seperator between two edges
            		if(edge_input.fail() ||comma_chk!=','){
            			std::cerr << "Error: Format of edges specified is wrong \n";
            			ugraph.clear_edges();
                		break;
            		}
            		edge_input>>dst;
            		//throwing error if no number is specified after comma
            		if (edge_input.fail()) {
                	std::cerr << "Error: parsing number\n";
                	ugraph.clear_edges();
                	break;
            		}
            		//checking if both edges are with in 0 to n-1
            		if(src<=upper_limit && dst<=upper_limit && src>=lower_limit && dst>=lower_limit ){
            			ugraph.add_edge(src,dst);
            			vertices.insert(src);
            			vertices.insert(dst);
            		}
            		else{
            			std::cerr << "Error: Edge number specified outside scope of number of vertices\n";
            			ugraph.clear_edges();
                		break;
            		}
            	}
	        }

        }
        //logic for parsing the input and finding shortest path between src and dst
        if(cmd=='s'){
        	std::istringstream findpath_input(line);
        	 // while there are characters in the input line
        	while (!findpath_input.eof()) {
        		char tmp_cmdchk;
            	// sliding through  input stream to get integers
            	findpath_input >> tmp_cmdchk;
/*            	if (findpath_input.fail()) {
               	 	std::cerr << "Error: Parsing Error\n";
                	break;
            	}*/
        		unsigned src,dst;
        		findpath_input>>src;
        		//to check if integer is given as src or not
/*        		if (findpath_input.fail()) {
            	std::cerr << "Error: parsing number\n";
            	break;
        		}*/
        		findpath_input>>dst;
        		//to check if integer is given as dst or not
/*        		if (findpath_input.fail()) {
            	std::cerr << "Error: parsing number\n";
            	break;
        		}*/
        		bool src_flag=false;
        		bool dst_flag=false;
        		//checking if both src and dst are available in nodes given already
        		for(unsigned vertex:vertices){
        			if(vertex==src){
        				src_flag=true;
        			}
        			if(vertex==dst){
        				dst_flag=true;
        			}
        		}
        		//throw error if src or dst in not available in E list given
        		if(!src_flag ||!dst_flag){
        			std::cerr << "Error: specified vertex is not available in Edges\n";
            		break;
        		}
        		std::vector<int> total_path;
        		unsigned i=0;
        		if(src!=dst){
        		//calling to get neighbours map for given src
        			total_path=get_shortpath(ugraph,src,dst);
    			
    			//to throw error if path does not exist
    			if(total_path.size()<2){
    				std::cerr << "Error: specified path does not exist\n";
            		break;
    			    }
    		    }
    		    else{
    		    	total_path.push_back(src);
    		    	total_path.push_back(dst);
    		    }
    		    
    			//printing path as per format given
            	for(unsigned x:total_path){
        			std::cout<<x;
        			i++;
        			if (i < total_path.size()) std::cout << "-";
    			}
    			std::cout << std::endl;
    			total_path.clear();
    			break;
	        }
        }

    	
       

    }
   	
    return 0;	
}