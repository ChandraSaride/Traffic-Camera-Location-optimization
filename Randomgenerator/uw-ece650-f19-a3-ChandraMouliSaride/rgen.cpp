#include <iostream>
#include <fstream>
#include <map>
#include<set>
#include<list>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
class Point{
public:
    int x,y;
public:
    void set_point(int x,int y){
        this->x=x;
        this->y=y;
    } 
};
class Segment{
public:
    Point src,dst;
public:
    void set_segment(Point src,Point dst){
        this->src=src;
        this->dst=dst;
    } 
};

int random_generator(int lower_limit,int upper_limit){

    std::ifstream urandom("/dev/urandom");
    int rnd_number;

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    // read a random 8-bit value.
    // Have to use read() method for low-level reading
    while(true){

    int num = 42;
    urandom.read((char*)&num, sizeof(int));
    int x=num%(upper_limit+1-lower_limit)+lower_limit;
    if(x>=lower_limit && x<=upper_limit){
        rnd_number=x;
        break;
    }
    }
    // close random stream
    urandom.close();
    return rnd_number;    
}

std::string random_name_generator(){

    std::ifstream urandom("/dev/urandom");


    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return "fail";
    }

    // read a random 8-bit value.
    // Have to use read() method for low-level reading
    char arr[10];
    int i=0;
    while(i<10){

    char ch = 'a';
    urandom.read(&ch, 1);
    // cast to integer to see the numeric value of the character
    if((ch>='a'&& ch<='z') || (ch>='A' && ch<='Z')){
    arr[i]=ch;
    i++;
        }
    }
    // close random stream
    urandom.close();
    std::string street_name(arr);
    return street_name;    
}

std::map<std::string,std::list<Point>> street_generator(int count_args,char ** rnd_spec){
    int loop_limit=25;
    std::map<std::string,std::list<Point>>street_details;
    while(loop_limit>0){
    street_details.clear();
    const int def_no_streets=10;
    const int def_no_segments=5;
    const int def_coord_range=20;
    int final_no_streets=random_generator(2,def_no_streets);
    int final_no_segments=random_generator(1,def_no_segments);
    int final_coord_range=def_coord_range;
        for (int i = 1; i < count_args;i++){ 
            std::string tmp=std::string(rnd_spec[i]);
            if(tmp.compare("-s")==0){
                if(rnd_spec[i+1]==nullptr){
                    std::cerr << "Error: Wrong Specification of s  \n";
                    exit(1);
                }
                int no_streets=atoi(rnd_spec[i+1]);
                if(no_streets>=2){
                    final_no_streets=random_generator(2,no_streets);
                }
                else{
                    std::cerr << "Error: Wrong Specification of s \n";
                    exit(1);
                }
            }
            if(tmp.compare("-n")==0){
                if(rnd_spec[i+1]==nullptr){
                    std::cerr << "Error: Wrong Specification of n  \n";
                    exit(1);
                }
                int no_segments=atoi(rnd_spec[i+1]);
                if(no_segments>=1){
                    final_no_segments=random_generator(1,no_segments);
                }
                else{
                    std::cerr << "Error: Wrong Specification of n \n";
                    exit(1);
                }
            }
            if(tmp.compare("-c")==0){
                if(rnd_spec[i+1]==nullptr){
                    std::cerr << "Error: Wrong Specification of c  \n";
                    exit(1);
                }
                int coord=atoi(rnd_spec[i+1]);
                if(coord>=1){
                    final_coord_range=coord;
                }
                else{
                    std::cerr << "Error: Wrong Specification of c\n";
                    exit(1);
                }
            }
        }
    std::list<Point> points_chk_list;
    int i=0;
    while(i<final_no_streets){
        std::list<Point> points_list;
        int j=0;
       int limit=(final_no_segments+1)*50;
       bool recheck=false;
        while(j<final_no_segments+1){
            bool do_again=false;
           Point p;
            p.set_point(random_generator(final_coord_range*-1,final_coord_range),random_generator(final_coord_range*-1,final_coord_range)); 
            for(Point chk_point:points_chk_list){
                if((chk_point.x==p.x && chk_point.y==p.y)){
                    do_again=true;
                }
            }
            if(!do_again){
            points_list.push_back(p);
            points_chk_list.push_back(p);
            j++;
            }
            else{
                limit--;
                if(limit<=0){
                    loop_limit--;
                    recheck=true;
                    break;
                }
                continue;
            }
        }
        if(!recheck){
        std::string gen_street_name=random_name_generator();
        street_details[gen_street_name]=points_list;
        i++;
        }
        else{
            break;
        }

    }
    if(street_details.size()==unsigned(final_no_streets)){
        break;
    }

    }
    if(loop_limit<=0){
        std::cerr << "Error:  failed to generate valid input for 25 simultaneous attempts\n";
        exit(1);

    }
    else{
        return street_details;
    }
}
int main(int count_args,char ** rnd_spec)
{
    const int def_wait_time=5;
    int final_wait_time=random_generator(5,def_wait_time);
        for (int i = 1; i < count_args;i++){ 
            std::string tmp=std::string(rnd_spec[i]);
            if(tmp.compare("-l")==0){
                if(rnd_spec[i+1]==nullptr){
                    std::cerr << "Error: Wrong Specification of l  \n";
                    exit(1);
                }
                int wait_time=atoi(rnd_spec[i+1]);
                if(wait_time>=5){
                    final_wait_time=random_generator(5,wait_time);
                }
                else{
                    std::cerr << "Error: Wrong Specification of l  \n";
                    exit(1);
                }
            }
        }
    std::map<std::string,std::list<Point>>street_details;
    while(true){
    if(street_details.size()>0){
        sleep(final_wait_time);
        std::list<std::string> remove_street_list;
        for (auto const& element : street_details) {
        remove_street_list.push_back(element.first);
        }
        for(std::string street:remove_street_list){
            std::string str=std::string("r ").append(std::string("\"")).append(street).append(std::string("\""));
            std::cout<<str<<std::endl;
        }
        street_details.clear();
    }
    street_details=street_generator(count_args,rnd_spec);
    std::list<std::string> street_names_list;
    for (auto const& element : street_details) {
        street_names_list.push_back(element.first);
    }
    for(std::string street_name:street_names_list){
        std::list<Point> points_list=street_details[street_name];
        std::string str;
        str=std::string("a ").append(std::string("\"")).append(street_name).append(std::string("\"")).append(std::string(" "));
        for(Point p:points_list){
            str=str.append(std::string("(")).append(std::to_string(p.x)).append(std::string(",")).append(std::to_string(p.y)).append(std::string(")"));
        }
        std::cout<<str<<std::endl;
    }
    std::cout<<"g"<<std::endl;
    }
    return 0; 
}