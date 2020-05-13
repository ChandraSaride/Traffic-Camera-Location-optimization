// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
//libraries used for running this program
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
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
#include <string.h>
#include <time.h>
#include <fstream>
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

struct timespec  pclock(std::string msg, clockid_t cid) {
    struct timespec ts;
    if (clock_gettime(cid, &ts) == -1) {
	perror("clock_gettime"); exit(0);
    }
    //printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
    return ts;
}
void errp(std::string s, int code) {
    std::cerr<<"Error: "<<s<<"--"<<strerror(code);
}

struct thread_data
{int n;
 int k;
 UndirectedGraph ugraph;
 struct timespec ts;
 int vc_len;
 pthread_mutex_t* mutex;
 int thread_timeout_check;
};
void* cnf_sat_vc(void *thread_arg){
	struct thread_data *my_data;
	my_data=(struct thread_data *)thread_arg;
	int n=my_data->n;
	int k=my_data->k;
	UndirectedGraph ugraph=my_data->ugraph;
	pthread_mutex_t *mutex=my_data->mutex;
    pthread_mutex_lock(mutex);
	std::vector<int> vertex_cover;
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
    std::cout<<"CNF-SAT-VC: ";
	for(int i:vertex_cover){
		std::cout<<i;
		chk++;
		if (chk < vertex_cover.size()) std::cout << ",";
	}
	std::cout<<std::endl;
	}
	my_data->vc_len=vertex_cover.size();
	my_data->thread_timeout_check=1;
	clockid_t cid;
	int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if(retcode) {
	errp("pthread_getcpuclockid", retcode);
    }
    else {
	struct timespec ts=pclock("Thread 1 cpu time: ", cid);
	my_data->ts=ts;
    }
    pthread_mutex_unlock(mutex);
	return nullptr;
}
float calc_time(struct timespec ts){
	float sec=float(ts.tv_sec);
	float nsec=float(ts.tv_nsec);
	float time=sec+(nsec / 1000000000);
	return time*1000000;
}

void* vc_1(void *thread_arg){
	usleep(1000);
	struct thread_data *my_data;
	my_data=(struct thread_data *)thread_arg;
	UndirectedGraph ugraph=my_data->ugraph;
	pthread_mutex_t *mutex=my_data->mutex;
    pthread_mutex_lock(mutex);
	std::vector<int> vertex_cover;
    std::map<int,std::set<int>>neighbours_map=ugraph.get_edges();
 	while(!neighbours_map.empty())
 	{
 		int countinc;
		int maxcount=0;
		int selnode;
 		for (auto const& key :neighbours_map) {
 			countinc=0;
			int i=key.first;
			//for(int j:ugraph.get_neighbours(i)){
			//	countinc++;
			//}
			countinc=ugraph.get_neighbours(i).size();
			if(countinc>maxcount)
			{
				maxcount=countinc;
				selnode=i;

			}
			}
			vertex_cover.push_back(selnode);
			neighbours_map.erase(selnode);
			for (auto const& key :neighbours_map) {
				int i=key.first;
			for(int j:ugraph.get_neighbours(i)){
				if(j==selnode)
				{
					ugraph.get_neighbours(i).erase(j);
					if(ugraph.get_neighbours(i).empty())
					{
						neighbours_map.erase(i);	
					}
				}
			}
		}

		}
    	
	//display of vertex cover
	if(vertex_cover.size()>0){
	//obtained from internet stack overflow
	std::sort(vertex_cover.begin(), vertex_cover.end(), std::less<int>());
	unsigned int chk=0;
	std::cout<<"APPROX-VC-1: ";
	for(int i:vertex_cover){
		std::cout<<i;
		chk++;
		if (chk < vertex_cover.size()) std::cout << ",";
	}
	std::cout<<std::endl;
	}
	my_data->vc_len=vertex_cover.size();
	my_data->thread_timeout_check=1;
	clockid_t cid;
	int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if(retcode) {
	errp("pthread_getcpuclockid", retcode);
    }
    else {
	struct timespec ts=pclock("Thread 2 cpu time: ", cid);
	my_data->ts=ts;
    }
    pthread_mutex_unlock(mutex);
	return nullptr;
}

