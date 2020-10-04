#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <sys/wait.h>

using namespace std;

int procRgen(int argc, char** argv) {

	// Process A writing to C
	argv[0] = (char *)"rgen";
	execv("rgen", argv); //REF https://github.com/eceuwaterloo/ece650-cpp/blob/master/using_exec.cpp

	return 0;
}


/// Entry point of process B

int procA1(int argc, char** argv) {

	// Process B writing to C

	//char* argv[3];
	argv[0] = (char *)"python";
	argv[1] = (char *)"ece650-a1.py";
	argv[2] = nullptr;

	execvp("python", argv);

	return 0;

}

int procA2(int argc, char** argv) {

	//char* argv[2];
	argv[0] = (char*)"ece650-a2";
	argv[1] = nullptr;

	execv("ece650-a2", argv);

	return 0;

}


int procInput(void) {


	while (!std::cin.eof()) {

		// read a line of input until EOL and store in a string

		std::string line;
		std::getline(std::cin, line);

		if (line.size() > 0) {

			std::cout << line << std::endl;
		}
	}
	return 0;
}


int main (int argc, char** argv) {// REF https://github.com/eceuwaterloo/ece650-cpp/blob/master/using_pipe.cpp

	std::vector<pid_t> kids;

	// create a pipe

	int pipe1[2];//pipe1 : rgen to a1
	pipe(pipe1);

	int pipe2[2];//pipe2: a1 to a2
	pipe(pipe2);

	pid_t child_pid;

	child_pid = fork();
	// rgen to a1
	if (child_pid == 0)

	{
		// redirect stdout to the pipe
		dup2(pipe1[1], STDOUT_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);     // Close this too!
		// start process A
		
		return procRgen(argc,argv);

	}

	kids.push_back(child_pid);
	
	child_pid = fork();


	if (child_pid == 0) {
		dup2(pipe1[0], STDIN_FILENO);// redirect stdin with pipe1
		close(pipe1[0]);
		close(pipe1[1]);

		dup2(pipe2[1], STDOUT_FILENO);// redirect stdout to pipe2
		close(pipe2[0]);
		close(pipe2[1]);
		return procA1(argc,argv);

	}

	kids.push_back(child_pid);
	child_pid = fork();

	if (child_pid == 0)

	{
		dup2(pipe2[0], STDIN_FILENO);// redirect stdout to pipe2
		close(pipe2[1]);
		close(pipe2[0]);

		// start process C

		return procA2(argc, argv);

	}

	kids.push_back(child_pid);

	child_pid = 0;

	// keyboard input to a2

	dup2(pipe2[1], STDOUT_FILENO);
	close(pipe2[0]);
	close(pipe2[1]);
	int output = procInput();

	// send kill signal to all children


    for (pid_t k : kids) {

        int status;

        kill (k, SIGTERM);

        waitpid(k, &status, 0);

    }


	return output;


}
