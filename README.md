# BWA-protocol
Protocol for making mpileup files from FASTQ files using BWA

Septermber 26, 2018

Author: Takahiro Maruki

**This package can be downloaded by clicking "Clone or download" on this web site.**

**Instructions**

0. Compile the programs by typing the following commands: <br />
chmod u+x B_Adapters.pl <br />
chmod u+x N_Adapters.pl <br />
g++ -o Make_Scripts_IU Make_Scripts_IU.cpp -lm <br />
g++ -o Make_JobScripts_IU Make_JobScripts_IU.cpp -lm <br />
g++ -o Make_SubmitJobs_IU Make_SubmitJobs_IU.cpp -lm <br />
- These programs are found and compiled in the following directory: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/ <br />

1. Make a working directory for the data. <br />
For example, type the following command to make a directory called KAP_DataPrep: <br />
mkdir KAP_DataPrep <br />

2. Prepare a text file of barcode sequences. <br />
- Example file in KAP using the Bioo kit (KAP_barcodes.txt) is found in the following directory <br />
/N/dc2/projects/daphpops/Population_samples/KAP2013 <br />
- Example file in SH using the Nextera kit (SH_barcodes.txt) is found in the following directory: <br />
/N/dc2/projects/daphpops/Population_samples/SH <br />

3. Prepare isolate-specific FASTA files of adapter sequences in the working directory. <br />
a. For example, to make isolate-specific FASTA files of adapter sequences in KAP, where the Bioo kit was used in the library preparation, type the following command: <br />
perl /N/dc2/projects/daphpops/Software_BWA_Protocol/B_Adapters.pl /N/dc2/projects/daphpops/Population_samples/KAP2013/KAP_barcodes.txt <br />
b. For example, to make isolate-specific FASTA files of adapter sequences in SH, where the Nextera kit was used in the library preparation, type the following command: <br />
perl /N/dc2/projects/daphpops/Software_BWA_Protocol/N_Adapters.pl /N/dc2/projects/daphpops/Population_samples/SH/SH_barcodes.txt <br />

4. Make text files of isolate IDs and FASTQ files in the working directory. <br />
For example, the text file of isolate IDs in KAP can be made by typing the following command: <br />
ls \*.fa | cut -c-9 > IDs_KAP.txt <br />
For example, the text file of FASTQ files in KAP can be made by typing the following command: <br />
ls /N/dc2/projects/daphpops/Population_samples/KAP2013/\*.fastq > FASTQs_KAP.txt <br />
- The order of the isolate IDs need to be consistent in the two files. <br />

5. Make scripts for preparing mpileup files for each of the individuals using Make_Scripts_IU. <br />
For example, type the following command to make the scripts in KAP. <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/Make_Scripts_IU -wd /N/dc2/scratch/xxxxxxx/KAP_DataPrep -lf FASTQs_KAP.txt -li IDs_KAP.txt -sd /N/dc2/projects/daphpops/Software_BWA_Protocol -rd /N/dc2/projects/daphpops/PA42.4.1_reference -rn PA42.4.1 -fd /N/dc2/projects/daphpops/Population_samples/KAP2013 <br />
- Substitue your user name for xxxxxxx. <br />
- Type the following command to find the available options: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/Make_Scripts_IU -h

6. Make the scripts executable. <br />
For example, type the following command to make the scripts executable in KAP: <br />
chmod u+x KAP-\*_Proc.sh <br />

7. Make corresponding job scripts using Make_JobScripts_IU. <br />
For example, type the following command to make the job scripts in KAP: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol//Make_JobScripts_IU -wd /N/dc2/scratch/xxxxxxx/KAP_DataPrep -li IDs_KAP.txt -em xxxxxxx@indiana.edu -t 48:00:00 <br />
- Type the following command to find the available options: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/Make_JobScripts_IU -h <br />

8. Make a script for submitting the jobs using Make_SubmitJobs_IU. <br />
For example, type the following command to make the script in KAP: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol//Make_SubmitJobs_IU -wd /N/dc2/scratch/xxxxxxx/KAP_DataPrep -li IDs_KAP.txt -out KAP_SubmitJobs.sh <br />
- Type the following command to find the available options: <br />
/N/dc2/projects/daphpops/Software_BWA_Protocol/ Make_SubmitJobs_IU -h <br />

9. Run the script for submitting the jobs. <br />
For example, type the following commands to submit the KAP jobs. <br />
chmod u+x KAP_SubmitJobs.sh <br />
./KAP_SubmitJobs.sh <br />
- A few jobs may sometimes be skipped in this way.  Check whether all the jobs have been submitted by typing the following command: <br />
qstat -au xxxxxxx <br />

