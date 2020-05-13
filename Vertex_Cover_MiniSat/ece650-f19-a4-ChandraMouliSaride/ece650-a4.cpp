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
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>

//Class for storing graph information
class UndirectedGraph{
private:
    std::map<int,std::set<int>>neighbours_map;
public:
    void add_edge(int src,int dst){
        neighbours_map[src].insert(dst);
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




//main method with logic to parse code and call relevant functions
int main(int argc, char** argv) {


    unsigned lower_limit=0;
    unsigned upper_limit=0;
    UndirectedGraph ugraph;
    std::set<int> vertices;
    std::vector<int> vertex_cover;
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
        if(cmd!='V' && cmd!='E'){
        	std::cerr << "Error: Command not recognised\n";
        }
        //logic for taking vertices count for setting upper limit of vertices
        if(cmd=='V'){
        	vertices.clear();
        	vertex_cover.clear();
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
        	std::istringstream edge_input(line);
        	bool err_flag=false;
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
                	err_flag=true;
                	break;
            		}
            		char comma_chk;
            		edge_input>>comma_chk;
            		//checking comma seperator between two edges
            		if(edge_input.fail() ||comma_chk!=','){
            			std::cerr << "Error: Format of edges specified is wrong \n";
            			ugraph.clear_edges();
            			err_flag=true;
                		break;
            		}
            		edge_input>>dst;
            		//throwing error if no number is specified after comma
            		if (edge_input.fail()) {
                	std::cerr << "Error: parsing number\n";
                	ugraph.clear_edges();
                	err_flag=true;
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
            			err_flag=true;
                		break;
            		}
            	}
	        }
	        //storing n and k for usage in clauses as per professor's reduction
	        int n=upper_limit+1;
	        int k=n;
	        if(!err_flag){
		        while(k>0){
		        	//Used Proffersor ece650-minisat example for our cause
		        	std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
		    		Minisat::Lit arr[n][k];
		    		//loop for creating all literals for clauses
		    		for(int i=0;i<n;i++){
		    			for(int j=0;j<k;j++){
		    				arr[i][j]= Minisat::mkLit(solver->newVar());
		    			}
		    		}
		    		//temporary list just for adding elements for a individual clause  
		            Minisat::vec<Minisat::Lit>lit_list;

		            //clause 1 as per professor's encoding
		            for(int i=0;i<k;i++){
		            	lit_list.clear();
		            	for(int j=0;j<n;j++){
		            		lit_list.push(arr[j][i]);
		            	}
		            	solver->addClause(lit_list);
		            }
		            lit_list.clear();
		            //clause as per professor's encoding
		            for(int m=0;m<n;m++){
		            	for(int q=0;q<k;q++){
		            		for(int p=0;p<k&&p<q;p++){
		            			lit_list.push(~arr[m][p]);
		            			lit_list.push(~arr[m][q]);
		            			solver->addClause(lit_list);
		            			lit_list.clear();
		            		}
		            	}
		            }

		            //clause 3 as per professor's encoding
		            for(int m=0;m<k;m++){
		            	for(int q=0;q<n;q++){
		            		for(int p=0;p<n&&p<q;p++){
		            			lit_list.push(~arr[p][m]);
		            			lit_list.push(~arr[q][m]);
		            			solver->addClause(lit_list);
		            			lit_list.clear();
		            		}
		            	}
		            }
		            lit_list.clear();
		            //getting map with vertex and it's connectec elements
		            std::map<int,std::set<int>>neighbours_map=ugraph.get_edges();
		            //clause 4 as per professor's encoding
		            for (auto const& key :neighbours_map) {
		    			int i=key.first;
		    			for(int j:ugraph.get_neighbours(i)){
		    				for(int z=0;z<k;z++){
		    					lit_list.push(arr[i][z]);
		            			lit_list.push(arr[j][z]);
		    				}
		    				solver->addClause(lit_list);
		    				lit_list.clear();
		    			}
		  			}


		  			bool res = solver->solve();
		  			//checking for res and adding elements for finding minimal vertex cover
		            if(res){
		            vertex_cover.clear();
		            for(int i=0;i<n;i++){
		    			for(int j=0;j<k;j++){
		    				if(!Minisat::toInt(solver->modelValue(arr[i][j]))){
		    					vertex_cover.push_back(i);
		    				}
		    			}
		    		}
		    		k--;
		    		}
		    		else{
		    			break;
		    		}
		    	}
		    	//display of vertex cover
		    	if(vertex_cover.size()>0){
	    		//obtained from internet stack overflow
	    		std::sort(vertex_cover.begin(), vertex_cover.end(), std::less<int>());
	    		unsigned int chk=0;
	    		for(int i:vertex_cover){
	    			std::cout<<i;
	    			chk++;
	        		if (chk < vertex_cover.size()) std::cout << " ";
	    		}
	    		std::cout<<std::endl;
	    		}
	    	}
        }

  
    }
   	
    return 0;	
}