void* vc_2(void *thread_arg){
	usleep(2000);
	struct thread_data *my_data;
	my_data=(struct thread_data *)thread_arg;
	UndirectedGraph ugraph=my_data->ugraph;
	pthread_mutex_t *mutex=my_data->mutex;
    pthread_mutex_lock(mutex);
	std::vector<int> vertex_cover;
	std::map<int,std::set<int>>neighbours_map=ugraph.get_edges();
	     for (auto const& key :neighbours_map) {
			int i=key.first;
			for(int j:ugraph.get_neighbours(i)){
				if (neighbours_map.find(i) == neighbours_map.end()) {
					continue;
				}
				else{
					neighbours_map.erase(i);
					vertex_cover.push_back(i);
					neighbours_map.erase(j);
					vertex_cover.push_back(j);
					for(auto const& chk_key :neighbours_map){
						int chk_x=chk_key.first;
						for(int chk_val:ugraph.get_neighbours(chk_x)){
							if(chk_val== i || chk_val== j){
								ugraph.get_neighbours(chk_x).erase(chk_val);
							}
						}
					}
					
					}
				}
			}
			
	//display of vertex cover
	if(vertex_cover.size()>0){
	//obtained from internet stack overflow
	std::sort(vertex_cover.begin(), vertex_cover.end(), std::less<int>());
	vertex_cover.erase( unique( vertex_cover.begin(), vertex_cover.end() ), vertex_cover.end() );
	unsigned int chk=0;
	std::cout<<"APPROX-VC-2: ";
	for(int i:vertex_cover){
		std::cout<<i;
		chk++;
		if (chk < vertex_cover.size()) std::cout << ",";
	}
	std::cout<<std::endl;
	}
	my_data->vc_len=vertex_cover.size();
	my_data->thread_timeout_check=1;
	clockid_t cid;
	int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if(retcode) {
	errp("pthread_getcpuclockid", retcode);
    }
    else {
	struct timespec ts=pclock("Thread 3 cpu time: ", cid);
	my_data->ts=ts;
    }
    pthread_mutex_unlock(mutex);
	return nullptr;
}


//main method with logic to parse code and call relevant functions
int main(int argc, char** argv) {


    unsigned lower_limit=0;
    unsigned upper_limit=0;
    UndirectedGraph ugraph;
    std::set<int> vertices;
    std::vector<int> vertex_cover;
    std::map<int,std::list<float>>cnf_time_map;
    std::map<int,std::list<float>>vc1_time_map;
    std::map<int,std::list<float>>vc2_time_map;
    std::map<int,std::list<float>>vc1_app_map;
    std::map<int,std::list<float>>vc2_app_map;
    while (!std::cin.eof()) 
    {

    	
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
	        	UndirectedGraph test1_graph(ugraph);
        		pthread_t cnf_vc,vc_1_thread,vc_2_thread;
        		bool cnf_check=true;
        		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
				struct thread_data td1;
				td1.n=n;
				td1.k=k;
				td1.ugraph=test1_graph;
				td1.mutex=&mutex;
				td1.thread_timeout_check=0;
				
				pthread_create(&cnf_vc,NULL,cnf_sat_vc,(void *)&td1);
				for(int t=0;t<12000;t++){
					usleep(10000);
					if(td1.thread_timeout_check!=1){
						cnf_check=false;
					}
					else if(td1.thread_timeout_check==1){
						cnf_check=true;
						break;
					}	
				}
				
				if(!cnf_check){
		    	pthread_mutex_unlock(&mutex);
		    	pthread_cancel(cnf_vc);
		    	std::cout<<"CNF-SAT-VC: timeout in the output"<<std::endl;
			    }
			    	
				UndirectedGraph test2_graph(ugraph);
				struct thread_data td2;
				td2.n=n;
				td2.k=k;
				td2.ugraph=test2_graph;
				td2.mutex=&mutex;
				pthread_create(&vc_1_thread,NULL,vc_1,(void *)&td2);
				
				UndirectedGraph test3_graph(ugraph);
				struct thread_data td3;
				td3.n=n;
				td3.k=k;
				td3.ugraph=test3_graph;
				td3.mutex=&mutex;
				pthread_create(&vc_2_thread,NULL,vc_2,(void *)&td3);


				if(cnf_check)pthread_join(cnf_vc,NULL);
				pthread_join(vc_1_thread,NULL);
				pthread_join(vc_2_thread,NULL);
				pthread_mutex_destroy(&mutex);
				//time
				float time;
				if(cnf_check){
				time=calc_time(td1.ts);
				cnf_time_map[n].push_back(time);
				}
				time=calc_time(td2.ts);
				vc1_time_map[n].push_back(time);
				time=calc_time(td3.ts);
				vc2_time_map[n].push_back(time);
				//app ratio
				if(cnf_check){
				float max_len=td1.vc_len;
				vc1_app_map[n].push_back(float(td2.vc_len)/max_len);
				vc2_app_map[n].push_back(float(td3.vc_len)/max_len);
				}
	    	}
        }
    

  
    }
