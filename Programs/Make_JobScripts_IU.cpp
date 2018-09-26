// Updated on 09/21/18

/*
	Program Make_JobScripts_IU.cpp to make job scripts for
	preparing mpileup files from FASTQ files in the IU 
	computing system.   

	Input:
	List of individual IDs.

	Output:
	Job scripts for the individuals. 

*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
	// Default values of the options
	const char* list_id = {""};
	const char* work_dir = {""};
	const char* jv = {"java/jdk/1.8.0_74"};
	int cores = 16;
	const char* wall_time = {"48:00:00"};
	const char* email = {""};
	int print_help = 0;

	int argz = 1;	// argument counter

	// Read the specified setting
	while( (argz<argc) && (argv[argz][0] == '-') ) {
		if (strcmp(argv[argz], "-h") == 0) {
			print_help = 1;
		} else if (strcmp(argv[argz], "-li") == 0) {
                        list_id = argv[++argz];
		} else if (strcmp(argv[argz], "-wd") == 0) {
			work_dir = argv[++argz];
		} else if (strcmp(argv[argz], "-jv") == 0) {
                        jv = argv[++argz];
		} else if (strcmp(argv[argz], "-em") == 0) {
			email = argv[++argz];
		} else if (strcmp(argv[argz], "-c") == 0) {
			sscanf(argv[++argz], "%d", &cores);
		} else if (strcmp(argv[argz], "-t") == 0) {
                        wall_time = argv[++argz];
		} else {
			fprintf(stderr, "unknown option %s\n", argv[argz]);
			print_help = 1;
			break;
		}
		argz++;
	}
	if (print_help) {	// print error/usage message ?
		fprintf(stderr, "USAGE: %s {<options>}\n", argv[0]);
		fprintf(stderr, "	options: -h: print the usage message\n");
		fprintf(stderr, "       -wd <s>: specify the full path to the working directory\n");
		fprintf(stderr, "       -li <s>: specify the name of the list file of individual IDs\n");
		fprintf(stderr, "       -jv <s>: specify the java version\n");
		fprintf(stderr, "       -em <s>: specify the email address\n");	
		fprintf(stderr, "       -c <d>: specify the cores\n");
		fprintf(stderr, "       -t <s>: specify the wall time (HH:MM:SS)\n");
		exit(1);
	}

	if ( (list_id != NULL) && (list_id[0] == '\0') ) { // List file not specified.
                fprintf(stderr, "Please specify the list file of individual IDs.\n");
                exit(1);
        }

	if ( (work_dir != NULL) && (work_dir[0] == '\0') ) { // Working directory not specified.
		fprintf(stderr, "Please specify the working directory.\n");
		exit(1);
	}
	
	if ( (email != NULL) && (email[0] == '\0') ) { // email address not specified.
		fprintf(stderr, "Please specify your email address.\n");
		exit(1);
	}
	
	printf("ID file: %s\n", list_id);
	printf("Working directory: %s\n", work_dir);
	printf("Cores: %d\n", cores);
	printf("Wall time: %s\n", wall_time);
		 
	ifstream i_inputFile(list_id);               // Try to open the list of individual IDs
        if ( !i_inputFile.is_open() ) {           // Exit on failure
                fprintf(stderr, "Cannot open %s for reading.\n", list_id);
                exit(1);
        }

	// Read the individual IDs into a vector
	string i_line; // String buffer
	vector <string> id;
	string t_id;
	int num_ids;	// Number of IDs

	// Clear the vector
	id.clear();

	while (getline(i_inputFile, i_line)) {
		istringstream ss(i_line);
		ss >> t_id;
		id.push_back(t_id);
	}
	
	num_ids = id.size();
	printf("%d IDs\n", num_ids);

	FILE *outstream;	// pointer to the output file
	string out_file_name;	// output file name

	int ig;		// individual counter
	for(ig=0; ig<num_ids; ig++) {
		out_file_name = id.at(ig);
		out_file_name = "Job_" + out_file_name;
		out_file_name.append("_Proc.sh");
		// printf("Making %s\n", out_file_name.c_str());
		// Open the output file
		outstream = fopen(out_file_name.c_str(), "w");
		if (outstream == NULL ) {	// Exit on failure
			fprintf(stderr, "Cannot open %s for writing.\n", out_file_name.c_str());
			exit(1);
		}
		fprintf(outstream, "#!/bin/bash\n");
		printf("#!/bin/bash\n");
		fprintf(outstream, "#PBS -k o\n");
		printf("#PBS -k o\n");
		fprintf(outstream, "#PBS -l nodes=1:ppn=%d,walltime=%s\n", cores, wall_time);
		printf("#PBS -l nodes=1:ppn=%d,walltime=%s\n", cores, wall_time);
		fprintf(outstream, "#PBS -M %s\n", email);
		printf("#PBS -M %s\n", email);;
		fprintf(outstream, "#PBS -m abe\n");
		printf("#PBS -m abe\n");
		fprintf(outstream, "#PBS -N %s_Proc\n", id.at(ig).c_str());
		printf("#PBS -N %s_Proc\n", id.at(ig).c_str());
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Go to the working directory\n");
		printf("# Go to the working directory\n");
		fprintf(outstream, "cd %s\n", work_dir);
		printf("cd %s\n", work_dir);
		fprintf(outstream, "\n");
		printf("\n");
		fprintf(outstream, "module load %s\n", jv);
		printf("module load %s\n", jv);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "time ./%s_Proc.sh\n", id.at(ig).c_str());
		printf("time ./%s_Proc.sh\n", id.at(ig).c_str());         		      
		// Close the output file
		fclose(outstream);
	}			 
			
	// Close the input file
	i_inputFile.close();
	
	return(0);
}
