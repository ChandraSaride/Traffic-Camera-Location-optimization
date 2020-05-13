#include<vector>

#include <unistd.h>
#include <signal.h>
#include<iostream>
#include <sys/wait.h>
/// Entry point of child `
int random_generator(int count_args,char** rnd_input) {
    // Process random writing to python
	 // Process A writing to C
/*	std::cout<<"count"<<count_args<<std::endl;
	for (int i = 0; i < count_args; ++i) 
        std::cout << rnd_input[i] <<std::endl; */
    for (int i = 1; i < count_args;i++){ 
        std::string tmp=std::string(rnd_input[i]);
        if(tmp.compare("-l")==0){
            if(rnd_input[i+1]==nullptr){
                std::cerr << "Error: Wrong Specification of l  \n";
                exit(1);
            }
            int wait_time=atoi(rnd_input[i+1]);
            if(wait_time>=5){
                continue;
            }
            else{
                std::cerr << "Error: Wrong Specification of l  \n";
                exit(1);
            }
        }
        if(tmp.compare("-s")==0){
                if(rnd_input[i+1]==nullptr){
                    std::cerr << "Error: Wrong Specification of s  \n";
                    exit(1);
                }
                int no_streets=atoi(rnd_input[i+1]);
                if(no_streets>=2){
                    continue;
                }
                else{
                    std::cerr << "Error: Wrong Specification of s \n";
                    exit(1);
                }
        }
        if(tmp.compare("-n")==0){
            if(rnd_input[i+1]==nullptr){
                std::cerr << "Error: Wrong Specification of n  \n";
                exit(1);
            }
            int no_segments=atoi(rnd_input[i+1]);
            if(no_segments>=1){
                continue;
            }
            else{
                std::cerr << "Error: Wrong Specification of n \n";
                exit(1);
            }
        }
        if(tmp.compare("-c")==0){
            if(rnd_input[i+1]==nullptr){
                std::cerr << "Error: Wrong Specification of c  \n";
                exit(1);
            }
            int coord=atoi(rnd_input[i+1]);
            if(coord>=1){
                continue;
            }
            else{
                std::cerr << "Error: Wrong Specification of c\n";
                exit(1);
            }
        }
    }	
	char* argv[10];
	argv[0] = (char*)"./rgen";
    argv[1] = (char*)rnd_input[1];
    argv[2] = (char*)rnd_input[2];
    argv[3] = (char*)rnd_input[3];
    argv[4] = (char*)rnd_input[4];
    argv[5] = (char*)rnd_input[5];
    argv[6] = (char*)rnd_input[6];
    argv[7] = (char*)rnd_input[7];
    argv[8] = (char*)rnd_input[8];
    argv[9] = nullptr;
    execv (argv[0], argv);
    perror ("Error: random_generator failed");
    return 1;
}

/// Entry point of parent
int keyboard(void) {
    // cpp taking from python
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line.size () > 0)
            std::cout <<line << std::endl;
    }
    return 0;
}

/// Entry point of parent
int cpp(void) {
    // cpp taking from python
    char* argv[2];
        argv[0]=(char*)"./ece650-a2";
    	argv[1] = nullptr;
  		execv ("./ece650-a2", argv);
  	perror ("Error: cpp failed");
    return 0;
}


/// Entry point of child 2
int python(void) {
    // Process python writing to cpp
      char* argv[2];
        argv[0]=(char*)"ece650-a1.py";
    	argv[1] = nullptr;
  		execv ("ece650-a1.py", argv);
  	perror ("Error: python failed");
    return 1;
}

int main(int count_args,char** rnd_input)
{
    std::vector<pid_t> kids;
    // create a pipe
    int randtopy[2];
    pipe(randtopy);
    int pytocpp[2];
    pipe(pytocpp);


    pid_t child_pid;
    child_pid = fork ();
    if (child_pid == 0)
    {
        // redirect stdout to the pipe
        dup2(randtopy[1], STDOUT_FILENO);
        close(randtopy[0]);
        close(randtopy[1]);     // Close this too!

        // start process A
        return random_generator(count_args,rnd_input);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect stdin from the pipe
        dup2(randtopy[0], STDIN_FILENO);
        close(randtopy[1]);
        close(randtopy[0]);
        dup2(pytocpp[1], STDOUT_FILENO);
        close(pytocpp[1]);
        close(pytocpp[0]);
        // start process C
        return python();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect stdin from the pipe
    // redirect stdout to the pipe
    dup2(pytocpp[0], STDIN_FILENO);
    close(pytocpp[1]);
    close(pytocpp[0]);
 
    // start process B
    return cpp();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    kids.push_back(child_pid);

    child_pid=0;
   

    // redirect stdout to the pipe
    dup2(pytocpp[1], STDOUT_FILENO);
    close(pytocpp[1]);
    close(pytocpp[0]);
 
    // start process B
    int res =  keyboard();

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    // exit with return code of process B
    return res;

}