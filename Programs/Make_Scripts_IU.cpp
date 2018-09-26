// Updated on 09/23/18

/*
	Program Make_Scripts_IU.cpp to make scripts for
	preparing mpileup files from FASTQ files in
	the IU computing system.   

	Inputs:
	1. List of individual IDs.
	2. List of FASTQ files.

	Output:
	Scripts for the individuals. 

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
	const char* jv = {"java/jdk/1.8.0_74"};
	const char* list_id = {""};
	const char* list_fq = {""};
	const char* work_dir = {""};
	const char* soft_dir = {"/N/dc2/projects/daphpops/Software_BWA_Protocol"};
	const char* ref_dir = {"/N/dc2/projects/daphpops/PA42.4.0_reference"};
	const char* ref_name = {"PA42.4.0"};
	const char* fq_dir = {""};
	int cores = 16;
	int print_help = 0;

	int argz = 1;	// argument counter

	// Read the specified setting
	while( (argz<argc) && (argv[argz][0] == '-') ) {
		if (strcmp(argv[argz], "-h") == 0) {
			print_help = 1;
		} else if (strcmp(argv[argz], "-jv") == 0) {
                        jv = argv[++argz];
		} else if (strcmp(argv[argz], "-lf") == 0) {
                        list_fq = argv[++argz];
		} else if (strcmp(argv[argz], "-li") == 0) {
                        list_id = argv[++argz];
		} else if (strcmp(argv[argz], "-wd") == 0) {
			work_dir = argv[++argz];
		} else if (strcmp(argv[argz], "-sd") == 0) {
			soft_dir = argv[++argz];
		} else if (strcmp(argv[argz], "-rd") == 0) {
                        ref_dir = argv[++argz];
		} else if (strcmp(argv[argz], "-rn") == 0) {
                        ref_name = argv[++argz];
		} else if (strcmp(argv[argz], "-fd") == 0) {
			fq_dir = argv[++argz];
		} else if (strcmp(argv[argz], "-c") == 0) {
			sscanf(argv[++argz], "%d", &cores);
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
		fprintf(stderr, "       -jv <s>: specify the java version\n");
		fprintf(stderr, "       -lf <s>: specify the name of the list file of FASTQ files\n");
		fprintf(stderr, "       -li <s>: specify the name of the list file of individual IDs\n");	
		fprintf(stderr, "       -sd <s>: specify the full path to the software directory\n");
		fprintf(stderr, "       -rd <s>: specify the full path to the reference directory\n");
		fprintf(stderr, "       -rn <s>: specify the name of the reference sequence\n");
		fprintf(stderr, "       -fd <s>: specify the full path to the directory containing FASTQ files\n");
		fprintf(stderr, "       -c <d>: specify the cores\n");
		exit(1);
	}

	if ( (list_fq != NULL) && (list_fq[0] == '\0') ) { // List file of FASTQ files not specified.
                fprintf(stderr, "Please specify the list file of FASTQ files.\n");
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
	if ( (fq_dir != NULL) && (fq_dir[0] == '\0') ) { // FASTQ directory not specified.
                fprintf(stderr, "Please specify the FASTQ directory.\n");
                exit(1);
        }
	
	printf("FASTQ file: %s\n", list_fq);
	printf("ID file: %s\n", list_id);
	printf("Working directory: %s\n", work_dir);
	printf("Software directory: %s\n", soft_dir);
	printf("Reference directory: %s\n", ref_dir);
	printf("FASTQ directory: %s\n", fq_dir);
		 
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

	ifstream f_inputFile(list_fq);               // Try to open the list of FASTQ file names
        if ( !f_inputFile.is_open() ) {           // Exit on failure
                fprintf(stderr, "Cannot open %s for reading.\n", list_fq);
                exit(1);
        }

	// Read the FASTQ file names into vectors
        string f_line; // String buffer
        vector <string> fq_R1, fq_R2;
        string t_fq;
        int num_inds;   // Number of FASTQ files

        // Clear the vector
        fq_R1.clear();
	fq_R2.clear();

        while (getline(f_inputFile, f_line)) {
                istringstream ss_R1(f_line);
                ss_R1 >> t_fq;
                fq_R1.push_back(t_fq);
		getline(f_inputFile, f_line);
		istringstream ss_R2(f_line);
		ss_R2 >> t_fq;
                fq_R2.push_back(t_fq);
        }

        num_inds = fq_R1.size();
        printf("%d individuals\n", num_inds);

	if (num_ids == num_inds) {
		printf("Making scripts for %d individuals\n", num_ids);
	} else {
		fprintf(stderr, "The number of individuals differs from the number of IDs.\n");
		exit(1);
	}

	FILE *outstream;	// pointer to the output file
	string out_file_name;	// output file name

	int ig;		// individual counter
	for(ig=0; ig<num_ids; ig++) {
		out_file_name = id.at(ig);
		out_file_name.append("_Proc.sh");
		// printf("Making %s\n", out_file_name.c_str());
		// Open the output file
		outstream = fopen(out_file_name.c_str(), "w");
		if (outstream == NULL ) {	// Exit on failure
			fprintf(stderr, "Cannot open %s for writing.\n", out_file_name.c_str());
			exit(1);
		}
		fprintf(outstream, "# Go to the working directory\n");
		printf("# Go to the working directory\n");
		fprintf(outstream, "cd %s\n", work_dir);
		printf("cd %s\n", work_dir);
		fprintf(outstream, "\n");
		printf("\n");
		fprintf(outstream, "# Trim adapter sequences from sequence reads.\n");
		printf("# Trim adapter sequences from sequence reads.\n");
		fprintf(outstream, "module load %s\n", jv);
		printf("module load %s\n", jv);
		fprintf(outstream, "java -jar %s/Trimmomatic-0.38/trimmomatic-0.38.jar PE %s/%s %s/%s %s_R1-paired.fastq %s_R1-unpaired.fastq %s_R2-paired.fastq %s_R2-unpaired.fastq HEADCROP:3 ILLUMINACLIP:%s_Adapters.fa:2:30:10:2 SLIDINGWINDOW:4:15 MINLEN:30\n", soft_dir, fq_dir, fq_R1.at(ig).c_str(), fq_dir, fq_R2.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str());
		printf("java -jar %s/Trimmomatic-0.38/trimmomatic-0.38.jar PE %s/%s %s/%s %s_R1-paired.fastq %s_R1-unpaired.fastq %s_R2-paired.fastq %s_R2-unpaired.fastq HEADCROP:3 ILLUMINACLIP:%s_Adapters.fa:2:30:10:2 SLIDINGWINDOW:4:15 MINLEN:30\n", soft_dir, fq_dir, fq_R1.at(ig).c_str(), fq_dir, fq_R2.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str());
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Map reads to the reference sequence.\n");
		printf("# Map reads to the reference sequence.\n");
		fprintf(outstream, "%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R1-paired.fastq %s_R2-paired.fastq > %s_%s-paired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
		printf("%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R1-paired.fastq %s_R2-paired.fastq > %s_%s-paired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
		fprintf(outstream, "%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R1-unpaired.fastq > %s_%s_R1-unpaired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
		printf("%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R1-unpaired.fastq > %s_%s_R1-unpaired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
		fprintf(outstream, "%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R2-unpaired.fastq > %s_%s_R2-unpaired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
                printf("%s/bwa-0.7.17/bwa mem -t %d -M %s/%s.fasta %s_R2-unpaired.fastq > %s_%s_R2-unpaired.sam &\n", soft_dir, cores, ref_dir, ref_name, id.at(ig).c_str(), id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
		printf("\n");
		fprintf(outstream, "wait\n");
                printf("wait\n");
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Combine the SAM files using Picard.\n");
		printf("# Combine the SAM files using Picard.\n");
		fprintf(outstream, "java -jar %s/picard.jar MergeSamFiles I=%s_%s-paired.sam I=%s_%s_R1-unpaired.sam I=%s_%s_R2-unpaired.sam O=%s_%s.sam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/picard.jar MergeSamFiles I=%s_%s-paired.sam I=%s_%s_R1-unpaired.sam I=%s_%s_R2-unpaired.sam O=%s_%s.sam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Convert the SAM file to the BAM file using Samtools.\n");
		printf("# Convert the SAM file to the BAM file using Samtools.\n");
		fprintf(outstream, "%s/samtools-1.9/samtools view -bS %s_%s.sam > %s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("%s/samtools-1.9/samtools view -bS %s_%s.sam > %s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Sort the BAM file using Picard.\n");
		printf("# Sort the BAM file using Picard.\n");
		fprintf(outstream, "java -jar %s/picard.jar SortSam INPUT=%s_%s.bam OUTPUT=Sorted_%s_%s.bam SORT_ORDER=coordinate\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/picard.jar SortSam INPUT=%s_%s.bam OUTPUT=Sorted_%s_%s.bam SORT_ORDER=coordinate\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Filter the BAM file using Samtools.\n");
		printf("# Filter the BAM file using Samtools.\n");
		fprintf(outstream, "%s/samtools-1.9/samtools view -q 20 -f 3 -F 3844 -b Sorted_%s_%s.bam > Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("%s/samtools-1.9/samtools view -q 20 -f 3 -F 3844 -b Sorted_%s_%s.bam > Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Add read groups to the sorted BAM file using Picard.\n");
		printf("# Add read groups to the sorted BAM file using Picard.\n");
		fprintf(outstream, "java -jar %s/picard.jar AddOrReplaceReadGroups INPUT=Filtered_Sorted_%s_%s.bam OUTPUT=RG_Filtered_Sorted_%s_%s.bam RGID=Daphnia RGLB=bar RGPL=illumina RGSM=%s RGPU=6\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str());
		printf("java -jar %s/picard.jar AddOrReplaceReadGroups INPUT=Filtered_Sorted_%s_%s.bam OUTPUT=RG_Filtered_Sorted_%s_%s.bam RGID=Daphnia RGLB=bar RGPL=illumina RGSM=%s RGPU=6\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str());
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Mark duplicate reads using Picard.\n");
		printf("# Mark duplicate reads using Picard.\n");
		fprintf(outstream, "java -jar %s/picard.jar MarkDuplicates INPUT=RG_Filtered_Sorted_%s_%s.bam OUTPUT=dedup_RG_Filtered_Sorted_%s_%s.bam METRICS_FILE=%s_%s_metrics.txt\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/picard.jar MarkDuplicates INPUT=RG_Filtered_Sorted_%s_%s.bam OUTPUT=dedup_RG_Filtered_Sorted_%s_%s.bam METRICS_FILE=%s_%s_metrics.txt\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Index the BAM file using Picard.\n");
		printf("# Index the BAM file using Picard.\n");
		fprintf(outstream, "java -jar %s/picard.jar BuildBamIndex INPUT=dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/picard.jar BuildBamIndex INPUT=dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Define intervals to target for the local realignment using GATK.\n");
		printf("# Define intervals to target for the local realignment using GATK.\n");
		fprintf(outstream, "java -jar %s/GenomeAnalysisTK.jar -T RealignerTargetCreator -R %s/%s.fasta -I dedup_RG_Filtered_Sorted_%s_%s.bam -o %s_%s.intervals\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/GenomeAnalysisTK.jar -T RealignerTargetCreator -R %s/%s.fasta -I dedup_RG_Filtered_Sorted_%s_%s.bam -o %s_%s.intervals\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Locally realign reads around indels using GATK.\n");
		printf("# Locally realign reads around indels using GATK.\n");
		fprintf(outstream, "java -jar %s/GenomeAnalysisTK.jar -T IndelRealigner -R %s/%s.fasta -I dedup_RG_Filtered_Sorted_%s_%s.bam -targetIntervals %s_%s.intervals -o realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("java -jar %s/GenomeAnalysisTK.jar -T IndelRealigner -R %s/%s.fasta -I dedup_RG_Filtered_Sorted_%s_%s.bam -targetIntervals %s_%s.intervals -o realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Clip overlapping read pairs using bamUtil.\n");
		printf("# Clip overlapping read pairs using bamUtil.\n");
		fprintf(outstream, "%s/bamUtil-1.0.14/bin/bam clipOverlap --in realigned_dedup_RG_Filtered_Sorted_%s_%s.bam --out Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("%s/bamUtil-1.0.14/bin/bam clipOverlap --in realigned_dedup_RG_Filtered_Sorted_%s_%s.bam --out Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Index the clipped BAM file using Samtools.\n");
		printf("# Index the clipped BAM file using Samtools.\n");
		fprintf(outstream, "%s/samtools-1.9/samtools index Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name);
		printf("%s/samtools-1.9/samtools index Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam\n", soft_dir, id.at(ig).c_str(), ref_name);
		fprintf(outstream, "\n");
                printf("\n");
		fprintf(outstream, "# Make the mpileup file from the BAM file using Samtools.\n");
		printf("# Make the mpileup file from the BAM file using Samtools.\n");
		fprintf(outstream, "%s/samtools-1.9/samtools mpileup -f %s/%s.fasta Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam > %s_%s.mpileup\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);
		printf("%s/samtools-1.9/samtools mpileup -f %s/%s.fasta Clipped_realigned_dedup_RG_Filtered_Sorted_%s_%s.bam > %s_%s.mpileup\n", soft_dir, ref_dir, ref_name, id.at(ig).c_str(), ref_name, id.at(ig).c_str(), ref_name);         		      
		// Close the output file
		fclose(outstream);
	}			 
			
	// Close the input files
	f_inputFile.close();
	i_inputFile.close();
	
	return(0);
}
