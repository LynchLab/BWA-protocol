// Updated on 09/22/18

/*
	Program Make_SubmitJobs_Bridges.cpp to submit jobs for
	preparing mpileup files from FASTQ files in the IU 
	computing system.   

	Input:
	List of individual IDs.

	Output:
	A script for submitting jobs. 

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
	const char* out_file_name = {"SubmitJobs.sh"};
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
		} else if (strcmp(argv[argz], "-out") == 0) {
			out_file_name = argv[++argz];
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
		fprintf(stderr, "       -out <s>: specify the name of the output file\n");	
		exit(1);
	}

	if ( (list_id != NULL) && (list_id[0] == '\0') ) { // List file of individual IDs not specified.
                fprintf(stderr, "Please specify the list file of individual IDs.\n");
                exit(1);
        }
	if ( (work_dir != NULL) && (work_dir[0] == '\0') ) { // Working directory not specified.
		fprintf(stderr, "Please specify the working directory.\n");
		exit(1);
	}
	
	printf("ID file: %s\n", list_id);
	printf("Working directory: %s\n", work_dir);
		 
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

	outstream = fopen(out_file_name, "w");
        if (outstream == NULL ) {       // Exit on failure
        	fprintf(stderr, "Cannot open %s for writing.\n", out_file_name);
                exit(1);
        }

	fprintf(outstream, "# Go to the working directory\n");
	printf("# Go to the working directory\n");
	fprintf(outstream, "cd %s\n", work_dir);
	printf("cd %s\n", work_dir);
	fprintf(outstream, "\n");
	printf("\n");

	int ig;		// individual counter
	for(ig=0; ig<num_ids; ig++) {
		fprintf(outstream, "qsub Job_%s_Proc.sh\n", id.at(ig).c_str());
		printf("qsub Job_%s_Proc.sh\n", id.at(ig).c_str());
	}			 
			
	// Close the input and output files
	i_inputFile.close();
	fclose(outstream);
	
	return(0);
}