/*	for (auto const& key :vc1_app_map) {
		int i=key.first;
	for(float j:vc1_app_map[i]){
		std::cout<<j<<std::endl;
	}
	}*/
	std::ofstream cnf_time_file;
  	cnf_time_file.open ("cnf_time_file.dat");
  	cnf_time_file<<"# X Y Yerr\n";
	for (auto const& key :cnf_time_map) {
		int i=key.first;
		float mean=0;
		float standardDeviation=0;
		for(float j:cnf_time_map[i]){
		mean=mean+j;
		}
		float len_list=cnf_time_map[i].size();
		mean=(float)mean/len_list;
		for(float j:cnf_time_map[i])
        standardDeviation += pow(j-mean, 2);
    	standardDeviation=sqrt(standardDeviation /len_list);
    	cnf_time_file<<i<<"  "<<mean<<"  "<<standardDeviation<<"\n";
	}
	cnf_time_file.close();
	/*FILE * stream;
	stream = popen("gnuplot graphdisp.gp", "r");
	pclose(stream);*/

	std::ofstream vc1_time_file;
  	vc1_time_file.open ("vc1_time_file.dat");
  	vc1_time_file<<"# X Y Yerr\n";
	for (auto const& key :vc1_time_map) {
		int i=key.first;
		float mean=0;
		float standardDeviation=0;
		for(float j:vc1_time_map[i]){
		mean=mean+j;
		}
		float len_list=vc1_time_map[i].size();
		mean=(float)mean/len_list;
		for(float j:vc1_time_map[i])
        standardDeviation += pow(j-mean, 2);
    	standardDeviation=sqrt(standardDeviation /len_list);
    	vc1_time_file<<i<<"  "<<mean<<"  "<<standardDeviation<<"\n";
	}
	vc1_time_file.close();


	std::ofstream vc2_time_file;
  	vc2_time_file.open ("vc2_time_file.dat");
  	vc2_time_file<<"# X Y Yerr\n";
	for (auto const& key :vc2_time_map) {
		int i=key.first;
		float mean=0;
		float standardDeviation=0;
		for(float j:vc2_time_map[i]){
		mean=mean+j;
		}
		float len_list=vc2_time_map[i].size();
		mean=(float)mean/len_list;
		for(float j:vc2_time_map[i])
        standardDeviation += pow(j-mean, 2);
    	standardDeviation=sqrt(standardDeviation /len_list);
    	vc2_time_file<<i<<"  "<<mean<<"  "<<standardDeviation<<"\n";
	}
	vc2_time_file.close();


	std::ofstream vc1_app_file;
  	vc1_app_file.open ("vc1_app_file.dat");
  	vc1_app_file<<"# X Y Yerr\n";
	for (auto const& key :vc1_app_map) {
		int i=key.first;
		float mean=0;
		float standardDeviation=0;
		for(float j:vc1_app_map[i]){
		mean=mean+j;
		}
		float len_list=vc1_app_map[i].size();
		mean=(float)mean/len_list;
		for(float j:vc1_app_map[i])
        standardDeviation += pow(j-mean, 2);
    	standardDeviation=sqrt(standardDeviation /len_list);
    	vc1_app_file<<i<<"  "<<mean<<"  "<<standardDeviation<<"\n";
	}
	vc1_app_file.close();

	std::ofstream vc2_app_file;
  	vc2_app_file.open ("vc2_app_file.dat");
  	vc2_app_file<<"# X Y Yerr\n";
	for (auto const& key :vc2_app_map) {
		int i=key.first;
		float mean=0;
		float standardDeviation=0;
		for(float j:vc2_app_map[i]){
		mean=mean+j;
		}
		float len_list=vc2_app_map[i].size();
		mean=(float)mean/len_list;
		for(float j:vc2_app_map[i])
        standardDeviation += pow(j-mean, 2);
    	standardDeviation=sqrt(standardDeviation /len_list);
    	vc2_app_file<<i<<"  "<<mean<<"  "<<standardDeviation<<"\n";
	}
	vc2_app_file.close();
	system("gnuplot graph_time_disp.gp");
	system("gnuplot graph_app_disp.gp");
	system("gnuplot graph_time_ex.gp");
    return 0;	
}


